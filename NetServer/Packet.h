#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define DATA_SIZE 250

struct Client {
	SOCKET socket;
	std::string name;
	PlayerInfo player;
	int& id = player.id;
	Client(SOCKET sock, int clientId, const std::string& clientName)
		: socket(sock), name(clientName)
	{
		id = clientId;
	}
};

	
enum PacketType
{
	//InputKey,
	LoginRequest,
	PlayerChoice,
	GameStartRequest,
	PlayerMove,
	PlayerAttack,

	GameStartResponse,
	GameState,
	GameEndNotification,
	PlayerInfoUpdate,
	MonsterInfoUpdate,
};

class Packet
{
public:
	int client_id = -1;
	int type = 0;
	int data_size = 0;
	char data[DATA_SIZE]{};
	Packet() {}
	Packet(int client_id_, int type_, int data_size_, const char* data_)
		: client_id(client_id_), type(type_), data_size(data_size_)
	{
		memcpy(data, data_, data_size_);
	}
};

struct C_PlayerMovePkt :public Packet
{
	float x = -1;
	float y = -1;

	C_PlayerMovePkt(float x_, float y_)
		: x(x_), y(y_)
	{
		type = PlayerMove;
		sprintf_s(data, "%f %f", x, y);
		data_size = strlen(data);
	}
	void deserialize()
	{
		sscanf_s(data, "%f %f", &x, &y);
	}
};


struct C_LoginRequestPkt : public Packet
{
	C_LoginRequestPkt(const std::string& player_name_)
	{
		type = LoginRequest;
		sprintf_s(data, "%s", player_name_.c_str());
		data_size = strlen(data);
	}

	void deserialize()
	{
		
	}
};

struct C_PlayerChoicePkt : public Packet
{
	int j;

	C_PlayerChoicePkt(const int job_)
	{
		type = PlayerChoice;
		sprintf_s(data, "%d", job_);
		data_size = strlen(data);
	}

	void deserialize()
	{
		sscanf_s(data, "%d", &j);
	}
};

struct C_GameStartRequestPkt : public Packet
{
	bool ready;

	C_GameStartRequestPkt(const bool bReady)
	{
		type = GameStartRequest;
		sprintf_s(data, "%d", bReady);
		data_size = strlen(data);
	}

	void deserialize()
	{
		sscanf_s(data, "%d", &ready);
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