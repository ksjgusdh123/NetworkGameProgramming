#include "ResourceManager.h"
#include "Texture/TextureManager.h"

DEFINITION_SINGLE(CResourceManager)

CResourceManager::CResourceManager()
{
}

CResourceManager::~CResourceManager()
{
}

bool CResourceManager::Init()
{
    m_textureManager = std::make_unique<CTextureManager>();

    if (!m_textureManager->Init())
        return false;

    return true;
}

bool CResourceManager::LoadTexture(const std::string& name, const std::wstring& fileName, const std::string& pathName)
{
    return m_textureManager->LoadTexture(name, fileName, pathName);
}

bool CResourceManager::LoadTextureFullPath(const std::string& name, const TCHAR* fullPath)
{
    return m_textureManager->LoadTextureFullPath(name, fullPath);
}

CTexture* CResourceManager::FindTexture(std::string_view name)
{
    return m_textureManager->FindTexture(name);
}

void CResourceManager::ReleaseTexture(std::string_view name)
{
    m_textureManager->ReleaseTexture(name);
}
