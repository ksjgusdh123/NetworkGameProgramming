#pragma once

constexpr int PLAYER_NUM = 2;
constexpr int MONSTER_NUM = 5;
constexpr int TILE_NUM = 10;
constexpr int ITEM_NUM = 3;
constexpr int NAME_LEN = 20;

#pragma pack(push, 1)
struct PlayerInfo {
	int id = -1;
	char name[NAME_LEN] = {};
	char job = 0;

	PlayerInfo() = default;

	PlayerInfo(int playerId, const char* playerName, char playerJob = 0)
		: id(playerId), job(playerJob)
	{
		strncpy_s(name, playerName, NAME_LEN);
	}
};

struct LobbyPlayerInfo : public PlayerInfo {
	bool bReady = false;

	LobbyPlayerInfo() = default;

	LobbyPlayerInfo(const PlayerInfo& base, bool ready = false)
		: PlayerInfo(base), bReady(ready) {}
};

struct GamePlayerInfo : public PlayerInfo {
	float x = 0, y = 0;
	short hp = 100;
	short damage = 10;
	char state = 0;

	GamePlayerInfo() = default;

	GamePlayerInfo(const PlayerInfo& base, float posX, float posY, short health, short dmg, char playerState)
		: PlayerInfo(base), x(posX), y(posY), hp(health), damage(dmg), state(playerState) {}
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
class GameData {};

struct LobbyData:public GameData
{
	std::array<LobbyPlayerInfo, PLAYER_NUM> players;
};

struct InGameData :public GameData
{
	std::array<GamePlayerInfo, PLAYER_NUM> players;
	/*std::array<MonsterInfo, MONSTER_NUM> monster;
	std::array<TileInfo, TILE_NUM> tile;
	std::array<ItemInfo, ITEM_NUM> item;*/
};

#pragma pack(pop)