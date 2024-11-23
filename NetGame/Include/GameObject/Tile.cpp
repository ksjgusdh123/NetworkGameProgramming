#include "Tile.h"
#include "Engine.h"
#include "Scene/Scene.h"
#include "Scene/Camera.h"
#include "Scene/SceneManager.h"
#include "Scene/SceneResource.h"
#include <Scene/Scene.h>
#include "Resource/Texture/Texture.h"
#include "Collision.h"


bool CTile::Init()
{
	CGameObject::Init();

	SetPivot(0.5f, 0.5f);
	CreateTexture(16);
	SetTexture("Tile(1)", TEXT("Map/Tile (1).bmp"), ETile_Num::Tile_1, ETexture_Type::Sprite);
	SetTexture("Tile(2)", TEXT("Map/Tile (2).bmp"), ETile_Num::Tile_2, ETexture_Type::Sprite);
	SetTexture("Tile(3)", TEXT("Map/Tile (3).bmp"), ETile_Num::Tile_3, ETexture_Type::Sprite);
	SetTexture("Tile(4)", TEXT("Map/Tile (4).bmp"), ETile_Num::Tile_4, ETexture_Type::Sprite);
	SetTexture("Tile(5)", TEXT("Map/Tile (5).bmp"), ETile_Num::Tile_5, ETexture_Type::Sprite);
	SetTexture("Tile(6)", TEXT("Map/Tile (6).bmp"), ETile_Num::Tile_6, ETexture_Type::Sprite);
	SetTexture("Tile(7)", TEXT("Map/Tile (7).bmp"), ETile_Num::Tile_7, ETexture_Type::Sprite);
	SetTexture("Tile(8)", TEXT("Map/Tile (8).bmp"), ETile_Num::Tile_8, ETexture_Type::Sprite);
	SetTexture("Tile(9)", TEXT("Map/Tile (9).bmp"), ETile_Num::Tile_9, ETexture_Type::Sprite);
	SetTexture("Tile(10)", TEXT("Map/Tile (10).bmp"), ETile_Num::Tile_10, ETexture_Type::Sprite);
	SetTexture("Tile(11)", TEXT("Map/Tile (11).bmp"), ETile_Num::Tile_11, ETexture_Type::Sprite);
	SetTexture("Tile(12)", TEXT("Map/Tile (12).bmp"), ETile_Num::Tile_12, ETexture_Type::Sprite);
	SetTexture("Tile(13)", TEXT("Map/Tile (13).bmp"), ETile_Num::Tile_13, ETexture_Type::Sprite);
	SetTexture("Tile(14)", TEXT("Map/Tile (14).bmp"), ETile_Num::Tile_14, ETexture_Type::Sprite);
	SetTexture("Tile(15)", TEXT("Map/Tile (15).bmp"), ETile_Num::Tile_15, ETexture_Type::Sprite);
	SetTexture("Tile(16)", TEXT("Map/Tile (16).bmp"), ETile_Num::Tile_16, ETexture_Type::Sprite);

	for (int i = 0 ; i < 16; ++i){
	SetColorKey(255.f, 255.f, 255.f, i);
	}
	SetSize(50.f, 50.f);
	
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
#ifdef DEBUG
	m_collisionBox->Render(hDC, elapsedTime);
#endif

	if (m_prevObjectState != m_objectState) {
		m_idx = 0;
		m_time = 0;
	}
	m_prevObjectState = m_objectState;

	Vector2	pos;
	Vector2	cameraPos;
	Vector2	resolution;

	if (m_scene)
	{
		cameraPos = m_scene->GetCamera()->GetPos();
		resolution = m_scene->GetCamera()->GetResolution();
		pos = m_pos - m_scene->GetCamera()->GetPos();
	}

	if (m_texture)
	{
		Vector2	renderLT = pos - m_pivot * m_size;
		Vector2	cullPos = m_pos - m_pivot * m_size;

		if (cullPos.x > cameraPos.x + resolution.x)
			return;

		else if (cullPos.x + m_size.x < cameraPos.x)
			return;

		else if (cullPos.y > cameraPos.y + resolution.y)
			return;

		else if (cullPos.y + m_size.y < cameraPos.y)
			return;



		if (m_texture[(int)m_objectNum - 1]->GetTextureType() == ETexture_Type::Sprite)
		{
			if (m_texture[(int)m_objectNum - 1]->GetEnableColorKey())
			{
				TransparentBlt(hDC, (int)renderLT.x, (int)renderLT.y, (int)m_size.x, (int)m_size.y,
					m_texture[(int)m_objectNum - 1]->GetDC(), 0, 0, 128, 128, m_texture[(int)m_objectNum - 1]->GetColorKey());
			}
			else
			{
				BitBlt(hDC, (int)renderLT.x, (int)renderLT.y, (int)m_size.x, (int)m_size.y,
					m_texture[(int)m_objectNum - 1]->GetDC(), 0, 0, SRCCOPY);
			}
		}
	}
}


void CTile::SetTexture(const std::string& name, const std::wstring& fileName, ETile_Num num, ETexture_Type type, const std::string& pathName)
{
	m_scene->GetSceneResource()->LoadTexture(name, fileName, pathName);

	m_texture[(int)num] = m_scene->GetSceneResource()->FindTexture(name);
	m_texture[(int)num]->SetTextureType(type);
	m_texture[(int)num]->AddRef();
	//SetSize((float)m_texture->GetWidth(), (float)m_texture->GetHeight());
}


bool CTile::SetColorKey(unsigned char r, unsigned char g, unsigned char b, int idx)
{
	if (!m_texture)
		return false;

	m_texture[idx]->SetColorKey(r, g, b);

	return true;
}