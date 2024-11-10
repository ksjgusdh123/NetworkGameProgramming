#pragma once
#include "TCPServer.h"
#include "ErrDisplay.h"
#include "ThreadMgr.h"

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

	return true;
}

void TCPServer::Connect()
{
	cout << "Connect()\n";

	ThreadMgr::GetInst().CreateWorkerThread();

	static int client_num{};
	while (client_num != 2)
	{
		cout << "Waiting...\n";
		SOCKET client_sock = accept(listen_sock, NULL, NULL);
		if (client_sock == INVALID_SOCKET) { err_display("accept()"); continue; }

		++client_num;

		ThreadMgr::GetInst().CreateRecvThread(client_sock);
	}
}

void TCPServer::Run()
{
	cout << "Run()\n";
	Connect();
}
