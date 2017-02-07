// ScreenList.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "ScreenList.h"
#include "PublicFunction.h"
#include "Dialog_Message.h"


IMPLEMENT_DYNCREATE(CScreenList, CFormView)

CScreenList::CScreenList()
	: CFormView(CScreenList::IDD)
{
	m_nListType		= 0;
}

CScreenList::~CScreenList()
{
}

void CScreenList::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM_DATA,			m_pGridData);
	DDX_Control(pDX, IDC_TREE_FILE_LIST,		m_treeFileList);
	DDX_Control(pDX, IDC_TREE_FILE_TYPE,		m_treeFileType);
	DDX_Control(pDX, IDC_GROUP_FILE_TYPE,		m_groupFileType);
	DDX_Control(pDX, IDC_GROUP_FILE_LIST,		m_groupFileList);
	DDX_Control(pDX, IDC_GROUP_DATA,			m_groupData);
//	DDX_Control(pDX, IDC_BTN_LIST_VIEW,			m_btnListView);
	DDX_Control(pDX, IDC_BTN_SAVE,				m_btnSave);
}

BEGIN_MESSAGE_MAP(CScreenList, CFormView)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_FILE_LIST, &CScreenList::OnSelchangedTreeFileList)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CScreenList::OnClickedBtnSave)
	ON_WM_ERASEBKGND()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_FILE_TYPE, &CScreenList::OnSelchangedTreeFileType)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CScreenList diagnostics

#ifdef _DEBUG
void CScreenList::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CScreenList::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CScreenList message handlers


BOOL CScreenList::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	/*if (m_pImagelist != NULL)
	{
		delete m_pImagelist;
		m_pImagelist = NULL;
	}

	st_handler_info.cWndList = NULL;*/

	return CFormView::DestroyWindow();
}


void CScreenList::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	OnInitGroupBox();
	OnInitTreeFileList();
	OnInitTreeFileType();
	OnTreeListData(m_nListType);
	OnInitButton();
	OnInitGridData();
	m_pGridData.DeleteAllItems();

//	st_handler_info.cWndList = this;
	// TODO: Add your specialized code here and/or call the base class
}

