#include "stdafx.h"
#include "BannerStatic.h"

const int CBannerStatic::MAXSPEED(1000);  
const int CBannerStatic::MAXSPEED_MODIFIER(CBannerStatic::MAXSPEED*10);
const int CBannerStatic::TIMERRESOLUTION(100);
const int CBannerStatic::STEPHEIGHT(250);

//------------------------------------------------------------------------------
// CBannerStatic::CBannerStatic

CBannerStatic::CBannerStatic() : CMultiColorStatic()
{
   m_tmScroll = 0;
   m_nTextOut = 0;
   SetWrapText(TRUE);
   SetScrollSpeed(0);
   SetScrollDelay(100);
   SetScrollSize(-1);
}

CBannerStatic::~CBannerStatic()
{
}

void CBannerStatic::SetScrollSpeed(const int& nSpeed)
{
   if (nSpeed == 0)
   {
      if (m_tmScroll)
      {
         timeKillEvent(m_tmScroll);
         m_tmScroll = 0;
      }
   }
   else
   {
      m_nBannerSpeed = nSpeed;
      CalculateScrollParameters();
      m_tmScroll = timeSetEvent(GetScrollDelay(), TIMERRESOLUTION, TimerProc, 
                                (DWORD)this, TIME_CALLBACK_FUNCTION);
   }
}

int CBannerStatic::GetScrollSpeed(void) const
{
   return (m_nBannerSpeed);
}

void CBannerStatic::SetScrollSize(const int& nScrollSize)
{
   m_nScrollSize = nScrollSize;
}

int CBannerStatic::GetScrollSize(void) const
{
   return (GetScrollSpeed() > 0 ? -1 : 1);
}

void CBannerStatic::SetScrollDelay(const DWORD& dwScrollDelay)
{
   m_dwScrollDelay = dwScrollDelay;
}

DWORD CBannerStatic::GetScrollDelay(void) const
{
   return (m_dwScrollDelay);
}

void CBannerStatic::CalculateScrollParameters(void)
{
   //---------------------------------------------------------------------------
   // MAXSPEED_MODIFIER is currently just MAXSPEED * 10; this will result in
   // a scroll delay of no less than 10ms
   //
   m_dwScrollDelay = abs(MAXSPEED_MODIFIER / m_nBannerSpeed);

   m_nScrollSize = (m_nBannerSpeed >= 0 ? -1 : 1);

   int nStepHeight = STEPHEIGHT;
   m_nScrollSize *= ((m_rcBounds.Height() / nStepHeight)+1);
}

void CBannerStatic::ScrollBanner(void)
{
   CPoint ptScroll;
   if(GetOrientation()==ORIENTATION_HORIZONTAL) 
   {
	ptScroll.x = GetScrollSize();
	ptScroll.y = 0;
   }
   if(GetOrientation()==ORIENTATION_VERTICAL_RIGHT) 
   {
	ptScroll.x = 0;
	ptScroll.y = GetScrollSize();
   }
   if(GetOrientation()==ORIENTATION_VERTICAL_LEFT) 
   {
	ptScroll.x = 0;
	ptScroll.y = -GetScrollSize();
   }

   CDC* pDC = GetDC();
   LPtoDP(pDC->m_hDC, &ptScroll, 1);
   pDC->DPtoLP(&ptScroll, 1);
   ReleaseDC(pDC);

   
   ScrollWindow(ptScroll.x, ptScroll.y, m_rcBounds, m_rcBounds);
}

void CALLBACK CBannerStatic::TimerProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
   CBannerStatic* pBanner = (CBannerStatic*)dwUser;

   if (::IsWindow(pBanner->m_hWnd))
   {
      pBanner->PostMessage(WM_TIMER, uID, 0);
   }
}

void CBannerStatic::PreSubclassWindow(void)
{
   CMultiColorStatic::PreSubclassWindow();

   SetScrollSpeed(0);
}

BEGIN_MESSAGE_MAP(CBannerStatic, CMultiColorStatic)
   ON_WM_TIMER()
   ON_WM_PAINT()
   ON_WM_SIZE()
   ON_WM_DESTROY()
END_MESSAGE_MAP()

