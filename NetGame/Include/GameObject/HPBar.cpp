#include "HPBar.h"
#include "..\ObjectAnimation.h"
#include <Scene/SceneManager.h>
#include <Scene/Scene.h>
#include <Scene/Camera.h>
bool CHPBar::Init()
{
    CGameObject::Init();
    return true;
}

void CHPBar::Update(float elapsedTime)
{
    CGameObject::Update(elapsedTime);
    SetPos(m_owner->GetPos().x, (m_owner->GetPos().y - m_owner->GetSize().y / 2) - 10);
}

void CHPBar::Render(HDC hDC, float elapsedTime)
{
    if (!m_owner)
        return;
    static const int maxHP = 100;

    Vector2 pos;
    Vector2 cameraPos;
    Vector2 resolution;

    CScene* scene = CSceneManager::GetInst()->GetScene();
    cameraPos = scene->GetCamera()->GetPos();
    resolution = scene->GetCamera()->GetResolution();
    pos = m_pos - cameraPos;

    float currentHP = static_cast<float>(m_owner->m_hp);
    float hpPercentage = currentHP / static_cast<float>(maxHP);
    //float hpPercentage = 50 / static_cast<float>(maxHP);

    Vector2 renderLT = pos - m_pivot * m_size;

    RECT backgroundRect;
    backgroundRect.left = static_cast<LONG>(renderLT.x - m_w / 2);
    backgroundRect.top = static_cast<LONG>(renderLT.y - m_h / 2);
    backgroundRect.right = static_cast<LONG>(renderLT.x + m_w / 2);
    backgroundRect.bottom = static_cast<LONG>(renderLT.y + m_h / 2);

    RECT foregroundRect;
    foregroundRect.left = backgroundRect.left;
    foregroundRect.top = backgroundRect.top;
    foregroundRect.right = static_cast<LONG>(backgroundRect.left + (m_w * hpPercentage));
    foregroundRect.bottom = backgroundRect.bottom;

    HBRUSH greenBrush = CreateSolidBrush(RGB(0, 50, 0));
    FillRect(hDC, &backgroundRect, greenBrush);
    DeleteObject(greenBrush);

    HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
    FillRect(hDC, &foregroundRect, redBrush);
    DeleteObject(redBrush);
}

void CHPBar::SetBarSize(float w, float h)
{
    m_w = w;
    m_h = h;
}