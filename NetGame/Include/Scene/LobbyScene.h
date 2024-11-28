#pragma once
#include "Scene/Scene.h"

// client ID로 배열 0, 1쓸 지 구분한다고 하였을 시

class CLobbyScene : public CScene
{
public:
	virtual bool Init() override;
	virtual void Update(float elapsedTime);
	virtual void KeyEvent(HWND hWnd, WPARAM wParam, LPARAM lParam);
	virtual void PacketEvent(const class Packet& packet);
	virtual void SendGameData();

private:
	HWND			m_hButton[3]{};
	class CLobbyPlayer* m_LobbyPlayer[2];
	int clientNum = 0;
	char name[2][20];
	LobbyData* m_lobbyData;
};

