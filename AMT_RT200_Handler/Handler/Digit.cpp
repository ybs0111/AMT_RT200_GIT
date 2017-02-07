// Digit.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Digit.h"

#include "Variable.h"		// 전역 변수 정의 클래스 추가 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BORDER_SPACE	2

/////////////////////////////////////////////////////////////////////////////
// CDigit

CDigit::CDigit()
{
	m_nStyle = DS_INT;

	m_nWidth = -1;
	m_nHeight = -1;

	m_nVal = 0;
	m_fVal = 0;
	for(int i=0; i<3; i++) m_nT[i] = m_nOldT[i] = 0;

	m_bFirst = TRUE;
	m_bResize = FALSE;
	m_bActive = FALSE;
	m_nClockTimer = 0;
	m_bMinus = FALSE;
	m_nErrNum = 0;
}

CDigit::~CDigit()
{
	m_bmActive.DeleteObject();
	m_bmInactive.DeleteObject();
}


BEGIN_MESSAGE_MAP(CDigit, CStatic)
	//{{AFX_MSG_MAP(CDigit)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDigit message handlers

void CDigit::SetStyle(int nStyle, int nPrec, int nActive, int nInactive)
{
	m_bFirst = TRUE;
	if(m_nClockTimer) 
	{
		KillTimer(m_nClockTimer);
		m_nClockTimer = 0;
	}
	if(nStyle == DS_CLOCK) m_nClockTimer = 5432;

	if(nStyle < DS_INT || nStyle > DS_FLOAT3) m_nStyle = DS_INT;
	else m_nStyle = nStyle;
	
	if(nStyle == DS_INT || nStyle == DS_FLOAT || nStyle == DS_FLOAT2 || nStyle == DS_FLOAT3 || nStyle == DS_CHAR) m_nPrec = nPrec;
	else m_nPrec = 8;

	if(LoadBitmap(&m_bmActive, nActive))
	{
		BITMAP bm;
		m_bmActive.GetBitmap(&bm);
		m_nWidth = (int) bm.bmWidth / 18;
		m_nHeight = bm.bmHeight;
	}

	if(nStyle == DS_CLOCK || nStyle == DS_TIMER)
	{
		LoadBitmap(&m_bmInactive, nInactive);
	}
}

void CDigit::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect r;
	GetClientRect(&r);

	if(m_bFirst) ResizeCtrl(r);

	int nLoop, destX, destY, nAsciiChar;
	CBitmap* pOldDigit, *pOldMem;
	pOldDigit = pOldMem = NULL;
	
 	CBitmap bmMem;
	CDC memDC, digitDC;

	if(memDC.CreateCompatibleDC(&dc))
	{
		if(bmMem.CreateCompatibleBitmap(&dc, r.Width(), r.Height()))
		{
			pOldMem = memDC.SelectObject(&bmMem);
			if(pOldMem == NULL)
			{
			}
			else  // 바탕을 검정색으로 칠한다.
			{
				CBrush brBack;
				brBack.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
				memDC.FillRect(CRect(0, 0, r.Width(), r.Height()), &brBack);
			}
			if(digitDC.CreateCompatibleDC(&dc))
			{
				// Select bitmap
				if(m_nStyle == DS_INT || m_nStyle == DS_FLOAT || m_nStyle == DS_FLOAT2 || m_nStyle == DS_FLOAT3 || m_nStyle == DS_CHAR) pOldDigit = digitDC.SelectObject(&m_bmActive);
				else
				{
					if(m_bActive) pOldDigit = digitDC.SelectObject(&m_bmActive);
					else pOldDigit = digitDC.SelectObject(&m_bmInactive);
				}

				if(pOldDigit == NULL)
				{
				}
				else
				{
					// Start from more significative digit
					if(m_bResize) { destX = BORDER_SPACE; destY = BORDER_SPACE; }
					else { destX = m_nOffsetX; destY = m_nOffsetY; }
					
					BOOL bMinusZero = FALSE;
					if(m_nErrNum > 0 && ((m_nStyle == DS_INT && m_nPrec > 3) || (m_nStyle == DS_FLOAT && m_nPrec > 4) || (m_nStyle == DS_FLOAT2 && m_nPrec > 5) || (m_nStyle == DS_FLOAT3 && m_nPrec > 6)))
					{
						MakeErrString();
					}
					else
					{
						MakeString();
						if(m_nStyle == DS_INT && m_nVal == 0 && m_bMinus) bMinusZero = TRUE;
						else if(m_nStyle == DS_FLOAT && m_fVal == 0 && m_bMinus) bMinusZero = TRUE;
						else if(m_nStyle == DS_FLOAT2 && m_fVal == 0 && m_bMinus) bMinusZero = TRUE;
						else if(m_nStyle == DS_FLOAT3 && m_fVal == 0 && m_bMinus) bMinusZero = TRUE;
					}
					
					for(nLoop = 0; nLoop < m_nPrec; nLoop++)
					{
						nAsciiChar = 10;
						
						if (m_strData[nLoop] >= '0' && m_strData[nLoop] <= '9')	nAsciiChar = m_strData[nLoop] - 48;
						else if(m_strData[nLoop] == ' ') nAsciiChar = 10;
						else if(m_strData[nLoop] == '-') nAsciiChar = 12;
						else if(m_strData[nLoop] == 'A') nAsciiChar = 13;
						else if(m_strData[nLoop] == 'L') nAsciiChar = 14;
						else if(m_strData[nLoop] == 'E') nAsciiChar = 15;
						else if(m_strData[nLoop] == 'r') nAsciiChar = 16;
						else if(m_strData[nLoop] == '.') nAsciiChar = 17;
						else if(m_strData[nLoop] == ':') nAsciiChar = 18;
						else if(m_strData[nLoop] == 'x') nAsciiChar = 19;
						else nAsciiChar = 10;
						
						if(nAsciiChar==17)
						{
							memDC.BitBlt(destX, destY, m_nWidth/2, m_nHeight, &digitDC, 0+(m_nWidth*nAsciiChar), 0, SRCCOPY);
							destX += m_nWidth/2;
						}
						else if(nAsciiChar==18)
						{
							memDC.BitBlt(destX, destY, m_nWidth/2, m_nHeight, &digitDC, 0+(m_nWidth*nAsciiChar)-(m_nWidth/2), 0, SRCCOPY);
							destX += m_nWidth/2;
						}
						else if(nAsciiChar==19)
						{
							memDC.BitBlt(destX, destY, m_nWidth/2, m_nHeight, &digitDC, 0+(m_nWidth*(nAsciiChar-1)), 0, SRCCOPY);
							destX += m_nWidth/2;
						}
						else
						{
							memDC.BitBlt(destX, destY, m_nWidth, m_nHeight, &digitDC, 0+(m_nWidth*nAsciiChar), 0, SRCCOPY);
							destX += m_nWidth;
						}
					}
					if(bMinusZero)
					{
						destX = destX - 2*m_nWidth;
						memDC.BitBlt(destX, destY, m_nWidth, m_nHeight, &digitDC, m_nWidth*12, 0, SRCCOPY);
					}
					dc.BitBlt(0, 0, r.Width(), r.Height(), &memDC, 0, 0, SRCCOPY);
					// Restore old selected bitmap
					digitDC.SelectObject(pOldDigit);
					digitDC.DeleteDC();
				}
			}
			
			if(pOldMem) memDC.SelectObject(pOldMem);
			bmMem.DeleteObject();
		}
		memDC.DeleteDC();
	}
}

