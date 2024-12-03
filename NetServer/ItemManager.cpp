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

    inGameData->item[0] = { ItemType::TRAP, vector2(-570.f, 415.f) };
    inGameData->item[1] = { ItemType::TRAP, vector2(-40.f, 140.f) };
    inGameData->item[2] = { ItemType::TRAP, vector2(600.f, 40.f) };
    inGameData->item[3] = { ItemType::TRAP, vector2(600.f, 240.f) };

    inGameData->item[4] = { ItemType::STAR, vector2(-570.f, 415.f) };

    inGameData->item[5] = { ItemType::HEART, vector2(280.f, 415.f) };
}

void ItemManager::CheckItemCollision(GamePlayerInfo& player)
{
    Collision playerCollision;
    vector2 playerSize = vector2(50, 60); // 플레이어 크기
    playerCollision.UpdateCollision(player.pos, playerSize);

    for (auto& item : inGameData->item)
    {
        if (item.type == -1) continue; // 비활성화된 아이템 건너뛰기

        Collision itemCollision;
        vector2 itemSize = vector2(30, 30); // 아이템 크기 (예시)
        itemCollision.UpdateCollision(item.pos, itemSize);

        if (playerCollision.CheckCollision(&itemCollision))
        {
            short effectAmount = item.GetEffectAmount();
            switch (item.type)
            {
            case ItemType::HEART:
                player.hp += effectAmount;
                break;
            case ItemType::STAR:
                player.damage += effectAmount;
                break;
            case ItemType::TRAP:
                player.hp -= effectAmount;
                break;
            default:
                break;
            }

            // 아이템 비활성화
            item.type = -1;
        }
    }
}