#include "MainScene.h"
#include "GameObject.h"
#include "..\GameObject\Player.h"
bool CMainScene::Init()
{
    player = CreateObject<CPlayer>("wns");
    player->SetSize(200.f, 100.f);
    player->SetPos(100.f, 100.f);
    SetPlayer(player);

    monster = CreateObject<CGameObject>("gus");
    monster->SetSize(100.f, 100.f);
    monster->SetPos(100.f, 100.f);

    return true;
}

