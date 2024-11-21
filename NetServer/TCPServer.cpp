#include "TCPServer.h"
#include "ErrDisplay.h"

TCPServer* TCPServer::m_inst;
CRITICAL_SECTION cs;

DWORD WINAPI RecvThread(LPVOID arg)
{
    cout << "Start RecvThread!\n";
    ClientInfo* client = (ClientInfo*)arg;

    while (true)
    {
        int data_size;
        int packet_type;
        char recv_buf[BUFSIZ];

        int res = recv(client->socket, (char*)&data_size, sizeof(int), MSG_WAITALL);
        if (res <= 0) break;
        res = recv(client->socket, (char*)&packet_type, sizeof(int), MSG_WAITALL);
        if (res <= 0) break;
        res = recv(client->socket, recv_buf, data_size, MSG_WAITALL);
        if (res <= 0) break;

        Packet p(data_size, packet_type, recv_buf);
        EnterCriticalSection(&cs);
        TCPServer::GetInst()->recvQ.push(p);
        LeaveCriticalSection(&cs);
    }

    cout << "End RecvThread!\n";
    closesocket(client->socket);
    return 0;
}

DWORD WINAPI WorkerThread(LPVOID arg)
{
    while (true)
    {
        EnterCriticalSection(&cs);
        auto& recvQ = TCPServer::GetInst()->recvQ;
        if (recvQ.empty())
        {
            LeaveCriticalSection(&cs);
            Sleep(100);
            continue;
        }
        Packet p = recvQ.front();
        recvQ.pop();
        LeaveCriticalSection(&cs);

        switch (p.type)
        {
        case LOGIN:
        {
            C_LoginRequestPkt* cur = (C_LoginRequestPkt*)&p;
            cur->deserialize();

            cout << "Login Request Received: Player ID=" << cur->player_id
                << ", Player Name=" << cur->player_name << endl;

            auto& clientList = TCPServer::GetInst()->clientInfos;
            if (std::any_of(clientList.begin(), clientList.end(), [&](const ClientInfo& client) { return client.id == cur->player_id; }))
            {
                cout << "Player ID " << cur->player_id << " already logged in.\n";

                Packet errorPacket(LOGIN_FAIL, "Player already logged in.");
                TCPServer::GetInst()->SendPacket(errorPacket);
            }
            else
            {
                cout << "Player ID " << cur->player_id << " successfully logged in.\n";

                Packet successPacket(LOGIN_SUCCESS, "Login successful.");
                TCPServer::GetInst()->SendPacket(successPacket);
            }
            break;
        }
        case MOVE:
        {
            C_PlayerMovePkt* cur = (C_PlayerMovePkt*)&p;
            cur->deserialize();
            cout << "Player " << cur->player_id << " moved to (" << cur->x << "," << cur->y << ")\n";
            TCPServer::GetInst()->SendPacket(p);
            break;
        }
        default:
            break;
        }
    }
}

bool TCPServer::Init()
{
    cout << "Init()\n";

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return false;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == INVALID_SOCKET) err_quit("socket()");

    sockaddr_in serveraddr = {};
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);

    if (bind(server_sock, (sockaddr*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR)
        err_quit("bind()");

    if (listen(server_sock, SOMAXCONN) == SOCKET_ERROR)
        err_quit("listen()");

    InitializeCriticalSection(&cs);

    hWorkerThread = CreateThread(NULL, 0, WorkerThread, NULL, 0, NULL);
    if (hWorkerThread == NULL) err_display("WorkerThread");

    return true;
}

void TCPServer::Connect()
{
    cout << "Connect()\n";
    while (clientInfos.size() < 2)
    {
        sockaddr_in clientaddr;
        int addrlen = sizeof(clientaddr);
        SOCKET client_sock = accept(server_sock, (sockaddr*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET)
        {
            err_display("accept()");
            continue;
        }

        int clientId = static_cast<int>(clientInfos.size() + 1);
        send(client_sock, (char*)&clientId, sizeof(int), 0);

        clientInfos.emplace_back(client_sock, clientId, "");

        cout << "Client " << clientId << " connected.\n";

        HANDLE hRecvThread = CreateThread(NULL, 0, RecvThread, (LPVOID)&clientInfos.back(), 0, NULL);
        if (hRecvThread == NULL)
        {
            cout << "Failed to create RecvThread for client " << clientId << ".\n";
            closesocket(client_sock);
            clientInfos.pop_back();
        }
        else
        {
            CloseHandle(hRecvThread);
        }
    }
}

void TCPServer::Run()
{
    cout << "Run()\n";
    Connect();
}

void TCPServer::SendPacket(const Packet& packet)
{
    for (const auto& client : clientInfos)
    {
        send(client.socket, (char*)&packet.data_size, sizeof(int), 0);
        send(client.socket, (char*)&packet.type, sizeof(int), 0);
        send(client.socket, packet.data, packet.data_size, 0);
    }
}
