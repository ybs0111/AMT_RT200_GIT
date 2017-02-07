#pragma once

#include "XPGroupBox.h"
#include "BtnST.h"
#include ".\\Controls\\ColorListBox.h"		// Į�� ����Ʈ �ڽ� Ŭ�������� �߰�
// CDialog_Message dialog

class CDialog_Message : public CDialog
{
	DECLARE_DYNAMIC(CDialog_Message)
public:
//	CXTPEditListBox  m_editMsg;
	void			OnBtnShow();
	void			OnMessageDisplay();
	void			OnSetEditStyle();
	void			OnInitButton();
	void			OnInitGroupBox();

	int				m_nMessageType;

	CString			m_strMessage;

	CColorListBox	m_listData;

	CButtonST		m_btnOk;
	CButtonST		m_btnPass;
	CButtonST		m_btnCancel;

public:
	CDialog_Message(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialog_Message();

// Dialog Data
	enum { IDD = IDD_DIALOG_MESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnCancel();
	afx_msg void OnBnClickedBtnPass();
	afx_msg void OnDestroy();
	CXPGroupBox m_groubMsgShow;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
