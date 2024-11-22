#include "TCPServer.h"
#include "ErrDisplay.h"

TCPServer* TCPServer::m_inst;
CRITICAL_SECTION cs;

DWORD WINAPI RecvThread(LPVOID arg)
{
	cout << "Start RecvThread!\n";

	SOCKET client_sock = (SOCKET)arg;
	int client_id = static_cast<int>(TCPServer::GetInst()->clients.size() + 1);
	int res = send(client_sock, (char*)&client_id, sizeof(int), 0);
	if (res == SOCKET_ERROR) { err_display("send()"); return -1; }

	Client client(client_sock, client_id, "");
	TCPServer::GetInst()->clients.emplace_back(client);

	while (true)
	{
		int packet_type;
		int data_size;
		char packet_data[DATA_SIZE];

		int res = recv(client.socket, (char*)&packet_type, sizeof(int), MSG_WAITALL);
		if (res <= 0) break;
		res = recv(client.socket, (char*)&data_size, sizeof(int), MSG_WAITALL);
		if (res <= 0) break;
		res = recv(client.socket, packet_data, data_size, MSG_WAITALL);
		if (res <= 0) break;

		Packet p(client.id, packet_type, data_size, packet_data);
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
			Sleep(100);
			continue;
		}
		Packet packet = recvQ.front();
		recvQ.pop();

		switch (packet.type)
		{
		case LOGIN:
		{
			C_LoginRequestPkt* cur = (C_LoginRequestPkt*)&packet;
			cur->deserialize();
			cout << "[LOGIN] " << cur->client_id << " Player Name = " << cur->data << endl;
			auto& cur_client = TCPServer::GetInst()->clients[cur->client_id-1];
			cur_client.name = cur->data;
			TCPServer::GetInst()->SendPacket(packet);
			break;
		}
		case MOVE:
		{
			C_PlayerMovePkt* cur = (C_PlayerMovePkt*)&packet;
			cur->deserialize();
			cout << "[MOVE] " << cur->client_id << " Player moved to (" << cur->x << "," << cur->y << ")\n";
			TCPServer::GetInst()->SendPacket(packet);
			break;
		}
		default:
			break;
		}
		LeaveCriticalSection(&cs);
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

	hWorkerThread = CreateThread(NULL, 0, WorkerThread, NULL, 0, NULL);
	if (hWorkerThread == NULL) err_display("WorkerThread");

	return true;
}

void TCPServer::Connect()
{
	cout << "Connect()\n";
	while (true)
	{
		sockaddr_in clientaddr;
		int addrlen = sizeof(clientaddr);
		SOCKET client_sock = accept(server_sock, (sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) { err_display("accept()"); continue; }

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

void TCPServer::SendPacket(const Packet& packet)
{
	for (const auto& client : clients)
	{
		int res = send(client.socket, (char*)&packet.client_id, sizeof(int), 0);
		res = send(client.socket, (char*)&packet.data_size, sizeof(int), 0);
		res = send(client.socket, (char*)&packet.type, sizeof(int), 0);
		res = send(client.socket, packet.data, packet.data_size, 0);
		if (res == SOCKET_ERROR) { err_display("send()"); }
	}
}
