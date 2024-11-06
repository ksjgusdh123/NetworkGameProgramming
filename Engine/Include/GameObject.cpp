#include "GameObject.h"
#include "Scene/Scene.h"
#include "Scene/Camera.h"
#include "Scene/SceneManager.h"
#include "Scene/SceneResource.h"
#include "Resource/Texture/Texture.h"
#include "Engine.h"

CGameObject::~CGameObject()
{
	if (m_texture)
	{
		delete[] m_texture;
		m_texture = nullptr;
	}
}

void CGameObject::SetTexture(const std::string& name, const std::wstring& fileName, EObject_Dir dir, ETexture_Type type, const std::string& pathName)
{
    m_scene->GetSceneResource()->LoadTexture(name, fileName, pathName);

    m_texture[(int)dir] = m_scene->GetSceneResource()->FindTexture(name);
	m_texture[(int)dir]->SetTextureType(type);
	m_texture[(int)dir]->AddRef();
    //SetSize((float)m_texture->GetWidth(), (float)m_texture->GetHeight());
}

void CGameObject::SetAnimation(RECT* rects, int num, EObject_State state, ETexture_Type type)
{
	for (int i = 0; i < num; ++i)
	{
		m_animationBox[(int)state].push_back(rects[i]);
	}
}

bool CGameObject::SetColorKey(unsigned char r, unsigned char g, unsigned char b, int idx)
{
    if (!m_texture)
        return false;

    m_texture[(int)m_objectDir]->SetColorKey(r, g, b, idx);

    return true;
}

bool CGameObject::Init()
{
	return true;
}

void CGameObject::Update(float elapsedTime)
{
}

void CGameObject::PostUpdate(float elapsedTime)
{
}

void CGameObject::Render(HDC hDC, float elapsedTime)
{
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
		// 카메라 바깥 오브젝트 컬링
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

		

		if (m_texture[(int)m_objectDir]->GetTextureType() == ETexture_Type::Sprite)
		{
			if (m_texture[(int)m_objectDir]->GetEnableColorKey())
			{
				TransparentBlt(hDC, (int)renderLT.x, (int)renderLT.y, (int)m_size.x, (int)m_size.y,
					m_texture[(int)m_objectDir]->GetDC(), 0, 0, (int)m_size.x, (int)m_size.y, m_texture[(int)m_objectDir]->GetColorKey());
			}
			else
			{
				BitBlt(hDC, (int)renderLT.x, (int)renderLT.y, (int)m_size.x, (int)m_size.y,
					m_texture[(int)m_objectDir]->GetDC(), 0, 0, SRCCOPY);
			}
		}
		else if (m_texture[(int)m_objectDir]->GetTextureType() == ETexture_Type::CIMAGE)
		{
			m_time += elapsedTime;
			m_idx = (((int)(m_time * m_animationBox[(int)m_objectState].size())) % m_animationBox[(int)m_objectState].size());
			m_texture[(int)m_objectDir]->GetCImage().Draw(hDC, (int)renderLT.x, (int)renderLT.y, (int)m_size.x, (int)m_size.y, m_animationBox[(int)m_objectState][m_idx].left, m_animationBox[(int)m_objectState][m_idx].top, m_animationBox[(int)m_objectState][m_idx].right, m_animationBox[(int)m_objectState][m_idx].bottom);
		}
	}
}

// 양방향으로 텍스쳐 필요하면 2개, 아니면 1개
void CGameObject::CreateTexture(int num)
{
	m_texture = new CTexture * [num];
}
