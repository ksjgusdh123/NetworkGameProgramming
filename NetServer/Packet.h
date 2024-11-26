#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define DATA_SIZE 2048

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
	LoginRequest,
	LobbyRequest,
	PlayerMove,
	GameStartRequest,
	Tiles,
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

struct C_LobbyRequestPkt : public Packet
{
	int ClientNum = 0;
	char name[2][20];
	C_LobbyRequestPkt()
	{
		type = LobbyRequest;
		sprintf_s(data, "%d", 4);
		data_size = strlen(data);
	}

	void initial(const std::vector<Client>& clients)
	{
		type = LobbyRequest;

		int num = clients.size();
		int offset = sprintf_s(data, "%d ", num);

		for (int i = 0; i < num; ++i) {
			offset += sprintf_s(data + offset, sizeof(data) - offset, "%s ", clients[i].name.c_str());
		}
		data_size = strlen(data);
	}

	void deserialize()
	{
		const char* current = data; // 현재 읽는 위치
		sscanf_s(current, "%d ", &ClientNum); // 클라이언트 수 읽기

		// 클라이언트 이름 읽기
		current = strchr(current, ' ') + 1; // 숫자 이후로 이동

		for (int i = 0; i < ClientNum; ++i)
		{
			char n[128]; // 이름을 임시로 저장할 버퍼
			sscanf_s(current, "%s ", n, (unsigned)_countof(n)); // 이름 읽기
			strcpy_s(name[i], sizeof(name[i]), n);
			current = strchr(current, ' ') + 1; // 다음 이름으로 이동
		}
	}

};

struct C_GameStartRequestPkt : public Packet
{
	C_GameStartRequestPkt()
	{
		type = GameStartRequest;
		sprintf_s(data, "%s", std::string{"HI"});
		data_size = strlen(data);
	}

	void deserialize()
	{

	}
};

struct C_TilesPkt : public Packet {
	C_TilesPkt(int tileCount, const std::vector<int>& tileTypes, const std::vector<vector2>& tilePositions) {
		type = Tiles; // 타입 설정
		
		// 데이터를 문자열로 직렬화하여 data에 저장
		char* write_ptr = data;
		int offset = 0;

		// 1. 타일 개수 추가
		offset += sprintf_s(write_ptr + offset, sizeof(data) - offset, "%d ", tileCount);

		// 2. 각 타일의 타입과 위치 추가
		for (int i = 0; i < tileCount; ++i) {
			offset += sprintf_s(write_ptr + offset, sizeof(data) - offset, "%d %.2f %.2f ",
				tileTypes[i], tilePositions[i].x, tilePositions[i].y);
		}
		
		// 데이터 크기 설정
		data_size = strlen(data);
	}

	void deserialize() {
		// 역직렬화는 필요 시 구현
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