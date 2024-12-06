#include "GameManager.h"
#include "TCPServer.h"
#include "TileManager.h"
#include "MonsterManager.h"
#include "ItemManager.h"

float elapsed;
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
	ItemManager::GetInst().CreateItem();
	gameTimer.Start();
}

void GameManager::InitBossData()
{
	TileManager::GetInst().CreateBossTile();
	MonsterManager::GetInst().CreateBossMonster();
}

void GameManager::UpdateInGameData()
{
	inGameData.playtime = (int)gameTimer.GetElapsedTime();
	elapsed = gameTimer.Update();
	UpdatePlayer();		
	MonsterManager::GetInst().UpdateMonster(elapsed);
	CalculateArrow();
	ProcessCollsion();
}

void GameManager::UpdatePlayer()
{
	for (int i = 0; i < 2; ++i)
	{
		inGameData.players[i].pos.y += 100 * elapsed;
		switch (inGameData.players[i].state)
		{
		case EObject_State::Basic:
		case EObject_State::Basic_L:
			inGameData.players[i].nowFrame = 0;
			break;
		case EObject_State::Walk:
		{
			inGameData.players[i].pos.x += 150.f * elapsed * 2;
			break;
		}
		case EObject_State::Walk_L:
		{
			inGameData.players[i].pos.x -= 150.f * elapsed * 2;
			break;
		}
		case EObject_State::Die:
		case EObject_State::Die_L:
			return;
		case EObject_State::Attack:
		case EObject_State::Attack_L:
			inGameData.players[i].nowFrame += elapsed;
			if (inGameData.players[i].nowFrame >= 1)
			{
				if (inGameData.players[i].dir == EObject_Dir::Right)
				{
					inGameData.players[i].state = EObject_State::Basic;
				}
				else
				{
					inGameData.players[i].state = EObject_State::Basic_L;
				}
			}
			break;
		default:
			break;
		}

		switch (inGameData.players[i].jumpState)
		{
		case EJump_State::Landed:
		case EJump_State::JumpDown:
			inGameData.players[i].jumpTime = 0.f;
			break;
		case EJump_State::Jumping:
		{
			inGameData.players[i].pos.y -= 250.f * elapsed;
			inGameData.players[i].jumpTime += elapsed;
			if (inGameData.players[i].jumpTime >= 0.5)
			{
				inGameData.players[i].jumpState = EJump_State::JumpDown;
			}
			break;
		}
		default:
			break;
		}
	}

}

void GameManager::UpdateBossData()
{
	inGameData.playtime = gameTimer.GetElapsedTime();
	elapsed = gameTimer.Update();
	UpdatePlayer();
	MonsterManager::GetInst().UpdateMonster(elapsed);
	CalculateArrow();
	ProcessBossCollsion();
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
			<< ", Job=" << (int)player.job << "\n";
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
		if (player.hp <= 0)
			continue;
		ItemManager::GetInst().CheckItemCollision(player);
		CheckPortalCollision(player);
	}
	CheckArrowCollision();
	MonsterCollision();
}

