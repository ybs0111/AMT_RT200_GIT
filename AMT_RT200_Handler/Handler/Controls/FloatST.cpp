// FloatST.cpp : implementation file
//

#include "stdafx.h"
#include "FloatST.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFloatST

CFloatST::CFloatST()
{
	// Set default foreground text
	m_crTextColor = ::GetSysColor(COLOR_BTNTEXT);

	// Set default background text
	m_crBkColor = ::GetSysColor(COLOR_BTNFACE);

	m_offsetX = 4;
	m_offsetY = 3;

	m_nAlign = STA_CENTER;
	m_nMargin = 3;

	m_pFont = NULL;
} // End of CFloatST


CFloatST::~CFloatST()
{
} // End of ~CFloatST


BEGIN_MESSAGE_MAP(CFloatST, CStatic)
	//{{AFX_MSG_MAP(CFloatST)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFloatST message handlers


void CFloatST::SetColor(COLORREF crBk, COLORREF crTx, int offx, int offy)
{
	// Set new background color
	if (crBk != 0xffffffff)
	{
		m_crBkColor = crBk;
	}
	else // Set default background color
	{
		m_crBkColor = ::GetSysColor(COLOR_BTNFACE);
	}

	if (crTx != 0xffffffff)
	{
		m_crTextColor = crTx;
	}
	else // Set default foreground color
	{
		m_crTextColor = ::GetSysColor(COLOR_BTNTEXT);
	}

	m_offsetX = offx;
	m_offsetY = offy;

	// Repaint control
	Invalidate();
}


void CFloatST::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect r1, r2, r3;
	GetClientRect(&r1);
	r2 = r1;

	r2.left += m_offsetX;	r2.top += m_offsetY;
	r1.right -= m_offsetX;	r1.bottom -= m_offsetY;


	CBrush br, *oldbr;
	br.CreateSolidBrush(::GetSysColor(COLOR_BTNSHADOW));
	oldbr = dc.SelectObject(&br);
	if(oldbr == NULL)
	{
	}
	else
	{
		dc.FillRect(&r2, &br);
		br.DeleteObject();
		br.CreateSolidBrush(m_crBkColor);
		CBrush *oldbr2 = dc.SelectObject(&br);
		if(oldbr2 == NULL)
		{
		}
		else
		{
			dc.FillRect(&r1, &br);
			
			CString str;
			GetWindowText(str);
			
			CSize size(0, 0);
			if (!str.IsEmpty() )
			{
				CFont* oldFont=NULL;
				COLORREF crOldText;
				if(m_pFont) oldFont = dc.SelectObject(m_pFont);
				size = dc.GetTextExtent(str);
				CPoint pt;
				if(m_nAlign == STA_CENTER) pt.x = (r1.Width() - size.cx) / 2;
				else if(m_nAlign == STA_RIGHT) pt.x = r1.Width() - size.cx - m_nMargin;
				else pt.x = r1.left + m_nMargin;
				pt.y = (r1.Height() - size.cy) / 2;
				//dc.SetBkColor(m_crBkColor);
				int nOldMode = dc.SetBkMode(TRANSPARENT);
				crOldText = dc.SetTextColor(m_crTextColor);
				dc.TextOut(pt.x, pt.y, str);
				if(m_pFont) dc.SelectObject(oldFont);
				dc.SetBkMode(nOldMode);
				dc.SetTextColor(crOldText);
			}
		}
		dc.SelectObject(oldbr);
		br.DeleteObject();
	}
	
	// Do not call CStatic::OnPaint() for painting messages
}

void CFloatST::SetTextAlign(int n, int nMargin)
{
	if(n < 0 || n > 2) return;
	m_nAlign = n;
	m_nMargin = nMargin;
}

void CFloatST::SetTextFont(CFont *pFont)
{
	m_pFont = pFont;
}

CString CFloatST::GetText()
{
	CString str;
	GetWindowText(str);

	return str;
}
