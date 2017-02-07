// Dialog_Pass_Check.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Dialog_Pass_Check.h"
#include "afxdialogex.h"
#include "PublicFunction.h"
#include "Variable.h"
#include "MyBasicData.h"
#include "AlgMemory.h"


// CDialog_Pass_Check dialog

IMPLEMENT_DYNAMIC(CDialog_Pass_Check, CDialog)

CDialog_Pass_Check::CDialog_Pass_Check(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog_Pass_Check::IDD, pParent)
{
	m_strPassword	= _T("");
}

CDialog_Pass_Check::~CDialog_Pass_Check()
{
}

void CDialog_Pass_Check::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_ADD, m_btnPassAdd);
	DDX_Control(pDX, IDC_TITLE_PASS_CHK, m_msgTitlePass);
	DDX_Control(pDX, IDC_MSG_PASS_CHK, m_msgPass);
	DDX_Control(pDX, IDC_GROUP_PASS_CHK, m_groupPass);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_SHARP, m_btnPassSharp);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_RBRACE_S, m_btnPassRbraceS);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_RBRACE_B, m_btnPassRbraceB);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_LINE, m_btnPassLine);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_LBRACE_S, m_btnPassLbraceS);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_LBRACE_B, m_btnPassLbraceB);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_DOT, m_btnPassDot);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_DOLLAR, m_btnPassDollar);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_DIV, m_btnPassDiv);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_DEL, m_btnPassDel);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_BACK, m_btnPassBack);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_Z, m_btnPassZ);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_Y, m_btnPassY);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_X, m_btnPassX);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_W, m_btnPassW);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_V, m_btnPassV);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_U, m_btnPassU);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_T, m_btnPassT);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_S, m_btnPassS);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_R, m_btnPassR);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_Q, m_btnPassQ);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_P, m_btnPassP);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_O, m_btnPassO);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_N, m_btnPassN);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_M, m_btnPassM);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_L, m_btnPassL);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_K, m_btnPassK);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_J, m_btnPassJ);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_I, m_btnPassI);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_H, m_btnPassH);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_G, m_btnPassG);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_F, m_btnPassF);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_E, m_btnPassE);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_D, m_btnPassD);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_C, m_btnPassC);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_B, m_btnPassB);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_A, m_btnPassA);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_9, m_btnPass9);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_8, m_btnPass8);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_7, m_btnPass7);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_6, m_btnPass6);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_5, m_btnPass5);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_4, m_btnPass4);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_3, m_btnPass3);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_2, m_btnPass2);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_1, m_btnPass1);
	DDX_Control(pDX, IDC_BTN_PASS_CHK_0, m_btnPass0);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CDialog_Pass_Check, CDialog)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_0, &CDialog_Pass_Check::OnClickedBtnPassChk0)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_1, &CDialog_Pass_Check::OnClickedBtnPassChk1)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_2, &CDialog_Pass_Check::OnClickedBtnPassChk2)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_3, &CDialog_Pass_Check::OnClickedBtnPassChk3)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_4, &CDialog_Pass_Check::OnClickedBtnPassChk4)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_5, &CDialog_Pass_Check::OnClickedBtnPassChk5)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_6, &CDialog_Pass_Check::OnClickedBtnPassChk6)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_7, &CDialog_Pass_Check::OnClickedBtnPassChk7)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_8, &CDialog_Pass_Check::OnClickedBtnPassChk8)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_9, &CDialog_Pass_Check::OnClickedBtnPassChk9)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_A, &CDialog_Pass_Check::OnClickedBtnPassChkA)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_ADD, &CDialog_Pass_Check::OnClickedBtnPassChkAdd)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_B, &CDialog_Pass_Check::OnClickedBtnPassChkB)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_BACK, &CDialog_Pass_Check::OnClickedBtnPassChkBack)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_C, &CDialog_Pass_Check::OnClickedBtnPassChkC)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_D, &CDialog_Pass_Check::OnClickedBtnPassChkD)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_DEL, &CDialog_Pass_Check::OnClickedBtnPassChkDel)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_DIV, &CDialog_Pass_Check::OnClickedBtnPassChkDiv)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_DOLLAR, &CDialog_Pass_Check::OnClickedBtnPassChkDollar)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_DOT, &CDialog_Pass_Check::OnClickedBtnPassChkDot)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_E, &CDialog_Pass_Check::OnClickedBtnPassChkE)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_F, &CDialog_Pass_Check::OnClickedBtnPassChkF)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_G, &CDialog_Pass_Check::OnClickedBtnPassChkG)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_H, &CDialog_Pass_Check::OnClickedBtnPassChkH)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_I, &CDialog_Pass_Check::OnClickedBtnPassChkI)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_J, &CDialog_Pass_Check::OnClickedBtnPassChkJ)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_K, &CDialog_Pass_Check::OnClickedBtnPassChkK)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_L, &CDialog_Pass_Check::OnClickedBtnPassChkL)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_LBRACE_B, &CDialog_Pass_Check::OnClickedBtnPassChkLbraceB)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_LBRACE_S, &CDialog_Pass_Check::OnClickedBtnPassChkLbraceS)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_LINE, &CDialog_Pass_Check::OnClickedBtnPassChkLine)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_M, &CDialog_Pass_Check::OnClickedBtnPassChkM)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_N, &CDialog_Pass_Check::OnClickedBtnPassChkN)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_O, &CDialog_Pass_Check::OnClickedBtnPassChkO)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_P, &CDialog_Pass_Check::OnClickedBtnPassChkP)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_Q, &CDialog_Pass_Check::OnClickedBtnPassChkQ)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_R, &CDialog_Pass_Check::OnClickedBtnPassChkR)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_RBRACE_B, &CDialog_Pass_Check::OnClickedBtnPassChkRbraceB)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_RBRACE_S, &CDialog_Pass_Check::OnClickedBtnPassChkRbraceS)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_S, &CDialog_Pass_Check::OnClickedBtnPassChkS)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_SHARP, &CDialog_Pass_Check::OnClickedBtnPassChkSharp)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_T, &CDialog_Pass_Check::OnClickedBtnPassChkT)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_U, &CDialog_Pass_Check::OnClickedBtnPassChkU)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_V, &CDialog_Pass_Check::OnClickedBtnPassChkV)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_W, &CDialog_Pass_Check::OnClickedBtnPassChkW)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_X, &CDialog_Pass_Check::OnClickedBtnPassChkX)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_Y, &CDialog_Pass_Check::OnClickedBtnPassChkY)
	ON_BN_CLICKED(IDC_BTN_PASS_CHK_Z, &CDialog_Pass_Check::OnClickedBtnPassChkZ)
	ON_BN_CLICKED(IDOK, &CDialog_Pass_Check::OnBnClickedOk)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDCANCEL, &CDialog_Pass_Check::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDialog_Pass_Check message handlers


