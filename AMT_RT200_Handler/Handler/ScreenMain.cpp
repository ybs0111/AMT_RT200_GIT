// ScreenMain.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Handler.h"
#include "MainFrm.h"
#include "ScreenMain.h"
#include "Variable.h"
#include "Dialog_Message.h"
#include "DialogMessageBox.h"
#include "Dialog_KeyBoard.h"
#include "PublicFunction.h"
#include "Ctlbd_Variable.h"
#include "ComizoaPublic.h"
#include "FastechPublic_IO.h"
#include "Cmmsdk.h"
#include "CmmsdkDef.h"
#include "FtpClient.h"
#include "ClientEcFirst.h"
#include "CtlBd_Library.h"
#include "Dialog_Message.h"
#include "DialogMessageView.h"
#include "AlgMemory.h"
#include "RunConveyor.h"
#include "RunRobot.h"

#define TM_FRONT_SMEMA		100
#define TM_REAR_SMEMA		200
#define TM_INTERFACE		300
#define TM_DOOR				400
#define TM_DATA_DISPLAY		500


IMPLEMENT_DYNCREATE(CScreenMain, CFormView)

CScreenMain::CScreenMain()
	: CFormView(CScreenMain::IDD)
{
	m_bmGround.LoadBitmap(IDB_BITMAP_BACK_GROUND);
	m_nTemp = 0;
	mbTurn = false;
	m_pImagelist		= NULL;
}

CScreenMain::~CScreenMain()
{
	if (m_pImagelist != NULL)
	{
		delete m_pImagelist;
		m_pImagelist = NULL;
	}
}

void CScreenMain::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CUSTOM_MAIN_WORK_INFO_C,				m_pGridWorkInfoC);
	DDX_Control(pDX, IDC_CUSTOM_MAIN_WORK_INFO_N,				m_pGridWorkInfoN);
	DDX_Control(pDX, IDC_CUSTOM_TIME_INFO,						m_pGridTimeInfo);
	DDX_Control(pDX, IDC_GROUP_TIME_INFO,						m_groupTimeInfo);
	DDX_Control(pDX, IDC_GROUP_WORK_INFO,						m_groupWorkInfo);
	DDX_Control(pDX, IDC_GROUP_CONVEYOR, m_group_Conv);
	DDX_Control(pDX, IDC_OUT_CONVEYOR, m_group_OutConv);
	DDX_Control(pDX, IDC_TURN_CONVEYOR, m_group_TurnConv);
	DDX_Control(pDX, IDC_IN_CONVEYOR, m_group_InConv);
	DDX_Control(pDX, IDC_GROUP_ROBOT, m_group_Robot);
	DDX_Control(pDX, IDC_GROUP_FEEDER, m_group_Feeder);
	DDX_Control(pDX, IDC_CHECK_CV_IN, m_Check_CV_In);
	DDX_Control(pDX, IDC_CHECK_IN, m_Check_In_pos);
	DDX_Control(pDX, IDC_CHECK_OUT_IN, m_Check_Out_InPos);
	DDX_Control(pDX, IDC_CHECK_TURN, m_Check_Turn_Pos);
	DDX_Control(pDX, IDC_CHECK_OUT, m_Check_Out_Pos);
	DDX_Control(pDX, IDC_CHECK_OUT_UP, m_Check_Out_Up);
	DDX_Control(pDX, IDC_CHECK_OUT_DOWN, m_Check_Out_Dw);
	DDX_Control(pDX, IDC_CHECK_TURN_UP, m_Check_TurnUp);
	DDX_Control(pDX, IDC_CHECK_TURN_DOWN, m_Check_TurnDw);
	DDX_Control(pDX, IDC_CHECK_IN_UP, m_Check_InUp);
	DDX_Control(pDX, IDC_CHECK_IN_DOWN, m_Check_InDw);
	DDX_Control(pDX, IDC_MSG_CV_IN, m_msg_Cv_In);
	DDX_Control(pDX, IDC_MSG_IN_POS, m_msg_In_Pos);
	DDX_Control(pDX, IDC_MSG_OUTIN_POS, m_msg_OutIn_Pos);
	DDX_Control(pDX, IDC_MSG_TURN_POS, m_msg_Turn_Pos);
	DDX_Control(pDX, IDC_MSG_OUT_POS, m_msg_Out_Pos);
	DDX_Control(pDX, IDC_MSG_IN_UP, m_msg_In_Up);
	DDX_Control(pDX, IDC_MSG_IN_DOWN, m_msg_In_Down);
	DDX_Control(pDX, IDC_MSG_TURN_UP, m_msg_Turn_Up);
	DDX_Control(pDX, IDC_MSG_TURN_DOWN, m_msg_Turn_Dw);
	DDX_Control(pDX, IDC_MSG_OUT_UP, m_msg_Out_Up);
	DDX_Control(pDX, IDC_MSG_OUT_DOWN, m_msg_Out_Dw);
	DDX_Control(pDX, IDC_RIGHT_TURN, m_msg_Right_Turn);
	DDX_Control(pDX, IDC_LEFT_TURN, m_msg_Left_Turn);
	DDX_Control(pDX, IDC_CUSTOM_MAIN, m_pGridRobotTurnPos);
	DDX_Control(pDX, IDC_CUSTOM_YIELD_LOT,					m_pGridYieldL);
	DDX_Control(pDX, IDC_CUSTOM_YIELD_DAILY,				m_pGridYieldD);


	DDX_Control(pDX, IDC_CUSTOM_FRONT_CONV_SMEMA_INFO, m_pGridFrontSmema);
	DDX_Control(pDX, IDC_CUSTOM_REAR_CONV_SMEMA_INFO, m_pGridRearSmema);
	DDX_Control(pDX, IDC_CUSTOM_TM, m_GridTm);
	DDX_Control(pDX, IDC_CUSTOM_TM2, m_GridTm_2);
	DDX_Control(pDX, IDC_CUSTOM_TM3, m_GridTm_3);
	DDX_Control(pDX, IDC_CUSTOM_BUFFER_DATA, m_pGridBufferData);
	DDX_Control(pDX, IDC_CUSTOM_PCB_DATA, m_pGridPcbData);
}

BEGIN_MESSAGE_MAP(CScreenMain, CFormView)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_MESSAGE(WM_WORK_DISPLAY,									OnMainWorkInfoDisplay)  // 출력용 사용자 정의 메시지
	ON_MESSAGE(WM_WORK_COMMAND,									OnMainWorkInfoCommand)  // 출력용 사용자 정의 메시지

	ON_MESSAGE(WM_PCB_CV_IN_MOVE_DRAW_MAIN,						OnMainCvInMoveDisplay) 
	ON_MESSAGE(WM_PCB_CV_TURN_MOVE_DRAW_MAIN,					OnMainCvTurnMoveDisplay) 
	ON_MESSAGE(WM_PCB_CV_OUT_MOVE_DRAW_MAIN,					OnMainCvOutMoveDisplay) 
	ON_MESSAGE(WM_MAIN_IO_DISPLAY,								OnMainIODisplay) 
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	//ON_NOTIFY(NM_CLICK, IDC_CUSTOM_WORK_INFO,		OnWorkCellClick)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_MAIN_ANIMATE,								&CScreenMain::OnClickedBtnMainAnimate)
	ON_BN_CLICKED(IDC_BTN_MAIN_DATA,								&CScreenMain::OnClickedBtnMainData)
	ON_BN_CLICKED(IDC_BUTTON_TRACK_OUT,								&CScreenMain::OnBnClickedButtonTrackOut)
	ON_NOTIFY(NM_CLICK, IDC_CUSTOM_TIME_INFO,						OnTimeCellClick)
	ON_NOTIFY(NM_CLICK, IDC_CUSTOM_MAIN_WORK_INFO_C,				OnWorkInfoCurrCellClick)
	ON_BN_CLICKED(IDC_BTN_START, &CScreenMain::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_CV_IN, &CScreenMain::OnBnClickedBtnCvIn)
	ON_BN_CLICKED(IDC_BTN_CV_IN_POS, &CScreenMain::OnBnClickedBtnCvInPos)
	ON_BN_CLICKED(IDC_BTN_CV_IN_UPDW, &CScreenMain::OnBnClickedBtnCvInUpdw)
	ON_BN_CLICKED(IDC_BTN_TURN_POS, &CScreenMain::OnBnClickedBtnTurnPos)
	ON_BN_CLICKED(IDC_BTN_TURN_UP_DW, &CScreenMain::OnBnClickedBtnTurnUpDw)
	ON_BN_CLICKED(IDC_BTN_OUT_IN_POS, &CScreenMain::OnBnClickedBtnOutInPos)
	ON_BN_CLICKED(IDC_BTN_OUT_POS, &CScreenMain::OnBnClickedBtnOutPos)
	ON_BN_CLICKED(IDC_BTN_OUT_POS_UP_DW, &CScreenMain::OnBnClickedBtnOutPosUpDw)
	ON_BN_CLICKED(IDC_BTN_STOP, &CScreenMain::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_STOP2, &CScreenMain::OnBnClickedBtnStop2)
	ON_BN_CLICKED(IDC_BTN_STOP3, &CScreenMain::OnBnClickedBtnStop3)
	ON_BN_CLICKED(IDC_BTN_START2, &CScreenMain::OnBnClickedBtnStart2)
	ON_BN_CLICKED(IDC_BTN_START3, &CScreenMain::OnBnClickedBtnStart3)
	ON_BN_CLICKED(IDC_BTN_START4, &CScreenMain::OnBnClickedBtnStart4)
	ON_BN_CLICKED(IDC_BTN_FRONT_SMEMA, &CScreenMain::OnBnClickedBtnFrontSmema)
	ON_BN_CLICKED(IDC_BTN_MID_CONV_REQ, &CScreenMain::OnBnClickedBtnMidConvReq)
	ON_BN_CLICKED(IDC_BTN_CV_IN_POS2, &CScreenMain::OnBnClickedBtnCvInPos2)
	ON_BN_CLICKED(IDC_BTN_OUT_POS_UP_DW2, &CScreenMain::OnBnClickedBtnOutPosUpDw2)
	ON_BN_CLICKED(IDC_BTN_OUT_POS_UP_DW3, &CScreenMain::OnBnClickedBtnOutPosUpDw3)
	ON_BN_CLICKED(IDC_BTN_CV_IN_POS3, &CScreenMain::OnBnClickedBtnCvInPos3)
	ON_BN_CLICKED(IDC_BTN_OUT_POS_UP_DW5, &CScreenMain::OnBnClickedBtnOutPosUpDw5)
	ON_BN_CLICKED(IDC_BTN_OUT_POS_UP_DW4, &CScreenMain::OnBnClickedBtnOutPosUpDw4)
END_MESSAGE_MAP()


// CScreenMain 진단입니다.

#ifdef _DEBUG
void CScreenMain::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CScreenMain::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

void CScreenMain::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	OnInitButton();
	OnInitGroupBox();
	OnInitLabel();
	OnInitDigit();
	OnInitGridWorkInfoCurr();
	OnInitGridWorkInfoNext();
	OnInitGridTimeInfo(st_work_info.nPgmInfo);	
	OnMainDisplay();
	OnInitGridPickerTurnPos();
	OnInitGridYieldLot();
	OnInitGridYieldDaily();
	OnInitGridFrontSmema();
	OnInitGridRearSmema();
	OninitTrayMove();
	OnInitGridPcbData();
	OnInitGridBufferData();
	SetTimer(TM_FRONT_SMEMA, 500, NULL);
	SetTimer(TM_REAR_SMEMA, 500, NULL);
	SetTimer(TM_DATA_DISPLAY, 1000, NULL);
	
	st_handler_info.cWndMain = this;

}


void CScreenMain::OnMainDisplay()
{
	OnMainCountDisplay();
	OnMainTimeDisplay();
	OnMainLotDisplay();
	
	if (st_handler_info.cWndTitle != NULL)
	{
		st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, DEVICE_MODE, st_basic_info.nModeDevice);			// Device Mode
		st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, INTERFACE_MODE, st_work_info.nFtpWork);
	}

	// jtkim 20150818
	if (st_track_info.nStatus == YES)
	{
		OnMainWorkInfoCommand(MAIN_TRACK_OUT_CREATE_REQ, 0);
	}
}


void CScreenMain::OnMainRobotLeftTurnDisplay()
{
	int   i, j;
	int	  nMaxRow, nMaxCol;
	int   nRow;
	CString strTmp;
	RECT rt;

	nMaxRow = 4; // unloader tray y count
	nMaxCol = 7;

	m_pGridRobotTurnPos.SetFrameFocusCell(FALSE);
	m_pGridRobotTurnPos.SetTrackFocusCell(FALSE);
	m_pGridRobotTurnPos.EnableSelection(FALSE);

	m_pGridRobotTurnPos.SetGridLineColor(BLACK_C);
	m_pGridRobotTurnPos.SetGridLines(1);

	m_pGridRobotTurnPos.SetRowCount(nMaxRow);
	m_pGridRobotTurnPos.SetColumnCount(nMaxCol);

	m_pGridRobotTurnPos.SetFixedRowCount(0);
	m_pGridRobotTurnPos.SetFixedColumnCount(0);
	m_pGridRobotTurnPos.SetFixedBkColor(RGB(0,0,200));
	m_pGridRobotTurnPos.SetFixedBkColor(RGB(200,200,255));
	m_pGridRobotTurnPos.SetTextBkColor(RGB(150,150,200));

	for (i=0; i<nMaxRow; i++) 
	{
		m_pGridRobotTurnPos.SetRowHeight(i, 40);

		for (j=0; j<nMaxCol; j++) 
		{
			if (i == 1&& j== 0 || i == 3 && j== 0) 
			{
				m_pGridRobotTurnPos.SetColumnWidth(j, 35);
			}
			else
			{
				m_pGridRobotTurnPos.SetColumnWidth(j,85);
			}

			m_pGridRobotTurnPos.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_pGridRobotTurnPos.SetItemState(i, j, GVIS_READONLY);
			m_pGridRobotTurnPos.SetItemBkColour(i, j, WHITE_C, CLR_DEFAULT);
		}
	}

	nRow = 0;
	m_pGridRobotTurnPos.SetItemBkColour(nRow, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 0, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 0, _T(""));

	m_pGridRobotTurnPos.SetItemBkColour(nRow, 1, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 1, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 1, _T("1"));

	m_pGridRobotTurnPos.SetItemBkColour(nRow, 2, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 2, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 2, _T("2"));

	m_pGridRobotTurnPos.SetItemBkColour(nRow, 3, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 3, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 3, _T("3"));

	m_pGridRobotTurnPos.SetItemBkColour(nRow, 4, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 4, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 4, _T("4"));

	m_pGridRobotTurnPos.SetItemBkColour(nRow, 5, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 5, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 5, _T("5"));

	m_pGridRobotTurnPos.SetItemBkColour(nRow, 6, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 6, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 6, _T("6"));

	nRow = 1;
	m_pGridRobotTurnPos.SetItemBkColour(nRow, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 0, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 0, _T("L"));

	nRow = 2;
	m_pGridRobotTurnPos.SetItemBkColour(nRow, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 0, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 0, _T(""));

	m_pGridRobotTurnPos.SetItemBkColour(nRow, 1, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 1, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 1, _T("7"));

	m_pGridRobotTurnPos.SetItemBkColour(nRow, 2, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 2, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 2, _T("8"));

	m_pGridRobotTurnPos.SetItemBkColour(nRow, 3, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 3, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 3, _T("9"));

	m_pGridRobotTurnPos.SetItemBkColour(nRow, 4, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 4, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 4, _T("10"));

	m_pGridRobotTurnPos.SetItemBkColour(nRow, 5, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 5, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 5, _T("11"));

	m_pGridRobotTurnPos.SetItemBkColour(nRow, 6, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 6, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 6, _T("12"));

	nRow = 3;
	m_pGridRobotTurnPos.SetItemBkColour(nRow, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 0, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 0, _T("R"));


	rt.left = 409;	rt.top = 308;	rt.right = rt.left + 550;	rt.bottom = rt.top + 175;
	GetDlgItem(IDC_CUSTOM_MAIN )->MoveWindow( &rt );
	m_pGridRobotTurnPos.Invalidate(FALSE);
	m_pGridRobotTurnPos.Refresh();
}

