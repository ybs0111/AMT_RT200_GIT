//------------------------------------------------------------------------------
// ColorString.h
// 
//	This is a string, complete with color information.
//   This class does not actually do any drawing; it's
//   just designed to be an information repository.
//	 
//		Copyright (c) 2000 Paul Wendt
// 
//		VERSION#	DATE			NAME	DESCRIPTION OF CHANGE
//		--------	----------	----	---------------------
//		1.01  	05/15/2000	PRW	Original creation.
// 
#ifndef COLORSTRING_H_
#define COLORSTRING_H_

#define COLORSTRING_BOLD			0x01000000
#define COLORSTRING_ITALIC			0x02000000
#define COLORSTRING_UNDERLINE		0x04000000
	
class CColorString : public CString
{
public:
	CColorString();
   CColorString(const CColorString& strRight);
	CColorString(LPCTSTR, const DWORD& dwStyle = 0x00000000,
                const COLORREF& crBackColor = 0xFFFFFFFF);

	CColorString(const CString& strData, const DWORD& dwStyle = 0x00000000,
                const COLORREF& crBackColor = 0xFFFFFFFF);

public:
	void SetStyle(const DWORD& dwStyle = 0x00000000);
	DWORD GetStyle(void) const;

	void SetColor(const COLORREF& crColor = RGB(0, 0, 0));
	COLORREF GetColor(void) const;

	void SetBold(const BOOL& fSetToBold = TRUE);
	BOOL GetBold(void) const;

	void SetUnderlined(const BOOL& fSetToUnderlined = TRUE);
	BOOL GetUnderlined(void) const;

	void SetItalic(const BOOL& fSetToItalic = TRUE);
	BOOL GetItalic(void) const;
	
	void SetBackColor(const COLORREF& crBackColor = 0xFFFFFFFF);
	COLORREF GetBackColor(void) const;

public:
	CColorString& operator=(const CColorString& strRight);
	
private:
	DWORD    m_dwTextStyle;
	COLORREF m_crBackColor;
};


#endif // COLORSTRING_H_