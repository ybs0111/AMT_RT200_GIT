#if !defined(AFX_GRADIENTSTATIC_H__0709E3A1_C8B6_11D6_B74E_004033A0FB96__INCLUDED_)
#define AFX_GRADIENTSTATIC_H__0709E3A1_C8B6_11D6_B74E_004033A0FB96__INCLUDED_

#pragma warning( disable : 4100 )

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GradientStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGradientStatic window
typedef UINT (CALLBACK* LPFNDLLFUNC1)(HDC,CONST PTRIVERTEX,DWORD,CONST PVOID,DWORD,DWORD);

class CGradientStatic : public CStatic
{
// Construction
public:
	CGradientStatic();
	virtual ~CGradientStatic();
	void SetColor(long cl) {clLeft=cl;};
	void SetGradientColor(long cl) {clRight=cl;};
	void SetTextColor(long cl) {clText=cl;};
	CString GetWindowText();
	void SetWindowText(CString lpszString);
	void SetLeftSpacing(int iNoOfPixels) { m_iLeftSpacing = iNoOfPixels; };
	void SetCenterText(BOOL bCenter = TRUE) { m_bCenter = bCenter; };
	void SetFont(CFont* pF, BOOL bRedraw = TRUE) { pFont = pF; };
	
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGradientStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	

	// Generated message map functions
protected:
	CString m_sTEXT;
	int m_iLeftSpacing;
	CFont* pFont;
	long clLeft;
	long clRight;
	long clText;
	BOOL m_bCenter;
	HINSTANCE hinst_msimg32;
	BOOL m_bCanDoGradientFill;
	LPFNDLLFUNC1 dllfunc_GradientFill;
	void DrawGradRect(CDC *pDC, CRect r, COLORREF cLeft, COLORREF cRight);
	
	//{{AFX_MSG(CGradientStatic)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRADIENTSTATIC_H__0709E3A1_C8B6_11D6_B74E_004033A0FB96__INCLUDED_)
