// BmpImage.cpp: implementation of the CBmpImage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BmpImage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBmpImage::CBmpImage()
{
	m_nColor = 0;
	m_lpLogPalette = NULL;
	m_pBitmapInfo = NULL;
	m_pDib = NULL;
	m_pPalette = NULL;
	m_ptDib = NULL;
	m_pBitmap = NULL;
}

CBmpImage::~CBmpImage()
{
	if(m_lpLogPalette != NULL)
	{
		delete m_lpLogPalette;
		m_lpLogPalette = NULL;
	}
	if(m_pBitmapInfo != NULL)
	{
		delete m_pBitmapInfo;
		m_pBitmapInfo = NULL;
	}
	if(m_pPalette != NULL)
	{
		delete m_pPalette;
		m_pPalette = NULL;
	}
	if(m_pBitmap != NULL)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}

//	if(m_pDib != NULL) delete m_pDib;
}

BOOL CBmpImage::LoadBmp(CString fname)
{
	m_strFileName = fname;
	HANDLE fd = CreateFile(m_strFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(fd<0) return FALSE;
	
	DWORD size, len;
	if(!ReadFile(fd, (LPSTR)&m_bmpHeader, sizeof(m_bmpHeader), &len, NULL)) return FALSE;
	if(m_bmpHeader.bfType != DIB_HEADER_MARKER) return FALSE;
	
	size = m_bmpHeader.bfSize - sizeof(m_bmpHeader);
	m_ptDib = new char[size];
	if(!ReadFile(fd, m_ptDib, size, &len, NULL)) return FALSE;
	if(len != size) return FALSE;
	
	CloseHandle(fd);
	
	m_pBitmapInfo = (LPBITMAPINFO)m_ptDib;
	switch(m_pBitmapInfo->bmiHeader.biBitCount)
	{
		case 24:
			m_nColor = 0;
			break;
		case 8:
			m_nColor = 256;
			break;
		case 4:
			m_nColor = 16;
			break;
		case 1:
			m_nColor = 2;
			break;
	}
	
	m_pDib = (m_ptDib + *(LPDWORD)m_ptDib + (m_nColor * sizeof(RGBQUAD)));

	if(m_nColor)
	{
		if(!CreatePalette(TRUE)) return FALSE;
	}

	MakeBitmap();
	
	return TRUE;
}

BOOL CBmpImage::DrawImg(int x, int y, CDC *pDC)
{
	HDC hDC = pDC->m_hDC;
	HPALETTE hPal = NULL;
	HPALETTE hOldPal = NULL;

	if(m_pDib == NULL) return FALSE;
	
	if(m_pPalette != NULL)
	{
		hPal = (HPALETTE) m_pPalette->m_hObject;
		hOldPal = ::SelectPalette(hDC, hPal, TRUE);
		RealizePalette(hDC);
	}
	else
	{
		if(SetSysPalette(hDC)) RealizePalette(hDC);
	}

	::SetStretchBltMode(hDC, COLORONCOLOR);
	::SetDIBitsToDevice(hDC, x, y, GetSize().cx, GetSize().cy, 0, 0, 0, (WORD)GetSize().cy, m_pDib, m_pBitmapInfo, DIB_RGB_COLORS);

	if(hOldPal != NULL)
	{
		::SelectPalette(hDC, hOldPal, TRUE);
	}
	
	return TRUE;
}

BOOL CBmpImage::DrawImg(int x, int y, int width, int height, CDC* pDC)
{
	HDC hDC = pDC->m_hDC;
	HPALETTE hPal = NULL;
	HPALETTE hOldPal = NULL;

	if(m_pDib == NULL) return FALSE;
	
	if(m_pPalette != NULL)
	{
		hPal = (HPALETTE) m_pPalette->m_hObject;
		hOldPal = ::SelectPalette(hDC, hPal, TRUE);
		RealizePalette(hDC);
	}
	else
	{
		if(SetSysPalette(hDC)) RealizePalette(hDC);
	}

	::SetStretchBltMode(hDC, COLORONCOLOR);
	::SetDIBitsToDevice(hDC, x, y, width, height, 0, 0, 0, (WORD)height, m_pDib, m_pBitmapInfo, DIB_RGB_COLORS);

	if(hOldPal != NULL)
	{
		::SelectPalette(hDC, hOldPal, TRUE);
	}
	
	return TRUE;
}

void CBmpImage::MakeBitmap()
{
	if(m_pBitmap) delete m_pBitmap;
	m_pBitmap = NULL;

	CDC memDC;
	CDC ScreenDC;

	m_pBitmap = new CBitmap;
	ScreenDC.CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	memDC.CreateCompatibleDC(&ScreenDC);
	m_pBitmap->CreateCompatibleBitmap(&ScreenDC, GetSize().cx, GetSize().cy);
	memDC.SelectObject(m_pBitmap);
	DrawImg(0, 0, &memDC);
}

SIZE CBmpImage::GetSize()
{
	SIZE size;

	size.cx = size.cy = 0;
	if(m_pBitmapInfo == NULL) return size;
	else
	{
		size.cx = m_pBitmapInfo->bmiHeader.biWidth;
		size.cy = m_pBitmapInfo->bmiHeader.biHeight;
	}
	return size;
}

BOOL CBmpImage::SetSysPalette(HDC hDC)
{
	if(m_nColor == 0) return FALSE;
	if(!(::GetDeviceCaps(hDC, RASTERCAPS) & RC_PALETTE)) return FALSE;

	int nSysColors = ::GetDeviceCaps(hDC, NUMCOLORS);
	int nPalEntries = ::GetDeviceCaps(hDC, SIZEPALETTE);
	if(nPalEntries == 0) return FALSE;

	int nEntries = (nPalEntries == 0) ? nSysColors : nPalEntries;
	LPLOGPALETTE pLogPal = (LPLOGPALETTE) new char[2 * sizeof(WORD) + nEntries * sizeof(PALETTEENTRY)];
	pLogPal->palVersion = 0x300;
	pLogPal->palNumEntries = nEntries;

	::GetSystemPaletteEntries(hDC, 0, nEntries, (LPPALETTEENTRY)((LPBYTE)pLogPal+2*sizeof(WORD)));
	HPALETTE m_hPalette;

	m_hPalette = ::CreatePalette(pLogPal);
	SelectPalette(hDC, m_hPalette, TRUE);
	delete pLogPal;

	return TRUE;
}

BOOL CBmpImage::CreatePalette(BOOL mode)
{
	int r, g, b;
	
	if(m_pPalette) delete m_pPalette;
	m_pPalette = NULL;

	if(m_lpLogPalette) delete m_lpLogPalette;
	m_lpLogPalette = NULL;

	m_lpLogPalette = (LPLOGPALETTE) new char[sizeof(LOGPALETTE)+sizeof(PALETTEENTRY)*m_nColor];
	m_pPalette = new CPalette;
	if(m_lpLogPalette == NULL || m_pPalette==NULL) return FALSE;

	m_lpLogPalette->palVersion = PALVERSION;
	m_lpLogPalette->palNumEntries=m_nColor;

	if(mode)
	{
		for(int i=0; i<m_nColor; i++)
		{
			m_lpLogPalette->palPalEntry[i].peRed = m_pBitmapInfo->bmiColors[i].rgbRed;
			m_lpLogPalette->palPalEntry[i].peGreen = m_pBitmapInfo->bmiColors[i].rgbGreen;
			m_lpLogPalette->palPalEntry[i].peBlue = m_pBitmapInfo->bmiColors[i].rgbBlue;
		}
	}
	else
	{
		int i=0;
		for(r=0; r<256; r+=51)
		{
			for(g=0; g<256; g+=51)
			{
				for(b=0; b<256; b+=51)
				{
					m_lpLogPalette->palPalEntry[i].peRed = r;
					m_pBitmapInfo->bmiColors[i].rgbRed = r;
					m_lpLogPalette->palPalEntry[i].peGreen = g;
					m_pBitmapInfo->bmiColors[i].rgbGreen = g;
					m_lpLogPalette->palPalEntry[i].peBlue = b;
					m_pBitmapInfo->bmiColors[i].rgbBlue = b;
					i++;
					if(i>m_nColor) return TRUE;
				}
			}
		}
	}

	m_pPalette->CreatePalette(m_lpLogPalette);
	
	return TRUE;
}
