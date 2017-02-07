// ScreenMotor.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Handler.h"
#include "ScreenMotor.h"
#include "PublicFunction.h"
#include "Alg_Excel.h"
#include "ComizoaPublic.h"
#include "CtlBd_Variable.h"
#include "CtlBd_Library.h"
#include "Cmmsdk.h"
#include "Dialog_KeyPad.h"
#include "Dialog_Message.h"
#include "Dialog_Part_IO.h "
#include "AlgMemory.h"
#include "DialogManualIO.h"
#include ".\\Ctrl\\KeyBoard.h"
// CScreenMotor

#define TMR_STATUS_CHECK		100
#define TMR_POSITION_CHECK		200
#define TMR_MOVE_CHECK			300
#define TMR_HOME_CHECK			400

IMPLEMENT_DYNCREATE(CScreenMotor, CFormView)

CScreenMotor::CScreenMotor()
	: CFormView(CScreenMotor::IDD)
{
	m_nMotorPartNum		= 0;
	m_nMotorPartAxisNum	= 0;
	m_nMoveMode			= 0;

	m_nCellRowPos		= m_nCellColPos = 0;
	m_nImgPos			= -1;

	m_dDistance			= 10.0f;

	m_pIO				= NULL;
}

CScreenMotor::~CScreenMotor()
{
}

void CScreenMotor::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_MOTOR_LIST, m_treeMotorList);
	DDX_Control(pDX, IDC_GROUP_PART_LIST, m_groupPartList);
	DDX_Control(pDX, IDC_GROUP_MOTOR_LIST, m_groupMotorList);
	DDX_Control(pDX, IDC_CUSTOM_MOTOR_POSITION, m_pGridMotPosition);
	DDX_Control(pDX, IDC_CUSTOM_MOTOR_STATUS, m_pGridMotStatus);
	DDX_Control(pDX, IDC_BTN_HOME, m_btnHome);
	DDX_Control(pDX, IDC_BTN_LEFT, m_btnLeft);
	DDX_Control(pDX, IDC_BTN_RIGHT, m_btnRight);
	DDX_Control(pDX, IDC_BTN_APPLY, m_btnApply);
	DDX_Control(pDX, IDC_BTN_PART_IO, m_btnPartIO);
	DDX_Control(pDX, IDC_BTN_EMERGENCY, m_btnEmergency);
	DDX_Control(pDX, IDC_BTN_MANUAL_FUNCTION, m_btnManualFunction); //2015.02.13
	DDX_Control(pDX, IDC_BTN_MANUAL_IO, m_btnManualIO);
}

BEGIN_MESSAGE_MAP(CScreenMotor, CFormView)
	ON_BN_CLICKED(IDC_GROUP_IO_LIST, &CScreenMotor::OnBnClickedGroupIoList)
	ON_BN_CLICKED(IDC_BTN_EMERGENCY, &CScreenMotor::OnClickedBtnEmergency)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_MOTOR_LIST, &CScreenMotor::OnSelchangedTreeMotorList)
	ON_WM_PAINT()
	ON_MESSAGE(WMU_CHILD_LBUTTON_DOWN, OnUserLButtonDown)
	ON_MESSAGE(WMU_CHILD_LBUTTON_UP, OnUserLButtonUp)
	ON_NOTIFY(NM_CLICK, IDC_CUSTOM_MOTOR_POSITION, OnMotorCellClick)
	ON_NOTIFY(NM_CLICK, IDC_CUSTOM_MOTOR_STATUS, OnStatusCellClick)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_APPLY, &CScreenMotor::OnBnClickedBtnApply)
	ON_BN_CLICKED(IDC_BTN_PART_IO, &CScreenMotor::OnBnClickedBtnPartIo)
	ON_BN_CLICKED(IDC_BTN_MANUAL_FUNCTION,&CScreenMotor::OnBnClickedBtnManualFunction) //2015.02.13
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_MANUAL_IO, &CScreenMotor::OnBnClickedBtnManualIo)
	ON_MESSAGE(WM_WORK_COMMAND,			OnMotorWorkInfoCommand) 
	ON_BN_CLICKED(IDC_BTN_LEFT_UNCLAMP, &CScreenMotor::OnBnClickedBtnLeftUnclamp)
END_MESSAGE_MAP()


// CScreenMotor 진단입니다.

#ifdef _DEBUG
void CScreenMotor::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CScreenMotor::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CScreenMotor 메시지 처리기입니다.


void CScreenMotor::OnBnClickedGroupIoList()
{
	// TODO: Add your control notification handler code here
}

void CScreenMotor::OnInitButton()
{
	CString strTmp;

	m_btnLeft.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnLeft.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnLeft.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnLeft.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnLeft.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnLeft.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	strTmp.Format(_T("[%s] : - Move"), clsExcel.m_strPartAxisName[m_nMotorPartNum][m_nMotorPartAxisNum]);
	m_btnLeft.SetTooltipText(strTmp);

	m_btnRight.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnRight.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnRight.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnRight.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnRight.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnRight.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	strTmp.Format(_T("[%s] : + Move"), clsExcel.m_strPartAxisName[m_nMotorPartNum][m_nMotorPartAxisNum]);
	m_btnRight.SetTooltipText(strTmp);

	m_btnHome.SetBitmaps(IDC_BTN_HOME, IDB_HOME_DN, WINDOW_DN, IDB_HOME_UP, WINDOW_UP);
	m_btnHome.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnHome.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnHome.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnHome.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnHome.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnHome.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	strTmp.Format(_T("[%s] : Home Move"), clsExcel.m_strPartAxisName[m_nMotorPartNum][m_nMotorPartAxisNum]);
	m_btnHome.SetTooltipText(strTmp);

	switch (clsExcel.m_nAxisDirection[m_nMotorPartNum][m_nMotorPartAxisNum])
	{
		case 0:
			m_btnLeft.SetBitmaps(IDC_BTN_LEFT, IDB_LEFT_M_DN, WINDOW_DN, IDB_LEFT_M_UP, WINDOW_UP);

			m_btnRight.SetBitmaps(IDC_BTN_RIGHT, IDB_RIGHT_P_DN, WINDOW_DN, IDB_RIGHT_P_UP, WINDOW_UP);
			break;

		case 1:
			m_btnLeft.SetBitmaps(IDC_BTN_RIGHT, IDB_LEFT_P_DN , WINDOW_DN,IDB_LEFT_P_UP , WINDOW_UP);
			m_btnRight.SetBitmaps( IDC_BTN_LEFT,IDB_RIGHT_M_DN , WINDOW_DN, IDB_RIGHT_M_UP, WINDOW_UP);
			break;

		case 2:
			m_btnLeft.SetBitmaps(IDC_BTN_LEFT, IDB_UP_M_DN, WINDOW_DN, IDB_UP_M_UP, WINDOW_UP);
			m_btnRight.SetBitmaps(IDC_BTN_RIGHT, IDB_DN_P_DN, WINDOW_DN, IDB_DN_P_UP, WINDOW_UP);
			break;

		case 3:
			m_btnLeft.SetBitmaps(IDC_BTN_LEFT, IDB_DN_M_DN, WINDOW_DN, IDB_DN_M_UP, WINDOW_UP);
			m_btnRight.SetBitmaps(IDC_BTN_RIGHT, IDB_UP_P_DN, WINDOW_DN, IDB_UP_P_UP, WINDOW_UP);
			break;
	}

	m_btnApply.SetBitmaps(IDC_BTN_APPLY, IDB_BITMAP_APPLY_DN, WINDOW_DN, IDB_BITMAP_APPLY_UP, WINDOW_UP);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnApply.SetFont(clsFunc.m_pFont[1]);
	m_btnApply.SetTooltipText(_T("Motor Position Data Apply"));

	m_btnPartIO.SetBitmaps(IDC_BTN_APPLY, IDB_BITMAP_APPLY_DN, WINDOW_DN, IDB_BITMAP_APPLY_UP, WINDOW_UP);
	m_btnPartIO.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnPartIO.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnPartIO.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnPartIO.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnPartIO.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnPartIO.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnPartIO.SetFont(clsFunc.m_pFont[1]);
	m_btnPartIO.SetTooltipText(_T("Part IO View Show"));

	m_btnEmergency.SetBitmaps(IDC_BTN_RELOAD, IDB_BITMAP_EMERGENCY_DN, WINDOW_DN, IDB_BITMAP_EMERGENCY_UP, WINDOW_UP);
	m_btnEmergency.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnEmergency.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnEmergency.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnEmergency.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnEmergency.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnEmergency.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnEmergency.SetFont(clsFunc.m_pFont[1]);
	m_btnEmergency.SetTooltipText(_T("Motor Emergency Stop"));
	//2015.02.13
	m_btnManualFunction.SetBitmaps(IDC_BTN_MANUAL_FUNCTION, IDB_BITMAP_APPLY_DN, WINDOW_DN, IDB_BITMAP_APPLY_UP, WINDOW_UP);
	m_btnManualFunction.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnManualFunction.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnManualFunction.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnManualFunction.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnManualFunction.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnManualFunction.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnManualFunction.SetFont(clsFunc.m_pFont[1]);
	m_btnManualFunction.SetTooltipText(_T("Manual Function Dlg Show"));

	m_btnManualIO.SetBitmaps(IDC_BTN_MANUAL_IO, IDB_BITMAP_GEAR_DN, WINDOW_DN, IDB_BITMAP_GEAR_UP, WINDOW_UP);
	m_btnManualIO.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnManualIO.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnManualIO.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnManualIO.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnManualIO.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnManualIO.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnManualIO.SetFont(clsFunc.m_pFont[1]);
	m_btnManualIO.SetTooltipText(_T("Manual Function Dlg Show"));
}

