// Dialog_Password_Change.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Handler.h"
#include "Dialog_Password_Change.h"
#include "afxdialogex.h"
#include "Variable.h"
#include "PublicFunction.h"

// CDialog_Password_Change ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDialog_Password_Change, CDialog)

CDialog_Password_Change::CDialog_Password_Change(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog_Password_Change::IDD, pParent)
{
	m_nPassState		= 0;
	m_btmBk.LoadBitmap(IDB_BITMAP_DLG_BACK_GROUND);   
}

CDialog_Password_Change::~CDialog_Password_Change()
{
}

void CDialog_Password_Change::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GROUP_BOARD,					m_groupKeyBoard);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_SHARP,			m_btnBoardSharp);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_RBRACE_S,			m_btnBoardRbraceS);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_RBRACE_B,			m_btnBoardRbraceB);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_LINE,				m_btnBoardLine);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_LBRACE_S,			m_btnBoardLbraceS);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_LBRACE_B,			m_btnBoardLbraceB);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_DOT,				m_btnBoardDot);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_DOLLAR,			m_btnBoardDollar);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_DIV,				m_btnBoardDiv);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_DEL,				m_btnBoardDel);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_BACK,				m_btnBoardBack);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_ADD,				m_btnBoardAdd);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_Z,				m_btnBoardZ);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_Y,				m_btnBoardY);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_X,				m_btnBoardX);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_W,				m_btnBoardW);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_V,				m_btnBoardV);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_U,				m_btnBoardU);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_T,				m_btnBoardT);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_S,				m_btnBoardS);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_R,				m_btnBoardR);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_Q,				m_btnBoardQ);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_P,				m_btnBoardP);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_O,				m_btnBoardO);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_N,				m_btnBoardN);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_M,				m_btnBoardM);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_L,				m_btnBoardL);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_K,				m_btnBoardK);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_J,				m_btnBoardJ);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_I,				m_btnBoardI);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_H,				m_btnBoardH);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_G,				m_btnBoardG);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_F,				m_btnBoardF);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_E,				m_btnBoardE);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_D,				m_btnBoardD);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_C,				m_btnBoardC);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_B,				m_btnBoardB);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_A,				m_btnBoardA);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_9,				m_btnBoard9);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_8,				m_btnBoard8);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_7,				m_btnBoard7);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_6,				m_btnBoard6);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_5,				m_btnBoard5);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_4,				m_btnBoard4);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_3,				m_btnBoard3);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_2,				m_btnBoard2);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_1,				m_btnBoard1);
	DDX_Control(pDX, IDC_BTN_PASS_CHG_0,				m_btnBoard0);
	DDX_Control(pDX, IDOK,								m_btnOk);
	DDX_Control(pDX, IDCANCEL,							m_btnCancel);
	DDX_Control(pDX, IDC_MSG_PASS_CHG,					m_msgPassChg);
	DDX_Control(pDX, IDC_CHECK_OLD,						m_btnOld);
	DDX_Control(pDX, IDC_CHECK_NEW,						m_btnNew);
	DDX_Control(pDX, IDC_CHECK_VERIFY,					m_btnVerify);
}


