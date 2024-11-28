#include "PacketManager.h"
#include "..\ClientInfo.h"
#include "..\Scene\MainScene.h"
#include <Scene/SceneManager.h>
#include <Scene/Scene.h>

void PacketManager::ProcessPacket(const Packet& packet)
{
	switch (packet.type)
	{
	case LobbyInfo:
	{
		CSceneManager::GetInst()->GetScene()->PacketEvent(packet);
		break;
	}
	case PlayerMove:
	{
		C_PlayerMovePkt* cur = (C_PlayerMovePkt*)&packet;
		cur->deserialize();
		if (cur->client_id == m_myID) break;
		auto curScene = CSceneManager::GetInst()->GetScene();
		curScene->GetPlayer()->SetPos({ cur->x,cur->y });
		break;
	}
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

