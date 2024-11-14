#pragma once
#include "Define.h"

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000

class TCPClient {
public:
	TCPClient() : sock(INVALID_SOCKET) {};
	~TCPClient() { Cleanup(); }

	bool Init();
	void Connect();
	void Cleanup();

private:
	SOCKET sock;
	HANDLE hRecvThread;
};