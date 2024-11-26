#include "PacketManager.h"
#define RECV_CS 0
#define SEND_CS 1

void PacketManager::ProcessPacket(const Packet& packet)
{
	switch (packet.type)
	{
	case LoginRequest:
	{
		C_LoginRequestPkt* cur = (C_LoginRequestPkt*)&packet;
		cur->deserialize();
	}
	break;
	case PlayerMove:
	{
		C_PlayerMovePkt* cur = (C_PlayerMovePkt*)&packet;
		cur->deserialize();
	}
	break;
	case LobbyRequest:
	{
		C_LobbyRequestPkt* cur = (C_LobbyRequestPkt*)&packet;
		cur->deserialize();
		ClientNum = cur->ClientNum;
		for (int i = 0; i < ClientNum; ++i)
		{
			strcpy_s(name[i], sizeof(name[i]), cur->name[i]);
		}
	}
	break;
	case Tiles:
	{		
		C_TilesPkt* cur = (C_TilesPkt*)&packet;
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
	EnterCriticalSection(&cs);
	send_queue.push(packet);
	LeaveCriticalSection(&cs);
}
	
void PacketManager::DequeueSendPacket()
{
	EnterCriticalSection(&cs);
	if (send_queue.empty() || m_sock == INVALID_SOCKET) {
		LeaveCriticalSection(&cs);
		return;
	}
	Packet packet = send_queue.front();
	send_queue.pop();
	LeaveCriticalSection(&cs);
	SendPacket(packet);
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

