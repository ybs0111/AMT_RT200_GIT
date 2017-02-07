///////////////////////////////////////////////////////////////////////////////
/* ****************************************************************************** */
// FILE NAME : CheckerCtrl.cpp
//
// Copyright (C) 2003 AMT Corporation
// All rights reserved.
//
// CREATED BY  : Lee Kyu Il
// REVISION    :
//     1> 2003. 09. 14 : Created by Lee Kyu Il
// DESCRIPTION :
//     Implementation of the CheckerCtrl and associated classes
//	   This code may be used in compiled form in any way you desire. This
//	   file may be redistributed unmodified by any means PROVIDING it is
//	   not sold for profit without the authors written consent, and
//	   providing that this notice and the authors name and all copyright
//	   notices remains intact.
//
//	   An email letting me know how you are using
//it would be nice as well.
//
//
// REFERENCE   :
// 
/* ****************************************************************************** */
///////////////////////////////////////////////////////////////////////////////
//test

#include "stdafx.h"
#include "CheckerCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckerCtrl

#define BLANKED_BLOCKS_COLOR	RGB(252, 252, 252)  /* 기본 칼라 정의 */

CCheckerCtrl::CCheckerCtrl()
{
	/* ************************************************************************** */
	/* 클라이언트 영역에 사용할 백그라운트 브러쉬를 생성한다                      */
	/* -> CreateSolidBrush() : 주어진 색깔 사용하여 논리적인 펜을 생성하는 함수   */
	/* -> GetSysColor() : 디스플레이 구성 요소에 대한 칼라 (R/G/B) 값 리턴 함수   */
	/* ************************************************************************** */
//	m_backgroundBrush.CreateSolidBrush(GetSysColor(COLOR_WINDOW));
	m_backgroundBrush.CreateSolidBrush(RGB(0, 0, 0));
	/* ************************************************************************** */
	
	/* ************************************************************************** */
	/* 클래스 변수 초기화 작업 진행한다                                           */
	/* -> 클라이언트 영역에 생성할 블록 관련 변수들에 대한 초기화 작업이다        */
	/* ************************************************************************** */
	m_nyPos = 0;				/* 스크롤 위치 초기화 */
	m_nOffset = 0;				/* 스크롤 OFFSET 초기화 */

	m_nBlockStartPos = 0;		/* 블록 시작 위치 초기화 */
	m_nStartIndex = 0;			/* 시작 블록 인덱스 초기화 */

	m_nNumberofBlocks = 0;      /* 생성할 총 블록 수 초기화 */
	m_nTotalVisibleBlocks = 0;  /* 출력할 총 블록 수 초기화 */

	m_nBlocksPerRow = 0;		/* 가로 블록 수 초기화 */
	m_nBlocksPerColumn = 0;		/* 세로 블록 수 초기화 */
	/* ************************************************************************** */

	m_nVbox = 0;				// 출력 여부 초기화.
}

CCheckerCtrl::~CCheckerCtrl()
{
}