BEGIN_MESSAGE_MAP(CDialog_Password_Change, CDialog)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_0, &CDialog_Password_Change::OnClickedBtnPassChg0)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_1, &CDialog_Password_Change::OnClickedBtnPassChg1)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_2, &CDialog_Password_Change::OnClickedBtnPassChg2)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_3, &CDialog_Password_Change::OnClickedBtnPassChg3)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_4, &CDialog_Password_Change::OnClickedBtnPassChg4)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_5, &CDialog_Password_Change::OnClickedBtnPassChg5)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_6, &CDialog_Password_Change::OnClickedBtnPassChg6)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_7, &CDialog_Password_Change::OnClickedBtnPassChg7)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_8, &CDialog_Password_Change::OnClickedBtnPassChg8)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_9, &CDialog_Password_Change::OnClickedBtnPassChg9)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_A, &CDialog_Password_Change::OnClickedBtnPassChgA)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_ADD, &CDialog_Password_Change::OnClickedBtnPassChgAdd)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_B, &CDialog_Password_Change::OnClickedBtnPassChgB)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_BACK, &CDialog_Password_Change::OnClickedBtnPassChgBack)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_C, &CDialog_Password_Change::OnClickedBtnPassChgC)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_D, &CDialog_Password_Change::OnClickedBtnPassChgD)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_DEL, &CDialog_Password_Change::OnClickedBtnPassChgDel)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_DIV, &CDialog_Password_Change::OnClickedBtnPassChgDiv)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_DOLLAR, &CDialog_Password_Change::OnClickedBtnPassChgDollar)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_DOT, &CDialog_Password_Change::OnClickedBtnPassChgDot)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_E, &CDialog_Password_Change::OnClickedBtnPassChgE)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_F, &CDialog_Password_Change::OnClickedBtnPassChgF)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_G, &CDialog_Password_Change::OnClickedBtnPassChgG)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_H, &CDialog_Password_Change::OnClickedBtnPassChgH)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_I, &CDialog_Password_Change::OnClickedBtnPassChgI)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_J, &CDialog_Password_Change::OnClickedBtnPassChgJ)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_K, &CDialog_Password_Change::OnClickedBtnPassChgK)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_L, &CDialog_Password_Change::OnClickedBtnPassChgL)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_LBRACE_B, &CDialog_Password_Change::OnClickedBtnPassChgLbraceB)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_LBRACE_S, &CDialog_Password_Change::OnClickedBtnPassChgLbraceS)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_LINE, &CDialog_Password_Change::OnClickedBtnPassChgLine)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_M, &CDialog_Password_Change::OnClickedBtnPassChgM)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_N, &CDialog_Password_Change::OnClickedBtnPassChgN)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_O, &CDialog_Password_Change::OnClickedBtnPassChgO)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_P, &CDialog_Password_Change::OnClickedBtnPassChgP)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_Q, &CDialog_Password_Change::OnClickedBtnPassChgQ)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_R, &CDialog_Password_Change::OnClickedBtnPassChgR)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_RBRACE_B, &CDialog_Password_Change::OnClickedBtnPassChgRbraceB)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_RBRACE_S, &CDialog_Password_Change::OnClickedBtnPassChgRbraceS)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_S, &CDialog_Password_Change::OnClickedBtnPassChgS)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_SHARP, &CDialog_Password_Change::OnClickedBtnPassChgSharp)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_T, &CDialog_Password_Change::OnClickedBtnPassChgT)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_U, &CDialog_Password_Change::OnClickedBtnPassChgU)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_W, &CDialog_Password_Change::OnClickedBtnPassChgW)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_V, &CDialog_Password_Change::OnClickedBtnPassChgV)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_X, &CDialog_Password_Change::OnClickedBtnPassChgX)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_Y, &CDialog_Password_Change::OnClickedBtnPassChgY)
	ON_BN_CLICKED(IDC_BTN_PASS_CHG_Z, &CDialog_Password_Change::OnClickedBtnPassChgZ)
//	ON_BN_CLICKED(IDC_RADIO_PASS_NEW, &CDialog_Password_Change::OnClickedRadioPassNew)
//	ON_BN_CLICKED(IDC_RADIO_PASS_OLD, &CDialog_Password_Change::OnClickedRadioPassOld)
//	ON_BN_CLICKED(IDC_RADIO_PASS_VERIFY, &CDialog_Password_Change::OnClickedRadioPassVerify)
	ON_BN_CLICKED(IDOK, &CDialog_Password_Change::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_NEW, &CDialog_Password_Change::OnClickedCheckNew)
	ON_BN_CLICKED(IDC_CHECK_OLD, &CDialog_Password_Change::OnClickedCheckOld)
	ON_BN_CLICKED(IDC_CHECK_VERIFY, &CDialog_Password_Change::OnClickedCheckVerify)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDialog_Password_Change �޽��� ó�����Դϴ�.


