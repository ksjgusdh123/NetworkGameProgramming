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
    struct LobbyData {
        std::array<LobbyPlayerInfo, PLAYER_NUM> players;
    };

    struct GameData {
        std::array<GamePlayerInfo, PLAYER_NUM> players;
        std::array<MonsterInfo, MONSTER_NUM> monster;
        std::array<TileInfo, TILE_NUM> tile;
        std::array<ItemInfo, ITEM_NUM> item;
    };

private:
    LobbyData lobbyData;
    GameData gameData;

public:
    GameManager() = default;

    void AddLobbyPlayer(const Client& client, int index);

    void InitGameData();
    void PrintGameState();

public:
    static GameManager& GetInst()
    {
        static GameManager inst;
        return inst;
    }
};