BOOL CCheckerCtrl::Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID)
{
	/* ************************************************************************** */
	/* 윈도우 레지스터에 등록 후 생성한다                                         */
	/* -> AfxRegisterWndClass() : 클래스 이름을 레지스터에 등록하는 함수          */
	/*  : 이 함수는 Create() 멤버 함수에서 사용한다                               */
	/* -> CreateEx() : 윈도우 생성 함수                                           */
	/* ************************************************************************** */
	static CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW);
	BOOL bRet = CWnd::CreateEx(WS_EX_CLIENTEDGE, 
								className,
								NULL,
								dwStyle,
								rect.left,
								rect.top,
								rect.right - rect.left,
								rect.bottom - rect.top,
								pParentWnd->GetSafeHwnd(),
								(HMENU) nID);
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 마우스 우측 버튼 DOWN 시 해당 메시지 전송하기 위한 변수 초기화 작업        */
	/* ************************************************************************** */
	m_nID = nID;                /* 컨트롤 ID 설정 */
	m_pParentWnd = pParentWnd;  /* 컨트롤 핸들 설정 */
	/* ************************************************************************** */
	
	/* ************************************************************************** */
	/* SetCursor() : 커서의 모양을 변경하는 함수                                  */
	/* -> 한 번 사용된 후에는 빠르게 수행된다                                     */
	/* -> 프로그램 속도에는 거의 영향을 받지 않고 커서 모양을 변경할 수 있다      */
	/* -> 커서 모양은 반드시 LoadCursor() 함수로 처음 읽어 들여야 한다            */
	/*  : 클래스 정의에 커서를 가진 윈도우에서 이 함수를 사용하려면 커서가 깜박임 */
	/*  : WM_MOUSEMOVE 메시지가 보내질 때마다 클래스 커서와 SetCursor() 함수로    */
	/*    로드된 커서 사이를 윈도우가 전환하기 때문이다                           */
	/*  : 이 문제를 피하려면 클래스 커서를 NULL로 설정하면 된다                   */
	/* -> LoadCursor() : 주어진 인스턴스 핸들을 가지는 실행 파일이나 DLL로부터    */
	/*    커서 자원을 로드한다                                                    */
	/*  : 커서가 윈도우 클래스 정의 부분에서 로드된다면 마우스가 윈도우           */
	/*    클라이언트 영역 내에 있을 때 읽어들인 마우스 형태로 변경된다            */
	/*  : 같은 윈도우 클라이언트 영역 내에서도 어떤 때에 다른 커서 모양을 갖게    */
	/*    하려면 윈도우 클래스 커서 모양은 NULL로 설정하고 WM_SETCURSOR 메시지가  */
	/*    받아 들여질 때 SetCursor() 함수를 사용하면 가능하다                     */
	/* ************************************************************************** */
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	/* ************************************************************************** */
	
	return bRet;  /* 함수 리턴 플래그 설정 */
}


BEGIN_MESSAGE_MAP(CCheckerCtrl, CWnd)
	//{{AFX_MSG_MAP(CCheckerCtrl)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCheckerCtrl message handlers

void CCheckerCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	/* ************************************************************************** */
	/* FillRect() : 지정된 사각형을 주어진 브러시로 채우는 함수                   */
	/* -> 클라이언트 영역의 백그라운드 칼라로 채운다                              */
	/* ************************************************************************** */
	dc.FillRect(m_rcClient, &m_backgroundBrush);
	/* ************************************************************************** */

	UINT nColumn = 0, nRow = 0;  /* 블록에 대한 가로/세로 정보 저장 변수 초기화 */
	
	/* ************************************************************************** */
	/* 클라이언트 영역에 존재하는 마지막 출력 블록 인덱스 설정한다                */
	/* ************************************************************************** */
	UINT nBlockEndPos = m_nBlockStartPos + m_nTotalVisibleBlocks + m_nBlocksPerRow;
	if (nBlockEndPos > m_nNumberofBlocks)
		nBlockEndPos = m_nNumberofBlocks;
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 클라이언트 영역에 생성한 브러시 이용하여 블록 그린다                       */
	/* ************************************************************************** */
	for(UINT i=m_nBlockStartPos; i<nBlockEndPos; i++)
	{
		CBrush brush(m_crColor.GetAt(i));
		SetBlock(nRow, nColumn, brush, dc);  /* 주어진 펜과 브러시 사용하여 블록 그리는 함수 */
		if ((i + 1 - m_nBlockStartPos) % m_nBlocksPerRow == 0)
		{
			nRow++;
			nColumn = 0;
		}
		else
			nColumn++;
	}
	/* ************************************************************************** */
}

