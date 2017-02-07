// Dialog_Exit.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Handler.h"
#include "Dialog_Exit.h"
#include "afxdialogex.h"


// CDialog_Exit 대화 상자입니다.

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


// CDialog_Exit 메시지 처리기입니다.


BOOL CDialog_Exit::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDialog_Exit::OnBnClickedBtnExitC()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
