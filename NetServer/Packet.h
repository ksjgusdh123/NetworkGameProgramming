#pragma once
#pragma pack(push, 1)
enum PacketType
{
	//InputKey,
	LoginRequest,
	PlayerChoice,

	LobbyUpdateRequest,

	GameStartRequest,
	PlayerMove,
	PlayerAttack,

	LobbyInfo,
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
	char data[BUFSIZ]{};
	Packet() {}
	Packet(int client_id_, int type_, int data_size_, const char* data_)
		: client_id(client_id_), type(type_), data_size(data_size_)
	{
		std::memcpy(data, data_, data_size_);
	}
};

struct S_GameInfoPacket :public Packet
{
	S_GameInfoPacket(const InGameData& GameData_)
	{
		type = GameState;
		memcpy(data, &GameData_, sizeof(GameData_));
	}
};

struct S_LobbyInfoPacket :public Packet
{
	S_LobbyInfoPacket(const LobbyData& lobbyData_)
	{
		type = LobbyInfo;
		data_size = sizeof(lobbyData_);
		memcpy(data, &lobbyData_, data_size);
	}
};

struct C_PlayerMovePkt :public Packet
{
	float x = -1;
	float y = -1;

	C_PlayerMovePkt(float x_, float y_)
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

struct C_LobbyUpdateRequest : public Packet
{
	C_LobbyUpdateRequest(const LobbyPlayerInfo& lobbyData_)
	{
		type = LobbyUpdateRequest;
		data_size = sizeof(lobbyData_);
		memcpy(data, &lobbyData_, data_size);
	}
};

#pragma pack(pop)