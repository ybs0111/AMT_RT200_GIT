#pragma once

#include ".\\Controls\\PictureEx.h"      // GIF 로딩 클래스
#include ".\\Controls\\MultiColorStatic.h"
#include ".\\Controls\\BannerStatic.h"      // 배너 문자열 클래스
#include "BtnST.h"
// CScreenLock 폼 뷰입니다.

class CScreenLock : public CFormView
{
	DECLARE_DYNCREATE(CScreenLock)

public:
	CScreenLock();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CScreenLock();
	//==============================================================//
	// 배너 관련 클래스 변수 선언
	BOOL				m_bBold;
	BOOL				m_bItalic;
	BOOL				m_bUnderlined;

	int					m_nCount;
	int					m_nWidth;
	int					m_nHeight;
	int					m_nBannerWidth;
	int					m_nBannerHeight;
	int					m_nBannerTop;
	int					m_nBannerLeft;
	int					m_nSpeed;

	COLORREF			m_crBackColor;
	COLORREF			m_crForeColor;
	//--------------------------------------------------------------//

	CPictureEx			mp_gifSysLock;
	CBannerStatic		mp_stcWarning;

	CButtonST			m_btnSysUnLock;
	void				OnInitButton();
	void				OnSetPicture(); 
//	void				OnSetBanner(); 

	//==============================================================//
	// 배너 관련 클래스 함수
	void				OnSetInitVariable();  // 변수 초기화
	void				OnSetBanner();  // 칼라 배너 생성

	void				OnSetScrollSpeed(int nzSpeed);  // 배너 스크롤 속도 설정
	void				OnChangeColor(int nzType, COLORREF crzColor);  // 배너 칼라 변경
	void				OnChangeOrientation(int nzOrient);  // 배너 출력 방향 설정
	void				OnBannerRemoveAll();  // 배너로 출력된 모든 문자열 제거
	void				OnAddString(CString szBanner);  // 배너 문자열 추가
	//--------------------------------------------------------------//

public:
	enum { IDD = IDD_SCREEN_LOCK };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedBtnSysunlock();
	virtual BOOL DestroyWindow();
	virtual void PostNcDestroy();
	afx_msg void OnDestroy();
};


