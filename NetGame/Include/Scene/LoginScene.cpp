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

	CGameObject* back = CreateObject<CGameObject>("eawoi");
	back->CreateTexture(1);
	back->SetTexture("LoginBackground", TEXT("Map/LoginBackground.bmp"), EObject_Dir::Right);
	back->SetSize(960.f, 650.f);
	back->SetPivot(0.f, 0.f);

	HWND hwnd = CEngine::GetInst()->GetWindowHandle();
	HINSTANCE hInst = CEngine::GetInst()->GetWindowInstance();

	m_hEdit = CreateWindow(TEXT("edit"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER, 380, 500, 180, 25, hwnd, (HMENU)IDC_EDIT, hInst, NULL);
	m_hButton = CreateWindow(L"button", L"Ȯ��", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 600, 500, 100, 25, hwnd, (HMENU)IDC_BUTTON, hInst, NULL);

	// ����Ʈ ��Ʈ�ѿ� �ִ� 10�ڸ� �Է��� �� �ֵ��� ����
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
	if (str == "") return;
	// ���� ������ ���� �ް� -> �ߺ� �˻� �� �α��� bool��
	if (SendLoginRequest(str))
	{
		DestroyWindow(m_hEdit);
		DestroyWindow(m_hButton);

		CSceneManager::GetInst()->CreateScene<CLobbyScene>();
	}
}

std::string CLoginScene::EditBoxToString()
{
	// �����ڵ� ���ڿ��� ���� wchar_t �迭 ���
	wchar_t str[11];
	GetDlgItemText(CEngine::GetInst()->GetWindowHandle(), IDC_EDIT, str, 11);

	// wstring�� string���� ��ȯ
	std::wstring wstr(str);
	std::string result(wstr.begin(), wstr.end());

	return result;
}

bool CLoginScene::SendLoginRequest(std::string name)
{
	//if (name.length() <= 0)
	//	return false;

	C_LoginRequestPkt packet(name);
	PacketManager::GetInst().EnqueueSendPacket(packet);

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
