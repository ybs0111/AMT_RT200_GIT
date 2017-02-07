///////////////////////////////////////////////////////////////////////////////
/* ****************************************************************************** */
// FILE NAME : UniButtonEx.cpp
//
// Copyright (C) 2003 AMT Corporation
// All rights reserved.
//
// CREATED BY  : Statically
// MODIFIED BY : Lee Kyu Il
//             : Lee Kyong Koo
//             : Lee Kyu Il
// REVISION    :
//     1> 2000. 07. 25 : Statically
//     2> 2000. 08. 02 : Lee Kyu Il
//     3> 2000. 08. 22 : Lee Kyong Koo
//     4> 2003. 09. 22 : Lee Kyu Il
// DESCRIPTION :
//     Implementation of the UniButtonEx and associated classes
//	   This code may be used in compiled form in any way you desire. This
//	   file may be redistributed unmodified by any means PROVIDING it is
//	   not sold for profit without the authors written consent, and
//	   providing that this notice and the authors name and all copyright
//	   notices remains intact.
//
//	   An email letting me know how you are using it would be nice as well.
//
//
// REFERENCE   :
// 
/* ****************************************************************************** */
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UniButtonEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUniButtonEx

CUniButtonEx::CUniButtonEx()
{
	m_nColor = GetSysColor(COLOR_BTNFACE);
	m_sColor = m_nColor;
	m_hColor = m_nColor;
	m_dColor = m_nColor;
	m_nBorder = 1;
	m_lfEscapement = 0;
	m_pNormal = NULL;
	m_pSelected = NULL;
	m_pHover = NULL;
	m_pDisabled = NULL;
	m_hRgn = 0;
	m_bHover = false;
	m_bCapture = false;
	m_bMouseDown = false;
	m_bNeedBitmaps = true;
}

CUniButtonEx::~CUniButtonEx()
{
	delete m_pNormal;
	delete m_pSelected;
	delete m_pHover;
	delete m_pDisabled;
	DeleteObject(m_hRgn);
}

BEGIN_MESSAGE_MAP(CUniButtonEx, CButton)
	//{{AFX_MSG_MAP(CUniButtonEx)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_SETTEXT, OnSetText )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUniButtonEx message handlers

BOOL CUniButtonEx::Create(LPCTSTR lpszCaption, DWORD dwStyle, const CPoint point, const HRGN hRgn, CWnd* pParentWnd, UINT nID)
{
	// store region in member variable
	DeleteObject(m_hRgn);
	m_hRgn = CreateRectRgn(0, 0, 31, 31);
	CRect box(0, 0, 0, 0);
	if (m_hRgn != 0) 
		CombineRgn(m_hRgn, hRgn, 0, RGN_COPY);

	// make sure that region bounding rect is located in (0, 0)
	GetRgnBox(m_hRgn, &box);
	OffsetRgn(m_hRgn, -box.left, -box.top);
	GetRgnBox(m_hRgn, &box);

	// update position of region center for caption output
	m_CenterPoint = CPoint(box.left + box.Width() /2 , box.top + box.Height() /2);
	box.OffsetRect(point);

	return CButton::Create(lpszCaption, dwStyle, box, pParentWnd, nID);
}

BOOL CUniButtonEx::Create(LPCTSTR lpszCaption, DWORD dwStyle, const CPoint point, const HRGN hRgn, CWnd* pParentWnd, UINT nID, COLORREF color)
{
	m_sColor = color;
	m_hColor = color;

	// call another constructor
	return Create(lpszCaption, dwStyle, point, hRgn, pParentWnd, nID);
}

BOOL CUniButtonEx::Create(LPCTSTR lpszCaption, DWORD dwStyle, const CPoint point, const HRGN hRgn, CWnd* pParentWnd, UINT nID, UINT nBorder, LONG lfEscapement, COLORREF nColor, COLORREF sColor, COLORREF hColor, COLORREF dColor)
{
	m_lfEscapement = lfEscapement;
	return Create(lpszCaption, dwStyle, point, hRgn, pParentWnd, nID, nBorder, nColor, sColor, hColor, dColor);
}

