// Animate.cpp : implementation file
//
#include "stdafx.h"
#include "Handler.h"
#include "Animate.h"
#include "math.h"

#include "Variable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnimate

#define getrandom(min,max) ((rand()%(int)(((max)+1)-(min)))+(min));

//*********************************************
//					CAxisMod
//*********************************************
/*
CAxisMod::CAxisMod() : m_pBitAxis1(NULL),m_HomePoint(100,100),m_dEqWidth(0),m_nImgCurPos_X(0),m_nImgCurPos_Y(0)
					  ,m_dRatio(1),m_dEqCurPos_X(0),m_dEqCurPos_Y(0)
{

}
*/

CAxisMod::CAxisMod()
{
	m_pBitAxis[0]	= NULL;
	m_pBitAxis[1]	= NULL;
	m_pBitAxis[2]	= NULL;
	m_pBitAxis[3]	= NULL;
	m_pBitAxis[4]	= NULL;
	
	m_HomePoint.x	= m_HomePoint.y = 100;
	m_dEqWidth		= 0.0f;
	m_nImgCurPos_X	= 0;
	m_nImgCurPos_Y	= 0;
	m_dRatio		= 1.0f;
	m_dEqCurPos_X	= 0.0f;
	m_dEqCurPos_Y	= 0.0f;
}

CAxisMod::~CAxisMod()
{
	m_BitAxis[0].DeleteObject();
	m_BitAxisMask[0].DeleteObject();
}

void CAxisMod::ComverEqPosToImgPos(int nImg)
{
	m_nImgCurPos_X =    m_HomePoint.x + int(m_dEqCurPos_X * m_dRatio)  - int(m_ImgSize[nImg].cx/2.0+ 0.5) ;
	m_nImgCurPos_Y =    m_HomePoint.y + int(m_dEqCurPos_Y * m_dRatio)  - int(m_ImgSize[nImg].cy/2.0+ 0.5) ;
}


void CAxisMod::CreateImg(CDC *pDC, int nImg1, int nImgMask1, int nImg2, int nImgMask2, int nImg3, int nImgMask3, int nImg4, int nImgMask4, int nImg5, int nImgMask5)
{
	BITMAP bs;

	m_DcAxis[0].CreateCompatibleDC(pDC);
	m_BitAxis[0].LoadBitmap(nImg1);
	m_pBitAxis[0] = (CBitmap *)m_DcAxis[0].SelectObject(&m_BitAxis[0]);

	m_DcAxisMask[0].CreateCompatibleDC(pDC);
	m_BitAxisMask[0].LoadBitmap(nImgMask1);
	m_pBitAxisMask[0] = (CBitmap *)m_DcAxisMask[0].SelectObject(&m_BitAxisMask[0]);

	m_BitAxis[0].GetBitmap(&bs);
	m_ImgSize[0].cx = bs.bmWidth;
	m_ImgSize[0].cy = bs.bmHeight;

	m_DcAxis[1].CreateCompatibleDC(pDC);
	m_BitAxis[1].LoadBitmap(nImg2);
	m_pBitAxis[1] = (CBitmap *)m_DcAxis[1].SelectObject(&m_BitAxis[1]);

	m_DcAxisMask[1].CreateCompatibleDC(pDC);
	m_BitAxisMask[1].LoadBitmap(nImgMask2);
	m_pBitAxisMask[1] = (CBitmap *)m_DcAxisMask[1].SelectObject(&m_BitAxisMask[1]);

	m_BitAxis[1].GetBitmap(&bs);
	m_ImgSize[1].cx = bs.bmWidth;
	m_ImgSize[1].cy = bs.bmHeight;

	m_DcAxis[2].CreateCompatibleDC(pDC);
	m_BitAxis[2].LoadBitmap(nImg3);
	m_pBitAxis[2] = (CBitmap *)m_DcAxis[2].SelectObject(&m_BitAxis[2]);

	m_DcAxisMask[2].CreateCompatibleDC(pDC);
	m_BitAxisMask[2].LoadBitmap(nImgMask3);
	m_pBitAxisMask[2] = (CBitmap *)m_DcAxisMask[2].SelectObject(&m_BitAxisMask[2]);

	m_BitAxis[2].GetBitmap(&bs);
	m_ImgSize[2].cx = bs.bmWidth;
	m_ImgSize[2].cy = bs.bmHeight;

	m_DcAxis[3].CreateCompatibleDC(pDC);
	m_BitAxis[3].LoadBitmap(nImg4);
	m_pBitAxis[3] = (CBitmap *)m_DcAxis[3].SelectObject(&m_BitAxis[3]);

	m_DcAxisMask[3].CreateCompatibleDC(pDC);
	m_BitAxisMask[3].LoadBitmap(nImgMask4);
	m_pBitAxisMask[3] = (CBitmap *)m_DcAxisMask[3].SelectObject(&m_BitAxisMask[3]);

	m_BitAxis[3].GetBitmap(&bs);
	m_ImgSize[3].cx = bs.bmWidth;
	m_ImgSize[3].cy = bs.bmHeight;

	m_DcAxis[4].CreateCompatibleDC(pDC);
	m_BitAxis[4].LoadBitmap(nImg5);
	m_pBitAxis[4] = (CBitmap *)m_DcAxis[4].SelectObject(&m_BitAxis[4]);

	m_DcAxisMask[4].CreateCompatibleDC(pDC);
	m_BitAxisMask[4].LoadBitmap(nImgMask5);
	m_pBitAxisMask[4] = (CBitmap *)m_DcAxisMask[4].SelectObject(&m_BitAxisMask[4]);

	m_BitAxis[4].GetBitmap(&bs);
	m_ImgSize[4].cx = bs.bmWidth;
	m_ImgSize[4].cy = bs.bmHeight;
}

