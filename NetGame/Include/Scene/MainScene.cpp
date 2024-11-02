#include "MainScene.h"
#include "GameObject.h"
#include "..\GameObject\Player.h"
#include <Engine.h>
#include <Scene/Camera.h>

bool CMainScene::Init()
{
    CScene::Init();

    Vector2 resolution = { (float)CEngine::GetInst()->GetResolution().width,
    (float)CEngine::GetInst()->GetResolution().height };
    GetCamera()->SetResolution(resolution);
    GetCamera()->SetWorldResolution(2000.f, 2000.f);
    GetCamera()->SetViewType(ECamera_Type::Target);
    m_cameraVelocity = Vector2(100.f, 100.f);


    player = CreateObject<CPlayer>("wns");
    player->SetSize(200.f, 100.f);
    player->SetPos(100.f, 100.f);
    SetPlayer(player);
    GetCamera()->SetTarget(player);

    monster = CreateObject<CGameObject>("gus");
    monster->SetSize(100.f, 100.f);
    monster->SetPos(100.f, 100.f);

    return true;
}