void CScreenList::OnInitGridData()
{
	//2015.02.10
	CString strTmp;
	
	int i, j;

	m_pGridData.DeleteAllItems();
	m_pGridData.SetFrameFocusCell(FALSE);
	m_pGridData.SetTrackFocusCell(FALSE);
	m_pGridData.EnableSelection(FALSE);

	m_pGridData.SetGridLineColor(BLACK_C);
	m_pGridData.SetGridLines(1);

	m_pGridData.SetRowCount(50);
	m_pGridData.SetColumnCount(3);

	m_pGridData.SetFixedRowCount(0);
	m_pGridData.SetFixedColumnCount(0);
	m_pGridData.SetFixedBkColor(RGB(0,0,200));
	m_pGridData.SetFixedBkColor(RGB(200,200,255));
	m_pGridData.SetTextBkColor(RGB(150,150,200));

	for (i=0; i<50; i++) 
	{
		m_pGridData.SetRowHeight(i, 41);

		for (j=0; j<3; j++) 
		{
			switch (j)
			{
			case 0:
				m_pGridData.SetColumnWidth(j, 150);
				break;

			case 1:
				m_pGridData.SetColumnWidth(j, 500);
				break;

			case 2:
				m_pGridData.SetColumnWidth(j, 200);
				break;
			}

			m_pGridData.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_pGridData.SetItemState(i, j, GVIS_READONLY);
			m_pGridData.SetItemBkColour(i, j, WHITE_C, CLR_DEFAULT);
		}
	}

	// IO MAP Header ////////////////////////////////////////////////////////////////////////////////////////////// 
	m_pGridData.SetItemBkColour(0, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridData.SetItemFont(0, 0, &clsFunc.OnLogFont(32));
	m_pGridData.SetItemText(0, 0, _T("No."));

	m_pGridData.SetItemBkColour(0, 1, RGB(219, 229, 241), BLACK_L);
	m_pGridData.SetItemFont(0, 1, &clsFunc.OnLogFont(20));
	m_pGridData.SetItemText(0, 1, _T("Thread Description"));

	m_pGridData.SetItemBkColour(0, 2,RGB(219, 229, 241), BLACK_L);
	m_pGridData.SetItemFont(0, 2, &clsFunc.OnLogFont(20));
	m_pGridData.SetItemText(0, 2, _T("Step"));

	for (i=0; i<49; i++) 
	{
		strTmp.Format(_T("%d"), i + 1);
		m_pGridData.SetItemBkColour( i + 1, 0, RGB(219, 229, 241), BLACK_L);
		m_pGridData.SetItemFont(i + 1, 0, &clsFunc.OnLogFont(24));
		m_pGridData.SetItemText(i + 1, 0, strTmp);

		m_pGridData.SetItemBkColour( i + 1, 1, RGB(242, 242, 242),  BLACK_L);
		m_pGridData.SetItemFont(i + 1, 1, &clsFunc.OnLogFont(18));
		//m_pGridData.SetItemText(i + 1, 1, strHead[i]);

		m_pGridData.SetItemBkColour( i + 1, 2, WHITE_C,  BLACK_L);
		m_pGridData.SetItemFont(i + 1, 2, &clsFunc.OnLogFont(20));
		m_pGridData.SetItemText(i + 1, 2, _T("0"));
	}
	//2015.02.10
}

void CScreenList::OnInitGroupBox()
{
	m_groupFileType.SetFont(clsFunc.OnLogFont(16));
	m_groupFileType.SetCatptionTextColor(BLUE_C);
	m_groupFileType.SetBorderColor(ORANGE_C);
	m_groupFileType.SetFontBold(TRUE);
	m_groupFileType.SetBackgroundColor(WINDOW_UP);

	m_groupFileList.SetFont(clsFunc.OnLogFont(16));
	m_groupFileList.SetCatptionTextColor(BLUE_C);
	m_groupFileList.SetBorderColor(ORANGE_C);
	m_groupFileList.SetFontBold(TRUE);
	m_groupFileList.SetBackgroundColor(WINDOW_UP);

	m_groupData.SetFont(clsFunc.OnLogFont(16));
	m_groupData.SetCatptionTextColor(BLUE_C);
	m_groupData.SetBorderColor(ORANGE_C);
	m_groupData.SetFontBold(TRUE);
	m_groupData.SetBackgroundColor(WINDOW_UP);
}


void CScreenList::OnInitTreeFileList()
{
	CString strTmp;

	CBitmap bmp;
	m_pImagelist=new CImageList;
	m_pImagelist->Create(24,24,ILC_COLOR32|ILC_MASK,0,-1);

	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_HIRFOLDER));
	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_HIOPEN));//7
	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_HICLOSE));//6
}