BOOL CUniButtonEx::Create(LPCTSTR lpszCaption, DWORD dwStyle, const CPoint point, const HRGN hRgn, CWnd* pParentWnd, UINT nID, UINT nBorder, COLORREF nColor, COLORREF sColor, COLORREF hColor, COLORREF dColor)
{
	// change default colors
	m_nBorder = nBorder;
	m_nColor = nColor;
	m_sColor = sColor;
	m_hColor = hColor;
	m_dColor = dColor;

	// call another constructor
	return Create(lpszCaption, dwStyle, point, hRgn, pParentWnd, nID);
}

void CUniButtonEx::PreSubclassWindow() 
{
	// change window style to allow owner draw
	ModifyStyle(0, BS_OWNERDRAW | BS_PUSHBUTTON);	
	CButton::PreSubclassWindow();
}

int CUniButtonEx::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;

	// assign new region to a window
	m_bNeedBitmaps = true;
	SetWindowRgn(m_hRgn, true);

	return 0;
}

void CUniButtonEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// record that mouse is down
	m_bMouseDown = true;
	if (!m_bCapture) {
		SetCapture();
		m_bCapture = true;
	}
	CButton::OnLButtonDown(nFlags, point);
}

void CUniButtonEx::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// record that mouse is released
	CButton::OnLButtonUp(nFlags, point);

	m_bMouseDown = false;	
	if (m_bCapture) {
		ReleaseCapture();
		m_bCapture = false;
	}
	CheckHover(point);
}

void CUniButtonEx::OnMouseMove(UINT nFlags, CPoint point) 
{
	// Test if mouse is above the button.
	/*
	if (!m_bMouseDown)
		CheckHover(point);
	*/


	/* ************************************************************************** */
    /* 좌측 마우스 눌린 상태에서 마우스 드래그하는 경우 대비하여 추가 함          */
	/* -> 규이리 추가 (2003.09.22)                                                */
    /* ************************************************************************** */
	if (!m_bMouseDown)
		CheckHover(point);
	    // BEGIN - Added Code
	else
	{
		// Added this code to handle correctly the button
		if (! HitTest(point))
		{
			SetState(FALSE);
			return;
		}
	}
	/* ************************************************************************** */

	CButton::OnMouseMove(nFlags, point);
}

void CUniButtonEx::CheckHover(CPoint point)
{
		if (HitTest(point)) {
			if (!m_bCapture) {
				SetCapture();
				m_bCapture = true;
			}
			if (!m_bHover) {
				m_bHover = true;
				RedrawWindow();
			}
		}
		else {
			if (m_bCapture) {
				ReleaseCapture();
				m_bCapture = false;
			}
			m_bHover = false;
			RedrawWindow();
		}
}

LRESULT CUniButtonEx::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// I have noticed that default windows buttons can be clicked very quickly.
	// Double or single click both result in a button being pushed down.
	// For owner drawn buttons this is not the case. Double click does
	// not push button down. Here is a solution for the problem:
	// double click message is substituted for single click.

	if (message == WM_LBUTTONDBLCLK)
		message = WM_LBUTTONDOWN;
	
	return CButton::DefWindowProc(message, wParam, lParam);
}

BOOL CUniButtonEx::HitTest(CPoint point)
{
	BOOL result = false;

	// Obtain handle to window region.
	HRGN hRgn = CreateRectRgn(0, 0, 0, 0);
	GetWindowRgn(hRgn);
	CRect rgnRect;
	GetRgnBox(hRgn, &rgnRect);

	// First check if point is in region bounding rect.
	// Then check if point is in the region in adition to being in the bouding rect.
	result = PtInRect(&rgnRect, point) && PtInRegion(hRgn, point.x, point.y);

	// Clean up and exit.
	DeleteObject(hRgn);
	return result;
}

BOOL CUniButtonEx::OnEraseBkgnd(CDC* pDC) 
{
	// do not erase background
	return 1;
}

//////////////////////// DRAWING ROUTINES ////////////////////////////

void CUniButtonEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// prepare DC
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct -> hDC);
	CRect rect;
	GetClientRect(rect);

	// prepare bitmaps they need to be prepared
	if (m_bNeedBitmaps)
		PrepareStateBitmaps(pDC, &rect);

	// draw button to the screen
	DrawButton(pDC, &rect, lpDrawItemStruct -> itemState);
}

void CUniButtonEx::PrepareStateBitmaps(CDC * pDC, CRect * pRect)
{
	// prepare memory DC
	CDC * pMemDC;
	pMemDC = new CDC;
	pMemDC -> CreateCompatibleDC(pDC);

	// prepare bitmaps for all button states and for the mask
	PrepareNormalState(pDC, pMemDC, pRect);
	PrepareSelectedState(pDC, pMemDC, pRect);
	PrepareHoverState(pDC, pMemDC, pRect);
	PrepareDisabledState(pDC, pMemDC, pRect);

	// clean up
	delete pMemDC; 
	m_bNeedBitmaps = false;
}


void CUniButtonEx::PrepareNormalState(CDC * pDC, CDC * pMemDC, CRect * pRect)
{
	// prepare MYBS_NORMAL state bitmap
	delete m_pNormal;
	m_pNormal = new CBitmap;
	PaintRgn(pDC, pMemDC, m_pNormal, m_nColor, pRect, true, false);
}

void CUniButtonEx::PrepareSelectedState(CDC * pDC, CDC * pMemDC, CRect * pRect)
{
	// prepare MYBS_SELECTED state bitmap
	delete m_pSelected;
	m_pSelected = new CBitmap;
	PaintRgn(pDC, pMemDC, m_pSelected, m_sColor, pRect, true, true);
}

void CUniButtonEx::PrepareHoverState(CDC * pDC, CDC * pMemDC, CRect * pRect)
{
	// prepare MYBS_HOVER state bitmap
	delete m_pHover;
	m_pHover = new CBitmap;
	PaintRgn(pDC, pMemDC, m_pHover, m_hColor, pRect, true, false);
}

void CUniButtonEx::PrepareDisabledState(CDC * pDC, CDC * pMemDC, CRect * pRect)
{
	// prepare MYBS_DISABLED state bitmap
	delete m_pDisabled;
	m_pDisabled = new CBitmap;
	PaintRgn(pDC, pMemDC, m_pDisabled, m_dColor, pRect, false, false);
}

void CUniButtonEx::PaintRgn(CDC * pDC, CDC * pMemDC, CBitmap * pBitmap, COLORREF color, CRect * pRect, BOOL bEnabled, BOOL bSunken)
{
	// create bitmap
	pBitmap -> CreateCompatibleBitmap(pDC, pRect -> Width(), pRect -> Height());
	CBitmap * pOldBitmap = pMemDC -> SelectObject(pBitmap); 

	// prepare region
	HRGN hRgn = CreateRectRgn(0, 0, 0, 0);
	GetWindowRgn(hRgn);

	// fill rect a with transparent color and fill rgn
	HBRUSH hBrush = CreateSolidBrush(color);
	pMemDC -> FillSolidRect(pRect, RGB(0, 0, 0));
	FillRgn(pMemDC -> GetSafeHdc(), hRgn, hBrush);
	DeleteObject(hBrush);

	// draw 3D border and text
	DrawButtonCaption(pMemDC -> GetSafeHdc(), pRect, bEnabled, bSunken);
	FrameRgn3D(pMemDC -> GetSafeHdc(), hRgn, bSunken);

	// clean up
	DeleteObject(hRgn);
	pMemDC -> SelectObject(pOldBitmap); 
}

