#include "Arrow.h"
#include "..\ObjectAnimation.h"

bool CArrow::Init()
{
	CGameObject::Init();
	SetPivot(0.5f, 0.5f);
	SetVelocity(400.f, 10.f);
	CreateTexture(2);
	SetTexture("Player", TEXT("Player/player.png"), EObject_Dir::Right, ETexture_Type::CIMAGE);
	SetTexture("Player_L", TEXT("Player/player_L.png"), EObject_Dir::Left, ETexture_Type::CIMAGE);
	SetAnimation(Arrow, 1, EObject_State::Basic);
	SetAnimation(Arrow_L, 1, EObject_State::Basic_L);
	SetSize(40, 10);	
	m_timer = 0;
	SetEnable(false);
	return true;
}

void CArrow::Update(float elapsedTime)
{
	CGameObject::Update(elapsedTime);
	
}

void CArrow::PostUpdate(float elapsedTime)
{
	CGameObject::PostUpdate(elapsedTime);
}

void CArrow::Render(HDC hDC, float elapsedTime)
{
	CGameObject::Render(hDC, elapsedTime);
}
