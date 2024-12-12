#pragma once
#include "../EngineInfo.h"

class CScene
{
public:
	class CSceneResource* GetSceneResource() const { return m_resource.get(); }
	class CCamera* GetCamera() const { return m_camera.get(); }
	class CGameObject* GetMyPlayer() const { return m_myPlayer; }
	class CGameObject* GetPlayer() const { return m_player; }

	void SetMyPlayer(class CGameObject* player) { m_myPlayer = player; }
	void SetPlayer(class CGameObject* player) { m_player = player; }

	void DrawCenteredText(HDC hDC, const wchar_t* text, Vector2 pos, HFONT hFont, COLORREF textColor = RGB(255, 255, 255));
	HFONT CreateFontWithSize(HFONT originalFont, int newFontSize);
public:
	virtual bool Init();
	virtual void Update(float elapsedTime);
	virtual void PostUpdate(float elapsedTime);
	virtual void Render(HDC hDC, float elapsedTime);
	virtual void KeyEvent(HWND hWnd, WPARAM wParam, LPARAM lParam) {}
	virtual bool SendGameData() { return true; };
public:
	template <typename T>
	T* CreateObject(const std::string& name)
	{
		std::shared_ptr<T> obj = std::make_shared<T>();
		obj->SetName(name);
		obj->SetScene(this);
		if (!obj->Init())
			return nullptr;

		m_objects[(int)obj->GetObjectType()].push_back(obj);

		return obj.get();
	}
	virtual ~CScene();
	int m_sceneType = 0;

protected:
	std::shared_ptr<class CSceneResource> m_resource;
	std::shared_ptr<class CCamera> m_camera;
	std::array<std::list<std::shared_ptr<class CGameObject>>, (int)EObject_Type::Max> m_objects;
	class CGameObject* m_myPlayer;
	class CGameObject* m_player;
	int m_myid;
	int m_mateId;

	HFONT m_hFont;
};

