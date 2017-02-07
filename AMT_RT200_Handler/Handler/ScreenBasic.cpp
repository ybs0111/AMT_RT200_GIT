// ScreenBasic.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Handler.h"
#include "ScreenBasic.h"
#include "PublicFunction.h"
#include "Dialog_Message.h"
#include "io.h"			// 파일 존재 유무 검사 함수 호출하기 위해서는 반드시 필요
#include "Dialog_KeyBoard.h"
#include "Dialog_KeyPad.h"
#include "MyBasicData.h"
#include "Dialog_Message.h"
#include "Alg_Mysql.h"
//#include "InterfaceGms.h"
#include "XgemClient.h"
#include "AlgMemory.h"
#include "ClientGms.h"
#include "Dialog_Pass_Check.h"
#include "Dialog_Pass_Check.h"
#include ".\\Ctrl\\KeyBoard.h"

// CScreenBasic


IMPLEMENT_DYNCREATE(CScreenBasic, CFormView)

CScreenBasic::CScreenBasic()
	: CFormView(CScreenBasic::IDD)
{
 	m_nRobotPickMode = 0;
 	m_nRobotPlaceMode = 0;
	m_nPcbTurn = 1;
	mb_GridInit = true;
}

CScreenBasic::~CScreenBasic()
{
}

void CScreenBasic::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_MODEL_CREATE,		m_btnModelCreate);
	DDX_Control(pDX, IDC_BTN_MODEL_DELETE,		m_btnModelDelete);
	DDX_Control(pDX, IDC_BTN_APPLY,				m_btnApply);
	DDX_Control(pDX, IDC_BTN_RELOAD,			m_btnReLoad);
	DDX_Control(pDX, IDC_GROUP_MODEL_INFO,		m_groupModelInfo);
	DDX_Control(pDX, IDC_GROUP_DEVICE_MODE,		m_groupDeviceMode);
	DDX_Control(pDX, IDC_CUSTOM_DEVICE_MODE,	m_pGridDevice);
	DDX_Control(pDX, IDC_CUSTOM_MODEL_LIST,		m_pGridModel);
	DDX_Control(pDX, IDC_GROUP_PCB_SETTING, m_groupPcbSetting);
	DDX_Control(pDX, IDC_MSG_PCB_ROW_CNT, m_msgPcbRowCnt);
	DDX_Control(pDX, IDC_MSG_PCB_COL_CNT, m_msgPcbColCnt);
	DDX_Control(pDX, IDC_DGT_PCB_ROW_CNT, m_dgtPcbRowCnt);
	DDX_Control(pDX, IDC_DGT_PCB_COL_CNT, m_dgtPcbColCnt);
	DDX_Radio(pDX, IDC_RADIO_PCB_TURN, m_nPcbTurn);

	DDX_Radio(pDX, IDC_RADIO_BASIC_PICK_CENTER, m_nRobotPickMode);
	DDX_Radio(pDX, IDC_RADIO_BASIC_PLACE_CENTER, m_nRobotPlaceMode);

	DDX_Control(pDX, IDC_CUSTOM_MODE, m_GridPcbType);
	DDX_Control(pDX, IDC_BTN_PCB_CREATE, m_btnPcbCreate);
	DDX_Control(pDX, IDC_BTN_SODIMM, m_BtnSodimm);
	DDX_Control(pDX, IDC_BTN_RDIMM, m_BtnRdimm);
	DDX_Control(pDX, IDC_BTN_UDIMM9, m_BtnUdimm_9);
	DDX_Control(pDX, IDC_BTN_UDIMM10, m_BtnUdimm_10);

	DDX_Control(pDX, IDC_GROUP_BASIC_PICK_POS, m_groupRobotPickPos);
	DDX_Control(pDX, IDC_GROUP_BASIC_PLACE_POS, m_groupRobotPlacePos);
	DDX_Control(pDX, IDC_GROUP_PICKER_SELECT, m_group_Picker_Select);
	DDX_Control(pDX, IDC_CUSTOM_PICKER_SELECT, m_GridPickerSelect);
	DDX_Control(pDX, IDC_DGT_VISION_ERROR_CNT, m_dgtVisionErrorCnt);
	DDX_Control(pDX, IDC_MSG_VISION_ERROR_CNT, m_msgVisionErrorCnt);
}

BEGIN_MESSAGE_MAP(CScreenBasic, CFormView)
	ON_NOTIFY(NM_CLICK, IDC_CUSTOM_MODEL_LIST,	OnModelCellClick)
	ON_NOTIFY(NM_CLICK, IDC_CUSTOM_MODEL_LIST,	OnModelCellRClick)
	ON_NOTIFY(NM_CLICK, IDC_CUSTOM_MODE,	OnPcbCellClick)
	ON_NOTIFY(NM_CLICK, IDC_CUSTOM_PICKER_SELECT,	OnPickerSelect)
	ON_NOTIFY(NM_CLICK, IDC_CUSTOM_DEVICE_MODE, OnDeviceCellClick)////////
	ON_BN_CLICKED(IDC_BTN_MODEL_CREATE,			&CScreenBasic::OnClickedBtnModelCreate)
	ON_BN_CLICKED(IDC_BTN_MODEL_DELETE,			&CScreenBasic::OnClickedBtnModelDelete)
	ON_BN_CLICKED(IDC_BTN_RELOAD,				&CScreenBasic::OnClickedBtnReload)
	ON_BN_CLICKED(IDC_BTN_APPLY,				&CScreenBasic::OnClickedBtnApply)
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_STN_CLICKED(IDC_DGT_PCB_ROW_CNT, &CScreenBasic::OnStnClickedDgtPcbRowCnt)
	ON_STN_CLICKED(IDC_DGT_PCB_COL_CNT, &CScreenBasic::OnStnClickedDgtPcbColCnt)
	ON_BN_CLICKED(IDC_RADIO_PCB_TURN, &CScreenBasic::OnBnClickedRadioPcbTurn)
	ON_BN_CLICKED(IDC_RADIO_PCB_NOTURN, &CScreenBasic::OnBnClickedRadioPcbNoturn)
	ON_BN_CLICKED(IDC_BTN_PCB_CREATE, &CScreenBasic::OnBnClickedBtnPcbCreate)
	ON_BN_CLICKED(IDC_BTN_SODIMM, &CScreenBasic::OnBnClickedBtnSodimm)
	ON_BN_CLICKED(IDC_BTN_RDIMM, &CScreenBasic::OnBnClickedBtnRdimm)
	ON_BN_CLICKED(IDC_BTN_UDIMM9, &CScreenBasic::OnBnClickedBtnUdimm9)
	ON_BN_CLICKED(IDC_BTN_UDIMM10, &CScreenBasic::OnBnClickedBtnUdimm10)


	ON_BN_CLICKED(IDC_RADIO_BASIC_PICK_CENTER, &CScreenBasic::OnBnClickedRadioBasicPickCenter)
	ON_COMMAND(IDC_RADIO_BASIC_PICK_RIGHT, &CScreenBasic::OnRadioBasicPickRight)
	ON_COMMAND(IDC_RADIO_BASIC_PICK_LEFT, &CScreenBasic::OnRadioBasicPickLeft)
	ON_BN_CLICKED(IDC_RADIO_BASIC_PLACE_CENTER, &CScreenBasic::OnBnClickedRadioBasicPlaceCenter)
	ON_COMMAND(IDC_RADIO_BASIC_PLACE_RIGHT, &CScreenBasic::OnRadioBasicPlaceRight)
	ON_COMMAND(IDC_RADIO_BASIC_PLACE_LEFT, &CScreenBasic::OnRadioBasicPlaceLeft)
	ON_STN_CLICKED(IDC_DGT_VISION_ERROR_CNT, &CScreenBasic::OnStnClickedDgtVisionErrorCnt)
END_MESSAGE_MAP()


// CScreenBasic 진단입니다.

#ifdef _DEBUG
void CScreenBasic::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CScreenBasic::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CScreenBasic 메시지 처리기입니다.


void CScreenBasic::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	OnDataInit();
	OnInitGroupBox();
	OnInitLabel();
	OnInitPickerSelect();
	OnInitButton();

	OnInitPcbType();
	OnInitDigit();
	
	OnInitGridDeviceMode();
	OnInitGridModelList();

// 	if (m_RobotPickPos[1]<=0)
// 	{
// 		m_RobotPickPos[1] = 1;
// 	}
// 
// 	if (m_RobotPlacePos[1]<=0)
// 	{
// 		m_RobotPlacePos[1] = 1;
// 	}

	//m_nRobotPickMode = m_RobotPickPos[1] - 1;
	//m_nRobotPlaceMode = m_RobotPlacePos[1]  - 1;
	UpdateData(FALSE);
	st_handler_info.cWndBasic = this;
}


BOOL CScreenBasic::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
/*	if (OnDataComparison() == RET_ERROR)
	{
		OnDataHistoryLog();
		OnDataApply();
	}
	st_handler_info.cWndBasic = NULL;*/
	return CFormView::DestroyWindow();
}

void CScreenBasic::OnInitGroupBox()
{
	m_groupModelInfo.SetFont(clsFunc.OnLogFont(16));
	m_groupModelInfo.SetCatptionTextColor(BLUE_C);
	m_groupModelInfo.SetBorderColor(ORANGE_C);
	m_groupModelInfo.SetFontBold(TRUE);
	m_groupModelInfo.SetBackgroundColor(WINDOW_UP);

	m_groupDeviceMode.SetFont(clsFunc.OnLogFont(16));
	m_groupDeviceMode.SetCatptionTextColor(BLUE_C);
	m_groupDeviceMode.SetBorderColor(ORANGE_C);
	m_groupDeviceMode.SetFontBold(TRUE);
	m_groupDeviceMode.SetBackgroundColor(WINDOW_UP);

	//kwlee 2016.0323
	m_groupPcbSetting.SetFont(clsFunc.OnLogFont(16));
	m_groupPcbSetting.SetCatptionTextColor(BLUE_C);
	m_groupPcbSetting.SetBorderColor(ORANGE_C);
	m_groupPcbSetting.SetFontBold(TRUE);
	m_groupPcbSetting.SetBackgroundColor(WINDOW_UP);

	m_groupRobotPickPos.SetFont(clsFunc.OnLogFont(16));
	m_groupRobotPickPos.SetCatptionTextColor(BLUE_C);
	m_groupRobotPickPos.SetBorderColor(ORANGE_C);
	m_groupRobotPickPos.SetFontBold(TRUE);
	m_groupRobotPickPos.SetBackgroundColor(WINDOW_UP);

	m_groupRobotPlacePos.SetFont(clsFunc.OnLogFont(16));
	m_groupRobotPlacePos.SetCatptionTextColor(BLUE_C);
	m_groupRobotPlacePos.SetBorderColor(ORANGE_C);
	m_groupRobotPlacePos.SetFontBold(TRUE);
	m_groupRobotPlacePos.SetBackgroundColor(WINDOW_UP);

	m_group_Picker_Select.SetFont(clsFunc.OnLogFont(16));
	m_group_Picker_Select.SetCatptionTextColor(BLUE_C);
	m_group_Picker_Select.SetBorderColor(ORANGE_C);
	m_group_Picker_Select.SetFontBold(TRUE);
	m_group_Picker_Select.SetBackgroundColor(WINDOW_UP);

	
}


