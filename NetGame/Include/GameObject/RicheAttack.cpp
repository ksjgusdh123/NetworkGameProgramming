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
	SetEnable(false);
	m_objectDir = EObject_Dir::Right;
	m_objectState = EObject_State::Attack;
	m_timer = 0.f;
	return true;
}

void CRicheAttack::Update(float elapsedTime)
{
	CGameObject::Update(elapsedTime);

}

void CRicheAttack::PostUpdate(float elapsedTime)
{
	CGameObject::PostUpdate(elapsedTime);
}

void CRicheAttack::Render(HDC hDC, float elapsedTime)
{
	CGameObject::Render(hDC, elapsedTime);
}

