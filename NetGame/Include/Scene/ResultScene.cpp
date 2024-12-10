#include "ResultScene.h"
#include <Engine.h>
#include <Scene/SceneManager.h>
#include "..\NetClient\TCPClient.h"
#include "GameObject.h"

#define IDC_BUTTON 100	
bool CResultScene::Init()
{
	CScene::Init();
	m_myid = PacketManager::GetInst().GetMyID();

	CGameObject* back = CreateObject<CGameObject>("RoomBackground");
	back->CreateTexture(1);
	back->SetTexture("img_result", TEXT("Map/img_result.bmp"), EObject_Dir::Right);
	back->SetSize(960.f, 650.f);
	back->SetPivot(0.f, 0.f);


	m_myid = PacketManager::GetInst().GetMyID();
	m_resultData = PacketManager::GetInst().m_resultData;

	HWND hwnd = CEngine::GetInst()->GetWindowHandle();
	HINSTANCE hInst = CEngine::GetInst()->GetWindowInstance();
	m_hButton = CreateWindow(L"button", L"Finish", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 720, 450, 150, 100, hwnd, (HMENU)IDC_BUTTON, hInst, NULL);
	SendMessage(m_hButton, WM_SETFONT, (WPARAM)m_hFont, TRUE);
	return true;
}

void CResultScene::Update(float elapsedTime)
{
	CScene::Update(elapsedTime);
	UpdateGameData();
}

void CResultScene::Render(HDC hDC, float elapsedTime)
{
	CScene::Render(hDC, elapsedTime);

	HFONT hFont = CreateFontWithSize(m_hFont, 80);
	Vector2 ResultPos = Vector2(480, 250);
	if (m_resultData.bWin)
	{
		wchar_t win[] = L"WIN!!";
		DrawCenteredText(hDC, win, ResultPos, hFont, RGB(100, 100, 255));
	}
	else
	{
		wchar_t lose[] = L"LOSE!!";
		DrawCenteredText(hDC, lose, ResultPos, hFont, RGB(255, 100, 100));
	}

	hFont = CreateFontWithSize(m_hFont, 50);
	Vector2 timePos = Vector2(480, 320);

	int totalSeconds = m_resultData.playTime;
	int hours = totalSeconds / 3600;
	int minutes = (totalSeconds % 3600) / 60;
	int seconds = totalSeconds % 60;
	wchar_t timeText[64];
	swprintf_s(timeText, L"Play Time %02d:%02d:%02d", hours, minutes, seconds);
	DrawCenteredText(hDC, timeText, timePos, hFont, RGB(255, 255, 255));

	DeleteObject(hFont);
}

void CResultScene::KeyEvent(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_BUTTON:
	{
		CEngine::m_loop = false;
		PostQuitMessage(0);
		break;
	}
	}
}

void CResultScene::UpdateGameData()
{
	m_resultData = PacketManager::GetInst().m_resultData;
}
