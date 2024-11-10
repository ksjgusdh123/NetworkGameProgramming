#include "NetworkMgr.h"

DWORD __stdcall ThreadMgr::RecvThread(LPVOID arg)
{
	cout << "RecvThread()\n"; return 0;
}

DWORD __stdcall ThreadMgr::WorkerThread(LPVOID arg)
{
	cout << "WorkerThread()\n"; return 0;
}

void ThreadMgr::CreateWorkerThread()
{
	if (hWorkerThread != NULL) return;
	hWorkerThread = ::CreateThread(NULL, 0, [](LPVOID arg) -> DWORD {
		return ThreadMgr::GetInst().WorkerThread(arg); }, NULL, 0, NULL);
}

void ThreadMgr::CreateRecvThread(SOCKET& client_sock)
{
	hRecvThread = ::CreateThread(NULL, 0, [](LPVOID arg) -> DWORD {
		return ThreadMgr::GetInst().RecvThread(arg); }, (LPVOID)client_sock, 0, NULL);
	if (hRecvThread == NULL) { closesocket(client_sock); }
	else { CloseHandle(hRecvThread); }

	cout << "Accept!\n";
}