/* ****************************************************************************** */
/* 클라이언트 영역에 그릴 블록 갯수 정보 입력받는 함수                            */
/* ****************************************************************************** */
void CCheckerCtrl::SetTotalBlocks(const UINT nBlocksPerRow, const UINT nBlocksPerColumn, const UINT nStartIndex)
{
	m_nNumberofBlocks = nBlocksPerRow * nBlocksPerColumn;  /* 생성할 총 블록 갯수 설정 */

	/* ************************************************************************** */
	/* 생성할 블록에 적용할 COLORREF 배열을 생성한다                              */
	/* ************************************************************************** */
	m_crColor.SetSize(m_nNumberofBlocks);
	for(UINT i=0; i<m_nNumberofBlocks; i++)
		m_crColor.SetAt(i, BLANKED_BLOCKS_COLOR);
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* GetClientRect() : 주어진 윈도우의 클라이언트 영역 사각형을 얻는 함수       */
	/* -> 좌표는 윈도우의 클라이언트 영역으로부터 계산되는 상대 좌표이다          */
	/*  : 따라서, 좌측 상단의 좌표는 (0,0)이다                                    */
	/* -> GetClientRect() 특정 윈도우의 클라이언트 영역의 크기 알고 싶을 때       */
	/*    유용하게 사용할 수 있다                                                 */
	/* ************************************************************************** */
	GetClientRect(m_rcClient);
	/* ************************************************************************** */

	m_nBlocksPerRow = nBlocksPerRow;        /* 가로 생성 블록 수 설정 */
	m_nBlocksPerColumn = nBlocksPerColumn;  /* 세로 생성 블록 수 설정 */

	m_nBlocksPerXSize = (m_rcClient.Width()-3) / m_nBlocksPerRow;      /* 블록 가로 크기 설정 */
	m_nBlocksPerYSize = (m_rcClient.Height()-1) / m_nBlocksPerColumn;  /* 블록 세로 크기 설정 */

	m_nTotalVisibleBlocks = m_nBlocksPerRow * m_nBlocksPerColumn;  /* 총 생성할 블록 수 설정 */

	/* ************************************************************************** */
	/* 클라이언트 영역에 대한 스크롤바 범위 계산한다                              */
	/* -> SetScrollRange() : 주어진 스크롤바에서 최저와 최고 스크롤 박스의 현재   */
	/*    위치 설정하는 함수                                                      */
	/*  : 이 함수는 일반적인 스크롤 바를 숨기거나 보여주는데 사용되기도 한다      */
	/*  : SetScrollRange() 함수는 스크롤 바를 만들고 상측과 하측의 스크롤 바의    */
	/*    범위를 정하는데 사용하면 된다                                           */
	/*  : 파라메터 모두 0으로 설정하면 스크롤 박스가 출력되지 않는다              */
	/* ************************************************************************** */
	int nOffset = (m_nNumberofBlocks / m_nBlocksPerRow);
	if (m_nNumberofBlocks % m_nBlocksPerRow != 0)
		nOffset++;
	
	m_nOffset = nOffset - m_nBlocksPerColumn;
	if (m_nOffset > 0)
		SetScrollRange(SB_VERT, 0, m_nOffset);
	/* ************************************************************************** */

	m_nStartIndex = nStartIndex;  /* 블록 시작 위치 설정 */
}
/* ****************************************************************************** */

/* ****************************************************************************** */
/* 주어진 펜과 브러시 사용하여 블록 그리는 함수                                   */
/* ****************************************************************************** */
void CCheckerCtrl::SetBlock(int nRow, int nColumn, CBrush &brush, CDC &dc)
{
	CRect rect = GetRect(nRow, nColumn);  /* 클라이언트 영역 정보 설정 */

	/* ************************************************************************** */
	/* Rectangle() : 주어진 좌표에 의해 정의되는 직사각형을 그리는 함수           */
	/* -> 직사각형은 현재 펜으로 그려지고 현재 브러시로 채워진다                  */
	/* ************************************************************************** */
	dc.Rectangle(&rect);
	/* ************************************************************************** */
	
	rect.left++;
	rect.top++;
	rect.bottom--;
	rect.right--;
	
	/* ************************************************************************** */
	/* FillRect() : 지정된 사각형을 주어진 브러시로 채우는 함수                   */
	/* ************************************************************************** */
	dc.FillRect(rect, &brush);
	/* ************************************************************************** */
}
/* ****************************************************************************** */

/* ****************************************************************************** */
/* 특정 블록에 대한 칼라 정보 설정 함수                                           */
/* ****************************************************************************** */
void CCheckerCtrl::SetBlock(const UINT nBlockNumber, const COLORREF crColor)
{
	ASSERT(nBlockNumber - m_nStartIndex < m_nNumberofBlocks);
	m_crColor.SetAt(nBlockNumber - m_nStartIndex, crColor);
}
/* ****************************************************************************** */

void CCheckerCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default

	int nyOffset;  /* 스크롤 OFFSET 저장 변수 */

	switch(nSBCode)
	{
	case SB_PAGEDOWN :  /* PAGE DOWN 요청 */
		/* ********************************************************************** */
		/* 클라이언트 영역에 스크롤 박스 다시 그리고 스크롤한다                   */
		/* -> SetScrollPos() : 주어진 스크롤 바 내부에 있는 스크롤 박스 위치 정의 */
		/*  : 요청되면 새로운 위치로 스크롤 바를 다시 그린다                      */
		/*  : SetScrollPos()는 처음 스크롤 바가 만들어지거나 출력될 때            */
		/*    스크롤 박스의 위치를 정의하기 위해 주로 사용한다                    */
		/*  : 키보드 인터페이스 기능을 만드는 데도 사용된다                       */
		/* -> ScrollWindow() : 주어진 윈도우의 작업 영역의 내용물을 스크롤 한다   */
		/* ********************************************************************** */
		if (m_nyPos<m_nOffset)
		{
			if (m_nyPos+2 < m_nOffset)
				nyOffset = 2;
			else
				nyOffset = m_nOffset - m_nyPos;

			m_nyPos += nyOffset;
			m_nBlockStartPos += m_nBlocksPerRow * nyOffset;

			SetScrollPos(SB_VERT, m_nyPos);
			ScrollWindow(0, -nyOffset * m_nBlocksPerYSize);
		}
		/* ********************************************************************** */
		break;
	case SB_PAGEUP :  /* PAGE UP 요청 */
		if (m_nyPos>0)
		{
			if (m_nyPos-2 > 0)
				nyOffset = 2;
			else
				nyOffset = m_nyPos;

			m_nBlockStartPos -= m_nBlocksPerRow * nyOffset;

			m_nyPos -= nyOffset;
			SetScrollPos(SB_VERT, m_nyPos);
			ScrollWindow(0, nyOffset * m_nBlocksPerYSize);
		}
		break;
	case SB_LINEUP :  /* LINE UP 요청 */
		if (m_nyPos>0)
		{
			m_nBlockStartPos -= m_nBlocksPerRow;

			m_nyPos--;
			SetScrollPos(SB_VERT, m_nyPos);
			ScrollWindow(0, m_nBlocksPerYSize);
		}
		break;
	case SB_LINEDOWN :  /* LINE DOWN 요청 */
		if (m_nyPos<m_nOffset)
		{
			m_nBlockStartPos += m_nBlocksPerRow;

			m_nyPos++;
			SetScrollPos(SB_VERT, m_nyPos);
			ScrollWindow(0, -m_nBlocksPerYSize);
		}
		break;
	}

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

/* ****************************************************************************** */
/* 클라이언트 영역 갱신하는 함수                                                  */
/* ****************************************************************************** */
void CCheckerCtrl::Refresh()
{
	Invalidate();  /* 윈도우 영역 갱신 함수 */
}
/* ****************************************************************************** */

/* ****************************************************************************** */
/* 블럭에 대한 칼라 리턴 함수                                                     */
/* ****************************************************************************** */
COLORREF CCheckerCtrl::GetBlock(const UINT nBlockNumber) const
{
	/* ************************************************************************** */
	/* -> nBlockNumber : 정의된 블록 내에 존재하는 범위 값이어야 한다             */
	/* ************************************************************************** */
	ASSERT(nBlockNumber >= m_nStartIndex && nBlockNumber - m_nStartIndex <= m_nNumberofBlocks);
	return m_crColor.GetAt(nBlockNumber - m_nStartIndex);
	/* ************************************************************************** */
}
/* ****************************************************************************** */

void CCheckerCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_nVbox == 0)	return;
	/* ************************************************************************** */
	/* SetFocus() : 주어진 윈도우에 입력 초점을 전달하는 함수                     */
	/* -> 모든 키보드 입력이 초점을 가진 윈도우로 전달된다                        */
	/* -> 한번에 하나의 윈도우만이 입력 초점을 가질 수 있다                       */
	/* ************************************************************************** */
	SetFocus();
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* GetMessagePos() : 마지막 메시지가 전달되었을 때 화면 좌표의 커서 위치 반환 */
	/* -> 응용 프로그램이 후면에서 메시지 처리 수행할 때 마우스 커서는 메시지가   */
	/*    처리되기 전에 움직일 수 있다                                            */
	/* -> 이 함수는 메시지가 전달되었을 때 마우스 커서의 위치 반환한다            */
	/* ScreenToClient() : 화면 좌표를 클라이언트 좌표로 변환하는 함수             */
	/* -> 마우스 커서의 위치를 윈도우 클라이언트 영역의 X/Y로 변환하기 위해       */
	/*    자주 GetCursorPos() 함수와 결합하여 사용된다                            */
	/*    처리되기 전에 움직일 수 있다                                            */
	/* -> 선을 그리거나 텍스트 위치시키는 동안 WM_MOUSEMOVE 메시지 처리될 때 사용 */
	/* ************************************************************************** */
	DWORD dwPos = GetMessagePos();
	CPoint clickedPoint((int)(short)LOWORD(dwPos), (int)(short)HIWORD(dwPos));  /* X/Y 좌표 초기화 */
	ScreenToClient(&clickedPoint);
	/* ************************************************************************** */
	
	if (clickedPoint.x % m_nBlocksPerXSize == 0 || clickedPoint.y % m_nBlocksPerYSize == 0)
		m_bShouldUpdated = FALSE;
	else
	{
		/* ********************************************************************** */
		/* 해당 블럭에 대한 인덱스 정보 설정 및 인덱스 출력 위치 설정한다         */
		/* ********************************************************************** */
		UINT nY = clickedPoint.y / m_nBlocksPerYSize;
		UINT nX = clickedPoint.x / m_nBlocksPerXSize;

		UINT nIndex = nY * m_nBlocksPerRow + nX + m_nyPos * m_nBlocksPerRow;
		/* ********************************************************************** */
		
		if (nIndex<m_nNumberofBlocks && nX<m_nBlocksPerRow)
		{
// jtkim 20130525
/*			if (st_handler.cwnd_main != NULL)			// Main 화면 존재
			{
				if (st_work.n_run_status != dSTOP) return;

//				st_handler.cwnd_main->SendMessage(WM_WORK_END, MAIN_TRAYINFO, 0);
				return;
			}*/
			/* ****************************************************************** */
			/* SetCapture() : 마우스 캡쳐를 현재 쓰레드에 속한 윈도우 상에 설정   */
			/* -> 마우스를 포착한 윈도우는 마우스가 그 윈도우 바깥 영역에         */
			/*    마우스의 모든 입력을 직접 받게 된다                             */
			/* -> 하나의 윈도우만이 마우스를 포착할 수 있다                       */
			/* -> 다른 쓰레드에 의해 만들어진 윈도우 위에서 버튼이 클릭될 때에는  */
			/*    그 윈도우로 마우스 입력을 보내게 된다                           */
			/* ****************************************************************** */
			SetCapture();
			/* ****************************************************************** */

			/* ****************************************************************** */
			/* 출력할 블록의 인덱스 정보를 생성한다                               */
			/* ****************************************************************** */
			CString strNumber;
			strNumber.Format(_T("%d"), nIndex + m_nStartIndex);
			/* ****************************************************************** */
			
			CreateSafeTooltipRect(clickedPoint.x, clickedPoint.y, strNumber);  /* 툴팁 출력할 영역 위치 설정 함수 */
			
			/* ****************************************************************** */
			/* 클라이언트 영역에 툴팁 출력할 사각형 생성한다                      */
			/* -> CreateSolidBrush() : 주어진 칼라를 가지고 논리적인 펜을 생성    */
			/* -> Rectangle() : 주어진 좌표에 의해 정의되는 직사각형 그리는 함수  */
			/*  : 직사각형은 현재 펜으로 그려지고 현재 브러시로 채워진다          */
			/* -> FillRect() : 지정된 사각형을 주어진 브러시로 채우는 함수        */
			/* ****************************************************************** */
			CClientDC dc(this);
			CBrush brush;
			brush.CreateSolidBrush(GetSysColor(COLOR_INFOBK));
			dc.Rectangle(tooltipRect);
			
			tooltipRect.left++;
			tooltipRect.top++;
			tooltipRect.bottom--;
			tooltipRect.right--;
			
			dc.FillRect(tooltipRect, &brush);
			/* ****************************************************************** */
			
			/* ****************************************************************** */
			/* 지정한 칼라를 사용하여 툴팁 정보를 출력한다                        */
			/* -> MulDiv() : 두 개의 32비트 값을 곱하고 그 결과값인 64비트 값을   */
			/*    세 번째 32비트 값으로 나누는 함수                               */
			/*  : 결과값은 정수에 가장 가깝게 반올림한다                          */
			/* -> CreateFontIndirect() : 주어진 LOGFONT 구조체에서 정의한 특성을  */
			/*    가지는 논리적 글꼴을 생성하는 함수                              */
			/*  : 응용 프로그램은 이 글꼴을 선택하여 어떤 장치에서도 이 글꼴을    */
			/*    사용할 수 있다                                                  */
			/* -> SelectObject() : 장치 컨텍스트에 현재 오브젝트(폰트) 설정       */
			/* -> SetTextColor() : 지정한 칼라로 장치 컨텍스트의 텍스트 칼라 설정 */
			/*  : 기본값 텍스트 칼라는 검정이다                                   */
			/*  : 텍스트 출력에 어떠한 RGB 칼라도 설정할 수 있다                  */
			/*  : 칼라는 장치 컨텍스트가 해제되거나 다른 칼라 설정까지 유효       */
			/* -> SetBkColor() : 현재의 배경색을 설정하는 함수                    */
			/*  : 배경 모드는 윈도우가 텍스트와 선들 내의 공백을 그리고           */
			/*    색의 비트맵을 단색으로 변환하는 방식을 지정한다                 */
			/* -> DrawText() : 원하는 출력 형식으로 텍스트 그리는 함수            */
			/* ****************************************************************** */
			CFont font;
			LOGFONT logFont;
			wsprintf(logFont.lfFaceName, TEXT("%s"), TEXT("Verdana"));
			logFont.lfHeight = -MulDiv(12, GetDeviceCaps(dc.m_hDC, LOGPIXELSY), 72);
			logFont.lfWidth = 0;
			logFont.lfEscapement = 0;
			logFont.lfItalic = FALSE;
			logFont.lfStrikeOut = FALSE;
			logFont.lfUnderline = FALSE;
			logFont.lfWeight = FW_BOLD;
			
			font.CreateFontIndirect(&logFont);
			
			dc.SelectObject(font);
			
			dc.SetTextColor(GetSysColor(COLOR_INFOTEXT));
			dc.SetBkColor(GetSysColor(COLOR_INFOBK));
			dc.DrawText(strNumber, tooltipRect, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
			
			tooltipRect.left--;
			tooltipRect.top--;
			tooltipRect.bottom++;
			tooltipRect.right++;
			/* ****************************************************************** */

			m_bShouldUpdated = TRUE;
		}
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CCheckerCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_nVbox == 0)	return;
	/* ************************************************************************** */
	/* 마우스 UP되면 클라이언트 영역을 갱신하고 장치 컨텍스트 해제한다            */
	/* -> SetCapture() 후에는 반드시 ReleaseCapture()해야 한다                    */
	/* -> InvalidateRect() : 주어진 윈도우에 현재 갱신 영역을 지정된 직사각형에   */
	/*    덧붙이는 함수                                                           */
	/*  : 갱신 영역은 다시 그려져야 하는 윈도우의 부분을 정의한다                 */
	/* -> ReleaseCapture() : 현재 스레드의 윈도우로부터 마우스 포착을 해제하고    */
	/*    일반적인 마우스 입력 프로세싱으로 복원하는 함수                         */
	/*  : 다른 쓰레드의 윈도우 안에 마우스가 클릭되는 것을 제외하고는             */
	/*    모든 마우스 입력을 마우스를 포착한 윈도우가 받게 된다                   */
	/* ************************************************************************** */
	if (m_bShouldUpdated)
	{
		CClientDC dc(this);
		InvalidateRect(&tooltipRect, FALSE);
		ReleaseCapture();
		
		m_bShouldUpdated = FALSE;
	}
	/* ************************************************************************** */

	CWnd::OnLButtonUp(nFlags, point);
}

