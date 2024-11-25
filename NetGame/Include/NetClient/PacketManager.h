#pragma once
#include "Define.h"

class PacketManager
{
public:
	void ProcessPacket(const Packet& packet);
	void SendPacket(const Packet& packet);
	Packet RecvPacket();
	void EnqueueSendPacket(const Packet& packet);
	void DequeueSendPacket();

	int GetMyID() { return m_myID; };
	
	static PacketManager& GetInst()
	{
		static PacketManager inst;
		return inst;
	}
	void Init(const SOCKET& sock);

	PacketManager();
	~PacketManager();
private:
	queue<Packet> send_queue;
	CRITICAL_SECTION cs;
	SOCKET m_sock;
	int m_myID;
};

