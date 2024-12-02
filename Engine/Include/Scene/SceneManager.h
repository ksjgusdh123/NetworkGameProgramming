#pragma once

#include "../EngineInfo.h"

class CSceneManager
{
public:
	class CScene* GetScene() const { return m_scene.get(); }

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
		ChangeScene();
		return true;
	}
	int m_tileNum{};
	std::vector<int> m_tileType;
	std::vector<float> m_tilePosX;
	std::vector<float> m_tilePosY;
	bool m_bWin = false;
	float m_playTime = 0;
private:
	bool ChangeScene();
	
private:
	std::unique_ptr<class CScene> m_scene;
	std::unique_ptr<class CScene> m_nextScene;

	DECLARE_SINGLE(CSceneManager)
};

