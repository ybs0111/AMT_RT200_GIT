// DialogManualIO.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Handler.h"
#include "DialogManualIO.h"
#include "afxdialogex.h"
#include "Variable.h"
#include "PublicFunction.h"
#include "Dialog_KeyBoard.h"
#include "Dialog_KeyPad.h"
#include "Dialog_Message.h"
#include "Ctlbd_Variable.h"
#include "Dialog_Password_Change.h"
#include "ComizoaPublic.h"
#include "FastechPublic_IO.h"
#include "AlgMemory.h"
#include "RunRobot.h"
#include "RunConveyor.h"

// CDialogManualIO 대화 상자입니다.
#define TM_IO		100
IMPLEMENT_DYNAMIC(CDialogManualIO, CDialog)

CDialogManualIO::CDialogManualIO(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogManualIO::IDD, pParent)
{
	m_bmGround.LoadBitmap(IDB_BITMAP_BACK_GROUND);   

	m_nManualWaitTime = 5000;
	m_nManualStep = 0;
	mn_Retry = 0;
	m_nManualActionCount[0]	= 1;
	m_nManualActionCount[1]	= 0;
	m_dPitch_x = 0;
	m_dPitch_Y = 0;

// 	for (int i =0; i<PICKCNT; i++ )
// 	{
// 		clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_NO;
// 	}

	m_lpAxisNum[0] = M_PICKERRBT_Y;  
	m_lpAxisNum[1] = M_PICKERRBT_X;
	m_lpAxisNum[2] = M_PICKERRBT_Z;
	m_nLinearMove_Index	= 0;   //현재 map index
	m_lAxisCnt	= 2; 
	m_bSecondPos = false;
	m_nPlaceCnt = 0;
}

CDialogManualIO::~CDialogManualIO()
{
}

void CDialogManualIO::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	// 	DDX_Control(pDX, IDC_CUSTOM_IO_FUNCTION_TSITE,		m_pGridTsite);
	// 	DDX_Control(pDX, IDC_CUSTOM_IO_FUNCTION_TSITE_RBT,	m_pGridTRbt);
	// 	DDX_Control(pDX, IDC_CUSTOM_IO_FUNCTION_LD_RBT,		m_pGridLdRbt);
	// 	DDX_Control(pDX, IDC_CUSTOM_IO_FUNCTION_ULD_RBT,	m_pGridUldRbt);
	// 	DDX_Control(pDX, IDC_CUSTOM_IO_FUNCTION_LF_CONTACT,	m_pGridLfContact);
	// 	DDX_Control(pDX, IDC_CUSTOM_IO_FUNCTION_RI_CONTACT,	m_pGridRiContact);
	// 	DDX_Control(pDX, IDC_CUSTOM_IO_FUNCTION_BUFFER,		m_pGridBuf);
	// 	DDX_Control(pDX, IDC_BTN_EXIT,						m_btnExit);
	// 	DDX_Control(pDX, IDC_BTN_REFRESH,					m_btnDoor);
	// 	DDX_Control(pDX, IDC_BTN_LEFT_CLAMP,				m_btnLeftMpClamp);
	// 	DDX_Control(pDX, IDC_BTN_LEFT_UNCLAMP,				m_btnLeftMpUnClamp);
	// 	DDX_Control(pDX, IDC_BTN_RIGHT_CLAMP,				m_btnRightMpClamp);
	// 	DDX_Control(pDX, IDC_BTN_RIGHT_UNCLAMP,				m_btnRightMpUnClamp);

	DDX_Control(pDX, IDC_WAIT_TIME, m_WaitTime);
	DDX_Control(pDX, IDC_CHECK_MANUAL_ACTION_1, m_btnManualAction1);
	DDX_Control(pDX, IDC_CHECK_MANUAL_ACTION_10, m_btnManualAction10);
	DDX_Control(pDX, IDC_CHECK_MANUAL_ACTION_100, m_btnManualAction100);
	DDX_Control(pDX, IDC_BTN_MANUAL_EMERGENCY, m_btnManualEmergency);
	DDX_Control(pDX, IDC_BTN_EXT, m_btnManualExit);

	DDX_Control(pDX, IDC_CUSTOM_RUNTIME, m_pGridRuntime);
	DDX_Control(pDX, IDC_BTN_PICK, m_btnPickUp);
	DDX_Control(pDX, IDC_BTN_MOVE, m_btnMove);
	DDX_Control(pDX, IDC_BTN_PLACE, m_btnPlace);
	DDX_Control(pDX, IDC_BTN_RELAY, m_btnRelay);
	DDX_Control(pDX, IDC_CUSTOM_RUNTIME2, m_pGridRobotRunTime);

	DDX_Control(pDX, IDC_GROUP_TURN_RUNTIME, m_groupTurnCovRuntime);
	DDX_Control(pDX, IDC_GROUP_PICKER_RUNTIME, m_groupPickerRuntime);
	DDX_Control(pDX, IDC_GROUP_TURN_CONVEYOR, m_group_TurnConveyor);
	DDX_Control(pDX, IDC_GROUP_ROBOT_PICKER, m_groupRbotPicker);
	DDX_Control(pDX, IDC_BTN_PICK_PLACE, m_btnPickerPlace);
	DDX_Control(pDX, IDC_PITCH_X, m_Pitch_x);
	DDX_Control(pDX, IDC_PITCH_Y, m_Pitch_Y);
	DDX_Control(pDX, IDC_MSG_X_PITCH, m_msgPitchX);
	DDX_Control(pDX, IDC_MSG_Y_PITCH, m_msgPitchY);
	DDX_Control(pDX, IDC_BTN_PICKER, m_btnPicker);
}


BEGIN_MESSAGE_MAP(CDialogManualIO, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()

	
	
	ON_BN_CLICKED(IDC_CHECK_MANUAL_ACTION_1, &CDialogManualIO::OnBnClickedCheckManualAction1)
	ON_BN_CLICKED(IDC_CHECK_MANUAL_ACTION_10, &CDialogManualIO::OnBnClickedCheckManualAction10)
	ON_BN_CLICKED(IDC_CHECK_MANUAL_ACTION_100, &CDialogManualIO::OnBnClickedCheckManualAction100)
	ON_BN_CLICKED(IDC_BTN_MANUAL_EMERGENCY, &CDialogManualIO::OnBnClickedBtnManualEmergency)
	ON_STN_CLICKED(IDC_WAIT_TIME, &CDialogManualIO::OnStnClickedWaitTime)
	ON_BN_CLICKED(IDC_BTN_EXT, &CDialogManualIO::OnBnClickedBtnExt)
	ON_BN_CLICKED(IDC_BTN_PICK, &CDialogManualIO::OnBnClickedBtnPick)
	ON_BN_CLICKED(IDC_BTN_MOVE, &CDialogManualIO::OnBnClickedBtnMove)
	ON_BN_CLICKED(IDC_BTN_PLACE, &CDialogManualIO::OnBnClickedBtnPlace)
	ON_BN_CLICKED(IDC_BTN_RELAY, &CDialogManualIO::OnBnClickedBtnRelay)
	ON_BN_CLICKED(IDC_BTN_PICK_PLACE, &CDialogManualIO::OnBnClickedBtnPickPlace)
	ON_BN_CLICKED(IDC_BTN_PICKER, &CDialogManualIO::OnBnClickedBtnPicker)
// 	ON_STN_CLICKED(IDC_PITCH_Y, &CDialogManualIO::OnStnClickedPitchY)
// 	ON_STN_CLICKED(IDC_PITCH_X, &CDialogManualIO::OnStnClickedPitchX)
	ON_BN_CLICKED(IDC_BTN_MANUAL_STOP, &CDialogManualIO::OnClickedBtnManualEmergency2)
END_MESSAGE_MAP()


// CDialogManualIO 메시지 처리기입니다.

BOOL CDialogManualIO::Create()
{
	return CDialog::Create( CDialogManualIO::IDD );
}

BOOL CDialogManualIO::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CDC  memDC;                     // 메모리 DC
	CBitmap * pOldBitmap = NULL;    // m_BackBitmap으로 새로운 그림을 DC에 그릴 때, 이전 그려졌던 DC(즉, Bitmap)을 저장.
	BITMAP bitmapInfo;              // 그림의 정보(m_BackBitmap)
	m_bmGround.GetBitmap(&bitmapInfo); // Bitmap 크기 구함.

	memDC.CreateCompatibleDC(pDC);  // 메모리 DC 생성
	pOldBitmap = memDC.SelectObject(&m_bmGround);
	pDC->BitBlt(0, 0, bitmapInfo.bmWidth, bitmapInfo.bmHeight, &memDC, 0, 0, SRCCOPY);   // 메모리 DC에 그림을 그림

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();

//	return CFormView::OnEraseBkgnd(pDC);
	return true;
}


BOOL CDialogManualIO::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
// 	KillTimer(100);
// 	KillTimer(200);
// 	KillTimer(300);
// 	KillTimer(400);
// 	KillTimer(500);
// 	KillTimer(600);
// 	KillTimer(700);
	return CDialog::DestroyWindow();
}


BOOL CDialogManualIO::OnInitDialog()
{
	CDialog::OnInitDialog();

	OnInitButton();
	OnInitDigit();
	OnInitGrid();
	OnInitGroup();
	OnInitLabel();
// 	OnInitGridTsite();
// 	OnInitGridTsiteRbt();
// 	OnInitGridLdRbt();
// 	OnInitGridUldRbt();
// 	OnInitGridLfContact();
// 	OnInitGridRiContact();
// 	OnInitGridBuffer();

	m_lpAxisNum[0] = M_PICKERRBT_Y;  
	m_lpAxisNum[1] = M_PICKERRBT_X;
	m_lpAxisNum[2] = M_PICKERRBT_Z;
	m_nLinearMove_Index	= 0;   //현재 map index
	m_lAxisCnt	= 2; 

 	SetTimer(TM_LABELFEEDER, 100, NULL);

	return TRUE;  
	
}


void CDialogManualIO::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int Ret;
	switch (nIDEvent)
	{
	case TMR_IO_CONTINUE:

		 Ret = OnCylinderExcution();

		 if (Ret == RET_GOOD)
		 {
			 KillTimer(TMR_IO_CONTINUE);		// 타이머 삭제

			 if (st_handler_info.nMenuLock)
			 {
				 st_handler_info.nMenuLock = FALSE;
			 }

			 m_nManualActionCount[0] = 1;
			 m_nManualActionCount[1] = 0;

			 m_nManualActionMode		= 0;
			 OnIoActionMode(m_nManualActionMode);
		 }
		 else if(Ret == RET_ERROR)
		 {
			 KillTimer(TMR_IO_CONTINUE);		// 타이머 삭제

			 if (st_handler_info.nMenuLock)
			 {
				 st_handler_info.nMenuLock = FALSE;
			 }

			 m_nManualActionCount[0] = 1;
			 m_nManualActionCount[1] = 0;

			 m_nManualActionMode		= 0;
			 OnIoActionMode(m_nManualActionMode);

			 UpdateData(FALSE);
		 }
		break;
		case TM_ROBOT_MOVE:
			Ret = OnRobotMove();

			if (Ret != RET_PROCEED)
			{
				KillTimer(TM_ROBOT_MOVE);

				st_handler_info.nMenuLock = FALSE;
			}
			break;
 
		case TM_ROBOT_RELAY_MOVE:
			Ret = OnRobotRelayMove();
			if (Ret != RET_PROCEED)
			{
				KillTimer(TM_ROBOT_RELAY_MOVE);

				st_handler_info.nMenuLock = FALSE;
			}
			break;
// 
		case TM_ROBOT_PICK_PLACE: 
			Ret =OnRobotPickPlace();

			if (Ret != RET_PROCEED)
			{
				KillTimer(TM_ROBOT_PICK_PLACE);
				st_handler_info.nMenuLock = FALSE;
			}
			break;


		case TM_ROBOT_PICK_UP:
			Ret =OnRobotPickUp();

			if (Ret != RET_PROCEED)
			{
				KillTimer(TM_ROBOT_PICK_UP);
				st_handler_info.nMenuLock = FALSE;
			}
			break;

		case TM_ROBOT_PICK_DN:
			Ret = OnRobotPickDn();

			if (Ret != RET_PROCEED)
			{
				KillTimer(TM_ROBOT_PICK_DN);
				st_handler_info.nMenuLock = FALSE;
			}
			break;
		case TM_PICKER:
			Ret = OnRunPicker();
			
			if (Ret != RET_PROCEED)
			{
				KillTimer(TM_PICKER);
				st_handler_info.nMenuLock = FALSE;
			}
			break;
// 
		case TM_LABELFEEDER:
			Ret = OnFeederInterface();

			if (Ret != RET_PROCEED)
			{
				KillTimer(TM_LABELFEEDER);
				st_handler_info.nMenuLock = FALSE;
			}
			break;
// 
// 		case 500:
// 			OnDisplayRightContact();
// 			break;
// 
// 		case 600:
// 			OnDisplayTestSite();
// 			break;
// 
// 		case 700:
// 			OnDisplayBuffer();
// 			break;
	}
	
	CDialog::OnTimer(nIDEvent);
}

	

