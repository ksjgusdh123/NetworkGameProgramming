#include "TCPServer.h"
#include "ErrDisplay.h"

queue<Packet> recv_queue;

DWORD WINAPI RecvThread(LPVOID arg)
{
	cout << "RecvThead!\n";
	SOCKET client_sock = (SOCKET)arg;

	while (true)
	{
		int packet_size;
		char recv_buf[BUFSIZ];
		recv(client_sock, (char*)&packet_size, sizeof(int), MSG_WAITALL);
		recv(client_sock, recv_buf, packet_size, MSG_WAITALL);
		Packet p(packet_size, recv_buf);
		cout << p.data << endl;
		recv_queue.push(p);
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
	while(1)
	{
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
		hThread = CreateThread(NULL, 0, RecvThread, (LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { cout << "err\n"; closesocket(client_sock); }
		else { CloseHandle(hThread); }
	}
}

void TCPServer::Run()
{
	cout << "Run()\n";
	Connect();
}
