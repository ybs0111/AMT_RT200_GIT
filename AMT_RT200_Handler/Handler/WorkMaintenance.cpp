// WorkMaintenance.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Handler.h"
#include "WorkMaintenance.h"
#include "afxdialogex.h"
#include "Variable.h"
#include "PublicFunction.h"
#include "Dialog_KeyBoard.h"
#include "Dialog_KeyPad.h"
#include "Dialog_Message.h"
#include "Ctlbd_Variable.h"
#include "Dialog_Password_Change.h"
#include "ComizoaPublic.h"
#include "Dialog_Pass_Check.h"
#include "FastechPublic_IO.h"
#include ".\\Ctrl\\KeyBoard.h"
// CWorkMaintenance 대화 상자입니다.

IMPLEMENT_DYNAMIC(CWorkMaintenance, CDialog)

CWorkMaintenance::CWorkMaintenance(CWnd* pParent /*=NULL*/)
	: CDialog(CWorkMaintenance::IDD, pParent)
{
	m_bLamp			= FALSE;
}

CWorkMaintenance::~CWorkMaintenance()
{
}

void CWorkMaintenance::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GROUP_LAMP,					m_groupLamp);
	DDX_Control(pDX, IDC_GROUP_SM_LAMP_WARNING,			m_groupLampWarning);
	DDX_Control(pDX, IDC_GROUP_SM_LAMP_STOP,			m_groupLampStop);
	DDX_Control(pDX, IDC_GROUP_SM_LAMP_RUN,				m_groupLampRun);
	DDX_Control(pDX, IDC_GROUP_SM_LAMP_LOTEND,			m_groupLampLotend);
	DDX_Control(pDX, IDC_GROUP_SM_LAMP_LOCK,			m_groupLampLock);
	DDX_Control(pDX, IDC_GROUP_SM_LAMP_FLICKER_TIME,	m_groupLampFlickerTime);
	DDX_Control(pDX, IDC_GROUP_SM_LAMP_ALARM,			m_groupLampAlarm);
	DDX_Control(pDX, IDC_GROUP_SM_LAMP_IDLE,			m_groupLampIdle);
	DDX_Control(pDX, IDC_GROUP_SM_BUZZER,				m_groupBuzzer);
	DDX_Control(pDX, IDC_GROUP_SM_PASSWORD_CHANGE,		m_groupPassword);
	DDX_Control(pDX, IDC_GROUP_COMMAND,					m_groupCommand);
	DDX_Control(pDX, IDC_GROUP_SPEED,					m_groupSpeed);
	DDX_Control(pDX, IDC_GROUP_OFFSET,					m_groupOffset);
	DDX_Control(pDX, IDC_GROUP_MAINTENANCE_EQP_INFO,	m_groupEqpInfo);
	DDX_Control(pDX, IDC_SM_LAMP_WARNING,				m_stLampWarning);
	DDX_Control(pDX, IDC_SM_LAMP_STOP,					m_stLampStop);
	DDX_Control(pDX, IDC_SM_LAMP_RUN,					m_stLampRun);
	DDX_Control(pDX, IDC_SM_LAMP_LOTEND,				m_stLampLotend);
	DDX_Control(pDX, IDC_SM_LAMP_LOCK,					m_stLampLock);
	DDX_Control(pDX, IDC_SM_LAMP_ALARM,					m_stLampAlarm);
	DDX_Control(pDX, IDC_SM_LAMP_IDLE,					m_stLampIdle);
	DDX_Control(pDX, IDC_BTN_APPLY,						m_btnApply);
	DDX_Control(pDX, IDC_BTN_RELOAD,					m_btnReLoad);
	DDX_Control(pDX, IDC_BTN_BUZZER,					m_btnBuzzer);
	DDX_Control(pDX, IDC_BTN_MAINT_CYLINDER,			m_btnCylinder);
	DDX_Control(pDX, IDC_BTN_SM_PASSWORD_CHANGE_S_LOCK,	m_btnPasswordSystem);
	DDX_Control(pDX, IDC_BTN_SM_PASSWORD_CHANGE_LEVLE1,	m_btnPasswordLevel1);
	DDX_Control(pDX, IDC_BTN_SM_PASSWORD_CHANGE_LEVEL2,	m_btnPasswordLevel2);
	DDX_Control(pDX, IDC_DGT_SM_LAMP_FLICKER_TIME,		m_dgtFlickerTime);
	DDX_Control(pDX, IDC_DGT_RUN_SPEED,					m_dgtRunSpeed);
	DDX_Control(pDX, IDC_DGT_MANUAL_SPEED,				m_dgtManualSpeed);
	DDX_Control(pDX, IDC_DGT_REJ_TRAY_PICK_OFFSET,		m_dgtRejTrayPick);		
	DDX_Control(pDX, IDC_DGT_REJ_TRAY_PLACE_OFFSET,		m_dgtRejTrayPlace);				
	DDX_Control(pDX, IDC_MSG_RUN_SPEED,					m_msgRunSpeed);
	DDX_Control(pDX, IDC_MSG_MANUAL_SPEED,				m_msgManualSpeed);
	DDX_Control(pDX, IDC_MSG_MAINTENANCE_EQP,			m_msgEqp);
	DDX_Control(pDX, IDC_MSG_MAINTENANCE_EQP_DATA,		m_msgEqpData);
	DDX_Control(pDX, IDC_MSG_REJ_TRAY_PICK,				m_msgRejTrayPick);
	DDX_Control(pDX, IDC_MSG_REJ_TRAY_PLACE,				m_msgRejTrayPlace);
	DDX_Control(pDX, IDC_CUSTOM_MAINTENANCE_XGEM,		m_pGridXgem); //2015.02.09
	DDX_Control(pDX, IDC_GROUP_MAINTENANCE_XGEM_INFO,		m_groupXgemInfo); //2015.02.09
}


BEGIN_MESSAGE_MAP(CWorkMaintenance, CDialog)
	ON_WM_PAINT()
	ON_COMMAND_RANGE(ID_LAMP_RLAMP, ID_LAMP_RLAMP+SELFCHECK, OnRLampClick)
	ON_COMMAND_RANGE(ID_LAMP_YLAMP, ID_LAMP_YLAMP+SELFCHECK, OnYLampClick)
	ON_COMMAND_RANGE(ID_LAMP_GLAMP, ID_LAMP_GLAMP+SELFCHECK, OnGLampClick)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_APPLY, &CWorkMaintenance::OnBnClickedBtnApply)
	ON_BN_CLICKED(IDC_BTN_SM_PASSWORD_CHANGE_S_LOCK, &CWorkMaintenance::OnBnClickedBtnSmPasswordChangeSLock)
	ON_BN_CLICKED(IDC_BTN_SM_PASSWORD_CHANGE_LEVLE1, &CWorkMaintenance::OnBnClickedBtnSmPasswordChangeLevle1)
	ON_BN_CLICKED(IDC_BTN_SM_PASSWORD_CHANGE_LEVEL2, &CWorkMaintenance::OnBnClickedBtnSmPasswordChangeLevel2)
	ON_BN_CLICKED(IDC_BTN_BUZZER, &CWorkMaintenance::OnClickedBtnBuzzer)
	ON_STN_CLICKED(IDC_DGT_SM_LAMP_FLICKER_TIME, &CWorkMaintenance::OnStnClickedDgtSmLampFlickerTime)
	ON_STN_CLICKED(IDC_DGT_RUN_SPEED, &CWorkMaintenance::OnClickedDgtRunSpeed)
	ON_STN_CLICKED(IDC_DGT_MANUAL_SPEED, &CWorkMaintenance::OnClickedDgtManualSpeed)
	ON_WM_DESTROY()
	ON_STN_CLICKED(IDC_MSG_MAINTENANCE_EQP_DATA, &CWorkMaintenance::OnStnClickedMsgMaintenanceEqpData)
	ON_STN_CLICKED(IDC_DGT_REJ_TRAY_PICK_OFFSET, &CWorkMaintenance::OnStnClickedDgtRejTrayPickOffset)
	ON_STN_CLICKED(IDC_DGT_REJ_TRAY_PLACE_OFFSET, &CWorkMaintenance::OnStnClickedDgtRejTrayPlaceOffset)
	ON_NOTIFY(NM_CLICK, IDC_CUSTOM_MAINTENANCE_XGEM, OnXgemCellClick)//2015.02.09
	ON_BN_CLICKED(IDC_BTN_MAINT_CYLINDER, &CWorkMaintenance::OnBnClickedBtnMaintCylinder)
END_MESSAGE_MAP()


// CWorkMaintenance 메시지 처리기입니다.


BOOL CWorkMaintenance::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
/*
	if (OnDataComparison() == RET_ERROR)
	{
		OnDataHistoryLog();
		OnDataApply();
	}
*/
	return CDialog::DestroyWindow();
}


