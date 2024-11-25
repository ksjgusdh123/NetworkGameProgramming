#include "PacketManager.h"
#define RECV_CS 0
#define SEND_CS 1

void PacketManager::ProcessPacket()
{
	EnterCriticalSection(&cs[RECV_CS]);
	if (recv_queue.empty())
	{
		LeaveCriticalSection(&cs[RECV_CS]);
	}
	Packet p = recv_queue.front();
	recv_queue.pop();
	LeaveCriticalSection(&cs[RECV_CS]);

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
	return Packet(client_id, packet_type, packet_size, recv_buf);
}

void PacketManager::EnqueueSendPacket(const Packet& packet)
{
	EnterCriticalSection(&cs[SEND_CS]);
	send_queue.push(packet);
	LeaveCriticalSection(&cs[SEND_CS]);
}

void PacketManager::DequeueSendPacket()
{
	EnterCriticalSection(&cs[SEND_CS]);
	if (send_queue.empty() || m_sock == INVALID_SOCKET) {
		LeaveCriticalSection(&cs[SEND_CS]);
		return;
	}
	Packet packet = send_queue.front();
	send_queue.pop();
	LeaveCriticalSection(&cs[SEND_CS]);
	SendPacket(packet);
}

void PacketManager::EnqueueRecvPacket()
{
	Packet packet = RecvPacket();
	EnterCriticalSection(&cs[RECV_CS]);
	recv_queue.push(packet);
	LeaveCriticalSection(&cs[RECV_CS]);
}

void PacketManager::Init(const SOCKET& sock)
{
	m_sock = sock;
	recv(m_sock, (char*)&m_myID, sizeof(int), MSG_WAITALL);
}

PacketManager::PacketManager()
{
	for (auto& c : cs)
		InitializeCriticalSection(&c);
}

PacketManager::~PacketManager()
{
	for (auto& c : cs)
		DeleteCriticalSection(&c);
}

