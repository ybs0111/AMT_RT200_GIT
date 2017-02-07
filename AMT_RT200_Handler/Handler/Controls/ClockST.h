//
//	Class:		CClockST
//
//	Compiler:	Visual C++
//	Tested on:	Visual C++ 5.0
//				Visual C++ 6.0
//
//	Version:	See GetVersionC() or GetVersionI()
//
//	Created:	15/June/1999
//	Updated:	10/July/2002
//
//	Author:		Davide Calabro'		davide_calabro@yahoo.com
//									http://www.softechsoftware.it
//
//	Disclaimer
//	----------
//	THIS SOFTWARE AND THE ACCOMPANYING FILES ARE DISTRIBUTED "AS IS" AND WITHOUT
//	ANY WARRANTIES WHETHER EXPRESSED OR IMPLIED. NO REPONSIBILITIES FOR POSSIBLE
//	DAMAGES OR EVEN FUNCTIONALITY CAN BE TAKEN. THE USER MUST ASSUME THE ENTIRE
//	RISK OF USING THIS SOFTWARE.
//
//	Terms of use
//	------------
//	THIS SOFTWARE IS FREE FOR PERSONAL USE OR FREEWARE APPLICATIONS.
//	IF YOU USE THIS SOFTWARE IN COMMERCIAL OR SHAREWARE APPLICATIONS YOU
//	ARE GENTLY ASKED TO DONATE 1$ (ONE U.S. DOLLAR) TO THE AUTHOR:
//
//		Davide Calabro'
//		P.O. Box 65
//		21019 Somma Lombardo (VA)
//		Italy
//
#ifndef _CLOCKST_H_
#define _CLOCKST_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// Return values
#ifndef	CLOCKST_OK
#define	CLOCKST_OK					0
#endif
#ifndef	CLOCKST_INVALIDRESOURCE
#define	CLOCKST_INVALIDRESOURCE		1
#endif
#ifndef	CLOCKST_THREADKO
#define	CLOCKST_THREADKO			2
#endif

class CClockST : public CStatic
{
public:
	CClockST();
	virtual ~CClockST();

	DWORD Start(int nPaneID, int nBigID, int nSmallID, BOOL bAlternateDateFormat = FALSE);

	static short GetVersionI()		{return 13;}
	static LPCTSTR GetVersionC()	{return (LPCTSTR)_T("1.3");}

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClockST)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CClockST)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

private:
	BOOL GetBitmapAndPalette(UINT nIDResource, CBitmap& bitmap, CPalette& pal);
	void Refresh();
	void Clean();

	typedef struct _STRUCT_THRCLOCK
	{
		BOOL		bRun;
		int			nStep;
		LPVOID		pParent;
		HANDLE		hThrHandle;
		DWORD		dwThrId;

		_STRUCT_THRCLOCK::_STRUCT_THRCLOCK()
		{::ZeroMemory(this, sizeof(_STRUCT_THRCLOCK));}
	} STRUCT_THRCLOCK;

	STRUCT_THRCLOCK m_thrParam;
	static UINT thrClock(LPVOID pParam);

	typedef struct _STRUCT_CDCBUFFER
	{
		BOOL		bValid;
		CDC			dcMemory;
		CBitmap*	pOldBmp;

		_STRUCT_CDCBUFFER::_STRUCT_CDCBUFFER()
		{bValid = FALSE;}
	} STRUCT_CDCBUFFER;

	STRUCT_CDCBUFFER m_cdcBufClock;
	STRUCT_CDCBUFFER m_cdcBufBig;
	STRUCT_CDCBUFFER m_cdcBufSmall;

	BYTE		m_idxDigit[14];

	CBitmap		m_bmClock;
	BITMAP		m_bmInfoClock;
	CBitmap		m_bmBig;
	CBitmap		m_bmSmall;
	CPalette	m_palPalette;

	BOOL		m_bAlternateDateFormat;

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif
