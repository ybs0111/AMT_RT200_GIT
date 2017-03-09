// ScreenInitial.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Handler.h"
#include "ScreenInitial.h"
#include "Variable.h"
#include "CtlBd_Library.h"
#include "Ctlbd_Variable.h"
#include "PublicFunction.h"
#include "FastechPublic_IO.h"
#include "ComizoaPublic.h"
#include "Dialog_Message.h"
#include "LedButton.h"				// LED 버튼 생성 클래스 추가 
#include "Thread.h"
#include "XgemClient.h"
#include "AlgMemory.h"
#include "DialogMessageView.h"

// CScreenInitial

IMPLEMENT_DYNCREATE(CScreenInitial, CFormView)

CScreenInitial::CScreenInitial()
	: CFormView(CScreenInitial::IDD)
{
	m_nInitStep		= 0;
	m_nProcessStep	= 0;
	m_nProcessStep1 = 0;
	mb_MsgCheck = true;
	m_pMsgView = NULL;

	m_bmGround.LoadBitmap(IDB_BITMAP_BACK_GROUND);   
}

CScreenInitial::~CScreenInitial()
{
	if (m_pMsgView != NULL)
	{
		delete m_pMsgView;
		m_pMsgView = NULL;
	}
}

void CScreenInitial::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_PG_01,			m_radPg[0]);
	DDX_Control(pDX, IDC_RADIO_PG_02,			m_radPg[1]);
	DDX_Control(pDX, IDC_RADIO_PG_03,			m_radPg[2]);
	DDX_Control(pDX, IDC_RADIO_PG_04,			m_radPg[3]);
	DDX_Control(pDX, IDC_RADIO_PG_05,			m_radPg[4]);
	DDX_Control(pDX, IDC_RADIO_PG_06,			m_radPg[5]);
	DDX_Control(pDX, IDC_RADIO_PG_07,			m_radPg[6]);
	DDX_Control(pDX, IDC_RADIO_PG_08,			m_radPg[7]);
	DDX_Control(pDX, IDC_RADIO_PG_09,			m_radPg[8]);
	DDX_Control(pDX, IDC_RADIO_PG_10,			m_radPg[9]);
	DDX_Control(pDX, IDC_RADIO_PG_11,			m_radPg[10]);
	DDX_Control(pDX, IDC_RADIO_PG_12,			m_radPg[11]);
	DDX_Control(pDX, IDC_RADIO_PG_13,			m_radPg[12]);
	DDX_Control(pDX, IDC_RADIO_PG_14,			m_radPg[13]);
	DDX_Control(pDX, IDC_RADIO_PG_15,			m_radPg[14]);
	DDX_Control(pDX, IDC_RADIO_PG_16,			m_radPg[15]);
	DDX_Control(pDX, IDC_RADIO_PG_17,			m_radPg[16]);
	DDX_Control(pDX, IDC_RADIO_PG_18,			m_radPg[17]);
	DDX_Control(pDX, IDC_RADIO_PG_19,			m_radPg[18]);
	DDX_Control(pDX, IDC_RADIO_PG_20,			m_radPg[19]);
	DDX_Control(pDX, IDC_RADIO_PG_21,			m_radPg[20]);
	DDX_Control(pDX, IDC_MSG_INITIAL,			m_msgInitial);
	DDX_Control(pDX, IDC_GIF_INITIALIZING,		m_pgifInitializing);
	DDX_Control(pDX, IDC_BTN_SKIP,				m_btnSkip);
}

BEGIN_MESSAGE_MAP(CScreenInitial, CFormView)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_SKIP,					&CScreenInitial::OnClickedBtnSkip)
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_WORK_COMMAND,					OnMainWorkInfoCommand)  // 출력용 사용자 정의 메시지
END_MESSAGE_MAP()


// CScreenInitial 진단입니다.

#ifdef _DEBUG
void CScreenInitial::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CScreenInitial::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

void CScreenInitial::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	OnReadyInit();

	OnInitLabel();
	OnInitButton();
	OnInitProgress();
	OnInitRadio();
	clsZebra.OnPrintAnswerMode(2, 0, CLS_BCR_PRINTER1); //kwlee 2017.0309
	OnInitChangeStatus(0);
	
	
	st_handler_info.cWndInitial = this;
}

