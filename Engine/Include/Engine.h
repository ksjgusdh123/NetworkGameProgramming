#pragma once

#include "EngineInfo.h"

class CEngine
{
public:
	Resolution GetResolution() const { return m_resolution; }
	float GetElapsedTime() { return m_elapsedTime; }

public:
	bool Init(HINSTANCE hInst, int windowWidth, int windowHeight);
	int Run();

public:
	void Logic();
	bool Update(float elapsedTime);
	bool PostUpdate(float elapsedTime);
	void Render(float elapsedTime);

private:
	void Register();	// 창 클래스 등록
	bool Create();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static void HandleInput(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	HINSTANCE	m_hInst = 0;
	HWND		m_hWnd = 0;
	HDC			m_hDC = 0;
	HDC			m_hBackDC = 0;
	HBITMAP		m_hBackBmp = 0;
	HBITMAP		m_hBackPrevBmp = 0;
	Resolution	m_resolution;
	std::unique_ptr<class CTimer>	m_timer;
	float		m_elapsedTime = 0.f;

	static bool	m_loop;

	DECLARE_SINGLE(CEngine)
};

