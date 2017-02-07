// Dialog_Title.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Handler.h"
#include "Dialog_Title.h"
#include "afxdialogex.h"
#include "Variable.h"
#include "PublicFunction.h"
#include "Dialog_KeyPad.h"
#include ".\\Ctrl\\KeyBoard.h"

// CDialog_Title 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialog_Title, CInitDialogBar)

CDialog_Title::CDialog_Title()
	: CInitDialogBar()
{
	m_bInlineMode = FALSE;
	m_nInlineMode = NO;
	m_btmBk.LoadBitmap(IDB_BITMAP_TITLE_BACKGROUND);   
}

CDialog_Title::~CDialog_Title()
{
}

void CDialog_Title::DoDataExchange(CDataExchange* pDX)
{
	CInitDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSG_FORM_NAME,				m_msgFormName);
	DDX_Control(pDX, IDC_MSG_INTERFACE_MODE_INFO,	m_msgInterfaceModeInfo);
	DDX_Control(pDX, IDC_MSG_DEVICE_MODE_INFO,		m_msgDeviceModeInfo);
	DDX_Control(pDX, IDC_MSG_MODEL_NAME,			m_msgModelName);
	DDX_Control(pDX, IDC_MSG_STATUS_INFO,			m_msgStatusInfo);
	DDX_Control(pDX, IDC_MSG_MES_MODE_INFO,			m_msgMesModeInfo);
	DDX_Control(pDX, IDC_GIF_TITLE_LOGO,			m_gifTitleLogo);
	DDX_Control(pDX, IDC_MSG_MES_ABORT_RETEST,		m_msgOtherInfo);
}


BEGIN_MESSAGE_MAP(CDialog_Title, CInitDialogBar)
	ON_MESSAGE(WM_STATUS_CHANGE, OnStatusDataUpdate)  // 타이틀 바 화면으로 전송된 메시지 출력시키기 위한 사용자 정의 메시지 추가 
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_STN_CLICKED(IDC_MSG_MES_ABORT_RETEST, &CDialog_Title::OnStnClickedMsgMesAbortRetest)
END_MESSAGE_MAP()


// CDialog_Title 메시지 처리기입니다.


BOOL CDialog_Title::OnInitDialogBar()
{
	CInitDialogBar::OnInitDialogBar();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	OnInit_Label();
	OnInitButton();
	OnInit_Picture();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDialog_Title::OnInitButton()
{
}


void CDialog_Title::OnInit_Label()
{
	m_msgFormName.SetTextFont(clsFunc.m_pFont[1]);
	m_msgFormName.SetColor(RGB(181, 182, 140), RGB(0, 0, 0));
	m_msgFormName.SetTextAlign(DT_LEFT);

	m_msgModelName.SetFont(clsFunc.m_pFont[1]);
	m_msgModelName.SetCenterText();
	m_msgModelName.SetColor(BLUE_D);
	m_msgModelName.SetGradientColor(BLUE_D);
	m_msgModelName.SetTextColor(RGB(255,255,255));

	m_msgDeviceModeInfo.SetFont(clsFunc.m_pFont[1]);
	m_msgDeviceModeInfo.SetCenterText();
	m_msgDeviceModeInfo.SetColor(RGB(0,0,255));
	m_msgDeviceModeInfo.SetGradientColor(RGB(0,0,0));
	m_msgDeviceModeInfo.SetTextColor(RGB(255,255,255));

	m_msgInterfaceModeInfo.SetFont(clsFunc.m_pFont[1]);
	m_msgInterfaceModeInfo.SetCenterText();
	m_msgInterfaceModeInfo.SetColor(RGB(0,0,255));
	m_msgInterfaceModeInfo.SetGradientColor(RGB(0,0,255));
	m_msgInterfaceModeInfo.SetTextColor(RGB(255,255,255));

	m_msgStatusInfo.SetFont(clsFunc.m_pFont[1]);
	m_msgStatusInfo.SetCenterText();
	m_msgStatusInfo.SetColor(RGB(0,0,255));
	m_msgStatusInfo.SetGradientColor(RGB(0,0,0));
	m_msgStatusInfo.SetTextColor(RGB(255,255,255));

	m_msgMesModeInfo.SetFont(clsFunc.m_pFont[1]);
	m_msgMesModeInfo.SetCenterText();
	m_msgMesModeInfo.SetColor(RGB(0,0,255));
	m_msgMesModeInfo.SetGradientColor(RGB(0,0,0));
	m_msgMesModeInfo.SetTextColor(RGB(255,255,255));

	m_msgOtherInfo.SetFont(clsFunc.m_pFont[1]);
	m_msgOtherInfo.SetCenterText();
	m_msgOtherInfo.SetColor(RGB(0,0,255));
	m_msgOtherInfo.SetGradientColor(RGB(0,0,0));
	m_msgOtherInfo.SetTextColor(RGB(255,255,255));
}

void CDialog_Title::OnInit_Picture()
{
	//kwlee 
// 	if (m_gifTitleLogo.Load(MAKEINTRESOURCE(IDR_GIF_LOGO),_T("GIF")))
// 	{
// 		m_gifTitleLogo.Draw();
// 	}
}

BOOL CDialog_Title::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	return CInitDialogBar::DestroyWindow();
}

