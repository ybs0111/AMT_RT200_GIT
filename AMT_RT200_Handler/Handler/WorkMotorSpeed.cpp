// WorkMotorSpeed.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Handler.h"
#include "WorkMotorSpeed.h"
#include "afxdialogex.h"
#include "Variable.h"
#include "Ctlbd_Variable.h"
#include "PublicFunction.h"
#include "Alg_Excel.h"
#include "Dialog_KeyPad.h"
#include "Ctlbd_Variable.h"
#include "ComizoaPublic.h"
#include "Dialog_Message.h"
#include "DialogXgem.h"
#include ".\\Ctrl\\KeyBoard.h"

// CWorkMotorSpeed 대화 상자입니다.

IMPLEMENT_DYNAMIC(CWorkMotorSpeed, CDialog)

CWorkMotorSpeed::CWorkMotorSpeed(CWnd* pParent /*=NULL*/)
	: CDialog(CWorkMotorSpeed::IDD, pParent)
{
	int nMotor;

	nMotor = 0;
	m_nAccLimit[nMotor]		= 300;
	m_nDecLimit[nMotor]		= 300;
	m_nVelLimit[nMotor]		= 600;

	nMotor = 1;
	m_nAccLimit[nMotor]		= 300;
	m_nDecLimit[nMotor]		= 300;
	m_nVelLimit[nMotor]		= 600;

	nMotor = 2;
	m_nAccLimit[nMotor]		= 300;
	m_nDecLimit[nMotor]		= 300;
	m_nVelLimit[nMotor]		= 400;

	nMotor = 3;
	m_nAccLimit[nMotor]		= 300;
	m_nDecLimit[nMotor]		= 300;
	m_nVelLimit[nMotor]		= 600;

	nMotor = 4;
	m_nAccLimit[nMotor]		= 300;
	m_nDecLimit[nMotor]		= 300;
	m_nVelLimit[nMotor]		= 600;

	nMotor = 5;
	m_nAccLimit[nMotor]		= 300;
	m_nDecLimit[nMotor]		= 300;
	m_nVelLimit[nMotor]		= 100;

	nMotor = 6;
	m_nAccLimit[nMotor]		= 300;
	m_nDecLimit[nMotor]		= 300;
	m_nVelLimit[nMotor]		= 500;

	nMotor = 7;
	m_nAccLimit[nMotor]		= 300;
	m_nDecLimit[nMotor]		= 300;
	m_nVelLimit[nMotor]		= 1000;

	nMotor = 8;
	m_nAccLimit[nMotor]		= 300;
	m_nDecLimit[nMotor]		= 300;
	m_nVelLimit[nMotor]		= 500;

	nMotor = 9;
	m_nAccLimit[nMotor]		= 300;
	m_nDecLimit[nMotor]		= 300;
	m_nVelLimit[nMotor]		= 1000;

	nMotor = 10;
	m_nAccLimit[nMotor]		= 300;
	m_nDecLimit[nMotor]		= 300;
	m_nVelLimit[nMotor]		= 200;

	nMotor = 11;
	m_nAccLimit[nMotor]		= 300;
	m_nDecLimit[nMotor]		= 300;
	m_nVelLimit[nMotor]		= 150;

	nMotor = 12;
	m_nAccLimit[nMotor]		= 300;
	m_nDecLimit[nMotor]		= 300;
	m_nVelLimit[nMotor]		= 1000;

	nMotor = 13;
	m_nAccLimit[nMotor]		= 300;
	m_nDecLimit[nMotor]		= 300;
	m_nVelLimit[nMotor]		= 1000;

	nMotor = 14;
	m_nAccLimit[nMotor]		= 300;
	m_nDecLimit[nMotor]		= 300;
	m_nVelLimit[nMotor]		= 600;

	nMotor = 15;
	m_nAccLimit[nMotor]		= 300;
	m_nDecLimit[nMotor]		= 300;
	m_nVelLimit[nMotor]		= 1000;

	nMotor = 16;
	m_nAccLimit[nMotor]		= 300;
	m_nDecLimit[nMotor]		= 300;
	m_nVelLimit[nMotor]		= 1000;

	nMotor = 17;
	m_nAccLimit[nMotor]		= 300;
	m_nDecLimit[nMotor]		= 300;
	m_nVelLimit[nMotor]		= 1000;

	nMotor = 18;
	m_nAccLimit[nMotor]		= 300;
	m_nDecLimit[nMotor]		= 300;
	m_nVelLimit[nMotor]		= 600;

	nMotor = 19;
	m_nAccLimit[nMotor]		= 300;
	m_nDecLimit[nMotor]		= 300;
	m_nVelLimit[nMotor]		= 1000;

	nMotor = 20;
	m_nAccLimit[nMotor]		= 500;
	m_nDecLimit[nMotor]		= 500;
	m_nVelLimit[nMotor]		= 1200;

	nMotor = 21;
	m_nAccLimit[nMotor]		= 500;
	m_nDecLimit[nMotor]		= 500;
	m_nVelLimit[nMotor]		= 1200;

	nMotor = 22;
	m_nAccLimit[nMotor]		= 300;
	m_nDecLimit[nMotor]		= 300;
	m_nVelLimit[nMotor]		= 600;

	nMotor = 23;
	m_nAccLimit[nMotor]		= 200;
	m_nDecLimit[nMotor]		= 200;
	m_nVelLimit[nMotor]		= 100;

	nMotor = 24;
	m_nAccLimit[nMotor]		= 200;
	m_nDecLimit[nMotor]		= 200;
	m_nVelLimit[nMotor]		= 100;

	nMotor = 25;
	m_nAccLimit[nMotor]		= 300;
	m_nDecLimit[nMotor]		= 300;
	m_nVelLimit[nMotor]		= 250;

	nMotor = 26;
	m_nAccLimit[nMotor]		= 300;
	m_nDecLimit[nMotor]		= 300;
	m_nVelLimit[nMotor]		= 200;

	nMotor = 27;
	m_nAccLimit[nMotor]		= 100;
	m_nDecLimit[nMotor]		= 100;
	m_nVelLimit[nMotor]		= 100;

	nMotor = 28;
	m_nAccLimit[nMotor]		= 100;
	m_nDecLimit[nMotor]		= 100;
	m_nVelLimit[nMotor]		= 100;
}

