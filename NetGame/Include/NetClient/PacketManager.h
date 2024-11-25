#pragma once
#include "Define.h"

class PacketManager
{
public:
	void ProcessPacket();
	void SendPacket(const Packet& packet);
	Packet RecvPacket();
	void RecvPacketPushQueue();

	static PacketManager& GetInst()
	{
		static PacketManager inst;
		return inst;
	}
	void Init(const SOCKET& sock);

	PacketManager();
	~PacketManager();
private:
	queue<Packet> recv_queue;
	CRITICAL_SECTION cs;
	SOCKET m_sock;
	int m_myID;
};