void CScreenList::OnInitTreeFileType()
{
	m_treeFileType.SetImageList(m_pImagelist,TVSIL_NORMAL);
	m_treeFileType.DeleteAllItems();

	TV_INSERTSTRUCT tvstruct;
	HTREEITEM item[2]={NULL,NULL};//아이템 핸들러 3개 초기화

	tvstruct.hParent				= 0;
	tvstruct.hInsertAfter			= TVI_LAST;
	tvstruct.item.pszText			= (LPWSTR)(LPCTSTR)_T("Alarm");
	tvstruct.item.mask				= TVIF_TEXT | TVIF_IMAGE |TVIF_SELECTEDIMAGE ;
	tvstruct.item.iImage			= 0;
	tvstruct.item.iSelectedImage	= 0;
	item[0]							= m_treeFileType.InsertItem(&tvstruct);

	tvstruct.hParent				=	item[0];
 	tvstruct.hInsertAfter			=	TVI_LAST;
	tvstruct.item.pszText			=	(LPWSTR)(LPCTSTR)(LPWSTR)(LPCTSTR)_T("Alarm Month");
 	tvstruct.item.mask				=	TVIF_TEXT| TVIF_IMAGE |TVIF_SELECTEDIMAGE ;
 	tvstruct.item.iImage			=	2;
 	tvstruct.item.iSelectedImage	=	1;
 	item[1]							= m_treeFileType.InsertItem(&tvstruct);

	tvstruct.hParent				=	item[0];
 	tvstruct.hInsertAfter			=	TVI_LAST;
	tvstruct.item.pszText			=	(LPWSTR)(LPCTSTR)(LPWSTR)(LPCTSTR)_T("Alarm Day");
 	tvstruct.item.mask				=	TVIF_TEXT| TVIF_IMAGE |TVIF_SELECTEDIMAGE ;
 	tvstruct.item.iImage			=	2;
 	tvstruct.item.iSelectedImage	=	1;
 	item[1]							= m_treeFileType.InsertItem(&tvstruct);

	tvstruct.hParent				= 0;
	tvstruct.hInsertAfter			= TVI_LAST;
	tvstruct.item.pszText			= (LPWSTR)(LPCTSTR)_T("Machine");
	tvstruct.item.mask				= TVIF_TEXT | TVIF_IMAGE |TVIF_SELECTEDIMAGE ;
	tvstruct.item.iImage			= 0;
	tvstruct.item.iSelectedImage	= 0;
	item[0]							= m_treeFileType.InsertItem(&tvstruct);

	tvstruct.hParent				= 0;
	tvstruct.hInsertAfter			= TVI_LAST;
	tvstruct.item.pszText			= (LPWSTR)(LPCTSTR)_T("Operation");
	tvstruct.item.mask				= TVIF_TEXT | TVIF_IMAGE |TVIF_SELECTEDIMAGE ;
	tvstruct.item.iImage			= 0;
	tvstruct.item.iSelectedImage	= 0;
	item[0]							= m_treeFileType.InsertItem(&tvstruct);
	m_treeFileType.SelectItem(item[0]);
	m_treeFileType.SetFocus();

	tvstruct.hParent				= 0;
	tvstruct.hInsertAfter			= TVI_LAST;
	tvstruct.item.pszText			= (LPWSTR)(LPCTSTR)_T("Total");
	tvstruct.item.mask				= TVIF_TEXT | TVIF_IMAGE |TVIF_SELECTEDIMAGE ;
	tvstruct.item.iImage			= 0;
	tvstruct.item.iSelectedImage	= 0;
	item[0]							= m_treeFileType.InsertItem(&tvstruct);

	tvstruct.hParent				= 0;
	tvstruct.hInsertAfter			= TVI_LAST;
	tvstruct.item.pszText			= (LPWSTR)(LPCTSTR)_T("Thread Step");
	tvstruct.item.mask				= TVIF_TEXT | TVIF_IMAGE |TVIF_SELECTEDIMAGE ;
	tvstruct.item.iImage			= 0;
	tvstruct.item.iSelectedImage	= 0;
	item[0]							= m_treeFileType.InsertItem(&tvstruct);

	m_treeFileType.EnsureVisible(item[1]);
}


