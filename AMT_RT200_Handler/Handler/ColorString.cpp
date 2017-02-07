#include "StdAfx.h"
#include "ColorString.h"

CColorString::CColorString()
{
	SetStyle();
   SetBackColor();
}

CColorString::CColorString(const CColorString& strRight)
{
   *this = strRight;
}

CColorString::CColorString(LPCTSTR lpszData, const DWORD& dwStyle /* = 0x00000000 */,
                           const COLORREF& crBackColor /* = 0xFFFFFFFF */)
{
	CString strTmp;
	SetStyle(dwStyle);
	SetBackColor(crBackColor);

	strTmp = lpszData;
//	for (UINT i = 0; i < strlen(lpszData); i++)
	for (UINT i = 0; i < (UINT)strTmp.GetLength(); i++)
	{
		Insert(i, lpszData[i]);
	}
}

CColorString::CColorString(const CString& strData, const DWORD& dwStyle /* = 0x00000000 */,
                           const COLORREF& crBackColor /* = 0xFFFFFFFF */)
{
	SetStyle(dwStyle);
   SetBackColor(crBackColor);

	for (int i = 0; i < strData.GetLength(); i++)
	{
		Insert(i, strData.GetAt(i));
	}
}

void CColorString::SetStyle(const DWORD& dwStyle /*= 0x00000000*/)
{
	m_dwTextStyle = dwStyle;
}

DWORD CColorString::GetStyle(void) const
{
	return (m_dwTextStyle);
}

void CColorString::SetColor(const COLORREF& crColor /* = RGB(0, 0, 0) */)
{
	m_dwTextStyle = ((m_dwTextStyle & 0xFF000000) | crColor);
}

COLORREF CColorString::GetColor(void) const
{
	return (m_dwTextStyle & 0x00FFFFFF);
}

void CColorString::SetBold(const BOOL& fSetToBold /* = TRUE */)
{
	if (fSetToBold)
	{
		m_dwTextStyle |= COLORSTRING_BOLD;
	}
	else
	{
		if (GetBold())
		{
			m_dwTextStyle -= COLORSTRING_BOLD;
		}
	}
}

BOOL CColorString::GetBold(void) const
{
	return ((m_dwTextStyle & COLORSTRING_BOLD) == COLORSTRING_BOLD);
}

void CColorString::SetUnderlined(const BOOL& fSetToUnderlined /* = TRUE */)
{
	if (fSetToUnderlined)
	{
		m_dwTextStyle |= COLORSTRING_UNDERLINE;
	}
	else
	{
		if (GetUnderlined())
		{
			m_dwTextStyle -= COLORSTRING_UNDERLINE;
		}
	}
}

BOOL CColorString::GetUnderlined(void) const
{
	return ((m_dwTextStyle & COLORSTRING_UNDERLINE) == COLORSTRING_UNDERLINE);
}

BOOL CColorString::GetItalic(void) const
{
	return ((m_dwTextStyle & COLORSTRING_ITALIC) == COLORSTRING_ITALIC);
}

void CColorString::SetItalic(const BOOL& fSetToItalic /* = TRUE*/)
{
	if (fSetToItalic)
	{
		m_dwTextStyle |= COLORSTRING_ITALIC;
	}
	else
	{
		if (GetItalic())
		{
			m_dwTextStyle -= COLORSTRING_ITALIC;
		}
	}
}

void CColorString::SetBackColor(const COLORREF& crBackColor /* = 0xFFFFFFFF */)
{
   m_crBackColor = (crBackColor == 0xFFFFFFFF ? ::GetSysColor(COLOR_BTNFACE) : crBackColor);
}

COLORREF CColorString::GetBackColor(void) const
{
   return (m_crBackColor);
}

CColorString& CColorString::operator=(const CColorString& strRight)
{
	Empty();

	m_dwTextStyle = strRight.m_dwTextStyle;
   m_crBackColor = strRight.m_crBackColor;
	
	for (int i = 0; i < strRight.GetLength(); i++)
	{
		Insert(i, strRight.GetAt(i));
	}

	return (*this);
}
