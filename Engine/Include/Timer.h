#pragma once

#include "EngineInfo.h"

class CTimer
{
public:
	float GetElapsedTime() const { return m_elapsedTime; };
	float GetFPS() const { return m_fps; };

public:
	bool Init();
	void Update();

private:
	std::chrono::steady_clock::time_point	m_time;
	std::chrono::steady_clock::time_point	m_prevTime;
	float			m_elapsedTime	= 0.f;
	float			m_fps			= 0.f;
	float			m_fpsTime		= 0.f;
	int				m_tick			= 0;
};

