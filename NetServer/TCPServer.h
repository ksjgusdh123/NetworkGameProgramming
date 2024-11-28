#pragma once
#include "Define.h"
#include "GameManager.h"

class TCPServer
{
public:
	TCPServer() : server_sock(INVALID_SOCKET) {};
	~TCPServer() { Cleanup(); };
	static TCPServer* GetInst()
	{
		if (!m_inst)
			m_inst = new TCPServer;
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
public:
	void SendPacket(const Packet& packet);
	
	bool Init();
	void Connect();
	void Run();
	void Cleanup();
	void CreateTilePacket();
public:
	queue<Packet> recvQ;
	vector<Client> clients;

private:
	static TCPServer* m_inst;
	WSADATA wsa;
	SOCKET server_sock;
};

