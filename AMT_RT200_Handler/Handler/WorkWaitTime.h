#pragma once


// CWorkWaitTime ��ȭ �����Դϴ�.
#include "Variable.h"
#include "XPGroupBox.h"
#include "xShadeButton.h"	// ���� ��ư ���� Ŭ���� �߰�
#include ".\\Union\\Grid\\GridCtrl.h"
#include ".\\Union\\Grid\\GridURLCell.h"
#include ".\\Union\\Grid\\GridCellCombo.h"
#include ".\\Union\\Grid\\GridCellCheck.h"
#include ".\\Union\\Grid\\GridCellNumeric.h"
#include ".\\Union\\Grid\\GridCellDateTime.h"
#include "BtnST.h"

class CWorkWaitTime : public CDialog
{
	DECLARE_DYNAMIC(CWorkWaitTime)

public:
	CString						m_strWaitTimeLabel[100];

	int							m_nWaitTimeMaxCnt;
	int							m_nOnWaitTime[100][2];
	int							m_nOffWaitTime[100][2];
	int							m_nLimitWaitTime[100][2];
/*
	CXTPButton					m_btnApply;
	CXTPButton					m_btnReload;
*/
	CButtonST					m_btnApply;
	CButtonST					m_btnReLoad;

	CGridCtrl					m_pGridWaitTime;

	CXPGroupBox					m_groupCommand;
	CXPGroupBox					m_groupWaitTime;

	void						OnInitGridWaitTime();
	void						OnInitGroupBox();
	void						OnInitLabel();
	void						OnInitButton();
	void						OnDataHistoryLog();
	void						OnDataRecovery();
	int							OnDataComparison();
	void						OnDataApply();
	void						OnDataBackup();
	void						OnDataInit();

public:
	CWorkWaitTime(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CWorkWaitTime();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_WORK_WAIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
	afx_msg void OnCellClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnClickedBtnApply();
	afx_msg void OnClickedBtnReload();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void PostNcDestroy();
	afx_msg void OnDestroy();
};
