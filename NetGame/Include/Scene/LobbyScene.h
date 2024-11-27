#pragma once
#include "Scene/Scene.h"

// client ID로 배열 0, 1쓸 지 구분한다고 하였을 시

struct LobbyData
{
	EPlayer_Job job = EPlayer_Job::Sword;
	bool bReady = false;
	int id;
	class CLobbyPlayer* player = NULL;
};

class CLobbyScene : public CScene
{
public:
	virtual bool Init() override;
	virtual void Update(float elapsedTime);
	virtual void Render(HDC hDC, float elapsedTime);
	virtual void KeyEvent(HWND hWnd, WPARAM wParam, LPARAM lParam);
	virtual void PacketEvent(const class Packet& packet);

public:
	void SendLobbyData();
	void ReceiveLobbyData();	

private:
	//std::string m_name[2];
	HWND			m_hButton[3]{};
	LobbyData       m_LobbyData[2];
	int clientNum = 0;
	char name[2][20];
};

