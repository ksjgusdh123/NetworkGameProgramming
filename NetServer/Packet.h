#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define DATA_SIZE 2048

#pragma pack(push, 1)
	
enum PacketType
{
	//InputKey,
	LoginRequest,
	
	TileRequest,
	TileResponse,

	LobbyUpdateRequest,
	LobbyUpdateResponse,

	GameUpdateRequest,
	GameUpdateResponse,
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

struct C_LoginRequestPkt : public Packet
{
	C_LoginRequestPkt(const std::string& player_name_)
	{
		type = LoginRequest;
		sprintf_s(data, "%s", player_name_.c_str());
		data_size = strlen(data);
	}

};

struct C_LobbyUpdateRequest : public Packet
{
	C_LobbyUpdateRequest(const LobbyPlayerInfo& lobbyPlayer_)
	{
		type = LobbyUpdateRequest;
		data_size = sizeof(lobbyPlayer_);
		memcpy(data, &lobbyPlayer_, data_size);
	}
};

struct C_GameUpdateRequest : public Packet
{
	C_GameUpdateRequest(const GamePlayerInfo& GamePlayer_)
	{
		type = GameUpdateRequest;
		data_size = sizeof(GamePlayer_);
		memcpy(data, &GamePlayer_, data_size);
	}
};


struct C_TileRequestPkt : public Packet
{
	C_TileRequestPkt()
	{
		type = TileRequest;
		sprintf_s(data, "%s", std::string{"HI"});
		data_size = strlen(data);
	}
};

struct S_TilesPkt : public Packet {
	S_TilesPkt(int tileCount, const std::vector<int>& tileTypes, const std::vector<vector2>& tilePositions) {
		type = TileResponse; // 타입 설정
		
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

	void deserialize(int& tileNum, std::vector<int>& tileTypes, std::vector<float>& tilePosX, std::vector<float>& tilePosY) {
		// 데이터 파싱 시작
		const char* read_ptr = data;

		// 1. 타일 개수 파싱
		sscanf_s(read_ptr, "%d ", &tileNum);

		// 타일 개수 크기만큼 읽고 포인터 이동
		while (*read_ptr != ' ') ++read_ptr; // 첫 공백까지 이동
		++read_ptr; // 공백 넘어 이동

		// 2. 각 타일의 타입과 위치 파싱
		tileTypes.clear();
		tilePosX.clear();
		tilePosY.clear();
		for (int i = 0; i < tileNum; ++i) {
			int tileType;
			float posX, posY;

			// 타입, x, y 순으로 파싱
			sscanf_s(read_ptr, "%d %f %f ", &tileType, &posX, &posY);

			tileTypes.push_back(tileType);
			tilePosX.push_back(posX);
			tilePosY.push_back(posY);

			// 읽은 데이터만큼 포인터 이동
			while (*read_ptr != ' ') ++read_ptr; // 첫 공백까지 이동
			++read_ptr; // 공백 넘어 이동

			while (*read_ptr != ' ') ++read_ptr; // 두 번째 공백까지 이동
			++read_ptr; // 공백 넘어 이동

			while (*read_ptr != ' ' && *read_ptr != '\0') ++read_ptr; // 세 번째 공백 또는 끝까지 이동
			if (*read_ptr == ' ') ++read_ptr; // 마지막 공백 넘어 이동
		}
	}
};

struct S_LobbyInfoPacket :public Packet
{
	S_LobbyInfoPacket(const LobbyData& lobbyData_)
	{
		type = LobbyUpdateResponse;
		data_size = sizeof(lobbyData_);
		memcpy(data, &lobbyData_, data_size);
	}
};

struct S_GameInfoPacket :public Packet
{
	S_GameInfoPacket(const InGameData& GameData_)
	{
		type = GameUpdateResponse;
		memcpy(data, &GameData_, sizeof(GameData_));
	}
};
#pragma pack(pop)