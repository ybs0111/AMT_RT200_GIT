#include "stdafx.h"
#include "ClockST.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define OFSX_H1  5
#define OFSY_H1  33

#define OFSX_H2  32
#define OFSY_H2  33

#define OFSX_M1  65
#define OFSY_M1  33

#define OFSX_M2  92
#define OFSY_M2  33

#define OFSX_S1  120
#define OFSY_S1  50

#define OFSX_S2  134
#define OFSY_S2  50

#define OFSX_MDAY1  105
#define OFSY_MDAY1  5

#define OFSX_MDAY2  119
#define OFSY_MDAY2  5

#define OFSX_MON1  69
#define OFSY_MON1  5

#define OFSX_MON2  83
#define OFSY_MON2  5

#define OFSX_YEAR1  5
#define OFSY_YEAR1  5
#define OFSX_YEAR2  19
#define OFSY_YEAR2  5
#define OFSX_YEAR3  33
#define OFSY_YEAR3  5
#define OFSX_YEAR4  47
#define OFSY_YEAR4  5

#define WIDTH_BIG		24
#define HEIGHT_BIG		34

#define WIDTH_SMALL		12
#define HEIGHT_SMALL	17

// m_idxDigit positions
//  0 hour-left
//  1 hour-right
//  2 min-left
//  3 min-right
//  4 sec-left
//  5 sec-right
//  6 day-left
//  7 day-right
//  8 month-left
//  9 month-right
// 10 century-left
// 11 century-right
// 12 year-left
// 13 year-right

CClockST::CClockST()
{
	m_bAlternateDateFormat = FALSE;		// Use occidental date format dd-mm-yyyy
	::ZeroMemory(&m_idxDigit, sizeof(m_idxDigit));

	/*
	m_cdcBufClock.bValid = FALSE;
	m_cdcBufBig.bValid = FALSE;
	m_cdcBufSmall.bValid = FALSE;
	*/
}

CClockST::~CClockST()
{
	if (m_thrParam.hThrHandle)
	{
		m_thrParam.bRun = FALSE;
		::WaitForSingleObject(m_thrParam.hThrHandle, 1500);
	}

	if (m_cdcBufClock.bValid == TRUE)
	{
		//m_cdcBufClock.dcMemory.SelectObject(m_cdcBufClock.pOldBmp);
		m_cdcBufClock.bValid = FALSE;
	}
	if (m_cdcBufBig.bValid == TRUE)
	{
		//m_cdcBufBig.dcMemory.SelectObject(m_cdcBufBig.pOldBmp);
		m_cdcBufBig.bValid = FALSE;
	}
	if (m_cdcBufSmall.bValid == TRUE)
	{
		//m_cdcBufSmall.dcMemory.SelectObject(m_cdcBufSmall.pOldBmp);
		m_cdcBufSmall.bValid = FALSE;
	}

	Clean();
}

BEGIN_MESSAGE_MAP(CClockST, CStatic)
	//{{AFX_MSG_MAP(CClockST)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// Detach previous resources.
//
void CClockST::Clean()
{
	m_bmClock.DeleteObject();
	m_bmBig.Detach();
	m_bmSmall.Detach();
	m_palPalette.DeleteObject();  //###
} // End of Clean