void CScreenBasic::OnInitLabel()
{
	m_msgPcbRowCnt.SetFont(clsFunc.m_pFont[5]);
	m_msgPcbRowCnt.SetWindowText(_T("RowCount"));
	m_msgPcbRowCnt.SetCenterText();
	m_msgPcbRowCnt.SetColor(WHITE_C);
	m_msgPcbRowCnt.SetGradientColor(ORANGE_C);
	m_msgPcbRowCnt.SetTextColor(BLACK_C);

	m_msgPcbColCnt.SetFont(clsFunc.m_pFont[5]);
	m_msgPcbColCnt.SetWindowText(_T("ColCount"));
	m_msgPcbColCnt.SetCenterText();
	m_msgPcbColCnt.SetColor(WHITE_C);
	m_msgPcbColCnt.SetGradientColor(ORANGE_C);
	m_msgPcbColCnt.SetTextColor(BLACK_C);

	
	m_msgVisionErrorCnt.SetFont(clsFunc.m_pFont[5]);
	m_msgVisionErrorCnt.SetWindowText(_T("Vision Error Cnt"));
	m_msgVisionErrorCnt.SetCenterText();
	m_msgVisionErrorCnt.SetColor(WHITE_C);
	m_msgVisionErrorCnt.SetGradientColor(ORANGE_C);
	m_msgVisionErrorCnt.SetTextColor(BLACK_C);
}


