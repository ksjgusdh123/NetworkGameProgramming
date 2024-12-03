#include "TileManager.h"
#include "TCPServer.h"

void TileManager::CreateTile()
{
	TileInfo info;
	vector2 blockSize = vector2(50, 50);

	float tilePosX = -930.f;
	float tilePosY = 475.f;

	// 첫 발판
	AddTile(info, blockSize, 1, tilePosX, tilePosY);

	tilePosX += 50.f;

	for (int i = 0; i < 20; ++i) {
		AddTile(info, blockSize, 2, tilePosX, tilePosY);
		tilePosX += 50.f;
	}

	AddTile(info, blockSize, 3, tilePosX, tilePosY);
	tilePosX += 150.f;

	// 상자 계단
	AddTile(info, blockSize, 17, -800.f, 425.f);
	AddTile(info, blockSize, 17, -750.f, 425.f);
	AddTile(info, blockSize, 17, -750.f, 375.f);
	AddTile(info, blockSize, 17, -700.f, 425.f);
	AddTile(info, blockSize, 17, -700.f, 375.f);
	AddTile(info, blockSize, 17, -700.f, 325.f);

	// 상자 계단2
	AddTile(info, blockSize, 17, -550.f, 425.f);
	AddTile(info, blockSize, 17, -500.f, 425.f);
	AddTile(info, blockSize, 17, -500.f, 375.f);
	AddTile(info, blockSize, 17, -500.f, 325.f);
	AddTile(info, blockSize, 17, -450.f, 425.f);
	AddTile(info, blockSize, 17, -450.f, 375.f);
	AddTile(info, blockSize, 17, -400.f, 425.f);

	// 중간 발판
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

	// 근접 몬스터 가두는 상자
	AddTile(info, blockSize, 17, -250.f, 425.f);
	AddTile(info, blockSize, 17, 50.f, 425.f);

	// 두 번째 발판
	AddTile(info, blockSize, 1, tilePosX, 475.f);

	tilePosX += 50.f;

	for (int i = 0; i < 6; ++i) {
		AddTile(info, blockSize, 2, tilePosX, 475.f);
		tilePosX += 50.f;
	}

	AddTile(info, blockSize, 3, tilePosX, 475.f);
	tilePosX += 250.f;

	// 점프맵 발판
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

	// 포탈 발판
	AddTile(info, blockSize, 14, tilePosX, -100.f);
	tilePosX += 50.f;

	for (int i = 0; i < 4; ++i) {
		AddTile(info, blockSize, 15, tilePosX, -100.f);
		tilePosX += 50.f;
	}

	AddTile(info, blockSize, 16, tilePosX, -100.f);

}
void TileManager::CreateBossTile()
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

void TileManager::AddTile(TileInfo& info, vector2 blockSize, int type, int x, int y)
{
	tileNumbers.push_back(type);
	tilePositions.push_back(vector2(x, y));
	info.pos = vector2(x, y);
	info.box.UpdateCollision(info.pos, blockSize);
	tiles.push_back(info);
}

bool TileManager::CheckTileCollision(GamePlayerInfo& player)
{
	Collision box;
	vector2 size = vector2(50, 60);
	box.UpdateCollision(player.pos, size);
	bool bCheck = false;

	for (TileInfo& tile : tiles)
	{
		vector2 playerPos = player.pos;
		vector2 playerSize = size; // size는 플레이어 크기 (50, 60)
		vector2 boxLT = tile.box.m_info.LT; // 타일 박스 왼쪽 위
		vector2 boxRB = tile.box.m_info.RB; // 타일 박스 오른쪽 아래

		// 플레이어의 경계 계산
		float playerLeft = playerPos.x - playerSize.x / 2;
		float playerRight = playerPos.x + playerSize.x / 2;
		float playerTop = playerPos.y - playerSize.y / 2;
		float playerBottom = playerPos.y + playerSize.y / 2;

		// 여유 거리 설정
		const float offset = 2.0f; // 2 픽셀 정도 여유를 둠

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
				player.pos.x -= (overlapLeft); // 왼쪽으로 밀어냄
			}
			else if (overlapRight < overlapLeft && overlapRight < overlapTop && overlapRight < overlapBottom) {
				player.pos.x += (overlapRight); // 오른쪽으로 밀어냄
			}
			else if (overlapTop <= overlapLeft && overlapTop <= overlapRight && overlapTop <= overlapBottom) {
				player.pos.y -= (overlapTop - offset); // 위쪽으로 밀어냄
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
				player.pos.y += (overlapBottom + offset); // 아래쪽으로 밀어냄
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

void TileManager::SendTilePacket()
{
	S_TilesPkt packet((int)tileNumbers.size(), tileNumbers, tilePositions);
	TCPServer::GetInst()->SendPacket(packet);
}
