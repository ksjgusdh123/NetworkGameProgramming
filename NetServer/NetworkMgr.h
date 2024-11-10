#pragma once
#include "Define.h"

class NetworkMgr
{
	HANDLE hRecvThread;
	HANDLE hSendThread;
private:
	NetworkMgr() = default;
	NetworkMgr(const NetworkMgr&) = delete;
	NetworkMgr& operator=(const NetworkMgr&) = delete;
	
public:
	static NetworkMgr& GetInst()
	{
		static NetworkMgr inst;
		return inst;
	}
public:
	DWORD WINAPI RecvThread(LPVOID arg)
	{
		cout << "RecvThread()\n"; return 0;
	};
	DWORD WINAPI SendThread(LPVOID arg)
	{
		cout << "SendThread()\n"; return 0;
	};
	void CreateThread(SOCKET& client_sock)
	{
		hRecvThread = ::CreateThread(NULL, 0, [](LPVOID arg) -> DWORD {
			return NetworkMgr::GetInst().RecvThread(arg);}, (LPVOID)client_sock, 0, NULL);
		if (hRecvThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hRecvThread); }

		hRecvThread = ::CreateThread(NULL, 0, [](LPVOID arg) -> DWORD {
			return NetworkMgr::GetInst().SendThread(arg);}, (LPVOID)client_sock, 0, NULL);
		if (hRecvThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hRecvThread); }
		cout << "Accept!\n";
	}
};