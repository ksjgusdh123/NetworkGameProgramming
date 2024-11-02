#pragma once

#include "../EngineInfo.h"

class CSceneManager
{
public:
	bool Init();
	bool Update(float elapsedTime);
	bool PostUpdate(float elapsedTime);
	void Render(HDC hDC, float elapsedTime);
public:
	template <typename T>
	bool CreateScene()
	{
		std::unique_ptr<T> scene = std::make_unique<T>();
		if (!scene->Init())
			return false;

		if (!m_scene)
			m_scene = std::move(scene);
		else
			m_nextScene = std::move(scene);

		return true;
	}

private:
	bool ChangeScene();
	
private:
	std::unique_ptr<class CScene> m_scene;
	std::unique_ptr<class CScene> m_nextScene;

	DECLARE_SINGLE(CSceneManager)
};