void CScreenInitial::OnInitProgress()
{
	CSuperProgressCtrl::RegisterClass();

	m_ctrlProgress.Create(this, 250, 200, LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_INITIAL_STR_1)), 0);
	m_ctrlProgress.SetFillStyle(SP_FILL_HORZGRAD);
	m_ctrlProgress.SetColours(RGB(255,0,0), RGB(0,0,0));
	m_ctrlProgress.SetRange(0, 100);
	m_ctrlProgress.SetStep(1);

	m_ctrlProgress1.Create(this, 250, 350, LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_INITIAL_STR_2)), 0);
	m_ctrlProgress1.SetFillStyle(SP_FILL_HORZGRAD);
	m_ctrlProgress1.SetColours(RGB(0,0,255), RGB(0,0,0));
	m_ctrlProgress1.SetRange(0, 100);
	m_ctrlProgress1.SetStep(1);

}

// ********************************
// 초기화 작업 준비 
// ********************************
void CScreenInitial::OnReadyInit()
{
	clsFunc.OnDoorClose();

	if(st_handler_info.cWndTitle != NULL)
	{
		st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, MACHINE_STATUS, st_handler_info.nRunStatus);
	}

	SetTimer(TMR_INIT, 250, NULL);  // 초기화 작업 타이머 생성
}

void CScreenInitial::OnInitLabel()     // 칼라 Label 생성
{
	if (m_pgifInitializing.Load(MAKEINTRESOURCE(IDR_GIF_INITIALIZING), _T("GIF")))
	{
		m_pgifInitializing.Draw();
	}

	m_msgInitial.SetFont(clsFunc.m_pFont[9]);
	m_msgInitial.SetWindowText(_T("initializing~!! Wait a moment~"));
	m_msgInitial.SetCenterText();
	m_msgInitial.SetColor(RGB(0,0,0));
	m_msgInitial.SetGradientColor(RGB(0,0,255));
	m_msgInitial.SetTextColor(RGB(255,255,255));
}


void CScreenInitial::OnInitButton()
{
	m_btnSkip.SetBitmaps(IDC_BTN_SKIP,  WINDOW_DN,  WINDOW_UP);
	m_btnSkip.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnSkip.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnSkip.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnSkip.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnSkip.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnSkip.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnSkip.SetFont(clsFunc.m_pFont[3]);
}

void CScreenInitial::OnInitRadio()
{
	LedState pLed;
	int i = 0;

	for (i=0; i<21; i++)
	{
		pLed = m_radPg[i].GetLedState();
		m_radPg[i].SetTextBackground(pLed, WINDOW_UP, TRUE);
		m_radPg[i].SetImage(IDB_REDBUTTON, 15);
	}
}


void CScreenInitial::OnDestroy()
{
	CFormView::OnDestroy();

	st_handler_info.cWndInitial = NULL;
}