void CDialogManualIO::OnInitButton()
{
	m_btnManualEmergency.SetBitmaps(IDC_BTN_RELOAD, IDB_BITMAP_EMERGENCY_DN, WINDOW_DN, IDB_BITMAP_EMERGENCY_UP, WINDOW_UP);
	m_btnManualEmergency.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnManualEmergency.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnManualEmergency.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnManualEmergency.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnManualEmergency.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnManualEmergency.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnManualEmergency.SetFont(clsFunc.m_pFont[3]);
	m_btnManualEmergency.SetTooltipText(_T("IO Action Emergency Stop"));

	m_btnManualAction1.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnManualAction1.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnManualAction1.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnManualAction1.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnManualAction1.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnManualAction1.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnManualAction1.SetFont(clsFunc.m_pFont[1]);

	m_btnManualAction10.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnManualAction10.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnManualAction10.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnManualAction10.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnManualAction10.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnManualAction10.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnManualAction10.SetFont(clsFunc.m_pFont[1]);

	m_btnManualAction100.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnManualAction100.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnManualAction100.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnManualAction100.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnManualAction100.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnManualAction100.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnManualAction100.SetFont(clsFunc.m_pFont[1]);

	m_btnManualExit.SetBitmaps(IDC_BTN_EXIT, IDB_BITMAP_EXIT, WINDOW_DN, IDB_BITMAP_EXIT, WINDOW_UP);
	m_btnManualExit.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnManualExit.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnManualExit.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnManualExit.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnManualExit.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnManualExit.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnManualExit.SetFont(clsFunc.m_pFont[3]);
	m_btnManualExit.SetTooltipText(_T("Exit"));

	///////////////////////////////////////////////////////////////////////////////


	m_btnRelay.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnRelay.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnRelay.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnRelay.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnRelay.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnRelay.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnRelay.SetFont(clsFunc.m_pFont[3]);
	m_btnRelay.SetTooltipText(_T("연속 동작"));
	
	m_btnPicker.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnPicker.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnPicker.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnPicker.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnPicker.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnPicker.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnPicker.SetFont(clsFunc.m_pFont[3]);
	m_btnPicker.SetTooltipText(_T("Picker"));

	m_btnMove.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnMove.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnMove.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnMove.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnMove.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnMove.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnMove.SetFont(clsFunc.m_pFont[3]);
	m_btnMove.SetTooltipText(_T("Move"));

	
	m_btnPickUp.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnPickUp.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnPickUp.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnPickUp.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnPickUp.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnPickUp.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnPickUp.SetFont(clsFunc.m_pFont[3]);
	m_btnPickUp.SetTooltipText(_T("Pick Up"));

	m_btnPickerPlace.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnPickerPlace.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnPickerPlace.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnPickerPlace.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnPickerPlace.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnPickerPlace.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnPickerPlace.SetFont(clsFunc.m_pFont[3]);
	m_btnPickerPlace.SetTooltipText(_T("Pick Dw"));
	
	m_btnPlace.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnPlace.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnPlace.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnPlace.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnPlace.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnPlace.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnPlace.SetFont(clsFunc.m_pFont[3]);
	m_btnPlace.SetTooltipText(_T("Place"));

	OnIoActionMode(0);
}
void CDialogManualIO::OnInitDigit()
{
	m_WaitTime.SetStyle(CDigit::DS_INT, 7, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	m_WaitTime.SetVal(m_nManualWaitTime);

	m_Pitch_x.SetStyle(CDigit::DS_INT, 6, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	//m_Pitch_x.SetVal(m_dPitch_x); 
										 
	m_Pitch_Y.SetStyle(CDigit::DS_INT, 6, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	//m_Pitch_Y.SetVal(m_dPitch_Y);
}
void CDialogManualIO::OnInitLabel()
{
	m_msgPitchX.SetFont(clsFunc.m_pFont[5]);
	m_msgPitchX.SetWindowText(_T("Label Pitch X"));
	m_msgPitchX.SetCenterText();
	m_msgPitchX.SetColor(WHITE_C);
	m_msgPitchX.SetGradientColor(GREEN_C);
	m_msgPitchX.SetTextColor(BLACK_C);

	m_msgPitchY.SetFont(clsFunc.m_pFont[5]);
	m_msgPitchY.SetWindowText(_T("Label Pitch Y"));
	m_msgPitchY.SetCenterText();
	m_msgPitchY.SetColor(WHITE_C);
	m_msgPitchY.SetGradientColor(GREEN_C);
	m_msgPitchY.SetTextColor(BLACK_C);

}

void CDialogManualIO::OnInitGrid()
{
	OnTurnConvRuntTimeGrid();
	OnPickerRunTimeGrid();
}

int CDialogManualIO::OnCylinderExcution()
{
	CString strMsg,str;
	int nRet = RET_PROCEED, i=0;
//	int nRect[2];
	
	// **************************************************************************
	// 모터 동작 중 ESTOP 요청 발생했는지 검사한다                               
	// -> ESTOP 요청 발생한 경우 긴급 정지로 모터 멈춘다                         
	// **************************************************************************
	if(m_bEmergencyStop)
	{
		if (m_nManualStep < 1000)
		{
			m_nManualStep = 1000;
		}
	}
	// **************************************************************************


	switch (m_nManualStep)
	{
	case 0:
		FAS_IO.set_out_bit(st_io_info.o_TableTurnCylFor, IO_ON);
		FAS_IO.set_out_bit(st_io_info.o_TableTurnCylBack, IO_OFF);
		m_lWaitTime[0]	= GetTickCount();	
		m_nManualStep = 100;
		break;
		
	case 100:
		//nRect[0] = FAS_IO.get_in_bit(st_io_info.i_TurnRotatCylForChk,IO_ON);
		//nRect[1] = FAS_IO.get_in_bit(st_io_info.i_TurnRotatCylBackChk,IO_OFF);

		if (FAS_IO.get_in_bit(st_io_info.i_TurnRotatCylForChk,IO_ON) == IO_ON && 
			FAS_IO.get_in_bit(st_io_info.i_TurnRotatCylBackChk,IO_OFF) == IO_OFF)
		{
			m_lWaitTime[0] = GetTickCount();
			m_nManualStep = 200;
		}
		else
		{
			m_lWaitTime[1] = GetTickCount();

			m_lWaitTime[2] = m_lWaitTime[1] - m_lWaitTime[0];

			if (m_lWaitTime[2] < 0)
			{
				m_lWaitTime[0] = GetTickCount();
				break;
			}
			//		st_wait_inf0o.nLimitWaitTime
			if (m_lWaitTime[2] > st_wait_info.dLimitWaitTime[WAIT_CONV_TURN])
			{
				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
				{
					strMsg = _T("Please Cylinder State is Abnomal");
					//clsFunc.OnStringToChar(strMsg, st_other_info.cNormalMsg);
					clsMem.OnAbNormalMessagWrite(strMsg);
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
					m_nManualStep = 1000;
				}
			}
		}
		break;

	case 200:
		m_lWaitTime[1] = GetTickCount();
		m_lWaitTime[2] = m_lWaitTime[1] - m_lWaitTime[0];
		if (m_lWaitTime[2] < 0)
		{
			m_lWaitTime[0] = GetTickCount();
			break;
		}
		if (m_lWaitTime[2] > st_wait_info.dOnWaitTime[WAIT_CONV_TURN])
		{
			m_nManualStep = 300;
		}
		break;

	case 300:
		FAS_IO.set_out_bit(st_io_info.o_TableTurnCylFor, IO_OFF);
		FAS_IO.set_out_bit(st_io_info.o_TableTurnCylBack, IO_ON);
		m_lWaitTime[0]	= GetTickCount();	
		m_nManualStep = 400;
		break;

	case 400:

		//nRect[0] = FAS_IO.get_in_bit(st_io_info.i_TurnRotatCylForChk,IO_OFF);
		//nRect[1] = FAS_IO.get_in_bit(st_io_info.i_TurnRotatCylBackChk,IO_ON);

		if (FAS_IO.get_in_bit(st_io_info.i_TurnRotatCylForChk,IO_OFF) == IO_OFF && 
			FAS_IO.get_in_bit(st_io_info.i_TurnRotatCylBackChk,IO_ON) == IO_ON)
		{
			m_nManualStep = 500;
		}
		else
		{
			m_lWaitTime[1] = GetTickCount();

			m_lWaitTime[2] = m_lWaitTime[1] - m_lWaitTime[0];

			if (m_lWaitTime[2] < 0)
			{
				m_lWaitTime[0] = GetTickCount();
				break;
			}
			//		st_wait_info.nLimitWaitTime
			if (m_lWaitTime[2] > st_wait_info.dLimitWaitTime[WAIT_CONV_TURN])
			{
				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
				{
					strMsg = _T("Please Cylinder State is Abnomal");
					//clsFunc.OnStringToChar(strMsg, st_other_info.cNormalMsg);
					clsMem.OnAbNormalMessagWrite(strMsg);
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
					m_nManualStep = 1000;
				}

			}
		}
		break;

	case 500:
		m_lWaitTime[1] = GetTickCount();
		m_lWaitTime[2] = m_lWaitTime[1] - m_lWaitTime[0];
		if (m_lWaitTime[2] < 0)
		{
			m_lWaitTime[0] = GetTickCount();
			break;
		}
		if (m_lWaitTime[2] > st_wait_info.dOnWaitTime[WAIT_CONV_TURN])
		{
			m_nManualStep = 600;
		}
		break;	

	case 600:	
		m_nManualActionCount[1]++;		

		if(m_nManualActionCount[1] >= m_nManualActionCount[0])
		{
			m_nManualStep = 700;
		}
		else
		{
			m_nManualStep = 0;
		}
		break;

	case 700:
		if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
		{
			strMsg = _T("[Repeat action] The action was completed.!");

			//clsFunc.OnStringToChar(strMsg, st_other_info.cNormalMsg);
			clsMem.OnNormalMessageWrite(strMsg);
			st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			m_dwTimeCheck[1] = GetCurrentTime();

			m_dwTimeCheck[2] = m_dwTimeCheck[1] - m_dwTimeCheck[0]; 
			
			str.Format(_T("%3.2f"),((float)m_dwTimeCheck[2]/(long)1000));
			m_pGridRuntime.SetItemText(0, 1,str);
			m_pGridRuntime.Invalidate(FALSE);
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

		if(m_bEmergencyStop)
		{
			m_bEmergencyStop = FALSE;  // ESTOP 요청 플래그 초기화
		}
		break;
	}

	return nRet;
}


void CDialogManualIO::OnIoActionMode(int nMode)
{
	m_nManualActionMode = nMode;
	switch (nMode)
	{
	case 0:
		m_btnManualAction1.SetCheck(TRUE);
		m_btnManualAction10.SetCheck(FALSE);
		m_btnManualAction100.SetCheck(FALSE);
		break;

	case 1:
		m_btnManualAction1.SetCheck(FALSE);
		m_btnManualAction10.SetCheck(TRUE);
		m_btnManualAction100.SetCheck(FALSE);
		break;

	case 2:
		m_btnManualAction1.SetCheck(FALSE);
		m_btnManualAction10.SetCheck(FALSE);
		m_btnManualAction100.SetCheck(TRUE);
		break;
	}

	UpdateData(FALSE);
}



void CDialogManualIO::OnBnClickedCheckManualAction1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nManualActionCount[0]	= 1;
	m_nManualActionCount[1]	= 0;
	m_bEmergencyStop		= FALSE;
	m_nManualStep = 0;
	
	//	dw_timeer = GetCurrentTime();
	m_dwTimeCheck[0] = GetCurrentTime();

	SetTimer(TMR_IO_CONTINUE,100, NULL);
	OnIoActionMode(0);
}


void CDialogManualIO::OnBnClickedCheckManualAction10()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nManualActionCount[0]	= 10;
	m_nManualActionCount[1]	= 0;
	m_bEmergencyStop		= FALSE;
	m_dwTimeCheck[0] = GetCurrentTime();
	m_nManualStep = 0;
	SetTimer(TMR_IO_CONTINUE,100, NULL);
	
	OnIoActionMode(1);
}
// int CDialogManualIO::OnRobotPickPlace()
// {
// 
// 
// }

int CDialogManualIO::OnRobotPickUp()
{
	int nRet_1, nRet_2=0, nRet_3=0, nRect[12]={0,};
//	int nRet;

	double d_CurPos[30] = {0,};
	double d_Pos[30] = {0,};

	if( m_bEmergencyStop == true )
	{
		m_RunPickUpStep = 0;
		return RET_ERROR;
	}

	switch (m_RunPickUpStep)
	{
	case 0:
		d_CurPos[0] = COMI.Get_MotCurrentPos(M_PICKERRBT_X);
		d_CurPos[1] = COMI.Get_MotCurrentPos(M_PICKERRBT_Y);

		if(d_CurPos[0] > st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_FIRST] + st_motor_info[M_PICKERRBT_X].d_allow &&
			d_CurPos[1] > st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST] + st_motor_info[M_PICKERRBT_Y].d_allow)
		{
			m_strAlarmCode.Format(_T("%02d0008"), M_PICKERRBT_Z); //000008 0 A "MOTOR MOVE SAFETY CHECK ERROR -[M_ROBOT_X]."
			CTL_Lib.Alarm_Error_Occurrence(100, dMOTOR_WARNING, m_strAlarmCode);//dMOTOR_WARNING ==> 이때는 알람은 발생시키지 않는다(run status를 바꾸지 않는다).thread 에서만 판단하여 알람 발생시킨다 

			if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
			{
				clsMem.OnNormalMessageWrite(_T("Robot Reject Place Position Error"));
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
			return BD_ERROR;
		}
		else
		{
			m_RunPickUpStep = 100;
		}
		break;




	case 100:
		//vaccum On
		for (int i = 0; i < PICKCNT; i++)
		{
			FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[i],IO_ON);
		}
		m_RunPickUpStep = 200;
		break;

	case 200:
		
		for (int i = 0; i < PICKCNT; i++)
		{
			nRect[i] = FAS_IO.get_out_bit(st_io_info.o_HeadVaccum[i],IO_ON);
			if (nRect[i] == IO_OFF)
			{
				//505080 1 A "Head Vaccum #1 Off Check Error ."
				//505081 1 A "Head Vaccum #1 On Check Error ."
				m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.o_HeadVaccum[i], IO_OFF);
				CTL_Lib.Alarm_Error_Occurrence(402, dWARNING, m_strAlarmCode);
				m_RunPickUpStep = 0;
				break;
			}
			else
			{ 
				if (i< PICKCNT -1)
				{
					continue;
				}						
				m_RunPickUpStep = 1000;				
							
			}
		}
		break;

	case 1000:
		//picker up
		for (int i =0; i<PICKCNT; i++ )
		{
			clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_YES;
		}

		clsRunRobot.OnSetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);

		m_RunPickUpStep = 2000;

		break;

	case 2000:	
		//z축 up
		
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_Z, st_motor_info[M_PICKERRBT_Z].d_pos[ROBOT_SAFETY], COMI.mn_runspeed_rate);

		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			m_RunPickUpStep = 3000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(404, dWARNING, st_alarm_info.strCode);
			m_RunPickUpStep = 0;
		}
		break;



	case 3000:
		// picker up 체크
		nRet_1 = clsRunRobot.OnGetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);

		if(nRet_1 == RET_GOOD)
		{
			m_RunPickUpStep = 4000;
		}
		else if(nRet_1 == RET_ERROR)
		{
			//	m_RunRelayMoveStep = 0;

			CTL_Lib.Alarm_Error_Occurrence(401, dWARNING, m_strAlarmCode);
		}
		break;



	case 4000:
		for (int i = 0; i < PICKCNT; i++)
		{

			if (FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[i],IO_ON) == IO_OFF)
			{
				//505080 1 A "Head Vaccum #1 Off Check Error ."
				//505081 1 A "Head Vaccum #1 On Check Error ."
				
				m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_HeadVaccumCheck[i], IO_OFF);
				CTL_Lib.Alarm_Error_Occurrence(402, dWARNING, m_strAlarmCode);

				break;
			}
			else
			{ 
				if (i< PICKCNT -1)
				{
					continue;
				}	

				m_RunPickUpStep = 0;
				return RET_GOOD;
							
			}
		}
		break;

	}
	return RET_PROCEED;
}

