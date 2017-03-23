// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "Handler.h"
#include "MainFrm.h"
#include "Variable.h"
#include "ScreenMain.h"
#include "ScreenBasic.h"
#include "ScreenInitial.h"
#include "ScreenIO.h"
#include "ScreenLock.h"
#include "ScreenMotor.h"
#include "ScreenList.h"
#include "ScreenWork.h"
#include "ScreenAlarm.h"
#include "ScreenPrgInfo.h"
#include "Dialog_Exit.h"
#include "Ctlbd_Variable.h"
#include "FastechPublic_IO.h"
#include "Dialog_Pass_Check.h"
#include "PublicFunction.h"
#include "MyBasicData.h"
#include "MyJamData.h"
#include "Dialog_Message.h"
#include "Dialog_About.h"
#include "ClientSocket.h"
#include "ServerSocket.h"
#include "ClientGms.h"
#include "XgemClient.h"
#include "AlgMemory.h"
#include "ClientEcFirst.h"
// #include "BarcodeUnLoadA.h"
// #include "BarcodeUnLoadB.h"
#include "ClientGms.h"
#include "ClientRfid.h"
#include "ServerFront.h"
#include "ServerVnR.h"
#include "ComizoaPublic.h"
#include "io.h"	
#include "ACriticalSection.h"
#include "CtlBd_Library.h"
#include "ZebraPrint.h"
#include "RunRobot.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame
tagCLIENT_INFO				st_client_info[20];
tagSERVER_INFO				st_server_info[20];
tagHANDLER_INFO				st_handler_info;
tagSERIAL_INFO				st_serial_info;
tagALARM_INFO				st_alarm_info;
tagALARM_DISPLAY_INFO		st_alarm_display_info;
tagBASIC_INFO				st_basic_info;
tagLAMP_INFO				st_lamp_info;
tagIO_INFO					st_io_info;
tagPATH_INFO				st_path_info;
tagWORK_INFO				st_work_info;
tagOTHER_INFO				st_other_info;
tagWAIT_INFO				st_wait_info;
tagSYNC_INFO				st_sync_info = {0};		//구조체 0으로 초기화
tagRS232_INFO				st_rs232_info;
tagMOTOR_INFO				st_motor_info[MOT_MAXMOTOR];
tagRECIPE_INFO				st_recipe_info;
tagRECOVERY_INFO			st_recovery_info;
tagANIMATE_INFO				st_animate_info	= {0};	//구조체 0으로 초기화
tagCOUNT_INFO				st_count_info;
tagWORKER_INFO				st_worker_info;
tagGMS_INFO					st_gms_info;
tagLOT_HISTORY_INFO			st_lot_history_info;
tagPART_INFO				st_part_info;
tagTYPE_INFO				st_type_info;
tagPICKER_DATA_INFO			st_Picker_info; //kwlee 2017.0204
tagBUFFER_DATA_INFO			st_Buffer_info;
tagVISION_INFO				st_Vision_info; //kwlee 2017.0315
/* 2015.0108
// loader picker 구조체
tagLD_PICKER_INFO			st_ld_picker_info[MAX_PICKER];
// loader buffer 구조체
// [0] load picker -> buffer [1] buffer -> test site robot
tagLD_BUF_INFO				st_ld_buf_info[2];
// unloader picker 구조체
tagUNLD_PICKER_INFO			st_unld_picker_info[MAX_PICKER];
// unloader buffer 구조체
// [0] unload picker <-> buffer [1] buffer <-> test site robot
tagUNLD_BUF_INFO			st_unld_buf_info[2];
// reject buffer 구조체
tagREJECT_BUFFER_INFO		st_reject_buf_info[2];
// retest buffer 구조체
tagRETEST_BUFFER_INFO		st_retest_buf_info;
// test site robot 구조체
tagTEST_SITE_RBT_INFO		st_test_site_rbt_info;


tagLD_TRAY_INFO				st_ld_tray_info[2];
// loader tray 구조체
tagUNLD_TRAY_INFO			st_unld_tray_info[2];

tagBOARD_INFO				st_bd_info[2][2];
*/
//2015.0102 
tagVARIABLE_INFO			st_var;
tagBOARD_INFO				st_bd_info[2][2];

// test site 구조체
//[0][0] left bottom [0][1] left top [1][0] right bottom [1][1] right top


tagLOT_INFO					st_lot_info[3];		// 2015.0115
tagCOKBUFF_SITE_INFO		st_Cok_Buff_info[4]; //cok 4개 사이트 정보 //2015.0124

tagLOT_DISPLAY_INFO			st_lot_display_info; // 2015.01.29 lot 전산처리 구조체

tagTEST_SITE_SOCKET_FIND_INFO st_test_socket_info[12]; //0~47까지의 소켓을 12가지로 구간을 나눠 관리 

tagTEST_SITE_SOCKET_FIND_INFO st_test_socket_status[12]; //2015.0222 james 소켓의 상태를 확인할때 사용. 고유 소켓정보 작업 변경까지 유지  //0~47까지의 소켓을 12가지로 구간을 나눠 관리,
tagTRACK_INFO				st_track_info;
tagSCRAP_CODE				st_code_info[2];
//20151005
tagDB_time_param			st_DB_time;
tagdbtime_database			st_dbtime;
tagdbTimeTmp_database		*st_dbTimeTmp;
tagdbTimeTmp_database       *st_dbTimeDate;
tagBuffer_DB_database		st_BufferTemp;
st_barcode_param			st_barcode;

PCB_info                    st_Pcb_info;

#define TM_MAIN_REFRESH	100
#define TM_FILE_CREATE	200
#define TM_GMS			300
#define TM_XGEM			400

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_BASIC, &CMainFrame::OnBasic)
	ON_COMMAND(ID_ALARM, &CMainFrame::OnAlarm)
	ON_COMMAND(ID_DOOR, &CMainFrame::OnDoor)
	ON_COMMAND(ID_EXIT, &CMainFrame::OnExit)
	ON_COMMAND(ID_IO, &CMainFrame::OnIo)
	ON_COMMAND(ID_LEVEL, &CMainFrame::OnLevel)
	ON_COMMAND(ID_LIST, &CMainFrame::OnList)
	ON_COMMAND(ID_LOCK, &CMainFrame::OnLock)
	ON_COMMAND(ID_MAIN, &CMainFrame::OnMain)
	ON_COMMAND(ID_Menu, &CMainFrame::OnMenu)
	ON_COMMAND(ID_MOTOR, &CMainFrame::OnMotor)
	ON_COMMAND(ID_RESET, &CMainFrame::OnReset)
	ON_COMMAND(ID_SAVE, &CMainFrame::OnSave)
	ON_COMMAND(ID_SETTING, &CMainFrame::OnSetting)
	ON_COMMAND(ID_VER, &CMainFrame::OnVer)
	ON_MESSAGE(WM_FORM_CHANGE, OnViewChangeMode)					// Post Message에 대한 화면 전환 사용자 사용자 정의 메시지 선언 
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_COMMAND(ID_PGMINFO, &CMainFrame::OnPgminfo)
	ON_MESSAGE(WM_CLIENT_MSG_1, OnClientEcFirst)					// Network관련된 작업을 담당한다.
	ON_MESSAGE(WM_CLIENT_MSG_2, OnClientXgem)						// Network관련된 작업을 담당한다.
	ON_MESSAGE(WM_CLIENT_MSG_3,	OnClientZebraPrint)
	ON_MESSAGE(WM_CLIENT_MSG_6, OnClientRfid)						// Network관련된 작업을 담당한다.
	ON_MESSAGE(WM_CLIENT_MSG_7,	OnClientFtp)
	ON_MESSAGE(WM_SERVER_MSG_3,	OnServerZebraPrinter)
	ON_MESSAGE(WM_SERVER_MSG_4, OnServerFront)						// Network관련된 작업을 담당한다.
	ON_MESSAGE(WM_SERVER_MSG_8, OnServerGms)
	ON_MESSAGE(WM_WORK_COMMAND, OnXgemCommand)						// Network관련된 작업을 담당한다. 
	ON_MESSAGE(WM_LEFT_POS, OnRobotLeftTurnUI)
	ON_MESSAGE(WM_RIGHT_POS, OnRobotRightTurnUI)
	ON_MESSAGE(WM_CONV_STATE, OnConVeyorUI)
	ON_MESSAGE(WM_CLIENT_MSG_8, OnBarcode_1) //kwlee 2017.0204
	ON_MESSAGE(WM_CLIENT_MSG_9, OnBarcode_2) //kwlee 2017.0204
	
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_NCLBUTTONDOWN()
//	ON_WM_KEYUP()
ON_WM_COPYDATA()
ON_MESSAGE(WM_MAINFRAME_WORK, OnMainframe_VisionWork) 
ON_WM_COPYDATA()
END_MESSAGE_MAP()

// CMainFrame 생성/소멸


CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
	// File 경로 지정 함수......
	//int i, j;
//	int nRect;
	// 온양삼성/중국 IO 불일치  온양 (0) 중국(1)
	st_handler_info.nPgmVer = 1;

	for (int i=0; i<20; i++)
	{
		m_pClient[i] = NULL;
		m_pServer[i] = NULL;

		st_client_info[i].nConnect = CTL_NO;
		st_server_info[i].nConnect = CTL_NO;
	}

// 	st_dbTimeTmp = NULL;
// 	st_dbTimeDate = NULL;
// 	
// 	m_pNewActiveView = NULL;
// 	m_pOldActiveView = NULL;

	OnFilePath();	
	// Excel Map 정보 읽기....
	OnExcelRead();
	OnConfigLoad();
	//kwlee 2016.0412 테스트용 삭제 예정
	//nRect = CTL_Lib.Initialize_motor_board(1,  _T("C:\\AMT_RT200\\AMT.cme2"));	
	// 변수 및 HANDLE 초기화
	OnMainVarDefaultSet();
	st_work_info.nPgmInfo = 0;
	
	//kwlee 2016.0323 추후 변경 예정
	//CTL_Lib.Initialize_motor_board(1,  _T("C:\\AMT_RT200\\Setting\\AMT.cme2"));			// MOTOR 보드 초기화 여부 검사한다
	//////////////////////

	clsMyJamData.OnAlarmInfoLoad();	// 파일에 저장된 모든 알람 정보 전역 변수에 설정하는 함수

	// I/O Board Initialize
	if (FAS_IO.Initialize_Board() == RET_GOOD)
	{
		FAS_IO.Initialize_Map();
	}
	//kwlee 2017.0216
	CTL_Lib.Initialize_motor_board(1,  _T("C:\\AMT_RT200\\Setting\\Label.cme2"));	
	//

	// jtkim 20150721
//	st_basic_info.nSocketCount = 100000;
	st_work_info.nCycleYesNo		= NO;
	st_work_info.nTsiteCycleYesNo	= YES;
	st_handler_info.nInitialSuccess = NO;
	st_handler_info.nInitialError = NO;
	st_sync_info.nConvTurnCheck = FALSE; //kwlee 2017.0220

	//kwlee 2017.0216 프로그램 시작 시 Turn 방지
	FAS_IO.set_out_bit(st_io_info.o_TableTurnCylFor,IO_ON);
	FAS_IO.set_out_bit(st_io_info.o_TableTurnCylBack,IO_ON);

	FAS_IO.set_out_bit(st_io_info.o_InConvPosStopperDw,IO_ON);
	FAS_IO.set_out_bit(st_io_info.o_InConvPosStopperUp,IO_OFF);

	FAS_IO.set_out_bit(st_io_info.o_TurnPosDw,IO_ON);
	FAS_IO.set_out_bit(st_io_info.o_TurnPosUp,IO_OFF);

	FAS_IO.set_out_bit(st_io_info.o_OutPosStopperDw,IO_ON);
	FAS_IO.set_out_bit(st_io_info.o_OutPosStopperUp,IO_OFF);
	//kwlee 2017.0310
	FAS_IO.set_out_bit(st_io_info.o_LabelStopperCylinder,IO_OFF);
	////
	
	//kwle 2017.0317
	for(int i =0; i<PICKCNT; i++)
	{
		FAS_IO.set_out_bit(st_io_info.o_HeadCylUpDn[i],IO_OFF);
	}

	st_count_info.nHourSocket = st_count_info.nSocketStart;
	if (st_count_info.nHourSocket <= 0)
	{
		st_count_info.nHourSocket = 0;
		st_count_info.dHourPer = 0.0f;
	}
	else
	{
		st_count_info.dHourPer = ((double)st_count_info.nHourSocket / (double)48) * (double)100;
	}
}