void CScreenInitial::OnTimer(UINT_PTR nIDEvent)
{
	int nRunCHK;
	int i = 0, y=0, x=0;

	if (nIDEvent == TMR_INIT)
	{
		/*if (st_handler_info.nMenuLock != TRUE)	
		{
			st_handler_info.nMenuLock = TRUE;	// 메뉴 사용 못하도록 설정
		}*/
		
		nRunCHK = OnInitExcution();				// 시컨스 초기화 작업 진행
		if (nRunCHK == RET_GOOD)
		{
			KillTimer(TMR_INIT);  // INIT 수행 타이머 삭제
			
			if (st_handler_info.cWndList != NULL)	
			{
				//wsprintfA(st_other_info.cNormalMsg, "%S", _T("[INIT] EQuipment's Init Process END"));
				//clsFunc.OnStringToChar(_T("[INIT] EQuipment's Init Process END"), st_other_info.cNormalMsg);
				clsMem.OnNormalMessageWrite(_T("[INIT] EQuipment's Init Process END"));
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
			if (st_handler_info.nRunStatus != dSTOP)
			{
				clsFunc.OnMCStop();
			}
			
			if (st_other_info.nConfirmMsg == YES)
			{
				if (st_handler_info.nMenuLock != FALSE)	
				{
					st_handler_info.nMenuLock = FALSE;  // 메뉴 사용 가능하도록 설정
				}

				st_handler_info.nInitialSuccess = YES;

				::PostMessage(st_handler_info.hWnd, WM_FORM_CHANGE, SCREEN_MAIN, 0);  // 메인 화면으로 전환 요청
			}
		
			/////////////////////////////////////////// 2015.0407 james //////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////////////////////
		}
		else if (nRunCHK == RET_ERROR)
		{
			KillTimer(TMR_INIT);  // INIT 수행 타이머 삭제

			if (st_handler_info.cWndList != NULL)	
			{
				//wsprintfA(st_other_info.cAbnormalMsg, "%S", _T("[INIT] Equipment's Init Failure"));
				//clsFunc.OnStringToChar(_T("[INIT] Equipment's Init Failure"), st_other_info.cAbnormalMsg);
				clsMem.OnAbNormalMessagWrite(_T("[INIT] Equipment's Init Failure"));
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}

			if (st_handler_info.nMenuLock != FALSE)	
			{
				st_handler_info.nMenuLock = FALSE;  // 메뉴 사용 가능하도록 설정
			}

			st_handler_info.nInitialSuccess = NO;
			::PostMessage(st_handler_info.hWnd, WM_FORM_CHANGE, SCREEN_MAIN, 0);  // 메인 화면으로 전환 요청
		}
	}

	CFormView::OnTimer(nIDEvent);
}

int CScreenInitial::OnInitExcution()
{
	int i = 0;
	int nRet[2];

	double dCurrPos;
	double dData;

	int nCount;

	CString strTemp;

	nRet[0] = RET_PROCEED;

	CDialog_Message msg_dlg;

//	if (st_handler_info.nRunStatus != dINIT)  return nRet[0];				// 현재 장비 상태가 RUN 아니면 리턴한다.

	switch (m_nInitStep) 
	{
		case 0:
			if(clsFunc.OnDoorOpenCheck() == RET_GOOD)
			{
				m_nProcessStep = 0;
				m_nInitStep = 50;
			}
			else
			{
				return RET_ERROR;
			}		
			break;

		case 50:
			if (FAS_IO.get_in_bit(st_io_info.i_AutoChk, IO_ON) == IO_OFF)
			{
				//st_other_info.strBoxMsg = _T("Please Check Auto Key Status.");
				//CTL_Lib.Alarm_Error_Occurrence(500, dWARNING, _T("500080"));
				PostMessage(WM_WORK_COMMAND, INIT_AUTO_MODE_CHECK, 0);
				
				m_nInitStep = 60;	
			
			}
			else
			{
				m_nInitStep = 1000;
			}
			break;

		case 60:
			if (st_other_info.nConfirmMsg == YES)
			{
				if (st_handler_info.nMenuLock != FALSE)	
				{
					st_handler_info.nMenuLock = FALSE;
				}

				if (FAS_IO.get_in_bit(st_io_info.i_AutoChk, IO_ON) == IO_ON)
				{
					m_nInitStep = 1000;
				}
			}
			else if(st_other_info.nConfirmMsg == NO)
			{
				m_nInitStep = 60;
			}
			break;

		case 1000:		
			if (FAS_IO.m_nIoBoardInitialFlag == NO)  
			{
				//900001 1 0 " "Initialize_Error."
				CTL_Lib.Alarm_Error_Occurrence(501, dWARNING, _T("900001"));
				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
				{
					clsMem.OnAbNormalMessagWrite(_T("I/O Board Initialized Fail..."));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				m_nInitStep = 0;						// 초기화 진행 스텝 정보 초기화 
				return RET_ERROR;
			}
			else
			{
				OnInitChangeStatus(1);	// I/O Board 초기화 완료

				clsFunc.OnIOPortClear();
				
				m_nProcessStep += 4;

				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
				{
					clsMem.OnNormalMessageWrite(_T("I/O Board Initialized Success..."));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
				}

				m_nInitStep = 1100;
			}
			break;

		case 1100:
			if (COMI.mn_motorbd_init_end == YES)
			{
				OnInitChangeStatus(2);	// I/O Board 초기화 완료

				m_nProcessStep += 8;

				PostMessage(WM_WORK_COMMAND, INIT_INITIALIZE_COMMAND, 0);
				m_nInitStep = 1200;
			}
			else
			{
				if (st_work_info.nSimulationMode == 1)
				{
					OnInitChangeStatus(2);	// I/O Board 초기화 완료

					m_nInitStep = 1200;
				}
				else
				{
					nRet[1] = CTL_Lib.Initialize_motor_board(1,  _T("C:\\AMT_RT200\\Setting\\Label.cme2"));			// MOTOR 보드 초기화 여부 검사한다
					if (nRet[1] == RET_ERROR)
					{
						st_alarm_info.strCode		= _T("900000"); 
						st_alarm_info.nCountMode	= 0;
						st_alarm_info.nTypeMode		= eWARNING;
						st_handler_info.nRunStatus  = dWARNING;
						st_alarm_info.nAlarmAssignSection = 502;

						m_nInitStep		= 0;						// 초기화 진행 스텝 정보 초기화 

						if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
						{
							clsMem.OnAbNormalMessagWrite(_T("Motor Board Initialized Fail..."));
							st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
						}	

	 					return RET_ERROR;
					}
					else
					{
						/// 추가.......
						if (st_recovery_info.nMcDataRecovery == NO)
						{
							for (i = 0; i < MAXMOTOR; i++)
							{
								COMI.mn_homechk_flag[i] = NO;
							}
							m_nProcessStep += 8;

							if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
							{
								clsMem.OnNormalMessageWrite(_T("Motor Board Initialized Success..."));
								st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
							}

							OnInitChangeStatus(2);	// I/O Board 초기화 완료

							PostMessage(WM_WORK_COMMAND, INIT_INITIALIZE_COMMAND, 0);

							m_nInitStep = 1200;
						}
						else
						{
							nCount = 0;
							for (i=0; i<M_MAX_MOTOR_NUM; i++)
							{
								dCurrPos	= COMI.Get_MotCurrentPos(i);
								dData		= fabs(dCurrPos - st_work_info.dCurrPos[i]);

								if (dData > st_motor_info[i].d_allow)
								{
									COMI.mn_homechk_flag[i] = NO;
								}
								else
								{
									COMI.mn_homechk_flag[i] = YES;
								}
							}

							m_nProcessStep += 8;

							if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
							{
								clsMem.OnNormalMessageWrite(_T("Motor Board Initialized Success..."));
								st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
							}

							OnInitChangeStatus(2);	// I/O Board 초기화 완료

							m_nInitStep = 1200;
						}
					}
				}
			}
			break;

	
		case 1200:
			if (st_other_info.nConfirmMsg == YES)
			{
				if (st_handler_info.nMenuLock != FALSE)	
				{
					st_handler_info.nMenuLock = FALSE;
				}

				m_nInitStep = 1300;
			}
			else if(st_other_info.nConfirmMsg == NO)
			{
				nRet[0]	= RET_ERROR;

				m_nInitStep = 0;
			}
			break;

		case 1300:
			clsFunc.OnInitVariable();
			
			AllStop[1] = 1;
			// RunRobot Thread
			while(WaitForSingleObject(m_thrHandle[5], 5000) != WAIT_OBJECT_0)
			{
				if (m_thrHandle[5] == NULL) break;
				Sleep(0);
			}
			// Loading Tray Plate Thread
			while(WaitForSingleObject(m_thrHandle[6], 5000) != WAIT_OBJECT_0)
			{
				if (m_thrHandle[6] == NULL) break;
				Sleep(0);
			}
			// Empty Stacker Thread
			while(WaitForSingleObject(m_thrHandle[7], 5000) != WAIT_OBJECT_0)
			{
				if (m_thrHandle[7] == NULL) break;
				Sleep(0);
			}
		

			// jtkim 20150330 recovery동작
			if (st_recovery_info.nMcDataRecovery == NO)
			{
				st_handler_info.nRunStatus = dINIT;
			}
			clsFunc.OnDoorClose();
			
			if (st_handler_info.cWndTitle != NULL)	
			{
 				st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, MACHINE_STATUS, st_handler_info.nRunStatus);
 			}
			m_nInitStep = 1400;
			break;

		case 1400:

			AllStop[1] = 0;
			m_thread[5] = AfxBeginThread(OnThreadRobot, this);  
			if (m_thread[5] != NULL)	
			{
				m_thrHandle[5] = m_thread[5]->m_hThread;
			}
			m_thread[6] = AfxBeginThread(OnThreadConv, this);  
			if (m_thread[6] != NULL)	
			{
				m_thrHandle[6] = m_thread[6]->m_hThread;
			}
			m_thread[7] = AfxBeginThread(OnThreadLabel, this);  
			if (m_thread[7] != NULL)	
			{
				m_thrHandle[7] = m_thread[7]->m_hThread;
			}
			m_nInitStep = 1500;
			break;

		case 1500:
			if (st_sync_info.nInitPickerRbt == INIT_READY)
			{
				m_nProcessStep					+= 5;

				OnInitChangeStatus(3);
				st_sync_info.nInitPickerRbt		= INIT_COMPLETE;
			}
			if (st_sync_info.nInitConveyor == INIT_READY)
			{
				m_nProcessStep					+= 5;

				OnInitChangeStatus(4);
				st_sync_info.nInitConveyor		= INIT_COMPLETE;
			}

			if( st_sync_info.nInitLabel  == INIT_READY)
			{
				m_nProcessStep					+= 5;

				OnInitChangeStatus(5);
				st_sync_info.nInitLabel	= INIT_COMPLETE;
			}

			if ( st_sync_info.nInitPickerRbt == INIT_COMPLETE && st_sync_info.nInitConveyor == INIT_COMPLETE &&
				st_sync_info.nInitLabel == INIT_COMPLETE )
			{
				m_nRetry	= 0;
				m_nMcStep	= 0;

				m_nInitStep = 2000;
			}
			break;


		case 2000:
			m_nInitStep = 0; 
			st_handler_info.nInitialSuccess = YES;
			
			PostMessage(WM_WORK_COMMAND, INIT_COMPLETE_COMMAND, 0);
			//kwlee 2015.1116
	//		clsBasic.OnFillratio_Data_Load();
			nRet[0]		= RET_GOOD;
			break;
	}

	if (m_nProcessStep < 50)
	{
		m_ctrlProgress.SetPos(m_nProcessStep);
	}
	else if (m_nProcessStep > 50)
	{
		if (m_nProcessStep > 100)	m_nProcessStep = 100;

		m_ctrlProgress.SetPos(100);
		m_ctrlProgress1.SetPos(m_nProcessStep);
	}

	if( st_handler_info.nInitialError == YES )
	{
		nRet[0] = RET_ERROR;
	}

	return nRet[0];
}


void CScreenInitial::OnInitChangeStatus(int nNum)
{
	int i = 0;

	if (nNum == 0)
	{
		for (i=0; i<21; i++)
		{
			m_radPg[i].Depress(FALSE);
		}
	}
	else if (nNum >= 1 && nNum <= 21)
	{
		m_radPg[nNum-1].Depress(TRUE);
	}


	/*switch(nNum)
	{
		case 0:
			for (i=0; i<21; i++)
			{
				m_radPg[i].Depress(FALSE);
			}
			
			break;

		case 1:
			m_radPg01.Depress(TRUE);
			break;

		case 2:
			m_radPg02.Depress(TRUE);
			break;

		case 3:
			m_radPg03.Depress(TRUE);
			break;

		case 4:
			m_radPg04.Depress(TRUE);
			break;

		case 5:
			m_radPg05.Depress(TRUE);
			break;

		case 6:
			m_radPg06.Depress(TRUE);
			break;

		case 7:
			m_radPg07.Depress(TRUE);
			break;

		case 8:
			m_radPg08.Depress(TRUE);
			break;

		case 9:
			m_radPg09.Depress(TRUE);
			break;

		case 10:
			m_radPg10.Depress(TRUE);
			break;

		case 11:
			m_radPg11.Depress(TRUE);
			break;

		case 12:
			m_radPg12.Depress(TRUE);
			break;
	}*/
}


BOOL CScreenInitial::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
//	st_handler_info.cWndInitial = NULL;

	return CFormView::DestroyWindow();
}


