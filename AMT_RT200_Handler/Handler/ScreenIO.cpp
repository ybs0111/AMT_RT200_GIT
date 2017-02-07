// ScreenIO.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Handler.h"
#include "ScreenIO.h"
#include "Variable.h"
#include "Alg_Excel.h"
#include "PublicFunction.h"
#include "FastechPublic_IO.h"
#include "Dialog_KeyPad.h"
#include "AlgMemory.h"
#include ".\\Ctrl\\KeyBoard.h"

#define TMR_IO_CONTINUE		100
#define TMR_IO_INPUT		200
#define TMR_IO_OUTPUT		300

IMPLEMENT_DYNCREATE(CScreenIO, CFormView)

CScreenIO::CScreenIO()
	: CFormView(CScreenIO::IDD)
{
	m_nIOMaster			= 0;
	m_nIOPort			= 0;
	m_nIOSlave			= 0;
	m_nActionMode		= 0;

	m_nIOActionCount[0]	= 1;
	m_nIOActionCount[1]	= 0;

	m_nContinueWaitTime	= 5000;
}

CScreenIO::~CScreenIO()
{
}

void CScreenIO::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CUSTOM_IO,				m_pGridIo);
	DDX_Control(pDX, IDC_TREE_lO_LIST,			m_treeIoList);
	DDX_Control(pDX, IDC_GROUP_IO_LIST,			m_groupIoList);
	DDX_Control(pDX, IDC_GROUP_ACTION,			m_groupAction);
	DDX_Control(pDX, IDC_CHECK_ACTION_1,		m_btnAction1);
	DDX_Control(pDX, IDC_CHECK_ACTION_10,		m_btnAction10);
	DDX_Control(pDX, IDC_CHECK_ACTION_100,		m_btnAction100);
	DDX_Control(pDX, IDC_BTN_EMERGENCY,			m_btnEmergency);
	DDX_Control(pDX, IDC_DGT_WAIT_TIME,			m_dgtWaitTime);
}

BEGIN_MESSAGE_MAP(CScreenIO, CFormView)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_lO_LIST, &CScreenIO::OnSelchangedTreeLoList)
	ON_NOTIFY(NM_CLICK, IDC_CUSTOM_IO, OnCellClick)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_EMERGENCY, &CScreenIO::OnClickedBtnEmergency)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_CHECK_ACTION_1, &CScreenIO::OnClickedCheckAction1)
	ON_BN_CLICKED(IDC_CHECK_ACTION_10, &CScreenIO::OnClickedCheckAction10)
	ON_BN_CLICKED(IDC_CHECK_ACTION_100, &CScreenIO::OnClickedCheckAction100)
	ON_STN_CLICKED(IDC_DGT_WAIT_TIME, &CScreenIO::OnClickedDgtWaitTime)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CScreenIO 진단입니다.

#ifdef _DEBUG
void CScreenIO::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CScreenIO::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CScreenIO 메시지 처리기입니다.


void CScreenIO::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	OnInitButton();
	OnInitGroupBox();
	OnInitDigit();
	OnInitGridIo();
	OnInitTree();
	
	SetTimer(TMR_IO_INPUT,	100, NULL);
	SetTimer(TMR_IO_OUTPUT,	100, NULL);

	st_handler_info.cWndIO = this;
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}


BOOL CScreenIO::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
/*	KillTimer(TMR_IO_INPUT);
	KillTimer(TMR_IO_OUTPUT);

	if (m_bIOAction)
	{
		KillTimer(TMR_IO_CONTINUE);
	}

	if (m_pImagelist != NULL)
	{
		delete m_pImagelist;
		m_pImagelist = NULL;
	}

	st_handler_info.cWndIO = NULL;*/

	return CFormView::DestroyWindow();
}

