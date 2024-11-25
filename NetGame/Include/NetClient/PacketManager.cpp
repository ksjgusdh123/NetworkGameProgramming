#include "PacketManager.h"

void PacketManager::ProcessPacket()
{
	EnterCriticalSection(&cs);
	if (recv_queue.empty())
	{
		LeaveCriticalSection(&cs);
	}
	Packet p = recv_queue.front();
	recv_queue.pop();
	LeaveCriticalSection(&cs);

	switch (p.type)
	{
	case LoginRequest:
	{
		C_LoginRequestPkt* cur = (C_LoginRequestPkt*)&p;
		cur->deserialize();
	}
	break;
	case PlayerMove:
	{
		C_PlayerMovePkt* cur = (C_PlayerMovePkt*)&p;
		cur->deserialize();
	}
	break;
	default:
		break;
	}
}

void PacketManager::SendPacket(const Packet& packet)
{
	send(m_sock, (char*)&packet.type, sizeof(int), 0);
	send(m_sock, (char*)&packet.data_size, sizeof(int), 0);
	send(m_sock, packet.data, packet.data_size, 0);
}

Packet PacketManager::RecvPacket()
{
	int client_id;
	int packet_size;
	int packet_type;
	char recv_buf[BUFSIZ];
	recv(m_sock, (char*)&client_id, sizeof(int), MSG_WAITALL);
	recv(m_sock, (char*)&packet_type, sizeof(int), MSG_WAITALL);
	recv(m_sock, (char*)&packet_size, sizeof(int), MSG_WAITALL);
	recv(m_sock, recv_buf, packet_size, MSG_WAITALL);
	return Packet(client_id, packet_size, packet_type, recv_buf);
}

void PacketManager::RecvPacketPushQueue()
{
	Packet p = RecvPacket();
	EnterCriticalSection(&cs);
	recv_queue.push(p);
	LeaveCriticalSection(&cs);
}

void PacketManager::Init(const SOCKET& sock)
{
	m_sock = sock;
	recv(m_sock, (char*)&m_myID, sizeof(int), MSG_WAITALL);
}

PacketManager::PacketManager()
{
	InitializeCriticalSection(&cs);
}

PacketManager::~PacketManager()
{
	DeleteCriticalSection(&cs);
}

