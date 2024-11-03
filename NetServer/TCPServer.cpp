#pragma once
#include "TCPServer.h"
#include "ErrDisplay.h"

DWORD WINAPI RecvThread(LPVOID arg) { cout << "RecvThread()\n"; return 0; };
DWORD WINAPI SendThread(LPVOID arg) { cout << "SendThread()\n"; return 0; };

bool TCPServer::Init()
{
	cout << "Init()\n";

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return false;
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	int retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

}

void TCPServer::Connect()
{
	cout << "Connect()\n";

	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	int addrlen;

	static int client_num{};
	while (client_num != 2)
	{
		cout << "Waiting...\n";
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) { err_display("accept()"); continue; }

		++client_num;

		hRecvThread = CreateThread(NULL, 0, RecvThread, (LPVOID)client_sock, 0, NULL);
		if (hRecvThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hRecvThread); }

		hRecvThread = CreateThread(NULL, 0, RecvThread, (LPVOID)client_sock, 0, NULL);
		if (hRecvThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hRecvThread); }
		cout << "Accept!\n";
	}
}

void TCPServer::Run()
{
	cout << "Run()\n";
	Connect();
}
