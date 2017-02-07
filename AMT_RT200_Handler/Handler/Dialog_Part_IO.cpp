// Dialog_Part_IO.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Handler.h"
#include "Dialog_Part_IO.h"
#include "afxdialogex.h"
#include "Alg_Excel.h"
#include "Variable.h"
#include "PublicFunction.h"
#include "FastechPublic_IO.h"


// CDialog_Part_IO 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialog_Part_IO, CDialog)

CDialog_Part_IO::CDialog_Part_IO(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog_Part_IO::IDD, pParent)
{

}

CDialog_Part_IO::~CDialog_Part_IO()
{
}

void CDialog_Part_IO::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TREE_lO_LIST,			m_treePartIoList);
	DDX_Control(pDX, IDC_CUSTOM_INPUT_PART_IO,	m_pGridInPutIo);
	DDX_Control(pDX, IDC_CUSTOM_OUTPUT_PART_IO,	m_pGridOutPutIo);
	DDX_Control(pDX, IDC_BTN_PART_IO_EXIT,		m_btnPartIoExit);
}


BEGIN_MESSAGE_MAP(CDialog_Part_IO, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_lO_LIST, &CDialog_Part_IO::OnSelchangedTreeMotorList)
	ON_NOTIFY(NM_CLICK, IDC_CUSTOM_INPUT_PART_IO, OnCellInputClick)
	ON_NOTIFY(NM_CLICK, IDC_CUSTOM_OUTPUT_PART_IO, OnCellOutputClick)
	ON_BN_CLICKED(IDC_BTN_PART_IO_EXIT, &CDialog_Part_IO::OnBnClickedBtnPartIoExit)
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDialog_Part_IO 메시지 처리기입니다.

BOOL CDialog_Part_IO::OnInitDialog()
{
	CDialog::OnInitDialog();

	OnInitButton();
	OnInitTree();
	OnInputInitGrid();
	OnOutputInitGrid();
	OnInitGridDataShow();

	SetTimer(TMR_PART_IO_INPUT_UPDATE, 100, NULL);
	return TRUE;  
}


void CDialog_Part_IO::OnSelchangedTreeMotorList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	if(st_handler_info.nMenuLock) return;

	int nPos;
//	bool bfind;
	CString strTmp, sInOutPortName;
	CString strFileName;

	strTmp = m_treePartIoList.GetItemText(pNMTreeView->itemNew.hItem);

	nPos = strTmp.Find(_T(":"), 0);

	if(nPos != -1)
	{
		sInOutPortName = strTmp.Mid(0,nPos);
		strTmp = strTmp.Mid(nPos+1, nPos+2);

		if(sInOutPortName == _T("INPUT_PORT"))	{
			nPartNo[0] = clsExcel.m_nMotorSelectPartNum;
			nPortNo[0] = _wtoi(strTmp);

			nInOut = 0;
			OnInputInitGrid();
			OnDisplayInputIO();
		}
		else {
			nPartNo[1] = clsExcel.m_nMotorSelectPartNum;
			nPortNo[1] = _wtoi(strTmp);

			nInOut = 1;
			OnOutputInitGrid();
			OnDisplayOutputIO();
		}	
	}
}

void CDialog_Part_IO::OnInitGridDataShow()
{
	nPartNo[0] = clsExcel.m_nMotorSelectPartNum;
	nInOut = 0;
	nPortNo[0] = clsExcel.m_nFirstPortNo[nPartNo[0]][nInOut][0];
	OnInputInitGrid();
	OnDisplayInputIO();

	nPartNo[1] = clsExcel.m_nMotorSelectPartNum;
	nInOut = 1;
	nPortNo[1] = clsExcel.m_nFirstPortNo[nPartNo[1]][nInOut][0];
	OnOutputInitGrid();
	OnDisplayOutputIO();
}

void CDialog_Part_IO::OnInitButton()
{
	m_btnPartIoExit.SetBitmaps(IDC_BTN_PART_IO_EXIT, IDB_BITMAP_APPLY_DN, WINDOW_DN, IDB_BITMAP_APPLY_UP, WINDOW_UP);
	m_btnPartIoExit.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnPartIoExit.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnPartIoExit.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnPartIoExit.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnPartIoExit.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnPartIoExit.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnPartIoExit.SetFont(clsFunc.m_pFont[3]);
	m_btnPartIoExit.SetTooltipText(_T("Part IO Exit"));
}

