#include "GameManager.h"
#include "TCPServer.h"

void GameManager::AddLobbyPlayer(const Client& client)
{
    int i = client.player.id;
    lobbyData.players[i] = LobbyPlayerInfo(client.player);
}

void GameManager::InitBossData()
{
	CreateBossTile();
	CreateBossMonster();
}

void GameManager::InitGameData()
{
	CreateTile();
	CreateMonster();
	gameTimer.Start();
}

void GameManager::UpdateInGameData()
{
	inGameData.playtime = gameTimer.GetElapsedTime();
	UpdateMonster();
	PortalCollision();
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
            << ", Name=" <<player.name
            << ", Pos=(" << player.pos.x << "," << player.pos.y << ")"
            << ", HP=" << player.hp
            << ", Job=" << player.job << "\n";
    }
}

void GameManager::CreateTile()
{
	// Ÿ�� ��ȣ�� ��ġ ������ ���� ������
	//std::vector<int> tileNumbers;
	//std::vector<vector2> tilePositions;
	TileInfo info;
	vector2 blockSize = vector2(50, 50);

	// Ÿ�� ������ ���� (CreateMap���� ������ Ÿ�� ������ �������)
	float tilePosX = -930.f;
	float tilePosY = 475.f;

	// ù ����
	AddTile(info, blockSize, 1, tilePosX, tilePosY);

	tilePosX += 50.f;

	for (int i = 0; i < 20; ++i) {
		AddTile(info, blockSize, 2, tilePosX, tilePosY);
		tilePosX += 50.f;
	}

	AddTile(info, blockSize, 3, tilePosX, tilePosY);
	tilePosX += 150.f;

	// ���� ���
	AddTile(info, blockSize, 17, -800.f, 425.f);
	AddTile(info, blockSize, 17, -750.f, 425.f);
	AddTile(info, blockSize, 17, -750.f, 375.f);
	AddTile(info, blockSize, 17, -700.f, 425.f);
	AddTile(info, blockSize, 17, -700.f, 375.f);
	AddTile(info, blockSize, 17, -700.f, 325.f);

	// ���� ���2
	AddTile(info, blockSize, 17, -550.f, 425.f);
	AddTile(info, blockSize, 17, -500.f, 425.f);
	AddTile(info, blockSize, 17, -500.f, 375.f);
	AddTile(info, blockSize, 17, -500.f, 325.f);
	AddTile(info, blockSize, 17, -450.f, 425.f);
	AddTile(info, blockSize, 17, -450.f, 375.f);
	AddTile(info, blockSize, 17, -400.f, 425.f);

	// �߰� ����
	AddTile(info, blockSize, 14, -380.f, 250.f);
	AddTile(info, blockSize, 15, -330.f, 250.f);
	AddTile(info, blockSize, 16, -280.f, 250.f);


	AddTile(info, blockSize, 14, -130.f, 200.f);
	AddTile(info, blockSize, 15, -80.f, 200.f);
	AddTile(info, blockSize, 16, -30.f, 200.f);

	AddTile(info, blockSize, 14, 70.f, 170.f);
	AddTile(info, blockSize, 15, 120.f, 170.f);
	AddTile(info, blockSize, 16, 170.f, 170.f);

	AddTile(info, blockSize, 14, -380.f, 100.f);
	AddTile(info, blockSize, 15, -330.f, 100.f);
	AddTile(info, blockSize, 16, -280.f, 100.f);

	// ���� ���� ���δ� ����
	AddTile(info, blockSize, 17, -250.f, 425.f);
	AddTile(info, blockSize, 17, 50.f, 425.f);

	// �� ��° ����
	AddTile(info, blockSize, 1, tilePosX, 475.f);

	tilePosX += 50.f;

	for (int i = 0; i < 6; ++i) {
		AddTile(info, blockSize, 2, tilePosX, 475.f);
		tilePosX += 50.f;
	}

	AddTile(info, blockSize, 3, tilePosX, 475.f);
	tilePosX += 250.f;

	// ������ ����
	AddTile(info, blockSize, 14, tilePosX, 400.f);
	AddTile(info, blockSize, 14, tilePosX, 200.f);
	tilePosX += 50.f;

	AddTile(info, blockSize, 15, tilePosX, 400.f);
	AddTile(info, blockSize, 15, tilePosX, 200.f);
	tilePosX += 50.f;

	AddTile(info, blockSize, 16, tilePosX, 400.f);
	AddTile(info, blockSize, 16, tilePosX, 200.f);
	tilePosX -= 400.f;

	AddTile(info, blockSize, 14, tilePosX, 300.f);
	AddTile(info, blockSize, 14, tilePosX, 100.f);
	tilePosX += 50.f;

	AddTile(info, blockSize, 15, tilePosX, 300.f);
	AddTile(info, blockSize, 15, tilePosX, 100.f);
	tilePosX += 50.f;

	AddTile(info, blockSize, 16, tilePosX, 300.f);
	AddTile(info, blockSize, 16, tilePosX, 100.f);
	tilePosX -= 400.f;

	AddTile(info, blockSize, 14, tilePosX, 0.f);
	tilePosX += 50.f;

	AddTile(info, blockSize, 15, tilePosX, 0.f);
	tilePosX += 50.f;

	AddTile(info, blockSize, 16, tilePosX, 0.f);
	tilePosX += 200.f;

	// ��Ż ����
	AddTile(info, blockSize, 14, tilePosX, -100.f);
	tilePosX += 50.f;

	for (int i = 0; i < 4; ++i) {
		AddTile(info, blockSize, 15, tilePosX, -100.f);
		tilePosX += 50.f;
	}

	AddTile(info, blockSize, 16, tilePosX, -100.f);

}

