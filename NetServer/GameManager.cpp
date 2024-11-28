#include "GameManager.h"

void GameManager::AddLobbyPlayer(const Client& client)
{
    int i = client.player.id;
    lobbyData.players[i] = LobbyPlayerInfo(client.player);
}

void GameManager::InitGameData()
{

}

void GameManager::PrintLobbyState()
{
    std::cout << "Game State:\n";

    for (int i = 0; i < PLAYER_NUM; ++i) {
        const auto& player = lobbyData.players[i];
        std::cout << "Player " << i
            << ": ID=" << player.id
            << ", Name=" << player.name
            << ", Job=" << (int)player.job << "\n";
    }
}

void GameManager::PrintGameState()
{
    std::cout << "Game State:\n";

    for (int i = 0; i < PLAYER_NUM; ++i) {
        const auto& player = gameData.players[i];
        std::cout << "Player " << i
            << ": ID=" << player.id
            << ", Name=" <<player.name
            << ", Pos=(" << player.pos.x << "," << player.pos.y << ")"
            << ", HP=" << player.hp
            << ", Job=" << player.job << "\n";
    }
}
