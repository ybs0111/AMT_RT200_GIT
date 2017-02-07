// DialogMessageBox.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Handler.h"
#include "DialogMessageBox.h"
#include "afxdialogex.h"
#include "Variable.h"
#include "PublicFunction.h"
#include "FastechPublic_IO.h"

// CDialogMessageBox 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogMessageBox, CDialogEx)

CDialogMessageBox::CDialogMessageBox(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogMessageBox::IDD, pParent)
{

}

CDialogMessageBox::~CDialogMessageBox()
{
}

void CDialogMessageBox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MESSAGE, m_listMsg);
	DDX_Control(pDX, IDC_BUTTON_EXIT, m_btnExit);
	DDX_Control(pDX, IDC_GROUP_MESSAGE, m_groupMsg);
}


BEGIN_MESSAGE_MAP(CDialogMessageBox, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CDialogMessageBox::OnBnClickedButtonExit)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDialogMessageBox 메시지 처리기입니다.


BOOL CDialogMessageBox::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	OnInitGroup();
	OnInitButton();

	// jtkim 20150410
	if (st_other_info.nBuzYesNo == YES)
	{
//		FAS_IO.set_out_bit(st_io_info.o_Buzz2OnOff, IO_ON);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDialogMessageBox::OnMessageDisplay(CString strMsg)
{
	int nIndex = m_listMsg.GetCount();

	if (nIndex > 100)
	{
		m_listMsg.ResetContent();   // 리스트 박스 컨트롤에 추가된 모든 데이터 삭제 
	}

	if (m_strOldMsg == strMsg)
	{
		//2011.0504 추가 같은 메세지가 나오는 문제 처리 
		m_dwWaitTime[1] = GetCurrentTime();
		m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];
		
		if (m_dwWaitTime[2] < 0)
		{
			m_dwWaitTime[0] = GetTickCount();
			return;
		}
		else if (m_dwWaitTime[2] < 5000) 
		{
			//OK 파일로 내용 저장
			return;
		}
	}
	
	if (strMsg != _T(""))
	{
		m_dwWaitTime[0]	= GetCurrentTime();
		
		m_strOldMsg				= strMsg;
	}
	else
	{
		strMsg = _T("-No Message");
	}
	
	if (st_other_info.nBuzYesNo == YES)
	{
//		FAS_IO.set_out_bit(st_io_info.o_Buzz2OnOff, IO_ON);
	}

	nIndex = m_listMsg.AddString(strMsg, RGB(255, 0, 0));  // 현재 입력된 정보 리스트 박스 컨트롤에 추가 
	m_listMsg.SetCurSel(nIndex);
}

void CDialogMessageBox::OnInitButton()
{
	m_btnExit.SetBitmaps(IDC_BTN_OK, IDB_BITMAP_EXIT, WINDOW_DN1, IDB_BITMAP_EXIT, WINDOW_UP1);
	m_btnExit.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnExit.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnExit.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnExit.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnExit.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnExit.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnExit.SetFont(clsFunc.m_pFont[3]);
}

void CDialogMessageBox::OnInitGroup()
{
	m_groupMsg.SetFont(clsFunc.OnLogFont(16));
	m_groupMsg.SetCatptionTextColor(BLUE_C);
	m_groupMsg.SetBorderColor(ORANGE_C);
	m_groupMsg.SetFontBold(TRUE);
	m_groupMsg.SetBackgroundColor(WINDOW_UP1);
}

void CDialogMessageBox::OnBnClickedButtonExit()
{
	//st_work_info.nBdReadyChk = YES;

	if (st_other_info.nBuzYesNo == YES)
	{
		st_other_info.nBuzYesNo = NO;
	}

	if (st_handler_info.cWndMain != NULL)
	{
		st_handler_info.cWndMain->SendMessage(WM_WORK_COMMAND, MAIN_MESSAGE_BOX_DELETE_REQ, 0);
	}
}

BOOL CDialogMessageBox::OnEraseBkgnd(CDC* pDC)
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


HBRUSH CDialogMessageBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	switch(pWnd->GetDlgCtrlID())
		{
		case IDC_GROUP_MESSAGE:
			pDC->SetBkMode(TRANSPARENT);
			return (HBRUSH)::GetStockObject(NULL_BRUSH);
			break;
		}
	return hbr;
}

BOOL CDialogMessageBox::Create()
{
	return CDialog::Create( CDialogMessageBox::IDD );
}

BOOL CDialogMessageBox::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
//	FAS_IO.set_out_bit(st_io_info.o_Buzz2OnOff, IO_OFF);

	return CDialogEx::DestroyWindow();
}
