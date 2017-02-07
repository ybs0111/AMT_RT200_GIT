#ifndef BANNERSTATIC_H_
#define BANNERSTATIC_H_

#pragma warning( disable : 4100 )
//-------------------------------------------
// instead of using normal timers, we're 
// going to use multimedia timers
//
#pragma comment(lib, "winmm.lib")

// 규이리 수정 [2012.02.02]
//#include "StdAfx.h"
//#include "..\\stdafx.h"
#include "MultiColorStatic.h"
#include "Mmsystem.h"

class CBannerStatic : public CMultiColorStatic
{
public:     // construction
   CBannerStatic();
   virtual ~CBannerStatic();
   
private:
   CBannerStatic(const CBannerStatic& cSource);
   CBannerStatic& operator=(const CBannerStatic& cRight);

public:
   void SetScrollSpeed(const int& nSpeed);
   int GetScrollSpeed(void) const;
   void SetWrapText(const BOOL& fWrapText);
   BOOL GetWrapText(void) const;

protected:
   void ScrollBanner(void);
   static void CALLBACK TimerProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
   
   void SetScrollSize(const int& nScrollSize);
   int  GetScrollSize(void) const;
   void SetScrollDelay(const DWORD& dwScrollDelay);
   DWORD GetScrollDelay(void) const;
   void CalculateScrollParameters(void);

private:
   int   m_nBannerSpeed;   // -MAXSPEED <= m_nBannerSpeed <= MAXSPEED
   BOOL  m_fWrapText;      // should text wrap?
   UINT  m_tmScroll;       // timer identifier
   int   m_nTextOut;       // pixel to begin drawing text
   int   m_nTextLength;    // when text scrolls out of client rect, start over from the right
   int   m_nScrollSize;    // amount of pixels to move when scrolling
   DWORD m_dwScrollDelay;  // delay, in ms, for scrolling -- indirectly set by SetScrollSpeed

   static const int MAXSPEED;             // maxiumum speed the client is allowed
   static const int MAXSPEED_MODIFIER;    // used for determining scroll parameters
   static const int TIMERRESOLUTION;      // default for timeSetEvent function
   static const int STEPHEIGHT;           // when should we scroll by more than one pixel?

protected:
   virtual void PreSubclassWindow(void);

   afx_msg void OnTimer(UINT nIDEvent);
   afx_msg void OnPaint(void);
   afx_msg void OnSize(UINT nType, int cx, int cy);
   
   DECLARE_MESSAGE_MAP()
};

#endif