int CDialogManualIO::OnRobotPickDn()
{
	int nRet_1, nRet_2=0, nRet_3=0, nRect[12]={0,};
//	int nRet;

	double d_CurPos[30] = {0,};
	double d_Pos[30] = {0,};

	if( m_bEmergencyStop == true )
	{
		m_RunPickDnStep = 0;
		return RET_ERROR;
	}

	switch (m_RunPickDnStep)
	{
	case 0:
		d_CurPos[0] = COMI.Get_MotCurrentPos(M_PICKERRBT_X);
		d_CurPos[1] = COMI.Get_MotCurrentPos(M_PICKERRBT_Y);

		if(d_CurPos[0] > st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_FIRST] + st_motor_info[M_PICKERRBT_X].d_allow &&
			d_CurPos[1] > st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_X_FEEDER_PICK_FIRST] + st_motor_info[M_PICKERRBT_Y].d_allow)
		{
			m_strAlarmCode.Format(_T("%02d0008"), M_PICKERRBT_Z); //000008 0 A "MOTOR MOVE SAFETY CHECK ERROR -[M_ROBOT_X]."
			CTL_Lib.Alarm_Error_Occurrence(100, dMOTOR_WARNING, m_strAlarmCode);//dMOTOR_WARNING ==> 이때는 알람은 발생시키지 않는다(run status를 바꾸지 않는다).thread 에서만 판단하여 알람 발생시킨다 

			if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
			{
				clsMem.OnNormalMessageWrite(_T("Robot Reject Place Position Error"));
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
			return BD_ERROR;
		}
		else
		{
			m_RunPickDnStep = 100;
		}
		break;

	case 100:
		//vaccum On
		for (int i = 0; i < PICKCNT; i++)
		{
			FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[i],IO_ON);
		}
		m_RunPickDnStep = 200;
		break;

	case 200:

		for (int i = 0; i < PICKCNT; i++)
		{
			nRect[i] = FAS_IO.get_out_bit(st_io_info.o_HeadVaccum[i],IO_ON);
			if (nRect[i] == IO_OFF)
			{
				//505080 1 A "Head Vaccum #1 Off Check Error ."
				//505081 1 A "Head Vaccum #1 On Check Error ."
				m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.o_HeadVaccum[i], IO_OFF);

				
				CTL_Lib.Alarm_Error_Occurrence(402, dWARNING, m_strAlarmCode);
				m_RunPickDnStep = 0;
				break;
			}
			else
			{ 
				if (i< PICKCNT -1)
				{
					continue;
				}						
				m_RunPickDnStep = 1000;				
							
			}
		}
		break;

	case 1000:
		for (int i =0; i<PICKCNT; i++ )
		{
			clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_YES;
		}
		//picker down
		clsRunRobot.OnSetPickerUpDn(0, PICKER_DN, clsRunRobot.m_npTemp_Picker_YesNo);

		m_RunPickDnStep = 2000;
		break;

	case 2000:	
		//z축 내려서 label pick 위치 이동
		
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_Z, st_motor_info[M_PICKERRBT_Z].d_pos[ROBOT_Z_FEEDER_PICK], COMI.mn_runspeed_rate);

		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			m_RunPickDnStep = 3000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(404, dWARNING, st_alarm_info.strCode);
			m_RunPickDnStep = 0;
		}
		break;

	case 3000:
		//picker down
		nRet_1 = clsRunRobot.OnGetPickerUpDn(0, PICKER_DN, clsRunRobot.m_npTemp_Picker_YesNo);

		if(nRet_1 == RET_GOOD)
		{
			m_RunPickDnStep = 0;
			return RET_GOOD;
		}
		else if(nRet_1 == RET_ERROR)
		{
			//	m_RunRelayMoveStep = 0;

			CTL_Lib.Alarm_Error_Occurrence(401, dWARNING, m_strAlarmCode);
		}
		break;


	

	}
	return RET_PROCEED;
}

