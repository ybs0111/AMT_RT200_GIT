// DialogAlarmMotor.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Handler.h"
#include "DialogAlarmMotor.h"
#include "afxdialogex.h"
#include "Variable.h"
#include "PublicFunction.h"
#include "ComizoaPublic.h"
#include "Dialog_Message.h"


// CDialogAlarmMotor 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogAlarmMotor, CDialog)

CDialogAlarmMotor::CDialogAlarmMotor(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogAlarmMotor::IDD, pParent)
{
	m_btmBk.LoadBitmap(IDB_BITMAP_BACK_GROUND);  
}

CDialogAlarmMotor::~CDialogAlarmMotor()
{
}

void CDialogAlarmMotor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM_MOTOR_STATUS, m_pGridMotStatus);
	DDX_Control(pDX, IDC_BTN_EXIT,		m_btnExit);
}


BEGIN_MESSAGE_MAP(CDialogAlarmMotor, CDialog)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_EXIT, &CDialogAlarmMotor::OnClickedBtnExit)
	ON_NOTIFY(NM_CLICK, IDC_CUSTOM_MOTOR_STATUS, OnStatusCellClick)
END_MESSAGE_MAP()


// CDialogAlarmMotor 메시지 처리기입니다.

BOOL CDialogAlarmMotor::Create()
{
	return CDialog::Create( CDialogAlarmMotor::IDD );
}

void CDialogAlarmMotor::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	OnMotorStatusCheck();
	CDialog::OnTimer(nIDEvent);
}


BOOL CDialogAlarmMotor::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	KillTimer(100);

	return CDialog::DestroyWindow();
}


BOOL CDialogAlarmMotor::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	OnInitGridMotorStatus();
	OnInitButton();

	SetTimer(100, 100, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CDialogAlarmMotor::OnEraseBkgnd(CDC* pDC)
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

	return TRUE;
}