void CAxisMod::DrawImg(CDC &dc, int nImg)
{
	ComverEqPosToImgPos(nImg);

	dc.BitBlt(m_nImgCurPos_X, m_nImgCurPos_Y, m_ImgSize[nImg].cx, m_ImgSize[nImg].cy, &m_DcAxisMask[nImg], 0, 0, SRCAND);
	dc.BitBlt(m_nImgCurPos_X, m_nImgCurPos_Y, m_ImgSize[nImg].cx, m_ImgSize[nImg].cy, &m_DcAxis[nImg], 0, 0, SRCPAINT);
}


//*********************************************
//					CGlass
//*********************************************
CEqGlass::CEqGlass():m_sizGlassSize(80,140) ,m_nGlassPos(100,100),m_nDegree(0),m_nDir(2)
{}	 
CEqGlass::~CEqGlass()
{} 

void CEqGlass::CellRectCf(CRect &CellRect)
{

	return;
	CellRect.right	-= 10;
	CellRect.top	+= 10;
	

	const int deg =  abs(m_nDegree%180) /45;

	
	switch(m_nDir)
	{
	case 0:
		switch(deg)
		{
		case 0:
			CellRect.right	+= 10;
			CellRect.top	+= 10;
			break;

		case 1:
			CellRect.left	+= 10;
			CellRect.top	+= 10;
			break;

		case 3:
			CellRect.right	+= 10;
			CellRect.top	+= 10;
			break;
		}
		break;

	case 1:
		CellRect.left	+= 10;
		CellRect.bottom -= 10;
		break;

	case 2:
		CellRect.right	-= 10;
		CellRect.top	+= 10;
//
//		switch(deg)
//		{
//		case 0:
//			CellRect.left 	-= 10;
//			CellRect.top	+= 10;
//			break;
//
//		case 1:
//			CellRect.right	-= 10;
//			CellRect.top	+= 10;
//			break;
//
//		case 3:
//			CellRect.right	-= 10;
//			CellRect.top	+= 10;
//			break;
//		}

		break;

	case 3:
		CellRect.right	-= 10;
		CellRect.bottom	-= 10;
		break;
	}	
}

