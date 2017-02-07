// BmpImageST.cpp : implementation file
//

#include "stdafx.h"
#include "BmpImageST.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBmpImageST

CBmpImageST::CBmpImageST()
{
	m_nOffsetX = 0;
	m_nOffsetY = 0;
	m_nWidth = 0;
	m_nHeight = 0;
	m_nRadius = 0;
	m_nOrgRadius = 0;
	m_nStyle = JS_CIRCLE;
	m_nArrowIdx = 0;
	m_nArrowWidth = 0;
	m_nArrowHeight = 0;
	m_nMinRadius = 0;
	m_nTCnt = 0;
	m_nLineWidth = 1;
	m_LineColor = RGB(255, 0, 0);

	m_bLargeX = FALSE;
	m_bLargeY = FALSE;
	m_bCenterImg = TRUE;
	m_bFirst = TRUE;
	
	m_pBackImage = NULL;
	m_pArrow = NULL;
	m_pArrowBack = NULL;
}

CBmpImageST::~CBmpImageST()
{
	if(m_pBackImage) delete m_pBackImage;
	if(m_pArrow) delete m_pArrow;
	if(m_pArrowBack) delete m_pArrowBack;
}


BEGIN_MESSAGE_MAP(CBmpImageST, CStatic)
	//{{AFX_MSG_MAP(CBmpImageST)
	ON_WM_PAINT()
	//}}AFX_MSG_MAPON_WM_TIMER()
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBmpImageST message handlers

BOOL CBmpImageST::LoadImage(CString fname, BOOL bCenterImg)
{
	if(m_pBackImage) delete m_pBackImage;

	m_pBackImage = new CBmpImage;
	if(!(m_pBackImage->LoadBmp(fname))) return FALSE;

	m_bCenterImg = bCenterImg;
	GetClientRect(&m_Rect);
	if(m_Rect.Width() < m_pBackImage->GetSize().cx)
	{
		m_nWidth = m_Rect.Width();
		m_bLargeX = TRUE;
	}
	else
	{
		m_nWidth = m_pBackImage->GetSize().cx;
		m_bLargeX = FALSE;
	}
	
	if(m_Rect.Height() < m_pBackImage->GetSize().cy)
	{
		m_nHeight = m_Rect.Height();
		m_bLargeY = TRUE;
	}
	else
	{
		m_nHeight = m_pBackImage->GetSize().cy;
		m_bLargeY = FALSE;
	}

	if(bCenterImg)
	{
		m_nOffsetX = (int) (m_Rect.Width()-m_pBackImage->GetSize().cx)/2;
		m_nOffsetY = (int) (m_Rect.Height()-m_pBackImage->GetSize().cy)/2;
	}
	else
	{
		m_nOffsetX = m_nOffsetY = 0;
	}

	return TRUE;
}

BOOL CBmpImageST::LoadArrow(int style)
{
	if(m_pArrow) delete m_pArrow;
	if(m_pArrowBack) delete m_pArrowBack;
	m_pArrow = new CBmpImage;
	m_pArrowBack = new CBmpImage;

	int len = m_strArrowPath.GetLength();
	if(m_strArrowPath.Right(1) != _T("\\")) m_strArrowPath += _T("\\");
	CString arrowpath, arrowbackpath;

	switch(style)
	{
	case JS_ARROW_LEFT:
	case JS_ARROW_RIGHT:
		arrowpath = m_strArrowPath + _T("arrow_lr.bmp");
		arrowbackpath = m_strArrowPath + _T("arrowback_lr.bmp");
		break;
	case JS_ARROW_UP:
	case JS_ARROW_DOWN:
		arrowpath = m_strArrowPath + _T("arrow_ud.bmp");
		arrowbackpath = m_strArrowPath + _T("arrowback_ud.bmp");
		break;
	case JS_ARROW_LEFTUP:
	case JS_ARROW_LEFTDOWN:
		arrowpath = m_strArrowPath + _T("arrow_ld.bmp");
		arrowbackpath = m_strArrowPath + _T("arrowback_ld.bmp");
		break;
	case JS_ARROW_RIGHTUP:
	case JS_ARROW_RIGHTDOWN:
		arrowpath = m_strArrowPath + _T("arrow_rd.bmp");
		arrowbackpath = m_strArrowPath + _T("arrowback_rd.bmp");
		break;
	default:
		return FALSE;
	}

	if(!(m_pArrow->LoadBmp(arrowpath)) || !(m_pArrowBack->LoadBmp(arrowbackpath))) return FALSE;
	return TRUE;
}