void CUniButtonEx::DrawButtonCaption(HDC hDC, CRect * pRect, BOOL bEnabled, BOOL bSunken) 
{
	// select parent font
	int nOldMode = SetBkMode(hDC, TRANSPARENT);
	CString text;
	GetWindowText(text);
	LOGFONT lf;
	GetParent() -> GetFont() -> GetLogFont(&lf);
	HFONT hFont = CreateFontIndirect(&lf);
	HFONT hOldFont = (HFONT) SelectObject(hDC, hFont);

	// determine point where to output text
	TEXTMETRIC tm;
	GetTextMetrics(hDC, &tm);
	CPoint p = CPoint(m_CenterPoint.x, m_CenterPoint.y + tm.tmHeight/ 2); 
	if (bSunken) 
		p.Offset(m_nBorder, m_nBorder); 
		
	// draw button caption depending upon button state
	if (bEnabled) {
		SetTextColor(hDC, GetSysColor(COLOR_BTNTEXT));
		SetTextAlign(hDC, TA_CENTER | TA_BOTTOM);
		TextOut(hDC, p.x, p.y, text, text.GetLength());
	}
	else {
		SetTextColor(hDC, GetSysColor(COLOR_3DHILIGHT));
		TextOut(hDC, p.x + 1, p.y + 1, text, text.GetLength());
		SetTextColor(hDC, GetSysColor(COLOR_3DSHADOW));
		TextOut(hDC, p.x, p.y, text, text.GetLength());
	}

	SelectObject(hDC, hOldFont);
	DeleteObject(hFont);
	SetBkMode(hDC, nOldMode);
}

void CUniButtonEx::FrameRgn3D(HDC hDC, const HRGN hRgn, BOOL bSunken)
{
	// we need two differenr regions to keep base region and border region
	HBRUSH hBrush;
	HRGN hBaseRgn = CreateRectRgn(0, 0, 0, 0);

	COLORREF ltOuter, ltInner, rbOuter, rbInner;	// colors of inner and outer shadow for top-left and right-bottom corners

	// decide on color scheme
	if (!bSunken) {
		ltOuter = GetSysColor(COLOR_3DLIGHT);
		ltInner = GetSysColor(COLOR_3DHILIGHT);
		rbOuter = GetSysColor(COLOR_3DDKSHADOW);
		rbInner = GetSysColor(COLOR_3DSHADOW);
	}
	else {
		rbInner = GetSysColor(COLOR_3DLIGHT);
		rbOuter = GetSysColor(COLOR_3DHILIGHT);
		ltInner = GetSysColor(COLOR_3DDKSHADOW);
		ltOuter = GetSysColor(COLOR_3DSHADOW);
	}

	// offset highlight and shadow regions
	// substract them from the base region 
	switch (m_nBorder)
	{
	case 2:
		CombineRgn(hBaseRgn, hRgn, 0, RGN_COPY);
		OffsetRgn(hBaseRgn, 2, 2);
		CombineRgn(hBaseRgn, hRgn, hBaseRgn, RGN_DIFF);
		hBrush = CreateSolidBrush(ltInner);
		FillRgn(hDC, hBaseRgn, hBrush);
		DeleteObject(hBrush);

		CombineRgn(hBaseRgn, hRgn, 0, RGN_COPY);
		OffsetRgn(hBaseRgn, -2, -2);
		CombineRgn(hBaseRgn, hRgn, hBaseRgn, RGN_DIFF);
		hBrush = CreateSolidBrush(rbInner);
		FillRgn(hDC, hBaseRgn, hBrush);
		DeleteObject(hBrush);

		CombineRgn(hBaseRgn, hRgn, 0, RGN_COPY);
		OffsetRgn(hBaseRgn, 1, 1);
		CombineRgn(hBaseRgn, hRgn, hBaseRgn, RGN_DIFF);
		hBrush = CreateSolidBrush(ltOuter);
		FillRgn(hDC, hBaseRgn, hBrush);
		DeleteObject(hBrush);

		CombineRgn(hBaseRgn, hRgn, 0, RGN_COPY);
		OffsetRgn(hBaseRgn, -1, -1);
		CombineRgn(hBaseRgn, hRgn, hBaseRgn, RGN_DIFF);
		hBrush = CreateSolidBrush(rbOuter);
		FillRgn(hDC, hBaseRgn, hBrush);
		DeleteObject(hBrush);
		break;
	default:
		CombineRgn(hBaseRgn, hRgn, 0, RGN_COPY);
		OffsetRgn(hBaseRgn, 1, 1);
		CombineRgn(hBaseRgn, hRgn, hBaseRgn, RGN_DIFF);
		hBrush = CreateSolidBrush(ltInner);
		FillRgn(hDC, hBaseRgn, hBrush);
		DeleteObject(hBrush);

		CombineRgn(hBaseRgn, hRgn, 0, RGN_COPY);
		OffsetRgn(hBaseRgn, -1, -1);
		CombineRgn(hBaseRgn, hRgn, hBaseRgn, RGN_DIFF);
		hBrush = CreateSolidBrush(rbOuter);
		FillRgn(hDC, hBaseRgn, hBrush);
		DeleteObject(hBrush);
		break;
	}
	
	// clean up regions
	DeleteObject(hBaseRgn);
}

