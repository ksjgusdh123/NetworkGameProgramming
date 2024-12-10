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
#include "Timer.h"

bool CMainScene::Init()
{
	CScene::Init();
	ResourceInit();
	m_myid = PacketManager::GetInst().GetMyID();
	m_mateId = abs(1 - m_myid);

	CGameObject* back = CreateObject<CGameObject>("Background");
	back->CreateTexture(1);
	back->SetTexture("Background", TEXT("Map/BG.bmp"), EObject_Dir::Right);
	back->SetSize(2000.f, 1000.f);
	back->SetPivot(0.5f, 0.5f);

	CPortal* portal = CreateObject<CPortal>("portal");
	portal->SetPos(730.f, -150.f);

	Vector2 resolution = { (float)CEngine::GetInst()->GetResolution().width,
	(float)CEngine::GetInst()->GetResolution().height };
	GetCamera()->SetResolution(resolution);
	GetCamera()->SetWorldResolution(2000.f, 2000.f);
	GetCamera()->SetViewType(ECamera_Type::Target);
	m_cameraVelocity = Vector2(100.f, 100.f);

	ghost = CreateObject<CGhost>("fdkaj");
	ghost->SetPos(-100.f, 410.f);
	ghost->m_originalPosX = ghost->GetPos().x;
	ghost->CreateHPBar(this);

	riche = CreateObject<CRiche>("riche");
	riche->SetPos(120.f, 80.f);
	riche->CreateHPBar(this);
	riche->m_hpBar->SetBarSize(80, 5);

	for (int i = 0; i < 10; ++i) {
		richeAttack[i] = CreateObject<CRicheAttack>("richeAttack");
	}

	for (int j = 0; j < 2; ++j)
	{
		for (int i = 0; i < 10; ++i) {
			arrows[j][i] = CreateObject<CArrow>("arrow");
		}
	}

	auto lobbyData = PacketManager::GetInst().m_lobbyData;
	for (int i = 0; i < 2; ++i)
	{
		if (lobbyData.players[i].job == EPlayer_Job::Archer)
		{
			players[i] = CreateObject<CArcher>("player" + i);
			m_inGameData.players[i].job = EPlayer_Job::Archer;
		}
		else
		{
			players[i] = CreateObject<CSwordman>("player" + i);
			m_inGameData.players[i].job = EPlayer_Job::Sword;
		}
		players[i]->SetPos(-930.f, 200.f);
	}


	SetPlayer(players[abs(1 - m_myid)]);
	SetMyPlayer(players[m_myid]);
	GetCamera()->SetTarget(players[m_myid]);

	m_inputPlayer = CreateObject<CPlayer>("player");
	m_inputPlayer->InitInput();
	m_inputPlayer->m_bRender = false;

	players[0]->CreateHPBar(this);
	players[1]->CreateHPBar(this);

	CreateStageOneMap();
	CreateStageOneItem();

	return true;
}

void CMainScene::Update(float elapsedTime)
{
	CScene::Update(elapsedTime);
	m_timer += elapsedTime;

	UpdateGameData();

	GameStateCheck(elapsedTime);
	bool isPortalEntry = m_inGameData.players[0].bReady && m_inGameData.players[1].bReady;
	if (isPortalEntry)
		CSceneManager::GetInst()->CreateScene<CBossScene>();
}

void CMainScene::Render(HDC hDC, float elapsedTime)
{
	CScene::Render(hDC, elapsedTime);
	RenderPlayTime(hDC);
}

bool CMainScene::SendGameData()
{
	ClientGameData();
	C_GameUpdateRequest sendPacket(m_inGameData.players[m_myid]);
	return PacketManager::GetInst().SendPacket(sendPacket);
}

