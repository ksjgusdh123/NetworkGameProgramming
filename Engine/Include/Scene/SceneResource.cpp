#include "SceneResource.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Texture/Texture.h"

CSceneResource::CSceneResource()
{
}

CSceneResource::~CSceneResource()
{
}

bool CSceneResource::LoadTexture(const std::string& name, const std::wstring& fileName, const std::string& pathName)
{
	if (FindTexture(name))
		return false;

	if (!CResourceManager::GetInst()->LoadTexture(name, fileName, pathName))
		return false;

	CTexture* texture = CResourceManager::GetInst()->FindTexture(name);

	m_mapTexture.insert(std::make_pair(name, texture));

	return true;
}

bool CSceneResource::LoadTextureFullPath(const std::string& name, const TCHAR* fullPath)
{
	if (FindTexture(name))
		return false;

	if (!CResourceManager::GetInst()->LoadTextureFullPath(name, fullPath))
		return false;

	CTexture* texture = CResourceManager::GetInst()->FindTexture(name);

	m_mapTexture.insert(std::make_pair(name, texture));

	return true;
}

CTexture* CSceneResource::FindTexture(std::string_view name)
{
	auto iter = m_mapTexture.find(name.data());

	if (iter == m_mapTexture.end())
	{
		CTexture* texture = CResourceManager::GetInst()->FindTexture(name);

		if (!texture)
			return nullptr;

		m_mapTexture.insert(std::make_pair(name, texture));

		return texture;
	}

	return iter->second;
}
