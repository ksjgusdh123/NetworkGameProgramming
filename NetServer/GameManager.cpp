#include "GameManager.h"
#include "TCPServer.h"
#include "TileManager.h"
#include "MonsterManager.h"
#include "ItemManager.h"

void GameManager::AddLobbyPlayer(const Client& client)
{
	int i = client.player.id;
	lobbyData.players[i] = LobbyPlayerInfo(client.player);
}

void GameManager::InitObjectManager()
{
	MonsterManager::GetInst().Init();
	ItemManager::GetInst().Init();
}

void GameManager::InitGameData()
{
	InitObjectManager();
	TileManager::GetInst().CreateTile();
	MonsterManager::GetInst().CreateMonster();
	gameTimer.Start();
}

void GameManager::InitBossData()
{
	TileManager::GetInst().CreateBossTile();
	MonsterManager::GetInst().CreateBossMonster();
}

void GameManager::UpdateInGameData()
{
	inGameData.playtime = gameTimer.GetElapsedTime();
	MonsterManager::GetInst().UpdateMonster();
	ProcessCollsion();
}

void GameManager::PrintLobbyState()
{
	std::cout << "Game State:\n";

	for (int i = 0; i < PLAYER_NUM; ++i) {
		const auto& player = lobbyData.players[i];
		std::cout << "Player " << i
			<< ": ID=" << player.id
			<< ", Name=" << player.name
			<< ", Job=" << (int)player.job << "\n";
	}
}

void GameManager::PrintGameState()
{
	std::cout << "Game State:\n";

	for (int i = 0; i < PLAYER_NUM; ++i) {
		const auto& player = inGameData.players[i];
		std::cout << "Player " << i
			<< ": ID=" << player.id
			<< ", Name=" << player.name
			<< ", Pos=(" << player.pos.x << "," << player.pos.y << ")"
			<< ", HP=" << player.hp
			<< ", Job=" << player.job << "\n";
	}
}

void GameManager::CacluateResult(bool bWin)
{
	resultData.bWin = bWin;
	resultData.playTime = inGameData.playtime;
}

void GameManager::SendLobbyGameData()
{
	S_LobbyInfoPacket SendPacket(lobbyData);
	TCPServer::GetInst()->SendPacket(SendPacket);
}

void GameManager::SendInGameData()
{
	S_GameInfoPacket SendPacket(inGameData);
	TCPServer::GetInst()->SendPacket(SendPacket);
}

void GameManager::SendResultData()
{
	S_GameEndNotificationPacket SendPacket(resultData);
	TCPServer::GetInst()->SendPacket(SendPacket);
}

void GameManager::ProcessCollsion()
{
	for (auto& player : inGameData.players)
	{
		TileManager::GetInst().CheckTileCollision(player);
		ItemManager::GetInst().CheckItemCollision(player);
		CheckPortalCollision(player);
	}
}

void GameManager::CheckPortalCollision(GamePlayerInfo& player)
{
	vector2 size = vector2(50, 60);
	vector2 playerPos = player.pos;
	vector2 playerSize = size;
	float playerLeft = playerPos.x - playerSize.x / 2;
	float playerRight = playerPos.x + playerSize.x / 2;
	float playerTop = playerPos.y - playerSize.y / 2;
	float playerBottom = playerPos.y + playerSize.y / 2;

	vector2 PortalLT = vector2(717.5f, -180.f);
	vector2 PortalRB = vector2(742.5f, -120.f);
	if (playerRight > PortalLT.x && playerLeft < PortalRB.x &&
		playerBottom > PortalLT.y && playerTop < PortalRB.y) {
		player.bReady = true;
	}
	else
	{
		player.bReady = false;
	}
}