#include "ThreadMgr.h"

DWORD __stdcall ThreadMgr::RecvThread(LPVOID arg)
{
	cout << "RecvThread()\n";
    SOCKET client_sock = (SOCKET)arg;

    char buffer[BUFSIZ];
    int recv_len;

    while (true)
    {
        recv_len = recv(client_sock, buffer, sizeof(buffer), 0);

        Packet packet;
        packet.data.assign(buffer, buffer + recv_len);
        packet.length = recv_len;

        {
            lock_guard<mutex> lock(m_queueMutex);
            m_packetQueue.push(packet);
        }
    }

    return 0;
}

DWORD __stdcall ThreadMgr::WorkerThread(LPVOID arg)
{
	cout << "WorkerThread()\n";

    while (true)
    {
        Packet packet;
        {
            unique_lock<mutex> lock(m_queueMutex);
            if (m_packetQueue.empty())
            {
                lock.unlock();
                Sleep(10);
                continue;
            }

            packet = m_packetQueue.front();
            m_packetQueue.pop();
        }

        {
           lock_guard<mutex> lock(m_clientLock);
            for (SOCKET client_sock : m_clientSockets)
            {
                send(client_sock, packet.data.data(), packet.length, 0);
            }
        }
    }

    return 0;
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