CMainFrame::~CMainFrame()
{
	int i;

	for (i=0; i<20; i++)
	{
		if (m_pClient[i] != NULL)
		{
			delete m_pClient[i];
			m_pClient[i] = NULL;
		}


		if (m_pServer[i] != NULL)
		{
			delete m_pServer[i];
			m_pServer[i] = NULL;
		}
	}

}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	st_handler_info.hWnd = GetSafeHwnd();  // 메인 프레임 핸들 설정

	//메모리 DC에 사용할 Background Bitmap
	st_handler_info.m_TitleBackBitmap.LoadBitmap(/*IDB_BITMAP_BACK_GROUND1*/IDB_BITMAP_TITLE_BACKGROUND); 
	st_handler_info.m_BackBitmap.LoadBitmap(/*IDB_BITMAP_BACK_GROUND1*/IDB_BITMAP_BACK_GROUND);   
	st_handler_info.m_BackDlgBitmap.LoadBitmap(/*IDB_BITMAP_BACK_GROUND1*/IDB_BITMAP_DLG_BACK_GROUND);   
		
	m_wndToolBar.SetBorders(1, 1, 1, 1);
	if (!m_wndToolBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	m_wndToolBar.ModifyStyle(0, TBSTYLE_FLAT);

	m_wndToolBar.LoadTrueColorToolBar(TOOLBAR_DRAW_BUTTON_WIDTH,
									  IDB_TOOLBAR_DRAW,
		                              IDB_TOOLBAR_DRAW_HOT,
								      IDB_TOOLBAR_DRAW_DISABLED);

	m_wndToolBar.EnableDocking(CBRS_ALIGN_TOP);
	EnableDocking(CBRS_ALIGN_TOP);

//	m_wndToolBar.AddDropDownButton(this, ID_PGMINFO, IDR_MENU_YASUNAGA);

	// **************************************************************************
    // 타이틀 정보 출력 Initial Dialog bar 생성
    // **************************************************************************
	if (!m_wndTitleBar.Create(this, IDD_DIALOG_TITLE, CBRS_ALIGN_TOP, IDD_DIALOG_TITLE))
	{
		TRACE0("Failed to create my init bar\n");
		return -1;      // fail to create
	}
	st_handler_info.cWndTitle = &m_wndTitleBar;
	//==============================================================//
	// 메시지 출력용 리스트바 생성
	// : 상태 바 밑에 위치하도록 하려면 이 부분을 상태 바 생성 부분 위로 올린다
	//==============================================================//
	//if (!m_wndListBar.Create(this, IDD_DIALOG_LIST, CBRS_ALIGN_BOTTOM, IDD_DIALOG_LIST))
	if (!m_wndListBar.Create(this, IDD_DIALOG_LIST,
		WS_CHILD | WS_VISIBLE | CBRS_ALIGN_BOTTOM | WS_BORDER | CBRS_GRIPPER, IDD_DIALOG_LIST))
	{
		TRACE0("Failed to create my list bar\n");
		return -1;      // fail to create
	}
	st_handler_info.cWndList = &m_wndListBar;
	// =============================================================================
    // 텍스트 툴바 스타일 설정 (드롭 다운 메뉴로 설정)                            //
    // =============================================================================
	m_wndToolBar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);

	OnThreadCreate();
#ifdef _WIN64
//	OnVisionInitialize();
#endif
	/* ************************************************************************** */
    /* 초기화 진행 화면으로 전환하여 장비 초기화 작업 진행한다                    */
    /* ************************************************************************** */
	::PostMessage(st_handler_info.hWnd, WM_FORM_CHANGE, 1, 3);  // 초기화 진행 화면 전환
	/* ************************************************************************** */
	
// 	SendMessage(WM_SERVER_MSG_3, SERVER_CONNECT);
// 	SendMessage(WM_SERVER_MSG_4, SERVER_CONNECT);
// 	SendMessage(WM_SERVER_MSG_8, SERVER_CONNECT);

	SendMessage(WM_SERVER_MSG_3, SERVER_CONNECT);
	//SendMessage(WM_CLIENT_MSG_3, SERVER_CONNECT);
	
	SetTimer(TM_MAIN_REFRESH, 500, NULL);  // 리스트 파일 생성 타이머
	SetTimer(TM_FILE_CREATE, 500, NULL);
	//SetTimer(TM_GMS, 200, NULL);

	if (st_handler_info.nVerUpdate == 1)
	{
		clsEcFirst.OnEqpVersionUpdate(theApp.m_strVer);
	}

	m_nXgemStep = 0;
	//SetTimer(TM_XGEM, 5000, NULL);
	//this->SendMessage(WM_SERVER_MSG_4, SERVER_CONNECT);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

//	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
//		  | WS_MAXIMIZE | WS_SYSMENU;

//	cs.style =  WS_MAXIMIZE| WS_THICKFRAME ;
	// jtkim 20150901
	//cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE | WS_SYSMENU;
	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
	cs.style &= ~FWS_ADDTOTITLE;								// 화면 타이틀 정보 제어 가능하도록 스타일 설정 
	//cs.style &= ~WS_MAXIMIZEBOX;

//	cs.lpszClass=((CHANDLERApp*)AfxGetApp())->GetClassName();	// 현재 응용 프로그램에 대한 클래스 이름을 얻는다(Need for Single Instance)

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기
LRESULT CMainFrame::OnViewChangeMode(WPARAM wParam,LPARAM lParam)
{
	if (wParam != 8)  
	{
		if (st_handler_info.nMenuLock	!= FALSE)  return 0;
		if (st_handler_info.nSystemLock	!= FALSE)  return 0;  // 현재 시스템 Lock 상태인 경우 리턴 
	}
	/* ************************************************************************** */

	switch(wParam)
	{
		case SCREEN_MAIN:
			OnMain();
			break;

		case SCREEN_INITIAL:
			OnMcInitial();
			break;

		case SCREEN_BASIC:
			OnBasic();
			break;

		case SCREEN_SETTING:
			OnSetting();
			break;

		case SCREEN_MOTOR:
			OnMotor();
			break;

		case SCREEN_IO:
			OnIo();
			break;

		case SCREEN_LIST:
			OnList();
			break;

		case SCREEN_ALARM:
			OnAlarm();
			break;

		case SCREEN_LOCK:
			OnLock();
			break;

		default:

			break;
	}

	return 0 ;
}

void CMainFrame::OnMcInitial()
{
	if ( OnMenuChangeChecking() == RET_ERROR ) return ;
	if ( GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreenInitial)) )   return;

	

	// **************************************************************************
	// 메뉴 사용 불가능한 경우에 대한 조건 검사한다                              
	// -> 메인트 모드 또는 티칭 모드가 설정되어 있는지 검사                      
	// **************************************************************************

	if (st_handler_info.nLotStatus == LOT_START) return;

	if (st_handler_info.nInitialSuccess == YES)
	{
		if (st_handler_info.nLevelTeach !=  TRUE)
		{
			if (st_handler_info.nLevelMaint !=  TRUE) 
			{
				OnLevel();
				if (st_handler_info.nLevelTeach !=  TRUE)
				{
					if (st_handler_info.nLevelMaint !=  TRUE)  return;
				}
			}
		}
	}

	OnSwitchToForm(IDW_SCREEN_INIT);
}

void CMainFrame::OnBasic()
{
	if (st_handler_info.nMachineMode	!= MACHINE_MANUAL) return;
	if ( OnMenuChangeChecking() == RET_ERROR ) return ;
	//if (st_lot_info[LOT_CURR].nLastModule != YES)	return;
	if ( GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreenBasic)) )   return;

	OnSwitchToForm(IDW_SCREEN_BASIC);
}


void CMainFrame::OnAlarm()
{
	/* ************************************************************************** */
    /* 화면 뷰 전환 불가능한 정보 검사한다.                                       */
    /* ************************************************************************** */
	if (st_handler_info.nMenuLock != FALSE)		return;
	if (st_handler_info.nSystemLock != FALSE)	return;  // 현재 시스템 Lock 상태인 경우 리턴 

	if (st_handler_info.nRunStatus == dRUN || st_handler_info.nRunStatus == dINIT) return;

	//if ( OnMenuChangeChecking() == RET_ERROR ) return ;
//	if ( OnMenuChangeChecking() == RET_ERROR ) return ;
	if ( GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreenAlarm)) )   return;

	OnSwitchToForm(IDW_SCREEN_ALARM);
}


void CMainFrame::OnDoor()
{
//	int nResponse;
//	CDialog_Door_Open dlgDoor;

	if (st_handler_info.nMachineMode	!= MACHINE_MANUAL) return;
	if ( OnMenuChangeChecking() == RET_ERROR ) return ;
	
	FAS_IO.set_out_bit(st_io_info.i_DoorLockChk,IO_OFF);

// 	nResponse = (int)dlgDoor.DoModal();
// 	if (nResponse == IDOK)
// 	{
// 	}
}

void CMainFrame::OnExit()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CDialog_Message dlgMsg;

	int i;
	int nResponse;

	if (st_handler_info.nRunStatus != dSTOP)
	{
		dlgMsg.m_nMessageType	= 0;
		dlgMsg.m_strMessage		= _T("Now Machine is working.");
		
		dlgMsg.DoModal();
		return ;
	} 

	if (st_handler_info.nSystemLock) return;

	dlgMsg.m_nMessageType	= 1;
	dlgMsg.m_strMessage		= _T("AMT_RT200 Exit the program.");

	nResponse = (int)dlgMsg.DoModal();
	if (nResponse == IDOK)
	{
		KillTimer(TM_MAIN_REFRESH);
		KillTimer(TM_FILE_CREATE);
		KillTimer(TM_GMS);
		KillTimer(TM_XGEM);

		// jtkim 20150405
// 		if (FAS_IO.get_out_bit(st_io_info.o_LdCvyMotOnOff, IO_ON) == IO_ON)
// 		{
// 			FAS_IO.set_out_bit(st_io_info.o_LdCvyMotOnOff, IO_OFF);
// 		}
// 
// 		if (FAS_IO.get_out_bit(st_io_info.o_UnldMvCvyMotOnOff, IO_ON) == IO_ON)
// 		{
// 			FAS_IO.set_out_bit(st_io_info.o_UnldMvCvyMotOnOff, IO_OFF);
// 		}
// 		
// 		if (FAS_IO.get_out_bit(st_io_info.o_UnldTrayMotOnOff, IO_ON) == IO_ON)
// 		{
// 			FAS_IO.set_out_bit(st_io_info.o_UnldTrayMotOnOff, IO_OFF);
// 		}
// 
// 		//2015.0307 smema 초기화 처리 
// 		FAS_IO.set_out_bit(st_io_info.o_FrontRequest,	IO_OFF);
// 		FAS_IO.set_out_bit(st_io_info.o_FrontComplete,	IO_OFF); 
// 		FAS_IO.set_out_bit(st_io_info.o_RearTransfer,	IO_OFF);
// 		FAS_IO.set_out_bit(st_io_info.o_RearFlipper,	IO_OFF); 
/*
		// //////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 현재 선택된 View가 NULL인 경우 마지막 View를 가져온다....
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (m_pNewActiveView == NULL)
		{
			m_pNewActiveView = GetActiveView();			// 현재 화면에 출력된 뷰 정보 설정 (save old view)
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		m_pNewActiveView->DestroyWindow();
		m_pNewActiveView = NULL;
*/
		/*clsReg.WriteCreateTime(st_handler_info.tCreate);
		clsReg.WriteMcTime(dRUN, st_handler_info.tRun);
		clsReg.WriteMcTime(dSTOP, st_handler_info.tStop);
		clsReg.WriteMcTime(dJAM, st_handler_info.tJam);
		clsReg.WriteAlarmInfo();*/

		AllStop[0] = 1;
		AllStop[1] = 1;

		if (COMI.mn_motorbd_init_end == YES)
		{
			for (i=0; i<M_MAX_MOTOR_NUM; i++)
			{
				st_work_info.dCurrPos[i] = COMI.Get_MotCurrentPos(i);
			}
		}
		st_count_info.nInCount[0][0] = 0; //kwlee 2017.0316
		OnThreadDelete();

		OnConfigSave();

		CFrameWnd::OnClose();
	}
}


void CMainFrame::OnIo()
{
	if (st_handler_info.nMachineMode	!= MACHINE_MANUAL) return;
	if ( OnMenuChangeChecking() == RET_ERROR ) return ;
	if ( GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreenIO)) )   return;

	// **************************************************************************
	// 메뉴 사용 불가능한 경우에 대한 조건 검사한다                              
	// -> 메인트 모드 또는 티칭 모드가 설정되어 있는지 검사                      
	// **************************************************************************
	if (st_handler_info.nLevelTeach !=  TRUE)
	{
		if (st_handler_info.nLevelMaint !=  TRUE) 
		{
			OnLevel();
			if (st_handler_info.nLevelTeach !=  TRUE)
			{
				if (st_handler_info.nLevelMaint !=  TRUE)  return;
			}
		}
	}

	OnSwitchToForm(IDW_SCREEN_IO_MAP);
}


void CMainFrame::OnLevel()
{
	CDialog_Pass_Check  pass_chk_dlg;
	int nResponse;

	if (st_handler_info.nMachineMode	!= MACHINE_MANUAL) return;
	if ( OnMenuChangeChecking() == RET_ERROR ) return ;
	if ( GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreenLock)) )   return;

	pass_chk_dlg.m_nPassLevel = 5;
	nResponse = (int)pass_chk_dlg.DoModal();

	if (nResponse == IDOK)
	{
		if (st_handler_info.cWndList != NULL)
		{
			//wsprintfA(st_other_info.cNormalMsg, "%S", _T("Level Change Success.."));
			//clsFunc.OnStringToChar(_T("Level Change Success.."), st_other_info.cNormalMsg);
			clsMem.OnNormalMessageWrite(_T("Level Change Success.."));
			st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}
	}
	else
	{
		if (st_handler_info.cWndList != NULL)
		{
			//wsprintfA(st_other_info.cNormalMsg, "%S", _T("Level Change Fail.."));
			//clsFunc.OnStringToChar(_T("Level Change Success.."), st_other_info.cNormalMsg);
			clsMem.OnNormalMessageWrite(_T("Level Change Fail.."));
			st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}
	}
}


void CMainFrame::OnList()
{
	if (st_handler_info.nMachineMode	!= MACHINE_MANUAL) return;
	if ( OnMenuChangeChecking() == RET_ERROR ) return ;
	if ( GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreenList)) )   return;

	OnSwitchToForm(IDW_SCREEN_LIST);
}

void CMainFrame::OnLock()
{
	CDialog_Pass_Check  pass_chk_dlg;
	int nResponse;

	if (st_handler_info.nMachineMode	!= MACHINE_MANUAL) return;
	if ( OnMenuChangeChecking() == RET_ERROR ) return ;
	if ( GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreenLock)) )   return;

	pass_chk_dlg.m_nPassLevel = 2;
	nResponse = (int)pass_chk_dlg.DoModal();

	if (nResponse == IDOK)
	{
		//::PostMessage(st_handler_info.hWnd, WM_FORM_CHANGE, SCREEN_LOCK, 2);  // 메인 프레임에 SYSTEM LOCK 화면 출력 요청
		OnSwitchToForm(IDW_SCREEN_LOCK);

		if (st_handler_info.cWndList != NULL)
		{
			//wsprintfA(st_other_info.cNormalMsg, "%S", _T("System Locking."));
			//clsFunc.OnStringToChar(_T("System Locking"), st_other_info.cNormalMsg);
			clsMem.OnNormalMessageWrite(_T("System Locking.."));
			st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}
	}
}