BOOL CWorkMaintenance::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	OnDataInit();

	OnInitGroupBox();
	OnInitLabel();
	OnInitButton();
	OnInitDigit();
	OnInitGridXgem();//2015.02.09
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CWorkMaintenance::OnInitGroupBox()
{
	m_groupLampRun.SetFont(clsFunc.OnLogFont(16));
	m_groupLampRun.SetCatptionTextColor(BLUE_C);
	m_groupLampRun.SetBorderColor(ORANGE_C);
	m_groupLampRun.SetBackgroundColor(WINDOW_UP1);
	m_groupLampRun.SetFontBold(TRUE);
	
	m_groupLampStop.SetFont(clsFunc.OnLogFont(16));
	m_groupLampStop.SetCatptionTextColor(BLUE_C);
	m_groupLampStop.SetBorderColor(ORANGE_C);
	m_groupLampStop.SetBackgroundColor(WINDOW_UP1);
	m_groupLampStop.SetFontBold(TRUE);
	
	m_groupLampAlarm.SetFont(clsFunc.OnLogFont(16));
	m_groupLampAlarm.SetCatptionTextColor(BLUE_C);
	m_groupLampAlarm.SetBorderColor(ORANGE_C);
	m_groupLampAlarm.SetBackgroundColor(WINDOW_UP1);
	m_groupLampAlarm.SetFontBold(TRUE);
	
	m_groupLampWarning.SetFont(clsFunc.OnLogFont(16));
	m_groupLampWarning.SetCatptionTextColor(BLUE_C);
	m_groupLampWarning.SetBorderColor(ORANGE_C);
	m_groupLampWarning.SetBackgroundColor(WINDOW_UP1);
	m_groupLampWarning.SetFontBold(TRUE);
	
	m_groupLampLock.SetFont(clsFunc.OnLogFont(16));
	m_groupLampLock.SetCatptionTextColor(BLUE_C);
	m_groupLampLock.SetBorderColor(ORANGE_C);
	m_groupLampLock.SetBackgroundColor(WINDOW_UP1);
	m_groupLampLock.SetFontBold(TRUE);
	
	m_groupLampLotend.SetFont(clsFunc.OnLogFont(16));
	m_groupLampLotend.SetCatptionTextColor(BLUE_C);
	m_groupLampLotend.SetBorderColor(ORANGE_C);
	m_groupLampLotend.SetBackgroundColor(WINDOW_UP1);
	m_groupLampLotend.SetFontBold(TRUE);

	m_groupLampIdle.SetFont(clsFunc.OnLogFont(16));
	m_groupLampIdle.SetCatptionTextColor(BLUE_C);
	m_groupLampIdle.SetBorderColor(ORANGE_C);
	m_groupLampIdle.SetBackgroundColor(WINDOW_UP1);
	m_groupLampIdle.SetFontBold(TRUE);
	
	m_groupLampFlickerTime.SetFont(clsFunc.OnLogFont(16));
	m_groupLampFlickerTime.SetCatptionTextColor(BLUE_C);
	m_groupLampFlickerTime.SetBorderColor(ORANGE_C);
	m_groupLampFlickerTime.SetBackgroundColor(WINDOW_UP1);
	m_groupLampFlickerTime.SetFontBold(TRUE);
	
	m_groupBuzzer.SetFont(clsFunc.OnLogFont(16));
	m_groupBuzzer.SetCatptionTextColor(BLUE_C);
	m_groupBuzzer.SetBorderColor(ORANGE_C);
	m_groupBuzzer.SetBackgroundColor(WINDOW_UP1);
	m_groupBuzzer.SetFontBold(TRUE);

	m_groupLamp.SetFont(clsFunc.OnLogFont(16));
	m_groupLamp.SetCatptionTextColor(BLUE_C);
	m_groupLamp.SetBorderColor(ORANGE_C);
	m_groupLamp.SetBackgroundColor(WINDOW_UP1);
	m_groupLamp.SetFontBold(TRUE);

	m_groupPassword.SetFont(clsFunc.OnLogFont(16));
	m_groupPassword.SetCatptionTextColor(BLUE_C);
	m_groupPassword.SetBorderColor(ORANGE_C);
	m_groupPassword.SetBackgroundColor(WINDOW_UP1);
	m_groupPassword.SetFontBold(TRUE);

	m_groupCommand.SetFont(clsFunc.OnLogFont(16));
	m_groupCommand.SetCatptionTextColor(BLUE_C);
	m_groupCommand.SetBorderColor(ORANGE_C);
	m_groupCommand.SetBackgroundColor(WINDOW_UP1);
	m_groupCommand.SetFontBold(TRUE);

	m_groupSpeed.SetFont(clsFunc.OnLogFont(16));
	m_groupSpeed.SetCatptionTextColor(BLUE_C);
	m_groupSpeed.SetBorderColor(ORANGE_C);
	m_groupSpeed.SetBackgroundColor(WINDOW_UP1);
	m_groupSpeed.SetFontBold(TRUE);

	m_groupEqpInfo.SetFont(clsFunc.OnLogFont(16));
	m_groupEqpInfo.SetCatptionTextColor(BLUE_C);
	m_groupEqpInfo.SetBorderColor(ORANGE_C);
	m_groupEqpInfo.SetBackgroundColor(WINDOW_UP1);
	m_groupEqpInfo.SetFontBold(TRUE);

	m_groupOffset.SetFont(clsFunc.OnLogFont(16));
	m_groupOffset.SetCatptionTextColor(BLUE_C);
	m_groupOffset.SetBorderColor(ORANGE_C);
	m_groupOffset.SetBackgroundColor(WINDOW_UP1);
	m_groupOffset.SetFontBold(TRUE);

	//2015.02.09
	m_groupXgemInfo.SetFont(clsFunc.OnLogFont(16));
	m_groupXgemInfo.SetCatptionTextColor(BLUE_C);
	m_groupXgemInfo.SetBorderColor(ORANGE_C);
	m_groupXgemInfo.SetBackgroundColor(WINDOW_UP1);
	m_groupXgemInfo.SetFontBold(TRUE);
}


void CWorkMaintenance::OnInitLabel()
{
	m_msgRunSpeed.SetFont(clsFunc.m_pFont[2]);
	m_msgRunSpeed.SetWindowText(_T("Run Speed"));
	m_msgRunSpeed.SetCenterText();
	m_msgRunSpeed.SetColor(WHITE_C);
	m_msgRunSpeed.SetGradientColor(GREEN_C);
	m_msgRunSpeed.SetTextColor(BLACK_C);

	m_msgManualSpeed.SetFont(clsFunc.m_pFont[2]);
	m_msgManualSpeed.SetWindowText(_T("Manual Speed"));
	m_msgManualSpeed.SetCenterText();
	m_msgManualSpeed.SetColor(WHITE_C);
	m_msgManualSpeed.SetGradientColor(ORANGE_C);
	m_msgManualSpeed.SetTextColor(BLACK_C);

	m_msgRejTrayPick.SetFont(clsFunc.m_pFont[2]);
	m_msgRejTrayPick.SetWindowText(_T("Reject Tray Pick"));
	m_msgRejTrayPick.SetCenterText();
	m_msgRejTrayPick.SetColor(WHITE_C);
	m_msgRejTrayPick.SetGradientColor(ORANGE_C);
	m_msgRejTrayPick.SetTextColor(BLACK_C);

	m_msgRejTrayPlace.SetFont(clsFunc.m_pFont[2]);
	m_msgRejTrayPlace.SetWindowText(_T("Reject Tray Place"));
	m_msgRejTrayPlace.SetCenterText();
	m_msgRejTrayPlace.SetColor(WHITE_C);
	m_msgRejTrayPlace.SetGradientColor(ORANGE_C);
	m_msgRejTrayPlace.SetTextColor(BLACK_C);

	m_msgEqp.SetFont(clsFunc.m_pFont[2]);
	m_msgEqp.SetWindowText(_T("EQP ID"));
	m_msgEqp.SetCenterText();
	m_msgEqp.SetColor(WHITE_C);
	m_msgEqp.SetGradientColor(ORANGE_C);
	m_msgEqp.SetTextColor(BLACK_C);

	m_msgEqpData.SetFont(clsFunc.m_pFont[2]);
	m_msgEqpData.SetWindowText(m_strEqp[1]);
	m_msgEqpData.SetCenterText();
	m_msgEqpData.SetColor(WHITE_C);
	m_msgEqpData.SetGradientColor(WHITE_C);
	m_msgEqpData.SetTextColor(BLACK_C);
}