void CScreenMotor::OnInitGridMotorPosition(int nPartNum, int nAxisNum)
{
	CString strTmp;
	int i, j;
	int row;
	int col;
	
	m_pGridMotPosition.SetFrameFocusCell(FALSE);
	m_pGridMotPosition.SetTrackFocusCell(FALSE);
	m_pGridMotPosition.EnableSelection(FALSE);

	m_pGridMotPosition.SetGridLineColor(BLACK_C);
	m_pGridMotPosition.SetGridLines(1);

	m_pGridMotPosition.SetRowCount(clsExcel.m_nAxisPosNum[nPartNum][nAxisNum] + 2);
	m_pGridMotPosition.SetColumnCount(4);

	m_pGridMotPosition.SetFixedRowCount(0);
	m_pGridMotPosition.SetFixedColumnCount(0);
	m_pGridMotPosition.SetFixedBkColor(RGB(0,0,200));
	m_pGridMotPosition.SetFixedBkColor(RGB(200,200,255));
	m_pGridMotPosition.SetTextBkColor(RGB(150,150,200));

	for (i=0; i<clsExcel.m_nAxisPosNum[nPartNum][nAxisNum] + 2; i++) 
	{
		m_pGridMotPosition.SetRowHeight(i, 50);

		for (j=0; j<4; j++) 
		{
			switch (j)
			{
				case 0:
					m_pGridMotPosition.SetColumnWidth(j, 255);
					break;

				case 1:
					m_pGridMotPosition.SetColumnWidth(j, 150);
					break;

				case 2:
					m_pGridMotPosition.SetColumnWidth(j, 100);
					break;

				case 3:
					m_pGridMotPosition.SetColumnWidth(j, 100);
					break;
			}

			m_pGridMotPosition.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_pGridMotPosition.SetItemState(i, j, GVIS_READONLY);
			m_pGridMotPosition.SetItemBkColour(i, j, WHITE_C, CLR_DEFAULT);
		}
	}

	row = 0;
	col = 0;
	m_pGridMotPosition.MergeCells(row, col, row, 3);
	m_pGridMotPosition.SetItemBkColour(row, col, RGB(219, 229, 241), BLACK_L);
	m_pGridMotPosition.SetItemFont(row, col, &clsFunc.OnLogFont(24));
	m_pGridMotPosition.SetItemText(row, col, clsExcel.m_strPartAxisName[nPartNum][nAxisNum]);

	row = 1;
	col = 0;
	m_pGridMotPosition.SetItemBkColour(row, col,RGB(79, 79, 225), WHITE_C);
	m_pGridMotPosition.SetItemFont(row, col, &clsFunc.OnLogFont(20));
	m_pGridMotPosition.SetItemText(row, col, _T(""));

	col = 1;
	m_pGridMotPosition.SetItemBkColour(row, col, RGB(79, 79, 225), WHITE_C);
	m_pGridMotPosition.SetItemFont(row, col, &clsFunc.OnLogFont(20));
	m_pGridMotPosition.SetItemText(row, col, _T("Position"));

	col = 2;
	m_pGridMotPosition.SetItemBkColour(row, col, RGB(79, 79, 225), WHITE_C);
	m_pGridMotPosition.SetItemFont(row, col, &clsFunc.OnLogFont(20));
	m_pGridMotPosition.SetItemText(row, col, _T("Move"));

	col = 3;
	m_pGridMotPosition.SetItemBkColour(row, col, RGB(79, 79, 225), WHITE_C);
	m_pGridMotPosition.SetItemFont(row, col, &clsFunc.OnLogFont(20));
	m_pGridMotPosition.SetItemText(row, col, _T("Read"));
	
	for(i=0; i<clsExcel.m_nAxisPosNum[nPartNum][nAxisNum]; i++)
	{
		m_pGridMotPosition.SetItemBkColour(i + 2, 0, RGB(79, 79, 225), WHITE_C);
		m_pGridMotPosition.SetItemFont(i + 2, 0, &clsFunc.OnLogFont(16));
		m_pGridMotPosition.SetItemText(i + 2, 0, clsExcel.m_strAxisPosName[nPartNum][nAxisNum][i]);

		m_pGridMotPosition.SetItemBkColour(i + 2, 1, GREEN_L, BLACK_L);
		m_pGridMotPosition.SetItemFont(i + 2, 1, &clsFunc.OnLogFont(20));
		strTmp.Format(_T("%.3f"), 0.f);
		m_pGridMotPosition.SetItemText(i + 2, 1, strTmp);

		m_pGridMotPosition.SetItemBkColour(i + 2, 2, SKY_C, ORANGE_C);
		m_pGridMotPosition.SetItemFont(i + 2, 2, &clsFunc.OnLogFont(20));
		m_pGridMotPosition.SetItemText(i + 2, 2, _T("Go."));

		m_pGridMotPosition.SetItemBkColour(i + 2, 3, TEXT_BC, BLACK_L);
		m_pGridMotPosition.SetItemFont(i + 2, 3, &clsFunc.OnLogFont(20));
		m_pGridMotPosition.SetItemText(i + 2, 3, _T("Read."));
	}

	m_pGridMotStatus.Invalidate(FALSE);
}

void CScreenMotor::OnInitGridMotorStatus(int nPartNum, int nAxisNum)
{
	CString strTmp;
	int i, j;
	int row;
	
	m_pGridMotStatus.SetFrameFocusCell(FALSE);
	m_pGridMotStatus.SetTrackFocusCell(FALSE);
	m_pGridMotStatus.EnableSelection(FALSE);

	m_pGridMotStatus.SetGridLineColor(BLACK_C);
	m_pGridMotStatus.SetGridLines(1);

	m_pGridMotStatus.SetRowCount(8);
	m_pGridMotStatus.SetColumnCount(6);

	m_pGridMotStatus.SetFixedRowCount(0);
	m_pGridMotStatus.SetFixedColumnCount(0);
	m_pGridMotStatus.SetFixedBkColor(RGB(0,0,200));
	m_pGridMotStatus.SetFixedBkColor(RGB(200,200,255));
	m_pGridMotStatus.SetTextBkColor(RGB(150,150,200));

	for (i=0; i<8; i++) 
	{
		m_pGridMotStatus.SetRowHeight(i, 39);

		for (j=0; j<6; j++) 
		{
			m_pGridMotStatus.SetColumnWidth(j, 51);

			m_pGridMotStatus.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_pGridMotStatus.SetItemState(i, j, GVIS_READONLY);
			m_pGridMotStatus.SetItemBkColour(i, j, WHITE_C, CLR_DEFAULT);
		}
	}

	if (clsExcel.m_nAxisLimitCnt[nPartNum][nAxisNum] == 2)
	{
		m_pGridMotStatus.UnMergeCells(0, 0, 0, 1);
		m_pGridMotStatus.UnMergeCells(0, 2, 0, 3);
		m_pGridMotStatus.UnMergeCells(0, 4, 0, 5);

		m_pGridMotStatus.UnMergeCells(1, 0, 1, 1);
		m_pGridMotStatus.UnMergeCells(1, 2, 1, 3);
		m_pGridMotStatus.UnMergeCells(1, 4, 1, 5);

		row = 0;
		m_pGridMotStatus.MergeCells(row, 0, row, 2);
		m_pGridMotStatus.SetItemBkColour(row, 0, RGB(79, 79, 225), WHITE_C);
		m_pGridMotStatus.SetItemFont(row, 0, &clsFunc.OnLogFont(20));

		m_pGridMotStatus.MergeCells(row, 3, row, 5);
		m_pGridMotStatus.SetItemBkColour(row, 3, RGB(79, 79, 225), WHITE_C);
		m_pGridMotStatus.SetItemFont(row, 3, &clsFunc.OnLogFont(20));

		row = 1;
		m_pGridMotStatus.MergeCells(row, 0, row, 2);
		m_pGridMotStatus.SetItemBkColour(row, 0, BLACK_L, WHITE_C);
		m_pGridMotStatus.SetItemFont(row, 0, &clsFunc.OnLogFont(20));

		m_pGridMotStatus.MergeCells(row, 3, row, 5);
		m_pGridMotStatus.SetItemBkColour(row, 3, BLACK_L, WHITE_C);
		m_pGridMotStatus.SetItemFont(row, 3, &clsFunc.OnLogFont(20));

		for(i=0; i<2; i++)
		{
			switch (clsExcel.m_nAxisLimitInfo[nPartNum][nAxisNum][i])
			{
				case 0:
					m_pGridMotStatus.SetItemText(0, i * 3, _T("HOME"));
					break;

				case 1:
					m_pGridMotStatus.SetItemText(0, i * 3, _T("ML"));
					break;

				case 2:
					m_pGridMotStatus.SetItemText(0, i * 3, _T("PL"));
					break;

				case 3:
					m_pGridMotStatus.SetItemText(0, i * 3, _T("SD"));
					break;
			}
		}

	}
	else if (clsExcel.m_nAxisLimitCnt[nPartNum][nAxisNum] == 3)
	{
		m_pGridMotStatus.UnMergeCells(0, 0, 0, 2);
		m_pGridMotStatus.UnMergeCells(0, 3, 0, 5);

		m_pGridMotStatus.UnMergeCells(1, 0, 1, 2);
		m_pGridMotStatus.UnMergeCells(1, 3, 1, 5);

		row = 0;
		m_pGridMotStatus.MergeCells(row, 0, row, 1);
		m_pGridMotStatus.SetItemBkColour(row, 0, RGB(79, 79, 225), WHITE_C);
		m_pGridMotStatus.SetItemFont(row, 0, &clsFunc.OnLogFont(20));

		m_pGridMotStatus.MergeCells(row, 2, row, 3);
		m_pGridMotStatus.SetItemBkColour(row, 2, RGB(79, 79, 225), WHITE_C);
		m_pGridMotStatus.SetItemFont(row, 2, &clsFunc.OnLogFont(20));

		m_pGridMotStatus.MergeCells(row, 4, row, 5);
		m_pGridMotStatus.SetItemBkColour(row, 4, RGB(79, 79, 225), WHITE_C);
		m_pGridMotStatus.SetItemFont(row, 4, &clsFunc.OnLogFont(20));

		row = 1;
		m_pGridMotStatus.MergeCells(row, 0, row, 1);
		m_pGridMotStatus.SetItemBkColour(row, 0, BLACK_L, WHITE_C);
		m_pGridMotStatus.SetItemFont(row, 0, &clsFunc.OnLogFont(20));

		m_pGridMotStatus.MergeCells(row, 2, row, 3);
		m_pGridMotStatus.SetItemBkColour(row, 2, BLACK_L, WHITE_C);
		m_pGridMotStatus.SetItemFont(row, 2, &clsFunc.OnLogFont(20));

		m_pGridMotStatus.MergeCells(row, 4, row, 5);
		m_pGridMotStatus.SetItemBkColour(row, 4, BLACK_L, WHITE_C);
		m_pGridMotStatus.SetItemFont(row, 4, &clsFunc.OnLogFont(20));

		for(i=0; i<3; i++)
		{
			switch (clsExcel.m_nAxisLimitInfo[nPartNum][nAxisNum][i])
			{
				case 0:
					m_pGridMotStatus.SetItemText(0, i * 2, _T("HOME"));
					break;

				case 1:
					m_pGridMotStatus.SetItemText(0, i * 2, _T("ML"));
					break;

				case 2:
					m_pGridMotStatus.SetItemText(0, i * 2, _T("PL"));
					break;

				case 3:
					m_pGridMotStatus.SetItemText(0, i * 2, _T("SD"));
					break;
			}
		}
	}

	row = 2;
	m_pGridMotStatus.MergeCells(row, 0, row, 1);
	m_pGridMotStatus.SetItemBkColour(row, 0, RGB(79, 79, 225), WHITE_C);
	m_pGridMotStatus.SetItemFont(row, 0, &clsFunc.OnLogFont(18));
	m_pGridMotStatus.SetItemText(row, 0, _T("Motor Power"));

	m_pGridMotStatus.MergeCells(row, 2, row + 1, 3);
	m_pGridMotStatus.SetItemBkColour(row, 2, BLACK_L, YELLOW_C);
	m_pGridMotStatus.SetItemFont(row, 2, &clsFunc.OnLogFont(18));
	m_pGridMotStatus.SetItemText(row, 2, _T("Power Off"));

	m_pGridMotStatus.MergeCells(row, 4, row + 1, 5);
	m_pGridMotStatus.SetItemBkColour(row, 4, RED_C, BLACK_C);
	m_pGridMotStatus.SetItemFont(row, 4, &clsFunc.OnLogFont(18));
	m_pGridMotStatus.SetItemText(row, 4, _T("Power On"));

	row = 3;
	m_pGridMotStatus.MergeCells(row, 0, row, 1);
	m_pGridMotStatus.SetItemBkColour(row, 0, BLACK_L, WHITE_C);
	m_pGridMotStatus.SetItemFont(row, 0, &clsFunc.OnLogFont(18));
	m_pGridMotStatus.SetItemText(row, 0, _T(""));

	row = 4;
	m_pGridMotStatus.MergeCells(row, 0, row, 1);
	m_pGridMotStatus.SetItemBkColour(row, 0, RGB(79, 79, 225), WHITE_C);
	m_pGridMotStatus.SetItemFont(row, 0, &clsFunc.OnLogFont(18));
	m_pGridMotStatus.SetItemText(row, 0, _T("Motor Alarm"));

	m_pGridMotStatus.MergeCells(row, 2, row + 1, 5);
	m_pGridMotStatus.SetItemBkColour(row, 2, RED_C, BLACK_C);
	m_pGridMotStatus.SetItemFont(row, 2, &clsFunc.OnLogFont(20));
	m_pGridMotStatus.SetItemText(row, 2, _T("Alarm Reset"));

	row = 5;
	m_pGridMotStatus.MergeCells(row, 0, row, 1);
	m_pGridMotStatus.SetItemBkColour(row, 0, BLACK_L, WHITE_C);
	m_pGridMotStatus.SetItemFont(row, 0, &clsFunc.OnLogFont(20));
	m_pGridMotStatus.SetItemText(row, 0, _T(""));

	row = 6;
	m_pGridMotStatus.MergeCells(row, 0, row, 1);
	m_pGridMotStatus.SetItemBkColour(row, 0, RGB(79, 79, 225), WHITE_C);
	m_pGridMotStatus.SetItemFont(row, 0, &clsFunc.OnLogFont(18));
	m_pGridMotStatus.SetItemText(row, 0, _T("Motor Move"));

	m_pGridMotStatus.MergeCells(row, 2, row, 3);
	m_pGridMotStatus.SetItemBkColour(row, 2, RGB(79, 79, 225), WHITE_C);
	m_pGridMotStatus.SetItemFont(row, 2, &clsFunc.OnLogFont(18));
	m_pGridMotStatus.SetItemText(row, 2, _T("Current"));

	m_pGridMotStatus.MergeCells(row, 4, row, 5);
	m_pGridMotStatus.SetItemBkColour(row, 4, RGB(79, 79, 225), WHITE_C);
	m_pGridMotStatus.SetItemFont(row, 4, &clsFunc.OnLogFont(18));
	m_pGridMotStatus.SetItemText(row, 4, _T("Distance"));

	row = 7;
	m_pGridMotStatus.SetItemBkColour(row, 0, GREEN_C, BLACK_C);
	m_pGridMotStatus.SetItemFont(row, 0, &clsFunc.OnLogFont(18));
	m_pGridMotStatus.SetItemText(row, 0, _T("Jog"));

	m_pGridMotStatus.SetItemBkColour(row, 1, BLACK_L, YELLOW_C);
	m_pGridMotStatus.SetItemFont(row, 1, &clsFunc.OnLogFont(18));
	m_pGridMotStatus.SetItemText(row, 1, _T("mm"));

	m_pGridMotStatus.MergeCells(row, 2, row, 3);
	m_pGridMotStatus.SetItemBkColour(row, 2, WHITE_C, BLACK_C);
	m_pGridMotStatus.SetItemFont(row, 2, &clsFunc.OnLogFont(18));
	m_pGridMotStatus.SetItemText(row, 2, _T("0"));

	m_pGridMotStatus.MergeCells(row, 4, row, 5);
	m_pGridMotStatus.SetItemBkColour(row, 4, WHITE_C, BLACK_C);
	m_pGridMotStatus.SetItemFont(row, 4, &clsFunc.OnLogFont(18));
	m_pGridMotStatus.SetItemText(row, 4, _T("10.0"));
}

