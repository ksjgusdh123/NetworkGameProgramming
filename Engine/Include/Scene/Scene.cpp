#include "Scene.h"
#include "../GameObject.h"
#include "Camera.h"

bool CScene::Init()
{
    m_camera = std::make_shared<CCamera>();

    return true;
}

void CScene::Update(float elapsedTime)
{
    for (auto& objects : m_objects)
    {
        auto iter = objects.begin();
        auto iterEnd = objects.end();
        for (; iter != iterEnd;)
        {
            if (!(*iter)->GetActive())
            {
                iter = objects.erase(iter);
                iterEnd = objects.end();
                continue;
            }
            else if ((*iter)->GetEnable())
            {
                (*iter)->Update(elapsedTime);
            }

            ++iter;
        }
    }

    m_camera->Update(elapsedTime);
}

void CScene::PostUpdate(float elapsedTime)
{
    for (auto& objects : m_objects)
    {
        auto iter = objects.begin();
        auto iterEnd = objects.end();
        for (; iter != iterEnd;)
        {
            if (!(*iter)->GetActive())
            {
                iter = objects.erase(iter);
                iterEnd = objects.end();
                continue;
            }
            else if ((*iter)->GetEnable())
            {
                (*iter)->PostUpdate(elapsedTime);
            }

            ++iter;
        }
    }
}

void CScene::Render(HDC hDC, float elapsedTime)
{
    for (auto& objects : m_objects)
    {
        auto iter = objects.begin();
        auto iterEnd = objects.end();
        for (; iter != iterEnd;)
        {
            if (!(*iter)->GetActive())
            {
                iter = objects.erase(iter);
                iterEnd = objects.end();
                continue;
            }
            else if ((*iter)->GetEnable())
            {
                (*iter)->Render(hDC, elapsedTime);
            }

            ++iter;
        }
    }
}