void CDialogAlarmMotor::OnInitGridMotorStatus()
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

	row = 0;
	m_pGridMotStatus.MergeCells(row, 0, 1, 5);
	m_pGridMotStatus.SetItemBkColour(row, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridMotStatus.SetItemFont(row, 0, &clsFunc.OnLogFont(20));
	m_pGridMotStatus.SetItemText(row, 0, m_strAxisName);

	if (m_nAxisLimitCnt == 2)
	{
		m_pGridMotStatus.UnMergeCells(2, 0, 0, 1);
		m_pGridMotStatus.UnMergeCells(2, 2, 0, 3);
		m_pGridMotStatus.UnMergeCells(2, 4, 0, 5);

		m_pGridMotStatus.UnMergeCells(3, 0, 1, 1);
		m_pGridMotStatus.UnMergeCells(3, 2, 1, 3);
		m_pGridMotStatus.UnMergeCells(3, 4, 1, 5);

		row = 2;
		m_pGridMotStatus.MergeCells(row, 0, row, 2);
		m_pGridMotStatus.SetItemBkColour(row, 0, RGB(79, 79, 225), WHITE_C);
		m_pGridMotStatus.SetItemFont(row, 0, &clsFunc.OnLogFont(20));

		m_pGridMotStatus.MergeCells(row, 3, row, 5);
		m_pGridMotStatus.SetItemBkColour(row, 3, RGB(79, 79, 225), WHITE_C);
		m_pGridMotStatus.SetItemFont(row, 3, &clsFunc.OnLogFont(20));

		row = 3;
		m_pGridMotStatus.MergeCells(row, 0, row, 2);
		m_pGridMotStatus.SetItemBkColour(row, 0, BLACK_L, WHITE_C);
		m_pGridMotStatus.SetItemFont(row, 0, &clsFunc.OnLogFont(20));

		m_pGridMotStatus.MergeCells(row, 3, row, 5);
		m_pGridMotStatus.SetItemBkColour(row, 3, BLACK_L, WHITE_C);
		m_pGridMotStatus.SetItemFont(row, 3, &clsFunc.OnLogFont(20));

		for(i=0; i<2; i++)
		{
			switch (m_nAxisLimitInfo[i])
			{
				case 0:
					m_pGridMotStatus.SetItemText(2, i * 3, _T("HOME"));
					break;

				case 1:
					m_pGridMotStatus.SetItemText(2, i * 3, _T("ML"));
					break;

				case 2:
					m_pGridMotStatus.SetItemText(2, i * 3, _T("PL"));
					break;

				case 3:
					m_pGridMotStatus.SetItemText(2, i * 3, _T("SD"));
					break;
			}
		}

	}
	else if (m_nAxisLimitCnt == 3)
	{
		m_pGridMotStatus.UnMergeCells(2, 0, 0, 2);
		m_pGridMotStatus.UnMergeCells(2, 3, 0, 5);

		m_pGridMotStatus.UnMergeCells(3, 0, 1, 2);
		m_pGridMotStatus.UnMergeCells(3, 3, 1, 5);

		row = 2;
		m_pGridMotStatus.MergeCells(row, 0, row, 1);
		m_pGridMotStatus.SetItemBkColour(row, 0, RGB(79, 79, 225), WHITE_C);
		m_pGridMotStatus.SetItemFont(row, 0, &clsFunc.OnLogFont(20));

		m_pGridMotStatus.MergeCells(row, 2, row, 3);
		m_pGridMotStatus.SetItemBkColour(row, 2, RGB(79, 79, 225), WHITE_C);
		m_pGridMotStatus.SetItemFont(row, 2, &clsFunc.OnLogFont(20));

		m_pGridMotStatus.MergeCells(row, 4, row, 5);
		m_pGridMotStatus.SetItemBkColour(row, 4, RGB(79, 79, 225), WHITE_C);
		m_pGridMotStatus.SetItemFont(row, 4, &clsFunc.OnLogFont(20));

		row = 3;
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
			switch (m_nAxisLimitInfo[i])
			{
				case 0:
					m_pGridMotStatus.SetItemText(2, i * 2, _T("HOME"));
					break;

				case 1:
					m_pGridMotStatus.SetItemText(2, i * 2, _T("ML"));
					break;

				case 2:
					m_pGridMotStatus.SetItemText(2, i * 2, _T("PL"));
					break;

				case 3:
					m_pGridMotStatus.SetItemText(2, i * 2, _T("SD"));
					break;
			}
		}
	}

	row = 4;
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

	row = 5;
	m_pGridMotStatus.MergeCells(row, 0, row, 1);
	m_pGridMotStatus.SetItemBkColour(row, 0, BLACK_L, WHITE_C);
	m_pGridMotStatus.SetItemFont(row, 0, &clsFunc.OnLogFont(18));
	m_pGridMotStatus.SetItemText(row, 0, _T(""));

	row = 6;
	m_pGridMotStatus.MergeCells(row, 0, row, 1);
	m_pGridMotStatus.SetItemBkColour(row, 0, RGB(79, 79, 225), WHITE_C);
	m_pGridMotStatus.SetItemFont(row, 0, &clsFunc.OnLogFont(18));
	m_pGridMotStatus.SetItemText(row, 0, _T("Motor Alarm"));

	m_pGridMotStatus.MergeCells(row, 2, row + 1, 5);
	m_pGridMotStatus.SetItemBkColour(row, 2, RED_C, BLACK_C);
	m_pGridMotStatus.SetItemFont(row, 2, &clsFunc.OnLogFont(20));
	m_pGridMotStatus.SetItemText(row, 2, _T("Alarm Reset"));

	row = 7;
	m_pGridMotStatus.MergeCells(row, 0, row, 1);
	m_pGridMotStatus.SetItemBkColour(row, 0, BLACK_L, WHITE_C);
	m_pGridMotStatus.SetItemFont(row, 0, &clsFunc.OnLogFont(20));
	m_pGridMotStatus.SetItemText(row, 0, _T(""));
/*
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
	m_pGridMotStatus.SetItemText(row, 4, _T("10.0"));*/
}


