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
#include "ResultScene.h"
#include <Scene/SceneManager.h>
#include "BossScene.h"

bool CMainScene::Init()
{
	CScene::Init();
	ResourceInit();
	m_myid = PacketManager::GetInst().GetMyID();
	m_inGameData = &PacketManager::GetInst().inGameData;


	CGameObject* back = CreateObject<CGameObject>("Background");
	back->CreateTexture(1);
	back->SetTexture("Background", TEXT("Map/BG.bmp"), EObject_Dir::Right);
	back->SetSize(2000.f, 1000.f);
	back->SetPivot(0.5f, 0.5f);

	boss = CreateObject<CBoss>("boss");
	boss->SetPos(0.f, 0.f);
	boss->CreateHPBar(this);
	boss->m_hpBar->SetBarSize(100, 5);

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

	player[0] = CreateObject<CSwordman>("player0");
	player[1] = CreateObject<CSwordman>("player1");
	player[0]->SetPos(-930.f, 300.f);
	player[1]->SetPos(-930.f, 300.f);
	SetPlayer(player[abs(1 - m_myid)]);

	player[m_myid]->InitInput();
	SetMyPlayer(player[m_myid]);
	GetCamera()->SetTarget(player[m_myid]);

	player[0]->CreateHPBar(this);
	player[1]->CreateHPBar(this);

	ghost = CreateObject<CGhost>("fdkaj");
	ghost->SetPos(-100.f, 410.f);
	ghost->m_originalPosX = ghost->GetPos().x;

	riche = CreateObject<CRiche>("riche");
	riche->SetPos(120.f, 80.f);
	riche->CreateHPBar(this);
	riche->m_hpBar->SetBarSize(80, 5);

	CSceneManager* manager = CSceneManager::GetInst();
	m_tileNum = manager->m_tileNum;
	m_tileType = manager->m_tileType;
	m_tilePosX = manager->m_tilePosX;
	m_tilePosY = manager->m_tilePosY;
	CreateStageOneMap();

	//// 게임 시작 요청 패킷 전송
	//if (m_myid == 0)
	//{
	//    C_TileRequestPkt packet{};
	//    PacketManager::GetInst().EnqueueSendPacket(packet);
	//}
	{
		Packet packet = PacketManager::GetInst().RecvPacket();
		PacketManager::GetInst().ProcessPacket(packet);
	}
	return true;
}

void CMainScene::Update(float elapsedTime)
{
	CScene::Update(elapsedTime);

	GameDataCopy();


	m_timer += elapsedTime;
	if (m_timer > 2.0f) {
		if (boss->GetActive())
			BossAttack();
		m_timer = 0.f;
	}

	for (auto& object : m_objects[(int)EObject_Type::Item])
	{
		if (player[m_myid]->GetCollision()->CheckCollision(object->GetCollision()))
		{
			object->Destroy();
			break;
		}
	}

	GameStateCheck(elapsedTime);

	if (m_inGameData->players[0].bReady && m_inGameData->players[1].bReady)
		CSceneManager::GetInst()->CreateScene<CBossScene>();
}

void CMainScene::Render(HDC hDC, float elapsedTime)
{
	CScene::Render(hDC, elapsedTime);
	RenderPlayTime(hDC);
}