CWorkMotorSpeed::~CWorkMotorSpeed()
{
}

void CWorkMotorSpeed::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM_SPEED,	m_pGridSpeed);
	DDX_Control(pDX, IDC_GROUP_COMMAND, m_groupCommand);
	DDX_Control(pDX, IDC_BTN_APPLY,		m_btnApply);
	DDX_Control(pDX, IDC_BTN_RELOAD,	m_btnReLoad);
	DDX_Control(pDX, IDC_BTN_SAFETY,	m_btnSafety);
	DDX_Control(pDX, IDC_BTN_TESTSITE,  m_btnTsite);
}


BEGIN_MESSAGE_MAP(CWorkMotorSpeed, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_CUSTOM_SPEED, OnCellClick)
	ON_BN_CLICKED(IDC_BTN_APPLY, &CWorkMotorSpeed::OnClickedBtnApply)
	ON_BN_CLICKED(IDC_BTN_RELOAD, &CWorkMotorSpeed::OnClickedBtnReload)
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_SAFETY, &CWorkMotorSpeed::OnBnClickedBtnSafety)
	ON_BN_CLICKED(IDC_BTN_TESTSITE, &CWorkMotorSpeed::OnBnClickedBtnTestsite)
	ON_BN_CLICKED(IDC_BUTTON_XGEM, &CWorkMotorSpeed::OnBnClickedButtonXgem)
END_MESSAGE_MAP()


// CWorkMotorSpeed 메시지 처리기입니다.


BOOL CWorkMotorSpeed::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
/*	if (OnDataComparison() == RET_ERROR)
	{
		OnDataHistoryLog();
		OnDataApply();
	}*/
	return CDialog::DestroyWindow();
}