void CMainFrame::OnMain()
{
	if ( OnMenuChangeChecking() == RET_ERROR ) return ;
	if ( GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreenMain)) )   return;

	OnSwitchToForm(IDW_SCREEN_MAIN);
}


void CMainFrame::OnMenu()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CMainFrame::OnMotor()
{
	if (st_handler_info.nMachineMode	!= MACHINE_MANUAL) return;
	if ( OnMenuChangeChecking() == RET_ERROR ) return ;
	if ( GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreenMotor)) )   return;

	// **************************************************************************
	// 메뉴 사용 불가능한 경우에 대한 조건 검사한다                              
	// -> 메인트 모드 또는 티칭 모드가 설정되어 있는지 검사                      
	// **************************************************************************
	if (st_handler_info.nLevelTeach !=  TRUE)
	{
		if (st_handler_info.nLevelMaint !=  TRUE) 
		{
			OnLevel();
			if (st_handler_info.nLevelTeach !=  TRUE)
			{
				if (st_handler_info.nLevelMaint !=  TRUE)  return;
			}
		}
	}

	OnSwitchToForm(IDW_SCREEN_MOTOR);
}


//void CMainFrame::OnPgminfo()
//{
//	if ( OnMenuChangeChecking() == RET_ERROR ) return ;
//	if ( GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreenPrgInfo)) )   return;
//
//	OnSwitchToForm(IDW_SCREEN_PGM_INFO);
//}


void CMainFrame::OnReset()
{
	CDialog_Pass_Check  pass_chk_dlg;

	int nResponse;

	if ( OnMenuChangeChecking() == RET_ERROR ) return ;
	if ( GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreenInitial)) )   return;

	// **************************************************************************
	// 메뉴 사용 불가능한 경우에 대한 조건 검사한다                              
	// -> 메인트 모드 또는 티칭 모드가 설정되어 있는지 검사                      
	// **************************************************************************
	if (st_lot_info[LOT_CURR].nLotStatus == LOT_START || st_lot_info[LOT_CURR].nLotStatus == LOT_END_START) return;
	if (st_lot_info[LOT_NEXT].nLotStatus == LOT_START || st_lot_info[LOT_NEXT].nLotStatus == LOT_END_START) return;

	
	pass_chk_dlg.m_nPassLevel = 7;
	nResponse = (int)pass_chk_dlg.DoModal();

	if (nResponse == IDOK)
	{
		OnSwitchToForm(IDW_SCREEN_INIT);
	}
}


void CMainFrame::OnSave()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CDialog_Message  dlgMsg;
	int nResponse;
	if (st_handler_info.nMenuLock	!= FALSE)  return;
	if (st_handler_info.nSwitchLock	!= FALSE)  return;  // 현재 시스템 Lock 상태인 경우 리턴 
	
	dlgMsg.m_nMessageType	= 1;
	dlgMsg.m_strMessage		= _T("Do you want to save the changed information?");

	nResponse = (int)dlgMsg.DoModal();
	
	if(nResponse == IDOK)
	{
		st_handler_info.nMenuLock = TRUE;
		
		OnConfigSave();					// 화면에 설정된 모든 데이터 저장 함수
		
		st_handler_info.nMenuLock = FALSE;
	}
}


void CMainFrame::OnSetting()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (st_handler_info.nMachineMode	!= MACHINE_MANUAL) return;
	if ( OnMenuChangeChecking() == RET_ERROR ) return ;
	if ( GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreenWork)) )   return;

	// **************************************************************************
	// 메뉴 사용 불가능한 경우에 대한 조건 검사한다                              
	// -> 메인트 모드 또는 티칭 모드가 설정되어 있는지 검사                      
	// **************************************************************************
	if (st_handler_info.nLevelTeach !=  TRUE)
	{
		if (st_handler_info.nLevelMaint !=  TRUE) 
		{
			OnLevel();
			if (st_handler_info.nLevelTeach !=  TRUE)
			{
				if (st_handler_info.nLevelMaint !=  TRUE)  return;
			}
		}
	}

	OnSwitchToForm(IDW_SCREEN_WORK_INFO);
}


void CMainFrame::OnVer()
{
	if (st_handler_info.nMenuLock != FALSE)	
	{
		st_handler_info.nMenuLock = FALSE;	// 메뉴 사용 못하도록 설정
	}
	CDialog_About dlgABOUT;
	int nResponse;
	
	if (st_handler_info.nRunStatus != dSTOP) return;

	nResponse = (int)dlgABOUT.DoModal();
	if (nResponse == IDOK)
	{
	}
}

void CMainFrame::OnSwitchToForm(int nForm)
{
	if (st_handler_info.nMenuLock	!= FALSE) return;
	//if (st_handler_info.nSystemLock	!= FALSE) return;		// 현재 시스템 Lock 상태인 경우 리턴 

	m_pOldActiveView = GetActiveView();			// 현재 화면에 출력된 뷰 정보 설정 (save old view)

	m_pNewActiveView = (CView*)GetDlgItem(nForm);  // 새로 출력할 뷰 정보 설정 (get new view)
	
	if(m_pNewActiveView == NULL)							// 새로 출력할 뷰가 아직 출력되지 않았는지 검사 (if it hasn't been created yet)
	{
		switch(nForm)			// 뷰에 설정된 ID 정보 비교 (these IDs are the dialog IDs of the view but can use anything)
		{
			case IDW_SCREEN_MAIN:				// 메인 화면 
				m_pNewActiveView = (CView*)new CScreenMain;
				break;

			case IDW_SCREEN_INIT:
				m_pNewActiveView = (CView*)new CScreenInitial;
				break;

			case IDW_SCREEN_BASIC:
				m_pNewActiveView = (CView*)new CScreenBasic;
				break;

			case IDW_SCREEN_WORK_INFO:
				m_pNewActiveView = (CView*)new CScreenWork;
				break;

			case IDW_SCREEN_SET_MAINTENANCE:
				break;

			case IDW_SCREEN_SET_INTERFACE:
				break;

			case IDW_SCREEN_SET_MEASURE:
				break;

			case IDW_SCREEN_WAIT_TIME:
				break;

			case IDW_SCREEN_MOTOR:
				m_pNewActiveView = (CView*)new CScreenMotor;
				break;

			case IDW_SCREEN_MOTOR_SPEED:
				break;

			case IDW_SCREEN_LIST:
				m_pNewActiveView = (CView*)new CScreenList;
				break;

			case IDW_SCREEN_LIST_WORK:
				break;

			case IDW_SCREEN_LIST_ALARM:
				break;

			case IDW_SCREEN_LIST_STEP:
				break;

			case IDW_SCREEN_LIST_ERROR:
				break;

			case IDW_SCREEN_IO_MAP:
				m_pNewActiveView = (CView*)new CScreenIO;
				break;

			case IDW_SCREEN_LOCK:
				m_pNewActiveView = (CView*)new CScreenLock;
				break;

			case IDW_SCREEN_ALARM:
				m_pNewActiveView = (CView*)new CScreenAlarm;
				break;

			case IDW_SCREEN_PGM_INFO:
				m_pNewActiveView = (CView*)new CScreenPrgInfo;
				break;
		}

		CCreateContext context;  // 새로운 뷰에 도큐먼트 Attach ( attach the document to the new view)
		context.m_pCurrentDoc = m_pOldActiveView->GetDocument();

		m_pNewActiveView->Create(NULL, NULL, 0L, CFrameWnd::rectDefault, this, nForm, &context);
		m_pNewActiveView->OnInitialUpdate();  		
	}

	SetActiveView(m_pNewActiveView);			// 새로 출력된 뷰를 액티브 뷰로 설정 (change the active view)
	m_pNewActiveView->ShowWindow(SW_SHOW);	// 새로운 뷰를 출력 (show the new window)

	m_pOldActiveView->ShowWindow(SW_HIDE);	// 기존에 출력된 뷰를 감춘다 (hide the old)
	m_pOldActiveView->DestroyWindow();		// 기존에 출력된 뷰를 파괴시킨다 (Old View Destroy 시키기 위하여 추가)
	m_pOldActiveView = NULL;					// 기존에 출력된 뷰에 대한 정보 저장 변수 초기화 

	::SetWindowLong(m_pNewActiveView->m_hWnd, GWL_ID, AFX_IDW_PANE_FIRST);  // gotta have it
	RecalcLayout();							// 프레임을 조율한다 (adjust frame)
	
	delete	m_pOldActiveView;					// 기존에 출력된 뷰 정보를 제거한다 (kill old view)
	
	st_handler_info.nMenuNum = nForm;

	/* ************************************************************************** */
	/* 현재 화면 정보 타이틀바에 출력한다.                                        */
	/* ************************************************************************** */
	if (st_handler_info.cWndTitle != NULL)
	{
		st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, FORM_NAME, nForm);
		st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, MODEL_MODE, 0);
//		st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, INTERFACE_MODE, st_basic_info.nModeInterface);
		st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, INTERFACE_MODE, st_handler_info.nFtpVer);
		st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, XGEM_MODE, clsXgem.m_nXgemInterface);
	}
	/* ************************************************************************** */
}


int CMainFrame::OnMenuChangeChecking(void)
{
	/* ************************************************************************** */
    /* 화면 뷰 전환 불가능한 정보 검사한다.                                       */
    /* ************************************************************************** */
	if (st_handler_info.nMenuLock != FALSE)		return RET_ERROR;
	if (st_handler_info.nSystemLock != FALSE)	return RET_ERROR;  // 현재 시스템 Lock 상태인 경우 리턴 

	/* ************************************************************************** */
	/* 현재 장비 상태가 STOP 상태인지 검사                                        */
	/* ************************************************************************** */
	if (st_handler_info.nRunStatus != dSTOP && st_handler_info.nRunStatus != dLOCK)
	{
//		sprintf(st_other_info.cOpMsg, _T("[Now RUNNING] do first STOP Switch"));
//		_snprintf(st_other_info.cNormalMsg, sizeof(st_other_info.cNormalMsg), "%S", _T("[Now RUNNING] do first STOP Switch"));
		//wsprintfA(st_other_info.cNormalMsg, "%S", _T("[Now RUNNING] do first STOP Switch"));
		
		if (st_handler_info.cWndList != NULL)  
		{
			//clsFunc.OnStringToChar(_T("[Now RUNNING] do first STOP Switch"), st_other_info.cNormalMsg);
			clsMem.OnNormalMessageWrite(_T("[Now RUNNING] do first STOP Switch"));
			st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
		}
		return RET_ERROR;
	}
	/* ************************************************************************** */

	return RET_GOOD;
}

void CMainFrame::OnMainVarDefaultSet()
{
	int i,j;
	// 핸들러 시뮬레이션 임시로 TRUE를 준다. 
	st_work_info.nSimulationMode	= 0;
	st_handler_info.nLotStatus		= LOT_CLEAR;

	st_handler_info.cWndTitle		= NULL;
	st_handler_info.cWndList		= NULL;
	st_handler_info.cWndMain		= NULL;
/*
	st_animate_info.nImgNum[ANI_PCB_LF_ELV]	= 1;
	st_animate_info.nImgNum[ANI_MGZ_ELV]	= 1;
	st_animate_info.nImgNum[ANI_JIG_IN_CVY]	= 1;
	st_animate_info.nImgNum[ANI_JIG_OUT_CVY]= 4;
	st_animate_info.nImgNum[ANI_JIG_ELV]	= 1;
	st_animate_info.nImgNum[ANI_PCB_ROBOT]	= 0;
	st_animate_info.nImgNum[ANI_MGZ_IN]		= 1;
	st_animate_info.nImgNum[ANI_MGZ_OUT]	= 1;*/
	ZeroMemory(st_other_info.nSocket, sizeof(st_other_info.nSocket));

	//kwlee 2015.1126
	for (i =0; i<2; i++)
	{
		for (j = 0; j<48; j++)
		{
			st_handler_info.m_tStopTime[i][j] = 0;
		}
		
	}

	for (i=0; i<MAX_GMS_CHANNEL; i++)
	{
		st_gms_info.ArValVolt[i].SetSize(60);
		st_gms_info.ArValRes[i].SetSize(60);
	}

	//2017.0209
		clsZebra.SetDarkness_TCP(0);
		clsZebra.LabelTop_TCP(0);
	//	clsZebra.OnPrintAnswerMode(2, 0, CLS_BCR_PRINTER1);
		clsZebra.Rewind_Mode_TCP(0);
// 	

//	char chr_data[50];

}

void CMainFrame::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return;
	CFrameWnd::OnClose();
}

void CMainFrame::OnExcelRead()
{
	clsExcel.OnOpenIOMap(st_path_info.strIoMap);
	clsExcel.OnCloseIOMap();

	clsExcel.OnOpenPartIOMap(st_path_info.strPartIoMap);
	clsExcel.OnClosePartIOMap();

	clsExcel.OnOpenMotorPart(st_path_info.strMotorPartMap);
	clsExcel.OnCloseMotorPart();

	clsExcel.OnOpenMotorPosition(st_path_info.strMotorAxisMap);
	clsExcel.OnCloseMotorPosition();

	clsExcel.OnOpenWaitTime(st_path_info.strWaitTimeMap);
	clsExcel.OnCloseWaitTime();
}


