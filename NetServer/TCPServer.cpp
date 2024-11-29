#include "ErrDisplay.h"
#include "TCPServer.h"
#include "GameManager.h"

TCPServer* TCPServer::m_inst;
CRITICAL_SECTION cs;
HANDLE hRecvEvent, hWorkEvent[2];

DWORD WINAPI RecvThread(LPVOID arg)
{
	cout << "Start RecvThread!\n";
	SOCKET client_sock = (SOCKET)arg;

	Client client(client_sock);
	TCPServer::GetInst()->clients.emplace_back(client);

	int client_id = client.player.id;
	int res = send(client_sock, (char*)&client_id, sizeof(int), 0);
	if (res == SOCKET_ERROR) { err_display("send()"); return -1; }

	while (true)
	{
		WaitForSingleObject(hRecvEvent, INFINITE);

		int type;
		int data_size;
		char data[DATA_SIZE];
		int res;

		res = recv(client.socket, (char*)&type, sizeof(int), MSG_WAITALL);
		if (res <= 0) break;
		res = recv(client.socket, (char*)&data_size, sizeof(int), MSG_WAITALL);
		if (res <= 0) break;
		res = recv(client.socket, data, data_size, MSG_WAITALL);
		if (res <= 0) break;

		Packet p(client_id, type, data_size, data);
		EnterCriticalSection(&cs);
		TCPServer::GetInst()->recvQ.push(p);
		LeaveCriticalSection(&cs);

		SetEvent(hWorkEvent[client_id]);
	}

	cout << "End RecvThread!\n";
	closesocket(client.socket);
	return 0;
}

DWORD WINAPI WorkerThread(LPVOID arg)
{
	while (true)
	{
		WaitForMultipleObjects(2, hWorkEvent, TRUE, INFINITE);
		ResetEvent(hRecvEvent);
		EnterCriticalSection(&cs);
		auto& recvQ = TCPServer::GetInst()->recvQ;
		while (!recvQ.empty())
		{
			Packet packet = recvQ.front();
			recvQ.pop();
			LeaveCriticalSection(&cs);

			switch (packet.type)
			{
			case LoginRequest:
			{
				C_LoginRequestPkt* RecvPacket = (C_LoginRequestPkt*)&packet;
				cout << "[LOGIN] " << RecvPacket->client_id << " Player Name = " << RecvPacket->data << endl;

				auto client = TCPServer::GetInst()->clients[RecvPacket->client_id];
				strncpy_s(client.player.name, RecvPacket->data, NAME_LEN);
				GameManager::GetInst().AddLobbyPlayer(client);
				break;
			}
			case LobbyUpdateRequest:
			{
				C_LobbyUpdateRequest* RecvPacket = (C_LobbyUpdateRequest*)&packet;
				LobbyData* gameData = GameManager::GetInst().GetLobbyData();
				const int i = RecvPacket->client_id;
				memcpy(&gameData->players[i], RecvPacket->data, RecvPacket->data_size);
				S_LobbyInfoPacket SendPacket(*gameData);
				TCPServer::GetInst()->SendPacket(SendPacket);
				break;
			}
			case GameUpdateRequest:
			{
				C_GameUpdateRequest* RecvPacket = (C_GameUpdateRequest*)&packet;
				InGameData* gameData = GameManager::GetInst().GetInGameData();
				const int i = RecvPacket->client_id;
				memcpy(&gameData->players[i], RecvPacket->data, RecvPacket->data_size);
				TCPServer::GetInst()->ServerUpdate(i, gameData);
				S_GameInfoPacket SendPacket(*gameData);
				TCPServer::GetInst()->SendPacket(SendPacket);
				break;
			}
			case TileRequest:
			{
				cout << "[TileRequest] - 전송 받음" << endl;
				TCPServer::GetInst()->CreateTilePacket();
				break;
			}

			default:
				break;
			}

			EnterCriticalSection(&cs);
		}
		LeaveCriticalSection(&cs);
		SetEvent(hRecvEvent);
	}
}

void TCPServer::SendPacket(const Packet& packet)
{
	int res;
	for (const auto& client : clients)
	{
		res = send(client.socket, (char*)&packet.client_id, sizeof(int), 0);
		res = send(client.socket, (char*)&packet.type, sizeof(int), 0);
		res = send(client.socket, (char*)&packet.data_size, sizeof(int), 0);
		res = send(client.socket, packet.data, packet.data_size, 0);
		if (res == SOCKET_ERROR) { err_display("send()"); }
	}
}