int CDialogManualIO::OnRobotPickPlace()
{
	int nRet_1, nRet_2=0, nRet_3=0, nRect[12]={0,};
//	int nRet;

	double d_CurPos[30] = {0,};
	double d_Pos[30] = {0,};

	if( m_bEmergencyStop == true )
	{
		m_RunPickPlaceStep = 0;
		return RET_ERROR;
	}

	switch (m_RunPickPlaceStep)
	{

	case 0:
		
		 nRet_1 = COMI.Check_MotPosRange(M_PICKERRBT_X,st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_FIRST],st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_FIRST] + st_motor_info[M_PICKERRBT_X].d_allow);
		 nRet_2 = COMI.Check_MotPosRange(M_PICKERRBT_Y, st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_FIRST],st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_FIRST] + st_motor_info[M_PICKERRBT_Y].d_allow);

		 if (nRet_1 == BD_GOOD && nRet_2 == BD_GOOD)
		 {
			 m_RunPickPlaceStep = 100;
		 }
		 else
		 {
			 m_strAlarmCode.Format(_T("%02d0008"), M_PICKERRBT_Z); //000008 0 A "MOTOR MOVE SAFETY CHECK ERROR -[M_ROBOT_X]."
			 CTL_Lib.Alarm_Error_Occurrence(100, dMOTOR_WARNING, m_strAlarmCode);//dMOTOR_WARNING ==> 이때는 알람은 발생시키지 않는다(run status를 바꾸지 않는다).thread 에서만 판단하여 알람 발생시킨다 

			 if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
			 {
				 clsMem.OnNormalMessageWrite(_T("Robot Reject Place Position Error"));
				 st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			 }
			 return BD_ERROR;
		 }
		break;

	
		//////////////////////////////
	case 100:	
		//ROBOT TURN
		
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_TURN, st_motor_info[M_PICKERRBT_TURN].d_pos[PITCH_UP_HEAD_TURN_ROTATOR_R_90], COMI.mn_runspeed_rate);

		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			m_RunPickPlaceStep = 110;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(401, dWARNING, st_alarm_info.strCode);
			m_RunPickPlaceStep = 0;
		}
		break;
		
	case 110:	
		////ROBOT PITCH 작업 위치 이동
		
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_PITCH, st_motor_info[M_PICKERRBT_PITCH].d_pos[PITCH_UP_HEAD_PLACE_PCB], COMI.mn_runspeed_rate);

		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			m_RunPickPlaceStep = 120;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(401, dWARNING, st_alarm_info.strCode);
			m_RunPickPlaceStep = 0;
		}
		break;

	case 120:
		for (int i =0; i<PICKCNT; i++ )
		{
			clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_YES;
		}
		//picker down
		clsRunRobot.OnSetPickerUpDn(0, PICKER_DN, clsRunRobot.m_npTemp_Picker_YesNo);

		m_RunPickPlaceStep = 200;
		break;

	case 200:	
		//z축 내려서 label pick 위치 이동
		
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_Z, st_motor_info[M_PICKERRBT_Z].d_pos[ROBOT_Z_PLACE], COMI.mn_runspeed_rate);

		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			m_RunPickPlaceStep = 1000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(404, dWARNING, st_alarm_info.strCode);
			m_RunPickPlaceStep = 0;
		}
		break;

	case 1000:
		//picker down
		nRet_1 = clsRunRobot.OnGetPickerUpDn(0, PICKER_DN, clsRunRobot.m_npTemp_Picker_YesNo);

		if(nRet_1 == RET_GOOD)
		{
			m_RunPickPlaceStep = 2000;
		}
		else if(nRet_1 == RET_ERROR)
		{
			//	m_RunRelayMoveStep = 0;

			CTL_Lib.Alarm_Error_Occurrence(401, dWARNING, m_strAlarmCode);
		}
		break;


	case 2000:
		
		for (int i = 0; i < PICKCNT; i++)
		{
			FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[i],IO_OFF);
		}
		m_RunPickPlaceStep = 3000;
		break;

	case 3000:
		//label 부착 확인
		for (int i = 0; i < PICKCNT; i++)
		{
			nRect[i] = FAS_IO.get_out_bit(st_io_info.o_HeadVaccum[i],IO_OFF);
			if (nRect[i] == IO_ON)
			{
				//505080 1 A "Head Vaccum #1 Off Check Error ."
				//505081 1 A "Head Vaccum #1 On Check Error ."
				m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.o_HeadVaccum[i], IO_ON);
				CTL_Lib.Alarm_Error_Occurrence(402, dWARNING, m_strAlarmCode);
				m_RunPickPlaceStep = 0;
				
			}
			else
			{ 
				if (i< PICKCNT -1)
				{
					continue;
				}						
				m_RunPickPlaceStep = 4000;							
			}
		}
		break;

	case 4000:
		//picker up
		for (int i =0; i<PICKCNT; i++ )
		{
			clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_YES;
		}

		clsRunRobot.OnSetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);

		m_RunPickPlaceStep = 5000;

		break;

	case 5000:	
		//z축 up

		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_Z, st_motor_info[M_PICKERRBT_Z].d_pos[ROBOT_SAFETY], COMI.mn_runspeed_rate);

		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			m_RunPickPlaceStep = 6000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(404, dWARNING, st_alarm_info.strCode);
			m_RunPickPlaceStep = 0;
		}
		break;



	case 6000:
		// picker up 체크
		nRet_1 = clsRunRobot.OnGetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);

		if(nRet_1 == RET_GOOD)
		{
			m_RunPickPlaceStep = 7000;
		}
		else if(nRet_1 == RET_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(401, dWARNING, m_strAlarmCode);
		}
		break;

	case 7000:
		for (int i = 0; i < PICKCNT; i++)
		{
			if (FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[i],IO_OFF) == IO_ON)
			{
				//505080 1 A "Head Vaccum #1 Off Check Error ."
				//505081 1 A "Head Vaccum #1 On Check Error ."
				m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_HeadVaccumCheck[i], IO_OFF);
				CTL_Lib.Alarm_Error_Occurrence(402, dWARNING, m_strAlarmCode);

				break;
			}
			else
			{ 
				if (i< PICKCNT -1)
				{
					continue;
				}	

				m_RunPickPlaceStep = 0;				

				return RET_GOOD;
			}
		}
		break;

	}
	return RET_PROCEED;
}
int CDialogManualIO::OnRobotRelayMove()
{
	int nRet_1, nRet_2=0, nRet_3=0, nRect[12]={0,};
	int nRet;
	double dCurrentPos = 0;
	CString str;
	
	//m_nPickCnt = st_basic_info.nColCnt * st_basic_info.nRowCnt;
	//m_nPickCnt = 6;
	if( m_bEmergencyStop == true )
	{
		m_RunRelayMoveStep = 0;
		return RET_ERROR;
	}

	if (FAS_IO.get_in_bit(st_io_info.i_LfAlarmChk,IO_ON) == IO_ON)
	{	
		//507151 1 A "Label Feeder LF_ALARM On Check Error."
		m_strAlarmCode.Format(_T("507151"));
		CTL_Lib.Alarm_Error_Occurrence(404, dWARNING, m_strAlarmCode);
		m_RunRelayMoveStep = 0;

		return RET_ERROR;
	}

	switch (m_RunRelayMoveStep)
	{
		case 0:
			//picker vaccum off
			for (int i = 0; i < PICKCNT; i++)
			{
				FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[i],IO_OFF);

				//if (nRect[i] == IO_ON)
				//vaccum 확인 
				if (FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[i],IO_OFF) == IO_ON)
				{
					//505080 1 A "Head Vaccum #1 Off Check Error ."
					//505081 1 A "Head Vaccum #1 On Check Error ."
					m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_HeadVaccumCheck[i], IO_ON);
					CTL_Lib.Alarm_Error_Occurrence(402, dWARNING, m_strAlarmCode);
					m_RunRelayMoveStep = 0;
					break;
				}
				else
				{ 
					if (i< PICKCNT -1)
					{
						continue;
					}						
					m_RunRelayMoveStep = 100;								
				}
			}
			break;

		case 100:
			// loader robot picker up 요청
			for (int i =0; i<PICKCNT; i++ )
			{
				clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_YES;
			}

			//picker up
			clsRunRobot.OnSetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);
			m_RunRelayMoveStep = 200;
			break;

		case 200:	
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_Z, st_motor_info[M_PICKERRBT_Z].d_pos[ROBOT_SAFETY], COMI.mn_manualspeed_rate);
			//z 축 up
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_RunRelayMoveStep = 300;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(404, dWARNING, st_alarm_info.strCode);
				m_RunRelayMoveStep = 0;
			}
			break;

			//////////////////////////////
		case 300:	
			//ROBOT TURN
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_TURN, st_motor_info[M_PICKERRBT_TURN].d_pos[PITCH_UP_HEAD_TURN_ROTATOR_0], COMI.mn_manualspeed_rate);

			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_RunRelayMoveStep = 400;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(401, dWARNING, st_alarm_info.strCode);
				m_RunRelayMoveStep = 0;
			}
			break;

		case 400:	
			////ROBOT PITCH 작업 위치 이동
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_PITCH, st_motor_info[M_PICKERRBT_PITCH].d_pos[PITCH_UP_HEAD_PICK_FEEDER], COMI.mn_manualspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_RunRelayMoveStep = 1000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(401, dWARNING, st_alarm_info.strCode);
				m_RunRelayMoveStep = 0;
			}
			break;
			/////////////////////////////////////////////////////////////////////////////////////////

		case 1000:
			// picker up 체크
			nRet_1 = clsRunRobot.OnGetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);

			if(nRet_1 == RET_GOOD)
			{
				m_RunRelayMoveStep = 2000;
			}
			else if(nRet_1 == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(401, dWARNING, m_strAlarmCode);
			}
			break;

		case 2000:

			m_dPitch_Y = (st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST] - st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_END])/5;
			m_dPitch_x = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_FIRST] - st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_END];

// 			m_dpTargetPosList[0] = st_motor_info[m_nRobot_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST];
// 			m_dpTargetPosList[1] = st_motor_info[m_nRobot_X].d_pos[ROBOT_X_FEEDER_PICK_FIRST];

			if( m_nPickerNum < 6)
			{
				m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST] - (m_nPickerNum*m_dPitch_Y);
				m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_FIRST];
			}
			else
			{
				m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST] - ((PICKCNT- 1 - m_nPickerNum) * m_dPitch_Y);
				//m_dpTargetPosList[0] = st_motor_info[m_nRobot_Y].d_pos[ROBOT_Y_FEEDER_PICK_END] - ((m_nPickerNum-6) * m_dPitch_Y);
				m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_END];
			}

 			m_lpAxisNum[0] = M_PICKERRBT_Y; //m_lpAxisNum[4];        //현재의 IndexNum에서 사용하는 모터 실재 번호를 가진다 
 			m_lpAxisNum[1] = M_PICKERRBT_X;

			//label pick 위치 이동
			nRet_1 = CTL_Lib.Linear_Move(ONLY_MOVE_START, m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, COMI.mn_manualspeed_rate);

			if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
			{
				m_RunRelayMoveStep = 3000;
			}
			else if(nRet_1 == BD_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(409, dWARNING, st_alarm_info.strCode);
				m_RunRelayMoveStep = 0;
			}
			break;

		case 3000:
			//label pick 위치 확인
			nRet = CTL_Lib.Linear_Move(ONLY_MOVE_CHECK, m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, COMI.mn_manualspeed_rate);
			if(nRet == BD_GOOD) //정상적으로 완료된 상태 
			{						
				//m_RunMoveStep = 0;
				m_RunRelayMoveStep = 4000;

				/*return RET_GOOD;*/
			}
			else if (nRet == BD_RETRY)
			{
				m_RunRelayMoveStep = 2000;
			}
			else if (nRet == BD_ERROR || nRet == BD_SAFETY)
			{			
				m_RunRelayMoveStep = 2000;
				CTL_Lib.Alarm_Error_Occurrence(409, dWARNING, st_alarm_info.strCode);
			}
			break;
			
		case 4000:
			for (int i =0; i<PICKCNT; i++ )
			{
				clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_NO;
			}
			
			if( m_nPickerNum < 6)
			{
				//st_Pcb_info.nPcbSelect[i][j] = TRUE;
				clsRunRobot.m_npTemp_Picker_YesNo[m_nPickerNum] = CTL_YES;
				//picker down			
				clsRunRobot.OnSetPickerUpDn(0, PICKER_DN, clsRunRobot.m_npTemp_Picker_YesNo);
			}
			else
			{
				if(m_nPickCnt > (PICKCNT + 5 - m_nPickerNum)) 
				{
					clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT + 5 - m_nPickerNum] = CTL_YES;
					//picker down			
					clsRunRobot.OnSetPickerUpDn(0, PICKER_DN, clsRunRobot.m_npTemp_Picker_YesNo);
				}
				else
				{
					m_RunRelayMoveStep = 7500;
					break;
				}
			}
			m_RunRelayMoveStep = 5000;
			break;

		case 5000:
			//vaccum On
			if( m_nPickerNum < 6)
			{
				FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[m_nPickerNum],IO_ON);
			}
			else
			{
				//FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[PICKCNT + 5 - m_nPickerNum],IO_ON);
				if(m_nPickCnt > (PICKCNT + 5 - m_nPickerNum)) 
				{
					FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[PICKCNT + 5 - m_nPickerNum],IO_ON);
				}
				else
				{
					m_RunRelayMoveStep = 7500;
					break;
				}
			}
			m_RunRelayMoveStep = 6000;
			break;

		case 6000:	
			//z축 내려서 label pick 위치 이동

			//함수호출
			nRet_1 = COMI.Check_MotPosRange(M_PICKERRBT_X,st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_FIRST],st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_FIRST] + st_motor_info[M_PICKERRBT_X].d_allow);
			nRet_2 = COMI.Check_MotPosRange(M_PICKERRBT_Y, st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST],st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST] + st_motor_info[M_PICKERRBT_Y].d_allow);

			if (nRet_1 == BD_GOOD && nRet_2 == BD_GOOD)
			{
				nRet_3 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_Z, st_motor_info[M_PICKERRBT_Z].d_pos[ROBOT_FEEDER_PICK_FIRST], COMI.mn_manualspeed_rate);

				if (nRet_3 == BD_GOOD) //좌측으로 이동
				{
					m_RunRelayMoveStep = 7000;
				}
				else if (nRet_3 == BD_ERROR || nRet_3 == BD_SAFETY)
				{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
					CTL_Lib.Alarm_Error_Occurrence(406, dWARNING, st_alarm_info.strCode);
					m_RunRelayMoveStep = 0;
				}
			}
			else
			{
				m_strAlarmCode.Format(_T("%02d0006"), M_PICKERRBT_Z); //000008 0 A "MOTOR MOVE SAFETY CHECK ERROR -[M_ROBOT_X]."
				CTL_Lib.Alarm_Error_Occurrence(407, dMOTOR_WARNING, m_strAlarmCode);//dMOTOR_WARNING ==> 이때는 알람은 발생시키지 않는다(run status를 바꾸지 않는다).thread 에서만 판단하여 알람 발생시킨다 

				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
				{
					clsMem.OnNormalMessageWrite(_T("Robot Z Place Position Error"));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
				}
			}
			break;

		case 7000:
			//picker down
			if( m_nPickerNum < 6)
			{
				clsRunRobot.m_npTemp_Picker_YesNo[m_nPickerNum] = CTL_YES;
				nRet_1 = clsRunRobot.OnGetPickerUpDn(0, PICKER_DN,clsRunRobot.m_npTemp_Picker_YesNo);
			}
			else
			{
// 				clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT + 5 - m_nPickerNum] = CTL_YES;
// 				nRet_1 = clsRunRobot.OnGetPickerUpDn(0, PICKER_DN,clsRunRobot.m_npTemp_Picker_YesNo);
// 				if(m_nPickCnt > (PICKCNT + 5 - m_nPickerNum)) 
// 				{
					clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT + 5 - m_nPickerNum] = CTL_YES;
					//picker down			
					
					nRet_1 = clsRunRobot.OnGetPickerUpDn(0, PICKER_DN,clsRunRobot.m_npTemp_Picker_YesNo);
				
