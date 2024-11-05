#include "Ghost.h"
#include "Engine.h"
#include <Scene/Scene.h>
#include "Resource/Texture/Texture.h"
#include "..\MonsterAnimation.h"

bool CGhost::Init()
{
	CGameObject::Init();

	SetPivot(0.5f, 0.5f);
	SetVelocity(150.f, 300.f);
	CreateTexture(2);
	SetTexture("Ghost_L", TEXT("Monster/Ghost_L.png"), EObject_Dir::Left, ETexture_Type::CIMAGE);
	SetTexture("Ghost_R", TEXT("Monster/Ghost_R.png"), EObject_Dir::Right, ETexture_Type::CIMAGE);
	SetAnimation(Ghost_L, 6, EObject_State::Walk_L, ETexture_Type::Sprite);
	SetAnimation(Ghost_R, 6, EObject_State::Walk, ETexture_Type::Sprite);
	SetSize(43.f, 61.f);
	SetColorKey(180, 213, 255);
	SetPos(200.f, 100.f);

	m_hp = 100;
	m_objectDir = EObject_Dir::Right;
	m_objectState = EObject_State::Walk;
	m_bIsAlive = true;

	return true;
}

void CGhost::Update(float elapsedTime)
{
	CGameObject::Update(elapsedTime);
}

void CGhost::PostUpdate(float elapsedTime)
{
	CGameObject::PostUpdate(elapsedTime);
}

void CGhost::Render(HDC hDC, float elapsedTime)
{
	CGameObject::Render(hDC, elapsedTime);
}

