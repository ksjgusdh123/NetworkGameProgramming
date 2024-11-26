#pragma once

constexpr int PLAYER_NUM = 2;
constexpr int MONSTER_NUM = 5;
constexpr int TILE_NUM = 10;
constexpr int ITEM_NUM = 3;

struct PlayerInfo {
    int id = -1;
    std::string name;
    char job = 0;

    PlayerInfo() = default;

    PlayerInfo(int playerId, const std::string& playerName ="", char playerJob = 0)
        : id(playerId), name(playerName), job(playerJob) {}
};

struct LobbyPlayerInfo : public PlayerInfo {
    bool bReady = false;

    LobbyPlayerInfo() = default;

    LobbyPlayerInfo(const PlayerInfo& base, bool ready = false)
        : PlayerInfo(base), bReady(ready) {}
};

struct GamePlayerInfo : public PlayerInfo {
    int x = 0, y = 0;
    short hp = 100;
    short damage = 10;
    char state = 0;

    GamePlayerInfo() = default;

    GamePlayerInfo(const PlayerInfo& base, int posX, int posY, short health, short dmg, char playerState)
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