void CDigit::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 5432)
	{
		CTime cur = CTime::GetCurrentTime();
		m_nT[0] = cur.GetHour();
		m_nT[1] = cur.GetMinute();
		m_nT[2] = cur.GetSecond();
		Invalidate();
	}
	
	CStatic::OnTimer(nIDEvent);
}

void CDigit::MakeString()
{
	int i, diff;
	
	switch(m_nStyle)
	{
	case DS_INT:
		m_strData.Format(_T("%d"), m_nVal);
		diff = m_nPrec - m_strData.GetLength();
		for(i=0; i<diff; i++) m_strData = _T(" ") + m_strData;
		break;
	case DS_FLOAT:
		m_strData.Format(_T("%4.1f"), m_fVal);
		diff = m_nPrec - m_strData.GetLength();
		for(i=0; i<diff; i++) m_strData = _T(" ") + m_strData;
		break;
	case DS_TIMER:
		m_strData.Format(_T("%.2d:%.2d:%.2d"), m_nT[0], m_nT[1], m_nT[2]);
		break;
	case DS_CHAR:
		m_strData = m_strChar;
		diff = m_nPrec - m_strData.GetLength();
		for(i=0; i<diff; i++) m_strData = _T(" ") + m_strData;
		break;
	case DS_FLOAT2:
		m_strData.Format(_T("%4.2f"), m_fVal);
		diff = m_nPrec - m_strData.GetLength();
		for(i=0; i<diff; i++) m_strData = _T(" ") + m_strData;
		break;
	case DS_FLOAT3:
		m_strData.Format(_T("%4.3f"), m_fVal);
		diff = m_nPrec - m_strData.GetLength();
		for(i=0; i<diff; i++) m_strData = _T(" ") + m_strData;
		break;
	case DS_CLOCK:
		int bSame = TRUE;
		for(i=0; i<3; i++)
		{
			if(m_nT[i] != m_nOldT[i]) bSame = FALSE;
			m_nOldT[i] = m_nT[i];
		}
		if(!bSame) m_strData.Format(_T("%.2d:%.2d:%.2d"), m_nT[0], m_nT[1], m_nT[2]);
		else m_strData.Format(_T("%.2d:%.2dx%.2d"), m_nT[0], m_nT[1], m_nT[2]);
		break;
	
	}
//	if(m_strData.GetLength() > m_nPrec) m_strData = m_strData.Left(m_nPrec);
}

