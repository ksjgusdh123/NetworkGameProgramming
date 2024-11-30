#include "GameManager.h"
#include "TCPServer.h"

void GameManager::AddLobbyPlayer(const Client& client)
{
    int i = client.player.id;
    lobbyData.players[i] = LobbyPlayerInfo(client.player);
}

void GameManager::InitGameData()
{

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

void GameManager::CreateTilePacket()
{

	// 타일 번호와 위치 정보를 담을 데이터
	std::vector<int> tileNumbers;
	std::vector<vector2> tilePositions;
	TileInfo info;
	vector2 blockSize = vector2(50, 50);

	// 타일 데이터 수집 (CreateMap에서 정의한 타일 정보를 기반으로)
	float tilePosX = -930.f;
	float tilePosY = 475.f;

	// 첫 발판
	tileNumbers.push_back(1);
	tilePositions.push_back({ tilePosX, tilePosY });
	info.pos = vector2(tilePosX, tilePosY);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);
	tilePosX += 50.f;

	for (int i = 0; i < 20; ++i) {
		tileNumbers.push_back(2);
		tilePositions.push_back({ tilePosX, 475.f });
		info.pos = vector2(tilePosX, tilePosY);
		info.box.UpdateCollision(info.pos, blockSize);
		tiles.push_back(info);
		tilePosX += 50.f;
	}

	tileNumbers.push_back(3);
	tilePositions.push_back({ tilePosX, 475.f });
	info.pos = vector2(tilePosX, tilePosY);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);
	tilePosX += 150.f;

	// 상자 계단
	tileNumbers.push_back(17);
	tilePositions.push_back({ -800.f, 425.f });
	tileNumbers.push_back(17);
	tilePositions.push_back({ -750.f, 425.f });
	tileNumbers.push_back(17);
	tilePositions.push_back({ -750.f, 375.f });
	tileNumbers.push_back(17);
	tilePositions.push_back({ -700.f, 425.f });
	tileNumbers.push_back(17);
	tilePositions.push_back({ -700.f, 375.f });
	tileNumbers.push_back(17);
	tilePositions.push_back({ -700.f, 325.f });

	// 상자 계단2
	tileNumbers.push_back(17);
	tilePositions.push_back({ -550.f, 425.f });
	tileNumbers.push_back(17);
	tilePositions.push_back({ -500.f, 425.f });
	tileNumbers.push_back(17);
	tilePositions.push_back({ -500.f, 375.f });
	tileNumbers.push_back(17);
	tilePositions.push_back({ -500.f, 325.f });
	tileNumbers.push_back(17);
	tilePositions.push_back({ -450.f, 425.f });
	tileNumbers.push_back(17);
	tilePositions.push_back({ -450.f, 375.f });
	tileNumbers.push_back(17);
	tilePositions.push_back({ -400.f, 425.f });

	// 중간 발판
	tileNumbers.push_back(14);
	tilePositions.push_back({ -380.f, 250.f });
	info.pos = vector2(-380.f, 250.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);
	tileNumbers.push_back(15);
	tilePositions.push_back({ -330.f, 250.f });
	info.pos = vector2(-330.f, 250.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);
	tileNumbers.push_back(16);
	tilePositions.push_back({ -280.f, 250.f });
	info.pos = vector2(-280.f, 250.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);

	tileNumbers.push_back(14);
	tilePositions.push_back({ -130.f, 200.f });
	info.pos = vector2(-130.f, 200.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);
	tileNumbers.push_back(15);
	tilePositions.push_back({ -80.f, 200.f });
	info.pos = vector2(-80.f, 200.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);
	tileNumbers.push_back(16);
	tilePositions.push_back({ -30.f, 200.f });
	info.pos = vector2(-30.f, 200.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);

	tileNumbers.push_back(14);
	tilePositions.push_back({ 70.f, 170.f });
	info.pos = vector2(70.f, 170.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);
	tileNumbers.push_back(15);
	tilePositions.push_back({ 120.f, 170.f });
	info.pos = vector2(120.f, 170.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);
	tileNumbers.push_back(16);
	tilePositions.push_back({ 170.f, 170.f });
	info.pos = vector2(170.f, 170.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);

	tileNumbers.push_back(14);
	tilePositions.push_back({ -380.f, 100.f });
	info.pos = vector2(-380.f, 100.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);
	tileNumbers.push_back(15);
	tilePositions.push_back({ -330.f, 100.f });
	info.pos = vector2(-330.f, 100.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);
	tileNumbers.push_back(16);
	tilePositions.push_back({ -280.f, 100.f });
	info.pos = vector2(-280.f, 100.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);

	// 근접 몬스터 가두는 상자
	tileNumbers.push_back(17);
	tilePositions.push_back({ -250.f, 425.f });
	info.pos = vector2(-250.f, 425.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);
	tileNumbers.push_back(17);
	tilePositions.push_back({ 50.f, 425.f });
	info.pos = vector2(50.f, 425.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);

	// 두 번째 발판
	tileNumbers.push_back(1);
	tilePositions.push_back({ tilePosX, 475.f });
	info.pos = vector2(tilePosX, 475.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);

	tilePosX += 50.f;

	for (int i = 0; i < 6; ++i) {
		tileNumbers.push_back(2);
		tilePositions.push_back({ tilePosX, 475.f });
		info.pos = vector2(tilePosX, 475.f);
		info.box.UpdateCollision(info.pos, blockSize);
		tiles.push_back(info);
		tilePosX += 50.f;
	}

	tileNumbers.push_back(3);
	tilePositions.push_back({ tilePosX, 475.f });
	info.pos = vector2(tilePosX, 475.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);
	tilePosX += 250.f;

	// 점프맵 발판
	tileNumbers.push_back(14);
	tilePositions.push_back({ tilePosX, 400.f });
	info.pos = vector2(tilePosX, 400.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);
	tileNumbers.push_back(14);
	tilePositions.push_back({ tilePosX, 200.f });
	info.pos = vector2(tilePosX, 200.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);
	tilePosX += 50.f;

	tileNumbers.push_back(15);
	tilePositions.push_back({ tilePosX, 400.f });
	info.pos = vector2(tilePosX, 400.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);
	tileNumbers.push_back(15);
	tilePositions.push_back({ tilePosX, 200.f });
	info.pos = vector2(tilePosX, 200.f);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);
	tilePosX += 50.f;

	tileNumbers.push_back(16);
	tilePositions.push_back({ tilePosX, 400.f });
	tileNumbers.push_back(16);
	tilePositions.push_back({ tilePosX, 200.f });

	tilePosX -= 400.f;

	tileNumbers.push_back(14);
	tilePositions.push_back({ tilePosX, 300.f });
	tileNumbers.push_back(14);
	tilePositions.push_back({ tilePosX, 100.f });
	tilePosX += 50.f;

	tileNumbers.push_back(15);
	tilePositions.push_back({ tilePosX, 300.f });
	tileNumbers.push_back(15);
	tilePositions.push_back({ tilePosX, 100.f });
	tilePosX += 50.f;

	tileNumbers.push_back(16);
	tilePositions.push_back({ tilePosX, 300.f });
	tileNumbers.push_back(16);
	tilePositions.push_back({ tilePosX, 100.f });

	tilePosX -= 400.f;

	tileNumbers.push_back(14);
	tilePositions.push_back({ tilePosX, 0.f });
	tilePosX += 50.f;

	tileNumbers.push_back(15);
	tilePositions.push_back({ tilePosX, 0.f });
	tilePosX += 50.f;

	tileNumbers.push_back(16);
	tilePositions.push_back({ tilePosX, 0.f });

	tilePosX += 200.f;

	// 포탈 발판
	tileNumbers.push_back(14);
	tilePositions.push_back({ tilePosX, -100.f });
	tilePosX += 50.f;

	for (int i = 0; i < 4; ++i) {
		tileNumbers.push_back(15);
		tilePositions.push_back({ tilePosX, -100.f });
		tilePosX += 50.f;
	}

	tileNumbers.push_back(16);
	tilePositions.push_back({ tilePosX, -100.f });

	S_TilesPkt packet((int)tileNumbers.size(), tileNumbers, tilePositions);
	TCPServer::GetInst()->SendPacket(packet);

}

