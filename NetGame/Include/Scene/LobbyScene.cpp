#include "LobbyScene.h"
#include <GameObject.h>
#include <Input.h>
#include <Engine.h>
#include <Scene/SceneManager.h>
#include "MainScene.h"

#define IDC_BUTTON 100	
#define IDC_BUTTON2 101	
#define IDC_BUTTON3 102	


bool CLobbyScene::Init()
{
	CScene::Init();

	CGameObject* back = CreateObject<CGameObject>("eawoi");
	back->CreateTexture(1);
	back->SetTexture("LobbyBackground", TEXT("Map/RoomBackground.bmp"), EObject_Dir::Right);
	back->SetSize(960.f, 650.f);
	back->SetPivot(0.f, 0.f);

	HWND hwnd = CEngine::GetInst()->GetWindowHandle();
	HINSTANCE hInst = CEngine::GetInst()->GetWindowInstance();

	m_hButton[0] = CreateWindow(L"button", L"확인", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 720, 150, 100, 100, hwnd, (HMENU)IDC_BUTTON, hInst, NULL);
	m_hButton[1] = CreateWindow(L"button", L"확인", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 720, 250, 100, 100, hwnd, (HMENU)IDC_BUTTON2, hInst, NULL);
	m_hButton[2] = CreateWindow(L"button", L"확인", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 720, 450, 100, 100, hwnd, (HMENU)IDC_BUTTON3, hInst, NULL);

	return true;
}

void CLobbyScene::Update(float elapsedTime)
{
	CScene::Update(elapsedTime);

	if (m_bReady[0] && m_bReady[1])
	{
		for (int i = 0; i < 3; ++i)
		{
			DestroyWindow(m_hButton[i]);
			m_hButton[i] = NULL;
		}
		if(m_hButton[2] == NULL)
		CSceneManager::GetInst()->CreateScene<CMainScene>();
	}
}

void CLobbyScene::KeyEvent(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_BUTTON:
		m_job[0] = EPlayer_Job::Sword;
		break;
	case IDC_BUTTON2:
		m_job[0] = EPlayer_Job::Archer;
		break;
	case IDC_BUTTON3:
		m_bReady[0] = !m_bReady[0];
		m_bReady[1] = !m_bReady[1];
		break;
	}
}

void CLobbyScene::SendLobbyData()
{
}

void CLobbyScene::ReceiveLobbyData()
{
}
