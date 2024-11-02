#include "TextureManager.h"
#include "Texture.h"

CTextureManager::CTextureManager()
{
}

CTextureManager::~CTextureManager()
{
}

bool CTextureManager::Init()
{
	return true;
}

bool CTextureManager::LoadTexture(const std::string& name, const std::wstring& fileName, const std::string& pathName)
{
	if (FindTexture(name))
		return true;

	std::shared_ptr<CTexture> texture = std::make_shared<CTexture>();
	texture->SetName(name);

	if (!texture->LoadTexture(fileName, pathName))
		return false;

	m_mapTexture.insert(std::make_pair(name, texture));

	return true;
}

bool CTextureManager::LoadTextureFullPath(const std::string& name, const TCHAR* fullPath)
{
	if (FindTexture(name))
		return true;

	std::shared_ptr<CTexture> texture = std::make_shared<CTexture>();
	texture->SetName(name);

	if (!texture->LoadTextureFullPath(fullPath))
		return false;

	m_mapTexture.insert(std::make_pair(name, texture));

	return true;
}

CTexture* CTextureManager::FindTexture(std::string_view name)
{
	auto iter = m_mapTexture.find(name.data());

	if (iter == m_mapTexture.end())
		return nullptr;

	return iter->second.get();
}

void CTextureManager::ReleaseTexture(std::string_view name)
{
	auto iter = m_mapTexture.find(name.data());

	if (iter == m_mapTexture.end())
		return;

	if (iter->second->GetRefCount() == 1)
		m_mapTexture.erase(iter);
}
