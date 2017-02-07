// ScreenLock.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Handler.h"
#include "ScreenLock.h"
#include "Dialog_Pass_Check.h" 
#include "Variable.h"
#include "PublicFunction.h"
//==================================================================//
// ��� ��� ��ġ ����
enum
{
	ORIENT_LEFT   = 1,
	ORIENT_UP     = 2,
	ORIENT_BOTTOM = 3,
	ORIENT_RIGHT  = 4,
};

enum
{
	COLOR_BACK = 1,
	COLOR_FOR  = 2,
};
//------------------------------------------------------------------//

// CScreenLock

IMPLEMENT_DYNCREATE(CScreenLock, CFormView)

CScreenLock::CScreenLock()
	: CFormView(CScreenLock::IDD)
{

}

CScreenLock::~CScreenLock()
{
}

void CScreenLock::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_SYSLOCK_GIF, mp_gifSysLock);
	DDX_Control(pDX, IDC_STC_WARNING, mp_stcWarning);
	DDX_Control(pDX, IDC_BTN_SYSUNLOCK, m_btnSysUnLock);
}

BEGIN_MESSAGE_MAP(CScreenLock, CFormView)
	ON_BN_CLICKED(IDC_BTN_SYSUNLOCK, &CScreenLock::OnBnClickedBtnSysunlock)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CScreenLock �����Դϴ�.

#ifdef _DEBUG
void CScreenLock::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CScreenLock::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CScreenLock �޽��� ó�����Դϴ�.


void CScreenLock::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	if (st_handler_info.nSystemLock != TRUE)	
	{
		st_handler_info.nSystemLock = TRUE;
	}

	OnInitButton();

	OnSetPicture(); 
	OnSetBanner();  // Į�� ��� ����

	st_handler_info.cWndLock = this;
}

//==================================================================//
// GIF �ε�
//==================================================================//
void CScreenLock::OnSetPicture() 
{
	if (mp_gifSysLock.Load(MAKEINTRESOURCE(IDR_GIF_LOCK), _T("GIF")))
	{
		mp_gifSysLock.Draw();
	}
}

