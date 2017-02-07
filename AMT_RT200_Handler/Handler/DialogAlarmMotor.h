#pragma once


// CDialogAlarmMotor ��ȭ �����Դϴ�.
#include ".\\Union\\Grid\\GridCtrl.h"
#include ".\\Union\\Grid\\GridURLCell.h"
#include ".\\Union\\Grid\\GridCellCombo.h"
#include ".\\Union\\Grid\\GridCellCheck.h"
#include ".\\Union\\Grid\\GridCellNumeric.h"
#include ".\\Union\\Grid\\GridCellDateTime.h"
#include "BtnST.h"

class CDialogAlarmMotor : public CDialog
{
	DECLARE_DYNAMIC(CDialogAlarmMotor)
public:
	BOOL			Create();
	int				OnMotorStatusCheck(int nMotor, int nMode);
	void			OnInitGridMotorStatus();
	void			OnInitButton();
	void			OnMotorStatusCheck();

	int				m_nAxis;
	int				m_nAxisLimitCnt;
	int				m_nAxisLimitInfo[3];

	CString			m_strAxisName;

	CBitmap			m_btmBk;

	CGridCtrl		m_pGridMotStatus;

	CButtonST		m_btnExit;

public:
	CDialogAlarmMotor(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogAlarmMotor();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_ALARM_MOTOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnClickedBtnExit();
	afx_msg void OnStatusCellClick(NMHDR *pNotifyStruct, LRESULT* pResult);
};
