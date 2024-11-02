#include "GameObject.h"
#include "Scene/Scene.h"
#include "Scene/Camera.h"
#include "Scene/SceneManager.h"

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
    Vector2 TextOffset = { -10.f, -10.f };

    RECT rc{};
    Vector2 size = m_size * m_pivot;
    Vector2 pos;
    if (m_scene)
        pos = m_pos - m_scene->GetCamera()->GetPos();
    else
        pos = m_pos - CSceneManager::GetInst()->GetScene()->GetCamera()->GetPos();

    rc.left = (int)pos.x - (int)size.x;
    rc.top = (int)pos.y - (int)size.y;
    rc.right = (int)pos.x + (int)size.x;
    rc.bottom = (int)pos.y + (int)size.y;

    switch (m_renderType)
    {
    case ERender_Type::Elipse:
        Ellipse(hDC, rc.left, rc.top, rc.right, rc.bottom);
        break;
    case ERender_Type::Rectangle:
        Rectangle(hDC, rc.left, rc.top, rc.right, rc.bottom);
        break;
    }

    TextOutA(hDC, (int)pos.x + (int)TextOffset.x,
        (int)pos.y + (int)TextOffset.y,
        m_name.c_str(), (int)m_name.length());
}