BOOL CClockST::GetBitmapAndPalette(UINT nIDResource, CBitmap& bitmap, CPalette& pal)
{
	HINSTANCE	hInstResource	= NULL;

	// Find correct resource handle
	//hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nIDResource), RT_GROUP_BITMAP);
	hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nIDResource), RT_BITMAP);  

	HBITMAP hBmp = (HBITMAP)::LoadImage( hInstResource, 
		MAKEINTRESOURCE(nIDResource), IMAGE_BITMAP, 0,0, LR_CREATEDIBSECTION);

	if (hBmp == NULL) return FALSE;

	bitmap.Attach(hBmp);

	// Create a logical palette for the bitmap
	DIBSECTION ds;
	BITMAPINFOHEADER &bmInfo = ds.dsBmih;
	bitmap.GetObject(sizeof(ds), &ds);

	int nColors = bmInfo.biClrUsed ? bmInfo.biClrUsed : 1 << bmInfo.biBitCount;

	// Create a halftone palette if colors > 256. 
	CClientDC dc(NULL); // Desktop DC

	if(nColors > 256)
		pal.CreateHalftonePalette(&dc);
	else
	{
		// Create the palette
		RGBQUAD *pRGB = new RGBQUAD[nColors];
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);
		memDC.SelectObject( &bitmap );
		::GetDIBColorTable( memDC, 0, nColors, pRGB );
		UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * nColors);
		LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];
		pLP->palVersion = 0x300;
		pLP->palNumEntries = (WORD) nColors;
		for (int i=0; i < nColors; i++)
		{
			pLP->palPalEntry[i].peRed = pRGB[i].rgbRed;
			pLP->palPalEntry[i].peGreen = pRGB[i].rgbGreen;
			pLP->palPalEntry[i].peBlue = pRGB[i].rgbBlue;
			pLP->palPalEntry[i].peFlags = 0;
		}
		pal.CreatePalette( pLP );
		delete[] pLP;
		delete[] pRGB;
	}
	return TRUE;
} // End of GetBitmapAndPalette

void CClockST::Refresh()
{
	char szDate[15], szTime[15];
	SYSTEMTIME	csTime;

	// Get current date and time
	::GetLocalTime(&csTime);

//	if (m_bAlternateDateFormat == TRUE)
//		sprintf(szDate, "%02d-%02d-%04d", csTime.wMonth, csTime.wDay, csTime.wYear);
//	else
//		sprintf(szDate, "%02d-%02d-%04d", csTime.wDay, csTime.wMonth, csTime.wYear);

	sprintf_s(szDate, "%04d-%02d-%02d", csTime.wYear, csTime.wMonth, csTime.wDay);

	sprintf_s(szTime, "%02d:%02d:%02d", csTime.wHour, csTime.wMinute, csTime.wSecond);

	// Set year
	m_idxDigit[6] = szDate[0] - 48;
	m_idxDigit[7] = szDate[1] - 48;
	m_idxDigit[8] = szDate[2] - 48;
	m_idxDigit[9] = szDate[3] - 48;
		
	// Set month
	m_idxDigit[10] = szDate[5] - 48;
	m_idxDigit[11] = szDate[6] - 48;

	// Set day
	m_idxDigit[12] = szDate[8] - 48;
	m_idxDigit[13] = szDate[9] - 48;

	// Set day
//	m_idxDigit[6] = szDate[0] - 48;
//	m_idxDigit[7] = szDate[1] - 48;
		
	// Set month
//	m_idxDigit[8] = szDate[3] - 48;
//	m_idxDigit[9] = szDate[4] - 48;

	// Set year
//	m_idxDigit[10] = szDate[6] - 48;
//	m_idxDigit[11] = szDate[7] - 48;
//	m_idxDigit[12] = szDate[8] - 48;
//	m_idxDigit[13] = szDate[9] - 48;

	// Set hour
	m_idxDigit[0] = szTime[0] - 48;
	m_idxDigit[1] = szTime[1] - 48;

	// Set minutes
	m_idxDigit[2] = szTime[3] - 48;
	m_idxDigit[3] = szTime[4] - 48;

	// Set seconds
	m_idxDigit[4] = szTime[6] - 48;
	m_idxDigit[5] = szTime[7] - 48;

	if (::IsWindow(m_hWnd)) Invalidate();
//	SetWindowText(szTime);
} // End of Refresh

BOOL CClockST::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
} // End of OnEraseBkgnd