LRESULT CDialog_Title::OnStatusDataUpdate(WPARAM wParam,LPARAM lParam)
{
	CString  strTemp;

	if (wParam == FORM_NAME)			// 화면 뷰 전환인 경우 
	{
		OnTitleFormNameChange((int)lParam); // 현재 출력된 뷰 화면 이름 정보 출력 함수

		strTemp.Format(_T("ABORT : %d / %d RETEST : %d"), st_recipe_info.nAbortTime,
														  st_lot_info[LOT_CURR].nAbortTime,
														  st_recipe_info.nTestRetest_Count);
		m_msgOtherInfo.SetWindowText(strTemp);
	}
	else if (wParam == MACHINE_STATUS)	// 장비 상태 정보 변경인 경우 
	{
		OnTitleStatusChange((int)lParam);
	}
	else if (wParam == DEVICE_MODE)	// Device Mode
	{
		OnTitleDeviceModeDisplay((int)lParam);
	}
	else if (wParam == MODEL_MODE)	// Device Mode
	{
		OnTitleModelNameDisplay();
	}
	else if (wParam == INTERFACE_MODE)
	{
		OnTitleInterfaceDisplay((int)lParam);
	}
	else if (wParam == XGEM_MODE)
	{
		OnTitleMesDisplay((int)lParam);
	}
	else if (wParam == COMMUNICATION_MODE)
	{
		strTemp.Format(_T("ABORT : %d / %d RETEST : %d"), st_recipe_info.nAbortTime,
														  st_lot_info[LOT_CURR].nAbortTime,
														  st_recipe_info.nTestRetest_Count);
		m_msgOtherInfo.SetWindowText(strTemp);
	}

	return 0;
}