void CMainFrame::OnFilePath()
{
	CString strPath				= _T("C:\\AMT_RT200\\"); //kwlee 2016.0323
	CString strCreate;

	clsFunc.OnCreateFolder(strPath);

	st_path_info.strBasic			=	strPath + _T("Setting\\");				// 환경 설정 정보 저장 파일에 대한 폴더 설정 
	clsFunc.OnCreateFolder(st_path_info.strBasic);
	
	st_path_info.strPathDvc			= strPath + _T("File\\");					// 디바이스별 티칭 정보 저장 파일 생성 폴더 설정
	clsFunc.OnCreateFolder(st_path_info.strPathDvc);
	
	st_path_info.strPathAlarm		= strPath + _T("Bmp\\");					// 출력 알람 정보 존재 폴더 설정
	clsFunc.OnCreateFolder(st_path_info.strPathAlarm);
	
	st_path_info.strPathOp			= strPath + _T("Operator\\");				// 오퍼레이터 정보 저장 폴더 설정
	clsFunc.OnCreateFolder(st_path_info.strPathOp);

	st_path_info.strPathBack		= strPath + _T("BackUp\\");					// BACKUP 파일 생성 폴더 설정
	clsFunc.OnCreateFolder(st_path_info.strPathBack);

	st_path_info.strFileBasic		= strPath + _T("File\\File.TXT");			// 기본 셋팅 정보 저장 파일명 설정
	st_path_info.strFileWait		= strPath + _T("WaitTime");					// 대기 시간 정보 저장 파일명 설정

	st_path_info.strOperation		= strPath + _T("Log\\Operation\\");			// Operation Log 파일에 대한 폴더 설정 
	clsFunc.OnCreateFolder(st_path_info.strOperation);

	st_path_info.strMachine			= strPath + _T("Log\\Machine\\");				// Machine Log 파일에 대한 폴더 설정 
	clsFunc.OnCreateFolder(st_path_info.strMachine);
	
	st_path_info.strTotal			= strPath + _T("Log\\Total\\");				// Total Log 파일에 대한 폴더 설정 
	clsFunc.OnCreateFolder(st_path_info.strTotal);

	st_path_info.strData			= strPath + _T("Log\\Data\\");				// Total Log 파일에 대한 폴더 설정 
	clsFunc.OnCreateFolder(st_path_info.strData);

	st_path_info.strInterface		= strPath + _T("Log\\Interface\\");	
	clsFunc.OnCreateFolder(st_path_info.strInterface);

	st_path_info.strBarcode		= strPath + _T("Log\\Barcode\\");	
	clsFunc.OnCreateFolder(st_path_info.strBarcode);

	st_path_info.strBarcodeNg		= strPath + _T("Log\\BarcodeNg\\");	
	clsFunc.OnCreateFolder(st_path_info.strBarcodeNg);

	st_path_info.strFileMotor		= strPath + _T("Motor\\");
	clsFunc.OnCreateFolder(st_path_info.strFileMotor);

	st_path_info.strAlarmMonth		= strPath + _T("Alarm\\Monthly\\");
	clsFunc.OnCreateFolder(st_path_info.strAlarmMonth);

	st_path_info.strAlarmDay		= strPath + _T("Alarm\\Daily\\");
	clsFunc.OnCreateFolder(st_path_info.strAlarmDay);

	strCreate						= strPath + _T("Excel\\");
	clsFunc.OnCreateFolder(strCreate);

	// jtkim 20150607
	st_path_info.strPathLotWork		= strPath + _T("Log\\LOT_DATA\\WORK\\");	
	clsFunc.OnCreateFolder(st_path_info.strPathLotWork);

	st_path_info.strPathLotPcb		= strPath + _T("Log\\LOT_DATA\\RESULT\\");	
	clsFunc.OnCreateFolder(st_path_info.strPathLotPcb);

	strCreate = strPath + _T("Log\\LOT_DATA\\LD_TRAY\\");
	clsFunc.OnCreateFolder(strCreate);

	strCreate = strPath + _T("Log\\LOT_DATA\\LD_ROBOT\\");
	clsFunc.OnCreateFolder(strCreate);

	strCreate = strPath + _T("Log\\LOT_DATA\\LD_BUFFER\\");
	clsFunc.OnCreateFolder(strCreate);

	strCreate = strPath + _T("Log\\LOT_DATA\\UNLD_ROBOT\\");
	clsFunc.OnCreateFolder(strCreate);

	strCreate = strPath + _T("Log\\LOT_DATA\\UNLD_BUFFER\\");
	clsFunc.OnCreateFolder(strCreate);

	strCreate = strPath + _T("Log\\LOT_DATA\\TEST_SITE_ROBOT\\");
	clsFunc.OnCreateFolder(strCreate);

	strCreate = strPath + _T("Log\\LOT_DATA\\UNLD_TRAY\\");
	clsFunc.OnCreateFolder(strCreate);

	st_path_info.strPathCycle		= strPath + _T("Log\\LOT_DATA\\");	
	clsFunc.OnCreateFolder(st_path_info.strPathCycle);

	st_path_info.strPathTestSite	= strPath + _T("Log\\LOT_DATA\\TEST_SITE_CYCLE\\");	
	clsFunc.OnCreateFolder(st_path_info.strPathTestSite);

	st_path_info.strIoMap			= strPath + _T("Excel\\AMT_IO_MAP.xls");
	st_path_info.strPartIoMap		= strPath + _T("Excel\\AMT_IO_PART_MAP.xls");
	st_path_info.strIoPartMap		= strPath + _T("Excel\\AMT_IO_MAP.xls");
	st_path_info.strMotorPartMap	= strPath + _T("Excel\\AMT_MOTOR_PART_MAP.xls");
	st_path_info.strMotorAxisMap	= strPath + _T("Excel\\AMT_MOTOR_AXIS_MAP.xls");
	st_path_info.strWaitTimeMap		= strPath + _T("Excel\\AMT_WAITTIME_MAP.xls");

	// jtkim 20150408
	//st_path_info.strPathRecipe		= _T("D:\\XGEM\\AMT8562\\AMT8562\\AMT8562\\XWork\\Recipe\\");

	// 20140811 jtkim
	st_path_info.strTool			= strPath + _T("Tool.Ini");

	st_path_info.strPathRecovery	= strPath + _T("Recovery\\");
	clsFunc.OnCreateFolder(st_path_info.strPathRecovery);
}

void CMainFrame::OnConfigSave()
{
	/* **************************************************************************** */
	/* 화면에 설정된 모든 데이터 파일에 저장한다										*/
	/* -> 프로그램 종료 시점에서 오퍼레이터가 데이터 저장 파일 삭제하는 경우 대비		*/
	/* -> 장비 동작 중에 파일 삭제되더라도 현재 설정 정보 유지된다						*/
	/* **************************************************************************** */
	clsBasic.OnBasic_Data_Save();				// 전역 변수에 저장된 Basic 셋팅 정보를 파일에 저장하는 함수

	clsBasic.OnWaitTime_Data_Save();			// 전역 변수에 저장된 Wait Time 셋팅 정보를 파일에 저장하는 함수
	clsBasic.OnMotorSpeed_Set_Data_Save();		// 전역 변수에 저장된 Motor Speed 셋팅 정보를 파일에 저장하는 함수.

	clsBasic.OnModuleRobot_Teach_Data_Save();
	clsBasic.OnMaintenance_Data_Save();
	clsBasic.OnInterface_Data_Save();
	clsBasic.OnRecipe_Data_Save();					// 레시피 설정 정보를 파일에 저장
	clsBasic.OnRecoveryDataSave();
}

void CMainFrame::OnConfigLoad()
{
	/* **************************************************************************** */
	/* 화면에 설정된 모든 데이터 파일에 읽는다										*/
	/* -> 프로그램 종료 시점에서 오퍼레이터가 데이터 저장 파일 삭제하는 경우 대비		*/
	/* -> 장비 동작 중에 파일 삭제되더라도 현재 설정 정보 유지된다						*/
	/* **************************************************************************** */
/*
	int nNum = 0;
	// pcb left elevator
	// 거리
	st_animate_info.nDistance[ANI_PCB_LF_ELV]		= 100;
	// 현재 위치
	st_animate_info.dMovePos[ANI_PCB_LF_ELV]		= 0.0;
	// motor 최대위치
	st_animate_info.dMaxPos[ANI_PCB_LF_ELV]			= 200.0;
	// image 번호
	st_animate_info.nImgNum[ANI_PCB_LF_ELV]			= 0;

	// magazine elevator
	// 거리
	st_animate_info.nDistance[ANI_MGZ_ELV]			= 100;
	// 현재 위치
	st_animate_info.dMovePos[ANI_MGZ_ELV]			= 0.0;
	// motor 최대위치
	st_animate_info.dMaxPos[ANI_MGZ_ELV]			= 200.0;
	// image 번호
	st_animate_info.nImgNum[ANI_MGZ_ELV]			= 0;

	// pcb shuttle
	// 거리
	st_animate_info.nDistance[ANI_PCB_SHUTT]		= 100;
	// 현재 위치
	st_animate_info.dMovePos[ANI_PCB_SHUTT]			= 0.0;
	// motor 최대위치
	st_animate_info.dMaxPos[ANI_PCB_SHUTT]			= 200.0;
	// image 번호
	st_animate_info.nImgNum[ANI_PCB_SHUTT]			= 0;

	// pcb robot
	// 거리
	st_animate_info.nDistance[ANI_PCB_ROBOT]		= 100;
	// 현재 위치
	st_animate_info.dMovePos[ANI_PCB_ROBOT]			= 0.0;
	// motor 최대위치
	st_animate_info.dMaxPos[ANI_PCB_ROBOT]			= 200.0;
	// image 번호
	st_animate_info.nImgNum[ANI_PCB_ROBOT]			= 0;

	// jig in conveyor
	// 거리
	st_animate_info.nDistance[ANI_JIG_IN_CVY]		= 100;
	// 현재 위치
	st_animate_info.dMovePos[ANI_JIG_IN_CVY]		= 0.0;
	// motor 최대위치
	st_animate_info.dMaxPos[ANI_JIG_IN_CVY]			= 200.0;
	// image 번호
	st_animate_info.nImgNum[ANI_JIG_IN_CVY]			= 1;

	// jig out conveyor
	// 거리
	st_animate_info.nDistance[ANI_JIG_OUT_CVY]		= 100;
	// 현재 위치
	st_animate_info.dMovePos[ANI_JIG_OUT_CVY]		= 0.0;
	// motor 최대위치
	st_animate_info.dMaxPos[ANI_JIG_OUT_CVY]		= 200.0;
	// image 번호
	st_animate_info.nImgNum[ANI_JIG_OUT_CVY]		= 0;

	// jig elevator
	// 거리
	st_animate_info.nDistance[ANI_JIG_ELV]			= 100;
	// 현재 위치
	st_animate_info.dMovePos[ANI_JIG_ELV]			= 0.0;
	// motor 최대위치
	st_animate_info.dMaxPos[ANI_JIG_ELV]			= 200.0;
	// image 번호
	st_animate_info.nImgNum[ANI_JIG_ELV]			= 0;

	// jig elevator
	// 거리
	st_animate_info.nDistance[ANI_JIG_ROBOT]		= 100;
	// 현재 위치
	st_animate_info.dMovePos[ANI_JIG_ROBOT]			= 0.0;
	// motor 최대위치
	st_animate_info.dMaxPos[ANI_JIG_ROBOT]			= 200.0;
	// image 번호
	st_animate_info.nImgNum[ANI_JIG_ROBOT]			= 0;
*/
	clsBasic.OnBasic_Data_Load(0);				// 전역 변수에 저장된 Basic 셋팅 정보를 파일에 읽는 함수

	clsBasic.OnWaitTime_Data_Load();			// 전역 변수에 저장된 Wait Time 셋팅 정보를 파일에 읽는 함수
	clsBasic.OnMotorSpeed_Set_Data_Load();		// 전역 변수에 저장된 Motor Speed 셋팅 정보를 파일에 읽는 함수.

	clsBasic.OnModuleRobot_Teach_Data_Load();
	clsBasic.OnMaintenance_Data_Load();
	clsBasic.OnInterface_Data_Load();
	clsBasic.OnRecipe_Data_Load();
	clsBasic.OnRecoveryDataLoad();
	clsBasic.OnAnimateDataLoad();
}

void CMainFrame::OnThreadDelete()
{
	while(WaitForSingleObject(m_thrHandle[0], 5000) != WAIT_OBJECT_0)
	{
		if (m_thrHandle[0] == NULL) break;
		Sleep(1);
	}

	while(WaitForSingleObject(m_thrHandle[1], 5000) != WAIT_OBJECT_0)
	{
		if (m_thrHandle[1] == NULL) break;
		Sleep(1);
	}

	while(WaitForSingleObject(m_thrHandle[2], 5000) != WAIT_OBJECT_0)
	{
		if (m_thrHandle[2] == NULL) break;
		Sleep(1);
	}

	while(WaitForSingleObject(m_thrHandle[3], 5000) != WAIT_OBJECT_0)
	{
		if (m_thrHandle[3] == NULL) break;
		Sleep(1);
	}

	while(WaitForSingleObject(m_thrHandle[4], 5000) != WAIT_OBJECT_0)
	{
		if (m_thrHandle[4] == NULL) break;
		Sleep(0);
	}

	while(WaitForSingleObject(m_thrHandle[5], 5000) != WAIT_OBJECT_0)
	{
		if (m_thrHandle[5] == NULL) break;
		Sleep(0);
	}

	while(WaitForSingleObject(m_thrHandle[6], 5000) != WAIT_OBJECT_0)
	{
		if (m_thrHandle[6] == NULL) break;
		Sleep(0);
	}

	while(WaitForSingleObject(m_thrHandle[7], 5000) != WAIT_OBJECT_0)
	{
		if (m_thrHandle[7] == NULL) break;
		Sleep(0);
	}

	while(WaitForSingleObject(m_thrHandle[8], 5000) != WAIT_OBJECT_0)
	{
		if (m_thrHandle[8] == NULL) break;
		Sleep(0);
	}

	while(WaitForSingleObject(m_thrHandle[9], 5000) != WAIT_OBJECT_0)
	{
		if (m_thrHandle[9] == NULL) break;
		Sleep(0);
	}

}


