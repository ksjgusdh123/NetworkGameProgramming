#pragma once
#include "TCPClient.h"
#include "ErrDisplay.h"

DWORD WINAPI RecvThread(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;
	char buffer[BUFSIZ] = "test";
	int recv_len;
	while (1)
	{
		int retval = send(sock, buffer, BUFSIZ, 0);
		if (retval == SOCKET_ERROR) {
		err_display("send()");
		return -1;
	}
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