BOOL CDialog_Password_Change::DestroyWindow()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	return CDialog::DestroyWindow();
}


BOOL CDialog_Password_Change::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	OnInitLabel();
	OnInitGroupBox();
	OnInitButton();
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CDialog_Password_Change::OnInitLabel()
{
	m_msgPassChg.SetFont(clsFunc.m_pFont[1]);
	m_msgPassChg.SetWindowText(_T("Change the New Password"));
	m_msgPassChg.SetCenterText();
	m_msgPassChg.SetColor(RGB(0,0,255));
	m_msgPassChg.SetGradientColor(RGB(0,0,0));
	m_msgPassChg.SetTextColor(RGB(255,255,255));
}


void CDialog_Password_Change::OnInitGroupBox()
{
	m_groupKeyBoard.SetFont(clsFunc.OnLogFont(16));
	m_groupKeyBoard.SetCatptionTextColor(BLUE_C);
	m_groupKeyBoard.SetBorderColor(ORANGE_C);
	m_groupKeyBoard.SetFontBold(TRUE);
	m_groupKeyBoard.SetBackgroundColor(WINDOW_UP1);
}


void CDialog_Password_Change::OnInitButton()
{
	m_btnOld.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnOld.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnOld.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnOld.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnOld.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnOld.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnOld.SetFont(clsFunc.m_pFont[2]);

	m_btnNew.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnNew.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnNew.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnNew.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnNew.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnNew.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnNew.SetFont(clsFunc.m_pFont[2]);

	m_btnVerify.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnVerify.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnVerify.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnVerify.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnVerify.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnVerify.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnVerify.SetFont(clsFunc.m_pFont[2]);

	OnPassChangeRadioStateSet(0);
}


BOOL CDialog_Password_Change::OnPassChangeLengthCheck(int nCase)
{
	int nLength;

	switch(nCase)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData.MakeUpper();
			m_strOldData.TrimLeft(' ');               
			m_strOldData.TrimRight(' ');

			nLength = m_strOldData.GetLength();
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData.MakeUpper();
			m_strNewData.TrimLeft(' ');               
			m_strNewData.TrimRight(' ');

			nLength = m_strNewData.GetLength();
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData.MakeUpper();
			m_strVerifyData.TrimLeft(' ');               
			m_strVerifyData.TrimRight(' ');

			nLength = m_strVerifyData.GetLength();
			break;

		default:
			return FALSE;
			break;	
	}

	if (nLength >= 15) return FALSE;

	return TRUE;
}