void CMainFrame::OnThreadCreate()
{
	for(int i=0; i<10; i++)	
	{
		m_thread[i] = NULL;  // 쓰레드 핸들 초기화
	}

	m_thread[0] = AfxBeginThread(OnThreadReadIO, this);  // I/O 입출력 상태 읽기 쓰레드
	if (m_thread[0] != NULL)	
	{
		m_thrHandle[0] = m_thread[0]->m_hThread;
	}
	else	
	{
		AfxMessageBox(_T("Cannot begin the thread [thrReadIO]"));
	}

	m_thread[1] = AfxBeginThread(OnThreadHandlerCheck, this);  // 타워 램프 상태 변경 쓰레드
	if (m_thread[1] != NULL)	
	{
		m_thrHandle[1] = m_thread[1]->m_hThread;
	}
	else	
	{
		AfxMessageBox(_T("Cannot begin the thread [OnThreadHandlerCheck]"));
	}

	m_thread[2] = AfxBeginThread(OnThreadTowerLamp, this);  // 타워 램프 상태 변경 쓰레드
	if (m_thread[2] != NULL)	
	{
		m_thrHandle[2] = m_thread[2]->m_hThread;
	}
	else	
	{
		AfxMessageBox(_T("Cannot begin the thread [thrTowerLamp]"));
	}

	m_thread[3] = AfxBeginThread(OnThreadAlarmDisplay, this);  // 알람 출력 쓰레드
	if (m_thread[3] != NULL)	
	{
		m_thrHandle[3] = m_thread[3]->m_hThread;
	}
	else	
	{
		AfxMessageBox(_T("Cannot begin the thread [thrAlarmDisplay]"));
	}	

// 	m_thread[4] = AfxBeginThread(OnThreadConveyorDisplay, this);  // 알람 출력 쓰레드
// 	if (m_thread[4] != NULL)	
// 	{
// 		m_thrHandle[4] = m_thread[4]->m_hThread;
// 	}
// 	else	
// 	{
// 		AfxMessageBox(_T("Cannot begin the thread [OnThreadConveyorDisplay]"));
// 	}	
}


void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	COleDateTime cur_time;
	COleDateTime next_time;
	COleDateTimeSpan timeSpan;
	COleDateTimeSpan timeSpanUph;
	CTime bin_time;
	int nDay;
	int nHour;
//	int nSecond;
	int nMinute;
	int nDiff[2];
	int i;


	CString strTemp;
	CString strLog,strlog_1;
	if (nIDEvent == TM_MAIN_REFRESH)
	{
 		CTime cur				= CTime::GetCurrentTime();
 		CTimeSpan diff			= cur - st_handler_info.tRef;

		st_handler_info.tRef	= cur;
		

		if (st_handler_info.nMaintMode == YES)		// 메인트 모드이면 메인트 타임만 업데이트 된다.
		{
			// jtkim 20150307
			//if (st_handler_info.nLotStatus == LOT_START)
			if (st_lot_info[LOT_CURR].nLotStatus == LOT_START)
			{
				st_handler_info.tMaint = st_handler_info.tMaint + diff;
			}
			st_handler_info.tMaint = st_handler_info.tDMaint + diff;
			
			if (st_handler_info.cWndMain != NULL)
			{
				st_handler_info.cWndMain->PostMessage(WM_WORK_DISPLAY, MAIN_TIMEINFO, dMAINT);
			}
		}
		else
		{
			switch(st_handler_info.nRunStatus)
			{			
				case dRUN:			
					if (st_handler_info.nIdleStatus == NO)
					{
						st_handler_info.tRun = st_handler_info.tRun + diff;
						// jtkim 20150529
						st_handler_info.tRunUph = st_handler_info.tRunUph + diff;

						////2015.1005
						for (int i =0; i<TSITE_SOCKET_CNT;i++)
						{
							st_DB_time.n_Db_time[RUN_TIME][i] = st_handler_info.tRun;	
						}
						///////
					}
					else
					{
						// jtkim 20150307
						//if (st_handler_info.nLotStatus == LOT_START)
						if (st_lot_info[LOT_CURR].nLotStatus == LOT_START)
						{
							// jtkim 20150307
							//if (st_lot_info.strLotNo != _T(""))
							if (st_lot_info[LOT_CURR].strLotNo != _T(""))
							{
								st_handler_info.tRun = st_handler_info.tRun + diff;
								// jtkim 20150529
								st_handler_info.tRunUph = st_handler_info.tRunUph + diff;

							}
							else
							{
								st_handler_info.tStop = st_handler_info.tStop + diff;
								////2015.1005
// 								for (i =0; i<TSITE_SOCKET_CNT;i++)
// 								{	
// 									st_handler_info.mn_InstandyBy_1[i] = CTL_NO;
// 								}
								/////////////
							}
						}
						else
						{
							// jtkim 20150307
							//if (st_handler_info.nLotStatus == LOT_START)
							if (st_lot_info[LOT_CURR].nLotStatus == LOT_START)
							{
								st_handler_info.tStop = st_handler_info.tStop + diff;
							}
							st_handler_info.tDStop = st_handler_info.tDStop + diff;
						}

					}
					break;

				case dWARNING:
				case dJAM:		
					// jtkim 20150307
					//if (st_handler_info.nLotStatus == LOT_START)
					if (st_lot_info[LOT_CURR].nLotStatus == LOT_START)
					{
						st_handler_info.tJam = st_handler_info.tJam + diff;
					}
					st_handler_info.tDJam = st_handler_info.tDJam + diff;

					break;

				case dLOCK:
					// jtkim 20150307
					//if (st_handler_info.nLotStatus == LOT_START)
					if (st_lot_info[LOT_CURR].nLotStatus == LOT_START)
					{
						st_handler_info.tMaint = st_handler_info.tMaint + diff;
					}
					st_handler_info.tDMaint = st_handler_info.tDMaint + diff;
					break;
			
				case dLOTEND:												// Lot End 시에도 Stop으로 시간을 올린다.
				case dSTOP:
					// jtkim 20150307
					//if (st_handler_info.nLotStatus == LOT_START)
					if (st_lot_info[LOT_CURR].nLotStatus == LOT_START)
					{
						// jtkim 20150307
						//if (st_lot_info.strLotNo != _T(""))
						if (st_lot_info[LOT_CURR].strLotNo != _T(""))
						{
							st_handler_info.tStop = st_handler_info.tStop + diff;
						}
					}
					st_handler_info.tDStop = st_handler_info.tDStop + diff;
					break;
				}

				// jtkim 20130627
				if (st_handler_info.cWndMain != NULL)
				{
					st_handler_info.cWndMain->PostMessage(WM_WORK_DISPLAY, MAIN_TIMEINFO, st_handler_info.nRunStatus);
				}
		}
	}
	else if (nIDEvent == TM_FILE_CREATE)
	{
		cur_time = COleDateTime::GetCurrentTime();
	
		if (cur_time.GetSecond() == 0)
		{
			timeSpan = cur_time - st_handler_info.tCreate;

			nDay	= (int)timeSpan.GetTotalDays();

			if (nDay >= 1)
			{
				if (cur_time.GetMinute() == 0)
				{
					//st_handler_info.tCreate.SetDateTime(st_handler_info.tCreate.GetYear(), st_handler_info.tCreate.GetMonth(), st_handler_info.tCreate.GetDay(), 22, 0, 0);
					next_time.SetDateTime(st_handler_info.tCreate.GetYear(), st_handler_info.tCreate.GetMonth(), st_handler_info.tCreate.GetDay(), 22, 0, 0); 
					st_handler_info.tCreate = next_time + COleDateTimeSpan(1, 0, 0, 0);
					// Daily Data 저장
					clsFunc.OnDailyDataSave();
					// 현재Main 화면에 표시되는 시간 초기화..
					clsFunc.OnTimeReset(); 
				}
			}

			timeSpanUph = cur_time - st_handler_info.tUph;
			nHour	= (int)timeSpanUph.GetTotalHours();
			if (nHour >= 1)
			{
				// Daily Data 저장
				if (cur_time.GetMinute() == 0)
				{
					st_handler_info.tUph.SetDateTime(cur_time.GetYear(), cur_time.GetMonth(), cur_time.GetDay(), cur_time.GetHour(), 0, 0);

					if (cur_time.GetHour() == 23)
					{
						// jtkim 20150721
						st_count_info.nDailyUph = 0;
						st_count_info.nUphCnt	= 0;
						st_count_info.dDailyPer = 0.0f;

						next_time = cur_time + COleDateTimeSpan(1, 0, 0, 0);
						st_handler_info.strHourUph.Format(_T("HOUR_DATA%04d%02d%02d"), next_time.GetYear(), next_time.GetMonth(), next_time.GetDay());
					}
					else
					{
						if (st_handler_info.strHourUph == _T(""))
						{
							st_handler_info.strHourUph.Format(_T("HOUR_DATA%04d%02d%02d"), cur_time.GetYear(), cur_time.GetMonth(), cur_time.GetDay());
						}
					}

					clsFunc.OnHourDataSave(st_handler_info.strHourUph, 
										   st_count_info.nUph,
										   st_count_info.dHourPer,
										   st_count_info.nSocketStart, 
										   st_count_info.nSocketOn,
										   st_count_info.nSocketOff,
										   st_work_info.strSocketOff,
										   st_work_info.strSocketOn);

					st_count_info.nUph = 0;
					//jtkim 20160111
					st_count_info.nSocketStart	= 0;
					st_count_info.nSocketOn		= 0;
					st_count_info.nSocketOff	= 0;
					st_count_info.dHourPer		= 0.0f;

					st_work_info.strSocketOff	= _T("");
					st_work_info.strSocketOn	= _T("");

					st_handler_info.tRunUph = 0;
				}
			}

			timeSpan = cur_time - m_tGms;
			nMinute = (int)timeSpan.GetTotalMinutes();
			if (st_basic_info.nVnRTime <= 0) return;

			nDiff[0] = nMinute % (st_basic_info.nVnRTime);
			if (nDiff[0] == 0)
			{
			}
		}
	}
	else if (nIDEvent == TM_GMS)
	{
		//clsGms.OnCommunication();
	}
	else if (nIDEvent == TM_XGEM)
	{
		if (OnXgemInterface() == RET_GOOD)
		{
			KillTimer(TM_XGEM);
		}
	}

	CFrameWnd::OnTimer(nIDEvent);
}


void CMainFrame::OnPgminfo()
{
}


LRESULT CMainFrame::OnClientEcFirst(WPARAM wParam, LPARAM lParam)
{
	CString strMsg;

	int nLength;

	switch(wParam)
	{
		case CLIENT_CONNECT:
			if (m_pClient[EC_FIRST_NETWORK] != NULL)
			{
				if (st_client_info[EC_FIRST_NETWORK].nConnect == YES) 
				{
					st_client_info[EC_FIRST_NETWORK].nConnect = NO;

					delete m_pClient[EC_FIRST_NETWORK];
					m_pClient[EC_FIRST_NETWORK] = NULL;
				}

				m_pClient[EC_FIRST_NETWORK] = new CClientSocket();
				m_pClient[EC_FIRST_NETWORK]->Create();
	
				m_pClient[EC_FIRST_NETWORK]->OnOpen(this, EC_FIRST_NETWORK);
				if (m_pClient[EC_FIRST_NETWORK]->Connect(st_client_info[EC_FIRST_NETWORK].strIp, st_client_info[EC_FIRST_NETWORK].nPort))
				{
					delete m_pClient[EC_FIRST_NETWORK];
					m_pClient[EC_FIRST_NETWORK] = NULL;

					if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
					{
						clsMem.OnAbNormalMessagWrite(_T("[TCP/IP] EC_FIRST_NETWORK Client Connect Error."));
						st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력
					}

					return 0;
				}

				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
				{
					clsMem.OnNormalMessageWrite(_T("[TCP/IP] EC_FIRST_NETWORK Client Connect Success."));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
				}
			}
			else
			{
				st_client_info[EC_FIRST_NETWORK].nConnect = NO;

				m_pClient[EC_FIRST_NETWORK] = new CClientSocket();
				m_pClient[EC_FIRST_NETWORK]->Create();
	
				m_pClient[EC_FIRST_NETWORK]->OnOpen(this, EC_FIRST_NETWORK);
				if (m_pClient[EC_FIRST_NETWORK]->Connect(st_client_info[EC_FIRST_NETWORK].strIp, st_client_info[EC_FIRST_NETWORK].nPort))
				{
					delete m_pClient[EC_FIRST_NETWORK];
					m_pClient[EC_FIRST_NETWORK] = NULL;

					if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
					{
						clsMem.OnAbNormalMessagWrite(_T("[TCP/IP] EC_FIRST_NETWORK Client Connect Error."));
						st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력
					}
					return 0;
				}

				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
				{
					clsMem.OnNormalMessageWrite(_T("[TCP/IP] EC_FIRST_NETWORK Client Connect Success."));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
				}
			}
			break;

		case CLIENT_CLOSE:
			if (m_pClient[EC_FIRST_NETWORK] != NULL)
			{
				st_client_info[EC_FIRST_NETWORK].nConnect = NO;

				delete m_pClient[EC_FIRST_NETWORK];
				m_pClient[EC_FIRST_NETWORK] = NULL;
			}
			break;

		case CLIENT_SEND:
			if (st_client_info[EC_FIRST_NETWORK].nConnect == YES)
			{
				if (m_pClient[EC_FIRST_NETWORK] == NULL) return 0;

				clsFunc.OnStringToChar(st_client_info[EC_FIRST_NETWORK].strSend, st_client_info[EC_FIRST_NETWORK].chSend);
				nLength = st_client_info[EC_FIRST_NETWORK].strSend.GetLength();

				m_pClient[EC_FIRST_NETWORK]->Send(st_client_info[EC_FIRST_NETWORK].chSend, nLength);

				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
				{
					strMsg.Format(_T("[EC_FIRST_NETWORK] %s"), st_client_info[EC_FIRST_NETWORK].strSend);
					clsMem.OnNormalMessageWrite(strMsg);
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
				}
			}
			break;

		case CLIENT_REV:
			clsEcFirst.OnDataReceive(st_client_info[EC_FIRST_NETWORK].strRev);
			break;

		case CLIENT_ACCEPT:
			if (lParam != 0)
			{
				st_client_info[EC_FIRST_NETWORK].nConnect = NO;

				delete m_pClient[EC_FIRST_NETWORK];
				m_pClient[EC_FIRST_NETWORK] = NULL;

				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
				{
					clsMem.OnAbNormalMessagWrite(_T("[TCP/IP] EC_FIRST_NETWORK Client Accept Fail."));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력
				}

				return 0;
			}

			st_client_info[EC_FIRST_NETWORK].nConnect = YES;

			if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
			{
				clsMem.OnNormalMessageWrite(_T("[TCP/IP] EC_FIRST_NETWORK Client Accept Success."));
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
			}
			break;
	}

	return 0;
}


