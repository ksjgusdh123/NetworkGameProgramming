#include "GameManager.h"
#include "TCPServer.h"
#include "TileManager.h"
#include "MonsterManager.h"

void GameManager::AddLobbyPlayer(const Client& client)
{
	int i = client.player.id;
	lobbyData.players[i] = LobbyPlayerInfo(client.player);
}

void GameManager::InitGameData()
{
	MonsterManager::GetInst().Init();

	TileManager::GetInst().CreateTile();
	MonsterManager::GetInst().CreateMonster();
	gameTimer.Start();
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
		TileManager::GetInst().CollisionCheck(player);
	}
	PortalCollision();
	MonsterCollision();

}

void GameManager::PortalCollision()
{
	for (auto& player : inGameData.players) {
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
}

void GameManager::MonsterCollision()
{
	for (auto& player : inGameData.players) {
		vector2 size = vector2(50, 60);
		vector2 playerPos = player.pos;
		vector2 playerSize = size;
		float playerLeft = playerPos.x - playerSize.x / 2;
		float playerRight = playerPos.x + playerSize.x / 2;
		float playerTop = playerPos.y - playerSize.y / 2;
		float playerBottom = playerPos.y + playerSize.y / 2;

		for (int i = 0; i < MONSTER_NUM; ++i) {
			if (!inGameData.monster[i].is_alive) continue;
			vector2 MonsterPos = inGameData.monster[i].pos;
			vector2 MonsterSize = inGameData.monster[i].size;
			vector2 MonsterLT = vector2(MonsterPos.x - MonsterSize.x / 2, MonsterPos.y - MonsterSize.y / 2);
			vector2 MonsterRB = vector2(MonsterPos.x + MonsterSize.x / 2, MonsterPos.y + MonsterSize.y / 2);
			if (playerRight > MonsterLT.x && playerLeft < MonsterRB.x &&
				playerBottom > MonsterLT.y && playerTop < MonsterRB.y) {

				if (player.state == 5 || player.state == 6 || player.state == 7 || player.state == 8) { // 플레이어가 공격상태일 경우 
					if (inGameData.monster[i].direct == EObject_Dir::Right)
						inGameData.monster[i].state = EObject_State::Die;
					else
						inGameData.monster[i].state = EObject_State::Die_L;
				}
				else
				{
					// 데미지 입음		
				}
			}
			
		}
	}
}