void CUniButtonEx::DrawButton(CDC * pDC, CRect * pRect, UINT state)
{
	// create memory DC
	CDC * pMemDC = new CDC;
	pMemDC -> CreateCompatibleDC(pDC);
	CBitmap * pOldBitmap;

	// get region
	HRGN hRgn = CreateRectRgn(0, 0, 0, 0);
	GetWindowRgn(hRgn);

	// select bitmap to paint depending upon button state
	if (state & ODS_DISABLED)
		pOldBitmap = pMemDC -> SelectObject(m_pDisabled);
	else {
		if (state & ODS_SELECTED)
			pOldBitmap = pMemDC -> SelectObject(m_pSelected);
		else {
			if (m_bHover)
				pOldBitmap = pMemDC -> SelectObject(m_pHover);
			else 
				pOldBitmap = pMemDC -> SelectObject(m_pNormal);
		}	
	}

	// paint using region for clipping
	::SelectClipRgn(pDC -> GetSafeHdc(), hRgn);
	pDC -> BitBlt(0, 0, pRect -> Width(), pRect -> Height(), pMemDC, 0, 0, SRCCOPY);
	::SelectClipRgn(pDC -> GetSafeHdc(), NULL);

	// clean up
	DeleteObject(hRgn);
	pMemDC -> SelectObject(pOldBitmap);
	delete pMemDC;
}

void CUniButtonEx::RgnPixelWork(CDC * pDC, CRgn * pRgn)
{
	int res;
	// get size of data composing region
	int size = pRgn -> GetRegionData(NULL, 0);
	HANDLE hData = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, size);
	RGNDATA * pData = (RGNDATA *)GlobalLock(hData);

	// retrieve region data
	res = pRgn -> GetRegionData(pData, size);
	RECT * pRect = (RECT *) pData -> Buffer;

	// now we know how region is represented and we are able to manipulate it as we like
	for (DWORD i = 0; i < pData -> rdh.nCount; i++) {
		RECT rect = *(pRect + i);
		for (int x = rect.left; x < rect.right; x++)
			for (int y = rect.top; y < rect.bottom; y++) {
				// use SetPixel(x, y, color) to do pixel work
			}
	}

	// free region data
	GlobalUnlock(hData);
	GlobalFree(hData);
}

void CUniButtonEx::ChangeColor(COLORREF nColor, COLORREF sColor, COLORREF hColor, COLORREF dColor)
{
	m_nColor = nColor;
	m_sColor = sColor;
	m_hColor = hColor;
	m_dColor = dColor;
	m_bNeedBitmaps = true;
	RedrawWindow();
}

void CUniButtonEx::MakeCircle()
{
	CRect r;
	GetClientRect(&r);
	
	DeleteObject(m_hRgn);
	m_hRgn = CreateEllipticRgn(r.left, r.top, r.right, r.bottom);
	
	CRect box;
	GetRgnBox(m_hRgn, &box);
	m_CenterPoint = CPoint(box.left + box.Width() /2 , box.top + box.Height() /2);
	
	m_bNeedBitmaps = true;
	SetWindowRgn(m_hRgn, true);
}

void CUniButtonEx::MakeCircle(COLORREF nColor, COLORREF sColor)
{
	m_nColor = nColor;
	m_sColor = sColor;
	m_hColor = sColor;
	MakeCircle();
}

