#include "Texture.h"
#include "../../PathManager.h"
#include "../../Engine.h"

CTexture::CTexture()
{
	SetTypeID<CTexture>();
}

CTexture::~CTexture()
{
}

bool CTexture::LoadTexture(const std::wstring& fileName, const std::string& pathName)
{
	std::filesystem::path path = CPathManager::GetInst()->FindPath(pathName);
	path /= fileName;

	if (!LoadTextureFullPath(path.c_str()))
		return false;

	lstrcpy(m_vecImageInfo.back()->fileName, fileName.c_str());
	m_vecImageInfo.back()->pathName = pathName;

	return true;
}

bool CTexture::LoadTextureFullPath(const TCHAR* fullPath)
{
	// 화면DC를 넣고 메모리 DC를 얻는다.
	HDC	hDC = CreateCompatibleDC(CEngine::GetInst()->GetWindowDC());

	// 비트맵을 로딩한다.
	HBITMAP	hBmp = (HBITMAP)LoadImage(CEngine::GetInst()->GetWindowInstance(),
		fullPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	std::shared_ptr<ImageInfo> info = std::make_shared<ImageInfo>();
	if (!hBmp)
	{
		info->cImage.Load(fullPath);
		m_vecImageInfo.push_back(info);
		return true;
		//return false;
	}

	// 읽어온 비트맵을 메모리 DC에 지정한다.
	// 기존에 DC가 가지고 있던 도구를 반환한다.
	HBITMAP hPrevBmp = (HBITMAP)SelectObject(hDC, hBmp);

	BITMAP	bmpInfo;
	GetObject(hBmp, sizeof(BITMAP), &bmpInfo);

	info->hMemDC = hDC;
	info->hBmp = hBmp;
	info->hPrevBmp = hPrevBmp;
	info->bmpInfo = bmpInfo;
	lstrcpy(info->fullPath, fullPath);

	m_vecImageInfo.push_back(info);

	return true;
}
