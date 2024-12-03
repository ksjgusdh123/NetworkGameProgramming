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

class GameManager{

public:
	void AddLobbyPlayer(const Client& client);
	LobbyData* GetLobbyData() { return &lobbyData; };
	InGameData* GetInGameData() { return &inGameData; };
	ResultData* GetResultData() { return &resultData; };

	void InitGameData();
	void PrintLobbyState();
	void PrintGameState();
	void CacluateResult(bool bWin);

	void SendLobbyGameData();
	void SendInGameData();
	void SendResultData();

	void UpdateInGameData();
	void ProcessCollsion();
	void PortalCollision();
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
};
