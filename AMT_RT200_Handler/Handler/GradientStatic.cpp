// GradientStatic.cpp : implementation file
//

#include "stdafx.h"
#include "GradientStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGradientStatic

BEGIN_MESSAGE_MAP(CGradientStatic, CStatic)
	//{{AFX_MSG_MAP(CGradientStatic)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGradientStatic message handlers



CGradientStatic::CGradientStatic()
{

	m_iLeftSpacing = 10;
	clLeft = GetSysColor(COLOR_ACTIVECAPTION);
	clRight = GetSysColor(COLOR_BTNFACE);
	clText = GetSysColor(COLOR_CAPTIONTEXT);
	
	m_bCenter = FALSE;

	hinst_msimg32 = LoadLibrary( _T("msimg32.dll") );
	m_bCanDoGradientFill = FALSE;
	pFont = NULL;
	
	if(hinst_msimg32)
	{
		m_bCanDoGradientFill = TRUE;		
		dllfunc_GradientFill = ((LPFNDLLFUNC1) GetProcAddress( hinst_msimg32, (LPCSTR)"GradientFill"));
	}

}

CGradientStatic::~CGradientStatic()
{
	FreeLibrary( hinst_msimg32 );
}



//this function will be used only if msimg32.dll library is not available
//it was suggested by Paolo Adami (adamix@libero.it)
void CGradientStatic::DrawGradRect(CDC *pDC, CRect r, COLORREF cLeft, COLORREF cRight)
{
	CRect stepR;					// rectangle for color's band
	COLORREF color;				// color for the bands
	float fStep = ((float)r.Width())/255.0f;	// width of color's band

	for (int iOnBand = 0; iOnBand < 255; iOnBand++) 
	{
	// set current band
	SetRect(&stepR,
		r.left+(int)(iOnBand * fStep), 
		r.top,
		r.left+(int)((iOnBand+1)* fStep), 
		r.bottom);	
	// set current color
	color = RGB((GetRValue(cRight)-GetRValue(cLeft))*((float)iOnBand)/255.0f+GetRValue(cLeft),
			(GetGValue(cRight)-GetGValue(cLeft))*((float)iOnBand)/255.0f+GetGValue(cLeft),
			(GetBValue(cRight)-GetBValue(cLeft))*((float)iOnBand)/255.0f+GetBValue(cLeft));
	// fill current band
	pDC->FillSolidRect(stepR,color);
	}
}


void CGradientStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(&rect);

	if(m_bCanDoGradientFill) //msimg32.dll library is loaded
	{
	
		TRIVERTEX rcVertex[2];
		rect.right--; // exclude this point, like FillRect does 
		rect.bottom--;
		rcVertex[0].x=rect.left;
		rcVertex[0].y=rect.top;
		rcVertex[0].Red=GetRValue(clLeft)<<8;	// color values from 0x0000 to 0xff00 !!!!
		rcVertex[0].Green=GetGValue(clLeft)<<8;
		rcVertex[0].Blue=GetBValue(clLeft)<<8;
		rcVertex[0].Alpha=0x0000;
		rcVertex[1].x=rect.right; 
		rcVertex[1].y=rect.bottom;
		rcVertex[1].Red=GetRValue(clRight)<<8;
		rcVertex[1].Green=GetGValue(clRight)<<8;
		rcVertex[1].Blue=GetBValue(clRight)<<8;
		rcVertex[1].Alpha=0;
		GRADIENT_RECT rect;
		rect.UpperLeft=0;
		rect.LowerRight=1;
		
		// fill the area 
		dllfunc_GradientFill( dc,rcVertex,2,&rect,1,GRADIENT_FILL_RECT_H);
		
	}
	else
	{
		//msimg32.dll is not available. Let's use our own code to display gradient background.
		//This code is very simple and produces worse gradient that function from the library - but works!
		DrawGradRect(&dc,rect,clLeft,clRight);
	}
	
	//let's set color defined by user
	::SetTextColor(dc,clText);

	HFONT hfontOld;
	if(pFont)
		hfontOld = (HFONT)SelectObject(dc.m_hDC, (HFONT)pFont->m_hObject);

	::SetBkMode(dc, TRANSPARENT);
	GetClientRect(&rect);

	if(m_bCenter)
		::DrawText(dc, m_sTEXT, -1, &rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	else
	{
		rect.left+=m_iLeftSpacing;
		::DrawText(dc, m_sTEXT, -1, &rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
	}

	if(pFont)
		::SelectObject(dc.m_hDC, hfontOld);
}

void CGradientStatic::SetWindowText(CString lpszString)
{
	m_sTEXT = lpszString;
	Invalidate(TRUE);
}

CString CGradientStatic::GetWindowText()
{
	return m_sTEXT;
}