void CClockST::OnPaint() 
{
	// If not started
	if (m_thrParam.hThrHandle == NULL) 
	{
		// Show as a normal static control
		CStatic::OnPaint();
		return;
	} // if

	CPaintDC dc(this); // device context for painting

	if (m_cdcBufClock.bValid == FALSE)
	{
		m_cdcBufClock.dcMemory.CreateCompatibleDC(&dc);
    	m_cdcBufClock.pOldBmp = m_cdcBufClock.dcMemory.SelectObject(&m_bmClock);
		m_cdcBufClock.bValid = TRUE;
	} // if
	if (m_cdcBufBig.bValid == FALSE)
	{
		m_cdcBufBig.dcMemory.CreateCompatibleDC(&dc);
		m_cdcBufBig.pOldBmp = m_cdcBufBig.dcMemory.SelectObject(&m_bmBig);
		m_cdcBufBig.bValid = TRUE;
	} // if
	if (m_cdcBufSmall.bValid == FALSE)
	{
		m_cdcBufSmall.dcMemory.CreateCompatibleDC(&dc);
		m_cdcBufSmall.pOldBmp = m_cdcBufSmall.dcMemory.SelectObject(&m_bmSmall);
		m_cdcBufSmall.bValid = TRUE;
	} // if

	//dc.SelectPalette(&m_palPalette, FALSE);
	//dc.RealizePalette();

	// Update hour
	m_cdcBufClock.dcMemory.BitBlt(OFSX_H1, OFSY_H1, WIDTH_BIG, HEIGHT_BIG, &m_cdcBufBig.dcMemory, WIDTH_BIG*m_idxDigit[0], 0, SRCCOPY);
    m_cdcBufClock.dcMemory.BitBlt(OFSX_H2, OFSY_H2, WIDTH_BIG, HEIGHT_BIG, &m_cdcBufBig.dcMemory, WIDTH_BIG*m_idxDigit[1], 0, SRCCOPY);
	// Update minutes
	m_cdcBufClock.dcMemory.BitBlt(OFSX_M1, OFSY_M1, WIDTH_BIG, HEIGHT_BIG, &m_cdcBufBig.dcMemory, WIDTH_BIG*m_idxDigit[2], 0, SRCCOPY);
	m_cdcBufClock.dcMemory.BitBlt(OFSX_M2, OFSY_M2, WIDTH_BIG, HEIGHT_BIG, &m_cdcBufBig.dcMemory, WIDTH_BIG*m_idxDigit[3], 0, SRCCOPY);
	// Update seconds
	m_cdcBufClock.dcMemory.BitBlt(OFSX_S1, OFSY_S1, WIDTH_SMALL, HEIGHT_SMALL, &m_cdcBufSmall.dcMemory, WIDTH_SMALL*m_idxDigit[4], 0, SRCCOPY);
	m_cdcBufClock.dcMemory.BitBlt(OFSX_S2, OFSY_S2, WIDTH_SMALL, HEIGHT_SMALL, &m_cdcBufSmall.dcMemory, WIDTH_SMALL*m_idxDigit[5], 0, SRCCOPY);

	// Update year
	m_cdcBufClock.dcMemory.BitBlt(OFSX_YEAR1, OFSY_YEAR1, WIDTH_SMALL, HEIGHT_SMALL, &m_cdcBufSmall.dcMemory, WIDTH_SMALL*m_idxDigit[6], 0, SRCCOPY);
	m_cdcBufClock.dcMemory.BitBlt(OFSX_YEAR2, OFSY_YEAR2, WIDTH_SMALL, HEIGHT_SMALL, &m_cdcBufSmall.dcMemory, WIDTH_SMALL*m_idxDigit[7], 0, SRCCOPY);
	m_cdcBufClock.dcMemory.BitBlt(OFSX_YEAR3, OFSY_YEAR3, WIDTH_SMALL, HEIGHT_SMALL, &m_cdcBufSmall.dcMemory, WIDTH_SMALL*m_idxDigit[8], 0, SRCCOPY);
	m_cdcBufClock.dcMemory.BitBlt(OFSX_YEAR4, OFSY_YEAR4, WIDTH_SMALL, HEIGHT_SMALL, &m_cdcBufSmall.dcMemory, WIDTH_SMALL*m_idxDigit[9], 0, SRCCOPY);

	// Update month
	m_cdcBufClock.dcMemory.BitBlt(OFSX_MON1, OFSY_MON1, WIDTH_SMALL, HEIGHT_SMALL, &m_cdcBufSmall.dcMemory, WIDTH_SMALL*m_idxDigit[10], 0, SRCCOPY);
	m_cdcBufClock.dcMemory.BitBlt(OFSX_MON2, OFSY_MON2, WIDTH_SMALL, HEIGHT_SMALL, &m_cdcBufSmall.dcMemory, WIDTH_SMALL*m_idxDigit[11], 0, SRCCOPY);

	// Updated day
	m_cdcBufClock.dcMemory.BitBlt(OFSX_MDAY1, OFSY_MDAY1, WIDTH_SMALL, HEIGHT_SMALL, &m_cdcBufSmall.dcMemory, WIDTH_SMALL*m_idxDigit[12], 0, SRCCOPY);
	m_cdcBufClock.dcMemory.BitBlt(OFSX_MDAY2, OFSY_MDAY2, WIDTH_SMALL, HEIGHT_SMALL, &m_cdcBufSmall.dcMemory, WIDTH_SMALL*m_idxDigit[13], 0, SRCCOPY);

	dc.BitBlt(0, 0, m_bmInfoClock.bmWidth, m_bmInfoClock.bmHeight, &m_cdcBufClock.dcMemory, 0, 0, SRCCOPY);
	
	// Do not call CStatic::OnPaint() for painting messages
} // End of OnPaint

