#pragma once
#include "Define.h"

class TCPServer
{
	WSADATA wsa;
	SOCKET listen_sock;
public:
	TCPServer() : listen_sock(INVALID_SOCKET) {};
	~TCPServer() { WSACleanup(); closesocket(listen_sock);};
	bool Init();
	void Connect();
	void Run();
};

