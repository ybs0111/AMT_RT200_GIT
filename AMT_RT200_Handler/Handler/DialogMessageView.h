#pragma once


// CDialogMessageView ��ȭ �����Դϴ�.
#include "XPGroupBox.h"
#include "BtnST.h"
#include ".\\Controls\\ColorListBox.h"		// Į�� ����Ʈ �ڽ� Ŭ�������� �߰�

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
	CDialogMessageView(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogMessageView();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_MESSAGEBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnCancel();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedBtnRetry();
};
