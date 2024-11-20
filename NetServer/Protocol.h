#pragma once

enum PacketType
{
	LOGIN,
	LOGIN_SUCCESS,
	LOGIN_FAIL,
	MOVE,
};

class Packet
{
public:
	int len = 0;
	int type = 0;
	char data[BUFSIZ]{};
	Packet() {}
	Packet(int len_, int type_, const char* data_) : len(len_), type(type_)
	{
		memcpy(data, data_, len_);
		data[len] = '\0';
	}
	Packet(int type_, const std::string& message)
	{
		type = type_;
		snprintf(data, sizeof(data), "%s", message.c_str());
		len = strlen(data);
	}
};

struct C_PlayerMovePkt :public Packet
{
	int player_id = -1;
	float x = -1;
	float y = -1;

	C_PlayerMovePkt(int player_id_, float x_, float y_)
		: player_id(player_id_), x(x_), y(y_)
	{
		type = MOVE;
		sprintf_s(data, "%d %f %f", player_id, x, y);
		len = strlen(data);
	}
	void deserialize()
	{
		sscanf_s(data, "%d %f %f", &player_id, &x, &y);
	}
};


struct C_LoginRequestPkt : public Packet
{
	int player_id = -1;
	std::string player_name;

	C_LoginRequestPkt(int player_id_, const std::string& player_name_)
		: player_id(player_id_), player_name(player_name_)
	{
		type = LOGIN;
		sprintf_s(data, "%d %s", player_id, player_name.c_str());
		len = strlen(data);
	}

	void deserialize()
	{
		char name_buffer[BUFSIZ];
		sscanf_s(data, "%d %s", &player_id, name_buffer, sizeof(name_buffer));
		player_name = name_buffer;
	}
};

//
//struct C_GameStartRequestPkt
//{
//	PacketHeader header;
//};
//

//
//struct C_PlayerAttackPkt
//{
//	PacketHeader header;
//};
//
//struct S_RoomDataPkt
//{
//	PacketHeader header;
//	bool isReady1;
//	bool isReady2;
//	int PlayerRole1;
//	int PlayerRole2;
//	int PlayerName1;
//	int PlayerName2;
//};
//
//struct Vec2
//{
//	int x, y;
//};
//
//#define PlayerNum 2
//#define MonsterNum 10
//#define TileNum 10
//#define ItemNum 10
//
//struct S_GameStartDataPkt
//{
//	PacketHeader header;
//	int Stage;
//
//	int PlayerType[PlayerNum];
//	Vec2 PlayerPos[PlayerNum];
//	int PlayerState[PlayerNum];
//	int PlayerDir[PlayerNum];
//
//	int MonsterType[MonsterNum]; 
//	Vec2 MonsterPos[MonsterNum];
//	int MonsterState[MonsterNum];
//	int MonsterDir[MonsterNum];
//
//	int TileType[TileNum];
//	Vec2 TilePos[TileNum];
//	
//	int ItemType[ItemNum];
//	Vec2 ItemPos[ItemNum];
//};
//
//struct S_PlayerInfoUpdatePkt
//{
//	PacketHeader header;
//	int PlayerID;
//	Vec2 PlayerPos;
//	int PlayerState;
//	int PlayerDir;
//};
//
//struct S_MonsterInfoUpdatePkt
//{
//	PacketHeader header;
//	int MonsterID; //Monster[MonsterID]
//	Vec2 MonsterPos;
//	int MonsterState;
//	int MonsterDir;
//};
//
//struct S_GameEndDataPkt
//{
//	PacketHeader header;
//	int TotalTime;
//};