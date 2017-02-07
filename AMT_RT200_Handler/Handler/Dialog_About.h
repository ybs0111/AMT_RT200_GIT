#pragma once


// CDialog_About 대화 상자입니다.
#include ".\\Controls\\StarWarsCtrl.h"  // 스타워즈 컨트롤 클래스
#include "BtnST.h"

class CDialog_About : public CDialog
{
	DECLARE_DYNAMIC(CDialog_About)

public:
	CDialog_About(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialog_About();

	CStarWarsCtrl	mp_imgAbout;

	CButtonST			m_btnUdate;
	CButtonST			m_btnAboutOK;

	void OnSetButton();  // 칼라 버튼 생성
	void OnSetUserControl(); 

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_ABOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnUpdate();
	afx_msg void OnBnClickedBtnAboutOk();
};
