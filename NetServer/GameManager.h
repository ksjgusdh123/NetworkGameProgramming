#pragma once
#include "Define.h"
#include "GameTimer.h"

struct Client {
	SOCKET socket;
	PlayerInfo player;

	Client(SOCKET sock) : socket(sock)
	{
		static int i = 0;
		player.id = i++;
	}
};

class GameManager {

public:
	void AddLobbyPlayer(const Client& client);
	LobbyData* GetLobbyData() { return &lobbyData; };
	InGameData* GetInGameData() { return &inGameData; };

	void InitGameData();
	void PrintLobbyState();
	void PrintGameState();

	void CreateTile();
	void SendTilePacket();
	void SendLobbyGameData();
	void SendInGameData();

	void UpdateInGameData();
	void ProcessCollsion();
	bool CollisionCheck(GamePlayerInfo& player);
	
public:
	static GameManager& GetInst()
	{
		static GameManager inst;
		return inst;
	}
	GameManager() { InitGameData(); };
private:
	LobbyData lobbyData;
	InGameData inGameData;
	GameTimer gameTimer;

	vector<TileInfo> tiles;
	std::vector<int> tileNumbers;
	std::vector<vector2> tilePositions;
};
