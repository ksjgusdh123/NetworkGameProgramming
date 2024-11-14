#pragma once
#include "TCPClient.h"
#include "ErrDisplay.h"

queue<Packet> recv_queue;

DWORD WINAPI RecvThread(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;
	while (true)
    {
		int packet_size;
		char recv_buf[BUFSIZ];
		recv(sock, (char*)&packet_size, sizeof(int), MSG_WAITALL);
		recv(sock, recv_buf, packet_size, MSG_WAITALL);
		Packet p(packet_size, recv_buf);
		cout << p.data << endl;
		recv_queue.push(p);
    }
	return true;
}
bool TCPClient::Init()
{
	cout << "Client Init()\n";
	// 扩加 檬扁拳
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return false;

	// 家南 积己
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

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

void TCPClient::SendPacket(int len, const char* data)
{
	send(sock, (char*)&len, sizeof(int), 0);
	send(sock, data, len, 0);
}