void CDialogAlarmMotor::OnInitButton()
{
	m_btnExit.SetBitmaps(IDC_BTN_EXIT, IDB_BITMAP_EXIT, WINDOW_DN, IDB_BITMAP_EXIT, WINDOW_UP);
	m_btnExit.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnExit.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnExit.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnExit.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnExit.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnExit.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnExit.SetFont(clsFunc.m_pFont[3]);
	m_btnExit.SetTooltipText(_T("Exit"));
}

void CDialogAlarmMotor::OnClickedBtnExit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (st_handler_info.cWndAlarm != NULL)
	{
		st_handler_info.cWndAlarm->SendMessage(WM_WORK_COMMAND, ALARM_MOTOR_DELETE_REQ, 0);
	}
//	CDialog::OnOK();
}


void CDialogAlarmMotor::OnMotorStatusCheck()
{
	if (COMI.mn_motorbd_init_end != YES)
	{
		return;
	}

	int nMotor	= m_nAxis;

	if (m_nAxisLimitCnt == 2)
	{
		if (OnMotorStatusCheck(nMotor, m_nAxisLimitInfo[0]) == RET_GOOD)
		{
			m_pGridMotStatus.SetItemBkColour(1, 0, RED_C, WHITE_C);
		}
		else
		{
			m_pGridMotStatus.SetItemBkColour(1, 0, BLACK_L, WHITE_C);
		}

		if (OnMotorStatusCheck(nMotor, m_nAxisLimitInfo[1]) == RET_GOOD)
		{
			m_pGridMotStatus.SetItemBkColour(1, 3, RED_C, WHITE_C);
		}
		else
		{
			m_pGridMotStatus.SetItemBkColour(1, 3, BLACK_L, WHITE_C);
		}
	}
	else if (m_nAxisLimitCnt == 3)
	{
		if (OnMotorStatusCheck(nMotor, m_nAxisLimitInfo[0]) == RET_GOOD)
		{
			m_pGridMotStatus.SetItemBkColour(1, 0, RED_C, WHITE_C);
		}
		else
		{
			m_pGridMotStatus.SetItemBkColour(1, 0, BLACK_L, WHITE_C);
		}

		if (OnMotorStatusCheck(nMotor, m_nAxisLimitInfo[1]) == RET_GOOD)
		{
			m_pGridMotStatus.SetItemBkColour(1, 2, RED_C, WHITE_C);
		}
		else
		{
			m_pGridMotStatus.SetItemBkColour(1, 2, BLACK_L, WHITE_C);
		}

		if (OnMotorStatusCheck(nMotor, m_nAxisLimitInfo[2]) == RET_GOOD)
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


int CDialogAlarmMotor::OnMotorStatusCheck(int nMotor, int nMode)
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

void CDialogAlarmMotor::OnStatusCellClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	CDialog_Message dlgMsg;

	int nRow, nCol;
	int nMotor;

	CString strTmp;
	CString strMsg;

	NM_GRIDVIEW* pItem	= (NM_GRIDVIEW*) pNotifyStruct;

	nRow				= pItem->iRow;
	nCol				= pItem->iColumn;

	nMotor				= m_nAxis;

	switch(nRow)
	{
		case 4:
		case 5:
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
					strTmp.Format(_T("[Motor] (%s)_Axis Power Off"), m_strAxisName);
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
					strTmp.Format(_T("[Motor] (%s)_Axis Power Off"), m_strAxisName);
					clsFunc.OnLogFileAdd(99, strTmp);
						
					COMI.Set_MotPower(nMotor, TRUE);
					break;
			}
			break;

		case 6:
		case 7:
			if (nCol == 2 || nCol == 3 || nCol == 4 || nCol == 5)
			{
				if(COMI.mn_motorbd_init_end != YES)  
				{
					dlgMsg.m_nMessageType	= 0;
					dlgMsg.m_strMessage		= _T("First Init Motion board try please.");

					dlgMsg.DoModal();

					return;
				}

				strTmp.Format(_T("[Motor] (%s)_Axis Alarm Clear"), m_strAxisName);
				clsFunc.OnLogFileAdd(99, strTmp);
					
				COMI.Set_MotAlarmClear(nMotor);
			}
			break;
	}

	m_pGridMotStatus.Invalidate(FALSE);
}