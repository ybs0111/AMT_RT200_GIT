// WorkRecipe.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Handler.h"
#include "WorkRecipe.h"
#include "afxdialogex.h"
#include "Dialog_KeyBoard.h"
#include "Dialog_KeyPad.h"
#include "Dialog_Message.h"
#include "PublicFunction.h"
#include "CtlBd_Variable.h"
#include "Dialog_Message.h"
#include "XgemClient.h"
#include ".\\Ctrl\\KeyBoard.h"

// CWorkRecipe 대화 상자입니다.
#define TM_INTERFACE	100

IMPLEMENT_DYNAMIC(CWorkRecipe, CDialog)

CWorkRecipe::CWorkRecipe(CWnd* pParent /*=NULL*/)
	: CDialog(CWorkRecipe::IDD, pParent)
{
	
}

CWorkRecipe::~CWorkRecipe()
{
}

void CWorkRecipe::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_APPLY,							m_btnApply);
	DDX_Control(pDX, IDC_BTN_RELOAD,						m_btnReLoad);
	DDX_Control(pDX, IDC_MSG_TRAY_ROW_CNT,					m_msgTrayRowCnt);
	DDX_Control(pDX, IDC_MSG_TRAY_COL_CNT,					m_msgTrayColCnt);
	DDX_Control(pDX, IDC_MSG_REJECT_TRAY_X_CNT,				m_msgRejectTrayXCnt);
	DDX_Control(pDX, IDC_MSG_RETEST_TRAY_X_CNT,				m_msgRetestTrayXCnt);
	DDX_Control(pDX, IDC_MSG_TRAY_ROW_OFFSET,				m_msgTrayRowOffset);
	DDX_Control(pDX, IDC_MSG_TRAY_COL_OFFSET,				m_msgTrayColOffset);
	DDX_Control(pDX, IDC_MSG_BUFFER_ROW_OFFSET,				m_msgBufRowOffset);
	DDX_Control(pDX, IDC_MSG_BUFFER_COL_OFFSET,				m_msgBufColOffset);
	DDX_Control(pDX, IDC_MSG_TESTSITE_OFFSET,				m_msgTSiteOffset);
	DDX_Control(pDX, IDC_MSG_COK_X_OFFSET,					m_msgCokXOffset);
	DDX_Control(pDX, IDC_MSG_RETEST_COUNT,					m_msgRetCnt);
	DDX_Control(pDX, IDC_MSG_FAIL_COUNT,					m_msgFailCnt);
	DDX_Control(pDX, IDC_MSG_REJECT_FULL_OFFSET,			m_msgRejectFullOffset);
	DDX_Control(pDX, IDC_MSG_REJECT_EMPTY_OFFSET,			m_msgRejectEmptyOffset);
	DDX_Control(pDX, IDC_MSG_SOCKET_COUNT,					m_msgSocketCount);
	DDX_Control(pDX, IDC_MSG_15T_OFFSET,					m_msg15TOffset);
	DDX_Control(pDX, IDC_DGT_TRAY_ROW_CNT,					m_dgtTrayRowCnt);
	DDX_Control(pDX, IDC_DGT_TRAY_COL_CNT,					m_dgtTrayColCnt);
	DDX_Control(pDX, IDC_DGT_REJECT_TRAY_X_CNT,				m_dgtRejectTrayXCnt);
	DDX_Control(pDX, IDC_DGT_RETEST_TRAY_X_CNT,				m_dgtRetestTrayXCnt);
	DDX_Control(pDX, IDC_DGT_TRAY_ROW_OFFSET,				m_dgtTrayRowOffset);
	DDX_Control(pDX, IDC_DGT_TRAY_COL_OFFSET,				m_dgtTrayColOffset);
	DDX_Control(pDX, IDC_DGT_BUFFER_ROW_OFFSET,				m_dgtBufRowOffset);
	DDX_Control(pDX, IDC_DGT_BUFFER_COL_OFFSET,				m_dgtBufColOffset);
	DDX_Control(pDX, IDC_DGT_TESTSITE_OFFSET,				m_dgtTSiteOffset);
	DDX_Control(pDX, IDC_DGT_COK_X_OFFSET,					m_dgtCokXOffset);
	DDX_Control(pDX, IDC_DGT_RETEST_COUNT,					m_dgtRetCnt);
	DDX_Control(pDX, IDC_DGT_FAIL_COUNT,					m_dgtFailCnt);
	DDX_Control(pDX, IDC_DGT_REJECT_FULL_OFFSET,			m_dgtRejectFullOffset);
	DDX_Control(pDX, IDC_DGT_REJECT_EMPTY_OFFSET,			m_dgtRejectEmptyOffset);
	DDX_Control(pDX, IDC_DGT_SOCKET_COUNT,					m_dgtSocketCount);
	DDX_Control(pDX, IDC_DGT_15T_OFFSET,					m_dgt15TOffset);
	DDX_Control(pDX, IDC_GROUP_RECIPE_TRAY,					m_groupTrayInfo);
	DDX_Control(pDX, IDC_GROUP_RECIPE_ROTATOR,				m_groupRotator);
	//	DDX_Control(pDX, IDC_GROUP_RECIPE_SOCKET,				m_groupSocket);
	DDX_Control(pDX, IDC_CUSTOM_RECIPE_LEFT_INTERFACE,		m_pGridLfInterface);
	DDX_Control(pDX, IDC_CUSTOM_RECIPE_RIGHT_INTERFACE,		m_pGridRiInterface);
	DDX_Control(pDX, IDC_CUSTOM_RECIPE_DEGREE,				m_pGridDegree);

// 	DDX_Control(pDX, IDC_MSG_PCB_ROW_OFFSET, m_msgPcbRowOffset);
// 	DDX_Control(pDX, IDC_DGT_PCB_ROW_OFFSET, m_dgtPcbRowOffset);
// 	DDX_Control(pDX, IDC_MSG_PCB_COL_OFFSET, m_msgPcbColOffset);
// 	DDX_Control(pDX, IDC_DGT_PCB_COL_OFFSET, m_dgtPcbColOffset);
// 	DDX_Control(pDX, IDC_MSG_PICKER_ROW_OFFSET, m_msgPickerRowOffset);
// 	DDX_Control(pDX, IDC_DGT_PICKER_ROW_OFFSET, m_dgtPickerRowOffset);
// 	DDX_Control(pDX, IDC_MSG_PICKER_COL_OFFSET, m_msgPickerColOffset);
// 	DDX_Control(pDX, IDC_DGT_PICKER_COL_OFFSET, m_dgtPickerColOffset);
// 	DDX_Control(pDX, IDC_MSG_LABEL_ROW_OFFSET, m_msgLabelRowOffset);
// 	DDX_Control(pDX, IDC_MSG_LABEL_COL_OFFSET, m_msgLabelColOffset);
// 	DDX_Control(pDX, IDC_DGT_LABEL_ROW_OFFSET, m_dgtLabelRowOffset);
// 	DDX_Control(pDX, IDC_DGT_LABEL_COL_OFFSET, m_dgtLabelColOffset);
}


BEGIN_MESSAGE_MAP(CWorkRecipe, CDialog)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_APPLY,							&CWorkRecipe::OnBnClickedBtnApply)
	ON_BN_CLICKED(IDC_BTN_RELOAD,							&CWorkRecipe::OnBnClickedBtnReload)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_STN_CLICKED(IDC_DGT_TRAY_ROW_CNT,					&CWorkRecipe::OnClickedDgtTrayRowCnt)
	ON_STN_CLICKED(IDC_DGT_TRAY_COL_CNT,					&CWorkRecipe::OnClickedDgtTrayColCnt)
	ON_STN_CLICKED(IDC_DGT_TRAY_ROW_OFFSET,					&CWorkRecipe::OnClickedDgtTrayRowOffset)
	ON_STN_CLICKED(IDC_DGT_TRAY_COL_OFFSET,					&CWorkRecipe::OnClickedDgtTrayColOffset)
	ON_STN_CLICKED(IDC_DGT_TESTSITE_OFFSET,					&CWorkRecipe::OnClickedDgtTestsiteOffset)
	ON_NOTIFY(NM_CLICK, IDC_CUSTOM_RECIPE_LEFT_INTERFACE,	OnLeftInterfaceCellClick)
	ON_NOTIFY(NM_CLICK, IDC_CUSTOM_RECIPE_RIGHT_INTERFACE,	OnRightInterfaceCellClick)
	ON_NOTIFY(NM_CLICK, IDC_CUSTOM_RECIPE_DEGREE,			OnDegreeCellClick)
	ON_STN_CLICKED(IDC_DGT_RETEST_COUNT,					&CWorkRecipe::OnStnClickedDgtRetestCount)
	ON_STN_CLICKED(IDC_DGT_FAIL_COUNT,						&CWorkRecipe::OnStnClickedDgtFailCount)
	ON_STN_CLICKED(IDC_DGT_BUFFER_ROW_OFFSET,				&CWorkRecipe::OnClickedDgtBufferRowOffset)
	ON_STN_CLICKED(IDC_DGT_BUFFER_COL_OFFSET,				&CWorkRecipe::OnClickedDgtBufferColOffset)
	ON_STN_CLICKED(IDC_DGT_REJECT_TRAY_X_CNT,				&CWorkRecipe::OnStnClickedDgtRejectTrayXCnt)
	ON_STN_CLICKED(IDC_DGT_RETEST_TRAY_X_CNT,				&CWorkRecipe::OnStnClickedDgtRetestTrayXCnt)
	ON_STN_CLICKED(IDC_DGT_REJECT_FULL_OFFSET, &CWorkRecipe::OnStnClickedDgtRejectFullOffset)
	ON_STN_CLICKED(IDC_DGT_REJECT_EMPTY_OFFSET, &CWorkRecipe::OnStnClickedDgtRejectEmptyOffset)
