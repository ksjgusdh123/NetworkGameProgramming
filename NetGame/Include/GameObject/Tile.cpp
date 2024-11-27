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


		if (m_texture[0]->GetTextureType() == ETexture_Type::Sprite)
		{
			if (m_texture[0]->GetEnableColorKey())
			{
				int size{ 128 };
				if ((int)m_objectNum == 17) 
					size = 106;
			

				TransparentBlt(hDC, (int)renderLT.x, (int)renderLT.y, (int)m_size.x, (int)m_size.y,
					m_texture[0]->GetDC(), 0, 0, size, size, m_texture[0]->GetColorKey());
			}
		}
	}
}


void CTile::SetTexture(const std::string& name, const std::wstring& fileName, ETile_Num num, ETexture_Type type, const std::string& pathName)
{
	CreateTexture(1);
	m_scene->GetSceneResource()->LoadTexture(name, fileName, pathName);

	m_texture[0] = m_scene->GetSceneResource()->FindTexture(name);
	m_texture[0]->SetTextureType(type);
	m_texture[0]->AddRef();

	SetColorKey(255.f, 255.f, 255.f, 0);

}

void CTile::SettingTile(int num)
{

}



void CTile::SetTileNum(int num)
{
	m_objectNum = (ETile_Num)num;

	std::wstring filePath = L"Map/Tile (" + std::to_wstring(num) + L").bmp";
	std::string textureName = "Tile(" + std::to_string(num) + ")";

	SetTexture(textureName.c_str(), filePath.c_str(), m_objectNum, ETexture_Type::Sprite);
}

