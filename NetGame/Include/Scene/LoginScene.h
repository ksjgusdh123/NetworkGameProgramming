#pragma once
#include "Scene/Scene.h"
class CLoginScene : public CScene
{
public:
	virtual bool Init() override;
	virtual void Update(float elapsedTime);
	virtual void KeyEvent(HWND hWnd, WPARAM wParam, LPARAM lParam);
	virtual bool SendGameData();
public:
	bool CheckButton();
	std::string EditBoxToString();
	bool SendLoginRequest(std::string name);
private:
	HWND		m_hEdit = 0;
	HWND		m_hButton = 0;
	bool isSendLogin = false;
	std::string m_myName ="";
};

