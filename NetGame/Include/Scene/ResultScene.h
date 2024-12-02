#pragma once
#include "Scene/Scene.h"
class CResultScene : public CScene
{
public:
	virtual bool Init() override;
	virtual void Update(float elapsedTime);
	virtual void Render(HDC hDC, float elapsedTime);
	virtual void KeyEvent(HWND hWnd, WPARAM wParam, LPARAM lParam);

private:
	ResultData* m_resultData;
	HWND m_hButton;
};