BOOL CDialog_Pass_Check::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	return CDialog::DestroyWindow();
}


BOOL CDialog_Pass_Check::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	OnInitGroupBox();
	OnInitLabel();

	// System Lock 관련 암호 정보 로딩 함수
//	if (st_other_info.strPassLevel!= "SystemLock")  OnPassChkPassDataLoad();
	if (m_nPassLevel!= 2)  OnPassChkPassDataLoad();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDialog_Pass_Check::OnClickedBtnPassChk0()
{
	// TODO: Add your control notification handler code here
	if(!OnPassChkLengthCheck())
	{
		return;
	}

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("0") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChk1()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("1") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChk2()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("2") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChk3()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("3") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChk4()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("4") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChk5()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("5") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChk6()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("6") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChk7()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("7") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChk8()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("8") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChk9()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("9") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkA()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("A") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkAdd()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("+") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkB()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("B") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkBack()
{
	// TODO: Add your control notification handler code here
	int nCnt;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);
	nCnt = m_strPassword.GetLength() ;

	if (nCnt == 0) return;

	m_strPassword = m_strPassword.Left(nCnt-1) ;
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkC()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("C") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkD()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("D") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkDel()
{
	// TODO: Add your control notification handler code here
	m_strPassword.Empty();
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkDiv()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("/") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkDollar()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("$") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkDot()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T(".") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkE()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("E") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkF()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("F") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkG()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("G") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkH()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("H") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkI()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("I") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkJ()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("J") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkK()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("K") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkL()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("L") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkLbraceB()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("[") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkLbraceS()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("(") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkLine()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("-") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkM()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("M") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkN()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("N") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkO()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("O") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkP()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("P") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkQ()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("Q") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkR()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("R") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkRbraceB()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("]") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkRbraceS()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T(")") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkS()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("S") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkSharp()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("#") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkT()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("T") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkU()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("U") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkV()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("V") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkW()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("W") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkX()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("X") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkY()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("Y") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnClickedBtnPassChkZ()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword +=_T("Z") ;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
}


void CDialog_Pass_Check::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);
	m_strPassword.MakeUpper();
	m_strPassword.TrimLeft(' ');               
	m_strPassword.TrimRight(' ');

	switch(m_nPassLevel)
	{
		case 0:						// System Lock Mode
			clsBasic.OnPassWord_Save(st_other_info.strPassLevel, m_strPassword);		// System Lock 암호 정보 저장 함수
			break;

		case 1:						// System Unlock Mode
			if (m_strTemp == m_strPassword)
			{

			}
			else														//이도 저도 아닐때...
			{
				m_msgPass.SetWindowText(_T("Invalid Password."));

				m_strPassword.Empty();

				((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
				((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetFocus();

				if (st_handler_info.cWndList != NULL)
				{
					//clsFunc.OnStringToChar(_T("Invalid Password."), st_other_info.cAbnormalMsg);
					clsMem.OnAbNormalMessagWrite(_T("Invalid Password"));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				return;
			}

			break;

		case 2:
			if (!st_handler_info.nSystemLock)						// 암호가 Level1일때...
			{
				st_handler_info.nSystemLock = TRUE;						// 메인트 레벨(Level 1) 플래그 설정
				clsBasic.OnPassWord_Save(_T("SysLock"), m_strPassword);
				
				if (st_handler_info.cWndList != NULL)
				{
					//clsFunc.OnStringToChar(_T("System Mode (System Lock Mode."), st_other_info.cAbnormalMsg);
					clsMem.OnNormalMessageWrite(_T("System Mode (System Lock Mode)"));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
				}
				st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, LEVEL_MODE, 0);
			}
			break;

		case 3:
			if (m_strSystemLock == m_strPassword)						// 암호가 Level1일때...
			{
				st_handler_info.nSystemLock = FALSE;						// 메인트 레벨(Level 1) 플래그 설정

				if (st_handler_info.cWndList != NULL)
				{
					//wsprintfA(st_other_info.cAbnormalMsg, "%S", _T("System Mode (System Lock Mode)"));
					//clsFunc.OnStringToChar(_T("System Mode (System UnLock Mode."), st_other_info.cAbnormalMsg);
					clsMem.OnNormalMessageWrite(_T("System Mode (System Lock Mode)"));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
				}
				st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, LEVEL_MODE, 0);
				::PostMessage(st_handler_info.hWnd, WM_FORM_CHANGE, SCREEN_MAIN, 0);
			}
			else
			{
				m_msgPass.SetWindowText(_T("Invalid Password."));

				m_strPassword.Empty();
				((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
				((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetFocus();

				st_handler_info.nLevelMaint = FALSE;						// 메인트 레벨(Level 1) 플래그 초기화
				st_handler_info.nLevelTeach = FALSE;						// 티칭 레벨(Level 2) 플래그 초기화	

				if (st_handler_info.cWndList != NULL)
				{
					//clsFunc.OnStringToChar(_T("Invalid Password."), st_other_info.cAbnormalMsg);
					clsMem.OnAbNormalMessagWrite(_T("Invalid Password"));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}

				return;
			}
			break;

		case 4:
			if(m_strLevel1 == m_strPassword)						// 암호가 Level1일때...
			{
				st_handler_info.nLevelMaint = TRUE;						// 메인트 레벨(Level 1) 플래그 설정
			
				if (st_handler_info.cWndList != NULL)
				{
					//clsFunc.OnStringToChar(_T("Level 1 Mode (Maintenance Mode"), st_other_info.cAbnormalMsg);
					clsMem.OnNormalMessageWrite(_T("Level 1 Mode (Maintenance Mode)"));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
				}

				st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, LEVEL_MODE, 0);
			}
			else if (m_strLevel2 == m_strPassword)	//암호가 Programmer 모드일때...
			{
				st_handler_info.nLevelTeach = TRUE;						// 티칭 레벨(Level 2) 설정 플래그 초기화
			
				if (st_handler_info.cWndList != NULL)
				{
					//wsprintfA(st_other_info.cAbnormalMsg, "%S", _T("Admin Mode (Machine Administrator Mode)"));
					//clsFunc.OnStringToChar(_T("Admin Mode (Machine Administrator Mode"), st_other_info.cAbnormalMsg);
					clsMem.OnNormalMessageWrite(_T("Admin Mode (Machine Administrator Mode)"));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
				}

				st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, LEVEL_MODE, 0);
			}
			else
			{
				m_msgPass.SetWindowText(_T("Invalid Password"));

				m_strPassword.Empty();
				((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
				((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetFocus();

				st_handler_info.nLevelMaint = FALSE;						// 메인트 레벨(Level 1) 플래그 초기화
				st_handler_info.nLevelTeach = FALSE;						// 티칭 레벨(Level 2) 플래그 초기화	

				if (st_handler_info.cWndList != NULL)
				{
					//clsFunc.OnStringToChar(_T("Invalid Password."), st_other_info.cAbnormalMsg);
					clsMem.OnAbNormalMessagWrite(_T("Invalid Password"));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}

				return;
			}
			break;

		case 5:
			if (m_strLevel1 == m_strPassword)	//암호가 Programmer 모드일때...
			{
				st_handler_info.nLevelTeach = TRUE;						// 티칭 레벨(Level 2) 설정 플래그 초기화
			
				if (st_handler_info.cWndList != NULL)
				{
					//wsprintfA(st_other_info.cAbnormalMsg, "%S", _T("Admin Mode (Machine Administrator Mode)"));
					//clsFunc.OnStringToChar(_T("Admin Mode (Machine Administrator Mode"), st_other_info.cAbnormalMsg);
					clsMem.OnNormalMessageWrite(_T("Admin Mode (Machine Administrator Mode)"));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
				}

				st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, LEVEL_MODE, 0);
			}
			else if(m_strLevel2 == m_strPassword)						// 암호가 Level1일때...
			{
				st_handler_info.nLevelMaint = TRUE;						// 메인트 레벨(Level 1) 플래그 설정
			
				if (st_handler_info.cWndList != NULL)
				{
					//clsFunc.OnStringToChar(_T("Level 1 Mode (Maintenance Mode"), st_other_info.cAbnormalMsg);
					clsMem.OnNormalMessageWrite(_T("Level 1 Mode (Maintenance Mode)"));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
				}

				st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, LEVEL_MODE, 0);
			}
			else
			{
				m_msgPass.SetWindowText(_T("Invalid Password."));

				m_strPassword.Empty();
				((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
				((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetFocus();

				st_handler_info.nLevelMaint = FALSE;						// 메인트 레벨(Level 1) 플래그 초기화
				st_handler_info.nLevelTeach = FALSE;						// 티칭 레벨(Level 2) 플래그 초기화	

				if (st_handler_info.cWndList != NULL)
				{
					//clsFunc.OnStringToChar(_T("암호가 일치하지 않습니다."), st_other_info.cAbnormalMsg);
					clsMem.OnAbNormalMessagWrite(_T("Invalid Password"));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}

				return;
			}
			break;

		case 6:
			if ("AMT_MASTER" == m_strPassword || "M" == m_strPassword)
			{
				st_handler_info.nLevelSpeed = TRUE;

				if (st_handler_info.cWndList != NULL)
				{
					//clsFunc.OnStringToChar(_T("Speed Mode (Machine Speed Mode"), st_other_info.cAbnormalMsg);
					clsMem.OnNormalMessageWrite(_T("Speed Mode (Machine Speed Mode)"));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
				}
			}
			else 
			{
				m_msgPass.SetWindowText(_T("Invalid Password"));

				m_strPassword.Empty();
				((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
				((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetFocus();

				st_handler_info.nLevelMaint = FALSE;						// 메인트 레벨(Level 1) 플래그 초기화
				st_handler_info.nLevelTeach = FALSE;						// 티칭 레벨(Level 2) 플래그 초기화	

				if (st_handler_info.cWndList != NULL)
				{
					//clsFunc.OnStringToChar(_T("Invalid Password"), st_other_info.cAbnormalMsg);
					clsMem.OnAbNormalMessagWrite(_T("Invalid Password"));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				return;
			}
			break;

		case 7:
			if ("ENGR" == m_strPassword)
			{
				st_handler_info.nLevelSpeed = TRUE;

				if (st_handler_info.cWndList != NULL)
				{
					//clsFunc.OnStringToChar(_T("Speed Mode (Machine Speed Mode"), st_other_info.cAbnormalMsg);
					clsMem.OnNormalMessageWrite(_T("Speed Mode (Machine Speed Mode)"));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
				}
			}
			else 
			{
				m_msgPass.SetWindowText(_T("Invalid Password"));

				m_strPassword.Empty();
				((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetWindowText(m_strPassword);
				((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->SetFocus();

				st_handler_info.nLevelMaint = FALSE;						// 메인트 레벨(Level 1) 플래그 초기화
				st_handler_info.nLevelTeach = FALSE;						// 티칭 레벨(Level 2) 플래그 초기화	

				if (st_handler_info.cWndList != NULL)
				{
					//clsFunc.OnStringToChar(_T("Invalid Password"), st_other_info.cAbnormalMsg);
					clsMem.OnAbNormalMessagWrite(_T("Invalid Password"));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}

				return;
			}
			break;
	}

	CDialog::OnOK();
}

void CDialog_Pass_Check::OnPassChkPassDataLoad()
{
	m_strAdmin.Empty();		// 암호 임시 저장 변수 초기화
	m_strSystemLock.Empty();// 시스템 롹 저장 변수 초기화 
	m_strLevel1.Empty();	// 설정 암호 임시 저장 변수 초기화 
	m_strLevel2.Empty();	// 설정 암호 임시 저장 변수 초기화 

	mcls_pass_chk.OnPassWord_Load(_T("SysLock"));			// 레벨에 따른 암호 정보 로딩 함수

	if ((st_other_info.strPassword).IsEmpty())  m_msgPass.SetWindowText(_T("SysLock"));
	else	m_strSystemLock = st_other_info.strPassword;	// 전역 변수에 저장된 암호 정보 클래스 변수에 설정 

	mcls_pass_chk.OnPassWord_Load(_T("Level1"));			// 레벨에 따른 암호 정보 로딩 함수

	if ((st_other_info.strPassword).IsEmpty())  m_msgPass.SetWindowText(_T("Haven't Password"));
	else	m_strLevel1 = st_other_info.strPassword;	// 전역 변수에 저장된 암호 정보 클래스 변수에 설정 

	mcls_pass_chk.OnPassWord_Load(_T("Level2"));			// 레벨에 따른 암호 정보 로딩 함수

	if ((st_other_info.strPassword).IsEmpty())  m_msgPass.SetWindowText(_T("Haven't Password"));
	else	m_strLevel2 = st_other_info.strPassword;	// 전역 변수에 저장된 암호 정보 클래스 변수에 설정 

	mcls_pass_chk.OnPassWord_Load(_T("SysLock"));				// 레벨에 따른 암호 로딩 함수

	if ((st_other_info.strPassword).IsEmpty())	m_msgPass.SetWindowText(_T("Haven't Password"));
	else	m_strAdmin = st_other_info.strPassword;		// 암호 정보 클래스 변수에 설정
}

void CDialog_Pass_Check::OnInitLabel()
{
	m_msgPass.SetFont(clsFunc.m_pFont[1]);
	m_msgPass.SetWindowText(_T("Input Password"));
	m_msgPass.SetCenterText();
	m_msgPass.SetColor(RGB(0,0,255));
	m_msgPass.SetGradientColor(RGB(0,0,0));
	m_msgPass.SetTextColor(RGB(255,255,255));

	m_msgTitlePass.SetFont(clsFunc.m_pFont[1]);
	m_msgTitlePass.SetWindowText(_T("=>"));
	m_msgTitlePass.SetCenterText();
	m_msgTitlePass.SetColor(RGB(0,0,0));
	m_msgTitlePass.SetGradientColor(RGB(0,0,0));
	m_msgTitlePass.SetTextColor(RGB(255,255,255));
}

void CDialog_Pass_Check::OnInitGroupBox()
{
	m_groupPass.SetFont(clsFunc.OnLogFont(16));
	m_groupPass.SetCatptionTextColor(BLUE_C);
	m_groupPass.SetBorderColor(ORANGE_C);
	m_groupPass.SetFontBold(TRUE);
	m_groupPass.SetBackgroundColor(WINDOW_UP1);
}

bool CDialog_Pass_Check::OnPassChkLengthCheck()
{
	int nCnt;

	((CEdit*)GetDlgItem(IDC_EDIT_PASS_CHK_DATA))->GetWindowText(m_strPassword);

	m_strPassword.MakeUpper();
	m_strPassword.TrimLeft(' ');               
	m_strPassword.TrimRight(' ');

	nCnt = m_strPassword.GetLength();
	if (nCnt >= 15) return false;

	return true;
}

BOOL CDialog_Pass_Check::OnEraseBkgnd(CDC* pDC)
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


void CDialog_Pass_Check::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnCancel();
}