void CDialog_Part_IO::OnInitTree()
{
	int i, k, l, m;
	int nOldPort = -1;
	int nTmp = 0;
	CString strTmp;
	CString strInOut[2] = {_T("INPUT"),_T("OUTPUT")};

	CBitmap bmp;
	m_pImagelist = new CImageList;
	m_pImagelist->Create(24, 24, ILC_COLOR32|ILC_MASK,0, -1);
	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_HIRFOLDER));//12
	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_HIOPEN));//7
	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_HICLOSE));//6
/*	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_HINEW));//0 %%0부터 시작한다 주의
	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_HIFIND));//1
	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_HISAVE1));//2
	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_HIDELETE));//3

	//bmp.LoadBitmap(IDB_LIST);
	//m_pImagelist->Add(&bmp,0xEFEFEF);bmp.DeleteObject();//4
	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_READ));;//4
	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_HIPRINT2));;//5
	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_HICLOSE));//6
	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_HIOPEN));//7
	bmp.LoadBitmap(IDB_SEARCH);
	m_pImagelist->Add(&bmp,0xEFEFEF);bmp.DeleteObject();//8
	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_BOOK));//9

	bmp.LoadBitmap(IDB_PREVIEW);
	m_pImagelist->Add(&bmp,0xEFEFEF);bmp.DeleteObject();//10
	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_CLEAR));//11
	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_HIRFOLDER));//12
	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_HIFILE));//13
	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_HISEARCH));//14*/


	m_treePartIoList.SetImageList(m_pImagelist, TVSIL_NORMAL);
	m_treePartIoList.DeleteAllItems();

	TV_INSERTSTRUCT tvstruct;
	HTREEITEM item[3]={NULL,NULL,NULL};//아이템 핸들러 3개 초기화

	strTmp.Format(_T("%s"), clsExcel.m_strPartName[clsExcel.m_nMotorSelectPartNum]);

	tvstruct.hParent				= 0;
	tvstruct.hInsertAfter			= TVI_LAST;
	tvstruct.item.pszText			= (LPWSTR)(LPCTSTR)strTmp;
	tvstruct.item.mask				= TVIF_TEXT | TVIF_IMAGE |TVIF_SELECTEDIMAGE ;
	tvstruct.item.iImage			= 0;
	tvstruct.item.iSelectedImage	= 0;
	item[0]							= m_treePartIoList.InsertItem(&tvstruct);

	for(i=0; i<clsExcel.m_nPartCnt; i++)
	{
		if(i == clsExcel.m_nMotorSelectPartNum/*모터 화면에서 선택 Part*/)
		{
			for(k=0; k<2; k++) 
			{
				tvstruct.hParent				= item[0];
				tvstruct.hInsertAfter			= TVI_LAST;
				tvstruct.item.pszText			= (LPWSTR)(LPCTSTR)strInOut[k];
				tvstruct.item.mask				= TVIF_TEXT | TVIF_IMAGE |TVIF_SELECTEDIMAGE ;
				tvstruct.item.iImage			= 0;
				tvstruct.item.iSelectedImage	= 0;
				item[1]							= m_treePartIoList.InsertItem(&tvstruct);

				for(l=0; l<60; l++)
				{
					for(m=0; m<16; m++)
					{
						if(clsExcel.m_sPartInOutPutName[i+1][k][l][m] != "")
						{
							if(nOldPort != l)
							{
								nTmp++;
								nOldPort = l;

								if(k == 0)	{
									strTmp.Format(_T("INPUT_PORT:%d"), nOldPort);
								}
								else	{
									strTmp.Format(_T("OUTPUT_PORT:%d"), nOldPort);
								}

								tvstruct.hParent				=	item[1];
								tvstruct.hInsertAfter			=	TVI_LAST;
								tvstruct.item.pszText			=	(LPWSTR)(LPCTSTR) strTmp;
								tvstruct.item.mask				=	TVIF_TEXT| TVIF_IMAGE |TVIF_SELECTEDIMAGE ;
								tvstruct.item.iImage			=	2;
								tvstruct.item.iSelectedImage	=	1;
								item[2]							= m_treePartIoList.InsertItem(&tvstruct);

								m_treePartIoList.EnsureVisible(item[2]);
							
							}
						}
					}
				}
				nTmp = 0;
			}
		}
	}
}

