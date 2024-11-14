#pragma once

enum PacketType
{
	InputKey,
	LoginRequest,
	PlayerChoice,
};

class Packet
{
public:
	int len;
	char data[BUFSIZ];
	Packet() {};
	Packet(int len_, const char* data_): len(len_)
	{
		memcpy(data, data_ , len_);
		data[len + 1] = '\0';
	}
};


//struct PacketHeader
//{
//	char type;
//	int length;
//};
//
//struct C_LoginRequestPkt
//{
//	PacketHeader header;
//};
//
//struct C_GameStartRequestPkt
//{
//	PacketHeader header;
//};
//
//struct C_PlayerMovePkt
//{
//	PacketHeader header;
//	int dir;
//};
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