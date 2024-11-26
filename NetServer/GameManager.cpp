#include "GameManager.h"

void GameManager::AddLobbyPlayer(const Client& client, int index)
{
    lobbyData.players[index] = LobbyPlayerInfo(client.player);
}

void GameManager::InitGameData()
{
    // 로비 데이터를 기반으로 플레이어 초기화
    for (int i = 0; i < PLAYER_NUM; ++i) {
        const LobbyPlayerInfo& lobbyPlayer = lobbyData.players[i];
        gameData.players[i] = GamePlayerInfo(
            lobbyPlayer, 0, 0, 100, 10, 0 // 초기 좌표, 체력, 데미지, 상태 설정
        );
    }

    // 몬스터 초기화
    for (int i = 0; i < MONSTER_NUM; ++i) {
        gameData.monster[i] = { i, 'G', i * 10, i * 10, 50, 5, 'N', 'M' };
    }

    // 타일 초기화
    for (int i = 0; i < TILE_NUM; ++i) {
        gameData.tile[i] = { 0, i, i };
    }

    // 아이템 초기화
    gameData.item[0] = { 'W', 5, 5, 1 };
    gameData.item[1] = { 'A', 10, 10, 1 };
    gameData.item[2] = { 'P', 15, 15, 1 };

}

void GameManager::PrintGameState()
{
    std::cout << "Game State:\n";

    for (int i = 0; i < PLAYER_NUM; ++i) {
        const auto& player = gameData.players[i];
        std::cout << "Player " << i
            << ": ID=" << player.id
            << ", Pos=(" << player.x << "," << player.y << ")"
            << ", HP=" << player.hp
            << ", Job=" << player.job << "\n";
    }

    for (int i = 0; i < MONSTER_NUM; ++i) {
        const auto& monster = gameData.monster[i];
        std::cout << "Monster " << i
            << ": Type=" << monster.type
            << ", Pos=(" << monster.x << "," << monster.y << ")"
            << ", HP=" << monster.hp
            << ", Damage=" << monster.damage << "\n";
    }
}