//	ON_STN_CLICKED(IDC_DGT_COK_X_OFFSET, &CWorkRecipe::OnStnClickedDgtCokXOffset)
	ON_STN_CLICKED(IDC_DGT_COK_X_OFFSET, &CWorkRecipe::OnStnClickedDgtCokXOffset)
	ON_STN_CLICKED(IDC_DGT_SOCKET_COUNT, &CWorkRecipe::OnStnClickedDgtSocketCount)
	ON_STN_CLICKED(IDC_DGT_15T_OFFSET, &CWorkRecipe::OnStnClickedDgt15tOffset)
	ON_STN_CLICKED(IDC_DGT_PCB_ROW_OFFSET, &CWorkRecipe::OnStnClickedDgtPcbRowOffset)
	ON_STN_CLICKED(IDC_DGT_PCB_COL_OFFSET, &CWorkRecipe::OnStnClickedDgtPcbColOffset)
	ON_STN_CLICKED(IDC_DGT_PICKER_ROW_OFFSET, &CWorkRecipe::OnStnClickedDgtPickerRowOffset)
	ON_STN_CLICKED(IDC_DGT_PICKER_COL_OFFSET, &CWorkRecipe::OnStnClickedDgtPickerColOffset)
	ON_STN_CLICKED(IDC_DGT_LABEL_ROW_OFFSET, &CWorkRecipe::OnStnClickedDgtLabelRowOffset)
	ON_STN_CLICKED(IDC_DGT_LABEL_COL_OFFSET, &CWorkRecipe::OnStnClickedDgtLabelColOffset)
END_MESSAGE_MAP()


// CWorkRecipe 메시지 처리기입니다.


BOOL CWorkRecipe::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
/*	if (OnDataComparison() == RET_ERROR)
	{
		OnDataHistoryLog();
		OnDataApply();
	}*/
	return CDialog::DestroyWindow();
}


BOOL CWorkRecipe::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	OnInitGroupBox();
	OnInitLabel();
	OnInitButton();
	OnInitDigit();
	OnInitGridLeftInterface();
	OnInitGridRightInterface();
	OnInitGridDegree();

	OnDataDisplay();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CWorkRecipe::OnInitGroupBox()
{
	m_groupTrayInfo.SetFont(clsFunc.OnLogFont(16));
	m_groupTrayInfo.SetCatptionTextColor(BLUE_C);
	m_groupTrayInfo.SetBorderColor(ORANGE_C);
	m_groupTrayInfo.SetBackgroundColor(WINDOW_UP1);
	m_groupTrayInfo.SetFontBold(TRUE);

	m_groupRotator.SetFont(clsFunc.OnLogFont(16));
	m_groupRotator.SetCatptionTextColor(BLUE_C);
	m_groupRotator.SetBorderColor(ORANGE_C);
	m_groupRotator.SetBackgroundColor(WINDOW_UP1);
	m_groupRotator.SetFontBold(TRUE);
/*
	m_groupSocket.SetFont(clsFunc.OnLogFont(16));
	m_groupSocket.SetCatptionTextColor(BLUE_C);
	m_groupSocket.SetBorderColor(ORANGE_C);
	m_groupSocket.SetBackgroundColor(WINDOW_UP1);
	m_groupSocket.SetFontBold(TRUE);
*/
}

void CWorkRecipe::OnInitLabel()
{
	m_msgTrayRowCnt.SetFont(clsFunc.m_pFont[2]);
	m_msgTrayRowCnt.SetWindowText(_T("Tray Y Count"));
	m_msgTrayRowCnt.SetCenterText();
	m_msgTrayRowCnt.SetColor(WHITE_C);
	m_msgTrayRowCnt.SetGradientColor(GREEN_C);
	m_msgTrayRowCnt.SetTextColor(BLACK_C);

	m_msgTrayColCnt.SetFont(clsFunc.m_pFont[2]);
	m_msgTrayColCnt.SetWindowText(_T("Tray X Count"));
	m_msgTrayColCnt.SetCenterText();
	m_msgTrayColCnt.SetColor(WHITE_C);
	m_msgTrayColCnt.SetGradientColor(RED_C);
	m_msgTrayColCnt.SetTextColor(BLACK_C);

	m_msgRejectTrayXCnt.SetFont(clsFunc.m_pFont[2]);
	m_msgRejectTrayXCnt.SetWindowText(_T("Reject Tray X Count"));
	m_msgRejectTrayXCnt.SetCenterText();
	m_msgRejectTrayXCnt.SetColor(WHITE_C);
	m_msgRejectTrayXCnt.SetGradientColor(GREEN_C);
	m_msgRejectTrayXCnt.SetTextColor(BLACK_C);

	m_msgRetestTrayXCnt.SetFont(clsFunc.m_pFont[2]);
	m_msgRetestTrayXCnt.SetWindowText(_T("Retest Tray X Count"));
	m_msgRetestTrayXCnt.SetCenterText();
	m_msgRetestTrayXCnt.SetColor(WHITE_C);
	m_msgRetestTrayXCnt.SetGradientColor(RED_C);
	m_msgRetestTrayXCnt.SetTextColor(BLACK_C);

	m_msgTrayRowOffset.SetFont(clsFunc.m_pFont[2]);
	m_msgTrayRowOffset.SetWindowText(_T("Tray Y Offset"));
	m_msgTrayRowOffset.SetCenterText();
	m_msgTrayRowOffset.SetColor(WHITE_C);
	m_msgTrayRowOffset.SetGradientColor(GREEN_C);
	m_msgTrayRowOffset.SetTextColor(BLACK_C);

	m_msgTrayColOffset.SetFont(clsFunc.m_pFont[2]);
	m_msgTrayColOffset.SetWindowText(_T("Tray X Offset"));
	m_msgTrayColOffset.SetCenterText();
	m_msgTrayColOffset.SetColor(WHITE_C);
	m_msgTrayColOffset.SetGradientColor(RED_C);
	m_msgTrayColOffset.SetTextColor(BLACK_C);

	m_msgBufRowOffset.SetFont(clsFunc.m_pFont[2]);
	m_msgBufRowOffset.SetWindowText(_T("Buffer Y Offset"));
	m_msgBufRowOffset.SetCenterText();
	m_msgBufRowOffset.SetColor(WHITE_C);
	m_msgBufRowOffset.SetGradientColor(GREEN_C);
	m_msgBufRowOffset.SetTextColor(BLACK_C);

	m_msgBufColOffset.SetFont(clsFunc.m_pFont[2]);
	m_msgBufColOffset.SetWindowText(_T("Buffer X Offset"));
	m_msgBufColOffset.SetCenterText();
	m_msgBufColOffset.SetColor(WHITE_C);
	m_msgBufColOffset.SetGradientColor(RED_C);
	m_msgBufColOffset.SetTextColor(BLACK_C);

	m_msgTSiteOffset.SetFont(clsFunc.m_pFont[2]);
	m_msgTSiteOffset.SetWindowText(_T("Testsite Offset"));
	m_msgTSiteOffset.SetCenterText();
	m_msgTSiteOffset.SetColor(WHITE_C);
	m_msgTSiteOffset.SetGradientColor(BLUE_C);
	m_msgTSiteOffset.SetTextColor(BLACK_C);

	m_msgCokXOffset.SetFont(clsFunc.m_pFont[2]);
	m_msgCokXOffset.SetWindowText(_T("Cok X Offset"));
	m_msgCokXOffset.SetCenterText();
	m_msgCokXOffset.SetColor(WHITE_C);
	m_msgCokXOffset.SetGradientColor(BLUE_C);
	m_msgCokXOffset.SetTextColor(BLACK_C);

	m_msgRetCnt.SetFont(clsFunc.m_pFont[2]);
	m_msgRetCnt.SetWindowText(_T("Retest Count"));
	m_msgRetCnt.SetCenterText();
	m_msgRetCnt.SetColor(WHITE_C);
	m_msgRetCnt.SetGradientColor(BLUE_C);
	m_msgRetCnt.SetTextColor(BLACK_C);

	m_msgFailCnt.SetFont(clsFunc.m_pFont[2]);
	m_msgFailCnt.SetWindowText(_T("Fail Count"));
	m_msgFailCnt.SetCenterText();
	m_msgFailCnt.SetColor(WHITE_C);
	m_msgFailCnt.SetGradientColor(BLUE_C);
	m_msgFailCnt.SetTextColor(BLACK_C);

	m_msgRejectFullOffset.SetFont(clsFunc.m_pFont[2]);
	m_msgRejectFullOffset.SetWindowText(_T("Reject Full Offset"));
	m_msgRejectFullOffset.SetCenterText();
	m_msgRejectFullOffset.SetColor(WHITE_C);
	m_msgRejectFullOffset.SetGradientColor(GREEN_C);
	m_msgRejectFullOffset.SetTextColor(BLACK_C);

	m_msgRejectEmptyOffset.SetFont(clsFunc.m_pFont[2]);
	m_msgRejectEmptyOffset.SetWindowText(_T("Reject Empty Offset"));
	m_msgRejectEmptyOffset.SetCenterText();
	m_msgRejectEmptyOffset.SetColor(WHITE_C);
	m_msgRejectEmptyOffset.SetGradientColor(RED_C);
	m_msgRejectEmptyOffset.SetTextColor(BLACK_C);

	m_msgSocketCount.SetFont(clsFunc.m_pFont[2]);
	m_msgSocketCount.SetWindowText(_T("Socket Count"));
	m_msgSocketCount.SetCenterText();
	m_msgSocketCount.SetColor(WHITE_C);
	m_msgSocketCount.SetGradientColor(GREEN_C);
	m_msgSocketCount.SetTextColor(BLACK_C);

	m_msg15TOffset.SetFont(clsFunc.m_pFont[2]);
	m_msg15TOffset.SetWindowText(_T("15T Tray Offset"));
	m_msg15TOffset.SetCenterText();
	m_msg15TOffset.SetColor(WHITE_C);
	m_msg15TOffset.SetGradientColor(GREEN_C);
	m_msg15TOffset.SetTextColor(BLACK_C);
}

