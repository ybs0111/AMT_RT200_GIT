//////////////////////////////////////////////////////////////////
// SuperProgressCtrl.h
// Interface file of the CSuperProgressCtrl class

// Copyright (C) 1998-1999 Mirae Corporation
// All rights reserved.
//
// DIVISION    : MIRAE Coperation
// MODIFIED BY : Lee Kyu Il
// REVISION    :
//     2000. 06. 16 : Modified by Lee Kyu Il
// 

#ifndef __SUPERPROGRESSCTRL_H__
#define __SUPERPROGRESSCTRL_H__

/////////////////////////////////////////////////////////////////////////////
// CSuperProgressCtrl window

#define SP_FILL_VERT		(1)	// Vertical solid fill
#define SP_FILL_HORZ		(2)	// Horizontal solid fill
#define SP_FILL_VERTGRAD	(3)	// Vertical gradient fill
#define SP_FILL_HORZGRAD	(4)	// Horizontal gradient fill

class CSuperProgressCtrl : public CWnd
{
// Construction
public:
	CSuperProgressCtrl();

// Static members
public:
	static void RegisterClass();
protected:
	static CString s_Class; 

// Attributes
public:
	BOOL Create(CWnd* pParent, int x, int y,
				HBITMAP hbmArea, UINT nID);

// Operations
public:
	// Sets the range of the progress control
	void SetRange(int nMin, int nMax);
	// Sets the current position of the progress control
	int SetPos(int nPos);
	// Offsets the current position of the progress control
	int OffsetPos(int nOffset);
	// Sets the step count of the progress control
	int SetStep(int nStep);
	// Steps the progress control
	int StepIt();

	// Sets the fill style
	int SetFillStyle(int nStyle);
	// Gets the current fill style
	int GetFillStyle() const;
	// Sets the current colours
	void SetColours(COLORREF Colour1, COLORREF Colour2);
	// Gets the current colours
	// If Colour1 or Colour2 are NULL, the
	// corresponding colour is not returned
	void GetColours(COLORREF* Colour1, COLORREF* Colour2) const;
	// Sets the background colour
	COLORREF SetBackColour(COLORREF Colour);
	// Gets the current background colour
	COLORREF GetBackColour() const;

	// Performs an MFC-compatible message loop, returns FALSE
	// if a WM_QUIT is received
	BOOL MessageLoop() const;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSuperProgressCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSuperProgressCtrl();

// Data Members
protected:
	int		m_nMin;			// Minimum range
	int		m_nMax;			// Maximum range
	int		m_nStep;		// Step size
	int		m_nPosition;	// The current position
	COLORREF m_Colour1;		// First colour of gradient
	COLORREF m_Colour2;		// Second colour of gradient
	COLORREF m_Background;	// Background colour
	HBITMAP	m_hbmArea;		// The window area bitmap
	HRGN	m_hRegion;		// The window region
	int		m_nFillStyle;	// What style is used for filling?

	// Generated message map functions
protected:
	//{{AFX_MSG(CSuperProgressCtrl)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __SUPERPROGRESSCTRL_H__