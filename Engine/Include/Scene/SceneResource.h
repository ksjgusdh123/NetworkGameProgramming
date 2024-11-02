#pragma once

#include "../EngineInfo.h"

class CSceneResource
{
public:
	CSceneResource();
	~CSceneResource();

public:
	bool LoadTexture(const std::string& name, const std::wstring& fileName, const std::string& pathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& name, const TCHAR* fullPath);

	class CTexture* FindTexture(std::string_view name);

private:
	std::unordered_map<std::string, class CTexture*>	m_mapTexture;
};

