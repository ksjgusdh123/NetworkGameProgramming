#pragma once
#include "Define.h"

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
	HANDLE hThread;
};

