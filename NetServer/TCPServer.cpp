#include "ErrDisplay.h"
#include "TCPServer.h"
#include "GameManager.h"

TCPServer* TCPServer::m_inst;
CRITICAL_SECTION cs;

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
		int type;
		int data_size;
		char data[BUFSIZ];
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
	}

	cout << "End RecvThread!\n";
	closesocket(client.socket);
	return 0;
}

DWORD WINAPI WorkerThread(LPVOID arg)
{
	while (true)
	{
		EnterCriticalSection(&cs);
		auto& recvQ = TCPServer::GetInst()->recvQ;
		if (recvQ.empty())
		{
			LeaveCriticalSection(&cs);
			continue;
		}
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
			//client.player.name = RecvPacket->data;
			strncpy_s(client.player.name, RecvPacket->data, NAME_LEN);
			GameManager::GetInst().AddLobbyPlayer(client);
			break;
		}
		case LobbyUpdateRequest:
		{
			LobbyData* gameData = GameManager::GetInst().GetLobbyData();
			S_LobbyInfoPacket* RecvPacket = (S_LobbyInfoPacket*)&packet;
			const int i = RecvPacket->client_id;
			memcpy(&gameData->players[i], RecvPacket->data, RecvPacket->data_size);
			GameManager::GetInst().PrintLobbyState();
			S_LobbyInfoPacket SendPacket(*gameData);
			TCPServer::GetInst()->SendPacket(SendPacket);
			break;
		}
		case PlayerMove:
		{
			C_PlayerMovePkt* cur = (C_PlayerMovePkt*)&packet;
			cur->deserialize();
			cout << "[MOVE] " << cur->client_id << " Player moved to (" << cur->x << "," << cur->y << ")\n";
			TCPServer::GetInst()->SendPacket(packet);
			break;
		}
		case PlayerChoice:
		{
			C_PlayerChoicePkt* cur = (C_PlayerChoicePkt*)&packet;
			cur->deserialize();
			//
			S_LobbyInfoPacket SendPacket(*GameManager::GetInst().GetLobbyData());
			TCPServer::GetInst()->SendPacket(SendPacket);
			break;
		}
		case GameStartRequest:
		{
			C_GameStartRequestPkt* cur = (C_GameStartRequestPkt*)&packet;
			cur->deserialize();
			TCPServer::GetInst()->SendPacket(packet);
			break;
		}
		default:
			break;
		}
		LeaveCriticalSection(&cs);
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
	DeleteCriticalSection(&cs);
	closesocket(server_sock);
	WSACleanup();
};