void CScreenIO::OnInitGridIo()
{
	CString strTmp;
	int i, j;
	
	m_pGridIo.SetFrameFocusCell(FALSE);
	m_pGridIo.SetTrackFocusCell(FALSE);
	m_pGridIo.EnableSelection(FALSE);

	m_pGridIo.SetGridLineColor(BLACK_C);
	m_pGridIo.SetGridLines(1);

	m_pGridIo.SetRowCount(19);
	m_pGridIo.SetColumnCount(6);

	m_pGridIo.SetFixedRowCount(0);
	m_pGridIo.SetFixedColumnCount(0);
	m_pGridIo.SetFixedBkColor(RGB(0,0,200));
	m_pGridIo.SetFixedBkColor(RGB(200,200,255));
	m_pGridIo.SetTextBkColor(RGB(150,150,200));

	for (i=0; i<19; i++) 
	{
		m_pGridIo.SetRowHeight(i, 41);

		for (j=0; j<6; j++) 
		{
			switch (j)
			{
				case 0:
					m_pGridIo.SetColumnWidth(j, 102);
					break;

				case 1:
					m_pGridIo.SetColumnWidth(j, 130);
					break;

				case 2:
					m_pGridIo.SetColumnWidth(j, 120);
					break;

				case 3:
					m_pGridIo.SetColumnWidth(j, 50);
					break;

				case 4:
					m_pGridIo.SetColumnWidth(j, 120);
					break;

				case 5:
					m_pGridIo.SetColumnWidth(j, 380);
					break;
			}

			m_pGridIo.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_pGridIo.SetItemState(i, j, GVIS_READONLY);
			m_pGridIo.SetItemBkColour(i, j, WHITE_C, CLR_DEFAULT);
		}
	}
	
	// IO MAP Header ////////////////////////////////////////////////////////////////////////////////////////////// 
	m_pGridIo.MergeCells(0, 0, 0, 5);
	m_pGridIo.SetItemBkColour(0, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridIo.SetItemFont(0, 0, &clsFunc.OnLogFont(32));
	m_pGridIo.SetItemText(0, 0, _T("I/O Map Design Sheet"));

	m_pGridIo.MergeCells(1, 0, 1, 2);
	m_pGridIo.SetItemBkColour(1, 0, RGB(242, 242, 242), BLACK_L);
	m_pGridIo.SetItemFont(1, 0, &clsFunc.OnLogFont(20));
	m_pGridIo.SetItemText(1, 0, _T("Master : [0] Port : [0] Slave : [0]"));

	m_pGridIo.MergeCells(1, 3, 1, 4);
	m_pGridIo.SetItemBkColour(1, 3, RGB(252, 213, 180), BLACK_L);
	m_pGridIo.SetItemFont(1, 3, &clsFunc.OnLogFont(20));
	m_pGridIo.SetItemText(1, 3, _T("Ass'Y"));

	m_pGridIo.SetItemBkColour(1, 5, RGB(242, 242, 242), BLACK_L);
	m_pGridIo.SetItemFont(1, 5, &clsFunc.OnLogFont(20));
	m_pGridIo.SetItemText(1, 5, _T("-"));

	m_pGridIo.SetItemBkColour(2, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridIo.SetItemFont(2, 0, &clsFunc.OnLogFont(20));
	m_pGridIo.SetItemText(2, 0, _T("No."));

	m_pGridIo.SetItemBkColour(2, 1, RGB(219, 229, 241), BLACK_L);
	m_pGridIo.SetItemFont(2, 1, &clsFunc.OnLogFont(20));
	m_pGridIo.SetItemText(2, 1, _T("Label"));

	m_pGridIo.SetItemBkColour(2, 2, RGB(219, 229, 241), BLACK_L);
	m_pGridIo.SetItemFont(2, 2, &clsFunc.OnLogFont(20));
	m_pGridIo.SetItemText(2, 2, _T("I/O"));

	m_pGridIo.MergeCells(2, 3, 2, 4);
	m_pGridIo.SetItemBkColour(2, 3, RGB(219, 229, 241), BLACK_L);
	m_pGridIo.SetItemFont(2, 3, &clsFunc.OnLogFont(20));
	m_pGridIo.SetItemText(2, 3, _T("Ststus"));

	m_pGridIo.SetItemBkColour(2, 5, RGB(219, 229, 241), BLACK_L);
	m_pGridIo.SetItemFont(2, 5, &clsFunc.OnLogFont(20));
	m_pGridIo.SetItemText(2, 5, _T("Description"));

	for (i=0; i<16; i++) 
	{
		strTmp.Format(_T("%d"), i+1);
		m_pGridIo.SetItemBkColour(i + 3, 0, RGB(79, 79, 225), WHITE_C);
		m_pGridIo.SetItemFont(i + 3, 0, &clsFunc.OnLogFont(20));
		m_pGridIo.SetItemText(i + 3, 0, strTmp);
	}
	OnDisplayIO(m_nIOMaster, m_nIOPort, m_nIOSlave);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}


void CScreenIO::OnInitTree()
{
	CString strTmp;

	int i, j, k;

	CBitmap bmp;
	m_pImagelist=new CImageList;
	m_pImagelist->Create(24,24,ILC_COLOR32|ILC_MASK,0,-1);

	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_HIRFOLDER));//12
	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_HIOPEN));//7
	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_HICLOSE));//6

	m_treeIoList.SetImageList(m_pImagelist,TVSIL_NORMAL);
	m_treeIoList.DeleteAllItems();

	TV_INSERTSTRUCT tvstruct;
	HTREEITEM item[3]={NULL,NULL,NULL};//아이템 핸들러 3개 초기화

	for (i=0; i<3; i++)
	{
		tvstruct.hParent				= 0;
		tvstruct.hInsertAfter			= TVI_LAST;
		strTmp.Format(_T("AMT_RT200 I/O Board : [%d]"), i + 1);
		tvstruct.item.pszText			= (LPWSTR)(LPCTSTR)strTmp;
		tvstruct.item.mask				= TVIF_TEXT | TVIF_IMAGE |TVIF_SELECTEDIMAGE ;
		tvstruct.item.iImage			= 0;
		tvstruct.item.iSelectedImage	= 0;
		item[0]							= m_treeIoList.InsertItem(&tvstruct);

		for (j=0; j<4; j++)
		{
			tvstruct.hParent				= item[0];
 			tvstruct.hInsertAfter			= TVI_LAST;
			strTmp.Format(_T("Master: [%d] Port : [%d]"), i + 1, j+1);
			tvstruct.item.pszText			= (LPWSTR)(LPCTSTR)strTmp;
 			tvstruct.item.mask				= TVIF_TEXT| TVIF_IMAGE |TVIF_SELECTEDIMAGE ;
 			tvstruct.item.iImage			= 2;
 			tvstruct.item.iSelectedImage	= 1;
 			item[1]							= m_treeIoList.InsertItem(&tvstruct); 

			for (k=0; k<clsExcel.m_nIOPort[i][j]+1; k++)
			{
				tvstruct.hParent				=	item[1];
 				tvstruct.hInsertAfter			=	TVI_LAST;
				tvstruct.item.pszText			=	(LPWSTR)(LPCTSTR)clsExcel.m_strIOPortLabel[i][j][k];
 				tvstruct.item.mask				=	TVIF_TEXT| TVIF_IMAGE |TVIF_SELECTEDIMAGE ;
 				tvstruct.item.iImage			=	2;
 				tvstruct.item.iSelectedImage	=	1;
 				item[2]							= m_treeIoList.InsertItem(&tvstruct);

//				m_treeIoList.Expand(item[1], TVE_EXPAND);
				m_treeIoList.EnsureVisible(item[2]);
			}
		}
	}
}

