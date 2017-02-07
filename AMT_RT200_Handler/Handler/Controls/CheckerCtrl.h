#if !defined(AFX_CHECKERCTRL_H__AD4BC4E3_7750_11D4_8718_008048CA4A77__INCLUDED_)
#define AFX_CHECKERCTRL_H__AD4BC4E3_7750_11D4_8718_008048CA4A77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CheckerCtrl.h : header file
//

#include "afxtempl.h"  /* 리소트 템플릿 파일 추가 */

/////////////////////////////////////////////////////////////////////////////
// CCheckerCtrl window

#define WM_CHECKERCTRL_RBUTTONDOWN	WM_USER + 110  /* 사용자 정의 메시지 */

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
	void SetTotalBlocks(const UINT nBlocksPerRow, const UINT nBlocksPerColumn, const UINT nStartIndex = 0);  /* 클라이언트 영역에 그릴 블록 갯수 정보 입력받는 함수 */
	void Update(const UINT nBlockNumber);  /* 출력된 블럭들에 대한 칼라 갱신 함수 */
	void Reset();    /* 블럭들에 할당된 칼라 리셋 함수 */
	void Refresh();  /* 클라이언트 영역 갱신하는 함수 */
	void SetBlock(const UINT nBlockNumber, const COLORREF crColor);  /* 특정 블록에 대한 칼라 정보 설정 함수 */
	COLORREF GetBlock(const UINT nBlockNumber) const;                /* 블럭에 대한 칼라 리턴 함수 */
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
	BOOL IsVisible(const UINT nBlockNumber);  /* 해당 블럭 출력 유무 검사 함수 */
	CRect GetRect(const UINT nRow, const UINT nColumn);            /* 블럭에 대한 사각형 영역 정보 리턴 함수 */
	void CreateSafeTooltipRect(int x, int y, LPCTSTR lpszText);    /* 툴팁 출력할 영역 위치 설정 함수 */
	void SetBlock(int nRow, int nColumn, CBrush &brush, CDC &dc);  /* 주어진 펜과 브러시 사용하여 블록 그리는 함수 */

	CWnd	*m_pParentWnd;  /* 컨트롤 핸들 저장 변수 */
	UINT	m_nID;          /* 컨트롤 ID 저장 변수 */
	UINT	m_nStartIndex;  /* 시작 블록 인덱스 저장 변수 */
	BOOL	m_bShouldUpdated;  /* UPDATE 요청 플래그 */

	CRect	tooltipRect;    /* 툴팁 출력 사각형 영역 저장 변수 */

	UINT	m_nTotalVisibleBlocks;  /* 출력할 총 블록 수 저장 변수 */
	UINT	m_nBlockStartPos;       /* 블록 시작 위치 저장 변수 */

	int		m_nyPos;             /* 스크롤 Y 위치 저장 변수 */
	int		m_nOffset;           /* 스크롤 OFFSET 저장 변수 */

	CRect	m_rcClient;          /* 클라이언트 영역 저장 변수 */

	UINT	m_nBlocksPerRow;     /* 가로 블록 수 저장 변수 */
	UINT	m_nBlocksPerColumn;  /* 세로 블록 수 저장 변수 */

	int	m_nBlocksPerXSize;       /* 가로 블록 길이 저장 변수 */
	int	m_nBlocksPerYSize;       /* 세로 블록 길이 저장 변수 */
	int m_nVbox;				//Value Box 출력 여부.

	UINT	m_nNumberofBlocks;  /* 생성할 총 블록 수 저장 변수 */
	CBrush	m_backgroundBrush;  /* 브러시 정보 저장 변수 */
	CArray	<COLORREF, COLORREF> m_crColor;  /* 칼라 저장 배열 */
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECKERCTRL_H__AD4BC4E3_7750_11D4_8718_008048CA4A77__INCLUDED_)