void CScreenList::OnInitButton()
{
/*	m_btnListView.SetBitmaps(IDC_BTN_LIST_VIEW, IDB_BITMAP_LIST_DN, WINDOW_DN, IDB_BITMAP_LIST_UP, WINDOW_UP);
	m_btnListView.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnListView.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnListView.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnListView.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnListView.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnListView.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnListView.SetFont(clsFunc.m_pFont[3]);
	m_btnListView.SetWindowTextW(_T("Operation"));
	m_btnListView.SetTooltipText(_T("List Screen Display Change"));
	m_btnListView.SetMenu(IDR_MENU_LIST, this->m_hWnd);*/

	m_btnSave.SetBitmaps(IDC_BTN_SAVE, IDB_BITMAP_SAVE_DN, WINDOW_DN, IDB_BITMAP_SAVE_UP, WINDOW_UP);
	m_btnSave.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnSave.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnSave.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnSave.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnSave.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnSave.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnSave.SetFont(clsFunc.m_pFont[3]);
	m_btnSave.SetTooltipText(_T("Thread Step Data Save"));
	m_btnSave.ShowWindow(SW_HIDE);
}


void CScreenList::OnTreeListData(int nList)
{
	int nPos = 0;
	CString strFileTitle;
	CString strFileName;
	CString strTemp;
	CString strItem[2];

	m_treeFileList.SetImageList(m_pImagelist,TVSIL_NORMAL);
	m_treeFileList.DeleteAllItems();

	TV_INSERTSTRUCT tvstruct;
	HTREEITEM item[2]={NULL,NULL};//아이템 핸들러 3개 초기화

	HANDLE hFind;
    WIN32_FIND_DATA fd;

	switch (nList)
	{
		case 0:
			strFileName = st_path_info.strOperation + _T("*.*");
			break;

		case 1:
			strFileName = st_path_info.strMachine + _T("*.*");
			break;

		case 2:
			strFileName = st_path_info.strTotal + _T("*.*");
			break;

		case 3:
			strFileName = st_path_info.strAlarmDay + _T("*.*");
			break;

		case 4:
			strFileName = st_path_info.strAlarmMonth + _T("*.*");
			break;
	}

	strFileName += "*.*";
    
    if ((hFind = ::FindFirstFile ((LPCTSTR) strFileName, &fd)) != INVALID_HANDLE_VALUE) 
	{
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
		{
			strTemp.Format(_T("%s"), fd.cFileName);
			nPos = strTemp.Find(_T("."), 0);
			strFileTitle = strTemp.Mid(0, nPos);

			strItem[1] = strFileTitle.Mid(2, 6);

			if(strItem[0] !=  strItem[1])
			{
				strItem[0]						= strItem[1];

				tvstruct.hParent				= 0;
				tvstruct.hInsertAfter			= TVI_LAST;
				tvstruct.item.pszText			= (LPWSTR)(LPCTSTR)strItem[0];
				tvstruct.item.mask				= TVIF_TEXT | TVIF_IMAGE |TVIF_SELECTEDIMAGE ;
				tvstruct.item.iImage			= 0;
				tvstruct.item.iSelectedImage	= 0;
				item[0]							= m_treeFileList.InsertItem(&tvstruct);

				tvstruct.hParent				=	item[0];
				tvstruct.hInsertAfter			=	TVI_LAST;
				tvstruct.item.pszText			=	(LPWSTR)(LPCTSTR)strFileTitle;
				tvstruct.item.mask				=	TVIF_TEXT| TVIF_IMAGE |TVIF_SELECTEDIMAGE ;
				tvstruct.item.iImage			=	2;
				tvstruct.item.iSelectedImage	=	1;
				item[1]							= m_treeFileList.InsertItem(&tvstruct);
				m_treeFileList.EnsureVisible(item[1]);
			}
			else
			{
				tvstruct.hParent				=	item[0];
				tvstruct.hInsertAfter			=	TVI_LAST;
				tvstruct.item.pszText			=	(LPWSTR)(LPCTSTR)strFileTitle;
				tvstruct.item.mask				=	TVIF_TEXT| TVIF_IMAGE |TVIF_SELECTEDIMAGE ;
				tvstruct.item.iImage			=	2;
				tvstruct.item.iSelectedImage	=	1;
				item[1]							= m_treeFileList.InsertItem(&tvstruct);
				m_treeFileList.EnsureVisible(item[1]);
			}
        }

        while (::FindNextFile (hFind, &fd)) 
		{
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				strTemp.Format(_T("%s"), fd.cFileName);
				nPos = strTemp.Find(_T("."), 0);
				strFileTitle = strTemp.Mid(0, nPos);

				strItem[1] = strFileTitle.Mid(2, 6);

				if(strItem[0] !=  strItem[1])
				{
					strItem[0]						= strItem[1];

					tvstruct.hParent				= 0;
					tvstruct.hInsertAfter			= TVI_LAST;
					tvstruct.item.pszText			= (LPWSTR)(LPCTSTR)strItem[0];
					tvstruct.item.mask				= TVIF_TEXT | TVIF_IMAGE |TVIF_SELECTEDIMAGE ;
					tvstruct.item.iImage			= 0;
					tvstruct.item.iSelectedImage	= 0;
					item[0]							= m_treeFileList.InsertItem(&tvstruct);

					tvstruct.hParent				=	item[0];
					tvstruct.hInsertAfter			=	TVI_LAST;
					tvstruct.item.pszText			=	(LPWSTR)(LPCTSTR)strFileTitle;
					tvstruct.item.mask				=	TVIF_TEXT| TVIF_IMAGE |TVIF_SELECTEDIMAGE ;
					tvstruct.item.iImage			=	2;
					tvstruct.item.iSelectedImage	=	1;
					item[1]							= m_treeFileList.InsertItem(&tvstruct);
					m_treeFileList.EnsureVisible(item[1]);
				}
				else
				{
					tvstruct.hParent				=	item[0];
					tvstruct.hInsertAfter			=	TVI_LAST;
					tvstruct.item.pszText			=	(LPWSTR)(LPCTSTR)strFileTitle;
					tvstruct.item.mask				=	TVIF_TEXT| TVIF_IMAGE |TVIF_SELECTEDIMAGE ;
					tvstruct.item.iImage			=	2;
					tvstruct.item.iSelectedImage	=	1;
					item[1]							= m_treeFileList.InsertItem(&tvstruct);
					m_treeFileList.EnsureVisible(item[1]);
				}
			}
        }
		::FindClose(hFind);
    }
}