UINT /*WINAPI*/ CClockST::thrClock(void* pParam)
{
	STRUCT_THRCLOCK* p;
	CClockST* pParent;

	p = (STRUCT_THRCLOCK*)pParam;
	pParent = (CClockST*)p->pParent;

	TRACE(_T("thrClock: Enter \n"));

	// Main loop
	while (p->bRun == TRUE)
	{
		pParent->Refresh();

		::Sleep(1000);
	} // while

	TRACE(_T("thrClock: Exit \n"));

	return 0;
} // End of thrClock

// This function starts (and displays) the clock loading all the necessary bitmaps
// and creating the worker thread.
//
// Parameters:
//		[IN]	nPaneID
//				ID number of the bitmap resource of the whole clock.
//		[IN]	nBigID
//				ID number of the bitmap resource of the big digits.
//		[IN]	nSmallID
//				ID number of the bitmap resource of the small digits.
//		[IN]	bAlternateDateFormat
//				TRUE to display date in mm-dd-yyyy format, else
//				FALSE to display date in dd-mm-yyyy format.
//
// Return value:
//		CLOCKST_OK
//			Function executed successfully.
//		CLOCKST_INVALIDRESOURCE
//			Some error loading bitmaps.
//		CLOCKST_THREADKO
//			Failed creating worker thread.
//
DWORD CClockST::Start(int nPaneID, int nBigID, int nSmallID, BOOL bAlternateDateFormat)
{
	BOOL	bFailed = FALSE;

	// If already started
	if (m_thrParam.hThrHandle) return FALSE;

	// Detach previous resources
	Clean();

	// Load bitmaps & palette
	if (GetBitmapAndPalette(nSmallID, m_bmSmall, m_palPalette) == FALSE)	bFailed = TRUE;
	m_palPalette.DeleteObject();
	if (GetBitmapAndPalette(nBigID, m_bmBig, m_palPalette) == FALSE)		bFailed = TRUE;
	m_palPalette.DeleteObject();
	// Get palette of the whole clock
	if (GetBitmapAndPalette(nPaneID, m_bmClock, m_palPalette) == FALSE)	bFailed = TRUE;

	// If failed loading bitmaps
	if (bFailed)
	{
		Clean();
		return CLOCKST_INVALIDRESOURCE;
	} // if

	// Get dimension of the whole clock
	m_bmClock.GetObject(sizeof(m_bmInfoClock), &m_bmInfoClock);

	// Modify control rect to fit clock
	CRect rect;
	GetWindowRect(rect);

	rect.right = rect.left + m_bmInfoClock.bmWidth;
	rect.bottom = rect.top + m_bmInfoClock.bmHeight;

	SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOZORDER | SWP_NOMOVE);

	// Date format to use
	m_bAlternateDateFormat = bAlternateDateFormat;

	// Create thread
	::ZeroMemory(&m_thrParam, sizeof(m_thrParam));
	m_thrParam.pParent = this;
	m_thrParam.bRun = TRUE;

    CWinThread* cw = AfxBeginThread( &thrClock, (LPVOID)&m_thrParam );
	if (cw == NULL)
	{
		Clean();
		return CLOCKST_THREADKO;
	} // if

	m_thrParam.hThrHandle = cw->m_hThread;
	m_thrParam.dwThrId = cw->m_nThreadID;

	return CLOCKST_OK;
} // End of Start
