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
	ResultData* GetResultData() { return &resultData; };

	void InitBossData();
	void InitGameData();
	void PrintLobbyState();
	void PrintGameState();

	void CreateTile();
	void CreateMonster();
	void CacluateResult(bool bWin);

	void CreateBossTile();
	void CreateBossMonster();

	void AddTile(TileInfo& info, vector2 blockSize, int type, int x, int y);
	void SendTilePacket();
	void SendLobbyGameData();
	void SendInGameData();
	void SendResultData();

	void UpdateInGameData();
	void ProcessCollsion();
	bool CollisionCheck(GamePlayerInfo& player);
	void PortalCollision();


	void UpdateMonster();
	int IsPlayerInRicheAttackArea();
	
	void UpdatePlayer();

public:
	static GameManager& GetInst()
	{
		static GameManager inst;
		return inst;
	}
	
private:
	LobbyData lobbyData;
	InGameData inGameData;
	ResultData resultData;
	GameTimer gameTimer;

	vector<TileInfo> tiles;
	std::vector<int> tileNumbers;
	std::vector<vector2> tilePositions;
};
