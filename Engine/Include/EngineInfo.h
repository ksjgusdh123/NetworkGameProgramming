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
#include <format>
#include <array>
#include <filesystem>
#include <atlimage.h>

#include "Macro.h"
#include "Flag.h";

#pragma comment(lib, "msimg32.lib")

#define ROOT_PATH		"RootPath"
#define TEXTURE_PATH	"TexturePath"

#define	ELAPSED_TIME	CEngine::GetInst()->GetElapsedTime()

struct Resolution
{
	int	width = 0;
	int	height = 0;
};



struct Vector2
{
	float x = 0.f, y = 0.f;	

	Vector2() { x = 0.f; y = 0.f; }
	Vector2(float _x, float _y) { x = _x; y = _y; }
	Vector2(const Vector2& v) { x = v.x; y = v.y; }

	Vector2 operator+ (const Vector2& v) const { return Vector2(x + v.x, y + v.y); }
	Vector2 operator- (const Vector2& v) const { return Vector2(x - v.x, y - v.y); }
	Vector2 operator* (const Vector2& v) const { return Vector2(x * v.x, y * v.y); }
};

struct BoxInfo
{
	Vector2	LT;
	Vector2	RB;
};