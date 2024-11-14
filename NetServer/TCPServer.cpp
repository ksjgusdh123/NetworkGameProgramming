#include "TCPServer.h"
#include "ErrDisplay.h"

DWORD WINAPI RecvThread(LPVOID arg)
{
	cout << "RecvThead!\n";
	SOCKET client_sock = (SOCKET)arg;
	struct sockaddr_in clientaddr;
	char addr[INET_ADDRSTRLEN];
	int addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
	char buf[BUFSIZ];

	while (true)
	{
		int retval = recv(client_sock, buf, BUFSIZ, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			return -1;
		}
		cout << buf << endl;
	}
	return 0;
}
bool TCPServer::Init()
{
	cout << "Init()\n";

	int retval;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	return true;
}

void TCPServer::Connect()
{
	cout << "Connect()\n";

	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	int addrlen;
	char buf[BUFSIZ];
	addrlen = sizeof(clientaddr);
	client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
	if (client_sock == INVALID_SOCKET) {
		err_display("accept()");
		return;
	}
	hThread = CreateThread(NULL, 0, RecvThread,
		(LPVOID)client_sock, 0, NULL);
	if (hThread == NULL) { cout << "err\n"; closesocket(client_sock); }
	else { CloseHandle(hThread); }
}

void TCPServer::Run()
{
	cout << "Run()\n";
	Connect();
}
