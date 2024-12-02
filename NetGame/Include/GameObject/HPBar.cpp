#include "HPBar.h"
#include "..\ObjectAnimation.h"
bool CHPBar::Init()
{
	CGameObject::Init();
	/*CreateTexture(1);
	SetTexture("hpBar", TEXT("UI/hp_bar.png"), EObject_Dir::Right, ETexture_Type::CIMAGE);
	SetAnimation(&m_rect, 1, EObject_State::Basic);*/
	return true;
}

void CHPBar::Update(float elapsedTime)
{
	CGameObject::Update(elapsedTime);
    SetPos(m_owner->GetPos().x, (m_owner->GetPos().y - m_owner->GetSize().y / 2) - 10);
}

void CHPBar::Render(HDC hDC, float elapsedTime)
{
	//CGameObject::Render(hDC, elapsedTime);
	static const int maxHP = 100;
	static const float w = 80;
	static const float h = 10;

	int hpPercentage = m_owner->m_hp / maxHP;
	//RECT hpBarRect;

	RECT hpBarRect;
	hpBarRect.left = m_pos.x - w / 2;
	hpBarRect.top = m_pos.y - h / 2;
	//hpBarRect.right = static_cast<LONG>(m_pos.x + (w * hpPercentage));
	hpBarRect.right = m_pos.x + w / 2;
	hpBarRect.bottom = m_pos.y + h / 2;


	HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
	HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, brush);

	Rectangle(hDC, hpBarRect.left, hpBarRect.top, hpBarRect.right, hpBarRect.bottom);

	SelectObject(hDC, oldBrush);
	DeleteObject(brush);
}