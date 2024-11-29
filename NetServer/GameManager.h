#pragma once
#include "Define.h"

struct Client {
    SOCKET socket;
    PlayerInfo player;

    Client(SOCKET sock) : socket(sock)
    {
        static int i = 0;
        player.id = i++;
    }
};

class GameManager {

public:
    void AddLobbyPlayer(const Client& client);
    LobbyData* GetLobbyData() { return &lobbyData; };
    InGameData* GetInGameData() { return &inGameData; };

    void InitGameData();
    void PrintLobbyState();
    void PrintGameState();

    void CreateTilePacket();
    bool CollisionCheck(int clientID);
    void ServerUpdate(int clientID);
public:
    static GameManager& GetInst()
    {
        static GameManager inst;
        return inst;
    }

private:
    LobbyData lobbyData;
    InGameData inGameData;
    vector<TileInfo> tiles;
};
