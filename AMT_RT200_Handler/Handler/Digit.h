#if !defined(AFX_DIGIT_H__A067B162_B041_11D4_B910_005004FDF587__INCLUDED_)
#define AFX_DIGIT_H__A067B162_B041_11D4_B910_005004FDF587__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Digit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDigit window

class CDigit : public CStatic
{
// Construction
public:
	enum { DS_INT		= 0,
		   DS_FLOAT		= 1,		     
		   DS_CLOCK		= 2,
		   DS_TIMER		= 3,
		   DS_CHAR		= 4,
		   DS_FLOAT2	= 5,
		   DS_FLOAT3	= 6,
	};
	enum { DC_BLACK		= 0,
		   DC_CYAN		= 1,
		   DC_GREEN		= 2,
		   DC_YELLOW	= 3,
		   DC_RED		= 4,
		   DC_DISABLE	= 5,
		   		   
		   DC_BBLACK	= 6,
		   DC_BCYAN		= 7,
		   DC_BGREEN	= 8,
		   DC_BYELLOW	= 9,
		   DC_BRED		= 10,
		   DC_BBLUE		= 11,
		   DC_BDISABLE	= 12,
		   DC_BBDISABLE	= 13,
		   DC_SBDISABLE = 14,
	};

	CDigit();

// Attributes
public:
	BOOL m_bMinus;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDigit)
	//}}AFX_VIRTUAL

// Implementation
public:
	void ClearErr();
	void SetError(int nErrNum);
	void SetChar(CString str) { m_strChar = str; Invalidate(); }
	void UpdateTimer();
	void ActivateTimer(BOOL bAct=TRUE);
	void EnableResize();
	void GetTime(int *h, int *m, int* s) { *h=m_nT[0]; *m=m_nT[1]; *s=m_nT[2]; }
	float GetFloatVal() { return m_fVal; }
	int GetVal() {return m_nVal; }
	void SetTime(int h, int m, int s) { m_nT[0]=h; m_nT[1]=m; m_nT[2]=s; Invalidate(); }
	void SetFloatVal(float f);
	void SetVal(int n);
	void SetStyle(int nStyle, int nPrec, int nActive=DC_BLACK, int nInactive=DC_RED);
	virtual ~CDigit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDigit)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	void MakeErrString();
	void ResizeCtrl(CRect r);
	UINT m_nClockTimer;
	BOOL LoadBitmap(CBitmap* bm, int nType);
	BOOL m_bActive;
	CString m_strData;
	int m_nOffsetX;
	int m_nOffsetY;
	int m_nWidth;
	int m_nHeight;
	int m_nPrec;
	CBitmap m_bmActive;
	CBitmap m_bmInactive;
	BOOL m_bResize;
	BOOL m_bFirst;
	void MakeString();
	int m_nT[3];
	int m_nOldT[3];
	CString m_strChar;
	float m_fVal;
	int m_nVal;
	int m_nStyle;
	int m_nErrNum;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIGIT_H__A067B162_B041_11D4_B910_005004FDF587__INCLUDED_)
