#include "Engine.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Timer.h"
#include "Input.h"
#include "Resource/ResourceManager.h"
#include "PathManager.h"


DEFINITION_SINGLE(CEngine)
bool CEngine::m_loop = true;

CEngine::CEngine()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(220);
}

CEngine::~CEngine()
{
    CSceneManager::DestroyInst();
    CResourceManager::DestroyInst();
    CInput::DestroyInst();
    CPathManager::DestroyInst();

    ReleaseDC(m_hWnd, m_hDC);
}

bool CEngine::Init(HINSTANCE hInst, int windowWidth, int windowHeight)
{
    m_hInst = hInst;

    m_resolution.width = windowWidth;
    m_resolution.height = windowHeight;

    Register();
    Create();

    if (!CInput::GetInst()->Init())
        return false;

    if (!CSceneManager::GetInst()->Init())
        return false;

    if (!CPathManager::GetInst()->Init())
        return false;

    if (!CResourceManager::GetInst()->Init())
        return false;

    m_hDC = GetDC(m_hWnd);
    m_hBackDC = CreateCompatibleDC(m_hDC);
    m_hBackBmp = CreateCompatibleBitmap(m_hDC, m_resolution.width, m_resolution.height);

    m_hBackPrevBmp = (HBITMAP)SelectObject(m_hBackDC, m_hBackBmp);

    m_timer = std::make_unique<CTimer>();
    m_timer->Init();

    return true;
}

int CEngine::Run()
{
    MSG msg = {};

    while (m_loop)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            Logic();
        }
    }

    return (int)msg.wParam;
}

void CEngine::Logic()
{
    m_timer->Update();

    m_elapsedTime = m_timer->GetElapsedTime();

    CInput::GetInst()->Update();

    // Scene이 교체될 경우 처음부터 다시 동작시킨다
    if (Update(m_elapsedTime))
        return;

    if (PostUpdate(m_elapsedTime))
        return;


    Render(m_elapsedTime);
}

bool CEngine::Update(float elapsedTime)
{
    return CSceneManager::GetInst()->Update(elapsedTime);
}

bool CEngine::PostUpdate(float elapsedTime)
{
    return CSceneManager::GetInst()->PostUpdate(elapsedTime);
}

void CEngine::Render(float elapsedTime)
{
    Rectangle(m_hBackDC, -1, -1,
    m_resolution.width + 1, m_resolution.height + 1);

    CSceneManager::GetInst()->Render(m_hBackDC, elapsedTime);

   // std::wstring fps = std::to_wstring((int)m_timer->GetFPS());
   // fps += L" fps";
   // TextOut(m_hBackDC, 10, 10, fps.c_str(), (int)fps.size());

   BitBlt(m_hDC, 0, 0, m_resolution.width, m_resolution.height, m_hBackDC, 0, 0, SRCCOPY);
}

void CEngine::Register()
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_hInst;
    wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_ENGINE));    // 실행파일에 사용할 아이콘 등록
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);  // 마우스 커서 모양
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;// MAKEINTRESOURCEW(IDC_MY220428);  // 메뉴를 사용할 것인지
    wcex.lpszClassName = TEXT("NetGame");
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));   // 윈도우창 좌상단에 표시할 작은 아이콘

    RegisterClassExW(&wcex);
}

bool CEngine::Create()
{
    m_hWnd = CreateWindowW(TEXT("NetGame"), TEXT("NetGame"), WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
        100, 0, 0, 0, nullptr, nullptr, m_hInst, nullptr);

    if (!m_hWnd)
        return false;

    RECT    rc = { 0, 0, m_resolution.width, m_resolution.height };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    MoveWindow(m_hWnd, 50, 50, rc.right - rc.left, rc.bottom - rc.top, TRUE);   // 윈도우 창의 크기를 변경한다.
    ShowWindow(m_hWnd, SW_SHOW);    // 윈도우 창을 보여준다.

    // 클라이언트 영역 갱신에 실패했을 경우 0을 반환한다.
    if (!UpdateWindow(m_hWnd))
        return false;

    return true;
}

LRESULT CEngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        // 윈도우가 종료될때 들어오는 메세지이다.
        m_loop = false;
        PostQuitMessage(0);
        break;
    case WM_COMMAND:
        (CSceneManager::GetInst()->GetScene())->KeyEvent(hWnd, wParam, lParam);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

