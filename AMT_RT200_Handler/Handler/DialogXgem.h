#pragma once


// CDialogXgem 대화 상자입니다.

class CDialogXgem : public CDialog
{
	DECLARE_DYNAMIC(CDialogXgem)

public:
	CDialogXgem(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogXgem();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_XGEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
