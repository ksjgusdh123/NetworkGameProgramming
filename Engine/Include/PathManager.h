#pragma once

#include "EngineInfo.h"

class CPathManager
{
public:
	bool Init();
	bool AddPath(const std::string& name, const std::wstring& directoryName, const std::string& basePathName = ROOT_PATH);
	std::filesystem::path FindPath(const std::string& name);

private:
	std::unordered_map<std::string, std::filesystem::path>	m_mapPath;


	DECLARE_SINGLE(CPathManager)
};

