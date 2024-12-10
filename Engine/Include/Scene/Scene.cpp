#include "Scene.h"
#include "../GameObject.h"
#include "Camera.h"
#include "SceneResource.h"

void CScene::DrawCenteredText(HDC hDC, const wchar_t* text, Vector2 pos, HFONT hFont, COLORREF textColor)
{
    HFONT oldFont = (HFONT)SelectObject(hDC, hFont);
    SIZE textSize;
    GetTextExtentPoint32(hDC, text, wcslen(text), &textSize);
    SetTextColor(hDC, textColor);
    SetBkMode(hDC, TRANSPARENT);
    int x = static_cast<int>(pos.x - (textSize.cx / 2));
    int y = static_cast<int>(pos.y - (textSize.cy / 2));
    TextOut(hDC, x, y, text, wcslen(text));
    SelectObject(hDC, oldFont);
}


HFONT CScene::CreateFontWithSize(HFONT originalFont, int newFontSize)
{
    LOGFONT logFont;
    GetObject(originalFont, sizeof(LOGFONT), &logFont);
    logFont.lfHeight = newFontSize;
    return CreateFontIndirect(&logFont);
}

bool CScene::Init()
{
    m_resource = std::make_shared<CSceneResource>();
    m_camera = std::make_shared<CCamera>();
    AddFontResourceEx(L"Font/DungGeunMo.ttf", FR_PRIVATE, nullptr);
    m_hFont = CreateFont(
        30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"DungGeunMo");
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

CScene::~CScene()
{
    RemoveFontResourceEx(L"Font/DungGeunMo.ttf", FR_PRIVATE, nullptr);
}


