#pragma once
#include "Scene/Scene.h"

class CLobbyScene : public CScene
{
public:
	virtual bool Init() override;
	virtual void Update(float elapsedTime);
	virtual void Render(HDC hDC, float elapsedTime);
	virtual void KeyEvent(HWND hWnd, WPARAM wParam, LPARAM lParam);
	virtual void RecvGameData(const class Packet& packet);
	virtual void SendGameData();

public:
	void PrintName(HDC hDC);

private:
	HWND m_hButton[3]{};
	class CLobbyPlayer* m_LobbyPlayer[2];
	LobbyData* m_lobbyData;
};

