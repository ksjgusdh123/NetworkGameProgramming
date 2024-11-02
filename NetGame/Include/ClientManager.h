#pragma once

#include "ClientInfo.h"

class CClientManager
{
public:
	CClientManager();
	~CClientManager();

public:
	bool Init(HINSTANCE hInst);
	int Run();
};