void CDialog_Password_Change::OnClickedBtnPassChg0()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("0");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("0");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("0");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChg1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("1");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("1");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("1");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChg2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("2");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("2");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("2");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChg3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("3");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("3");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("3");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChg4()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("4");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("4");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("4");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChg5()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("5");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("5");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("5");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChg6()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("6");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("6");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("6");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChg7()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("7");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("7");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("7");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChg8()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("8");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("8");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("8");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChg9()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("9");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("9");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("9");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgA()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("A");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("A");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("A");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgAdd()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("+");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("+");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("+");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgB()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("B");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("B");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("B");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgBack()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString strData;  // �Է� ���� �ӽ� ���� ���� 
	int nCnt;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			nCnt = m_strOldData.GetLength();
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			nCnt = m_strNewData.GetLength();
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			nCnt = m_strVerifyData.GetLength();
			break;

		default:
			break;
	}

	if (nCnt == 0) return;  // �Է� �����Ͱ� �������� �ʴ� ��쿡�� ���� 

	switch(m_nPassState)
	{
		case 0:
			strData			= m_strOldData ;
			m_strOldData	= strData.Left(nCnt-1) ;
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			strData			= m_strNewData ;
			m_strNewData	= strData.Left(nCnt-1) ;
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			strData			= m_strVerifyData ;
			m_strVerifyData = strData.Left(nCnt-1) ;
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;
		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgC()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("C");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("C");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("C");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgD()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("D");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("D");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("D");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgDel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	switch(m_nPassState)  
	{
		case 0:
			m_strOldData.Empty();
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;
		case 1:
			m_strNewData.Empty();
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;
		case 2:
			m_strVerifyData.Empty();
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;
		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgDiv()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("/");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("/");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("/");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgDollar()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("$");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("$");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("$");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgDot()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T(".");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T(".");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T(".");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgE()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("E");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("E");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("E");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgF()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("F");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("F");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("F");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgG()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("G");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("G");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("G");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgH()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("H");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("H");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("H");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgI()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("I");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("I");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("I");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgJ()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("J");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("J");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("J");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgK()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("K");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("K");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("K");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgL()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("L");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("L");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("L");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgLbraceB()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("[");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("[");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("[");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgLbraceS()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("(");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("(");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("(");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgLine()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("-");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("-");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("-");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgM()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("M");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("M");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("M");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgN()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("N");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("N");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("N");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgO()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("O");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("O");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("O");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgP()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("P");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("P");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("P");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgQ()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("Q");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("Q");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("Q");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgR()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("R");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("R");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("R");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgRbraceB()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("]");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("]");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("]");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgRbraceS()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T(")");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T(")");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T(")");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgS()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("S");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("S");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("S");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgSharp()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("#");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("#");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("#");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgT()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("T");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("T");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("T");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgU()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("U");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("U");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("U");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgW()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("W");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("W");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("W");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgV()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("V");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("V");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("V");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgX()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("X");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("X");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("X");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgY()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("Y");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("Y");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("Y");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


void CDialog_Password_Change::OnClickedBtnPassChgZ()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!OnPassChangeLengthCheck(m_nPassState))  return;

	switch(m_nPassState)
	{
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
			m_strOldData +=_T("Z");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
			break;

		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
			m_strNewData +=_T("Z");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
			break;

		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
			m_strVerifyData +=_T("Z");
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
			break;

		default:
			break;
	}
}


//void CDialog_Password_Change::OnClickedRadioPassNew()
//{
//	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//	m_nPassState = 1; 
//
//	m_btnOldData.SetChecked(FALSE);
//	m_btnNewData.SetChecked(TRUE);
//	m_btnVerifyData.SetChecked(FALSE);
//
//	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetFocus();
//}


//void CDialog_Password_Change::OnClickedRadioPassOld()
//{
//	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//	m_nPassState = 0;
//
//	m_btnOldData.SetChecked(TRUE);
//	m_btnNewData.SetChecked(FALSE);
//	m_btnVerifyData.SetChecked(FALSE);
//
//	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetFocus();
//}


//void CDialog_Password_Change::OnClickedRadioPassVerify()
//{
//	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//	m_nPassState = 2;
//
//	m_btnOldData.SetChecked(FALSE);
//	m_btnNewData.SetChecked(FALSE);
//	m_btnVerifyData.SetChecked(TRUE);
//
//	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetFocus();
//}


