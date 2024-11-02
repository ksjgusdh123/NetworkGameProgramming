#pragma once
#include "../EngineInfo.h"

class CScene
{
public:
	class CCamera* GetCamera() const { return m_camera.get(); }
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
		m_objects[(int)obj->GetObjectType()].push_back(obj);

		return obj.get();
	}

private:
	std::shared_ptr<class CCamera> m_camera;
	std::array<std::list<std::shared_ptr<class CGameObject>>, 4> m_objects;
	class CGameObject* m_player;
};

