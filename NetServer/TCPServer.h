#pragma once
#include "Define.h"

class TCPServer
{
public:
	TCPServer() : listen_sock(INVALID_SOCKET) {};
	~TCPServer() { WSACleanup(); closesocket(listen_sock);};
	bool Init();
	void Connect();
	void Run();
private:
	WSADATA wsa;
	SOCKET listen_sock;
};

