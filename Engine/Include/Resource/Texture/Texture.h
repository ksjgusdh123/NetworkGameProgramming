#pragma once

#include "../../Ref.h"

struct ImageInfo
{
	HDC			hMemDC				= 0;
	HBITMAP		hBmp				= 0;
	HBITMAP		hPrevBmp			= 0;	
	CImage		cImage;
	BITMAP		bmpInfo				= {};
	COLORREF	colorKey			= RGB(255, 0, 255);
	bool		enableColorKey		= false;
	TCHAR		fileName[MAX_PATH]	= {};
	TCHAR		fullPath[MAX_PATH]	= {};
	std::string	pathName;

	~ImageInfo()
	{
		if (hMemDC)
		{
			SelectObject(hMemDC, hPrevBmp);
			DeleteDC(hMemDC);
		}
		if (hBmp)
		{
			DeleteObject(hBmp);
		}
	}
};

class CTexture : public CRef
{
public:
	CTexture();
	virtual ~CTexture();
public:
	int GetWidth(int idx = 0) const					{ return (int)m_vecImageInfo[idx]->bmpInfo.bmWidth; }
	int GetHeight(int idx = 0) const				{ return (int)m_vecImageInfo[idx]->bmpInfo.bmHeight; }
	ETexture_Type GetTextureType() const			{ return m_type; }
	HDC GetDC(int idx = 0) const					{ return m_vecImageInfo[idx]->hMemDC; }
	bool GetEnableColorKey(int idx = 0) const		{ return m_vecImageInfo[idx]->enableColorKey; }
	COLORREF GetColorKey(int idx = 0) const			{ return m_vecImageInfo[idx]->colorKey; }
	CImage& GetCImage(int idx = 0) const			{ return m_vecImageInfo[idx]->cImage; }

	void SetTextureType(ETexture_Type type) { m_type = type; }

	void SetColorKey(unsigned char r, unsigned char g, unsigned char b, int idx = 0)
	{
		m_vecImageInfo[idx]->colorKey = RGB(r, g, b);
		m_vecImageInfo[idx]->enableColorKey = true;
	}

public:
	bool LoadTexture(const std::wstring& fileName, const std::string& pathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const TCHAR* fullPath);

private:
	ETexture_Type							m_type = ETexture_Type::Sprite;
	std::vector<std::shared_ptr<ImageInfo>>	m_vecImageInfo;
};