void GameManager::CreateMonster()
{
	MonsterInfo info;
	info.type = '0';
	info.pos = vector2(-100.f, 410.f);
	info.original_pos = info.pos;
	info.hp = 100;
	info.direct = EObject_Dir::Right;
	info.state = EObject_State::Walk;
	info.is_alive = true;
	info.velocity = 50.f;
	inGameData.monster[0] = info;

	info.type = '1';
	info.pos = vector2(120.f, 80.f);
	info.original_pos = info.pos;
	info.hp = 100;
	info.direct = EObject_Dir::Right;
	info.state = EObject_State::Basic;
	info.is_alive = true;
	info.timer = 0.f;
	inGameData.monster[1] = info;

}

void GameManager::CreateBossTile()
{
	tiles.clear();
	tileNumbers.clear();
	tilePositions.clear();

	TileInfo info;
	vector2 blockSize = vector2(50, 50);
	
	float tilePosX = -930.f;
	float tilePosY = 475.f;

	AddTile(info, blockSize, 1, tilePosX, tilePosY);

	tilePosX += 50.f;

	for (int i = 0; i < 37; ++i) {
		AddTile(info, blockSize, 2, tilePosX, tilePosY);
		tilePosX += 50.f;
	}

	AddTile(info, blockSize, 3, tilePosX, tilePosY);
	tilePosX += 150.f;
}

void GameManager::CreateBossMonster()
{
	MonsterInfo info;
	info.type = '2';
	info.pos = vector2(-100.f, 410.f);
	info.original_pos = info.pos;
	info.hp = 100;
	info.direct = EObject_Dir::Right;
	info.state = EObject_State::Walk;
	info.is_alive = true;
	info.velocity = 50.f;
	inGameData.monster[0] = info;
}

void GameManager::AddTile(TileInfo& info, vector2 blockSize, int type, int x, int y)
{
	tileNumbers.push_back(type);
	tilePositions.push_back(vector2(x, y));
	info.pos = vector2(x, y);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);
}