void CEqGlass::RotateRectAngle(CRect rect,double nDegree, CRect rectCen)
{
	CPoint pointCenter;
	double cosine = cos(nDegree * 3.14159265359 / 180.);
	double sine	  = sin(nDegree * 3.14159265359 / 180.);
	// ¡ﬂΩ…√‡¿ª ±‚¡ÿ¿∏∑Œ »∏¿¸
	pointCenter.x = rect.CenterPoint().x;
	pointCenter.y = rect.CenterPoint().y;

	int x0 = pointCenter.x;
	int y0 = pointCenter.y;

	double x1 = cosine	*	(rect.TopLeft().x - x0)		- sine		*	(rect.TopLeft().y - y0)		+ x0;
	double y1 = sine	*   (rect.TopLeft().x - x0)		+ cosine	*	(rect.TopLeft().y - y0)		+ y0;

	double x2 = cosine	*	(rect.BottomRight().x - x0) - sine		*   (rect.TopLeft().y - y0)		+ x0;
	double y2 = sine	*   (rect.BottomRight().x - x0) + cosine	*	(rect.TopLeft().y - y0)		+ y0;

	double x3 = cosine	*	(rect.BottomRight().x - x0) - sine		*   (rect.BottomRight().y - y0) + x0;
	double y3 = sine	*   (rect.BottomRight().x - x0) + cosine	*	(rect.BottomRight().y - y0) + y0;

	double x4 = cosine	*	(rect.TopLeft().x - x0)		- sine		*   (rect.BottomRight().y - y0) + x0;
    double y4 = sine	*	(rect.TopLeft().x - x0)		+ cosine	*   (rect.BottomRight().y - y0) + y0;	

	m_arrGlassPoint[0].x = int(x1); 
	m_arrGlassPoint[0].y = int(y1);
	m_arrGlassPoint[1].x = int(x2); 
	m_arrGlassPoint[1].y = int(y2); 
	m_arrGlassPoint[2].x = int(x3); 
	m_arrGlassPoint[2].y = int(y3); 
	m_arrGlassPoint[3].x = int(x4); 
	m_arrGlassPoint[3].y = int(y4);		
}

void CEqGlass::DrawImg(CDC &dc)
{
//	m_sizGlassSize.cx = GdbSetCellRecipe.fCellInfoShort/7.5;
//	m_sizGlassSize.cy = GdbSetCellRecipe.fCellInfoLong/7.5;

	CRect CellRect;
	CBrush tmpBrush,tmpBrush1,*pBrush;
	CellRect.left	 = m_nGlassPos.x - (m_sizGlassSizeMod.cx / 2);
	CellRect.top	 = m_nGlassPos.y - (m_sizGlassSizeMod.cy / 2);
	CellRect.right	 = m_nGlassPos.x + (m_sizGlassSizeMod.cx / 2);
	CellRect.bottom	 = m_nGlassPos.y + (m_sizGlassSizeMod.cy / 2);
	RotateRectAngle(CellRect, m_nDegree, CellRect);
//	tmpBrush.CreateSolidBrush(GdbSetColor.colorLightGray);
	pBrush = dc.SelectObject(&tmpBrush);
	dc.Polygon(m_arrGlassPoint, 4);
	CellRectCf(CellRect);
	RotateRectAngle(CellRect, m_nDegree, CellRect);
//	tmpBrush1.CreateSolidBrush(GdbSetColor.colorUser3);
	dc.SelectObject(&tmpBrush1);
	dc.Polygon(m_arrGlassPoint, 4);
	dc.SelectObject(pBrush);
	tmpBrush1.DeleteObject();
	tmpBrush.DeleteObject();

}


//*********************************************
//					CAnimate
//*********************************************

CAnimate::CAnimate():m_CursorPoint(0,0)
{
	int nBtnWidth, nBtnGab;
	nBtnWidth = 120;
	nBtnGab = 30;

	for(int i=0; i<3; i++)
	{
		m_BtnRect[i] = CRect((nBtnGab+(nBtnWidth)*(i)),65,((nBtnWidth)*(i+1)),95);
		m_bClicked[i] = FALSE;
	}
}

