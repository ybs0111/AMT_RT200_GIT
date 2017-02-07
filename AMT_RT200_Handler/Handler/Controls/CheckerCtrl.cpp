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

#define BLANKED_BLOCKS_COLOR	RGB(252, 252, 252)  /* �⺻ Į�� ���� */

CCheckerCtrl::CCheckerCtrl()
{
	/* ************************************************************************** */
	/* Ŭ���̾�Ʈ ������ ����� ��׶��Ʈ �귯���� �����Ѵ�                      */
	/* -> CreateSolidBrush() : �־��� ���� ����Ͽ� ������ ���� �����ϴ� �Լ�   */
	/* -> GetSysColor() : ���÷��� ���� ��ҿ� ���� Į�� (R/G/B) �� ���� �Լ�   */
	/* ************************************************************************** */
//	m_backgroundBrush.CreateSolidBrush(GetSysColor(COLOR_WINDOW));
	m_backgroundBrush.CreateSolidBrush(RGB(0, 0, 0));
	/* ************************************************************************** */
	
	/* ************************************************************************** */
	/* Ŭ���� ���� �ʱ�ȭ �۾� �����Ѵ�                                           */
	/* -> Ŭ���̾�Ʈ ������ ������ ��� ���� �����鿡 ���� �ʱ�ȭ �۾��̴�        */
	/* ************************************************************************** */
	m_nyPos = 0;				/* ��ũ�� ��ġ �ʱ�ȭ */
	m_nOffset = 0;				/* ��ũ�� OFFSET �ʱ�ȭ */

	m_nBlockStartPos = 0;		/* ��� ���� ��ġ �ʱ�ȭ */
	m_nStartIndex = 0;			/* ���� ��� �ε��� �ʱ�ȭ */

	m_nNumberofBlocks = 0;      /* ������ �� ��� �� �ʱ�ȭ */
	m_nTotalVisibleBlocks = 0;  /* ����� �� ��� �� �ʱ�ȭ */

	m_nBlocksPerRow = 0;		/* ���� ��� �� �ʱ�ȭ */
	m_nBlocksPerColumn = 0;		/* ���� ��� �� �ʱ�ȭ */
	/* ************************************************************************** */

	m_nVbox = 0;				// ��� ���� �ʱ�ȭ.
}

CCheckerCtrl::~CCheckerCtrl()
{
}