void GameManager::ProcessBossCollsion()
{
	for (auto& player : inGameData.players)
	{
		TileManager::GetInst().CheckTileCollision(player);
	}
	BossCollsion();
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

void GameManager::CheckArrowCollision()
{
	for (ArrowInfo& arrow : inGameData.arrowAttack)
	{
		if (!arrow.is_alive)
			continue;

		if (TileManager::GetInst().CheckArrowTileCollision(arrow))
		{
			arrow.is_alive = false;
		}

		for (int i = 0; i < MONSTER_NUM; ++i) 
		{
			if (!inGameData.monster[i].is_alive) continue;

			if (inGameData.monster[i].box.CheckCollision(&arrow.box))
			{
				arrow.is_alive = false;
				if (inGameData.monster[i].direct == EObject_Dir::Right)
					inGameData.monster[i].state = EObject_State::Die;
				else
					inGameData.monster[i].state = EObject_State::Die_L;
				inGameData.monster[i].timer = 0.f;
			}
		}

		if (ItemManager::GetInst().CheckArrowItemCollision(arrow))
		{
			arrow.is_alive = false;
		}
	}
}

void GameManager::MonsterCollision()
{
	for (auto& player : inGameData.players) {
		vector2 size = player.GetPlayerSize();
		player.box.UpdateCollision(player.pos, size);
		for (int i = 0; i < MONSTER_NUM; ++i) 
		{
			if (!inGameData.monster[i].is_alive || inGameData.monster[i].state == EObject_State::Die || inGameData.monster[i].state == EObject_State::Die_L) continue;

			inGameData.monster[i].box.UpdateCollision(inGameData.monster[i].pos, inGameData.monster[i].size);
			if(inGameData.monster[i].box.CheckCollision(&player.box))
			{
				if (player.state == EObject_State::Attack || player.state == EObject_State::Attack_L) { // 플레이어가 공격상태일 경우 
					if (inGameData.monster[i].direct == EObject_Dir::Right)
						inGameData.monster[i].state = EObject_State::Die;
					else
						inGameData.monster[i].state = EObject_State::Die_L;
					inGameData.monster[i].timer = 0.f;
				}
				else
				{
					player.hp -= 1;
					player.hp = std::clamp((int)player.hp, 0, 100);
				}
			}

			for (int i = 0; i < MONSTER_ATTACK_NUM; ++i) 
			{
				if (!inGameData.monsterAttack[i].is_alive) continue;

				vector2 MonsterAttackPos = inGameData.monsterAttack[i].pos;
				vector2 MonsterAttackSize = inGameData.monsterAttack[i].size;
				Collision box;
				box.UpdateCollision(MonsterAttackPos, MonsterAttackSize);
				if (box.CheckCollision(&player.box))
				{
					if (player.job == EPlayer_Job::Sword) 
					{ // 전사일 때 원거리 공격 삭제
						if (player.state == EObject_State::Attack || player.state == EObject_State::Attack_L)
						{ // 플레이어가 공격상태일 경우 
							inGameData.monsterAttack[i].is_alive = false;
							break;
						}
						else
						{
							player.hp -= 1;
							player.hp = std::clamp((int)player.hp, 0, 100);
						}
					}
					else 
					{	// 궁수 일 때 피 깎임 
						player.hp -= 1;
						player.hp = std::clamp((int)player.hp, 0, 100);
					}

				}

			}

		}
	}
}

void GameManager::BossCollsion()
{
	for (auto& player : inGameData.players) {
		vector2 size = vector2(50, 60);
		vector2 playerPos = player.pos;
		vector2 playerSize = size;
		float playerLeft = playerPos.x - playerSize.x / 2;
		float playerRight = playerPos.x + playerSize.x / 2;
		float playerTop = playerPos.y - playerSize.y / 2;
		float playerBottom = playerPos.y + playerSize.y / 2;



		if (!inGameData.monster[0].is_alive) continue;
		vector2 MonsterPos = inGameData.monster[0].pos;
		vector2 MonsterSize = inGameData.monster[0].size;
		vector2 MonsterLT = vector2(MonsterPos.x - MonsterSize.x / 2, MonsterPos.y - MonsterSize.y / 2);
		vector2 MonsterRB = vector2(MonsterPos.x + MonsterSize.x / 2, MonsterPos.y + MonsterSize.y / 2);
		if (playerRight > MonsterLT.x && playerLeft < MonsterRB.x &&
			playerBottom > MonsterLT.y && playerTop < MonsterRB.y) {
			if (player.state == EObject_State::Attack || player.state == EObject_State::Attack_L) { // 플레이어가 공격상태일 경우 
				inGameData.monster[0].hp -= 1;
				inGameData.monster[0].hp = std::clamp((int)inGameData.monster[0].hp, 0, 100);
			}
			else
			{
				player.hp -= 1;
				player.hp = std::clamp((int)player.hp, 0, 100);
			}
		}


		for (int j = 0; j < ARROW_NUM; ++j) {
			if (!inGameData.arrowAttack[j].is_alive) continue;
			vector2 ArrowPos = inGameData.arrowAttack[j].pos;
			vector2 ArrowSize = inGameData.arrowAttack[j].size;
			vector2 ArrowLT = vector2(ArrowPos.x - ArrowSize.x / 2, ArrowPos.y - ArrowSize.y / 2);
			vector2 ArrowRB = vector2(ArrowPos.x + ArrowSize.x / 2, ArrowPos.y + ArrowSize.y / 2);
			if (ArrowRB.x > MonsterLT.x && ArrowLT.x < MonsterRB.x &&
				ArrowRB.y > MonsterLT.y && ArrowLT.y < MonsterRB.y) {
				inGameData.monster[0].hp -= 1;
				inGameData.monster[0].hp = std::clamp((int)inGameData.monster[0].hp, 0, 100);
				inGameData.arrowAttack[j].is_alive = false;
			}
		}

		for (int i = 0; i < MONSTER_ATTACK_NUM; ++i) {
			if (!inGameData.monsterAttack[i].is_alive) continue;
			vector2 MonsterAttackPos = inGameData.monsterAttack[i].pos;
			vector2 MonsterAttackSize = inGameData.monsterAttack[i].size;
			vector2 MonsterAttackLT = vector2(MonsterAttackPos.x - MonsterAttackSize.x / 2, MonsterAttackPos.y - MonsterAttackSize.y / 2);
			vector2 MonsterAttackRB = vector2(MonsterAttackPos.x + MonsterAttackSize.x / 2, MonsterAttackPos.y + MonsterAttackSize.y / 2);
			if (playerRight > MonsterAttackLT.x && playerLeft < MonsterAttackRB.x &&
				playerBottom > MonsterAttackLT.y && playerTop < MonsterAttackRB.y) {

				if (player.job == EPlayer_Job::Sword) { // 전사일 때 원거리 공격 삭제
					if (player.state == EObject_State::Attack || player.state == EObject_State::Attack_L) { // 플레이어가 공격상태일 경우 
						inGameData.monsterAttack[i].is_alive = false;
						break;
					}
					else
					{
						player.hp -= 1;
						player.hp = std::clamp((int)player.hp, 0, 100);
					}
				}
				else {	// 궁수 일 때 피 깎임 
					player.hp -= 1;
					player.hp = std::clamp((int)player.hp, 0, 100);
				}

			}

		}

	}
}



void GameManager::CalculateArrow()
{
	for (auto& player : inGameData.players)
	{
		if (player.job != EPlayer_Job::Archer)
			continue;

		if (player.state == EObject_State::Attack || player.state == EObject_State::Attack_L)
		{
			player.timer += elapsed;

			if (player.timer >= 0.9f)
			{
				for (ArrowInfo& arrow : inGameData.arrowAttack)
				{
					if (arrow.is_alive)	continue;
					arrow.pos = player.pos;
					arrow.is_alive = true;
					arrow.timer = 0.f;
					player.timer = 0.f;
					if (player.state == EObject_State::Attack)
					{
						arrow.direct = EObject_Dir::Right;
						arrow.state = EObject_State::Basic;
						player.state = EObject_State::Basic;
					}
					else if (player.state == EObject_State::Attack_L)
					{
						arrow.direct = EObject_Dir::Left;
						arrow.state = EObject_State::Basic_L;
						player.state = EObject_State::Basic_L;
					}
					break;
				}
			}
		}
	}

	for (ArrowInfo& arrow : inGameData.arrowAttack)
	{
		if (!arrow.is_alive)
			continue;

		int num;
		if (arrow.direct == EObject_Dir::Right)
			num = 1;
		else
			num = -1;

		arrow.pos.x += arrow.velocity.x * elapsed * num;
		arrow.pos.y += arrow.velocity.y * elapsed;

		vector2 arrowSize = vector2(40, 10);
		arrow.box.UpdateCollision(arrow.pos, arrowSize);

		arrow.timer += elapsed;
		if (arrow.timer >= 3.0f)
		{
			arrow.is_alive = false;
		}
	}
}