CAnimate::~CAnimate()
{
	m_AniFont.DeleteObject();
	m_AniSmallFont.DeleteObject();
	m_AniPen.DeleteObject();
	m_AniBrush.DeleteObject();
	m_AniBicFont.DeleteObject();

	m_pDcFrame.DeleteDC();
	m_bmpFrame.DeleteObject();
}

BEGIN_MESSAGE_MAP(CAnimate, CWnd)
	//{{AFX_MSG_MAP(CAnimate)
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAnimate message handlers
BOOL CAnimate::Create ( DWORD exstyle, DWORD style, const CRect& r, CWnd *p, UINT id, CCreateContext *pc ) 
{					
	
	CBrush mybrush ( RGB (255, 255, 0 ) ) ;

	CString mywindowclass ;
	mywindowclass = AfxRegisterWndClass ( CS_HREDRAW | CS_VREDRAW, 
										AfxGetApp( ) -> LoadCursor ( IDI_CURSOR ), 
										mybrush, 0 ) ;

	return CreateEx ( exstyle, mywindowclass, _T(""), style, 
						r.left, r.top, r.Width(),r.Height(), 
						p -> GetSafeHwnd( ), NULL, NULL ) ;
	mybrush.DeleteObject();

	return TRUE;
	
}

void CAnimate::OnDestroy() 
{
	CWnd::OnDestroy();

	m_AniFont.DeleteObject();
	m_AniSmallFont.DeleteObject();
	m_AniPen.DeleteObject();
	m_AniBrush.DeleteObject();
	m_AniBicFont.DeleteObject();

	m_pDcFrame.DeleteDC();
	m_bmpFrame.DeleteObject();
}


void CAnimate::OnInitAniWnd()
{
	int i=0;
	CDC* pAniDC = GetDC(); 

	m_AniFont.CreateFontW(12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, _T("±√º≠")); 
	m_AniBicFont.CreateFontW(17, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, _T("±√º≠")); 
	m_AniSmallFont.CreateFontW(9, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, _T("±√º≠")); 

	m_AniPen.CreatePen(PS_DASH, 3, RGB(50,200,255));
	m_AniBrush.CreateSolidBrush(RGB(155,255,0));
	
	
	// ===== BackFrame ======
	m_pDcFrame.CreateCompatibleDC(pAniDC);
	m_bmpFrame.LoadBitmap(IDB_ANI_BASE);
 	m_pbmpFrame = (CBitmap *)m_pDcFrame.SelectObject(&m_bmpFrame);

	ReleaseDC(pAniDC);	
}

