#include "Riche.h"
#include "Engine.h"
#include <Scene/Scene.h>
#include "Resource/Texture/Texture.h"
#include "..\MonsterAnimation.h"

bool CRiche::Init()
{
	CGameObject::Init();

	SetPivot(0.5f, 0.5f);
	SetVelocity(50.f, 0.f);
	CreateTexture(2);
	SetTexture("Riche_L", TEXT("Monster/Riche_L.png"), EObject_Dir::Left, ETexture_Type::CIMAGE);
	SetTexture("Riche_R", TEXT("Monster/Riche_R.png"), EObject_Dir::Right, ETexture_Type::CIMAGE);
	SetAnimation(Riche_L, 6, EObject_State::Basic_L, ETexture_Type::Sprite);
	SetAnimation(Riche_R, 6, EObject_State::Basic, ETexture_Type::Sprite);
	SetSize(107.f, 139.f);
	SetColorKey(180, 213, 255);
	
	m_hp = 100;
	m_objectDir = EObject_Dir::Left;
	m_objectState = EObject_State::Basic_L;
	m_bIsAlive = true;

	return true;
}

void CRiche::Update(float elapsedTime)
{
	CGameObject::Update(elapsedTime);
}

void CRiche::PostUpdate(float elapsedTime)
{
	CGameObject::PostUpdate(elapsedTime);
}

void CRiche::Render(HDC hDC, float elapsedTime)
{
	CGameObject::Render(hDC, elapsedTime);
}