void CScreenLock::OnInitButton()
{
	m_btnSysUnLock.SetBitmaps(IDC_BTN_APPLY, IDB_BITMAP_UNLOCK, WINDOW_DN, IDB_BITMAP_UNLOCK, WINDOW_UP);
	m_btnSysUnLock.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnSysUnLock.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnSysUnLock.SetColor(CButtonST::BTNST_COLOR_FG_IN, ORANGE_C);
	m_btnSysUnLock.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLACK_C);
	m_btnSysUnLock.SetFont(clsFunc.m_pFont[2]);
	m_btnSysUnLock.SetTooltipText(_T("System UnLock"));
}
/*
//==================================================================//
// Į�� ��� ����
//==================================================================//
void CScreenLock::uSetBanner() 
{
	//==============================================================//
	// Į�� ��� ����
	m_crBackColor = RGB(255,255,255);
	m_crForeColor = RGB(255,0,0);
	mp_stcWarning.SetBackColor(m_crBackColor);
	mp_stcWarning.SetOrientation(ORIENTATION_HORIZONTAL);
	//--------------------------------------------------------------//

	// ���Ŀ� ��� ���� ���ڿ� �� �ʿ��� �κ� ������ ��� �߻����� ���� �ϴ� �߰�
	m_nCount=0;

	//==============================================================//
	// ��� ��Ʈ�� ��� ���� ����
	CRect rect;
	this->GetWindowRect(&rect);
	m_nWidth  = rect.Width();
	m_nHeight = rect.Height();

	mp_stcWarning.GetWindowRect(&rect);

	m_nBannerWidth  = rect.Width();
	m_nBannerHeight = rect.Height();
	m_nBannerTop  = rect.top;
	m_nBannerLeft = rect.left;
	//--------------------------------------------------------------//

	OnBannerRemoveAll();  // ��ʷ� ��µ� ��� ���ڿ� ����

	CString sBanner =_T("System Locking!!     ");
	OnAddString(sBanner);  // ��� ���ڿ� �߰�

	// ���Ŀ��� ������ ����� ���� �޽����� �Է¹޾� ó���ϵ����Ѵ�
	// �ϴ��� �ϵ��ڵ����� ó���Ѵ�
	OnChangeColor(COLOR_FOR, RGB(0,0,175));  // ��� Į�� ����
	sBanner =_T("Now TESTING.. DON'T TOUCH     ");
	//sBanner  = (stOTHER.strPassComment);
	sBanner += _T("     ");
	OnAddString(sBanner);

	m_nSpeed=1000;
	OnSetScrollSpeed(m_nSpeed);  // ��� ��ũ�� �ӵ� ����
}
*/
//==================================================================//
// ���� �ʱ�ȭ
//==================================================================//
void CScreenLock::OnSetInitVariable() 
{
	m_bBold = FALSE;
	m_bItalic = FALSE;
	m_bUnderlined = FALSE;
}
//==================================================================//
// Į�� ��� ����
//==================================================================//
void CScreenLock::OnSetBanner() 
{
	//==============================================================//
	// Į�� ��� ����
	//m_crBackColor = RGB(0,0,255);
	//m_crForeColor = RGB(255,0,0);
	m_crBackColor = RGB(255,255,255);
	m_crForeColor = RGB(255,0,0);
	mp_stcWarning.SetBackColor(m_crBackColor);
	mp_stcWarning.SetOrientation(ORIENTATION_HORIZONTAL);
	//--------------------------------------------------------------//

	// ���Ŀ� ��� ���� ���ڿ� �� �ʿ��� �κ� ������ ��� �߻����� ���� �ϴ� �߰�
	m_nCount=0;

	//==============================================================//
	// ��� ��Ʈ�� ��� ���� ����
	CRect rect;
	this->GetWindowRect(&rect);
	m_nWidth  = rect.Width();
	m_nHeight = rect.Height();

	mp_stcWarning.GetWindowRect(&rect);

	m_nBannerWidth  = rect.Width();
	m_nBannerHeight = rect.Height();
	m_nBannerTop  = rect.top;
	m_nBannerLeft = rect.left;
	//--------------------------------------------------------------//

	OnBannerRemoveAll();  // ��ʷ� ��µ� ��� ���ڿ� ����

	CString sBanner =_T("System Locking!!     ");
	OnAddString(sBanner);  // ��� ���ڿ� �߰�

	// ���Ŀ��� ������ ����� ���� �޽����� �Է¹޾� ó���ϵ����Ѵ�
	// �ϴ��� �ϵ��ڵ����� ó���Ѵ�
	OnChangeColor(COLOR_FOR, RGB(0,0,175));  // ��� Į�� ����
	sBanner =_T("Now TESTING.. DON'T TOUCH     ");
	//sBanner  = (stOTHER.strPassComment);
	sBanner += _T("     ");
	OnAddString(sBanner);

	m_nSpeed=600;
	OnSetScrollSpeed(m_nSpeed);  // ��� ��ũ�� �ӵ� ����
}

//==================================================================//
// ��� ��ũ�� �ӵ� ����
//==================================================================//
void CScreenLock::OnSetScrollSpeed(int nzSpeed) 
{
	mp_stcWarning.SetScrollSpeed(nzSpeed);
}

//==================================================================//
// ��� Į�� ����
// : Fore Color ������ ������ �߰��� ���ڿ��� ����ȴ�
// : �׷��Ƿ� Fore Color �����Ŀ� AddString���� ���ڿ� �߰��Ѵ�
// : �ʿ��ϴٸ� Į�� ���� ��ȭ���ڷ� Į�� ���� �Է¹޾Ƶ� �ȴ�
//==================================================================//
void CScreenLock::OnChangeColor(int nzType, COLORREF crzColor) 
{
	switch(nzType)
	{
	case COLOR_BACK:
		m_crBackColor = crzColor;
		mp_stcWarning.SetBackColor(m_crBackColor);
		break;
	case COLOR_FOR:
		m_crForeColor = crzColor;
		break;

	default :
		break;
	}
}

