#pragma once
#include "Define.h"

struct Client {
	SOCKET socket;
	string name;
	PlayerInfo player;
	int& id = player.id;
	Client(SOCKET sock, int clientId, const string& clientName)
		: socket(sock), id(clientId), name(clientName) {}
};

class TCPServer
{
public:
	TCPServer() : server_sock(INVALID_SOCKET) {};
	~TCPServer() { closesocket(server_sock); WSACleanup(); };
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
	bool Init();
	void Connect();
	void Run();

	void SendPacket(const Packet& packet);
public:
	queue<Packet> recvQ;
	vector<Client> clients;

private:
	static TCPServer* m_inst;

	WSADATA wsa;
	SOCKET server_sock;
	HANDLE hRecvThread, hWorkerThread;
};