void CScreenMotor::OnInitGroupBox()
{
	m_groupMotorList.SetFont(clsFunc.OnLogFont(16));
	m_groupMotorList.SetCatptionTextColor(BLUE_C);
	m_groupMotorList.SetBorderColor(ORANGE_C);
	m_groupMotorList.SetFontBold(TRUE);
	m_groupMotorList.SetBackgroundColor(WINDOW_UP);

	m_groupPartList.SetFont(clsFunc.OnLogFont(16));
	m_groupPartList.SetCatptionTextColor(BLUE_C);
	m_groupPartList.SetBorderColor(ORANGE_C);
	m_groupPartList.SetFontBold(TRUE);
	m_groupPartList.SetBackgroundColor(WINDOW_UP);
}

void CScreenMotor::OnInitRadio()
{

}

void CScreenMotor::OnInitTree()
{
	int i, j;

	CString strTmp;

	CBitmap bmp;
	m_pImagelist = new CImageList;
	m_pImagelist->Create(24, 24, ILC_COLOR32|ILC_MASK,0, -1);

	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_HIRFOLDER));//12
	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_HIOPEN));//7
	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_HICLOSE));//6

	m_treeMotorList.SetImageList(m_pImagelist, TVSIL_NORMAL);
	m_treeMotorList.DeleteAllItems();

	TV_INSERTSTRUCT tvstruct;
	HTREEITEM item[2]={NULL,NULL};//아이템 핸들러 3개 초기화
	HTREEITEM firstitem;
	for(i=0; i<clsExcel.m_nPartCnt; i++)
	{
		tvstruct.hParent				= 0;
		tvstruct.hInsertAfter			= TVI_LAST;
		tvstruct.item.pszText			= (LPWSTR)(LPCTSTR)clsExcel.m_strPartName[i];
		tvstruct.item.mask				= TVIF_TEXT | TVIF_IMAGE |TVIF_SELECTEDIMAGE ;
		tvstruct.item.iImage			= 0;
		tvstruct.item.iSelectedImage	= 0;
		item[0]							= m_treeMotorList.InsertItem(&tvstruct);

		if (i == 0)
		{
			firstitem = item[0];
		}

		for(j=0; j<clsExcel.m_nPartAxisCnt[i]; j++)
		{
			tvstruct.hParent				=	item[0];
 			tvstruct.hInsertAfter			=	TVI_LAST;
			tvstruct.item.pszText			=	(LPWSTR)(LPCTSTR)clsExcel.m_strPartAxisName[i][j];
 			tvstruct.item.mask				=	TVIF_TEXT| TVIF_IMAGE |TVIF_SELECTEDIMAGE ;
 			tvstruct.item.iImage			=	2;
 			tvstruct.item.iSelectedImage	=	1;
 			item[1]							= m_treeMotorList.InsertItem(&tvstruct);

			m_treeMotorList.EnsureVisible(item[1]);
		}
	}

	m_treeMotorList.EnsureVisible(firstitem);
}

BOOL CScreenMotor::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
/*	KillTimer(TMR_MOVE_CHECK);
	KillTimer(TMR_HOME_CHECK);
	KillTimer(TMR_STATUS_CHECK);
	KillTimer(TMR_POSITION_CHECK);
	
	if (m_pImagelist != NULL)
	{
		delete m_pImagelist;
		m_pImagelist = NULL;
	}

	if (OnDataComparison(m_nMotorPartNum, m_nMotorPartAxisNum) == RET_ERROR)
	{
		OnDataHistoryLog(m_nMotorPartNum, m_nMotorPartAxisNum);
		OnDataApply(m_nMotorPartNum, m_nMotorPartAxisNum);
	}

	st_handler_info.cWndMotor = NULL;*/

	return CFormView::DestroyWindow();
}


void CScreenMotor::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	OnInitButton();
	OnInitGroupBox();

	OnInitGridMotorPosition(m_nMotorPartNum, m_nMotorPartAxisNum);
	OnInitGridMotorStatus(m_nMotorPartNum, m_nMotorPartAxisNum);

	OnInitTree();
	OnDataInit(m_nMotorPartNum, m_nMotorPartAxisNum);

	SetTimer(TMR_STATUS_CHECK,		100, NULL);
	SetTimer(TMR_POSITION_CHECK,	100, NULL);

	st_handler_info.cWndMotor = this;
}


void CScreenMotor::OnClickedBtnEmergency()
{
	// TODO: Add your control notification handler code here
	if (!m_bEmergency)
	{
		m_bEmergency = true;
	}
}



void CScreenMotor::OnSelchangedTreeMotorList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	if(st_handler_info.nMenuLock) return;

	int i, j;
	bool bfind;
	CString strTmp;
	CString strFileName;

	strTmp = m_treeMotorList.GetItemText(pNMTreeView->itemNew.hItem);
	if (NULL == m_treeMotorList.GetParentItem(pNMTreeView->itemNew.hItem))
	{
		for(i=0; i<clsExcel.m_nPartCnt; i++)
		{
			if (strTmp == clsExcel.m_strPartName[i])
			{
				strFileName.Format(_T("d:\\AMT8562\\Bmp\\%s"), clsExcel.m_strPartInfo[i]);
				m_imgPart.Load(strFileName);
				Invalidate(FALSE);
				return;
			}
		}
	}

	bfind = false;
	for (i=0; i<clsExcel.m_nPartCnt; i++)
	{
		for (j=0; j<clsExcel.m_nPartAxisCnt[i]; j++)
		{
			if (strTmp == clsExcel.m_strPartAxisName[i][j])
			{
				bfind = true;

				if (m_nMotorPartNum != i || m_nMotorPartAxisNum != j)
				{
					if (OnDataComparison(m_nMotorPartNum, m_nMotorPartAxisNum) == RET_ERROR)
					{
						OnDataApply(m_nMotorPartNum, m_nMotorPartAxisNum);
					}

					m_nMotorPartNum		= i;
					m_nMotorPartAxisNum	= j;
					clsExcel.m_nMotorSelectPartNum = m_nMotorPartNum;

					OnInitGridMotorPosition(m_nMotorPartNum, m_nMotorPartAxisNum);
					OnInitGridMotorStatus(m_nMotorPartNum, m_nMotorPartAxisNum);

					OnDataInit(m_nMotorPartNum, m_nMotorPartAxisNum);

					switch (clsExcel.m_nAxisDirection[m_nMotorPartNum][m_nMotorPartAxisNum])
					{
						case 0:
							m_btnLeft.SetBitmaps(IDC_BTN_LEFT, IDB_LEFT_M_DN, WINDOW_DN, IDB_LEFT_M_UP, WINDOW_UP);

							m_btnRight.SetBitmaps(IDC_BTN_RIGHT, IDB_RIGHT_P_DN, WINDOW_DN, IDB_RIGHT_P_UP, WINDOW_UP);
							break;

						case 1:
							m_btnLeft.SetBitmaps(IDC_BTN_LEFT, IDB_LEFT_P_DN, WINDOW_DN,IDB_LEFT_P_UP, WINDOW_UP);

							m_btnRight.SetBitmaps(IDC_BTN_RIGHT, IDB_RIGHT_M_DN, WINDOW_DN, IDB_RIGHT_M_UP , WINDOW_UP);
							break;

						case 2:
							m_btnLeft.SetBitmaps(IDC_BTN_LEFT, IDB_UP_M_DN, WINDOW_DN, IDB_UP_M_UP, WINDOW_UP);

							m_btnRight.SetBitmaps(IDC_BTN_RIGHT, IDB_DN_P_DN, WINDOW_DN, IDB_DN_P_UP, WINDOW_UP);
							break;

						case 3:
							m_btnLeft.SetBitmaps(IDC_BTN_LEFT, IDB_DN_M_DN, WINDOW_DN, IDB_DN_M_UP, WINDOW_UP);

							m_btnRight.SetBitmaps(IDC_BTN_RIGHT, IDB_UP_P_DN, WINDOW_DN, IDB_UP_P_UP, WINDOW_UP);
							break;
					}
					m_nMoveMode = 0;
					m_dDistance	= 10.0f;
					m_nImgPos	= -1;
				}
				break;
			}
		}
		if(bfind) break;
	}
	*pResult = 0;
}


