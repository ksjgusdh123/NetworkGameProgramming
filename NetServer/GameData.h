#pragma once
#include "../Engine/Include/Flag.h"
constexpr int PLAYER_NUM = 2;
constexpr int MONSTER_NUM = 2;
constexpr int TILE_NUM = 10;
constexpr int ITEM_NUM = 3;
constexpr int NAME_LEN = 20;

#pragma pack(push, 1)
struct vector2
{
	float x = 0.f, y = 0.f;

	vector2() { x = 0.f; y = 0.f; }
	vector2(float _x, float _y) { x = _x; y = _y; }
	vector2(const vector2& v) { x = v.x; y = v.y; }

	vector2 operator+ (const vector2& v) const { return vector2(x + v.x, y + v.y); }
	vector2 operator- (const vector2& v) const { return vector2(x - v.x, y - v.y); }
	vector2 operator* (const vector2& v) const { return vector2(x * v.x, y * v.y); }
};

struct CollisionBoxInfo
{
	vector2	LT;
	vector2	RB;
};

class Collision
{
public:
	void UpdateCollision(vector2& pos, vector2& size)
	{
		m_info.LT.x = pos.x - size.x / 2.f;
		m_info.LT.y = pos.y - size.y / 2.f;
		m_info.RB.x = pos.x + size.x / 2.f;
		m_info.RB.y = pos.y + size.y / 2.f;
	}

	bool CheckCollision(Collision* box)
	{
		vector2 otherLT = box->m_info.LT;
		vector2 otherRB = box->m_info.RB;

		if (m_info.RB.x < otherLT.x || m_info.LT.x > otherRB.x)
			return false; // X축에서 겹치지 않음

		if (m_info.RB.y < otherLT.y || m_info.LT.y > otherRB.y)
			return false; // Y축에서 겹치지 않음

		return true;
	}
public:
	CollisionBoxInfo m_info;

};

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
	vector2 prev_pos;
	vector2 pos{ -930.f ,475.f };
	short hp = 100;
	short damage = 10;
	char state = 0;
	char dir = 0;
	bool isLanded = false;
	bool isJump = false;
	bool isDoubleJump = false;
	bool bReady = false;

	GamePlayerInfo() = default;

	GamePlayerInfo(const PlayerInfo& base, vector2 pos, short health, short dmg, char playerState, char dir)
		: PlayerInfo(base), pos(pos), hp(health), damage(dmg), state(playerState), dir(dir) {}
};

struct MonsterInfo
{
	int id = 0;
	char type = '0';
	vector2 pos = vector2(0.f,0.f);
	short hp = 100;
	short damage = 10;
	EObject_State state = EObject_State::Basic;
	EObject_Dir direct = EObject_Dir::Right;
	char attack_type;
	float velocity = 50.f;
	vector2 original_pos= vector2(0.f,0.f);
	bool is_alive = true;
	float timer = 0.f;
	vector2 target;
};

struct TileInfo
{
	short type;
	vector2 pos;
	Collision box;
};

struct ItemInfo
{
	char type;
	vector2 pos;
	short amount;
};

enum SCENE_STATE
{
	LOGINSCENE,
	LOBBYSCENE,
	GAMESCENE,
	BOSSSCENE,
};
class GameData {};

struct LobbyData:public GameData
{
	int scene = LOBBYSCENE;
	std::array<LobbyPlayerInfo, PLAYER_NUM> players;
};

struct InGameData :public GameData
{
	int scene = GAMESCENE;
	int playtime = -1;
	std::array<GamePlayerInfo, PLAYER_NUM> players;
	std::array<MonsterInfo, MONSTER_NUM> monster;
	//std::array<ItemInfo, ITEM_NUM> item;
};

#pragma pack(pop)