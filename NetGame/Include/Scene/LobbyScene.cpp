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
CRITICAL_SECTION cs;

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

	AddFontResourceEx(L"Font/DungGeunMo.ttf", FR_PRIVATE, nullptr);
	// 폰트 생성
	HFONT hFont = CreateFont(
		35, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"DungGeunMo"
	);

	m_hButton[0] = CreateWindow(L"button", L"전사", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 220, 450, 150, 80, hwnd, (HMENU)IDC_BUTTON, hInst, NULL);
	m_hButton[1] = CreateWindow(L"button", L"궁수", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 420, 450, 150, 80, hwnd, (HMENU)IDC_BUTTON2, hInst, NULL);
	m_hButton[2] = CreateWindow(L"button", L"준비완료", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 720, 450, 150, 100, hwnd, (HMENU)IDC_BUTTON3, hInst, NULL);
	SendMessage(m_hButton[0], WM_SETFONT, (WPARAM)hFont, TRUE);
	SendMessage(m_hButton[1], WM_SETFONT, (WPARAM)hFont, TRUE);
	SendMessage(m_hButton[2], WM_SETFONT, (WPARAM)hFont, TRUE);
	return true;
}

void CLobbyScene::Update(float elapsedTime)
{
	CScene::Update(elapsedTime);

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

void CLobbyScene::Render(HDC hDC, float elapsedTime)
{
	CScene::Render(hDC, elapsedTime);

	PrintName(hDC);
}

void CLobbyScene::KeyEvent(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_BUTTON:
	{
		m_lobbyData->players[m_myid].job = (char)(EPlayer_Job::Sword);
		break;
	}
	case IDC_BUTTON2:
	{
		m_lobbyData->players[m_myid].job = (char)(EPlayer_Job::Archer);
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
	//EnterCriticalSection(&cs);
	switch (packet.type)
	{
	case TileResponse:
	{
		S_TilesPkt* cur = (S_TilesPkt*)&packet;
		CSceneManager* manager = CSceneManager::GetInst();
		cur->deserialize(manager->m_tileNum, manager->m_tileType, manager->m_tilePosX, manager->m_tilePosY);
		break;
	}
	case LobbyUpdateResponse:				
	{
		S_LobbyInfoPacket* RecvPacket = (S_LobbyInfoPacket*)&packet;
		memcpy(m_lobbyData, RecvPacket->data, RecvPacket->data_size);
		m_lobbyData->players[m_myid].id = m_myid;
		for (int i = 0; i < 2; ++i)
		{
			m_LobbyPlayer[i]->SetJob((EPlayer_Job)(int)m_lobbyData->players[i].job);
			m_LobbyPlayer[i]->SetReady(m_lobbyData->players[i].bReady);
			m_LobbyPlayer[i]->SetName(m_lobbyData->players[i].name);
		}


		break;
	}
	default:
		break;
	}
	//LeaveCriticalSection(&cs);
}

void CLobbyScene::SendGameData()
{
	//EnterCriticalSection(&cs);
	C_LobbyUpdateRequest sendPacket(m_lobbyData->players[m_myid]);
	PacketManager::GetInst().SendPacket(sendPacket);	
	//LeaveCriticalSection(&cs);
}

void CLobbyScene::PrintName(HDC hDC)
{
	AddFontResourceEx(L"Font/DungGeunMo.ttf", FR_PRIVATE, nullptr);
	HFONT hFont = CreateFont(
		40, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"DungGeunMo");
	HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);
	for (int i = 0; i < 2; ++i)
	{
		wchar_t wideName[NAME_LEN] = {};
		MultiByteToWideChar(CP_ACP, 0, m_lobbyData->players[i].name, -1, wideName, NAME_LEN);

		SetTextColor(hDC, RGB(255, 255, 255));   // 텍스트 색상 (흰색)
		SetBkColor(hDC, RGB(0, 0, 0));         // 배경색 (파란색)
		SetBkMode(hDC, TRANSPARENT);

		if (i == 0)
		{
			wchar_t ready[] = L"Ready!";

			TextOut(hDC, 230, 100, wideName, wcslen(wideName));
			if (m_lobbyData->players[i].bReady)
				TextOut(hDC, 210, 400, ready, wcslen(ready));
		}
		else
		{
			wchar_t ready[] = L"Ready!";
			TextOut(hDC, 630, 100, wideName, wcslen(wideName));
			if (m_lobbyData->players[i].bReady)
				TextOut(hDC, 610, 400, ready, wcslen(ready));
		}
	}
	// 이전 폰트를 복원하고 새 폰트를 삭제
	SelectObject(hDC, hOldFont);
	DeleteObject(hFont);
	RemoveFontResourceEx(L"Font/DungGeunMo.ttf", FR_PRIVATE, nullptr);
}
