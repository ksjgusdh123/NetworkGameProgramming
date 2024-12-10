#include "PacketManager.h"
#include "..\ClientInfo.h"
#include "..\Scene\MainScene.h"
#include <Scene/SceneManager.h>
#include <Scene/Scene.h>

void PacketManager::ProcessPacket(const Packet& packet)
{
	switch (packet.type)
	{
	case TileResponse:
	{
		S_TilesPkt* cur = (S_TilesPkt*)&packet;
		cur->deserialize(m_tileNum, m_tileType, m_tilePosX, m_tilePosY);
		break;
	}
	case LobbyUpdateResponse:
	{
		S_LobbyInfoPacket* RecvPacket = (S_LobbyInfoPacket*)&packet;
		memcpy(&m_lobbyData, RecvPacket->data, RecvPacket->data_size);
		break;
	}
	case GameUpdateResponse:
	{
		S_GameInfoPacket* RecvPacket = (S_GameInfoPacket*)&packet;
		memcpy(&m_inGameData, RecvPacket->data, RecvPacket->data_size);
		break;
	}
	case GameEndNotification:
	{
		S_GameEndNotificationPacket* RecvPacket = (S_GameEndNotificationPacket*)&packet;
		memcpy(&m_resultData, RecvPacket->data, RecvPacket->data_size);
		break;
	}
	default:
		break;
	}
}

bool PacketManager::SendPacket(const Packet& packet)
{
	int res;
	res = send(m_sock, (char*)&packet.type, sizeof(int), 0);
	res = send(m_sock, (char*)&packet.data_size, sizeof(int), 0);
	res = send(m_sock, packet.data, packet.data_size, 0);
	if (res == SOCKET_ERROR)
		return false;
	else return true;
}

Packet PacketManager::RecvPacket()
{
	int client_id;
	int packet_size;
	int packet_type;
	char recv_buf[DATA_SIZE];
	recv(m_sock, (char*)&client_id, sizeof(int), MSG_WAITALL);
	recv(m_sock, (char*)&packet_type, sizeof(int), MSG_WAITALL);
	recv(m_sock, (char*)&packet_size, sizeof(int), MSG_WAITALL);
	recv(m_sock, recv_buf, packet_size, MSG_WAITALL);
	return Packet(client_id, packet_type, packet_size, recv_buf);
}

void PacketManager::Init(const SOCKET& sock)
{
	m_sock = sock;
	recv(m_sock, (char*)&m_myID, sizeof(int), MSG_WAITALL);
	m_lobbyData.players[m_myID].id = m_myID;
}

PacketManager::PacketManager()
{
	InitializeCriticalSection(&cs);
}

PacketManager::~PacketManager()
{
	DeleteCriticalSection(&cs);
}

