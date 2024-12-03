#pragma once
#include "GameManager.h"

class ItemManager
{
public:
	void Init();
	void CreateItem();
	void CheckItemCollision(GamePlayerInfo& player);

public:
	static ItemManager& GetInst()
	{
		static ItemManager inst;
		return inst;
	}
private:
	InGameData* inGameData;
};