BOOL CCheckerCtrl::Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID)
{
	/* ************************************************************************** */
	/* ������ �������Ϳ� ��� �� �����Ѵ�                                         */
	/* -> AfxRegisterWndClass() : Ŭ���� �̸��� �������Ϳ� ����ϴ� �Լ�          */
	/*  : �� �Լ��� Create() ��� �Լ����� ����Ѵ�                               */
	/* -> CreateEx() : ������ ���� �Լ�                                           */
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
	/* ���콺 ���� ��ư DOWN �� �ش� �޽��� �����ϱ� ���� ���� �ʱ�ȭ �۾�        */
	/* ************************************************************************** */
	m_nID = nID;                /* ��Ʈ�� ID ���� */
	m_pParentWnd = pParentWnd;  /* ��Ʈ�� �ڵ� ���� */
	/* ************************************************************************** */
	
	/* ************************************************************************** */
	/* SetCursor() : Ŀ���� ����� �����ϴ� �Լ�                                  */
	/* -> �� �� ���� �Ŀ��� ������ ����ȴ�                                     */
	/* -> ���α׷� �ӵ����� ���� ������ ���� �ʰ� Ŀ�� ����� ������ �� �ִ�      */
	/* -> Ŀ�� ����� �ݵ�� LoadCursor() �Լ��� ó�� �о� �鿩�� �Ѵ�            */
	/*  : Ŭ���� ���ǿ� Ŀ���� ���� �����쿡�� �� �Լ��� ����Ϸ��� Ŀ���� ������ */
	/*  : WM_MOUSEMOVE �޽����� ������ ������ Ŭ���� Ŀ���� SetCursor() �Լ���    */
	/*    �ε�� Ŀ�� ���̸� �����찡 ��ȯ�ϱ� �����̴�                           */
	/*  : �� ������ ���Ϸ��� Ŭ���� Ŀ���� NULL�� �����ϸ� �ȴ�                   */
	/* -> LoadCursor() : �־��� �ν��Ͻ� �ڵ��� ������ ���� �����̳� DLL�κ���    */
	/*    Ŀ�� �ڿ��� �ε��Ѵ�                                                    */
	/*  : Ŀ���� ������ Ŭ���� ���� �κп��� �ε�ȴٸ� ���콺�� ������           */
	/*    Ŭ���̾�Ʈ ���� ���� ���� �� �о���� ���콺 ���·� ����ȴ�            */
	/*  : ���� ������ Ŭ���̾�Ʈ ���� �������� � ���� �ٸ� Ŀ�� ����� ����    */
	/*    �Ϸ��� ������ Ŭ���� Ŀ�� ����� NULL�� �����ϰ� WM_SETCURSOR �޽�����  */
	/*    �޾� �鿩�� �� SetCursor() �Լ��� ����ϸ� �����ϴ�                     */
	/* ************************************************************************** */
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	/* ************************************************************************** */
	
	return bRet;  /* �Լ� ���� �÷��� ���� */
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
	/* FillRect() : ������ �簢���� �־��� �귯�÷� ä��� �Լ�                   */
	/* -> Ŭ���̾�Ʈ ������ ��׶��� Į��� ä���                              */
	/* ************************************************************************** */
	dc.FillRect(m_rcClient, &m_backgroundBrush);
	/* ************************************************************************** */

	UINT nColumn = 0, nRow = 0;  /* ��Ͽ� ���� ����/���� ���� ���� ���� �ʱ�ȭ */
	
	/* ************************************************************************** */
	/* Ŭ���̾�Ʈ ������ �����ϴ� ������ ��� ��� �ε��� �����Ѵ�                */
	/* ************************************************************************** */
	UINT nBlockEndPos = m_nBlockStartPos + m_nTotalVisibleBlocks + m_nBlocksPerRow;
	if (nBlockEndPos > m_nNumberofBlocks)
		nBlockEndPos = m_nNumberofBlocks;
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* Ŭ���̾�Ʈ ������ ������ �귯�� �̿��Ͽ� ��� �׸���                       */
	/* ************************************************************************** */
	for(UINT i=m_nBlockStartPos; i<nBlockEndPos; i++)
	{
		CBrush brush(m_crColor.GetAt(i));
		SetBlock(nRow, nColumn, brush, dc);  /* �־��� ��� �귯�� ����Ͽ� ��� �׸��� �Լ� */
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
/* Ŭ���̾�Ʈ ������ �׸� ��� ���� ���� �Է¹޴� �Լ�                            */
/* ****************************************************************************** */
void CCheckerCtrl::SetTotalBlocks(const UINT nBlocksPerRow, const UINT nBlocksPerColumn, const UINT nStartIndex)
{
	m_nNumberofBlocks = nBlocksPerRow * nBlocksPerColumn;  /* ������ �� ��� ���� ���� */

	/* ************************************************************************** */
	/* ������ ��Ͽ� ������ COLORREF �迭�� �����Ѵ�                              */
	/* ************************************************************************** */
	m_crColor.SetSize(m_nNumberofBlocks);
	for(UINT i=0; i<m_nNumberofBlocks; i++)
		m_crColor.SetAt(i, BLANKED_BLOCKS_COLOR);
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* GetClientRect() : �־��� �������� Ŭ���̾�Ʈ ���� �簢���� ��� �Լ�       */
	/* -> ��ǥ�� �������� Ŭ���̾�Ʈ �������κ��� ���Ǵ� ��� ��ǥ�̴�          */
	/*  : ����, ���� ����� ��ǥ�� (0,0)�̴�                                    */
	/* -> GetClientRect() Ư�� �������� Ŭ���̾�Ʈ ������ ũ�� �˰� ���� ��       */
	/*    �����ϰ� ����� �� �ִ�                                                 */
	/* ************************************************************************** */
	GetClientRect(m_rcClient);
	/* ************************************************************************** */

	m_nBlocksPerRow = nBlocksPerRow;        /* ���� ���� ��� �� ���� */
	m_nBlocksPerColumn = nBlocksPerColumn;  /* ���� ���� ��� �� ���� */

	m_nBlocksPerXSize = (m_rcClient.Width()-3) / m_nBlocksPerRow;      /* ��� ���� ũ�� ���� */
	m_nBlocksPerYSize = (m_rcClient.Height()-1) / m_nBlocksPerColumn;  /* ��� ���� ũ�� ���� */

	m_nTotalVisibleBlocks = m_nBlocksPerRow * m_nBlocksPerColumn;  /* �� ������ ��� �� ���� */

	/* ************************************************************************** */
	/* Ŭ���̾�Ʈ ������ ���� ��ũ�ѹ� ���� ����Ѵ�                              */
	/* -> SetScrollRange() : �־��� ��ũ�ѹٿ��� ������ �ְ� ��ũ�� �ڽ��� ����   */
	/*    ��ġ �����ϴ� �Լ�                                                      */
	/*  : �� �Լ��� �Ϲ����� ��ũ�� �ٸ� ����ų� �����ִµ� ���Ǳ⵵ �Ѵ�      */
	/*  : SetScrollRange() �Լ��� ��ũ�� �ٸ� ����� ������ ������ ��ũ�� ����    */
	/*    ������ ���ϴµ� ����ϸ� �ȴ�                                           */
	/*  : �Ķ���� ��� 0���� �����ϸ� ��ũ�� �ڽ��� ��µ��� �ʴ´�              */
	/* ************************************************************************** */
	int nOffset = (m_nNumberofBlocks / m_nBlocksPerRow);
	if (m_nNumberofBlocks % m_nBlocksPerRow != 0)
		nOffset++;
	
	m_nOffset = nOffset - m_nBlocksPerColumn;
	if (m_nOffset > 0)
		SetScrollRange(SB_VERT, 0, m_nOffset);
	/* ************************************************************************** */

	m_nStartIndex = nStartIndex;  /* ��� ���� ��ġ ���� */
}
/* ****************************************************************************** */

/* ****************************************************************************** */
/* �־��� ��� �귯�� ����Ͽ� ��� �׸��� �Լ�                                   */
/* ****************************************************************************** */
void CCheckerCtrl::SetBlock(int nRow, int nColumn, CBrush &brush, CDC &dc)
{
	CRect rect = GetRect(nRow, nColumn);  /* Ŭ���̾�Ʈ ���� ���� ���� */

	/* ************************************************************************** */
	/* Rectangle() : �־��� ��ǥ�� ���� ���ǵǴ� ���簢���� �׸��� �Լ�           */
	/* -> ���簢���� ���� ������ �׷����� ���� �귯�÷� ä������                  */
	/* ************************************************************************** */
	dc.Rectangle(&rect);
	/* ************************************************************************** */
	
	rect.left++;
	rect.top++;
	rect.bottom--;
	rect.right--;
	
	/* ************************************************************************** */
	/* FillRect() : ������ �簢���� �־��� �귯�÷� ä��� �Լ�                   */
	/* ************************************************************************** */
	dc.FillRect(rect, &brush);
	/* ************************************************************************** */
}
/* ****************************************************************************** */

/* ****************************************************************************** */
/* Ư�� ��Ͽ� ���� Į�� ���� ���� �Լ�                                           */
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

	int nyOffset;  /* ��ũ�� OFFSET ���� ���� */

	switch(nSBCode)
	{
	case SB_PAGEDOWN :  /* PAGE DOWN ��û */
		/* ********************************************************************** */
		/* Ŭ���̾�Ʈ ������ ��ũ�� �ڽ� �ٽ� �׸��� ��ũ���Ѵ�                   */
		/* -> SetScrollPos() : �־��� ��ũ�� �� ���ο� �ִ� ��ũ�� �ڽ� ��ġ ���� */
		/*  : ��û�Ǹ� ���ο� ��ġ�� ��ũ�� �ٸ� �ٽ� �׸���                      */
		/*  : SetScrollPos()�� ó�� ��ũ�� �ٰ� ��������ų� ��µ� ��            */
		/*    ��ũ�� �ڽ��� ��ġ�� �����ϱ� ���� �ַ� ����Ѵ�                    */
		/*  : Ű���� �������̽� ����� ����� ���� ���ȴ�                       */
		/* -> ScrollWindow() : �־��� �������� �۾� ������ ���빰�� ��ũ�� �Ѵ�   */
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
	case SB_PAGEUP :  /* PAGE UP ��û */
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
	case SB_LINEUP :  /* LINE UP ��û */
		if (m_nyPos>0)
		{
			m_nBlockStartPos -= m_nBlocksPerRow;

			m_nyPos--;
			SetScrollPos(SB_VERT, m_nyPos);
			ScrollWindow(0, m_nBlocksPerYSize);
		}
		break;
	case SB_LINEDOWN :  /* LINE DOWN ��û */
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
/* Ŭ���̾�Ʈ ���� �����ϴ� �Լ�                                                  */
/* ****************************************************************************** */
void CCheckerCtrl::Refresh()
{
	Invalidate();  /* ������ ���� ���� �Լ� */
}
/* ****************************************************************************** */

/* ****************************************************************************** */
/* ���� ���� Į�� ���� �Լ�                                                     */
/* ****************************************************************************** */
COLORREF CCheckerCtrl::GetBlock(const UINT nBlockNumber) const
{
	/* ************************************************************************** */
	/* -> nBlockNumber : ���ǵ� ��� ���� �����ϴ� ���� ���̾�� �Ѵ�             */
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
	/* SetFocus() : �־��� �����쿡 �Է� ������ �����ϴ� �Լ�                     */
	/* -> ��� Ű���� �Է��� ������ ���� ������� ���޵ȴ�                        */
	/* -> �ѹ��� �ϳ��� �����츸�� �Է� ������ ���� �� �ִ�                       */
	/* ************************************************************************** */
	SetFocus();
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* GetMessagePos() : ������ �޽����� ���޵Ǿ��� �� ȭ�� ��ǥ�� Ŀ�� ��ġ ��ȯ */
	/* -> ���� ���α׷��� �ĸ鿡�� �޽��� ó�� ������ �� ���콺 Ŀ���� �޽�����   */
	/*    ó���Ǳ� ���� ������ �� �ִ�                                            */
	/* -> �� �Լ��� �޽����� ���޵Ǿ��� �� ���콺 Ŀ���� ��ġ ��ȯ�Ѵ�            */
	/* ScreenToClient() : ȭ�� ��ǥ�� Ŭ���̾�Ʈ ��ǥ�� ��ȯ�ϴ� �Լ�             */
	/* -> ���콺 Ŀ���� ��ġ�� ������ Ŭ���̾�Ʈ ������ X/Y�� ��ȯ�ϱ� ����       */
	/*    ���� GetCursorPos() �Լ��� �����Ͽ� ���ȴ�                            */
	/*    ó���Ǳ� ���� ������ �� �ִ�                                            */
	/* -> ���� �׸��ų� �ؽ�Ʈ ��ġ��Ű�� ���� WM_MOUSEMOVE �޽��� ó���� �� ��� */
	/* ************************************************************************** */
	DWORD dwPos = GetMessagePos();
	CPoint clickedPoint((int)(short)LOWORD(dwPos), (int)(short)HIWORD(dwPos));  /* X/Y ��ǥ �ʱ�ȭ */
	ScreenToClient(&clickedPoint);
	/* ************************************************************************** */
	
	if (clickedPoint.x % m_nBlocksPerXSize == 0 || clickedPoint.y % m_nBlocksPerYSize == 0)
		m_bShouldUpdated = FALSE;
	else
	{
		/* ********************************************************************** */
		/* �ش� ���� ���� �ε��� ���� ���� �� �ε��� ��� ��ġ �����Ѵ�         */
		/* ********************************************************************** */
		UINT nY = clickedPoint.y / m_nBlocksPerYSize;
		UINT nX = clickedPoint.x / m_nBlocksPerXSize;

		UINT nIndex = nY * m_nBlocksPerRow + nX + m_nyPos * m_nBlocksPerRow;
		/* ********************************************************************** */
		
		if (nIndex<m_nNumberofBlocks && nX<m_nBlocksPerRow)
		{
// jtkim 20130525
/*			if (st_handler.cwnd_main != NULL)			// Main ȭ�� ����
			{
				if (st_work.n_run_status != dSTOP) return;

//				st_handler.cwnd_main->SendMessage(WM_WORK_END, MAIN_TRAYINFO, 0);
				return;
			}*/
			/* ****************************************************************** */
			/* SetCapture() : ���콺 ĸ�ĸ� ���� �����忡 ���� ������ �� ����   */
			/* -> ���콺�� ������ ������� ���콺�� �� ������ �ٱ� ������         */
			/*    ���콺�� ��� �Է��� ���� �ް� �ȴ�                             */
			/* -> �ϳ��� �����츸�� ���콺�� ������ �� �ִ�                       */
			/* -> �ٸ� �����忡 ���� ������� ������ ������ ��ư�� Ŭ���� ������  */
			/*    �� ������� ���콺 �Է��� ������ �ȴ�                           */
			/* ****************************************************************** */
			SetCapture();
			/* ****************************************************************** */

			/* ****************************************************************** */
			/* ����� ����� �ε��� ������ �����Ѵ�                               */
			/* ****************************************************************** */
			CString strNumber;
			strNumber.Format(_T("%d"), nIndex + m_nStartIndex);
			/* ****************************************************************** */
			
			CreateSafeTooltipRect(clickedPoint.x, clickedPoint.y, strNumber);  /* ���� ����� ���� ��ġ ���� �Լ� */
			
			/* ****************************************************************** */
			/* Ŭ���̾�Ʈ ������ ���� ����� �簢�� �����Ѵ�                      */
			/* -> CreateSolidBrush() : �־��� Į�� ������ ������ ���� ����    */
			/* -> Rectangle() : �־��� ��ǥ�� ���� ���ǵǴ� ���簢�� �׸��� �Լ�  */
			/*  : ���簢���� ���� ������ �׷����� ���� �귯�÷� ä������          */
			/* -> FillRect() : ������ �簢���� �־��� �귯�÷� ä��� �Լ�        */
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
			/* ������ Į�� ����Ͽ� ���� ������ ����Ѵ�                        */
			/* -> MulDiv() : �� ���� 32��Ʈ ���� ���ϰ� �� ������� 64��Ʈ ����   */
			/*    �� ��° 32��Ʈ ������ ������ �Լ�                               */
			/*  : ������� ������ ���� ������ �ݿø��Ѵ�                          */
			/* -> CreateFontIndirect() : �־��� LOGFONT ����ü���� ������ Ư����  */
			/*    ������ ���� �۲��� �����ϴ� �Լ�                              */
			/*  : ���� ���α׷��� �� �۲��� �����Ͽ� � ��ġ������ �� �۲���    */
			/*    ����� �� �ִ�                                                  */
			/* -> SelectObject() : ��ġ ���ؽ�Ʈ�� ���� ������Ʈ(��Ʈ) ����       */
			/* -> SetTextColor() : ������ Į��� ��ġ ���ؽ�Ʈ�� �ؽ�Ʈ Į�� ���� */
			/*  : �⺻�� �ؽ�Ʈ Į��� �����̴�                                   */
			/*  : �ؽ�Ʈ ��¿� ��� RGB Į�� ������ �� �ִ�                  */
			/*  : Į��� ��ġ ���ؽ�Ʈ�� �����ǰų� �ٸ� Į�� �������� ��ȿ       */
			/* -> SetBkColor() : ������ ������ �����ϴ� �Լ�                    */
			/*  : ��� ���� �����찡 �ؽ�Ʈ�� ���� ���� ������ �׸���           */
			/*    ���� ��Ʈ���� �ܻ����� ��ȯ�ϴ� ����� �����Ѵ�                 */
			/* -> DrawText() : ���ϴ� ��� �������� �ؽ�Ʈ �׸��� �Լ�            */
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
	/* ���콺 UP�Ǹ� Ŭ���̾�Ʈ ������ �����ϰ� ��ġ ���ؽ�Ʈ �����Ѵ�            */
	/* -> SetCapture() �Ŀ��� �ݵ�� ReleaseCapture()�ؾ� �Ѵ�                    */
	/* -> InvalidateRect() : �־��� �����쿡 ���� ���� ������ ������ ���簢����   */
	/*    �����̴� �Լ�                                                           */
	/*  : ���� ������ �ٽ� �׷����� �ϴ� �������� �κ��� �����Ѵ�                 */
	/* -> ReleaseCapture() : ���� �������� ������κ��� ���콺 ������ �����ϰ�    */
	/*    �Ϲ����� ���콺 �Է� ���μ������� �����ϴ� �Լ�                         */
	/*  : �ٸ� �������� ������ �ȿ� ���콺�� Ŭ���Ǵ� ���� �����ϰ��             */
	/*    ��� ���콺 �Է��� ���콺�� ������ �����찡 �ް� �ȴ�                   */
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
/* ���� ����� ���� ��ġ ���� �Լ�                                                */
/* -> ���� ���콺 ��ư DOWN �� ����� ���� �ڽ��� ��ġ ������ �����ϰ� �ȴ�       */
/* ****************************************************************************** */
void CCheckerCtrl::CreateSafeTooltipRect(int x, int y, LPCTSTR lpszText)
{
//	int nTextLength = strlen(lpszText);  /* ����� ���� ���� ���� */
	CString strTmp;
	strTmp = lpszText;
	int nTextLength = strTmp.GetLength();  /* ����� ���� ���� ���� */
	int nTextWidth = nTextLength;        /* ����� ���� ���� �ڽ��� ���� ���� ���� */
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
	/* SetCursor() : Ŀ���� ����� �����ϴ� �Լ�                                  */
	/* -> �� �� ���� �Ŀ��� ������ ����ȴ�                                     */
	/* -> ���α׷� �ӵ����� ���� ������ ���� �ʰ� Ŀ�� ����� ������ �� �ִ�      */
	/* -> Ŀ�� ����� �ݵ�� LoadCursor() �Լ��� ó�� �о� �鿩�� �Ѵ�            */
	/*  : Ŭ���� ���ǿ� Ŀ���� ���� �����쿡�� �� �Լ��� ����Ϸ��� Ŀ���� ������ */
	/*  : WM_MOUSEMOVE �޽����� ������ ������ Ŭ���� Ŀ���� SetCursor() �Լ���    */
	/*    �ε�� Ŀ�� ���̸� �����찡 ��ȯ�ϱ� �����̴�                           */
	/*  : �� ������ ���Ϸ��� Ŭ���� Ŀ���� NULL�� �����ϸ� �ȴ�                   */
	/* -> LoadCursor() : �־��� �ν��Ͻ� �ڵ��� ������ ���� �����̳� DLL�κ���    */
	/*    Ŀ�� �ڿ��� �ε��Ѵ�                                                    */
	/*  : Ŀ���� ������ Ŭ���� ���� �κп��� �ε�ȴٸ� ���콺�� ������           */
	/*    Ŭ���̾�Ʈ ���� ���� ���� �� �о���� ���콺 ���·� ����ȴ�            */
	/*  : ���� ������ Ŭ���̾�Ʈ ���� �������� � ���� �ٸ� Ŀ�� ����� ����    */
	/*    �Ϸ��� ������ Ŭ���� Ŀ�� ����� NULL�� �����ϰ� WM_SETCURSOR �޽�����  */
	/*    �޾� �鿩�� �� SetCursor() �Լ��� ����ϸ� �����ϴ�                     */
	/* ************************************************************************** */
//	SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_HAND)));
	/* ************************************************************************** */

	CWnd::OnMouseMove(nFlags, point);
}

/* ****************************************************************************** */
/* ���鿡 �Ҵ�� Į�� ���� �Լ�                                                 */
/* ****************************************************************************** */
void CCheckerCtrl::Reset()
{
	/* ************************************************************************** */
	/* ���鿡 �Ҵ�� Į�� ���� Į��(����)���� �����Ѵ�                        */
	/* ************************************************************************** */
	for(UINT i=0; i<m_nNumberofBlocks; i++)
		m_crColor.SetAt(i, BLANKED_BLOCKS_COLOR);
	/* ************************************************************************** */
}
/* ****************************************************************************** */

/* ****************************************************************************** */
/* ��µ� ���鿡 ���� Į�� ���� �Լ�                                            */
/* ****************************************************************************** */
void CCheckerCtrl::Update(const UINT nBlockNumber)
{
	ASSERT(nBlockNumber >= m_nStartIndex && nBlockNumber - m_nStartIndex <= m_nNumberofBlocks);

	/* ************************************************************************** */
	/* ��µ� ������ Į�� �����Ѵ�                                              */
	/* -> �� �� ������ ������ ���ǵ� �� ���� �ȿ� �����ؾ� �Ѵ�               */
	/* -> CreateSolidBrush() : �־��� ���� ����Ͽ� ������ ���� �����ϴ� �Լ�   */
	/* -> FillRect() : ������ �簢���� �־��� �귯�÷� ä��� �Լ�                */
	/*  : Ŭ���̾�Ʈ ������ ��׶��� Į��� ä���                              */
	/* ************************************************************************** */
	if (IsVisible(nBlockNumber))  /* �ش� �� ��� ���� �˻� �Լ� */
	{
		UINT nRow = (nBlockNumber - m_nStartIndex) / m_nBlocksPerRow - m_nyPos;
		UINT nColumn = (nBlockNumber - m_nStartIndex) % m_nBlocksPerRow;
		
		CClientDC dc(this);
		CRect rect = GetRect(nRow, nColumn);  /* ���� ���� �簢�� ���� ���� ���� �Լ� */
		
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
/* ���� ���� �簢�� ���� ���� ���� �Լ�                                         */
/* ****************************************************************************** */
CRect CCheckerCtrl::GetRect(const UINT nRow, const UINT nColumn)
{
	/* ************************************************************************** */
	/* ���� ǥ���� �簢�� ���� ���� ����Ͽ� �����Ѵ�                           */
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
/* �ش� �� ��� ���� �˻� �Լ�                                                  */
/* -> �� ���۰� �� ���� ������ ���� ��� ����ؾ� �Ѵ�                      */
/* ****************************************************************************** */
BOOL CCheckerCtrl::IsVisible(const UINT nBlockNumber)
{
	/* ************************************************************************** */
	/* Ŭ���̾�Ʈ ������ �����ϴ� ������ ���� �ε��� ����Ѵ�                   */
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
	/* ���콺 ������ ��ư�� DOWN�Ǿ��ٴ� �޽��� Ŭ���̾�Ʈ �����쿡 �����Ѵ�      */
	/* -> SetFocus() : �־��� �����쿡 �Է� ������ �����ϴ� �Լ�                  */
	/*  : ��� Ű���� �Է��� ������ ���� ������� ���޵ȴ�                        */
	/*  : �ѹ��� �ϳ��� �����츸�� �Է� ������ ���� �� �ִ�                       */
	/* -> PostMessage() : �־��� �����츦 ������ ������� �����ִ� �޽��� ť��    */
	/*    �޽����� ���� ������ ��ٸ��� �ʰ� ��ȯ�ϴ� �Լ�                        */
	/*  : ť�� �ִ� �޽������� GetMessage()�� PeekMessage() �Լ� ȣ���Ͽ� ��´�  */
	/*  : ���� ���� �޽����� ó���� �� ���� ���α׷��� �ٸ� �κ��� �����ϱ� ����  */
	/*    PostMessage()�� ����Ѵ�                                                */
	/*  : PostMessage()�� ��ȯ���� �ִ� ��Ʈ�ѿ��� �޽��� �����µ� ����� �� ���� */
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