void CMainScene::RecvGameData(const Packet& packet)
{
	switch (packet.type)
	{
	case TileResponse:
	{
		S_TilesPkt* cur = (S_TilesPkt*)&packet;
		CSceneManager* manager = CSceneManager::GetInst();
		manager->m_tileNum = 0;
		manager->m_tileType.clear();
		manager->m_tilePosX.clear();
		manager->m_tilePosY.clear();
		cur->deserialize(manager->m_tileNum, manager->m_tileType, manager->m_tilePosX, manager->m_tilePosY);
		break;
	}
	case GameUpdateResponse:
	{
		S_GameInfoPacket* RecvPacket = (S_GameInfoPacket*)&packet;
		memcpy(m_inGameData, RecvPacket->data, RecvPacket->data_size);
		for (int i = 0; i < PLAYER_NUM; ++i)
		{
			player[i]->SetPos(m_inGameData->players[i].pos.x, m_inGameData->players[i].pos.y);
			player[i]->SetState((EObject_State)(int)m_inGameData->players[i].state);
			player[i]->SetDir((EObject_Dir)(int)m_inGameData->players[i].dir);
			player[i]->m_bIsLanded = m_inGameData->players[i].isLanded;
			player[i]->m_bJump = m_inGameData->players[i].isJump;
			player[i]->m_bDoubleJump = m_inGameData->players[i].isDoubleJump;
			player[i]->m_hp = m_inGameData->players[i].hp;
		}

		for (MonsterInfo& m : m_inGameData->monster)
		{
			
			switch (m.type) {
			case '0':
			{
				if (!ghost) break;
				if (!ghost->m_bIsAlive) {
					ghost->Destroy();
					break;
				}
				ghost->SetPos(m.pos.x, m.pos.y);
				ghost->SetState(m.state);
				ghost->SetDir(m.direct);
				ghost->m_bIsAlive = m.is_alive;
				ghost->m_hp = m.hp;
			}
			break;
			case '1':
			{
				if (!riche) break;
				if (!riche->m_bIsAlive) {
					riche->Destroy();
					break;
				}				
				riche->SetState(m.state);
				riche->SetDir(m.direct);
				riche->m_bIsAlive = m.is_alive;
				riche->m_hp = m.hp;
				riche->m_target = Vector2(m.target.x, m.target.y);
			}
			break;
			case '2':
				break;
			default:
				break;
			}
		}

		break;
	}
	case GameEndNotification:
	{
		S_GameEndNotificationPacket* RecvPacket = (S_GameEndNotificationPacket*)&packet;
		CSceneManager* manager = CSceneManager::GetInst();
		RecvPacket->deserialize(manager->m_bWin, manager->m_playTime);
		m_bEnd = true;
		break;
	}
	default:
		break;
	}
}

void CMainScene::SendGameData()
{
	C_GameUpdateRequest sendPacket(m_inGameData->players[m_myid]);
	PacketManager::GetInst().SendPacket(sendPacket);
}

void CMainScene::GameDataCopy()
{
	m_inGameData->players[m_myid].pos = vector2(player[m_myid]->GetPos().x, player[m_myid]->GetPos().y);
	m_inGameData->players[m_myid].state = (char)(EObject_State)(player[m_myid]->GetState());
	m_inGameData->players[m_myid].dir = (char)(EObject_Dir)(player[m_myid]->GetDir());
	m_inGameData->players[m_myid].isLanded = player[m_myid]->m_bIsLanded;
	m_inGameData->players[m_myid].isJump = player[m_myid]->m_bJump;
	m_inGameData->players[m_myid].isDoubleJump = player[m_myid]->m_bDoubleJump;
	m_inGameData->players[m_myid].hp = player[m_myid]->m_hp;
}

void CMainScene::GameStateCheck(float elapsedTime)
{
	if (((CPlayer*)m_myPlayer)->m_hp <= 0)
	{
		m_deathTime += elapsedTime;
		if (!m_bDieCheck && m_deathTime >= 1)
		{
			GetCamera()->SetTarget(m_player);
			m_bDieCheck = true;
		}
	}

	if (m_bEnd)
	{
		CSceneManager::GetInst()->CreateScene<CResultScene>();
	}
}

void CMainScene::RenderPlayTime(HDC hDC)
{
	HFONT hFont = CreateFontWithSize(m_hFont, 30);
	HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);

	int totalSeconds = m_inGameData->playtime;
	int hours = totalSeconds / 3600;
	int minutes = (totalSeconds % 3600) / 60;
	int seconds = totalSeconds % 60;

	wchar_t timeText[64];
	swprintf_s(timeText, L"Play Time %02d:%02d:%02d", hours, minutes, seconds);

	SetTextColor(hDC, RGB(255, 255, 204));
	SetBkMode(hDC, TRANSPARENT);

	TextOut(hDC, 20, 20, timeText, wcslen(timeText));
	SelectObject(hDC, hOldFont);
	DeleteObject(hFont);
}


bool CMainScene::IsPlayerInRicheAttackArea()
{

	EObject_State riche_state = riche->GetState();
	if (riche_state == EObject_State::Attack_L || riche_state == EObject_State::Attack) return false;

	float dx = player[m_myid]->GetPos().x - riche->GetPos().x;
	float dy = player[m_myid]->GetPos().y - riche->GetPos().y;
	float distance = sqrt(dx * dx + dy * dy);
	if (distance < 400) return true;


	return false;
}

void CMainScene::BossAttack()
{
	boss->Attack(player[m_myid]->GetPos());
}

void CMainScene::CreateStageOneMap()
{
	CTile* tile;
	for (int i = 0; i < m_tileNum; ++i) {
		std::string tileName = "tile" + std::to_string(m_tileType[i]);
		tile = CreateObject<CTile>(tileName);
		tile->SetPos(m_tilePosX[i], m_tilePosY[i]);
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

