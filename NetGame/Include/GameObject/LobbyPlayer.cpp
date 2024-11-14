#include "LobbyPlayer.h"
#include "Scene/Scene.h"
#include "Scene/Camera.h"
#include "Scene/SceneManager.h"
#include "Collision.h"
#include "Resource/Texture/Texture.h"
#include "Scene/SceneResource.h"

bool CLobbyPlayer::Init()
{
	CGameObject::Init();
	CreateTexture(2);
	SetTexture("LobbySword", TEXT("Player/Sword.bmp"), 0, ETexture_Type::Sprite);
	SetTexture("LobbyArcher", TEXT("Player/Archer.bmp"), 1, ETexture_Type::Sprite);
	SetSize(100.f, 200.f);

	for (int i = 0; i < 2; ++i) 
	{
		SetColorKey(255.f, 255.f, 255.f, i);
	}

	return true;
}

void CLobbyPlayer::Update(float elapsedTime)
{
	CGameObject::Update(elapsedTime);
}

void CLobbyPlayer::PostUpdate(float elapsedTime)
{
	CGameObject::PostUpdate(elapsedTime);
}

void CLobbyPlayer::Render(HDC hDC, float elapsedTime)
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

		if (m_texture[(int)m_job]->GetTextureType() == ETexture_Type::Sprite)
		{
			if (m_texture[(int)m_job]->GetEnableColorKey())
			{
				TransparentBlt(hDC, (int)renderLT.x, (int)renderLT.y, (int)m_size.x, (int)m_size.y,
					m_texture[(int)m_job]->GetDC(), 0, 0, (int)m_size.x, (int)m_size.y, m_texture[(int)m_job]->GetColorKey());
			}
			else
			{
				BitBlt(hDC, (int)renderLT.x, (int)renderLT.y, (int)m_size.x, (int)m_size.y,
					m_texture[(int)m_job]->GetDC(), 0, 0, SRCCOPY);
			}
		}
	}
}

bool CLobbyPlayer::SetColorKey(unsigned char r, unsigned char g, unsigned char b, int idx)
{
	if (!m_texture)
		return false;

	m_texture[idx]->SetColorKey(r, g, b);

	return true;
}

void CLobbyPlayer::SetTexture(const std::string& name, const std::wstring& fileName, int num, ETexture_Type type, const std::string& pathName)
{
	m_scene->GetSceneResource()->LoadTexture(name, fileName, pathName);

	m_texture[(int)num] = m_scene->GetSceneResource()->FindTexture(name);
	m_texture[(int)num]->SetTextureType(type);
	m_texture[(int)num]->AddRef();
}
