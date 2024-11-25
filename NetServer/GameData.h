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
