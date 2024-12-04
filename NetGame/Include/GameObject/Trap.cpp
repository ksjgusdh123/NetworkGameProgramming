#include "Trap.h"

bool CTrap::Init()
{
	CGameObject::Init();

	SetPivot(0.5f, 0.5f);
	SetObjectType(EObject_Type::Item);
	CreateTexture(1);
	SetTexture("Trap", TEXT("Map/DeadBush.png"), EObject_Dir::Right, ETexture_Type::CIMAGE);
	SetSize(132.f, 74.f);

	RECT rect{ 0, 0, 132, 74 };
	SetAnimation(&rect, 1, EObject_State::Basic);

	return true;
}

void CTrap::Update(float elapsedTime)
{
	CGameObject::Update(elapsedTime);
}

void CTrap::PostUpdate(float elapsedTime)
{
	CGameObject::PostUpdate(elapsedTime);
}

void CTrap::Render(HDC hDC, float elapsedTime)
{
	CGameObject::Render(hDC, elapsedTime);
}