void CDigit::MakeErrString()
{
	if(m_nStyle == DS_FLOAT) m_strData.Format(_T("Err.%.1d"), m_nErrNum);
	else if(m_nStyle == DS_INT) m_strData.Format(_T("Err%.1d"), m_nErrNum);
	else if(m_nStyle == DS_FLOAT2) m_strData.Format(_T("Err%.2d"), m_nErrNum);
	else if(m_nStyle == DS_FLOAT3) m_strData.Format(_T("Err%.3d"), m_nErrNum);
}

void CDigit::EnableResize()
{
	m_bResize = TRUE;
	m_bFirst = TRUE;
}

BOOL CDigit::LoadBitmap(CBitmap* bm, int nType)
{
	int bRet;

	if((HBITMAP)*bm) bm->DeleteObject();
	switch(nType)
	{
		case DC_BLACK:
			bRet = bm->LoadBitmap(IDB_DIGITSTEX_BLACK);
			break;
		case DC_CYAN:
			bRet = bm->LoadBitmap(IDB_DIGITSTEX_CYAN);
			break;
		case DC_GREEN:
			bRet = bm->LoadBitmap(IDB_DIGITSTEX_GREEN);
			break;
		case DC_YELLOW:
			bRet = bm->LoadBitmap(IDB_DIGITSTEX_YELLOW);
			break;
		case DC_RED:
			bRet = bm->LoadBitmap(IDB_DIGITSTEX_RED);
			break;
		case DC_DISABLE:
			bRet = bm->LoadBitmap(IDB_DIGITSTEX_DISABLE);
			break;
		case DC_BBLACK:
			bRet = bm->LoadBitmap(IDB_DIGITSTEX_BBLACK);
			break;
		case DC_BCYAN:
			bRet = bm->LoadBitmap(IDB_DIGITSTEX_BCYAN);
			break;
		case DC_BGREEN:
			bRet = bm->LoadBitmap(IDB_DIGITSTEX_BGREEN);
			break;
		case DC_BYELLOW:
			bRet = bm->LoadBitmap(IDB_DIGITSTEX_BYELLOW);
			break;
		case DC_BRED:
			bRet = bm->LoadBitmap(IDB_DIGITSTEX_BRED);
			break;
		case DC_BBLUE:
			bRet = bm->LoadBitmap(IDB_DIGITSTEX_BBLUE);
			break;
		case DC_BDISABLE:
			bRet = bm->LoadBitmap(IDB_DIGITSTEX_BDISABLE);
			break;
		case DC_BBDISABLE:
			bRet = bm->LoadBitmap(IDB_DIGITSTEX_BBDISABLE);
			break;
		case DC_SBDISABLE:
			bRet = bm->LoadBitmap(IDB_DIGITSTEX_SBDISABLE);
			break;
		default:
			bRet = FALSE;
			break;
	}
	
	return bRet;
}

void CDigit::ActivateTimer(BOOL bAct)
{
	m_bActive = bAct;
}

void CDigit::UpdateTimer()
{
	if(!m_bFirst) Invalidate();
}

void CDigit::SetVal(int n)
{
	if(m_nVal == n) return;
	else m_nVal = n;

	Invalidate();
}

void CDigit::SetFloatVal(float f)
{
	if(m_fVal == f) return;
	else m_fVal = f;

	Invalidate();
}

void CDigit::ResizeCtrl(CRect r)
{
	m_bFirst = FALSE;
	
	if(m_bResize)
	{
		CRect rWin;
		GetWindowRect(&rWin);
		GetParent()->ScreenToClient(&rWin);
		
		rWin.right = rWin.left + (m_nWidth * m_nPrec)+(BORDER_SPACE*2);
		if (m_nStyle==DS_CLOCK || m_nStyle==DS_TIMER) rWin.right -= m_nWidth;
		else if(m_nStyle==DS_FLOAT || m_nStyle==DS_FLOAT2 || m_nStyle==DS_FLOAT3) rWin.right -= (m_nWidth / 2);
		
		rWin.bottom = rWin.top + m_nHeight+(BORDER_SPACE*2);
		
		MoveWindow(&rWin);
	}
	else
	{
		m_nOffsetX = (int) (r.Width() - (m_nWidth * m_nPrec)) / 2;
		if (m_nStyle==DS_CLOCK || m_nStyle==DS_TIMER) m_nOffsetX += (m_nWidth / 2);
		else if(m_nStyle==DS_FLOAT || m_nStyle==DS_FLOAT2 || m_nStyle==DS_FLOAT3)	m_nOffsetX += (m_nWidth / 4);
		
		m_nOffsetY = (int) (r.Height() - m_nHeight) / 2;
	}
	if(m_nStyle == DS_CLOCK) SetTimer(5432, 500, NULL);
}

void CDigit::SetError(int nErrNum)
{
	if(nErrNum < 1 || nErrNum > 9) return;
	if(nErrNum == m_nErrNum) return;

	m_nErrNum = nErrNum;
	Invalidate();
}

void CDigit::ClearErr()
{
	m_nErrNum = 0;
	Invalidate();
}
