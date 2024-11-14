#pragma once
#include "Define.h"
#include <queue>

class TCPServer
{
public:
	TCPServer() : listen_sock(INVALID_SOCKET) {};
	~TCPServer() { closesocket(listen_sock); WSACleanup();};
	bool Init();
	void Connect();
	void Run();

private:
	WSADATA wsa;
	SOCKET listen_sock;
	HANDLE hRecvThread;
	HANDLE hSendThread;
};

