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

	m_hButton[0] = CreateWindow(L"button", L"전사", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 220, 450, 100, 100, hwnd, (HMENU)IDC_BUTTON, hInst, NULL);
	m_hButton[1] = CreateWindow(L"button", L"궁수", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 420, 450, 100, 100, hwnd, (HMENU)IDC_BUTTON2, hInst, NULL);
	m_hButton[2] = CreateWindow(L"button", L"준비완료", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 720, 450, 100, 100, hwnd, (HMENU)IDC_BUTTON3, hInst, NULL);

	return true;
}

void CLobbyScene::Update(float elapsedTime)
{
	CScene::Update(elapsedTime);

	switch (clientNum)
	{
	case 1:
		if (!m_LobbyPlayer[0]->GetEnable())
			m_LobbyPlayer[0]->SetEnable(true);
		break;
	case 2:
		if (!m_LobbyPlayer[0]->GetEnable())
			m_LobbyPlayer[0]->SetEnable(true);
		if (!m_LobbyPlayer[1]->GetEnable())
			m_LobbyPlayer[1]->SetEnable(true);
		break;
	default:
		break;
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
		m_lobbyData->players[PacketManager::GetInst().GetMyID()].job = (int)(EPlayer_Job::Sword);
		break;
	}
	case IDC_BUTTON2:
	{
		m_lobbyData->players[PacketManager::GetInst().GetMyID()].job = (int)(EPlayer_Job::Archer);
		break;
	}
	case IDC_BUTTON3:
	{
		m_lobbyData->players[PacketManager::GetInst().GetMyID()].bReady = !m_lobbyData->players[PacketManager::GetInst().GetMyID()].bReady;
		//m_LobbyData[0].bReady = !m_LobbyData[0].bReady;
		//m_LobbyData[1].bReady = !m_LobbyData[1].bReady;
		break;
	}
	}
}

void CLobbyScene::PacketEvent(const Packet& packet)
{
	switch (packet.type)
	{
	case LobbyInfo:
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
