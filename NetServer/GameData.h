#pragma once

struct PlayerInfo
{
	int id;
	int x, y;
	short hp;
	short damage;
	char role_type;
	char state;
};

struct MonsterInfo
{
	int id;
	char type;
	int x, y;
	short hp;
	short damage;
	char direct;
	char attack_type;
};

struct TileInfo
{
	short type;
	int x, y;
};

struct ItemInfo
{
	char type;
	int x, y;
	short amount;
};
#define PLAYER_NUM 2
#define MONSTER_NUM 5
#define TILE_NUM 1
#define ITEM_NUM 1

class GameData
{
public:
	PlayerInfo* player[PLAYER_NUM];
	MonsterInfo monster[MONSTER_NUM];
	TileInfo tile[TILE_NUM];
	ItemInfo item[ITEM_NUM];
};