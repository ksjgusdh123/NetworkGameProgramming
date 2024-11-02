#pragma once

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include "resource.h"

#include <string>
#include <list>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <memory>
#include <chrono>
#include <functional>

#include "Macro.h"

#define	ELAPSED_TIME	CEngine::GetInst()->GetElapsedTime()

struct Resolution
{
	int	width = 0;
	int	height = 0;
};

struct Vector2
{
	float x, y;	
};