BOOL CWorkMotorSpeed::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	OnDataInit();

	OnInitGroupBox();
	OnInitLabel();
	OnInitButton();
	OnInitGridSpeed();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CWorkMotorSpeed::OnInitGroupBox()
{
	m_groupCommand.SetFont(clsFunc.OnLogFont(16));
	m_groupCommand.SetCatptionTextColor(BLUE_C);
	m_groupCommand.SetBorderColor(ORANGE_C);
	m_groupCommand.SetBackgroundColor(WINDOW_UP1);
	m_groupCommand.SetFontBold(TRUE);
}


void CWorkMotorSpeed::OnInitLabel()
{
}


void CWorkMotorSpeed::OnInitButton()
{
	m_btnApply.SetBitmaps(IDC_BTN_APPLY, IDB_BITMAP_APPLY_DN1, WINDOW_DN1, IDB_BITMAP_APPLY_UP1, WINDOW_UP1);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnApply.SetFont(clsFunc.m_pFont[2]);
	m_btnApply.SetTooltipText(_T("Motor Speed Data Apply"));

	m_btnReLoad.SetBitmaps(IDC_BTN_RELOAD, IDB_BITMAP_RELOAD_DN1, WINDOW_DN1, IDB_BITMAP_RELOAD_UP1, WINDOW_UP1);
	m_btnReLoad.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnReLoad.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnReLoad.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnReLoad.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnReLoad.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnReLoad.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnReLoad.SetFont(clsFunc.m_pFont[2]);
	m_btnReLoad.SetTooltipText(_T("Motor Speed Data ReLoad"));

	m_btnSafety.SetBitmaps(IDC_BTN_SAFETY, IDB_BITMAP_GEAR_DN, WINDOW_DN1, IDB_BITMAP_GEAR_UP, WINDOW_UP1);
	m_btnSafety.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnSafety.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnSafety.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnSafety.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnSafety.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnSafety.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnSafety.SetFont(clsFunc.m_pFont[2]);
	m_btnSafety.SetTooltipText(_T("Motor Safety"));

	m_btnTsite.SetBitmaps(IDC_BTN_TESTSITE, IDB_BITMAP_GEAR_DN, WINDOW_DN1, IDB_BITMAP_GEAR_UP, WINDOW_UP1);
	m_btnTsite.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnTsite.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnTsite.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnTsite.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnTsite.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnTsite.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnTsite.SetFont(clsFunc.m_pFont[2]);
	m_btnTsite.SetTooltipText(_T("Testsite Config"));
}


