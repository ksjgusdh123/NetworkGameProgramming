#include "ClientManager.h"
#include "NetClient/TCPClient.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    CClientManager mgr;
    if (!mgr.Init(hInstance))
        return 0;

    TCPClient::GetInst();

    int ret = mgr.Run();


    return ret;
}