void CScreenIO::OnIoActionMode(int nMode)
{
	m_nActionMode = nMode;
	switch (nMode)
	{
		case 0:
			m_btnAction1.SetCheck(TRUE);
			m_btnAction10.SetCheck(FALSE);
			m_btnAction100.SetCheck(FALSE);
			break;

		case 1:
			m_btnAction1.SetCheck(FALSE);
			m_btnAction10.SetCheck(TRUE);
			m_btnAction100.SetCheck(FALSE);
			break;

		case 2:
			m_btnAction1.SetCheck(FALSE);
			m_btnAction10.SetCheck(FALSE);
			m_btnAction100.SetCheck(TRUE);
			break;
	}

	UpdateData(FALSE);
}

void CScreenIO::OnDisplayIO(int nMaster, int nPort, int nSlave)
{
	int i;

	CString strTmp;

	strTmp.Format(_T("Master : [%d]		"), nMaster, nPort, nSlave);
	m_pGridIo.SetItemText(1, 0, strTmp);

	m_pGridIo.SetItemText(1, 5,clsExcel.m_strIOPortLabel[nMaster][nPort][nSlave]);

	for (i=0; i<16; i++) 
	{
		if (clsExcel.m_nIOBitType[nMaster][nPort][nSlave][i] == IO_IN)
		{
			// input
			m_pGridIo.SetItemBkColour(i + 3, 1, RGB(255, 255, 153), BLACK_L);
			m_pGridIo.SetItemFont(i + 3, 1, &clsFunc.OnLogFont(20));
//			strTmp.Format(_T("PS%04d"), clsExcel.m_nINIOBitNum[nMaster][nPort][nSlave][i]);
//			m_pGridIo.SetItemText(i + 3, 1,strTmp);
			m_pGridIo.SetItemText(i + 3, 1, clsExcel.m_strIOBitLabel[nMaster][nPort][nSlave][i]);

			m_pGridIo.SetItemBkColour(i + 3, 2, RGB(255, 255, 153), BLACK_L);
			m_pGridIo.SetItemFont(i + 3, 2, &clsFunc.OnLogFont(20));
			m_pGridIo.SetItemText(i + 3, 2, _T("Input"));

			m_pGridIo.SetItemBkColour(i + 3, 3, RGB(242, 242, 242), BLACK_L);
			m_pGridIo.SetItemFont(i + 3, 3, &clsFunc.OnLogFont(20));
			m_pGridIo.SetItemText(i + 3, 3, _T(""));

			m_pGridIo.SetItemBkColour(i + 3, 4, YELLOW_C, BLACK_L);
			m_pGridIo.SetItemFont(i + 3, 4, &clsFunc.OnLogFont(20));
			m_pGridIo.SetItemText(i + 3, 4, _T("OFF"));

			m_pGridIo.SetItemBkColour(i + 3, 5, RGB(255, 255, 153), BLACK_L);
			m_pGridIo.SetItemFont(i + 3, 5, &clsFunc.OnLogFont(16));
//			m_pGridIo.SetItemText(i + 3, 5, clsExcel.m_strIOBitLabel[nMaster][nPort][nSlave][i]);
			m_pGridIo.SetItemText(i + 3, 5, clsExcel.m_strIOBitDescrip[nMaster][nPort][nSlave][i]);
		}
		else if (clsExcel.m_nIOBitType[nMaster][nPort][nSlave][i] == IO_OUT)
		{
			// input
			m_pGridIo.SetItemBkColour(i + 3, 1, RGB(202, 255, 185), BLACK_L);
			m_pGridIo.SetItemFont(i + 3, 1, &clsFunc.OnLogFont(20));
//			strTmp.Format(_T("S%04d"), clsExcel.m_nOUTIOBitNum[nMaster][nPort][nSlave][i]);
//			m_pGridIo.SetItemText(i + 3, 1,strTmp);
			m_pGridIo.SetItemText(i + 3, 1, clsExcel.m_strIOBitLabel[nMaster][nPort][nSlave][i]);

			m_pGridIo.SetItemBkColour(i + 3, 2, RGB(202, 255, 185), BLACK_L);
			m_pGridIo.SetItemFont(i + 3, 2, &clsFunc.OnLogFont(20));
			m_pGridIo.SetItemText(i + 3, 2, _T("Output"));

			m_pGridIo.SetItemBkColour(i + 3, 3,  RGB(242, 220, 220), BLACK_C);
			m_pGridIo.SetItemFont(i + 3, 3, &clsFunc.OnLogFont(20));
			m_pGridIo.SetItemText(i + 3, 3, _T("N"));

			m_pGridIo.SetItemBkColour(i + 3, 4, GREEN_C, BLACK_L);
			m_pGridIo.SetItemFont(i + 3, 4, &clsFunc.OnLogFont(20));
			m_pGridIo.SetItemText(i + 3, 4, _T("ON"));

			m_pGridIo.SetItemBkColour(i + 3, 5, RGB(202, 255, 185), BLACK_L);
			m_pGridIo.SetItemFont(i + 3, 5, &clsFunc.OnLogFont(16));
//			m_pGridIo.SetItemText(i + 3, 5, clsExcel.m_strIOBitLabel[nMaster][nPort][nSlave][i]);
			m_pGridIo.SetItemText(i + 3, 5, clsExcel.m_strIOBitDescrip[nMaster][nPort][nSlave][i]);
		}
		else
		{
			m_pGridIo.SetItemBkColour(i + 3, 1, RGB(242, 242, 242), BLACK_L);
			m_pGridIo.SetItemFont(i + 3, 1, &clsFunc.OnLogFont(20));
			m_pGridIo.SetItemText(i + 3, 1, _T("-"));

			m_pGridIo.SetItemBkColour(i + 3, 2, RGB(242, 242, 242), BLACK_L);
			m_pGridIo.SetItemFont(i + 3, 2, &clsFunc.OnLogFont(20));
			m_pGridIo.SetItemText(i + 3, 2, _T("-"));

			m_pGridIo.SetItemBkColour(i + 3, 3,  RGB(242, 242, 242), BLACK_L);
			m_pGridIo.SetItemFont(i + 3, 3, &clsFunc.OnLogFont(20));
			m_pGridIo.SetItemText(i + 3, 3, _T("-"));

			m_pGridIo.SetItemBkColour(i + 3, 4, RGB(242, 242, 242), BLACK_L);
			m_pGridIo.SetItemFont(i + 3, 4, &clsFunc.OnLogFont(20));
			m_pGridIo.SetItemText(i + 3, 4, _T("-"));

			m_pGridIo.SetItemBkColour(i + 3, 5, RGB(242, 242, 242), BLACK_L);
			m_pGridIo.SetItemFont(i + 3, 5, &clsFunc.OnLogFont(20));
			m_pGridIo.SetItemText(i + 3, 5, _T("-"));
		}
	}

	m_pGridIo.Invalidate(FALSE);
}