void CScreenMotor::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CFormView::OnPaint()을(를) 호출하지 마십시오.
	CDC *pDC[2];
	CRect rect;

	pDC[0] = GetDlgItem(IDC_PART_IMAGE)->GetDC();
	GetDlgItem(IDC_PART_IMAGE)->GetWindowRect(rect);
	m_imgPart.Show(pDC[0], CPoint(0, 0), CPoint(rect.Width(), rect.Height()), 0,0);

	ReleaseDC(pDC[0]);

	pDC[1] = GetDlgItem(IDC_POSITION_IMAGE)->GetDC();
	GetDlgItem(IDC_POSITION_IMAGE)->GetWindowRect(rect);
	m_imgPosition.Show(pDC[1], CPoint(0, 0), CPoint(rect.Width(), rect.Height()), 0,0);

	ReleaseDC(pDC[1]);
}

void CScreenMotor::OnImageLoad(int nMode, CString strFilename)
{
	if (nMode == 0)
	{
		m_imgPart.Load(strFilename);
	}
	else
	{
		m_imgPosition.Load(strFilename);
	}
	Invalidate(FALSE);
}

LRESULT CScreenMotor::OnUserLButtonDown(WPARAM wParam, LPARAM lParam)
{
	CString strTmp;
	CString strMsg;
	int		nRet;
	int		nMotor = clsExcel.m_nPartAxis[m_nMotorPartNum][m_nMotorPartAxisNum];

	if (wParam != IDC_BTN_LEFT && wParam != IDC_BTN_RIGHT && wParam != IDC_BTN_HOME)
	{
		return 0;
	}

	if (COMI.mn_motorbd_init_end != TRUE)
	{
		if(st_handler_info.cWndList != NULL)
		{
			strMsg = _T("First, you may initialize FAS motion board.");
			//wsprintfA(st_other_info.cNormalMsg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, st_other_info.cNormalMsg);
			clsMem.OnNormalMessageWrite(strMsg);
			st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);					
		}
		return 0;
	}
	
	if (st_handler_info.nMenuLock)
	{
		return 0;
	}
	
	switch (wParam)
	{
		case IDC_BTN_LEFT:
			if (m_nMoveMode == 0)
			{
				 nRet = CTL_Lib.Motor_SafetyCheck(0, nMotor, 0);

				if(nRet == RET_ERROR)
				{
					return 0;
				}
		
				OnButtonControl(false);	
				
				strTmp.Format(_T("[Motor] (%s)_Axis - Jog Move"), clsExcel.m_strPartAxisName[m_nMotorPartNum][m_nMotorPartAxisNum]);
				clsFunc.OnLogFileAdd(99, strTmp);
				
//				COMI.Set_MotJogSpeed(MOT_SPD_CM_CTL, nMotor, st_motor_info[nMotor].d_spd_jog[0], st_motor_info[nMotor].d_accel[0], st_motor_info[nMotor].d_decel[0]) ;
				COMI.Set_MotSpeed(MOT_SPD_CM_CTL, nMotor, 0, st_motor_info[nMotor].d_spd_jog[0], st_motor_info[nMotor].d_accel[0], st_motor_info[nMotor].d_decel[0]) ;
				if (nMotor == M_PICKERRBT_Y)
				{
					if (cmmSxVMoveStart(nMotor, PLUS) != cmERR_NONE)
					{ 
						cmmErrShowLast(GetSafeHwnd());
					}
					else
					{
						st_handler_info.nMenuLock = TRUE;
					}
				}
				else
				{
					if(cmmSxVMoveStart(nMotor, MINUS) != cmERR_NONE)
					{ 
						cmmErrShowLast(GetSafeHwnd());
					}
					else
					{
						st_handler_info.nMenuLock = TRUE;
					}
				}
				
			}
			break;

		case IDC_BTN_RIGHT:
			if(m_nMoveMode == 0)
			{
				nRet = CTL_Lib.Motor_SafetyCheck(0, nMotor, 0);

				if(nRet == RET_ERROR)
				{
					return 0;
				}

				OnButtonControl(false);						// 버튼 비활성화
				
				strTmp.Format(_T("[Motor] (%s)_Axis - Jog Move"), clsExcel.m_strPartAxisName[m_nMotorPartNum][m_nMotorPartAxisNum]);
				clsFunc.OnLogFileAdd(99, strTmp);

				COMI.Set_MotSpeed(MOT_SPD_CM_CTL, nMotor, 0, st_motor_info[nMotor].d_spd_jog[0], st_motor_info[nMotor].d_accel[0], st_motor_info[nMotor].d_decel[0]) ;	

				if (nMotor == M_PICKERRBT_Y)
				{
					if(cmmSxVMoveStart(nMotor, MINUS) != cmERR_NONE)
					{ 
						cmmErrShowLast(GetSafeHwnd());
					}
					else
					{
						st_handler_info.nMenuLock = TRUE;
					}
				}
				else
				{
					if (cmmSxVMoveStart(nMotor, PLUS) != cmERR_NONE)
					{ 
						cmmErrShowLast(GetSafeHwnd());
					}
					else
					{
						st_handler_info.nMenuLock = TRUE;
					}
				}
				/*COMI.Set_MotSpeed(MOT_SPD_CM_CTL, nMotor + 1 , 0, st_motor_info[nMotor].d_spd_jog[0], st_motor_info[nMotor].d_accel[0], st_motor_info[nMotor].d_decel[0]) ;	
				if (cmmSxVMoveStart(nMotor +1, PLUS) != cmERR_NONE)
				{ 
					cmmErrShowLast(GetSafeHwnd());
				}
				else
				{
					st_handler_info.nMenuLock = TRUE;
				}*/
			}
			break;
	}
	return 0;
}

LRESULT CScreenMotor::OnUserLButtonUp(WPARAM wParam, LPARAM lParam)
{	
	CString strMsg;
	CString strTmp;
	int		nRet;
	double  dmot_curr_pos;
	int		nMotor = clsExcel.m_nPartAxis[m_nMotorPartNum][m_nMotorPartAxisNum];

	if(wParam != IDC_BTN_LEFT && wParam != IDC_BTN_RIGHT && wParam != IDC_BTN_HOME)
	{
		return 0;
	}

	if(COMI.mn_motorbd_init_end != TRUE)
	{
		if(st_handler_info.cWndList != NULL)
		{
			strMsg = _T("First, you may initialize FAS motion board.");
			//wsprintfA(st_other_info.cNormalMsg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, st_other_info.cNormalMsg);
			st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);					
		}
		return 0;
	}
	
	
	switch(wParam)
	{
		case IDC_BTN_LEFT:
			if(m_nMoveMode == 0)
			{
				strTmp.Format(_T("[Motor] (%s)_Axis - Jog Stop"), clsExcel.m_strPartAxisName[m_nMotorPartNum][m_nMotorPartAxisNum]);
				clsFunc.OnLogFileAdd(99, strTmp);

				cmmSxStop(nMotor, FALSE, FALSE);  // 모터 이동 정지 함수 
				//cmmSxStop(nMotor + 1, FALSE, FALSE);  // 모터 이동 정지 함수 
				
				if(st_handler_info.nMenuLock)
				{
					st_handler_info.nMenuLock = FALSE;
				}
				OnButtonControl(TRUE);	
			}
			else
			{
				if(st_handler_info.nMenuLock)
				{
					return 0;
				}

				dmot_curr_pos		= COMI.Get_MotCurrentPos(nMotor);

				if (nMotor == M_PICKERRBT_Y)
				{
					m_dMotorGotoPos		= dmot_curr_pos + m_dDistance;
				}
				else
				{
					m_dMotorGotoPos		= dmot_curr_pos - m_dDistance;
				}
				

				nRet = CTL_Lib.Motor_SafetyCheck(1, nMotor, m_dMotorGotoPos);

				if(nRet == RET_ERROR)
				{
					return 0;
				}

				nRet = CTL_Lib.OnMotorGoCheck(nMotor, 0);
			
				if(nRet != RET_GOOD)
				{
					return 0;
				}

				OnButtonControl(FALSE);						// 버튼 비활성화
				OnMotorMoveStepClear();							// 모터제어 함수 스텝 초기화
				
				m_nCmdMotorNum		= nMotor;					// 모터 번호 설정
				m_nCmdNum			= MOVING;							// 실행 명령 번호 설정 [모터 이동 명령]
				m_dMotorGotoSpd		= st_handler_info.nManualSpeed;
				m_strCmdMotor		= clsExcel.m_strPartAxisName[m_nMotorPartNum][m_nMotorPartAxisNum];
				
				strTmp.Format(_T("[Motor] (%s)_Axis (%.3f) - Data Move"), clsExcel.m_strPartAxisName[m_nMotorPartNum][m_nMotorPartAxisNum], m_dMotorGotoPos);
				clsFunc.OnLogFileAdd(99, strTmp);

				if (st_handler_info.nMenuLock != TRUE)
				{
					st_handler_info.nMenuLock = TRUE;
				}

				SetTimer(TMR_MOVE_CHECK, 100, NULL);
			}
			break;
		case IDC_BTN_RIGHT:
			if(m_nMoveMode == 0)
			{
				strTmp.Format(_T("[Motor] (%s)_Axis + Jog Stop"), clsExcel.m_strPartAxisName[m_nMotorPartNum][m_nMotorPartAxisNum]);
				clsFunc.OnLogFileAdd(99, strTmp);
			
				cmmSxStop(nMotor, FALSE, FALSE);  // 모터 이동 정지 함수 
				//cmmSxStop(nMotor + 1, FALSE, FALSE);  // 모터 이동 정지 함수 

				if(st_handler_info.nMenuLock)
				{
					st_handler_info.nMenuLock = FALSE;
				}

				OnButtonControl(TRUE);	
			}
			else
			{
				if(st_handler_info.nMenuLock)
				{
					return 0;
				}

				dmot_curr_pos		= COMI.Get_MotCurrentPos(nMotor);
				if (nMotor == M_PICKERRBT_Y)
				{
					m_dMotorGotoPos		= dmot_curr_pos - m_dDistance;
				}
				else
				{
					m_dMotorGotoPos		= dmot_curr_pos + m_dDistance;
				}
				nRet = CTL_Lib.Motor_SafetyCheck(1, nMotor, m_dMotorGotoPos);

				if(nRet == RET_ERROR)
				{
					return 0;
				}

				nRet = CTL_Lib.OnMotorGoCheck(nMotor, 0);
			
				if(nRet != RET_GOOD)
				{
					return 0;
				}

				OnButtonControl(FALSE);						// 버튼 비활성화
				OnMotorMoveStepClear();							// 모터제어 함수 스텝 초기화
				
				m_nCmdMotorNum		= nMotor;					// 모터 번호 설정
				m_nCmdNum			= MOVING;							// 실행 명령 번호 설정 [모터 이동 명령]
				m_dMotorGotoSpd		= st_handler_info.nManualSpeed;
				m_strCmdMotor		= clsExcel.m_strPartAxisName[m_nMotorPartNum][m_nMotorPartAxisNum];
				
				strTmp.Format(_T("[Motor] (%s)_Axis (%.3f) + Data Move"), clsExcel.m_strPartAxisName[m_nMotorPartNum][m_nMotorPartAxisNum], m_dMotorGotoPos);
				clsFunc.OnLogFileAdd(99, strTmp);

				if (st_handler_info.nMenuLock != TRUE)
				{
					st_handler_info.nMenuLock = TRUE;
				}

				SetTimer(TMR_MOVE_CHECK, 100, NULL);
			}
			break;

		case IDC_BTN_HOME:
			nRet = CTL_Lib.Motor_SafetyCheck(0, nMotor, 0);
				
			if(nRet == RET_ERROR)
			{
				return 0;
			}

			nRet = CTL_Lib.OnMotorGoCheck(nMotor, 0);
			
			if(nRet != RET_GOOD)
			{
				return 0;
			}

			OnButtonControl(FALSE);  // 모든 컨트롤 화면 출력 제어 함수
			
			// ==============================================================================
			// 메뉴 사용 불가능하도록 플래그 설정한다                                    
			// -> 현재 모터 동작 중에 다른 메뉴 모터 동작시키면 오동작할 수 있기 때문이다
			// -> st_handler.i_menu_lock   : 메뉴 사용 가능 상태 플래그                  
			//  : 메인 프레임의 메뉴 전환 부분에서 이 플래그 검사하게 된다               
			// ==============================================================================
			if (st_handler_info.nMenuLock != TRUE)
			{
				st_handler_info.nMenuLock = TRUE;
			}
			// ==============================================================================
			
			// ==============================================================================
			// -> HOMING 작업 처리 함수는 CPublic_Function 클래스에 존재                 
			// ==============================================================================
			st_motor_info[nMotor].n_home_state	= NO;	// 모터 HOMING 완료 플래그
//			COMI.mn_homechk_flag[nMotor]		= NO;
			// ==============================================================================
			
			OnMotorMoveStepClear();							// 모터제어 함수 스텝 초기화
			
			m_nCmdMotorNum			= nMotor;					// 모터 번호 설정
			m_nCmdNum				= HOMING;							// 실행 명령 번호 설정 [모터 이동 명령]
			m_strCmdMotor			= clsExcel.m_strPartAxisName[m_nMotorPartNum][m_nMotorPartAxisNum];

			COMI.mn_home_step[nMotor]	= 0;

			strTmp.Format(_T("[Motor] (%s)_Axis Home Move"), clsExcel.m_strPartAxisName[m_nMotorPartNum][m_nMotorPartAxisNum]);
			clsFunc.OnLogFileAdd(99, strTmp);
			
			SetTimer(TMR_HOME_CHECK, 100, NULL);			// 모터 동작 제어 타이머
			break;
	}

	return 0;
}