void CScreenInitial::OnClickedBtnSkip()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다
	KillTimer(TMR_INIT) ;  // 초기화 진행 타이머 삭제 
	// **************************************************************************
	// 화면 전환 가능하도록 플래그 설정한다                                      
	// -> st_handler.n_menu_lock : 메뉴 사용 가능 상태 플래그                    
	// -> st_handler.n_initial   : 초기화 작업 완료 플래그                       
	// **************************************************************************
	if (st_handler_info.nInitialSuccess == YES)
	{
		st_handler_info.nInitialSuccess = NO;
	}
	
	if (st_handler_info.nMenuLock != FALSE)
	{
		st_handler_info.nMenuLock = FALSE;
	}
	// **************************************************************************
	
	if (st_handler_info.nRunStatus != dSTOP)  // STOP 상태 
	{
		clsFunc.OnMCStop();		// 장비 STOP 시 플래그 설정 및 I/O 출력 함수
	}

	PostMessage(WM_WORK_COMMAND, INIT_SKIP_COMMAND, 0);


	::PostMessage(st_handler_info.hWnd, WM_FORM_CHANGE, SCREEN_MAIN, 0); 
	
	
}


BOOL CScreenInitial::OnEraseBkgnd(CDC* pDC)
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

LRESULT CScreenInitial::OnMainWorkInfoCommand(WPARAM wParam, LPARAM lParam) 
{
	CDialog_Message dlgMsg;

	switch (wParam)
	{
		case INIT_RECOVERY_COMMAND:
			dlgMsg.m_strMessage			= _T("Do you want a recovery Machine Data ?");
			dlgMsg.m_nMessageType		= 1;
	
			st_other_info.nConfirmMsg	= -1;

			dlgMsg.DoModal();
			break;

		case INIT_INITIALIZE_COMMAND:
			dlgMsg.m_strMessage			= _T("Do you want a Machine Initialize?");
			dlgMsg.m_nMessageType		= 1;
	
			st_other_info.nConfirmMsg	= -1;

			dlgMsg.DoModal();
			break;

		case INIT_AUTO_MODE_CHECK:
			dlgMsg.m_strMessage			= _T("Please Check Auto Key Status");
			dlgMsg.m_nMessageType		= 1;

			st_other_info.nConfirmMsg	= -1;

			dlgMsg.DoModal();
			break;

		case INIT_COMPLETE_COMMAND:
			dlgMsg.m_strMessage			= _T("Initialize Complete");
			dlgMsg.m_nMessageType		= 1;

			st_other_info.nConfirmMsg	= -1;

			dlgMsg.DoModal();
			break;

		case INIT_SKIP_COMMAND:
			dlgMsg.m_strMessage			= _T("Initialize Skip");
			dlgMsg.m_nMessageType		= 1;

			st_other_info.nConfirmMsg	= -1;

			dlgMsg.DoModal();
			break;

	}
	return 0;
}


