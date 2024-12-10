#pragma once
#include "Define.h"

class PacketManager
{
public:
	void ProcessPacket(const Packet& packet);
	bool SendPacket(const Packet& packet);
	Packet RecvPacket();

	int GetMyID() { return m_myID; };
	string GetMyName() { return m_myName; };
	void SetMyName(string myName) { m_myName = myName; }
	
	static PacketManager& GetInst()
	{
		static PacketManager inst;
		return inst;
	}
	void Init(const SOCKET& sock);

	PacketManager();
	~PacketManager();
public:
	LobbyData m_lobbyData{};
	InGameData m_inGameData{};
	ResultData m_resultData{};

	int m_tileNum{};
	std::vector<int> m_tileType;
	std::vector<float> m_tilePosX;
	std::vector<float> m_tilePosY;
private:
	CRITICAL_SECTION cs;
	SOCKET m_sock;
	int m_myID;
	string m_myName;
};

