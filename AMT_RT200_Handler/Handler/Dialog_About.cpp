// Dialog_About.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Handler.h"
#include "Dialog_About.h"
#include "afxdialogex.h"
#include "PublicFunction.h"
#include "Variable.h"


// CDialog_About 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialog_About, CDialog)

CDialog_About::CDialog_About(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog_About::IDD, pParent)
{

}

CDialog_About::~CDialog_About()
{
}

void CDialog_About::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_MATRIX, mp_imgAbout);

	DDX_Control(pDX, IDC_BTN_UPDATE,				m_btnUdate);
	DDX_Control(pDX, IDC_BTN_ABOUT_OK,				m_btnAboutOK);
}

BEGIN_MESSAGE_MAP(CDialog_About, CDialog)
	ON_BN_CLICKED(IDC_BTN_UPDATE, &CDialog_About::OnBnClickedBtnUpdate)
	ON_BN_CLICKED(IDC_BTN_ABOUT_OK, &CDialog_About::OnBnClickedBtnAboutOk)
END_MESSAGE_MAP()

void CDialog_About::OnSetButton()
{
	m_btnUdate.SetBitmaps(IDC_BTN_UPDATE, IDB_BITMAP_CREATE_DN, WINDOW_DN, IDB_BITMAP_CREATE_UP, WINDOW_UP);
	m_btnUdate.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnUdate.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnUdate.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnUdate.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnUdate.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnUdate.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnUdate.SetFont(clsFunc.m_pFont[3]);
	m_btnUdate.SetTooltipText(_T("About Box Update"));

	m_btnAboutOK.SetBitmaps(IDC_BTN_ABOUT_OK, IDB_OK_DN, WINDOW_DN1, IDB_OK_UP, WINDOW_UP1);
	m_btnAboutOK.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnAboutOK.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnAboutOK.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnAboutOK.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnAboutOK.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnAboutOK.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnAboutOK.SetFont(clsFunc.m_pFont[3]);
	m_btnAboutOK.SetTooltipText(_T("About Box OK"));
}

//==================================================================//
// 스타워즈 컨트롤 생성
//==================================================================//
void CDialog_About::OnSetUserControl() 
{
	mp_imgAbout.SetStarSpeed(30);	// 백 그라운드 (별 모양) 움직이는 속도
	mp_imgAbout.SetScrollSpeed(2);	// 버전 정보가 움직이는 속도 설정

	mp_imgAbout.AddTextLine(_T("This Application is"));
	mp_imgAbout.AddTextLine(_T(""));
	mp_imgAbout.AddTextLine(_T("MODULE Auto Labeler"));
	mp_imgAbout.AddTextLine(_T(""));
	mp_imgAbout.AddTextLine(_T("Motion Control System"));
	mp_imgAbout.AddTextLine(_T("&"));
	mp_imgAbout.AddTextLine(_T("Human Machine Interface"));
	mp_imgAbout.AddTextLine(_T(""));
	mp_imgAbout.AddTextLine(_T(""));
	mp_imgAbout.AddTextLine(_T("Manufacturing Date : 2016-04-15"));
	mp_imgAbout.AddTextLine(_T("Update Info        : 2016-04-15"));
	mp_imgAbout.AddTextLine(_T("Development a company : AMT"));
	mp_imgAbout.AddTextLine(_T(""));
	mp_imgAbout.AddTextLine(_T("This application was programmed by"));
	mp_imgAbout.AddTextLine(_T(""));
	mp_imgAbout.AddTextLine(_T("Control 3 Teem "));
	mp_imgAbout.AddTextLine(_T(""));
	mp_imgAbout.AddTextLine(_T("Copyright(C) 2014.05.15"));
	mp_imgAbout.AddTextLine(_T("Advanced Mechatronics Tech'. Co."));
	mp_imgAbout.AddTextLine(_T(""));
	mp_imgAbout.AddTextLine(_T("http://www.imamt.com"));
}

BOOL CDialog_About::OnInitDialog()
{
	CDialog::OnInitDialog();

	OnSetButton();  // 칼라 버튼 생성

	OnSetUserControl();  // 스타워즈 컨트롤 생성

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDialog_About::OnBnClickedBtnUpdate()
{
	CString sUpdateInfoFile = _T("C:\\AMT8562\\System\\Update.Info");

	CFileFind find;
	if (find.FindFile(sUpdateInfoFile.GetBuffer(0)) != 0)  // 파일 존재 유무 검사
	{
		CString sExcuteCmd;
		sExcuteCmd.Format(_T("notepad %s"), sUpdateInfoFile);

		TCHAR tchExcute[MAX_PATH];
		STARTUPINFO si = {0,};
		si.cb = sizeof(si);

		PROCESS_INFORMATION pi;
// jtkim 20130704
//		_tcscpy(tchExcute, sExcuteCmd);
		_tcscpy_s(tchExcute, sizeof(tchExcute), sExcuteCmd);

		CreateProcess(NULL, (LPWSTR)(LPCSTR)tchExcute, NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
	}
	sUpdateInfoFile.ReleaseBuffer();
}


void CDialog_About::OnBnClickedBtnAboutOk()
{
	CDialog::OnOK();
}