//==================================================================//
// ��� ��� ���� ����
//==================================================================//
void CScreenLock::OnChangeOrientation(int nzOrient) 
{
	switch(nzOrient)
	{
	case ORIENT_LEFT:
		mp_stcWarning.SetWindowPos(NULL, m_nBannerLeft-3, 10, m_nBannerHeight, m_nBannerWidth, SWP_NOZORDER);
		mp_stcWarning.SetOrientation(ORIENTATION_VERTICAL_LEFT);
		mp_stcWarning.SetScrollSpeed(m_nSpeed);
		break;
	case ORIENT_UP:
		mp_stcWarning.SetWindowPos(NULL, m_nBannerLeft-3, 10, m_nBannerWidth, m_nBannerHeight, SWP_NOZORDER);
		mp_stcWarning.SetOrientation(ORIENTATION_HORIZONTAL);
		mp_stcWarning.SetScrollSpeed(m_nSpeed);
		break;
	case ORIENT_BOTTOM:
		mp_stcWarning.SetWindowPos(NULL, m_nBannerLeft-3, m_nHeight-m_nBannerHeight-m_nBannerTop, m_nBannerWidth, m_nBannerHeight, SWP_NOZORDER);
		mp_stcWarning.SetOrientation(ORIENTATION_HORIZONTAL);
		mp_stcWarning.SetScrollSpeed(m_nSpeed);
		break;
	case ORIENT_RIGHT:
		mp_stcWarning.SetWindowPos(NULL, m_nWidth-m_nBannerHeight-m_nBannerLeft, 10, m_nBannerHeight, m_nBannerWidth, SWP_NOZORDER);
		mp_stcWarning.SetOrientation(ORIENTATION_VERTICAL_RIGHT);
		mp_stcWarning.SetScrollSpeed(-m_nSpeed);
		break;

	default :
		break;
	}
	UpdateData(true);
}


//==================================================================//
// ��ʷ� ��µ� ��� ���ڿ� ����
//==================================================================//
void CScreenLock::OnBannerRemoveAll() 
{
	UpdateData(true);
	mp_stcWarning.RemoveAllStrings();
	m_nCount=0;
	UpdateData(false);
}

//==================================================================//
// ��� ���ڿ� �߰�
//==================================================================//
void CScreenLock::OnAddString(CString szBanner) 
{
	CString sAddString =_T("");
	sAddString = szBanner;

	UpdateData(true);
	DWORD dwAddStyle = m_crForeColor;
	dwAddStyle |= (m_bItalic ? COLORSTRING_ITALIC : 0);
	dwAddStyle |= (m_bBold ? COLORSTRING_BOLD : 0);
	dwAddStyle |= (m_bUnderlined ? COLORSTRING_UNDERLINE : 0);

	sAddString.TrimLeft();
	if (sAddString !=_T(""))
	{
		CColorString strAddAppearance(sAddString, dwAddStyle);
		mp_stcWarning.AddString(strAddAppearance);
		m_nCount++;
	}
	UpdateData(false);
}

void CScreenLock::OnBnClickedBtnSysunlock()
{
	CDialog_Pass_Check	dlgPASSCHK;

	dlgPASSCHK.m_nPassLevel = 3;

	st_other_info.strPassLevel = _T("SystemUnlock");

	dlgPASSCHK.DoModal();
/*
	if (nResponse == IDOK)
	{
		st_handler_info.nSystemLock = FALSE;
		::WritePrivateProfileString(_T("Password"), _T("SysLockStatus"), LPCTSTR("NO"), st_path_info.strBasic);

		if (st_handler_info.cWndList != NULL)	
		{
			// jtkim 20130625
			//sprintf(st_other_info.cNormalMsg, "System Unlocking.");
			//wsprintfA(st_other_info.cNormalMsg, "%S", _T("System Unlocking."));
			clsFunc.OnStringToChar(_T("System Unlocking."), st_other_info.cNormalMsg);
			st_handler_info.cWndList->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}
		::PostMessage(st_handler_info.hWnd, WM_FORM_CHANGE, SCREEN_MAIN, 0);
	}
	else if (nResponse == IDCANCEL)
	{

	}
*/
}


BOOL CScreenLock::DestroyWindow()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
//	st_handler_info.cWndLock = NULL;
	return CFormView::DestroyWindow();
}


void CScreenLock::PostNcDestroy()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
//	st_handler_info.cWndLock = NULL;
	CFormView::PostNcDestroy();
}


void CScreenLock::OnDestroy()
{
	CFormView::OnDestroy();
	
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	st_handler_info.cWndLock = NULL;
}
