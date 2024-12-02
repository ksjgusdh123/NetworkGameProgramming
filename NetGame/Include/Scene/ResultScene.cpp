#include "ResultScene.h"
#include <Engine.h>
#include <Scene/SceneManager.h>
#include "..\NetClient\TCPClient.h"
#include "GameObject.h"

#define IDC_BUTTON 100	
bool CResultScene::Init()
{
	CScene::Init();

	CGameObject* back = CreateObject<CGameObject>("RoomBackground");
	back->CreateTexture(1);
	back->SetTexture("LobbyBackground", TEXT("Map/RoomBackground.bmp"), EObject_Dir::Right);
	back->SetSize(960.f, 650.f);
	back->SetPivot(0.f, 0.f);


	m_myid = PacketManager::GetInst().GetMyID();
	m_resultData = &PacketManager::GetInst().resultData;

	CSceneManager* manager = CSceneManager::GetInst();
	m_resultData->bWin = manager->m_bWin;
	m_resultData->playTime = manager->m_playTime;


	AddFontResourceEx(L"Font/DungGeunMo.ttf", FR_PRIVATE, nullptr);
	// 폰트 생성
	HFONT hFont = CreateFont(
		35, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"DungGeunMo"//글꼴 적용하고 싶은데.. 경로가 이게 아닌가?
	);

	HWND hwnd = CEngine::GetInst()->GetWindowHandle();
	HINSTANCE hInst = CEngine::GetInst()->GetWindowInstance();
	m_hButton = CreateWindow(L"button", L"종료", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 720, 450, 150, 100, hwnd, (HMENU)IDC_BUTTON, hInst, NULL);
	SendMessage(m_hButton, WM_SETFONT, (WPARAM)hFont, TRUE);
	return true;
}

void CResultScene::Update(float elapsedTime)
{
	CScene::Update(elapsedTime);
}

void CResultScene::Render(HDC hDC, float elapsedTime)
{
	CScene::Render(hDC, elapsedTime);
	AddFontResourceEx(L"Font/DungGeunMo.ttf", FR_PRIVATE, nullptr);
	HFONT hFont = CreateFont(
		40, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"DungGeunMo");
	HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);

	SetTextColor(hDC, RGB(255, 0, 0));   // 텍스트 색상 (흰색)
	SetBkColor(hDC, RGB(0, 0, 0));         // 배경색 (파란색)
	SetBkMode(hDC, TRANSPARENT);
	
	if (m_resultData->bWin)
	{
		wchar_t win[] = L"WIN!!";
		TextOut(hDC, 430, 100, win, wcslen(win));
	}
	else
	{
		wchar_t lose[] = L"LOSE!!";
		TextOut(hDC, 430, 100, lose, wcslen(lose));
	}

	wchar_t buffer[50];
	swprintf(buffer, 50, L"play time: %.3f", m_resultData->playTime);
	std::wstring time = buffer;
	TextOut(hDC, 330, 400, time.c_str(), (int)time.size());


	SelectObject(hDC, hOldFont);
	DeleteObject(hFont);
	RemoveFontResourceEx(L"Font/DungGeunMo.ttf", FR_PRIVATE, nullptr);
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