LRESULT CMainFrame::OnClientXgem(WPARAM wParam, LPARAM lParam)
{
	int nLength;

	switch(wParam)
	{
		case CLIENT_CONNECT:
			if (m_pClient[XGEM_NETWORK] != NULL)
			{
				//if (st_client_info[XGEM_NETWORK].nConnect == YES) m_pClient[XGEM_NETWORK]->Close();

				st_client_info[XGEM_NETWORK].nConnect = NO;

				delete m_pClient[XGEM_NETWORK];
				m_pClient[XGEM_NETWORK] = NULL;

				m_pClient[XGEM_NETWORK] = new CClientSocket();
				m_pClient[XGEM_NETWORK]->Create();
	
				m_pClient[XGEM_NETWORK]->OnOpen(this, XGEM_NETWORK);
				if (m_pClient[XGEM_NETWORK]->Connect(st_client_info[XGEM_NETWORK].strIp, st_client_info[XGEM_NETWORK].nPort))
				{
					delete m_pClient[XGEM_NETWORK];
					m_pClient[XGEM_NETWORK] = NULL;

					if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
					{
						//clsFunc.OnStringToChar(_T("[TCP/IP] Front Client Connect Error."), st_other_info.cAbnormalMsg);
						clsMem.OnAbNormalMessagWrite(_T("[TCP/IP] XGEM_NETWORK Client Connect Error."));
						st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력
					}

					return 0;
				}

				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
				{
					//clsFunc.OnStringToChar(_T("[TCP/IP] Front Client Connect Success."), st_other_info.cNormalMsg);
					clsMem.OnNormalMessageWrite(_T("[TCP/IP] XGEM_NETWORK Client Connect Success."));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
				}
			}
			else
			{
				st_client_info[XGEM_NETWORK].nConnect = NO;

				m_pClient[XGEM_NETWORK] = new CClientSocket();
				m_pClient[XGEM_NETWORK]->Create();
	
				m_pClient[XGEM_NETWORK]->OnOpen(this, XGEM_NETWORK);
				if (m_pClient[XGEM_NETWORK]->Connect(st_client_info[XGEM_NETWORK].strIp, st_client_info[XGEM_NETWORK].nPort))
				{
					delete m_pClient[XGEM_NETWORK];
					m_pClient[XGEM_NETWORK] = NULL;

					if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
					{
						//clsFunc.OnStringToChar(_T("[TCP/IP] Front Client Connect Error."), st_other_info.cAbnormalMsg);
						clsMem.OnAbNormalMessagWrite(_T("[TCP/IP] XGEM_NETWORK Client Connect Error."));
						st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력
					}

					return 0;
				}

				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
				{
					//clsFunc.OnStringToChar(_T("[TCP/IP] Front Client Connect Success."), st_other_info.cNormalMsg);
					clsMem.OnAbNormalMessagWrite(_T("[TCP/IP] XGEM_NETWORK Client Connect Success."));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력
				}
			}
			break;

		case CLIENT_CLOSE:
			if (m_pClient[XGEM_NETWORK] != NULL)
			{
				//if (st_client_info[XGEM_NETWORK].nConnect == YES) m_pClient[XGEM_NETWORK]->Close();

				st_client_info[XGEM_NETWORK].nConnect = NO;

				delete m_pClient[XGEM_NETWORK];
				m_pClient[XGEM_NETWORK] = NULL;

				clsXgem.m_nXgemInterface = MES_OFF_LINE;
				if(st_handler_info.cWndTitle != NULL)  
				{
					st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, XGEM_MODE, clsXgem.m_nXgemInterface);
 				}

				m_nXgemStep = 0;
				SetTimer(TM_XGEM, 5000, NULL);
			}
			break;

		case CLIENT_SEND:
			if (st_client_info[XGEM_NETWORK].nConnect == YES)
			{
				if (m_pClient[XGEM_NETWORK] == NULL) return 0;

				clsFunc.OnStringToChar(st_client_info[XGEM_NETWORK].strSend, st_client_info[XGEM_NETWORK].chSend);
				nLength = st_client_info[XGEM_NETWORK].strSend.GetLength();

				m_pClient[XGEM_NETWORK]->Send(st_client_info[XGEM_NETWORK].chSend, nLength);
			}
			break;

		case CLIENT_REV:
			clsXgem.OnDataReceive(st_client_info[XGEM_NETWORK].strRev);
			break;

		case CLIENT_ACCEPT:
			if (lParam != 0)
			{
				st_client_info[XGEM_NETWORK].nConnect = NO;

				delete m_pClient[XGEM_NETWORK];
				m_pClient[XGEM_NETWORK] = NULL;

				return 0;
			}

			st_client_info[XGEM_NETWORK].nConnect = YES;
			break;
	}

	return 0;
}

LRESULT CMainFrame::OnClientRfid(WPARAM wParam, LPARAM lParam)
{
	CString strMsg;

	int nLength;

	switch(wParam)
	{
		case CLIENT_CONNECT:
			if (m_pClient[RFID_NETWORK] != NULL)
			{
				//if (st_client_info[RFID_NETWORK].nConnect == YES) m_pClient[RFID_NETWORK]->Close();

				st_client_info[RFID_NETWORK].nConnect = NO;

				delete m_pClient[RFID_NETWORK];
				m_pClient[RFID_NETWORK] = NULL;

				m_pClient[RFID_NETWORK] = new CClientSocket();
				m_pClient[RFID_NETWORK]->Create();
	
				m_pClient[RFID_NETWORK]->OnOpen(this, RFID_NETWORK);
				if (m_pClient[RFID_NETWORK]->Connect(st_client_info[RFID_NETWORK].strIp, st_client_info[RFID_NETWORK].nPort))
				{
					delete m_pClient[RFID_NETWORK];
					m_pClient[RFID_NETWORK] = NULL;

					if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
					{
						//clsFunc.OnStringToChar(_T("[TCP/IP] RFID Client Connect Error."), st_other_info.cAbnormalMsg);
						clsMem.OnAbNormalMessagWrite(_T("[TCP/IP] RFID_NETWORK Client Connect Error."));
						st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력
					}

					return 0;
				}

				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
				{
					//clsFunc.OnStringToChar(_T("[TCP/IP] RFID Client Connect Success."), st_other_info.cNormalMsg);
					clsMem.OnNormalMessageWrite(_T("[TCP/IP] RFID_NETWORK Client Connect Success."));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
				}
			}
			else
			{
				st_client_info[RFID_NETWORK].nConnect = NO;

				m_pClient[RFID_NETWORK] = new CClientSocket();
				m_pClient[RFID_NETWORK]->Create();
	
				m_pClient[RFID_NETWORK]->OnOpen(this, RFID_NETWORK);
				if (m_pClient[RFID_NETWORK]->Connect(st_client_info[RFID_NETWORK].strIp, st_client_info[RFID_NETWORK].nPort))
				{
					delete m_pClient[RFID_NETWORK];
					m_pClient[RFID_NETWORK] = NULL;

					if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
					{
						//clsFunc.OnStringToChar(_T("[TCP/IP] RFID Client Connect Error."), st_other_info.cAbnormalMsg);
						clsMem.OnAbNormalMessagWrite(_T("[TCP/IP] RFID_NETWORK Client Connect Error."));
						st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력
					}

					return 0;
				}

				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
				{
					//clsFunc.OnStringToChar(_T("[TCP/IP] RFID Client Connect Success."), st_other_info.cNormalMsg);
					clsMem.OnNormalMessageWrite(_T("[TCP/IP] RFID_NETWORK Client Connect Success."));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
				}
			}
			break;

		case CLIENT_CLOSE:
			if (m_pClient[RFID_NETWORK] != NULL)
			{
				//if (st_client_info[RFID_NETWORK].nConnect == YES) m_pClient[RFID_NETWORK]->Close();

				st_client_info[RFID_NETWORK].nConnect = NO;

				delete m_pClient[RFID_NETWORK];
				m_pClient[RFID_NETWORK] = NULL;
			}
			break;

		case CLIENT_SEND:
			if (st_client_info[RFID_NETWORK].nConnect == YES)
			{
				if (m_pClient[RFID_NETWORK] == NULL) return 0;

				clsFunc.OnStringToChar(st_client_info[RFID_NETWORK].strSend, st_client_info[RFID_NETWORK].chSend);
				nLength = st_client_info[RFID_NETWORK].strSend.GetLength();

				m_pClient[RFID_NETWORK]->Send(st_client_info[RFID_NETWORK].chSend, nLength);

				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
				{
					strMsg.Format(_T("[RFID_NETWORK] %s"), st_client_info[RFID_NETWORK].strSend);
					//clsFunc.OnStringToChar(strMsg, st_other_info.cNormalMsg);
					clsMem.OnNormalMessageWrite(strMsg);
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
				}
			}
			break;

		case CLIENT_REV:
			clsRfid.OnDataReceive(st_client_info[RFID_NETWORK].strRev);
			break;

		case CLIENT_ACCEPT:
			if (lParam != 0)
			{
				st_client_info[RFID_NETWORK].nConnect = NO;

				delete m_pClient[RFID_NETWORK];
				m_pClient[RFID_NETWORK] = NULL;

				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
				{
					//clsFunc.OnStringToChar(_T("[TCP/IP] RFID Client Accept Fail."), st_other_info.cAbnormalMsg);
					clsMem.OnAbNormalMessagWrite(_T("[TCP/IP] RFID_NETWORK Client Accept Fail."));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력
				}
				return 0;
			}

			st_client_info[RFID_NETWORK].nConnect = YES;

			if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
			{
				//clsFunc.OnStringToChar(_T("[TCP/IP] RFID Client Accept Success."), st_other_info.cNormalMsg);
				clsMem.OnNormalMessageWrite(_T("[TCP/IP] RFID_NETWORK Client Accept Success."));
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
			}
			break;
	}

	return 0;
}

LRESULT CMainFrame::OnServerGms(WPARAM wParam, LPARAM lParam)
{
	int i;
	int nLength;

	switch(wParam)
	{
		case SERVER_CONNECT:
			if (m_pServer[GMS_NETWORK] != NULL)
			{
				m_pServer[GMS_NETWORK]->OnClose();

				delete m_pServer[GMS_NETWORK];
				m_pServer[GMS_NETWORK] = NULL;
			}

			// sevrer 생성
			m_pServer[GMS_NETWORK] = new CServerSocket;

			// Tcp/IP server open
			if (m_pServer[GMS_NETWORK]->OnOpen(this, st_server_info[GMS_NETWORK].nPort, GMS_NETWORK))
			{
				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
				{
					//clsFunc.OnStringToChar(_T("[TCP/IP] Rear Server Open Success."), st_other_info.cNormalMsg);
					clsMem.OnNormalMessageWrite(_T("[TCP/IP] TTS_VNR_NETWORK Server Open Success."));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
				}
			}
			else
			{
				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
				{
					//clsFunc.OnStringToChar(_T("[TCP/IP] Rear Server Open Error."), st_other_info.cAbnormalMsg);
					clsMem.OnAbNormalMessagWrite(_T("[TCP/IP] TTS_VNR_NETWORK Server Open Error."));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력
				}

				delete m_pServer[GMS_NETWORK];
				m_pServer[GMS_NETWORK] = NULL;
			}
			break;

		case SERVER_CLOSE:
			if (m_pServer[GMS_NETWORK] != NULL)
			{
				m_pServer[GMS_NETWORK]->OnCloseClient(lParam);
			}
			break;

		case SERVER_REV:
			clsVnR.OnDataReceive(st_server_info[GMS_NETWORK].strRev);
			break;

		case SERVER_SEND:
			for (i=0; i<MAX_USER; i++)
			{
				if (m_pServer[GMS_NETWORK] != NULL)
				{
					// client가 server에 접속중이면 
					if (m_pServer[GMS_NETWORK]->m_pClient[i] != NULL)
					{
						// message 전송
						//m_pServer[GMS_NETWORK]->m_pClient[i]->Send(st_server_info[GMS_NETWORK].strSend, st_server_info[GMS_NETWORK].strSend.GetLength() * 2, 0);
						clsFunc.OnStringToChar(st_server_info[GMS_NETWORK].strSend, st_server_info[GMS_NETWORK].chSend);
						nLength = st_server_info[GMS_NETWORK].strSend.GetLength();

						m_pServer[GMS_NETWORK]->m_pClient[i]->Send(st_server_info[GMS_NETWORK].chSend, nLength);
					}
				}
			}
			break;

		case SERVER_ACCEPT:
			clsVnR.OnMakeTime();
			break;
	}

	return 0;
}

LRESULT CMainFrame::OnServerFront(WPARAM wParam, LPARAM lParam)
{
	int i;
	int nLength;

	switch(wParam)
	{
		case SERVER_CONNECT:
			if (m_pServer[FRONT_NETWORK] != NULL)
			{
				m_pServer[FRONT_NETWORK]->OnClose();

				delete m_pServer[FRONT_NETWORK];
				m_pServer[FRONT_NETWORK] = NULL;
			}

			// sevrer 생성
			m_pServer[FRONT_NETWORK] = new CServerSocket;
	
			// Tcp/IP server open
			if (m_pServer[FRONT_NETWORK]->OnOpen(this, st_server_info[FRONT_NETWORK].nPort, FRONT_NETWORK))
			{
				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
				{
					//clsFunc.OnStringToChar(_T("[TCP/IP] Rear Server Open Success."), st_other_info.cNormalMsg);
					clsMem.OnNormalMessageWrite(_T("[TCP/IP] FRONT_NETWORK Server Open Success."));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
				}
			}
			else
			{
				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
				{
					//clsFunc.OnStringToChar(_T("[TCP/IP] Rear Server Open Error."), st_other_info.cAbnormalMsg);
					clsMem.OnAbNormalMessagWrite(_T("[TCP/IP] FRONT_NETWORK Server Open Error."));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력
				}

				delete m_pServer[FRONT_NETWORK];
				m_pServer[FRONT_NETWORK] = NULL;
			}
			break;

		case SERVER_CLOSE:
			if (m_pServer[FRONT_NETWORK] != NULL)
			{
				m_pServer[FRONT_NETWORK]->OnCloseClient(lParam);
			}
			break;

		case SERVER_REV:
			clsFront.OnDataReceive(st_server_info[FRONT_NETWORK].strRev);
			break;

		case SERVER_SEND:
			for (i=0; i<MAX_USER; i++)
			{
				if (m_pServer[FRONT_NETWORK] != NULL)
				{
					// client가 server에 접속중이면 
					if (m_pServer[FRONT_NETWORK]->m_pClient[i] != NULL)
					{
						// message 전송
						clsFunc.OnStringToChar(st_server_info[FRONT_NETWORK].strSend, st_server_info[FRONT_NETWORK].chSend);
						nLength = st_server_info[FRONT_NETWORK].strSend.GetLength();

						m_pServer[FRONT_NETWORK]->m_pClient[i]->Send(st_server_info[FRONT_NETWORK].chSend, nLength);

						clsMem.OnNormalMessageWrite(st_server_info[FRONT_NETWORK].strSend);
						st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
					}
				}
			}
			break;
	}

	return 0;
}

