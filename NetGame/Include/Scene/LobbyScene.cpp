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
	m_mateId = abs(1 - m_myid);
	string myName = PacketManager::GetInst().GetMyName();
	memcpy(m_lobbyData.players[m_myid].name, myName.c_str(), myName.length());

	CGameObject* back = CreateObject<CGameObject>("RoomBackground");
	back->CreateTexture(1);
	back->SetTexture("LobbyBackground", TEXT("Map/RoomBackground.bmp"), EObject_Dir::Right);
	back->SetSize(960.f, 650.f);
	back->SetPivot(0.f, 0.f);

	m_LobbyPlayer[0] = CreateObject<CLobbyPlayer>("Player1");
	m_LobbyPlayer[0]->SetSize(100.f, 200.f);
	m_LobbyPlayer[0]->SetPos(270.f, 300.f);
	m_LobbyPlayer[0]->SetEnable(false);

	m_LobbyPlayer[1] = CreateObject<CLobbyPlayer>("Player2");
	m_LobbyPlayer[1]->SetSize(100.f, 200.f);
	m_LobbyPlayer[1]->SetPos(680.f, 300.f);
	m_LobbyPlayer[1]->SetEnable(false);

	HWND hwnd = CEngine::GetInst()->GetWindowHandle();
	HINSTANCE hInst = CEngine::GetInst()->GetWindowInstance();

	m_hButton[0] = CreateWindow(L"button", L"전사", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 220, 450, 150, 80, hwnd, (HMENU)IDC_BUTTON, hInst, NULL);
	m_hButton[1] = CreateWindow(L"button", L"궁수", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 420, 450, 150, 80, hwnd, (HMENU)IDC_BUTTON2, hInst, NULL);
	m_hButton[2] = CreateWindow(L"button", L"준비완료", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 720, 450, 150, 100, hwnd, (HMENU)IDC_BUTTON3, hInst, NULL);
	SendMessage(m_hButton[0], WM_SETFONT, (WPARAM)m_hFont, TRUE);
	SendMessage(m_hButton[1], WM_SETFONT, (WPARAM)m_hFont, TRUE);
	SendMessage(m_hButton[2], WM_SETFONT, (WPARAM)m_hFont, TRUE);
	return true;
}

void CLobbyScene::Update(float elapsedTime)
{
	CScene::Update(elapsedTime);
	UpdateGameData();
	if (m_lobbyData.scene == GAMESCENE)
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
		m_lobbyData.players[m_myid].job = (EPlayer_Job::Sword);
		break;
	}
	case IDC_BUTTON2:
	{
		m_lobbyData.players[m_myid].job = (EPlayer_Job::Archer);
		break;
	}
	case IDC_BUTTON3:
	{
		m_lobbyData.players[m_myid].bReady = !m_lobbyData.players[m_myid].bReady;
		break;
	}
	}
}

bool CLobbyScene::SendGameData()
{
	C_LobbyUpdateRequest sendPacket(m_lobbyData.players[m_myid]);
	return PacketManager::GetInst().SendPacket(sendPacket);
}

void CLobbyScene::UpdateGameData()
{
	m_lobbyData.players[m_mateId] = PacketManager::GetInst().m_lobbyData.players[m_mateId];
	m_lobbyData.scene = PacketManager::GetInst().m_lobbyData.scene;

	for (int i = 0; i < 2; ++i)
	{
		m_LobbyPlayer[i]->SetJob((EPlayer_Job)(int)m_lobbyData.players[i].job);
		m_LobbyPlayer[i]->SetName(m_lobbyData.players[i].name);
		if (strcmp(m_lobbyData.players[i].name, ""))
		{
			m_LobbyPlayer[i]->SetEnable(true);
		}
	}
}

void CLobbyScene::ClientGameData()
{

}

void CLobbyScene::PrintName(HDC hDC)
{
	HFONT hFont = CreateFontWithSize(m_hFont, 40);

	for (int i = 0; i < 2; ++i)
	{
		wchar_t wideName[NAME_LEN] = {};
		MultiByteToWideChar(CP_ACP, 0, m_lobbyData.players[i].name, -1, wideName, NAME_LEN);

		wchar_t ready[] = L"Ready!";

		Vector2 namePos = m_LobbyPlayer[i]->GetPos();
		namePos.y -= m_LobbyPlayer[i]->GetSize().y / 2 + 80;
		Vector2 readyPos = m_LobbyPlayer[i]->GetPos();
		readyPos.y += m_LobbyPlayer[i]->GetSize().y / 2 + 10;

		DrawCenteredText(hDC, wideName, namePos, hFont, RGB(255, 255, 255));

		if (m_lobbyData.players[i].bReady)
		{
			DrawCenteredText(hDC, ready, readyPos, hFont, RGB(0, 255, 0)); // Ready 상태는 녹색으로 표시
		}
	}

	DeleteObject(hFont);
}