//				}
// 				else
// 				{
// 					m_RunRelayMoveStep = 7500;
// 					break;
// 				}
			}
		
			if(nRet_1 == RET_GOOD)
			{
				mn_Retry = 0;
				m_RunRelayMoveStep = 7100;	
				//kwlee 2017.0202
				//OnFeederReq(COM_START);//label Pick &발행 요청.
			}
			else if(nRet_1 == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(408, dWARNING, m_strAlarmCode);
			}
			break;

		case 7010:
			nRet_3 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_Z, st_motor_info[M_PICKERRBT_Z].d_pos[ROBOT_FEEDER_PICK_FIRST] + (mn_Retry*0.2), COMI.mn_manualspeed_rate);

			if (nRet_3 == BD_GOOD) //좌측으로 이동
			{
				m_RunRelayMoveStep = 7100;
			}
			else if (nRet_3 == BD_ERROR || nRet_3 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(406, dWARNING, st_alarm_info.strCode);
				m_RunRelayMoveStep = 0;
			}
			break;
			
		case 7100:
			//label 부착 확인
			if( m_nPickerNum < 6)
			{
				nRect[0] = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[m_nPickerNum],IO_ON);
			}
			else
			{
// 				if(m_nPickCnt > (PICKCNT + 5 - m_nPickerNum)) 
// 				{
					nRect[0] = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[PICKCNT + 5 - m_nPickerNum],IO_ON);
// 					clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT + 5 - m_nPickerNum] = CTL_YES;
// 					//picker down			
// 					clsRunRobot.OnSetPickerUpDn(0, PICKER_DN, clsRunRobot.m_npTemp_Picker_YesNo);
// 				}
// 				else
// 				{
// 					m_RunRelayMoveStep = 7500;
// 					break;
// 				}

			/*	nRect[0] = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[PICKCNT + 5 - m_nPickerNum],IO_ON);*/
			} 
			
// 			if (nRect[0] == IO_OFF)
// 			{
// 				if (mn_Retry > RETRYCNT)
// 				{
// 					nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_Z, st_motor_info[M_PICKERRBT_Z].d_pos[ROBOT_SAFETY], COMI.mn_manualspeed_rate);
// 
//  					if (nRet_1 == BD_GOOD)
//  					{
// 						/////////////////////////////////////
// 						//505080 1 A "Head Vaccum #1 Off Check Error ."
// 						//505081 1 A "Head Vaccum #1 On Check Error ."
// 						m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_HeadVaccumCheck[m_nPickerNum], IO_OFF);
// 						CTL_Lib.Alarm_Error_Occurrence(409, dWARNING, m_strAlarmCode);
// 						OnFeederReq(COM_CLEAR); //강제로 Complete Signal 보낸다.
// 					}
// 					else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
// 					{
// 						CTL_Lib.Alarm_Error_Occurrence(410, dWARNING, st_alarm_info.strCode);
// 						m_RunRelayMoveStep = 0;
// 					}
// 				}
// 				else
// 				{	
// 					mn_Retry++;
// 					clsRunRobot.OnSetPickerUpDn(0, PICKER_DN, clsRunRobot.m_npTemp_Picker_YesNo);
// 					m_RunRelayMoveStep = 7010;
// 				}
// 			}
// 			else
			{
				//OnFeederReq(COM_START);//label Pick &발행 요청.
				m_RunRelayMoveStep = 7110;
				m_dwTimeCheck[0] = GetCurrentTime();
				mn_Retry = 0;
			}
			break;

		case 7110:
			m_dwTimeCheck[1] = GetCurrentTime();
			m_dwTimeCheck[2] = m_dwTimeCheck[1] - m_dwTimeCheck[0];
			if( m_dwTimeCheck[2] <= 0 ) m_dwTimeCheck[0] = GetCurrentTime();
			if( m_dwTimeCheck[2] >  st_wait_info.dOnWaitTime[WAIT_PICKER_UP_DN] )
			{
				clsRunRobot.OnSetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);
				//kwlee 2017.0202
// 				OnFeederReq(COM_START);//label Pick &발행 요청.
 				m_RunRelayMoveStep = 7120;
 				//m_dwTimeCheck[0] = GetCurrentTime();
			}
			break;

		case 7120:
			nRet_1 = clsRunRobot.OnGetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);
			
			if(nRet_1 == RET_GOOD)
			{
				if( m_nPickerNum < 6)
				{
					nRect[0] = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[m_nPickerNum],IO_ON);
				}
				else
				{
// 					if(m_nPickCnt > (PICKCNT + 5 - m_nPickerNum)) 
// 					{
						nRect[0] = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[PICKCNT + 5 - m_nPickerNum],IO_ON);
// 					}
// 					else
// 					{
// 						m_RunRelayMoveStep = 7500;
// 						break;
// 					}
				} 

// 				if(nRect[0] == IO_ON )
// 				{
					//OnFeederReq(COM_START);//label Pick &발행 요청.
					m_RunRelayMoveStep = 7300;
					m_dwTimeCheck[0] = GetCurrentTime();
			//	}
// 				else
// 				{
// 					m_RunRelayMoveStep = 7010;
// 				}
			}
			
			break;

// 		case 7200:
// 			//picker up
// 			if( GetIntefaceInfo(COM_READY) == TRUE)
// 			{
// 				OnFeederReq(COM_READY_CHK); //Label Feeder로 Complete 신호 보냄
// 				m_RunRelayMoveStep = 7210;
// 				m_dwTimeCheck[0] = GetCurrentTime();
// 			}
// 			else
// 			{
// 				m_dwTimeCheck[1] = GetCurrentTime();
// 				m_dwTimeCheck[2] = m_dwTimeCheck[1] - m_dwTimeCheck[0];
// 				if( m_dwTimeCheck[2] <= 0 ) m_dwTimeCheck[0] = GetCurrentTime();
// 				if( m_dwTimeCheck[2] > 30000)
// 				{//손으로 라벨 제거
// 					//507140 1 A "Label Feeder LF_COMPLETE Off Check Error."
// 					m_strAlarmCode.Format(_T("507140")); 
// 					CTL_Lib.Alarm_Error_Occurrence(414, dWARNING, m_strAlarmCode);
// 					clsRunRobot.OnSetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);
// 					/*SetInterfaceStep(COM_NONE);*/
// 					m_RunRelayMoveStep = 7100;
// 					break;
// 				}
// 			}
// 
// 			break;
// 
// 		case 7210:
// 			m_dwTimeCheck[1] = GetCurrentTime();
// 			m_dwTimeCheck[2] = m_dwTimeCheck[1] - m_dwTimeCheck[0];
// 			if( m_dwTimeCheck[2] <= 0 ) m_dwTimeCheck[0] = GetCurrentTime();
// 			if( m_dwTimeCheck[2] > 30000)
// 			{//손으로 라벨 제거
// 				//507140 1 A "Label Feeder LF_COMPLETE Off Check Error."
// 				m_strAlarmCode.Format(_T("507140")); 
// 				CTL_Lib.Alarm_Error_Occurrence(414, dWARNING, m_strAlarmCode);
// 				clsRunRobot.OnSetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);
// 				/*SetInterfaceStep(COM_NONE);*/
// 				m_RunRelayMoveStep = 7000;
// 				break;
// 			}
// 			if ( GetIntefaceInfo(COM_COMPLETE_CHK) == TRUE )
// 			{		
// 				if( m_nPickerNum < 6)
// 				{
// 					clsRunRobot.m_npTemp_Picker_YesNo[m_nPickerNum] = CTL_YES;
// 				}
// 				else
// 				{
// 					clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT + 5 - m_nPickerNum] = CTL_YES;
// 				}
// 				/*clsRunRobot.OnSetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);*/
// 
// 				m_RunRelayMoveStep = 7300;
// 				//1ch vlfdy
// 				SetInterfaceStep(COM_NONE);
// 			}
// 			break;

		case 7300:	
			//z축 up
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_Z, st_motor_info[M_PICKERRBT_Z].d_pos[ROBOT_SAFETY], COMI.mn_manualspeed_rate);

			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_RunRelayMoveStep = 7310;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(404, dWARNING, st_alarm_info.strCode);
				m_RunRelayMoveStep = 0;
			}
			break;
			///////////////////////////////////////////////////////////////////

		case 7310:
			//picker up
			if( GetIntefaceInfo(COM_READY) == TRUE)
			{
				//kwlee 2017.0202
				//OnFeederReq(COM_READY_CHK); //Label Feeder로 Complete 신호 보냄
				m_RunRelayMoveStep = 7320;
				m_dwTimeCheck[0] = GetCurrentTime();
			}
			else
			{
				m_dwTimeCheck[1] = GetCurrentTime();
				m_dwTimeCheck[2] = m_dwTimeCheck[1] - m_dwTimeCheck[0];
				if( m_dwTimeCheck[2] <= 0 ) m_dwTimeCheck[0] = GetCurrentTime();
				if( m_dwTimeCheck[2] > 30000)
				{//손으로 라벨 제거
					//507140 1 A "Label Feeder LF_COMPLETE Off Check Error."
					m_strAlarmCode.Format(_T("507140")); 
					CTL_Lib.Alarm_Error_Occurrence(414, dWARNING, m_strAlarmCode);
					clsRunRobot.OnSetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);
					/*SetInterfaceStep(COM_NONE);*/
					m_RunRelayMoveStep = 7100;
					break;
				}
			}
			break;

		case 7320:
			m_dwTimeCheck[1] = GetCurrentTime();
			m_dwTimeCheck[2] = m_dwTimeCheck[1] - m_dwTimeCheck[0];
			if( m_dwTimeCheck[2] <= 0 ) m_dwTimeCheck[0] = GetCurrentTime();
			if( m_dwTimeCheck[2] > 30000)
			{//손으로 라벨 제거
				//507140 1 A "Label Feeder LF_COMPLETE Off Check Error."
				m_strAlarmCode.Format(_T("507140")); 
				CTL_Lib.Alarm_Error_Occurrence(414, dWARNING, m_strAlarmCode);
				clsRunRobot.OnSetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);
				/*SetInterfaceStep(COM_NONE);*/
				m_RunRelayMoveStep = 7100;
				break;
			}
			if ( GetIntefaceInfo(COM_COMPLETE_CHK) == TRUE )
			{
				if( m_nPickerNum < 6)
				{
					clsRunRobot.m_npTemp_Picker_YesNo[m_nPickerNum] = CTL_YES;
				}
				else
				{
					//clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT + 5 - m_nPickerNum] = CTL_YES;
					if(m_nPickCnt > (PICKCNT + 5 - m_nPickerNum)) 
					{
						clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT + 5 - m_nPickerNum] = CTL_YES;
					}
					else
					{
						m_RunRelayMoveStep = 7500;
						break;
					}
				}
				/*clsRunRobot.OnSetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);*/

				m_RunRelayMoveStep = 7400;
				//1ch vlfdy
				SetInterfaceStep(COM_CLEAR);
			}
			break;
			/////////////////////////////////////////////////////////

		case 7400:
			// picker up 체크
			if( m_nPickerNum < 6)
			{
				clsRunRobot.m_npTemp_Picker_YesNo[m_nPickerNum] = CTL_YES;
			}
			else
			{
				//clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT + 5 - m_nPickerNum] = CTL_YES;

// 				if(m_nPickCnt > (PICKCNT + 5 - m_nPickerNum)) 
// 				{
					clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT + 5 - m_nPickerNum] = CTL_YES;
// 				}
// 				else
// 				{
// 					m_RunRelayMoveStep = 7500;
// 					break;
// 				}
			}

			nRet_1 = clsRunRobot.OnGetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);

			if(nRet_1 == RET_GOOD)
			{
				m_RunRelayMoveStep = 7500;
			}
			else if(nRet_1 == RET_ERROR)
			{
				//	m_RunRelayMoveStep = 0;
				CTL_Lib.Alarm_Error_Occurrence(401, dWARNING, m_strAlarmCode);
			}
			break;

		case 7500:
			
			if (st_basic_info.nPcbType == UDIMM_9 || st_basic_info.nPcbType == UDIMM_10 || 
				st_basic_info.nPcbType == SODIMM || st_basic_info.nPcbType == RDIMM)
			{
				//if( m_nPickCnt >= (m_nPickerNum))
				if( m_nPickerNum >= ( 6 - 1 ))
				{
					m_RunRelayMoveStep = 9000;	
				}
				else
				{
					m_RunRelayMoveStep = 100;
					m_nPickerNum++;
				}
			}
			break;

			////////////////////////////////////////////////////
			//MOVE
		case 9000:
			for (int i = 0; i < 6; i++)
			{
				nRect[i] = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[i],IO_ON);
				//버리는 동작 추가 예정.

