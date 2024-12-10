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

	GameEndRequest,
	GameEndNotification
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

//struct C_GameEndRequest : public Packet
//{
//	C_GameEndRequest(const GamePlayerInfo& GamePlayer_)
//	{
//		type = GameEndRequest;
//		data_size = sizeof(GamePlayer_);
//		memcpy(data, &GamePlayer_, data_size);
//	}
//};


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
		type = TileResponse; // Ÿ�� ����
		
		// �����͸� ���ڿ��� ����ȭ�Ͽ� data�� ����
		char* write_ptr = data;
		int offset = 0;

		// 1. Ÿ�� ���� �߰�
		offset += sprintf_s(write_ptr + offset, sizeof(data) - offset, "%d ", tileCount);

		// 2. �� Ÿ���� Ÿ�԰� ��ġ �߰�
		for (int i = 0; i < tileCount; ++i) {
			offset += sprintf_s(write_ptr + offset, sizeof(data) - offset, "%d %.2f %.2f ",
				tileTypes[i], tilePositions[i].x, tilePositions[i].y);
		}
		
		// ������ ũ�� ����
		data_size = strlen(data);
	}

	void deserialize(int& tileNum, std::vector<int>& tileTypes, std::vector<float>& tilePosX, std::vector<float>& tilePosY) {
		// ������ �Ľ� ����
		const char* read_ptr = data;

		// 1. Ÿ�� ���� �Ľ�
		sscanf_s(read_ptr, "%d ", &tileNum);

		// Ÿ�� ���� ũ�⸸ŭ �а� ������ �̵�
		while (*read_ptr != ' ') ++read_ptr; // ù ������� �̵�
		++read_ptr; // ���� �Ѿ� �̵�

		// 2. �� Ÿ���� Ÿ�԰� ��ġ �Ľ�
		tileTypes.clear();
		tilePosX.clear();
		tilePosY.clear();
		for (int i = 0; i < tileNum; ++i) {
			int tileType;
			float posX, posY;

			// Ÿ��, x, y ������ �Ľ�
			sscanf_s(read_ptr, "%d %f %f ", &tileType, &posX, &posY);

			tileTypes.push_back(tileType);
			tilePosX.push_back(posX);
			tilePosY.push_back(posY);

			// ���� �����͸�ŭ ������ �̵�
			while (*read_ptr != ' ') ++read_ptr; // ù ������� �̵�
			++read_ptr; // ���� �Ѿ� �̵�

			while (*read_ptr != ' ') ++read_ptr; // �� ��° ������� �̵�
			++read_ptr; // ���� �Ѿ� �̵�

			while (*read_ptr != ' ' && *read_ptr != '\0') ++read_ptr; // �� ��° ���� �Ǵ� ������ �̵�
			if (*read_ptr == ' ') ++read_ptr; // ������ ���� �Ѿ� �̵�
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
		data_size = sizeof(GameData_);
		memcpy(data, &GameData_, sizeof(GameData_));
	}
};

struct S_GameEndNotificationPacket :public Packet
{
	S_GameEndNotificationPacket(const ResultData& resultData_)
	{
		type = GameEndNotification;
		data_size = sizeof(resultData_);
		memcpy(data, &resultData_, sizeof(resultData_));
		//sprintf_s(data, "%f %d", resultData_.playTime, resultData_.bWin);
	}
	/*void deserialize(bool& bWin, float& playTime)
	{
		sscanf_s(data, "%f %d", &playTime, &bWin);
	}*/
};
#pragma pack(pop)