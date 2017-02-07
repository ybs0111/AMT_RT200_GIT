#pragma once

#include "BtnST.h"
#include "XPGroupBox.h"
#include ".\\Controls\\ColorListBox.h"		// 칼라 리스트 박스 클래스
// CDialogMessageBox 대화 상자입니다.

class CDialogMessageBox : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogMessageBox)

public:
	CDialogMessageBox(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogMessageBox();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MESSAGE_BOX };

public:
	void			OnMessageDisplay(CString strMsg);
	void			OnInitGroup();
	void			OnInitButton();
	BOOL			Create();

	DWORD			m_dwWaitTime[3];

	CString			m_strOldMsg;

	CButtonST		m_btnExit;

	CXPGroupBox		m_groupMsg;

	CColorListBox	m_listMsg;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonExit();
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL DestroyWindow();
};
