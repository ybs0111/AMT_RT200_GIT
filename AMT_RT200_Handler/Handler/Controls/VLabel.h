// ==========================================================================
// CVLabel.h
//
// Author : ViboX, Kim
//          akimara@nate.com
//
// Date of creation  : 2006/07/12
// Last modification : 2006/07/13
// ==========================================================================

#if !defined(AFX_VLABEL_H__305CE701_0B88_4E1D_9082_105D6F4EE946__INCLUDED_)
#define AFX_VLABEL_H__305CE701_0B88_4E1D_9082_105D6F4EE946__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VLabel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVLabel window

class CVLabel : public CStatic
{
// Construction
public:
	enum
	{
		HORZ_LEFT = 0,
		HORZ_CENTER,
		HORZ_RIGHT,
		VERT_TOP,
		VERT_CENTER,
		VERT_BOTTOM
	};

protected:
	UINT	m_uiAngle;
	CString	m_strText;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVLabel)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetText(LPCWSTR szText);
	void SetTextAlign(int n);
	void SetColor(COLORREF crBk, COLORREF crTx);
	void SetTextFont(CFont* pFont);
	CVLabel();
	virtual ~CVLabel();

	// Generated message map functions
protected:
	//{{AFX_MSG(CVLabel)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	int m_nMargin;
	int m_nAlign;
	COLORREF m_crTextColor;
	COLORREF m_crBkColor;
	CFont* m_pFont;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VLABEL_H__305CE701_0B88_4E1D_9082_105D6F4EE946__INCLUDED_)
