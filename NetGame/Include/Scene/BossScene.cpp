#include "BossScene.h"
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

bool CBossScene::Init()
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
	for (int i = 0; i < 10; ++i) {
		bossAttack[i] = CreateObject<CBossAttack>("richeAttack");
	}


	Vector2 resolution = { (float)CEngine::GetInst()->GetResolution().width,
	(float)CEngine::GetInst()->GetResolution().height };
	GetCamera()->SetResolution(resolution);
	GetCamera()->SetWorldResolution(2000.f, 2000.f);
	GetCamera()->SetViewType(ECamera_Type::Target);
	m_cameraVelocity = Vector2(100.f, 100.f);

	for (int j = 0; j < 2; ++j)
	{
		for (int i = 0; i < 10; ++i) {
			arrows[j][i] = CreateObject<CArrow>("arrow");
		}
	}
	CSceneManager* manager = CSceneManager::GetInst();
	for (int i = 0; i < 2; ++i)
	{
		if (manager->m_playerJob[i] == EPlayer_Job::Archer)
		{
			players[i] = CreateObject<CArcher>("player" + i);
			m_inGameData->players[i].job = EPlayer_Job::Archer;
		}
		else
		{
			players[i] = CreateObject<CSwordman>("player" + i);
			m_inGameData->players[i].job = EPlayer_Job::Sword;
		}
		players[i]->SetPos(-930.f, 300.f);
	}
	SetPlayer(players[abs(1 - m_myid)]);

	players[m_myid]->InitInput();
	SetMyPlayer(players[m_myid]);
	GetCamera()->SetTarget(players[m_myid]);

	players[0]->CreateHPBar(this);
	players[1]->CreateHPBar(this);

	m_tileNum = manager->m_tileNum;
	m_tileType = manager->m_tileType;
	m_tilePosX = manager->m_tilePosX;
	m_tilePosY = manager->m_tilePosY;
	CreateStageOneMap();

	{
		Packet packet = PacketManager::GetInst().RecvPacket();
		PacketManager::GetInst().ProcessPacket(packet);
	}
	return true;
}

void CBossScene::Update(float elapsedTime)
{
	CScene::Update(elapsedTime);

	GameDataCopy();
	GameStateCheck(elapsedTime);
}

void CBossScene::Render(HDC hDC, float elapsedTime)
{
	CScene::Render(hDC, elapsedTime);
	RenderPlayTime(hDC);
}

void CBossScene::RecvGameData(const Packet& packet)
{
	switch (packet.type)
	{
	case TileResponse:
	{
		m_tileNum = 0;
		m_tileType.clear();
		m_tilePosX.clear();
		m_tilePosY.clear();
		for (auto t : m_objects[(int)EObject_Type::WALL])
			(*t).Destroy();
		S_TilesPkt* cur = (S_TilesPkt*)&packet;
		cur->deserialize(m_tileNum, m_tileType, m_tilePosX, m_tilePosY);
		CreateStageOneMap();
		break;
	}
	case GameUpdateResponse:
	{
		S_GameInfoPacket* RecvPacket = (S_GameInfoPacket*)&packet;
		memcpy(m_inGameData, RecvPacket->data, RecvPacket->data_size);
		for (int i = 0; i < PLAYER_NUM; ++i)
		{
			players[i]->SetPos(m_inGameData->players[i].pos.x, m_inGameData->players[i].pos.y);
			players[i]->SetState((EObject_State)(int)m_inGameData->players[i].state);
			players[i]->SetDir((EObject_Dir)(int)m_inGameData->players[i].dir);
			players[i]->m_bIsLanded = m_inGameData->players[i].isLanded;
			players[i]->m_bJump = m_inGameData->players[i].isJump;
			players[i]->m_bDoubleJump = m_inGameData->players[i].isDoubleJump;
			players[i]->m_hp = m_inGameData->players[i].hp;
		}

		for (MonsterInfo& m : m_inGameData->monster)
		{

			switch (m.type) {
			case '2':
				if (!boss) break;
				if (!boss->m_bIsAlive) {
					boss->Destroy();
					break;
				}
				boss->SetPos(m.pos.x, m.pos.y);
				boss->SetState(m.state);
				boss->SetDir(m.direct);
				boss->m_bIsAlive = m.is_alive;
				boss->m_hp = m.hp;
				break;
			default:
				break;
			}
		}

		for (int j = 0; j < 2; ++j)
		{
			if (m_inGameData->players[j].job == EPlayer_Job::Archer)
			{
				for (int i = 0; i < ARROW_NUM; ++i)
				{
					arrows[j][i]->SetPos(m_inGameData->arrowAttack[i].pos.x, m_inGameData->arrowAttack[i].pos.y);
					arrows[j][i]->SetState(m_inGameData->arrowAttack[i].state);
					arrows[j][i]->SetDir(m_inGameData->arrowAttack[i].direct);
					if (m_inGameData->arrowAttack[i].is_alive)
						arrows[j][i]->SetEnable(true);
					else
						arrows[j][i]->SetEnable(false);
				}
			}
		}

		for (int i = 0; i < MONSTER_ATTACK_NUM; ++i)
		{
			bossAttack[i]->SetPos(m_inGameData->monsterAttack[i].pos.x, m_inGameData->monsterAttack[i].pos.y);
			bossAttack[i]->SetDir(m_inGameData->monsterAttack[i].direct);
			if (m_inGameData->monsterAttack[i].is_alive)
				bossAttack[i]->SetEnable(true);
			else
				bossAttack[i]->SetEnable(false);
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

void CBossScene::SendGameData()
{
	C_GameUpdateRequest sendPacket(m_inGameData->players[m_myid]);
	PacketManager::GetInst().SendPacket(sendPacket);
}

void CBossScene::GameDataCopy()
{
	m_inGameData->players[m_myid].pos = vector2(players[m_myid]->GetPos().x, players[m_myid]->GetPos().y);
	m_inGameData->players[m_myid].state = (EObject_State)(players[m_myid]->GetState());
	m_inGameData->players[m_myid].dir = (EObject_Dir)(players[m_myid]->GetDir());
	m_inGameData->players[m_myid].isLanded = players[m_myid]->m_bIsLanded;
	m_inGameData->players[m_myid].isJump = players[m_myid]->m_bJump;
	m_inGameData->players[m_myid].isDoubleJump = players[m_myid]->m_bDoubleJump;
	m_inGameData->players[m_myid].hp = players[m_myid]->m_hp;
}

void CBossScene::GameStateCheck(float elapsedTime)
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

void CBossScene::RenderPlayTime(HDC hDC)
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


void CBossScene::BossAttack()
{
	boss->Attack(players[m_myid]->GetPos());
}

void CBossScene::CreateStageOneMap()
{
	CTile* tile;
	for (int i = 0; i < m_tileNum; ++i) {
		std::string tileName = "tile" + std::to_string(m_tileType[i]);
		tile = CreateObject<CTile>(tileName);
		tile->SetPos(m_tilePosX[i], m_tilePosY[i]);
		tile->SetTileNum(m_tileType[i]);
	}
}

void CBossScene::ResourceInit()
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
