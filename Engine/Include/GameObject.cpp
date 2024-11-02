#include "GameObject.h"
#include "Scene/Scene.h"
#include "Scene/Camera.h"
#include "Scene/SceneManager.h"
#include "Scene/SceneResource.h"
#include "Resource/Texture/Texture.h"

void CGameObject::SetTexture(const std::string& name, const std::wstring& fileName, const std::string& pathName)
{
    m_scene->GetSceneResource()->LoadTexture(name, fileName, pathName);

    m_texture = m_scene->GetSceneResource()->FindTexture(name);

    SetSize((float)m_texture->GetWidth(), (float)m_texture->GetHeight());
}

bool CGameObject::SetColorKey(unsigned char r, unsigned char g, unsigned char b, int idx)
{
    if (!m_texture)
        return false;

    m_texture->SetColorKey(r, g, b, idx);

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

		if (m_texture->GetEnableColorKey())
		{
			if (m_texture->GetTextureType() == ETexture_Type::Sprite)
			{
				TransparentBlt(hDC, (int)renderLT.x, (int)renderLT.y, (int)m_size.x, (int)m_size.y,
					m_texture->GetDC(), 0, 0, (int)m_size.x, (int)m_size.y, m_texture->GetColorKey());
			}

			else
			{
			}
		}

		else
		{
			if (m_texture->GetTextureType() == ETexture_Type::Sprite)
			{
				BitBlt(hDC, (int)renderLT.x, (int)renderLT.y, (int)m_size.x, (int)m_size.y,
					m_texture->GetDC(), 0, 0, SRCCOPY);
			}

			else
			{
			}
		}
	}
}
