#pragma once
#include "TCPClient.h"
#include "ErrDisplay.h"

queue<Packet> recv_queue;
CRITICAL_SECTION cs;

TCPClient* TCPClient::m_inst;

DWORD WINAPI RecvThread(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;

	while (true)
    {
		int client_id;
		int packet_size;
		int packet_type;
		char recv_buf[BUFSIZ];
		recv(sock, (char*)&client_id, sizeof(int), MSG_WAITALL);
		recv(sock, (char*)&packet_type, sizeof(int), MSG_WAITALL);
		recv(sock, (char*)&packet_size, sizeof(int), MSG_WAITALL);
		recv(sock, recv_buf, packet_size, MSG_WAITALL);
		Packet p(client_id,packet_size, packet_type,recv_buf);
		cout << p.data << endl;
		recv_queue.push(p);
    }
	return true;
}

void TCPClient::ProcessPacket()
{
	EnterCriticalSection(&cs);
	if (recv_queue.empty())
	{
		LeaveCriticalSection(&cs);
	}
	Packet p = recv_queue.front();
	recv_queue.pop();
	LeaveCriticalSection(&cs);

	switch (p.type)
	{
	case LoginRequest:
	{
		C_LoginRequestPkt* cur = (C_LoginRequestPkt*)&p;
		cur->deserialize();
		break;
	}
	case PlayerMove:
	{
		C_PlayerMovePkt* cur = (C_PlayerMovePkt*)&p;
		cur->deserialize();
		cout << "Player " << cur->client_id << " moved to (" << cur->x << "," << cur->y << ")\n";
		
		break;
	}
	default:
		break;
	}
}

void TCPClient::SendPacket(Packet* packet)
{
	send(sock, (char*)&packet->type, sizeof(int), 0);
	send(sock, (char*)&packet->data_size, sizeof(int), 0);
	send(sock, packet->data, packet->data_size, 0);
}

bool TCPClient::Init()
{
	cout << "Client Init()\n";

	InitializeCriticalSection(&cs);

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return false;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");
	
	Connect();	
	return true;
}

void TCPClient::Connect()
{
	cout << "Client Connect()\n";

	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);
	int retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");
	
	recv(sock, (char*)&myID, sizeof(int), MSG_WAITALL);

	hRecvThread = CreateThread(NULL, 0, RecvThread, (LPVOID)sock, 0, NULL);
	if (hRecvThread == NULL) { closesocket(sock); }
	else { CloseHandle(hRecvThread); }
}


void TCPClient::Cleanup() {
	DeleteCriticalSection(&cs);
	if (sock != INVALID_SOCKET) {
		closesocket(sock);
		sock = INVALID_SOCKET;
	}
	WSACleanup();
}