void CScreenList::OnSelchangedTreeFileList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	if(st_handler_info.nMenuLock) return;

	CString strTmp;
	CString strFileName;

	strTmp = m_treeFileList.GetItemText(pNMTreeView->itemNew.hItem);

	if (NULL == m_treeFileList.GetParentItem(pNMTreeView->itemNew.hItem))
	{
		return ;
	}

	switch (m_nListType)
	{
		case 0:
			strFileName.Format(_T("%s%s.txt"), st_path_info.strOperation, strTmp); 
			break;

		case 1:
			strFileName.Format(_T("%s%s.txt"), st_path_info.strMachine, strTmp); 
			break;

		case 2:
			strFileName.Format(_T("%s%s.txt"), st_path_info.strTotal, strTmp); 
			break;

		case 3:
			strFileName.Format(_T("%s%s.txt"), st_path_info.strAlarmDay, strTmp); 
			break;

		case 4:
			strFileName.Format(_T("%s%s.txt"), st_path_info.strAlarmMonth, strTmp); 
			break;
	} 
	

	m_pGridData.Load(strFileName, '\r\n');

	m_pGridData.AutoSizeColumns(GVS_DATA);
	m_pGridData.AutoSize();

	m_pGridData.Invalidate();
}


void CScreenList::OnClickedBtnSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog_Message dlgMsg;
	CTime time;
	char chData[4096];
	char fileName[256];
	CString strFileName;
	CString strTemp;
	CString strTmp;
	int nResponse;
	int i;

	FILE  *fp ;
	time = CTime::GetCurrentTime();

	dlgMsg.m_nMessageType	= 1;
	dlgMsg.m_strMessage.Format(_T("Do you want a Step Save?"));

	nResponse = (int)dlgMsg.DoModal();
		
	if(nResponse == IDOK)
	{
		strFileName.Format(_T("%s\\%04d%02d%02d%02d%02d.txt"), st_path_info.strPathBack, time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute());
		clsFunc.OnStringToChar(strFileName, fileName);	//2015.03.04 night

		m_pGridData.Save(strFileName);

		for (i=0; i<50; i++)
		{
			//2015.02.10
			if (st_other_info.strHistoryStep[i] != _T(""))
			{
				strTmp = clsFunc.ThreadID(i);
				strTemp.Format(_T("THREAD ID[%s] %s \r\n"),strTmp , st_other_info.strHistoryStep[i]);

				fopen_s(&fp, fileName, "a+");
				clsFunc.OnStringToChar(strTemp, chData);
				fprintf(fp, chData) ;
				fclose(fp); 

			}
		}

		clsFunc.OnRecoveryReadWrite(1, 0);
	}
}