/* ****************************************************************************** */
/* 툴팁 출력할 영역 위치 설정 함수                                                */
/* -> 왼족 마우스 버튼 DOWN 시 출력할 툴팁 박스의 위치 정보를 설정하게 된다       */
/* ****************************************************************************** */
void CCheckerCtrl::CreateSafeTooltipRect(int x, int y, LPCTSTR lpszText)
{
//	int nTextLength = strlen(lpszText);  /* 출력할 문자 길이 설정 */
	CString strTmp;
	strTmp = lpszText;
	int nTextLength = strTmp.GetLength();  /* 출력할 문자 길이 설정 */
	int nTextWidth = nTextLength;        /* 출력할 문자 포함 박스의 가로 길이 설정 */
	if (nTextWidth < 5)
		nTextWidth = 5;
	
	if (x + 12 * nTextWidth <= m_rcClient.right)
		tooltipRect.right = x + 12 * nTextWidth;
	else
		tooltipRect.right = x - 12 * nTextWidth;

	if (y + 25 <= m_rcClient.bottom)
		tooltipRect.bottom = y + 25;
	else
		tooltipRect.bottom = y - 25;

	tooltipRect.left = x;
	tooltipRect.top = y;

	if (tooltipRect.left > tooltipRect.right)
	{
		int nTemp = tooltipRect.left;

		tooltipRect.left = tooltipRect.right;
		tooltipRect.right = nTemp;
	}

	if (tooltipRect.top > tooltipRect.bottom)
	{
		int nTemp = tooltipRect.bottom;

		tooltipRect.bottom = tooltipRect.top;
		tooltipRect.top = nTemp;
	}
}
/* ****************************************************************************** */

void CCheckerCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
return;
	/* ************************************************************************** */
	/* SetCursor() : 커서의 모양을 변경하는 함수                                  */
	/* -> 한 번 사용된 후에는 빠르게 수행된다                                     */
	/* -> 프로그램 속도에는 거의 영향을 받지 않고 커서 모양을 변경할 수 있다      */
	/* -> 커서 모양은 반드시 LoadCursor() 함수로 처음 읽어 들여야 한다            */
	/*  : 클래스 정의에 커서를 가진 윈도우에서 이 함수를 사용하려면 커서가 깜박임 */
	/*  : WM_MOUSEMOVE 메시지가 보내질 때마다 클래스 커서와 SetCursor() 함수로    */
	/*    로드된 커서 사이를 윈도우가 전환하기 때문이다                           */
	/*  : 이 문제를 피하려면 클래스 커서를 NULL로 설정하면 된다                   */
	/* -> LoadCursor() : 주어진 인스턴스 핸들을 가지는 실행 파일이나 DLL로부터    */
	/*    커서 자원을 로드한다                                                    */
	/*  : 커서가 윈도우 클래스 정의 부분에서 로드된다면 마우스가 윈도우           */
	/*    클라이언트 영역 내에 있을 때 읽어들인 마우스 형태로 변경된다            */
	/*  : 같은 윈도우 클라이언트 영역 내에서도 어떤 때에 다른 커서 모양을 갖게    */
	/*    하려면 윈도우 클래스 커서 모양은 NULL로 설정하고 WM_SETCURSOR 메시지가  */
	/*    받아 들여질 때 SetCursor() 함수를 사용하면 가능하다                     */
	/* ************************************************************************** */
//	SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_HAND)));
	/* ************************************************************************** */

	CWnd::OnMouseMove(nFlags, point);
}

/* ****************************************************************************** */
/* 블럭들에 할당된 칼라 리셋 함수                                                 */
/* ****************************************************************************** */
void CCheckerCtrl::Reset()
{
	/* ************************************************************************** */
	/* 블럭들에 할당된 칼라를 원래 칼라(검정)으로 복원한다                        */
	/* ************************************************************************** */
	for(UINT i=0; i<m_nNumberofBlocks; i++)
		m_crColor.SetAt(i, BLANKED_BLOCKS_COLOR);
	/* ************************************************************************** */
}
/* ****************************************************************************** */

/* ****************************************************************************** */
/* 출력된 블럭들에 대한 칼라 갱신 함수                                            */
/* ****************************************************************************** */
void CCheckerCtrl::Update(const UINT nBlockNumber)
{
	ASSERT(nBlockNumber >= m_nStartIndex && nBlockNumber - m_nStartIndex <= m_nNumberofBlocks);

	/* ************************************************************************** */
	/* 출력된 블럭들의 칼라 갱신한다                                              */
	/* -> 이 때 블럭들은 이전에 정의된 블럭 범위 안에 존재해야 한다               */
	/* -> CreateSolidBrush() : 주어진 색깔 사용하여 논리적인 펜을 생성하는 함수   */
	/* -> FillRect() : 지정된 사각형을 주어진 브러시로 채우는 함수                */
	/*  : 클라이언트 영역의 백그라운드 칼라로 채운다                              */
	/* ************************************************************************** */
	if (IsVisible(nBlockNumber))  /* 해당 블럭 출력 유무 검사 함수 */
	{
		UINT nRow = (nBlockNumber - m_nStartIndex) / m_nBlocksPerRow - m_nyPos;
		UINT nColumn = (nBlockNumber - m_nStartIndex) % m_nBlocksPerRow;
		
		CClientDC dc(this);
		CRect rect = GetRect(nRow, nColumn);  /* 블럭에 대한 사각형 영역 정보 리턴 함수 */
		
		rect.left++;
		rect.top++;
		rect.bottom--;
		rect.right--;
		
		CBrush brush;
		brush.CreateSolidBrush(m_crColor.GetAt(nBlockNumber - m_nStartIndex));
		dc.FillRect(rect, &brush);
	}
	/* ************************************************************************** */
}
/* ****************************************************************************** */

