#pragma once

#include "Scene/Scene.h"
#include "Flag.h"
#include "../GameObject/Ghost.h"
#include "../GameObject/Riche.h"
#include "../GameObject/RicheAttack.h"
#include "../GameObject//Tile.h"
// Warning: SceneManager에서 MainScene을 알 수 없기 때문에 씬이 제거될 때 MainScene으로 캐스팅 되지 않는다. (Scene 소멸자만 호출)
//			때문에 할당된 객체는 모두 Scene 클래스에 등록해야 한다. ex) CScene::CreateObject()
//			이거 안하면 메모리 누수.

class CMainScene :
	public CScene
{
public:
	virtual bool Init() override;
	virtual void Update(float elapsedTime);
	bool IsPlayerInRicheAttackArea();

private:
	class CPlayer* player;
	class CGameObject* monster;
	CGhost* ghost;
	CRiche* riche;
	Vector2				m_cameraVelocity;
};

