#include "ItemManager.h"

void ItemManager::Init()
{
	inGameData = GameManager::GetInst().GetInGameData();
}

void ItemManager::CreateItem()
{
	if (!inGameData)
	{
		cout << "inGameData is null!\n";
		return;
	}

	inGameData->item.fill({});

	for (int i = 0; i < (int)ItemId::MAX; ++i)
	{
		int type;
		if (i< STAR1)
			type = ItemType::TRAP;
		else if (i< HEART1)
			type = ItemType::STAR_H;
		else
			type = ItemType::HEART;

		inGameData->item[i] = { i, type, ItemPos[i] };
	}
}

void ItemManager::CheckItemCollision(GamePlayerInfo& player)
{
	Collision playerCollision;
	vector2 playerSize = player.GetPlayerSize(); // 플레이어 크기
	playerCollision.UpdateCollision(player.pos, playerSize);

	for (auto& item : inGameData->item)
	{
		if (item.type == -1) continue; // 비활성화된 아이템 건너뛰기

		Collision itemCollision;
		vector2 itemSize = item.GetSize();
		itemCollision.UpdateCollision(item.pos, itemSize);

		if (playerCollision.CheckCollision(&itemCollision))
		{
			short effectAmount = item.GetEffectAmount();
			switch (item.type)
			{
			case ItemType::HEART:
				player.hp += effectAmount;
				player.hp = std::clamp((int)player.hp, 0, 100);
				item.type = -1;
				break;
			case ItemType::STAR:
				player.damage += effectAmount;
				item.type = -1;
				break;
			case ItemType::TRAP:
				if (player.job == EPlayer_Job::Sword &&
					(player.state == EObject_State::Attack || player.state == EObject_State::Attack_L))
				{
					inGameData->item[item.id + STAR1].type = ItemType::STAR;
					item.type = -1;
				}
				else
				{
					player.hp -= effectAmount;
					player.hp = std::clamp((int)player.hp, 0, 100);
				}
				break;
			default:
				break;
			}
		}
	}
}

bool ItemManager::CheckArrowItemCollision(ArrowInfo& arrow)
{
	for (auto& item : inGameData->item)
	{
		if (item.type != ItemType::TRAP)
			continue;

		Collision itemCollision;
		vector2 itemSize = item.GetSize();
		itemCollision.UpdateCollision(item.pos, itemSize);

		if (itemCollision.CheckCollision(&arrow.box))
		{
			item.type = -1;
			return true;
		}
	}
	return false;
}
