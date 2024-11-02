#pragma once
#include "../EngineInfo.h"

class CScene
{
public:
	class CGameObject* GetPlayer() const { return m_player; }

	void SetPlayer(class CGameObject* player) { m_player = player; }

public:
	virtual bool Init();
	virtual void Update(float elapsedTime);
	virtual void PostUpdate(float elapsedTime);
	virtual void Render(HDC hDC, float elapsedTime);

public:
	template <typename T>
	T* CreateObject(const std::string& name)
	{
		std::shared_ptr<T> obj = std::make_shared<T>();
		if (!obj->Init())
			return nullptr;

		obj->SetName(name);
		obj->SetScene(this);
		m_objects.push_back(obj);

		return obj.get();
	}

private:
	std::list<std::shared_ptr<class CGameObject>> m_objects;
	class CGameObject* m_player;
};

