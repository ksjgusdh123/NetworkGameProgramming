#pragma once
#include "Define.h"
#include "Protocol.h"

class TCPServer
{
	WSADATA wsa;
	SOCKET listen_sock;

	HANDLE hRecvThread;
	HANDLE hSendThread;
public:
	TCPServer() {};
	~TCPServer() { WSACleanup(); closesocket(listen_sock);};
	bool Init();
	void Connect();
	void Run();
};