void CScreenInitial::OnRecoveryInit()
{

}

void CScreenInitial::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	//	st_handler_info.cWndInitial = NULL;
	CFormView::PostNcDestroy();
}


int CScreenInitial::OnMcEnable()
{
	switch	(m_nMcStep)
	{
		case 0:
			// 작업중인 모드체크/
			if (st_basic_info.nModeInterface == EQP_ON_LINE)
			{
				// 작업모드가 on line 모드이면
				m_dwWaitTime[0] = GetTickCount();

				m_nMcStep = 1000;
			}
			else
			{
				// off line 모드이면 
				m_nMcStep = 0;

				return RET_GOOD;
			}
			break;

		case 1000:
			// datacenter client 현재 상태 체크
			if (st_client_info[EC_FIRST_NETWORK].nConnect == NO)
			{
				// 클라이언트 대기중이면 
				m_nRetry = 0;

				m_nMcStep = 1100;
			}
			else
			{
				m_dwWaitTime[1] = GetTickCount();
				m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];

				if (m_dwWaitTime[2] <= 0)
				{
					m_dwWaitTime[0] = GetTickCount();
					break;
				}

				if (m_dwWaitTime[2] > 10000)
				{
					// 5초동안 대기해서 강제 종료
					m_dwWaitTime[1] = GetTickCount();
					//::PostMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_CLOSE, 0);
					::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_CLOSE, 0);
				}
			}
			break;

		case 1100:
			// client 연결 요청 
			m_dwWaitTime[0] = GetTickCount();
			//::PostMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_CONNECT, 0);
			::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_CONNECT, 0);

			m_nMcStep = 1200;
			break;

		case 1200:
			// client 연결상태 체크
			if (st_client_info[EC_FIRST_NETWORK].nConnect == YES)
			{
				// 현재 연결중이면 데이터 전송으로 
				m_nMcStep = 1300;
			}
			else 
			{
				m_dwWaitTime[1] = GetTickCount();
				m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];

				if (m_dwWaitTime[2] <= 0)
				{
					m_dwWaitTime[0] = GetTickCount();
					break;
				}

				if (m_dwWaitTime[2] > 5000)
				{
					m_nRetry++;
					if (m_nRetry < st_basic_info.nRetry)
					{
						m_nMcStep = 1100;
					}
					else
					{
						CTL_Lib.Alarm_Error_Occurrence(515, dWARNING, _T("550001"));

						m_nMcStep = 0;

						return RET_ERROR;
					}
				}
			}
			break;

		case 1300:
			m_dwWaitTime[0]								= GetTickCount();

