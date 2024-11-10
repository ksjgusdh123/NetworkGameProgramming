#pragma once
#include "Define.h"
#include <queue>
#include <mutex>

class ThreadMgr
{
public:
	DWORD WINAPI RecvThread(LPVOID arg);
	DWORD WINAPI WorkerThread(LPVOID arg);
	void CreateWorkerThread();
	void CreateRecvThread(SOCKET& client_sock);
public:
	static ThreadMgr& GetInst()
	{
		static ThreadMgr inst;
		return inst;
	}
private:
	ThreadMgr() = default;
	ThreadMgr(const ThreadMgr&) = delete;
	ThreadMgr& operator=(const ThreadMgr&) = delete;
private:
	HANDLE hRecvThread;
	HANDLE hWorkerThread;

	queue<Packet> m_packetQueue;
	mutex m_queueMutex;
	vector<SOCKET> m_clientSockets;
	mutex m_clientLock;
};