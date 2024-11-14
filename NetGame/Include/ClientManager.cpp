#include "ClientManager.h"
#include "Scene/SceneManager.h"
#include "Scene/MainScene.h"
#include "Scene/LoginScene.h"
CClientManager::CClientManager()
{
}

CClientManager::~CClientManager()
{
    CEngine::DestroyInst();
}

bool CClientManager::Init(HINSTANCE hInst)
{
    if (!CEngine::GetInst()->Init(hInst, 960, 640))
        return false;

    CSceneManager::GetInst()->CreateScene<CLoginScene>();

	return true;
}

int CClientManager::Run()
{
    return CEngine::GetInst()->Run();
}
