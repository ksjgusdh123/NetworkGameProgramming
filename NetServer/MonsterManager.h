#pragma once
#include "GameManager.h"

class MonsterManager
{
public:
	void Init();
	void InitBossData();
	void CreateMonster();
	void CreateBossMonster();
	void UpdateMonster();
	int IsPlayerInRicheAttackArea();
public:
	static MonsterManager& GetInst()
	{
		static MonsterManager inst;
		return inst;
	}
private:
	InGameData* inGameData;
};

