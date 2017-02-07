#ifndef MULTICOLORSTATIC_H_
#define MULTICOLORSTATIC_H_

#define ORIENTATION_HORIZONTAL		0
#define ORIENTATION_VERTICAL_LEFT	1
#define ORIENTATION_VERTICAL_RIGHT	2

#include "ColorString.h"

class CMultiColorStatic : public CStatic
{
public:
   CMultiColorStatic();
   virtual ~CMultiColorStatic();

public:
  	virtual BOOL SetString(const int& nIndex, const CColorString& strData);
	virtual CColorString GetString(const int& nIndex = 0) const;
	virtual BOOL AddString(const CColorString& strData);
	virtual BOOL AddString(LPCTSTR lpszText, const DWORD& dwStyle = 0x00000000,
                          const COLORREF& crBackColor = 0xFFFFFFFF);
	virtual BOOL RemoveString(const int& nIndex);
	virtual void RemoveAllStrings(void);
	DWORD GetNumStrings(void) const;

	void SetBackColor(const COLORREF& crBackColor = 0xFFFFFFFF);
	COLORREF GetBackColor(void) const;

	void SetAutoSize(const bool& fAutoSize);
	bool GetAutoSize(void) const;

   	void SetOrientation(int iOrientation);
	int GetOrientation(void);

   void SetWindowText(LPCTSTR lpszValue);
   int GetWindowText(LPTSTR lpszStringBuf, int nMaxCount);
   void GetWindowText(CString& strData);

protected:
   void DetermineFont(const CColorString* const pstrData);
   void CalculateWindowText(void);

protected:
	CPtrArray   m_astrData;       // array of CColorString's to paint
	COLORREF    m_crBackColor;    // background color -- used for ALL strings
	CBrush      m_brBackGround;   // background brush -- used for ALL strings
	CFont       m_ftText;         // used for all strings for now
	CRect       m_rcBounds;       // client area -- update on size
	CString     m_strWindowText;  // text for the entire window
	bool        m_fAutoSize;      // should the text height adjust itself to client height?
	int			m_iOrientation;	  // line direction -- used for ALL strings							

protected:
   virtual void PreSubclassWindow(void);

   afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

   DECLARE_MESSAGE_MAP()
};

#endif