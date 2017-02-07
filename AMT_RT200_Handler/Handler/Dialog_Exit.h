#pragma once


// CDialog_Exit 대화 상자입니다.

class CDialog_Exit : public CDialog
{
	DECLARE_DYNAMIC(CDialog_Exit)

public:
	CDialog_Exit(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialog_Exit();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_EXIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
//	CButton m_a;
	afx_msg void OnBnClickedBtnExitC();
};
