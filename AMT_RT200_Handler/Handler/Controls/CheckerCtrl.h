#if !defined(AFX_CHECKERCTRL_H__AD4BC4E3_7750_11D4_8718_008048CA4A77__INCLUDED_)
#define AFX_CHECKERCTRL_H__AD4BC4E3_7750_11D4_8718_008048CA4A77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CheckerCtrl.h : header file
//

#include "afxtempl.h"  /* ����Ʈ ���ø� ���� �߰� */

/////////////////////////////////////////////////////////////////////////////
// CCheckerCtrl window

#define WM_CHECKERCTRL_RBUTTONDOWN	WM_USER + 110  /* ����� ���� �޽��� */

class CCheckerCtrl : public CWnd
{
// Construction
public:
	CCheckerCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckerCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetValueBox(int n_Vbox);
	void SetTotalBlocks(const UINT nBlocksPerRow, const UINT nBlocksPerColumn, const UINT nStartIndex = 0);  /* Ŭ���̾�Ʈ ������ �׸� ��� ���� ���� �Է¹޴� �Լ� */
	void Update(const UINT nBlockNumber);  /* ��µ� ���鿡 ���� Į�� ���� �Լ� */
	void Reset();    /* ���鿡 �Ҵ�� Į�� ���� �Լ� */
	void Refresh();  /* Ŭ���̾�Ʈ ���� �����ϴ� �Լ� */
	void SetBlock(const UINT nBlockNumber, const COLORREF crColor);  /* Ư�� ��Ͽ� ���� Į�� ���� ���� �Լ� */
	COLORREF GetBlock(const UINT nBlockNumber) const;                /* ���� ���� Į�� ���� �Լ� */
	BOOL Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID);
	virtual ~CCheckerCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCheckerCtrl)
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL IsVisible(const UINT nBlockNumber);  /* �ش� �� ��� ���� �˻� �Լ� */
	CRect GetRect(const UINT nRow, const UINT nColumn);            /* ���� ���� �簢�� ���� ���� ���� �Լ� */
	void CreateSafeTooltipRect(int x, int y, LPCTSTR lpszText);    /* ���� ����� ���� ��ġ ���� �Լ� */
	void SetBlock(int nRow, int nColumn, CBrush &brush, CDC &dc);  /* �־��� ��� �귯�� ����Ͽ� ��� �׸��� �Լ� */

	CWnd	*m_pParentWnd;  /* ��Ʈ�� �ڵ� ���� ���� */
	UINT	m_nID;          /* ��Ʈ�� ID ���� ���� */
	UINT	m_nStartIndex;  /* ���� ��� �ε��� ���� ���� */
	BOOL	m_bShouldUpdated;  /* UPDATE ��û �÷��� */

	CRect	tooltipRect;    /* ���� ��� �簢�� ���� ���� ���� */

	UINT	m_nTotalVisibleBlocks;  /* ����� �� ��� �� ���� ���� */
	UINT	m_nBlockStartPos;       /* ��� ���� ��ġ ���� ���� */

	int		m_nyPos;             /* ��ũ�� Y ��ġ ���� ���� */
	int		m_nOffset;           /* ��ũ�� OFFSET ���� ���� */

	CRect	m_rcClient;          /* Ŭ���̾�Ʈ ���� ���� ���� */

	UINT	m_nBlocksPerRow;     /* ���� ��� �� ���� ���� */
	UINT	m_nBlocksPerColumn;  /* ���� ��� �� ���� ���� */

	int	m_nBlocksPerXSize;       /* ���� ��� ���� ���� ���� */
	int	m_nBlocksPerYSize;       /* ���� ��� ���� ���� ���� */
	int m_nVbox;				//Value Box ��� ����.

	UINT	m_nNumberofBlocks;  /* ������ �� ��� �� ���� ���� */
	CBrush	m_backgroundBrush;  /* �귯�� ���� ���� ���� */
	CArray	<COLORREF, COLORREF> m_crColor;  /* Į�� ���� �迭 */
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECKERCTRL_H__AD4BC4E3_7750_11D4_8718_008048CA4A77__INCLUDED_)