void CScreenIO::OnSelchangedTreeLoList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	if(st_handler_info.nMenuLock) return;

	int i, j, k;
	bool bfind;
	CString strTmp;
/*
	// Tree 최상위 정보 가져오기
	HTREEITEM a = m_treeIoList.GetRootItem();
	strTmp = m_treeIoList.GetItemText(a);

	// Tree 바로 위 정보 가져오기
	HTREEITEM b = m_treeIoList.GetParentItem(pNMTreeView->itemNew.hItem);
	strTmp = m_treeIoList.GetItemText(b);
*/
	strTmp = m_treeIoList.GetItemText(pNMTreeView->itemNew.hItem);

	bfind = false;
	for (i=0; i<3; i++)
	{
		for (j=0; j<4; j++)
		{
			for (k=0; k<clsExcel.m_nIOPort[i][j]+1; k++)
			{
				if (strTmp == clsExcel.m_strIOPortLabel[i][j][k])
				{
					bfind = true;

					if (m_nIOMaster != i || m_nIOPort != j || m_nIOSlave != k)
					{
						m_nIOMaster = i;
						m_nIOPort	= j;
						m_nIOSlave	= k;

						OnDisplayIO(m_nIOMaster, m_nIOPort, m_nIOSlave);
					}
					break;
				}
			}
			if(bfind) break;
		}
		if(bfind) break;
	}
	*pResult = 0;
}