void CScreenBasic::OnInitDigit()
{

	m_dgtPcbRowCnt.SetStyle(CDigit::DS_INT, 2, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	m_dgtPcbRowCnt.SetVal(m_nRowCnt[1]);

	m_dgtPcbColCnt.SetStyle(CDigit::DS_INT, 2, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	m_dgtPcbColCnt.SetVal(m_nColCnt[1]);

	m_dgtVisionErrorCnt.SetStyle(CDigit::DS_INT, 2, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	m_dgtVisionErrorCnt.SetVal(m_nVisionErrorCnt[1]);
}
void CScreenBasic::OnInitPickerSelect()
{
	int i,j,nTemp;
	CString strTmp;
	//RECT rt;

	m_GridPickerSelect.SetFrameFocusCell(FALSE);
	m_GridPickerSelect.SetTrackFocusCell(FALSE);
	m_GridPickerSelect.EnableSelection(FALSE);
	m_GridPickerSelect.SetGridLineColor(RGB(25, 1, 1));
	m_GridPickerSelect.SetGridLines(1);
	m_GridPickerSelect.SetRowCount(2);
	m_GridPickerSelect.SetColumnCount(6);
	m_GridPickerSelect.SetFixedRowCount(0);
	m_GridPickerSelect.SetFixedColumnCount(0);
	m_GridPickerSelect.SetFixedBkColor(RGB(0,0,200));
	m_GridPickerSelect.SetFixedBkColor(RGB(200,200,255));
	m_GridPickerSelect.SetTextBkColor(RGB(150,150,200));

	nTemp =2 * 6;

	for (i=0; i<2; i++) 
	{
		m_GridPickerSelect.SetRowHeight(i, 92/2);
		
		for (j=0; j<6; j++) 
		{
			if (st_basic_info.nCellPos[i][j] == TRUE)
			{
				m_GridPickerSelect.SetItemBkColour(i, j, RED_C, CLR_DEFAULT);
			}
			else
			{
				m_GridPickerSelect.SetItemBkColour(i, j, RGB(255, 255, 128), CLR_DEFAULT);
			}

			m_GridPickerSelect.SetColumnWidth(j, 37);
			m_GridPickerSelect.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_GridPickerSelect.SetItemState(i, j, GVIS_READONLY);

			strTmp.Format(_T("%d"), nTemp--);
			m_GridPickerSelect.SetItemText(i, j, strTmp);
		}

	}

}
void CScreenBasic::OnInitPcbType()
{

	int i,j,nTemp;
	CString strTmp;
	RECT rt;

	m_GridPcbType.SetFrameFocusCell(FALSE);
	m_GridPcbType.SetTrackFocusCell(FALSE);
	m_GridPcbType.EnableSelection(FALSE);
	m_GridPcbType.SetGridLineColor(RGB(25, 1, 1));
	m_GridPcbType.SetGridLines(1);
	m_GridPcbType.SetRowCount(m_nRowCnt[1]);
	m_GridPcbType.SetColumnCount(m_nColCnt[1]);
	m_GridPcbType.SetFixedRowCount(0);
	m_GridPcbType.SetFixedColumnCount(0);
	m_GridPcbType.SetFixedBkColor(RGB(0,0,200));
	m_GridPcbType.SetFixedBkColor(RGB(200,200,255));
	m_GridPcbType.SetTextBkColor(RGB(150,150,200));


	m_dgtPcbRowCnt.SetVal(m_nRowCnt[1]);
	m_dgtPcbColCnt.SetVal(m_nColCnt[1]);

	mn_Size = 18;
	
	for (i=0; i<m_nRowCnt[1]; i++) 
	{
	//	nTemp = m_nColCnt[1];
		nTemp = m_nColCnt[1];
		m_GridPcbType.SetRowHeight(i, 74/m_nRowCnt[1]);

		for (j=0; j<m_nColCnt[1]; j++) 
		{
			if (i == 0)
			{
				mn_Size = mn_Size +18;
			}

			if (st_Pcb_info.nPcbType == SODIMM)
			{	
				if (i == 0)
				{
					m_GridPcbType.SetItemBkColour(i, j, GREEN_C, CLR_DEFAULT);
					st_Pcb_info.nPcbSelect[i][j] = TRUE;
				}
				else
				{
					m_GridPcbType.SetItemBkColour(i, j, RGB(255, 255, 128), CLR_DEFAULT);
				}
			}
			else if (st_Pcb_info.nPcbType == RDIMM)
			{	
				if (j >= 3)
				{

					m_GridPcbType.SetItemBkColour(i, j, GREEN_C, CLR_DEFAULT);
					st_Pcb_info.nPcbSelect[i][j] = TRUE;
				}
				else
				{
					m_GridPcbType.SetItemBkColour(i, j, RGB(255, 255, 128), CLR_DEFAULT);
				}
			}
			else if (st_Pcb_info.nPcbType == UDIMM_9)
			{
				
				m_GridPcbType.SetItemBkColour(i, j, GREEN_C, CLR_DEFAULT);
				st_Pcb_info.nPcbSelect[i][j] = TRUE;
			}
			else if (st_Pcb_info.nPcbType == UDIMM_10)
			{
				if (j >= 5)
				{
					m_GridPcbType.SetItemBkColour(i, j, GREEN_C, CLR_DEFAULT);
					st_Pcb_info.nPcbSelect[i][j] = TRUE;
				}
				else
				{
					m_GridPcbType.SetItemBkColour(i, j, RGB(255, 255, 128), CLR_DEFAULT);
				}
			}
			else
			{
				if (st_Pcb_info.nPcbSelect[i][j] == TRUE)
				{
					m_GridPcbType.SetItemBkColour(i, j, GREEN_C, CLR_DEFAULT);
				}
				else
				{
					m_GridPcbType.SetItemBkColour(i, j, RGB(255, 255, 128), CLR_DEFAULT);
				}
			}
			strTmp.Format(_T("%d"), nTemp--);

			m_GridPcbType.SetColumnWidth(j,19);
			m_GridPcbType.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_GridPcbType.SetItemState(i, j, GVIS_READONLY);
			//m_GridPcbType.SetItemBkColour(i, j, RGB(255, 255, 128), CLR_DEFAULT);
			m_GridPcbType.SetItemText(i, j, strTmp);
		}
	}	
	if (m_nRowCnt[1]  == 1 && m_nColCnt[1] == 1)
	{
		rt.left =0;	rt.top = 0;	rt.right = rt.left + 0;	rt.bottom = rt.top + 0;
	}
	else
	{
		rt.left =310;	rt.top = 148;	rt.right = rt.left + mn_Size;	rt.bottom = rt.top + 80;
	}

	st_Pcb_info.nLeftSize = mn_Size;
	GetDlgItem( IDC_CUSTOM_MODE)->MoveWindow( &rt );
}

// int CScreenBasic::OnPickerNum()
// {
// 	int nRet = 0;
// 	int i,j;
// 	
// }
void CScreenBasic::OnInitButton()
{
	m_btnModelCreate.SetBitmaps(IDC_BTN_MODEL_CREATE, IDB_BITMAP_CREATE_DN, WINDOW_DN, IDB_BITMAP_CREATE_UP, WINDOW_UP);
	m_btnModelCreate.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnModelCreate.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnModelCreate.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnModelCreate.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnModelCreate.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnModelCreate.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnModelCreate.SetFont(clsFunc.m_pFont[3]);
	m_btnModelCreate.SetTooltipText(_T("Model Create"));

	m_btnModelDelete.SetBitmaps(IDC_BTN_MODEL_DELETE, IDB_BITMAP_DELETE_DN, WINDOW_DN, IDB_BITMAP_DELETE_UP, WINDOW_UP);
	m_btnModelDelete.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnModelDelete.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnModelDelete.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnModelDelete.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnModelDelete.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnModelDelete.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnModelDelete.SetFont(clsFunc.m_pFont[3]);
	m_btnModelDelete.SetTooltipText(_T("Model Delete"));

	m_btnApply.SetBitmaps(IDC_BTN_APPLY, IDB_BITMAP_APPLY_DN, WINDOW_DN, IDB_BITMAP_APPLY_UP, WINDOW_UP);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnApply.SetFont(clsFunc.m_pFont[3]);
	m_btnApply.SetTooltipText(_T("Basic Data Apply"));

	m_btnReLoad.SetBitmaps(IDC_BTN_RELOAD, IDB_BITMAP_RELOAD_DN, WINDOW_DN, IDB_BITMAP_RELOAD_UP, WINDOW_UP);
	m_btnReLoad.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnReLoad.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnReLoad.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnReLoad.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnReLoad.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnReLoad.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnReLoad.SetFont(clsFunc.m_pFont[3]);
	m_btnReLoad.SetTooltipText(_T("Basic Data ReLoad"));


	m_btnPcbCreate.SetBitmaps(IDC_BTN_MODEL_CREATE, IDB_BITMAP_CREATE_DN, WINDOW_DN, IDB_BITMAP_CREATE_UP, WINDOW_UP);
	m_btnPcbCreate.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnPcbCreate.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnPcbCreate.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnPcbCreate.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnPcbCreate.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnPcbCreate.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnPcbCreate.SetFont(clsFunc.m_pFont[3]);
	m_btnPcbCreate.SetTooltipText(_T("PCB Type Create"));


	//m_BtnSodimm.SetBitmaps(IDC_BTN_MODEL_CREATE, IDB_BITMAP_CREATE_DN, WINDOW_DN, IDB_BITMAP_CREATE_UP, WINDOW_UP);
	m_BtnSodimm.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_BtnSodimm.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_BtnSodimm.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_BtnSodimm.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_BtnSodimm.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_BtnSodimm.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_BtnSodimm.SetFont(clsFunc.m_pFont[3]);
	m_BtnSodimm.SetTooltipText(_T("PCB SODIMM"));

//	m_btnPcbCreate.SetBitmaps(IDC_BTN_MODEL_CREATE, IDB_BITMAP_CREATE_DN, WINDOW_DN, IDB_BITMAP_CREATE_UP, WINDOW_UP);
	m_BtnRdimm.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_BtnRdimm.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_BtnRdimm.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_BtnRdimm.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_BtnRdimm.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_BtnRdimm.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_BtnRdimm.SetFont(clsFunc.m_pFont[3]);
	m_BtnRdimm.SetTooltipText(_T("PCB RDIMM"));

	//m_btnPcbCreate.SetBitmaps(IDC_BTN_MODEL_CREATE, IDB_BITMAP_CREATE_DN, WINDOW_DN, IDB_BITMAP_CREATE_UP, WINDOW_UP);
	m_BtnUdimm_9.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_BtnUdimm_9.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_BtnUdimm_9.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_BtnUdimm_9.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_BtnUdimm_9.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_BtnUdimm_9.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_BtnUdimm_9.SetFont(clsFunc.m_pFont[3]);
	m_BtnUdimm_9.SetTooltipText(_T("PCB UDIMM 9"));

	//m_BtnUdimm_10.SetBitmaps(IDC_BTN_MODEL_CREATE, IDB_BITMAP_CREATE_DN, WINDOW_DN, IDB_BITMAP_CREATE_UP, WINDOW_UP);
	m_BtnUdimm_10.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_BtnUdimm_10.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_BtnUdimm_10.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_BtnUdimm_10.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_BtnUdimm_10.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_BtnUdimm_10.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_BtnUdimm_10.SetFont(clsFunc.m_pFont[3]);
	m_BtnUdimm_10.SetTooltipText(_T("PCB UDIMM 10"));
}

void CScreenBasic::OnInitGridDeviceMode()
{
	CString strTmp;
	int i, j;
	
	m_pGridDevice.SetFrameFocusCell(FALSE);
	m_pGridDevice.SetTrackFocusCell(FALSE);
	m_pGridDevice.EnableSelection(FALSE);

	m_pGridDevice.SetGridLineColor(BLACK_C);
	m_pGridDevice.SetGridLines(1);

	m_pGridDevice.SetRowCount(18);
	m_pGridDevice.SetColumnCount(2);

	m_pGridDevice.SetFixedRowCount(0);
	m_pGridDevice.SetFixedColumnCount(0);
	m_pGridDevice.SetFixedBkColor(RGB(0,0,200));
	m_pGridDevice.SetFixedBkColor(RGB(200,200,255));
	m_pGridDevice.SetTextBkColor(RGB(150,150,200));

	for (i=0; i<18; i++) 
	{
		m_pGridDevice.SetRowHeight(i, 39);

		for (j=0; j<2; j++) 
		{
			switch (j)
			{
				case 0:
					m_pGridDevice.SetColumnWidth(j, 60);
					break;

				case 1:
					m_pGridDevice.SetColumnWidth(j, 166);
					break;
			}

			m_pGridDevice.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_pGridDevice.SetItemState(i, j, GVIS_READONLY);
			m_pGridDevice.SetItemBkColour(i, j, WHITE_C, CLR_DEFAULT);
		}
	}

	// IO MAP Header ////////////////////////////////////////////////////////////////////////////////////////////// 
	m_pGridDevice.SetItemBkColour(0, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridDevice.SetItemFont(0, 0, &clsFunc.OnLogFont(18));
	m_pGridDevice.SetItemText(0, 0, _T("No."));

	m_pGridDevice.SetItemBkColour(0, 1, RGB(219, 229, 241), BLACK_L);
	m_pGridDevice.SetItemFont(0, 1, &clsFunc.OnLogFont(18));
	m_pGridDevice.SetItemText(0, 1, _T("MODE"));


	m_pGridDevice.SetItemBkColour(1, 0, BLACK_L, YELLOW_C);
	m_pGridDevice.SetItemFont(1, 0, &clsFunc.OnLogFont(18));
	m_pGridDevice.SetItemText(1, 0, _T("1"));

	m_pGridDevice.SetItemBkColour(1, 1, BLACK_L, YELLOW_C);
	m_pGridDevice.SetItemFont(1, 1, &clsFunc.OnLogFont(18));
	m_pGridDevice.SetItemText(1, 1, _T("WithOut"));

	m_pGridDevice.SetItemBkColour(2, 0, BLACK_L, YELLOW_C);
	m_pGridDevice.SetItemFont(2, 0, &clsFunc.OnLogFont(18));
	m_pGridDevice.SetItemText(2, 0, _T("2"));

	m_pGridDevice.SetItemBkColour(2, 1, BLACK_L, YELLOW_C);
	m_pGridDevice.SetItemFont(2, 1, &clsFunc.OnLogFont(18));
	m_pGridDevice.SetItemText(2, 1, _T("With"));

	if (m_nModeDevice[1]		== WITHOUT_DVC)
	{
		m_pGridDevice.SetItemBkColour(1, 0, RGB(79, 79, 225), WHITE_C);
		m_pGridDevice.SetItemBkColour(1, 1, RGB(79, 79, 225), WHITE_C);
	}
	else if (m_nModeDevice[1]	== WITH_DVC)
	{
		m_pGridDevice.SetItemBkColour(2, 0, RGB(79, 79, 225), WHITE_C);
		m_pGridDevice.SetItemBkColour(2, 1, RGB(79, 79, 225), WHITE_C);
	}

	m_pGridDevice.MergeCells(3, 0, 3, 1);
	m_pGridDevice.SetItemBkColour(3, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridDevice.SetItemFont(3, 0, &clsFunc.OnLogFont(18));
	m_pGridDevice.SetItemText(3, 0, _T("INTERFACE"));

	m_pGridDevice.SetItemBkColour(4, 0, GREEN_D, BLACK_C);
	m_pGridDevice.SetItemFont(4, 0, &clsFunc.OnLogFont(18));
	m_pGridDevice.SetItemText(4, 0, _T("1"));

	m_pGridDevice.SetItemBkColour(4, 1, GREEN_D, BLACK_C);
	m_pGridDevice.SetItemFont(4, 1, &clsFunc.OnLogFont(18));
	m_pGridDevice.SetItemText(4, 1, _T("Off Line"));

	m_pGridDevice.SetItemBkColour(5, 0, GREEN_D, BLACK_C);
	m_pGridDevice.SetItemFont(5, 0, &clsFunc.OnLogFont(18));
	m_pGridDevice.SetItemText(5, 0, _T("2"));

	m_pGridDevice.SetItemBkColour(5, 1, GREEN_D, BLACK_C);
	m_pGridDevice.SetItemFont(5, 1, &clsFunc.OnLogFont(18));
	m_pGridDevice.SetItemText(5, 1, _T("On Line"));

	if (m_nModeInterface[1] == EQP_OFF_LINE)
	{
		m_pGridDevice.SetItemBkColour(4, 0, GREEN_C, BLACK_C);
		m_pGridDevice.SetItemBkColour(4, 1, GREEN_C, BLACK_C);
	}
	else
	{
		m_pGridDevice.SetItemBkColour(5, 0, GREEN_C, BLACK_C);
		m_pGridDevice.SetItemBkColour(5, 1, GREEN_C, BLACK_C);
	}

	m_pGridDevice.MergeCells(6, 0, 6, 1);
	m_pGridDevice.SetItemBkColour(6, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridDevice.SetItemFont(6, 0, &clsFunc.OnLogFont(18));
	m_pGridDevice.SetItemText(6, 0, _T("Work Mode"));

	m_pGridDevice.SetItemBkColour(7, 0, GREEN_D, BLACK_C);
	m_pGridDevice.SetItemFont(7, 0, &clsFunc.OnLogFont(18));
	m_pGridDevice.SetItemText(7, 0, _T("1"));

	m_pGridDevice.SetItemBkColour(7, 1, GREEN_D, BLACK_C);
	m_pGridDevice.SetItemFont(7, 1, &clsFunc.OnLogFont(18));
	m_pGridDevice.SetItemText(7, 1, _T("Stand Alone"));

	m_pGridDevice.SetItemBkColour(8, 0, GREEN_D, BLACK_C);
	m_pGridDevice.SetItemFont(8, 0, &clsFunc.OnLogFont(18));
	m_pGridDevice.SetItemText(8, 0, _T("2"));

	m_pGridDevice.SetItemBkColour(8, 1, GREEN_D, BLACK_C);
	m_pGridDevice.SetItemFont(8, 1, &clsFunc.OnLogFont(18));
	m_pGridDevice.SetItemText(8, 1, _T("In-Line"));

	if (m_nModeRearSmema[1] == NO)
	{
		m_pGridDevice.SetItemBkColour(7, 0, GREEN_C, BLACK_C);
		m_pGridDevice.SetItemBkColour(7, 1, GREEN_C, BLACK_C);
	}
	else
	{
		m_pGridDevice.SetItemBkColour(8, 0, GREEN_C, BLACK_C);
		m_pGridDevice.SetItemBkColour(8, 1, GREEN_C, BLACK_C);
	}
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	m_pGridDevice.MergeCells(9, 0, 9, 1);
	m_pGridDevice.SetItemBkColour(9, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridDevice.SetItemFont(9, 0, &clsFunc.OnLogFont(18));
	m_pGridDevice.SetItemText(9, 0, _T("RFID Read/Write"));

	m_pGridDevice.SetItemBkColour(10, 0, GREEN_D, BLACK_C);
	m_pGridDevice.SetItemFont(10, 0, &clsFunc.OnLogFont(18));
	m_pGridDevice.SetItemText(10, 0, _T("1"));

	m_pGridDevice.SetItemBkColour(10, 1, GREEN_D, BLACK_C);
	m_pGridDevice.SetItemFont(10, 1, &clsFunc.OnLogFont(18));
	m_pGridDevice.SetItemText(10, 1, _T("RFID No"));

	m_pGridDevice.SetItemBkColour(11, 0, GREEN_D, BLACK_C);
	m_pGridDevice.SetItemFont(11, 0, &clsFunc.OnLogFont(18));
	m_pGridDevice.SetItemText(11, 0, _T("2"));

	m_pGridDevice.SetItemBkColour(11, 1, GREEN_D, BLACK_C);
	m_pGridDevice.SetItemFont(11, 1, &clsFunc.OnLogFont(18));
	m_pGridDevice.SetItemText(11, 1, _T("RFID Yes"));

	if (m_nModeRfid[1] == NO)
	{
		m_pGridDevice.SetItemBkColour(10, 0, GREEN_C, BLACK_C);
		m_pGridDevice.SetItemBkColour(10, 1, GREEN_C, BLACK_C);
	}
	else
	{
		m_pGridDevice.SetItemBkColour(11, 0, GREEN_C, BLACK_C);
		m_pGridDevice.SetItemBkColour(11, 1, GREEN_C, BLACK_C);
	}
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	m_pGridDevice.MergeCells(12, 0, 12, 1);
	m_pGridDevice.SetItemBkColour(12, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridDevice.SetItemFont(12, 0, &clsFunc.OnLogFont(18));
	m_pGridDevice.SetItemText(12, 0, _T("BARCODE ERROR SKIP"));

	m_pGridDevice.SetItemBkColour(13, 0, BLACK_L, YELLOW_C);
	m_pGridDevice.SetItemFont(13, 0, &clsFunc.OnLogFont(18));
	m_pGridDevice.SetItemText(13, 0, _T("1"));

	m_pGridDevice.SetItemBkColour(13, 1, BLACK_L, YELLOW_C);
	m_pGridDevice.SetItemFont(13, 1, &clsFunc.OnLogFont(18));
	m_pGridDevice.SetItemText(13, 1, _T("NO"));

	m_pGridDevice.SetItemBkColour(14, 0, BLACK_L, YELLOW_C);
	m_pGridDevice.SetItemFont(14, 0, &clsFunc.OnLogFont(18));
	m_pGridDevice.SetItemText(14, 0, _T("2"));

	m_pGridDevice.SetItemBkColour(14, 1, BLACK_L, YELLOW_C);
	m_pGridDevice.SetItemFont(14, 1, &clsFunc.OnLogFont(18));
	m_pGridDevice.SetItemText(14, 1, _T("YES"));

	if (m_nBarcodeErrorSkip[1] == NO)
	{
		m_pGridDevice.SetItemBkColour(13, 0, GREEN_C, BLACK_C);
		m_pGridDevice.SetItemBkColour(13, 1, GREEN_C, BLACK_C);
	}
	else
	{
		m_pGridDevice.SetItemBkColour(14, 0, GREEN_C, BLACK_C);
		m_pGridDevice.SetItemBkColour(14, 1, GREEN_C, BLACK_C);
	}
	
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	m_pGridDevice.MergeCells(15, 0, 15, 1);
	m_pGridDevice.SetItemBkColour(15, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridDevice.SetItemFont(15, 0, &clsFunc.OnLogFont(18));
	m_pGridDevice.SetItemText(15, 0, _T("-"));

	m_pGridDevice.SetItemBkColour(16, 0, BLACK_L, YELLOW_C);
	m_pGridDevice.SetItemFont(16, 0, &clsFunc.OnLogFont(18));
	m_pGridDevice.SetItemText(16, 0, _T("1"));

	m_pGridDevice.SetItemBkColour(16, 1, BLACK_L, YELLOW_C);
	m_pGridDevice.SetItemFont(16, 1, &clsFunc.OnLogFont(18));
	m_pGridDevice.SetItemText(16, 1, _T("-"));

	m_pGridDevice.SetItemBkColour(17, 0, BLACK_L, YELLOW_C);
	m_pGridDevice.SetItemFont(17, 0, &clsFunc.OnLogFont(18));
	m_pGridDevice.SetItemText(17, 0, _T("2"));

	m_pGridDevice.SetItemBkColour(17, 1, BLACK_L, YELLOW_C);
	m_pGridDevice.SetItemFont(17, 1, &clsFunc.OnLogFont(18));
	m_pGridDevice.SetItemText(17, 1, _T("-"));
/*
	if (m_nModeUnLdBcr[1] == NO)
	{
		m_pGridDevice.SetItemBkColour(16, 0, GREEN_C, BLACK_C);
		m_pGridDevice.SetItemBkColour(16, 1, GREEN_C, BLACK_C);
	}
	else
	{
		m_pGridDevice.SetItemBkColour(17, 0, GREEN_C, BLACK_C);
		m_pGridDevice.SetItemBkColour(17, 1, GREEN_C, BLACK_C);
	}
*/
	m_pGridDevice.Invalidate(FALSE);
}


void CScreenBasic::OnDeviceCellClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	CDialog_Pass_Check  pass_chk_dlg;

	int nRow, nCol;

	CString strTmp;
	CString strMsg;

	NM_GRIDVIEW* pItem	= (NM_GRIDVIEW*) pNotifyStruct;

	nRow				= pItem->iRow;
	nCol				= pItem->iColumn;

	if(nRow < 1) return;

	if (nRow == 1 && m_nModeDevice[1] != WITHOUT_DVC)
	{
		if (st_lot_info[LOT_CURR].nLotStatus >= LOT_START || st_lot_info[LOT_NEXT].nLotStatus >= LOT_START)
		{
			if (st_handler_info.cWndList != NULL)
			{
				clsMem.OnNormalMessageWrite(_T("Data Change Fail. Lot Start Status...."));
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}

			return;
		}

		m_nModeDevice[1] = WITHOUT_DVC; 

		m_pGridDevice.SetItemBkColour(1, 0, RGB(79, 79, 225), WHITE_C);
		m_pGridDevice.SetItemBkColour(1, 1, RGB(79, 79, 225), WHITE_C);

		m_pGridDevice.SetItemBkColour(2, 0, BLACK_L, YELLOW_C);
		m_pGridDevice.SetItemBkColour(2, 1, BLACK_L, YELLOW_C);
	}
	else if (nRow == 2 && m_nModeDevice[1] != WITH_DVC)
	{
		if (st_lot_info[LOT_CURR].nLotStatus >= LOT_START || st_lot_info[LOT_NEXT].nLotStatus >= LOT_START)
		{
			if (st_handler_info.cWndList != NULL)
			{
				clsMem.OnNormalMessageWrite(_T("Data Change Fail. Lot Start Status...."));
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}

			return;
		}

		m_nModeDevice[1] = WITH_DVC;

		m_pGridDevice.SetItemBkColour(1, 0, BLACK_L, YELLOW_C);
		m_pGridDevice.SetItemBkColour(1, 1, BLACK_L, YELLOW_C);

		m_pGridDevice.SetItemBkColour(2, 0, RGB(79, 79, 225), WHITE_C);
		m_pGridDevice.SetItemBkColour(2, 1, RGB(79, 79, 225), WHITE_C);
	}
	else if (nRow == 4 && m_nModeInterface[1] != EQP_OFF_LINE)
	{
		if (st_lot_info[LOT_CURR].nLotStatus >= LOT_START || st_lot_info[LOT_NEXT].nLotStatus >= LOT_START)
		{
			if (st_handler_info.cWndList != NULL)
			{
				clsMem.OnNormalMessageWrite(_T("Data Change Fail. Lot Start Status...."));
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}

			return;
		}

		m_nModeInterface[1] = EQP_OFF_LINE;

		m_pGridDevice.SetItemBkColour(4, 0, GREEN_C, BLACK_C);
		m_pGridDevice.SetItemBkColour(4, 1, GREEN_C, BLACK_C);

		m_pGridDevice.SetItemBkColour(5, 0, GREEN_D, BLACK_C);
		m_pGridDevice.SetItemBkColour(5, 1, GREEN_D, BLACK_C);
	}
	else if (nRow == 5 && m_nModeInterface[1] != EQP_ON_LINE)
	{
		if (st_lot_info[LOT_CURR].nLotStatus >= LOT_START || st_lot_info[LOT_NEXT].nLotStatus >= LOT_START)
		{
			if (st_handler_info.cWndList != NULL)
			{
				clsMem.OnNormalMessageWrite(_T("Data Change Fail. Lot Start Status...."));
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}

			return;
		}

		m_nModeInterface[1] = EQP_ON_LINE;

		m_pGridDevice.SetItemBkColour(4, 0, GREEN_D, BLACK_C);
		m_pGridDevice.SetItemBkColour(4, 1, GREEN_D, BLACK_C);

		m_pGridDevice.SetItemBkColour(5, 0, GREEN_C, BLACK_C);
		m_pGridDevice.SetItemBkColour(5, 1, GREEN_C, BLACK_C);
	}
	else if (nRow == 7 && m_nModeRearSmema[1] != NO)
	{
/*
		if (st_lot_info[LOT_CURR].nLotStatus >= LOT_START || st_lot_info[LOT_NEXT].nLotStatus >= LOT_START)
		{
			if (st_handler_info.cWndList != NULL)
			{
				clsMem.OnNormalMessageWrite(_T("Data Change Fail. Lot Start Status...."));
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}

			return;
		}
*/
		m_nModeRearSmema[1] = NO;

		m_pGridDevice.SetItemBkColour(7, 0, GREEN_C, BLACK_C);
		m_pGridDevice.SetItemBkColour(7, 1, GREEN_C, BLACK_C);

		m_pGridDevice.SetItemBkColour(8, 0, GREEN_D, BLACK_C);
		m_pGridDevice.SetItemBkColour(8, 1, GREEN_D, BLACK_C);
	}
	else if (nRow == 8 && m_nModeRearSmema[1] != YES)
	{
/*		
		if (st_lot_info[LOT_CURR].nLotStatus >= LOT_START || st_lot_info[LOT_NEXT].nLotStatus >= LOT_START)
		{
			if (st_handler_info.cWndList != NULL)
			{
				clsMem.OnNormalMessageWrite(_T("Data Change Fail. Lot Start Status...."));
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}

			return;
		}
*/
		m_nModeRearSmema[1] = YES;

		m_pGridDevice.SetItemBkColour(7, 0, GREEN_D, BLACK_C);
		m_pGridDevice.SetItemBkColour(7, 1, GREEN_D, BLACK_C);

		m_pGridDevice.SetItemBkColour(8, 0, GREEN_C, BLACK_C);
		m_pGridDevice.SetItemBkColour(8, 1, GREEN_C, BLACK_C);
	}
	else if (nRow == 10 && m_nModeRfid[1] != NO)
	{
		if (st_lot_info[LOT_CURR].nLotStatus >= LOT_START || st_lot_info[LOT_NEXT].nLotStatus >= LOT_START)
		{
			if (st_handler_info.cWndList != NULL)
			{
				clsMem.OnNormalMessageWrite(_T("Data Change Fail. Lot Start Status...."));
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}

			return;
		}

		m_nModeRfid[1] = NO;

		m_pGridDevice.SetItemBkColour(10, 0, GREEN_C, BLACK_C);
		m_pGridDevice.SetItemBkColour(10, 1, GREEN_C, BLACK_C);

		m_pGridDevice.SetItemBkColour(11, 0, GREEN_D, BLACK_C);
		m_pGridDevice.SetItemBkColour(11, 1, GREEN_D, BLACK_C);

	}
	else if (nRow == 11 && m_nModeRfid[1] != YES)
	{
		if (st_lot_info[LOT_CURR].nLotStatus >= LOT_START || st_lot_info[LOT_NEXT].nLotStatus >= LOT_START)
		{
			if (st_handler_info.cWndList != NULL)
			{
				clsMem.OnNormalMessageWrite(_T("Data Change Fail. Lot Start Status...."));
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
			return;
		}

		m_nModeRfid[1] = YES;

		m_pGridDevice.SetItemBkColour(10, 0, GREEN_D, BLACK_C);
		m_pGridDevice.SetItemBkColour(10, 1, GREEN_D, BLACK_C);

		m_pGridDevice.SetItemBkColour(11, 0, GREEN_C, BLACK_C);
		m_pGridDevice.SetItemBkColour(11, 1, GREEN_C, BLACK_C);
	}
	//kwlee 2017.0204
	else if (nRow == 13 && m_nBarcodeErrorSkip[1] != NO)
	{
		if (st_lot_info[LOT_CURR].nLotStatus >= LOT_START || st_lot_info[LOT_NEXT].nLotStatus >= LOT_START)
		{
			if (st_handler_info.cWndList != NULL)
			{
				clsMem.OnNormalMessageWrite(_T("Data Change Fail. Lot Start Status...."));
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
			return;
		}

		m_nBarcodeErrorSkip[1] = NO;

		m_pGridDevice.SetItemBkColour(13, 0, GREEN_C, BLACK_C);
		m_pGridDevice.SetItemBkColour(13, 1, GREEN_C, BLACK_C);

		m_pGridDevice.SetItemBkColour(14, 0, GREEN_D, BLACK_C);
		m_pGridDevice.SetItemBkColour(14, 1, GREEN_D, BLACK_C);
	}
	else if (nRow == 14 && m_nBarcodeErrorSkip[1] != YES)
	{
		if (st_lot_info[LOT_CURR].nLotStatus >= LOT_START || st_lot_info[LOT_NEXT].nLotStatus >= LOT_START)
		{
			if (st_handler_info.cWndList != NULL)
			{
				clsMem.OnNormalMessageWrite(_T("Data Change Fail. Lot Start Status...."));
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
			return;
		}

		m_nBarcodeErrorSkip[1] = YES;

		m_pGridDevice.SetItemBkColour(13, 0, GREEN_D, BLACK_C);
		m_pGridDevice.SetItemBkColour(13, 1, GREEN_D, BLACK_C);

		m_pGridDevice.SetItemBkColour(14, 0, GREEN_C, BLACK_C);
		m_pGridDevice.SetItemBkColour(14, 1, GREEN_C, BLACK_C);
	}
	///
	m_pGridDevice.Invalidate();
}

void CScreenBasic::OnInitGridModelList()
{
	CString strTmp;
	
	m_pGridModel.SetFrameFocusCell(FALSE);
	m_pGridModel.SetTrackFocusCell(FALSE);
	m_pGridModel.EnableSelection(FALSE);

	m_pGridModel.SetGridLineColor(BLACK_C);
	m_pGridModel.SetGridLines(1);

	m_pGridModel.SetRowCount(1);
	m_pGridModel.SetColumnCount(1);

	m_pGridModel.SetFixedRowCount(0);
	m_pGridModel.SetFixedColumnCount(0);
	m_pGridModel.SetFixedBkColor(RGB(0,0,200));
	m_pGridModel.SetFixedBkColor(RGB(200,200,255));
	m_pGridModel.SetTextBkColor(RGB(150,150,200));

	m_pGridModel.SetRowHeight(0, 50);
	m_pGridModel.SetColumnWidth(0, 300);

	m_pGridModel.SetItemFormat(0, 0, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	m_pGridModel.SetItemState(0, 0, GVIS_READONLY);
	m_pGridModel.SetItemBkColour(0, 0, WHITE_C, CLR_DEFAULT);

	m_pGridModel.SetItemBkColour(0, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridModel.SetItemFont(0, 0, &clsFunc.OnLogFont(18));
	m_pGridModel.SetItemText(0, 0, _T("Model Name"));

	OnInitModelList(st_path_info.strBasic);
}

void CScreenBasic::OnModelCellClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	CDialog_Message dlgMsg;
	int nRow, nCol;
	INT_PTR nResponse;

	CString strTmp;
	CString strMsg;

	NM_GRIDVIEW* pItem	= (NM_GRIDVIEW*) pNotifyStruct;

	nRow				= pItem->iRow;
	nCol				= pItem->iColumn;

	if (nRow < 1) return;

	if (st_lot_info[LOT_CURR].nLotStatus >= LOT_START || st_lot_info[LOT_NEXT].nLotStatus >= LOT_START)
	{
		if (st_handler_info.cWndList != NULL)
		{
			clsMem.OnNormalMessageWrite(_T("Data Change Fail. Lot Start Status...."));
			st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}

		return;
	}

	strTmp = m_pGridModel.GetItemText(nRow, nCol);

	dlgMsg.m_nMessageType	= 1;
	dlgMsg.m_strMessage.Format(_T("[%s] Model\r\nDo you want a clicked model?"), strTmp);

	nResponse = dlgMsg.DoModal();
		
	if(nResponse == IDOK)
	{
		if(OnModelDeviceCheck(strTmp) == false)  // 입력된 디바이스 이름 정상 유무 검사 함수
		{
			return ;
		}

		m_strDeviceName[1]			= strTmp;
		st_basic_info.strDeviceName	= m_strDeviceName[1];
			
		clsBasic.OnBasic_Data_Load(1);
		clsBasic.OnModuleRobot_Teach_Data_Load(st_basic_info.strDeviceName);
		clsBasic.OnMaintenance_Data_Load();
		clsBasic.OnWaitTime_Data_Load();
		clsBasic.OnMotorSpeed_Set_Data_Load();
		clsBasic.OnInterface_Data_Load();
		clsBasic.OnRecipe_Data_Load(st_basic_info.strDeviceName);
			
		OnDataInit();
		OnInitGridDeviceMode();
		OnInitPcbType(); //kwlee 2016.0323
		OnDeviceColorChange(nRow, nCol);

		// jtkim 20150929
		::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + FTP_NETWORK, 1, 0);

		m_pGridModel.Refresh();// .Invalidate();
	}
	else
	{
		m_pGridModel.SetItemBkColour(nRow, nCol, RGB(79, 79, 225), WHITE_C);
	}
}
void CScreenBasic::OnPickerSelect(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	int nRow, nCol;
	int nNum[12] = {0,};
	CString strTmp;
	CString strMsg;

	NM_GRIDVIEW* pItem	= (NM_GRIDVIEW*) pNotifyStruct;

	nRow				= pItem->iRow;
	nCol				= pItem->iColumn;
	
	
	if (st_basic_info.nCellPos[nRow][nCol] == FALSE)
	{
		m_GridPickerSelect.SetItemBkColour(nRow, nCol, RED_C, BLACK_C);
		st_basic_info.nCellPos[nRow][nCol] = TRUE;
		
		if (nRow == 0)
		{
			st_basic_info.nPickerSelect[PICKCNT - 1 - (nRow + nCol)] = CTL_NO;
		}
		else
		{
			st_basic_info.nPickerSelect[6 - (nRow + nCol)] = CTL_NO;
		}
	}
	else
	{
		m_GridPickerSelect.SetItemBkColour(nRow, nCol, RGB(255,255,128), BLACK_C);
		st_basic_info.nCellPos[nRow][nCol] = FALSE;

		if (nRow == 0)
		{
			st_basic_info.nPickerSelect[PICKCNT - 1 - (nRow + nCol)] = CTL_YES;
		}
		else
		{
			st_basic_info.nPickerSelect[6 - (nRow + nCol)] = CTL_YES;
		}
		
	}

	m_GridPickerSelect.Invalidate();
}

void CScreenBasic::OnPcbCellClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	int nRow, nCol;
//	INT_PTR nResponse;

	CString strTmp;
	CString strMsg;

	NM_GRIDVIEW* pItem	= (NM_GRIDVIEW*) pNotifyStruct;

	nRow				= pItem->iRow;
	nCol				= pItem->iColumn;

// 	m_GridPcbType.SetItemFormat(nRow, nCol, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
// 	m_GridPcbType.SetItemState(nRow, nCol, GVIS_READONLY);

	
	
	if (st_Pcb_info.nPcbType == SODIMM || st_Pcb_info.nPcbType == RDIMM || st_Pcb_info.nPcbType == UDIMM_10 || st_Pcb_info.nPcbType == UDIMM_9 || nCol <0)
	{
		return;
	}
	if (st_Pcb_info.nPcbSelect[nRow][nCol] == FALSE)
	{
		m_GridPcbType.SetItemBkColour(nRow, nCol, GREEN_C, BLACK_C);
		st_Pcb_info.nPcbSelect[nRow][nCol] = TRUE;
		
	}
	else
	{
		m_GridPcbType.SetItemBkColour(nRow, nCol, RGB(255,255,128), BLACK_C);
		st_Pcb_info.nPcbSelect[nRow][nCol] = FALSE;
	}

	
	
//	m_GridPcbType.SetItemBkColour(nRow, nCol, RGB(128, 255, 255), CLR_DEFAULT);
//	UpdateData(TRUE);
	m_GridPcbType.Invalidate();
}
void CScreenBasic::OnModelCellRClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	CDialog_Message dlgMsg;
	CDialog_Pass_Check	dlgPass;

	int nRow, nCol;
	INT_PTR nResponse;

	CString strTmp;
	CString strMsg;

	NM_GRIDVIEW* pItem	= (NM_GRIDVIEW*) pNotifyStruct;

	nRow				= pItem->iRow;
	nCol				= pItem->iColumn;

	if (nRow < 1) return;

	st_other_info.strPassLevel = _T("AMT_MASTER");

	dlgPass.m_nPassLevel = 6;
	nResponse = (int)dlgPass.DoModal();

	if (nResponse == IDOK)
	{
		strTmp = m_pGridModel.GetItemText(nRow, nCol);

		dlgMsg.m_nMessageType	= 1;
		dlgMsg.m_strMessage.Format(_T("[%s] Model\r\nDo you want a clicked model?"), strTmp);

		nResponse = dlgMsg.DoModal();

		if(nResponse == IDOK)
		{
			if(OnModelDeviceCheck(strTmp) == false)  // 입력된 디바이스 이름 정상 유무 검사 함수
			{
				return ;
			}

			m_strDeviceName[1]			= strTmp;
			st_basic_info.strDeviceName	= m_strDeviceName[1];

			clsBasic.OnBasic_Data_Load(1);
			clsBasic.OnModuleRobot_Teach_Data_Load(st_basic_info.strDeviceName);
			clsBasic.OnMaintenance_Data_Load();
			clsBasic.OnWaitTime_Data_Load();
			clsBasic.OnMotorSpeed_Set_Data_Load();
			clsBasic.OnInterface_Data_Load();
			clsBasic.OnRecipe_Data_Load(st_basic_info.strDeviceName);

			OnDataInit();
			OnInitGridDeviceMode();

			OnDeviceColorChange(nRow, nCol);

			m_pGridModel.Refresh();// .Invalidate();
		}
		else
		{
			m_pGridModel.SetItemBkColour(nRow, nCol, RGB(79, 79, 225), WHITE_C);
		}
	}
	else 
	{
		return;
	}
}

void CScreenBasic::OnDeviceColorChange(int nSelectRow, int nSelectCol)
{
	int nTotRowCnt;
	nTotRowCnt = m_pGridModel.GetRowCount();

	m_nDeviceName = nSelectRow;

	m_pGridModel.SetItemBkColour(m_nDeviceName, nSelectCol, RGB(79, 79, 225), WHITE_C);

	for(int i=1; i< nTotRowCnt; i++)
	{
		if(i != m_nDeviceName) 
		{
			m_pGridModel.SetItemBkColour(i, nSelectCol, BLACK_L, WHITE_C);
		}
	}

	m_nOldDeviceName = m_nDeviceName;

}

void CScreenBasic::OnDataHistoryLog()
{
	CString strMsg;

	if (m_nModeDevice[0]		!= m_nModeDevice[1])
	{
		strMsg.Format(_T("[BASIC] Device Mode Change %d -> %d"), m_nModeDevice[0], m_nModeDevice[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}

	if (m_nModeInterface[0]		!= m_nModeInterface[1])
	{
		strMsg.Format(_T("[BASIC] Interface Mode Change %d -> %d"), m_nModeInterface[0], m_nModeInterface[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}

	if (m_nModeSecsGem[0]		!= m_nModeSecsGem[1])
	{
		strMsg.Format(_T("[BASIC] SecsGem Mode Change %d -> %d"), m_nModeSecsGem[0], m_nModeSecsGem[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}

	if (m_nModeWork[0]		!= m_nModeWork[1])
	{
		strMsg.Format(_T("[BASIC] Work Mode Change %d -> %d"), m_nModeWork[0], m_nModeWork[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}

	if (m_strDeviceName[0]		!= m_strDeviceName[1])
	{
		strMsg.Format(_T("[BASIC] Device Name Change %s -> %s"), m_strDeviceName[0], m_strDeviceName[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}

	if (m_nModeFrontSmema[0]			!= m_nModeFrontSmema[1])
	{
		strMsg.Format(_T("[BASIC] Front Smema Check Mode Change %d -> %d"), m_nModeFrontSmema[0], m_nModeFrontSmema[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}

	if (m_nModeRearSmema[0]			!= m_nModeRearSmema[1])
	{
		strMsg.Format(_T("[BASIC] Rear Smema Check Mode Change %d -> %d"), m_nModeRearSmema[0], m_nModeRearSmema[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}
	
	if (m_nModeRfid[0]			!= m_nModeRfid[1])
	{
		strMsg.Format(_T("[BASIC] RFID Mode Change %d -> %d"), m_nModeRfid[0], m_nModeRfid[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}
	//kwlee 2017.0204
	if (m_nBarcodeErrorSkip[0]			!= m_nBarcodeErrorSkip[1])
	{
		strMsg.Format(_T("[BASIC] m_nBarcodeErrorSkip Change %d -> %d"), m_nBarcodeErrorSkip[0], m_nBarcodeErrorSkip[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}
	///
	
	if (m_strModelName[0]		!= m_strModelName[1])
	{
		strMsg.Format(_T("[BASIC] Model Name Change %s -> %s"), m_strModelName[0], m_strModelName[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}
}

void CScreenBasic::OnDataRecovery()
{
	m_nModeDevice[1]		= m_nModeDevice[0];			// [Title Bar 상태 표시] < WHIT/WHIT OUT ㅡ MODE 표시
	m_nModeInterface[1]		= m_nModeInterface[0];
	m_nModeSecsGem[1]		= m_nModeSecsGem[0];

	m_strDeviceName[1]		= m_strDeviceName[0];		// [Title Bar 상태 표시] < 장비에서 설정한 DEVICE 표시	>

	m_nModeFrontSmema[1]	= m_nModeFrontSmema[0];
	m_nModeRearSmema[1]		= m_nModeRearSmema[0];
	m_nModeRfid[1]			= m_nModeRfid[0];
	m_nBarcodeErrorSkip[1]  = m_nBarcodeErrorSkip[0]; //kwlee 2017.0204
	m_strModelName[1]		= m_strModelName[0];
}


int	CScreenBasic::OnDataComparison()
{
	if (m_nModeDevice[0]		!= m_nModeDevice[1])			return RET_ERROR;			// [Title Bar 상태 표시] < WHIT/WHIT OUT ㅡ MODE 표시
	if (m_nModeInterface[0]		!= m_nModeInterface[1])			return RET_ERROR;
	if (m_nModeSecsGem[0]		!= m_nModeSecsGem[1])			return RET_ERROR;
	if (m_nModeWork[0]			!= m_nModeWork[1])				return RET_ERROR;

	if (m_strDeviceName[0]		!= m_strDeviceName[1])			return RET_ERROR;		// [Title Bar 상태 표시] < 장비에서 설정한 DEVICE 표시	>

	if (m_nModeFrontSmema[0]	!= m_nModeFrontSmema[1])		return RET_ERROR;
	if (m_nModeRearSmema[0]		!= m_nModeRearSmema[1])			return RET_ERROR;
	if (m_nModeRfid[0]			!= m_nModeRfid[1])				return RET_ERROR;
	if (m_nBarcodeErrorSkip[0]	!= m_nBarcodeErrorSkip[1])		return RET_ERROR; //kwlee 2017.0204
	if (m_strModelName[0]		!= m_strModelName[1])			return RET_ERROR;
	
	return RET_GOOD;
}


void CScreenBasic::OnDataApply()
{
	st_basic_info.nModeDevice		= m_nModeDevice[1];;	// [Title Bar 상태 표시] < WHIT/WHIT OUT ㅡ MODE 표시	>
	st_basic_info.nModeInterface	= m_nModeInterface[1];
//	st_basic_info.nModeSecsGem		= m_nModeSecsGem[1];
	st_basic_info.nModeWork			= m_nModeWork[1];
	st_basic_info.strDeviceName		= m_strDeviceName[1];	// [Title Bar 상태 표시] < 장비에서 설정한 DEVICE 표시	>
	st_basic_info.nModeFrontSmema	= m_nModeFrontSmema[1];
	st_basic_info.nModeRearSmema	= m_nModeRearSmema[1];
	
	//kwlee 2016.0325
	st_basic_info.nColCnt           = m_nColCnt[1];
	st_basic_info.nRowCnt           = m_nRowCnt[1];
	st_basic_info.nPcbType          = st_Pcb_info.nPcbType;
	m_nPcbTurn                      = st_basic_info.nPcbTurnEnable;
	st_basic_info.nLeftSize           = mn_Size;
	st_basic_info.nVisionErrorCnt   = m_nVisionErrorCnt[1];//kwlee 2017.0202
	

	st_basic_info.nRobotPickPos      =   m_RobotPickPos[1];                
	st_basic_info.nRobotPlacePos     =  m_RobotPlacePos[1];


	m_nRobotPickMode				  = st_basic_info.nRobotPickPos;
	m_nRobotPlaceMode				  =	st_basic_info.nRobotPlacePos;

	//st_basic_info.nRobotPickPos		  =	m_RobotPickPos[1];
	//st_basic_info.nRobotPlacePos	  =	m_RobotPlacePos[1];
 //	st_basic_info.nRobotPickPos		= m_nRobotPickMode;
 	//st_basic_info.nRobotPlacePos	= m_nRobotPlaceMode;
	//////////


	// jtkim 20150625 in-line
	if (st_basic_info.nModeRearSmema == YES)
	{
		if (st_lot_info[LOT_NEXT].nLotStatus == LOT_START)
		{
			if (st_lot_info[LOT_NEXT].nTrayRunMode_StandAlone == YES)
			{
				st_lot_info[LOT_NEXT].nTrayRunMode_StandAlone = NO;

				if (st_handler_info.cWndList != NULL)  
				{
					clsMem.OnNormalMessageWrite(_T("Next Lot In-line Mode Success"));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
				}
			}
		}
	}

	st_basic_info.strModelName		= m_strModelName[1];
}

void CScreenBasic::OnDataBackup()
{
	m_nModeDevice[0]		= m_nModeDevice[1];			// [Title Bar 상태 표시] < WHIT/WHIT OUT ㅡ MODE 표시
	m_nModeInterface[0]		= m_nModeInterface[1];
	m_nModeSecsGem[0]		= m_nModeSecsGem[1];
	m_nModeWork[0]			= m_nModeWork[1];
	m_strDeviceName[0]		= m_strDeviceName[1];		// [Title Bar 상태 표시] < 장비에서 설정한 DEVICE 표시	>
	m_nModeFrontSmema[0]	= m_nModeFrontSmema[1];
	m_nModeRearSmema[0]		= m_nModeRearSmema[1];
	m_nModeRfid[0]			= m_nModeRfid[1];
	m_nBarcodeErrorSkip[0]	= m_nBarcodeErrorSkip[1];  //kwlee 2017.0204
	m_strModelName[0]		= m_strModelName[1];
}


void CScreenBasic::OnDataInit()
{

	m_nModeDevice[1]					= st_basic_info.nModeDevice;
	m_strDeviceName[1]					= st_basic_info.strDeviceName;

	m_nModeInterface[1]					= st_basic_info.nModeInterface;
//	m_nModeSecsGem[1]					= st_basic_info.nModeSecsGem;
	m_nModeWork[1]						= st_basic_info.nModeWork;
	m_nModeFrontSmema[1]				= st_basic_info.nModeFrontSmema;
	m_nModeRearSmema[1]					= st_basic_info.nModeRearSmema;
	m_nModeRfid[1]						= st_basic_info.nModeRfid;
	m_strModelName[1]					= st_basic_info.strModelName;
	m_nBarcodeErrorSkip[1]              = st_basic_info.nBarcodeErrorSkip; //kwlee 2017.0204

	//kwlee 2016.0325
	m_nRowCnt[1]							= st_basic_info.nRowCnt;
	m_nColCnt[1]							= st_basic_info.nColCnt;
	st_Pcb_info.nPcbType                     = st_basic_info.nPcbType;
	m_nPcbTurn								= st_basic_info.nPcbTurnEnable;
	m_nVisionErrorCnt[1]                    = st_basic_info.nVisionErrorCnt; //kwlee 2017.0202
	m_nRobotPickMode                       = st_basic_info.nRobotPickPos;
	m_nRobotPlaceMode                       = st_basic_info.nRobotPlacePos;

	m_RobotPickPos[1]                      = st_basic_info.nRobotPickPos;
	m_RobotPlacePos[1]                       = st_basic_info.nRobotPlacePos;
	UpdateData(FALSE);
	/////////
	OnDataBackup();

	
}

int CScreenBasic::OnInitModelList(CString strFileName)
{
	int nCnt = 0;
	int nChk;						// 함수 리턴 플래그
	CString strTmp;

	HANDLE hFind;
    WIN32_FIND_DATA fd;

	if (strFileName.Right (1) != "\\")
	{
        strFileName += "\\";
	}

	strFileName += "*.*";
    
    if ((hFind = ::FindFirstFile ((LPCTSTR) strFileName, &fd)) != INVALID_HANDLE_VALUE) 
	{
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
		{
			nChk = OnAddItem(nCnt, &fd);
			if (nChk == RET_GOOD)
			{
				nCnt++;
			}
        }

        while (::FindNextFile (hFind, &fd)) 
		{
			nChk = RET_READY;
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				nChk = OnAddItem(nCnt, &fd);
				if (nChk == RET_ERROR) 
				{
					break;
				}
				else
				{
					nCnt++;
				}
			}
        }
		::FindClose(hFind);
    }
	
	return nCnt;
}

int CScreenBasic::OnAddItem(int nIndex, WIN32_FIND_DATA *pFd)
{
	CString strTempDvc;
	CString strDevice;
	CString strExpansion;
	int nPos;

	HICON	hIcon = NULL;

	strTempDvc = (pFd->cFileName);	// 파일 이름 설정

	strTempDvc.MakeUpper();
	
	/* ************************************************************************** */
	/* 파일 이름 중 확장자 확인                                                   */
	/* ************************************************************************** */
	nPos = strTempDvc.Find('.');		// 확장자 위치 설정
	
	if (nPos == -1)					// 확장자 미존재
	{
		strExpansion = _T("");
	}
	else								// 확장자 존재
	{
		strExpansion = strTempDvc.Mid(nPos + 1);
	}
	/* ************************************************************************** */

	if (strExpansion != _T("TXT"))
	{
		return RET_READY;	
	}

	strDevice = strTempDvc;			// 파일명 전체 화면 출력
	
	if (strDevice == m_strDeviceName[1])
	{
		m_nDeviceName = nIndex;
		OnDisplayModelList(1, nIndex, strDevice);
	}
	else
	{
		OnDisplayModelList(0, nIndex, strDevice);
	}
	/* ************************************************************************** */

    return RET_GOOD;
}

void CScreenBasic::OnDisplayModelList(int nMode, int nCount, CString strFileName)
{
	m_pGridModel.SetRowCount(nCount + 2); 
	m_pGridModel.SetColumnCount(1); 

	m_pGridModel.SetRowHeight(nCount + 1, 50);
	m_pGridModel.SetColumnWidth(0, 300);

	m_pGridModel.SetItemFormat(nCount + 1, 0, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	m_pGridModel.SetItemState(nCount + 1, 0, GVIS_READONLY);
	m_pGridModel.SetItemBkColour(nCount + 1, 0, WHITE_C, CLR_DEFAULT);

	if(nMode == 0)
	{
		m_pGridModel.SetItemBkColour(nCount + 1, 0, BLACK_L, YELLOW_C);
	}
	else
	{
		m_pGridModel.SetItemBkColour(nCount + 1, 0, RGB(79, 79, 225), WHITE_C);
	}
	m_pGridModel.SetItemFont(nCount + 1, 0, &clsFunc.OnLogFont(16));
	m_pGridModel.SetItemText(nCount + 1, 0, strFileName);
}

void CScreenBasic::OnClickedBtnModelCreate()
{
	// TODO: Add your control notification handler code here
	int nPos;

	CString strTmp, strFileName;
	
	KeyBoard(&strFileName);

	if(strFileName != "")
	{
		nPos = -1;
		nPos = strFileName.Find(_T("."), 0);
		
		strTmp = _T("");
		if(nPos > 0)
		{
			strTmp = strFileName.Mid(nPos+1, 3);
		}
		
		if(strTmp != "TXT")
		{
			strFileName += ".TXT";
		}
		
		if(OnModelCreateDeviceCheck(strFileName) == FALSE)
		{
			return;
		}

		OnDataBackup();

		clsBasic.OnBasic_Data_Save_As(strFileName);
		clsBasic.OnModuleRobot_Teach_Data_Save_As(strFileName);
		clsBasic.OnMaintenance_Data_Save_As(strFileName);
		clsBasic.OnInterface_Data_Save_As(strFileName);
		clsBasic.OnRecipe_Data_Save_As(strFileName);

		OnInitGridModelList();
	}
}


void CScreenBasic::OnClickedBtnModelDelete()
{
	// TODO: Add your control notification handler code here
	CDialog_Message	dlgMsg;
	char			chFileName[100];
	CString			strTmp;
	CString			strFileName;
	int				nResponse;
	int				nExistence;			// 파일 존재 유무 플래그

	strTmp = m_strDeviceName[1];
	if(OnModelDeviceCheck(strTmp) == false)  // 입력된 디바이스 이름 정상 유무 검사 함수
	{
		return ;
	}
	
	dlgMsg.m_nMessageType	= 1;
	dlgMsg.m_strMessage		= _T("Are you sure you want to delete the selected information?");

	nResponse = (int)dlgMsg.DoModal();
	
	if(nResponse == IDOK)
	{
		if(strTmp == m_strDeviceName[1])
		{
			m_strDeviceName[0] = m_strDeviceName[1] = m_pGridModel.GetItemText(1, 0);
			if(m_strDeviceName[1] == strTmp)
			{
				m_strDeviceName[1] = "";
				m_strDeviceName[0] = m_strDeviceName[1] = m_pGridModel.GetItemText(2, 0);
			}

		}
		
		strFileName = st_path_info.strBasic + strTmp;
		//wsprintfA(chFileName, "%S", strFileName);
		clsFunc.OnStringToChar(strFileName, chFileName);
// jtkim 20130704
//		nExistence = access(chFileName, 0) ;
		nExistence = _access(chFileName, 0) ;
		if (nExistence != -1)			// 파일 존재
		{
			DeleteFile(strFileName);  // 해당 파일 삭제
			
			OnInitGridModelList();
		}
		else  
		{
			return ;
		}
	}
}

void CScreenBasic::OnClickedBtnReload()
{
	CDialog_Message dlgMsg;
	int nResponse;

	dlgMsg.m_nMessageType	= 1;
	dlgMsg.m_strMessage		= _T("Do you want to change the existing value to the current value?");

	nResponse = (int)dlgMsg.DoModal();

	if (nResponse == IDOK)
	{
		OnDataRecovery();					// 화면 셋팅 정보 백업 받아놓은 변수로 복구 함수

		OnDataInit();
	}
}


void CScreenBasic::OnClickedBtnApply()
{
	CDialog_Message dlgMsg;
	int nResponse;
	
	dlgMsg.m_nMessageType	= 1;
	dlgMsg.m_strMessage		= _T("Do you want to change the existing value to the current value?");

	
	nResponse = (int)dlgMsg.DoModal();

	if (nResponse == IDOK)
	{
		OnDataHistoryLog();
		OnDataApply();
		OnDataBackup();

		OnInitialUpdate();
		
		if (st_handler_info.cWndTitle != NULL)
		{
			st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, MODEL_MODE, 0);			// Device Mode
		}
	}
}

bool CScreenBasic::OnModelDeviceCheck(CString strFileName)
{
	CString strChkFile;  /* 생성할 [폴더]+[파일명] 저장 변수 */
	CString strChkExt;   /* 확장자 저장 변수 */
	int nPos;

	/* ************************************************************************** */
    /* 입력된 디바이스명 설정한다                                                 */
    /* ************************************************************************** */

	strFileName.MakeUpper();
	strFileName.TrimLeft(' ');               
	strFileName.TrimRight(' ');

	if(strFileName.IsEmpty())  
	{
		if(st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
		{
			//wsprintfA(st_other_info.cAbnormalMsg, "%S",  _T("[DEVICE] A name input error occurrence."));
			clsFunc.OnStringToChar(_T("[DEVICE] A name input error occurrence."), st_other_info.cAbnormalMsg);
			st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		return false;
	}
	/* ************************************************************************** */

	strChkFile = st_path_info.strPathDvc + strFileName;  // 생성할 [폴더]+[파일명] 설정
	nPos = strChkFile.Find(_T("."));  // 확장자 검사
	if (nPos == -1) 
	{
		strChkFile += _T(".TXT");
	}
	else 
	{
		strChkExt = strChkFile.Mid(nPos);  // 확장자 정보 설정
		if (strChkExt != _T(".TXT"))  
		{
			if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
			{
				//wsprintfA(st_other_info.cAbnormalMsg, "%S", _T("[DEVICE] A name extension input error occurrence."));
				//clsFunc.OnStringToChar(_T("[DEVICE] A name extension input error occurrence."), st_other_info.cAbnormalMsg);
				clsMem.OnAbNormalMessagWrite(_T("[DEVICE] A name extension input error occurrence."));
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			return false;
		}
	}

	m_strLoadFile = strChkFile;  // 생성할 [폴더]+[파일명]+[확장자] 설정

	return true;
}

bool CScreenBasic::OnModelCreateDeviceCheck(CString strFileName)
{
	CString strChkFile;  // 생성할 [폴더]+[파일명] 저장 변수
	CString strChkExt;   // 확장자 저장 변수
	int nPos;

	/* ************************************************************************** */
    /* 입력된 디바이스명 설정한다                                                 */
    /* ************************************************************************** */
	strFileName.MakeUpper();
	strFileName.TrimLeft(' ');               
	strFileName.TrimRight(' ');

	if(strFileName.IsEmpty())  
	{
		if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
		{
			//wsprintfA(st_other_info.cNormalMsg, "%S", _T("[DEVICE] A name input error occurrence."));
			//clsFunc.OnStringToChar(_T("[DEVICE] A name input error occurrence."), st_other_info.cNormalMsg);
			clsMem.OnAbNormalMessagWrite(_T("[DEVICE] A name input error occurrence."));
			st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		return false;
	}
	/* ************************************************************************** */

	strChkFile = st_path_info.strBasic + strFileName;  // 생성할 [폴더]+[파일명] 설정
	nPos = strChkFile.Find(_T("."));  // 확장자 검사
	if (nPos == -1) 
		strChkFile += _T(".TXT");
	else 
	{
		strChkExt = strChkFile.Mid(nPos);  // 확장자 정보 설정
		if (strChkExt != _T(".TXT"))  
		{
			if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
			{
				//wsprintfA(st_other_info.cNormalMsg, "%S",  _T("[DEVICE] A name extension input error occurrence."));
				//clsFunc.OnStringToChar(_T("[DEVICE] A name extension input error occurrence."), st_other_info.cNormalMsg);
				clsMem.OnAbNormalMessagWrite(_T("[DEVICE] A name extension input error occurrence."));
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			return false;
		}
	}

	m_strLoadFile = strChkFile;  // 생성할 [폴더]+[파일명]+[확장자] 설정

	return TRUE;
}

BOOL CScreenBasic::OnEraseBkgnd(CDC* pDC)
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



void CScreenBasic::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
/*	if (OnDataComparison() == RET_ERROR)
	{
		OnDataHistoryLog();
		OnDataApply();
	}
	st_handler_info.cWndBasic = NULL;*/
	CFormView::PostNcDestroy();
}


void CScreenBasic::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (OnDataComparison() == RET_ERROR)
	{
		OnDataHistoryLog();
		OnDataApply();
	}
	st_handler_info.cWndBasic = NULL;
}

HBRUSH CScreenBasic::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		switch(pWnd->GetDlgCtrlID())
		{
		case IDC_TEXT_TOOLNAME:
		case IDC_TEXT_PARTNO:
		case IDC_TEXT_COMMENT:
			pDC->SetBkColor(WHITE_C);
			return (HBRUSH)::GetStockObject(WHITE_BRUSH);
			break;

		default:
			pDC->SetBkMode(TRANSPARENT);
			return (HBRUSH)::GetStockObject(NULL_BRUSH);
			break;
		}

	}
	return hbr;
}


void CScreenBasic::OnTimer(UINT_PTR nIDEvent)
{
	CFormView::OnTimer(nIDEvent);
}


void CScreenBasic::OnStnClickedDgtPcbRowCnt()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog_KeyPad dlgKeyPad;

	dlgKeyPad.m_nKeypadMode			= 0;
	dlgKeyPad.m_strKeypadLowLimit	= _T("0");
	dlgKeyPad.m_strKeypadHighLimit	= _T("2");
	dlgKeyPad.m_strKeypadVal.Format(_T("%d"), m_nRowCnt[1]);

	if (dlgKeyPad.DoModal() == IDOK)
	{
		m_nRowCnt[1] = _wtoi(dlgKeyPad.m_strNewVal);
		m_dgtPcbRowCnt.SetVal(m_nRowCnt[1]);
	}
}


void CScreenBasic::OnStnClickedDgtPcbColCnt()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog_KeyPad dlgKeyPad;

	dlgKeyPad.m_nKeypadMode			= 0;
	dlgKeyPad.m_strKeypadLowLimit	= _T("0");
	dlgKeyPad.m_strKeypadHighLimit	= _T("10");
	dlgKeyPad.m_strKeypadVal.Format(_T("%d"), m_nColCnt[1]);

	if (dlgKeyPad.DoModal() == IDOK)
	{
		m_nColCnt[1] = _wtoi(dlgKeyPad.m_strNewVal);
		m_dgtPcbColCnt.SetVal(m_nColCnt[1]);
	}
}


void CScreenBasic::OnBnClickedRadioPcbTurn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	st_basic_info.nPcbTurnEnable = 0;
}


void CScreenBasic::OnBnClickedRadioPcbNoturn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	st_basic_info.nPcbTurnEnable = 1;
}


void CScreenBasic::OnBnClickedBtnPcbCreate()
{
	int i,j,nTemp;
	CString strTmp;

	RECT rt;
	
	m_GridPcbType.SetFrameFocusCell(FALSE);
	m_GridPcbType.SetTrackFocusCell(FALSE);
	m_GridPcbType.EnableSelection(FALSE);
	m_GridPcbType.SetGridLineColor(RGB(25, 1, 1));
	m_GridPcbType.SetGridLines(1);
	m_GridPcbType.SetRowCount(m_nRowCnt[1]);
	m_GridPcbType.SetColumnCount(m_nColCnt[1]);
	m_GridPcbType.SetFixedRowCount(0);
	m_GridPcbType.SetFixedColumnCount(0);
	m_GridPcbType.SetFixedBkColor(RGB(0,0,200));
	m_GridPcbType.SetFixedBkColor(RGB(200,200,255));
	m_GridPcbType.SetTextBkColor(RGB(150,150,200));

	//m_GridPcbType.nLeftSize = 275;
	mn_Size = 18;
	
	st_Pcb_info.nPcbType = CTL_NONE;
	
	for (i=0; i<m_nRowCnt[1]; i++) 
	{
		nTemp = m_nColCnt[1];
		m_GridPcbType.SetRowHeight(i, 74/m_nRowCnt[1]);
		
		for (j=0; j<m_nColCnt[1]; j++) 
		{

			strTmp.Format(_T("%d"), nTemp--);
			if (i == 0)
			{
				mn_Size = mn_Size +18;
			}
			

			m_GridPcbType.SetColumnWidth(j,19);
			m_GridPcbType.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_GridPcbType.SetItemState(i, j, GVIS_READONLY);
			m_GridPcbType.SetItemBkColour(i, j, RGB(255, 255, 128), CLR_DEFAULT);
			m_GridPcbType.SetItemText(i, j, strTmp);
		}
	}	
	st_Pcb_info.nLeftSize = mn_Size;
	rt.left =310;	rt.top = 148;	rt.right = rt.left + mn_Size;	rt.bottom = rt.top + 80;
	GetDlgItem(IDC_CUSTOM_MODE )->MoveWindow( &rt );

}


void CScreenBasic::OnBnClickedBtnSodimm()
{

	int i,j,nTemp;
	CString strTmp;

	RECT rt;

	m_GridPcbType.SetFrameFocusCell(FALSE);
	m_GridPcbType.SetTrackFocusCell(FALSE);
	m_GridPcbType.EnableSelection(FALSE);
	m_GridPcbType.SetGridLineColor(RGB(25, 1, 1));
	m_GridPcbType.SetGridLines(1);
	m_GridPcbType.SetRowCount(2);
	m_GridPcbType.SetColumnCount(9);
	m_GridPcbType.SetFixedRowCount(0);
	m_GridPcbType.SetFixedColumnCount(0);
	m_GridPcbType.SetFixedBkColor(RGB(0,0,200));
	m_GridPcbType.SetFixedBkColor(RGB(200,200,255));
	m_GridPcbType.SetTextBkColor(RGB(150,150,200));

	
	//m_GridPcbType.nLeftSize = 275;

	
	mn_Size = 18;
	m_dgtPcbRowCnt.SetVal(2);
	m_dgtPcbColCnt.SetVal(9);
	
	m_nRowCnt[1] = 2;
	m_nColCnt[1] = 9;

	st_Pcb_info.nPcbType = SODIMM;

	for (i=0; i<2; i++) 
	{
		m_GridPcbType.SetRowHeight(i, 74/2);
		nTemp = 9;
		for (j=0; j<9; j++) 
		{
			if (i == 0)
			{
				//st_Pcb_info.nPcbSerial[i][j] = TRUE;
				st_Pcb_info.nPcbSelect[i][j] = TRUE;
				m_GridPcbType.SetItemBkColour(i, j, GREEN_C, CLR_DEFAULT);
				
				mn_Size = mn_Size +18;
			}
			else
			{
				m_GridPcbType.SetItemBkColour(i, j, RGB(255, 255, 128), CLR_DEFAULT);
			}

			m_GridPcbType.SetColumnWidth(j, 19);
			m_GridPcbType.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_GridPcbType.SetItemState(i, j, GVIS_READONLY);
			
			strTmp.Format(_T("%d"), nTemp--);
			m_GridPcbType.SetItemText(i, j, strTmp);
			}
			
		}
		st_Pcb_info.nLeftSize = mn_Size;
	

	rt.left =310;	rt.top = 148;	rt.right = rt.left + mn_Size;	rt.bottom = rt.top + 80;
	GetDlgItem(IDC_CUSTOM_MODE )->MoveWindow( &rt );
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CScreenBasic::OnBnClickedBtnRdimm()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int i,j,nTemp;
	CString strTmp;

	RECT rt;

	m_GridPcbType.SetFrameFocusCell(FALSE);
	m_GridPcbType.SetTrackFocusCell(FALSE);
	m_GridPcbType.EnableSelection(FALSE);
	m_GridPcbType.SetGridLineColor(RGB(25, 1, 1));
	m_GridPcbType.SetGridLines(1);
	m_GridPcbType.SetRowCount(1);
	m_GridPcbType.SetColumnCount(6);
	m_GridPcbType.SetFixedRowCount(0);
	m_GridPcbType.SetFixedColumnCount(0);
	m_GridPcbType.SetFixedBkColor(RGB(0,0,200));
	m_GridPcbType.SetFixedBkColor(RGB(200,200,255));
	m_GridPcbType.SetTextBkColor(RGB(150,150,200));

	m_dgtPcbRowCnt.SetVal(1);
	m_dgtPcbColCnt.SetVal(6);
	//m_GridPcbType.nLeftSize = 275;
	mn_Size = 18;
	nTemp = 6;
	m_nRowCnt[1] = 1;
	m_nColCnt[1] = 6;
	st_Pcb_info.nPcbType = RDIMM;
	for (i=0; i<1; i++) 
	{
		m_GridPcbType.SetRowHeight(i, 74);

		for (j=0; j<6; j++) 
		{

			strTmp.Format(_T("%d"), nTemp--);

			m_GridPcbType.SetColumnWidth(j, 19);
			m_GridPcbType.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_GridPcbType.SetItemState(i, j, GVIS_READONLY);
			if (i == 0)
			{
				mn_Size = mn_Size +18;
			}

			if (j >= 3)
			{
				//st_Pcb_info.nPcbSerial[i][j] = TRUE;
				st_Pcb_info.nPcbSelect[i][j] = TRUE;
				m_GridPcbType.SetItemBkColour(i, j, GREEN_C, CLR_DEFAULT);
				
			}
			else
			{
				m_GridPcbType.SetItemBkColour(i, j, RGB(255, 255, 128), CLR_DEFAULT);
			}
			
			m_GridPcbType.SetItemText(i, j, strTmp);
		}
	}	
	st_Pcb_info.nLeftSize = mn_Size;
	rt.left =310;	rt.top = 148;	rt.right = rt.left + mn_Size;	rt.bottom = rt.top + 80;
	GetDlgItem(IDC_CUSTOM_MODE )->MoveWindow( &rt );
}


void CScreenBasic::OnBnClickedBtnUdimm9()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int i,j,nTemp;
	CString strTmp;

	RECT rt;

	m_GridPcbType.SetFrameFocusCell(FALSE);
	m_GridPcbType.SetTrackFocusCell(FALSE);
	m_GridPcbType.EnableSelection(FALSE);
	m_GridPcbType.SetGridLineColor(RGB(25, 1, 1));
	m_GridPcbType.SetGridLines(1);
	m_GridPcbType.SetRowCount(1);
	m_GridPcbType.SetColumnCount(9);
	m_GridPcbType.SetFixedRowCount(0);
	m_GridPcbType.SetFixedColumnCount(0);
	m_GridPcbType.SetFixedBkColor(RGB(0,0,200));
	m_GridPcbType.SetFixedBkColor(RGB(200,200,255));
	m_GridPcbType.SetTextBkColor(RGB(150,150,200));

	//m_GridPcbType.nLeftSize = 275;
	m_dgtPcbRowCnt.SetVal(1);
	m_dgtPcbColCnt.SetVal(9);
	 m_nRowCnt[1] = 1;
	 m_nColCnt[1] = 9;
	st_Pcb_info.nPcbType = UDIMM_9;
	mn_Size = 18;
	nTemp = 9;
	for (i=0; i<1; i++) 
	{
		m_GridPcbType.SetRowHeight(i, 74);

		for (j=0; j<9; j++) 
		{

			strTmp.Format(_T("%d"), nTemp--);
			if (i == 0)
			{
				mn_Size = mn_Size +18;
			}

			//st_Pcb_info.nPcbSerial[i][j] = TRUE;
			st_Pcb_info.nPcbSelect[i][j] = TRUE;
			m_GridPcbType.SetColumnWidth(j, 19);
			m_GridPcbType.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_GridPcbType.SetItemState(i, j, GVIS_READONLY);
			m_GridPcbType.SetItemBkColour(i, j, GREEN_C, CLR_DEFAULT);
			m_GridPcbType.SetItemText(i, j, strTmp);
		}
	}	
	st_Pcb_info.nLeftSize = mn_Size;
	rt.left =310;	rt.top = 148;	rt.right = rt.left + mn_Size;	rt.bottom = rt.top + 80;
	GetDlgItem(IDC_CUSTOM_MODE )->MoveWindow( &rt );
}


void CScreenBasic::OnBnClickedBtnUdimm10()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int i,j,nTemp;
	CString strTmp;

	RECT rt;

	m_GridPcbType.SetFrameFocusCell(FALSE);
	m_GridPcbType.SetTrackFocusCell(FALSE);
	m_GridPcbType.EnableSelection(FALSE);
	m_GridPcbType.SetGridLineColor(RGB(25, 1, 1));
	m_GridPcbType.SetGridLines(1);
	m_GridPcbType.SetRowCount(1);
	m_GridPcbType.SetColumnCount(10);
	m_GridPcbType.SetFixedRowCount(0);
	m_GridPcbType.SetFixedColumnCount(0);
	m_GridPcbType.SetFixedBkColor(RGB(0,0,200));
	m_GridPcbType.SetFixedBkColor(RGB(200,200,255));
	m_GridPcbType.SetTextBkColor(RGB(150,150,200));

	//m_GridPcbType.nLeftSize = 275;
	
	st_Pcb_info.nPcbType = UDIMM_10;
	m_dgtPcbRowCnt.SetVal(1);
	m_dgtPcbColCnt.SetVal(10);

	m_nRowCnt[1] = 1;
	m_nColCnt[1] = 10;
	
	mn_Size = 18;
	nTemp = 10;
	for (i=0; i<1; i++) 
	{
		m_GridPcbType.SetRowHeight(i, 74);

		for (j=0; j<10; j++) 
		{

			strTmp.Format(_T("%d"), nTemp--);

			if (i == 0)
			{
				mn_Size = mn_Size +18;
			}

			m_GridPcbType.SetColumnWidth(j, 19);
			m_GridPcbType.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_GridPcbType.SetItemState(i, j, GVIS_READONLY);

			if (j >= 5)
			{
				//st_Pcb_info.nPcbSerial[i][j] = TRUE;
				st_Pcb_info.nPcbSelect[i][j] = TRUE;
				m_GridPcbType.SetItemBkColour(i, j, GREEN_C, CLR_DEFAULT);
			}
			else
			{
				m_GridPcbType.SetItemBkColour(i, j, RGB(255, 255, 128), CLR_DEFAULT);
			}
			
			m_GridPcbType.SetItemText(i, j, strTmp);
		}
	}	

	st_Pcb_info.nLeftSize = mn_Size;

	rt.left =310;	rt.top = 148;	rt.right = rt.left + mn_Size;	rt.bottom = rt.top + 80;
	GetDlgItem(IDC_CUSTOM_MODE )->MoveWindow( &rt );
}




void CScreenBasic::OnBnClickedRadioBasicPickCenter()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_RobotPickPos[1] = 0;
	
}


void CScreenBasic::OnRadioBasicPickRight()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_RobotPickPos[1] = 1;
}


void CScreenBasic::OnRadioBasicPickLeft()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_RobotPickPos[1] = 2;
}


void CScreenBasic::OnBnClickedRadioBasicPlaceCenter()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_RobotPlacePos[1] =0;
}


void CScreenBasic::OnRadioBasicPlaceRight()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_RobotPlacePos[1] = 1;
}


void CScreenBasic::OnRadioBasicPlaceLeft()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_RobotPlacePos[1] = 2;
}


void CScreenBasic::OnStnClickedDgtVisionErrorCnt()
{
	CDialog_KeyPad dlgKeyPad;

	dlgKeyPad.m_nKeypadMode			= 0;
	dlgKeyPad.m_strKeypadLowLimit	= _T("0");
	dlgKeyPad.m_strKeypadHighLimit	= _T("10");
	dlgKeyPad.m_strKeypadVal.Format(_T("%d"), m_nVisionErrorCnt[1]);

	if (dlgKeyPad.DoModal() == IDOK)
	{
		m_nVisionErrorCnt[1] = _wtoi(dlgKeyPad.m_strNewVal);
		m_dgtVisionErrorCnt.SetVal(m_nVisionErrorCnt[1]);
	}
}
