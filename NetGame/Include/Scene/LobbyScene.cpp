#include "LobbyScene.h"
#include <GameObject.h>
#include <Input.h>
#include <Engine.h>
#include <Scene/SceneManager.h>
#include "MainScene.h"
#include "..\PlayerAnimation.h"
#include "..\GameObject\LobbyPlayer.h"
#include "..\NetClient\TCPClient.h"

#define IDC_BUTTON 100	
#define IDC_BUTTON2 101	
#define IDC_BUTTON3 102	

bool CLobbyScene::Init()
{
	CScene::Init();
	m_myid = PacketManager::GetInst().GetMyID();
	m_lobbyData = &PacketManager::GetInst().lobbyData;

	CGameObject* back = CreateObject<CGameObject>("RoomBackground");
	back->CreateTexture(1);
	back->SetTexture("LobbyBackground", TEXT("Map/RoomBackground.bmp"), EObject_Dir::Right);
	back->SetSize(960.f, 650.f);
	back->SetPivot(0.f, 0.f);

	m_LobbyPlayer[0] = CreateObject<CLobbyPlayer>("Player1");
	m_LobbyPlayer[0]->SetSize(100.f, 200.f);
	m_LobbyPlayer[0]->SetPos(270.f, 300.f);
	m_LobbyPlayer[0]->SetEnable(true);

	m_LobbyPlayer[1] = CreateObject<CLobbyPlayer>("Player2");
	m_LobbyPlayer[1]->SetSize(100.f, 200.f);
	m_LobbyPlayer[1]->SetPos(680.f, 300.f);
	m_LobbyPlayer[1]->SetEnable(true);

	HWND hwnd = CEngine::GetInst()->GetWindowHandle();
	HINSTANCE hInst = CEngine::GetInst()->GetWindowInstance();

	// 폰트 생성
	HFONT hFont = CreateFont(
		40, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"UI/DungGeunMo.ttf"//글꼴 적용하고 싶은데.. 경로가 이게 아닌가?
	);
	m_hButton[0] = CreateWindow(L"button", L"전사", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 220, 450, 150, 80, hwnd, (HMENU)IDC_BUTTON, hInst, NULL);
	m_hButton[1] = CreateWindow(L"button", L"궁수", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 420, 450, 150, 80, hwnd, (HMENU)IDC_BUTTON2, hInst, NULL);
	m_hButton[2] = CreateWindow(L"button", L"준비완료", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 720, 450, 150, 100, hwnd, (HMENU)IDC_BUTTON3, hInst, NULL);
	SendMessage(m_hButton[0], WM_SETFONT, (WPARAM)hFont, TRUE);
	SendMessage(m_hButton[1], WM_SETFONT, (WPARAM)hFont, TRUE);
	SendMessage(m_hButton[2], WM_SETFONT, (WPARAM)hFont, TRUE);

	m_hPlayerName[0] = CreateWindow(L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_CENTER, 200, 100, 140, 50, hwnd, NULL, hInst, NULL);
	LONG_PTR styles0 = GetWindowLongPtr(m_hPlayerName[0], GWL_EXSTYLE);
	SetWindowLongPtr(m_hPlayerName[0], GWL_EXSTYLE, styles0 | WS_EX_TRANSPARENT);
	SendMessage(m_hPlayerName[0], WM_SETFONT, (WPARAM)hFont, TRUE);

	m_hPlayerName[1] = CreateWindow(L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_CENTER, 620, 100, 140, 50, hwnd, NULL, hInst, NULL);
	LONG_PTR styles1 = GetWindowLongPtr(m_hPlayerName[1], GWL_EXSTYLE);
	SetWindowLongPtr(m_hPlayerName[1], GWL_EXSTYLE, styles1 | WS_EX_TRANSPARENT);
	SendMessage(m_hPlayerName[1], WM_SETFONT, (WPARAM)hFont, TRUE);

	return true;
}

void CLobbyScene::Update(float elapsedTime)
{
	CScene::Update(elapsedTime);

	for (int i = 0; i < 2; ++i) {		//어느 위치가 적당한지 잘 모름.. 더블버퍼링 처리 좀 부탁
		TCHAR tPlayerName[20];
		auto playerName = m_lobbyData->players[i].name;
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, tPlayerName, playerName, _TRUNCATE);
		SetWindowText(m_hPlayerName[i], tPlayerName);
	}

	if (m_LobbyPlayer[0]->GetReady() && m_LobbyPlayer[1]->GetReady())
	{
		for (int i = 0; i < 3; ++i)
		{
			DestroyWindow(m_hButton[i]);
			m_hButton[i] = NULL;
		}
		if (m_hButton[2] == NULL)
			CSceneManager::GetInst()->CreateScene<CMainScene>();
	}
}

void CLobbyScene::KeyEvent(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_BUTTON:
	{
		m_lobbyData->players[m_myid].job = (int)(EPlayer_Job::Sword);
		break;
	}
	case IDC_BUTTON2:
	{
		m_lobbyData->players[m_myid].job = (int)(EPlayer_Job::Archer);
		break;
	}
	case IDC_BUTTON3:
	{
		m_lobbyData->players[m_myid].bReady = !m_lobbyData->players[m_myid].bReady;
		break;
	}
	}
}

void CLobbyScene::RecvGameData(const Packet& packet)
{
	switch (packet.type)
	{
	case LobbyUpdateResponse:
	{
		S_LobbyInfoPacket* RecvPacket = (S_LobbyInfoPacket*)&packet;
		memcpy(m_lobbyData, RecvPacket->data, RecvPacket->data_size);
		m_lobbyData->players[m_myid].id = m_myid;
		for (int i = 0; i < 2; ++i)
		{
			m_LobbyPlayer[i]->SetJob((EPlayer_Job)m_lobbyData->players[i].job);
			m_LobbyPlayer[i]->SetReady(m_lobbyData->players[i].bReady);
			m_LobbyPlayer[i]->SetName(m_lobbyData->players[i].name);
		}

		break;
	}
	default:
		break;
	}
}

void CLobbyScene::SendGameData()
{
	C_LobbyUpdateRequest sendPacket(m_lobbyData->players[m_myid]);
	PacketManager::GetInst().SendPacket(sendPacket);
}
