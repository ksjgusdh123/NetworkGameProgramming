#include "LoginScene.h"
#include <GameObject.h>
#include <Input.h>
#include <Engine.h>
#include <Scene/SceneManager.h>
#include "MainScene.h"
#include "LobbyScene.h"
#include "..\NetClient\TCPClient.h"

#define IDC_BUTTON 100
#define IDC_EDIT 101

bool CLoginScene::Init()
{
	CScene::Init();
	m_sceneType = LOGIN_SCENE;
	m_myid = PacketManager::GetInst().GetMyID();

	CGameObject* back = CreateObject<CGameObject>("eawoi");
	back->CreateTexture(1);
	back->SetTexture("LoginBackground", TEXT("Map/LoginBackground.bmp"), EObject_Dir::Right);
	back->SetSize(960.f, 650.f);
	back->SetPivot(0.f, 0.f);

	HWND hwnd = CEngine::GetInst()->GetWindowHandle();
	HINSTANCE hInst = CEngine::GetInst()->GetWindowInstance();

	m_hEdit = CreateWindow(TEXT("edit"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER, 380, 500, 180, 25, hwnd, (HMENU)IDC_EDIT, hInst, NULL);
	m_hButton = CreateWindow(L"button", L"È®ÀÎ", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 600, 500, 100, 25, hwnd, (HMENU)IDC_BUTTON, hInst, NULL);

	SendMessage(m_hEdit, EM_SETLIMITTEXT, 10, 0);
	return true;
}

void CLoginScene::Update(float elapsedTime)
{
	CScene::Update(elapsedTime);
	if(isSendLogin)
	{
		DestroyWindow(m_hEdit);
		DestroyWindow(m_hButton);

		CSceneManager::GetInst()->CreateScene<CLobbyScene>();
	}
}

bool CLoginScene::CheckButton()
{
	m_myName = EditBoxToString();
	if (m_myName == "")
		return false;
	else
		return true;
}

std::string CLoginScene::EditBoxToString()
{
	wchar_t str[11];
	GetDlgItemText(CEngine::GetInst()->GetWindowHandle(), IDC_EDIT, str, 11);

	std::wstring wstr(str);
	std::string result(wstr.begin(), wstr.end());

	return result;
}

bool CLoginScene::SendLoginRequest(std::string name)
{
	int i = PacketManager::GetInst().GetMyID();
	char* myName = PacketManager::GetInst().m_lobbyData.players[PacketManager::GetInst().GetMyID()].name;
	memcpy(myName, name.c_str(), name.length());

	C_LoginRequestPkt packet(name);
	return PacketManager::GetInst().SendPacket(packet);
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

bool CLoginScene::SendGameData()
{
	if (m_myName == "") return false;
	if (SendLoginRequest(m_myName))
	{
		isSendLogin = true;
		PacketManager::GetInst().SetMyName(m_myName);
	}
	return true;
}
