#pragma once
#include "Scene/Scene.h"

// client ID로 배열 0, 1쓸 지 구분한다고 하였을 시

class CLobbyScene : public CScene
{
public:
	virtual bool Init() override;
	virtual void Update(float elapsedTime);
	virtual void KeyEvent(HWND hWnd, WPARAM wParam, LPARAM lParam);

public:
	void SendLobbyData();
	void ReceiveLobbyData();	

private:
	//std::string m_name[2];
	HWND			m_hButton[3]{};
	EPlayer_Job		m_job[2];
	bool			m_bReady[2];
	CGameObject*	m_object[2];
	class CLobbyPlayer* m_player[2];
};

