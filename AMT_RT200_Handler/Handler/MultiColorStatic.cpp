#include "StdAfx.h"
#include "MultiColorStatic.h"

//------------------------------------------------------------------------------
// CMultiColorStatic::CMultiColorStatic
// 
//   This is the standard constructor 
// 
//   Access: public
// 
//   Args:
//      none
// 
//   Return:
//      none
// 
CMultiColorStatic::CMultiColorStatic()
{
   SetBackColor();
   m_astrData.RemoveAll();
   m_fAutoSize = true;
   m_iOrientation = ORIENTATION_HORIZONTAL;
}

//------------------------------------------------------------------------------
// CMultiColorStatic::~CMultiColorStatic
// 
//   This is the destructor.
// 
//   Access: protected
// 
//   Args:
//      none
// 
//   Return:
//      none
// 
CMultiColorStatic::~CMultiColorStatic()
{
   RemoveAllStrings();
}

//------------------------------------------------------------------------------
// CMultiColorStatic::SetString
// 
//   This function sets a string after validating that the given index was 
//    correct. 
// 
//   Access: public
// 
//   Args:
//      const int& nIndex             =   index of the string to set
//      const CColorString& strData   =   string to add
// 
//   Return:
//      BOOL    =   TRUE if the setting was successful, FALSE otherwise
// 
BOOL CMultiColorStatic::SetString(const int& nIndex, const CColorString& strData)
{
	CObject* pNewOne;
	BOOL     fStringRemoved = FALSE;

	if ((nIndex < 0) || (nIndex > m_astrData.GetSize()))
	{
		ASSERT(FALSE);
		return (FALSE);
	}

   //-------------------------------------------------
   // who believes in strings of 0 length????
   //
   if (strData.GetLength() == 0)
   {
      return (FALSE);
   }

   //--------------------------------------------------
   // if we're setting a previously-allocated string,
   // then we need to delete the old one.
   //
   if (nIndex < m_astrData.GetSize())
   {
      RemoveString(nIndex);
      fStringRemoved = TRUE;
   }

	pNewOne = reinterpret_cast<CObject*>(new CColorString(strData));

	if (pNewOne)
	{
      if (fStringRemoved)
      {
		   m_astrData.InsertAt(nIndex, pNewOne);
      }
      else
      {
         m_astrData.SetAtGrow(nIndex, pNewOne);
      }
      Invalidate();
		return (TRUE);
	}

	return (FALSE);
}

//------------------------------------------------------------------------------
// CMultiColorStatic::GetString
// 
//   This function returns the string referenced by nIndex. 
// 
//   Access: public
// 
//   Args:
//      const int& nIndex /* = 0 */   =   index of string
// 
//   Return:
//      CColorString    =   string at nIndex
// 
CColorString CMultiColorStatic::GetString(const int& nIndex /* = 0 */) const
{
	CColorString strTemp;		// for returning

	if ((nIndex < 0) || (nIndex > m_astrData.GetUpperBound()))
	{
		ASSERT(FALSE);
		return (FALSE);
	}

	strTemp = *(reinterpret_cast<CColorString*>(m_astrData.GetAt(nIndex)));

	return (strTemp);
}

//------------------------------------------------------------------------------
// CMultiColorStatic::AddString
// 
//   This function adds strData to the end of the array of strings.
// 
//   Access: public
// 
//   Args:
//      const CColorString& strData   =   string to add
// 
//   Return:
//      BOOL    =   TRUE if the addition was successful, FALSE otherwise
// 
BOOL CMultiColorStatic::AddString(const CColorString& strData)
{
   return (SetString((int)m_astrData.GetSize(), strData));
}