void CScreenIO::OnInitGroupBox()
{
	m_groupIoList.SetFont(clsFunc.OnLogFont(16));
	m_groupIoList.SetCatptionTextColor(BLUE_C);
	m_groupIoList.SetBorderColor(ORANGE_C);
	m_groupIoList.SetFontBold(TRUE);
	m_groupIoList.SetBackgroundColor(WINDOW_UP);

	m_groupAction.SetFont(clsFunc.OnLogFont(16));
	m_groupAction.SetCatptionTextColor(BLUE_C);
	m_groupAction.SetBorderColor(ORANGE_C);
	m_groupAction.SetFontBold(TRUE);
	m_groupAction.SetBackgroundColor(WINDOW_UP);
}

void CScreenIO::OnInitButton()
{
	m_btnEmergency.SetBitmaps(IDC_BTN_RELOAD, IDB_BITMAP_EMERGENCY_DN, WINDOW_DN, IDB_BITMAP_EMERGENCY_UP, WINDOW_UP);
	m_btnEmergency.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnEmergency.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnEmergency.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnEmergency.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnEmergency.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnEmergency.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnEmergency.SetFont(clsFunc.m_pFont[3]);
	m_btnEmergency.SetTooltipText(_T("IO Action Emergency Stop"));

	m_btnAction1.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnAction1.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnAction1.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnAction1.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnAction1.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnAction1.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnAction1.SetFont(clsFunc.m_pFont[1]);

	m_btnAction10.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnAction10.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnAction10.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnAction10.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnAction10.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnAction10.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnAction10.SetFont(clsFunc.m_pFont[1]);

	m_btnAction100.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnAction100.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnAction100.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnAction100.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnAction100.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnAction100.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnAction100.SetFont(clsFunc.m_pFont[1]);

	OnIoActionMode(0);
}

void CScreenIO::OnInitRadio()
{

}

void CScreenIO::OnInitDigit()
{
	m_dgtWaitTime.SetStyle(CDigit::DS_INT, 7, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	m_dgtWaitTime.SetVal(m_nContinueWaitTime);
}


void CScreenIO::OnCellClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	int nRow, nCol;
	int nBit;
	int nCount;
	int i;

	CString strTmp;
	CString strMsg;

	NM_GRIDVIEW* pItem	= (NM_GRIDVIEW*) pNotifyStruct;

	nRow				= pItem->iRow;
	nCol				= pItem->iColumn;

	// Header Click
	if (nRow < 2) return;
	
	switch (nCol)
	{
		case 3:
			nBit = nRow - 3;

			if (clsExcel.m_nIOBitType[m_nIOMaster][m_nIOPort][m_nIOSlave][nBit] == IO_OUT)
			{
				strTmp = m_pGridIo.GetItemText(nRow, nCol);

				if (strTmp == "N")
				{
					m_pGridIo.SetItemBkColour(nRow, nCol, RGB(245,130, 32), BLACK_C);  
					m_pGridIo.SetItemText(nRow, nCol, _T("Y"));  
				}
				else
				{
					m_pGridIo.SetItemBkColour(nRow, nCol, RGB(242, 220, 220), BLACK_C);  
					m_pGridIo.SetItemText(nRow, nCol, _T("N"));  
				}
			}
			break;

		case 4:
			nBit = nRow - 3;
			if(m_nIOActionCount[0] == 1)
			{
				if (clsExcel.m_nIOBitType[m_nIOMaster][m_nIOPort][m_nIOSlave][nBit] != IO_OUT) return;

				if (clsFunc.OnIoSafetyCheck(clsExcel.m_nIOBitNum[m_nIOMaster][m_nIOPort][m_nIOSlave][nBit]) == RET_ERROR) return;

				
				if(FAS_IO.get_out_bit(clsExcel.m_nIOBitNum[m_nIOMaster][m_nIOPort][m_nIOSlave][nBit], IO_OFF) == IO_ON)
				{
					FAS_IO.set_out_bit(clsExcel.m_nIOBitNum[m_nIOMaster][m_nIOPort][m_nIOSlave][nBit], IO_OFF);

					strMsg.Format(_T("Master :[%d] Port : [%d] Slave : [%d] Bit : [%d] Action : OFF"), m_nIOMaster,
																									   m_nIOPort,
																									   m_nIOSlave,
																									   nBit + 1);
				}
				else
				{
					FAS_IO.set_out_bit(clsExcel.m_nIOBitNum[m_nIOMaster][m_nIOPort][m_nIOSlave][nBit], IO_ON);

					strMsg.Format(_T("Master :[%d] Port : [%d] Slave : [%d] Bit : [%d] Action : ON"), m_nIOMaster,
																									  m_nIOPort,
																									  m_nIOSlave,
																									  nBit + 1);
				}

				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
				{
					//					sprintf(st_other_info.cNormalMsg, strMsg);
					//					_snprintf(st_other_info.cNormalMsg, sizeof(st_other_info.cNormalMsg), "%s", strMsg);
					//wsprintfA(st_other_info.cNormalMsg, "%S", strMsg);
					//clsFunc.OnStringToChar(strMsg, st_other_info.cNormalMsg);
					clsMem.OnNormalMessageWrite(strMsg);
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
				}
			}
			else
			{	
				nCount = 0;
				memset(m_nOutputInfo, -1, sizeof(m_nOutputInfo));

				for(i=0; i<16; i++)
				{
					if (clsExcel.m_nIOBitType[m_nIOMaster][m_nIOPort][m_nIOSlave][i] == IO_OUT)
					{
						strTmp = m_pGridIo.GetItemText(i + 3, 3);

						if (strTmp == _T("Y"))
						{
							nCount++;
							m_nOutputInfo[i] = YES;
						}
						else
						{
							m_nOutputInfo[i] = NO;
						}
					}
				}

				m_bIOAction = false;
				if (nCount > 0)
				{
					if (!st_handler_info.nMenuLock)
					{
						st_handler_info.nMenuLock = TRUE;
					}

					m_bIOAction			= true;
					m_nIOActionCount[1]	= 0;
					m_nIOActionStep		= 0;
					m_bEmergency		= FALSE;
					SetTimer(TMR_IO_CONTINUE, 100, NULL);	// I/O 상태 출력 타이머 생성
				}
			}
			break;
	}

	m_pGridIo.Invalidate(FALSE);
}


