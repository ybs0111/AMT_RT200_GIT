#pragma once


// CDialog_Exit ��ȭ �����Դϴ�.

class CDialog_Exit : public CDialog
{
	DECLARE_DYNAMIC(CDialog_Exit)

public:
	CDialog_Exit(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialog_Exit();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_EXIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
//	CButton m_a;
	afx_msg void OnBnClickedBtnExitC();
};
