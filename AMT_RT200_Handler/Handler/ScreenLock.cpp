// ScreenLock.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Handler.h"
#include "ScreenLock.h"
#include "Dialog_Pass_Check.h" 
#include "Variable.h"
#include "PublicFunction.h"
//==================================================================//
// 배너 출력 위치 정의
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


// CScreenLock 진단입니다.

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


// CScreenLock 메시지 처리기입니다.


void CScreenLock::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	if (st_handler_info.nSystemLock != TRUE)	
	{
		st_handler_info.nSystemLock = TRUE;
	}

	OnInitButton();

	OnSetPicture(); 
	OnSetBanner();  // 칼라 배너 생성

	st_handler_info.cWndLock = this;
}

//==================================================================//
// GIF 로딩
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
// 칼라 배너 생성
//==================================================================//
void CScreenLock::uSetBanner() 
{
	//==============================================================//
	// 칼라 배너 생성
	m_crBackColor = RGB(255,255,255);
	m_crForeColor = RGB(255,0,0);
	mp_stcWarning.SetBackColor(m_crBackColor);
	mp_stcWarning.SetOrientation(ORIENTATION_HORIZONTAL);
	//--------------------------------------------------------------//

	// 추후에 배너 다중 문자열 중 필요한 부분 삭제할 경우 발생할지 몰라 일단 추가
	m_nCount=0;

	//==============================================================//
	// 배너 컨트롤 출력 영역 설정
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

	OnBannerRemoveAll();  // 배너로 출력된 모든 문자열 제거

	CString sBanner =_T("System Locking!!     ");
	OnAddString(sBanner);  // 배너 문자열 추가

	// 추후에는 유저가 남기로 싶은 메시지를 입력받아 처리하도록한다
	// 일단은 하드코딩으로 처리한다
	OnChangeColor(COLOR_FOR, RGB(0,0,175));  // 배너 칼라 변경
	sBanner =_T("Now TESTING.. DON'T TOUCH     ");
	//sBanner  = (stOTHER.strPassComment);
	sBanner += _T("     ");
	OnAddString(sBanner);

	m_nSpeed=1000;
	OnSetScrollSpeed(m_nSpeed);  // 배너 스크롤 속도 설정
}
*/
//==================================================================//
// 변수 초기화
//==================================================================//
void CScreenLock::OnSetInitVariable() 
{
	m_bBold = FALSE;
	m_bItalic = FALSE;
	m_bUnderlined = FALSE;
}
//==================================================================//
// 칼라 배너 생성
//==================================================================//
void CScreenLock::OnSetBanner() 
{
	//==============================================================//
	// 칼라 배너 생성
	//m_crBackColor = RGB(0,0,255);
	//m_crForeColor = RGB(255,0,0);
	m_crBackColor = RGB(255,255,255);
	m_crForeColor = RGB(255,0,0);
	mp_stcWarning.SetBackColor(m_crBackColor);
	mp_stcWarning.SetOrientation(ORIENTATION_HORIZONTAL);
	//--------------------------------------------------------------//

	// 추후에 배너 다중 문자열 중 필요한 부분 삭제할 경우 발생할지 몰라 일단 추가
	m_nCount=0;

	//==============================================================//
	// 배너 컨트롤 출력 영역 설정
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

	OnBannerRemoveAll();  // 배너로 출력된 모든 문자열 제거

	CString sBanner =_T("System Locking!!     ");
	OnAddString(sBanner);  // 배너 문자열 추가

	// 추후에는 유저가 남기로 싶은 메시지를 입력받아 처리하도록한다
	// 일단은 하드코딩으로 처리한다
	OnChangeColor(COLOR_FOR, RGB(0,0,175));  // 배너 칼라 변경
	sBanner =_T("Now TESTING.. DON'T TOUCH     ");
	//sBanner  = (stOTHER.strPassComment);
	sBanner += _T("     ");
	OnAddString(sBanner);

	m_nSpeed=600;
	OnSetScrollSpeed(m_nSpeed);  // 배너 스크롤 속도 설정
}

//==================================================================//
// 배너 스크롤 속도 설정
//==================================================================//
void CScreenLock::OnSetScrollSpeed(int nzSpeed) 
{
	mp_stcWarning.SetScrollSpeed(nzSpeed);
}

//==================================================================//
// 배너 칼라 변경
// : Fore Color 변경은 다음에 추가할 문자열에 적용된다
// : 그러므로 Fore Color 변경후에 AddString으로 문자열 추가한다
// : 필요하다면 칼라 선태 대화상자로 칼라 정보 입력받아도 된다
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
// 배너 출력 방향 설정
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
// 배너로 출력된 모든 문자열 제거
//==================================================================//
void CScreenLock::OnBannerRemoveAll() 
{
	UpdateData(true);
	mp_stcWarning.RemoveAllStrings();
	m_nCount=0;
	UpdateData(false);
}

//==================================================================//
// 배너 문자열 추가
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
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
//	st_handler_info.cWndLock = NULL;
	return CFormView::DestroyWindow();
}


void CScreenLock::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
//	st_handler_info.cWndLock = NULL;
	CFormView::PostNcDestroy();
}


void CScreenLock::OnDestroy()
{
	CFormView::OnDestroy();
	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	st_handler_info.cWndLock = NULL;
}