void CScreenIO::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int nRet;
	int i;

	CString strTmp;

	switch (nIDEvent)
	{
		case TMR_IO_INPUT:
			OnIoInputCheck();
			break;

		case TMR_IO_OUTPUT:
			OnIoOutputCheck();
			break;

		case TMR_IO_CONTINUE:
			nRet = OnIoCylinderExcution();
			
			if (nRet == RET_GOOD)
			{
				KillTimer(TMR_IO_CONTINUE);		// 타이머 삭제

				if (st_handler_info.nMenuLock)
				{
					st_handler_info.nMenuLock = FALSE;
				}
				
				m_nIOActionCount[0] = 1;
				m_nIOActionCount[1] = 0;

				m_nActionMode		= 0;
				OnIoActionMode(m_nActionMode);

				for(i=0; i<16; i++)
				{
					if (clsExcel.m_nIOBitType[m_nIOMaster][m_nIOPort][m_nIOSlave][i] == IO_OUT)
					{
						strTmp = m_pGridIo.GetItemText(i + 3, 3);

						if (strTmp == _T("Y"))
						{
							m_pGridIo.SetItemBkColour(i+3, 3, RGB(242, 220, 220), BLACK_C);  
							m_pGridIo.SetItemText(i+3, 3, _T("N"));  
						}
					}
				}
			}
			else if(nRet == RET_ERROR)
			{
				KillTimer(TMR_IO_CONTINUE);		// 타이머 삭제

				if (st_handler_info.nMenuLock)
				{
					st_handler_info.nMenuLock = FALSE;
				}
				
				m_nIOActionCount[0] = 1;
				m_nIOActionCount[1] = 0;
				
				m_nActionMode		= 0;
				OnIoActionMode(m_nActionMode);

				for(i=0; i<16; i++)
				{
					if (clsExcel.m_nIOBitType[m_nIOMaster][m_nIOPort][m_nIOSlave][i] == IO_OUT)
					{
						strTmp = m_pGridIo.GetItemText(i + 3, 3);

						if (strTmp == _T("Y"))
						{
							m_pGridIo.SetItemBkColour(i + 3, 3, RGB(242, 220, 220), BLACK_C);  
							m_pGridIo.SetItemText(i + 3, 3, _T("N"));  
						}
					}
				}

				UpdateData(FALSE);
			}
			break;
	}
	CFormView::OnTimer(nIDEvent);
}

void CScreenIO::OnIoOutputCheck()
{
	int i;

	for(i=0; i<16; i++)
	{
		if (clsExcel.m_nIOBitType[m_nIOMaster][m_nIOPort][m_nIOSlave][i] == IO_OUT)
		{
			if (FAS_IO.get_out_bit(clsExcel.m_nIOBitNum[m_nIOMaster][m_nIOPort][m_nIOSlave][i], IO_OFF) == IO_ON)
			{
				m_pGridIo.SetItemText(i+ 3 , 4, _T("ON"));
				m_pGridIo.SetItemBkColour(i + 3, 4, GREEN_C, BLACK_C);
			}
			else
			{
				m_pGridIo.SetItemText(i + 3, 4, _T("OFF"));
				m_pGridIo.SetItemBkColour(i + 3, 4, GREEN_D, BLACK_C);
			}
			m_pGridIo.Invalidate(FALSE);
		}
	}
}