/* ****************************************************************************** */
/* 블럭에 대한 사각형 영역 정보 리턴 함수                                         */
/* ****************************************************************************** */
CRect CCheckerCtrl::GetRect(const UINT nRow, const UINT nColumn)
{
	/* ************************************************************************** */
	/* 블럭을 표시할 사각형 영역 정보 계산하여 리턴한다                           */
	/* ************************************************************************** */
	CRect rect;
	rect.left = nColumn * m_nBlocksPerXSize + 2;
	rect.top = nRow * m_nBlocksPerYSize + 2;
	rect.right = rect.left + (m_nBlocksPerXSize-2);
	rect.bottom = rect.top + m_nBlocksPerYSize-2;
	/* ************************************************************************** */

	return rect;
}
/* ****************************************************************************** */

/* ****************************************************************************** */
/* 해당 블럭 출력 유무 검사 함수                                                  */
/* -> 블럭 시작과 블럭 종료 사이의 블럭인 경우 출력해야 한다                      */
/* ****************************************************************************** */
BOOL CCheckerCtrl::IsVisible(const UINT nBlockNumber)
{
	/* ************************************************************************** */
	/* 클라이언트 영역에 존재하는 마지막 블럭의 인덱스 계산한다                   */
	/* ************************************************************************** */
	UINT nBlockEndPos = m_nBlockStartPos + m_nTotalVisibleBlocks + m_nBlocksPerRow;
	if (nBlockEndPos > m_nNumberofBlocks)
		nBlockEndPos = m_nNumberofBlocks;

	if (nBlockNumber >= m_nBlockStartPos && nBlockNumber <= nBlockEndPos)
		return TRUE;
	else
		return FALSE;
	/* ************************************************************************** */
}
/* ****************************************************************************** */

void CCheckerCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	/* ************************************************************************** */
	/* 마우스 오른쪽 버튼이 DOWN되었다는 메시지 클라이언트 윈도우에 전송한다      */
	/* -> SetFocus() : 주어진 윈도우에 입력 초점을 전달하는 함수                  */
	/*  : 모든 키보드 입력이 초점을 가진 윈도우로 전달된다                        */
	/*  : 한번에 하나의 윈도우만이 입력 초점을 가질 수 있다                       */
	/* -> PostMessage() : 주어진 윈도우를 생성한 쓰레드와 관련있는 메시지 큐에    */
	/*    메시지를 놓고 반응을 기다리지 않고 반환하는 함수                        */
	/*  : 큐에 있는 메시지들은 GetMessage()와 PeekMessage() 함수 호출하여 얻는다  */
	/*  : 보통 현재 메시지가 처리된 뒤 응용 프로그램의 다른 부분을 실행하기 위해  */
	/*    PostMessage()를 사용한다                                                */
	/*  : PostMessage()는 반환값이 있는 컨트롤에게 메시지 보내는데 사용할 수 없다 */
	/* ************************************************************************** */
	SetFocus();

	if (!(point.x % m_nBlocksPerXSize == 0 || point.y % m_nBlocksPerYSize == 0))
	{
		UINT nY = point.y / m_nBlocksPerYSize;
		UINT nX = point.x / m_nBlocksPerXSize;

		UINT nIndex = nY * m_nBlocksPerRow + nX + m_nyPos * m_nBlocksPerRow;
		if (nIndex<m_nNumberofBlocks && nX<m_nBlocksPerRow)
			m_pParentWnd->PostMessage(WM_CHECKERCTRL_RBUTTONDOWN, m_nID, nIndex + m_nStartIndex);
	}
	/* ************************************************************************** */

	CWnd::OnRButtonDown(nFlags, point);
}

void CCheckerCtrl::SetValueBox(int n_Vbox)
{
	m_nVbox = n_Vbox;
}
