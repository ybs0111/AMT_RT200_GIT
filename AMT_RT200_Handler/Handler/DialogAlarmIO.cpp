// DialogAlarmIO.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Handler.h"
#include "DialogAlarmIO.h"
#include "afxdialogex.h"
#include "Variable.h"
#include "PublicFunction.h"
#include "FastechPublic_IO.h"


// CDialogAlarmIO 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogAlarmIO, CDialog)

CDialogAlarmIO::CDialogAlarmIO(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogAlarmIO::IDD, pParent)
{
	m_btmBk.LoadBitmap(IDB_BITMAP_BACK_GROUND);   
}

CDialogAlarmIO::~CDialogAlarmIO()
{
}

void CDialogAlarmIO::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM_INPUT,	m_pGridInput);
	DDX_Control(pDX, IDC_CUSTOM_OUTPUT,	m_pGridOutput);
	DDX_Control(pDX, IDC_BTN_EXIT,		m_btnExit);
}


BEGIN_MESSAGE_MAP(CDialogAlarmIO, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_EXIT, &CDialogAlarmIO::OnClickedBtnExit)
	ON_NOTIFY(NM_CLICK, IDC_CUSTOM_OUTPUT, OnCellOutputClick)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDialogAlarmIO 메시지 처리기입니다.

BOOL CDialogAlarmIO::Create()
{
	return CDialog::Create( CDialogAlarmIO::IDD );
}


BOOL CDialogAlarmIO::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	KillTimer(100);

	return CDialog::DestroyWindow();
}


BOOL CDialogAlarmIO::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	OnInitGridInput();
	OnInitGridOutput();
	OnInitButton();

	SetTimer(100, 500, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDialogAlarmIO::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	OnInputCheck();
	OnOutputCheck();

	CDialog::OnTimer(nIDEvent);
}


void CDialogAlarmIO::OnClickedBtnExit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KillTimer(100);

	if (st_handler_info.cWndAlarm != NULL)
	{
		st_handler_info.cWndAlarm->SendMessage(WM_WORK_COMMAND, ALARM_IO_DELETE_REQ, 0);
	}
//	CDialog::OnOK();
}

void CDialogAlarmIO::OnInitButton()
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