void CScreenIO::OnIoInputCheck()
{
	int i;

	for(i=0; i<16; i++)
	{
		if (clsExcel.m_nIOBitType[m_nIOMaster][m_nIOPort][m_nIOSlave][i] == IO_IN)
		{
			if (FAS_IO.get_in_bit(clsExcel.m_nIOBitNum[m_nIOMaster][m_nIOPort][m_nIOSlave][i], IO_OFF) == IO_ON)
			{
				m_pGridIo.SetItemText(i + 3, 4, _T("ON"));
				m_pGridIo.SetItemBkColour(i + 3, 4, YELLOW_C, BLACK_C);
			}
			else
			{
				m_pGridIo.SetItemText(i + 3, 4, _T("OFF"));
				m_pGridIo.SetItemBkColour(i + 3, 4, YELLOW_D, BLACK_C);
			}
			m_pGridIo.Invalidate(FALSE);
		}
	}
}


void CScreenIO::OnClickedBtnEmergency()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_bEmergency)
	{
		m_bEmergency = true;
	}
}


int CScreenIO::OnIoCylinderExcution()
{
	CString strMsg;
	int nRet = RET_PROCEED;
	int i;

	// **************************************************************************
	// 모터 동작 중 ESTOP 요청 발생했는지 검사한다                               
	// -> ESTOP 요청 발생한 경우 긴급 정지로 모터 멈춘다                         
	// **************************************************************************
	if(m_bEmergency)
	{
		if (m_nIOActionStep < 1000)
		{
			m_nIOActionStep = 1000;
		}
	}
	// **************************************************************************


	switch (m_nIOActionStep)
	{
		case 0:
			for (i=0; i<16; i++)
			{
				if (clsExcel.m_nIOBitType[m_nIOMaster][m_nIOPort][m_nIOSlave][i] == IO_OUT && m_nOutputInfo[i] == YES)
				{
					if (clsFunc.OnIoSafetyCheck(clsExcel.m_nIOBitNum[m_nIOMaster][m_nIOPort][m_nIOSlave][i]) == RET_GOOD)
					{
						if (FAS_IO.get_out_bit(clsExcel.m_nIOBitNum[m_nIOMaster][m_nIOPort][m_nIOSlave][i], IO_OFF) == IO_ON)
						{
							FAS_IO.set_out_bit(clsExcel.m_nIOBitNum[m_nIOMaster][m_nIOPort][m_nIOSlave][i], IO_OFF);
						}
						else
						{
							FAS_IO.set_out_bit(clsExcel.m_nIOBitNum[m_nIOMaster][m_nIOPort][m_nIOSlave][i], IO_ON);
						}
					}
				}
			}

			if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
			{
				strMsg.Format(_T("[%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d] Word Output"), 
							  m_nOutputInfo[0],		m_nOutputInfo[1],	m_nOutputInfo[2],	m_nOutputInfo[3],
							  m_nOutputInfo[4],		m_nOutputInfo[5],	m_nOutputInfo[6],	m_nOutputInfo[7],
							  m_nOutputInfo[8],		m_nOutputInfo[9],	m_nOutputInfo[10],	m_nOutputInfo[11],
							  m_nOutputInfo[12],	m_nOutputInfo[13],	m_nOutputInfo[14],	m_nOutputInfo[15]);


				clsMem.OnNormalMessageWrite(strMsg);
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}

			m_lWaitTime[0]	= GetTickCount();
			m_nIOActionStep = 100;
			break;

		case 100:
			// *************************************************************
			//  잠시 대기후 Retry 진행                                      
			// *************************************************************
			m_lWaitTime[1] = GetTickCount();

			m_lWaitTime[2] = m_lWaitTime[1] - m_lWaitTime[0];
			if (m_lWaitTime[2] < 0)
			{
				m_lWaitTime[0] = GetTickCount();
				break;
			}

			if (m_lWaitTime[2] > m_nContinueWaitTime)
			{
				m_nIOActionStep = 200;
			}
			break;

		case 200:
			for (i=0; i<16; i++)
			{
				if (clsExcel.m_nIOBitType[m_nIOMaster][m_nIOPort][m_nIOSlave][i] == IO_OUT && m_nOutputInfo[i] == YES)
				{
					if (clsFunc.OnIoSafetyCheck(clsExcel.m_nIOBitNum[m_nIOMaster][m_nIOPort][m_nIOSlave][i]) == RET_GOOD)
					{
						if (FAS_IO.get_out_bit(clsExcel.m_nIOBitNum[m_nIOMaster][m_nIOPort][m_nIOSlave][i], IO_OFF) == IO_ON)
						{
							FAS_IO.set_out_bit(clsExcel.m_nIOBitNum[m_nIOMaster][m_nIOPort][m_nIOSlave][i], IO_OFF);
						}
						else
						{
							FAS_IO.set_out_bit(clsExcel.m_nIOBitNum[m_nIOMaster][m_nIOPort][m_nIOSlave][i], IO_ON);
						}
					}
				}
			}

			if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
			{
				strMsg.Format(_T("[%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d] Word Output"), 
							  m_nOutputInfo[0],		m_nOutputInfo[1],	m_nOutputInfo[2],	m_nOutputInfo[3],
							  m_nOutputInfo[4],		m_nOutputInfo[5],	m_nOutputInfo[6],	m_nOutputInfo[7],
							  m_nOutputInfo[8],		m_nOutputInfo[9],	m_nOutputInfo[10],	m_nOutputInfo[11],
							  m_nOutputInfo[12],	m_nOutputInfo[13],	m_nOutputInfo[14],	m_nOutputInfo[15]);

				clsMem.OnNormalMessageWrite(strMsg);
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}

			m_lWaitTime[0]	= GetTickCount();
			m_nIOActionStep = 300;
			break;

		case 300:
			// *************************************************************
			//  잠시 대기후 Retry 진행                                      
			// *************************************************************
			m_lWaitTime[1] = GetTickCount();

			m_lWaitTime[2] = m_lWaitTime[1] - m_lWaitTime[0];
			if (m_lWaitTime[2] < 0)
			{
				m_lWaitTime[0] = GetTickCount();
				break;
			}

			if (m_lWaitTime[2] > m_nContinueWaitTime)
			{
				m_nIOActionStep = 400;
			}
			break;

		case 400:	
			m_nIOActionCount[1]++;		

			if(m_nIOActionCount[1] > m_nIOActionCount[0])
			{
				m_nIOActionStep = 500;
			}
			else
			{
				m_nIOActionStep = 0;
			}

			break;

		case 500:
			if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
			{
				strMsg = _T("[Repeat action] The action was completed.!");

				//clsFunc.OnStringToChar(strMsg, st_other_info.cNormalMsg);
				clsMem.OnNormalMessageWrite(strMsg);
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
			nRet = RET_GOOD;
			break;

		case 1000:
			if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
			{
				strMsg = _T("A Repeat action became the cancel.");

				//clsFunc.OnStringToChar(strMsg, st_other_info.cNormalMsg);
				clsMem.OnAbNormalMessagWrite(strMsg);
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG); 
			}
			nRet = RET_GOOD;

			if(m_bEmergency)
			{
				m_bEmergency = FALSE;  // ESTOP 요청 플래그 초기화
			}
			break;
	}

	return nRet;
}


