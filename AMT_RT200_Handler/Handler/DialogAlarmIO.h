#pragma once


// CDialogAlarmIO 대화 상자입니다.
#include ".\\Union\\Grid\\GridCtrl.h"
#include ".\\Union\\Grid\\GridURLCell.h"
#include ".\\Union\\Grid\\GridCellCombo.h"
#include ".\\Union\\Grid\\GridCellCheck.h"
#include ".\\Union\\Grid\\GridCellNumeric.h"
#include ".\\Union\\Grid\\GridCellDateTime.h"
#include "BtnST.h"

class CDialogAlarmIO : public CDialog
{
	DECLARE_DYNAMIC(CDialogAlarmIO)
public:
	BOOL			Create();
	void			OnInputCheck();
	void			OnOutputCheck();
	void			OnInitGridInput();
	void			OnInitGridOutput();
	void			OnInitButton();

	CBitmap			m_btmBk;

	int				m_nInCnt;
	int				m_nOutCnt;

	int				m_nIn[16];
	int				m_nOut[16];

	CString			m_strIn[16];
	CString			m_strOut[16];

	CGridCtrl		m_pGridInput;
	CGridCtrl		m_pGridOutput;

	CButtonST		m_btnExit;
public:
	CDialogAlarmIO(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogAlarmIO();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_ALARM_IO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClickedBtnExit();
	afx_msg void OnCellOutputClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
