// VLabel.cpp : implementation file
//

#include "stdafx.h"
#include "VLabel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVLabel
CVLabel::CVLabel()
{
	// Set default foreground text
	m_crTextColor = ::GetSysColor(COLOR_BTNTEXT);

	// Set default background text
	m_crBkColor = ::GetSysColor(COLOR_BTNFACE);

	m_pFont = NULL;

	m_uiAngle = 0;
}

CVLabel::~CVLabel()
{
}


BEGIN_MESSAGE_MAP(CVLabel, CStatic)
	//{{AFX_MSG_MAP(CVLabel)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVLabel message handlers

void CVLabel::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	int				iLineCounter = 0;
	CStringArray	arrayText;
	int				nLength;
	TCHAR			*chLabel;
	char			*pszText;
	char			*context = NULL;
	char			*psz;

	CRect r1, r2;							// 크기를 얻기 위한 변수.
	GetClientRect(&r1);						// 크기를 얻는 함수.
	r2 = r1;								// 백업 작업.

	CBrush br, *oldbr;
	br.CreateSolidBrush(::GetSysColor(COLOR_BTNSHADOW));
	oldbr = dc.SelectObject(&br);	//SelectObject로 br을 받으면서 oldbr에 백업 작업.
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

			nLength		= str.GetLength() + 1;
			pszText		= new char[nLength];
			chLabel		= new TCHAR[nLength];

			wsprintfA(pszText, "%S", str);
	
			psz = strtok_s(pszText, "\n", &context);
			
			while (psz)
			{
				MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, psz, nLength, chLabel, nLength);

				arrayText.Add(chLabel);
				iLineCounter++;
				psz = strtok_s(NULL, "\n", &context);
			}

			delete pszText;
			pszText = NULL;

			delete chLabel;
			chLabel = NULL;

			CSize size(0, 0);
			
			if (!str.IsEmpty() )
			{
				CFont* oldFont=NULL;
				COLORREF crOldText;
				
				if(m_pFont) oldFont = dc.SelectObject(m_pFont);
				
				size = dc.GetTextExtent(str);

				int nOldMode = dc.SetBkMode(TRANSPARENT);
				
				crOldText = dc.SetTextColor(m_crTextColor);
			
				for (int I=0; I<arrayText.GetSize(); I++)
				{
					RECT stRect;

					stRect.left   = 0;
					stRect.top    = I * size.cy;
					if (iLineCounter == 1)						// 한줄짜리면 중간으로 밀어버린다.
					{
						stRect.top = (r1.Height() - size.cy) / 2;
					}
					stRect.right  = stRect.left + size.cx;
					stRect.bottom = stRect.top  + size.cy;

					::DrawText(dc, arrayText.GetAt(I), -1, &stRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
				}
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

void CVLabel::SetTextFont(CFont *pFont)
{
	m_pFont = pFont;
}

void CVLabel::SetColor(COLORREF crBk, COLORREF crTx)
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

	// Repaint control
	Invalidate();
}

void CVLabel::SetTextAlign(int n)
{
	if(n < 0 || n > 2) return;

	m_nAlign = n;
}

void CVLabel::SetText(LPCWSTR szText)
{
	m_strText = szText;

	CStatic::SetWindowText(szText);
	
	RedrawWindow();
}