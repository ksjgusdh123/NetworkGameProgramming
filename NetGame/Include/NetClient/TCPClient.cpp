#pragma once
#include "TCPClient.h"
#include "ErrDisplay.h"
#include <Scene/SceneManager.h>
#include <Scene/Scene.h>
#include "..\Scene\LoginScene.h"
HANDLE hRecvEvent, hSendEvent;

DWORD WINAPI RecvThread(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;
	PacketManager::GetInst().Init(sock);
	while (true)
	{
		WaitForSingleObject(hRecvEvent, INFINITE);
		Packet packet = PacketManager::GetInst().RecvPacket();
		PacketManager::GetInst().ProcessPacket(packet);
	}
	return true;
}

DWORD WINAPI SendThread(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;

	while (true)
	{
		if (CSceneManager::GetInst()->GetScene()->m_sceneType != LOGIN_SCENE)
			break;
	}

	while (true)
	{
		PacketManager::GetInst().DequeueSendPacket();
		CSceneManager::GetInst()->GetScene()->SendGameData();
		Sleep(1000 / 30);
		SetEvent(hRecvEvent);
	}
	return true;
}

bool TCPClient::Init()
{
	cout << "Client Init()\n";
	hRecvEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	hSendEvent = CreateEvent(NULL, TRUE, TRUE, NULL);

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return false;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	Connect();
	return true;
}

void TCPClient::Connect()
{
	cout << "Client Connect()\n";

	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);
	int retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	hRecvThread = CreateThread(NULL, 0, RecvThread, (LPVOID)sock, 0, NULL);
	if (hRecvThread == NULL) { closesocket(sock); }
	else { CloseHandle(hRecvThread); }

	hSendThread = CreateThread(NULL, 0, SendThread, (LPVOID)sock, 0, NULL);
	if (hSendThread == NULL) { closesocket(sock); }
	else { CloseHandle(hSendThread); }
}


void TCPClient::Cleanup() {
	if (sock != INVALID_SOCKET) {
		closesocket(sock);
		sock = INVALID_SOCKET;
	}
	CloseHandle(hRecvEvent);
	CloseHandle(hSendEvent);
	WSACleanup();
}