//			clsDataClient.m_nMcRecipeReq				= BIN_NOTUESD;
//			st_client_info[EC_FIRST_NETWORK].strSend	= clsDataClient.OnMcRecipe(st_basic_info.strToolName);

			//::PostMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_SEND, 0);
//			::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_SEND, 0);

			m_nMcStep = 1400;
			break;

		case 1400:
			// 데이터 송신 결과 확인 작업
/*			if (clsDataClient.m_nMcRecipeReq == BIN_GOODBIN)
			{
				m_nMcStep = 0;

				return RET_GOOD;
			}
			else if (clsDataClient.m_nMcRecipeReq == BIN_REJECTBIN)
			{
				m_nRetry++;
				if (m_nRetry < st_basic_info.nRetry)
				{
					m_nMcStep = 1000;
				}
				else
				{
					m_nRetry = 0;

					CTL_Lib.Alarm_Error_Occurrence(516, dWARNING, _T("550002"));

					m_nMcStep = 1000;

					return RET_ERROR;
				}
			}
			else
			{
				m_dwWaitTime[1] = GetTickCount();
				m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];

				if (m_dwWaitTime[2] <= 0)
				{
					m_dwWaitTime[0] = GetTickCount();
					break;
				}

				if (m_dwWaitTime[2] > 5000)
				{
					m_nRetry++;
					if (m_nRetry < st_basic_info.nRetry)
					{
						m_nMcStep = 1000;
					}
					else
					{
						m_nRetry = 0;

						CTL_Lib.Alarm_Error_Occurrence(517, dWARNING, _T("550002"));

						m_nMcStep = 1000;

						return RET_ERROR;
					}
				}
			}*/
			break;
	}

	return RET_PROCEED;
}

void CScreenInitial::OnMessageView()
{
	if(m_pMsgView == NULL)
	{
		m_pMsgView					= new CDialogMessageView;
		m_pMsgView->m_nConfirmType	= st_other_info.nConfirmType;
		m_pMsgView->m_strMessage	= st_other_info.strConfirmMsg;
		m_pMsgView->Create();
		m_pMsgView->ShowWindow(SW_SHOW);
	}
	else
	{
		//				m_pMsgView->SetFocus();
		m_pMsgView->ShowWindow(SW_SHOW);
	}
}