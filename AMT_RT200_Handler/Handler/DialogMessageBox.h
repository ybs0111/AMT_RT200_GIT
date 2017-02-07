#pragma once

#include "BtnST.h"
#include "XPGroupBox.h"
#include ".\\Controls\\ColorListBox.h"		// Į�� ����Ʈ �ڽ� Ŭ����
// CDialogMessageBox ��ȭ �����Դϴ�.

class CDialogMessageBox : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogMessageBox)

public:
	CDialogMessageBox(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogMessageBox();

// ��ȭ ���� �������Դϴ�.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonExit();
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL DestroyWindow();
};
