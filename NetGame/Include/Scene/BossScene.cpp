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
	m_myId = PacketManager::GetInst().GetMyID();

	CGameObject* back = CreateObject<CGameObject>("Background");
	back->CreateTexture(1);
	back->SetTexture("Background", TEXT("Map/BG.bmp"), EObject_Dir::Right);
	back->SetSize(2000.f, 1000.f);
	back->SetPivot(0.5f, 0.5f);

	boss = CreateObject<CBoss>("boss");
	boss->SetPos(0.f, 0.f);
	boss->CreateHPBar(this);
	boss->m_hpBar->SetBarSize(100, 5);

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
	SetPlayer(player[abs(1 - m_myId)]);

	player[m_myId]->InitInput();
	SetMyPlayer(player[m_myId]);
	GetCamera()->SetTarget(player[m_myId]);

	player[0]->CreateHPBar(this);
	player[1]->CreateHPBar(this);

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

	ClientGameData();


	m_timer += elapsedTime;
	if (m_timer > 2.0f) {
		if (boss->GetActive())
			BossAttack();
		m_timer = 0.f;
	}

	GameStateCheck(elapsedTime);
}

void CBossScene::Render(HDC hDC, float elapsedTime)
{
	CScene::Render(hDC, elapsedTime);
	RenderPlayTime(hDC);
}

bool CBossScene::SendGameData()
{
	ClientGameData();
	C_GameUpdateRequest sendPacket(m_inGameData.players[m_myId]);
	return PacketManager::GetInst().SendPacket(sendPacket);
}

void CBossScene::UpdateGameData()
{
	m_inGameData = PacketManager::GetInst().m_inGameData;

}

void CBossScene::ClientGameData()
{
	m_inGameData.players[m_myId].pos = vector2(player[m_myId]->GetPos().x, player[m_myId]->GetPos().y);
	m_inGameData.players[m_myId].state = (char)(EObject_State)(player[m_myId]->GetState());
	m_inGameData.players[m_myId].dir = (char)(EObject_Dir)(player[m_myId]->GetDir());
	m_inGameData.players[m_myId].isLanded = player[m_myId]->m_bIsLanded;
	m_inGameData.players[m_myId].isJump = player[m_myId]->m_bJump;
	m_inGameData.players[m_myId].isDoubleJump = player[m_myId]->m_bDoubleJump;
	m_inGameData.players[m_myId].hp = player[m_myId]->m_hp;
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


void CBossScene::BossAttack()
{
	boss->Attack(player[m_myId]->GetPos());
}

void CBossScene::CreateStageOneMap()
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
