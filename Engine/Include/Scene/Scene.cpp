#include "Scene.h"
#include "../GameObject.h"

bool CScene::Init()
{
    return true;
}

void CScene::Update(float elapsedTime)
{
    auto iter = m_objects.begin();
    auto iterEnd = m_objects.end();
    for (; iter != iterEnd;)
    {
        if (!(*iter)->GetActive())
        {
            iter = m_objects.erase(iter);
            iterEnd = m_objects.end();
            continue;
        }
        else if ((*iter)->GetEnable())
        {
            (*iter)->Update(elapsedTime);
        }

        ++iter;
    }
}

void CScene::PostUpdate(float elapsedTime)
{
    auto iter = m_objects.begin();
    auto iterEnd = m_objects.end();
    for (; iter != iterEnd;)
    {
        if (!(*iter)->GetActive())
        {
            iter = m_objects.erase(iter);
            iterEnd = m_objects.end();
            continue;
        }
        else if ((*iter)->GetEnable())
        {
            (*iter)->PostUpdate(elapsedTime);
        }

        ++iter;
    }
}

void CScene::Render(HDC hDC, float elapsedTime)
{
    auto iter = m_objects.begin();
    auto iterEnd = m_objects.end();
    for (; iter != iterEnd;)
    {
        if (!(*iter)->GetActive())
        {
            iter = m_objects.erase(iter);
            iterEnd = m_objects.end();
            continue;
        }
        else if ((*iter)->GetEnable())
        {
            (*iter)->Render(hDC, elapsedTime);
        }

        ++iter;
    }
}