void CBmpImageST::SetCircleStyle(int cx, int cy, int cradius)
{
	m_nCx = cx;
	m_nCy = cy;
	m_nMinRadius = cradius;
	m_nStyle = JS_CIRCLE;
	
	int rx, ry;

	if(m_bCenterImg)
	{
		if(m_bLargeX)
		{
			((m_nCx+m_nOffsetX) > m_Rect.Width()/2) ? rx = m_nCx+m_nOffsetX : rx = m_Rect.Width() - (m_nCx+m_nOffsetX);
		}
		else
		{
			(m_nCx > m_pBackImage->GetSize().cx/2) ? rx = m_nCx : rx = m_pBackImage->GetSize().cx-m_nCx;
		}
		if(m_bLargeY)
		{
			((m_nCy+m_nOffsetY) > m_Rect.Height()/2) ? ry = m_nCy+m_nOffsetY : ry = m_Rect.Height() - (m_nCy+m_nOffsetY);
		}
		else
		{
			(m_nCy > m_pBackImage->GetSize().cy/2) ? ry = m_nCy : ry = m_pBackImage->GetSize().cy-m_nCy;
		}
		(rx > ry) ? m_nRadius = rx : m_nRadius = ry;
	}
	else
	{
		(m_nWidth > m_nHeight) ? m_nRadius=m_nWidth : m_nRadius=m_nHeight;
	}

	m_nRDec = (int) (m_nRadius-m_nMinRadius)/6;
}

void CBmpImageST::SetLineStyle(int width, COLORREF color)
{
	m_nLineWidth = width;
	m_LineColor = color;
}

void CBmpImageST::SetArrowStyle(int cx, int cy, int style, CString arrowpath, int offset)
{
	m_nCx = cx;
	m_nCy = cy;
	m_nMinRadius = offset;
	m_nStyle = style;
	m_strArrowPath = arrowpath;

	if(LoadArrow(style))
	{
		m_nArrowIdx = 0;
		
		m_nArrowWidth = m_pArrow->GetSize().cx/16;
		m_nArrowHeight = m_pArrow->GetSize().cy;

		switch(m_nStyle)
		{
		case JS_ARROW_LEFT:
			m_nCx = m_nCx - (m_nMinRadius + m_nArrowWidth);
			m_nCy = m_nCy - m_nArrowHeight/2;
			break;
		case JS_ARROW_RIGHT:
			m_nCx = m_nCx + m_nMinRadius;
			m_nCy = m_nCy - m_nArrowHeight/2;
			break;
		case JS_ARROW_UP:
			m_nCx = m_nCx - m_nArrowWidth/2;
			m_nCy = m_nCy + m_nMinRadius;
			break;
		case JS_ARROW_DOWN:
			m_nCx = m_nCx - m_nArrowWidth/2;
			m_nCy = m_nCy - (m_nMinRadius + m_nArrowWidth);
			break;
		case JS_ARROW_LEFTUP:
			m_nCx = m_nCx - ((int)sqrt((double)m_nMinRadius) + m_nArrowWidth);
			m_nCy = m_nCy + ((int)sqrt((double)m_nMinRadius));
			break;
		case JS_ARROW_LEFTDOWN:
			m_nCx = m_nCx - ((int)sqrt((double)m_nMinRadius) + m_nArrowWidth);
			m_nCy = m_nCy - ((int)sqrt((double)m_nMinRadius) + m_nArrowHeight);
			break;
		case JS_ARROW_RIGHTUP:
			m_nCx = m_nCx + (int)sqrt((double)m_nMinRadius);
			m_nCy = m_nCy + (int)sqrt((double)m_nMinRadius);
			break;
		case JS_ARROW_RIGHTDOWN:
			m_nCx = m_nCx + (int)sqrt((double)m_nMinRadius);
			m_nCy = m_nCy - ((int)(sqrt((double)m_nMinRadius)) + m_nArrowHeight);
			break;
		}
	}
}

