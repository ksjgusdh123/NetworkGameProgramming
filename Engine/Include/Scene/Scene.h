#pragma once

#include "../EngineInfo.h"

// temp
struct GameObject
{
	std::wstring name;
	float sizeX;
	float sizeY;
	float posX;
	float posY;

	bool Init()
	{
		return true;
	}
};

class CScene
{
public:
	virtual bool Init();
	virtual void Update(float elapsedTime);
	virtual void PostUpdate(float elapsedTime);
	virtual void Render(HDC hDC, float elapsedTime);

public:
	template <typename T>
	T* CreateObject(std::wstring_view name)
	{
		std::shared_ptr<T> obj = std::make_shared<T>();
		if (!obj->Init())
			return nullptr;

		obj->name = name.data();
		m_objects.push_back(obj);

		return obj.get();
	}

private:
	std::list<std::shared_ptr<GameObject>> m_objects;
};