void CDialog_Title::OnTitleFormNameChange(int nViewID)
{
	CString strFormName;  // 현재 설정된 화면 정보 저장 변수 

	switch(nViewID)
	{
		case IDW_SCREEN_MAIN:
			strFormName = _T("\n");
			if (st_handler_info.nFtpVer == 0)
			{
				strFormName += _T(" Screen: Main (Not Used)");
			}
			else if (st_handler_info.nFtpVer == 1)
			{
				strFormName += _T(" Screen: Main (LOCAL)");
			}
			else
			{
				strFormName += _T(" Screen: Main (FTP)");
			}
			break;

		case IDW_SCREEN_LOCK:
			strFormName = _T("\n");
			strFormName += _T(" Screen: System Lock");
			break;

		case IDW_SCREEN_INIT:
			strFormName = _T("\n");
			strFormName += _T(" Screen: Initial");
			break;

		case IDW_SCREEN_BASIC :
			strFormName = _T("\n");
			strFormName += _T(" Screen: Basic");
			break;

		case IDW_SCREEN_SET_MAINTENANCE :
			strFormName += _T("\n");
			strFormName += _T(" Screen: Maintenance");
			break;

		case IDW_SCREEN_WAIT_TIME :
			strFormName += _T("\n");
			strFormName += _T(" Screen: Wait Time");
			break;

		case IDW_SCREEN_SET_RECIPE:
			strFormName += _T("\n");
			strFormName += _T(" Screen: Recipe");
			break;

		case IDW_SCREEN_SET_RFID:
			strFormName += _T("\n");
			strFormName += _T(" Screen: RFID");
			break;

		case IDW_SCREEN_SET_FTP:
			strFormName += _T("\n");
			strFormName += _T(" Screen: FTP");
			break;

		case IDW_SCREEN_SET_COK:
			strFormName += _T("\n");
			strFormName += _T(" Screen: COK");
			break;

		case IDW_SCREEN_SET_PART_NO:
			strFormName += _T("\n");
			strFormName += _T(" Screen: Part No.");
			break;
		
		case IDW_SCREEN_MOTOR_SPEED :
			strFormName += _T("\n");
			strFormName += _T(" Screen: Speed");
			break;

		case IDW_SCREEN_LIST :
			strFormName += _T("\n");
			strFormName += _T(" Screen: List");
			break;

		case IDW_SCREEN_IO_FUNCTION :
			strFormName += _T("\n");
			strFormName += _T(" Screen: Custom IO Function");
			break;

		case IDW_SCREEN_LIST_WORK :
			strFormName = _T(" Menu: List");
			strFormName += _T("\n");
			strFormName += _T(" Screen: Work");
			break;

		case IDW_SCREEN_LIST_ALARM :
			strFormName = _T(" Menu: List");
			strFormName += _T("\n");
			strFormName += _T(" Screen: Alarm");
			break;

		case IDW_SCREEN_LIST_STEP :
			strFormName = _T(" Menu: List");
			strFormName += _T("\n");
			strFormName += _T(" Screen: Step");
			break;

		case IDW_SCREEN_LIST_DATA_VIEW:
			strFormName = _T(" Menu: List");
			strFormName += _T("\n");
			strFormName += _T(" Screen: Data");
			break;

		case IDW_SCREEN_IO_MAP :
			strFormName = _T("\n");
			strFormName += _T(" Screen: I/O");
			break;

		case IDW_SCREEN_MOTOR:
			strFormName = _T("\n");
			strFormName += _T(" Screen: Motor Teach");
			break;

		case IDW_SCREEN_SET_MEASURE:
			strFormName = _T(" Menu: Setting");
			strFormName = _T("\n");
			strFormName += _T(" Screen: Measure");
			break;

		case IDW_SCREEN_SET_INTERFACE:
			strFormName += _T("\n");
			strFormName += _T(" Screen: Interface");
			break;

		case IDW_SCREEN_WORK_INFO:
			strFormName += _T("\n");
			strFormName += _T(" Screen: Maintenance");
			break;

		default:
			strFormName = _T(" ERROR: SCREEN VIEW ID");
			break;
	}

	m_msgFormName.SetText((LPCWSTR)strFormName);
}

void CDialog_Title::OnTitleDeviceModeDisplay(int nDevice)
{
	CString strDevice;   // 임시 저장 변수
	
	switch(nDevice)
	{
		case WITHOUT_DVC:
			m_msgDeviceModeInfo.SetColor(RGB(255,0,0));
			strDevice = _T("WITHOUT DEVICE");
			break;

		case WITH_DVC:
			m_msgDeviceModeInfo.SetColor(RGB(0,0,255));
			strDevice = _T("WITH DEVICE");
			break;

		case WITHOUT_TRAY:
			m_msgDeviceModeInfo.SetColor(RGB(255,0,0));
			strDevice = _T("NO C-TRAY");
			break;
		default:
			m_msgDeviceModeInfo.SetColor(RGB(255,0,0));
			strDevice = _T("ERROR");
			break;
	}
	
	m_msgDeviceModeInfo.SetWindowText(strDevice);
}

