#pragma once
#include <iostream>
#include <chrono>

using namespace std::chrono;

class GameTimer
{
private:
	time_point<high_resolution_clock> m_startTime;
	time_point<high_resolution_clock> m_endTime;
	bool m_isRunning;

public:
	GameTimer() : m_isRunning(false) {}

	void Start()
	{
		m_startTime = high_resolution_clock::now();
		m_isRunning = true;
	}

	void Stop()
	{
		m_endTime = high_resolution_clock::now();
		m_isRunning = false;
	}

	int GetElapsedTime()
	{
		auto endTime = m_isRunning ? high_resolution_clock::now() : m_endTime;
		duration<double> elapsed = endTime - m_startTime;
		return static_cast<int>(elapsed.count());
	}

};
