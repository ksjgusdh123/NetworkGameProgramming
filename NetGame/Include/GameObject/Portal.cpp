#include "Portal.h"
#include "..\ObjectAnimation.h"

bool CPortal::Init()
{
	CGameObject::Init();

	SetPivot(0.5f, 0.5f);
	CreateTexture(1);
	SetTexture("portal", TEXT("Object/portal.png"), EObject_Dir::Right, ETexture_Type::CIMAGE);

	SetAnimation(Portal, 4, EObject_State::Basic);

	return true;
}

void CPortal::Update(float elapsedTime)
{
	CGameObject::Update(elapsedTime);
}

void CPortal::PostUpdate(float elapsedTime)
{
	CGameObject::PostUpdate(elapsedTime);
}

void CPortal::Render(HDC hDC, float elapsedTime)
{
	CGameObject::Render(hDC, elapsedTime);
}