void CWorkMaintenance::OnInitDigit()
{
	m_dgtFlickerTime.SetStyle(CDigit::DS_INT, 5, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	m_dgtFlickerTime.SetVal(m_nLampFlickerTime[1]);

	m_dgtRunSpeed.SetStyle(CDigit::DS_INT, 5, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	m_dgtRunSpeed.SetVal(m_nRunSpeed[1]);

	m_dgtManualSpeed.SetStyle(CDigit::DS_INT, 5, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	m_dgtManualSpeed.SetVal(m_nManualSpeed[1]);

	m_dgtRejTrayPick.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	m_dgtRejTrayPick.SetFloatVal((float)m_dRejTrayPick[1]);

	m_dgtRejTrayPlace.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	m_dgtRejTrayPlace.SetFloatVal((float)m_dRejTrayPlace[1]);
}


void CWorkMaintenance::OnInitButton()
{
	m_btnApply.SetBitmaps(IDC_BTN_APPLY, IDB_BITMAP_APPLY_DN1, WINDOW_DN, IDB_BITMAP_APPLY_UP1, WINDOW_UP);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnApply.SetFont(clsFunc.m_pFont[2]);
	m_btnApply.SetTooltipText(_T("Maintenance Data Apply"));

	m_btnReLoad.SetBitmaps(IDC_BTN_RELOAD, IDB_BITMAP_RELOAD_DN1, WINDOW_DN, IDB_BITMAP_RELOAD_UP1, WINDOW_UP);
	m_btnReLoad.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnReLoad.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnReLoad.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnReLoad.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnReLoad.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnReLoad.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnReLoad.SetFont(clsFunc.m_pFont[2]);
	m_btnReLoad.SetTooltipText(_T("Maintenance Data ReLoad"));

	m_btnPasswordSystem.SetBitmaps(IDC_BTN_SM_PASSWORD_CHANGE_S_LOCK, IDB_BITMAP_PASSWORD_DN, WINDOW_DN, IDB_BITMAP_PASSWORD_UP, WINDOW_UP);
	m_btnPasswordSystem.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnPasswordSystem.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnPasswordSystem.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnPasswordSystem.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnPasswordSystem.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnPasswordSystem.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnPasswordSystem.SetFont(clsFunc.m_pFont[2]);
	m_btnPasswordSystem.SetTooltipText(_T("System Password Change"));

	m_btnPasswordLevel1.SetBitmaps(IDC_BTN_SM_PASSWORD_CHANGE_LEVLE1, IDB_BITMAP_PASSWORD_DN, WINDOW_DN, IDB_BITMAP_PASSWORD_UP, WINDOW_UP);
	m_btnPasswordLevel1.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnPasswordLevel1.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnPasswordLevel1.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnPasswordLevel1.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnPasswordLevel1.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnPasswordLevel1.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnPasswordLevel1.SetFont(clsFunc.m_pFont[2]);
	m_btnPasswordLevel1.SetTooltipText(_T("Level1 Password Change"));

	m_btnPasswordLevel2.SetBitmaps(IDC_BTN_SM_PASSWORD_CHANGE_LEVEL2, IDB_BITMAP_PASSWORD_DN, WINDOW_DN, IDB_BITMAP_PASSWORD_UP, WINDOW_UP);
	m_btnPasswordLevel2.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnPasswordLevel2.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnPasswordLevel2.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnPasswordLevel2.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnPasswordLevel2.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnPasswordLevel2.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnPasswordLevel2.SetFont(clsFunc.m_pFont[2]);
	m_btnPasswordLevel2.SetTooltipText(_T("Level2 Password Change"));

	if(m_nModeBuzzer[1] == YES)
	{
		m_btnBuzzer.SetBitmaps(IDC_BTN_SM_PASSWORD_CHANGE_LEVEL2, IDB_BITMAP_SOUND_ON, WINDOW_UP1, IDB_BITMAP_SOUND_ON, WINDOW_UP1);
		m_btnBuzzer.SetWindowTextW(_T("Buzzer On"));
	}
	else
	{
		m_btnBuzzer.SetBitmaps(IDC_BTN_SM_PASSWORD_CHANGE_LEVEL2, IDB_BITMAP_SOUND_OFF, WINDOW_UP1, IDB_BITMAP_SOUND_OFF, WINDOW_UP1);
		m_btnBuzzer.SetWindowTextW(_T("Buzzer Off"));
	}
	m_btnBuzzer.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_UP1);
	m_btnBuzzer.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnBuzzer.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnBuzzer.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLUE_C);
	m_btnBuzzer.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnBuzzer.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnBuzzer.SetFont(clsFunc.m_pFont[2]);
	m_btnBuzzer.SetTooltipText(_T("Buzzer On/Off Change"));

	m_btnCylinder.SetBitmaps(IDC_BTN_MAINT_CYLINDER, IDB_BITMAP_APPLY_DN1, WINDOW_DN, IDB_BITMAP_APPLY_UP1, WINDOW_UP);
	m_btnCylinder.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnCylinder.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnCylinder.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnCylinder.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnCylinder.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnCylinder.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnCylinder.SetFont(clsFunc.m_pFont[2]);
	m_btnCylinder.SetTooltipText(_T("SSD All Remove"));
}


void CWorkMaintenance::OnDataHistoryLog()
{
	int i;
	CString strMsg;
	
	if(m_nLampFlickerTime[0]		!= m_nLampFlickerTime[1])
	{
		strMsg.Format(_T("[Maintenance] Lamp FlickerTime Change %d -> %d"), m_nLampFlickerTime[0], m_nLampFlickerTime[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}

	if(m_nModeBuzzer[0]				!= m_nModeBuzzer[1])
	{
		strMsg.Format(_T("[Maintenance] Buzzer Mode Change %d -> %d"), m_nModeBuzzer[0], m_nModeBuzzer[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}

	if(m_nModeLampBlue[0]			!= m_nModeLampBlue[1])
	{
		strMsg.Format(_T("[Maintenance] Blue Lamp Change %d -> %d"), m_nModeLampBlue[0], m_nModeLampBlue[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}

	if(m_nModeLampBlueFlicker[0]	!= m_nModeLampBlueFlicker[1])
	{
		strMsg.Format(_T("[Maintenance] Blue Flicker Change %d -> %d"), m_nModeLampBlueFlicker[0], m_nModeLampBlueFlicker[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}

	if(m_nRunSpeed[0]				!= m_nRunSpeed[1])
	{
		strMsg.Format(_T("[Maintenance] Run Speed Change %d -> %d"), m_nRunSpeed[0], m_nRunSpeed[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}

	if(m_nManualSpeed[0]			!= m_nManualSpeed[1])
	{
		strMsg.Format(_T("[Maintenance] Manual Speed Change %d -> %d"), m_nManualSpeed[0], m_nManualSpeed[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}

	if(m_dRejTrayPick[0]			!= m_dRejTrayPick[1])
	{
		strMsg.Format(_T("[Maintenance] Reject Tray Pick Offset Change %0.3f -> %0.3f"), m_dRejTrayPick[0], m_dRejTrayPick[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}

	if(m_dRejTrayPlace[0]			!= m_dRejTrayPlace[1])
	{
		strMsg.Format(_T("[Maintenance] Reject Tray Place Offset Change %d -> %d"), m_dRejTrayPlace[0], m_dRejTrayPlace[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}

	if(m_strEqp[0]					!= m_strEqp[1])
	{
		strMsg.Format(_T("[Maintenance] EQP ID Change %s -> %s"), m_strEqp[0], m_strEqp[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}
	//2015.02.09
	if (m_nModeXgem[0]			!= m_nModeXgem[1])
	{
		strMsg.Format(_T("[Recipe] Xgem Used Change %d -> %d"), m_nModeXgem[0], m_nModeXgem[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}

	if (m_nModeXgemRunChk[0]	!= m_nModeXgemRunChk[1])
	{
		strMsg.Format(_T("[Recipe] Xgem Run Mode Change %d -> %d"), m_nModeXgemRunChk[0], m_nModeXgemRunChk[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}

	if (m_nModeXgemInterface[0]	!= m_nModeXgemInterface[1])
	{
		strMsg.Format(_T("[Recipe] Xgem Interface Mode Change %d -> %d"), m_nModeXgemInterface[0], m_nModeXgemInterface[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}
	//2015.02.09
	for(i=0 ; i<=SELFCHECK ; i++)
	{
		if(m_nRlamp[1][i]				!= m_nRlamp[0][i])
		{
			strMsg.Format(_T("[Maintenance] Red Lamp[%d] Change %d -> %d"), i, m_nRlamp[0][i], m_nRlamp[1][i]);
			clsFunc.OnLogFileAdd(0, strMsg);
		}

		if(m_nYlamp[1][i]				!= m_nYlamp[0][i])
		{
			strMsg.Format(_T("[Maintenance] Yellow Lamp[%d] Change %d -> %d"), i, m_nYlamp[0][i], m_nYlamp[1][i]);
			clsFunc.OnLogFileAdd(0, strMsg);
		}

		if(m_nGlamp[1][i]				!= m_nGlamp[0][i])
		{
			strMsg.Format(_T("[Maintenance] Green Lamp[%d] Change %d -> %d"), i, m_nGlamp[0][i], m_nGlamp[1][i]);
			clsFunc.OnLogFileAdd(0, strMsg);
		}
	}
}


void CWorkMaintenance::OnDataRecovery()
{
	int i;
	
	m_nLampFlickerTime[1]			= m_nLampFlickerTime[0];
	m_nModeBuzzer[1]				= m_nModeBuzzer[0];
	m_nModeLampBlue[1]				= m_nModeLampBlue[0];			
	m_nModeLampBlueFlicker[1]		= m_nModeLampBlueFlicker[0];

	m_nRunSpeed[1]					= m_nRunSpeed[0];
	m_nManualSpeed[1]				= m_nManualSpeed[0];

	m_dRejTrayPick[1]				= m_dRejTrayPick[0];
	m_dRejTrayPlace[1]				= m_dRejTrayPlace[0];

	m_strEqp[1]						= m_strEqp[0];

	for(i = 0 ; i<=SELFCHECK ; i++)
	{
		m_nRlamp[1][i]				= m_nRlamp[0][i];
		m_nYlamp[1][i]				= m_nYlamp[0][i];
		m_nGlamp[1][i]				= m_nGlamp[0][i];
	}
	//2015.02.09
	m_nModeXgem[1]			= m_nModeXgem[0];
	m_nModeXgemRunChk[1]	= m_nModeXgemRunChk[0];
	m_nModeXgemInterface[1] = m_nModeXgemInterface[0];
}


int	CWorkMaintenance::OnDataComparison()
{
	int i;
	
	if(m_nLampFlickerTime[0]		!= m_nLampFlickerTime[1])		return RET_ERROR;
	if(m_nModeBuzzer[0]				!= m_nModeBuzzer[1])			return RET_ERROR;
	if(m_nModeLampBlue[0]			!= m_nModeLampBlue[1])			return RET_ERROR;		
	if(m_nModeLampBlueFlicker[0]	!= m_nModeLampBlueFlicker[1])	return RET_ERROR;

	if(m_nRunSpeed[0]				!= m_nRunSpeed[1])				return RET_ERROR;
	if(m_nManualSpeed[0]			!= m_nManualSpeed[1])			return RET_ERROR;

	if(m_dRejTrayPick[0]			!= m_dRejTrayPick[1])			return RET_ERROR;
	if(m_dRejTrayPlace[0]			!= m_dRejTrayPlace[1])			return RET_ERROR;

	if(m_strEqp[0]					!= m_strEqp[1])					return RET_ERROR;

	for(i=0; i<=SELFCHECK; i++)
	{
		if(m_nRlamp[0][i]			!= m_nRlamp[1][i]) return RET_ERROR;
		if(m_nYlamp[0][i]			!= m_nYlamp[1][i]) return RET_ERROR;
		if(m_nGlamp[0][i]			!= m_nGlamp[1][i]) return RET_ERROR;
	}
	//2015.02.09
	if (m_nModeXgem[0]			!= m_nModeXgem[1])		return RET_ERROR;
	if (m_nModeXgemRunChk[0]	!= m_nModeXgemRunChk[1])return RET_ERROR;
	if (m_nModeXgemInterface[0]	!= m_nModeXgemInterface[1])return RET_ERROR;
	return RET_GOOD;
}


void CWorkMaintenance::OnDataApply()
{

	int i;

	st_lamp_info.nLampWaitTime			= m_nLampFlickerTime[1];
	st_lamp_info.nBuzzerMode			= m_nModeBuzzer[1];
	st_lamp_info.nModeLampBlue			= m_nModeLampBlue[1];			
	st_lamp_info.nModeLampBlueFlicker	= m_nModeLampBlueFlicker[1];

	st_handler_info.nRunSpeed			= m_nRunSpeed[1];
	st_handler_info.nManualSpeed		= m_nManualSpeed[1];

	st_recipe_info.dRejTrayPick			= m_dRejTrayPick[1];
	st_recipe_info.dRejTrayPlace		= m_dRejTrayPlace[1];

	COMI.mn_runspeed_rate				= st_handler_info.nRunSpeed;
	COMI.mn_manualspeed_rate			= st_handler_info.nManualSpeed;

	st_basic_info.strEqp				= m_strEqp[1];

	for(i=0 ; i<=SELFCHECK ; i++)
	{
		st_lamp_info.nLampR[i]			= m_nRlamp[1][i];
		st_lamp_info.nLampY[i]			= m_nYlamp[1][i];
		st_lamp_info.nLampG[i]			= m_nGlamp[1][i];
	}
	//2015.02.09
	st_basic_info.nModeXgem			= m_nModeXgem[1];
	st_basic_info.nModeXgemRunChk	= m_nModeXgemRunChk[1];
	st_basic_info.nModeXgemInterface= m_nModeXgemInterface[1];
}


void CWorkMaintenance::OnDataBackup()
{
	int i;
	
	m_nLampFlickerTime[0]			= m_nLampFlickerTime[1];
	m_nModeBuzzer[0]				= m_nModeBuzzer[1];
	m_nModeLampBlue[0]				= m_nModeLampBlue[1];			
	m_nModeLampBlueFlicker[0]		= m_nModeLampBlueFlicker[1];

	m_nRunSpeed[0]					= m_nRunSpeed[1];
	m_nManualSpeed[0]				= m_nManualSpeed[1];

	m_dRejTrayPick[0]				= m_dRejTrayPick[1];
	m_dRejTrayPlace[0]				= m_dRejTrayPlace[1];

	m_strEqp[0]						= m_strEqp[1];

	for(i=0 ; i <= SELFCHECK ; i++)
	{
		m_nRlamp[0][i]				= m_nRlamp[1][i];
		m_nYlamp[0][i]				= m_nYlamp[1][i];
		m_nGlamp[0][i]				= m_nGlamp[1][i];
	}
	//2015.02.09
	m_nModeXgem[0]			= m_nModeXgem[1];
	m_nModeXgemRunChk[0]	= m_nModeXgemRunChk[1];
	m_nModeXgemInterface[0]	= m_nModeXgemInterface[1];
}


void CWorkMaintenance::OnDataInit()
{
	int i;
	
	m_nLampFlickerTime[1]			= st_lamp_info.nLampWaitTime;
	m_nModeBuzzer[1]				= st_lamp_info.nBuzzerMode;
	m_nModeLampBlue[1]				= st_lamp_info.nModeLampBlue;			
	m_nModeLampBlueFlicker[1]		= st_lamp_info.nModeLampBlueFlicker;

	m_nRunSpeed[1]					= st_handler_info.nRunSpeed;
	m_nManualSpeed[1]				= st_handler_info.nManualSpeed;

	m_dRejTrayPick[1]				= st_recipe_info.dRejTrayPick;
	m_dRejTrayPlace[1]				= st_recipe_info.dRejTrayPlace;

	m_strEqp[1]						= st_basic_info.strEqp;

	for(i=0 ; i <=SELFCHECK ; i++)
	{
		m_nRlamp[1][i]				= st_lamp_info.nLampR[i];
		m_nYlamp[1][i]				= st_lamp_info.nLampY[i];
		m_nGlamp[1][i]				= st_lamp_info.nLampG[i];
	}
	//2015.02.09
	m_nModeXgem[1]				= st_basic_info.nModeXgem;
	m_nModeXgemRunChk[1]		= st_basic_info.nModeXgemRunChk;
	m_nModeXgemInterface[1]		= st_basic_info.nModeXgemInterface;
	OnDataBackup();
}

void CWorkMaintenance::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.

	if(m_bLamp == FALSE)
	{
		m_bLamp = TRUE;
		CRect r;
		GetWindowRect(&r);
		
		m_stLampRun.GetWindowRect(&m_rectArea[RUN]);
		m_stLampIdle.GetWindowRect(&m_rectArea[IDLE]);
		m_stLampStop.GetWindowRect(&m_rectArea[STOP]);
		m_stLampAlarm.GetWindowRect(&m_rectArea[ALARM]);
		m_stLampWarning.GetWindowRect(&m_rectArea[WARNING]);
		m_stLampLock.GetWindowRect(&m_rectArea[LOCK]);		
		m_stLampLotend.GetWindowRect(&m_rectArea[LOTEND]);
		
		for(int i=0; i<=SELFCHECK; i++)
		{
			m_rectArea[i].left -= r.left;
			m_rectArea[i].right -= r.left;
			m_rectArea[i].top -= r.top;
			m_rectArea[i].bottom -= r.top;
		}
		
		m_stLampRun.ShowWindow(SW_HIDE);
		m_stLampIdle.ShowWindow(SW_HIDE);
		m_stLampStop.ShowWindow(SW_HIDE);
		m_stLampAlarm.ShowWindow(SW_HIDE);
		m_stLampLock.ShowWindow(SW_HIDE);
		m_stLampLotend.ShowWindow(SW_HIDE);
		//m_lamp_initial.ShowWindow(SW_HIDE);
		m_stLampWarning.ShowWindow(SW_HIDE);
		//m_lamp_selfcheck.ShowWindow(SW_HIDE);
		// ********************************************************************** //
		
		for(int j=0; j<=SELFCHECK; j++)
		{
			OnLampDrawButton(j);						// 타워 램프 버튼 생성 함수
		}
		
		for(int k=0; k<=SELFCHECK; k++)
		{
			OnLampRedStateChange(k);     // RED 타워 램프 버튼 상태 변환 함수
			OnLampYellowStateChange(k);  // YELLOW 타워 램프 버튼 상태 변환 함수
			OnLampGreenStateChange(k);   // GREEN 타워 램프 버튼 상태 변환 함수
		}
	}
}


void CWorkMaintenance::OnLampDrawButton(int i)
{
	// ================================================================================
    // 타워 램프 출력할 컨트롤에 대한 정보 얻는다
    // ================================================================================
	CRect r;
	r = m_rectArea[i];  // 컨트롤 영역 크기 설정 //

	//Warning하고, Self Check는 숨긴다.
	if (i == dSELFCHECK || i == dINIT)	return;

	m_nPosX[i][0]	= r.left;
	m_nPosX[i][1]	= r.left + ((int)r.Width()-4);
	m_nPosY[i][0]	= r.top;
	m_nPosY[i][1]	= r.bottom-4;
	m_nYdiff[i]		= (int) (m_nPosY[i][1] - m_nPosY[i][0])/7;
	m_nYspace[i]	= (int) m_nYdiff[i]/6;
	// ================================================================================

	// ================================================================================
    // 타워 램프 출력 이미지 관련 데이터 저장 변수 선언한다                       //
    // ================================================================================
	CRgn b1, b2, e1, e2;
	COLORREF cr;
	int x1=0, x2, y1=0, y2, w = m_nPosX[i][1] - m_nPosX[i][0];
	CPoint p;
	// ================================================================================

	// ================================================================================
    // 타워 램프 막대 끝의 둥근 고정 브라켓 영역 계산하여 컨트롤에 출력한다       //
	// -> CreateEllipticRgn() : 주어진 사각형 영역으로 타원 영역을 생성한다       //
	//  : 성공적으로 영역이 생성되었으면 영역의 핸들 값이 리턴                    //
	//  : 영역이 생성되지 않았으면 NULL이 리턴                                    //
    // ================================================================================
	p.x = m_nPosX[i][0];
	p.y = m_nPosY[i][1] - m_nYspace[i] * 4;
	x2	= w;	
	y2	= m_nYspace[i] * 4;
	e1.CreateEllipticRgn(x1, y1, x2, y2);

	m_btnBase[i].Create(_T(""), WS_CHILD|WS_VISIBLE, p, e1, this, IDC_LAMP_BASE+i, 1, RGB(150, 159, 183), RGB(150, 159, 183), RGB(150, 159, 183), RGB(150, 159, 183));
	m_btnBase[i].EnableWindow(FALSE);

	e1.DeleteObject();
	// ================================================================================

	// ================================================================================
    // 타워 램프의 하단 막대 이미지 출력 위치 계산하여 컨트롤에 출력한다          //
	// -> CreateRectRgn() : 주어진 좌표로 사각 영역을 그린다                      //
	// -> OffsetRgn() : 지정된 사각형의 좌표를 이동한다                           //
	// -> CombineRgn() : 존재하는 두 개의 여역들을 합쳐서 세 번째 영역을 만든다   //
	//  : 세 번째 영역의 핸들은 이 함수가 쓰이기 전에 반드시 만들어져야만         //
	//    새로운 영역으로 대체된다                                                //
	// -> GetRgnBox() : 주어진 영역을 완전히 둘러싸는 가장 작은 직사각형 그린다   //
    // ================================================================================
	x2 = (int) w / 3;
	y2 = (int) m_nYspace[i] * 4 / 3;
	e1.CreateEllipticRgn(x1, y1, x2, y2);

	y2 = m_nYdiff[i] * 2 - m_nYspace[i] * 2;
	b1.CreateRectRgn(x1, y1, x2, y2);

	x2 = w;
	y2 = m_nYspace[i] * 4;
	e2.CreateEllipticRgn(x1, y1, x2, y2);

	e1.OffsetRgn(0, m_nYdiff[i]*2 - m_nYspace[i]*2 - (int)m_nYspace[i]*2/3);
	e2.OffsetRgn(-(int)w/3, -m_nYspace[i]);
	b1.CombineRgn(&e1, &b1, RGN_OR);
	b1.CombineRgn(&b1, &e2, RGN_DIFF);

	CRect rr;
	b1.GetRgnBox(&rr);

	p.y = rr.top;
	p.x = m_nPosX[i][0] + (int) w/3;
	p.y += (m_nPosY[i][1] - m_nYdiff[i] * 2);
	m_btnPole[i].Create(_T(""), WS_CHILD|WS_VISIBLE, p, b1, this, IDC_LAMP_POLE+i, 1, RGB(203, 185, 156), RGB(203, 185, 156), RGB(203, 185, 156), RGB(203, 185, 156));
	m_btnPole[i].EnableWindow(FALSE);

	e1.DeleteObject();
	e2.DeleteObject();
	b1.DeleteObject();
	// ================================================================================

	// ================================================================================
    // GREEN 램프 밑에 존재하는 플라스틱 영역 계산하여 컨트롤에 출력한다          //
    // ================================================================================
	x2 = w;
	y2 = m_nYdiff[i];
	b1.CreateRectRgn(x1, y1, x2, y2);

	y2 = m_nYspace[i] * 4;
	e1.CreateEllipticRgn(x1, y1, x2, y2);

	y2 = m_nYspace[i] * 4;
	e2.CreateEllipticRgn(x1, y1, x2, y2);

	e1.OffsetRgn(0, -m_nYspace[i]*2);
	b1.CombineRgn(&b1, &e1, RGN_DIFF);
	e2.OffsetRgn(0, m_nYdiff[i] - m_nYspace[i]*2);
	b1.CombineRgn(&b1, &e2, RGN_OR);

	p.x = m_nPosX[i][0];
	p.y = m_nPosY[i][1] - m_nYdiff[i] * 3 + m_nYspace[i];
	m_btnLower[i].Create(_T(""), WS_CHILD|WS_VISIBLE, p, b1, this, IDC_LAMP_LOWER+i, 1, RGB(217, 217, 217), RGB(217, 217, 217), RGB(217, 217, 217), RGB(217, 217, 217));
	m_btnLower[i].EnableWindow(FALSE);

	e1.OffsetRgn(0, m_nYspace[i]*2);
	e2.OffsetRgn(0, -(m_nYdiff[i] - m_nYspace[i]*2));

	b1.DeleteObject();
	// ================================================================================

	// ================================================================================
    // GREEN 램프 바로 밑에 존재하는 테두리 영역 계산하여 컨트롤에 출력한다       //
    // ================================================================================
	y2 = m_nYspace[i];
	b1.CreateRectRgn(x1, y1, x2, y2);

	e1.OffsetRgn(0, -m_nYspace[i]);
	b1.CombineRgn(&b1, &e1, RGN_OR);
	e2.OffsetRgn(0, -m_nYspace[i]*2);
	b1.CombineRgn(&b1, &e2, RGN_DIFF);

	p.y -= m_nYspace[i];
	m_btnStep1[i].Create(_T(""), WS_CHILD|WS_VISIBLE, p, b1, this, IDC_LAMP_SEP1+i, 1, RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0));
	m_btnStep1[i].EnableWindow(FALSE);
	e1.OffsetRgn(0, m_nYspace[i]);
	// ================================================================================

	// ================================================================================
    // 타워 GREEN 램프 출력 위치 계산하여 컨트롤에 출력한다                       //
    // ================================================================================
	y2 = m_nYdiff[i] - m_nYspace[i];
	b2.CreateRectRgn(x1, y1, x2, y2);

	e1.OffsetRgn(0, m_nYdiff[i] - m_nYspace[i]*3);
	b2.CombineRgn(&b2, &e1, RGN_OR);
	b2.CombineRgn(&b2, &e2, RGN_DIFF);

	CString title;  // 램프 상태 저장 변수 //

	p.y -= (m_nYdiff[i] - m_nYspace[i]);
	if (m_nGlamp[1][i]==0) 
	{
		cr = G_OFF;
		title = _T("OFF");
	}
	else if (m_nGlamp[1][i]==1)
	{
		cr = G_ON;
		title = _T("ON");
	}
	else if (m_nGlamp[1][i]==2)
	{
		cr = G_OFF;
		title = _T("Flicker");
	}

	m_btnGlamp[i].Create(title, WS_CHILD|WS_VISIBLE, p, b2, this, ID_LAMP_GLAMP+i, 1, cr, G_ON, cr, cr);
	e1.OffsetRgn(0, -(m_nYdiff[i] - m_nYspace[i]*3));
	// ================================================================================

	// ================================================================================
    // GREEN 램프와 YELLOW 램프 사이의 테두리 영역 계산하여 컨트롤에 출력한다     //
    // ================================================================================
	p.y -= m_nYspace[i];

	m_btnStep2[i].Create(_T(""), WS_CHILD|WS_VISIBLE, p, b1, this, IDC_LAMP_SEP2+i, 1, RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0));
	m_btnStep2[i].EnableWindow(FALSE);
	// ================================================================================

	// ================================================================================
    // 타워 YELLOW 램프 출력 위치 계산하여 컨트롤에 출력한다                      //
    // ================================================================================
	p.y -= (m_nYdiff[i] - m_nYspace[i]);
	if (m_nYlamp[1][i]==0) 
	{
		cr = Y_OFF;
		title = _T("OFF");
	}
	else if (m_nYlamp[1][i]==1)
	{
		cr = Y_ON;
		title = _T("ON");
	}
	else if (m_nYlamp[1][i]==2)
	{
		cr = Y_OFF;
		title = _T("Flicker");
	}

	m_btnYlamp[i].Create(title, WS_CHILD|WS_VISIBLE, p, b2, this, ID_LAMP_YLAMP+i, 1, cr, Y_ON, cr, cr);
	// ================================================================================

	// ================================================================================
    // YELLOW 램프와 RED 램프 사이의 테두리 영역 계산하여 컨트롤에 출력한다       //
    // ================================================================================
	p.y -= m_nYspace[i];

	 m_btnStep3[i].Create(_T(""), WS_CHILD|WS_VISIBLE, p, b1, this, IDC_LAMP_SEP3+i, 1, RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0));
	 m_btnStep3[i].EnableWindow(FALSE);
	// ================================================================================

	// ================================================================================
    // 타워 RED 램프 출력 위치 계산하여 컨트롤에 출력한다                         //
    // ================================================================================
	p.y -= (m_nYdiff[i] - m_nYspace[i]);
	if (m_nRlamp[1][i]==0) 
	{
		cr = R_OFF;
		title = _T("OFF");
	}
	else if (m_nRlamp[1][i]==1)
	{
		cr = R_ON;
		title = _T("ON");
	}
	else if (m_nRlamp[1][i]==2)
	{
		cr = R_OFF;
		title = _T("Flicker");
	}

	m_btnRlamp[i].Create(title, WS_CHILD|WS_VISIBLE, p, b2, this, ID_LAMP_RLAMP+i, 1, cr, R_ON, cr, cr);
	// ================================================================================

	// ================================================================================
    // RED 램프 위에 존재하는 플라스틱 영역 계산하여 컨트롤에 출력한다            //
    // ================================================================================
	p.y -= (m_nYdiff[i]- m_nYspace[i]);

	m_btnUpper[i].Create(_T(""), WS_CHILD|WS_VISIBLE, p, b2, this, IDC_LAMP_UPPER+i, 1, RGB(217, 217, 217), RGB(217, 217, 217), RGB(217, 217, 217), RGB(217, 217, 217));
	m_btnUpper[i].EnableWindow(FALSE);
	// ================================================================================

	// ================================================================================
    // 타워 램프 맨 위 둥근 플라스틱 영역 계산하여 컨트롤에 출력한다              //
    // ================================================================================
	p.y -= m_nYspace[i]*2;

	m_btnRoof[i].Create(_T(""), WS_CHILD|WS_VISIBLE, p, e1, this, IDC_LAMP_ROOF+i, 1, RGB(217, 217, 217), RGB(217, 217, 217), RGB(217, 217, 217), RGB(217, 217, 217));
	m_btnRoof[i].EnableWindow(FALSE);
	// ================================================================================
}

void CWorkMaintenance::OnLampRedStateChange(int nDx)
{
	//Warning하고, Self Check는 숨긴다.
	if (nDx == dSELFCHECK || nDx == dINIT)	return;
	
	int nRed = m_nRlamp[1][nDx];  // RED 상태 설정 //
	
	switch(nRed)
	{
		case 0 :  // OFF //
			m_btnRlamp[nDx].SetWindowText(_T("OFF"));
			m_btnRlamp[nDx].ChangeColor(R_OFF, R_ON, R_OFF, R_OFF);
			break;
		case 1 :  // ON //
			m_btnRlamp[nDx].SetWindowText(_T("ON"));
			m_btnRlamp[nDx].ChangeColor(R_ON, R_ON, R_ON, R_ON);
			break;
		case 2 :  // FLICKER //
			m_btnRlamp[nDx].SetWindowText(_T("FLICK"));
			m_btnRlamp[nDx].ChangeColor(FLICK, R_ON, FLICK, R_ON);
			break;
		default :
			break;
	}
}

void CWorkMaintenance::OnLampGreenStateChange(int nDx)
{
	if (nDx == dSELFCHECK || nDx == dINIT)	return;
	
	int nGreen = m_nGlamp[1][nDx];  // GREEN 상태 설정 //
	
	switch(nGreen)
	{
		case 0 :  // OFF //
			m_btnGlamp[nDx].SetWindowText(_T("OFF"));
			m_btnGlamp[nDx].ChangeColor(G_OFF, G_ON, G_OFF, G_OFF);
			break;
		case 1 :  // ON //
			m_btnGlamp[nDx].SetWindowText(_T("ON"));
			m_btnGlamp[nDx].ChangeColor(G_ON, G_ON, G_ON, G_ON);
			break;
		case 2 :  // FLICKER //
			m_btnGlamp[nDx].SetWindowText(_T("FLICK"));
			m_btnGlamp[nDx].ChangeColor(FLICK, G_ON, FLICK, G_OFF);
			break;
		default :
			break;
	}
}

void CWorkMaintenance::OnLampYellowStateChange(int nDx)
{
	//Warning하고, Self Check는 숨긴다.
	if (nDx == dSELFCHECK || nDx == dINIT)	return;
	
	int nYellow = m_nYlamp[1][nDx];  // YELLOW 상태 설정 //
	
	switch(nYellow)
	{
		case 0 :  // OFF //
			m_btnYlamp[nDx].SetWindowText(_T("OFF"));
			m_btnYlamp[nDx].ChangeColor(Y_OFF, Y_ON, Y_OFF, Y_OFF);
			break;
		case 1 :  // ON //
			m_btnYlamp[nDx].SetWindowText(_T("ON"));
			m_btnYlamp[nDx].ChangeColor(Y_ON, Y_ON, Y_ON, Y_ON);
			break;
		case 2 :  // FLICKER //
			m_btnYlamp[nDx].SetWindowText(_T("FLICK"));
			m_btnYlamp[nDx].ChangeColor(FLICK, Y_ON, FLICK, Y_OFF);
			break;
		default :
			break;
	}
}

void CWorkMaintenance::OnRLampClick(UINT nID)
{
	int nDx = nID - ID_LAMP_RLAMP;		// 배열 위치 설정
	
	m_nRlamp[1][nDx]++;
	if (m_nRlamp[1][nDx]<0 || m_nRlamp[1][nDx]>2)
		m_nRlamp[1][nDx] = 0;
	
	OnLampRedStateChange(nDx);		// RED 타워 램프 버튼 상태 변환 함수
}

void CWorkMaintenance::OnYLampClick(UINT nID)
{
	int nDx = nID - ID_LAMP_YLAMP;		// 배열 위치 설정
	
	m_nYlamp[1][nDx]++;
	if (m_nYlamp[1][nDx]<0 || m_nYlamp[1][nDx]>2)
		m_nYlamp[1][nDx] = 0;
	
	OnLampYellowStateChange(nDx);	// YELLOW 타워 램프 버튼 상태 변환 함수
}

void CWorkMaintenance::OnGLampClick(UINT nID)
{
	int nDx = nID - ID_LAMP_GLAMP;		// 배열 위치 설정
	
	m_nGlamp[1][nDx]++;
	if (m_nGlamp[1][nDx]<0 || m_nGlamp[1][nDx]>2)
		m_nGlamp[1][nDx] = 0;
	
	OnLampGreenStateChange(nDx);		// GREEN 타워 램프 버튼 상태 변환 함수
}

BOOL CWorkMaintenance::OnEraseBkgnd(CDC* pDC)
{
	CDC  memDC;                     // 메모리 DC
	CBitmap * pOldBitmap = NULL;    // m_BackBitmap으로 새로운 그림을 DC에 그릴 때, 이전 그려졌던 DC(즉, Bitmap)을 저장.
	BITMAP bitmapInfo;              // 그림의 정보(m_BackBitmap)
	st_handler_info.m_BackDlgBitmap.GetBitmap(&bitmapInfo); // Bitmap 크기 구함.

	memDC.CreateCompatibleDC(pDC);  // 메모리 DC 생성
	pOldBitmap = memDC.SelectObject(&st_handler_info.m_BackDlgBitmap);
	pDC->BitBlt(0, 0, bitmapInfo.bmWidth, bitmapInfo.bmHeight, &memDC, 0, 0, SRCCOPY);   // 메모리 DC에 그림을 그림

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	return true; // 흰 부러쉬가 아닌 배경을 그려야 하기 때문에 true
}


void CWorkMaintenance::OnBnClickedBtnApply()
{
	CDialog_Message  dlgMsg;
	int nResponse;

	if (OnDataComparison() == RET_ERROR)
	{
		dlgMsg.m_nMessageType	= 1;
		dlgMsg.m_strMessage		= _T("Do you want to change the existing value to the current value?");

		nResponse =  (int)dlgMsg.DoModal();

		if (nResponse == IDOK)
		{
			if (OnDataComparison() == RET_ERROR)
			{
				OnDataHistoryLog();
				OnDataApply();
				OnDataBackup();
			}
		}
	}
}

void CWorkMaintenance::OnBnClickedBtnSmPasswordChangeSLock()
{
//	clsFunc.OnDatabaseOpen();
	/*int nResponse;  // 대화 상자 리턴 플래그
	CDialog_Password_Change  dlgPass;

//	pass_cng_dlg.m_nPassLevel = 0;

	st_other_info.strPassLevel=_T("SysLock");  // 암호 레벨 : 메인트 설정
	nResponse = (int)dlgPass.DoModal();

	if(nResponse == IDOK)
	{
		if(st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
		{
			st_other_info.strNormalMsg = _T("Changed : SLock Password!");
			st_handler_info.cWndList->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}
	}*/
}

void CWorkMaintenance::OnBnClickedBtnSmPasswordChangeLevle1()
{
/*	tagPCB_INFO st_pcb_info = {0};

	st_pcb_info.nBinBuf[0] = 1;
	st_pcb_info.nBinBuf[1] = 1;
	st_pcb_info.nBinBuf[2] = 1;
	st_pcb_info.nBinBuf[3] = 1;
	st_pcb_info.nBinBuf[4] = 1;
	st_pcb_info.strLotNo = _T("1111");
	st_pcb_info.strSerialNo = _T("2231");
	st_pcb_info.strBarcode = _T("1112");
	st_pcb_info.strPartNo = _T("2111");
	st_pcb_info.strArrNo = _T("2231");
	st_pcb_info.strPPID = _T("2434");
	st_pcb_info.strPSID = _T("9999");
	st_pcb_info.strWWN = _T("2321");
	st_pcb_info.strCSerialNo = _T("2211");

	clsFunc.OnDatabasePcbAdd(st_pcb_info);*/
	/*int nResponse;  // 대화 상자 리턴 플래그
	CDialog_Password_Change  dlgPass;

//	pass_cng_dlg.m_nPassLevel = 0;

	st_other_info.strPassLevel=_T("Level1");  // 암호 레벨 : 메인트 설정
	nResponse = (int)dlgPass.DoModal();
	
	if(nResponse == IDOK)
	{
		if(st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
		{
			st_other_info.strNormalMsg = _T("Changed : Level1 Password!");
			st_handler_info.cWndList->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}
	}*/
}


void CWorkMaintenance::OnBnClickedBtnSmPasswordChangeLevel2()
{
	/*int nResponse;  // 대화 상자 리턴 플래그
	CDialog_Password_Change  dlgPass;


	st_other_info.strPassLevel = _T("Level2");  // 암호 레벨 : 메인트 설정
	nResponse = (int)dlgPass.DoModal();

	if(nResponse == IDOK)
	{
		if(st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
		{
			st_other_info.strNormalMsg = _T("Changed : Level2 Password!");
			st_handler_info.cWndList->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}
	}*/
}


void CWorkMaintenance::OnClickedBtnBuzzer()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nModeBuzzer[1] == YES)
	{
		m_nModeBuzzer[1] = NO;
		m_btnBuzzer.SetBitmaps(IDC_BTN_SM_PASSWORD_CHANGE_LEVEL2, IDB_BITMAP_SOUND_OFF, WINDOW_DN, IDB_BITMAP_SOUND_OFF, WINDOW_UP);
		m_btnBuzzer.SetWindowTextW(_T("Buzzer Off"));
	}
	else
	{
		m_nModeBuzzer[1] = YES;
		m_btnBuzzer.SetBitmaps(IDC_BTN_SM_PASSWORD_CHANGE_LEVEL2, IDB_BITMAP_SOUND_ON, WINDOW_DN, IDB_BITMAP_SOUND_ON, WINDOW_UP);
		m_btnBuzzer.SetWindowTextW(_T("Buzzer On"));
	}
}


void CWorkMaintenance::OnStnClickedDgtSmLampFlickerTime()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nKey = m_nLampFlickerTime[1];

	KeyPadI(0, 10000, &nKey);

	m_nLampFlickerTime[1] = nKey;
	
	m_dgtFlickerTime.SetVal(nKey);
}


void CWorkMaintenance::OnClickedDgtRunSpeed()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nKey = m_nRunSpeed[1];

	KeyPadI(0, 100, &nKey);

	m_nRunSpeed[1] = nKey;
	
	m_dgtRunSpeed.SetVal(nKey);
}


void CWorkMaintenance::OnClickedDgtManualSpeed()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nKey = m_nManualSpeed[1];

	KeyPadI(0, 100, &nKey);

	m_nManualSpeed[1] = nKey;
	
	m_dgtManualSpeed.SetVal(nKey);
}


void CWorkMaintenance::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
/*
	if (OnDataComparison() == RET_ERROR)
	{
		OnDataHistoryLog();
		OnDataApply();
	}
*/
	CDialog::PostNcDestroy();
}


void CWorkMaintenance::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (OnDataComparison() == RET_ERROR)
	{
		OnDataHistoryLog();
		OnDataApply();
	}
}


void CWorkMaintenance::OnStnClickedMsgMaintenanceEqpData()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strKey;

	KeyBoard(&strKey);

	if (strKey == _T("")) return;

	m_strEqp[1] = strKey;

	m_msgEqpData.SetWindowText(m_strEqp[1]);
}

//2015.02.09
void CWorkMaintenance::OnInitGridXgem()
{
	int   i, j;
	int	  max_row, max_col;
	int   row;
	CString strTmp;

	max_row = 10; // unloader tray y count
	max_col = 2;

	m_pGridXgem.SetFrameFocusCell(FALSE);
	m_pGridXgem.SetTrackFocusCell(FALSE);
	m_pGridXgem.EnableSelection(FALSE);

	m_pGridXgem.SetGridLineColor(BLACK_C);
	m_pGridXgem.SetGridLines(1);

	m_pGridXgem.SetRowCount(max_row);
	m_pGridXgem.SetColumnCount(max_col);

	m_pGridXgem.SetFixedRowCount(0);
	m_pGridXgem.SetFixedColumnCount(0);
	m_pGridXgem.SetFixedBkColor(RGB(0,0,200));
	m_pGridXgem.SetFixedBkColor(RGB(200,200,255));
	m_pGridXgem.SetTextBkColor(RGB(150,150,200));

	for (i=0; i<max_row; i++) 
	{
		m_pGridXgem.SetRowHeight(i, 31);

		for (j=0; j<max_col; j++) 
		{
			m_pGridXgem.SetColumnWidth(j, 88);

			m_pGridXgem.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_pGridXgem.SetItemState(i, j, GVIS_READONLY);
			m_pGridXgem.SetItemBkColour(i, j, WHITE_C, CLR_DEFAULT);
		}
	}

	row = 0;
	m_pGridXgem.MergeCells(row, 0, row, 1);
	m_pGridXgem.SetItemBkColour(row, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridXgem.SetItemFont(row, 0, &clsFunc.OnLogFont(17));
	m_pGridXgem.SetItemText(row, 0, _T("Xgem Used"));

	row = 1;
	m_pGridXgem.MergeCells(row, 0, row, 1);
	m_pGridXgem.SetItemBkColour(row, 0, YELLOW_D, BLACK_C);
	m_pGridXgem.SetItemFont(row, 0, &clsFunc.OnLogFont(17));
	m_pGridXgem.SetItemText(row, 0, _T("Xgem Yes"));

	row = 2;
	m_pGridXgem.MergeCells(row, 0, row, 1);
	m_pGridXgem.SetItemBkColour(row, 0, YELLOW_D, BLACK_C);
	m_pGridXgem.SetItemFont(row, 0, &clsFunc.OnLogFont(17));
	m_pGridXgem.SetItemText(row, 0, _T("Xgem No"));

	if (m_nModeXgem[1] == YES)
	{
		m_pGridXgem.SetItemBkColour(1, 0, YELLOW_C, BLACK_C);
	}
	else
	{
		m_pGridXgem.SetItemBkColour(2, 0, YELLOW_C, BLACK_C);
	}

	row = 3;
	m_pGridXgem.MergeCells(row, 0, row, 1);
	m_pGridXgem.SetItemBkColour(row, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridXgem.SetItemFont(row, 0, &clsFunc.OnLogFont(17));
	m_pGridXgem.SetItemText(row, 0, _T("Xgem Recipe Check"));

	row = 4;
	m_pGridXgem.MergeCells(row, 0, row, 1);
	m_pGridXgem.SetItemBkColour(row, 0, YELLOW_D, BLACK_C);
	m_pGridXgem.SetItemFont(row, 0, &clsFunc.OnLogFont(17));
	m_pGridXgem.SetItemText(row, 0, _T("Recipe Yes"));

	row = 5;
	m_pGridXgem.MergeCells(row, 0, row, 1);
	m_pGridXgem.SetItemBkColour(row, 0, YELLOW_D, BLACK_C);
	m_pGridXgem.SetItemFont(row, 0, &clsFunc.OnLogFont(17));
	m_pGridXgem.SetItemText(row, 0, _T("Recipe No"));

	if (m_nModeXgemRunChk[1] == YES)
	{
		m_pGridXgem.SetItemBkColour(4, 0, YELLOW_C, BLACK_C);
	}
	else
	{
		m_pGridXgem.SetItemBkColour(5, 0, YELLOW_C, BLACK_C);
	}

	row = 6;
	m_pGridXgem.MergeCells(row, 0, row, 1);
	m_pGridXgem.SetItemBkColour(row, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridXgem.SetItemFont(row, 0, &clsFunc.OnLogFont(17));
	m_pGridXgem.SetItemText(row, 0, _T("Xgem Interface"));

	row = 7;
	m_pGridXgem.MergeCells(row, 0, row, 1);
	m_pGridXgem.SetItemBkColour(row, 0, GREEN_D, BLACK_C);
	m_pGridXgem.SetItemFont(row, 0, &clsFunc.OnLogFont(17));
	m_pGridXgem.SetItemText(row, 0, _T("Off Line"));

	row = 8;
	m_pGridXgem.MergeCells(row, 0, row, 1);
	m_pGridXgem.SetItemBkColour(row, 0, GREEN_D, BLACK_C);
	m_pGridXgem.SetItemFont(row, 0, &clsFunc.OnLogFont(17));
	m_pGridXgem.SetItemText(row, 0, _T("On Line (Local)"));

	row = 9;
	m_pGridXgem.MergeCells(row, 0, row, 1);
	m_pGridXgem.SetItemBkColour(row, 0, GREEN_D, BLACK_C);
	m_pGridXgem.SetItemFont(row, 0, &clsFunc.OnLogFont(17));
	m_pGridXgem.SetItemText(row, 0, _T("On Line (Remote)"));

	if (m_nModeXgemInterface[1] == MES_OFF_LINE)
	{
		m_pGridXgem.SetItemBkColour(7, 0, GREEN_C, BLACK_C);
	}
	else if (m_nModeXgemInterface[1] == MES_ON_LINE_LOCAL)
	{
		m_pGridXgem.SetItemBkColour(8, 0, GREEN_C, BLACK_C);
	}
	else
	{
		m_pGridXgem.SetItemBkColour(9, 0, GREEN_C, BLACK_C);
	}
}
void CWorkMaintenance::OnXgemCellClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	int nRow, nCol;

	CString strTmp;
	CString strMsg;

	NM_GRIDVIEW* pItem	= (NM_GRIDVIEW*) pNotifyStruct;

	nRow				= pItem->iRow;
	nCol				= pItem->iColumn;

	// Header Click
	switch (nRow)
	{
	case 1:
		if (m_nModeXgem[1] == YES) return;
		m_nModeXgem[1] = YES;

		m_pGridXgem.SetItemBkColour(1, 0, YELLOW_C, BLACK_C);
		m_pGridXgem.SetItemBkColour(2, 0, YELLOW_D, BLACK_C);
		break;

	case 2:
		if (m_nModeXgem[1] == NO) return;
		m_nModeXgem[1] = NO;

		m_pGridXgem.SetItemBkColour(1, 0, YELLOW_D, BLACK_C);
		m_pGridXgem.SetItemBkColour(2, 0, YELLOW_C, BLACK_C);
		break;

	case 4:
		if (m_nModeXgemRunChk[1] == YES) return;
		m_nModeXgemRunChk[1] = YES;

		m_pGridXgem.SetItemBkColour(4, 0, YELLOW_C, BLACK_C);
		m_pGridXgem.SetItemBkColour(5, 0, YELLOW_D, BLACK_C);
		break;

	case 5:
		if (m_nModeXgemRunChk[1] == NO) return;
		m_nModeXgemRunChk[1] = NO;

		m_pGridXgem.SetItemBkColour(4, 0, YELLOW_D, BLACK_C);
		m_pGridXgem.SetItemBkColour(5, 0, YELLOW_C, BLACK_C);
		break;

	case 7:
		if (m_nModeXgemInterface[1] == MES_OFF_LINE) return;
		m_nModeXgemInterface[1] = MES_OFF_LINE;

		m_pGridXgem.SetItemBkColour(7, 0, GREEN_C, BLACK_C);
		m_pGridXgem.SetItemBkColour(8, 0, GREEN_D, BLACK_C);
		m_pGridXgem.SetItemBkColour(9, 0, GREEN_D, BLACK_C);

		clsXgem.OnMcInterface(MES_OFF_LINE);
		break;

	case 8:
		if (m_nModeXgemInterface[1] == MES_ON_LINE_LOCAL) return;
		m_nModeXgemInterface[1] = MES_ON_LINE_LOCAL;

		m_pGridXgem.SetItemBkColour(7, 0, GREEN_D, BLACK_C);
		m_pGridXgem.SetItemBkColour(8, 0, GREEN_C, BLACK_C);
		m_pGridXgem.SetItemBkColour(9, 0, GREEN_D, BLACK_C);

		clsXgem.OnMcInterface(MES_ON_LINE_LOCAL);
		break;

	case 9:
		if (m_nModeXgemInterface[1] == MES_ON_LINE_REMOTE) return;
		m_nModeXgemInterface[1] = MES_ON_LINE_REMOTE;

		m_pGridXgem.SetItemBkColour(7, 0, GREEN_D, BLACK_C);
		m_pGridXgem.SetItemBkColour(8, 0, GREEN_D, BLACK_C);
		m_pGridXgem.SetItemBkColour(9, 0, GREEN_C, BLACK_C);

		clsXgem.OnMcInterface(MES_ON_LINE_REMOTE);
		break;
	}

	m_pGridXgem.Invalidate(FALSE);
}
void CWorkMaintenance::OnStnClickedDgtRejTrayPickOffset()
{
	double dKey = m_dRejTrayPick[1];

	KeyPadD(0.0f, 100.0f, &dKey);

	m_dRejTrayPick[1] = dKey;
	
	m_dgtRejTrayPick.SetFloatVal((float)dKey);
}


void CWorkMaintenance::OnStnClickedDgtRejTrayPlaceOffset()
{
	double dKey = m_dRejTrayPlace[1];

	KeyPadD(0.0f, 100.0f, &dKey);

	m_dRejTrayPlace[1] = dKey;
	
	m_dgtRejTrayPlace.SetFloatVal((float)dKey);
}


void CWorkMaintenance::OnBnClickedBtnMaintCylinder()
{
	// TODO: ¿©±â¿¡ ÄÁÆ®·Ñ ¾Ë¸² Ã³¸®±â ÄÚµå¸¦ Ãß°¡ÇÕ´Ï´Ù.
	CDialog_Pass_Check  pass_chk_dlg;

	int nResponse;

	pass_chk_dlg.m_nPassLevel = 7;
	nResponse = (int)pass_chk_dlg.DoModal();

	if (nResponse == IDOK)
	{
		// bcr cylinder off
// 		FAS_IO.set_out_bit(st_io_info.o_LdCvyWaitTrayAlignCyl, IO_OFF);
// 		
// 		FAS_IO.set_out_bit(st_io_info.o_LdTrayRail1AlignCyl, IO_OFF);
// 
// 		FAS_IO.set_out_bit(st_io_info.o_LdTrayRail2AlignCyl, IO_OFF);
// 
// 		FAS_IO.set_out_bit(st_io_info.o_LdTrayIndexTrayClampCyl, IO_OFF);
// 		/////////////////////////
// 		FAS_IO.set_out_bit(st_io_info.o_UnldTrayRail1Unld1AlignCyl, IO_OFF);
// 
// 		FAS_IO.set_out_bit(st_io_info.o_UnldTrayRail1Unld2AlignCyl, IO_OFF);
// 
// 		FAS_IO.set_out_bit(st_io_info.o_UnldTrayIndexTrayClampCyl, IO_OFF);
// 
// 		FAS_IO.set_out_bit(st_io_info.o_LfTsiteBtmSockIns1Cyl, IO_OFF);
// 		FAS_IO.set_out_bit(st_io_info.o_LfTsiteBtmSockIns2Cyl, IO_OFF);
// 		FAS_IO.set_out_bit(st_io_info.o_LfTsiteBtmSockIns3Cyl, IO_OFF);
// 		FAS_IO.set_out_bit(st_io_info.o_LfTsiteBtmSockIns4Cyl, IO_OFF);
// 		FAS_IO.set_out_bit(st_io_info.o_LfTsiteBtmSockIns5Cyl, IO_OFF);
// 		FAS_IO.set_out_bit(st_io_info.o_LfTsiteBtmSockIns6Cyl, IO_OFF);
// 
// 		FAS_IO.set_out_bit(st_io_info.o_LfTsiteTopSockIns1Cyl, IO_OFF);
// 		FAS_IO.set_out_bit(st_io_info.o_LfTsiteTopSockIns2Cyl, IO_OFF);
// 		FAS_IO.set_out_bit(st_io_info.o_LfTsiteTopSockIns3Cyl, IO_OFF);
// 		FAS_IO.set_out_bit(st_io_info.o_LfTsiteTopSockIns4Cyl, IO_OFF);
// 		FAS_IO.set_out_bit(st_io_info.o_LfTsiteTopSockIns5Cyl, IO_OFF);
// 		FAS_IO.set_out_bit(st_io_info.o_LfTsiteTopSockIns6Cyl, IO_OFF);
// 
// 		FAS_IO.set_out_bit(st_io_info.o_RiTsiteBtmSockIns1Cyl, IO_OFF);
// 		FAS_IO.set_out_bit(st_io_info.o_RiTsiteBtmSockIns2Cyl, IO_OFF);
// 		FAS_IO.set_out_bit(st_io_info.o_RiTsiteBtmSockIns3Cyl, IO_OFF);
// 		FAS_IO.set_out_bit(st_io_info.o_RiTsiteBtmSockIns4Cyl, IO_OFF);
// 		FAS_IO.set_out_bit(st_io_info.o_RiTsiteBtmSockIns5Cyl, IO_OFF);
// 		FAS_IO.set_out_bit(st_io_info.o_RiTsiteBtmSockIns6Cyl, IO_OFF);
// 
// 		FAS_IO.set_out_bit(st_io_info.o_RiTsiteTopSockIns1Cyl, IO_OFF);
// 		FAS_IO.set_out_bit(st_io_info.o_RiTsiteTopSockIns2Cyl, IO_OFF);
// 		FAS_IO.set_out_bit(st_io_info.o_RiTsiteTopSockIns3Cyl, IO_OFF);
// 		FAS_IO.set_out_bit(st_io_info.o_RiTsiteTopSockIns4Cyl, IO_OFF);
// 		FAS_IO.set_out_bit(st_io_info.o_RiTsiteTopSockIns5Cyl, IO_OFF);
// 		FAS_IO.set_out_bit(st_io_info.o_RiTsiteTopSockIns6Cyl, IO_OFF);
	}
}