void CDialog_Password_Change::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int n_len, i, n_check_num = 0, n_check_string = 0, n_check_extra = 0;
	char chr_data[50];
	CString strOtherPassword;
	CString strLoadFile;

	strLoadFile = st_path_info.strBasic + st_basic_info.strDeviceName;

	if (st_other_info.strPassLevel == _T("Level1"))				// Level 1
	{
		:: GetPrivateProfileString(_T("Password"), _T("Level_2"), _T("M"), (LPWSTR)chr_data, 50, strLoadFile);
		strOtherPassword = chr_data;
		strOtherPassword.MakeUpper();     // ���ڿ� �빮�ڷ� ����
		strOtherPassword.TrimLeft(' ');   // ���� ���ڿ� ���� ����              
		strOtherPassword.TrimRight(' ');  // ���� ���ڿ� ���� ����
	}
	else if (st_other_info.strPassLevel == _T("Level2"))			// Level 1
	{
		:: GetPrivateProfileString(_T("Password"), _T("Level_1"), _T("M"), (LPWSTR)chr_data, 50, strLoadFile);
		strOtherPassword = chr_data;
		strOtherPassword.MakeUpper();     // ���ڿ� �빮�ڷ� ����
		strOtherPassword.TrimLeft(' ');   // ���� ���ڿ� ���� ����              
		strOtherPassword.TrimRight(' ');  // ���� ���ڿ� ���� ����
	}
	else if (st_other_info.strPassLevel == _T("SysLock"))			// Level 1
	{
		:: GetPrivateProfileString(_T("Password"), _T("SysLock"), _T("M"), (LPWSTR)chr_data, 50, strLoadFile);
		strOtherPassword = chr_data;
		strOtherPassword.MakeUpper();     // ���ڿ� �빮�ڷ� ����
		strOtherPassword.TrimLeft(' ');   // ���� ���ڿ� ���� ����              
		strOtherPassword.TrimRight(' ');  // ���� ���ڿ� ���� ����
	}


	// **************************************************************************
	// ���� ��ȣ �Է� ���� �˻�                                                  
	// **************************************************************************
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->GetWindowText(m_strOldData);
	m_strOldData.MakeUpper();
	m_strOldData.TrimLeft(' ');               
	m_strOldData.TrimRight(' ');

	if (m_strOldData.IsEmpty())
	{
		OnPassChangeErrorDisplay(_T("Password Input Fail : "), st_other_info.strPassLevel); // ��ȣ ���� �� �߻��� ���� ���� ��� �Լ�
		
		m_nPassState = 0;  // ���� ��ȣ ���·� ���� 

		OnPassChangeRadioStateSet(m_nPassState); // ���� ��ư ���� ���� �Լ�
		OnPassChangePasswordFocus(m_nPassState); // ��ȣ �Է� Į�� ����Ʈ �ڽ� ��Ŀ�� ���� �Լ�
		
		return;
	}
