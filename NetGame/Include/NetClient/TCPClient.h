#pragma once
#include "Define.h"
#include "../NetClient/PacketManager.h"

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000

class TCPClient {
public:
	TCPClient() : sock(INVALID_SOCKET) { Init(); };
	~TCPClient() { Cleanup(); }

	bool Init();
	void Connect();
	void Cleanup();
	static TCPClient& GetInst()
	{
		static TCPClient inst;
		return inst;
	}

private:
	SOCKET sock;
	HANDLE hRecvThread, hSendThread;
};