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
	back->SetTexture("LoginBackground", TEXT("Map/Login.bmp"), EObject_Dir::Right);
	back->SetSize(960.f, 650.f);
	back->SetPivot(0.f, 0.f);

	HWND hwnd = CEngine::GetInst()->GetWindowHandle();
	HINSTANCE hInst = CEngine::GetInst()->GetWindowInstance();

	m_hButton[0] = CreateWindow(L"button1", L"확인", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 720, 150, 100, 100, hwnd, (HMENU)IDC_BUTTON, hInst, NULL);
	m_hButton[1] = CreateWindow(L"button2", L"확인", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 720, 250, 100, 100, hwnd, (HMENU)IDC_BUTTON2, hInst, NULL);
	m_hButton[2] = CreateWindow(L"button3", L"확인", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 720, 450, 100, 100, hwnd, (HMENU)IDC_BUTTON3, hInst, NULL);

	return true;
}

void CLobbyScene::Update(float elapsedTime)
{
	CScene::Update(elapsedTime);
}

void CLobbyScene::KeyEvent(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	//switch (LOWORD(wParam))
	//{
	//case IDC_BUTTON:
	//	break;
	//case IDC_BUTTON2:
	//	break;
	//case IDC_BUTTON3:
	//	break;
	//}
}
