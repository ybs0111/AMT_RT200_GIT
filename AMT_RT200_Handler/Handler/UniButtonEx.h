#if !defined(AFX_UNIBUTTONEX_H__5882BE36_F63F_41A1_A3C8_7F719AF4C2F4__INCLUDED_)
#define AFX_UNIBUTTONEX_H__5882BE36_F63F_41A1_A3C8_7F719AF4C2F4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// UniButtonEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUniButtonEx window

class CUniButtonEx : public CButton
{
public:
	enum {	AS_LEFT		= 0,
			AS_RIGHT	= 1,
			AS_UP		= 2,
			AS_DOWN		= 3};

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUniButtonEx)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnSetText(WPARAM, LPARAM newName);  /* 유니 버튼 캡션 변경 함수 */
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CUniButtonEx)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	UINT m_nBorder;														// width of the border in pixels for 3D highlight
	LONG m_lfEscapement;												// orientation of the caption (in tenth of the degree as in LOGFONT)
	COLORREF m_nColor, m_sColor, m_hColor, m_dColor;					// background colors for button states: normal, selected, hover, disabled
	CBitmap * m_pNormal;												// bitmaps to hold button images
	CBitmap * m_pSelected;
	CBitmap * m_pHover;
	CBitmap * m_pDisabled;	
	CPoint m_CenterPoint;												// button caption will be centered around this point
	BOOL m_bMouseDown;													// indicated that mouse is pressed down
	BOOL m_bHover;														// indicates if mouse is over the button
	BOOL m_bCapture;													// indicates that mouse is captured in the buton
	HRGN m_hRgn;														// region in screen coordinates
	BOOL m_bNeedBitmaps;												// flag idicates that state bitmaps must be rebuild
	void DrawButton(CDC * pDC, CRect * pRect, UINT state);				// draws button to the screen
	void PrepareStateBitmaps(CDC * pDC, CRect * pRect);					// prepares bitmaps for button states
	BOOL HitTest(CPoint point);											// determines if point is inside the button region
	void RgnPixelWork(CDC * pDC, CRgn * pRgn);							// region pixel work - unused
	void FrameRgn3D(HDC hDC, const HRGN hRgn, BOOL bSunken);			// frames region to show 3D shadows
	void CheckHover(CPoint point);
protected:
	void PrepareNormalState(CDC * pDC, CDC * pMemDC, CRect * pRect);	// prepare normal state button bitmap
	void PrepareSelectedState(CDC * pDC, CDC * pMemDC, CRect * pRect);	// prepare selectedstate button bitmap
	void PrepareHoverState(CDC * pDC, CDC * pMemDC, CRect * pRect);		// prepare hover state button bitmap
	void PrepareDisabledState(CDC * pDC, CDC * pMemDC, CRect * pRect);	// prepare disabled state button bitmap
	void DrawButtonCaption(HDC hDC, CRect * pRect, BOOL bEnabled, BOOL bSunken);	// draws button caption 
	void PaintRgn(CDC * pDC, CDC * pMemDC, CBitmap * pBitmap, COLORREF color, CRect * pRect, BOOL bEnabled, BOOL bSunken);	// paint button 
public:
	void MakeArrow(COLORREF nColor, COLORREF sColor, int nStyle = AS_RIGHT, BOOL bHead = TRUE);
	void MakeArrow(int nStyle = AS_RIGHT, BOOL bHead = TRUE);
	void MakeCircle(COLORREF nColor, COLORREF sColor);
	void MakeCircle();
	void ChangeColor(COLORREF nColor, COLORREF sColor, COLORREF hColor, COLORREF dColor);
	CUniButtonEx();				// constructor
	virtual ~CUniButtonEx();	// destructor
	BOOL Create(LPCTSTR lpszCaption, DWORD dwStyle, const CPoint point, const HRGN hRgn, CWnd* pParentWnd, UINT nID);
							// constructor with default colors and border
	BOOL Create(LPCTSTR lpszCaption, DWORD dwStyle, const CPoint point, const HRGN hRgn, CWnd* pParentWnd, UINT nID, COLORREF color);	
							// constructor with hover & selected color specified
	BOOL Create(LPCTSTR lpszCaption, DWORD dwStyle, const CPoint point, const HRGN hRgn, CWnd* pParentWnd, UINT nID, UINT nBorder, COLORREF nColor, COLORREF sColor, COLORREF hColor, COLORREF dColor);	
							// complex parameters constructor
								// lpszCaption - window caption
								// dwStyle - window styles
								// point - position of the button on the parent window (in parent coordinates)
								// hRgn - handle to region which represents the button (in button client coordinates)
								// pParentWnd - handle to parent window
								// nID - control Id
								// nBorder - specifies width of the border in pixels for 3D highlight (allowed values are 1, 2)
								// nColor - normal color 
								// sColor - selected color
								// hColor - hover color
								// dColor - disabled color
	BOOL Create(LPCTSTR lpszCaption, DWORD dwStyle, const CPoint point, const HRGN hRgn, CWnd* pParentWnd, UINT nID, UINT nBorder, LONG lfEscapement, COLORREF nColor, COLORREF sColor, COLORREF hColor, COLORREF dColor);
								// variation of above
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNIBUTTONEX_H__5882BE36_F63F_41A1_A3C8_7F719AF4C2F4__INCLUDED_)
