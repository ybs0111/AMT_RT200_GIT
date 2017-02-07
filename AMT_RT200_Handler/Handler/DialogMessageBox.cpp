// DialogMessageBox.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Handler.h"
#include "DialogMessageBox.h"
#include "afxdialogex.h"
#include "Variable.h"
#include "PublicFunction.h"
#include "FastechPublic_IO.h"

// CDialogMessageBox ��ȭ �����Դϴ�.

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


// CDialogMessageBox �޽��� ó�����Դϴ�.


BOOL CDialogMessageBox::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	OnInitGroup();
	OnInitButton();

	// jtkim 20150410
	if (st_other_info.nBuzYesNo == YES)
	{
//		FAS_IO.set_out_bit(st_io_info.o_Buzz2OnOff, IO_ON);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CDialogMessageBox::OnMessageDisplay(CString strMsg)
{
	int nIndex = m_listMsg.GetCount();

	if (nIndex > 100)
	{
		m_listMsg.ResetContent();   // ����Ʈ �ڽ� ��Ʈ�ѿ� �߰��� ��� ������ ���� 
	}

	if (m_strOldMsg == strMsg)
	{
		//2011.0504 �߰� ���� �޼����� ������ ���� ó�� 
		m_dwWaitTime[1] = GetCurrentTime();
		m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];
		
		if (m_dwWaitTime[2] < 0)
		{
			m_dwWaitTime[0] = GetTickCount();
			return;
		}
		else if (m_dwWaitTime[2] < 5000) 
		{
			//OK ���Ϸ� ���� ����
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

	nIndex = m_listMsg.AddString(strMsg, RGB(255, 0, 0));  // ���� �Էµ� ���� ����Ʈ �ڽ� ��Ʈ�ѿ� �߰� 
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
	CDC  memDC;                     // �޸� DC
	CBitmap * pOldBitmap = NULL;    // m_BackBitmap���� ���ο� �׸��� DC�� �׸� ��, ���� �׷����� DC(��, Bitmap)�� ����.
	BITMAP bitmapInfo;              // �׸��� ����(m_BackBitmap)
	st_handler_info.m_BackDlgBitmap.GetBitmap(&bitmapInfo); // Bitmap ũ�� ����.

	memDC.CreateCompatibleDC(pDC);  // �޸� DC ����
	pOldBitmap = memDC.SelectObject(&st_handler_info.m_BackDlgBitmap);
	pDC->BitBlt(0, 0, bitmapInfo.bmWidth, bitmapInfo.bmHeight, &memDC, 0, 0, SRCCOPY);   // �޸� DC�� �׸��� �׸�

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	return true; // �� �η����� �ƴ� ����� �׷��� �ϱ� ������ true
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
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
//	FAS_IO.set_out_bit(st_io_info.o_Buzz2OnOff, IO_OFF);

	return CDialogEx::DestroyWindow();
}
