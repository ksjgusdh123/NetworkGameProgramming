#include "Heart.h"
#include "..\ObjectAnimation.h"

bool CHeart::Init()
{
	CGameObject::Init();

	SetPivot(0.5f, 0.5f);
	SetObjectType(EObject_Type::Item);
	CreateTexture(1);
	SetTexture("Heart", TEXT("UI/UI.png"), EObject_Dir::Right, ETexture_Type::CIMAGE);
	SetSize(60.f, 57.f);
	SetAnimation(Heart, 1, EObject_State::Basic);

	return true;
}

void CHeart::Update(float elapsedTime)
{
	CGameObject::Update(elapsedTime);
}

void CHeart::PostUpdate(float elapsedTime)
{
	CGameObject::PostUpdate(elapsedTime);
}

void CHeart::Render(HDC hDC, float elapsedTime)
{
	CGameObject::Render(hDC, elapsedTime);
}
