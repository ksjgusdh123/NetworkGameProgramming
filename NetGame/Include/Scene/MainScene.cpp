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
#include "..\NetClient\TCPClient.h"
#include "Scene/SceneResource.h"

bool CMainScene::Init()
{
    CScene::Init();
    ResourceInit();

    CGameObject* back = CreateObject<CGameObject>("Background");
    back->CreateTexture(1);
    back->SetTexture("Background", TEXT("Map/BG.bmp"), EObject_Dir::Right);
    back->SetSize(2000.f, 1000.f);
    back->SetPivot(0.5f, 0.5f);


    boss = CreateObject<CBoss>("boss");
    boss->SetPos(0.f, 0.f);

    CTrap* trap = CreateObject<CTrap>("trap");
    trap->SetPos(-570.f, 415.f);

    trap = CreateObject<CTrap>("trap");
    trap->SetPos(-40.f, 140.f);

    trap = CreateObject<CTrap>("trap");
    trap->SetPos(600.f, 40.f);

    trap = CreateObject<CTrap>("trap");
    trap->SetPos(600.f, 240.f);

    CStar* star = CreateObject<CStar>("star");
    star->SetPos(-570.f, 415.f);

    CHeart* heart = CreateObject<CHeart>("heart");
    heart->SetPos(280.f, 415.f);

    CPortal* portal = CreateObject<CPortal>("portal");
    portal->SetPos(730.f, -150.f);
    
    Vector2 resolution = { (float)CEngine::GetInst()->GetResolution().width,
    (float)CEngine::GetInst()->GetResolution().height };
    GetCamera()->SetResolution(resolution);
    GetCamera()->SetWorldResolution(2000.f, 2000.f);
    GetCamera()->SetViewType(ECamera_Type::Target);
    m_cameraVelocity = Vector2(100.f, 100.f);


    player = CreateObject<CArcher>("player");
    player->SetPos(-100.f, 180.f);
    SetPlayer(player);

    myplayer = CreateObject<CArcher>("myplayer");
    myplayer->SetPos(0.f, 180.f);
    myplayer->InitInput();
    SetMyPlayer(myplayer);
    GetCamera()->SetTarget(myplayer);

 /*   monster = CreateObject<CGameObject>("gus");
    monster->CreateTexture(1);
    monster->SetTexture("Plyer", TEXT("Player/Bazzi_Lobby.bmp"), EObject_Dir::Right);
	monster->SetColorKey(255, 0, 255);
    monster->SetSize(50.f, 60.f);
    monster->SetPos(100.f, 200.f);*/


    ghost = CreateObject<CGhost>("fdkaj");
    ghost->SetPos(-100.f, 410.f);
    ghost->m_originalPosX = ghost->GetPos().x;

    riche = CreateObject<CRiche>("riche");
    riche->SetPos(120.f, 80.f);
    
    riche = CreateObject<CRiche>("riche");
    riche->SetPos(450.f, 380.f);

    // 게임 시작 요청 패킷 전송
    C_TileRequestPkt packet{};
    PacketManager::GetInst().EnqueueSendPacket(packet);

   

    return true;
}