void CWorkMotorSpeed::OnInitGridSpeed()
{
	CString strHeader[9] = {_T("Axis Name"), _T("Accel(ms)"), _T("Decel(ms)"), _T("Homd(mm/s)"), _T("Jog(mm/s)"), _T("Vel(mm/s)"), _T("Allow(mm)"), _T("- Limit"), _T("+ Limit")};
	CString strTmp;
	int i, j;
	
	m_pGridSpeed.SetFrameFocusCell(FALSE);
	m_pGridSpeed.SetTrackFocusCell(FALSE);
	m_pGridSpeed.EnableSelection(FALSE);

	m_pGridSpeed.SetGridLineColor(BLACK_C);
	m_pGridSpeed.SetGridLines(1);

	m_pGridSpeed.SetRowCount(m_nMotorMaxCnt + 1);
	m_pGridSpeed.SetColumnCount(9);

	m_pGridSpeed.SetFixedRowCount(0);
	m_pGridSpeed.SetFixedColumnCount(0);
	m_pGridSpeed.SetFixedBkColor(RGB(0,0,200));
	m_pGridSpeed.SetFixedBkColor(RGB(200,200,255));
	m_pGridSpeed.SetTextBkColor(RGB(150,150,200));

	for (i=0; i<m_nMotorMaxCnt+1; i++) 
	{
		m_pGridSpeed.SetRowHeight(i, 41);

		for (j=0; j<9; j++) 
		{
			switch (j)
			{
				case 0:
					m_pGridSpeed.SetColumnWidth(j, 225);
					break;

				case 1:
					m_pGridSpeed.SetColumnWidth(j, 87);
					break;

				case 2:
					m_pGridSpeed.SetColumnWidth(j, 87);
					break;

				case 3:
					m_pGridSpeed.SetColumnWidth(j, 87);
					break;

				case 4:
					m_pGridSpeed.SetColumnWidth(j, 87);
					break;

				case 5:
					m_pGridSpeed.SetColumnWidth(j, 87);
					break;

				case 6:
					m_pGridSpeed.SetColumnWidth(j, 87);
					break;

				case 7:
					m_pGridSpeed.SetColumnWidth(j, 87);
					break;

				case 8:
					m_pGridSpeed.SetColumnWidth(j, 87);
					break;
			}

			m_pGridSpeed.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_pGridSpeed.SetItemState(i, j, GVIS_READONLY);
			m_pGridSpeed.SetItemBkColour(i, j, WHITE_C, CLR_DEFAULT);
		}
	}

	for (i=0; i<m_nMotorMaxCnt+1; i++) 
	{
		for (j=0; j<9; j++) 
		{
			if(i == 0)
			{
				m_pGridSpeed.SetItemBkColour(i, j, RGB(219, 229, 241), BLACK_L);
				m_pGridSpeed.SetItemFont(i, j, &clsFunc.OnLogFont(15));
				m_pGridSpeed.SetItemText(i, j,strHeader[j]);
			}
			else
			{
				switch(j)
				{
					case 0:
						m_pGridSpeed.SetItemBkColour(i, j, RGB(219, 229, 241), BLACK_L);
						m_pGridSpeed.SetItemFont(i, j, &clsFunc.OnLogFont(15));
						m_pGridSpeed.SetItemText(i, j, m_strMotorName[i - 1]);
						break;

					case 1:
						strTmp.Format(_T("%d"), (int)m_dAcc[i-1][1]);

						m_pGridSpeed.SetItemBkColour(i, j, RED_L, BLACK_C);
						m_pGridSpeed.SetItemFont(i, j, &clsFunc.OnLogFont(15));
						m_pGridSpeed.SetItemText(i, j, strTmp);
						break;

					case 2:
						strTmp.Format(_T("%d"), (int)m_dDec[i-1][1]);

						m_pGridSpeed.SetItemBkColour(i, j, GREEN_L, BLACK_C);
						m_pGridSpeed.SetItemFont(i, j, &clsFunc.OnLogFont(15));
						m_pGridSpeed.SetItemText(i, j, strTmp);
						break;

					case 3:
						strTmp.Format(_T("%d"), (int)m_dHome[i-1][1]);

						m_pGridSpeed.SetItemBkColour(i, j, YELLOW_L, BLACK_C);
						m_pGridSpeed.SetItemFont(i, j, &clsFunc.OnLogFont(15));
						m_pGridSpeed.SetItemText(i, j, strTmp);
						break;

					case 4:
						strTmp.Format(_T("%d"), (int)m_dJog[i-1][1]);

						m_pGridSpeed.SetItemBkColour(i, j, YELLOW_L, BLACK_C);
						m_pGridSpeed.SetItemFont(i, j, &clsFunc.OnLogFont(15));
						m_pGridSpeed.SetItemText(i, j, strTmp);
						break;

					case 5:
						strTmp.Format(_T("%d"), (int)m_dVel[i-1][1]);

						m_pGridSpeed.SetItemBkColour(i, j, YELLOW_L, BLACK_C);
						m_pGridSpeed.SetItemFont(i, j, &clsFunc.OnLogFont(15));
						m_pGridSpeed.SetItemText(i, j, strTmp);
						break;

					case 6:
						strTmp.Format(_T("%.2f"), m_dAllow[i-1][1]);

						m_pGridSpeed.SetItemBkColour(i, j, SKY_C, BLACK_C);
						m_pGridSpeed.SetItemFont(i, j, &clsFunc.OnLogFont(15));
						m_pGridSpeed.SetItemText(i, j, strTmp);
						break;

					case 7:
						strTmp.Format(_T("%.3f"), m_dLimitM[i-1][1]);

						m_pGridSpeed.SetItemBkColour(i, j, RED_L, BLACK_C);
						m_pGridSpeed.SetItemFont(i, j, &clsFunc.OnLogFont(15));
						m_pGridSpeed.SetItemText(i, j, strTmp);
						break;

					case 8:
						strTmp.Format(_T("%.3f"), m_dLimitP[i-1][1]);

						m_pGridSpeed.SetItemBkColour(i, j, GREEN_L, BLACK_C);
						m_pGridSpeed.SetItemFont(i, j, &clsFunc.OnLogFont(15));
						m_pGridSpeed.SetItemText(i, j, strTmp);
						break;
				}
			}
		}
	}
}