LRESULT CMainFrame::OnClientFtp(WPARAM wParam, LPARAM lParam)
{
	BOOL bRet;

	if (wParam == 0)
	{
		bRet = m_pFtp.OnScrapCodeDownload(lParam, st_path_info.strPathFtpGms, st_basic_info.strScrapName);

		if (bRet)
		{
			if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
			{
				clsMem.OnNormalMessageWrite(_T("[FTP] Scrap Code Download Success."));
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
			}

			// jtkim 20150719
			st_work_info.nFtpWork = 1;
			if (st_handler_info.cWndTitle != NULL)
			{
		//		st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, INTERFACE_MODE, st_basic_info.nModeInterface);
				st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, INTERFACE_MODE, st_handler_info.nFtpVer);
			}
			st_code_info[lParam].m_nScrapBin = BD_DATA_GOOD;
		}
		else
		{
			if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
			{
				clsMem.OnNormalMessageWrite(_T("[FTP] Scrap Code Download Fail."));
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
			}

			// jtkim 20150719
			st_work_info.nFtpWork = 0;
			if (st_handler_info.cWndTitle != NULL)
			{
		//		st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, INTERFACE_MODE, st_basic_info.nModeInterface);
				st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, INTERFACE_MODE, st_handler_info.nFtpVer);
			}
			st_code_info[lParam].m_nScrapBin = BD_DATA_REJECT;
		}
	}
	else
	{
		bRet = m_pFtp.OnRecipeDownload(st_path_info.strPathFtpGms);

		if (bRet)
		{
			if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
			{
				clsMem.OnNormalMessageWrite(_T("[FTP] Recipe Download Success."));
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
			}

			// jtkim 20150719
			st_work_info.nFtpWork = 1;
			if (st_handler_info.cWndTitle != NULL)
			{
		//		st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, INTERFACE_MODE, st_basic_info.nModeInterface);
				st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, INTERFACE_MODE, st_handler_info.nFtpVer);
			}
			st_code_info[lParam].m_nScrapBin = BD_DATA_GOOD;
		}
		else
		{
			if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
			{
				clsMem.OnNormalMessageWrite(_T("[FTP] Recipe Download Fail."));
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
			}

			// jtkim 20150719
			st_work_info.nFtpWork = 0;
			if (st_handler_info.cWndTitle != NULL)
			{
		//		st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, INTERFACE_MODE, st_basic_info.nModeInterface);
				st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, INTERFACE_MODE, st_handler_info.nFtpVer);
			}
			st_code_info[lParam].m_nScrapBin = BD_DATA_REJECT;
		}
	}

	return 0;
}

void CMainFrame::OnNcLButtonDblClk(UINT nHitTest, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//return ;

	CFrameWnd::OnNcLButtonDblClk(nHitTest, point);
}


void CMainFrame::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//return ;

	CFrameWnd::OnNcLButtonDown(nHitTest, point);
}


int CMainFrame::OnXgemInterface()
{
	switch (m_nXgemStep)
	{
		case 0:
			// 작업중인 모드체크
			if (st_basic_info.nModeXgem == YES)
			{
				// 작업모드가 xgem 모드이면
				m_nXgemStep = 100;
			}
			break;

		case 100:
			// client 연결 요청 
			m_dwXgemTime[0] = GetTickCount();
			::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + XGEM_NETWORK, CLIENT_CONNECT, 0);

			m_nXgemStep = 200;
			break;

		case 200:
			// client 연결상태 체크
			if (st_client_info[XGEM_NETWORK].nConnect == YES)
			{
				// 현재 연결중이면 데이터 전송으로 
				KillTimer(TM_XGEM);
				SetTimer(TM_XGEM, 100, NULL);

				m_nXgemStep = 300;
			}
			else 
			{
				m_dwXgemTime[1] = GetTickCount();
				m_dwXgemTime[2] = m_dwXgemTime[1] - m_dwXgemTime[0];

				if (m_dwXgemTime[2] <= (DWORD)0)
				{
					m_dwXgemTime[0] = GetTickCount();
					break;
				}

				if (m_dwXgemTime[2] > 5000)
				{
					m_nXgemStep = 0;

				}
			}
			break;

		case 300:
			m_dwXgemTime[0] = GetTickCount();
			clsXgem.OnXgemInitialize(_T("d:\\XGEM\\EqSample.cfg"));

			m_nXgemStep = 400;
			break;

		case 400:
			m_dwXgemTime[1] = GetTickCount();
			m_dwXgemTime[2] = m_dwXgemTime[1] - m_dwXgemTime[0];

			if (m_dwXgemTime[2] <= (DWORD)0)
			{
				m_dwXgemTime[0] = GetTickCount();
				break;
			}

			if (m_dwXgemTime[2] > 1000)
			{
				m_nXgemStep = 500;

			}
			break;

		case 500:
			clsXgem.OnMcInterface(st_basic_info.nModeXgemInterface);

			m_nXgemStep = 0;
			return RET_GOOD;

			break;
	}

	return RET_PROCEED;
}


LRESULT CMainFrame::OnXgemCommand(WPARAM wParam, LPARAM lParam)
{
	return 0;

	if (wParam == XGEM_MODE)
	{
		if (st_basic_info.nModeXgem == YES)
		{
			m_nXgemStep = 0;
			SetTimer(TM_XGEM, 5000, NULL);
		}
		else
		{
			KillTimer(TM_XGEM);
		}
	}

	return 0;
}

LRESULT CMainFrame::OnRobotLeftTurnUI(WPARAM wParam, LPARAM lParam)
{
	

	return 0;
}
LRESULT CMainFrame::OnRobotRightTurnUI(WPARAM wParam, LPARAM lParam)
{


	return 0;
}
LRESULT CMainFrame::OnConVeyorUI(WPARAM wParam, LPARAM lParam)
{

	return 0;
}
LRESULT CMainFrame::OnBarcode_1(WPARAM wParam, LPARAM lParam)
{
	CString strMsg, sRcv, sTmp, strTemp;

	int nLength;

	switch(wParam)
	{
	case CLIENT_CONNECT:
		if (m_pClient[BCR1_NETWORK] != NULL)
		{
			if (st_client_info[BCR1_NETWORK].nConnect == YES) 
			{
				st_client_info[BCR1_NETWORK].nConnect = NO;

				delete m_pClient[BCR1_NETWORK];
				m_pClient[BCR1_NETWORK] = NULL;
			}

			m_pClient[BCR1_NETWORK] = new CClientSocket();
			m_pClient[BCR1_NETWORK]->Create();

			m_pClient[BCR1_NETWORK]->OnOpen(this, BCR1_NETWORK);
			if (m_pClient[BCR1_NETWORK]->Connect(st_client_info[BCR1_NETWORK].strIp, st_client_info[BCR1_NETWORK].nPort))
			{
				delete m_pClient[BCR1_NETWORK];
				m_pClient[BCR1_NETWORK] = NULL;

				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
				{
					clsMem.OnAbNormalMessagWrite(_T("[TCP/IP] BCR1_NETWORK Client Connect Error."));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력
				}

				return 0;
			}

			if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
			{
				clsMem.OnNormalMessageWrite(_T("[TCP/IP] BCR1_NETWORK Client Connect Success."));
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
			}
		}
		else
		{
			st_client_info[BCR1_NETWORK].nConnect = NO;

			m_pClient[BCR1_NETWORK] = new CClientSocket();
			m_pClient[BCR1_NETWORK]->Create();

			m_pClient[BCR1_NETWORK]->OnOpen(this, BCR1_NETWORK);
			if (m_pClient[BCR1_NETWORK]->Connect(st_client_info[BCR1_NETWORK].strIp, st_client_info[BCR1_NETWORK].nPort))
			{
				delete m_pClient[BCR1_NETWORK];
				m_pClient[BCR1_NETWORK] = NULL;

				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
				{
					clsMem.OnAbNormalMessagWrite(_T("[TCP/IP] BCR1_NETWORK Client Connect Error."));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력
				}

				return 0;
			}

			st_client_info[BCR1_NETWORK].nConnect = YES;

			if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
			{
				clsMem.OnNormalMessageWrite(_T("[TCP/IP] BCR1_NETWORK Client Connect Success."));
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
			}
		}
		break;

	case CLIENT_CLOSE:
		if (m_pClient[BCR1_NETWORK] != NULL)
		{
			st_client_info[BCR1_NETWORK].nConnect = NO;

			delete m_pClient[BCR1_NETWORK];
			m_pClient[BCR1_NETWORK] = NULL;
		}
		break;

	case CLIENT_SEND:
		if (st_client_info[BCR1_NETWORK].nConnect == YES)
		{
			if (m_pClient[BCR1_NETWORK] == NULL) return 0;

			//clsFunc.OnStringToChar(st_client_info[PRINTER_NETWORK].strSend, st_client_info[PRINTER_NETWORK].chSend);

			//st_client_info[BCR1_NETWORK].strSend.Format(_T("%s"),st_client_info[BCR1_NETWORK].chSend);
			//kwlee 2017.0302


			strTemp.Format(_T("%cR%c"), 0x02, 0x03);
			st_client_info[BCR1_NETWORK].strSend = strTemp;

			clsFunc.OnStringToChar(st_client_info[BCR1_NETWORK].strSend, st_client_info[BCR1_NETWORK].chSend);
			nLength = st_client_info[BCR1_NETWORK].strSend.GetLength();
			
			m_pClient[BCR1_NETWORK]->Send(st_client_info[BCR1_NETWORK].chSend, nLength);

			if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
			{
				strMsg.Format(_T("[BCR1_NETWORK] %s"), st_client_info[BCR1_NETWORK].strSend);
				clsMem.OnNormalMessageWrite(strMsg);
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
			}
		}
		break;

	case CLIENT_REV:
		//clsEcFirst.OnDataReceive(st_client_info[PRINTER_NETWORK].strRev);
 		strMsg.Format(_T("%s"), st_client_info[BCR1_NETWORK].strRev);
// 		sRcv = sTmp.Mid(0, st_client_info[BCR1_NETWORK].nRevLength);
// 		
		//clsRunRobot.m_strBarcode[0] = strMsg;
		//kwlee 2017.0315
		nLength = strMsg.GetLength();
		clsRunRobot.m_strBarcode[0] = strMsg.Mid(1,nLength - 2);

		clsRunRobot.m_nBarcodeReadCheck[0] = TRUE;
		
		if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
		{
			//strMsg.Format(_T("[PRINTER_CLIENT_NETWORK] %s"), st_client_info[PRINTER_NETWORK].strRev);
			//clsMem.OnNormalMessageWrite(strMsg);
			strTemp.Format(_T("BCR1 RCV : %s"),clsRunRobot.m_strBarcode[0]);
			clsMem.OnNormalMessageWrite(strTemp);
			st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
		}
		break;

	case CLIENT_ACCEPT:
		if (lParam != 0)
		{
			st_client_info[BCR1_NETWORK].nConnect = NO;

			delete m_pClient[BCR1_NETWORK];
			m_pClient[BCR1_NETWORK] = NULL;

			if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
			{
				clsMem.OnAbNormalMessagWrite(_T("[TCP/IP] BCR1_NETWORK Client Accept Fail."));
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력
			}

			return 0;
		}

		st_client_info[BCR1_NETWORK].nConnect = YES;
		if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
		{
			clsMem.OnNormalMessageWrite(_T("[TCP/IP] BCR1_NETWORK Client Accept Success."));
			st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
		}
		break;
	}
	return 0;
}
//kwlee 2017.0315
LRESULT CMainFrame::OnMainframe_VisionWork(WPARAM wParam, LPARAM lParam)
{	
	int mn_command_num = wParam;  // 네트워크 작업을 할 구분 변수

	switch (mn_command_num)
	{		
	case MAIN_TEACH_VISION:
		///OnMainFrame_TeachVision( lParam );
		break;

	}
	return 0;
}