void CMainScene::Update(float elapsedTime)
{
    CScene::Update(elapsedTime);

    if (IsPlayerInRicheAttackArea()){
        riche->Attack(player->GetPos());
    }
    m_timer += elapsedTime;
    if (m_timer > 2.0f) {
        if (boss->GetActive())
            BossAttack();
        m_timer = 0.f;
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

void CMainScene::PacketEvent(const Packet& packet)
{
    switch (packet.type)
    {
    case Tiles:
    {	
        C_TilesPkt* cur = (C_TilesPkt*)&packet;
        cur->deserialize(m_tileNum, m_tileType, m_tilePosX, m_tilePosY);
        CreateStageOneMap();
    }
    default:
        break;
    }
}

void CMainScene::BossAttack()
{
    boss->Attack(player->GetPos());
}



void CMainScene::CreateMap()
{
    float tilePosX = -930.f;
    float tilePosY = 475.f;

    // 첫 발판
    CTile* tile = CreateObject<CTile>("tile");
    tile->SetPos(tilePosX, tilePosY);
    tile->SetTileNum(1);
    tilePosX += 50.f;

    for (int i = 0; i < 20; ++i) {
        tile = CreateObject<CTile>("tile2");
        tile->SetPos(tilePosX, 475.f);
        tilePosX += 50.f;
        tile->SetTileNum(2);
    }

    tile = CreateObject<CTile>("tile3");
    tile->SetPos(tilePosX, 475.f);
    tilePosX += 150.f;
    tile->SetTileNum(3);


    // 상자 계단
    tile = CreateObject<CTile>("crate");
    tile->SetPos(-800.f, 425.f);
    tile->SetTileNum(17);
    tile = CreateObject<CTile>("crate");
    tile->SetPos(-750.f, 425.f);
    tile->SetTileNum(17);
    tile = CreateObject<CTile>("crate");
    tile->SetPos(-750.f, 375.f);
    tile->SetTileNum(17);
    tile = CreateObject<CTile>("crate");
    tile->SetPos(-700.f, 425.f);
    tile->SetTileNum(17);
    tile = CreateObject<CTile>("crate");
    tile->SetPos(-700.f, 375.f);
    tile->SetTileNum(17);
    tile = CreateObject<CTile>("crate");
    tile->SetPos(-700.f, 325.f);
    tile->SetTileNum(17);

    // 상자 계단2
    tile = CreateObject<CTile>("crate");
    tile->SetPos(-550.f, 425.f);
    tile->SetTileNum(17);
    tile = CreateObject<CTile>("crate");
    tile->SetPos(-500.f, 425.f);
    tile->SetTileNum(17);
    tile = CreateObject<CTile>("crate");
    tile->SetPos(-500.f, 375.f);
    tile->SetTileNum(17);
    tile = CreateObject<CTile>("crate");
    tile->SetPos(-500.f, 325.f);
    tile->SetTileNum(17);

    tile = CreateObject<CTile>("crate");
    tile->SetPos(-450.f, 425.f);
    tile->SetTileNum(17);
    tile = CreateObject<CTile>("crate");
    tile->SetPos(-450.f, 375.f);
    tile->SetTileNum(17);

    tile = CreateObject<CTile>("crate");
    tile->SetPos(-400.f, 425.f);
    tile->SetTileNum(17);

    //
    tile = CreateObject<CTile>("tile14");
    tile->SetPos(-380.f, 250.f);
    tile->SetTileNum(14);
    tile = CreateObject<CTile>("tile15");
    tile->SetPos(-330.f, 250.f);
    tile->SetTileNum(15);
    tile = CreateObject<CTile>("tile16");
    tile->SetPos(-280.f, 250.f);
    tile->SetTileNum(16);

    tile = CreateObject<CTile>("tile14");
    tile->SetPos(-130.f, 200.f);
    tile->SetTileNum(14);
    tile = CreateObject<CTile>("tile15");
    tile->SetPos(-80.f, 200.f);
    tile->SetTileNum(15);
    tile = CreateObject<CTile>("tile16");
    tile->SetPos(-30.f, 200.f);
    tile->SetTileNum(16);

    tile = CreateObject<CTile>("tile14");
    tile->SetPos(70.f, 170.f);
    tile->SetTileNum(14);
    tile = CreateObject<CTile>("tile15");
    tile->SetPos(120.f, 170.f);
    tile->SetTileNum(15);
    tile = CreateObject<CTile>("tile16");
    tile->SetPos(170.f, 170.f);
    tile->SetTileNum(16);

    tile = CreateObject<CTile>("tile14");
    tile->SetPos(-380.f, 100.f);
    tile->SetTileNum(14);
    tile = CreateObject<CTile>("tile15");
    tile->SetPos(-330.f, 100.f);
    tile->SetTileNum(15);
    tile = CreateObject<CTile>("tile16");
    tile->SetPos(-280.f, 100.f);
    tile->SetTileNum(16);
    //
    // 근접 몬스터 가두는 상자

    tile = CreateObject<CTile>("crate");
    tile->SetPos(-250.f, 425.f);
    tile->SetTileNum(17);

    tile = CreateObject<CTile>("crate");
    tile->SetPos(50.f, 425.f);
    tile->SetTileNum(17);
    

    // 두번째 발판
    tile = CreateObject<CTile>("tile1");
    tile->SetPos(tilePosX, 475.f);
    tilePosX += 50.f;
    tile->SetTileNum(1);

    for (int i = 0; i < 6; ++i) {
        tile = CreateObject<CTile>("tile2");
        tile->SetPos(tilePosX, 475.f);
        tilePosX += 50.f;
        tile->SetTileNum(2);
    }

    tile = CreateObject<CTile>("tile3");
    tile->SetPos(tilePosX, 475.f);
    tilePosX += 250.f;
    tile->SetTileNum(3);

    // 점프맵 발판
    tile = CreateObject<CTile>("tile14");
    tile->SetPos(tilePosX, 400.f);
    tile->SetTileNum(14);
    tile = CreateObject<CTile>("tile14");
    tile->SetPos(tilePosX, 200.f);
    tile->SetTileNum(14);
    tilePosX += 50.f;

    tile = CreateObject<CTile>("tile15");
    tile->SetPos(tilePosX, 400.f);
    tile->SetTileNum(15);
    tile = CreateObject<CTile>("tile15");
    tile->SetPos(tilePosX, 200.f);
    tile->SetTileNum(15);
    tilePosX += 50.f;

    tile = CreateObject<CTile>("tile16");
    tile->SetPos(tilePosX, 400.f);
    tile->SetTileNum(16);
    tile = CreateObject<CTile>("tile16");
    tile->SetPos(tilePosX, 200.f);
    tile->SetTileNum(16);
    
    tilePosX -= 400.f;

    tile = CreateObject<CTile>("tile14");
    tile->SetPos(tilePosX, 300.f);
    tile->SetTileNum(14);
    tile = CreateObject<CTile>("tile14");
    tile->SetPos(tilePosX, 100.f);
    tile->SetTileNum(14);
    tilePosX += 50.f;

    tile = CreateObject<CTile>("tile15");
    tile->SetPos(tilePosX, 300.f);
    tile->SetTileNum(15);
    tile = CreateObject<CTile>("tile15");
    tile->SetPos(tilePosX, 100.f);
    tile->SetTileNum(15);
    tilePosX += 50.f;

    tile = CreateObject<CTile>("tile16");
    tile->SetPos(tilePosX, 300.f);
    tile->SetTileNum(16);
    tile = CreateObject<CTile>("tile16");
    tile->SetPos(tilePosX, 100.f);
    tile->SetTileNum(16);


    tilePosX -= 400.f;

    tile = CreateObject<CTile>("tile14");
    tile->SetPos(tilePosX, 0.f);
    tile->SetTileNum(14);
    tilePosX += 50.f;

    tile = CreateObject<CTile>("tile15");
    tile->SetPos(tilePosX, 0.f);
    tile->SetTileNum(15);
    tilePosX += 50.f;

    tile = CreateObject<CTile>("tile16");
    tile->SetPos(tilePosX, 0.f);
    tile->SetTileNum(16);

    tilePosX += 200.f;


    // 포탈 발판
    tile = CreateObject<CTile>("tile14");
    tile->SetPos(tilePosX, -100.f);
    tile->SetTileNum(14);
    tilePosX += 50.f;

    for (int i = 0; i < 4; ++i) {
        tile = CreateObject<CTile>("tile15");
        tile->SetPos(tilePosX, -100.f);
        tile->SetTileNum(15);
        tilePosX += 50.f;
    }

    tile = CreateObject<CTile>("tile16");
    tile->SetPos(tilePosX, -100.f);
    tile->SetTileNum(16);
}


void CMainScene::CreateStageOneMap()
{
    CTile* tile;
    for (int i = 0; i < m_tileNum; ++i) {
        std::string tileName = "tile" + std::to_string(m_tileType[i]);
        tile = CreateObject<CTile>(tileName);
        tile->SetPos(m_tilePosX[i] , m_tilePosY[i]);
        tile->SetTileNum(m_tileType[i]);
    }
}

void CMainScene::ResourceInit()
{
    GetSceneResource()->LoadTexture("Tile(1)", TEXT("Map/Tile (1).bmp"));
    GetSceneResource()->LoadTexture("Tile(2)", TEXT("Map/Tile (2).bmp"));
    GetSceneResource()->LoadTexture("Tile(3)", TEXT("Map/Tile (3).bmp"));
    GetSceneResource()->LoadTexture("Tile(4)", TEXT("Map/Tile (4).bmp"));
    GetSceneResource()->LoadTexture("Tile(5)", TEXT("Map/Tile (5).bmp"));
    GetSceneResource()->LoadTexture("Tile(6)", TEXT("Map/Tile (6).bmp"));
    GetSceneResource()->LoadTexture("Tile(7)", TEXT("Map/Tile (7).bmp"));
    GetSceneResource()->LoadTexture("Tile(8)", TEXT("Map/Tile (8).bmp"));
    GetSceneResource()->LoadTexture("Tile(9)", TEXT("Map/Tile (9).bmp"));
    GetSceneResource()->LoadTexture("Tile(10)", TEXT("Map/Tile (10).bmp"));
    GetSceneResource()->LoadTexture("Tile(11)", TEXT("Map/Tile (11).bmp"));
    GetSceneResource()->LoadTexture("Tile(12)", TEXT("Map/Tile (12).bmp"));
    GetSceneResource()->LoadTexture("Tile(13)", TEXT("Map/Tile (13).bmp"));
    GetSceneResource()->LoadTexture("Tile(14)", TEXT("Map/Tile (14).bmp"));
    GetSceneResource()->LoadTexture("Tile(15)", TEXT("Map/Tile (15).bmp"));
    GetSceneResource()->LoadTexture("Tile(16)", TEXT("Map/Tile (16).bmp"));
    GetSceneResource()->LoadTexture("Tile(17)", TEXT("Map/Tile (17).bmp"));
}
