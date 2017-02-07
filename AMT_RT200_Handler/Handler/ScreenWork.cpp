// ScreenWork.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Handler.h"
#include "Variable.h"
#include "ScreenWork.h"
#include "PublicFunction.h"
#include "Dialog_Pass_Check.h"

// CScreenWork

IMPLEMENT_DYNCREATE(CScreenWork, CFormView)

CScreenWork::CScreenWork()
	: CFormView(CScreenWork::IDD)
{

}

CScreenWork::~CScreenWork()
{
}

void CScreenWork::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_WORK,				m_tabWork);
	DDX_Control(pDX, IDC_GROUP_WORK_MODE,		m_groupWorkMode);
	DDX_Control(pDX, IDC_TREE_VIEW,				m_treeView);
}

BEGIN_MESSAGE_MAP(CScreenWork, CFormView)
	ON_WM_ERASEBKGND()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_VIEW, &CScreenWork::OnSelchangedTreeView)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CScreenWork 진단입니다.

#ifdef _DEBUG
void CScreenWork::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CScreenWork::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CScreenWork 메시지 처리기입니다.


BOOL CScreenWork::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
/*	if (m_pImagelist != NULL)
	{
		delete m_pImagelist;
		m_pImagelist = NULL;
	}

	st_handler_info.cWndWork = NULL;*/

	return CFormView::DestroyWindow();
}


void CScreenWork::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	OnInitGroupBox();
	OnInitLabel();
	OnInitButton();
	OnInitTree();

	m_tabWork.OnInitTab(this, CRect(0, 100, 0, 100));

	st_handler_info.cWndWork = this;
}

void CScreenWork::OnInitGroupBox()
{
	m_groupWorkMode.SetFont(clsFunc.OnLogFont(16));
	m_groupWorkMode.SetCatptionTextColor(BLUE_C);
	m_groupWorkMode.SetBorderColor(ORANGE_C);
	m_groupWorkMode.SetFontBold(TRUE);
}


void CScreenWork::OnInitLabel()
{
}


void CScreenWork::OnInitButton()
{

}


void CScreenWork::OnInitTree()
{
	CString strTmp;

	CBitmap bmp;
	m_pImagelist=new CImageList;
	m_pImagelist->Create(24,24,ILC_COLOR32|ILC_MASK,0,-1);

	bmp.LoadBitmap(IDB_BITMAP_INTERFACE_1);
	m_pImagelist->Add(&bmp,0xEFEFEF);bmp.DeleteObject();//0

	bmp.LoadBitmap(IDB_BITMAP_MAINTENANCE_1);
	m_pImagelist->Add(&bmp,0xEFEFEF);bmp.DeleteObject();//1

	bmp.LoadBitmap(IDB_BITMAP_GEAR_1);
	m_pImagelist->Add(&bmp,0xEFEFEF);bmp.DeleteObject();//2
	
	bmp.LoadBitmap(IDB_BITMAP_RECIPE_1);
	m_pImagelist->Add(&bmp,0xEFEFEF);bmp.DeleteObject();//3

	bmp.LoadBitmap(IDB_BITMAP_TIME_1);
	m_pImagelist->Add(&bmp,0xEFEFEF);bmp.DeleteObject();//4

	bmp.LoadBitmap(IDB_BITMAP_INTERFACE_1);
	m_pImagelist->Add(&bmp,0xEFEFEF);bmp.DeleteObject();//5

	m_treeView.SetImageList(m_pImagelist,TVSIL_NORMAL);
	m_treeView.DeleteAllItems();

	TV_INSERTSTRUCT tvstruct;
	HTREEITEM item[3]={NULL,NULL,NULL};//아이템 핸들러 3개 초기화

	tvstruct.hParent				= 0;
	tvstruct.hInsertAfter			= TVI_LAST;
	strTmp = _T("Maintenance");
	tvstruct.item.pszText			= (LPWSTR)(LPCTSTR)strTmp;
	tvstruct.item.mask				= TVIF_TEXT | TVIF_IMAGE |TVIF_SELECTEDIMAGE ;
	tvstruct.item.iImage			= 1;
	tvstruct.item.iSelectedImage	= 1;
	item[0]							= m_treeView.InsertItem(&tvstruct);
	m_treeView.EnsureVisible(item[0]);
	m_treeView.SelectDropTarget(item[0]);

	tvstruct.hParent				= 0;
	tvstruct.hInsertAfter			= TVI_LAST;
	strTmp = _T("Interface");
	tvstruct.item.pszText			= (LPWSTR)(LPCTSTR)strTmp;
	tvstruct.item.mask				= TVIF_TEXT | TVIF_IMAGE |TVIF_SELECTEDIMAGE ;
	tvstruct.item.iImage			= 0;
	tvstruct.item.iSelectedImage	= 0;
	item[0]							= m_treeView.InsertItem(&tvstruct);
	m_treeView.EnsureVisible(item[0]);

	tvstruct.hParent				= 0;
	tvstruct.hInsertAfter			= TVI_LAST;
	strTmp = _T("WaitTime");
	tvstruct.item.pszText			= (LPWSTR)(LPCTSTR)strTmp;
	tvstruct.item.mask				= TVIF_TEXT | TVIF_IMAGE |TVIF_SELECTEDIMAGE ;
	tvstruct.item.iImage			= 4;
	tvstruct.item.iSelectedImage	= 4;
	item[0]							= m_treeView.InsertItem(&tvstruct);
	m_treeView.EnsureVisible(item[0]);

	tvstruct.hParent				= 0;
	tvstruct.hInsertAfter			= TVI_LAST;
	strTmp = _T("Motor Speed");
	tvstruct.item.pszText			= (LPWSTR)(LPCTSTR)strTmp;
	tvstruct.item.mask				= TVIF_TEXT | TVIF_IMAGE |TVIF_SELECTEDIMAGE ;
	tvstruct.item.iImage			= 2;
	tvstruct.item.iSelectedImage	= 2;
	item[0]							= m_treeView.InsertItem(&tvstruct);
	m_treeView.EnsureVisible(item[0]);
}


