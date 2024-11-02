#include "PathManager.h"

DEFINITION_SINGLE(CPathManager)

CPathManager::CPathManager()
{
}

CPathManager::~CPathManager()
{
}

bool CPathManager::Init()
{
	TCHAR root[MAX_PATH] = {};
	GetModuleFileName(0, root, MAX_PATH);	// 실행파일 경로

	std::filesystem::path path(root);\
	path = path.parent_path();
	path /= "";		// 플랫폼에 따라 '/' 또는 '\\' 결합
	
	m_mapPath.insert(std::make_pair(ROOT_PATH, path));

	AddPath(TEXTURE_PATH, TEXT("Texture"));

	return true;
}

bool CPathManager::AddPath(const std::string& name, const std::wstring& directoryName, const std::string& basePathName)
{
	if (FindPath(name) != "")
		return false;

	std::filesystem::path path = FindPath(basePathName);
	path /= directoryName;
	path /= "";

	m_mapPath.insert(std::make_pair(name, path));

	return true;
}

std::filesystem::path CPathManager::FindPath(const std::string& name)
{
	auto iter = m_mapPath.find(name);
	if (iter == m_mapPath.end())
		return "";

	return iter->second;
}
