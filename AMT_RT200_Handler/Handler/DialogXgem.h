#pragma once


// CDialogXgem ��ȭ �����Դϴ�.

class CDialogXgem : public CDialog
{
	DECLARE_DYNAMIC(CDialogXgem)

public:
	CDialogXgem(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogXgem();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_XGEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_strAlarm;
	CString m_strLotNo;
	CString m_strPartNo;
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonEnd();
	afx_msg void OnBnClickedButtonAlarmOn();
	afx_msg void OnBnClickedButtonAlarmOff();
	afx_msg void OnBnClickedButtonRecipe();
	afx_msg void OnBnClickedButtonBoard();
	afx_msg void OnBnClickedButtonExit();
};