void CWorkMotorSpeed::OnDataHistoryLog()
{
	int i;
	CString strMsg;

	for(i = 0; i < m_nMotorMaxCnt; i++)
	{
		if(m_dAcc[i][0]		!= m_dAcc[i][1])
		{
			strMsg.Format(_T("[%s] Axis Accel Change %.3f -> %.3f"), m_strMotorName[i],
																	 m_dAcc[i][0], m_dAcc[i][1]);
			clsFunc.OnLogFileAdd(0, strMsg);
		}

		if(m_dDec[i][0]		!= m_dDec[i][1])
		{
			strMsg.Format(_T("[%s] Axis Decel Change %.3f -> %.3f"), m_strMotorName[i],
																	 m_dDec[i][0], m_dDec[i][1]);
			clsFunc.OnLogFileAdd(0, strMsg);
		}
		
		if(m_dMax[i][0]		!= m_dMax[i][1])
		{
			strMsg.Format(_T("[%s] Axis Max Change %.3f -> %.3f"),   m_strMotorName[i],
																	 m_dMax[i][0], m_dMax[i][1]);
			clsFunc.OnLogFileAdd(0, strMsg);
		}
		
		if(m_dVel[i][0]		!= m_dVel[i][1])
		{
			strMsg.Format(_T("[%s] Axis Vel Change %.3f -> %.3f"), m_strMotorName[i],
																	 m_dVel[i][0], m_dVel[i][1]);
			clsFunc.OnLogFileAdd(0, strMsg);
		}

		if(m_dJog[i][0]		!= m_dJog[i][1])
		{
			strMsg.Format(_T("[%s] Axis Jog Change %.3f -> %.3f"), m_strMotorName[i],
																	 m_dJog[i][0], m_dJog[i][1]);
			clsFunc.OnLogFileAdd(0, strMsg);
		}

		if(m_dHome[i][0]	!= m_dHome[i][1])
		{
			strMsg.Format(_T("[%s] Axis Home Change %.3f -> %.3f"), m_strMotorName[i],
																	m_dHome[i][0], m_dHome[i][1]);
			clsFunc.OnLogFileAdd(0, strMsg);
		}
		
		if(m_dAllow[i][0]	!= m_dAllow[i][1])
		{
			strMsg.Format(_T("[%s] Axis Accel Change %.2f -> %.2f"), m_strMotorName[i],
																 m_dAllow[i][0], m_dAllow[i][1]);
			clsFunc.OnLogFileAdd(0, strMsg);
		}

		if(m_dLimitM[i][0]	!= m_dLimitM[i][1])
		{
			strMsg.Format(_T("[%s] Axis Limit M Change %.3f -> %.3f"), m_strMotorName[i],
																	   m_dLimitM[i][0], m_dLimitM[i][1]);
			clsFunc.OnLogFileAdd(0, strMsg);
		}

		if(m_dLimitP[i][0]	!= m_dLimitP[i][1])
		{
			strMsg.Format(_T("[%s] Axis Limit P Change %.3f -> %.3f"), m_strMotorName[i],
																	   m_dLimitP[i][0], m_dLimitP[i][1]);
			clsFunc.OnLogFileAdd(0, strMsg);
		}
	}
}


void CWorkMotorSpeed::OnDataRecovery()
{
	int i;
	
	for(i = 0; i < m_nMotorMaxCnt; i++)
	{
		m_dAcc[i][1]		= m_dAcc[i][0];
		m_dDec[i][1]		= m_dDec[i][0];
		
		m_dMax[i][1]		= m_dMax[i][0];
		
		m_dVel[i][1]		= m_dVel[i][0];
		m_dJog[i][1]		= m_dJog[i][0];
		m_dHome[i][1]		= m_dHome[i][0];
		
		m_dAllow[i][1]		= m_dAllow[i][0];

		m_dLimitM[i][1]		= m_dLimitM[i][0];
		m_dLimitP[i][1]		= m_dLimitP[i][0];
	}
}