void CDialogAlarmIO::OnInitGridInput()
{
	CString strTmp;
	int i, j;

	m_pGridInput.SetFrameFocusCell(FALSE);
	m_pGridInput.SetTrackFocusCell(FALSE);
	m_pGridInput.EnableSelection(FALSE);

	m_pGridInput.SetGridLineColor(BLACK_C);
	m_pGridInput.SetGridLines(1);

	m_pGridInput.SetRowCount(17);
	m_pGridInput.SetColumnCount(2);

	m_pGridInput.SetFixedRowCount(0);
	m_pGridInput.SetFixedColumnCount(0);
	m_pGridInput.SetFixedBkColor(RGB(0,0,200));
	m_pGridInput.SetFixedBkColor(RGB(200,200,255));
	m_pGridInput.SetTextBkColor(RGB(150,150,200));

	for (i=0; i<17; i++) 
	{
		m_pGridInput.SetRowHeight(i, 30);

		for (j=0; j<2; j++) 
		{
			switch (j)
			{
				case 0:
					m_pGridInput.SetColumnWidth(j, 50);
					break;

				case 1:
					m_pGridInput.SetColumnWidth(j, 300);
					break;
			}

			m_pGridInput.SetItemFormat(i, j,	DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_pGridInput.SetItemState(i, j,		GVIS_READONLY);
			m_pGridInput.SetItemBkColour(i, j,	 BLACK_L, WHITE_C);
		}
	}

	// IO MAP Header ////////////////////////////////////////////////////////////////////////////////////////////// 
 	m_pGridInput.MergeCells(0, 0, 0, 1);
	m_pGridInput.SetItemBkColour(0, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridInput.SetItemFont(0, 0, &clsFunc.OnLogFont(24));
	m_pGridInput.SetItemText(0, 0, _T("Alarm Input Check"));

	if (m_nInCnt < 0) return;

	for (i=0; i<m_nInCnt; i++) 
	{
		m_pGridInput.SetItemText(i + 1, 0, _T("OFF"));
		m_pGridInput.SetItemBkColour(i + 1, 0, YELLOW_D, BLACK_C);

		m_pGridInput.SetItemBkColour(i + 1, 1, RGB(255, 255, 153), BLACK_L);
		m_pGridInput.SetItemFont(i + 1, 1, &clsFunc.OnLogFont(14));
		m_pGridInput.SetItemText(i + 1, 1, m_strIn[i]);
	}

	m_pGridInput.Invalidate(FALSE);
}


void CDialogAlarmIO::OnInitGridOutput()
{
	CString strTmp;
	int i, j;

	m_pGridOutput.SetFrameFocusCell(FALSE);
	m_pGridOutput.SetTrackFocusCell(FALSE);
	m_pGridOutput.EnableSelection(FALSE);

	m_pGridOutput.SetGridLineColor(BLACK_C);
	m_pGridOutput.SetGridLines(1);

	m_pGridOutput.SetRowCount(17);
	m_pGridOutput.SetColumnCount(2);

	m_pGridOutput.SetFixedRowCount(0);
	m_pGridOutput.SetFixedColumnCount(0);
	m_pGridOutput.SetFixedBkColor(RGB(0,0,200));
	m_pGridOutput.SetFixedBkColor(RGB(200,200,255));
	m_pGridOutput.SetTextBkColor(RGB(150,150,200));

	for (i=0; i<17; i++) 
	{
		m_pGridOutput.SetRowHeight(i, 30);

		for (j=0; j<2; j++) 
		{
			switch (j)
			{
				case 0:
					m_pGridOutput.SetColumnWidth(j, 50);
					break;

				case 1:
					m_pGridOutput.SetColumnWidth(j, 300);
					break;
			}

			m_pGridOutput.SetItemFormat(i, j,	DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_pGridOutput.SetItemState(i, j,	GVIS_READONLY);
			m_pGridOutput.SetItemBkColour(i, j,	 BLACK_L, WHITE_C);
		}
	}

	// IO MAP Header ////////////////////////////////////////////////////////////////////////////////////////////// 
 	m_pGridOutput.MergeCells(0, 0, 0, 1);
	m_pGridOutput.SetItemBkColour(0, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridOutput.SetItemFont(0, 0, &clsFunc.OnLogFont(24));
	m_pGridOutput.SetItemText(0, 0, _T("Alarm Output Check"));

	if (m_nOutCnt < 0) return;

	for (i=0; i<m_nOutCnt; i++) 
	{
		m_pGridOutput.SetItemText(i + 1, 0, _T("OFF"));
		m_pGridOutput.SetItemBkColour(i + 1, 0, GREEN_D, BLACK_C);

		m_pGridOutput.SetItemBkColour(i + 1, 1, RGB(255, 255, 153), BLACK_L);
		m_pGridOutput.SetItemFont(i + 1, 1, &clsFunc.OnLogFont(14));
		m_pGridOutput.SetItemText(i + 1, 1, m_strOut[i]);
	}

	m_pGridOutput.Invalidate(FALSE);
}


void CDialogAlarmIO::OnInputCheck()
{
	int i;
	int nIO;

	if (m_nInCnt < 0) return;
	if (m_nInCnt > 16) return;

	for (i=0; i<m_nInCnt; i++)
	{
		nIO = m_nIn[i];

		if (nIO >= 0 && nIO < 8716)
		{
			if (FAS_IO.get_in_bit(m_nIn[i], IO_OFF) == IO_ON)
			{
				m_pGridInput.SetItemText(i + 1, 0, _T("ON"));
				m_pGridInput.SetItemBkColour(i + 1, 0, YELLOW_C, BLACK_C);
			}
			else
			{
				m_pGridInput.SetItemText(i + 1, 0, _T("OFF"));
				m_pGridInput.SetItemBkColour(i + 1, 0, YELLOW_D, BLACK_C);
			}
		}
	}

	m_pGridInput.Invalidate(FALSE);
}


void CDialogAlarmIO::OnOutputCheck()
{
	int i;
	int nIO;

	if (m_nOutCnt < 0) return;
	if (m_nOutCnt > 16) return;

	for (i=0; i<m_nOutCnt; i++)
	{
		nIO = m_nOut[i];

		if (nIO >= 0 && nIO < 8716)
		{
			if (FAS_IO.get_out_bit(m_nOut[i], IO_OFF) == IO_ON)
			{
				m_pGridOutput.SetItemText(i + 1, 0, _T("ON"));
				m_pGridOutput.SetItemBkColour(i + 1, 0, GREEN_C, BLACK_C);
			}
			else
			{
				m_pGridOutput.SetItemText(i + 1, 0, _T("OFF"));
				m_pGridOutput.SetItemBkColour(i + 1, 0, GREEN_D, BLACK_C);
			}
		}
	}

	m_pGridOutput.Invalidate(FALSE);
}


void CDialogAlarmIO::OnCellOutputClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	int nRow, nCol;
	int nIO;

	NM_GRIDVIEW* pItem	= (NM_GRIDVIEW*) pNotifyStruct;

	nRow				= pItem->iRow;
	nCol				= pItem->iColumn;

	// Header Click
	if (nRow < 1) return;
	if (nCol != 0) return;

	nIO = m_nOut[nRow-1];
	if (nIO < 0) return;

	if (FAS_IO.get_out_bit(nIO, IO_OFF) == IO_ON)
	{
		FAS_IO.set_out_bit(nIO, IO_OFF);
	}
	else
	{
		FAS_IO.set_out_bit(nIO, IO_ON);
	}
}

BOOL CDialogAlarmIO::OnEraseBkgnd(CDC* pDC)
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
