// DialogMessageView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Handler.h"
#include "DialogMessageView.h"
#include "afxdialogex.h"
#include "Variable.h"
#include "PublicFunction.h"
#include "FastechPublic_IO.h"

// CDialogMessageView ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDialogMessageView, CDialog)

CDialogMessageView::CDialogMessageView(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogMessageView::IDD, pParent)
{
	m_bmGround.LoadBitmap(IDB_BITMAP_DLG_BACK_GROUND);   
}

CDialogMessageView::~CDialogMessageView()
{
}

void CDialogMessageView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_OK,		m_btnOk);
	DDX_Control(pDX, IDC_BTN_CANCEL,	m_btnCancel);
	DDX_Control(pDX, IDC_BTN_RETRY,	m_btnRetry);
	DDX_Control(pDX, IDC_LIST_DATA,		m_listData);
	DDX_Control(pDX, IDC_GROUP_MESSAGE, m_groubMsg);
}


BEGIN_MESSAGE_MAP(CDialogMessageView, CDialog)
	ON_BN_CLICKED(IDC_BTN_OK, &CDialogMessageView::OnBnClickedBtnOk)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CDialogMessageView::OnBnClickedBtnCancel)
	ON_WM_ERASEBKGND()
//	ON_BN_CLICKED(IDC_BTN_TRACK_IN, &CDialogMessageView::OnBnClickedBtnTrackIn)
ON_BN_CLICKED(IDC_BTN_RETRY, &CDialogMessageView::OnBnClickedBtnRetry)
END_MESSAGE_MAP()


// CDialogMessageView �޽��� ó�����Դϴ�.

BOOL CDialogMessageView::Create()
{
	return CDialog::Create( CDialogMessageView::IDD );
}


BOOL CDialogMessageView::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	OnInitButton();
	OnInitGroupBox();

	OnMessageDisplay();

//	FAS_IO.set_out_bit(st_io_info.o_Buzz3OnOff, IO_ON);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CDialogMessageView::OnInitButton()
{
	m_btnCancel.SetBitmaps(IDC_BTN_CANCEL, IDB_CANCEL_DN, WINDOW_DN1, IDB_CANCEL_UP, WINDOW_UP1);
	m_btnCancel.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnCancel.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnCancel.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnCancel.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnCancel.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnCancel.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnCancel.SetFont(clsFunc.m_pFont[3]);

	m_btnOk.SetBitmaps(IDC_BTN_OK, IDB_OK_DN, WINDOW_DN1, IDB_OK_UP, WINDOW_UP1);
	m_btnOk.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnOk.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnOk.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnOk.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnOk.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnOk.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnOk.SetFont(clsFunc.m_pFont[3]);

	m_btnRetry.SetBitmaps(IDC_BTN_RETRY, IDB_OK_DN, WINDOW_DN1, IDB_OK_UP, WINDOW_UP1);
	m_btnRetry.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnRetry.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnRetry.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnRetry.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnRetry.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnRetry.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnRetry.SetFont(clsFunc.m_pFont[3]);

	if (m_nConfirmType == 1)
	{
		m_btnOk.SetWindowText(_T("Pass"));
		m_btnCancel.SetWindowText(_T("Fail"));
	}
	else
	{
		m_btnRetry.ShowWindow(SW_HIDE);

		m_btnOk.SetWindowText(_T("Yes"));
		m_btnCancel.SetWindowText(_T("No"));
	}
}


void CDialogMessageView::OnInitGroupBox()
{
	m_groubMsg.SetFont(clsFunc.OnLogFont(16));
	m_groubMsg.SetCatptionTextColor(BLUE_C);
	m_groubMsg.SetBorderColor(ORANGE_C);
	m_groubMsg.SetFontBold(TRUE);
	m_groubMsg.SetBackgroundColor(WINDOW_UP1);
}


void CDialogMessageView::OnMessageDisplay()
{
	int nPos[2];
	bool bfind = true;
	CString strTmp;

	nPos[0]	= 0;
	while(bfind)
	{
		nPos[1]	= -1;
		nPos[1] = m_strMessage.Find(_T("\r\n"), nPos[0]);
		if(nPos[1] < 0)
		{
			strTmp	= _T(" - ");
			strTmp	+= m_strMessage.Mid(nPos[0], m_strMessage.GetLength());
			m_listData.AddString(strTmp, RGB(255, 0, 0));  // ���� �Էµ� ���� ����Ʈ �ڽ� ��Ʈ�ѿ� �߰� 
			bfind = false;
		}
		else
		{
			strTmp	= _T(" - ");
			strTmp	+= m_strMessage.Mid(nPos[0], nPos[1] - nPos[0]);
			m_listData.AddString(strTmp, RGB(255, 0, 0));  // ���� �Էµ� ���� ����Ʈ �ڽ� ��Ʈ�ѿ� �߰� 
			nPos[0]	= nPos[1] + 2; 
		}
	}
}

void CDialogMessageView::OnBnClickedBtnOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	st_other_info.nConfirmMsg = 1;
//	FAS_IO.set_out_bit(st_io_info.o_Buzz3OnOff, IO_OFF);

	if (st_handler_info.cWndMain != NULL)
	{
		st_handler_info.cWndMain->SendMessage(WM_WORK_COMMAND, MAIN_MESSAGE_VIEW_DELETE_REQ, 0);
	}
}


void CDialogMessageView::OnBnClickedBtnCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	st_other_info.nConfirmMsg = 0;
//	FAS_IO.set_out_bit(st_io_info.o_Buzz3OnOff, IO_OFF);

	if (st_handler_info.cWndMain != NULL)
	{
		st_handler_info.cWndMain->SendMessage(WM_WORK_COMMAND, MAIN_MESSAGE_VIEW_DELETE_REQ, 0);
	}
}


BOOL CDialogMessageView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CDC  memDC;                     // �޸� DC
	CBitmap * pOldBitmap = NULL;    // m_BackBitmap���� ���ο� �׸��� DC�� �׸� ��, ���� �׷����� DC(��, Bitmap)�� ����.
	BITMAP bitmapInfo;              // �׸��� ����(m_BackBitmap)
	m_bmGround.GetBitmap(&bitmapInfo); // Bitmap ũ�� ����.

	memDC.CreateCompatibleDC(pDC);  // �޸� DC ����
	pOldBitmap = memDC.SelectObject(&m_bmGround);
	pDC->BitBlt(0, 0, bitmapInfo.bmWidth, bitmapInfo.bmHeight, &memDC, 0, 0, SRCCOPY);   // �޸� DC�� �׸��� �׸�

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();

	//	return CFormView::OnEraseBkgnd(pDC);
	return true;
}


void CDialogMessageView::OnBnClickedBtnRetry()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	st_other_info.nConfirmMsg = 2;
//	FAS_IO.set_out_bit(st_io_info.o_Buzz3OnOff, IO_OFF);

	if (st_handler_info.cWndMain != NULL)
	{
		st_handler_info.cWndMain->SendMessage(WM_WORK_COMMAND, MAIN_MESSAGE_VIEW_DELETE_REQ, 0);
	}
}