int CWorkMotorSpeed::OnDataComparison()
{
	int i;
	
	for(i = 0; i < m_nMotorMaxCnt; i++)
	{
		if(m_dAcc[i][0]		!= m_dAcc[i][1])			return RET_ERROR;
		if(m_dDec[i][0]		!= m_dDec[i][1])			return RET_ERROR;
		
		if(m_dMax[i][0]		!= m_dMax[i][1])			return RET_ERROR;
		
		if(m_dVel[i][0]		!= m_dVel[i][1])			return RET_ERROR;
		if(m_dJog[i][0]		!= m_dJog[i][1])			return RET_ERROR;
		if(m_dHome[i][0]	!= m_dHome[i][1])			return RET_ERROR;

		if(m_dAllow[i][0]	!= m_dAllow[i][1])			return RET_ERROR;

		if(m_dLimitM[i][0]	!= m_dLimitM[i][1])			return RET_ERROR;
		if(m_dLimitP[i][0]	!= m_dLimitP[i][1])			return RET_ERROR;
	}
	return RET_GOOD;
}


void CWorkMotorSpeed::OnDataApply()
{
	int i;
	
	for(i = 0; i < m_nMotorMaxCnt; i++)
	{
		st_motor_info[i].d_accel[0]				= m_dAcc[i][1];
		st_motor_info[i].d_decel[0]				= m_dDec[i][1];
		
		st_motor_info[i].d_spd_max[0]			= m_dMax[i][1];
		
		st_motor_info[i].d_spd_vel[0]			= m_dVel[i][1];
		st_motor_info[i].d_spd_jog[0]			= m_dJog[i][1];
		st_motor_info[i].d_spd_home[0]			= m_dHome[i][1];
		
		st_motor_info[i].d_allow				= m_dAllow[i][1];

		COMI.md_limit_position[i][0]			= st_motor_info[i].d_limit_position[0]	= m_dLimitM[i][1];		
		COMI.md_limit_position[i][1]			= st_motor_info[i].d_limit_position[1]	= m_dLimitP[i][1];
	
		COMI.md_spd_vel[i][0]					= st_motor_info[i].d_spd_vel[0];
		COMI.md_spd_vel[i][1]					= st_motor_info[i].d_spd_vel[1];
		COMI.md_spd_vel[i][2]					= st_motor_info[i].d_spd_vel[2];

		COMI.md_spd_home[i]						= st_motor_info[i].d_spd_home[0];
		COMI.md_spd_jog[i]						= st_motor_info[i].d_spd_jog[0];

		COMI.md_allow_value[i]					= st_motor_info[i].d_allow;
	}
}


void CWorkMotorSpeed::OnDataBackup()
{
	int i;
	
	for(i = 0; i < m_nMotorMaxCnt; i++)
	{
		m_dAcc[i][0]		= m_dAcc[i][1];
		m_dDec[i][0]		= m_dDec[i][1];
		
		m_dMax[i][0]		= m_dMax[i][1];
		
		m_dVel[i][0]		= m_dVel[i][1];
		m_dJog[i][0]		= m_dJog[i][1];
		m_dHome[i][0]		= m_dHome[i][1];
		
		m_dAllow[i][0]		= m_dAllow[i][1];

		m_dLimitM[i][0]		= m_dLimitM[i][1];
		m_dLimitP[i][0]		= m_dLimitP[i][1];
	}
}


void CWorkMotorSpeed::OnDataInit()
{
	int i;
	
	m_nMotorMaxCnt			= clsExcel.m_nMotorMaxCnt;

	for(i = 0; i < m_nMotorMaxCnt; i++)
	{
		m_strMotorName[i]	= clsExcel.m_strAxisName[i];
		m_dAcc[i][1]		= st_motor_info[i].d_accel[0];
		m_dDec[i][1]		= st_motor_info[i].d_decel[0];
		
		m_dMax[i][1]		= st_motor_info[i].d_spd_max[0];
		
		m_dVel[i][1]		= st_motor_info[i].d_spd_vel[0];
		m_dJog[i][1]		= st_motor_info[i].d_spd_jog[0];
		m_dHome[i][1]		= st_motor_info[i].d_spd_home[0];
		
		m_dAllow[i][1]		= st_motor_info[i].d_allow;

		m_dLimitM[i][1]		= st_motor_info[i].d_limit_position[0];
		m_dLimitP[i][1]		= st_motor_info[i].d_limit_position[1];
	}
	
	OnDataBackup();
}