// 				if (nRect[i] == IO_OFF)
// 				{
// 					//505080 1 A "Head Vaccum #1 Off Check Error ."
// 					//505081 1 A "Head Vaccum #1 On Check Error ."
// 					m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_HeadVaccumCheck[i], IO_OFF);
// 					CTL_Lib.Alarm_Error_Occurrence(402, dWARNING, m_strAlarmCode);
// 					m_RunRelayMoveStep = 0;
// 					break;
// 				}
// 				else
				{ 
					if (i< 6 -1)
					{
						continue;
					}	

					m_RunRelayMoveStep = 13000;					
				}
			}
			break;
			//////////////////////////////
		case 13000:	
			//ROBOT TURN
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_TURN, st_motor_info[M_PICKERRBT_TURN].d_pos[PITCH_UP_HEAD_TURN_ROTATOR_R_90], COMI.mn_manualspeed_rate);

			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_RunRelayMoveStep = 14000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(401, dWARNING, st_alarm_info.strCode);
				m_RunRelayMoveStep = 0;
			}
			break;

			///////////////////////////////////
			//붙이는 위치 이동.
		case 14000:
			if (m_bSecondPos == false)
			{
				m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_FIRST];
				m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_FIRST];
			}
			else
			{
				m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_SECOND];
				m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_SECOND];
			}

			m_lpAxisNum[0] = M_PICKERRBT_Y; //m_lpAxisNum[4];        //현재의 IndexNum에서 사용하는 모터 실재 번호를 가진다 
			m_lpAxisNum[1] = M_PICKERRBT_X;

			nRet_1 = CTL_Lib.Linear_Move(ONLY_MOVE_START, m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, COMI.mn_manualspeed_rate);

			if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
			{
				m_RunRelayMoveStep = 15000;
			}
			else if(nRet_1 == BD_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(409, dWARNING, st_alarm_info.strCode);
				m_RunRelayMoveStep = 0;
			}
			break;

		case 15000:
			nRet = CTL_Lib.Linear_Move(ONLY_MOVE_CHECK, m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, COMI.mn_manualspeed_rate);
			if(nRet == BD_GOOD) //정상적으로 완료된 상태 
			{						
				//m_RunMoveStep = 0;
				//m_RunRelayMoveStep = 15000;
				m_RunRelayMoveStep = 16000;

				//return RET_GOOD;
			}
			else if(nRet == BD_ERROR)
			{			
				m_RunRelayMoveStep = 14000;
				CTL_Lib.Alarm_Error_Occurrence(409, dWARNING, st_alarm_info.strCode);
			}			
			break;

			/////////////////////////////////////////////////////////////////////////////////////////////
		case 16000:	
			////ROBOT PITCH 작업 위치 이동

			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_PITCH, st_motor_info[M_PICKERRBT_PITCH].d_pos[PITCH_UP_HEAD_PLACE_PCB], COMI.mn_manualspeed_rate);

			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_RunRelayMoveStep = 16100;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(401, dWARNING, st_alarm_info.strCode);
				m_RunRelayMoveStep = 0;
			}
			break;

			//////////////////////////////
			//붙이는 작업 
		case 16100:
			//for (int i =0; i<PICKCNT; i++ )
			for (int i =0; i < TOTAL_PICK; i++ )
			{
				if(m_bSecondPos == false)
				{
					if (i < 6)
					{
						clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_YES;
					}
				}
				else
				{
					if(m_nPickCnt >= (PICKCNT - 6 + i)) 
					{
						clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT - 6 + i] = CTL_YES;	
						//clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT - i ] = CTL_YES;	
					}
					
				}	
			}
			clsRunRobot.OnSetPickerUpDn(0, PICKER_DN, clsRunRobot.m_npTemp_Picker_YesNo);

			m_RunRelayMoveStep = 17000;
			break;

		case 17000:	
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_Z, st_motor_info[M_PICKERRBT_Z].d_pos[ROBOT_Z_PLACE], COMI.mn_manualspeed_rate);

			if (nRet_1 == BD_GOOD) 
			{
				m_RunRelayMoveStep = 18000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(404, dWARNING, st_alarm_info.strCode);
				m_RunRelayMoveStep = 0;
			}
			break;

		case 18000:
			// loader robot picker Dw 체크
			nRet_1 = clsRunRobot.OnGetPickerUpDn(0, PICKER_DN, clsRunRobot.m_npTemp_Picker_YesNo);

			if(nRet_1 == RET_GOOD)
			{
				m_RunRelayMoveStep = 19000;
			}
			else if(nRet_1 == RET_ERROR)
			{
				//	m_RunRelayMoveStep = 0;

				CTL_Lib.Alarm_Error_Occurrence(401, dWARNING, m_strAlarmCode);
			}
			break;

		case 19000:
			for (int i =0; i<TOTAL_PICK; i++ )
			{
				if(m_bSecondPos == false)
				{
					if (i < 6)
					{
						FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[i],IO_OFF);
					}
				}
				else
				{
					if(m_nPickCnt >= (PICKCNT - 6 + i)) 
					{	
						FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[PICKCNT - 6 + i],IO_OFF);
					}

				}

				//if (i< PICKCNT -1)
				if (i< m_nPickCnt - 1)
				{
					continue;
				}
				m_RunRelayMoveStep = 19100;
				
			
			}
			break;

		case 19100:
			for (int i =0; i<TOTAL_PICK; i++ )
			{
				if(m_bSecondPos == false)
				{
					if (i < 6)
					{
						FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[i],IO_ON);
					}
				}
				else
				{
					if(m_nPickCnt >= (PICKCNT - 6 + i)) 
					{	
						FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[PICKCNT - 6 + i],IO_ON);
					}

				}

				//if (i< PICKCNT -1)
				if (i< m_nPickCnt - 1)
				{
					continue;
				}
				m_RunRelayMoveStep = 20000;
				
			}

			m_lVaccumOffTime[0] = GetCurrentTime();

			break;
	

		case 20000:
			// loader robot picker up 요청
			//for (int i =0; i<PICKCNT; i++ )
			for (int i =0; i<TOTAL_PICK; i++ )
			{
				//clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_YES;
				if(m_bSecondPos == false)
				{
					if (i < 6)
					{
						clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_YES;
					}
				}
				else
				{
					if(m_nPickCnt >= (PICKCNT - 6 + i)) 
					{
						clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT - 6 + i] = CTL_YES;	
						//clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT - i ] = CTL_YES;	
					}
				}
			}
			//picker up
			clsRunRobot.OnSetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);

			m_RunRelayMoveStep = 21000;
			break;

		case 21000:	
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_Z, st_motor_info[M_PICKERRBT_Z].d_pos[ROBOT_SAFETY], COMI.mn_manualspeed_rate);
			//z 축 up
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_RunRelayMoveStep = 21100;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(404, dWARNING, st_alarm_info.strCode);
				m_RunRelayMoveStep = 0;
			}
			break;

		case 21100:
			m_lVaccumOffTime[1] = GetCurrentTime();
			m_lVaccumOffTime[2] = m_lVaccumOffTime[1] - m_lVaccumOffTime[0];

			if (m_lVaccumOffTime[2] > st_wait_info.dOffWaitTime[WAIT_PICKER_VACCUM])
			{
				for (int i =0; i<m_nPickCnt; i++ )
				{
					if(m_bSecondPos == false)
					{
						if (i < 6)
						{
							FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[i],IO_OFF);
						}
					}
					else
					{
						if(m_nPickCnt >= (PICKCNT - 6 + i)) 
						{	
							FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[PICKCNT - 6 + i],IO_OFF);
						}

					}

					//if (i< PICKCNT -1)
					if (i< m_nPickCnt - 1)
					{
						continue;
					}

					m_RunRelayMoveStep = 22000;

				}
			}
			break;

		case 22000:
			// picker up 체크
			nRet_1 = clsRunRobot.OnGetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);

			if(nRet_1 == RET_GOOD)
			{
				if(m_bSecondPos == false)
				{
					m_nLinearMove_Index = 0;
					m_bEmergencyStop = FALSE;
					CTL_Lib.m_nStep_Linear[m_nLinearMove_Index] = 0;
					m_dwTimeCheck[0] = GetCurrentTime();
					m_RunRelayMoveStep = 0;
					m_nPickerNum = 0;
					m_nPlaceCnt = 0;
					m_RunRelayMoveStep = 0;

				}
				else
				{
					m_dwTimeCheck[1] = GetCurrentTime();
					m_dwTimeCheck[2] = m_dwTimeCheck[1] - m_dwTimeCheck[0]; 

					str.Format(_T("%3.2f"),((float)m_dwTimeCheck[2]/(long)1000));
					m_pGridRobotRunTime.SetItemText(0, 1,str);
					
					m_bSecondPos = false;

					m_nPlaceCnt = 0;
					m_RunRelayMoveStep = 0;
					//return RET_GOOD;
				}

				for (int i = 0; i<PICKCNT; i++)
				{
					clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_NO;	
				}
			}
			else if(nRet_1 == RET_ERROR)
			{
				//	m_RunRelayMoveStep = 0;
				CTL_Lib.Alarm_Error_Occurrence(401, dWARNING, m_strAlarmCode);
			}
			break;
		}
		return RET_PROCEED;
}


int CDialogManualIO::OnRobotMove()
{
	int nRet_1, nRet_2=0, nRet_3=0, nRect[12]={0,};
	int nRet;
	double dCurrentPos = 0;

	if( m_bEmergencyStop == true )
	{
		m_RunMoveStep = 0;
		return RET_ERROR;
	}

	switch (m_RunMoveStep)
	{

	case 0:
		for (int i = 0; i < PICKCNT; i++)
		{
			 FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[i],IO_OFF);

			if (FAS_IO.get_out_bit(st_io_info.o_HeadVaccum[i],IO_OFF) == IO_ON)
			{
				m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.o_HeadVaccum[i], IO_ON);
				CTL_Lib.Alarm_Error_Occurrence(402, dWARNING, m_strAlarmCode);
				m_RunMoveStep = 0;
				break;
			}
			else
			{ 
				if (i< PICKCNT -1)
				{
					continue;
				}						
				m_RunMoveStep = 100;					
							
			}
		}
		break;

		case 100:
			// loader robot picker up 요청
			for (int i =0; i<PICKCNT; i++ )
			{
				clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_YES;
			}

			clsRunRobot.OnSetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);

			m_RunMoveStep = 200;

			break;


		case 200:	
			
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_Z, st_motor_info[M_PICKERRBT_Z].d_pos[ROBOT_SAFETY], COMI.mn_manualspeed_rate);

			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_RunMoveStep = 1000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(404, dWARNING, st_alarm_info.strCode);
				m_RunMoveStep = 0;
			}
			break;


		case 1000:
			// loader robot picker up 체크
			nRet_1 = clsRunRobot.OnGetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);

			if(nRet_1 == RET_GOOD)
			{
				m_RunMoveStep = 2000;
			}
			else if(nRet_1 == RET_ERROR)
			{
				if (m_nCnt>RETRYCNT)
				{
					m_RunMoveStep = 0;
					m_nCnt++;
				}
				else
				{
					CTL_Lib.Alarm_Error_Occurrence(401, dWARNING, m_strAlarmCode);
					m_nCnt = 0;
				}
			}
			break;

	
		case 2000:
			//m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_REJECT_PLACE];
			//m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_REJECT_PLACE];
			m_lpAxisNum[0] = M_PICKERRBT_Y; //m_lpAxisNum[4];        //현재의 IndexNum에서 사용하는 모터 실재 번호를 가진다 
			m_lpAxisNum[1] = M_PICKERRBT_X;

			nRet_1 = CTL_Lib.Linear_Move(ONLY_MOVE_START, m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, COMI.mn_manualspeed_rate);

			if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
			{
				m_RunMoveStep = 3000;
			}
			else if(nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				CTL_Lib.Alarm_Error_Occurrence(409, dWARNING, st_alarm_info.strCode);
				m_RunMoveStep = 0;
			}
			break;

		case 3000:
			nRet = CTL_Lib.Linear_Move(ONLY_MOVE_CHECK, m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, COMI.mn_manualspeed_rate);
			if(nRet == BD_GOOD) //정상적으로 완료된 상태 
			{						
				m_RunMoveStep = 0;

				return RET_GOOD;
			}
			else if(nRet == BD_ERROR || nRet == BD_SAFETY)
			{			
				m_RunMoveStep = 2000;
				CTL_Lib.Alarm_Error_Occurrence(409, dWARNING, st_alarm_info.strCode);
			}			
			break;

	}
	return RET_PROCEED;
}

void CDialogManualIO::OnBnClickedCheckManualAction100()
{
	m_nManualActionCount[0]	= 100;
	m_nManualActionCount[1]	= 0;
	
	m_nManualStep = 0;
	m_bEmergencyStop		= FALSE;
	m_dwTimeCheck[0] = GetCurrentTime();
	SetTimer(TMR_IO_CONTINUE,100, NULL);
	OnIoActionMode(2);
}