void CBannerStatic::OnTimer(UINT nIDEvent)
{
   if (nIDEvent == m_tmScroll)
   {
      if (GetNumStrings() > 0)
	  {
		if (GetWrapText())
        {
               	switch(GetOrientation())
				{
					case ORIENTATION_HORIZONTAL:
						if((--m_nTextOut + m_nTextLength) < m_rcBounds.left)
							m_nTextOut = m_rcBounds.right;
						break;
					case ORIENTATION_VERTICAL_LEFT:
						if((--m_nTextOut+m_nTextLength) < m_rcBounds.top)
							m_nTextOut = m_rcBounds.Height();
						break;
					case ORIENTATION_VERTICAL_RIGHT:
						if((--m_nTextOut + m_nTextLength) < m_rcBounds.top)
							m_nTextOut = m_rcBounds.bottom;
						Invalidate();
						break;
					default:
						break;
				}
		}	
		ScrollBanner();
	  }
      
      m_tmScroll = timeSetEvent(GetScrollDelay(), TIMERRESOLUTION, TimerProc, 
                                (DWORD)this, TIME_CALLBACK_FUNCTION);
   }
   else
   {
      CMultiColorStatic::OnTimer(nIDEvent);
   }
}

void CBannerStatic::SetWrapText(const BOOL& fWrapText)
{
   m_fWrapText = fWrapText;
}

BOOL CBannerStatic::GetWrapText(void) const
{
   return (m_fWrapText);
}

void CBannerStatic::OnPaint()
{
	CPaintDC dc(this);
      
	CRect rcBounds = m_rcBounds;      
	if(GetOrientation()==ORIENTATION_HORIZONTAL) 
		rcBounds.left = m_nTextOut;
	if(GetOrientation()==ORIENTATION_VERTICAL_LEFT) 
		rcBounds.bottom = m_rcBounds.Height()-m_nTextOut;
	if(GetOrientation()==ORIENTATION_VERTICAL_RIGHT) 
		rcBounds.top = m_nTextOut;
   dc.FillRect(m_rcBounds, &m_brBackGround);
   dc.IntersectClipRect(m_rcBounds); 

   m_nTextLength = 0;

   for (int i = 0; i < m_astrData.GetSize(); i++)
   {
      CColorString* pstrCurrent = reinterpret_cast<CColorString*>(m_astrData.GetAt(i));
      TEXTMETRIC    stFontMetrics;
      SIZE          stSize;

      DetermineFont(pstrCurrent);

      dc.SelectObject(&m_ftText)->DeleteObject();
		if (pstrCurrent->GetBackColor() == ::GetSysColor(COLOR_BTNFACE))
      {
         dc.SetBkColor(m_crBackColor);
      }
      else
      {
         dc.SetBkColor(pstrCurrent->GetBackColor());
      }
		dc.SetTextColor(pstrCurrent->GetColor());
		dc.GetOutputTextMetrics(&stFontMetrics);

      GetTextExtentPoint32(dc.GetSafeHdc(), *pstrCurrent, pstrCurrent->GetLength(), &stSize);

      if(GetOrientation()==ORIENTATION_VERTICAL_LEFT) 
		{
			dc.MoveTo(rcBounds.left, rcBounds.Height());
			dc.SetTextAlign( TA_UPDATECP);
		}
		if(GetOrientation()==ORIENTATION_VERTICAL_RIGHT)
		{
			dc.MoveTo(rcBounds.Width(),rcBounds.top);
			dc.SetTextAlign( TA_UPDATECP);
		}
		dc.DrawText(*pstrCurrent, rcBounds, DT_LEFT);
		
		if(GetOrientation()==ORIENTATION_HORIZONTAL) 
				rcBounds.left += stSize.cx + stFontMetrics.tmOverhang;
		if(GetOrientation()==ORIENTATION_VERTICAL_LEFT) 
				rcBounds.bottom -= stSize.cx + stFontMetrics.tmOverhang;
		if(GetOrientation()==ORIENTATION_VERTICAL_RIGHT) 
				rcBounds.top += stSize.cx + stFontMetrics.tmOverhang;

	  m_nTextLength += stSize.cx + stFontMetrics.tmOverhang;
   }
}

void CBannerStatic::OnSize(UINT nType, int cx, int cy)
{
   CMultiColorStatic::OnSize(nType, cx, cy);
   CalculateScrollParameters();
   Invalidate();
}