/*
	if (mstr_temp != m_strOldData)  // ���� ��ȣ �Է� ���� �˻� 
	{
		OnPassChange_Error_Display(_T("Password is inaccurate : "), st_other_info.strPassLevel); // ��ȣ ���� �� �߻��� ���� ���� ��� �Լ�
		
		mn_pass_state = 0;  // ���� ��ȣ ���·� ���� 

		OnPassChange_Radio_State_Set(mn_pass_state); // ���� ��ư ���� ���� �Լ�
		OnPassChange_Password_Focus(mn_pass_state); // ��ȣ �Է� Į�� ����Ʈ �ڽ� ��Ŀ�� ���� �Լ�
		
		return;
	}*/
	// **************************************************************************

	// **************************************************************************
	// �� ��ȣ �Է� ���� �˻�                                                    
	// **************************************************************************
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->GetWindowText(m_strNewData);
	m_strNewData.MakeUpper();
	m_strNewData.TrimLeft(' ');               
	m_strNewData.TrimRight(' ');

	if (m_strNewData.IsEmpty())
	{
		m_msgPassChg.SetWindowText(_T("New password input mistake"));

		m_strNewData.Empty();
		m_strVerifyData.Empty();

		((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
		((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);

		m_nPassState = 1;  // �� ��ȣ ���·� ���� 

		OnPassChangeRadioStateSet(m_nPassState); // ���� ��ư ���� ���� �Լ�
		OnPassChangePasswordFocus(m_nPassState); // ��ȣ �Է� Į�� ����Ʈ �ڽ� ��Ŀ�� ���� �Լ�

		return;
	}
	// **************************************************************************

	// **************************************************************************
	// Ȯ�� ��ȣ �Է� ���� �˻�                                                  
	// **************************************************************************
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->GetWindowText(m_strVerifyData);
	m_strVerifyData.MakeUpper();
	m_strVerifyData.TrimLeft(' ');               
	m_strVerifyData.TrimRight(' ');

	if (m_strVerifyData.IsEmpty())
	{
		m_msgPassChg.SetWindowText(_T("Confirmation password input mistake."));

		m_strVerifyData.Empty();
		((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);

		m_nPassState = 2;  // ��ȣ Ȯ�� ���·� ���� 

		OnPassChangeRadioStateSet(m_nPassState); // ���� ��ư ���� ���� �Լ�
		OnPassChangePasswordFocus(m_nPassState); // ��ȣ �Է� Į�� ����Ʈ �ڽ� ��Ŀ�� ���� �Լ�
		
		return;
	}

	if (m_strVerifyData != m_strNewData)  // �� ��ȣ ������ ��ȣ Ȯ�� ���� ��ġ ���� �˻� 
	{
		m_msgPassChg.SetWindowText(_T("Confirmation password input mistake."));

		m_strNewData.Empty();
		m_strVerifyData.Empty();

		((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
		((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);

		m_nPassState = 1;  // �� ��ȣ ���·� ���� 

		OnPassChangeRadioStateSet(m_nPassState);	// ���� ��ư ���� ���� �Լ�
		OnPassChangePasswordFocus(m_nPassState);		// ��ȣ �Է� Į�� ����Ʈ �ڽ� ��Ŀ�� ���� �Լ�

		return;
	}
	// **************************************************************************

	// **************************************************************************
	//  ADMINISTRATOR�� �浹 �˻�                                                
	// **************************************************************************
	if (m_strNewData == "ADMINISTRATOR" || m_strNewData == "ADMIN" || m_strNewData == "AMTSW")			// ������ ���̵�� ���� ����.
	{
		m_msgPassChg.SetWindowText(_T("Not be registered is the password."));
		m_strNewData.Empty();
		m_strVerifyData.Empty();
		
		((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
		((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
		return;
	}
 
	n_len = m_strNewData.GetLength();
	
	if (n_len > 50)
	{
		m_msgPassChg.SetWindowText(_T("Is a typographical error. Please set to less than 50 characters."));
		
		m_strNewData.Empty();
		m_strVerifyData.Empty();
		
		((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
		((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
		return;
	}
	else if (n_len < 1)
	{
		m_msgPassChg.SetWindowText(_T("Is a typographical error. Please set at least 6 characters."));
		
		m_strNewData.Empty();
		m_strVerifyData.Empty();
		
		((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
		((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
		return;
	}
	
	//wsprintfA(chr_data, "%S", m_strNewData);
	clsFunc.OnStringToChar(m_strNewData, chr_data);

	for (i = 0; i < n_len; i++)
	{
		if (chr_data[i] >= 48 && chr_data[i] <= 57)			// ���� Ȯ��
		{
			n_check_num++;
		}
		else if (chr_data[i] >= 65 && chr_data[i] <= 90)	// ���� Ȯ��
		{
			n_check_string++;
		}
		else
		{
			n_check_extra++;
		}
	}
	
	// ���� ���ڶ� PASSWORD�� ��� �����ϵ����Ѵ�. 
	if (n_check_string < 1)
	{
		m_msgPassChg.SetWindowText(_T("Is a typographical error. Alphabet did not go more than 3 characters."));
		
		m_strNewData.Empty();
		m_strVerifyData.Empty();
		
		((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
		((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
		return;
	}
	
	// ���ڰ� �������� Ȯ���Ѵ�. < ���ڰ� ��� �Ѿ���� 0�� �����Ѵ�. >
	if (n_check_num < 0)
	{
		m_msgPassChg.SetWindowText(_T("Is a typographical error. Numbers did not go longer than 3 characters."));
		
		m_strNewData.Empty();
		m_strVerifyData.Empty();
		
		((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
		((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
		return;
	}
	
	if (n_check_extra > 0)
	{
		m_msgPassChg.SetWindowText(_T("Is a typographical error. Special characters are entered."));
		
		m_strNewData.Empty();
		m_strVerifyData.Empty();
		
		((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
		((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
		return;
	}
	// **************************************************************************

	if (strOtherPassword == m_strNewData)									// �ٸ� ��ȣ�� ���ٸ�.
	{
		m_msgPassChg.SetWindowText(_T("Is a typographical error. Password is the same as the other."));
		
		m_strNewData.Empty();
		m_strVerifyData.Empty();
		
		((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
		((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
		return;
	}
	clsBasic.OnPassWord_Save(st_other_info.strPassLevel, m_strNewData);	// ������ ���� ��ȣ ���� ���� �Լ�

	CDialog::OnOK();
}


void CDialog_Password_Change::OnPassChangeErrorDisplay(CString strText, CString strLevel)
{
	m_msgPassChg.SetWindowText(strText + strLevel);

	m_strOldData.Empty();
	m_strNewData.Empty();
	m_strVerifyData.Empty();

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetWindowText(m_strOldData);
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetWindowText(m_strNewData);
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetWindowText(m_strVerifyData);
}


void CDialog_Password_Change::OnPassChangeRadioStateSet(int nState)
{
	switch(nState)
	{
		case 0 :
			m_btnOld.SetCheck(TRUE);
			m_btnNew.SetCheck(FALSE);
			m_btnVerify.SetCheck(FALSE);
			break;

		case 1 :
			m_btnOld.SetCheck(TRUE);
			m_btnNew.SetCheck(FALSE);
			m_btnVerify.SetCheck(FALSE);
			break;

		case 2 :
			m_btnOld.SetCheck(TRUE);
			m_btnNew.SetCheck(FALSE);
			m_btnVerify.SetCheck(FALSE);
			break;

		default :
			break;
	}
}

void CDialog_Password_Change::OnPassChangePasswordFocus(int nDx)
{
	switch(nDx)
	{
		case 0:  // ���� ��ȣ 
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetFocus();
			break;
		case 1:  // �� ��ȣ 
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetFocus();
			break;
		case 2:  // ��ȣ Ȯ�� 
			((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetFocus();
			break;
		default:
			break;
	}
}

void CDialog_Password_Change::OnClickedCheckNew()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_nPassState = 1; 

	m_btnOld.SetCheck(FALSE);
	m_btnNew.SetCheck(TRUE);
	m_btnVerify.SetCheck(FALSE);

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_NEW))->SetFocus();
}


void CDialog_Password_Change::OnClickedCheckOld()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_nPassState = 0; 

	m_btnOld.SetCheck(TRUE);
	m_btnNew.SetCheck(FALSE);
	m_btnVerify.SetCheck(FALSE);

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_OLD))->SetFocus();
}


void CDialog_Password_Change::OnClickedCheckVerify()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_nPassState = 2; 

	m_btnOld.SetCheck(FALSE);
	m_btnNew.SetCheck(FALSE);
	m_btnVerify.SetCheck(TRUE);

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHG_VERIFY))->SetFocus();
}


BOOL CDialog_Password_Change::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CDC  memDC;                     // �޸� DC
	CBitmap * pOldBitmap = NULL;    // m_BackBitmap���� ���ο� �׸��� DC�� �׸� ��, ���� �׷����� DC(��, Bitmap)�� ����.
	BITMAP bitmapInfo;              // �׸��� ����(m_BackBitmap)
	m_btmBk.GetBitmap(&bitmapInfo); // Bitmap ũ�� ����.

	memDC.CreateCompatibleDC(pDC);  // �޸� DC ����
	pOldBitmap = memDC.SelectObject(&m_btmBk);
	pDC->BitBlt(0, 0, bitmapInfo.bmWidth, bitmapInfo.bmHeight, &memDC, 0, 0, SRCCOPY);   // �޸� DC�� �׸��� �׸�

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();

	return TRUE;
}