void CMainScene::UpdateGameData()
{
	m_inGameData = PacketManager::GetInst().m_inGameData;

	for (int i = 0; i < PLAYER_NUM; ++i)
	{
		players[i]->SetPos(m_inGameData.players[i].pos.x, m_inGameData.players[i].pos.y);
		players[i]->SetState((EObject_State)(int)m_inGameData.players[i].state);
		players[i]->m_jumpState = m_inGameData.players[i].jumpState;
		players[i]->SetDir((EObject_Dir)(int)m_inGameData.players[i].dir);
		players[i]->m_hp = m_inGameData.players[i].hp;
	}

	GetCamera()->Update(ELAPSED_TIME);
	for (int i = 0; i < 2; ++i)
	{
		players[i]->m_hpBar->Update(ELAPSED_TIME);
	}

	if (m_inputPlayer->m_jumpState == EJump_State::Jumping)
	{
		if (m_inGameData.players[m_myid].jumpState != EJump_State::Jumping && m_inGameData.players[m_myid].jumpTime >= 0.1)
			m_inputPlayer->m_jumpState = m_inGameData.players[m_myid].jumpState;
	}

	m_inputPlayer->m_jumpTime += m_inGameData.players[m_myid].jumpTime;
	if (m_inputPlayer->m_jumpState == EJump_State::JumpDown)
	{
		if (m_inGameData.players[m_myid].jumpState == EJump_State::Landed)
		{
			m_inputPlayer->m_jumpState = EJump_State::Landed;
			m_inputPlayer->m_bDoubleJump = false;
		}
	}

	if (m_inputPlayer->GetState() == EObject_State::Attack)
	{
		if (m_inGameData.players[m_myid].state != EObject_State::Attack && m_inGameData.players[m_myid].nowFrame > 0.4)
			m_inputPlayer->SetState(EObject_State::Basic);
	}
	else if (m_inputPlayer->GetState() == EObject_State::Attack_L)
	{
		if (m_inGameData.players[m_myid].state != EObject_State::Attack_L && m_inGameData.players[m_myid].nowFrame > 0.4)
			m_inputPlayer->SetState(EObject_State::Basic_L);
	}

	if (m_inGameData.players[m_myid].hp <= 0)
	{
		m_inputPlayer->m_hp = 0;
	}


	for (MonsterInfo& m : m_inGameData.monster)
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

	for (int i = 0; i < MONSTER_ATTACK_NUM; ++i)
	{
		richeAttack[i]->SetPos(m_inGameData.monsterAttack[i].pos.x, m_inGameData.monsterAttack[i].pos.y);
		richeAttack[i]->SetState(m_inGameData.monsterAttack[i].state);
		richeAttack[i]->SetDir(m_inGameData.monsterAttack[i].direct);
		if (m_inGameData.monsterAttack[i].is_alive)
			richeAttack[i]->SetEnable(true);
		else
			richeAttack[i]->SetEnable(false);
	}

	for (int j = 0; j < 2; ++j)
	{
		if (m_inGameData.players[j].job == EPlayer_Job::Archer)
		{
			for (int i = 0; i < ARROW_NUM; ++i)
			{
				arrows[j][i]->SetPos(m_inGameData.arrowAttack[i].pos.x, m_inGameData.arrowAttack[i].pos.y);
				arrows[j][i]->SetState(m_inGameData.arrowAttack[i].state);
				arrows[j][i]->SetDir(m_inGameData.arrowAttack[i].direct);
				if (m_inGameData.arrowAttack[i].is_alive)
					arrows[j][i]->SetEnable(true);
				else
					arrows[j][i]->SetEnable(false);
			}
		}
	}

	for (auto& item : m_inGameData.item)
	{
		for (auto& object : m_objects[(int)EObject_Type::Item])
		{
			if (item.id != object->GetItemId()) continue;
			if (item.type == -1)
				object->Destroy();
			if (item.type == ItemType::STAR)
				object.get()->SetEnable(true);
		}
	}
}

void CMainScene::ClientGameData()
{
	m_inGameData.players[m_myid].elapsedTime = ELAPSED_TIME;
	m_inGameData.players[m_myid].state = (EObject_State)(m_inputPlayer->GetState());
	m_inGameData.players[m_myid].dir = (EObject_Dir)(m_inputPlayer->GetDir());
	m_inGameData.players[m_myid].jumpState = (m_inputPlayer->m_jumpState);

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

	int totalSeconds = m_inGameData.playtime;
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

	float dx = players[m_myid]->GetPos().x - riche->GetPos().x;
	float dy = players[m_myid]->GetPos().y - riche->GetPos().y;
	float distance = sqrt(dx * dx + dy * dy);
	if (distance < 400) return true;

	return false;
}

void CMainScene::CreateStageOneMap()
{
	int m_tileNum = PacketManager::GetInst().m_tileNum;
	vector<int> m_tileType = PacketManager::GetInst().m_tileType;
	vector<float> m_tilePosX = PacketManager::GetInst().m_tilePosX;
	vector<float> m_tilePosY = PacketManager::GetInst().m_tilePosY;

	CTile* tile;
	for (int i = 0; i < m_tileNum; ++i) {
		std::string tileName = "tile" + std::to_string(m_tileType[i]);
		tile = CreateObject<CTile>(tileName);
		tile->SetPos(m_tilePosX[i], m_tilePosY[i]);
		tile->SetTileNum(m_tileType[i]);
	}
}

void CMainScene::CreateStageOneItem()
{
	for (int i = 0; i < (int)ItemId::MAX; ++i)
	{
		int type;
		if (i < STAR1)
			type = ItemType::TRAP;
		else if (i < HEART1)
			type = ItemType::STAR_H;
		else
			type = ItemType::HEART;

		switch(type)
		{
		case ItemType::TRAP:
		{
			CTrap* trap = CreateObject<CTrap>("trap");
			trap->SetItemId(i);
			trap->SetPos(ItemPos[i].x, ItemPos[i].y);
			break;
		}
		case ItemType::STAR_H:
		{
			CStar* star = CreateObject<CStar>("star");
			star->SetItemId(i);
			star->SetPos(ItemPos[i].x, ItemPos[i].y);
			star->SetEnable(false);
			break;
		}
		case ItemType::HEART:
		{
			CHeart* heart = CreateObject<CHeart>("heart");
			heart->SetItemId(i);
			heart->SetPos(ItemPos[i].x, ItemPos[i].y);
			break;
		}
		}
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

