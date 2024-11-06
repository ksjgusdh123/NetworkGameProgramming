#include "TCPServer.h"

int main()
{
    TCPServer GameServer;

    if (GameServer.Init()) 
        GameServer.Run();
    

    return 0;
}