void CScreenMain::OnMainRobotCenterDisplay()
{
	int   i, j;
	int	  nMaxRow, nMaxCol;
//	int   nRow;
	CString strTmp;

	RECT rt;

	nMaxRow = 7; // unloader tray y count
	nMaxCol = 4;

	m_pGridRobotTurnPos.SetFrameFocusCell(FALSE);
	m_pGridRobotTurnPos.SetTrackFocusCell(FALSE);
	m_pGridRobotTurnPos.EnableSelection(FALSE);

	m_pGridRobotTurnPos.SetGridLineColor(BLACK_C);
	m_pGridRobotTurnPos.SetGridLines(1);

	m_pGridRobotTurnPos.SetRowCount(nMaxRow);
	m_pGridRobotTurnPos.SetColumnCount(nMaxCol);

	m_pGridRobotTurnPos.SetFixedRowCount(0);
	m_pGridRobotTurnPos.SetFixedColumnCount(0);
	m_pGridRobotTurnPos.SetFixedBkColor(RGB(0,0,200));
	m_pGridRobotTurnPos.SetFixedBkColor(RGB(200,200,255));
	m_pGridRobotTurnPos.SetTextBkColor(RGB(150,150,200));

	for (i=0; i<7; i++) 
	{
		if (i ==6)
		{
			m_pGridRobotTurnPos.SetRowHeight(i, 35);
		}
		else
		{
			m_pGridRobotTurnPos.SetRowHeight(i, 70);
		}
		for (j=0; j<4; j++) 
		{
// 			if (i == 1&& j== 0 || i == 3 && j== 0) 
// 			{
// 				m_pGridRobotTurnPos.SetColumnWidth(j, 35);
// 			}
// 			else
// 			{
			m_pGridRobotTurnPos.SetColumnWidth(j,40);
		//	}
			

			m_pGridRobotTurnPos.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_pGridRobotTurnPos.SetItemState(i, j, GVIS_READONLY);
			m_pGridRobotTurnPos.SetItemBkColour(i, j, WHITE_C, CLR_DEFAULT);
			m_pGridRobotTurnPos.SetItemFont(i, j, &clsFunc.OnLogFont(18));
			m_pGridRobotTurnPos.SetItemText(i, j, _T(""));
			if (i == 6&& j== 0|| i == 6&& j== 1 || i == 6&& j== 2|| i == 6&& j== 3)
			{
				m_pGridRobotTurnPos.SetItemBkColour(i, j, RGB(219, 229, 241), BLACK_L);

				
			}
			 if (j== 0 || j ==2)
			{
				m_pGridRobotTurnPos.SetItemBkColour(i, j, RGB(219, 229, 241), BLACK_L);
				m_pGridRobotTurnPos.SetItemFont(i, j, &clsFunc.OnLogFont(18));
				m_pGridRobotTurnPos.SetItemText(i, j, _T(""));

			}
		}

	}

	m_pGridRobotTurnPos.SetItemBkColour(0, 2, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(0, 2, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(0, 2, _T("12"));

	m_pGridRobotTurnPos.SetItemBkColour(1, 2, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(1, 2, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(1, 2, _T("11"));

	m_pGridRobotTurnPos.SetItemBkColour(2, 2, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(2, 2, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(2, 2, _T("10"));

	m_pGridRobotTurnPos.SetItemBkColour(3, 2, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(3, 2, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(3, 2, _T("9"));

	m_pGridRobotTurnPos.SetItemBkColour(4, 2, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(4, 2, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(4, 2, _T("8"));

	m_pGridRobotTurnPos.SetItemBkColour(5, 2, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(5, 2, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(5, 2, _T("7"));

	m_pGridRobotTurnPos.SetItemBkColour(6, 3, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(6, 3, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(6, 3, _T("R"));


	m_pGridRobotTurnPos.SetItemBkColour(0, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(0, 0, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(0, 0, _T("6"));

	m_pGridRobotTurnPos.SetItemBkColour(1, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(1, 0, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(1, 0, _T("5"));

	m_pGridRobotTurnPos.SetItemBkColour(2, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(2, 0, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(2, 0, _T("4"));

	m_pGridRobotTurnPos.SetItemBkColour(3, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(3, 0, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(3, 0, _T("3"));

	m_pGridRobotTurnPos.SetItemBkColour(4, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(4, 0, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(4, 0, _T("2"));

	m_pGridRobotTurnPos.SetItemBkColour(5, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(5, 0, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(5, 0, _T("1"));

	m_pGridRobotTurnPos.SetItemBkColour(6, 1, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(6, 1, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(6, 1, _T("L"));

	

	rt.left = 609;	rt.top = 30;	rt.right = rt.left + 168;	rt.bottom = rt.top + 460;
	GetDlgItem(IDC_CUSTOM_MAIN )->MoveWindow( &rt );

	m_pGridRobotTurnPos.Refresh();
	m_pGridRobotTurnPos.Invalidate(TRUE);
	
}

void CScreenMain::OnMainRobotRightTurnDisplay()
{
	int   i, j;
	int	  nMaxRow, nMaxCol;
	int   nRow;
	CString strTmp;
	
	RECT rt;

	nMaxRow = 4; // unloader tray y count
	nMaxCol = 7;

	m_pGridRobotTurnPos.SetFrameFocusCell(FALSE);
	m_pGridRobotTurnPos.SetTrackFocusCell(FALSE);
	m_pGridRobotTurnPos.EnableSelection(FALSE);

	m_pGridRobotTurnPos.SetGridLineColor(BLACK_C);
	m_pGridRobotTurnPos.SetGridLines(1);

	m_pGridRobotTurnPos.SetRowCount(nMaxRow);
	m_pGridRobotTurnPos.SetColumnCount(nMaxCol);

	m_pGridRobotTurnPos.SetFixedRowCount(0);
	m_pGridRobotTurnPos.SetFixedColumnCount(0);
	m_pGridRobotTurnPos.SetFixedBkColor(RGB(0,0,200));
	m_pGridRobotTurnPos.SetFixedBkColor(RGB(200,200,255));
	m_pGridRobotTurnPos.SetTextBkColor(RGB(150,150,200));

	for (i=0; i<nMaxRow; i++) 
	{
		m_pGridRobotTurnPos.SetRowHeight(i, 40);

		for (j=0; j<nMaxCol; j++) 
		{
			if (i == 1&& j== 0 || i == 3 && j== 0) 
			{
				m_pGridRobotTurnPos.SetColumnWidth(j, 35);
			}
			else
			{
				m_pGridRobotTurnPos.SetColumnWidth(j,85);
			}

			m_pGridRobotTurnPos.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_pGridRobotTurnPos.SetItemState(i, j, GVIS_READONLY);
			m_pGridRobotTurnPos.SetItemBkColour(i, j, WHITE_C, CLR_DEFAULT);
		}
	}

	nRow = 0;
	m_pGridRobotTurnPos.SetItemBkColour(nRow, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 0, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 0, _T(""));

	m_pGridRobotTurnPos.SetItemBkColour(nRow, 1, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 1, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 1, _T("12"));

	m_pGridRobotTurnPos.SetItemBkColour(nRow, 2, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 2, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 2, _T("11"));

	m_pGridRobotTurnPos.SetItemBkColour(nRow, 3, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 3, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 3, _T("10"));

	m_pGridRobotTurnPos.SetItemBkColour(nRow, 4, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 4, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 4, _T("9"));

	m_pGridRobotTurnPos.SetItemBkColour(nRow, 5, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 5, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 5, _T("8"));

	m_pGridRobotTurnPos.SetItemBkColour(nRow, 6, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 6, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 6, _T("7"));

	nRow = 1;
	m_pGridRobotTurnPos.SetItemBkColour(nRow, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 0, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 0, _T("R"));

	nRow = 2;
	m_pGridRobotTurnPos.SetItemBkColour(nRow, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 0, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 0, _T(""));

	m_pGridRobotTurnPos.SetItemBkColour(nRow, 1, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 1, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 1, _T("6"));

	m_pGridRobotTurnPos.SetItemBkColour(nRow, 2, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 2, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 2, _T("5"));

	m_pGridRobotTurnPos.SetItemBkColour(nRow, 3, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 3, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 3, _T("4"));

	m_pGridRobotTurnPos.SetItemBkColour(nRow, 4, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 4, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 4, _T("3"));

	m_pGridRobotTurnPos.SetItemBkColour(nRow, 5, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 5, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 5, _T("2"));

	m_pGridRobotTurnPos.SetItemBkColour(nRow, 6, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 6, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 6, _T("1"));

	nRow = 3;
	m_pGridRobotTurnPos.SetItemBkColour(nRow, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 0, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 0, _T("L"));


	rt.left = 409;	rt.top = 308;	rt.right = rt.left + 550;	rt.bottom = rt.top + 175;
	GetDlgItem(IDC_CUSTOM_MAIN )->MoveWindow( &rt );
	
	m_pGridRobotTurnPos.Refresh();

	m_pGridRobotTurnPos.Invalidate(FALSE);
	
}

void CScreenMain::OnInitGridPcbData()
{
	int   i, j;
	int	  nMaxRow, nMaxCol;
	int   nRow;
	CString strTmp;

	nMaxRow = 4; // unloader tray y count
	nMaxCol = 10;

	m_pGridPcbData.SetFrameFocusCell(FALSE);
	m_pGridPcbData.SetTrackFocusCell(FALSE);
	m_pGridPcbData.EnableSelection(FALSE);

	m_pGridPcbData.SetGridLineColor(BLACK_C);
	m_pGridPcbData.SetGridLines(1);

	m_pGridPcbData.SetRowCount(nMaxRow);
	m_pGridPcbData.SetColumnCount(nMaxCol);

	m_pGridPcbData.SetFixedRowCount(0);
	m_pGridPcbData.SetFixedColumnCount(0);
	m_pGridPcbData.SetFixedBkColor(RGB(0,0,200));
	m_pGridPcbData.SetFixedBkColor(RGB(200,200,255));
	m_pGridPcbData.SetTextBkColor(RGB(150,150,200));

	for (i=0; i<nMaxRow; i++) 
	{
		m_pGridPcbData.SetRowHeight(i, 30);

		for (j=0; j<nMaxCol; j++) 
		{
		
			if (j == 0)
			{
				m_pGridPcbData.SetColumnWidth(j,55);
			}
			else
			{
				m_pGridPcbData.SetColumnWidth(j,66);
			}

			m_pGridPcbData.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_pGridPcbData.SetItemState(i, j, GVIS_READONLY);
			m_pGridPcbData.SetItemBkColour(i, j, WHITE_C, CLR_DEFAULT);
		}
	}

	nRow = 0;
	m_pGridPcbData.MergeCells(nRow,0,nRow + 1,0);
	m_pGridPcbData.SetItemBkColour(nRow, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridPcbData.SetItemFont(nRow, 0, &clsFunc.OnLogFont(15));
	m_pGridPcbData.SetItemText(nRow, 0, _T("Top"));

	m_pGridPcbData.SetItemBkColour(nRow, 1, RGB(219, 229, 241), BLACK_L);
	m_pGridPcbData.SetItemFont(nRow, 1, &clsFunc.OnLogFont(15));
	m_pGridPcbData.SetItemText(nRow, 1, _T("1"));

	m_pGridPcbData.SetItemBkColour(nRow, 2, RGB(219, 229, 241), BLACK_L);
	m_pGridPcbData.SetItemFont(nRow, 2, &clsFunc.OnLogFont(15));
	m_pGridPcbData.SetItemText(nRow, 2, _T("2"));

	m_pGridPcbData.SetItemBkColour(nRow, 3, RGB(219, 229, 241), BLACK_L);
	m_pGridPcbData.SetItemFont(nRow, 3, &clsFunc.OnLogFont(15));
	m_pGridPcbData.SetItemText(nRow, 3, _T("3"));

	m_pGridPcbData.SetItemBkColour(nRow, 4, RGB(219, 229, 241), BLACK_L);
	m_pGridPcbData.SetItemFont(nRow, 4, &clsFunc.OnLogFont(15));
	m_pGridPcbData.SetItemText(nRow, 4, _T("4"));

	m_pGridPcbData.SetItemBkColour(nRow, 5, RGB(219, 229, 241), BLACK_L);
	m_pGridPcbData.SetItemFont(nRow, 5, &clsFunc.OnLogFont(15));
	m_pGridPcbData.SetItemText(nRow, 5, _T("5"));

	m_pGridPcbData.SetItemBkColour(nRow, 6, RGB(219, 229, 241), BLACK_L);
	m_pGridPcbData.SetItemFont(nRow, 6, &clsFunc.OnLogFont(15));
	m_pGridPcbData.SetItemText(nRow, 6, _T("6"));

	m_pGridPcbData.SetItemBkColour(nRow, 7, RGB(219, 229, 241), BLACK_L);
	m_pGridPcbData.SetItemFont(nRow, 7, &clsFunc.OnLogFont(15));
	m_pGridPcbData.SetItemText(nRow, 7, _T("7"));

	m_pGridPcbData.SetItemBkColour(nRow, 8, RGB(219, 229, 241), BLACK_L);
	m_pGridPcbData.SetItemFont(nRow, 8, &clsFunc.OnLogFont(15));
	m_pGridPcbData.SetItemText(nRow, 8, _T("8"));

	m_pGridPcbData.SetItemBkColour(nRow, 9, RGB(219, 229, 241), BLACK_L);
	m_pGridPcbData.SetItemFont(nRow, 9, &clsFunc.OnLogFont(15));
	m_pGridPcbData.SetItemText(nRow, 9, _T("9"));
	

	nRow = 2;
	m_pGridPcbData.MergeCells(nRow,0,nRow + 1,0);
	m_pGridPcbData.SetItemBkColour(nRow, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridPcbData.SetItemFont(nRow, 0, &clsFunc.OnLogFont(15));
	m_pGridPcbData.SetItemText(nRow, 0, _T("Bottom"));

	m_pGridPcbData.SetItemBkColour(nRow, 1, RGB(219, 229, 241), BLACK_L);
	m_pGridPcbData.SetItemFont(nRow, 1, &clsFunc.OnLogFont(15));
	m_pGridPcbData.SetItemText(nRow, 1, _T("9"));

	m_pGridPcbData.SetItemBkColour(nRow, 2, RGB(219, 229, 241), BLACK_L);
	m_pGridPcbData.SetItemFont(nRow, 2, &clsFunc.OnLogFont(15));
	m_pGridPcbData.SetItemText(nRow, 2, _T("8"));

	m_pGridPcbData.SetItemBkColour(nRow, 3, RGB(219, 229, 241), BLACK_L);
	m_pGridPcbData.SetItemFont(nRow, 3, &clsFunc.OnLogFont(15));
	m_pGridPcbData.SetItemText(nRow, 3, _T("7"));

	m_pGridPcbData.SetItemBkColour(nRow, 4, RGB(219, 229, 241), BLACK_L);
	m_pGridPcbData.SetItemFont(nRow, 4, &clsFunc.OnLogFont(15));
	m_pGridPcbData.SetItemText(nRow, 4, _T("6"));

	m_pGridPcbData.SetItemBkColour(nRow, 5, RGB(219, 229, 241), BLACK_L);
	m_pGridPcbData.SetItemFont(nRow, 5, &clsFunc.OnLogFont(15));
	m_pGridPcbData.SetItemText(nRow, 5, _T("5"));

	m_pGridPcbData.SetItemBkColour(nRow, 6, RGB(219, 229, 241), BLACK_L);
	m_pGridPcbData.SetItemFont(nRow, 6, &clsFunc.OnLogFont(15));
	m_pGridPcbData.SetItemText(nRow, 6, _T("4"));

	m_pGridPcbData.SetItemBkColour(nRow, 7, RGB(219, 229, 241), BLACK_L);
	m_pGridPcbData.SetItemFont(nRow, 7, &clsFunc.OnLogFont(15));
	m_pGridPcbData.SetItemText(nRow, 7, _T("3"));

	m_pGridPcbData.SetItemBkColour(nRow, 8, RGB(219, 229, 241), BLACK_L);
	m_pGridPcbData.SetItemFont(nRow, 8, &clsFunc.OnLogFont(15));
	m_pGridPcbData.SetItemText(nRow, 8, _T("2"));

	m_pGridPcbData.SetItemBkColour(nRow, 9, RGB(219, 229, 241), BLACK_L);
	m_pGridPcbData.SetItemFont(nRow, 9, &clsFunc.OnLogFont(15));
	m_pGridPcbData.SetItemText(nRow, 9, _T("1"));



	m_pGridPcbData.Refresh();
	m_pGridPcbData.Invalidate(FALSE);
}

void CScreenMain::OnInitGridPickerTurnPos()
{
	int   i, j;
	int	  nMaxRow, nMaxCol;
	int   nRow;
	CString strTmp;

	nMaxRow = 4; // unloader tray y count
	nMaxCol = 6;

	m_pGridRobotTurnPos.SetFrameFocusCell(FALSE);
	m_pGridRobotTurnPos.SetTrackFocusCell(FALSE);
	m_pGridRobotTurnPos.EnableSelection(FALSE);

	m_pGridRobotTurnPos.SetGridLineColor(BLACK_C);
	m_pGridRobotTurnPos.SetGridLines(1);

	m_pGridRobotTurnPos.SetRowCount(nMaxRow);
	m_pGridRobotTurnPos.SetColumnCount(nMaxCol);

	m_pGridRobotTurnPos.SetFixedRowCount(0);
	m_pGridRobotTurnPos.SetFixedColumnCount(0);
	m_pGridRobotTurnPos.SetFixedBkColor(RGB(0,0,200));
	m_pGridRobotTurnPos.SetFixedBkColor(RGB(200,200,255));
	m_pGridRobotTurnPos.SetTextBkColor(RGB(150,150,200));

	for (i=0; i<nMaxRow; i++) 
	{
		m_pGridRobotTurnPos.SetRowHeight(i, 31);

		for (j=0; j<nMaxCol; j++) 
		{
// 			if (i == 1&& j== 0 || i == 3 && j== 0) 
// 			{
// 				m_pGridRobotTurnPos.SetColumnWidth(j, 35);
// 			}
// 			else
// 			{
				m_pGridRobotTurnPos.SetColumnWidth(j,103);
	//		}

			m_pGridRobotTurnPos.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_pGridRobotTurnPos.SetItemState(i, j, GVIS_READONLY);
			m_pGridRobotTurnPos.SetItemBkColour(i, j, WHITE_C, CLR_DEFAULT);
		}
	}

	nRow = 0;
	m_pGridRobotTurnPos.MergeCells(nRow,0,nRow + 1,0);
	m_pGridRobotTurnPos.SetItemBkColour(nRow, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 0, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 0, _T("LEFT"));

	m_pGridRobotTurnPos.SetItemBkColour(nRow, 1, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 1, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 1, _T("1"));

	m_pGridRobotTurnPos.SetItemBkColour(nRow, 2, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 2, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 2, _T("2"));

	m_pGridRobotTurnPos.SetItemBkColour(nRow, 3, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 3, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 3, _T("3"));

	m_pGridRobotTurnPos.SetItemBkColour(nRow, 4, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 4, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 4, _T("4"));

	m_pGridRobotTurnPos.SetItemBkColour(nRow, 5, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 5, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 5, _T("5"));

// 	m_pGridRobotTurnPos.SetItemBkColour(nRow, 6, RGB(219, 229, 241), BLACK_L);
// 	m_pGridRobotTurnPos.SetItemFont(nRow, 6, &clsFunc.OnLogFont(18));
// 	m_pGridRobotTurnPos.SetItemText(nRow, 6, _T("6"));

//  	nRow = 1;
//  	m_pGridRobotTurnPos.SetItemBkColour(nRow, 0, RGB(219, 229, 241), BLACK_L);
//  	m_pGridRobotTurnPos.SetItemFont(nRow, 0, &clsFunc.OnLogFont(18));
//  	m_pGridRobotTurnPos.SetItemText(nRow, 0, _T("_"));

	nRow = 2;
	m_pGridRobotTurnPos.MergeCells(nRow,0,nRow + 1,0);
	m_pGridRobotTurnPos.SetItemBkColour(nRow, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 0, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 0, _T("RIGHT"));

	m_pGridRobotTurnPos.SetItemBkColour(nRow, 1, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 1, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 1, _T("6"));

	m_pGridRobotTurnPos.SetItemBkColour(nRow, 2, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 2, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 2, _T("7"));

	m_pGridRobotTurnPos.SetItemBkColour(nRow, 3, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 3, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 3, _T("8"));

	m_pGridRobotTurnPos.SetItemBkColour(nRow, 4, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 4, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 4, _T("9"));

	m_pGridRobotTurnPos.SetItemBkColour(nRow, 5, RGB(219, 229, 241), BLACK_L);
	m_pGridRobotTurnPos.SetItemFont(nRow, 5, &clsFunc.OnLogFont(18));
	m_pGridRobotTurnPos.SetItemText(nRow, 5, _T("10"));

// 	m_pGridRobotTurnPos.SetItemBkColour(nRow, 6, RGB(219, 229, 241), BLACK_L);
// 	m_pGridRobotTurnPos.SetItemFont(nRow, 6, &clsFunc.OnLogFont(18));
// 	m_pGridRobotTurnPos.SetItemText(nRow, 6, _T("_"));


	m_pGridRobotTurnPos.Refresh();
	m_pGridRobotTurnPos.Invalidate(FALSE);
	
}


BOOL CScreenMain::DestroyWindow()
{
	KillTimer(TM_DATA_DISPLAY); //kwlee 2017.0218
	KillTimer(TM_FRONT_SMEMA);
	KillTimer(TM_REAR_SMEMA);
	KillTimer(TM_INTERFACE);
	KillTimer(TM_DOOR);
	KillTimer(500);

	if (m_pImagelist != NULL)
	{
		delete m_pImagelist;
		m_pImagelist = NULL;
	}

	st_handler_info.cWndMain = NULL;

	return CFormView::DestroyWindow();
}


BOOL CScreenMain::OnEraseBkgnd(CDC* pDC)
{
	CDC  memDC;                     // 메모리 DC
	CBitmap * pOldBitmap = NULL;    // m_BackBitmap으로 새로운 그림을 DC에 그릴 때, 이전 그려졌던 DC(즉, Bitmap)을 저장.
	BITMAP bitmapInfo;              // 그림의 정보(m_BackBitmap)
	st_handler_info.m_BackBitmap.GetBitmap(&bitmapInfo); // Bitmap 크기 구함.

	memDC.CreateCompatibleDC(pDC);  // 메모리 DC 생성
	pOldBitmap = memDC.SelectObject(&m_bmGround);
	pDC->BitBlt(0, 0, bitmapInfo.bmWidth, bitmapInfo.bmHeight, &memDC, 0, 0, SRCCOPY);   // 메모리 DC에 그림을 그림

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();

	return true; // 흰 부러쉬가 아닌 배경을 그려야 하기 때문에 true
}


void CScreenMain::OnInitButton()
{

	short	shBtnColor = 30;

	m_Check_CV_In.SetIcon(IDI_GREEN_LED_ICON,IDI_ICON_LED_ON);
	m_Check_CV_In.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_Check_CV_In.SetButtonColor(1);

	m_Check_In_pos.SetIcon(IDI_GREEN_LED_ICON,IDI_ICON_LED_ON );
	m_Check_In_pos.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_Check_In_pos.SetButtonColor(1);

	m_Check_InUp.SetIcon(IDI_ICON_UP,IDI_GRAY_LED_ICON );
	m_Check_InUp.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_Check_InUp.SetButtonColor(1);

	m_Check_InDw.SetIcon(IDI_GRAY_LED_ICON,IDI_ICON_UP );
	m_Check_InDw.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_Check_InDw.SetButtonColor(1);

	m_Check_Out_InPos.SetIcon(IDI_GREEN_LED_ICON,IDI_ICON_LED_ON );
	m_Check_Out_InPos.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_Check_Out_InPos.SetButtonColor(1);

	m_Check_Turn_Pos.SetIcon(IDI_GREEN_LED_ICON,IDI_ICON_LED_ON);
	m_Check_Turn_Pos.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_Check_Turn_Pos.SetButtonColor(1);

	m_Check_TurnUp.SetIcon(IDI_ICON_UP,IDI_GRAY_LED_ICON );
	m_Check_TurnUp.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_Check_TurnUp.SetButtonColor(1);

	m_Check_TurnDw.SetIcon(IDI_GRAY_LED_ICON,IDI_ICON_UP);
	m_Check_TurnDw.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_Check_TurnDw.SetButtonColor(1);

	m_Check_Out_Pos.SetIcon(IDI_GREEN_LED_ICON,IDI_ICON_LED_ON);
	m_Check_Out_Pos.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_Check_Out_Pos.SetButtonColor(1);

	m_Check_Out_Up.SetIcon(IDI_ICON_UP,IDI_GRAY_LED_ICON );
	m_Check_Out_Up.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_Check_Out_Up.SetButtonColor(1);

	m_Check_Out_Dw.SetIcon(IDI_GRAY_LED_ICON,IDI_ICON_UP );
	m_Check_Out_Dw.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_Check_Out_Dw.SetButtonColor(1);
}

void CScreenMain::OnInitDigit()
{

}


void CScreenMain::OnInitLabel()
{

	
	m_msg_Cv_In.SetFont(clsFunc.m_pFont[5]);
	m_msg_Cv_In.SetWindowText(_T("Conv_In"));
	m_msg_Cv_In.SetCenterText();
	m_msg_Cv_In.SetColor(RGB(250,255,254));
	//m_msg_Cv_In.SetGradientColor(ORANGE_C);
	m_msg_Cv_In.SetTextColor(BLUE_C);

	m_msg_In_Pos.SetFont(clsFunc.m_pFont[5]);
	m_msg_In_Pos.SetWindowText(_T("Conv_In_Pos"));
	m_msg_In_Pos.SetCenterText();
	m_msg_In_Pos.SetColor(RGB(250,255,254));
	m_msg_In_Pos.SetTextColor(BLUE_C);

	m_msg_OutIn_Pos.SetFont(clsFunc.m_pFont[5]);
	m_msg_OutIn_Pos.SetWindowText(_T("Conv_Out_In_Pos"));
	m_msg_OutIn_Pos.SetCenterText();
	m_msg_OutIn_Pos.SetColor(RGB(250,255,254));
	m_msg_OutIn_Pos.SetTextColor(BLUE_C);

	m_msg_Turn_Pos.SetFont(clsFunc.m_pFont[5]);
	m_msg_Turn_Pos.SetWindowText(_T("Conv_Turn_Pos"));
	m_msg_Turn_Pos.SetCenterText();
	m_msg_Turn_Pos.SetColor(RGB(250,255,254));
	m_msg_Turn_Pos.SetTextColor(BLUE_C);

	m_msg_Out_Pos.SetFont(clsFunc.m_pFont[5]);
	m_msg_Out_Pos.SetWindowText(_T("Conv_Out_Pos"));
	m_msg_Out_Pos.SetCenterText();
	m_msg_Out_Pos.SetColor(RGB(250,255,254));
	m_msg_Out_Pos.SetTextColor(BLUE_C);


	m_msg_In_Up.SetFont(clsFunc.m_pFont[5]);
	m_msg_In_Up.SetWindowText(_T("Up"));
	m_msg_In_Up.SetCenterText();
	m_msg_In_Up.SetColor(RGB(250,255,254));
	m_msg_In_Up.SetTextColor(BLUE_C);

	m_msg_In_Down.SetFont(clsFunc.m_pFont[5]);
	m_msg_In_Down.SetWindowText(_T("Down"));
	m_msg_In_Down.SetCenterText();
	m_msg_In_Down.SetColor(RGB(250,255,254));
	m_msg_In_Down.SetTextColor(BLUE_C);

	m_msg_Turn_Up.SetFont(clsFunc.m_pFont[5]);
	m_msg_Turn_Up.SetWindowText(_T("Up"));
	m_msg_Turn_Up.SetCenterText();
	m_msg_Turn_Up.SetColor(RGB(250,255,254));
	m_msg_Turn_Up.SetTextColor(BLUE_C);

	m_msg_Turn_Dw.SetFont(clsFunc.m_pFont[5]);
	m_msg_Turn_Dw.SetWindowText(_T("Down"));
	m_msg_Turn_Dw.SetCenterText();
	m_msg_Turn_Dw.SetColor(RGB(250,255,254));
	m_msg_Turn_Dw.SetTextColor(BLUE_C);

	m_msg_Out_Up.SetFont(clsFunc.m_pFont[5]);
	m_msg_Out_Up.SetWindowText(_T("Up"));
	m_msg_Out_Up.SetCenterText();
	m_msg_Out_Up.SetColor(RGB(250,255,254));
	m_msg_Out_Up.SetTextColor(BLUE_C);

	m_msg_Out_Dw.SetFont(clsFunc.m_pFont[5]);
	m_msg_Out_Dw.SetWindowText(_T("Down"));
	m_msg_Out_Dw.SetCenterText();
	m_msg_Out_Dw.SetColor(RGB(250,255,254));
	m_msg_Out_Dw.SetTextColor(BLUE_C);

	m_msg_Right_Turn.SetFont(clsFunc.m_pFont[5]);
	m_msg_Right_Turn.SetWindowText(_T("RIGHT_TURN"));
	m_msg_Right_Turn.SetCenterText();
	m_msg_Right_Turn.SetColor(RGB(250,255,254));
	m_msg_Right_Turn.SetTextColor(BLUE_C);

	m_msg_Left_Turn.SetFont(clsFunc.m_pFont[5]);
	m_msg_Left_Turn.SetWindowText(_T("LEFT_TURN"));
	m_msg_Left_Turn.SetCenterText();
	m_msg_Left_Turn.SetColor(RGB(250,255,254));
	m_msg_Left_Turn.SetTextColor(BLUE_C);
}


void CScreenMain::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	CRect rect;

}

LRESULT CScreenMain::OnMainWorkInfoCommand(WPARAM wParam, LPARAM lParam) 
{

	return 0;
}
int CScreenMain::GetCustomTrayMove(int iIdx)
{
	switch( iIdx )
	{
	case 0:			return	IDC_CUSTOM_TM;
	case 1:			return	IDC_CUSTOM_TM;//inConveyor_Position
	case 2:			return	IDC_CUSTOM_TM;
	case 3:			return	IDC_CUSTOM_TM;
	case 4:			return	IDC_CUSTOM_TM;
	case 5:			return	IDC_CUSTOM_TM2; //Turn Conveyor Turn 위치
	case 6:			return	IDC_CUSTOM_TM2; 
	case 7:			return	IDC_CUSTOM_TM2; //Turn
	case 8:			return	IDC_CUSTOM_TM2; //reverse
	case 9:			return	IDC_CUSTOM_TM2;
	case 10:		return	IDC_CUSTOM_TM2;
	case 11:		return	IDC_CUSTOM_TM3;
	case 12:		return	IDC_CUSTOM_TM3;
	case 13:		return	IDC_CUSTOM_TM3;//out_Conveyor_Postion
	}
	return IDC_CUSTOM_TM;
}
void CScreenMain::OninitTrayMove()
{
	for( int i=0; i<13; i++ )
	{
		RECT rt;
		rt.left = 0;
		rt.top = 0;
		rt.right = 0;
		rt.bottom = 0;
		GetDlgItem( GetCustomTrayMove(i) )->MoveWindow( &rt );	
	}
}
void CScreenMain::SetIO_OnOff(int nID,int Onoff) 
{
	switch(nID)
	{
	case CONVEYOR_IN:
		if(Onoff == IO_ON)
		{
			m_Check_CV_In.SetIcon(IDI_ICON_LED_ON,IDI_GREEN_LED_ICON);
		}
		else
		{
			m_Check_CV_In.SetIcon(IDI_GREEN_LED_ICON,IDI_ICON_LED_ON);
		}
		break;

	case IN_CONVEYOR_POSITION:
		if(Onoff == IO_ON)
		{
			m_Check_In_pos.SetIcon(IDI_ICON_LED_ON,IDI_GREEN_LED_ICON);
		}
		else
		{
			m_Check_In_pos.SetIcon(IDI_GREEN_LED_ICON,IDI_ICON_LED_ON);
		}
		break;

	case IN_CONVEYOR_STOPPER_UP:
		if(Onoff == IO_UP)
		{
			m_Check_InUp.SetIcon(IDI_GRAY_LED_ICON,IDI_ICON_UP);
			m_Check_InDw.SetIcon(IDI_ICON_UP,IDI_GRAY_LED_ICON);
		}
		break;
	case IN_CONVEYOR_STOPPER_DOWN:
		if(Onoff == IO_DOWN)
		{
		
			m_Check_InUp.SetIcon(IDI_ICON_UP,IDI_GRAY_LED_ICON);
			m_Check_InDw.SetIcon(IDI_GRAY_LED_ICON,IDI_ICON_UP);
		}
		break;
	case OUT_IN_POSITION:

		if(Onoff == IO_ON)
		{
			m_Check_Out_InPos.SetIcon(IDI_ICON_LED_ON,IDI_GREEN_LED_ICON);
			
		}
		else
		{
			m_Check_Out_InPos.SetIcon(IDI_GREEN_LED_ICON,IDI_ICON_LED_ON);
		}
		break;
	case TURN_CONVEYOR_POSITION:
		if(Onoff == IO_ON)
		{
			m_Check_Turn_Pos.SetIcon(IDI_ICON_LED_ON,IDI_GREEN_LED_ICON);
		}
		else
		{
			m_Check_Turn_Pos.SetIcon(IDI_GREEN_LED_ICON,IDI_ICON_LED_ON);
		}
		break;
	case TURN_STOPPER_UP:
		if(Onoff == IO_UP)
		{
			m_Check_TurnUp.SetIcon(IDI_GRAY_LED_ICON,IDI_ICON_UP);
			m_Check_TurnDw.SetIcon(IDI_ICON_UP,IDI_GRAY_LED_ICON);
		}
		break;
	case TURN_STOPPER_DOWN:
		if(Onoff == IO_DOWN)
		{
			m_Check_TurnUp.SetIcon(IDI_ICON_UP,IDI_GRAY_LED_ICON);
			m_Check_TurnDw.SetIcon(IDI_GRAY_LED_ICON,IDI_ICON_UP);
		}
		break;
	case OUT_CONVEYOR:
		if(Onoff == IO_ON)
		{
			m_Check_Out_Pos.SetIcon(IDI_ICON_LED_ON,IDI_GREEN_LED_ICON);
		}
		else
		{
			m_Check_Out_Pos.SetIcon(IDI_GREEN_LED_ICON,IDI_ICON_LED_ON);
		}
		break;
	case OUT_CONVEYOR_STOPPER_UP:
		if(Onoff == IO_UP)
		{
			m_Check_Out_Up.SetIcon(IDI_GRAY_LED_ICON,IDI_ICON_UP);
			m_Check_Out_Dw.SetIcon(IDI_ICON_UP,IDI_GRAY_LED_ICON);
		}

		break;
	case OUT_CONVEYOR_STOPPER_DOWN:
		if(Onoff == IO_DOWN)
		{
			m_Check_Out_Up.SetIcon(IDI_ICON_UP,IDI_GRAY_LED_ICON);
			m_Check_Out_Dw.SetIcon(IDI_GRAY_LED_ICON,IDI_ICON_UP);
		}
		break;
	}
	
}
LRESULT CScreenMain::OnMainIODisplay(WPARAM wParam, LPARAM lParam)
{
	int nID = (int)wParam;
	int nOnOff = (int)lParam;


	SetIO_OnOff(nID,nOnOff);
	return 0;
}

LRESULT CScreenMain::OnMainCvInMoveDisplay(WPARAM wParam, LPARAM lParam) 
{

	int nPos = (int)wParam;

	int i,j,nTemp,nRow,nCol;
	CString strTmp;

	RECT rt;

	nRow = st_basic_info.nRowCnt;
	nCol = st_basic_info.nColCnt;

	m_GridTm.SetFrameFocusCell(FALSE);
	m_GridTm.SetTrackFocusCell(FALSE);
	m_GridTm.EnableSelection(FALSE);
	m_GridTm.SetGridLineColor(RGB(25, 1, 1));
	m_GridTm.SetGridLines(1);
	m_GridTm.SetRowCount(nRow);
	m_GridTm.SetColumnCount(nCol);
	m_GridTm.SetFixedRowCount(0);
	m_GridTm.SetFixedColumnCount(0);
	m_GridTm.SetFixedBkColor(RGB(0,0,200));
	m_GridTm.SetFixedBkColor(RGB(200,200,255));
	m_GridTm.SetTextBkColor(RGB(150,150,200));

	//m_GridPcbType.nLeftSize = 275;
	
	st_Pcb_info.nPcbType = st_basic_info.nPcbType;
	mn_Size = 18;
	
	for (i=0; i<nRow; i++) 
	{
		nTemp = nCol;
		m_GridTm.SetRowHeight(i, 74/nRow);
		
		for (j=0; j<nCol; j++) 
		{	
			if (i == 0)
			{
				mn_Size = mn_Size +18;
			}

			strTmp.Format(_T("%d"), nTemp--);
			m_GridTm.SetColumnWidth(j, 19);
			m_GridTm.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_GridTm.SetItemState(i, j, GVIS_READONLY);
		
			m_GridTm.SetItemBkColour(i, j, RGB(255,255,128), CLR_DEFAULT);
			
			m_GridTm.SetItemText(i, j, strTmp);
		}
	}	
	st_Pcb_info.nLeftSize = mn_Size;
	//rt.left =310;	rt.top = 148;	rt.right = rt.left + mn_Size;	rt.bottom = rt.top + 80;
	//GetDlgItem(IDC_CUSTOM_MODE )->MoveWindow( &rt );
	rt.left =0;	rt.top = 0;	rt.right = rt.left +0;	rt.bottom = rt.top + 0;
	GetDlgItem(IDC_CUSTOM_TM3 )->MoveWindow( &rt );
	
	SetTrayPos(nPos);
	

	return 0;
	
}

LRESULT CScreenMain::OnMainCvTurnMoveDisplay(WPARAM wParam, LPARAM lParam) 
{
	
		
	int nPos = (int)wParam;

	int i,j,nTemp,nRow,nCol;
	CString strTmp;

	RECT rt;

	nRow = st_basic_info.nRowCnt;
	nCol = st_basic_info.nColCnt;

	m_GridTm_2.SetFrameFocusCell(FALSE);
	m_GridTm_2.SetTrackFocusCell(FALSE);
	m_GridTm_2.EnableSelection(FALSE);
	m_GridTm_2.SetGridLineColor(RGB(25, 1, 1));
	m_GridTm_2.SetGridLines(1);
	m_GridTm_2.SetRowCount(nRow);
	m_GridTm_2.SetColumnCount(nCol);
	m_GridTm_2.SetFixedRowCount(0);
	m_GridTm_2.SetFixedColumnCount(0);
	m_GridTm_2.SetFixedBkColor(RGB(0,0,200));
	m_GridTm_2.SetFixedBkColor(RGB(200,200,255));
	m_GridTm_2.SetTextBkColor(RGB(150,150,200));

	//m_GridPcbType.nLeftSize = 275;

	st_Pcb_info.nPcbType = st_basic_info.nPcbType;
	mn_Size = 18;

	for (i=0; i<nRow; i++) 
	{
		nTemp = nCol;
		m_GridTm_2.SetRowHeight(i, 74/nRow);

		for (j=0; j<nCol; j++) 
		{	
			if (i == 0)
			{
				mn_Size = mn_Size +18;
			}

			strTmp.Format(_T("%d"), nTemp--);
			m_GridTm_2.SetColumnWidth(j, 19);
			m_GridTm_2.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_GridTm_2.SetItemState(i, j, GVIS_READONLY);

			m_GridTm_2.SetItemBkColour(i, j, RGB(255,255,128), CLR_DEFAULT);

			m_GridTm_2.SetItemText(i, j, strTmp);
		}
	}	
	st_Pcb_info.nLeftSize = mn_Size;
	rt.left =0;	rt.top = 0;	rt.right = rt.left +0;	rt.bottom = rt.top + 0;
	GetDlgItem(IDC_CUSTOM_TM )->MoveWindow( &rt );

	SetTrayPos(nPos);
	
	return 0;

}

LRESULT CScreenMain::OnMainCvOutMoveDisplay(WPARAM wParam, LPARAM lParam) 
{

	int nPos = (int)wParam;

	int i,j,nTemp,nRow,nCol;
	CString strTmp;

	RECT rt;

	nRow = st_basic_info.nRowCnt;
	nCol = st_basic_info.nColCnt;

	m_GridTm_3.SetFrameFocusCell(FALSE);
	m_GridTm_3.SetTrackFocusCell(FALSE);
	m_GridTm_3.EnableSelection(FALSE);
	m_GridTm_3.SetGridLineColor(RGB(25, 1, 1));
	m_GridTm_3.SetGridLines(1);
	m_GridTm_3.SetRowCount(nRow);
	m_GridTm_3.SetColumnCount(nCol);
	m_GridTm_3.SetFixedRowCount(0);
	m_GridTm_3.SetFixedColumnCount(0);
	m_GridTm_3.SetFixedBkColor(RGB(0,0,200));
	m_GridTm_3.SetFixedBkColor(RGB(200,200,255));
	m_GridTm_3.SetTextBkColor(RGB(150,150,200));

	st_Pcb_info.nPcbType = st_basic_info.nPcbType;
	mn_Size = 18;

	if (st_basic_info.nPcbTurnEnable == TRUE)
	{
		for (i=0; i<nRow; i++) 
		{
			nTemp = nCol;
			m_GridTm_3.SetRowHeight(i, 74/nRow);

			for (j=0; j<nCol; j++) 
			{	
				if (i == 0)
				{
					mn_Size = mn_Size +18;
				}

				strTmp.Format(_T("%d"), nTemp--);
				m_GridTm_3.SetColumnWidth(j, 19);
				m_GridTm_3.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
				m_GridTm_3.SetItemState(i, j, GVIS_READONLY);

				m_GridTm_3.SetItemBkColour(i, j, RGB(128,255,255), CLR_DEFAULT);

				m_GridTm_3.SetItemText(i, j, strTmp);
			}
		}
	}
	else
	{
		for (i=0; i<nRow; i++) 
		{
			nTemp = nCol;
			m_GridTm_3.SetRowHeight(i, 74/nRow);

			for (j=0; j<nCol; j++) 
			{	
				if (i == 0)
				{
					mn_Size = mn_Size +18;
				}

				strTmp.Format(_T("%d"), j+1);
				m_GridTm_3.SetColumnWidth(j, 19);
				m_GridTm_3.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
				m_GridTm_3.SetItemState(i, j, GVIS_READONLY);

				m_GridTm_3.SetItemBkColour(i, j, RGB(128,255,255), CLR_DEFAULT);

				m_GridTm_3.SetItemText(i, j, strTmp);
			}
		}	
	}
	st_Pcb_info.nLeftSize = mn_Size;
	//rt.left =310;	rt.top = 148;	rt.right = rt.left + mn_Size;	rt.bottom = rt.top + 80;
	//GetDlgItem(IDC_CUSTOM_MODE )->MoveWindow( &rt );
	rt.left =0;	rt.top = 0;	rt.right = rt.left +0;	rt.bottom = rt.top + 0;
	GetDlgItem(IDC_CUSTOM_TM2 )->MoveWindow( &rt );
	
	SetTrayPos(nPos);
	

	return 0;

}

void CScreenMain::SetTrayPos(int iIdx)
{
	
	RECT rt;
	switch( iIdx )
	{
// 	case 0:				rt.left = 0;	rt.top = 0;		rt.right = 0;									rt.bottom = 0;				break;
// 	case 1:				rt.left = 10;	rt.top = 610;	rt.right = rt.left + st_basic_info.nLeftSize;	rt.bottom = rt.top + 80;	break;//inConveyor_Position
// 	case 2:				rt.left = 50;	rt.top = 610;	rt.right = rt.left + st_basic_info.nLeftSize;	rt.bottom = rt.top + 80;	break; 
// 	case 3:				rt.left = 150;	rt.top = 610;	rt.right = rt.left + st_basic_info.nLeftSize;	rt.bottom = rt.top + 80;	break;
// 	case 4:				rt.left = 250;	rt.top = 610;	rt.right = rt.left + st_basic_info.nLeftSize;	rt.bottom = rt.top + 80;	break;
// 	case 5:				rt.left = 350;	rt.top = 610;	rt.right = rt.left + st_basic_info.nLeftSize;	rt.bottom = rt.top + 80;	break; //Turn Conveyor Turn 위치
// 	
// 	case 6:				OnMainPcbWork(); 
// 						rt.left = 350;	rt.top = 610;	rt.right = rt.left + st_basic_info.nLeftSize;	rt.bottom = rt.top + 80;	break; //Turn Conveyor Turn 위치
// 	
// 	case 7:				if (st_basic_info.nPcbTurnEnable == TRUE)
// 						{
// 							OnMainPcbWork(); 
// 							m_nTemp= 10;
// 							rt.left = 350;	rt.top = 610;	rt.right = rt.left + st_basic_info.nLeftSize;	rt.bottom = rt.top + 80;	break;
// 							
// 							break;
// 						}
// 						TurnPositionGrid();
// 						rt.left = 410;	rt.top = 570;	rt.right = rt.left + 85;	rt.bottom = rt.top + st_basic_info.nLeftSize;	break; //Turn
// 
// 	case 8:	
// 		
// 						ReversePositionGrid();
// 						rt.left = 350;	rt.top = 610;	rt.right = rt.left + st_basic_info.nLeftSize;	rt.bottom = rt.top + 80;	break; //reverse
// 
// 	case 9:
// 		
// 						OnMainPcbWork();
// 						rt.left = 350;	rt.top = 610;	rt.right = rt.left + st_basic_info.nLeftSize;	rt.bottom = rt.top + 80;	break; //reverse
// 
// 	case 10:			rt.left = 450;	rt.top = 610;	rt.right = rt.left + st_basic_info.nLeftSize;	rt.bottom = rt.top + 80;	break;
// 	case 11:            rt.left = 550;	rt.top = 610;	rt.right = rt.left + st_basic_info.nLeftSize;	rt.bottom = rt.top + 80;	break;
// 	case 12:			rt.left = 650;	rt.top = 610;	rt.right = rt.left + st_basic_info.nLeftSize;	rt.bottom = rt.top + 80;    break;	
// 	case 13:			rt.left = 770;	rt.top = 610;	rt.right = rt.left + st_basic_info.nLeftSize;	rt.bottom = rt.top + 80;	break;//out_Conveyor_Postion

	//kwlee 2017.0220
	case 0:				rt.left = 0;	rt.top = 0;		rt.right = 0;									rt.bottom = 0;				break; //Ready
	case 1:				rt.left = 350;	rt.top = 460;	rt.right = rt.left + st_basic_info.nLeftSize;	rt.bottom = rt.top + 80;	break;//inConveyor_Position
	case 2:				rt.left = 450;	rt.top = 460;	rt.right = rt.left + st_basic_info.nLeftSize;	rt.bottom = rt.top + 80;	break;//Turn Conv 진입
	case 3:				rt.left = 550;	rt.top = 460;	rt.right = rt.left + st_basic_info.nLeftSize;	rt.bottom = rt.top + 80;	break;////Turn Conveyor Turn 위치
	case 4:				OnMainPcbWork(); 
		 				rt.left = 550;	rt.top = 460;	rt.right = rt.left + st_basic_info.nLeftSize;	rt.bottom = rt.top + 80;	break;
	case 5:				if (st_basic_info.nPcbTurnEnable == TRUE)
						{
							OnMainPcbWork(); 
							m_nTemp= 10;
							rt.left = 550;	rt.top = 460;	rt.right = rt.left + st_basic_info.nLeftSize;	rt.bottom = rt.top + 80;	break;

							break;
						}
						TurnPositionGrid();
						rt.left = 610;	rt.top = 380;	rt.right = rt.left + 85;	rt.bottom = rt.top + st_basic_info.nLeftSize;	break; //Turn
	
	case 6:	
						ReversePositionGrid();
						rt.left = 550;	rt.top = 460;	rt.right = rt.left + st_basic_info.nLeftSize;	rt.bottom = rt.top + 80;	break; //reverse
	
	case 7:
						OnMainPcbWork();
						rt.left = 550;	rt.top = 460;	rt.right = rt.left + st_basic_info.nLeftSize;	rt.bottom = rt.top + 80;	break; //reverse

	case 8:				rt.left = 650;	rt.top = 460;	rt.right = rt.left + st_basic_info.nLeftSize;	rt.bottom = rt.top + 80;	break;
	case 9:				rt.left = 750;	rt.top = 460;	rt.right = rt.left + st_basic_info.nLeftSize;	rt.bottom = rt.top + 80;	break; //Out Conveyor Turn 위치
	case 10:			rt.left = 900;	rt.top = 460;	rt.right = rt.left + st_basic_info.nLeftSize;	rt.bottom = rt.top + 80;	break; //Out Conveyor Turn 위치

// 	case 6:				OnMainPcbWork(); 
// 		rt.left = 350;	rt.top = 610;	rt.right = rt.left + st_basic_info.nLeftSize;	rt.bottom = rt.top + 80;	break; //Turn Conveyor Turn 위치
// 
// 	case 7:				if (st_basic_info.nPcbTurnEnable == TRUE)
// 						{
// 							OnMainPcbWork(); 
// 							m_nTemp= 10;
// 							rt.left = 350;	rt.top = 610;	rt.right = rt.left + st_basic_info.nLeftSize;	rt.bottom = rt.top + 80;	break;
// 
// 							break;
// 						}
// 						TurnPositionGrid();
// 						rt.left = 410;	rt.top = 570;	rt.right = rt.left + 85;	rt.bottom = rt.top + st_basic_info.nLeftSize;	break; //Turn
// 
// 	case 8:	
// 
// 		ReversePositionGrid();
// 		rt.left = 350;	rt.top = 610;	rt.right = rt.left + st_basic_info.nLeftSize;	rt.bottom = rt.top + 80;	break; //reverse
// 
// 	case 9:
// 
// 		OnMainPcbWork();
// 		rt.left = 350;	rt.top = 610;	rt.right = rt.left + st_basic_info.nLeftSize;	rt.bottom = rt.top + 80;	break; //reverse
// 
// 	case 10:			rt.left = 450;	rt.top = 610;	rt.right = rt.left + st_basic_info.nLeftSize;	rt.bottom = rt.top + 80;	break;
// 	case 11:            rt.left = 550;	rt.top = 610;	rt.right = rt.left + st_basic_info.nLeftSize;	rt.bottom = rt.top + 80;	break;
// 	case 12:			rt.left = 650;	rt.top = 610;	rt.right = rt.left + st_basic_info.nLeftSize;	rt.bottom = rt.top + 80;    break;	
// 	case 13:			rt.left = 770;	rt.top = 610;	rt.right = rt.left + st_basic_info.nLeftSize;	rt.bottom = rt.top + 80;	break;//out_Conveyor_Postion
	}

	GetDlgItem( GetCustomTrayMove(iIdx) )->MoveWindow( &rt );	

}


LRESULT CScreenMain::OnMainWorkInfoDisplay(WPARAM wParam, LPARAM lParam) 
{
	CString sFile, sPath;

	int nPart = 0;
	int nSite = 0;

	nPart = (int)wParam;
	nSite = (int)lParam;

	switch (wParam)
	{
		case MAIN_TIMEINFO:
			OnMainTimeDisplay();
			break;

		case MAIN_COUNT_DISPLAY:
			OnMainCountDisplay();
			break;

		case MAIN_LOT_DISPLAY:

			break;

		case MAIN_ROBOT_LEFT_TURN_DISPLAY: 
			OnMainRobotLeftTurnDisplay();
			break;

		case MAIN_ROBOT_RIGHT_TURN_DISPLAY: 
			OnMainRobotRightTurnDisplay();
			break;

		case MAIN_ROBOT_CENTER_DISPLAY: 
			OnMainRobotCenterDisplay();
			break;

		case MAIN_PCB_WORK:
			OnMainPcbWork();
			break;     
	}

	return 0;
}

void CScreenMain::OnInitGridTimeInfo(int nMode)
{
	int   i, j;
	int	  max_row, max_col;
	int   row;
	CString strTmp;

	if (nMode == 0)
	{
		max_row = 4; // 
		max_col = 2;
	}
	else
	{
		max_row = 6; // 
		max_col = 2;
	}
	
	m_pGridTimeInfo.SetFrameFocusCell(FALSE);
	m_pGridTimeInfo.SetTrackFocusCell(FALSE);
	m_pGridTimeInfo.EnableSelection(FALSE);

	m_pGridTimeInfo.SetGridLineColor(BLACK_C);
	m_pGridTimeInfo.SetGridLines(1);

	m_pGridTimeInfo.SetRowCount(max_row);
	m_pGridTimeInfo.SetColumnCount(max_col);

	m_pGridTimeInfo.SetFixedRowCount(0);
	m_pGridTimeInfo.SetFixedColumnCount(0);
	m_pGridTimeInfo.SetFixedBkColor(RGB(0,0,200));
	m_pGridTimeInfo.SetFixedBkColor(RGB(200,200,255));
	m_pGridTimeInfo.SetTextBkColor(RGB(150,150,200));

	for (i=0; i<max_row; i++) 
	{
		if (nMode == 0)
		{
			m_pGridTimeInfo.SetRowHeight(i, 37);
		}
		else
		{
			m_pGridTimeInfo.SetRowHeight(i, 25);
		}

		for (j=0; j<max_col; j++) 
		{
			m_pGridTimeInfo.SetColumnWidth(j, 120);
				
			m_pGridTimeInfo.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_pGridTimeInfo.SetItemState(i, j, GVIS_READONLY);
			m_pGridTimeInfo.SetItemBkColour(i, j, WHITE_C, CLR_DEFAULT);
		}
	}

	row = 0;
	m_pGridTimeInfo.SetItemBkColour(row, 0, GREEN_L, BLACK_L);
	m_pGridTimeInfo.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridTimeInfo.SetItemText(row, 0, _T("RUN"));

	m_pGridTimeInfo.SetItemBkColour(row, 1, GREEN_L, BLACK_L);
	m_pGridTimeInfo.SetItemFont(row, 1, &clsFunc.OnLogFont(16));
	m_pGridTimeInfo.SetItemText(row, 1, _T("00 : 00 : 00"));

	row = 1;
	m_pGridTimeInfo.SetItemBkColour(row, 0, YELLOW_L, BLACK_L);
	m_pGridTimeInfo.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridTimeInfo.SetItemText(row, 0, _T("STOP"));

	m_pGridTimeInfo.SetItemBkColour(row, 1, YELLOW_L, BLACK_L);
	m_pGridTimeInfo.SetItemFont(row, 1, &clsFunc.OnLogFont(16));
	m_pGridTimeInfo.SetItemText(row, 1, _T("00 : 00 : 00"));

	row = 2;
	m_pGridTimeInfo.SetItemBkColour(row, 0, RED_L, BLACK_L);
	m_pGridTimeInfo.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridTimeInfo.SetItemText(row, 0, _T("ALARM"));

	m_pGridTimeInfo.SetItemBkColour(row, 1, RED_L, BLACK_L);
	m_pGridTimeInfo.SetItemFont(row, 1, &clsFunc.OnLogFont(16));
	m_pGridTimeInfo.SetItemText(row, 1, _T("00 : 00 : 00"));

	row = 3;
	m_pGridTimeInfo.SetItemBkColour(row, 0, BLUE_C, WHITE_C);
	m_pGridTimeInfo.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridTimeInfo.SetItemText(row, 0, _T("MTBI"));

	m_pGridTimeInfo.SetItemBkColour(row, 1, BLUE_C, WHITE_C);
	m_pGridTimeInfo.SetItemFont(row, 1, &clsFunc.OnLogFont(16));
	m_pGridTimeInfo.SetItemText(row, 1, _T("00 : 00 : 00"));

	if (nMode == 1)
	{
		row = 4;
		m_pGridTimeInfo.SetItemBkColour(row, 0, BLACK_C, GREEN_C);
		m_pGridTimeInfo.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
		m_pGridTimeInfo.SetItemText(row, 0, _T("Latest/Daily UPH"));
		//m_pGridTimeInfo.SetItemText(row, 0, _T("Daily UPH"));

		m_pGridTimeInfo.SetItemBkColour(row, 1, BLACK_C, GREEN_C);
		m_pGridTimeInfo.SetItemFont(row, 1, &clsFunc.OnLogFont(16));
		m_pGridTimeInfo.SetItemText(row, 1, _T("0"));

		row = 5;
		m_pGridTimeInfo.SetItemBkColour(row, 0, YELLOW_L, BLACK_L);
		m_pGridTimeInfo.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
		m_pGridTimeInfo.SetItemText(row, 0, _T("Cycle Time"));

		m_pGridTimeInfo.SetItemBkColour(row, 1, WHITE_C, BLACK_L);
		m_pGridTimeInfo.SetItemFont(row, 1, &clsFunc.OnLogFont(16));
		m_pGridTimeInfo.SetItemText(row, 1, _T("0"));
	}
}


void CScreenMain::OnInitGroupBox()
{
	m_groupWorkInfo.SetFont(clsFunc.OnLogFont(14));
	m_groupWorkInfo.SetCatptionTextColor(BLUE_C);
	m_groupWorkInfo.SetBorderColor(ORANGE_C);
	m_groupWorkInfo.SetFontBold(TRUE);
	m_groupWorkInfo.SetBackgroundColor(WINDOW_UP);

	m_groupTimeInfo.SetFont(clsFunc.OnLogFont(14));
	m_groupTimeInfo.SetCatptionTextColor(BLUE_C);
	m_groupTimeInfo.SetBorderColor(ORANGE_C);
	m_groupTimeInfo.SetFontBold(TRUE);
	m_groupTimeInfo.SetBackgroundColor(WINDOW_UP);
	

	m_group_InConv.SetFont(clsFunc.OnLogFont(14));
	m_group_InConv.SetCatptionTextColor(BLUE_C);
	m_group_InConv.SetBorderColor(ORANGE_C);
	m_group_InConv.SetFontBold(TRUE);
	m_group_InConv.SetBackgroundColor(WINDOW_UP);

	m_group_TurnConv.SetFont(clsFunc.OnLogFont(14));
	m_group_TurnConv.SetCatptionTextColor(BLUE_C);
	m_group_TurnConv.SetBorderColor(ORANGE_C);
	m_group_TurnConv.SetFontBold(TRUE);
	m_group_TurnConv.SetBackgroundColor(WINDOW_UP);

	m_group_OutConv.SetFont(clsFunc.OnLogFont(14));
	m_group_OutConv.SetCatptionTextColor(BLUE_C);
	m_group_OutConv.SetBorderColor(ORANGE_C);
	m_group_OutConv.SetFontBold(TRUE);
	m_group_OutConv.SetBackgroundColor(WINDOW_UP);


	m_group_Feeder.SetFont(clsFunc.OnLogFont(14));
	m_group_Feeder.SetCatptionTextColor(BLUE_C);
	m_group_Feeder.SetBorderColor(ORANGE_C);
	m_group_Feeder.SetFontBold(TRUE);
	m_group_Feeder.SetBackgroundColor(WINDOW_UP);

	m_group_Conv.SetFont(clsFunc.OnLogFont(14));
	m_group_Conv.SetCatptionTextColor(BLUE_C);
	m_group_Conv.SetBorderColor(ORANGE_C);
	m_group_Conv.SetFontBold(TRUE);
	m_group_Conv.SetBackgroundColor(WINDOW_UP);

	m_group_Robot.SetFont(clsFunc.OnLogFont(14));
	m_group_Robot.SetCatptionTextColor(BLUE_C);
	m_group_Robot.SetBorderColor(ORANGE_C);
	m_group_Robot.SetFontBold(TRUE);
	m_group_Robot.SetBackgroundColor(WINDOW_UP);

	
}

void CScreenMain::OnInitGridWorkInfoCurr()
{
	int   i, j;
	int	  max_row, max_col;
	int   row;

	CString strTmp;
	CRect rect;

	max_row = 4; // unloader tray y count
	max_col = 2;

	m_pGridWorkInfoC.SetFrameFocusCell(FALSE);
	m_pGridWorkInfoC.SetTrackFocusCell(FALSE);
	m_pGridWorkInfoC.EnableSelection(FALSE);

	m_pGridWorkInfoC.SetGridLineColor(BLACK_C);
	m_pGridWorkInfoC.SetGridLines(1);

	m_pGridWorkInfoC.SetRowCount(max_row);
	m_pGridWorkInfoC.SetColumnCount(max_col);

	m_pGridWorkInfoC.SetFixedRowCount(0);
	m_pGridWorkInfoC.SetFixedColumnCount(0);
	m_pGridWorkInfoC.SetFixedBkColor(RGB(0,0,200));
	m_pGridWorkInfoC.SetFixedBkColor(RGB(200,200,255));
	m_pGridWorkInfoC.SetTextBkColor(RGB(150,150,200));

	for (i=0; i<max_row; i++) 
	{
		m_pGridWorkInfoC.SetRowHeight(i, 23);

		for (j=0; j<max_col; j++) 
		{
			m_pGridWorkInfoC.SetColumnWidth(j, 120);
				
			m_pGridWorkInfoC.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_pGridWorkInfoC.SetItemState(i, j, GVIS_READONLY);
			m_pGridWorkInfoC.SetItemBkColour(i, j, WHITE_C, CLR_DEFAULT);
		}
	}

	row = 0;
	m_pGridWorkInfoC.MergeCells(row, 0, row, 1);
	m_pGridWorkInfoC.SetItemBkColour(row, 0, RGB(219, 229, 241), BLACK_C);
	m_pGridWorkInfoC.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridWorkInfoC.SetItemText(row, 0, _T("Current Lot Info"));

	row = 1;
	m_pGridWorkInfoC.SetItemBkColour(row, 0, BLUE_D, WHITE_C);
	m_pGridWorkInfoC.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridWorkInfoC.SetItemText(row, 0, _T("Lot No"));

	m_pGridWorkInfoC.SetItemBkColour(row, 1, WHITE_C, BLACK_C);
	m_pGridWorkInfoC.SetItemFont(row, 1, &clsFunc.OnLogFont(10));
	m_pGridWorkInfoC.SetItemText(row, 1, _T("-"));

	row = 2;
	m_pGridWorkInfoC.SetItemBkColour(row, 0, BLUE_D, WHITE_C);
	m_pGridWorkInfoC.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridWorkInfoC.SetItemText(row, 0, _T("Part No"));

	m_pGridWorkInfoC.SetItemBkColour(row, 1, WHITE_C, BLACK_C);
	m_pGridWorkInfoC.SetItemFont(row, 1, &clsFunc.OnLogFont(10));
	m_pGridWorkInfoC.SetItemText(row, 1, _T("-"));

	row = 3;
	m_pGridWorkInfoC.SetItemBkColour(row, 0, BLUE_D, WHITE_C);
	m_pGridWorkInfoC.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridWorkInfoC.SetItemText(row, 0, _T("Last Module"));

	m_pGridWorkInfoC.SetItemBkColour(row, 1, WHITE_C, BLACK_C);
	m_pGridWorkInfoC.SetItemFont(row, 1, &clsFunc.OnLogFont(10));
	m_pGridWorkInfoC.SetItemText(row, 1, _T("-"));

// 	row = 4;
// 	m_pGridWorkInfoC.SetItemBkColour(row, 0, ORANGE_C, BLACK_C);
// 	m_pGridWorkInfoC.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
// 	m_pGridWorkInfoC.SetItemText(row, 0, _T("Hour Socket"));
// 
// 	m_pGridWorkInfoC.SetItemBkColour(row, 1, WHITE_C, BLACK_C);
// 	m_pGridWorkInfoC.SetItemFont(row, 1, &clsFunc.OnLogFont(16));
// 	m_pGridWorkInfoC.SetItemText(row, 1, _T("-"));
// 
// 	row = 5;
// 	m_pGridWorkInfoC.SetItemBkColour(row, 0, ORANGE_C, BLACK_C);
// 	m_pGridWorkInfoC.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
// 	m_pGridWorkInfoC.SetItemText(row, 0, _T("Daily Socket"));
// 
// 	m_pGridWorkInfoC.SetItemBkColour(row, 1, WHITE_C, BLACK_C);
// 	m_pGridWorkInfoC.SetItemFont(row, 1, &clsFunc.OnLogFont(16));
// 	m_pGridWorkInfoC.SetItemText(row, 1, _T("-"));
}

void CScreenMain::OnInitGridWorkInfoNext()
{
	int   i, j;
	int	  max_row, max_col;
	int   row;

	CString strTmp;
	CRect rect;

	max_row = 3; // unloader tray y count
	max_col = 2;

	m_pGridWorkInfoN.SetFrameFocusCell(FALSE);
	m_pGridWorkInfoN.SetTrackFocusCell(FALSE);
	m_pGridWorkInfoN.EnableSelection(FALSE);

	m_pGridWorkInfoN.SetGridLineColor(BLACK_C);
	m_pGridWorkInfoN.SetGridLines(1);

	m_pGridWorkInfoN.SetRowCount(max_row);
	m_pGridWorkInfoN.SetColumnCount(max_col);

	m_pGridWorkInfoN.SetFixedRowCount(0);
	m_pGridWorkInfoN.SetFixedColumnCount(0);
	m_pGridWorkInfoN.SetFixedBkColor(RGB(0,0,200));
	m_pGridWorkInfoN.SetFixedBkColor(RGB(200,200,255));
	m_pGridWorkInfoN.SetTextBkColor(RGB(150,150,200));

	for (i=0; i<max_row; i++) 
	{
		m_pGridWorkInfoN.SetRowHeight(i, 23);

		for (j=0; j<max_col; j++) 
		{
			m_pGridWorkInfoN.SetColumnWidth(j, 120);
				
			m_pGridWorkInfoN.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_pGridWorkInfoN.SetItemState(i, j, GVIS_READONLY);
			m_pGridWorkInfoN.SetItemBkColour(i, j, WHITE_C, CLR_DEFAULT);
		}
	}

	row = 0;
	m_pGridWorkInfoN.MergeCells(row, 0, row, 1);
	m_pGridWorkInfoN.SetItemBkColour(row, 0, RGB(219, 229, 241), BLACK_C);
	m_pGridWorkInfoN.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridWorkInfoN.SetItemText(row, 0, _T("Next Lot Info"));

	row = 1;
	m_pGridWorkInfoN.SetItemBkColour(row, 0, BLUE_D, WHITE_C);
	m_pGridWorkInfoN.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridWorkInfoN.SetItemText(row, 0, _T("Lot No"));

	m_pGridWorkInfoN.SetItemBkColour(row, 1, WHITE_C, BLACK_C);
	m_pGridWorkInfoN.SetItemFont(row, 1, &clsFunc.OnLogFont(10));
	m_pGridWorkInfoN.SetItemText(row, 1, _T("-"));

	row = 2;
	m_pGridWorkInfoN.SetItemBkColour(row, 0, BLUE_D, WHITE_C);
	m_pGridWorkInfoN.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridWorkInfoN.SetItemText(row, 0, _T("Part No"));

	m_pGridWorkInfoN.SetItemBkColour(row, 1, WHITE_C, BLACK_C);
	m_pGridWorkInfoN.SetItemFont(row, 1, &clsFunc.OnLogFont(10));
	m_pGridWorkInfoN.SetItemText(row, 1, _T("-"));
}


void CScreenMain::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case TM_FRONT_SMEMA:
		OnMainDisplayFrontSmema();
		break;

	case TM_REAR_SMEMA:
		OnMainDisplayRearSmema();
		break;

	
	case TM_DATA_DISPLAY:
		OnMainBufferDisplay();
		OnMainPickerDisplay();
		OnMainPcbDataDisplay();
		break;
	}

	CFormView::OnTimer(nIDEvent);
}

void CScreenMain::OnMainTimeDisplay()
{
//	int nState;
	static int nOldTime = 0;
	int nMtbi = 0;
	double dTemp;
	CString strTime;

	strTime.Format(_T("%02d:%02d:%02d"), st_handler_info.tStop.GetHours(), st_handler_info.tStop.GetMinutes(), st_handler_info.tStop.GetSeconds());
	m_pGridTimeInfo.SetItemText(1, 1, strTime);
	
	strTime.Format(_T("%02d:%02d:%02d"), st_handler_info.tRun.GetHours(), st_handler_info.tRun.GetMinutes(), st_handler_info.tRun.GetSeconds());
	m_pGridTimeInfo.SetItemText(0, 1, strTime);

	nMtbi = (int)st_handler_info.tRun.GetTotalHours() * 3600 + (int)st_handler_info.tRun.GetMinutes() * 60 + (int)st_handler_info.tRun.GetSeconds();
	if (st_alarm_info.nAlarmNum > 0)
	{
		nMtbi = nMtbi / st_alarm_info.nAlarmNum;
	}

	strTime.Format(_T("%02d:%02d:%02d"), nMtbi / 3600, (nMtbi % 3600) / 60, nMtbi % 60 );
	m_pGridTimeInfo.SetItemText(3, 1, strTime);
	
	strTime.Format(_T("%02d:%02d:%02d"), st_handler_info.tJam.GetHours(), st_handler_info.tJam.GetMinutes(), st_handler_info.tJam.GetSeconds());
	m_pGridTimeInfo.SetItemText(2, 1, strTime);

	if (st_work_info.nPgmInfo == 1)
	{
		if (st_count_info.nPassCount[1][0] <= 0) return;
		if (((int)st_handler_info.tRun.GetTotalHours() * 60 + (int)st_handler_info.tRun.GetMinutes()) <= 0) return;

		if (nOldTime != (int)(st_handler_info.tRef.GetHour()))
		{
			nOldTime = (int)(st_handler_info.tRef.GetHour() - 1);

			if (nOldTime == -1) nOldTime = 23;

			m_pGridTimeInfo.Invalidate(FALSE);
		}

		//dTemp = (double)((int)st_handler_info.tRun.GetTotalHours() * 60 + (int)st_handler_info.tRun.GetMinutes()) / 60.0f;
		//strTime.Format(_T("%d / %0.2f"), st_count_info.nLatestUph[nOldTime] ,double(st_count_info.nPassCount[1][0] / dTemp));
		//strTime.Format(_T("%0.2f"), double(st_count_info.nPassCount[1][0] / dTemp));
		dTemp = 0.0f;
		if (st_count_info.nDailyUph > 0 && st_count_info.nUphCnt > 0)
		{
			dTemp = (double)st_count_info.nDailyUph / (double)st_count_info.nUphCnt;
		}
		strTime.Format(_T("%d / %0.2f"), st_count_info.nUph , dTemp);
		m_pGridTimeInfo.SetItemText(4, 1, strTime);

		if (st_work_info.nCycleCount > 0)
		{
			dTemp = (double)st_work_info.dwLotCycle / (double)st_work_info.nCycleCount;

			strTime.Format(_T("%d / %0.2f"), st_work_info.dwTrayCycle , dTemp);
			m_pGridTimeInfo.SetItemText(5, 1, strTime);
		}
		else
		{
			strTime.Format(_T("%d / %0.2f"), st_work_info.dwTrayCycle , 0.0);
			m_pGridTimeInfo.SetItemText(5, 1, strTime);
		}
	}



	m_pGridTimeInfo.Invalidate(FALSE);
}

void CScreenMain::OnMainCountDisplay()
{
	CString strTemp;

	double dAve;

	// lot 수량.
	strTemp.Format(_T("%d"), st_count_info.nInCount[0][0]);
	m_pGridYieldL.SetItemText(2, 1, strTemp);

	strTemp.Format(_T("%d"), st_count_info.nPrimeCount[0][0]);
	m_pGridYieldL.SetItemText(3, 1, strTemp);

	strTemp.Format(_T("%d"), st_count_info.nPassCount[0][0]);
	m_pGridYieldL.SetItemText(3, 2, strTemp);

	// jtkim 20150307
// 	strTemp.Format(_T("%d"), st_count_info.nPrimeRejectCount[0][0]);
// 	m_pGridYieldL.SetItemText(4, 1, strTemp);
// 
// 	strTemp.Format(_T("%d"), st_count_info.nRejectCount[0][0]);
// 	m_pGridYieldL.SetItemText(4, 2, strTemp);

 	if (st_count_info.nPrimeCount[0][0] > 0) 
 	{
 		dAve = ((double)st_count_info.nPrimeCount[0][0] / (double)st_count_info.nInCount[0][0]) * (double)100;
// 		dAve = ((double)st_count_info.nPrimeCount[0][0] / ((double)st_count_info.nPrimeCount[0][0] + (double)st_count_info.nPrimeRejectCount[0][0])) * (double)100;
 		strTemp.Format(_T("%.2f"), dAve);
		m_pGridYieldL.SetItemText(5, 1, strTemp);
 	}
 
 	if (st_count_info.nPassCount[0][0] > 0) 
 	{
		dAve = ((double)st_count_info.nPassCount[0][0] / (double)st_count_info.nInCount[0][0]) * (double)100;
// 		dAve = ((double)st_count_info.nPassCount[0][0] / ((double)st_count_info.nPassCount[0][0] + (double)st_count_info.nRejectCount[0][0])) * (double)100;
		strTemp.Format(_T("%.2f"), dAve);
		m_pGridYieldL.SetItemText(5, 2, strTemp);
	}

	// daily 수량.
	strTemp.Format(_T("%d"), st_count_info.nInCount[1][0]);
	m_pGridYieldD.SetItemText(2, 1, strTemp);

	strTemp.Format(_T("%d"), st_count_info.nPrimeCount[1][0]);
	m_pGridYieldD.SetItemText(3, 1, strTemp);

	strTemp.Format(_T("%d"), st_count_info.nPassCount[1][0]);
	m_pGridYieldD.SetItemText(3, 2, strTemp);

	// jtkim 20150307
// 	strTemp.Format(_T("%d"), st_count_info.nPrimeRejectCount[1][0]);
// 	m_pGridYieldD.SetItemText(4, 1, strTemp);
// 
// 	strTemp.Format(_T("%d"), st_count_info.nRejectCount[1][0]);
// 	m_pGridYieldD.SetItemText(4, 2, strTemp);

	if (st_count_info.nPrimeCount[1][0] > 0) 
	{
 		dAve = ((double)st_count_info.nPrimeCount[1][0] / (double)st_count_info.nInCount[1][0]) * (double)100;
// 		dAve = ((double)st_count_info.nPrimeCount[1][0] / ((double)st_count_info.nPrimeCount[1][0] + (double)st_count_info.nPrimeRejectCount[1][0])) * (double)100;
		strTemp.Format(_T("%.2f"), dAve);
		m_pGridYieldD.SetItemText(5, 1, strTemp);
	}
 
	if (st_count_info.nPassCount[1][0] > 0) 
	{
		dAve = ((double)st_count_info.nPassCount[1][0] / (double)st_count_info.nInCount[1][0]) * (double)100;
// 		dAve = ((double)st_count_info.nPassCount[1][0] / ((double)st_count_info.nPassCount[1][0] + (double)st_count_info.nRejectCount[1][0])) * (double)100;
		strTemp.Format(_T("%.2f"), dAve);
		m_pGridYieldD.SetItemText(5, 2, strTemp);
	}

	m_pGridYieldL.Invalidate(FALSE);
	m_pGridYieldD.Invalidate(FALSE);
}


void CScreenMain::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFormView::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CScreenMain::OnTimeCellClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	int nRow, nCol;

	NM_GRIDVIEW* pItem	= (NM_GRIDVIEW*) pNotifyStruct;

	nRow				= pItem->iRow;
	nCol				= pItem->iColumn;

	if (st_work_info.nPgmInfo == 0)
	{
		st_work_info.nPgmInfo = 1;
	}
	else
	{
		st_work_info.nPgmInfo = 0;
	}
	OnInitGridTimeInfo(st_work_info.nPgmInfo);
}

void CScreenMain::OnWorkInfoCurrCellClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	if (st_handler_info.nRunStatus != dSTOP) return;

}
/*
void CScreenMain::OnWorkCellClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	int nRow, nCol;


	NM_GRIDVIEW* pItem	= (NM_GRIDVIEW*) pNotifyStruct;

	nRow				= pItem->iRow;
	nCol				= pItem->iColumn;

	if(nRow < 2) return ; 
	if (st_handler_info.nRunStatus != dSTOP) return;

	switch (nRow)
	{
		case 5:
			break;

		default:
			break;
	}
//	m_pGridWorkInfo.Refresh();
}*/


void CScreenMain::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CFormView::PostNcDestroy();
}


void CScreenMain::OnDestroy()
{
	CFormView::OnDestroy();
/*
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_pImagelist != NULL)
	{
		delete m_pImagelist;
		m_pImagelist = NULL;
	}

	if (m_pPcbInfo != NULL)
	{
		delete m_pPcbInfo;
		m_pPcbInfo = NULL;
	}

	if (m_pPcbStatus != NULL)
	{
		delete m_pPcbStatus;
		m_pPcbStatus = NULL;
	}

	if (m_pLotDisplay != NULL)
	{
		m_pLotDisplay->DestroyWindow();

		delete m_pLotDisplay;
		m_pLotDisplay = NULL;
	}

	st_handler_info.cWndMain = NULL;*/
}



void CScreenMain::OnClickedBtnMainAnimate()
{
	
	if (st_handler_info.nRunStatus != dSTOP) return;

	CDialog_Message dlgMsg;
//	int nResponse;

	dlgMsg.m_nMessageType	= 1;

// 	if (FAS_IO.get_in_bit(st_io_info.i_CokChangeCokClampFwdChk, IO_OFF)	== IO_ON ||
// 		FAS_IO.get_in_bit(st_io_info.i_CokChangeCokClampBwdChk, IO_ON)	== IO_OFF)
// 	{
// 		dlgMsg.m_strMessage		= _T("Do you want a COK Buffer UnClamp?");
// 
// 		nResponse = (int)dlgMsg.DoModal();
// 
// 		if (nResponse == IDOK)
// 		{
// 			FAS_IO.set_out_bit(st_io_info.o_CokChangeCokClampFwdCyl, IO_OFF);
// 			FAS_IO.set_out_bit(st_io_info.o_CokChangeCokClampBwdCyl, IO_ON);
// 		}
// 	}
// 	else
// 	{
// 		dlgMsg.m_strMessage		= _T("Do you want a COK Buffer Clamp?");
// 
// 		nResponse = (int)dlgMsg.DoModal();
// 
// 		if (nResponse == IDOK)
// 		{
// 			FAS_IO.set_out_bit(st_io_info.o_CokChangeCokClampFwdCyl, IO_OFF);
// 			FAS_IO.set_out_bit(st_io_info.o_CokChangeCokClampBwdCyl, IO_ON);
// 		}
// 	}
}


void CScreenMain::OnClickedBtnMainData()
{
	
	
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
 	if (st_handler_info.nRunStatus != dSTOP) return;
 
 	CDialog_Message dlgMsg;

// 	if (FAS_IO.get_in_bit(st_io_info.i_FrontSelectSwChk, IO_ON) == IO_OFF ||
// 		FAS_IO.get_in_bit(st_io_info.i_RearSelectSwChk1, IO_ON)	== IO_OFF ||
// 		FAS_IO.get_in_bit(st_io_info.i_RearSelectSwChk2, IO_ON)	== IO_OFF)
// 	{
// 		FAS_IO.set_out_bit(st_io_info.o_DoorLock, IO_OFF);
// 	}
// 	else
// 	{
// 		dlgMsg.m_nMessageType	= 0;
// 		dlgMsg.m_strMessage		= _T("Please Check Manual Key Status.");
// 
// 		dlgMsg.DoModal();

/*		if (FAS_IO.get_in_bit(st_io_info.i_ManualModeChk, IO_OFF) == IO_ON)
		{
			FAS_IO.set_out_bit(st_io_info.o_DoorLock, IO_OFF);
		}
		else
		{
			dlgMsg.m_nMessageType	= 0;
			dlgMsg.m_strMessage		= _T("Please Check Manual Key Status.");

			dlgMsg.DoModal();
		}*/
	//}
}


void CScreenMain::OnBnClickedButtonTrackOut()
{
	if (st_handler_info.nLotStatus != dSTOP) return;

	st_track_info.nStatus = YES;
	if (st_handler_info.cWndMain != NULL)
	{
		st_handler_info.cWndMain->SendMessage(WM_WORK_COMMAND, MAIN_TRACK_OUT_CREATE_REQ, 0);
	}
}

// LRESULT CScreenMain::OnMainRobotTurnDisplay(WPARAM wParam, LPARAM lParam)
// {
// 
// }
void CScreenMain::OnInitGridYieldLot()
{
	int   i, j;
	int	  max_row, max_col;
	int   row;

	CString strTmp;
	CRect rect;

	max_row = 6; // unloader tray y count
	max_col = 3;

	m_pGridYieldL.SetFrameFocusCell(FALSE);
	m_pGridYieldL.SetTrackFocusCell(FALSE);
	m_pGridYieldL.EnableSelection(FALSE);

	m_pGridYieldL.SetGridLineColor(BLACK_C);
	m_pGridYieldL.SetGridLines(1);

	m_pGridYieldL.SetRowCount(max_row);
	m_pGridYieldL.SetColumnCount(max_col);

	m_pGridYieldL.SetFixedRowCount(0);
	m_pGridYieldL.SetFixedColumnCount(0);
	m_pGridYieldL.SetFixedBkColor(RGB(0,0,200));
	m_pGridYieldL.SetFixedBkColor(RGB(200,200,255));
	m_pGridYieldL.SetTextBkColor(RGB(150,150,200));

	for (i=0; i<max_row; i++) 
	{
		m_pGridYieldL.SetRowHeight(i, 23);

		for (j=0; j<max_col; j++) 
		{
			m_pGridYieldL.SetColumnWidth(j, 80);

			m_pGridYieldL.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_pGridYieldL.SetItemState(i, j, GVIS_READONLY);
			m_pGridYieldL.SetItemBkColour(i, j, WHITE_C, CLR_DEFAULT);
		}
	}

	row = 0;
	m_pGridYieldL.MergeCells(row, 0, row, 2);
	m_pGridYieldL.SetItemBkColour(row, 0, RGB(219, 229, 241), BLACK_C);
	m_pGridYieldL.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridYieldL.SetItemText(row, 0, _T("Lot Yield Info"));

	row = 1;
	m_pGridYieldL.SetItemBkColour(row, 0, BLUE_D, WHITE_C);
	m_pGridYieldL.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridYieldL.SetItemText(row, 0, _T(""));

	m_pGridYieldL.SetItemBkColour(row, 1, BLUE_D, WHITE_C);
	m_pGridYieldL.SetItemFont(row, 1, &clsFunc.OnLogFont(16));
	m_pGridYieldL.SetItemText(row, 1, _T("Prime"));

	m_pGridYieldL.SetItemBkColour(row, 2, BLUE_D, WHITE_C);
	m_pGridYieldL.SetItemFont(row, 2, &clsFunc.OnLogFont(16));
	m_pGridYieldL.SetItemText(row, 2, _T("Cum"));

	row = 2;
	m_pGridYieldL.SetItemBkColour(row, 0, BLUE_D, WHITE_C);
	m_pGridYieldL.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridYieldL.SetItemText(row, 0, _T("Input"));

	m_pGridYieldL.MergeCells(row, 1, row, 2);
	m_pGridYieldL.SetItemBkColour(row, 1, WHITE_C, BLACK_C);
	m_pGridYieldL.SetItemFont(row, 1, &clsFunc.OnLogFont(16));
	m_pGridYieldL.SetItemText(row, 1, _T("0"));

	row = 3;
	m_pGridYieldL.SetItemBkColour(row, 0, BLUE_D, WHITE_C);
	m_pGridYieldL.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridYieldL.SetItemText(row, 0, _T("Pass"));

	m_pGridYieldL.SetItemBkColour(row, 1, GREEN_L, BLACK_C);
	m_pGridYieldL.SetItemFont(row, 1, &clsFunc.OnLogFont(16));
	m_pGridYieldL.SetItemText(row, 1, _T("0"));

	m_pGridYieldL.SetItemBkColour(row, 2, GREEN_L, BLACK_C);
	m_pGridYieldL.SetItemFont(row, 2, &clsFunc.OnLogFont(16));
	m_pGridYieldL.SetItemText(row, 2, _T("0"));

	row = 4;
	m_pGridYieldL.SetItemBkColour(row, 0, BLUE_D, WHITE_C);
	m_pGridYieldL.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridYieldL.SetItemText(row, 0, _T("Fail"));

	m_pGridYieldL.SetItemBkColour(row, 1, RED_L, BLACK_C);
	m_pGridYieldL.SetItemFont(row, 1, &clsFunc.OnLogFont(16));
	m_pGridYieldL.SetItemText(row, 1, _T("0"));

	m_pGridYieldL.SetItemBkColour(row, 2, RED_L, BLACK_C);
	m_pGridYieldL.SetItemFont(row, 2, &clsFunc.OnLogFont(16));
	m_pGridYieldL.SetItemText(row, 2, _T("0"));

	row = 5;
	m_pGridYieldL.SetItemBkColour(row, 0, BLUE_D, WHITE_C);
	m_pGridYieldL.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridYieldL.SetItemText(row, 0, _T("Yield"));

	m_pGridYieldL.SetItemBkColour(row, 1, GREEN_L, BLACK_C);
	m_pGridYieldL.SetItemFont(row, 1, &clsFunc.OnLogFont(16));
	m_pGridYieldL.SetItemText(row, 1, _T("0"));

	m_pGridYieldL.SetItemBkColour(row, 2, GREEN_L, BLACK_C);
	m_pGridYieldL.SetItemFont(row, 2, &clsFunc.OnLogFont(16));
	m_pGridYieldL.SetItemText(row, 2, _T("0"));
}

void CScreenMain::OnInitGridYieldDaily()
{
	int   i, j;
	int	  max_row, max_col;
	int   row;

	CString strTmp;
	CRect rect;

	max_row = 6; // unloader tray y count
	max_col = 3;

	m_pGridYieldD.SetFrameFocusCell(FALSE);
	m_pGridYieldD.SetTrackFocusCell(FALSE);
	m_pGridYieldD.EnableSelection(FALSE);

	m_pGridYieldD.SetGridLineColor(BLACK_C);
	m_pGridYieldD.SetGridLines(1);

	m_pGridYieldD.SetRowCount(max_row);
	m_pGridYieldD.SetColumnCount(max_col);

	m_pGridYieldD.SetFixedRowCount(0);
	m_pGridYieldD.SetFixedColumnCount(0);
	m_pGridYieldD.SetFixedBkColor(RGB(0,0,200));
	m_pGridYieldD.SetFixedBkColor(RGB(200,200,255));
	m_pGridYieldD.SetTextBkColor(RGB(150,150,200));

	for (i=0; i<max_row; i++) 
	{
		m_pGridYieldD.SetRowHeight(i, 23);

		for (j=0; j<max_col; j++) 
		{
			m_pGridYieldD.SetColumnWidth(j, 80);

			m_pGridYieldD.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_pGridYieldD.SetItemState(i, j, GVIS_READONLY);
			m_pGridYieldD.SetItemBkColour(i, j, WHITE_C, CLR_DEFAULT);
		}
	}

	row = 0;
	m_pGridYieldD.MergeCells(row, 0, row, 2);
	m_pGridYieldD.SetItemBkColour(row, 0, RGB(219, 229, 241), BLACK_C);
	m_pGridYieldD.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridYieldD.SetItemText(row, 0, _T("Daily Yield Info"));

	row = 1;
	m_pGridYieldD.SetItemBkColour(row, 0, BLUE_D, WHITE_C);
	m_pGridYieldD.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridYieldD.SetItemText(row, 0, _T(""));

	m_pGridYieldD.SetItemBkColour(row, 1, BLUE_D, WHITE_C);
	m_pGridYieldD.SetItemFont(row, 1, &clsFunc.OnLogFont(16));
	m_pGridYieldD.SetItemText(row, 1, _T("Prime"));

	m_pGridYieldD.SetItemBkColour(row, 2, BLUE_D, WHITE_C);
	m_pGridYieldD.SetItemFont(row, 2, &clsFunc.OnLogFont(16));
	m_pGridYieldD.SetItemText(row, 2, _T("Cum"));

	row = 2;
	m_pGridYieldD.SetItemBkColour(row, 0, BLUE_D, WHITE_C);
	m_pGridYieldD.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridYieldD.SetItemText(row, 0, _T("Input"));

	m_pGridYieldD.MergeCells(row, 1, row, 2);
	m_pGridYieldD.SetItemBkColour(row, 1, WHITE_C, BLACK_C);
	m_pGridYieldD.SetItemFont(row, 1, &clsFunc.OnLogFont(16));
	m_pGridYieldD.SetItemText(row, 1, _T("0"));

	row = 3;
	m_pGridYieldD.SetItemBkColour(row, 0, BLUE_D, WHITE_C);
	m_pGridYieldD.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridYieldD.SetItemText(row, 0, _T("Pass"));

	m_pGridYieldD.SetItemBkColour(row, 1, GREEN_L, BLACK_C);
	m_pGridYieldD.SetItemFont(row, 1, &clsFunc.OnLogFont(16));
	m_pGridYieldD.SetItemText(row, 1, _T("0"));

	m_pGridYieldD.SetItemBkColour(row, 2, GREEN_L, BLACK_C);
	m_pGridYieldD.SetItemFont(row, 2, &clsFunc.OnLogFont(16));
	m_pGridYieldD.SetItemText(row, 2, _T("0"));

	row = 4;
	m_pGridYieldD.SetItemBkColour(row, 0, BLUE_D, WHITE_C);
	m_pGridYieldD.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridYieldD.SetItemText(row, 0, _T("Fail"));

	m_pGridYieldD.SetItemBkColour(row, 1, RED_L, BLACK_C);
	m_pGridYieldD.SetItemFont(row, 1, &clsFunc.OnLogFont(16));
	m_pGridYieldD.SetItemText(row, 1, _T("0"));

	m_pGridYieldD.SetItemBkColour(row, 2, RED_L, BLACK_C);
	m_pGridYieldD.SetItemFont(row, 2, &clsFunc.OnLogFont(16));
	m_pGridYieldD.SetItemText(row, 2, _T("0"));

	row = 5;
	m_pGridYieldD.SetItemBkColour(row, 0, BLUE_D, WHITE_C);
	m_pGridYieldD.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridYieldD.SetItemText(row, 0, _T("Yield"));

	m_pGridYieldD.SetItemBkColour(row, 1, GREEN_L, BLACK_C);
	m_pGridYieldD.SetItemFont(row, 1, &clsFunc.OnLogFont(16));
	m_pGridYieldD.SetItemText(row, 1, _T("0"));

	m_pGridYieldD.SetItemBkColour(row, 2, GREEN_L, BLACK_C);
	m_pGridYieldD.SetItemFont(row, 2, &clsFunc.OnLogFont(16));
	m_pGridYieldD.SetItemText(row, 2, _T("0"));
}

void CScreenMain::OnInitGridBufferData()
{
	int   i, j;
	int	  max_row, max_col;
	int   row;
	int   nCnt;
	CString strTmp;
	CRect rect;

	max_row = 41; // unloader tray y count
	max_col = 3;

	m_pGridBufferData.SetFrameFocusCell(FALSE);
	m_pGridBufferData.SetTrackFocusCell(FALSE);
	m_pGridBufferData.EnableSelection(FALSE);

	m_pGridBufferData.SetGridLineColor(BLACK_C);
	m_pGridBufferData.SetGridLines(1);

	m_pGridBufferData.SetRowCount(max_row);
	m_pGridBufferData.SetColumnCount(max_col);

	m_pGridBufferData.SetFixedRowCount(0);
	m_pGridBufferData.SetFixedColumnCount(0);
	m_pGridBufferData.SetFixedBkColor(RGB(0,0,200));
	m_pGridBufferData.SetFixedBkColor(RGB(200,200,255));
	m_pGridBufferData.SetTextBkColor(RGB(150,150,200));

	nCnt = max_row - 1;

	for (i=0; i<max_row; i++) 
	{
		//strTmp.Format(_T("%d"),i + 1);
		strTmp.Format(_T("%d"),nCnt--);
		m_pGridBufferData.SetRowHeight(i, 16);
		m_pGridBufferData.SetItemText(i + 1, 0, strTmp);

		for (j=0; j<max_col; j++) 
		{
			if (j == 0)
			{
				m_pGridBufferData.SetColumnWidth(j, 40);
			}
			else
			{
				m_pGridBufferData.SetColumnWidth(j, 105);
			}
		
			m_pGridBufferData.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_pGridBufferData.SetItemState(i, j, GVIS_READONLY);
			m_pGridBufferData.SetItemBkColour(i, j, WHITE_C, CLR_DEFAULT);
		}
	}

	row = 0;
	m_pGridBufferData.SetItemBkColour(row, 0, RGB(219, 229, 241), BLACK_C);
	m_pGridBufferData.SetItemFont(row, 0, &clsFunc.OnLogFont(13));
	m_pGridBufferData.SetItemText(row, 0, _T("No"));

	
	m_pGridBufferData.SetItemBkColour(row, 1, RGB(219, 229, 241), BLACK_C);
	m_pGridBufferData.SetItemFont(row, 1, &clsFunc.OnLogFont(13));
	m_pGridBufferData.SetItemText(row, 1, _T("Left"));

	
	m_pGridBufferData.SetItemBkColour(row, 2, RGB(219, 229, 241), BLACK_C);
	m_pGridBufferData.SetItemFont(row, 2, &clsFunc.OnLogFont(13));
	m_pGridBufferData.SetItemText(row, 2, _T("Right"));

	m_pGridBufferData.Invalidate(FALSE);
}

void CScreenMain::OnMainBufferDisplay()
{
	int	  max_row, max_col;


	CString strTmp;
	CRect rect;

	max_row = MAX_BUFFER + 1; // unloader tray y count
	max_col = 3;

	for (int i = 0; i<MAX_BUFFER; i++)
	{
		for (int j = 0; j<2; j++)
		{
			m_pGridBufferData.SetItemText(i + 1,j + 1, st_Buffer_info.strBufferSerial[j][i]);
		}
	}
	m_pGridBufferData.Invalidate(FALSE);
}

void CScreenMain::OnMainPickerDisplay()
{
//	int   j;
	int	  /*max_row,*/ max_col;
	

	CString strTmp;
	CRect rect;
	max_col = 3;


		for (int j = 0; j<MAX_PICKER; j++)
		{
			if (st_Picker_info.nPickerData[0][j][BIN] == FAIL )
			{
				m_pGridRobotTurnPos.SetItemBkColour(1, j + 1, RED_C, BLACK_L);
			}
			else if (st_Picker_info.nPickerData[0][j][BIN] == -1 )
			{
				m_pGridRobotTurnPos.SetItemBkColour(1, j + 1, WHITE_C, BLACK_L);
			}

			if (st_Picker_info.nPickerData[1][j][BIN] == FAIL)
			{
				m_pGridRobotTurnPos.SetItemBkColour(3, j + 1, RED_C, BLACK_L);
			}
			else if (st_Picker_info.nPickerData[1][j][BIN] == -1 )
			{
				m_pGridRobotTurnPos.SetItemBkColour(3, j + 1, WHITE_C, BLACK_L);
			}

			m_pGridRobotTurnPos.SetItemText(1,j + 1, st_Picker_info.strPickerSerial[0][j]);
			m_pGridRobotTurnPos.SetItemText(3,j + 1, st_Picker_info.strPickerSerial[1][j]);
	  }
		
	m_pGridRobotTurnPos.Invalidate(FALSE);
}

void CScreenMain::OnMainPcbDataDisplay()
{
//	int   j;
	int	 /* max_row,*/ max_col;


	CString strTmp;
	CRect rect;
	max_col = 3;


	for (int j = 0; j<PCB_CNT; j++)
	{
		m_pGridPcbData.SetItemText(1, j + 1, st_Pcb_info.strPcbSerial[1][j]);
		m_pGridPcbData.SetItemText(3,9 - j , st_Pcb_info.strPcbSerial[0][j]);
	}

	m_pGridPcbData.Invalidate(FALSE);
}



void CScreenMain::OnMainLotDisplay()
{
	CString strTemp;

//	double dAve;

	m_pGridWorkInfoC.SetItemText(1, 1, st_lot_info[LOT_CURR].strLotNo);
	m_pGridWorkInfoN.SetItemText(1, 1, st_lot_info[LOT_NEXT].strLotNo);

	m_pGridWorkInfoC.SetItemText(2, 1, st_lot_info[LOT_CURR].strPartNo);
	m_pGridWorkInfoN.SetItemText(2, 1, st_lot_info[LOT_NEXT].strPartNo);

	if (st_lot_info[LOT_CURR].nLastModule == YES)
	{
		m_pGridWorkInfoC.SetItemText(3, 1, _T("YES"));
	}
	else
	{
		m_pGridWorkInfoC.SetItemText(3, 1, _T("NO"));
	}

	//	strTemp.Format(_T("%d"), st_lot_info[LOT_CURR].nTotalCnt);
	//	m_pGridWorkInfoC.SetItemText(4, 1, strTemp);

	//	strTemp.Format(_T("%d"), st_lot_info[LOT_CURR].nLane);
	//	m_pGridWorkInfoC.SetItemText(5, 1, strTemp);

// 	strTemp.Format(_T("%.2f / %d"), st_count_info.dHourPer, st_count_info.nHourSocket);
// 	m_pGridWorkInfoC.SetItemText(4, 1, strTemp);

// 	dAve = 0.0f;
// 	if (st_count_info.nUphCnt > 0)
// 	{
// 		dAve = st_count_info.dDailyPer / (double)st_count_info.nUphCnt;
// 	}
// 	strTemp.Format(_T("%.2f"), dAve);
// 	m_pGridWorkInfoC.SetItemText(5, 1, strTemp);

	m_pGridWorkInfoC.Invalidate(FALSE);
	m_pGridWorkInfoN.Invalidate(FALSE);
}

void CScreenMain::OnInitGridFrontSmema()
{
	int   i, j;
	int	  nMaxRow, nMaxCol;
	int   nRow;
	CString strTmp;

	nMaxRow = 1; // unloader tray y count
	nMaxCol = 1;
	
	m_pGridFrontSmema.SetFrameFocusCell(FALSE);
	m_pGridFrontSmema.SetTrackFocusCell(FALSE);
	m_pGridFrontSmema.EnableSelection(FALSE);

	m_pGridFrontSmema.SetGridLineColor(BLACK_C);
	m_pGridFrontSmema.SetGridLines(1);

	m_pGridFrontSmema.SetRowCount(nMaxRow);
	m_pGridFrontSmema.SetColumnCount(nMaxCol);

	m_pGridFrontSmema.SetFixedRowCount(0);
	m_pGridFrontSmema.SetFixedColumnCount(0);
	m_pGridFrontSmema.SetFixedBkColor(RGB(0,0,200));
	m_pGridFrontSmema.SetFixedBkColor(RGB(200,200,255));
	m_pGridFrontSmema.SetTextBkColor(RGB(150,150,200));

	for (i=0; i<nMaxRow; i++) 
	{
		m_pGridFrontSmema.SetRowHeight(i, 30);

		for (j=0; j<nMaxCol; j++) 
		{
			m_pGridFrontSmema.SetColumnWidth(j,90);
				
			m_pGridFrontSmema.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_pGridFrontSmema.SetItemState(i, j, GVIS_READONLY);
			m_pGridFrontSmema.SetItemBkColour(i, j, WHITE_C, CLR_DEFAULT);
		}
	}
	nRow = 0;
	m_pGridFrontSmema.SetItemBkColour(nRow, 0, GREEN_D, BLACK_C);
	m_pGridFrontSmema.SetItemFont(nRow, 0, &clsFunc.OnLogFont(18));
	m_pGridFrontSmema.SetItemText(nRow, 0, _T(">> Transfer"));

// 	nRow = 1;
// 	m_pGridFrontSmema.SetItemBkColour(nRow, 0, GREEN_D, BLACK_C);
// 	m_pGridFrontSmema.SetItemFont(nRow, 0, &clsFunc.OnLogFont(14));
// 	m_pGridFrontSmema.SetItemText(nRow, 0, _T("<< Request"));

	m_pGridFrontSmema.Invalidate(FALSE);
}

void CScreenMain::OnInitGridRearSmema()
{
	int   i, j;
	int	  nMaxRow, nMaxCol;
	int   nRow;
	CString strTmp;

	nMaxRow = 1; // unloader tray y count
	nMaxCol = 1;
	
	m_pGridRearSmema.SetFrameFocusCell(FALSE);
	m_pGridRearSmema.SetTrackFocusCell(FALSE);
	m_pGridRearSmema.EnableSelection(FALSE);

	m_pGridRearSmema.SetGridLineColor(BLACK_C);
	m_pGridRearSmema.SetGridLines(1);

	m_pGridRearSmema.SetRowCount(nMaxRow);
	m_pGridRearSmema.SetColumnCount(nMaxCol);

	m_pGridRearSmema.SetFixedRowCount(0);
	m_pGridRearSmema.SetFixedColumnCount(0);
	m_pGridRearSmema.SetFixedBkColor(RGB(0,0,200));
	m_pGridRearSmema.SetFixedBkColor(RGB(200,200,255));
	m_pGridRearSmema.SetTextBkColor(RGB(150,150,200));

	for (i=0; i<nMaxRow; i++) 
	{
		m_pGridRearSmema.SetRowHeight(i, 30);

		for (j=0; j<nMaxCol; j++) 
		{
			m_pGridRearSmema.SetColumnWidth(j, 90);
				
			m_pGridRearSmema.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_pGridRearSmema.SetItemState(i, j, GVIS_READONLY);
			m_pGridRearSmema.SetItemBkColour(i, j, WHITE_C, CLR_DEFAULT);
		}
	}

	//nRow = 0;
// 	m_pGridRearSmema.SetItemBkColour(nRow, 0, YELLOW_D, BLACK_C);
// 	m_pGridRearSmema.SetItemFont(nRow, 0, &clsFunc.OnLogFont(14));
// 	m_pGridRearSmema.SetItemText(nRow, 0, _T("Request <<"));

	nRow = 0;
	m_pGridRearSmema.SetItemBkColour(nRow, 0, GREEN_D, BLACK_C);
	m_pGridRearSmema.SetItemFont(nRow, 0, &clsFunc.OnLogFont(18));
	m_pGridRearSmema.SetItemText(nRow, 0, _T("Transfer >>"));

	m_pGridRearSmema.Invalidate(FALSE);
}

void CScreenMain::OnMainDisplayFrontSmema()
{

	if (FAS_IO.get_out_bit(st_io_info.i_InConvInChk, IO_OFF) == IO_ON)
	{
		m_pGridFrontSmema.SetItemBkColour(0, 0, GREEN_C, BLACK_C);
	}
	else
	{
		m_pGridFrontSmema.SetItemBkColour(0 ,0, GREEN_D, BLACK_C);
	}

	m_pGridFrontSmema.Invalidate(FALSE);
}

void CScreenMain::OnMainDisplayRearSmema()
{

	if (FAS_IO.get_out_bit(st_io_info.i_OutConvChk, IO_OFF) == IO_ON)
	{
		m_pGridRearSmema.SetItemBkColour(0, 0, GREEN_C, BLACK_C);
	}
	else
	{
		m_pGridRearSmema.SetItemBkColour(0, 0, GREEN_D, BLACK_C);
	}

	m_pGridRearSmema.Invalidate(FALSE);
}

void CScreenMain::OnBnClickedBtnStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int pos;
	

	if ( m_nTemp < 3 )
	{
		pos = WM_PCB_CV_IN_MOVE_DRAW_MAIN;
	}
	else if ( m_nTemp > 2)
	{
		
// 		if (m_nTemp == 5)
// 		{
// 			OnMainPcbWork();
// 		}
// 		else if (m_nTemp ==6)
// 		{
// 			TurnPositionGrid();
// 		}else if (m_nTemp == 7)
// 		{
// 			ReversePositionGrid();
// 		}

		pos =  WM_PCB_CV_TURN_MOVE_DRAW_MAIN;
		
	}
	else if (m_nTemp<18)
	{
		pos =  WM_PCB_CV_OUT_MOVE_DRAW_MAIN;	
	}
	else
	{
		pos =  WM_PCB_CV_IN_MOVE_DRAW_MAIN;
		//pos = 0;
		m_nTemp = 0;
	}


	st_handler_info.cWndMain->PostMessage(pos,m_nTemp,0);
	m_nTemp++;
}


void CScreenMain::OnBnClickedBtnCvIn()
{
   // clsRunRobot.OnFeederReq(COM_CLEAR);

	st_handler_info.nRunStatus = dRUN;
	//clsRunRobot.m_nRunStep =0;
	//st_sync_info.nSmema_Tray_Output_Req = CTL_NO;
	//st_sync_info.TurnConvJobReady[ROBOT] = CTL_NONE;
	
	//FAS_IO.set_out_bit(st_io_info.o_InConvPosStopperDw,IO_ON);
	//FAS_IO.set_out_bit(st_io_info.o_InConvPosStopperUp,IO_OFF);
// 	for (i =0; i<3; i++)
// 	{
// 		clsRunConveyor.m_nRunStep[i] = 0;
// 	}

	m_thread[6] = AfxBeginThread(OnThreadConv, this);  
	if (m_thread[6] != NULL)	
	{
		m_thrHandle[6] = m_thread[6]->m_hThread;
	}
	return;

	if(mb_Check[0] == true)
	{
		st_handler_info.cWndMain->PostMessage(WM_MAIN_IO_DISPLAY,CONVEYOR_IN,IO_ON);
		mb_Check[0] = false;
	}
	else
	{
		st_handler_info.cWndMain->PostMessage(WM_MAIN_IO_DISPLAY,CONVEYOR_IN,IO_OFF);
		mb_Check[0] = true;
	}
	
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CScreenMain::OnBnClickedBtnCvInPos()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
// 	FAS_IO.set_out_bit(st_io_info.o_InConvPosStopperDw,IO_ON);
// 	FAS_IO.set_out_bit(st_io_info.o_InConvPosStopperUp,IO_OFF);
// 	FAS_IO.set_out_bit(st_io_info.o_TurnMotorDirection,IO_OFF);
// 	FAS_IO.set_out_bit(st_io_info.o_InConvMotorOn,IO_OFF);
// 	FAS_IO.set_out_bit(st_io_info.o_OutConvMotorOn,IO_OFF);
// 	FAS_IO.set_out_bit(st_io_info.o_OutPosStopperDw,IO_ON);
// 	FAS_IO.set_out_bit(st_io_info.o_OutPosStopperUp,IO_OFF);
// 	FAS_IO.set_out_bit(st_io_info.o_TurnPosDw,IO_ON);
// 	FAS_IO.set_out_bit(st_io_info.o_TurnPosUp,IO_OFF);
// 	clsRunRobot.OnFeederReq(CTL_CLEAR);
// 	clsRunRobot.m_nLinearMove_Index = 0;
// 	CTL_Lib.m_nStep_Linear[clsRunRobot.m_nLinearMove_Index] = 0;
// 	clsRunRobot.m_nPickerNum = 0;
	st_handler_info.nRunStatus = dSTOP;
// 	st_sync_info.nFrontTrayIn = CTL_NO;
// 	st_sync_info.nMidTrayIn = CTL_NO;
// 	st_sync_info.nRearTrayIn = CTL_NO;
	return;

	if(mb_Check[1] == true)
	{
		st_handler_info.cWndMain->PostMessage(WM_MAIN_IO_DISPLAY,IN_CONVEYOR_POSITION,IO_ON);
		mb_Check[1] = false;
	}
	else
	{
		st_handler_info.cWndMain->PostMessage(WM_MAIN_IO_DISPLAY,IN_CONVEYOR_POSITION,IO_OFF);
		mb_Check[1] = true;
	}
}


void CScreenMain::OnBnClickedBtnCvInUpdw()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(mb_Check[2] == true)
	{
		st_handler_info.cWndMain->PostMessage(WM_MAIN_IO_DISPLAY,IN_CONVEYOR_STOPPER_UP,IO_UP);
		mb_Check[2] = false;
	}
	else
	{
		st_handler_info.cWndMain->PostMessage(WM_MAIN_IO_DISPLAY,IN_CONVEYOR_STOPPER_DOWN,IO_DOWN);
		mb_Check[2] = true;
	}
}


void CScreenMain::OnBnClickedBtnTurnPos()
{

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	st_basic_info.nPcbType = RDIMM;
	st_basic_info.nColCnt = 6;
	st_basic_info.nRowCnt = 1;
	return;

	if(mb_Check[3] == true)
	{
		st_handler_info.cWndMain->PostMessage(WM_MAIN_IO_DISPLAY,TURN_CONVEYOR_POSITION,IO_ON);
		mb_Check[3] = false;
	}
	else
	{
		st_handler_info.cWndMain->PostMessage(WM_MAIN_IO_DISPLAY,TURN_CONVEYOR_POSITION,IO_OFF);
		mb_Check[3] = true;
	}
}


void CScreenMain::OnBnClickedBtnTurnUpDw()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(mb_Check[4] == true)
	{
		st_handler_info.cWndMain->PostMessage(WM_MAIN_IO_DISPLAY,TURN_STOPPER_UP,IO_UP);
		mb_Check[4] = false;
	}
	else
	{
		st_handler_info.cWndMain->PostMessage(WM_MAIN_IO_DISPLAY,TURN_STOPPER_DOWN,IO_DOWN);
		mb_Check[4] = true;
	}
}


void CScreenMain::OnBnClickedBtnOutInPos()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	st_basic_info.nPcbType = UDIMM_9;
	st_basic_info.nColCnt = 9;
	st_basic_info.nRowCnt = 1;
	return;
	if(mb_Check[5] == true)
	{
		st_handler_info.cWndMain->PostMessage(WM_MAIN_IO_DISPLAY,OUT_IN_POSITION,IO_ON);
		mb_Check[5] = false;
	}
	else
	{
		st_handler_info.cWndMain->PostMessage(WM_MAIN_IO_DISPLAY,OUT_IN_POSITION,IO_OFF);
		mb_Check[5] = true;
	}
}


void CScreenMain::OnBnClickedBtnOutPos()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	st_basic_info.nPcbType = SODIMM;
	st_basic_info.nColCnt = 9;
	st_basic_info.nRowCnt = 2;
	 
	return;

	if(mb_Check[6] == true)
	{
		st_handler_info.cWndMain->PostMessage(WM_MAIN_IO_DISPLAY,OUT_CONVEYOR,IO_ON);
		mb_Check[6] = false;
	}
	else
	{
		st_handler_info.cWndMain->PostMessage(WM_MAIN_IO_DISPLAY,OUT_CONVEYOR,IO_OFF);
		mb_Check[6] = true;
	}
}


void CScreenMain::OnBnClickedBtnOutPosUpDw()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		int i;
	 	FAS_IO.set_out_bit(st_io_info.o_InConvPosStopperDw,IO_ON);
	 	FAS_IO.set_out_bit(st_io_info.o_InConvPosStopperUp,IO_OFF);
	 	FAS_IO.set_out_bit(st_io_info.o_TurnMotorDirection,IO_OFF);
	 	FAS_IO.set_out_bit(st_io_info.o_InConvMotorOn,IO_OFF);
	 	FAS_IO.set_out_bit(st_io_info.o_OutConvMotorOn,IO_OFF);
	 	FAS_IO.set_out_bit(st_io_info.o_OutPosStopperDw,IO_ON);
	 	FAS_IO.set_out_bit(st_io_info.o_OutPosStopperUp,IO_OFF);
	 	FAS_IO.set_out_bit(st_io_info.o_TurnPosDw,IO_ON);
	 	FAS_IO.set_out_bit(st_io_info.o_TurnPosUp,IO_OFF);
	 	clsRunRobot.OnFeederReq(COM_CLEAR);
	 	clsRunRobot.m_nLinearMove_Index = 0;
	 	CTL_Lib.m_nStep_Linear[clsRunRobot.m_nLinearMove_Index] = 0;
	 	clsRunRobot.m_nPickerNum = 0;
		st_handler_info.nRunStatus = dSTOP;
	 	st_sync_info.nFrontTrayIn = CTL_NO;
	 	st_sync_info.nMidTrayIn = CTL_NO;
	 	st_sync_info.nRearTrayIn = CTL_NO;
		clsRunRobot.m_nRunStep =0;
		st_sync_info.nSmema_Tray_Output_Req = CTL_NO;
		st_sync_info.TurnConvJobReady[ROBOT] = CTL_NONE;
		clsRunConveyor.m_nSmemaStep = 0;
		st_sync_info.nSmema_Front =  -1;
		
		 	for (i =0; i<3; i++)
		 	{
		 		clsRunConveyor.m_nRunStep[i] = 0;
		 	}
	return;
	if(mb_Check[7] == true)
	{
		st_handler_info.cWndMain->PostMessage(WM_MAIN_IO_DISPLAY,OUT_CONVEYOR_STOPPER_UP,IO_UP);
		mb_Check[7] = false;
	}
	else
	{
		st_handler_info.cWndMain->PostMessage(WM_MAIN_IO_DISPLAY,OUT_CONVEYOR_STOPPER_DOWN,IO_DOWN);
		mb_Check[7] = true;
	}
}


void CScreenMain::OnMainPcbWork()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	int i,j,nTemp,nRow,nCol;
	CString strTmp;

//	RECT rt;

	nRow = st_basic_info.nRowCnt;
	nCol = st_basic_info.nColCnt;

	m_GridTm_2.SetFrameFocusCell(FALSE);
	m_GridTm_2.SetTrackFocusCell(FALSE);
	m_GridTm_2.EnableSelection(FALSE);
	m_GridTm_2.SetGridLineColor(RGB(25, 1, 1));
	m_GridTm_2.SetGridLines(1);
	m_GridTm_2.SetRowCount(nRow);
	m_GridTm_2.SetColumnCount(nCol);
	m_GridTm_2.SetFixedRowCount(0);
	m_GridTm_2.SetFixedColumnCount(0);
	m_GridTm_2.SetFixedBkColor(RGB(0,0,200));
	m_GridTm_2.SetFixedBkColor(RGB(200,200,255));
	m_GridTm_2.SetTextBkColor(RGB(150,150,200));

	//m_GridPcbType.nLeftSize = 275;

	mn_Size = 18;

	if (mbTurn)
	{
		for (i=0; i<nRow; i++) 
		{
			nTemp = nCol;
			m_GridTm_2.SetRowHeight(i, 74/nRow);

			for (j=0; j<nCol; j++) 
			{	
				if (i == 0)
				{
					mn_Size = mn_Size +18;
				}
				//strTmp.Format(_T("%d"), j+1);
				strTmp.Format(_T("%d"), nTemp--);
				m_GridTm_2.SetColumnWidth(j, 19);
				m_GridTm_2.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
				m_GridTm_2.SetItemState(i, j, GVIS_READONLY);

// 				if (st_Pcb_info.nPcbSelect[i][j] == FALSE)
// 				{
// 					m_GridTm_2.SetItemBkColour(i, j, RGB(255,255,128), CLR_DEFAULT);
// 				}
// 				else
// 				{
					m_GridTm_2.SetItemBkColour(i, j, RGB(128,255,255), CLR_DEFAULT);
					
			//	}
				m_GridTm_2.SetItemText(i, j, strTmp);	
				
				
			}
		}
		mbTurn = false;
	}
	else
	{
		for (i=0; i<nRow; i++) 
		{
			nTemp = nCol;
			m_GridTm_2.SetRowHeight(i, 74/nRow);

			for (j=0; j<nCol; j++) 
			{	
				if (i == 0)
				{
					mn_Size = mn_Size +18;
				}
				strTmp.Format(_T("%d"), nTemp--);
				m_GridTm_2.SetColumnWidth(j, 19);
				m_GridTm_2.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
				m_GridTm_2.SetItemState(i, j, GVIS_READONLY);



				if (st_Pcb_info.nPcbSelect[i][j] == TRUE)
				{
					m_GridTm_2.SetItemBkColour(i, j, RGB(128,255,255), CLR_DEFAULT);
				}
				else
				{
					m_GridTm_2.SetItemBkColour(i, j, RGB(255,255,128), CLR_DEFAULT);
				}


				m_GridTm_2.SetItemText(i, j, strTmp);
			}
		}	
	}
	st_Pcb_info.nLeftSize = mn_Size;
	
	//SetTrayPos(5);
}
void CScreenMain::TurnPositionGrid()
{
	int i,j,nRow,nCol;
	CString strTmp;

	nRow = st_basic_info.nColCnt;
	nCol = st_basic_info.nRowCnt;

	m_GridTm_2.SetFrameFocusCell(FALSE);
	m_GridTm_2.SetTrackFocusCell(FALSE);
	m_GridTm_2.EnableSelection(FALSE);
	m_GridTm_2.SetGridLineColor(RGB(25, 1, 1));
	m_GridTm_2.SetGridLines(1);
	m_GridTm_2.SetRowCount(nRow);
	m_GridTm_2.SetColumnCount(nCol);
	m_GridTm_2.SetFixedRowCount(0);
	m_GridTm_2.SetFixedColumnCount(0);
	m_GridTm_2.SetFixedBkColor(RGB(0,0,200));
	m_GridTm_2.SetFixedBkColor(RGB(200,200,255));
	m_GridTm_2.SetTextBkColor(RGB(150,150,200));

	for (i=0; i<nRow; i++) 
	{
		m_GridTm_2.SetRowHeight(i, 19);
		strTmp.Format(_T("%d"), i+1);
		for (j=0; j<nCol; j++) 
		{
			m_GridTm_2.SetColumnWidth(j, 74/nCol);		
			m_GridTm_2.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_GridTm_2.SetItemState(i, j, GVIS_READONLY);

			if (st_Pcb_info.nPcbSelect[j][i] == TRUE)
			{
				m_GridTm_2.SetItemBkColour(i, j, RGB(255, 255,128), CLR_DEFAULT);
			}
			else
			{
				m_GridTm_2.SetItemBkColour(i, j, RGB(128, 255, 255), CLR_DEFAULT);
			}

			m_GridTm_2.SetItemText(i, j, strTmp);
		}
	}
	mbTurn = true;
	//SetTrayPos(6);
}


void CScreenMain::ReversePositionGrid()
{
	int i,j,nRow,nCol;
	CString strTmp;

	nRow = st_basic_info.nRowCnt;
	nCol = st_basic_info.nColCnt;

	m_GridTm_2.SetFrameFocusCell(FALSE);
	m_GridTm_2.SetTrackFocusCell(FALSE);
	m_GridTm_2.EnableSelection(FALSE);
	m_GridTm_2.SetGridLineColor(RGB(25, 1, 1));
	m_GridTm_2.SetGridLines(1);
	m_GridTm_2.SetRowCount(nRow);
	m_GridTm_2.SetColumnCount(nCol);
	m_GridTm_2.SetFixedRowCount(0);
	m_GridTm_2.SetFixedColumnCount(0);
	m_GridTm_2.SetFixedBkColor(RGB(0,0,200));
	m_GridTm_2.SetFixedBkColor(RGB(200,200,255));
	m_GridTm_2.SetTextBkColor(RGB(150,150,200));

	//OnSetReverseGridType();


	mn_Size = 18;

	for (i=0; i<nRow; i++) 
	{
		/*nTemp = nCol;*/
		m_GridTm_2.SetRowHeight(i, 74/nRow);

		for (j=0; j<nCol; j++) 
		{	
			if (i == 0)
			{
				mn_Size = mn_Size +18;
			}
			strTmp.Format(_T("%d"), j+1);
			m_GridTm_2.SetColumnWidth(j, 19);
			m_GridTm_2.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_GridTm_2.SetItemState(i, j, GVIS_READONLY);

			if (st_Pcb_info.nPcbSelect[i][j] == FALSE)
			{
				m_GridTm_2.SetItemBkColour(i, j, RGB(128,255,255), CLR_DEFAULT);
			}
			else
			{
				m_GridTm_2.SetItemBkColour(i, j, RGB(255,255,128), CLR_DEFAULT);
			}


			m_GridTm_2.SetItemText(i, j, strTmp);
		}
	}	
	st_Pcb_info.nLeftSize = mn_Size;

	//SetTrayPos(7);
}
void CScreenMain::OnBnClickedBtnStop3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnMainPcbWork();
}

void CScreenMain::OnBnClickedBtnStop2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	ReversePositionGrid();
	
}

void CScreenMain::OnBnClickedBtnStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	TurnPositionGrid();

}


void CScreenMain::OnBnClickedBtnStart2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	st_handler_info.cWndMain->PostMessage(WM_WORK_DISPLAY,MAIN_ROBOT_LEFT_TURN_DISPLAY,0);
}


void CScreenMain::OnBnClickedBtnStart3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	st_handler_info.cWndMain->PostMessage(WM_WORK_DISPLAY,MAIN_ROBOT_RIGHT_TURN_DISPLAY,0);
}


void CScreenMain::OnBnClickedBtnStart4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	st_handler_info.cWndMain->PostMessage(WM_WORK_DISPLAY,MAIN_ROBOT_CENTER_DISPLAY,0);
}


void CScreenMain::OnBnClickedBtnFrontSmema()
{
	st_sync_info.nSmema_Tray_Output_Req = CONV_READY;
	st_sync_info.nSmema_Tray_Input_Req = CONV_READY;

	return;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//st_sync_info.nSmema_Tray_Input_Req = CONV_REQ;
}


void CScreenMain::OnBnClickedBtnMidConvReq()
{
	st_basic_info.nPcbType = UDIMM_10;
	st_basic_info.nColCnt = 10;
	st_basic_info.nRowCnt = 1;
	return;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	st_sync_info.nMidTrayIn = CTL_YES;
}


void CScreenMain::OnBnClickedBtnCvInPos2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//kwlee 2017.0209 test..
	st_handler_info.nRunStatus = dRUN;
	st_sync_info.nInitPickerRbt = INIT_COMPLETE;
	st_work_info.nSimulationMode = 1;
	clsRunRobot.m_nRunStep = 3100;
	st_sync_info.nConvTurnCheck = 1;
	m_thread[5] = AfxBeginThread(OnThreadRobot, this);  
	if (m_thread[5] != NULL)	
	{
		m_thrHandle[5] = m_thread[5]->m_hThread;
	}

	return;
	//st_work_info.nSimulationMode = 1;
	//st_sync_info.nInitPickerRbt = INIT_COMPLETE;
	st_Buffer_info.strBufferSerial[0][0] = _T("0000_0");
	st_Buffer_info.strBufferSerial[0][1] = _T("0000_1");
	st_Buffer_info.strBufferSerial[0][2] = _T("0000_2");
	st_Buffer_info.strBufferSerial[0][3] = _T("0000_3");
	st_Buffer_info.strBufferSerial[0][4] = _T("0000_4");
	st_Buffer_info.strBufferSerial[0][5] = _T("0000_5");
	st_Buffer_info.strBufferSerial[0][6] = _T("0000_6");
	st_Buffer_info.strBufferSerial[0][7] = _T("0000_7");
	st_Buffer_info.strBufferSerial[0][8] = _T("0000_8");
	st_Buffer_info.strBufferSerial[0][9] = _T("0000_9");
	st_Buffer_info.strBufferSerial[0][10] = _T("0000_10");
	st_Buffer_info.strBufferSerial[0][11] = _T("0000_11");
	st_Buffer_info.strBufferSerial[0][12] = _T("0000_12");
	st_Buffer_info.strBufferSerial[0][13] = _T("0000_13");
	st_Buffer_info.strBufferSerial[0][14] = _T("0000_14");
	st_Buffer_info.strBufferSerial[0][15] = _T("0000_15");
	st_Buffer_info.strBufferSerial[0][16] = _T("0000_16");
	st_Buffer_info.strBufferSerial[0][17] = _T("0000_17");
	st_Buffer_info.strBufferSerial[0][18] = _T("0000_18");
	st_Buffer_info.strBufferSerial[0][19] = _T("0000_19");
	st_Buffer_info.strBufferSerial[0][20] = _T("0000_20");
	st_Buffer_info.strBufferSerial[0][21] = _T("0000_21");
	st_Buffer_info.strBufferSerial[0][22] = _T("0000_22");
	st_Buffer_info.strBufferSerial[0][23] = _T("0000_23");
	st_Buffer_info.strBufferSerial[0][24] = _T("0000_24");
	st_Buffer_info.strBufferSerial[0][25] = _T("0000_25");
	st_Buffer_info.strBufferSerial[0][26] = _T("0000_26");
	st_Buffer_info.strBufferSerial[0][27] = _T("0000_27");
	st_Buffer_info.strBufferSerial[0][28] = _T("0000_28");
	st_Buffer_info.strBufferSerial[0][29] = _T("0000_29");
	st_Buffer_info.strBufferSerial[0][30] = _T("0000_30");
	st_Buffer_info.strBufferSerial[0][31] = _T("0000_31");
	st_Buffer_info.strBufferSerial[0][32] = _T("0000_32");
	st_Buffer_info.strBufferSerial[0][33] = _T("0000_33");
	st_Buffer_info.strBufferSerial[0][34] = _T("0000_34");
	st_Buffer_info.strBufferSerial[0][35] = _T("0000_35");
	st_Buffer_info.strBufferSerial[0][36] = _T("0000_36");
	st_Buffer_info.strBufferSerial[0][37] = _T("0000_37");
	st_Buffer_info.strBufferSerial[0][38] = _T("0000_38");

	st_Buffer_info.strBufferSerial[1][0] = _T("1111_0");
	st_Buffer_info.strBufferSerial[1][1] = _T("1111_1");
	st_Buffer_info.strBufferSerial[1][2] = _T("1111_2");
	st_Buffer_info.strBufferSerial[1][3] = _T("1111_3");
	st_Buffer_info.strBufferSerial[1][4] = _T("1111_4");
	st_Buffer_info.strBufferSerial[1][5] = _T("1111_5");
	st_Buffer_info.strBufferSerial[1][6] = _T("1111_6");
	st_Buffer_info.strBufferSerial[1][7] = _T("1111_7");
	st_Buffer_info.strBufferSerial[1][8] = _T("1111_8");
	st_Buffer_info.strBufferSerial[1][9] = _T("1111_9");
	st_Buffer_info.strBufferSerial[1][10] = _T("1111_10");
	st_Buffer_info.strBufferSerial[1][11] = _T("1111_11");
	st_Buffer_info.strBufferSerial[1][12] = _T("1111_12");
	st_Buffer_info.strBufferSerial[1][13] = _T("1111_13");
	st_Buffer_info.strBufferSerial[1][14] = _T("1111_14");
	st_Buffer_info.strBufferSerial[1][15] = _T("1111_15");
	st_Buffer_info.strBufferSerial[1][16] = _T("1111_16");
	st_Buffer_info.strBufferSerial[1][17] = _T("1111_17");
	st_Buffer_info.strBufferSerial[1][18] = _T("1111_18");
	st_Buffer_info.strBufferSerial[1][19] = _T("1111_19");
	st_Buffer_info.strBufferSerial[1][20] = _T("1111_20");
	st_Buffer_info.strBufferSerial[1][21] = _T("1111_21");
	st_Buffer_info.strBufferSerial[1][22] = _T("1111_22");
	st_Buffer_info.strBufferSerial[1][23] = _T("1111_23");
	st_Buffer_info.strBufferSerial[1][24] = _T("1111_24");
	st_Buffer_info.strBufferSerial[1][25] = _T("1111_25");
	st_Buffer_info.strBufferSerial[1][26] = _T("1111_26");
	st_Buffer_info.strBufferSerial[1][27] = _T("1111_27");
	st_Buffer_info.strBufferSerial[1][28] = _T("1111_28");
	st_Buffer_info.strBufferSerial[1][29] = _T("1111_29");
	st_Buffer_info.strBufferSerial[1][30] = _T("1111_30");
	st_Buffer_info.strBufferSerial[1][31] = _T("1111_31");
	st_Buffer_info.strBufferSerial[1][32] = _T("1111_32");
	st_Buffer_info.strBufferSerial[1][33] = _T("1111_33");
	st_Buffer_info.strBufferSerial[1][34] = _T("1111_34");
	st_Buffer_info.strBufferSerial[1][35] = _T("1111_35");
	st_Buffer_info.strBufferSerial[1][36] = _T("1111_36");
	st_Buffer_info.strBufferSerial[1][37] = _T("1111_37");
	st_Buffer_info.strBufferSerial[1][38] = _T("1111_38");


	
	
	//clsRunRobot.m_nRunStep = 4000;
//	clsRunRobot.m_nPrintOutPutCnt = 16;
	for (int i =0; i<2; i++)
	{
		for (int j = 0; j<MAX_BUFFER; j++)
		{
			if (st_Buffer_info.strBufferSerial[i][j] != _T(""))
			{
				st_Buffer_info.nBufferData[i][j][BIN] = GOOD;

			}
			else
			{
				st_Buffer_info.nBufferData[i][j][BIN] = FAIL;
			}
		}
	}


}


void CScreenMain::OnBnClickedBtnOutPosUpDw2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
		st_Buffer_info.nBufferData[0][3][BIN] = FAIL;
		st_Buffer_info.nBufferData[0][3][EXIST] = NO;
	
		st_Buffer_info.nBufferData[0][1][BIN] = FAIL;
		st_Buffer_info.nBufferData[0][1][EXIST] = NO;
	return;

	st_Buffer_info.strBufferSerial[0][1] = _T("0000_1");
	st_Buffer_info.strBufferSerial[0][2] = _T("0000_2");
	st_Buffer_info.strBufferSerial[0][3] = _T("0000_3");
	st_Buffer_info.strBufferSerial[0][4] = _T("0000_4");
	st_Buffer_info.strBufferSerial[0][5] = _T("0000_5");
	st_Buffer_info.strBufferSerial[0][6] = _T("0000_6");
	st_Buffer_info.strBufferSerial[0][7] = _T("0000_7");
	st_Buffer_info.strBufferSerial[0][8] = _T("0000_8");
	st_Buffer_info.strBufferSerial[0][9] = _T("0000_9");
	st_Buffer_info.strBufferSerial[0][10] = _T("0000_10");
	st_Buffer_info.strBufferSerial[0][11] = _T("0000_11");
	st_Buffer_info.strBufferSerial[0][12] = _T("0000_12");
	st_Buffer_info.strBufferSerial[0][13] = _T("0000_13");
	st_Buffer_info.strBufferSerial[0][14] = _T("0000_14");
	st_Buffer_info.strBufferSerial[0][15] = _T("0000_15");
	st_Buffer_info.strBufferSerial[0][16] = _T("0000_16");
	st_Buffer_info.strBufferSerial[0][17] = _T("0000_17");
	st_Buffer_info.strBufferSerial[0][18] = _T("0000_18");
	st_Buffer_info.strBufferSerial[0][19] = _T("0000_19");
	st_Buffer_info.strBufferSerial[0][20] = _T("0000_20");
	st_Buffer_info.strBufferSerial[0][21] = _T("0000_21");
	st_Buffer_info.strBufferSerial[0][22] = _T("0000_22");
	st_Buffer_info.strBufferSerial[0][23] = _T("0000_23");
	st_Buffer_info.strBufferSerial[0][24] = _T("0000_24");
	st_Buffer_info.strBufferSerial[0][25] = _T("0000_25");
	st_Buffer_info.strBufferSerial[0][26] = _T("0000_26");
	st_Buffer_info.strBufferSerial[0][27] = _T("0000_27");
	st_Buffer_info.strBufferSerial[0][28] = _T("0000_28");
	st_Buffer_info.strBufferSerial[0][29] = _T("0000_29");
	st_Buffer_info.strBufferSerial[0][30] = _T("0000_30");
	st_Buffer_info.strBufferSerial[0][31] = _T("0000_31");
	st_Buffer_info.strBufferSerial[0][32] = _T("0000_32");
	st_Buffer_info.strBufferSerial[0][33] = _T("0000_33");
	st_Buffer_info.strBufferSerial[0][34] = _T("0000_34");
	st_Buffer_info.strBufferSerial[0][35] = _T("0000_35");
	st_Buffer_info.strBufferSerial[0][36] = _T("0000_36");
	st_Buffer_info.strBufferSerial[0][37] = _T("0000_37");
	st_Buffer_info.strBufferSerial[0][38] = _T("0000_38");

	st_Buffer_info.strBufferSerial[1][0] = _T("1111_0");
	st_Buffer_info.strBufferSerial[1][1] = _T("1111_1");
	st_Buffer_info.strBufferSerial[1][2] = _T("1111_2");
	st_Buffer_info.strBufferSerial[1][3] = _T("1111_3");
	st_Buffer_info.strBufferSerial[1][4] = _T("1111_4");
	st_Buffer_info.strBufferSerial[1][5] = _T("1111_5");
	st_Buffer_info.strBufferSerial[1][6] = _T("1111_6");
	st_Buffer_info.strBufferSerial[1][7] = _T("1111_7");
	st_Buffer_info.strBufferSerial[1][8] = _T("1111_8");
	st_Buffer_info.strBufferSerial[1][9] = _T("1111_9");
	st_Buffer_info.strBufferSerial[1][10] = _T("1111_10");
	st_Buffer_info.strBufferSerial[1][11] = _T("1111_11");
	st_Buffer_info.strBufferSerial[1][12] = _T("1111_12");
	st_Buffer_info.strBufferSerial[1][13] = _T("1111_13");
	st_Buffer_info.strBufferSerial[1][14] = _T("1111_14");
	st_Buffer_info.strBufferSerial[1][15] = _T("1111_15");
	st_Buffer_info.strBufferSerial[1][16] = _T("1111_16");
	st_Buffer_info.strBufferSerial[1][17] = _T("1111_17");
	st_Buffer_info.strBufferSerial[1][18] = _T("1111_18");
	st_Buffer_info.strBufferSerial[1][19] = _T("1111_19");
	st_Buffer_info.strBufferSerial[1][20] = _T("1111_20");
	st_Buffer_info.strBufferSerial[1][21] = _T("1111_21");
	st_Buffer_info.strBufferSerial[1][22] = _T("1111_22");
	st_Buffer_info.strBufferSerial[1][23] = _T("1111_23");
	st_Buffer_info.strBufferSerial[1][24] = _T("1111_24");
	st_Buffer_info.strBufferSerial[1][25] = _T("1111_25");
	st_Buffer_info.strBufferSerial[1][26] = _T("1111_26");
	st_Buffer_info.strBufferSerial[1][27] = _T("1111_27");
	st_Buffer_info.strBufferSerial[1][28] = _T("1111_28");
	st_Buffer_info.strBufferSerial[1][29] = _T("1111_29");
	st_Buffer_info.strBufferSerial[1][30] = _T("1111_30");
	st_Buffer_info.strBufferSerial[1][31] = _T("1111_31");
	st_Buffer_info.strBufferSerial[1][32] = _T("1111_32");
	st_Buffer_info.strBufferSerial[1][33] = _T("1111_33");
	st_Buffer_info.strBufferSerial[1][34] = _T("1111_34");
	st_Buffer_info.strBufferSerial[1][35] = _T("1111_35");
	st_Buffer_info.strBufferSerial[1][36] = _T("1111_36");
	st_Buffer_info.strBufferSerial[1][37] = _T("1111_37");
	st_Buffer_info.strBufferSerial[1][38] = _T("1111_38");
	return;
	//st_sync_info.nSmema_Front = CTL_READY;
}


void CScreenMain::OnBnClickedBtnOutPosUpDw3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(st_Buffer_info.nBufferData[0][37][EXIST] == YES)
		st_Buffer_info.nBufferData[0][37][BIN] = FAIL; 
//	st_Buffer_info.nBufferData[0][37][EXIST] = YES;
// 	st_Buffer_info.nBufferData[0][37][X_POS] = 0;
// 	st_Buffer_info.nBufferData[0][37][Y_POS] = 1;
//	st_Buffer_info.nBufferData[0][37][FAILPICK] = YES;

	if(st_Buffer_info.nBufferData[1][38][EXIST] == YES)
		st_Buffer_info.nBufferData[1][38][BIN] = FAIL; 
//	st_Buffer_info.nBufferData[1][38][EXIST] = YES;
// 	st_Buffer_info.nBufferData[1][38][X_POS] = 1;
// 	st_Buffer_info.nBufferData[1][38][Y_POS] = 1;
//	st_Buffer_info.nBufferData[1][38][FAILPICK] = YES;
	if(st_Buffer_info.nBufferData[0][36][EXIST] == YES)
		st_Buffer_info.nBufferData[0][36][BIN] = FAIL; 
	return;
	st_sync_info.nSmema_Front = CTL_COMPLETE;
}


void CScreenMain::OnBnClickedBtnCvInPos3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//kwlee 2017.0209 test..
	st_handler_info.nRunStatus = dSTOP;
	return;
	///
	st_sync_info.nSmema_Rear = CTL_REQ;
	clsRunConveyor.m_nRunStep[CONV_OUT] = 1300;
}


void CScreenMain::OnBnClickedBtnOutPosUpDw5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	st_sync_info.nSmema_Rear = CTL_COMPLETE;
}


void CScreenMain::OnBnClickedBtnOutPosUpDw4()
{
	st_Picker_info.nPickerData[0][0][FAILPICK] = YES;
	st_Picker_info.nPickerData[0][3][FAILPICK] = YES;
	st_Picker_info.nPickerData[1][1][FAILPICK] = YES;
	st_Picker_info.nPickerData[1][2][FAILPICK] = YES;
}
