#include "Timer.h"

bool CTimer::Init()
{
	m_time = std::chrono::high_resolution_clock::now();
	m_prevTime = m_time;

	return true;
}

void CTimer::Update()
{
    // 현재 시간을 기록한다.
    m_time = std::chrono::high_resolution_clock::now();

	// elapsedTime = (현재 타이머값 - 이전 프레임의 타이머값) / 초당 타이머값
    m_elapsedTime = std::chrono::duration<float, std::chrono::milliseconds::period>(m_time - m_prevTime).count() / 1000.f;

    // 현재 시간을 이전 시간으로 업데이트한다.
    m_prevTime = m_time;

	// FPS 계산
	m_fpsTime += m_elapsedTime;
	++m_tick;

	if (m_tick >= 1.f)
	{
		m_fps = m_tick / m_fpsTime;
		m_fpsTime = 0.f;
		m_tick = 0;
	}
}
