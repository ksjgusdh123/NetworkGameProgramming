#pragma once
#include "Scene/Scene.h"

class CLobbyScene : public CScene
{
public:
	virtual bool Init() override;
	virtual void Update(float elapsedTime);
	virtual void Render(HDC hDC, float elapsedTime);
	virtual void KeyEvent(HWND hWnd, WPARAM wParam, LPARAM lParam);
	virtual bool SendGameData();
public:
	void UpdateGameData();
	void ClientGameData();
	void PrintName(HDC hDC);

private:
	bool m_isMatching = false;
	HWND m_hButton[3]{};
	class CLobbyPlayer* m_LobbyPlayer[2];
	LobbyData m_lobbyData;
};