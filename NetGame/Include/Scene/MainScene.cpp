#include "MainScene.h"

bool CMainScene::Init()
{
    player = CreateObject<GameObject>(L"준호");
    player->sizeX = 100.f;
    player->sizeY = 100.f;
    player->posX = 100.f;
    player->posY = 100.f;

    monster = CreateObject<GameObject>(L"현도");
    monster->sizeX = 150.f;
    monster->sizeY = 150.f;
    monster->posX = 150.f;
    monster->posY = 400.f;

    return true;
}

void CMainScene::Update(float elapsedTime)
{
    player->posX += 100.f * elapsedTime;
}

void CMainScene::PostUpdate(float elapsedTime)
{
}

void CMainScene::Render(HDC hDC, float elapsedTime)
{
    Ellipse(hDC, (int)player->posX - (int)player->sizeX / 2, (int)player->posY - (int)player->sizeY / 2,
        (int)player->posX + (int)player->sizeX / 2, (int)player->posY + (int)player->sizeY / 2);

    Rectangle(hDC, (int)monster->posX - (int)monster->sizeX / 2, (int)monster->posY - (int)monster->sizeY / 2,
        (int)monster->posX + (int)monster->sizeX / 2, (int)monster->posY + (int)monster->sizeY / 2);

    //TextOut(hDC, (int)player->posX, (int)player->posY, player->name.c_str(), (int)player->name.length());
    //TextOut(hDC, (int)monster->posX, (int)monster->posY, monster->name.c_str(), (int)monster->name.length());
}