void CDialog_Part_IO::OnInputInitGrid()
{
	CString strTmp;
	int i, j;

	m_pGridInPutIo.SetFrameFocusCell(FALSE);
	m_pGridInPutIo.SetTrackFocusCell(FALSE);
	m_pGridInPutIo.EnableSelection(FALSE);

	m_pGridInPutIo.SetGridLineColor(BLACK_C);
	m_pGridInPutIo.SetGridLines(1);

	m_pGridInPutIo.SetRowCount(17);
	m_pGridInPutIo.SetColumnCount(3);

	m_pGridInPutIo.SetFixedRowCount(0);
	m_pGridInPutIo.SetFixedColumnCount(0);
	m_pGridInPutIo.SetFixedBkColor(RGB(0,0,200));
	m_pGridInPutIo.SetFixedBkColor(RGB(200,200,255));
	m_pGridInPutIo.SetTextBkColor(RGB(150,150,200));

	for (i=0; i<17; i++) 
	{
		m_pGridInPutIo.SetRowHeight(i, 30);

		for (j=0; j<3; j++) 
		{
			switch (j)
			{
			case 0:
				m_pGridInPutIo.SetColumnWidth(j, 50);
				break;

			case 1:
				m_pGridInPutIo.SetColumnWidth(j, 80);
				break;

			case 2:
				m_pGridInPutIo.SetColumnWidth(j, 330);
				break;
			}

			m_pGridInPutIo.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_pGridInPutIo.SetItemState(i, j, GVIS_READONLY);
			m_pGridInPutIo.SetItemBkColour(i, j, WHITE_C, CLR_DEFAULT);
		}
	}

	// IO MAP Header ////////////////////////////////////////////////////////////////////////////////////////////// 
 	m_pGridInPutIo.MergeCells(0, 0, 0, 2);
	m_pGridInPutIo.SetItemBkColour(0, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridInPutIo.SetItemFont(0, 0, &clsFunc.OnLogFont(35));
	m_pGridInPutIo.SetItemText(0, 0, _T("Part I/O Input Sheet"));

	for (i=0; i<16; i++) 
	{
		strTmp.Format(_T("%d"), i + 1);
		m_pGridInPutIo.SetItemBkColour(i + 1, 0, RGB(79, 79, 225), WHITE_C);
		m_pGridInPutIo.SetItemFont(i + 1, 0, &clsFunc.OnLogFont(10));
		m_pGridInPutIo.SetItemText(i + 1, 0, strTmp);
	}
}

void CDialog_Part_IO::OnOutputInitGrid()
{
	CString strTmp;
	int i, j;

	m_pGridOutPutIo.SetFrameFocusCell(FALSE);
	m_pGridOutPutIo.SetTrackFocusCell(FALSE);
	m_pGridOutPutIo.EnableSelection(FALSE);
	
	m_pGridOutPutIo.SetGridLineColor(BLACK_C);
	m_pGridOutPutIo.SetGridLines(1);
	
	m_pGridOutPutIo.SetRowCount(17);
	m_pGridOutPutIo.SetColumnCount(3);
	
	m_pGridOutPutIo.SetFixedRowCount(0);
	m_pGridOutPutIo.SetFixedColumnCount(0);
	m_pGridOutPutIo.SetFixedBkColor(RGB(0,0,200));
	m_pGridOutPutIo.SetFixedBkColor(RGB(200,200,255));
	m_pGridOutPutIo.SetTextBkColor(RGB(150,150,200));

	for (i=0; i<17; i++) 
	{
		m_pGridOutPutIo.SetRowHeight(i, 30);

		for (j=0; j<3; j++) 
		{
			switch (j)
			{
			case 0:
				m_pGridOutPutIo.SetColumnWidth(j, 50);
				break;

			case 1:
				m_pGridOutPutIo.SetColumnWidth(j, 80);
				break;

			case 2:
				m_pGridOutPutIo.SetColumnWidth(j, 330);
				break;
			}

			m_pGridOutPutIo.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_pGridOutPutIo.SetItemState(i, j, GVIS_READONLY);
			m_pGridOutPutIo.SetItemBkColour(i, j, WHITE_C, CLR_DEFAULT);
		}
	}

	// IO MAP Header ////////////////////////////////////////////////////////////////////////////////////////////// 
	m_pGridOutPutIo.MergeCells(0, 0, 0, 2);
	m_pGridOutPutIo.SetItemBkColour(0, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridOutPutIo.SetItemFont(0, 0, &clsFunc.OnLogFont(35));
	m_pGridOutPutIo.SetItemText(0, 0, _T("Part I/O Output Sheet"));

	for (i=0; i<16; i++) 
	{
		strTmp.Format(_T("%d"), i + 1);
		m_pGridOutPutIo.SetItemBkColour(i + 1, 0, RGB(79, 79, 225), WHITE_C);
		m_pGridOutPutIo.SetItemFont(i + 1, 0, &clsFunc.OnLogFont(10));
		m_pGridOutPutIo.SetItemText(i + 1, 0, strTmp);
	}
}

void CDialog_Part_IO::OnDisplayInputIO()
{
	int i=0, j=0;
	int nRtn;

	CString strTmp;

	strTmp.Format(_T("Port : [%d] Slave : [%d]"), nPartNo, nPortNo);

	for (;;) 
	{
		i++;
		if(clsExcel.m_sPartInOutPutName[nPartNo[0]+1][0][nPortNo[0]][i-1] != _T(""))
		{
			j++;
			// input

			m_pGridInPutIo.SetItemBkColour(j, 2, RGB(255, 255, 153), BLACK_L);
			m_pGridInPutIo.SetItemFont(j, 2, &clsFunc.OnLogFont(15));
			m_pGridInPutIo.SetItemText(j, 2, clsExcel.m_sPartInOutPutName[nPartNo[0]+1][0][nPortNo[0]][i-1]);

			nInPartNo[j] = nPartNo[0];
			nInPotrNo[j] = nPortNo[0];
			nInBitNo[j] = clsExcel.m_nPartAccBittNo[nInPartNo[j]+1][0][nInPotrNo[j]][j-1];

			nRtn = FAS_IO.get_in_bit((nInPotrNo[j] * 100) + nInBitNo[j], IO_ON);
			if(nRtn == IO_ON) 
			{
				m_pGridInPutIo.SetItemBkColour(j, 1, GREEN_C, BLACK_L);
				m_pGridInPutIo.SetItemFont(j, 1, &clsFunc.OnLogFont(20));
				m_pGridInPutIo.SetItemText(j, 1, _T("ON"));
			}
			else
			{
				m_pGridInPutIo.SetItemBkColour(j, 1, YELLOW_C, BLACK_L);
				m_pGridInPutIo.SetItemFont(j, 1, &clsFunc.OnLogFont(20));
				m_pGridInPutIo.SetItemText(j, 1, _T("OFF"));
			}
		}
			
		if(i >= 16/*clsExcel.m_nPartInOutTPutPortCnt[nPartNo+1][0][nPortNo]+1*/)
		{
			for(int k=j+1; k<i; k++) {
				m_pGridInPutIo.SetItemBkColour(k, 1, DSERBLE_BC, BLACK_L);
				m_pGridInPutIo.SetItemFont(k, 1, &clsFunc.OnLogFont(20));
				m_pGridInPutIo.SetItemText(k, 1, _T(""));

				m_pGridInPutIo.SetItemBkColour(k, 2, DSERBLE_BC, BLACK_L);
				m_pGridInPutIo.SetItemFont(k, 2, &clsFunc.OnLogFont(15));
				m_pGridInPutIo.SetItemText(k, 2, _T(""));
			}
			break;
		}
	}
	m_pGridInPutIo.Invalidate(FALSE);
}

void CDialog_Part_IO::OnDisplayOutputIO()
{
	int i = 0, j = 0;
	int nRtn = -1;

	CString strTmp;

	strTmp.Format(_T("Port : [%d] Slave : [%d]"), nPartNo, nPortNo);

	for (;;) 
	{
		i++;
		if(clsExcel.m_sPartInOutPutName[nPartNo[1]+1][1][nPortNo[1]][i-1] != _T(""))
		{
			j++;
			// input
			m_pGridOutPutIo.SetItemBkColour(j, 2, RGB(202, 255, 185), BLACK_L);
			m_pGridOutPutIo.SetItemFont(j, 2, &clsFunc.OnLogFont(15));
			m_pGridOutPutIo.SetItemText(j, 2, clsExcel.m_sPartInOutPutName[nPartNo[1]+1][1][nPortNo[1]][i-1]);

			nOutPartNo[j] = nPartNo[1];
			nOutPotrNo[j] = nPortNo[1];
			nOutBitNo[j] = clsExcel.m_nPartAccBittNo[nOutPartNo[j]+1][1][nOutPotrNo[j]][j-1];

			nRtn = FAS_IO.get_out_bit((nOutPotrNo[j] * 100) + nOutBitNo[j], IO_ON);
			if(nRtn == IO_ON) 
			{
				m_pGridOutPutIo.SetItemBkColour(j, 1, GREEN_C, BLACK_L);
				m_pGridOutPutIo.SetItemFont(j, 1, &clsFunc.OnLogFont(20));
				m_pGridOutPutIo.SetItemText(j, 1, _T("ON"));
			}
			else
			{
				m_pGridOutPutIo.SetItemBkColour(j, 1, YELLOW_C, BLACK_L);
				m_pGridOutPutIo.SetItemFont(j, 1, &clsFunc.OnLogFont(20));
				m_pGridOutPutIo.SetItemText(j, 1, _T("OFF"));
			}
			m_pGridOutPutIo.Invalidate(FALSE);
		}

		if(i >= 16)
		{
			for(int k=j+1; k<i; k++) {
				m_pGridOutPutIo.SetItemBkColour(k, 1, DSERBLE_BC, BLACK_L);
				m_pGridOutPutIo.SetItemFont(k, 1, &clsFunc.OnLogFont(20));
				m_pGridOutPutIo.SetItemText(k, 1, _T(""));

				m_pGridOutPutIo.SetItemBkColour(k, 2, DSERBLE_BC, BLACK_L);
				m_pGridOutPutIo.SetItemFont(k, 2, &clsFunc.OnLogFont(15));
				m_pGridOutPutIo.SetItemText(k, 2, _T(""));
			}
			break;
		}
	}
	m_pGridOutPutIo.Invalidate(FALSE);
}

void CDialog_Part_IO::OnCellInputClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	
	int nRow, nCol;
//	int nBit;
//	int nCount;
//	int i;

	CString strTmp;
	CString strMsg;

	NM_GRIDVIEW* pItem	= (NM_GRIDVIEW*) pNotifyStruct;

	nRow				= pItem->iRow;
	nCol				= pItem->iColumn;

	// Header Click
	if (nRow < 1) return;
	if (nCol < 1 || nCol > 1) return;
}

void CDialog_Part_IO::OnCellOutputClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	int nRow, nCol;
	int nRtn;
	int nTmpOut;

	CString strTmp;
	CString strMsg;

	NM_GRIDVIEW* pItem	= (NM_GRIDVIEW*) pNotifyStruct;

	nRow				= pItem->iRow;
	nCol				= pItem->iColumn;

	// Header Click
	if (nRow < 1) return;
	if (nCol < 1 || nCol > 1) return;

	switch (nCol)
	{
		case 1:
			nOutPartNo[nRow] = nPartNo[1];
			nOutPotrNo[nRow] = nPortNo[1];
			nOutBitNo[nRow] = clsExcel.m_nPartAccBittNo[nOutPartNo[nRow]+1][1][nOutPotrNo[nRow]][nRow-1];
	
			nTmpOut = (nOutPotrNo[nRow] * 100) + nOutBitNo[nRow];

			if (clsFunc.OnIoSafetyCheck(nTmpOut) == RET_ERROR) return;

			nRtn = FAS_IO.get_out_bit(nTmpOut, IO_OFF);

			if(nRtn == IO_OFF) 
			{
				FAS_IO.set_out_bit(nTmpOut, IO_ON);
			}
			else	
			{
				FAS_IO.set_out_bit(nTmpOut, IO_OFF);
			}
			break;
	}

	//nInOut = 0;
	OnDisplayInputIO();
	//nInOut = 1;
	OnDisplayOutputIO();

	m_pGridInPutIo.Invalidate(FALSE);
}

void CDialog_Part_IO::OnBnClickedBtnPartIoExit()
{
	CDialog::OnOK();
}


void CDialog_Part_IO::OnDestroy()
{
	CDialog::OnDestroy();

	if (m_pImagelist != NULL)
	{
		delete m_pImagelist;
		m_pImagelist = NULL;
	}

	KillTimer(TMR_PART_IO_INPUT_UPDATE);
}


void CDialog_Part_IO::OnTimer(UINT_PTR nIDEvent)
{

	switch(nIDEvent)
	{
	case TMR_PART_IO_INPUT_UPDATE:
		OnDisplayInputIO();
		break;
	}

	CDialog::OnTimer(nIDEvent);
}