bool TCPServer::Init()
{
	cout << "Init()\n";
	hRecvEvent = CreateEvent(NULL, TRUE, TRUE, NULL);	//두번째 인자 True로 설정시 수동
	hWorkEvent[0] = CreateEvent(NULL, FALSE, FALSE, NULL);//자동으로 worker실행 후 다시 false
	hWorkEvent[1] = CreateEvent(NULL, FALSE, FALSE, NULL);

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return false;

	server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sock == INVALID_SOCKET) err_quit("socket()");

	sockaddr_in serveraddr = {};
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);

	if (bind(server_sock, (sockaddr*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR)
		err_quit("bind()");

	if (listen(server_sock, SOMAXCONN) == SOCKET_ERROR)
		err_quit("listen()");

	InitializeCriticalSection(&cs);

	HANDLE hWorkerThread = CreateThread(NULL, 0, WorkerThread, NULL, 0, NULL);
	if (hWorkerThread == NULL) err_display("WorkerThread");

	return true;
}

void TCPServer::Connect()
{
	cout << "Connect()\n";
	while (clients.size() < 2)
	{
		sockaddr_in clientaddr;
		int addrlen = sizeof(clientaddr);
		SOCKET client_sock = accept(server_sock, (sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			err_display("accept()");
			continue;
		}

		HANDLE hRecvThread = CreateThread(NULL, 0, RecvThread, (LPVOID)client_sock, 0, NULL);
		if (hRecvThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hRecvThread); }
	}
}

void TCPServer::Run()
{
	cout << "Run()\n";
	Connect();
}

void TCPServer::Cleanup()
{
	CloseHandle(hRecvEvent);
	CloseHandle(hWorkEvent[0]);
	CloseHandle(hWorkEvent[1]);
	DeleteCriticalSection(&cs);
	closesocket(server_sock);
	WSACleanup();
};

void TCPServer::CreateTilePacket()
{

	// 타일 번호와 위치 정보를 담을 데이터
	std::vector<int> tileNumbers;
	std::vector<vector2> tilePositions;
	TileInfo info;
	vector2 blockSize = vector2(50, 50);

	// 타일 데이터 수집 (CreateMap에서 정의한 타일 정보를 기반으로)
	float tilePosX = -930.f;
	float tilePosY = 475.f;

	// 첫 발판
	tileNumbers.push_back(1);
	tilePositions.push_back({ tilePosX, tilePosY });
	info.pos = vector2(tilePosX, tilePosY);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);
	tilePosX += 50.f;

	for (int i = 0; i < 20; ++i) {
		tileNumbers.push_back(2);
		tilePositions.push_back({ tilePosX, 475.f });
		info.pos = vector2(tilePosX, tilePosY);
		info.box.UpdateCollision(info.pos, blockSize);
		tiles.push_back(info);
		tilePosX += 50.f;
	}

	tileNumbers.push_back(3);
	tilePositions.push_back({ tilePosX, 475.f });
	info.pos = vector2(tilePosX, tilePosY);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);
	tilePosX += 150.f;

	// 상자 계단
	tileNumbers.push_back(17);
	tilePositions.push_back({ -800.f, 425.f });
	tileNumbers.push_back(17);
	tilePositions.push_back({ -750.f, 425.f });
	tileNumbers.push_back(17);
	tilePositions.push_back({ -750.f, 375.f });
	tileNumbers.push_back(17);
	tilePositions.push_back({ -700.f, 425.f });
	tileNumbers.push_back(17);
	tilePositions.push_back({ -700.f, 375.f });
	tileNumbers.push_back(17);
	tilePositions.push_back({ -700.f, 325.f });

	// 상자 계단2
	tileNumbers.push_back(17);
	tilePositions.push_back({ -550.f, 425.f });
	tileNumbers.push_back(17);
	tilePositions.push_back({ -500.f, 425.f });
	tileNumbers.push_back(17);
	tilePositions.push_back({ -500.f, 375.f });
	tileNumbers.push_back(17);
	tilePositions.push_back({ -500.f, 325.f });
	tileNumbers.push_back(17);
	tilePositions.push_back({ -450.f, 425.f });
	tileNumbers.push_back(17);
	tilePositions.push_back({ -450.f, 375.f });
	tileNumbers.push_back(17);
	tilePositions.push_back({ -400.f, 425.f });

	// 중간 발판
	tileNumbers.push_back(14);
	tilePositions.push_back({ -380.f, 250.f });
	info.pos = vector2(-380.f, 250.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);
	tileNumbers.push_back(15);
	tilePositions.push_back({ -330.f, 250.f });
	info.pos = vector2(-330.f, 250.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);
	tileNumbers.push_back(16);
	tilePositions.push_back({ -280.f, 250.f });
	info.pos = vector2(-280.f, 250.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);

	tileNumbers.push_back(14);
	tilePositions.push_back({ -130.f, 200.f });
	info.pos = vector2(-130.f, 200.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);
	tileNumbers.push_back(15);
	tilePositions.push_back({ -80.f, 200.f });
	info.pos = vector2(-80.f, 200.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);
	tileNumbers.push_back(16);
	tilePositions.push_back({ -30.f, 200.f });
	info.pos = vector2(-30.f, 200.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);

	tileNumbers.push_back(14);
	tilePositions.push_back({ 70.f, 170.f });
	info.pos = vector2(70.f, 170.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);
	tileNumbers.push_back(15);
	tilePositions.push_back({ 120.f, 170.f });
	info.pos = vector2(120.f, 170.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);
	tileNumbers.push_back(16);
	tilePositions.push_back({ 170.f, 170.f });
	info.pos = vector2(170.f, 170.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);

	tileNumbers.push_back(14);
	tilePositions.push_back({ -380.f, 100.f });
	info.pos = vector2(-380.f, 100.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);
	tileNumbers.push_back(15);
	tilePositions.push_back({ -330.f, 100.f });
	info.pos = vector2(-330.f, 100.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);
	tileNumbers.push_back(16);
	tilePositions.push_back({ -280.f, 100.f });
	info.pos = vector2(-280.f, 100.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);

	// 근접 몬스터 가두는 상자
	tileNumbers.push_back(17);
	tilePositions.push_back({ -250.f, 425.f });
	info.pos = vector2(-250.f, 425.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);
	tileNumbers.push_back(17);
	tilePositions.push_back({ 50.f, 425.f });
	info.pos = vector2(50.f, 425.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);

	// 두 번째 발판
	tileNumbers.push_back(1);
	tilePositions.push_back({ tilePosX, 475.f });
	info.pos = vector2(tilePosX, 475.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);

	tilePosX += 50.f;

	for (int i = 0; i < 6; ++i) {
		tileNumbers.push_back(2);
		tilePositions.push_back({ tilePosX, 475.f });
		info.pos = vector2(tilePosX, 475.f);
		info.box.UpdateCollision(info.pos, blockSize);
		tiles.push_back(info);
		tilePosX += 50.f;
	}

	tileNumbers.push_back(3);
	tilePositions.push_back({ tilePosX, 475.f });
	info.pos = vector2(tilePosX, 475.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);
	tilePosX += 250.f;

	// 점프맵 발판
	tileNumbers.push_back(14);
	tilePositions.push_back({ tilePosX, 400.f });
	info.pos = vector2(tilePosX, 400.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);
	tileNumbers.push_back(14);
	tilePositions.push_back({ tilePosX, 200.f });
	info.pos = vector2(tilePosX, 200.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);
	tilePosX += 50.f;

	tileNumbers.push_back(15);
	tilePositions.push_back({ tilePosX, 400.f });
	info.pos = vector2(tilePosX, 400.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);
	tileNumbers.push_back(15);
	tilePositions.push_back({ tilePosX, 200.f });
	info.pos = vector2(tilePosX, 200.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);
	tilePosX += 50.f;

	tileNumbers.push_back(16);
	tilePositions.push_back({ tilePosX, 400.f });
	tileNumbers.push_back(16);
	tilePositions.push_back({ tilePosX, 200.f });

	tilePosX -= 400.f;

	tileNumbers.push_back(14);
	tilePositions.push_back({ tilePosX, 300.f });
	tileNumbers.push_back(14);
	tilePositions.push_back({ tilePosX, 100.f });
	tilePosX += 50.f;

	tileNumbers.push_back(15);
	tilePositions.push_back({ tilePosX, 300.f });
	tileNumbers.push_back(15);
	tilePositions.push_back({ tilePosX, 100.f });
	tilePosX += 50.f;

	tileNumbers.push_back(16);
	tilePositions.push_back({ tilePosX, 300.f });
	tileNumbers.push_back(16);
	tilePositions.push_back({ tilePosX, 100.f });

	tilePosX -= 400.f;

	tileNumbers.push_back(14);
	tilePositions.push_back({ tilePosX, 0.f });
	tilePosX += 50.f;

	tileNumbers.push_back(15);
	tilePositions.push_back({ tilePosX, 0.f });
	tilePosX += 50.f;

	tileNumbers.push_back(16);
	tilePositions.push_back({ tilePosX, 0.f });

	tilePosX += 200.f;

	// 포탈 발판
	tileNumbers.push_back(14);
	tilePositions.push_back({ tilePosX, -100.f });
	tilePosX += 50.f;

	for (int i = 0; i < 4; ++i) {
		tileNumbers.push_back(15);
		tilePositions.push_back({ tilePosX, -100.f });
		tilePosX += 50.f;
	}

	tileNumbers.push_back(16);
	tilePositions.push_back({ tilePosX, -100.f });

	S_TilesPkt packet((int)tileNumbers.size(), tileNumbers, tilePositions);
	TCPServer::GetInst()->SendPacket(packet);

}