//------------------------------------------------------------------------------
// CMultiColorStatic::AddString
// 
//   This is the overloaded function that adds a string and its properties
//    to the end of the array of strings
// 
//   Access: public
// 
//   Args:
//      LPCTSTR  lpszText     =  text to add
//      DWORD    dwStyle      =  high byte is the style -- low three bytes is colorref
//      COLORREF crBackColor  =  background color for the string
// 
//   Return:
//      BOOL    =   TRUE if the addition was successful, FALSE otherwise
// 
BOOL CMultiColorStatic::AddString(LPCTSTR lpszText, const DWORD& dwStyle /* = 0x00000000*/,
                              const COLORREF& crBackColor /* = 0xFFFFFFFF */)
{
   return (AddString(CColorString(lpszText, dwStyle, crBackColor)));
}

//------------------------------------------------------------------------------
// CMultiColorStatic::RemoveString
// 
//   This function removes a string at an index. 
// 
//   Access: public
// 
//   Args:
//      const int& nIndex   =   index of the string to remove
// 
//   Return:
//      BOOL    =   TRUE on success [string there], FALSE on failure[no string at nIndex]
// 
BOOL CMultiColorStatic::RemoveString(const int& nIndex)
{
   //-----------------------------------------------------
   // make sure they pass in a valid index
   //
   if ((nIndex < 0) || (nIndex > m_astrData.GetUpperBound()))
   {
      ASSERT(FALSE);    // an invalid index was passed in
      return (FALSE);
   }

   CColorString* pstrCurrent = reinterpret_cast<CColorString*>(m_astrData.GetAt(nIndex));
   if (pstrCurrent)
   {
      m_astrData.RemoveAt(nIndex);
      delete pstrCurrent;
   }

   //--------------------------------------
   // the destructor calls this indirectly
   // it's not a bad idea to check anyway
   // i guess
   //
   if (::IsWindow(m_hWnd))
   {
      Invalidate();
   }

   return (TRUE);
}

//------------------------------------------------------------------------------
// CMultiColorStatic::RemoveAllStrings
// 
//   This function removes all strings in the control. 
// 
//   Access: public
// 
//   Args:
//      none
// 
//   Return:
//      none
// 
void CMultiColorStatic::RemoveAllStrings(void)
{
  	while (m_astrData.GetSize() > 0)
	{
      RemoveString((int)m_astrData.GetUpperBound());
	}
   
   //------------------------------------------
   // this isn't really needed; i just do it
   // to make sure
   //
   if (::IsWindow(m_hWnd))
   {
      Invalidate();
   }
}

//------------------------------------------------------------------------------
// CMultiColorStatic::GetNumStrings
// 
//   This function returns the number of strings in the control. 
// 
//   Access: public
// 
//   Args:
//      none
// 
//   Return:
//      DWORD    =   number of strings in the control
// 
DWORD CMultiColorStatic::GetNumStrings(void) const
{
   return (DWORD)(m_astrData.GetSize());
}

//------------------------------------------------------------------------------
// CMultiColorStatic::SetBackColor
// 
//   This function sets the default back color of the control. Of course, 
//    the strings contained within can override this with their own 
//    background color. 
// 
//   Access: public
// 
//   Args:
//      const COLORREF& crBackColor /* = 0xFFFFFFFF */   =   backcolor
// 
//   Return:
//      none
// 
void CMultiColorStatic::SetBackColor(const COLORREF& crBackColor /* = 0xFFFFFFFF */)
{
	if (crBackColor == 0xFFFFFFFF)
	{
		m_crBackColor = ::GetSysColor(COLOR_BTNFACE);
	}
	else
	{
		m_crBackColor = crBackColor;
	}

	m_brBackGround.DeleteObject();
	m_brBackGround.CreateSolidBrush(m_crBackColor);

   if (::IsWindow(m_hWnd))
   {
      Invalidate();
   }
}

//------------------------------------------------------------------------------
// CMultiColorStatic::GetBackColor
// 
//   This function returns the background color of the entire control. 
// 
//   Access: public
// 
//   Args:
//      none
// 
//   Return:
//      COLORREF    =   back color
// 
COLORREF CMultiColorStatic::GetBackColor(void) const
{
	return (m_crBackColor);
}

