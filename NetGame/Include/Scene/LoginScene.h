#pragma once
#include "Scene/Scene.h"
class CLoginScene : public CScene
{
public:
	virtual bool Init() override;
	virtual void Update(float elapsedTime);
	virtual void KeyEvent(HWND hWnd, WPARAM wParam, LPARAM lParam);

public:
	void CheckButton();
	std::string EditBoxToString();
	bool SendLoginRequest(std::string name);
private:
	std::string m_playerID;
	HWND		m_hEdit = 0;
	HWND		m_hButton = 0;
};

