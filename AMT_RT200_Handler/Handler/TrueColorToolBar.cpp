// TrueColorToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "TrueColorToolBar.h"
#include "Variable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrueColorToolBar

CTrueColorToolBar::CTrueColorToolBar()
{
	m_bDropDown = FALSE;
}

CTrueColorToolBar::~CTrueColorToolBar()
{
}


BEGIN_MESSAGE_MAP(CTrueColorToolBar, CToolBar)
	//{{AFX_MSG_MAP(CTrueColorToolBar)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_NOTIFY_REFLECT(TBN_DROPDOWN, OnToolbarDropDown)
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
//	ON_NOTIFY_REFLECT(NM_CLICK, &CTrueColorToolBar::OnNMClick)
//	ON_NOTIFY_REFLECT(NM_LDOWN, &CTrueColorToolBar::OnNMLDown)
//	ON_NOTIFY_REFLECT(TBN_TOOLBARCHANGE, &CTrueColorToolBar::OnTbnToolbarChange)
//	ON_NOTIFY_REFLECT(NM_KEYDOWN, &CTrueColorToolBar::OnNMKeyDown)
	ON_NOTIFY_REFLECT(NM_CLICK, &CTrueColorToolBar::OnNMClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrueColorToolBar message handlers
BOOL CTrueColorToolBar::LoadTrueColorToolBar(int nBtnWidth,
											 UINT uToolBar,
											 UINT uToolBarHot,
											 UINT uToolBarDisabled)
{
	if (!SetTrueColorToolBar(TB_SETIMAGELIST, uToolBar, nBtnWidth))
		return FALSE;
	
	if (!SetTrueColorToolBar(TB_SETHOTIMAGELIST, uToolBarHot, nBtnWidth))
		return FALSE;

	if (uToolBarDisabled) {
	if (!SetTrueColorToolBar(TB_SETDISABLEDIMAGELIST, uToolBarDisabled, nBtnWidth))
		return FALSE;
	}
	return TRUE;
}


BOOL CTrueColorToolBar::SetTrueColorToolBar(UINT uToolBarType,
							     	        UINT uToolBar,
										    int nBtnWidth)
{
	CImageList	cImageList;
	CBitmap		cBitmap;
	BITMAP		bmBitmap;
	CSize		cSize;
	int			nNbBtn;
	
	if (!cBitmap.Attach(LoadImage(AfxGetInstanceHandle(),
								  MAKEINTRESOURCE(uToolBar),
								  IMAGE_BITMAP, 0, 0,
		                          LR_DEFAULTSIZE|LR_CREATEDIBSECTION)) ||
	    !cBitmap.GetBitmap(&bmBitmap))
		return FALSE;

	cSize  = CSize(bmBitmap.bmWidth, bmBitmap.bmHeight); 
	nNbBtn = cSize.cx/nBtnWidth;
	RGBTRIPLE* rgb		= (RGBTRIPLE*)(bmBitmap.bmBits);


	COLORREF   rgbMask	= RGB(rgb[0].rgbtRed,
		                      rgb[0].rgbtGreen,
							  rgb[0].rgbtBlue);

	if (!cImageList.Create(nBtnWidth, cSize.cy,
		                   ILC_COLOR24|ILC_MASK,
						   nNbBtn, 0))
		return FALSE;
//	imageList.Add(&bitmap, RGB(255,0,255)); //바탕의 분홍색이 마스크
	if (cImageList.Add(&cBitmap, rgbMask) == -1)
		return FALSE;

	SendMessage(uToolBarType, 0, (LPARAM)cImageList.m_hImageList);
	cImageList.Detach(); 
	cBitmap.Detach();
	
	return TRUE;
}

BOOL CTrueColorToolBar::LoadToolBar(UINT nIDResource)
{
	BOOL bReturn = CToolBar::LoadToolBar(nIDResource);

//	return bReturn;

    // Check if we loaded the toolbar.
    if (bReturn == FALSE) return bReturn;

     // Make it flat.
    ModifyStyle(0, GetStyle()|TBSTYLE_FLAT);

     // Set the text for each button
    CToolBarCtrl& bar = GetToolBarCtrl();

    int nIndex = 0;
    TBBUTTON    tb;

	TCHAR *p_next_token = NULL;
	TCHAR *p_token;

    for (nIndex = bar.GetButtonCount(); nIndex >= 0; nIndex--)
    {
         ZeroMemory(&tb, sizeof(TBBUTTON));
         bar.GetButton(nIndex, &tb);

         // Do we have a separator?
         if ((tb.fsStyle & TBSTYLE_SEP) == TBSTYLE_SEP) continue;

         // Have we got a valid command id?
         if (tb.idCommand == 0) continue;

         // Get the resource string if there is one.
        CString strText((LPCSTR)tb.idCommand);
         LPCTSTR lpszButtonText = NULL;

         CString strButtonText(_T(""));

		TCHAR seps[] = _T("\n");
		 p_next_token = NULL;

         if (!strText.IsEmpty())
         {
			  p_token = _tcstok_s( (TCHAR *)(const TCHAR *)strText, seps, &p_next_token );

			  while( p_token != NULL )
			  {
				  strButtonText = lpszButtonText;
				  p_token = _tcstok_s( NULL, seps, &p_next_token );
				 lpszButtonText = p_token;
			  }  
         }

         if (!strButtonText.IsEmpty()) SetButtonText(nIndex, strButtonText);
    }

   // Resize the buttons so that the text will fit.
    CRect rc(0, 0, 0, 0);
    CSize sizeMax(0, 0);

    for (nIndex = bar.GetButtonCount(); nIndex >= 0; nIndex--)
    {
        bar.GetItemRect(nIndex, rc);
        rc.NormalizeRect();
        sizeMax.cx = __max(rc.Size().cx, sizeMax.cx);
        sizeMax.cy = __max(rc.Size().cy, sizeMax.cy);
    }
    SetSizes(sizeMax, CSize(32,32));
    return bReturn;
}

BOOL CTrueColorToolBar::OnEraseBkgnd(CDC* pDC)
{
	CDC  memDC;                     // 메모리 DC
	CBitmap * pOldBitmap = NULL;    // m_BackBitmap으로 새로운 그림을 DC에 그릴 때, 이전 그려졌던 DC(즉, Bitmap)을 저장.
	BITMAP bitmapInfo;              // 그림의 정보(m_BackBitmap)
	st_handler_info.m_TitleBackBitmap.GetBitmap(&bitmapInfo); // Bitmap 크기 구함.

	memDC.CreateCompatibleDC(pDC);  // 메모리 DC 생성
	pOldBitmap = memDC.SelectObject(&st_handler_info.m_TitleBackBitmap);
	pDC->BitBlt(0, 0, bitmapInfo.bmWidth, bitmapInfo.bmHeight, &memDC, 0, 0, SRCCOPY);   // 메모리 DC에 그림을 그림

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	return true; // 흰 부러쉬가 아닌 배경을 그려야 하기 때문에 true
	//return CToolBar::OnEraseBkgnd(pDC);
}


void CTrueColorToolBar::AddDropDownButton(CWnd* pParent, UINT uButtonID, UINT uMenuID)
{
	if (!m_bDropDown) 
	{
		GetToolBarCtrl().SendMessage(TB_SETEXTENDEDSTYLE, 0, (LPARAM)TBSTYLE_EX_DRAWDDARROWS);
		m_bDropDown = TRUE;
	}

	SetButtonStyle(CommandToIndex(uButtonID), TBSTYLE_DROPDOWN);

	stDropDownInfo DropDownInfo;
	DropDownInfo.pParent	= pParent;
	DropDownInfo.uButtonID	= uButtonID;
	DropDownInfo.uMenuID	= uMenuID;
	m_lstDropDownButton.Add(DropDownInfo);
}

void CTrueColorToolBar::OnToolbarDropDown(NMHDR* pnmtb, LRESULT *plr)
{
	NMTOOLBAR* pNMToolBar = (NMTOOLBAR*)pnmtb;

	for (int i = 0; i < m_lstDropDownButton.GetSize(); i++)
	{
		
		stDropDownInfo DropDownInfo = m_lstDropDownButton.GetAt(i);

		if (DropDownInfo.uButtonID == UINT(pNMToolBar->iItem)) 
		{

			CMenu menu;
			menu.LoadMenu(DropDownInfo.uMenuID);
			CMenu* pPopup = menu.GetSubMenu(0);
			
			CRect rc;
			SendMessage(TB_GETRECT, (WPARAM)pNMToolBar->iItem, (LPARAM)&rc);
			ClientToScreen(&rc);
			
			pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_VERTICAL,
				                   rc.left, rc.bottom, DropDownInfo.pParent, &rc);
			break;
		}
	}
}



void CTrueColorToolBar::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCLICK pNMClick = reinterpret_cast<LPNMCLICK>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	NMTOOLBAR* pNMToolBar = (NMTOOLBAR*)pNMHDR;

	for (int i = 0; i < m_lstDropDownButton.GetSize(); i++)
	{
		
		stDropDownInfo DropDownInfo = m_lstDropDownButton.GetAt(i);

		if (DropDownInfo.uButtonID == UINT(pNMToolBar->iItem)) 
		{

			CMenu menu;
			menu.LoadMenu(DropDownInfo.uMenuID);
			CMenu* pPopup = menu.GetSubMenu(0);
			
			CRect rc;
			SendMessage(TB_GETRECT, (WPARAM)pNMToolBar->iItem, (LPARAM)&rc);
			ClientToScreen(&rc);
			
			pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_VERTICAL,
				                   rc.left, rc.bottom, DropDownInfo.pParent, &rc);
			break;
		}
	}
}
