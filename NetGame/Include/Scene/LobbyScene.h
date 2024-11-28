#pragma once
#include "Scene/Scene.h"

// client ID로 배열 0, 1쓸 지 구분한다고 하였을 시

class CLobbyScene : public CScene
{
public:
	virtual bool Init() override;
	virtual void Update(float elapsedTime);
	virtual void KeyEvent(HWND hWnd, WPARAM wParam, LPARAM lParam);
	virtual void RecvGameData(const class Packet& packet);
	virtual void SendGameData();

private:
	HWND m_hButton[3]{};
	HWND m_hPlayerName[2];
	class CLobbyPlayer* m_LobbyPlayer[2];
	LobbyData* m_lobbyData;
};