LRESULT CMainFrame::OnBarcode_2(WPARAM wParam, LPARAM lParam)
{
	CString strMsg, sRcv, sTmp,strTemp;

	int nLength;

	switch(wParam)
	{
		case CLIENT_CONNECT:
			if (m_pClient[BCR2_NETWORK] != NULL)
			{
				if (st_client_info[BCR2_NETWORK].nConnect == YES) 
				{
					st_client_info[BCR2_NETWORK].nConnect = NO;

					delete m_pClient[BCR2_NETWORK];
					m_pClient[BCR2_NETWORK] = NULL;
				}

				m_pClient[BCR2_NETWORK] = new CClientSocket();
				m_pClient[BCR2_NETWORK]->Create();

				m_pClient[BCR2_NETWORK]->OnOpen(this, BCR2_NETWORK);
				if (m_pClient[BCR2_NETWORK]->Connect(st_client_info[BCR2_NETWORK].strIp, st_client_info[BCR2_NETWORK].nPort))
				{
					delete m_pClient[BCR2_NETWORK];
					m_pClient[BCR2_NETWORK] = NULL;

					if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
					{
						clsMem.OnAbNormalMessagWrite(_T("[TCP/IP] BCR2_NETWORK Client Connect Error."));
						st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력
					}

					return 0;
				}

				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
				{
					clsMem.OnNormalMessageWrite(_T("[TCP/IP] BCR2_NETWORK Client Connect Success."));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
				}
			}
			else
			{
				st_client_info[BCR2_NETWORK].nConnect = NO;

				m_pClient[BCR2_NETWORK] = new CClientSocket();
				m_pClient[BCR2_NETWORK]->Create();

				m_pClient[BCR2_NETWORK]->OnOpen(this, BCR2_NETWORK);
				if (m_pClient[BCR2_NETWORK]->Connect(st_client_info[BCR2_NETWORK].strIp, st_client_info[BCR2_NETWORK].nPort))
				{
					delete m_pClient[BCR2_NETWORK];
					m_pClient[BCR2_NETWORK] = NULL;

					if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
					{
						clsMem.OnAbNormalMessagWrite(_T("[TCP/IP] BCR2_NETWORK Client Connect Error."));
						st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력
					}
					return 0;
				}

				st_client_info[BCR2_NETWORK].nConnect = YES;

				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
				{
					clsMem.OnNormalMessageWrite(_T("[TCP/IP] BCR2_NETWORK Client Connect Success."));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
				}
			}
			break;

		case CLIENT_CLOSE:
			if (m_pClient[BCR2_NETWORK] != NULL)
			{
				st_client_info[BCR2_NETWORK].nConnect = NO;
				delete m_pClient[BCR2_NETWORK];
				m_pClient[BCR2_NETWORK] = NULL;
			}
			break;

		case CLIENT_SEND:
			if (st_client_info[BCR2_NETWORK].nConnect == YES)
			{
				if (m_pClient[BCR2_NETWORK] == NULL) return 0;

				//clsFunc.OnStringToChar(st_client_info[PRINTER_NETWORK].strSend, st_client_info[PRINTER_NETWORK].chSend);

				//st_client_info[BCR2_NETWORK].strSend.Format(_T("%s"),st_client_info[BCR2_NETWORK].chSend);
				//kwlee 2017.0302
				strTemp.Format(_T("%cR%c"), 0x02, 0x03);
				st_client_info[BCR2_NETWORK].strSend = strTemp;

				clsFunc.OnStringToChar(st_client_info[BCR2_NETWORK].strSend, st_client_info[BCR2_NETWORK].chSend);
				nLength = st_client_info[BCR2_NETWORK].strSend.GetLength();
				m_pClient[BCR2_NETWORK]->Send(st_client_info[BCR2_NETWORK].chSend, nLength);

				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
				{
					strMsg.Format(_T("[BCR2_NETWORK] %s"), st_client_info[BCR2_NETWORK].strSend);
					clsMem.OnNormalMessageWrite(strMsg);
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
				}
			}
			break;

		case CLIENT_REV:
			strMsg.Format(_T("%s"), st_client_info[BCR2_NETWORK].strRev);
			sRcv = sTmp.Mid(0, st_client_info[BCR2_NETWORK].nRevLength);
			//clsRunRobot.m_strBarcode[1] = strMsg;
			//kwlee 2017.0315
			nLength = strMsg.GetLength();
			clsRunRobot.m_strBarcode[1] = strMsg.Mid(1,nLength - 2);
			clsRunRobot.m_nBarcodeReadCheck[1] = TRUE;

			if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
			{
				//strMsg.Format(_T("[PRINTER_CLIENT_NETWORK] %s"), st_client_info[PRINTER_NETWORK].strRev);
				//clsMem.OnNormalMessageWrite(strMsg);
				strTemp.Format(_T("BCR2 RCV : %s"),clsRunRobot.m_strBarcode[1]);
				clsMem.OnNormalMessageWrite(strTemp);
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
			}
			break;

		case CLIENT_ACCEPT:
			if (lParam != 0)
			{
				st_client_info[BCR2_NETWORK].nConnect = NO;

				delete m_pClient[BCR2_NETWORK];
				m_pClient[BCR2_NETWORK] = NULL;

				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
				{
					clsMem.OnAbNormalMessagWrite(_T("[TCP/IP] BCR2_NETWORK Client Accept Fail."));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력
				}

				return 0;
			}

			st_client_info[BCR2_NETWORK].nConnect = YES;
			if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
			{
				clsMem.OnNormalMessageWrite(_T("[TCP/IP] BCR2_NETWORK Client Accept Success."));
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
			}
			break;
		}
	return 0;
}

LRESULT CMainFrame::OnServerZebraPrinter(WPARAM wParam, LPARAM lParam)
{

	int i;
	int nLength;
	CString strMsg, sRcv;

	switch(wParam)
	{
	case SERVER_CONNECT:
		if (m_pServer[PRINTER_NETWORK] != NULL)
		{
			m_pServer[PRINTER_NETWORK]->OnClose();

			delete m_pServer[PRINTER_NETWORK];
			m_pServer[PRINTER_NETWORK] = NULL;
		}

		// sevrer 생성
		m_pServer[PRINTER_NETWORK] = new CServerSocket;

		// Tcp/IP server open
		if (m_pServer[PRINTER_NETWORK]->OnOpen(this, st_server_info[PRINTER_NETWORK].nPort, PRINTER_NETWORK))
		{
			if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
			{
				//clsFunc.OnStringToChar(_T("[TCP/IP] Rear Server Open Success."), st_other_info.cNormalMsg);
				clsMem.OnNormalMessageWrite(_T("[TCP/IP] PRINTER_SERVER_NETWORK Server Open Success."));
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
			}
		}
		else
		{
			if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
			{
				//clsFunc.OnStringToChar(_T("[TCP/IP] Rear Server Open Error."), st_other_info.cAbnormalMsg);
				clsMem.OnAbNormalMessagWrite(_T("[TCP/IP] PRINTER_SERVER_NETWORK Server Open Error."));
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력
			}

			delete m_pServer[PRINTER_NETWORK];
			m_pServer[PRINTER_NETWORK] = NULL;
		}
		break;

	case SERVER_CLOSE:
		if (m_pServer[PRINTER_NETWORK] != NULL)
		{
			m_pServer[PRINTER_NETWORK]->OnCloseClient(lParam);
		}
		break;

	case SERVER_REV:
	//	clsFront.OnDataReceive(st_server_info[FRONT_NETWORK].strRev);
		strMsg.Format(_T("%s"), st_server_info[PRINTER_NETWORK].strRev);
		//sRcv = sTmp.Mid(0, st_client_info[PRINTER_NETWORK].nRevLength);

		st_sync_info.nLabelRecv = CTL_YES;

		if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
		{
			//strMsg.Format(_T("[PRINTER_CLIENT_NETWORK] %s"), st_client_info[PRINTER_NETWORK].strRev);
			clsMem.OnNormalMessageWrite(strMsg);
			st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
		}
		break;

	case SERVER_SEND:
		for (i=0; i<MAX_USER; i++)
		{
			if (m_pServer[PRINTER_NETWORK] != NULL)
			{
				// client가 server에 접속중이면 
				if (m_pServer[PRINTER_NETWORK]->m_pClient[i] != NULL)
				{
					// message 전송
					clsFunc.OnStringToChar(st_server_info[PRINTER_NETWORK].strSend, st_server_info[PRINTER_NETWORK].chSend);
					nLength = st_server_info[PRINTER_NETWORK].strSend.GetLength();

					m_pServer[PRINTER_NETWORK]->m_pClient[i]->Send(st_server_info[PRINTER_NETWORK].chSend, nLength);

					clsMem.OnNormalMessageWrite(st_server_info[PRINTER_NETWORK].strSend);
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
				}
			}
		}
		break;
	}

	return 0;
}

//Client 192.168.0.16 9100
//Server 192.168.0.101

LRESULT CMainFrame::OnClientZebraPrint(WPARAM wParam, LPARAM lParam)
{	
	CString strMsg, sRcv, sTmp;

	int nLength;

	switch(wParam)
	{
	case CLIENT_CONNECT:
		if (m_pClient[PRINTER_NETWORK] != NULL)
		{
			if (st_client_info[PRINTER_NETWORK].nConnect == YES) 
			{
				st_client_info[PRINTER_NETWORK].nConnect = NO;

				delete m_pClient[PRINTER_NETWORK];
				m_pClient[PRINTER_NETWORK] = NULL;
			}

			m_pClient[PRINTER_NETWORK] = new CClientSocket();
			m_pClient[PRINTER_NETWORK]->Create();

			m_pClient[PRINTER_NETWORK]->OnOpen(this, PRINTER_NETWORK);
			if (m_pClient[PRINTER_NETWORK]->Connect(st_client_info[PRINTER_NETWORK].strIp, st_client_info[PRINTER_NETWORK].nPort))
			{
				delete m_pClient[PRINTER_NETWORK];
				m_pClient[PRINTER_NETWORK] = NULL;

				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
				{
					clsMem.OnAbNormalMessagWrite(_T("[TCP/IP] PRINTER_CLIENT_NETWORK Client Connect Error."));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력
				}

				return 0;
			}

			if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
			{
				clsMem.OnNormalMessageWrite(_T("[TCP/IP] PRINTER_CLIENT_NETWORK Client Connect Success."));
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
			}
		}
		else
		{
			st_client_info[PRINTER_NETWORK].nConnect = NO;

			m_pClient[PRINTER_NETWORK] = new CClientSocket();
			m_pClient[PRINTER_NETWORK]->Create();

			m_pClient[PRINTER_NETWORK]->OnOpen(this, PRINTER_NETWORK);
			if (m_pClient[PRINTER_NETWORK]->Connect(st_client_info[PRINTER_NETWORK].strIp, st_client_info[PRINTER_NETWORK].nPort))
			{
				delete m_pClient[PRINTER_NETWORK];
				m_pClient[PRINTER_NETWORK] = NULL;

				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
				{
					clsMem.OnAbNormalMessagWrite(_T("[TCP/IP] PRINTER_CLIENT_NETWORK Client Connect Error."));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력
				}

				return 0;
			}

			st_client_info[PRINTER_NETWORK].nConnect = YES;

			if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
			{
				clsMem.OnNormalMessageWrite(_T("[TCP/IP] PRINTER_CLIENT_NETWORK Client Connect Success."));
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
			}
		}
		break;

	case CLIENT_CLOSE:
		if (m_pClient[PRINTER_NETWORK] != NULL)
		{
			st_client_info[PRINTER_NETWORK].nConnect = NO;

			delete m_pClient[PRINTER_NETWORK];
			m_pClient[PRINTER_NETWORK] = NULL;
		}
		break;

	case CLIENT_SEND:
		if (st_client_info[PRINTER_NETWORK].nConnect == YES)
		{
			if (m_pClient[PRINTER_NETWORK] == NULL) return 0;

			//clsFunc.OnStringToChar(st_client_info[PRINTER_NETWORK].strSend, st_client_info[PRINTER_NETWORK].chSend);
			
			//st_client_info[PRINTER_NETWORK].strSend.Format(_T("%s"),st_client_info[PRINTER_NETWORK].chSend);

			st_client_info[PRINTER_NETWORK].strSend = st_client_info[PRINTER_NETWORK].chSend;
			nLength = st_client_info[PRINTER_NETWORK].strSend.GetLength();

			m_pClient[PRINTER_NETWORK]->Send(st_client_info[PRINTER_NETWORK].chSend, nLength);

			if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
			{
				strMsg.Format(_T("[PRINTER_CLIENT_NETWORK] %s"), st_client_info[PRINTER_NETWORK].chSend);
				clsMem.OnNormalMessageWrite(strMsg);
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
			}
		}
		break;

	case CLIENT_REV:
		//clsEcFirst.OnDataReceive(st_client_info[PRINTER_NETWORK].strRev);
		strMsg.Format(_T("%s"), st_client_info[PRINTER_NETWORK].strRev);
		sRcv = sTmp.Mid(0, st_client_info[PRINTER_NETWORK].nRevLength);

		if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
		{
			//strMsg.Format(_T("[PRINTER_CLIENT_NETWORK] %s"), st_client_info[PRINTER_NETWORK].strRev);
			clsMem.OnNormalMessageWrite(strMsg);
			st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
		}
		break;

	case CLIENT_ACCEPT:
		if (lParam != 0)
		{
			st_client_info[PRINTER_NETWORK].nConnect = NO;

			delete m_pClient[PRINTER_NETWORK];
			m_pClient[PRINTER_NETWORK] = NULL;

			if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
			{
				clsMem.OnAbNormalMessagWrite(_T("[TCP/IP] PRINTER_NETWORK Client Accept Fail."));
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력
			}

			return 0;
		}

		st_client_info[PRINTER_NETWORK].nConnect = YES;
		if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
		{
			clsMem.OnNormalMessageWrite(_T("[TCP/IP] PRINTER_NETWORK Client Accept Success."));
			st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
		}
		break;
	}

	return 0;
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다
	if( pMsg->message == WM_KEYDOWN) 
	{
/*		if( pMsg->wParam == 'T')
		{
			if (st_handler_info.cWndMain != NULL)
			{
				st_handler_info.cWndMain->SendMessage(WM_WORK_COMMAND, MAIN_PCB_INFO, 0);
			}
		}
		else if( pMsg->wParam == 'S')
		{
			if (st_handler_info.cWndMain != NULL)
			{
				st_handler_info.cWndMain->SendMessage(WM_WORK_COMMAND, MAIN_PCB_STATUS, 0);
			}
		}
		else if( pMsg->wParam == 'N')
		{
			if (st_handler_info.cWndMain != NULL)
			{
				st_handler_info.cWndMain->SendMessage(WM_WORK_COMMAND, MAIN_PCB_INFO_NEXT, 0);
			}
		}
		else if( pMsg->wParam == 'B')
		{
			if (st_handler_info.cWndMain != NULL)
			{
				st_handler_info.cWndMain->SendMessage(WM_WORK_COMMAND, MAIN_PCB_INFO_BACK, 0);
			}
		}*/
	}
	return CFrameWnd::PreTranslateMessage(pMsg);
}