void CScreenMotor::OnButtonControl(bool bStatus)
{
}

void CScreenMotor::OnMotorMoveStepClear()
{
	m_bEmergency	= false;	// ESTOP 요청 플래그 초기화
	
	m_nCmdNum		= 0;			// 실행 명령 번호 저장 변수 초기화
	m_nCmdMotorNum	= -1;	// 모터 번호 저장 변수 초기화
	m_strCmdMotor	= _T("Motor NULL");
	
	m_nHomeStep		= 0;		// 모터 HOMING 처리 스텝 저장 변수 초기화
	m_nMoveStep		= 0;		// 모터 이동 처리 스텝 저장 변수 초기화
}

void CScreenMotor::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	int nResponse;

	switch(nIDEvent)
	{
		case TMR_STATUS_CHECK:
			OnMotorStatusCheck();
			break;

		case TMR_POSITION_CHECK:
			OnMotorPositionCheck();
			break;

		case TMR_HOME_CHECK:
			nResponse = OnMotorHomingExcution();		// 모터 HOMING 처리 함수 //
			
			if(nResponse == RET_GOOD)
			{
				KillTimer(TMR_HOME_CHECK) ;  // 타이머 삭제 //COMI.mn_homechk_flag[nMotor]		= NO;
//				if(st_motor_info[clsExcel.m_nPartAxis[m_nMotorPartNum][m_nMotorPartAxisNum]].n_home_state != YES)
				if(COMI.mn_homechk_flag[clsExcel.m_nPartAxis[m_nMotorPartNum][m_nMotorPartAxisNum]] != YES)
				{
//					st_motor_info[clsExcel.m_nPartAxis[m_nMotorPartNum][m_nMotorPartAxisNum]].n_home_state = YES;
					COMI.mn_homechk_flag[clsExcel.m_nPartAxis[m_nMotorPartNum][m_nMotorPartAxisNum]] = YES;
				}
				
				m_nCmdNum = 0;  // 실행 명령 번호 저장 변수 초기화 //
				
				// ==============================================================================
				// 메뉴 사용 가능하도록 플래그 설정한다
				// -> st_handler.i_menu_lock   : 메뉴 사용 가능 상태 플래그
				//  : 메인 프레임의 메뉴 전환 부분에서 이 플래그 검사하게 된다
				// ==============================================================================
				if (st_handler_info.nMenuLock != FALSE)
				{
					st_handler_info.nMenuLock = FALSE;
				}
				// ==============================================================================

				OnButtonControl(true);  // 모든 컨트롤 화면 출력 제어 함수 //
			}
			else if(nResponse == RET_ERROR)
			{
				KillTimer(TMR_HOME_CHECK) ;  // 타이머 삭제 //
				
				m_nCmdNum = 0;  // 실행 명령 번호 저장 변수 초기화 //
				
				// ==============================================================================
				// 메뉴 사용 가능하도록 플래그 설정한다
				// -> st_handler.i_menu_lock   : 메뉴 사용 가능 상태 플래그
				//  : 메인 프레임의 메뉴 전환 부분에서 이 플래그 검사하게 된다
				// ==============================================================================
				if(st_handler_info.nMenuLock != FALSE)
				{
					st_handler_info.nMenuLock = FALSE;
				}
				// ==============================================================================
				OnButtonControl(true);  // 모든 컨트롤 화면 출력 제어 함수 //
			}
			break;

		case TMR_MOVE_CHECK:
			nResponse = OnMotorMovingExcution();		// 모터 HOMING 처리 함수 //
			
			if(nResponse == RET_GOOD)
			{
				KillTimer(TMR_MOVE_CHECK) ;  // 타이머 삭제 //
		
				// ==============================================================================
				// 메뉴 사용 가능하도록 플래그 설정한다
				// -> st_handler.i_menu_lock   : 메뉴 사용 가능 상태 플래그
				//  : 메인 프레임의 메뉴 전환 부분에서 이 플래그 검사하게 된다
				// ==============================================================================
				if(st_handler_info.nMenuLock != FALSE)
				{
					st_handler_info.nMenuLock = FALSE;
				}

				OnButtonControl(true);  // 모든 컨트롤 화면 출력 제어 함수 //
				if(m_nPositionMove == YES)
				{
					m_nPositionMove = NO;
					OnMotorCompletion(m_nCellRowPos, m_nCellColPos);
				}
				m_nCmdNum = 0;  // 실행 명령 번호 저장 변수 초기화 //
			}
			else if(nResponse == RET_ERROR)
			{
				KillTimer(TMR_MOVE_CHECK) ;  // 타이머 삭제 //
				
				// ==============================================================================
				// 메뉴 사용 가능하도록 플래그 설정한다
				// -> st_handler.i_menu_lock   : 메뉴 사용 가능 상태 플래그
				//  : 메인 프레임의 메뉴 전환 부분에서 이 플래그 검사하게 된다
				// ==============================================================================
				if(st_handler_info.nMenuLock != FALSE)
				{
					st_handler_info.nMenuLock = FALSE;
				}
				// ==============================================================================
				
				OnButtonControl(true);  // 모든 컨트롤 화면 출력 제어 함수 //
				if(m_nPositionMove == YES)
				{
					m_nPositionMove = NO;
					OnMotorCompletion(m_nCellRowPos, m_nCellColPos);
				}
				m_nCmdNum = 0;  // 실행 명령 번호 저장 변수 초기화 //
			}
			break;
	}
	CFormView::OnTimer(nIDEvent);
}

void CScreenMotor::OnMotorPositionCheck()
{
	if (COMI.mn_motorbd_init_end != YES)
	{
		return;
	}

	CString strTmp;

	strTmp.Format(_T("%.3f"), COMI.Get_MotCurrentPos(clsExcel.m_nPartAxis[m_nMotorPartNum][m_nMotorPartAxisNum]));
	m_pGridMotStatus.SetItemText(7, 2, strTmp);

	m_pGridMotStatus.Invalidate(FALSE);
}


