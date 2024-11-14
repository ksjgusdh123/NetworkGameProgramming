#include "MainScene.h"
#include "GameObject.h"
#include "..\GameObject\Player.h"
#include "..\GameObject\Swordman.h"
#include "..\GameObject\Archer.h"
#include "..\GameObject\Star.h"
#include "..\GameObject\Trap.h"
#include "..\GameObject\Heart.h"
#include "..\GameObject\Portal.h"
#include <Collision.h>
#include <Engine.h>
#include <Scene/Camera.h>

bool CMainScene::Init()
{
    CScene::Init();

    CGameObject* back = CreateObject<CGameObject>("Background");
    back->CreateTexture(1);
    back->SetTexture("Background", TEXT("Map/BG.bmp"), EObject_Dir::Right);
    back->SetSize(2000.f, 1000.f);

    CTile* tile = CreateObject<CTile>("tile");
    tile->SetPos(100.f, 150.f);
    tile->SetTileNum(1);

    tile = CreateObject<CTile>("tile2");
    tile->SetPos(350.f, 150.f);
    tile->SetTileNum(16);

    CStar* star = CreateObject<CStar>("star");
    star->SetPos(400.f, 100.f);

    CHeart* heart = CreateObject<CHeart>("heart");
    heart->SetPos(200.f, 350.f);

    CTrap* trap = CreateObject<CTrap>("trap");
    trap->SetPos(300.f, 350.f);

    CPortal* portal = CreateObject<CPortal>("portal");
    portal->SetPos(400.f, 350.f);
    
    Vector2 resolution = { (float)CEngine::GetInst()->GetResolution().width,
    (float)CEngine::GetInst()->GetResolution().height };
    GetCamera()->SetResolution(resolution);
    GetCamera()->SetWorldResolution(2000.f, 2000.f);
    GetCamera()->SetViewType(ECamera_Type::Target);
    m_cameraVelocity = Vector2(100.f, 100.f);


    player = CreateObject<CArcher>("wns");
    player->SetPos(100.f, 100.f);
    SetPlayer(player);
    GetCamera()->SetTarget(player);

    monster = CreateObject<CGameObject>("gus");
    monster->CreateTexture(1);
    monster->SetTexture("Plyer", TEXT("Player/Bazzi_Lobby.bmp"), EObject_Dir::Right);
	monster->SetColorKey(255, 0, 255);
    monster->SetSize(50.f, 60.f);
    monster->SetPos(100.f, 200.f);


    ghost = CreateObject<CGhost>("fdkaj");

    riche = CreateObject<CRiche>("riche");
    riche->SetPos(300.f, 100.f);

    return true;
}

void CMainScene::Update(float elapsedTime)
{
    CScene::Update(elapsedTime);

    if (IsPlayerInRicheAttackArea()){
        riche->Attack(player->GetPos());
    }

    for (auto& object : m_objects[2])
    {
        if (player->GetCollision()->CheckCollision(object->GetCollision()))
        {
            object->Destroy();
            break;
        }
    }
}

bool CMainScene::IsPlayerInRicheAttackArea()
{
    EObject_State riche_state = riche->GetState();
    if (riche_state == EObject_State::Attack_L || riche_state == EObject_State::Attack) return false;

    float dx = player->GetPos().x - riche->GetPos().x;
    float dy = player->GetPos().y - riche->GetPos().y;
    float distance = sqrt(dx * dx + dy * dy);
    if (distance < 400) return true;

    return false;
}

