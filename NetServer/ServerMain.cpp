#include "TCPServer.h"

int main()
{
    if (TCPServer::GetInst()->Init())
        TCPServer::GetInst()->Run();

    return 0;
}