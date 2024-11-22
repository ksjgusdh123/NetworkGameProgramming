#include "LoginScene.h"
#include <GameObject.h>
#include <Input.h>
#include <Engine.h>
#include <Scene/SceneManager.h>
#include "MainScene.h"
#include "LobbyScene.h"
#include "Protocol.h"
#include "..\NetClient\TCPClient.h"

#define IDC_BUTTON 100
#define IDC_EDIT 101

bool CLoginScene::Init()
{
	CScene::Init();

	CGameObject* back = CreateObject<CGameObject>("eawoi");
	back->CreateTexture(1);
	back->SetTexture("LoginBackground", TEXT("Map/LoginBackground.bmp"), EObject_Dir::Right);
	back->SetSize(960.f, 650.f);
	back->SetPivot(0.f, 0.f);

	HWND hwnd = CEngine::GetInst()->GetWindowHandle();
	HINSTANCE hInst = CEngine::GetInst()->GetWindowInstance();

	m_hEdit = CreateWindow(TEXT("edit"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER, 380, 500, 180, 25, hwnd, (HMENU)IDC_EDIT, hInst, NULL);
	m_hButton = CreateWindow(L"button", L"확인", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 600, 500, 100, 25, hwnd, (HMENU)IDC_BUTTON, hInst, NULL);

	// 에디트 컨트롤에 최대 10자만 입력할 수 있도록 설정
	SendMessage(m_hEdit, EM_SETLIMITTEXT, 10, 0);

	return true;
}

void CLoginScene::Update(float elapsedTime)
{
	CScene::Update(elapsedTime);
}

void CLoginScene::CheckButton()
{
	std::string str = EditBoxToString();
	// 서버 보내고 정보 받고 -> 중복 검사 후 로그인 bool값
	if (SendLoginRequest(str))
	{
		DestroyWindow(m_hEdit);
		DestroyWindow(m_hButton);

		CSceneManager::GetInst()->CreateScene<CLobbyScene>();
	}
}

std::string CLoginScene::EditBoxToString()
{
	// 유니코드 문자열을 위한 wchar_t 배열 사용
	wchar_t str[11];
	GetDlgItemText(CEngine::GetInst()->GetWindowHandle(), IDC_EDIT, str, 11);

	// wstring을 string으로 변환
	std::wstring wstr(str);
	std::string result(wstr.begin(), wstr.end());

	return result;
}

bool CLoginScene::SendLoginRequest(std::string name)
{
	//if (name.length() <= 0)
	//	return false;

	C_LoginRequestPkt packet(name);
	TCPClient::GetInst()->SendPacket(&packet);

	return true;
}

void CLoginScene::KeyEvent(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_BUTTON:
		CheckButton();
		break;
	}
}