void CDialogManualIO::OnBnClickedBtnManualEmergency()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_bEmergencyStop)
	{
		m_bEmergencyStop = true;
	}
}


void CDialogManualIO::OnStnClickedWaitTime()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog_KeyPad dlgKeyPad;
	CString strTmp;

	dlgKeyPad.m_nKeypadMode			= 0;
	dlgKeyPad.m_strKeypadLowLimit	= _T("0");
	dlgKeyPad.m_strKeypadHighLimit	= _T("5000");
	strTmp.Format(_T("%d"), m_nManualWaitTime);
	dlgKeyPad.m_strKeypadVal		= strTmp;

	if (dlgKeyPad.DoModal() == IDOK)
	{
		strTmp				= dlgKeyPad.m_strNewVal;
		m_nManualWaitTime = _wtoi(strTmp);

		m_WaitTime.SetVal(m_nManualWaitTime);
	}
}

int CDialogManualIO::OnRunPicker()
{
	int nRet_1, nRet_2=0, nRet_3=0, nRect[12]={0,} ,i= 0;
	//	int nRet;

	double d_CurPos[30] = {0,};
	double d_Pos[30] = {0,};

	if( m_bEmergencyStop == true )
	{
		m_RunPickerStep = 0;
		return RET_ERROR;
	}
	switch (m_RunPickerStep)
	{
		case 0:
			for (i = 0; i < PICKCNT; i++)
			{
				FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[i],IO_OFF);
			}
			m_RunPickerStep = 100;
			break;

		case 100:
			for (i = 0; i < PICKCNT; i++)
			{
				if (FAS_IO.get_out_bit(st_io_info.o_HeadVaccum[i],IO_OFF) == IO_ON)
				{
					//505080 1 A "Head Vaccum #1 Off Check Error ."
					//505081 1 A "Head Vaccum #1 On Check Error ."
					m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.o_HeadVaccum[i], IO_ON);
					CTL_Lib.Alarm_Error_Occurrence(400, dWARNING, m_strAlarmCode);
					m_RunPickerStep = 0;
					break;
				}
				else
				{ 
					if (i< PICKCNT -1)
					{
						continue;
					}

					FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[i],IO_OFF);
					m_RunPickerStep = 1000;					
				}
			}
			break;

		case 1000:
			//picker up
			for (i =0; i<PICKCNT; i++ )
			{
				clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_YES;
			}

			clsRunRobot.OnSetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);

			m_RunPickerStep = 2000;

			break;

		case 2000:	
			//z축 up
			
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_Z, st_motor_info[M_PICKERRBT_Z].d_pos[ROBOT_SAFETY], COMI.mn_manualspeed_rate);

			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_RunPickerStep = 3000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(401, dWARNING, st_alarm_info.strCode);
				m_RunPickerStep = 0;
			}
			break;

		case 3000:
			for (i =0; i<PICKCNT; i++ )
			{
				clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_YES;
			}
			// picker up 체크
			nRet_1 = clsRunRobot.OnGetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);

			if(nRet_1 == RET_GOOD)
			{ 
				m_RunPickerStep = 3100;
			}
			else if(nRet_1 == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(402, dWARNING, m_strAlarmCode);
			}
			break;
			///////////////
		case 3100:	
			//ROBOT TURN
			
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_TURN, st_motor_info[M_PICKERRBT_TURN].d_pos[PITCH_UP_HEAD_TURN_ROTATOR_0], COMI.mn_manualspeed_rate);

			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_RunPickerStep = 3150;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(401, dWARNING, st_alarm_info.strCode);
				m_RunPickerStep = 0;
			}
			break;

		case 3150:	
			////ROBOT PITCH 작업 위치 이동
			
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_PITCH, st_motor_info[M_PICKERRBT_PITCH].d_pos[PITCH_UP_HEAD_PICK_FEEDER], COMI.mn_manualspeed_rate);

			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_RunPickerStep = 3200;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(401, dWARNING, st_alarm_info.strCode);
				m_RunPickerStep = 0;
			}
			break;
			/////////////////////////////////////////////////////////////////////////////////
		case 3200:
			m_dPitch_Y = (st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST] - st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_END])/5;
			m_dPitch_x = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_FIRST] - st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_END];
			// X,Y 축 작업 위치 이동
			if( m_nPickerNum < 6)
			{
				m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST] - (m_nPickerNum*m_dPitch_Y);
				m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_FIRST];
			}
			else
			{
				m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST] - ((PICKCNT- 1 - m_nPickerNum) * m_dPitch_Y);
				//m_dpTargetPosList[0] = st_motor_info[m_nRobot_Y].d_pos[ROBOT_Y_FEEDER_PICK_END] - ((m_nPickerNum-6) * m_dPitch_Y);
				m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_END];
			}
			
// 			m_lpAxisNum[0] = M_PICKERRBT_Y;  
// 			m_lpAxisNum[1] = M_PICKERRBT_X;
// 			m_lpAxisNum[2] = M_PICKERRBT_Z;
// 			m_nLinearMove_Index	= 0;   //현재 map index
// 			m_lAxisCnt	= 2; 


			nRet_1 = CTL_Lib.Linear_Move(ONLY_MOVE_START, m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, COMI.mn_manualspeed_rate);

			if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
			{
				m_RunPickerStep = 3300;
			}
			else if(nRet_1 == BD_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(403, dWARNING, st_alarm_info.strCode);
				m_RunPickerStep = 0;
			}
			break;
			/////////////////////////////////////////////////////////////////


		case 3300:
			nRet_1 = CTL_Lib.Linear_Move(ONLY_MOVE_CHECK, m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, COMI.mn_manualspeed_rate);

			if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태 
			{						 
				m_RunPickerStep = 4000;	
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_RunPickerStep = 3200;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{			
				m_RunPickerStep = 3200;
				CTL_Lib.Alarm_Error_Occurrence(404, dWARNING, st_alarm_info.strCode);
			}
			break;

		case 4000:
			for (int i =0; i<PICKCNT; i++ )
			{
				clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_NO;
			}
			clsRunRobot.m_npTemp_Picker_YesNo[m_nPickerNum] = CTL_YES;

			
			clsRunRobot.OnSetPickerUpDn(0, PICKER_DN, clsRunRobot.m_npTemp_Picker_YesNo);
			m_RunPickerStep = 5000;
			break;

		case 5000:
			//vaccum On
			FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[m_nPickerNum],IO_ON);
			m_RunPickerStep = 6000;
			break;

		case 6000:	
			//z축 내려서 label pick 위치 이동
			

			//함수호출
			nRet_1 = COMI.Check_MotPosRange(M_PICKERRBT_X,st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_FIRST],st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_FIRST] + st_motor_info[M_PICKERRBT_X].d_allow);
			nRet_2 = COMI.Check_MotPosRange(M_PICKERRBT_Y, st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST],st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST] + st_motor_info[M_PICKERRBT_Y].d_allow);

			if (nRet_1 == BD_GOOD && nRet_2 == BD_GOOD)
			{
				nRet_3 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_Z, st_motor_info[M_PICKERRBT_Z].d_pos[ROBOT_FEEDER_PICK_FIRST], COMI.mn_manualspeed_rate);

				if (nRet_3 == BD_GOOD) //좌측으로 이동
				{
					m_RunPickerStep = 7000;
				}
				else if (nRet_3 == BD_ERROR || nRet_3 == BD_SAFETY)
				{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
					CTL_Lib.Alarm_Error_Occurrence(406, dWARNING, st_alarm_info.strCode);
					m_RunPickerStep = 0;
				}
			}
			else
			{
				m_strAlarmCode.Format(_T("%02d0006"), M_PICKERRBT_Z); //000008 0 A "MOTOR MOVE SAFETY CHECK ERROR -[M_ROBOT_X]."
				CTL_Lib.Alarm_Error_Occurrence(407, dMOTOR_WARNING, m_strAlarmCode);//dMOTOR_WARNING ==> 이때는 알람은 발생시키지 않는다(run status를 바꾸지 않는다).thread 에서만 판단하여 알람 발생시킨다 

				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
				{
					clsMem.OnNormalMessageWrite(_T("Robot Z Place Position Error"));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
				}
			}
			break;

		case 7000:
			//picker down
			//kwlee 2017.0202
			//OnFeederReq(COM_START);
			clsRunRobot.m_npTemp_Picker_YesNo[m_nPickerNum] = CTL_YES;
			nRet_1 = clsRunRobot.OnGetPickerUpDn(0, PICKER_DN,clsRunRobot.m_npTemp_Picker_YesNo);

			if(nRet_1 == RET_GOOD)
			{
				m_RunPickerStep = 7100;	
			}
			else if(nRet_1 == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(408, dWARNING, m_strAlarmCode);
			}
			break;

		case 7100:
			nRect[0] = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[m_nPickerNum],IO_ON);

			if (nRect[0] == IO_OFF)
			{
				if (mn_Retry>RETRYCNT)
				{
					
					nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_Z, st_motor_info[M_PICKERRBT_Z].d_pos[ROBOT_SAFETY], COMI.mn_manualspeed_rate);

					if (nRet_1 == BD_GOOD)
					{
						/////////////////////////////////////
						//505080 1 A "Head Vaccum #1 Off Check Error ."
						//505081 1 A "Head Vaccum #1 On Check Error ."
						m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_HeadVaccumCheck[m_nPickerNum], IO_OFF);
						CTL_Lib.Alarm_Error_Occurrence(409, dWARNING, m_strAlarmCode);
						//kwlee 2017.0202
						//OnFeederReq(COM_CLEAR); //강제로 Complete Signal 보낸다.
						mn_Retry = 0;
					}
					else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
					{
						CTL_Lib.Alarm_Error_Occurrence(410, dWARNING, st_alarm_info.strCode);
						m_RunPickerStep = 0;
					}
				}
				else
				{	
					clsRunRobot.OnSetPickerUpDn(0, PICKER_DN, clsRunRobot.m_npTemp_Picker_YesNo);
					m_RunPickerStep = 7000;
					mn_Retry++;
				}
			}
			else
			{
				m_RunPickerStep = 7200;
				
			}
			break;

		case 7200:
			//picker up
			
			clsRunRobot.m_npTemp_Picker_YesNo[m_nPickerNum] = CTL_YES;
			clsRunRobot.OnSetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);
			//kwlee 2017.0202
			//OnFeederReq(COM_READY_CHK); //Label Feeder로 Complete 신호 보냄
			m_RunPickerStep = 7300; 
			break;

		case 7300:	
			//z축 up
			
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_Z, st_motor_info[M_PICKERRBT_Z].d_pos[ROBOT_SAFETY], COMI.mn_manualspeed_rate);

			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_RunPickerStep = 7400;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(411, dWARNING, st_alarm_info.strCode);
				m_RunPickerStep = 0;
			}
			break;

		case 7400:
			// picker up 체크
			clsRunRobot.m_npTemp_Picker_YesNo[m_nPickerNum] = CTL_YES;
			nRet_1 = clsRunRobot.OnGetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);

			if(nRet_1 == RET_GOOD)
			{
				
				m_RunPickerStep = 7500;
			}
			else if(nRet_1 == RET_ERROR)
			{
				//	m_RunRelayMoveStep = 0;

				CTL_Lib.Alarm_Error_Occurrence(412, dWARNING, m_strAlarmCode);
			}
			break;
			///////////////////////////////////////////
		case 7500:
			if( m_nPickerNum >= ( PICKCNT-1 ) )
				m_RunPickerStep = 9000;	
			else
			{
				m_nPickerNum++;
				m_RunPickerStep = 3000;
			}
			break;	

		case 9000:
			//label 부착 확인
			for (int i = 0; i < PICKCNT; i++)
			{
				nRect[i] = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[i],IO_ON);
				if (nRect[i] == IO_OFF)
				{
					//505080 1 A "Head Vaccum #1 Off Check Error ."
					//505081 1 A "Head Vaccum #1 On Check Error ."
					m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_HeadVaccumCheck[i], IO_OFF);
					CTL_Lib.Alarm_Error_Occurrence(413, dWARNING, m_strAlarmCode);
					m_RunPickerStep = 0;
					break;
				}
				else
				{ 
					if (i< PICKCNT -1)
					{
						continue;
					}						
					m_RunPickerStep = 0;
					return BD_GOOD;
				}
			}
			break;
		}
	return RET_PROCEED;
}