void CScreenMotor::OnMotorStatusCheck()
{
	if (COMI.mn_motorbd_init_end != YES)
	{
		return;
	}

	int nMotor	= clsExcel.m_nPartAxis[m_nMotorPartNum][m_nMotorPartAxisNum];

	if (clsExcel.m_nAxisLimitCnt[m_nMotorPartNum][m_nMotorPartAxisNum] == 2)
	{
		if (OnMotorStatusCheck(nMotor, clsExcel.m_nAxisLimitInfo[m_nMotorPartNum][m_nMotorPartAxisNum][0]) == RET_GOOD)
		{
			m_pGridMotStatus.SetItemBkColour(1, 0, RED_C, WHITE_C);
		}
		else
		{
			m_pGridMotStatus.SetItemBkColour(1, 0, BLACK_L, WHITE_C);
		}

		if (OnMotorStatusCheck(nMotor, clsExcel.m_nAxisLimitInfo[m_nMotorPartNum][m_nMotorPartAxisNum][1]) == RET_GOOD)
		{
			m_pGridMotStatus.SetItemBkColour(1, 3, RED_C, WHITE_C);
		}
		else
		{
			m_pGridMotStatus.SetItemBkColour(1, 3, BLACK_L, WHITE_C);
		}
	}
	else if (clsExcel.m_nAxisLimitCnt[m_nMotorPartNum][m_nMotorPartAxisNum] == 3)
	{
		if (OnMotorStatusCheck(nMotor, clsExcel.m_nAxisLimitInfo[m_nMotorPartNum][m_nMotorPartAxisNum][0]) == RET_GOOD)
		{
			m_pGridMotStatus.SetItemBkColour(1, 0, RED_C, WHITE_C);
		}
		else
		{
			m_pGridMotStatus.SetItemBkColour(1, 0, BLACK_L, WHITE_C);
		}

		if (OnMotorStatusCheck(nMotor, clsExcel.m_nAxisLimitInfo[m_nMotorPartNum][m_nMotorPartAxisNum][1]) == RET_GOOD)
		{
			m_pGridMotStatus.SetItemBkColour(1, 2, RED_C, WHITE_C);
		}
		else
		{
			m_pGridMotStatus.SetItemBkColour(1, 2, BLACK_L, WHITE_C);
		}

		if (OnMotorStatusCheck(nMotor, clsExcel.m_nAxisLimitInfo[m_nMotorPartNum][m_nMotorPartAxisNum][2]) == RET_GOOD)
		{
			m_pGridMotStatus.SetItemBkColour(1, 4, RED_C, WHITE_C);
		}
		else
		{
			m_pGridMotStatus.SetItemBkColour(1, 4, BLACK_L, WHITE_C);
		}
	}

	if (COMI.Get_MotPower(nMotor) == RET_GOOD)
	{
		m_pGridMotStatus.SetItemBkColour(3, 0, RED_C, WHITE_C);
	}
	else
	{
		m_pGridMotStatus.SetItemBkColour(3, 0, BLACK_L, WHITE_C);
	}

	if (COMI.Get_MotAlarmStatus(nMotor) == RET_GOOD)
	{
		m_pGridMotStatus.SetItemBkColour(5, 0, BLACK_L, WHITE_C);
	}
	else
	{
		m_pGridMotStatus.SetItemBkColour(5, 0, RED_C, WHITE_C);
	}
	m_pGridMotStatus.Invalidate(FALSE);
}

int CScreenMotor::OnMotorStatusCheck(int nMotor, int nMode)
{
	int nRet;
	
	switch (nMode)
	{
		case 0:
			nRet = COMI.Get_MotIOSensor(nMotor, MOT_SENS_HOME);
			break;

		case 1:
			nRet = COMI.Get_MotIOSensor(nMotor, MOT_SENS_ELM);
			break;

		case 2:
			nRet = COMI.Get_MotIOSensor(nMotor, MOT_SENS_ELP);
			break;

		case 3:
			nRet = COMI.Get_MotIOSensor(nMotor, MOT_SENS_SD);
			break;
	}
	return nRet;
}

