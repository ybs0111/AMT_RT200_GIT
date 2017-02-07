// Dialog_Image_View.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Handler.h"
#include "Dialog_Image_View.h"
#include "afxdialogex.h"


// CDialog_Image_View 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialog_Image_View, CDialog)

CDialog_Image_View::CDialog_Image_View(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog_Image_View::IDD, pParent)
{

}

CDialog_Image_View::~CDialog_Image_View()
{
}

void CDialog_Image_View::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialog_Image_View, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDialog_Image_View 메시지 처리기입니다.

BOOL CDialog_Image_View::Create()
{
	return CDialog::Create( CDialog_Image_View::IDD );
}

BOOL CDialog_Image_View::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialog::DestroyWindow();
}


BOOL CDialog_Image_View::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CRect rect;

	GetWindowRect(rect);

	MoveWindow(905, 405, rect.Width(), rect.Height());

	m_imgThumb.Load(m_strFileName);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDialog_Image_View::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.
	CDC *pDC;
	CRect rect;

	pDC = GetDlgItem(IDC_IMAGE_VIEW)->GetDC();
	GetDlgItem(IDC_IMAGE_VIEW)->GetWindowRect(rect);
	m_imgThumb.Show(pDC, CPoint(0, 0), CPoint(rect.Width(), rect.Height()), 0,0);

	ReleaseDC(pDC);
}
