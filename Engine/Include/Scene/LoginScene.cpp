#include "LoginScene.h"
#include <GameObject.h>
#include <Input.h>
#include <Engine.h>
#include <Scene/SceneManager.h>


bool CLoginScene::Init()
{
	CScene::Init();

	CGameObject* back = CreateObject<CGameObject>("Background");
	back->CreateTexture(1);
	back->SetTexture("Background", TEXT("Map/Login.bmp"), EObject_Dir::Right);
	back->SetSize(960.f, 650.f);
	back->SetPivot(0, 0);

	CInput::GetInst()->AddBindFunction<CLoginScene>("IDCheck", EInput_Type::PUSH, this, &CLoginScene::EnterLogin, this);

	return true;
}

void CLoginScene::Update(float elapsedTime)
{
	CScene::Update(elapsedTime);
}

void CLoginScene::EnterLogin()
{
	std::string str = CEngine::GetInst()->GetID();
	CSceneManager::GetInst()->CreateScene<CMainScene>();

}
