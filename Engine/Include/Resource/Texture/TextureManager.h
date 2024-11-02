#pragma once

#include "../../EngineInfo.h"

class CTextureManager
{
public:
	CTextureManager();
	~CTextureManager();

public:
	bool Init();

public:
	bool LoadTexture(const std::string& name, const std::wstring& fileName, const std::string& pathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& name, const TCHAR* fullPath);

	class CTexture* FindTexture(std::string_view name);
	void ReleaseTexture(std::string_view name);

private:
	std::unordered_map<std::string, std::shared_ptr<class CTexture>>	m_mapTexture;
};