BOOL CScreenWork::OnEraseBkgnd(CDC* pDC)
{
	CDC  memDC;                     // 메모리 DC
	CBitmap * pOldBitmap = NULL;    // m_BackBitmap으로 새로운 그림을 DC에 그릴 때, 이전 그려졌던 DC(즉, Bitmap)을 저장.
	BITMAP bitmapInfo;              // 그림의 정보(m_BackBitmap)
	st_handler_info.m_BackBitmap.GetBitmap(&bitmapInfo); // Bitmap 크기 구함.

	memDC.CreateCompatibleDC(pDC);  // 메모리 DC 생성
	pOldBitmap = memDC.SelectObject(&st_handler_info.m_BackBitmap);
	pDC->BitBlt(0, 0, bitmapInfo.bmWidth, bitmapInfo.bmHeight, &memDC, 0, 0, SRCCOPY);   // 메모리 DC에 그림을 그림

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();

	return true; // 흰 부러쉬가 아닌 배경을 그려야 하기 때문에 true
}


void CScreenWork::OnSelchangedTreeView(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	CDialog_Pass_Check	dlgPass;
		
	int					nResponse;
	// **************************************************************************
	// 메뉴 사용 불가능한 경우에 대한 조건 검사한다                              
	// -> 메인트 모드 또는 티칭 모드가 설정되어 있는지 검사                      
	// **************************************************************************

	if(st_handler_info.nMenuLock) return;

	CString strTmp;

	strTmp = m_treeView.GetItemText(pNMTreeView->itemNew.hItem);

	if (strTmp == _T("Maintenance"))
	{
		m_tabWork.OnViewChange(0);
		
		/* ************************************************************************** */
		/* 현재 화면 정보 타이틀바에 출력한다.                                        */
		/* ************************************************************************** */
		if (st_handler_info.cWndTitle != NULL)
		{
			st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, FORM_NAME, IDW_SCREEN_SET_MAINTENANCE);
		}
		/* ************************************************************************** */
	}
	else if (strTmp == _T("Interface"))
	{
		m_tabWork.OnViewChange(1);

		/* ************************************************************************** */
		/* 현재 화면 정보 타이틀바에 출력한다.                                        */
		/* ************************************************************************** */
		if (st_handler_info.cWndTitle != NULL)
		{
			st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, FORM_NAME, IDW_SCREEN_SET_INTERFACE);
		}
		/* ************************************************************************** */
	}
	else if (strTmp == _T("WaitTime"))
	{
		m_tabWork.OnViewChange(2);

		/* ************************************************************************** */
		/* 현재 화면 정보 타이틀바에 출력한다.                                        */
		/* ************************************************************************** */
		if (st_handler_info.cWndTitle != NULL)
		{
			st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, FORM_NAME, IDW_SCREEN_WAIT_TIME);
		}
		/* ************************************************************************** */
	}
	else if (strTmp == _T("Recipe"))
	{
		m_tabWork.OnViewChange(4);

		/* ************************************************************************** */
		/* 현재 화면 정보 타이틀바에 출력한다.                                        */
		/* ************************************************************************** */
		if (st_handler_info.cWndTitle != NULL)
		{
			st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, FORM_NAME, IDW_SCREEN_SET_RECIPE);
		}
		/* ************************************************************************** */
	}
	else if (strTmp == _T("RFID"))
	{
		m_tabWork.OnViewChange(5);

		/* ************************************************************************** */
		/* 현재 화면 정보 타이틀바에 출력한다.                                        */
		/* ************************************************************************** */
		if (st_handler_info.cWndTitle != NULL)
		{
			st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, FORM_NAME, IDW_SCREEN_SET_RFID);
		}
		/* ************************************************************************** */
	}
	else if (strTmp == _T("FTP"))
	{
		m_tabWork.OnViewChange(6);

		/* ************************************************************************** */
		/* 현재 화면 정보 타이틀바에 출력한다.                                        */
		/* ************************************************************************** */
		if (st_handler_info.cWndTitle != NULL)
		{
			st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, FORM_NAME, IDW_SCREEN_SET_FTP);
		}
		/* ************************************************************************** */
	}
	else if (strTmp == _T("COK"))
	{
		m_tabWork.OnViewChange(7);

		/* ************************************************************************** */
		/* 현재 화면 정보 타이틀바에 출력한다.                                        */
		/* ************************************************************************** */
		if (st_handler_info.cWndTitle != NULL)
		{
			st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, FORM_NAME, IDW_SCREEN_SET_COK);
		}
		/* ************************************************************************** */
	}
	else if (strTmp == _T("Part No."))
	{
		m_tabWork.OnViewChange(8);

		/* ************************************************************************** */
		/* 현재 화면 정보 타이틀바에 출력한다.                                        */
		/* ************************************************************************** */
		if (st_handler_info.cWndTitle != NULL)
		{
			st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, FORM_NAME, IDW_SCREEN_SET_PART_NO);
		}
		/* ************************************************************************** */
	}
	else if (strTmp == _T("Motor Speed"))
	{
		st_other_info.strPassLevel = _T("AMT_MASTER");
		
		dlgPass.m_nPassLevel = 6;
		nResponse = (int)dlgPass.DoModal();
	
		if (nResponse == IDOK)
		{
			m_tabWork.OnViewChange(3);
		}
		else 
		{
			m_treeView.SelectItem(pNMTreeView->itemOld.hItem);
			return;
		}

		/* ************************************************************************** */
		/* 현재 화면 정보 타이틀바에 출력한다.                                        */
		/* ************************************************************************** */
		if (st_handler_info.cWndTitle != NULL)
		{
			st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, FORM_NAME, IDW_SCREEN_MOTOR_SPEED);
		}
		/* ************************************************************************** */
	}
	else if (strTmp == _T("Test Site Offset"))
	{
		m_tabWork.OnViewChange(10);

		/* ************************************************************************** */
		/* 현재 화면 정보 타이틀바에 출력한다.                                        */
		/* ************************************************************************** */
		if (st_handler_info.cWndTitle != NULL)
		{
			st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, FORM_NAME, IDW_SCREEN_TEST_SITE);
		}
		/* ************************************************************************** */
	}
	else if (strTmp == _T("Custom IO Function"))
	{
		m_tabWork.OnViewChange(11);

		/* ************************************************************************** */
		/* 현재 화면 정보 타이틀바에 출력한다.                                        */
		/* ************************************************************************** */
		if (st_handler_info.cWndTitle != NULL)
		{
			st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, FORM_NAME, IDW_SCREEN_IO_FUNCTION);
		}
		/* ************************************************************************** */
	}
	m_treeView.SelectDropTarget(pNMTreeView->itemNew.hItem);
}


void CScreenWork::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
/*
	if (m_pImagelist != NULL)
	{
		delete m_pImagelist;
		m_pImagelist = NULL;
	}

	st_handler_info.cWndWork = NULL;
*/
	CFormView::PostNcDestroy();
}


void CScreenWork::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_pImagelist != NULL)
	{
		delete m_pImagelist;
		m_pImagelist = NULL;
	}

	if (st_handler_info.nMenuLock != FALSE)
	{
		st_handler_info.nMenuLock = FALSE;
	}

	st_handler_info.cWndWork = NULL;
}
