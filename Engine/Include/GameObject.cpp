#include "GameObject.h"

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
    POINT TextOffset = { -10.f, -10.f };

    RECT rc{};
    rc.left = (int)m_pos.x - (int)m_size.x / 2;
    rc.top = (int)m_pos.y - (int)m_size.y / 2;
    rc.right = (int)m_pos.x + (int)m_size.x / 2;
    rc.bottom = (int)m_pos.y + (int)m_size.y / 2;

    Ellipse(hDC, rc.left, rc.top, rc.right, rc.bottom);
    /*switch (m_renderType)
    {
    case ERender_Type::Elipse:
        Ellipse(hDC, rc.left, rc.top, rc.right, rc.bottom);
        break;
    case ERender_Type::Rectangle:
        Rectangle(hDC, rc.left, rc.top, rc.right, rc.bottom);
        break;
    }*/

    TextOutA(hDC, (int)m_pos.x + (int)TextOffset.x,
        (int)m_pos.y + (int)TextOffset.y,
        m_name.c_str(), (int)m_name.length());
}