int	CScreenMotor::OnMotorMovingExcution()
{
	CString strMsg;
	int nRet[2];
	nRet[0] = RET_PROCEED;

	// =============================================================================
	// 모터 동작 중 ESTOP 요청 발생했는지 검사한다                               
	// -> ESTOP 요청 발생한 경우 긴급 정지로 모터 멈춘다                         
	// =============================================================================
	if (m_bEmergency)
	{
		if (m_nMoveStep < 100)
		{
			m_nMoveStep = 100;
		}
	}
	// =============================================================================

	switch(m_nMoveStep)
	{
		case 0 :
			// =============================================================================
			// 모터 특정 위치로 이동시킨다. (이동위치 세팅~)                         
			// =============================================================================
			nRet[1] = COMI.Start_SingleMove(m_nCmdMotorNum, m_dMotorGotoPos, st_handler_info.nManualSpeed);
			

			if (nRet[1] == RET_GOOD)
			{
				m_nMoveStep = 10;
			}
			else if (nRet[1] == RET_ERROR)
			{
				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
				{
					strMsg += _T("[");
					strMsg += m_strCmdMotor;
					strMsg += _T("] Motor failed to move.");

					//wsprintfA(st_other_info.cAbnormalMsg, "%S", strMsg);
					//clsFunc.OnStringToChar(strMsg, st_other_info.cAbnormalMsg);
					clsMem.OnAbNormalMessagWrite(strMsg);
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				m_nMoveStep = 130; 
			}
			else if (nRet[1] == RET_SAFETY)
			{
				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
				{
					strMsg = _T("[SAFETY] Do not move being thought that is not safe at relevant motor difference.");
					//wsprintfA(st_other_info.cAbnormalMsg, "%S", strMsg);
					//clsFunc.OnStringToChar(strMsg, st_other_info.cAbnormalMsg);
					clsMem.OnAbNormalMessagWrite(strMsg);
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				m_nMoveStep = 130;
			}
			break;
		case 10 :
			nRet[1] = COMI.Check_SingleMove(m_nCmdMotorNum, m_dMotorGotoPos);

			if (nRet[1] == RET_GOOD)
			{
				m_nMoveStep = 20;
			}
			else if (nRet[1] == RET_ERROR)
			{
				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
				{
					strMsg += _T("[");
					strMsg += m_strCmdMotor;
					strMsg += _T( "] Motor failed to move.");
					//wsprintfA(st_other_info.cAbnormalMsg, "%S", strMsg);
					//clsFunc.OnStringToChar(strMsg, st_other_info.cAbnormalMsg);
					clsMem.OnAbNormalMessagWrite(strMsg);
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				m_nMoveStep = 130; 
			}
			else if (nRet[1] == RET_SAFETY)
			{
				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
				{
					strMsg = _T("[SAFETY] Do not move being thought that is not safe at relevant motor difference.");
					//wsprintfA(st_other_info.cAbnormalMsg, "%S", strMsg);
					//clsFunc.OnStringToChar(strMsg, st_other_info.cAbnormalMsg);
					clsMem.OnAbNormalMessagWrite(strMsg);
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				m_nMoveStep = 130;
			}
			break;

		case 20 :
			if (st_handler_info.cWndList != NULL)
			{
				strMsg += _T("[");
				strMsg += m_strCmdMotor;
				strMsg += _T( "] Motor completed transfer.");
				//wsprintfA(st_other_info.cNormalMsg, "%s", strMsg);
				//clsFunc.OnStringToChar(strMsg, st_other_info.cNormalMsg);
				clsMem.OnNormalMessageWrite(strMsg);
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 완료 출력 요청
			}
			
			if (m_bEmergency)
			{
				m_bEmergency = false;  // ESTOP 요청 플래그 초기화
			}
			
			m_nMoveStep = 0;
			nRet[0] = RET_GOOD;
			break;

		case 100 :
			// =============================================================================
			// 특정 축 긴급 정지 이벤트로 모터 정지한다                              
			// =============================================================================
			nRet[1] = COMI.Set_MotStop(0, m_nCmdMotorNum);
			
			if (nRet[1] == RET_GOOD)
			{
				m_nMoveStep = 110;
			}
			else 
			{
				if (st_handler_info.cWndList != NULL)
				{  // 리스트 바 화면 존재
					strMsg += _T("[");
					strMsg += m_strCmdMotor;
					strMsg += _T("] Though canceled transfer of motor, error happened.");
					//wsprintfA(st_other_info.cAbnormalMsg, "%S", strMsg);
					//clsFunc.OnStringToChar(strMsg, st_other_info.cAbnormalMsg);
					clsMem.OnAbNormalMessagWrite(strMsg);
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				
				m_nMoveStep = 120;
			}
			// =============================================================================
			break;
		case 110 :
			if (st_handler_info.cWndList != NULL)
			{  // 리스트 바 화면 존재
				strMsg += _T("[");
				strMsg += m_strCmdMotor;
				strMsg += _T("] Motor shut down during transfer.");
				//wsprintfA(st_other_info.cNormalMsg, "%S", strMsg);
				//clsFunc.OnStringToChar(strMsg, st_other_info.cNormalMsg);
				clsMem.OnNormalMessageWrite(strMsg);
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 완료 출력 요청
			}
			
			if (m_bEmergency)
			{
				m_bEmergency = false;  // ESTOP 요청 플래그 초기화
			}
			
			m_nMoveStep = 0;
			nRet[0]		= RET_GOOD;
			break;

		case 120:
			if (st_handler_info.cWndList != NULL)
			{  // 리스트 바 화면 존재
				strMsg += _T("[");
				strMsg += m_strCmdMotor;
				strMsg += _T( "] Transfer command of motor failed.");
				//wsprintfA(st_other_info.cAbnormalMsg, "%S", strMsg);
				//clsFunc.OnStringToChar(strMsg, st_other_info.cAbnormalMsg);
				clsMem.OnAbNormalMessagWrite(strMsg);
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			
			if (m_bEmergency)
			{
				m_bEmergency = false;  // ESTOP 요청 플래그 초기화
			}
			
			m_nMoveStep = 0; 
			nRet[0]		= RET_ERROR;
			break;
			
		case 130:
			if (st_handler_info.cWndList != NULL)
			{  // 리스트 바 화면 존재
				strMsg += _T("[");
				strMsg += m_strCmdMotor;
				strMsg += _T( "] Transfer command of motor failed.");
				//wsprintfA(st_other_info.cAbnormalMsg, "%S", strMsg);
				//clsFunc.OnStringToChar(strMsg, st_other_info.cAbnormalMsg);
				clsMem.OnAbNormalMessagWrite(strMsg);
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			
			if (m_bEmergency)
			{
				m_bEmergency = false;  // ESTOP 요청 플래그 초기화
			}
			
			m_nMoveStep = 0; 
			nRet[0]		= RET_ERROR;
			break;
	}

	return nRet[0];
}


int	CScreenMotor::OnMotorHomingExcution()
{
	CString strMsg;

	int nRet[2];
	nRet[0] = RET_PROCEED;
	
	// =============================================================================
	// 모터 동작 중 ESTOP 요청 발생했는지 검사한다
	// -> ESTOP 요청 발생한 경우 긴급 정지로 모터 멈춘다
	// =============================================================================
	if (m_bEmergency)
	{
		if (m_nHomeStep < 100)
		{
			m_nHomeStep = 100;
		}
	}
	// =============================================================================

	switch(m_nHomeStep)
	{
		case 0 :
			nRet[1] = COMI.HomeCheck_Mot(m_nCmdMotorNum, COMI.mn_homecheck_method[m_nCmdMotorNum], MOT_TIMEOUT);

			if (nRet[1] == RET_GOOD)				// 정상적으로 Home Check가 끝났을 경우.
			{
				if (m_bEmergency != FALSE)
				{
					m_bEmergency = FALSE;		// ESTOP 요청 플래그 초기화
				}
//				COMI.mn_homechk_flag
//				if (st_motor_info[m_nCmdMotorNum].n_home_state != YES)
				if (COMI.mn_homechk_flag[m_nCmdMotorNum] != YES)
				{
//					st_motor_info[m_nCmdMotorNum].n_home_state = YES;
					COMI.mn_homechk_flag[m_nCmdMotorNum] = YES;
				}
				m_nHomeStep = 0; 
				nRet[0]		= RET_GOOD;
			}
			else if (nRet[1] == RET_ERROR)		// Home Check에 실패 했을 경우.
			{
				if (m_bEmergency != FALSE)
				{
					m_bEmergency = FALSE;		// ESTOP 요청 플래그 초기화
				}
				
				m_nHomeStep = 0; 
				nRet[0]		= RET_ERROR;
			}
			else if (nRet[1] == RET_SAFETY)
			{
				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
				{
					strMsg = _T("[SAFETY] Do not move being thought that is not safe at relevant motor difference.");
					//wsprintfA(st_other_info.cAbnormalMsg, "%S", strMsg);
					//clsFunc.OnStringToChar(strMsg, st_other_info.cAbnormalMsg);
					clsMem.OnAbNormalMessagWrite(strMsg);
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				m_nHomeStep = 0; 
				nRet[0]		= RET_ERROR;
			}
			break;

		case 100 :
			// =============================================================================
			// 특정 축 긴급 정지 이벤트로 모터 정지한다
			// =============================================================================
			nRet[1] = COMI.Set_MotStop(0, m_nCmdMotorNum);

			if (nRet[1] == RET_GOOD)
			{
				m_nHomeStep = 110;
			}
			else 
			{
				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
				{
//					strMsg = _T("[" + m_strCmdMotor + "] Though canceled transfer of motor, error happened.");
					strMsg += _T("[");
					strMsg += m_strCmdMotor;

					//wsprintfA(st_other_info.cAbnormalMsg, "%S", strMsg);
					//clsFunc.OnStringToChar(strMsg, st_other_info.cAbnormalMsg);
					//clsFunc.OnStringToChar(strMsg, st_other_info.cAbnormalMsg);
					clsMem.OnAbNormalMessagWrite(strMsg);
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				
				m_nHomeStep = 120;
			}
			break;

		case 110 :
			if (st_handler_info.cWndList != NULL)
			{  // 리스트 바 화면 존재 //
				strMsg += _T("[");
				strMsg += m_strCmdMotor;
				strMsg += _T("] Motor shut down during transfer.");
				//wsprintfA(st_other_info.cNormalMsg, "%S", strMsg);
				clsMem.OnNormalMessageWrite(strMsg);
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 완료 출력 요청 //
			}


			if (m_bEmergency)
			{
				m_bEmergency = false;		// ESTOP 요청 플래그 초기화
			}

			m_nHomeStep = 0;
			nRet[0]		= RET_ERROR;
			break;

		case 120 :
			if (st_handler_info.cWndList != NULL)
			{  // 리스트 바 화면 존재 //
				strMsg += _T("[");
				strMsg += m_strCmdMotor;
				strMsg += _T("] Transfer command of motor failed.");

				//wsprintfA(st_other_info.cAbnormalMsg, "%S", strMsg);
				//clsFunc.OnStringToChar(strMsg, st_other_info.cAbnormalMsg);
				clsMem.OnAbNormalMessagWrite(strMsg);
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청 //
			}

			if(m_bEmergency)
			{
				m_bEmergency = false;	// ESTOP 요청 플래그 초기화 //
			}

			st_motor_info[m_nCmdMotorNum].n_home_step = 0;

			m_nHomeStep = 0; 
			nRet[0]		= RET_ERROR;
			break;

		case 130 :
			if (st_handler_info.cWndList != NULL)
			{  // 리스트 바 화면 존재 //
				strMsg += _T("[");
				strMsg += m_strCmdMotor;
				strMsg += _T( "] Transfer command of motor failed.");

				//wsprintfA(st_other_info.cAbnormalMsg, "%S", strMsg);
				//clsFunc.OnStringToChar(strMsg, st_other_info.cAbnormalMsg);
				clsMem.OnAbNormalMessagWrite(strMsg);
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청 //
			}

			if(m_bEmergency)
			{
				m_bEmergency = FALSE;	// ESTOP 요청 플래그 초기화 //
			}

			st_motor_info[m_nCmdMotorNum].n_home_step = 0;
			m_nHomeStep = 0; 
			nRet[0]		= RET_ERROR;
			break;
	}
	return nRet[0];
}


void CScreenMotor::OnMotorCompletion(int nRow, int nCol)
{
	if(nCol == 3)
	{
		m_pGridMotPosition.SetItemText(nRow, nCol, _T("Read"));
		m_pGridMotPosition.SetItemBkColour(nRow, nCol, SKY_C, ORANGE_C);
	}
	else
	{
		m_pGridMotPosition.SetItemText(nRow, nCol, _T("Go."));
		m_pGridMotPosition.SetItemBkColour(nRow, nCol, SKY_C, ORANGE_C);
	}

	m_pGridMotPosition.Invalidate(FALSE);
}


void CScreenMotor::OnMotorCellClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	CDialog_Message dlgMsg;
	CDialog_KeyPad dlgKeypad;
	int nRow, nCol;
	int nMotor;
	int nResponse;
//	double dKey;

	double value;

	CString strTmp;
	CString strMsg;
	CString strFileName;
	CString strMotorName;
	CString strPosName;
	CString strImgName;

	NM_GRIDVIEW* pItem	= (NM_GRIDVIEW*) pNotifyStruct;

	nRow				= pItem->iRow;
	nCol				= pItem->iColumn;

	if(nRow < 2) return ; 

	m_nCellRowPos = nRow;
	m_nCellColPos = nCol;

	nMotor			= clsExcel.m_nPartAxis[m_nMotorPartNum][m_nMotorPartAxisNum];
	strMotorName	= clsExcel.m_strPartAxisName[m_nMotorPartNum][m_nMotorPartAxisNum];
	strPosName		= clsExcel.m_strAxisPosName[m_nMotorPartNum][m_nMotorPartAxisNum][nRow-2];
	strImgName		= clsExcel.m_strAxisPosInfo[m_nMotorPartNum][m_nMotorPartAxisNum][nRow-2];

	switch (nCol)
	{
		case 0:
			strTmp = m_pGridMotPosition.GetItemText(nRow, 0);
			
			if (strTmp == _T("")) return;

			strFileName.Format(_T("d:\\AMT8562\\Bmp\\%s"), strImgName);
			m_imgPosition.Load(strFileName);
				
			if(m_nImgPos != -1)
			{
				m_pGridMotPosition.SetItemBkColour(nRow, 0, YELLOW_L, BLACK_L);
				m_pGridMotPosition.SetItemBkColour(m_nImgPos, 0, RGB(79, 79, 225), WHITE_C);

				m_nImgPos = nRow;
			}
			else
			{
				m_pGridMotPosition.SetItemBkColour(nRow, 0, YELLOW_L, BLACK_L);

				m_nImgPos = nRow;
			}
			Invalidate(FALSE);
			break;

		case 1:
// 			dKey = m_dMotorPos[nRow-2][1];
// 			KeyPadD(st_motor_info[nMotor].d_limit_position[0], 5, &dKey);
// 
// 			m_dMotorPos[nRow-2][1] = dKey;
// 
// 			strTmp.Format(_T("%.3f"), dKey);
// 			m_pGridMotPosition.SetItemText(nRow, st_motor_info[nMotor].d_limit_position[1], strTmp);
// 
// 			strTmp.Format(_T("[Motor] (%s)_Axis (%s)_Position [%.3f] Change"), strMotorName, 
// 				strPosName,
// 				m_dMotorPos[nRow - 2][1]);
			//clsFunc.OnLogFileAdd(99, strTmp);

			dlgKeypad.m_nKeypadMode		= 1;
			dlgKeypad.m_strKeypadVal = m_pGridMotPosition.GetItemText(nRow, 1);

			dlgKeypad.m_strKeypadHighLimit.Format(_T("%.3f"), st_motor_info[nMotor].d_limit_position[1]);
			dlgKeypad.m_strKeypadLowLimit.Format(_T("%.3f"), st_motor_info[nMotor].d_limit_position[0]);

			if (dlgKeypad.DoModal() == IDOK)
			{
				strTmp					= dlgKeypad.m_strNewVal; //m_strKeypadVal;
				m_dMotorPos[nRow-2][1]	= _wtof(strTmp);

				m_pGridMotPosition.SetItemText(nRow, 1, strTmp);

				strTmp.Format(_T("[Motor] (%s)_Axis (%s)_Position [%.3f] Change"), strMotorName, 
					strPosName,
					m_dMotorPos[nRow - 2][1]);
				clsFunc.OnLogFileAdd(99, strTmp);
			}
			break;

		case 2:
			if (COMI.mn_motorbd_init_end != YES)  
			{
				dlgMsg.m_nMessageType	= 0;
				dlgMsg.m_strMessage		= _T("First Init Motion board try please.");
				dlgMsg.DoModal();
				return;
			}

			dlgMsg.m_nMessageType	= 1;
			dlgMsg.m_strMessage.Format(_T("Do you want to really move into this position?"));

			nResponse = (int)dlgMsg.DoModal();

			if (nResponse == IDOK)
			{
				m_pGridMotPosition.SetItemText(nRow, 2, _T("Wait.."));
				m_pGridMotPosition.SetItemBkColour(nRow, 2, NOR_C, BLACK_C);

				m_dMotorGotoPos = m_dMotorPos[nRow - 2][1];

				nResponse = CTL_Lib.OnMotorGoCheck(nMotor, m_dMotorGotoPos);

				if(nResponse != RET_GOOD)
				{
					OnMotorCompletion(m_nCellRowPos, m_nCellColPos);
					return;
				}

				if(m_dMotorGotoPos < COMI.md_limit_position[nMotor][0])
				{
					OnMotorCompletion(m_nCellRowPos, m_nCellColPos);
					return;
				}

				if (st_handler_info.nMenuLock != TRUE)
				{
					st_handler_info.nMenuLock = TRUE;
				}
				// =============================================================================

				OnMotorMoveStepClear();				// 모터 동작 처리 스텝 초기화 함수

				m_nCmdMotorNum		= nMotor;					// 모터 번호 설정
				m_nCmdNum			= POS_MOVING;	
				m_dMotorGotoSpd		= st_handler_info.nManualSpeed;

				m_strCmdMotor		= strMotorName;
				m_nPositionMove		= YES;

				strTmp.Format(_T("[Motor] (%s)_Axis (%s)_Position [%.3f] Change"), strMotorName, 
					strPosName,
					m_dMotorPos[nRow - 2][1]);
				clsFunc.OnLogFileAdd(99, strTmp);

				SetTimer(TMR_MOVE_CHECK, 10, NULL);		// 모터 동작 제어 타이머
			}
			break;

		case 3:
			if(COMI.mn_motorbd_init_end != YES)  
			{
				dlgMsg.m_nMessageType	= 0;
				dlgMsg.m_strMessage		= _T("First Init Motion board try please.");
				
				dlgMsg.DoModal();

				return;
			}

			value = COMI.Get_MotCurrentPos(clsExcel.m_nPartAxis[m_nMotorPartNum][m_nMotorPartAxisNum]);  // FASTECH 모터 특정 축의 커맨트 위치 리턴 함수 //
			
			dlgMsg.m_nMessageType	= 1;
			dlgMsg.m_strMessage.Format(_T("The current value of the input is 3, you want to set the value of this position to this value?"), value);
				
			nResponse = (int)dlgMsg.DoModal();

			if(nResponse == IDOK)
			{
				m_dMotorPos[nRow - 2][1] = value;
				strTmp.Format(_T("%.3f"), m_dMotorPos[nRow - 2][1]);
				m_pGridMotPosition.SetItemText(nRow, 1, strTmp);	
			}

			strTmp.Format(_T("[Motor] (%s)_Axis (%s)_Position [%.3f] Change"), strMotorName,
																			   strPosName, 
																			   m_dMotorPos[nRow - 2][1]);
			clsFunc.OnLogFileAdd(99, strTmp);

			OnMotorCompletion(m_nCellRowPos, m_nCellColPos);
			break;
	}
	m_pGridMotPosition.Refresh();
}


void CScreenMotor::OnStatusCellClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	CDialog_Message dlgMsg;

	int nRow, nCol;
	int nMotor;

	double dKey;

	CString strTmp;
	CString strMsg;
	CString strFileName;
	CString strMotorName;
	CString strPosName;

	NM_GRIDVIEW* pItem	= (NM_GRIDVIEW*) pNotifyStruct;

	nRow				= pItem->iRow;
	nCol				= pItem->iColumn;

	nMotor			= clsExcel.m_nPartAxis[m_nMotorPartNum][m_nMotorPartAxisNum];
	strMotorName	= clsExcel.m_strPartAxisName[m_nMotorPartNum][m_nMotorPartAxisNum];
	strPosName		= clsExcel.m_strAxisPosName[m_nMotorPartNum][m_nMotorPartAxisNum][nRow-2];

	switch(nRow)
	{
		case 2:
		case 3:
			switch(nCol)
			{
				case 2:
				case 3:
					if(COMI.mn_motorbd_init_end != YES)  
					{
						dlgMsg.m_nMessageType	= 0;
						dlgMsg.m_strMessage		= _T("First Init Motion board try please.");

						dlgMsg.DoModal();

						return;
					}
					strTmp.Format(_T("[Motor] (%s)_Axis Power Off"), strMotorName);
					clsFunc.OnLogFileAdd(99, strTmp);
						
					COMI.Set_MotPower(nMotor, FALSE);
					break;

				case 4:
				case 5:
					if (COMI.mn_motorbd_init_end != YES)  
					{
						dlgMsg.m_nMessageType	= 0;
						dlgMsg.m_strMessage		= _T("First Init Motion board try please.");

						dlgMsg.DoModal();

						return;
					}
					strTmp.Format(_T("[Motor] (%s)_Axis Power Off"), strMotorName);
					clsFunc.OnLogFileAdd(99, strTmp);
						
					COMI.Set_MotPower(nMotor, TRUE);
					break;
			}
			break;

		case 4:
		case 5:
			if (nCol == 2 || nCol == 3 || nCol == 4 || nCol == 5)
			{
				if(COMI.mn_motorbd_init_end != YES)  
				{
					dlgMsg.m_nMessageType	= 0;
					dlgMsg.m_strMessage		= _T("First Init Motion board try please.");

					dlgMsg.DoModal();

					return;
				}

				strTmp.Format(_T("[Motor] (%s)_Axis Alarm Clear"), strMotorName);
				clsFunc.OnLogFileAdd(99, strTmp);
					
				COMI.Set_MotAlarmClear(nMotor);
			}
			break;

		case 7:
			switch(nCol)
			{
				case 0:
					if(m_nMoveMode != 0)
					{
						m_nMoveMode = 0;

						m_pGridMotStatus.SetItemBkColour(7, 0, GREEN_C, BLACK_C);
						m_pGridMotStatus.SetItemBkColour(7, 1, BLACK_L, YELLOW_C);
					}
					break;

				case 1:
					if(m_nMoveMode != 1)
					{
						m_nMoveMode = 1;

						m_pGridMotStatus.SetItemBkColour(7, 0, BLACK_L, YELLOW_C);
						m_pGridMotStatus.SetItemBkColour(7, 1, GREEN_C, BLACK_C);
					}
					break;

				case 4:
				case 5:
					dKey = m_dDistance;

					KeyPadD(.0f, 200.0f, &dKey);

					m_dDistance = dKey;
					strTmp.Format(_T("%0.2f"), dKey);
					
					m_pGridMotStatus.SetItemText(7, 4, strTmp);
					break;

			}
			break;
	}

	m_pGridMotStatus.Invalidate(FALSE);
}

void CScreenMotor::OnDataHistoryLog(int nPartNum, int nAxisNum)
{
	CString strTmp;
	int i;

	for(i=0; i<clsExcel.m_nAxisPosNum[nPartNum][nAxisNum]; i++)
	{
		if(m_dMotorPos[i][0] != m_dMotorPos[i][1])
		{
			strTmp.Format(_T("[%s] Axis [%s] Position Change %.3f -> %.3f"), clsExcel.m_strPartAxisName[nPartNum][nAxisNum], 
																			 clsExcel.m_strAxisPosName[nPartNum][nAxisNum][i], 
																			m_dMotorPos[i][0], m_dMotorPos[i][1]);
			clsFunc.OnLogFileAdd(0, strTmp);
		}
	}
}

void CScreenMotor::OnDataRecovery(int nPartNum, int nAxisNum)
{
	int i;

	for(i=0; i<clsExcel.m_nAxisPosNum[nPartNum][nAxisNum]; i++)
	{
		m_dMotorPos[i][1]	= m_dMotorPos[i][0];
	}
}


int	CScreenMotor::OnDataComparison(int nPartNum, int nAxisNum)
{
	int i;

	for(i=0; i<clsExcel.m_nAxisPosNum[nPartNum][nAxisNum]; i++)
	{
		if(m_dMotorPos[i][0] != m_dMotorPos[i][1])
		{
			return RET_ERROR;
		}
	}

	return RET_GOOD;
}


void CScreenMotor::OnDataApply(int nPartNum, int nAxisNum)
{
	int i;
	int nMotor = clsExcel.m_nPartAxis[nPartNum][nAxisNum];
	
	for(i=0; i<clsExcel.m_nAxisPosNum[nPartNum][nAxisNum]; i++)
	{
		st_motor_info[nMotor].d_pos[i]	= m_dMotorPos[i][1];
	}
}


void CScreenMotor::OnDataBackup(int nPartNum, int nAxisNum)
{
	int i;

	for(i=0; i<clsExcel.m_nAxisPosNum[nPartNum][nAxisNum]; i++)
	{
		m_dMotorPos[i][0]	= m_dMotorPos[i][1];
	}
}


void CScreenMotor::OnDataInit(int nPartNum, int nAxisNum)
{
	CString strTmp;
	int i;
	int nMotor = clsExcel.m_nPartAxis[nPartNum][nAxisNum];
	
	for(i=0; i<clsExcel.m_nAxisPosNum[nPartNum][nAxisNum]; i++)
	{
		m_dMotorPos[i][1] = st_motor_info[nMotor].d_pos[i];

		strTmp.Format(_T("%.3f"), m_dMotorPos[i][1]);
		m_pGridMotPosition.SetItemText(i + 2, 1, strTmp);
	}

	OnDataBackup(nPartNum, nAxisNum);
}

BOOL CScreenMotor::OnEraseBkgnd(CDC* pDC)
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


void CScreenMotor::OnBnClickedBtnApply()
{
	CDialog_Message		dlgMsg;
	int					nResponse;

	dlgMsg.m_nMessageType	= 1;
	dlgMsg.m_strMessage.Format(_T("[%d]Axis Data\n Do you want a apply?"), m_nMotorPartAxisNum);

	nResponse =  (int)dlgMsg.DoModal();

	if (nResponse == IDOK)
	{
		OnDataApply(m_nMotorPartNum, m_nMotorPartAxisNum);
	}
}


void CScreenMotor::OnBnClickedBtnPartIo()
{
	CDialog_Part_IO 	dlgPartIO;
	int	nResponse;

	nResponse =  (int)dlgPartIO.DoModal();

	if (nResponse == IDOK)
	{

	}
}

void CScreenMotor::OnBnClickedBtnManualFunction()
{
	CDialogManualIO dlgIO;
	dlgIO.DoModal();
}


void CScreenMotor::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	/*	KillTimer(TMR_MOVE_CHECK);
	KillTimer(TMR_HOME_CHECK);
	KillTimer(TMR_STATUS_CHECK);
	KillTimer(TMR_POSITION_CHECK);
	
	if (m_pImagelist != NULL)
	{
		delete m_pImagelist;
		m_pImagelist = NULL;
	}

	if (OnDataComparison(m_nMotorPartNum, m_nMotorPartAxisNum) == RET_ERROR)
	{
		OnDataHistoryLog(m_nMotorPartNum, m_nMotorPartAxisNum);
		OnDataApply(m_nMotorPartNum, m_nMotorPartAxisNum);
	}

	st_handler_info.cWndMotor = NULL;*/
	CFormView::PostNcDestroy();
}


void CScreenMotor::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	KillTimer(TMR_MOVE_CHECK);
	KillTimer(TMR_HOME_CHECK);
	KillTimer(TMR_STATUS_CHECK);
	KillTimer(TMR_POSITION_CHECK);
	
	if (st_handler_info.nMenuLock != FALSE) return;
	st_handler_info.nMenuLock = FALSE;

	if (m_pIO != NULL)
	{
		m_pIO->DestroyWindow();

		delete m_pIO;
		m_pIO = NULL;
	}

	if (m_pImagelist != NULL)
	{
		delete m_pImagelist;
		m_pImagelist = NULL;
	}

	if (OnDataComparison(m_nMotorPartNum, m_nMotorPartAxisNum) == RET_ERROR)
	{
		OnDataHistoryLog(m_nMotorPartNum, m_nMotorPartAxisNum);
		OnDataApply(m_nMotorPartNum, m_nMotorPartAxisNum);
	}

	if (st_handler_info.nMenuLock != FALSE)
	{
		st_handler_info.nMenuLock = FALSE;
	}

	st_handler_info.cWndMotor = NULL;
}


void CScreenMotor::OnBnClickedBtnManualIo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	CDialogManualIO dlgIO;
//	dlgIO.DoModal();
// 	if (st_handler_info.cWndMotor != NULL)
// 	{
// 		st_handler_info.cWndMotor->SendMessage(WM_WORK_COMMAND, MOTOR_IO_CREATE_REQ, 0);
// 	}
}


LRESULT CScreenMotor::OnMotorWorkInfoCommand(WPARAM wParam, LPARAM lParam) 
{
	switch (wParam)
	{
		case MOTOR_IO_CREATE_REQ:
			if (m_pIO == NULL)
			{
				m_pIO			= new CDialogManualIO;

				m_pIO->Create();
				m_pIO->ShowWindow(SW_SHOW);
			}
			else
			{
				m_pIO->ShowWindow(SW_SHOW);
			}
			break;

		case MOTOR_IO_DELETE_REQ:
			if (m_pIO != NULL)
			{
				m_pIO->DestroyWindow();

				delete m_pIO;
				m_pIO = NULL;
			}
			break;
	}

	return 0;
}

void CScreenMotor::OnBnClickedBtnLeftUnclamp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