bool GameManager::CollisionCheck(int clientID)
{
	Collision box;
	vector2 size = vector2(50, 60);
	box.UpdateCollision(inGameData.players[clientID].pos, size);
	for (TileInfo& tile : tiles)
	{
		vector2 playerPos = inGameData.players[clientID].pos;
		vector2 playerSize = size; // size는 플레이어 크기 (50, 60)
		vector2 boxLT = tile.box.m_info.LT; // 타일 박스 왼쪽 위
		vector2 boxRB = tile.box.m_info.RB; // 타일 박스 오른쪽 아래

		// 플레이어의 경계 계산
		float playerLeft = playerPos.x - playerSize.x / 2;
		float playerRight = playerPos.x + playerSize.x / 2;
		float playerTop = playerPos.y - playerSize.y / 2;
		float playerBottom = playerPos.y + playerSize.y / 2;

		// 여유 거리 설정
		const float offset = 0.0f; // 2 픽셀 정도 여유를 둠

		// 박스와 플레이어의 충돌 검사
		if (playerRight > boxLT.x && playerLeft < boxRB.x &&
			playerBottom > boxLT.y && playerTop < boxRB.y) {
			// 충돌 발생 시 위치 조정
			float overlapLeft = playerRight - boxLT.x;   // 왼쪽 겹침
			float overlapRight = boxRB.x - playerLeft;   // 오른쪽 겹침
			float overlapTop = playerBottom - boxLT.y;  // 위쪽 겹침
			float overlapBottom = boxRB.y - playerTop;  // 아래쪽 겹침

			// 가장 작은 겹침을 기준으로 밀어냄 (여유 거리 추가)
			if (overlapLeft < overlapRight && overlapLeft < overlapTop && overlapLeft < overlapBottom) {
				inGameData.players[clientID].pos.x -= (overlapLeft + offset); // 왼쪽으로 밀어냄
			}
			else if (overlapRight < overlapLeft && overlapRight < overlapTop && overlapRight < overlapBottom) {
				inGameData.players[clientID].pos.x += (overlapRight + offset); // 오른쪽으로 밀어냄
			}
			else if (overlapTop < overlapLeft && overlapTop < overlapRight && overlapTop < overlapBottom) {
				inGameData.players[clientID].pos.y -= (overlapTop + offset); // 위쪽으로 밀어냄
				if (inGameData.players[clientID].state == 12)
				{
					inGameData.players[clientID].state = 1;
				}
				else if (inGameData.players[clientID].state == 11)
				{
					inGameData.players[clientID].state = 0;
				}
			}
			else {
				inGameData.players[clientID].pos.y += (overlapBottom + offset); // 아래쪽으로 밀어냄
			}
			return true;
		}
	}
	return false;
}

void GameManager::ServerUpdate(int clientID)
{
	if (!CollisionCheck(clientID))
	{
		//if(inGameData->players[clientID].dir == 0)
		//	inGameData->players[clientID].state = 11;
		//else
		//	inGameData->players[clientID].state = 12;
	}
}