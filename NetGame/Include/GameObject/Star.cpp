#include "Star.h"

bool CStar::Init()
{
	CGameObject::Init();

	SetPivot(0.5f, 0.5f);
	CreateTexture(1);
	SetTexture("star", TEXT("Object/star.bmp"), EObject_Dir::Right, ETexture_Type::Sprite);
	SetSize(60.f, 57.f);
	SetColorKey(255, 255, 255);

	return true;
}

void CStar::Update(float elapsedTime)
{
	CGameObject::Update(elapsedTime);
}

void CStar::PostUpdate(float elapsedTime)
{
	CGameObject::PostUpdate(elapsedTime);
}

void CStar::Render(HDC hDC, float elapsedTime)
{
	CGameObject::Render(hDC, elapsedTime);
}
