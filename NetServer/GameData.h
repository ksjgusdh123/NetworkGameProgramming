#pragma once
#include "../Engine/Include/Flag.h"
constexpr int PLAYER_NUM = 2;
constexpr int MONSTER_NUM = 2;
constexpr int MONSTER_ATTACK_NUM = 10;
constexpr int ARROW_NUM = 10;
constexpr int TILE_NUM = 10;
constexpr int ITEM_NUM = 10;
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
			return false;

		if (m_info.RB.y < otherLT.y || m_info.LT.y > otherRB.y)
			return false;

		return true;
	}
public:
	CollisionBoxInfo m_info;

};

struct PlayerInfo {
	int id = -1;
	char name[NAME_LEN] = {};
	EPlayer_Job job = EPlayer_Job::Sword;
	bool bReady = false;

	PlayerInfo() = default;

	PlayerInfo(int playerId, const char* playerName, EPlayer_Job playerJob = EPlayer_Job::Sword)
		: id(playerId), job(playerJob)
	{
		strncpy_s(name, playerName, NAME_LEN);
	}
};

struct LobbyPlayerInfo : public PlayerInfo {
	LobbyPlayerInfo() = default;

	LobbyPlayerInfo(const PlayerInfo& base)
		: PlayerInfo(base) {}
};

struct GamePlayerInfo : public PlayerInfo {
	vector2 pos{ -930.f ,475.f };
	short hp = 100;
	short damage = 10;
	EObject_State state = EObject_State::Basic;
	EObject_Dir dir = EObject_Dir::Right;
	bool isLanded = false;
	bool isJump = false;
	bool isDoubleJump = false;
	bool bReady = false;
	bool bBoss = false;
	float timer = 0.f;
	Collision box;

	vector2 GetPlayerSize()
	{
		if (state == EObject_State::Attack || state == EObject_State::Attack_L)
			return vector2(120, 60);
		else
			return vector2(50, 60);
	}

	GamePlayerInfo() = default;

	GamePlayerInfo(const PlayerInfo& base, vector2 pos, short health, short dmg, EObject_State playerState, EObject_Dir dir)
		: PlayerInfo(base), pos(pos), hp(health), damage(dmg), state(playerState), dir(dir) {}
};

struct ArrowInfo
{
	vector2 pos = vector2(0.f, 0.f);
	vector2 size = vector2(40.f, 10.f);
	EObject_State state = EObject_State::Basic;
	EObject_Dir direct = EObject_Dir::Right;
	vector2 velocity = vector2(400, 10);
	bool is_alive = false;
	float timer = 0.f;
	Collision box;
};

struct MonsterInfo
{
	int id = 0;
	char type = '0';
	vector2 pos = vector2(0.f, 0.f);
	short hp = 100;
	short damage = 10;
	EObject_State state = EObject_State::Basic;
	EObject_Dir direct = EObject_Dir::Right;
	char attack_type;
	float velocity = 50.f;
	vector2 original_pos = vector2(0.f, 0.f);
	bool is_alive = true;
	float timer = 0.f;
	vector2 target;
	vector2 size = vector2(50.f, 50.f);
	Collision box;
};

struct MonsterAttackInfo
{
	vector2 pos = vector2(0.f, 0.f);
	char type = '0';
	vector2 size = vector2(50.f, 50.f);
	EObject_State state = EObject_State::Attack;
	EObject_Dir direct = EObject_Dir::Right;
	float velocity = 10.f;
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

enum ItemType { HEART, STAR, TRAP };
enum ItemId
{
	TRAP1,
	TRAP2,
	TRAP3,
	TRAP4,
	STAR1,
	HEART1,
	HEART2,
	HEART3,
	MAX
};
static vector2 ItemPos[(int)ItemId::MAX] =
{
	{-550.f, 415.f},
	{-40.f, 140.f},
	{570.f, 40.f},
	{570.f, 240.f},
	{-570.f, 415.f},
	{280.f, 415.f},
	{-330.f, 55.f},
	{950.f, 140.f},
};
struct ItemInfo
{
	int id;
	int type;
	vector2 pos;
	vector2 GetSize() const
	{
		const static vector2 size[] = { {60.f, 57.f},{60.f, 57.f},{132.f, 74.f} };
		return size[type];
	}
	short GetEffectAmount() const
	{
		const static short amounts[] = { 30,10,1 };
		return amounts[type];
	}
};

enum SCENE_STATE
{
	LOGINSCENE,
	LOBBYSCENE,
	GAMESCENE,
	BOSSSCENE,
	RESULTSCENE,
};
class GameData {};

struct LobbyData :public GameData
{
	int scene = LOBBYSCENE;
	std::array<LobbyPlayerInfo, PLAYER_NUM> players;
};

struct InGameData :public GameData
{
	int scene = GAMESCENE;
	int playtime = -1;
	std::array<GamePlayerInfo, PLAYER_NUM> players;
	std::array<ArrowInfo, ARROW_NUM> arrowAttack;
	std::array<MonsterInfo, MONSTER_NUM> monster;
	std::array<ItemInfo, ITEM_NUM> item;
	std::array<MonsterAttackInfo, MONSTER_ATTACK_NUM> monsterAttack;
};

struct ResultData : public GameData
{
	int scene = RESULTSCENE;
	float playTime = -1;
	bool bWin = false;
};

#pragma pack(pop)