// Dialog_Exit.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Handler.h"
#include "Dialog_Exit.h"
#include "afxdialogex.h"


// CDialog_Exit ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDialog_Exit, CDialog)

CDialog_Exit::CDialog_Exit(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog_Exit::IDD, pParent)
{

}

CDialog_Exit::~CDialog_Exit()
{
}

void CDialog_Exit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_BTN_EXIT_C, m_btnExitCancel);
	//  DDX_Control(pDX, IDC_BTN_EXIT_C, m_a);
}


BEGIN_MESSAGE_MAP(CDialog_Exit, CDialog)
	ON_BN_CLICKED(IDC_BTN_EXIT_C, &CDialog_Exit::OnBnClickedBtnExitC)
END_MESSAGE_MAP()


// CDialog_Exit �޽��� ó�����Դϴ�.


BOOL CDialog_Exit::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CDialog_Exit::OnBnClickedBtnExitC()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
