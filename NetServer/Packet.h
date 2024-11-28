#pragma once
#pragma pack(push, 1)
enum PacketType
{
	LoginRequest,
	LobbyUpdateRequest,
	GameUpdateRequest,

	LobbyUpdateResponse,
	GameUpdateResponse,
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