void CWorkMotorSpeed::OnCellClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	int nRow, nCol;
	int i;

	double dKey;

	CString strTmp;
	CString strMsg;

	NM_GRIDVIEW* pItem	= (NM_GRIDVIEW*) pNotifyStruct;

	nRow				= pItem->iRow;
	nCol				= pItem->iColumn;

	// Header Click
	if (nCol < 1) return;

	if(nRow == 0)
	{
		return;

		switch(nCol)
		{
			case 1:
				dKey = m_dAcc[0][1];

				KeyPadD(0.0f, 1000.0f, &dKey);

				for (i=0; i<m_nMotorMaxCnt; i++)
				{
					m_dAcc[i][1] = dKey;

					strTmp.Format(_T("%.3f"), dKey);
					m_pGridSpeed.SetItemText(i + 1, nCol, strTmp);
				}
				break;
		
			case 2:
				dKey = m_dDec[0][1];

				KeyPadD(0.0f, 1000.0f, &dKey);

				for (i=0; i<m_nMotorMaxCnt; i++)
				{
					m_dDec[i][1] = dKey;

					strTmp.Format(_T("%.3f"), dKey);
					m_pGridSpeed.SetItemText(i + 1, nCol, strTmp);
				}
				break;

			case 3:
				dKey = m_dHome[0][1];

				KeyPadD(0.0f, 100.0f, &dKey);

				for (i=0; i<m_nMotorMaxCnt; i++)
				{
					m_dHome[i][1] = dKey;

					strTmp.Format(_T("%.3f"), dKey);
					m_pGridSpeed.SetItemText(i + 1, nCol, strTmp);
				}
				break;

			case 4:
				dKey = m_dJog[0][1];

				KeyPadD(0.0f, 100.0f, &dKey);

				for (i=0; i<m_nMotorMaxCnt; i++)
				{
					m_dJog[i][1] = dKey;

					strTmp.Format(_T("%.3f"), dKey);
					m_pGridSpeed.SetItemText(i + 1, nCol, strTmp);
				}
				break;

			case 5:
				dKey = m_dVel[0][1];

				KeyPadD(0.0f, 2000.0f, &dKey);

				for (i=0; i<m_nMotorMaxCnt; i++)
				{
					m_dVel[i][1] = dKey;

					strTmp.Format(_T("%.3f"), dKey);
					m_pGridSpeed.SetItemText(i + 1, nCol, strTmp);
				}
				break;

			case 6:
				dKey = m_dAllow[0][1];

				KeyPadD(-3.0f, 10.0f, &dKey);

				for (i=0; i<m_nMotorMaxCnt; i++)
				{
					m_dAllow[i][1] = dKey;

					strTmp.Format(_T("%.3f"), dKey);
					m_pGridSpeed.SetItemText(i + 1, nCol, strTmp);
				}
				break;

			case 7:
				dKey = m_dLimitM[0][1];

				KeyPadD(-300.0f, 5000.0f, &dKey);

				for (i=0; i<m_nMotorMaxCnt; i++)
				{
					m_dLimitM[i][1] = dKey;

					strTmp.Format(_T("%.3f"), dKey);
					m_pGridSpeed.SetItemText(i + 1, nCol, strTmp);
				}
				break;

			case 8:
				dKey = m_dLimitP[0][1];

				KeyPadD(1.0f, 5000.0f, &dKey);

				for (i=0; i<m_nMotorMaxCnt; i++)
				{
					m_dLimitP[i][1] = dKey;

					strTmp.Format(_T("%.3f"), dKey);
					m_pGridSpeed.SetItemText(i + 1, nCol, strTmp);
				}
				break;
		}
	}
	else
	{
		switch(nCol)
		{
			case 1:
				dKey = m_dAcc[nRow - 1][1];

				KeyPadD(1.0f, 1000.0f, &dKey);

				m_dAcc[nRow - 1][1] = dKey;

				strTmp.Format(_T("%.3f"), dKey);
				m_pGridSpeed.SetItemText(nRow, nCol, strTmp);
				break;
		
			case 2:
				dKey = m_dDec[nRow - 1][1];

				KeyPadD(1.0f, 1000.0f, &dKey);

				m_dDec[nRow - 1][1] = dKey;

				strTmp.Format(_T("%.3f"), dKey);
				m_pGridSpeed.SetItemText(nRow, nCol, strTmp);
				break;

			case 3:
				dKey = m_dHome[nRow - 1][1];

				KeyPadD(1.0f, 100.0f, &dKey);

				m_dHome[nRow - 1][1] = dKey;

				strTmp.Format(_T("%.3f"), dKey);
				m_pGridSpeed.SetItemText(nRow, nCol, strTmp);
				break;

			case 4:
				dKey = m_dJog[nRow - 1][1];

				KeyPadD(1.0f, 100.0f, &dKey);

				m_dJog[nRow - 1][1] = dKey;

				strTmp.Format(_T("%.3f"), dKey);
				m_pGridSpeed.SetItemText(nRow, nCol, strTmp);
				break;

			case 5:
				dKey = m_dVel[nRow - 1][1];

				KeyPadD(1.0f, 2000.0f, &dKey);

				m_dVel[nRow - 1][1] = dKey;

				strTmp.Format(_T("%.3f"), dKey);
				m_pGridSpeed.SetItemText(nRow, nCol, strTmp);
				break;

			case 6:
				dKey = m_dAllow[nRow - 1][1];

				KeyPadD(0.0f, 20.0f, &dKey);

				m_dAllow[nRow - 1][1] = dKey;

				strTmp.Format(_T("%.3f"), dKey);
				m_pGridSpeed.SetItemText(nRow, nCol, strTmp);
				break;

			case 7:
				dKey = m_dLimitM[nRow - 1][1];

				KeyPadD(-5000.0f, 20.0f, &dKey);

				m_dLimitM[nRow - 1][1] = dKey;

				strTmp.Format(_T("%.3f"), dKey);
				m_pGridSpeed.SetItemText(nRow, nCol, strTmp);
				break;

			case 8:
				dKey = m_dLimitP[nRow - 1][1];

				KeyPadD(1.0f, 5000.0f, &dKey);

				m_dLimitP[nRow - 1][1] = dKey;

				strTmp.Format(_T("%.3f"), dKey);
				m_pGridSpeed.SetItemText(nRow, nCol, strTmp);
				break;
		}
	}

	m_pGridSpeed.Invalidate(FALSE);
}

void CWorkMotorSpeed::OnClickedBtnApply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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


void CWorkMotorSpeed::OnClickedBtnReload()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnDataRecovery();
	if(OnDataComparison() == RET_ERROR)
	{
		OnInitGridSpeed();
	}
}


BOOL CWorkMotorSpeed::OnEraseBkgnd(CDC* pDC)
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


void CWorkMotorSpeed::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
/*	if (OnDataComparison() == RET_ERROR)
	{
		OnDataHistoryLog();
		OnDataApply();
	}*/
	CDialog::PostNcDestroy();
}


void CWorkMotorSpeed::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (OnDataComparison() == RET_ERROR)
	{
		OnDataHistoryLog();
		OnDataApply();
	}
}


void CWorkMotorSpeed::OnBnClickedBtnSafety()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CWorkMotorSpeed::OnBnClickedBtnTestsite()
{
}


void CWorkMotorSpeed::OnBnClickedButtonXgem()
{
	// TODO: ¿©±â¿¡ ÄÁÆ®·Ñ ¾Ë¸² Ã³¸®±â ÄÚµå¸¦ Ãß°¡ÇÕ´Ï´Ù.
	CDialogXgem dlgXgem;

	dlgXgem.DoModal();
}