void GameManager::SendTilePacket()
{
	S_TilesPkt packet((int)tileNumbers.size(), tileNumbers, tilePositions);
	TCPServer::GetInst()->SendPacket(packet);
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

bool GameManager::CollisionCheck(GamePlayerInfo& player)
{
	Collision box;
	vector2 size = vector2(50, 60);
	box.UpdateCollision(player.pos, size);

	vector2 playerPos = player.pos;
	vector2 playerSize = size; 
	float playerLeft = playerPos.x - playerSize.x / 2;
	float playerRight = playerPos.x + playerSize.x / 2;
	float playerTop = playerPos.y - playerSize.y / 2;
	float playerBottom = playerPos.y + playerSize.y / 2;
	bool bCheck = false;
	for (TileInfo& tile : tiles)
	{
		
		vector2 boxLT = tile.box.m_info.LT; // Ÿ�� �ڽ� ���� ��
		vector2 boxRB = tile.box.m_info.RB; // Ÿ�� �ڽ� ������ �Ʒ�

		

		// ���� �Ÿ� ����
		const float offset = 2.0f; // 2 �ȼ� ���� ������ ��

		// �ڽ��� �÷��̾��� �浹 �˻�
		if (playerRight > boxLT.x && playerLeft < boxRB.x &&
			playerBottom > boxLT.y && playerTop < boxRB.y) {
			// �浹 �߻� �� ��ġ ����
			float overlapLeft = playerRight - boxLT.x;   // ���� ��ħ
			float overlapRight = boxRB.x - playerLeft;   // ������ ��ħ
			float overlapTop = playerBottom - boxLT.y;  // ���� ��ħ
			float overlapBottom = boxRB.y - playerTop;  // �Ʒ��� ��ħ

			// ���� ���� ��ħ�� �������� �о (���� �Ÿ� �߰�)
			if (overlapLeft < overlapRight && overlapLeft < overlapTop && overlapLeft < overlapBottom) {
				player.pos.x -= (overlapLeft); // �������� �о
			}
			else if (overlapRight < overlapLeft && overlapRight < overlapTop && overlapRight < overlapBottom) {
				player.pos.x += (overlapRight); // ���������� �о
			}
			else if (overlapTop <= overlapLeft && overlapTop <= overlapRight && overlapTop <= overlapBottom) {
				player.pos.y -= (overlapTop - offset); // �������� �о
				player.isLanded = true;
				player.isJump = false;
				player.isDoubleJump = false;
				if (player.state == 12 || player.state == 10)
				{
					player.state = 1;
				}
				else if (player.state == 11 || player.state == 9)
				{
					player.state = 0;
				}
				bCheck = true;
			}
			else {
				player.pos.y += (overlapBottom + offset); // �Ʒ������� �о
				if (player.dir == 1)
				{
					player.state = 12;
				}
				else if (player.dir == 0)
				{
					player.state = 11;
				}
			}
		}
	}

	if (bCheck)
		return true;

	if (player.state == 2)
	{
		player.isLanded = false;
	}
	if (player.state == 3)
	{
		player.isLanded = false;
	}
	return false;
}

void GameManager::ProcessCollsion()
{
	for(auto& player: inGameData.players)
	{
		if (!CollisionCheck(player))
		{
			//if(inGameData->players[clientID].dir == 0)
			//	inGameData->players[clientID].state = 11;
			//else
			//	inGameData->players[clientID].state = 12;
		}
	}
}

int GameManager::IsPlayerInRicheAttackArea()
{
	EObject_State riche_state = inGameData.monster[1].state;
	if (riche_state == EObject_State::Attack_L || riche_state == EObject_State::Attack) return -1;

	for (int i = 0; i < PLAYER_NUM; ++i) {
		float dx = inGameData.players[i].pos.x - inGameData.monster[1].pos.x;
		float dy = inGameData.players[i].pos.y - inGameData.monster[1].pos.y;
		float distance = sqrt(dx * dx + dy * dy);
		if (distance < 400) return i;
	}

	return -1;
}


void GameManager::UpdateMonster()
{
	


	for (MonsterInfo& m : inGameData.monster)
	{
		if (!m.is_alive) 
			continue;

		switch (m.type) {
		case '0':
		{
			float range = 100.f;

			if (m.direct == EObject_Dir::Right && m.pos.x >= m.original_pos.x + range)
			{
				m.direct = EObject_Dir::Left;
				m.state = EObject_State::Walk_L;
				m.velocity = -50.f;
			}
			else if (m.direct == EObject_Dir::Left && m.pos.x <= m.original_pos.x - range)
			{
				m.direct = EObject_Dir::Right;
				m.state = EObject_State::Walk;
				m.velocity = 50.f;
			}

			m.pos.x += m.velocity * 0.1f;
		}
		break;
		case '1':
		{
			int targetNum = IsPlayerInRicheAttackArea();
			if (targetNum != -1) {
				GamePlayerInfo p = inGameData.players[targetNum];

				if (p.pos.x >= m.pos.x) {
					m.state = EObject_State::Attack;
					m.direct = EObject_Dir::Right;
				}
				else {
					m.state = EObject_State::Attack_L;
					m.direct = EObject_Dir::Left;
				}
				m.target = p.pos;
				m.timer = 0.f;
			}

			if (m.state == EObject_State::Attack || m.state == EObject_State::Attack_L) {
				m.timer += 0.05f;
				
				if (m.timer >= 1.f) {
					if (m.state == EObject_State::Attack)
						m.state = EObject_State::Basic;
					if (m.state == EObject_State::Attack_L)
						m.state = EObject_State::Basic_L;

					//CRicheAttack* ra = m_scene->CreateObject<CRicheAttack>("riche_attack");
					//ra->SetPos(m_pos);
					//ra->SetTarget(m_target);
				}
			}
		}
			break;
		default:

			break;
		}
	}
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