//------------------------------------------------------------------------------
// CMultiColorStatic::GetWindowText
// 
//   This function returns the window text as if there were no color-data. 
// 
//   Access: public
// 
//   Args:
//      LPTSTR lpszBuffer   =   buffer to accept the window text
//      int nMaxCount       =   amount of bytes lpszBuffer contains
// 
//   Return:
//      int    =   number of characters copied
// 
int CMultiColorStatic::GetWindowText(LPTSTR lpszBuffer, int nMaxCount)
{
   memset(lpszBuffer, 0, nMaxCount);
   CalculateWindowText();
 //  strncpy(lpszBuffer, (LPTSTR)m_strWindowText, nMaxCount);

   m_strWindowText = lpszBuffer;
   if (m_strWindowText.GetLength() == nMaxCount)
   {
      lpszBuffer[nMaxCount] = 0;
   }

   return (m_strWindowText.GetLength());
}

//------------------------------------------------------------------------------
// CMultiColorStatic::GetWindowText
// 
//   This function returns the window text as if there were no color-data 
// 
//   Access: public
// 
//   Args:
//      CString& strData   =   CString to accept the data
// 
//   Return:
//      none
// 
void CMultiColorStatic::GetWindowText(CString& strData)
{
   CalculateWindowText();
   strData = m_strWindowText;
   return;
}

//------------------------------------------------------------------------------
// CMultiColorStatic::SetWindowText
// 
//   This function sets the window text; the text will have a normal font. 
// 
//   Access: public
// 
//   Args:
//      LPCTSTR lpszValue   =   text to set the control to
// 
//   Return:
//      none
// 
void CMultiColorStatic::SetWindowText(LPCTSTR lpszValue)
{
   RemoveAllStrings();
   AddString(lpszValue);
}

//------------------------------------------------------------------------------
// CMultiColorStatic::SetAutoSize
// 
//   When the banner-size gets large, the text will increase in size along 
//    with it. Otherwise the text will be teeny tiny. 
// 
//   Access: public
// 
//   Args:
//      const bool& fAutoSize   =   should text increase in size?
// 
//   Return:
//      none
// 
void CMultiColorStatic::SetAutoSize(const bool& fAutoSize)
{
   m_fAutoSize = fAutoSize;
}

//------------------------------------------------------------------------------
// CMultiColorStatic::GetAutoSize
// 
//   This returns the AutoSize flag. 
// 
//   Access: public
// 
//   Args:
//      none
// 
//   Return:
//      bool    =   should text size increase in size?
// 
bool CMultiColorStatic::GetAutoSize(void) const
{
   return (m_fAutoSize);
}

//------------------------------------------------------------------------------
// CMultiColorStatic::DetermineFont
// 
//   This function determines the font based on a the value in pstrData. 
// 
//   Access: protected
// 
//   Args:
//      const CColorString* const pstrData   =   string from which a font gets build
// 
//   Return:
//      none
// 
void CMultiColorStatic::DetermineFont(const CColorString* const pstrData)
{
   LOGFONT stFont;

   m_ftText.DeleteObject();

   //---------------------------------------------------
   // set up the font based on pstrData
   //
   /*if (!GetFont()->GetLogFont(&stFont))
   {		
	   memset(&stFont, 0, sizeof(stFont));

	   stFont.lfWidth = ((stFont.lfHeight * 7) / 16);
	   stFont.lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS;
   }*/
	CFont font;	
	font.CreatePointFont(110, _T("Tahoma Bold"));
	font.GetLogFont(&stFont);
	font.DeleteObject();
	stFont.lfWidth = ((stFont.lfHeight * 12) / 16);
	stFont.lfPitchAndFamily = DEFAULT_PITCH | FF_SWISS;
	if (m_fAutoSize)
	{
		if(GetOrientation()==ORIENTATION_HORIZONTAL) stFont.lfHeight = m_rcBounds.Height();
		if(GetOrientation()==ORIENTATION_VERTICAL_LEFT) stFont.lfHeight = m_rcBounds.Width();
		if(GetOrientation()==ORIENTATION_VERTICAL_RIGHT) stFont.lfHeight = m_rcBounds.Width();
	}
	
	if(GetOrientation()==ORIENTATION_HORIZONTAL) 
		stFont.lfEscapement = stFont.lfOrientation = 0;
	if(GetOrientation()==ORIENTATION_VERTICAL_LEFT) 
		stFont.lfEscapement = stFont.lfOrientation = 900;
	if(GetOrientation()==ORIENTATION_VERTICAL_RIGHT) 
		stFont.lfEscapement = stFont.lfOrientation = -900;
	stFont.lfWeight = (pstrData->GetBold() ? FW_HEAVY : FW_NORMAL);
	stFont.lfUnderline = (BYTE) pstrData->GetUnderlined();
	stFont.lfItalic = (BYTE) pstrData->GetItalic();
	stFont.lfQuality = PROOF_QUALITY;
	m_ftText.CreateFontIndirect(&stFont);
}

