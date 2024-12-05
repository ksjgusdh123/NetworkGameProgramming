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
	time_point<high_resolution_clock> m_lastUpdateTime;
public:
	GameTimer() : m_isRunning(false) {
	}

	bool IsGameStart(){}

	void Start()
	{
		m_startTime = high_resolution_clock::now();
		m_lastUpdateTime = high_resolution_clock::now();
		m_isRunning = true;
	}

	void Stop()
	{
		m_endTime = high_resolution_clock::now();
		m_isRunning = false;
	}

	float GetElapsedTime()
	{
		auto endTime = m_isRunning ? high_resolution_clock::now() : m_endTime;
		duration<double> elapsed = endTime - m_startTime;
		return (elapsed.count());
	}


	double Update()
	{
		auto currentTime = high_resolution_clock::now();
		duration<double> elapsed = currentTime - m_lastUpdateTime;
		m_lastUpdateTime = currentTime;
		return elapsed.count();
	}

};