void CWorkRecipe::OnInitDigit()
{
	m_dgtTrayRowCnt.SetStyle(CDigit::DS_INT, 1, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	m_dgtTrayRowCnt.SetVal(m_nTrayYCnt[1]);
	
	m_dgtTrayColCnt.SetStyle(CDigit::DS_INT, 1, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	m_dgtTrayColCnt.SetVal(m_nTrayXCnt[1]);

	m_dgtRejectTrayXCnt.SetStyle(CDigit::DS_INT, 1, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	m_dgtRejectTrayXCnt.SetVal(m_nRejectTrayXCnt[1]);
	
	m_dgtRetestTrayXCnt.SetStyle(CDigit::DS_INT, 1, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	m_dgtRetestTrayXCnt.SetVal(m_nRetestTrayXCnt[1]);
	
	m_dgtRetCnt.SetStyle(CDigit::DS_INT, 5, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	m_dgtRetCnt.SetVal(m_nRetCnt[1]);

	m_dgtFailCnt.SetStyle(CDigit::DS_INT, 5, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	m_dgtFailCnt.SetVal(m_nFailCnt[1]);

	m_dgtTrayRowOffset.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	m_dgtTrayRowOffset.SetFloatVal((float)m_dTrayYOffset[1]);
	
	m_dgtTrayColOffset.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	m_dgtTrayColOffset.SetFloatVal((float)m_dTrayXOffset[1]);
	
	m_dgtTSiteOffset.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	m_dgtTSiteOffset.SetFloatVal((float)m_dTSiteOffset[1]);

	m_dgtCokXOffset.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	m_dgtCokXOffset.SetFloatVal((float)m_dCokXOffset[1]);

	m_dgtBufRowOffset.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	m_dgtBufRowOffset.SetFloatVal((float)m_dBufYOffset[1]);
	
	m_dgtBufColOffset.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	m_dgtBufColOffset.SetFloatVal((float)m_dBufXOffset[1]);

	m_dgtRejectFullOffset.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	m_dgtRejectFullOffset.SetFloatVal((float)m_dRejectFullOffset[1]);
	
	m_dgtRejectEmptyOffset.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	m_dgtRejectEmptyOffset.SetFloatVal((float)m_dRejectEmptyOffset[1]);

	m_dgtSocketCount.SetStyle(CDigit::DS_INT, 6, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	m_dgtSocketCount.SetVal(m_nSocketCount[1]);

	m_dgt15TOffset.SetStyle(CDigit::DS_FLOAT3, 7, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	m_dgt15TOffset.SetFloatVal((float)m_d15TOffset[1]);
}


void CWorkRecipe::OnInitButton()
{
	m_btnApply.SetBitmaps(IDC_BTN_APPLY, IDB_BITMAP_APPLY_DN1, WINDOW_DN1, IDB_BITMAP_APPLY_UP1, WINDOW_UP1);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnApply.SetFont(clsFunc.m_pFont[2]);
	m_btnApply.SetTooltipText(_T("Recipe Data Apply"));

	m_btnReLoad.SetBitmaps(IDC_BTN_RELOAD, IDB_BITMAP_RELOAD_DN1, WINDOW_DN1, IDB_BITMAP_RELOAD_UP1, WINDOW_UP1);
	m_btnReLoad.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnReLoad.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnReLoad.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnReLoad.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnReLoad.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnReLoad.SetFont(clsFunc.m_pFont[2]);
	m_btnReLoad.SetTooltipText(_T("Recipe Data ReLoad"));
}


void CWorkRecipe::OnDataHistoryLog()
{
	int i, j, k;
	CString strMsg;

	if (m_nTrayYCnt[0]	!=	m_nTrayYCnt[1])
	{
		strMsg.Format(_T("Tray Row Count Change %d -> %d"), m_nTrayYCnt[0], m_nTrayYCnt[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}
	if (m_nTrayXCnt[0]	!=	m_nTrayXCnt[1])		
	{
		strMsg.Format(_T("Tray Col Count Change %d -> %d"), m_nTrayXCnt[0], m_nTrayXCnt[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}
	if (m_nRetCnt[0]	!=	m_nRetCnt[1])		
	{
		strMsg.Format(_T("Retest Count Change %d -> %d"), m_nRetCnt[0], m_nRetCnt[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}
	if (m_nFailCnt[0]	!=	m_nFailCnt[1])		
	{
		strMsg.Format(_T("Fail Count Change %d -> %d"), m_nFailCnt[0], m_nFailCnt[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}
	if (m_dTrayYOffset[0]	!=	m_dTrayYOffset[1])	
	{
		strMsg.Format(_T("Tray Row Offset Change %.3f -> %.3f"), m_dTrayYOffset[0], m_dTrayYOffset[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}
	if (m_dTrayXOffset[0]	!=	m_dTrayXOffset[1])	
	{
		strMsg.Format(_T("Tray Col Offset Change %.3f -> %.3f"), m_dTrayXOffset[0], m_dTrayXOffset[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}
	if (m_dTSiteOffset[0]	!=	m_dTSiteOffset[1])		
	{
		strMsg.Format(_T("Testsite Offset Change %.3f -> %.3f"), m_dTSiteOffset[0], m_dTSiteOffset[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}

	if (m_dCokXOffset[0]	!=	m_dCokXOffset[1])	//2015.0406 추가 	
	{
		strMsg.Format(_T("Cok Buffer Offset Change %.3f -> %.3f"), m_dCokXOffset[0], m_dCokXOffset[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}
	
	if (m_dBufYOffset[0]	!=	m_dBufYOffset[1])		
	{
		strMsg.Format(_T("Buffer Y Offset Change %.3f -> %.3f"), m_dBufYOffset[0], m_dBufYOffset[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}

	if (m_dBufXOffset[0]	!=	m_dBufXOffset[1])		
	{
		strMsg.Format(_T("Buffer X Offset Change %.3f -> %.3f"), m_dBufXOffset[0], m_dBufXOffset[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}

	if (m_nRejectTrayXCnt[0]	!=	m_nRejectTrayXCnt[1])		
	{
		strMsg.Format(_T("Reject Tray X Count Change %d -> %d"), m_nRejectTrayXCnt[0], m_nRejectTrayXCnt[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}

	if (m_nRetestTrayXCnt[0]	!=	m_nRetestTrayXCnt[1])		
	{
		strMsg.Format(_T("Retest Tray X Count Change %d -> %d"), m_nRetestTrayXCnt[0], m_nRetestTrayXCnt[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}

	for (i=0; i<2; i++)
	{
		for (j=0; j<2; j++)
		{
			for (k=0; k<12; k++)
			{
				if (m_nTsiteUse[0][i][j][k]	!= m_nTsiteUse[1][i][j][k])
				{
					strMsg.Format(_T("Testsite Socket [%d][%d][k] Use Change  %d -> %d"), i, j, k, m_nTsiteUse[0][i][j][k], m_nTsiteUse[1][i][j][k]);
					clsFunc.OnLogFileAdd(0, strMsg);
				}
			}
		}
	}

	if (m_nDegree[0][0]	!= m_nDegree[0][1])
	{
		strMsg.Format(_T("Plate Site Rotator Change %d -> %d"), m_nDegree[0][0], m_nDegree[0][1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}

	if (m_nDegree[1][0]	!= m_nDegree[1][1])
	{
		strMsg.Format(_T("Test Site Rotator Change %d -> %d"), m_nDegree[1][0], m_nDegree[1][1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}

	if (m_nDegree[2][0]	!= m_nDegree[2][1])
	{
		strMsg.Format(_T("Barcode Rotator Change %d -> %d"), m_nDegree[2][0], m_nDegree[2][1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}

	if (m_dRejectEmptyOffset[0]	!=	m_dRejectEmptyOffset[1])		
	{
		strMsg.Format(_T("Reject Empty Offset Change %.3f -> %.3f"), m_dRejectEmptyOffset[0], m_dRejectEmptyOffset[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}

	if (m_dRejectFullOffset[0]	!=	m_dRejectFullOffset[1])		
	{
		strMsg.Format(_T("Reject Full Offset Change %.3f -> %.3f"), m_dRejectFullOffset[0], m_dRejectFullOffset[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}

	if (m_nSocketCount[0]	!= m_nSocketCount[1])
	{
		strMsg.Format(_T("Socket Count Change %d -> %d"), m_nSocketCount[0], m_nSocketCount[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}

	if (m_d15TOffset[0]	!= m_d15TOffset[1])
	{
		strMsg.Format(_T("Socket Count Change %.3f -> %.3f"), m_d15TOffset[0], m_d15TOffset[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}

	//kwlee 2017.0116
	if(m_dPcbRowOffset[0]		!=	m_dPcbRowOffset[1])
	{
		strMsg.Format(_T("Pcb Row Offset Change %d -> %d"), m_dPcbRowOffset[0], m_dPcbRowOffset[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}

	if (m_dPcbColOffset[0]	!= m_dPcbColOffset[1])
	{
		strMsg.Format(_T("Pcb Col Offset Change %.3f -> %.3f"), m_dPcbColOffset[0], m_dPcbColOffset[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}
	if (m_dPickerRowOffset[0]	!= m_dPickerRowOffset[1])
	{
		strMsg.Format(_T("Picker Row Offset Change %d -> %d"), m_dPickerRowOffset[0], m_dPickerRowOffset[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}

	if (m_dPickerColOffset[0]	!= m_dPickerColOffset[1])
	{
		strMsg.Format(_T("Picker Col Offset Change %.3f -> %.3f"), m_dPickerColOffset[0], m_dPickerColOffset[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}
	if (m_dLabelRowOffset[0]	!= m_dLabelRowOffset[1])
	{
		strMsg.Format(_T("Label Row Offset Change %d -> %d"), m_dLabelRowOffset[0], m_dLabelRowOffset[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}

	if (m_dLabelColOffset[0]	!= m_dLabelColOffset[1])
	{
		strMsg.Format(_T("Label Col Offset Change %.3f -> %.3f"), m_dLabelColOffset[0], m_dLabelColOffset[1]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}
}


void CWorkRecipe::OnDataRecovery()
{
	int i, j, k;

	m_nTrayYCnt[1]			=	m_nTrayYCnt[0];	
	m_nTrayXCnt[1]			=	m_nTrayXCnt[0];	
	m_nRetCnt[1]			=	m_nRetCnt[0];
	m_nFailCnt[1]			=	m_nFailCnt[0];
	m_dTrayYOffset[1]		=	m_dTrayYOffset[0];	
	m_dTrayXOffset[1]		=	m_dTrayXOffset[0];
	m_dTSiteOffset[1]		=	m_dTSiteOffset[0];	

	m_dCokXOffset[1]		=	m_dCokXOffset[0];	 //2015.0406 james 

	m_dBufYOffset[1]		=	m_dBufYOffset[0];	
	m_dBufXOffset[1]		=	m_dBufXOffset[0];

	m_nRejectTrayXCnt[1]	=	m_nRejectTrayXCnt[0];
	m_nRetestTrayXCnt[1]	=	m_nRetestTrayXCnt[0];

	m_dRejectEmptyOffset[1]	=	m_dRejectEmptyOffset[0];
	m_dRejectFullOffset[1]	=	m_dRejectFullOffset[0];

	for (i=0; i<2; i++)
	{
		for (j=0; j<2; j++)
		{
			for (k=0; k<12; k++)
			{
				m_nTsiteUse[1][i][j][k]	= m_nTsiteUse[0][i][j][k];
			}
		}
	}

	m_nDegree[0][1]	= m_nDegree[0][0];
	m_nDegree[1][1]	= m_nDegree[1][0];
	m_nDegree[2][1] = m_nDegree[2][0];

	m_nSocketCount[1] = m_nSocketCount[0];

	m_d15TOffset[1]	= m_d15TOffset[0];
}


int CWorkRecipe::OnDataComparison()
{
	int i, j, k;

	if (m_nTrayYCnt[0]		!=	m_nTrayYCnt[1])		return RET_ERROR;
	if (m_nTrayXCnt[0]		!=	m_nTrayXCnt[1])		return RET_ERROR;
	if (m_nRetCnt[0]		!=	m_nRetCnt[1])		return RET_ERROR;
	if (m_nFailCnt[0]		!=	m_nFailCnt[1])		return RET_ERROR;
	if (m_dTrayYOffset[0]	!=	m_dTrayYOffset[1])	return RET_ERROR;
	if (m_dTrayXOffset[0]	!=	m_dTrayXOffset[1])	return RET_ERROR;
	if (m_dTSiteOffset[0]	!=	m_dTSiteOffset[1])	return RET_ERROR;

	if (m_dCokXOffset[0]	!=	m_dCokXOffset[1])	return RET_ERROR; //2015.0406 james 

	if (m_dBufYOffset[0]	!=	m_dBufYOffset[1])	return RET_ERROR;
	if (m_dBufXOffset[0]	!=	m_dBufXOffset[1])	return RET_ERROR;

	if (m_nRejectTrayXCnt[0]	!=	m_nRejectTrayXCnt[1])return RET_ERROR;
	if (m_nRetestTrayXCnt[0]	!=	m_nRetestTrayXCnt[1])return RET_ERROR;

	if (m_dRejectEmptyOffset[0]	!=	m_dRejectEmptyOffset[1])return RET_ERROR;
	if (m_dRejectFullOffset[0]	!=	m_dRejectFullOffset[1])	return RET_ERROR;

	for (i=0; i<2; i++)
	{
		for (j=0; j<2; j++)
		{
			for (k=0; k<12; k++)
			{
				if (m_nTsiteUse[0][i][j][k]	!= m_nTsiteUse[1][i][j][k]) return RET_ERROR;
			}
		}
	}

	if (m_nDegree[0][0]	!= m_nDegree[0][1])	return RET_ERROR;
	if (m_nDegree[1][0]	!= m_nDegree[1][1]) return RET_ERROR;
	if (m_nDegree[2][0]	!= m_nDegree[2][1]) return RET_ERROR;

	if (m_nSocketCount[0]	!= m_nSocketCount[1]) return RET_ERROR;
	if (m_d15TOffset[0]	!= m_d15TOffset[1]) return RET_ERROR;
	//kwlee 2016.0116
	if (m_dPcbRowOffset[0]		!=	m_dPcbRowOffset[1])return RET_ERROR;
	if (m_dPcbColOffset[0]		!=	m_dPcbColOffset[1])return RET_ERROR;
	if (m_dPickerRowOffset[0]   !=	m_dPickerRowOffset[1])return RET_ERROR;
	if (m_dPickerColOffset[0]   !=	m_dPickerColOffset[1])return RET_ERROR;
	if (m_dLabelRowOffset[0]    !=	m_dLabelRowOffset[1])return RET_ERROR;
	if (m_dLabelColOffset[0]    !=	m_dLabelColOffset[1])return RET_ERROR;

	return RET_GOOD;
}


void CWorkRecipe::OnDataBackup()
{
	int i, j, k;

	m_nTrayYCnt[0]			=	m_nTrayYCnt[1];	
	m_nTrayXCnt[0]			=	m_nTrayXCnt[1];	
	m_nRetCnt[0]			=	m_nRetCnt[1];
	m_nFailCnt[0]			=	m_nFailCnt[1];
	m_dTrayYOffset[0]		=	m_dTrayYOffset[1];	
	m_dTrayXOffset[0]		=	m_dTrayXOffset[1];
	m_dTSiteOffset[0]		=	m_dTSiteOffset[1];	

	m_dCokXOffset[0]		=	m_dCokXOffset[1];	//2015.0406 james 

	m_dBufYOffset[0]		=	m_dBufYOffset[1];	
	m_dBufXOffset[0]		=	m_dBufXOffset[1];

	m_nRejectTrayXCnt[0]	=	m_nRejectTrayXCnt[1];
	m_nRetestTrayXCnt[0]	=	m_nRetestTrayXCnt[1];

	m_dRejectEmptyOffset[0]	=	m_dRejectEmptyOffset[1];
	m_dRejectFullOffset[0]	=	m_dRejectFullOffset[1];


	for (i=0; i<2; i++)
	{
		for (j=0; j<2; j++)
		{
			for (k=0; k<12; k++)
			{
				m_nTsiteUse[0][i][j][k]	= m_nTsiteUse[1][i][j][k];
			}
		}
	}

	m_nDegree[0][0]	= m_nDegree[0][1];
	m_nDegree[1][0]	= m_nDegree[1][1];
	m_nDegree[2][0] = m_nDegree[2][1];

	m_nSocketCount[0] = m_nSocketCount[1];

	m_d15TOffset[0]	= m_d15TOffset[1];

	//kwlee 2017.0116
	m_dPcbRowOffset[0]    =	m_dPcbRowOffset[1];
	m_dPcbColOffset[0]    =	m_dPcbColOffset[1];
	m_dPickerRowOffset[0] =	m_dPickerRowOffset[1];
	m_dPickerColOffset[0] =	m_dPickerColOffset[1];
	m_dLabelRowOffset[0]  =	m_dLabelRowOffset[1];
	m_dLabelColOffset[0]  = m_dLabelColOffset[1];
}

void CWorkRecipe::OnDataApply()
{
	st_recipe_info.dPcbRowOffset    = 	m_dPcbRowOffset[1];
	st_recipe_info.dPcbColOffset    =	m_dPcbColOffset[1];
	st_recipe_info.dPickerRowOffset =	m_dPickerRowOffset[1];
	st_recipe_info.dPickerColOffset =	m_dPickerColOffset[1];
	st_recipe_info.dLabelRowOffset  =	m_dLabelRowOffset[1];
	st_recipe_info.dLabelColOffset  =	m_dLabelColOffset[1];
}

BOOL CWorkRecipe::OnEraseBkgnd(CDC* pDC)
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


void CWorkRecipe::OnBnClickedBtnApply()
{
	CDialog_Message dlgMsg;
	int nResponse;

	dlgMsg.m_nMessageType	= 1;
	dlgMsg.m_strMessage		= _T("Do you want to change the existing value to the current value?");

	nResponse = (int)dlgMsg.DoModal();

	if (nResponse == IDOK)
	{
		if (OnDataComparison() == RET_ERROR)
		{
			OnDataHistoryLog();
			OnDataBackup();  // 현재 클래스 변수 설정 상태 백업
			OnDataApply();
		}
	}
}


void CWorkRecipe::OnBnClickedBtnReload()
{
	OnDataBackup();  // 현재 클래스 변수 설정 상태 백업
}


void CWorkRecipe::OnDataDisplay()
{
	
}


void CWorkRecipe::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (OnDataComparison() == RET_ERROR)
	{
		OnDataHistoryLog();
	}
	CDialog::PostNcDestroy();
}


void CWorkRecipe::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (OnDataComparison() == RET_ERROR)
	{
		OnDataHistoryLog();
//		OnDataApply(); lkw
	}
}

void CWorkRecipe::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	
	CDialog::OnTimer(nIDEvent);
}


void CWorkRecipe::OnClickedDgtTrayRowCnt()
{
	int nKey = m_nTrayYCnt[1];

	KeyPadI(1, 5, &nKey);

	m_nTrayYCnt[1] = nKey;
	m_dgtTrayRowCnt.SetVal(nKey);
}


void CWorkRecipe::OnClickedDgtTrayColCnt()
{
	int nKey = m_nTrayXCnt[1];

	KeyPadI(1, 5, &nKey);

	m_nTrayXCnt[1] = nKey;
	m_dgtTrayColCnt.SetVal(nKey);
}


void CWorkRecipe::OnClickedDgtTrayRowOffset()
{
	double dKey = m_dTrayYOffset[1];

	KeyPadD(0.0f, 110.0f, &dKey);

	m_dTrayYOffset[1] = dKey;
	m_dgtTrayRowOffset.SetFloatVal((float)dKey);
}


void CWorkRecipe::OnClickedDgtTrayColOffset()
{
	double dKey = m_dTrayXOffset[1];

	KeyPadD(0.0f, 110.0f, &dKey);

	m_dTrayXOffset[1] = dKey;
	m_dgtTrayColOffset.SetFloatVal((float)dKey);
}


void CWorkRecipe::OnClickedDgtTestsiteOffset()
{
	double dKey = m_dTSiteOffset[1];

	KeyPadD(0.0f, 110.0f, &dKey);

	m_dTSiteOffset[1] = dKey;
	m_dgtTSiteOffset.SetFloatVal((float)dKey);
}

void CWorkRecipe::OnStnClickedDgtRetestCount()
{
	int nKey = m_nRetCnt[1];

	KeyPadI(0, 110, &nKey);

	m_nRetCnt[1] = nKey;
	m_dgtRetCnt.SetVal(nKey);
}

void CWorkRecipe::OnStnClickedDgtFailCount()
{
	int nKey = m_nFailCnt[1];

	KeyPadI(0, 110, &nKey);

	m_nFailCnt[1] = nKey;
	m_dgtFailCnt.SetVal(nKey);
}

void CWorkRecipe::OnLeftInterfaceCellClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	int nRow, nCol;
	int nSite = 0;

	CString strMsg;

	NM_GRIDVIEW* pItem	= (NM_GRIDVIEW*) pNotifyStruct;

	nRow				= pItem->iRow;
	nCol				= pItem->iColumn;

	if (nRow < 1) return;


	m_pGridLfInterface.Invalidate(FALSE);
}

void CWorkRecipe::OnRightInterfaceCellClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	int nRow, nCol;
	int nSite = 0;

	CString strMsg;

	NM_GRIDVIEW* pItem	= (NM_GRIDVIEW*) pNotifyStruct;

	nRow				= pItem->iRow;
	nCol				= pItem->iColumn;

	if (nRow < 1) return;
}

void CWorkRecipe::OnDegreeCellClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	CDialog_Message dlgMsg;

	int nRow, nCol;
	CString strTemp;

	NM_GRIDVIEW* pItem	= (NM_GRIDVIEW*) pNotifyStruct;

	nRow				= pItem->iRow;
	nCol				= pItem->iColumn;

	if (nRow == 0) return;
	if (m_nDegree[nCol][1] == (nRow-1)) return;

	dlgMsg.m_nMessageType	= 1;
	switch (nRow-1)
	{
		case 0:
			strTemp = _T("0'");
			break;

		case 1:
			strTemp = _T("90'");
			break;

		case 2:
			strTemp = _T("180'");
			break;

		case 3:
			strTemp = _T("270'");
			break;
	}
	dlgMsg.m_strMessage.Format(_T("Do you want a select [%s] degree?"), strTemp);

	if (dlgMsg.DoModal() == IDOK)
	{
		m_pGridDegree.SetItemBkColour(1, nCol, GREEN_D, BLACK_C);
		m_pGridDegree.SetItemBkColour(2, nCol, GREEN_D, BLACK_C);
		m_pGridDegree.SetItemBkColour(3, nCol, GREEN_D, BLACK_C);
		m_pGridDegree.SetItemBkColour(4, nCol, GREEN_D, BLACK_C);

		m_nDegree[nCol][1] = nRow-1;
		m_pGridDegree.SetItemBkColour(nRow, nCol, GREEN_C, BLACK_C);
	}

	m_pGridDegree.Invalidate(FALSE);
}

void CWorkRecipe::OnInitGridLeftInterface()
{
	int   i, j;
	int	  nMaxRow, nMaxCol;
	int   nRow;
	CString strTmp;

	nMaxRow = 5; // unloader tray y count
	nMaxCol = 13;

	m_pGridLfInterface.SetFrameFocusCell(FALSE);
	m_pGridLfInterface.SetTrackFocusCell(FALSE);
	m_pGridLfInterface.EnableSelection(FALSE);

	m_pGridLfInterface.SetGridLineColor(BLACK_C);
	m_pGridLfInterface.SetGridLines(1);

	m_pGridLfInterface.SetRowCount(nMaxRow);
	m_pGridLfInterface.SetColumnCount(nMaxCol);

	m_pGridLfInterface.SetFixedRowCount(0);
	m_pGridLfInterface.SetFixedColumnCount(0);
	m_pGridLfInterface.SetFixedBkColor(RGB(0,0,200));
	m_pGridLfInterface.SetFixedBkColor(RGB(200,200,255));
	m_pGridLfInterface.SetTextBkColor(RGB(150,150,200));

	for (i=0; i<nMaxRow; i++) 
	{
		m_pGridLfInterface.SetRowHeight(i, 27);

		for (j=0; j<nMaxCol; j++) 
		{
			m_pGridLfInterface.SetColumnWidth(j, 35);

			m_pGridLfInterface.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_pGridLfInterface.SetItemState(i, j, GVIS_READONLY);
			m_pGridLfInterface.SetItemBkColour(i, j, WHITE_C, CLR_DEFAULT);
		}
	}

	nRow = 0;
	m_pGridLfInterface.MergeCells(nRow, 0, nRow, 12);
	m_pGridLfInterface.SetItemBkColour(nRow, 0, BLUE_D, WHITE_C);
	m_pGridLfInterface.SetItemFont(nRow, 0, &clsFunc.OnLogFont(18));
	m_pGridLfInterface.SetItemText(nRow, 0, _T("Left Interface"));

	nRow = 1;
	m_pGridLfInterface.SetItemBkColour(nRow, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridLfInterface.SetItemFont(nRow, 0, &clsFunc.OnLogFont(18));

	m_pGridLfInterface.SetItemBkColour(nRow, 1, RGB(219, 229, 241), BLACK_L);
	m_pGridLfInterface.SetItemFont(nRow, 1, &clsFunc.OnLogFont(18));
	m_pGridLfInterface.SetItemText(nRow, 1, _T("13"));

	m_pGridLfInterface.SetItemBkColour(nRow, 2, RGB(219, 229, 241), BLACK_L);
	m_pGridLfInterface.SetItemFont(nRow, 2, &clsFunc.OnLogFont(18));
	m_pGridLfInterface.SetItemText(nRow, 2, _T("14"));

	m_pGridLfInterface.SetItemBkColour(nRow, 3, RGB(219, 229, 241), BLACK_L);
	m_pGridLfInterface.SetItemFont(nRow, 3, &clsFunc.OnLogFont(18));
	m_pGridLfInterface.SetItemText(nRow, 3, _T("15"));

	m_pGridLfInterface.SetItemBkColour(nRow, 4, RGB(219, 229, 241), BLACK_L);
	m_pGridLfInterface.SetItemFont(nRow, 4, &clsFunc.OnLogFont(18));
	m_pGridLfInterface.SetItemText(nRow, 4, _T("16"));

	m_pGridLfInterface.SetItemBkColour(nRow, 5, RGB(219, 229, 241), BLACK_L);
	m_pGridLfInterface.SetItemFont(nRow, 5, &clsFunc.OnLogFont(18));
	m_pGridLfInterface.SetItemText(nRow, 5, _T("17"));

	m_pGridLfInterface.SetItemBkColour(nRow, 6, RGB(219, 229, 241), BLACK_L);
	m_pGridLfInterface.SetItemFont(nRow, 6, &clsFunc.OnLogFont(18));
	m_pGridLfInterface.SetItemText(nRow, 6, _T("18"));

	m_pGridLfInterface.SetItemBkColour(nRow, 7, RGB(219, 229, 241), BLACK_L);
	m_pGridLfInterface.SetItemFont(nRow, 7, &clsFunc.OnLogFont(18));
	m_pGridLfInterface.SetItemText(nRow, 7, _T("19"));

	m_pGridLfInterface.SetItemBkColour(nRow, 8, RGB(219, 229, 241), BLACK_L);
	m_pGridLfInterface.SetItemFont(nRow, 8, &clsFunc.OnLogFont(18));
	m_pGridLfInterface.SetItemText(nRow, 8, _T("20"));

	m_pGridLfInterface.SetItemBkColour(nRow, 9, RGB(219, 229, 241), BLACK_L);
	m_pGridLfInterface.SetItemFont(nRow, 9, &clsFunc.OnLogFont(18));
	m_pGridLfInterface.SetItemText(nRow, 9, _T("21"));

	m_pGridLfInterface.SetItemBkColour(nRow, 10, RGB(219, 229, 241), BLACK_L);
	m_pGridLfInterface.SetItemFont(nRow, 10, &clsFunc.OnLogFont(18));
	m_pGridLfInterface.SetItemText(nRow, 10, _T("22"));

	m_pGridLfInterface.SetItemBkColour(nRow, 11, RGB(219, 229, 241), BLACK_L);
	m_pGridLfInterface.SetItemFont(nRow, 11, &clsFunc.OnLogFont(18));
	m_pGridLfInterface.SetItemText(nRow, 11, _T("23"));

	m_pGridLfInterface.SetItemBkColour(nRow, 12, RGB(219, 229, 241), BLACK_L);
	m_pGridLfInterface.SetItemFont(nRow, 12, &clsFunc.OnLogFont(18));
	m_pGridLfInterface.SetItemText(nRow, 12, _T("24"));

	nRow = 2;
	m_pGridLfInterface.SetItemBkColour(nRow, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridLfInterface.SetItemFont(nRow, 0, &clsFunc.OnLogFont(18));
	m_pGridLfInterface.SetItemText(nRow, 0, _T("T"));

	nRow = 3;
	m_pGridLfInterface.SetItemBkColour(nRow, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridLfInterface.SetItemFont(nRow, 0, &clsFunc.OnLogFont(18));

	m_pGridLfInterface.SetItemBkColour(nRow, 1, RGB(219, 229, 241), BLACK_L);
	m_pGridLfInterface.SetItemFont(nRow, 1, &clsFunc.OnLogFont(18));
	m_pGridLfInterface.SetItemText(nRow, 1, _T("1"));

	m_pGridLfInterface.SetItemBkColour(nRow, 2, RGB(219, 229, 241), BLACK_L);
	m_pGridLfInterface.SetItemFont(nRow, 2, &clsFunc.OnLogFont(18));
	m_pGridLfInterface.SetItemText(nRow, 2, _T("2"));

	m_pGridLfInterface.SetItemBkColour(nRow, 3, RGB(219, 229, 241), BLACK_L);
	m_pGridLfInterface.SetItemFont(nRow, 3, &clsFunc.OnLogFont(18));
	m_pGridLfInterface.SetItemText(nRow, 3, _T("3"));

	m_pGridLfInterface.SetItemBkColour(nRow, 4, RGB(219, 229, 241), BLACK_L);
	m_pGridLfInterface.SetItemFont(nRow, 4, &clsFunc.OnLogFont(18));
	m_pGridLfInterface.SetItemText(nRow, 4, _T("4"));

	m_pGridLfInterface.SetItemBkColour(nRow, 5, RGB(219, 229, 241), BLACK_L);
	m_pGridLfInterface.SetItemFont(nRow, 5, &clsFunc.OnLogFont(18));
	m_pGridLfInterface.SetItemText(nRow, 5, _T("5"));

	m_pGridLfInterface.SetItemBkColour(nRow, 6, RGB(219, 229, 241), BLACK_L);
	m_pGridLfInterface.SetItemFont(nRow, 6, &clsFunc.OnLogFont(18));
	m_pGridLfInterface.SetItemText(nRow, 6, _T("6"));

	m_pGridLfInterface.SetItemBkColour(nRow, 7, RGB(219, 229, 241), BLACK_L);
	m_pGridLfInterface.SetItemFont(nRow, 7, &clsFunc.OnLogFont(18));
	m_pGridLfInterface.SetItemText(nRow, 7, _T("7"));

	m_pGridLfInterface.SetItemBkColour(nRow, 8, RGB(219, 229, 241), BLACK_L);
	m_pGridLfInterface.SetItemFont(nRow, 8, &clsFunc.OnLogFont(18));
	m_pGridLfInterface.SetItemText(nRow, 8, _T("8"));

	m_pGridLfInterface.SetItemBkColour(nRow, 9, RGB(219, 229, 241), BLACK_L);
	m_pGridLfInterface.SetItemFont(nRow, 9, &clsFunc.OnLogFont(18));
	m_pGridLfInterface.SetItemText(nRow, 9, _T("9"));

	m_pGridLfInterface.SetItemBkColour(nRow, 10, RGB(219, 229, 241), BLACK_L);
	m_pGridLfInterface.SetItemFont(nRow, 10, &clsFunc.OnLogFont(18));
	m_pGridLfInterface.SetItemText(nRow, 10, _T("10"));

	m_pGridLfInterface.SetItemBkColour(nRow, 11, RGB(219, 229, 241), BLACK_L);
	m_pGridLfInterface.SetItemFont(nRow, 11, &clsFunc.OnLogFont(18));
	m_pGridLfInterface.SetItemText(nRow, 11, _T("11"));

	m_pGridLfInterface.SetItemBkColour(nRow, 12, RGB(219, 229, 241), BLACK_L);
	m_pGridLfInterface.SetItemFont(nRow, 12, &clsFunc.OnLogFont(18));
	m_pGridLfInterface.SetItemText(nRow, 12, _T("12"));

	nRow = 4;
	m_pGridLfInterface.SetItemBkColour(nRow, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridLfInterface.SetItemFont(nRow, 0, &clsFunc.OnLogFont(18));
	m_pGridLfInterface.SetItemText(nRow, 0, _T("B"));

	for (i=0; i<2; i++)
	{
		for (j=0; j<12; j++)
		{
			if (m_nTsiteUse[1][0][i][j] == YES)
			{
				m_pGridLfInterface.SetItemBkColour(4-(i*2), j + 1, YELLOW_C, BLACK_L);
				m_pGridLfInterface.SetItemFont(4-(i*2), j + 1, &clsFunc.OnLogFont(18));
				m_pGridLfInterface.SetItemText(4-(i*2), j + 1, _T("Y"));
			}
			else
			{
				m_pGridLfInterface.SetItemBkColour(4-(i*2), j + 1, YELLOW_D, BLACK_L);
				m_pGridLfInterface.SetItemFont(4-(i*2), j + 1, &clsFunc.OnLogFont(18));
				m_pGridLfInterface.SetItemText(4-(i*2), j + 1, _T("N"));
			}
		}
	}

	m_pGridLfInterface.Invalidate(FALSE);
}

void CWorkRecipe::OnInitGridRightInterface()
{
	int   i, j;
	int	  nMaxRow, nMaxCol;
	int   nRow;
	CString strTmp;

	nMaxRow = 5; // unloader tray y count
	nMaxCol = 13;

	m_pGridRiInterface.SetFrameFocusCell(FALSE);
	m_pGridRiInterface.SetTrackFocusCell(FALSE);
	m_pGridRiInterface.EnableSelection(FALSE);

	m_pGridRiInterface.SetGridLineColor(BLACK_C);
	m_pGridRiInterface.SetGridLines(1);

	m_pGridRiInterface.SetRowCount(nMaxRow);
	m_pGridRiInterface.SetColumnCount(nMaxCol);

	m_pGridRiInterface.SetFixedRowCount(0);
	m_pGridRiInterface.SetFixedColumnCount(0);
	m_pGridRiInterface.SetFixedBkColor(RGB(0,0,200));
	m_pGridRiInterface.SetFixedBkColor(RGB(200,200,255));
	m_pGridRiInterface.SetTextBkColor(RGB(150,150,200));

	for (i=0; i<nMaxRow; i++) 
	{
		m_pGridRiInterface.SetRowHeight(i, 27);

		for (j=0; j<nMaxCol; j++) 
		{
			m_pGridRiInterface.SetColumnWidth(j, 35);

			m_pGridRiInterface.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_pGridRiInterface.SetItemState(i, j, GVIS_READONLY);
			m_pGridRiInterface.SetItemBkColour(i, j, WHITE_C, CLR_DEFAULT);
		}
	}

	nRow = 0;
	m_pGridRiInterface.MergeCells(nRow, 0, nRow, 12);
	m_pGridRiInterface.SetItemBkColour(nRow, 0, BLUE_D, WHITE_C);
	m_pGridRiInterface.SetItemFont(nRow, 0, &clsFunc.OnLogFont(18));
	m_pGridRiInterface.SetItemText(nRow, 0, _T("Right Interface"));

	nRow = 1;
	m_pGridRiInterface.SetItemBkColour(nRow, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridRiInterface.SetItemFont(nRow, 0, &clsFunc.OnLogFont(18));

	m_pGridRiInterface.SetItemBkColour(nRow, 1, RGB(219, 229, 241), BLACK_L);
	m_pGridRiInterface.SetItemFont(nRow, 1, &clsFunc.OnLogFont(18));
	m_pGridRiInterface.SetItemText(nRow, 1, _T("37"));

	m_pGridRiInterface.SetItemBkColour(nRow, 2, RGB(219, 229, 241), BLACK_L);
	m_pGridRiInterface.SetItemFont(nRow, 2, &clsFunc.OnLogFont(18));
	m_pGridRiInterface.SetItemText(nRow, 2, _T("38"));

	m_pGridRiInterface.SetItemBkColour(nRow, 3, RGB(219, 229, 241), BLACK_L);
	m_pGridRiInterface.SetItemFont(nRow, 3, &clsFunc.OnLogFont(18));
	m_pGridRiInterface.SetItemText(nRow, 3, _T("39"));

	m_pGridRiInterface.SetItemBkColour(nRow, 4, RGB(219, 229, 241), BLACK_L);
	m_pGridRiInterface.SetItemFont(nRow, 4, &clsFunc.OnLogFont(18));
	m_pGridRiInterface.SetItemText(nRow, 4, _T("40"));

	m_pGridRiInterface.SetItemBkColour(nRow, 5, RGB(219, 229, 241), BLACK_L);
	m_pGridRiInterface.SetItemFont(nRow, 5, &clsFunc.OnLogFont(18));
	m_pGridRiInterface.SetItemText(nRow, 5, _T("41"));

	m_pGridRiInterface.SetItemBkColour(nRow, 6, RGB(219, 229, 241), BLACK_L);
	m_pGridRiInterface.SetItemFont(nRow, 6, &clsFunc.OnLogFont(18));
	m_pGridRiInterface.SetItemText(nRow, 6, _T("42"));

	m_pGridRiInterface.SetItemBkColour(nRow, 7, RGB(219, 229, 241), BLACK_L);
	m_pGridRiInterface.SetItemFont(nRow, 7, &clsFunc.OnLogFont(18));
	m_pGridRiInterface.SetItemText(nRow, 7, _T("43"));

	m_pGridRiInterface.SetItemBkColour(nRow, 8, RGB(219, 229, 241), BLACK_L);
	m_pGridRiInterface.SetItemFont(nRow, 8, &clsFunc.OnLogFont(18));
	m_pGridRiInterface.SetItemText(nRow, 8, _T("44"));

	m_pGridRiInterface.SetItemBkColour(nRow, 9, RGB(219, 229, 241), BLACK_L);
	m_pGridRiInterface.SetItemFont(nRow, 9, &clsFunc.OnLogFont(18));
	m_pGridRiInterface.SetItemText(nRow, 9, _T("45"));

	m_pGridRiInterface.SetItemBkColour(nRow, 10, RGB(219, 229, 241), BLACK_L);
	m_pGridRiInterface.SetItemFont(nRow, 10, &clsFunc.OnLogFont(18));
	m_pGridRiInterface.SetItemText(nRow, 10, _T("46"));

	m_pGridRiInterface.SetItemBkColour(nRow, 11, RGB(219, 229, 241), BLACK_L);
	m_pGridRiInterface.SetItemFont(nRow, 11, &clsFunc.OnLogFont(18));
	m_pGridRiInterface.SetItemText(nRow, 11, _T("47"));

	m_pGridRiInterface.SetItemBkColour(nRow, 12, RGB(219, 229, 241), BLACK_L);
	m_pGridRiInterface.SetItemFont(nRow, 12, &clsFunc.OnLogFont(18));
	m_pGridRiInterface.SetItemText(nRow, 12, _T("48"));

	nRow = 2;
	m_pGridRiInterface.SetItemBkColour(nRow, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridRiInterface.SetItemFont(nRow, 0, &clsFunc.OnLogFont(18));
	m_pGridRiInterface.SetItemText(nRow, 0, _T("T"));

	nRow = 3;
	m_pGridRiInterface.SetItemBkColour(nRow, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridRiInterface.SetItemFont(nRow, 0, &clsFunc.OnLogFont(18));

	m_pGridRiInterface.SetItemBkColour(nRow, 1, RGB(219, 229, 241), BLACK_L);
	m_pGridRiInterface.SetItemFont(nRow, 1, &clsFunc.OnLogFont(18));
	m_pGridRiInterface.SetItemText(nRow, 1, _T("25"));

	m_pGridRiInterface.SetItemBkColour(nRow, 2, RGB(219, 229, 241), BLACK_L);
	m_pGridRiInterface.SetItemFont(nRow, 2, &clsFunc.OnLogFont(18));
	m_pGridRiInterface.SetItemText(nRow, 2, _T("26"));

	m_pGridRiInterface.SetItemBkColour(nRow, 3, RGB(219, 229, 241), BLACK_L);
	m_pGridRiInterface.SetItemFont(nRow, 3, &clsFunc.OnLogFont(18));
	m_pGridRiInterface.SetItemText(nRow, 3, _T("27"));

	m_pGridRiInterface.SetItemBkColour(nRow, 4, RGB(219, 229, 241), BLACK_L);
	m_pGridRiInterface.SetItemFont(nRow, 4, &clsFunc.OnLogFont(18));
	m_pGridRiInterface.SetItemText(nRow, 4, _T("28"));

	m_pGridRiInterface.SetItemBkColour(nRow, 5, RGB(219, 229, 241), BLACK_L);
	m_pGridRiInterface.SetItemFont(nRow, 5, &clsFunc.OnLogFont(18));
	m_pGridRiInterface.SetItemText(nRow, 5, _T("29"));

	m_pGridRiInterface.SetItemBkColour(nRow, 6, RGB(219, 229, 241), BLACK_L);
	m_pGridRiInterface.SetItemFont(nRow, 6, &clsFunc.OnLogFont(18));
	m_pGridRiInterface.SetItemText(nRow, 6, _T("30"));

	m_pGridRiInterface.SetItemBkColour(nRow, 7, RGB(219, 229, 241), BLACK_L);
	m_pGridRiInterface.SetItemFont(nRow, 7, &clsFunc.OnLogFont(18));
	m_pGridRiInterface.SetItemText(nRow, 7, _T("31"));

	m_pGridRiInterface.SetItemBkColour(nRow, 8, RGB(219, 229, 241), BLACK_L);
	m_pGridRiInterface.SetItemFont(nRow, 8, &clsFunc.OnLogFont(18));
	m_pGridRiInterface.SetItemText(nRow, 8, _T("32"));

	m_pGridRiInterface.SetItemBkColour(nRow, 9, RGB(219, 229, 241), BLACK_L);
	m_pGridRiInterface.SetItemFont(nRow, 9, &clsFunc.OnLogFont(18));
	m_pGridRiInterface.SetItemText(nRow, 9, _T("33"));

	m_pGridRiInterface.SetItemBkColour(nRow, 10, RGB(219, 229, 241), BLACK_L);
	m_pGridRiInterface.SetItemFont(nRow, 10, &clsFunc.OnLogFont(18));
	m_pGridRiInterface.SetItemText(nRow, 10, _T("34"));

	m_pGridRiInterface.SetItemBkColour(nRow, 11, RGB(219, 229, 241), BLACK_L);
	m_pGridRiInterface.SetItemFont(nRow, 11, &clsFunc.OnLogFont(18));
	m_pGridRiInterface.SetItemText(nRow, 11, _T("35"));

	m_pGridRiInterface.SetItemBkColour(nRow, 12, RGB(219, 229, 241), BLACK_L);
	m_pGridRiInterface.SetItemFont(nRow, 12, &clsFunc.OnLogFont(18));
	m_pGridRiInterface.SetItemText(nRow, 12, _T("36"));

	nRow = 4;
	m_pGridRiInterface.SetItemBkColour(nRow, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridRiInterface.SetItemFont(nRow, 0, &clsFunc.OnLogFont(18));
	m_pGridRiInterface.SetItemText(nRow, 0, _T("B"));

	for (i=0; i<2; i++)
	{
		for (j=0; j<12; j++)
		{
			if (m_nTsiteUse[1][1][i][j] == YES)
			{
				m_pGridRiInterface.SetItemBkColour(4-(i*2), j + 1, YELLOW_C, BLACK_L);
				m_pGridRiInterface.SetItemFont(4-(i*2), j + 1, &clsFunc.OnLogFont(18));
				m_pGridRiInterface.SetItemText(4-(i*2), j + 1, _T("Y"));
			}
			else
			{
				m_pGridRiInterface.SetItemBkColour(4-(i*2), j + 1, YELLOW_D, BLACK_L);
				m_pGridRiInterface.SetItemFont(4-(i*2), j + 1, &clsFunc.OnLogFont(18));
				m_pGridRiInterface.SetItemText(4-(i*2), j + 1, _T("N"));
			}
		}
	}

	m_pGridRiInterface.Invalidate(FALSE);
}

void CWorkRecipe::OnInitGridDegree()
{
	int   i, j;
	int	  nMaxRow, nMaxCol;
	int   nRow;
	CString strTmp;

	nMaxRow = 5; // unloader tray y count
	nMaxCol = 3;
	
	m_pGridDegree.SetFrameFocusCell(FALSE);
	m_pGridDegree.SetTrackFocusCell(FALSE);
	m_pGridDegree.EnableSelection(FALSE);

	m_pGridDegree.SetGridLineColor(BLACK_C);
	m_pGridDegree.SetGridLines(1);

	m_pGridDegree.SetRowCount(nMaxRow);
	m_pGridDegree.SetColumnCount(nMaxCol);

	m_pGridDegree.SetFixedRowCount(0);
	m_pGridDegree.SetFixedColumnCount(0);
	m_pGridDegree.SetFixedBkColor(RGB(0,0,200));
	m_pGridDegree.SetFixedBkColor(RGB(200,200,255));
	m_pGridDegree.SetTextBkColor(RGB(150,150,200));

	for (i=0; i<nMaxRow; i++) 
	{
		m_pGridDegree.SetRowHeight(i, 30);

		for (j=0; j<nMaxCol; j++) 
		{
			//m_pGridDegree.SetColumnWidth(j, 210);
			m_pGridDegree.SetColumnWidth(j, 140);

			m_pGridDegree.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_pGridDegree.SetItemState(i, j, GVIS_READONLY);
			m_pGridDegree.SetItemBkColour(i, j, WHITE_C, CLR_DEFAULT);
		}
	}

	nRow = 0;
	m_pGridDegree.SetItemBkColour(nRow, 0, BLUE_D, WHITE_C);
	m_pGridDegree.SetItemFont(nRow, 0, &clsFunc.OnLogFont(14));
	m_pGridDegree.SetItemText(nRow, 0, _T("Plate Degree"));

	m_pGridDegree.SetItemBkColour(nRow, 1, BLUE_D, WHITE_C);
	m_pGridDegree.SetItemFont(nRow, 1, &clsFunc.OnLogFont(14));
	m_pGridDegree.SetItemText(nRow, 1, _T("Test Site Degree"));

	m_pGridDegree.SetItemBkColour(nRow, 2, BLUE_D, WHITE_C);
	m_pGridDegree.SetItemFont(nRow, 2, &clsFunc.OnLogFont(14));
	m_pGridDegree.SetItemText(nRow, 2, _T("Barcode Degree"));

	nRow = 1;
	m_pGridDegree.SetItemBkColour(nRow, 0, GREEN_D, BLACK_C);
	m_pGridDegree.SetItemFont(nRow, 0, &clsFunc.OnLogFont(14));
	m_pGridDegree.SetItemText(nRow, 0, _T("0'"));

	m_pGridDegree.SetItemBkColour(nRow, 1, GREEN_D, BLACK_C);
	m_pGridDegree.SetItemFont(nRow, 1, &clsFunc.OnLogFont(14));
	m_pGridDegree.SetItemText(nRow, 1, _T("0'"));

	m_pGridDegree.SetItemBkColour(nRow, 2, GREEN_D, BLACK_C);
	m_pGridDegree.SetItemFont(nRow, 2, &clsFunc.OnLogFont(14));
	m_pGridDegree.SetItemText(nRow, 2, _T("0'"));

	nRow = 2;
	m_pGridDegree.SetItemBkColour(nRow, 0, GREEN_D, BLACK_C);
	m_pGridDegree.SetItemFont(nRow, 0, &clsFunc.OnLogFont(14));
	m_pGridDegree.SetItemText(nRow, 0, _T("90'"));

	m_pGridDegree.SetItemBkColour(nRow, 1, GREEN_D, BLACK_C);
	m_pGridDegree.SetItemFont(nRow, 1, &clsFunc.OnLogFont(14));
	m_pGridDegree.SetItemText(nRow, 1, _T("90'"));

	m_pGridDegree.SetItemBkColour(nRow, 2, GREEN_D, BLACK_C);
	m_pGridDegree.SetItemFont(nRow, 2, &clsFunc.OnLogFont(14));
	m_pGridDegree.SetItemText(nRow, 2, _T("90'"));

	nRow = 3;
	m_pGridDegree.SetItemBkColour(nRow, 0, GREEN_D, BLACK_C);
	m_pGridDegree.SetItemFont(nRow, 0, &clsFunc.OnLogFont(14));
	m_pGridDegree.SetItemText(nRow, 0, _T("180'"));

	m_pGridDegree.SetItemBkColour(nRow, 1, GREEN_D, BLACK_C);
	m_pGridDegree.SetItemFont(nRow, 1, &clsFunc.OnLogFont(14));
	m_pGridDegree.SetItemText(nRow, 1, _T("180'"));

	m_pGridDegree.SetItemBkColour(nRow, 2, GREEN_D, BLACK_C);
	m_pGridDegree.SetItemFont(nRow, 2, &clsFunc.OnLogFont(14));
	m_pGridDegree.SetItemText(nRow, 2, _T("180'"));

	nRow = 4;
	m_pGridDegree.SetItemBkColour(nRow, 0, GREEN_D, BLACK_C);
	m_pGridDegree.SetItemFont(nRow, 0, &clsFunc.OnLogFont(14));
	m_pGridDegree.SetItemText(nRow, 0, _T("270'"));

	m_pGridDegree.SetItemBkColour(nRow, 1, GREEN_D, BLACK_C);
	m_pGridDegree.SetItemFont(nRow, 1, &clsFunc.OnLogFont(14));
	m_pGridDegree.SetItemText(nRow, 1, _T("270'"));

	m_pGridDegree.SetItemBkColour(nRow, 2, GREEN_D, BLACK_C);
	m_pGridDegree.SetItemFont(nRow, 2, &clsFunc.OnLogFont(14));
	m_pGridDegree.SetItemText(nRow, 2, _T("270'"));
// 
// 	m_pGridDegree.SetItemBkColour(m_nDegree[0][1] + 1, 0, GREEN_C, BLACK_C);
// 	m_pGridDegree.SetItemBkColour(m_nDegree[1][1] + 1, 1, GREEN_C, BLACK_C);
// 	m_pGridDegree.SetItemBkColour(m_nDegree[2][1] + 1, 2, GREEN_C, BLACK_C);

	m_pGridDegree.Invalidate(FALSE);
}




void CWorkRecipe::OnClickedDgtBufferRowOffset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	double dKey = m_dBufYOffset[1];

	KeyPadD(0.0f, 100.0f, &dKey);

	m_dBufYOffset[1] = dKey;
	m_dgtBufRowOffset.SetFloatVal((float)dKey);
}


void CWorkRecipe::OnClickedDgtBufferColOffset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	double dKey = m_dBufXOffset[1];

	KeyPadD(0.0f, 100.0f, &dKey);

	m_dBufXOffset[1] = dKey;
	m_dgtBufColOffset.SetFloatVal((float)dKey);
}


void CWorkRecipe::OnStnClickedDgtRejectTrayXCnt()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nKey = m_nRejectTrayXCnt[1];

	KeyPadI(1, 9, &nKey);

	m_nRejectTrayXCnt[1] = nKey;
	m_dgtRejectTrayXCnt.SetVal(nKey);
}


void CWorkRecipe::OnStnClickedDgtRetestTrayXCnt()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nKey = m_nRetestTrayXCnt[1];

	KeyPadI(1, 9, &nKey);

	m_nRetestTrayXCnt[1] = nKey;
	m_dgtRetestTrayXCnt.SetVal(nKey);
}


void CWorkRecipe::OnStnClickedDgtRejectFullOffset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	double dKey = m_dRejectFullOffset[1];

	KeyPadD(0.0f, 100.0f, &dKey);

	m_dRejectFullOffset[1] = dKey;
	m_dgtRejectFullOffset.SetFloatVal((float)dKey);
}


void CWorkRecipe::OnStnClickedDgtRejectEmptyOffset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	double dKey = m_dRejectEmptyOffset[1];

	KeyPadD(0.0f, 100.0f, &dKey);

	m_dRejectEmptyOffset[1] = dKey;
	m_dgtRejectEmptyOffset.SetFloatVal((float)dKey);
}


 

//2015.0406 james 적용이 안되어 추가 정리 
void CWorkRecipe::OnStnClickedDgtCokXOffset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	double dKey = m_dCokXOffset[1];

	KeyPadD(0.0f, 250.0f, &dKey);

	m_dCokXOffset[1] = dKey;
	m_dgtCokXOffset.SetFloatVal((float)dKey);
}

void CWorkRecipe::OnStnClickedDgtSocketCount()
{
	// TODO: ¿©±â¿¡ ÄÁÆ®·Ñ ¾Ë¸² Ã³¸®±â ÄÚµå¸¦ Ãß°¡ÇÕ´Ï´Ù.
	int nKey = m_nSocketCount[1];

	KeyPadI(0, 500000, &nKey);

	m_nSocketCount[1] = nKey;
	m_dgtSocketCount.SetVal(nKey);
}


void CWorkRecipe::OnStnClickedDgt15tOffset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog_KeyPad dlgKeyPad;
	CString strTmp;
	double dVal;

	dlgKeyPad.m_nKeypadMode				= 1;
	dlgKeyPad.m_strKeypadLowLimit		= _T("-200");
	dlgKeyPad.m_strKeypadHighLimit		= _T("200");
	dlgKeyPad.m_strKeypadVal.Format(_T("%.3f"), m_d15TOffset[1]);

	if (dlgKeyPad.DoModal() == IDOK)
	{
		strTmp					= dlgKeyPad.m_strNewVal;
		dVal					= _wtof(strTmp);
		m_d15TOffset[1]			= dVal;

		m_dgt15TOffset.SetFloatVal((float)dVal);
	}

	double dKey = m_d15TOffset[1];

	KeyPadD(-200.f, 200.0f, &dKey);

	m_d15TOffset[1] = dKey;
	m_dgt15TOffset.SetFloatVal((float)dKey);
}

//kwlee 2017.0116
void CWorkRecipe::OnStnClickedDgtPcbRowOffset()
{
	
	double dKey = m_dPcbRowOffset[1];

	KeyPadD(0.0f, 110.0f, &dKey);

	m_dPcbRowOffset[1] = dKey;
	m_dgtPcbRowOffset.SetFloatVal((float)dKey);
}


void CWorkRecipe::OnStnClickedDgtPcbColOffset()
{
	double dKey = m_dPcbColOffset[1];

	KeyPadD(0.0f, 110.0f, &dKey);

	m_dPcbColOffset[1] = dKey;
	m_dgtPcbColOffset.SetFloatVal((float)dKey);
}


void CWorkRecipe::OnStnClickedDgtPickerRowOffset()
{
	double dKey = m_dPickerRowOffset[1];

	KeyPadD(0.0f, 110.0f, &dKey);

	m_dPickerRowOffset[1] = dKey;
	m_dgtPickerRowOffset.SetFloatVal((float)dKey);
}


void CWorkRecipe::OnStnClickedDgtPickerColOffset()
{
	double dKey = m_dPickerColOffset[1];

	KeyPadD(0.0f, 110.0f, &dKey);

	m_dPickerColOffset[1] = dKey;
	m_dgtPickerColOffset.SetFloatVal((float)dKey);
}


void CWorkRecipe::OnStnClickedDgtLabelRowOffset()
{
	double dKey = m_dLabelRowOffset[1];

	KeyPadD(0.0f, 110.0f, &dKey);

	m_dLabelRowOffset[1] = dKey;
	m_dgtLabelRowOffset.SetFloatVal((float)dKey);
}


void CWorkRecipe::OnStnClickedDgtLabelColOffset()
{
	double dKey = m_dLabelColOffset[1];

	KeyPadD(0.0f, 110.0f, &dKey);

	m_dLabelColOffset[1] = dKey;
	m_dgtLabelColOffset.SetFloatVal((float)dKey);
}