BOOL CScreenList::OnEraseBkgnd(CDC* pDC)
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


void CScreenList::OnSelchangedTreeFileType(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	if(st_handler_info.nMenuLock) return;

	CString strTmp;
	CString strFileName;

	strTmp = m_treeFileList.GetItemText(pNMTreeView->itemNew.hItem);

	if (strTmp == _T(""))
	{
		return ;
	}

	if (strTmp == _T("Alarm Month"))
	{
		m_pGridData.DeleteAllItems();
		m_pGridData.Invalidate();

		m_btnSave.ShowWindow(SW_HIDE);

		m_nListType = 4;
		m_treeFileList.ShowWindow(SW_SHOW);
	
		OnTreeListData(m_nListType);
	}
	else if (strTmp == _T("Alarm Day"))
	{
		m_pGridData.DeleteAllItems();
		m_pGridData.Invalidate();

		m_btnSave.ShowWindow(SW_HIDE);

		m_nListType = 3;
		m_treeFileList.ShowWindow(SW_SHOW);
	
		OnTreeListData(m_nListType);
	}
	else if (strTmp == _T("Machine"))
	{
		m_pGridData.DeleteAllItems();
		m_pGridData.Invalidate();

		m_btnSave.ShowWindow(SW_HIDE);

		m_nListType = 1;
		m_treeFileList.ShowWindow(SW_SHOW);
	
		OnTreeListData(m_nListType);
	}
	else if (strTmp == _T("Total"))
	{
		m_pGridData.DeleteAllItems();
		m_pGridData.Invalidate();

		m_btnSave.ShowWindow(SW_HIDE);

		m_nListType = 2;
		m_treeFileList.ShowWindow(SW_SHOW);
	
		OnTreeListData(m_nListType);
	}
	else if (strTmp == _T("Operation"))
	{
		m_pGridData.DeleteAllItems();
		m_pGridData.Invalidate();

		m_btnSave.ShowWindow(SW_HIDE);

		m_nListType = 0;
		m_treeFileList.ShowWindow(SW_SHOW);
	
		OnTreeListData(m_nListType);
	}
	else if (strTmp == _T("Thread Step"))
	{
		m_pGridData.DeleteAllItems();
		m_pGridData.Invalidate();

		m_btnSave.ShowWindow(SW_SHOW);

		m_nListType = 4;
		m_treeFileList.DeleteAllItems();
		m_treeFileList.ShowWindow(SW_HIDE);

		OnInitGridData();
	}
}


void CScreenList::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	/*if (m_pImagelist != NULL)
	{
		delete m_pImagelist;
		m_pImagelist = NULL;
	}

	st_handler_info.cWndList = NULL;*/
	CFormView::PostNcDestroy();
}


void CScreenList::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_pImagelist != NULL)
	{
		delete m_pImagelist;
		m_pImagelist = NULL;
	}

//	st_handler_info.cWndList = NULL;
}