void CDialogManualIO::OnTurnConvRuntTimeGrid()
{
	int   i, j;
	CString str;
	m_pGridRuntime.SetFrameFocusCell(FALSE);
	m_pGridRuntime.SetTrackFocusCell(FALSE);
	m_pGridRuntime.EnableSelection(FALSE);


	m_pGridRuntime.SetGridLineColor(BLACK_C);
	m_pGridRuntime.SetGridLines(1);

	m_pGridRuntime.SetRowCount(1);
	m_pGridRuntime.SetColumnCount(2);

	m_pGridRuntime.SetFixedRowCount(0);
	m_pGridRuntime.SetFixedColumnCount(0);
	m_pGridRuntime.SetFixedBkColor(RGB(0,0,200));
	m_pGridRuntime.SetFixedBkColor(RGB(200,200,255));
	m_pGridRuntime.SetTextBkColor(RGB(150,150,200));

	for (i=0; i<1; i++) 
	{
		m_pGridRuntime.SetRowHeight(i, 41);

		for (j=0; j<2; j++) 
		{
			if(j ==0)
			{

				m_pGridRuntime.SetColumnWidth(j, 80);
			}
			else
			{
				m_pGridRuntime.SetColumnWidth(j, 150);
			}
			m_pGridRuntime.SetItemBkColour(0, 0, GREEN_C, CLR_DEFAULT);
			m_pGridRuntime.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_pGridRuntime.SetItemState(i, j, GVIS_READONLY);
			m_pGridRuntime.SetItemBkColour(i, j, WHITE_C, CLR_DEFAULT);

		}
		str.Format(_T("RUNTIME"));
		m_pGridRuntime.SetItemText(0, 0,str);

	}
	str.Format(_T("00"));
	m_pGridRuntime.SetItemText(0, 1,str);

	m_pGridRuntime.Invalidate(FALSE);
}
void CDialogManualIO::OnPickerRunTimeGrid()
{
	int   i, j;
	CString str;
	m_pGridRobotRunTime.SetFrameFocusCell(FALSE);
	m_pGridRobotRunTime.SetTrackFocusCell(FALSE);
	m_pGridRobotRunTime.EnableSelection(FALSE);


	m_pGridRobotRunTime.SetGridLineColor(BLACK_C);
	m_pGridRobotRunTime.SetGridLines(1);

	m_pGridRobotRunTime.SetRowCount(1);
	m_pGridRobotRunTime.SetColumnCount(2);

	m_pGridRobotRunTime.SetFixedRowCount(0);
	m_pGridRobotRunTime.SetFixedColumnCount(0);
	m_pGridRobotRunTime.SetFixedBkColor(RGB(0,0,200));
	m_pGridRobotRunTime.SetFixedBkColor(RGB(200,200,255));
	m_pGridRobotRunTime.SetTextBkColor(RGB(150,150,200));

	for (i=0; i<1; i++) 
	{
		m_pGridRobotRunTime.SetRowHeight(i, 41);

		for (j=0; j<2; j++) 
		{
			if(j ==0)
			{

				m_pGridRobotRunTime.SetColumnWidth(j, 80);
			}
			else
			{
				m_pGridRobotRunTime.SetColumnWidth(j, 150);
			}
			m_pGridRobotRunTime.SetItemBkColour(0, 0, GREEN_C, CLR_DEFAULT);
			m_pGridRobotRunTime.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_pGridRobotRunTime.SetItemState(i, j, GVIS_READONLY);
			m_pGridRobotRunTime.SetItemBkColour(i, j, WHITE_C, CLR_DEFAULT);

		}
		str.Format(_T("RUNTIME"));
		m_pGridRobotRunTime.SetItemText(0, 0,str);

	}
	str.Format(_T("00"));
	m_pGridRobotRunTime.SetItemText(0, 1,str);

	m_pGridRobotRunTime.Invalidate(FALSE);
}
void CDialogManualIO::OnInitGroup()
{

	m_group_TurnConveyor.SetFont(clsFunc.OnLogFont(16));
	m_group_TurnConveyor.SetCatptionTextColor(BLUE_C);
	m_group_TurnConveyor.SetBorderColor(ORANGE_C);
	m_group_TurnConveyor.SetFontBold(TRUE);
	m_group_TurnConveyor.SetBackgroundColor(WINDOW_UP);

	m_groupTurnCovRuntime.SetFont(clsFunc.OnLogFont(16));
	m_groupTurnCovRuntime.SetCatptionTextColor(BLUE_C);
	m_groupTurnCovRuntime.SetBorderColor(ORANGE_C);
	m_groupTurnCovRuntime.SetFontBold(TRUE);
	m_groupTurnCovRuntime.SetBackgroundColor(WINDOW_UP);

	m_groupPickerRuntime.SetFont(clsFunc.OnLogFont(16));
	m_groupPickerRuntime.SetCatptionTextColor(BLUE_C);
	m_groupPickerRuntime.SetBorderColor(ORANGE_C);
	m_groupPickerRuntime.SetFontBold(TRUE);
	m_groupPickerRuntime.SetBackgroundColor(WINDOW_UP);

	m_groupRbotPicker.SetFont(clsFunc.OnLogFont(16));
	m_groupRbotPicker.SetCatptionTextColor(BLUE_C);
	m_groupRbotPicker.SetBorderColor(ORANGE_C);
	m_groupRbotPicker.SetFontBold(TRUE);
	m_groupRbotPicker.SetBackgroundColor(WINDOW_UP);

	
}
void CDialogManualIO::OnBnClickedBtnExt()
{
	KillTimer(TMR_IO_CONTINUE);
	CDialog::OnOK();
}


void CDialogManualIO::OnBnClickedBtnPick()
{
	m_RunPickUpStep = 0;
	m_nLinearMove_Index = 0;
	m_bEmergencyStop = FALSE;
	CTL_Lib.m_nStep_Linear[m_nLinearMove_Index] = 0;
	SetTimer(TM_ROBOT_PICK_UP, 100, NULL);
}


void CDialogManualIO::OnBnClickedBtnMove()
{
	m_RunMoveStep = 0;
	m_nLinearMove_Index = 0;
	m_bEmergencyStop = FALSE;
	CTL_Lib.m_nStep_Linear[m_nLinearMove_Index] = 0;
	SetTimer(TM_ROBOT_MOVE, 100, NULL);
}


void CDialogManualIO::OnBnClickedBtnPlace()
{

	m_RunPickPlaceStep = 0;
	m_nLinearMove_Index = 0;
	m_bEmergencyStop = FALSE;
	CTL_Lib.m_nStep_Linear[m_nLinearMove_Index] = 0;
	SetTimer(TM_ROBOT_PICK_PLACE, 100, NULL);
// 	for (int i =0; i<PICKCNT; i++ )
// 	{
// 		clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_YES;
// 	}
// 
// 	clsRunRobot.OnSetPickerUpDn(0, PICKER_DN, clsRunRobot.m_npTemp_Picker_YesNo);	
}


void CDialogManualIO::OnBnClickedBtnRelay()
{
	m_nLinearMove_Index = 0;
	m_bEmergencyStop = FALSE;
	CTL_Lib.m_nStep_Linear[m_nLinearMove_Index] = 0;
	SetTimer(TM_ROBOT_RELAY_MOVE, 100, NULL);
	m_dwTimeCheck[0] = GetCurrentTime();
	m_RunRelayMoveStep = 0;
	m_nPickerNum = 0;
	m_nPlaceCnt = 0;
}


void CDialogManualIO::OnBnClickedBtnPickPlace()
{
	m_RunPickDnStep = 0;
	m_bEmergencyStop = FALSE;
	CTL_Lib.m_nStep_Linear[m_nLinearMove_Index] = 0;
	SetTimer(TM_ROBOT_PICK_DN, 100, NULL);
}


void CDialogManualIO::OnBnClickedBtnPicker()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SetTimer(TM_PICKER, 100, NULL);
	m_RunPickerStep = 0;
	m_nPickerNum = 0;
}

void CDialogManualIO::OnFeederReq(int nStep)
{
	m_nInterFaceStep = nStep;
}
int CDialogManualIO::OnFeederInterface()
{
	int nRet;
	CString strMsg;
	if (FAS_IO.get_in_bit(st_io_info.i_LfAlarmChk,IO_ON) == IO_ON)
	{	
		//507151 1 A "Label Feeder LF_ALARM On Check Error."
		m_strAlarmCode.Format(_T("507151"));
		CTL_Lib.Alarm_Error_Occurrence(404, dWARNING, m_strAlarmCode);
		return RET_ERROR;
	}

	switch(m_nInterFaceStep)
	{
	case COM_CLEAR:
		FAS_IO.set_out_bit(st_io_info.o_PcComplete,IO_ON);

		if (FAS_IO.get_in_bit(st_io_info.i_LfCompChk,IO_ON) == IO_OFF)
		{
			FAS_IO.set_out_bit(st_io_info.o_PcComplete,IO_OFF);
		}
		m_nInterFaceStep = COM_NONE;
		break;

		case COM_START:
			nRet = FAS_IO.get_in_bit(st_io_info.i_LfReqChk,IO_ON);

			if(nRet == IO_OFF)
			{
				//507130 1 A "Label Feeder LF_REQUEST Off Check Error."
				m_strAlarmCode.Format(_T("507130")); 
				CTL_Lib.Alarm_Error_Occurrence(404, dWARNING, m_strAlarmCode);
			}
			else
			{
				m_nInterFaceStep = COM_READY;
			}
			break;

		case COM_READY:
		
			FAS_IO.set_out_bit(st_io_info.o_PcReady,IO_ON);
			m_lFeederWaitTime[0] = GetCurrentTime();

			break;

		case COM_READY_CHK:
// 			if (FAS_IO.get_out_bit(st_io_info.o_PcReady,IO_ON) == IO_OFF)
// 			{
// 				//507040 1 A "Label Feeder PC_Ready Error."
// 				m_strAlarmCode.Format(_T("507040")); 
// 				CTL_Lib.Alarm_Error_Occurrence(404, dWARNING, m_strAlarmCode);
// 			}
// 			else
// 			{
			m_lFeederWaitTime[1] = GetCurrentTime();
			m_lFeederWaitTime[2] = m_lFeederWaitTime[1] - m_lFeederWaitTime[0];

			if (m_lFeederWaitTime[2] > 500)
			{
				if (FAS_IO.get_in_bit(st_io_info.i_LfCompChk,IO_ON))
				{
					m_lFeederWaitTime[0] = GetCurrentTime();
					FAS_IO.set_out_bit(st_io_info.o_PcReady,IO_OFF);
					m_nInterFaceStep = COM_COMPLETE;
				}
				else
				{
					m_lFeederWaitTime[1] = GetCurrentTime();
					m_lFeederWaitTime[2] = m_lFeederWaitTime[1] - m_lFeederWaitTime[0];

					if (m_lFeederWaitTime[2]> st_wait_info.dLimitWaitTime[WAIT_FEEDER_REPLY])
					{
						//507141 1 A "Label Feeder LF_COMPLETE On Check Error."
						m_strAlarmCode.Format(_T("507141")); 
						CTL_Lib.Alarm_Error_Occurrence(404, dWARNING, m_strAlarmCode);
					}
				}
			  }
			break;

		case COM_COMPLETE:
			m_lFeederWaitTime[1] = GetCurrentTime();
			m_lFeederWaitTime[2] = m_lFeederWaitTime[1] - m_lFeederWaitTime[0];
			if (m_lFeederWaitTime[2] < 0) m_lFeederWaitTime[0] = GetCurrentTime();

			if (m_lFeederWaitTime[2] > 100)
			{
				FAS_IO.set_out_bit(st_io_info.o_PcComplete,IO_ON);
				m_nInterFaceStep = COM_COMPLETE_CHK;
			}
			break;

		case COM_COMPLETE_CHK:
			nRet = FAS_IO.get_in_bit(st_io_info.i_LfCompChk,IO_OFF);

			if (nRet == IO_OFF)
			{
				FAS_IO.set_out_bit(st_io_info.o_PcComplete,IO_OFF);
			}
			else
			{
				m_lFeederWaitTime[1] = GetCurrentTime();
				m_lFeederWaitTime[2] = m_lFeederWaitTime[1] - m_lFeederWaitTime[0];

				if (m_lFeederWaitTime[2]> st_wait_info.dLimitWaitTime[WAIT_FEEDER_REPLY])
				{
					//507140 1 A "Label Feeder LF_COMPLETE Off Check Error."
					m_strAlarmCode.Format(_T("507140")); 
					CTL_Lib.Alarm_Error_Occurrence(404, dWARNING, m_strAlarmCode);
				}
			}
			break;
	}
	return RET_PROCEED;
}

void CDialogManualIO::OnClickedBtnManualEmergency2()
{
	if (!m_bEmergencyStop)
	{
		m_bEmergencyStop = true;
	}
}
