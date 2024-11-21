#pragma once
#include "TCPClient.h"
#include "ErrDisplay.h"

queue<Packet> recv_queue;
TCPClient* TCPClient::m_inst;

DWORD WINAPI RecvThread(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;

	while (true)
    {
		int packet_size;
		int packet_type;
		char recv_buf[BUFSIZ];
		recv(sock, (char*)&packet_size, sizeof(int), MSG_WAITALL);
		recv(sock, (char*)&packet_type, sizeof(int), MSG_WAITALL);
		recv(sock, recv_buf, packet_size, MSG_WAITALL);
		Packet p(packet_size, packet_type,recv_buf);
		cout << p.data << endl;
		recv_queue.push(p);
    }
	return true;
}
bool TCPClient::Init()
{
	cout << "Client Init()\n";
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

	hRecvThread = CreateThread(NULL, 0, RecvThread, (LPVOID)sock, 0, NULL);
	if (hRecvThread == NULL) { closesocket(sock); }
	else { CloseHandle(hRecvThread); }
}


void TCPClient::Cleanup() {
	if (sock != INVALID_SOCKET) {
		closesocket(sock);
		sock = INVALID_SOCKET;
	}
	WSACleanup();
}

void TCPClient::SendPacket(Packet* packet)
{
	send(sock, (char*)&packet->data_size, sizeof(int), 0);
	send(sock, (char*)&packet->type, sizeof(int), 0);
	send(sock, packet->data, packet->data_size, 0);
}

