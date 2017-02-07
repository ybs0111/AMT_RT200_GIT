// Dialog_Message.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Dialog_Message.h"
#include "afxdialogex.h"
#include "Variable.h"
#include "PublicFunction.h"


// CDialog_Message dialog

IMPLEMENT_DYNAMIC(CDialog_Message, CDialog)

CDialog_Message::CDialog_Message(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog_Message::IDD, pParent)
{

}

CDialog_Message::~CDialog_Message()
{
}

void CDialog_Message::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_OK,		m_btnOk);
	DDX_Control(pDX, IDC_BTN_PASS,		m_btnPass);
	DDX_Control(pDX, IDC_BTN_CANCEL,	m_btnCancel);
	DDX_Control(pDX, IDC_LIST_DATA,		m_listData);
	DDX_Control(pDX, IDC_GROUP_MESSAGE, m_groubMsgShow);
}


BEGIN_MESSAGE_MAP(CDialog_Message, CDialog)
	ON_BN_CLICKED(IDC_BTN_OK, &CDialog_Message::OnBnClickedBtnOk)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CDialog_Message::OnBnClickedBtnCancel)
	ON_BN_CLICKED(IDC_BTN_PASS, &CDialog_Message::OnBnClickedBtnPass)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDialog_Message message handlers


BOOL CDialog_Message::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	st_handler_info.nDlgLock = FALSE;

	return CDialog::DestroyWindow();
}


BOOL CDialog_Message::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_listData.SetFont(clsFunc.m_pFont[2]);

	OnInitButton();
	OnInitGroupBox();

	OnMessageDisplay();

	return TRUE; 
}


void CDialog_Message::OnMessageDisplay()
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
			m_listData.AddString(strTmp, RGB(255, 0, 0));  // 현재 입력된 정보 리스트 박스 컨트롤에 추가 
			bfind = false;
		}
		else
		{
			strTmp	= _T(" - ");
			strTmp	+= m_strMessage.Mid(nPos[0], nPos[1] - nPos[0]);
			m_listData.AddString(strTmp, RGB(255, 0, 0));  // 현재 입력된 정보 리스트 박스 컨트롤에 추가 
			nPos[0]	= nPos[1] + 2; 
		}
	}
	
}

void CDialog_Message::OnInitButton()
{
	m_btnCancel.SetBitmaps(IDC_BTN_CANCEL, IDB_CANCEL_DN, WINDOW_DN1, IDB_CANCEL_UP, WINDOW_UP1);
	m_btnCancel.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnCancel.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnCancel.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnCancel.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnCancel.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnCancel.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnCancel.SetFont(clsFunc.m_pFont[3]);

	m_btnPass.SetBitmaps(IDC_BTN_PASS, IDB_CANCEL_DN, WINDOW_DN1, IDB_CANCEL_UP, WINDOW_UP1);
	m_btnPass.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnPass.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnPass.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnPass.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnPass.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnPass.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnPass.SetFont(clsFunc.m_pFont[3]);

	m_btnOk.SetBitmaps(IDC_BTN_OK, IDB_OK_DN, WINDOW_DN1, IDB_OK_UP, WINDOW_UP1);
	m_btnOk.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnOk.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnOk.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnOk.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnOk.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnOk.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnOk.SetFont(clsFunc.m_pFont[3]);

	switch (m_nMessageType)
	{
		case 0:
			m_btnCancel.ShowWindow(SW_SHOW);
			m_btnPass.ShowWindow(SW_HIDE);
			m_btnOk.ShowWindow(SW_HIDE);
			break;

		case 1:
			m_btnCancel.ShowWindow(SW_SHOW);
			m_btnPass.ShowWindow(SW_HIDE);
			m_btnOk.ShowWindow(SW_SHOW);
			break;

		case 2:
			m_btnCancel.ShowWindow(SW_SHOW);
			m_btnPass.ShowWindow(SW_SHOW);
			m_btnOk.ShowWindow(SW_SHOW);
			break;
	}
}

void CDialog_Message::OnInitGroupBox()
{
	m_groubMsgShow.SetFont(clsFunc.OnLogFont(16));
	m_groubMsgShow.SetCatptionTextColor(BLUE_C);
	m_groubMsgShow.SetBorderColor(ORANGE_C);
	m_groubMsgShow.SetFontBold(TRUE);
	m_groubMsgShow.SetBackgroundColor(WINDOW_UP1);
}

void CDialog_Message::OnBnClickedBtnOk()
{
//	st_work_info.nMessageInfo = IDOK;

//	OnOK();
	st_other_info.nConfirmMsg = YES;

	CDialog::OnOK();
}


void CDialog_Message::OnBnClickedBtnCancel()
{
//	st_work_info.nMessageInfo = IDCANCEL;

//	OnCancel();
//	st_work_info.nMessageInfo = NO;
	st_other_info.nConfirmMsg = NO;

	CDialog::OnCancel();
}


void CDialog_Message::OnBnClickedBtnPass()
{
	OnCancel();
}


void CDialog_Message::OnDestroy()
{
	CDialog::OnDestroy();
}


BOOL CDialog_Message::OnEraseBkgnd(CDC* pDC)
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
