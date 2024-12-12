#pragma once

#include "Scene/Scene.h"	
#include "Flag.h"
#include "../GameObject/Ghost.h"
#include "../GameObject/Riche.h"
#include "../GameObject/RicheAttack.h"
#include "../GameObject/Boss.h"
#include "../GameObject//Tile.h"
#include "..\NetClient\TCPClient.h"
// Warning: SceneManager에서 MainScene을 알 수 없기 때문에 씬이 제거될 때 MainScene으로 캐스팅 되지 않는다. (Scene 소멸자만 호출)
//			때문에 할당된 객체는 모두 Scene 클래스에 등록해야 한다. ex) CScene::CreateObject()
//			이거 안하면 메모리 누수.

class CMainScene :
	public CScene
{
public:
	virtual bool Init() override;
	virtual void Update(float elapsedTime);
	virtual void Render(HDC hDC, float elapsedTime);

	void RenderPlayTime(HDC hDC);
	bool IsPlayerInRicheAttackArea();
	void CreateStageOneMap();
	void CreateStageOneItem();
	void ResourceInit();
	bool SendGameData();
	void UpdateGameData();
	void ClientGameData();
	void GameStateCheck(float elapsedTime);
	CMainScene()
	{
		InitializeCriticalSection(&cs);
	}
	~CMainScene()
	{
		DeleteCriticalSection(&cs);
	}
	bool m_bDieCheck = false;
private:
	InGameData m_inGameData;
	class CPlayer* m_inputPlayer;
	class CPlayer* players[2];
	CGhost* ghost;
	CRiche* riche;
	CRicheAttack* richeAttack[10];
	class  CArrow* arrows[2][10];
	Vector2	m_cameraVelocity;

	float m_timer = 0.f;
	float m_deathTime = 0.f;
	bool m_bEnd = false;

	CRITICAL_SECTION cs;
};

