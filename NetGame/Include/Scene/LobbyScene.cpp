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

	
	CGameObject* back = CreateObject<CGameObject>("RoomBackground");
	back->CreateTexture(1);
	back->SetTexture("LobbyBackground", TEXT("Map/RoomBackground.bmp"), EObject_Dir::Right);
	back->SetSize(960.f, 650.f);
	back->SetPivot(0.f, 0.f);

	m_LobbyData[0].player = CreateObject<CLobbyPlayer>("Player1");
	m_LobbyData[0].player->SetSize(100.f, 200.f);
	m_LobbyData[0].player->SetPos(270.f, 300.f);
	m_LobbyData[0].player->SetEnable(true);
	
	m_LobbyData[1].player = CreateObject<CLobbyPlayer>("Player2");
	m_LobbyData[1].player->SetSize(100.f, 200.f);
	m_LobbyData[1].player->SetPos(680.f, 300.f);
	m_LobbyData[1].player->SetEnable(true);

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
		if(!m_LobbyData[0].player->GetEnable())
			m_LobbyData[0].player->SetEnable(true);
		break;
	case 2:
		if (!m_LobbyData[0].player->GetEnable())
			m_LobbyData[0].player->SetEnable(true);
		if (!m_LobbyData[1].player->GetEnable())
			m_LobbyData[1].player->SetEnable(true);
		break;
	default:
		break;
	}

	if (m_LobbyData[0].bReady && m_LobbyData[1].bReady)
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
	{
		C_PlayerChoicePkt packet{};
		m_LobbyData[PacketManager::GetInst().GetMyID()].player->SetJob(EPlayer_Job::Sword);
		packet.initial(PacketManager::GetInst().GetMyID(), (int)EPlayer_Job::Sword);
		PacketManager::GetInst().EnqueueSendPacket(packet);
		break;
	}
	case IDC_BUTTON2:
	{
		C_PlayerChoicePkt packet{};
		m_LobbyData[PacketManager::GetInst().GetMyID()].player->SetJob(EPlayer_Job::Archer);
		packet.initial(PacketManager::GetInst().GetMyID(), (int)EPlayer_Job::Archer);
		PacketManager::GetInst().EnqueueSendPacket(packet);
		break;
	}
	case IDC_BUTTON3:
	{
		//m_LobbyData[0].bReady = !m_LobbyData[0].bReady;
		//m_LobbyData[1].bReady = !m_LobbyData[1].bReady;
		m_LobbyData[PacketManager::GetInst().GetMyID()].bReady = !m_LobbyData[PacketManager::GetInst().GetMyID()].bReady;
		C_GameStartRequestPkt packet{};
		packet.initial(PacketManager::GetInst().GetMyID(), m_LobbyData[PacketManager::GetInst().GetMyID()].bReady);
		PacketManager::GetInst().EnqueueSendPacket(packet);
		break;
	}
	}
}

void CLobbyScene::PacketEvent(const Packet& packet)
{
	switch (packet.type)
	{
	case PlayerChoice:
	{
		C_PlayerChoicePkt* cur = (C_PlayerChoicePkt*)&packet;
		cur->deserialize();
		m_LobbyData[cur->idx].player->SetJob((EPlayer_Job)cur->j);
		break;
	}
	case GameStartRequest:
	{
		C_GameStartRequestPkt* cur = (C_GameStartRequestPkt*)&packet;
		cur->deserialize();
		m_LobbyData[cur->idx].bReady = cur->ready;
		break;
	}
	default:
		break;
	}
}

void CLobbyScene::SendLobbyData()
{
}

void CLobbyScene::ReceiveLobbyData()
{
}