void CAnimate::OnActionDisplay()
{
	int i=0;
		//if(!IsWindowVisible()) return;
		//===default Code =====
	CDC		*pDC = GetDC();
	CDC		memDC;
	CFont	*pOldFont = NULL;
	CPen    *pOldPen = NULL;
	CBrush	*pOldBrush = NULL;
	CString strTmp;	
	CRect rect;

	CRect rBtn[3] = {(m_BtnRect[0]),(m_BtnRect[1]),(m_BtnRect[2])};
 	rBtn[0] += CRect(6,6,6,6);
	rBtn[1] += CRect(6,6,6,6);

	CRect Rectangle = m_BtnRect[0];

 	GetClientRect(&rect);
	CBitmap memBitmap, *OldmemBitmap;
	memDC.CreateCompatibleDC(pDC);
	
	memBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	OldmemBitmap = (CBitmap *)memDC.SelectObject(&memBitmap);


	pOldFont  = memDC.SelectObject(&m_AniFont);
	pOldPen  = memDC.SelectObject(&m_AniPen);
	pOldBrush  = memDC.SelectObject(&m_AniBrush);

	memDC.SetBkMode(TRANSPARENT);	 
	memDC.SetTextColor(RGB(0,0,255));

	memDC.BitBlt(0,0, rect.Width(), rect.Height(), &m_pDcFrame, 0, 0, SRCCOPY);	


	//===default Code =====
	pDC->BitBlt(0,0,  rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	
	memDC.SelectObject(OldmemBitmap);
	memDC.SelectObject(pOldFont);
	memDC.SelectObject(pOldPen);
	memDC.SelectObject(pOldBrush);
	DeleteObject(OldmemBitmap);
	ReleaseDC(pDC);
	memDC.DeleteDC();
}

void CAnimate::OnAniPioStatus()
{
	//
}

void CAnimate::OnMagazineCntCheck(CDC *pDC, int nMagazineLoadExist, int nMagazineUnloadExist)
{	
	CString sTmp;
	int nCollor = 0;
	int nGap = 0;

	int nLoadStX = 137, nLoadStY = 675;
	int nUnLoadStX = 489, nUnLoadStY = 675;
	int nWidth = 35, nRectHight = 70;
	int nOffset = 0;

	for(int i=0; i<nMagazineLoadExist; i++) 
	{
		pDC->FillSolidRect(nLoadStX+nOffset+nGap, nLoadStY, nWidth, nRectHight, RGB(0,255,0));
		nGap = 2*(i+1);
		nOffset = nWidth * (i+1);
	}

	nOffset = 0; nGap = 0;

	for(int i=0; i<nMagazineUnloadExist; i++) 
	{
		pDC->FillSolidRect(nUnLoadStX-nOffset-nGap, nUnLoadStY, nWidth, nRectHight, RGB(0,0,255));
		nGap = 2*(i+1);
		nOffset = nWidth * (i+1);
	}
}

void CAnimate::OnMotorStepMonitoring(CDC *pDC)
{	
	CString sTmp;
	int nCollor=0;
	pDC->SetTextColor(RGB(255,0,0));

//	pDC->TextOut(10, 52, st_ani.m_sMotorStep[0]);
//	pDC->TextOut(270, 52, st_ani.m_sMotorStep[1]);
}
bool bTempFlag = false;
void CAnimate::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_BtnRect[0].PtInRect(point))
	{
		m_bClicked[0] = TRUE/*!m_bClicked*/;
		RedrawWindow(&m_BtnRect[0]);
	}

	if(m_BtnRect[1].PtInRect(point))
	{
		m_bClicked[1] = TRUE/*!m_bClicked*/;
		RedrawWindow(&m_BtnRect[1]);
	}
	bTempFlag = true;
	CWnd::OnLButtonDown(nFlags, point);
}

void CAnimate::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_BtnRect[0].PtInRect(point))
	{
		m_bClicked[0] = FALSE/*!m_bClicked*/;
		RedrawWindow(&m_BtnRect[0]);
	}

	if(m_BtnRect[1].PtInRect(point))
	{
		m_bClicked[1] = FALSE/*!m_bClicked*/;
		RedrawWindow(&m_BtnRect[1]);
	}
	bTempFlag = false;
	CWnd::OnLButtonDown(nFlags, point);
}

void CAnimate::OnMouseMove(UINT nFlags, CPoint point) 
{
	m_CursorPoint = point;

	if(m_bClicked[0] == TRUE)
	{
		if(!m_BtnRect[0].PtInRect(point))
		{
			m_bClicked[0] = FALSE;
			RedrawWindow(&m_BtnRect[0]);
		}
	}

	if(m_bClicked[1] == TRUE)
	{
		if(!m_BtnRect[1].PtInRect(point))
		{
			m_bClicked[1] = FALSE;
			RedrawWindow(&m_BtnRect[1]);
		}
	}

	/*if (bTempFlag == true)
	{
		m_clsLdRbt.SetImgHomePos(point);
	}*/
	CWnd::OnMouseMove(nFlags, point);
}

void CAnimate::OnRButtonDown(UINT nFlags, CPoint point)
{
	/*if (m_clsTSiteRbt.m_nImg > 4) m_clsTSiteRbt.m_nImg = 0;

	m_clsTSiteRbt.m_nImg += 1;*/

	CWnd::OnRButtonDown(nFlags, point);
}
