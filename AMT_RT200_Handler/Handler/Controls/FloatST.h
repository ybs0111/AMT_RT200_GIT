#if !defined(AFX_FLOATST_H__400D680F_89F1_44B8_9B63_24B85264838B__INCLUDED_)
#define AFX_FLOATST_H__400D680F_89F1_44B8_9B63_24B85264838B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FloatST.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFloatST window

class CFloatST : public CStatic
{
// Construction
public:
	enum { STA_LEFT		= 0,
		   STA_CENTER	= 1,
		   STA_RIGHT	= 2,
	};
	CFloatST();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFloatST)
	//}}AFX_VIRTUAL

// Implementation
public:
	CString GetText();
	void SetTextFont(CFont* pFont);
	void SetTextAlign(int n, int nMargin=3);
	void SetColor(COLORREF crBk, COLORREF crTx, int offx, int offy);
	virtual ~CFloatST();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFloatST)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	int m_nMargin;
	int m_nAlign;
	int m_offsetX;
	int m_offsetY;
	COLORREF m_crTextColor;
	COLORREF m_crBkColor;
	CFont* m_pFont;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLOATST_H__400D680F_89F1_44B8_9B63_24B85264838B__INCLUDED_)
