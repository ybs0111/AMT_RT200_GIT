// BmpImage.h: interface for the CBmpImage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BMPIMAGE_H__85167FFC_B6E7_4DE7_B2BB_ACAEC38D4965__INCLUDED_)
#define AFX_BMPIMAGE_H__85167FFC_B6E7_4DE7_B2BB_ACAEC38D4965__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PALVERSION	0x300
#define DIB_HEADER_MARKER	((WORD)('M'<<8) | 'B')

class CBmpImage  
{
public:
	CBitmap* m_pBitmap;

public:
	void MakeBitmap();
	BOOL DrawImg(int x, int y, CDC* pDC);
	BOOL DrawImg(int x, int y, int width, int height, CDC* pDC);
	SIZE GetSize();
	BOOL LoadBmp(CString fname);
	CBmpImage();
	virtual ~CBmpImage();

protected:
	LPLOGPALETTE m_lpLogPalette;
	BITMAPFILEHEADER m_bmpHeader;
	LPBITMAPINFO m_pBitmapInfo;
	LPSTR m_pDib;
	LPSTR m_ptDib;
	CString m_strFileName;
	CPalette* m_pPalette;
	int m_nColor;

protected:
	BOOL CreatePalette(BOOL mode);
	BOOL SetSysPalette(HDC hDC);
};

#endif // !defined(AFX_BMPIMAGE_H__85167FFC_B6E7_4DE7_B2BB_ACAEC38D4965__INCLUDED_)