//------------------------------------------------------------------------------
// CMultiColorStatic::CalculateWindowText
// 
//   This function calculates the window text based on all strings int he 
//    control. 
//   It just gets each string's value and adds it to a list.
// 
//   Access: protected
// 
//   Args:
//      none
// 
//   Return:
//      none
// 
void CMultiColorStatic::CalculateWindowText(void)
{
   m_strWindowText.Empty();

   for (int i = 0; i < m_astrData.GetSize(); i++)
   {
      CColorString* pCurrent = (CColorString*)m_astrData.GetAt(i);
      if (pCurrent)
      {
         m_strWindowText += *pCurrent;
      }
   }
}

void CMultiColorStatic::PreSubclassWindow(void)
{
   GetClientRect(m_rcBounds);

   CDC* pDC = GetDC();
   pDC->FillRect(m_rcBounds, &m_brBackGround);
   ReleaseDC(pDC);

   //---------------------------------------
   // derived classes should be able to 
   // notify parent of mouse input
   //
   ModifyStyle(0, SS_NOTIFY);
}

BEGIN_MESSAGE_MAP(CMultiColorStatic, CStatic)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// we're just painting the strings here
void CMultiColorStatic::OnPaint() 
{
   CPaintDC dc(this);
      
   CRect rcBounds = m_rcBounds;

   dc.FillRect(m_rcBounds, &m_brBackGround);

   //------------------------------------------------------
   // draw each string with its own font
   //
   for (int i = 0; i < m_astrData.GetSize(); i++)
   {
      CColorString* pstrCurrent = reinterpret_cast<CColorString*>(m_astrData.GetAt(i));
      TEXTMETRIC    stFontMetrics;
      SIZE          stSize;

      DetermineFont(pstrCurrent);

		//---------------------------------------------------
		// set up the drawing attributes
		//
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
				
		//---------------------------------------------------
		// do the drawing -- DrawText won't let us
		// go outside our client-area
		//			
      	if(GetOrientation()==ORIENTATION_VERTICAL_LEFT) 
		{
			dc.MoveTo(rcBounds.left,rcBounds.Height());
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
   }
}

// we need to keep track of our client rect; i hate recalculating when i need it
void CMultiColorStatic::OnSize(UINT nType, int cx, int cy) 
{
	CStatic::OnSize(nType, cx, cy);
	GetClientRect(m_rcBounds);
}

// make sure we clean up
void CMultiColorStatic::OnDestroy() 
{
	CStatic::OnDestroy();
   m_ftText.DeleteObject();
   m_brBackGround.DeleteObject();
}

// we're taking care of this with fillrect; forget about it here
BOOL CMultiColorStatic::OnEraseBkgnd(CDC* pDC) 
{	
   return (TRUE);
}

int CMultiColorStatic::GetOrientation()
{
	return m_iOrientation;
}

void CMultiColorStatic::SetOrientation(int iOrientation)
{
	m_iOrientation = iOrientation;
	Invalidate();
}

