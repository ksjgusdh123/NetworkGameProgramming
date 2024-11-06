#include "RicheAttack.h"
#include "Engine.h"
#include <Scene/Scene.h>
#include "Resource/Texture/Texture.h"
#include "..\MonsterAnimation.h"
#define min(a,b)            (((a) < (b)) ? (a) : (b))

bool CRicheAttack::Init()
{
	CGameObject::Init();

	SetPivot(0.5f, 0.5f);
	SetVelocity(0.1f, 0.f);
	CreateTexture(1);
	SetTexture("Riche_Attack", TEXT("Monster/Ghost_L.png"), EObject_Dir::Right, ETexture_Type::CIMAGE);
	SetAnimation(Riche_Attack, 2, EObject_State::Attack);
	SetSize(90.f, 90.f);
	
	m_objectDir = EObject_Dir::Right;
	m_objectState = EObject_State::Attack;
	m_timer = 0.f;
	return true;
}

void CRicheAttack::Update(float elapsedTime)
{
	CGameObject::Update(elapsedTime);

	float dx = m_target.x - m_pos.x;
	float dy = m_target.y - m_pos.y;
	float distance = sqrt(dx * dx + dy * dy);

	if (distance > 0.0f) { // 거리 계산
		float velocityFactor = min(distance, m_velocity.x); // distance와 x 방향 속도 중 작은 값 선택
		m_pos.x += (dx / distance) * velocityFactor; // x축 이동
		m_pos.y += (dy / distance) * velocityFactor; // y축 이동
	}

	m_timer += elapsedTime;
	if (m_timer >= 3.0f)
		Destroy();

	if (distance < 10)
		Destroy();
}

void CRicheAttack::PostUpdate(float elapsedTime)
{
	CGameObject::PostUpdate(elapsedTime);
}

void CRicheAttack::Render(HDC hDC, float elapsedTime)
{
	CGameObject::Render(hDC, elapsedTime);
}