void CUniButtonEx::MakeArrow(int nStyle, BOOL bHead)
{
	CRect r;
	GetClientRect(&r);

	CPoint pt[7];
	int off1, off2, off3 = 0, off4 = 0;

	switch(nStyle)
	{
	case AS_LEFT:
		off1 = r.Width() / 3;
		off2 = (bHead) ? r.Height() / 8 : 0;
		off3 = r.Width() / 12;
		pt[0].x = r.left;				pt[0].y = r.top + r.Height()/2;
		pt[1].x = r.left + off1;		pt[1].y = r.bottom;
		pt[2].x = pt[1].x;				pt[2].y = r.bottom - off2;
		pt[3].x = r.right;				pt[3].y = pt[2].y;
		pt[4].x = r.right;				pt[4].y = r.top + off2;
		pt[5].x = pt[1].x;				pt[5].y = pt[4].y;
		pt[6].x = pt[1].x;				pt[6].y = r.top;
		break;
	case AS_UP:
		off1 = (bHead) ? r.Width() / 8 : 0;
		off2 = r.Height() / 3;
		off4 = r.Height() / 12;
		pt[0].x = r.left;				pt[0].y = r.top + off2;
		pt[1].x = r.left + off1;		pt[1].y = pt[0].y;
		pt[2].x = pt[1].x;				pt[2].y = r.bottom;
		pt[3].x = r.right - off1;		pt[3].y = r.bottom;
		pt[4].x = pt[3].x;				pt[4].y = pt[0].y;
		pt[5].x = r.right;				pt[5].y = pt[0].y;
		pt[6].x = r.left + r.Width()/2;	pt[6].y = r.top;
		break;
	case AS_DOWN:
		off1 = (bHead) ? r.Width() / 8 : 0;
		off2 = r.Height() / 3;
		off4 = -(r.Height() / 12);
		pt[0].x = r.left + off1;		pt[0].y = r.top;
		pt[1].x = pt[0].x;				pt[1].y = r.bottom - off2;
		pt[2].x = r.left;				pt[2].y = pt[1].y;
		pt[3].x = r.left + r.Width()/2;	pt[3].y = r.bottom;
		pt[4].x = r.right;				pt[4].y = pt[1].y;
		pt[5].x = r.right - off1;		pt[5].y = pt[1].y;
		pt[6].x = pt[5].x;				pt[6].y = r.top;
		break;
	case AS_RIGHT:
	default:
		off1 = r.Width() / 3;
		off2 = (bHead) ? r.Height() / 8 : 0;
		off3 = -(r.Width() / 12);
		pt[0].x = r.left;				pt[0].y = r.top + off2;
		pt[1].x = r.left;				pt[1].y = r.bottom - off2;
		pt[2].x = r.right - off1;		pt[2].y = pt[1].y;
		pt[3].x = pt[2].x;				pt[3].y = r.bottom;
		pt[4].x = r.right;				pt[4].y = r.top + r.Height()/2;
		pt[5].x = pt[2].x;				pt[5].y = r.top;
		pt[6].x = pt[2].x;				pt[6].y = r.top + off2;
		break;
	}

	DeleteObject(m_hRgn);
	m_hRgn = CreatePolygonRgn(pt, 7, WINDING);
	
	CRect box;
	GetRgnBox(m_hRgn, &box);
	m_CenterPoint = CPoint(box.left + box.Width() /2 , box.top + box.Height() /2);
	m_CenterPoint.x += off3;
	m_CenterPoint.y += off4;
	
	m_bNeedBitmaps = true;
	SetWindowRgn(m_hRgn, true);
}

void CUniButtonEx::MakeArrow(COLORREF nColor, COLORREF sColor, int nStyle, BOOL bHead)
{
	m_nColor = nColor;
	m_sColor = sColor;
	m_hColor = sColor;
	MakeArrow(nStyle, bHead);
}

/* ****************************************************************************** */
/* 유니 버튼 캡션 변경 함수 [규이리 추가 (2003.09.22) ]                           */
/* ****************************************************************************** */
LRESULT CUniButtonEx::OnSetText(WPARAM, LPARAM newName)
{
	m_bNeedBitmaps = true;
	Invalidate(TRUE);

	return Default();
}
/* ****************************************************************************** */