BOOL CScreenIO::OnEraseBkgnd(CDC* pDC)
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

//void CScreenIO::OnRadAction1()
//{
//	// TODO: 여기에 명령 처리기 코드를 추가합니다.
//	if (st_handler_info.nMenuLock)
//	{
//		switch (m_nIOActionCount[0])
//		{
//			case 1:
//				m_nActionMode = 0;
//				break;
//
//			case 10:
//				m_nActionMode = 1;
//				break;
//
//			case 100:
//				m_nActionMode = 2;
//				break;
//		}
//		UpdateData(FALSE);
//		return;
//	}
//
//	m_nIOActionCount[0]	= 1;
//	OnIoActionMode(0);
//}


void CScreenIO::OnClickedCheckAction1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (st_handler_info.nMenuLock)
	{
		OnIoActionMode(m_nActionMode);
		return;
	}

	m_nIOActionCount[0]	= 1;
	OnIoActionMode(0);
}


void CScreenIO::OnClickedCheckAction10()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (st_handler_info.nMenuLock)
	{
		OnIoActionMode(m_nActionMode);
		return;
	}

	m_nIOActionCount[0]	= 10;
	OnIoActionMode(1);
}


void CScreenIO::OnClickedCheckAction100()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (st_handler_info.nMenuLock)
	{
		OnIoActionMode(m_nActionMode);
		return;
	}

	m_nIOActionCount[0]	= 100;
	OnIoActionMode(2);
}


void CScreenIO::OnClickedDgtWaitTime()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nKey = m_nContinueWaitTime;

	KeyPadI(10, 5000, &nKey);

	m_nContinueWaitTime = nKey;
	m_dgtWaitTime.SetVal(m_nContinueWaitTime);
}


void CScreenIO::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	/*	KillTimer(TMR_IO_INPUT);
	KillTimer(TMR_IO_OUTPUT);

	if (m_bIOAction)
	{
		KillTimer(TMR_IO_CONTINUE);
	}

	if (m_pImagelist != NULL)
	{
		delete m_pImagelist;
		m_pImagelist = NULL;
	}

	st_handler_info.cWndIO = NULL;*/
	CFormView::PostNcDestroy();
}


void CScreenIO::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	KillTimer(TMR_IO_INPUT);
	KillTimer(TMR_IO_OUTPUT);

	if (m_bIOAction)
	{
		KillTimer(TMR_IO_CONTINUE);
	}

	if (m_pImagelist != NULL)
	{
		delete m_pImagelist;
		m_pImagelist = NULL;
	}

	st_handler_info.cWndIO = NULL;
}