void CDialog_Title::OnTitleStatusChange(int nStatus)
{
	switch (nStatus)
	{
		case dSTOP :		// STOP 상태
			m_msgStatusInfo.SetWindowText(_T("STOP"));
			m_msgStatusInfo.SetTextColor(RGB(255,0,0));
			break;

		case dRUN :			// RUN 상태
			m_msgStatusInfo.SetWindowText(_T("RUN"));
			m_msgStatusInfo.SetTextColor(RGB(0, 0, 255));
			break;

		case dJAM :			// ALARM 상태
			m_msgStatusInfo.SetWindowText(_T("ALARM"));
			m_msgStatusInfo.SetTextColor(RGB(255,0,0));
			break;

		case dWARNING :		// ALARM 상태
			m_msgStatusInfo.SetWindowText(_T("ALARM"));
			m_msgStatusInfo.SetTextColor(RGB(255,0,0));
			break;

		case dMAINT :		// MAINT 상태
			m_msgStatusInfo.SetWindowText(_T("MAINT"));
			m_msgStatusInfo.SetTextColor(RGB(0,0,200));
			break;

		case dINIT :		// INIT 상태
			m_msgStatusInfo.SetWindowText(_T("INIT"));
			m_msgStatusInfo.SetTextColor(RGB(50,100,150));
			break;

		case dLOTEND :		// LOT END 상태
			m_msgStatusInfo.SetWindowText(_T("LOTEND"));
			m_msgStatusInfo.SetTextColor(RGB(50,100,150));
			break;

		case dLOCK :		// LOCK 상태
			m_msgStatusInfo.SetWindowText(_T("LOCK"));
			m_msgStatusInfo.SetTextColor(RGB(50,100,150));
			break;

		case dSELFCHECK :	// SELFCHECK 상태
			m_msgStatusInfo.SetWindowText(_T("SELFCHECK"));
			m_msgStatusInfo.SetTextColor(RGB(50,100,150));
			break;
		default :
			break;
	}
	
	m_msgStatusInfo.SetColor(RGB(255,255,255));
	m_msgStatusInfo.SetGradientColor(RGB(0,255,0));
}

void CDialog_Title::OnTitleModelNameDisplay()
{
	m_msgModelName.SetWindowText(st_basic_info.strDeviceName);
}


BOOL CDialog_Title::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CDC  memDC;                     // 메모리 DC
	CBitmap * pOldBitmap = NULL;    // m_BackBitmap으로 새로운 그림을 DC에 그릴 때, 이전 그려졌던 DC(즉, Bitmap)을 저장.
	BITMAP bitmapInfo;              // 그림의 정보(m_BackBitmap)
	m_btmBk.GetBitmap(&bitmapInfo); // Bitmap 크기 구함.

	memDC.CreateCompatibleDC(pDC);  // 메모리 DC 생성
	pOldBitmap = memDC.SelectObject(&m_btmBk);
	pDC->BitBlt(0, 0, bitmapInfo.bmWidth, bitmapInfo.bmHeight, &memDC, 0, 0, SRCCOPY);   // 메모리 DC에 그림을 그림

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	return TRUE; // 흰 부러쉬가 아닌 배경을 그려야 하기 때문에 true
}


