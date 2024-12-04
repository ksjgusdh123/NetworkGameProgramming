#include "SceneManager.h"
#include "Scene.h"
#include "DefaultScene.h"
#include "..\Input.h"

DEFINITION_SINGLE(CSceneManager)

CSceneManager::CSceneManager()
{
	hPauseEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

CSceneManager::~CSceneManager()
{
	CloseHandle(hPauseEvent);
}

bool CSceneManager::Init()
{
	CreateScene<CDefaultScene>();

	return true;
}

bool CSceneManager::Update(float elapsedTime)
{
	m_scene->Update(elapsedTime);

	return ChangeScene();
}

bool CSceneManager::PostUpdate(float elapsedTime)
{
	m_scene->PostUpdate(elapsedTime);

	return ChangeScene();
}

void CSceneManager::Render(HDC hDC, float elapsedTime)
{
	m_scene->Render(hDC, elapsedTime);
}

bool CSceneManager::ChangeScene()
{
	if (m_nextScene)
	{
		CInput::GetInst()->ClearCallback(m_scene.get());

		m_scene.swap(m_nextScene);
		m_nextScene.reset();
		return true;
	}

	return false;
}
