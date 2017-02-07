#pragma once


// CDialogMessageView 대화 상자입니다.
#include "XPGroupBox.h"
#include "BtnST.h"
#include ".\\Controls\\ColorListBox.h"		// 칼라 리스트 박스 클래스래스 추가

class CDialogMessageView : public CDialog
{
	DECLARE_DYNAMIC(CDialogMessageView)
public:
	int				m_nConfirmType;

	CString			m_strMessage;

	CColorListBox	m_listData;

	CButtonST		m_btnRetry;
	CButtonST		m_btnOk;
	CButtonST		m_btnCancel;

	CXPGroupBox		m_groubMsg;

	CBitmap			m_bmGround;

	BOOL			Create();
	void			OnMessageDisplay();
	void			OnInitButton();
	void			OnInitGroupBox();
public:
	CDialogMessageView(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogMessageView();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MESSAGEBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnCancel();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedBtnRetry();
};