void CDialog_Title::OnTitleInterfaceDisplay(int nInterfaceMode)
{
	switch (nInterfaceMode)
	{
		case 0 :		// STOP 상태
			m_msgInterfaceModeInfo.SetWindowText(_T("Not Used"));
			m_msgInterfaceModeInfo.SetColor(RGB(255, 0, 0));
			break;

		case 1 :			// RUN 상태
			m_msgInterfaceModeInfo.SetWindowText(_T("Local"));
			m_msgInterfaceModeInfo.SetColor(RGB(0, 255, 0));
			break;

		case 2 :			// RUN 상태
			if (st_work_info.nFtpWork == 1)
			{
				m_msgInterfaceModeInfo.SetWindowText(_T("Ftp (Pass)"));
				m_msgInterfaceModeInfo.SetColor(RGB(0, 255, 0));
			}
			else
			{
				m_msgInterfaceModeInfo.SetWindowText(_T("Ftp (Fail)"));
				m_msgInterfaceModeInfo.SetColor(RGB(0, 200, 200));
			}
			break;
	}
/*
	switch (nInterfaceMode)
	{
		case EQP_OFF_LINE :		// STOP 상태
			m_msgInterfaceModeInfo.SetWindowText(_T("OFF LINE"));
			m_msgInterfaceModeInfo.SetColor(RGB(255, 0, 0));
			break;

		case EQP_ON_LINE :			// RUN 상태
			if (st_lot_info[LOT_CURR].nLotStatus == NO)
			{
				if (st_basic_info.nModeRearSmema == YES)
				{
					m_msgInterfaceModeInfo.SetWindowText(_T("IN-LINE"));
					m_msgInterfaceModeInfo.SetColor(GREEN_C);
				}
				else
				{
					m_msgInterfaceModeInfo.SetWindowText(_T("STANDALONE"));
					m_msgInterfaceModeInfo.SetColor(ORANGE_C);
				}
			}
			else
			{
				if (st_lot_info[LOT_CURR].nTrayRunMode_StandAlone == NO)
				{
					m_msgInterfaceModeInfo.SetWindowText(_T("IN-LINE"));
					m_msgInterfaceModeInfo.SetColor(GREEN_C);
				}
				else
				{
					m_msgInterfaceModeInfo.SetWindowText(_T("STANDALONE"));
					m_msgInterfaceModeInfo.SetColor(ORANGE_C);
				}
			}
			break;
	}
*/
}


void CDialog_Title::OnTitleMesDisplay(int nMesMode)
{
	switch (nMesMode)
	{
		case MES_OFF_LINE:
			m_msgMesModeInfo.SetWindowText(_T("XGEM OFF LINE"));
			m_msgMesModeInfo.SetColor(RGB(255, 0, 0));
			break;

		case MES_ON_LINE_LOCAL:
			m_msgMesModeInfo.SetWindowText(_T("XGEM ON LINE LOCAL"));
			m_msgMesModeInfo.SetColor(GREEN_D);
			break;

		case MES_ON_LINE_REMOTE:
			m_msgMesModeInfo.SetWindowText(_T("XGEM ON LINE REMOTE"));
			m_msgMesModeInfo.SetColor(GREEN_C);
			break;
	}
}




void CDialog_Title::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ¿©±â¿¡ ¸Þ½ÃÁö Ã³¸®±â ÄÚµå¸¦ Ãß°¡ ¹×/¶Ç´Â ±âº»°ªÀ» È£ÃâÇÕ´Ï´Ù.
	switch (nIDEvent)
	{
		case 100:

			break;
	}
	CInitDialogBar::OnTimer(nIDEvent);
}


void CDialog_Title::OnStnClickedMsgMesAbortRetest()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (st_handler_info.nRunStatus != dSTOP) return;

	CString strTemp;

	int nKey = st_lot_info[LOT_CURR].nAbortTime;

	KeyPadI(0, 600, &nKey);

	st_lot_info[LOT_CURR].nAbortTime = nKey;

	strTemp.Format(_T("ABORT : %d / %d RETEST : %d"), st_recipe_info.nAbortTime,
													  st_lot_info[LOT_CURR].nAbortTime,
													  st_recipe_info.nTestRetest_Count);
	m_msgOtherInfo.SetWindowText(strTemp);
}