void CBmpImageST::Paint(CDC *pDC)
{
	if(!m_pBackImage) return;
	if(m_bFirst)
	{
		if(m_nStyle != JS_NONE) 
		{
			SetTimer(3050, 100, NULL);
			m_nTCnt = 0;
			m_nOrgRadius = m_nRadius;
			m_crBk = pDC->GetPixel(1, 1);
		}
		m_bFirst = FALSE;
	}

	m_pBackImage->MakeBitmap();

	CDC aniDC;
	aniDC.CreateCompatibleDC(pDC);
	aniDC.SelectObject(m_pBackImage->m_pBitmap);

	if(m_nStyle == JS_CIRCLE)
	{
		CPen cpen(PS_SOLID, m_nLineWidth, m_LineColor);
		CBrush* pOldBrush = (CBrush*)aniDC.SelectStockObject(NULL_BRUSH);
		CPen* pOldPen = (CPen*)aniDC.SelectObject(&cpen);
		
		aniDC.Ellipse(m_nCx-m_nRadius, m_nCy-m_nRadius, m_nCx+m_nRadius, m_nCy+m_nRadius);
		
		int d = (int) m_nRadius/5;
		
		aniDC.MoveTo(CPoint(m_nCx-m_nRadius-d, m_nCy));
		aniDC.LineTo(CPoint(m_nCx-m_nRadius+d, m_nCy));
		aniDC.MoveTo(CPoint(m_nCx+m_nRadius-d, m_nCy));
		aniDC.LineTo(CPoint(m_nCx+m_nRadius+d, m_nCy));
		aniDC.MoveTo(CPoint(m_nCx, m_nCy-m_nRadius-d));
		aniDC.LineTo(CPoint(m_nCx, m_nCy-m_nRadius+d));
		aniDC.MoveTo(CPoint(m_nCx, m_nCy+m_nRadius-d));
		aniDC.LineTo(CPoint(m_nCx, m_nCy+m_nRadius+d));
		
		aniDC.SelectObject(pOldBrush);
		aniDC.SelectObject(pOldPen);
	}
	else if(m_nStyle != JS_NONE)
	{
		int idx;
		switch(m_nStyle)
		{
		case JS_ARROW_LEFT:
		case JS_ARROW_UP:
		case JS_ARROW_LEFTUP:
		case JS_ARROW_RIGHTUP:
			idx = m_nArrowIdx;
			break;
		case JS_ARROW_RIGHT:
		case JS_ARROW_DOWN:
		case JS_ARROW_LEFTDOWN:
		case JS_ARROW_RIGHTDOWN:
			idx = m_nArrowIdx + 8;
			break;
		default:
			return;
			break;
		}

		CDC maskDC, arrowDC;
		maskDC.CreateCompatibleDC(pDC);
		arrowDC.CreateCompatibleDC(pDC);
		maskDC.SelectObject(m_pArrowBack->m_pBitmap);
		arrowDC.SelectObject(m_pArrow->m_pBitmap);
		
		aniDC.BitBlt(m_nCx, m_nCy, m_nArrowWidth, m_nArrowHeight, &maskDC, idx*m_nArrowWidth, 0, SRCAND);
		aniDC.BitBlt(m_nCx, m_nCy, m_nArrowWidth, m_nArrowHeight, &arrowDC, idx*m_nArrowWidth, 0, SRCPAINT);
	}
	int tx, ty, sx, sy;
	if(m_bLargeX)
	{
		tx = 0;
		sx = -m_nOffsetX;
	}
	else
	{
		tx = m_nOffsetX;
		sx = 0;
	}
	if(m_bLargeY)
	{
		ty = 0;
		sy = -m_nOffsetY;
	}
	else
	{
		ty = m_nOffsetY;
		sy = 0;
	}
	pDC->BitBlt(tx, ty, m_nWidth, m_nHeight, &aniDC, sx, sy, SRCCOPY);
}

void CBmpImageST::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	Paint(&dc);

	// Do not call CStatic::OnPaint() for painting messages
}


void CBmpImageST::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	static int radius = m_nRadius;

	if(m_nStyle == JS_CIRCLE)
	{
		if(m_nTCnt < 6) 
		{
			m_nRadius -= m_nRDec;
			Invalidate();
		}
		else if(m_nTCnt >= 9) 
		{
			m_nRadius = m_nOrgRadius;
			m_nTCnt = -1;
		}
	}
	else
	{
		if(m_nTCnt < 8) 
		{
			m_nArrowIdx = m_nTCnt;
			Invalidate();
		}
		else if(m_nTCnt >= 8) m_nTCnt = -1;
	}
	m_nTCnt++;
	
	CStatic::OnTimer(nIDEvent);
}

void CBmpImageST::ClearDrawing()
{
	if(!m_pBackImage) return;
	if(m_nStyle != JS_NONE)
	{
		KillTimer(3050);
	}
	m_bFirst = TRUE;
	m_nStyle = JS_NONE;

	CDC *pDC = GetDC();

	CBrush br;
	br.CreateSolidBrush(m_crBk);
	CBrush *old = pDC->SelectObject(&br);
	CRect r(m_Rect.left, m_Rect.top, m_Rect.right, m_Rect.bottom);
	pDC->FillRect(&r, &br);
	pDC->SelectObject(old);

	ReleaseDC(pDC);
}

void CBmpImageST::Delete()
{
	if(m_pBackImage != NULL)
	{
		delete m_pBackImage;
		m_pBackImage = NULL;
	}

	if(m_pArrow != NULL)
	{
		delete m_pArrow;
		m_pArrow = NULL;
	}

	if(m_pArrowBack != NULL)
	{
		delete m_pArrowBack;
		m_pArrowBack = NULL;
	}
}
