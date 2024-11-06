#include "Tile.h"
#include "Engine.h"
#include <Scene/Scene.h>
#include "Resource/Texture/Texture.h"


bool CTile::Init()
{
	CGameObject::Init();

	SetPivot(0.5f, 0.5f);
	CreateTexture(1);
	SetTexture("Tile(2)", TEXT("Map/Tile (2).bmp"), EObject_Dir::Right, ETexture_Type::Sprite);
	SetSize(90.f, 90.f);
	
	m_objectDir = EObject_Dir::Right;
	return true;
}

void CTile::Update(float elapsedTime)
{
	CGameObject::Update(elapsedTime);
}

void CTile::PostUpdate(float elapsedTime)
{
	CGameObject::PostUpdate(elapsedTime);
}

void CTile::Render(HDC hDC, float elapsedTime)
{
	CGameObject::Render(hDC, elapsedTime);
}
