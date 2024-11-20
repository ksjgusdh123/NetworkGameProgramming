#pragma once
#include "Define.h"

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000

class TCPClient {
public:
	TCPClient() : sock(INVALID_SOCKET) { Init(); };
	~TCPClient() { Cleanup(); }

	bool Init();
	void Connect();
	void Cleanup();
	void SendPacket(Packet* packet);

	static TCPClient* GetInst()
	{
		if (!m_inst)
			m_inst = new TCPClient;
			return m_inst;
	}
	static void DestroyInst()
	{
		if (m_inst)
		{
			delete m_inst;
			m_inst = nullptr;
		}
	}
private:
	static TCPClient* m_inst;
	SOCKET sock;
	HANDLE hRecvThread;
	int myId;
};