bool TCPServer::CollisionCheck(int clientID, InGameData* inGameData)
{
	Collision box;
	vector2 size = vector2(50, 60);
	box.UpdateCollision(inGameData->players[clientID].pos, size);
	for (TileInfo& tile : tiles)
	{
		vector2 playerPos = inGameData->players[clientID].pos;
		vector2 playerSize = size; // size는 플레이어 크기 (50, 60)
		vector2 boxLT = tile.box.m_info.LT; // 타일 박스 왼쪽 위
		vector2 boxRB = tile.box.m_info.RB; // 타일 박스 오른쪽 아래

		// 플레이어의 경계 계산
		float playerLeft = playerPos.x - playerSize.x / 2;
		float playerRight = playerPos.x + playerSize.x / 2;
		float playerTop = playerPos.y - playerSize.y / 2;
		float playerBottom = playerPos.y + playerSize.y / 2;

		// 여유 거리 설정
		const float offset = 0.0f; // 2 픽셀 정도 여유를 둠

		// 박스와 플레이어의 충돌 검사
		if (playerRight > boxLT.x && playerLeft < boxRB.x &&
			playerBottom > boxLT.y && playerTop < boxRB.y) {
			// 충돌 발생 시 위치 조정
			float overlapLeft = playerRight - boxLT.x;   // 왼쪽 겹침
			float overlapRight = boxRB.x - playerLeft;   // 오른쪽 겹침
			float overlapTop = playerBottom - boxLT.y;  // 위쪽 겹침
			float overlapBottom = boxRB.y - playerTop;  // 아래쪽 겹침

			// 가장 작은 겹침을 기준으로 밀어냄 (여유 거리 추가)
			if (overlapLeft < overlapRight && overlapLeft < overlapTop && overlapLeft < overlapBottom) {
				inGameData->players[clientID].pos.x -= (overlapLeft + offset); // 왼쪽으로 밀어냄
			}
			else if (overlapRight < overlapLeft && overlapRight < overlapTop && overlapRight < overlapBottom) {
				inGameData->players[clientID].pos.x += (overlapRight + offset); // 오른쪽으로 밀어냄
			}
			else if (overlapTop < overlapLeft && overlapTop < overlapRight && overlapTop < overlapBottom) {
				inGameData->players[clientID].pos.y -= (overlapTop + offset); // 위쪽으로 밀어냄
				if (inGameData->players[clientID].state == 12)
				{
					inGameData->players[clientID].state = 1;
				}
				else if (inGameData->players[clientID].state == 11)
				{
					inGameData->players[clientID].state = 0;
				}
			}
			else {
				inGameData->players[clientID].pos.y += (overlapBottom + offset); // 아래쪽으로 밀어냄
			}
			return true;
		}
	}
	return false;
}

void TCPServer::ServerUpdate(int clientID, InGameData* inGameData)
{
	if (!CollisionCheck(clientID, inGameData))
	{
		//if(inGameData->players[clientID].dir == 0)
		//	inGameData->players[clientID].state = 11;
		//else
		//	inGameData->players[clientID].state = 12;
	}
}

