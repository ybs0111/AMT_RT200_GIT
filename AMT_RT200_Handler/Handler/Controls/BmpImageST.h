#if !defined(AFX_BMPIMAGEST_H__63CCF3CD_FCF9_499F_B181_C137F8323BF3__INCLUDED_)
#define AFX_BMPIMAGEST_H__63CCF3CD_FCF9_499F_B181_C137F8323BF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BmpImageST.h : header file
//

#include "BmpImage.h"

/////////////////////////////////////////////////////////////////////////////
// CBmpImageST window

class CBmpImageST : public CStatic
{
// Construction
public:
	CBmpImageST();

// Attributes
public:
	enum { JS_NONE				= 0,
		   JS_CIRCLE			= 1, 
		   JS_ARROW_LEFT		= 2,
		   JS_ARROW_RIGHT		= 3,
		   JS_ARROW_UP			= 4,
		   JS_ARROW_DOWN		= 5,
		   JS_ARROW_LEFTUP		= 6,
		   JS_ARROW_LEFTDOWN	= 7,
		   JS_ARROW_RIGHTUP		= 8,
		   JS_ARROW_RIGHTDOWN	= 9
	};

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBmpImageST)
	//}}AFX_VIRTUAL

// Implementation
public:
	void Delete();
	void ClearDrawing();
	void SetArrowStyle(int cx, int cy, int style, CString arrowpath, int offset=0);
	void SetLineStyle(int width, COLORREF color);
	void SetCircleStyle(int cx, int cy, int cradius);
	BOOL LoadImage(CString fname, BOOL bCenterImg=TRUE);
	virtual ~CBmpImageST();

	// Generated message map functions
protected:
	void Paint(CDC* pDC);
	BOOL LoadArrow(int style);
	int m_nStyle;
	CString m_strArrowPath;
	CBmpImage* m_pArrow;
	CBmpImage* m_pArrowBack;
	CBmpImage* m_pBackImage;
	//{{AFX_MSG(CBmpImageST)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	int m_nCx;
	int m_nCy;
	int m_nMinRadius;
	int m_nRDec;
	int m_nRadius;
	int m_nTCnt;
	int m_nOrgRadius;
	int m_nOffsetX;
	int m_nOffsetY;
	int m_nWidth;
	int m_nHeight;
	CRect m_Rect;
	COLORREF m_LineColor;
	COLORREF m_crBk;
	int m_nLineWidth;
	int m_nArrowIdx;
	int m_nArrowWidth;
	int m_nArrowHeight;
	BOOL m_bLargeX;
	BOOL m_bLargeY;
	BOOL m_bCenterImg;
	BOOL m_bFirst;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BMPIMAGEST_H__63CCF3CD_FCF9_499F_B181_C137F8323BF3__INCLUDED_)
