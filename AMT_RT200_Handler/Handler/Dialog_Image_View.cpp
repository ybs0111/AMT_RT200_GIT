// Dialog_Image_View.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Handler.h"
#include "Dialog_Image_View.h"
#include "afxdialogex.h"


// CDialog_Image_View ��ȭ �����Դϴ�.

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


// CDialog_Image_View �޽��� ó�����Դϴ�.

BOOL CDialog_Image_View::Create()
{
	return CDialog::Create( CDialog_Image_View::IDD );
}

BOOL CDialog_Image_View::DestroyWindow()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	return CDialog::DestroyWindow();
}


BOOL CDialog_Image_View::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CRect rect;

	GetWindowRect(rect);

	MoveWindow(905, 405, rect.Width(), rect.Height());

	m_imgThumb.Load(m_strFileName);
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CDialog_Image_View::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CDialog::OnPaint()��(��) ȣ������ ���ʽÿ�.
	CDC *pDC;
	CRect rect;

	pDC = GetDlgItem(IDC_IMAGE_VIEW)->GetDC();
	GetDlgItem(IDC_IMAGE_VIEW)->GetWindowRect(rect);
	m_imgThumb.Show(pDC, CPoint(0, 0), CPoint(rect.Width(), rect.Height()), 0,0);

	ReleaseDC(pDC);
}
