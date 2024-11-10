#pragma once
#include "Define.h"

class ThreadMgr
{
	HANDLE hRecvThread;
	HANDLE hWorkerThread;
private:
	ThreadMgr() = default;
	ThreadMgr(const ThreadMgr&) = delete;
	ThreadMgr& operator=(const ThreadMgr&) = delete;
	
public:
	static ThreadMgr& GetInst()
	{
		static ThreadMgr inst;
		return inst;
	}
public:
	DWORD WINAPI RecvThread(LPVOID arg);
	DWORD WINAPI WorkerThread(LPVOID arg);
	void CreateWorkerThread();
	void CreateRecvThread(SOCKET& client_sock);
};