#pragma once


// CWorkMotorSpeed 대화 상자입니다.
#include "Variable.h"
#include "XPGroupBox.h"
#include "xShadeButton.h"	// 조그 버튼 생성 클래스 추가
#include ".\\Union\\Grid\\GridCtrl.h"
#include ".\\Union\\Grid\\GridURLCell.h"
#include ".\\Union\\Grid\\GridCellCombo.h"
#include ".\\Union\\Grid\\GridCellCheck.h"
#include ".\\Union\\Grid\\GridCellNumeric.h"
#include ".\\Union\\Grid\\GridCellDateTime.h"
#include "BtnST.h"
class CWorkMotorSpeed : public CDialog
{
	DECLARE_DYNAMIC(CWorkMotorSpeed)

public:
	// jtkim 20150703
	int							m_nAccLimit[100];
	int							m_nDecLimit[100];
	int							m_nVelLimit[100];

	double						m_dLimitP[100][2];
	double						m_dLimitM[100][2];
	double						m_dAcc[100][2];
	double						m_dDec[100][2];
	double						m_dVel[100][2];
	double						m_dHome[100][2];
	double						m_dJog[100][2];
	double						m_dMax[100][2];
	
	int							m_nMotorMaxCnt;
	double						m_dAllow[100][2];
	int							m_nPerJog[100][2];
	int							m_nPerHome[100][2];
	int							m_nPerVel[100][2];

	CString						m_strMotorName[100];
/*
	CXTPButton					m_btnApply;
	CXTPButton					m_btnReload;
*/
	CButtonST					m_btnApply;
	CButtonST					m_btnReLoad;
	CButtonST					m_btnSafety;
	CButtonST					m_btnTsite;

	CGridCtrl					m_pGridSpeed;

	CXPGroupBox					m_groupCommand;

	void						OnInitGroupBox();
	void						OnInitLabel();
	void						OnInitButton();
	void						OnInitGridSpeed();
	void						OnDataHistoryLog();
	void						OnDataRecovery();
	int							OnDataComparison();
	void						OnDataApply();
	void						OnDataBackup();
	void						OnDataInit();
public:
	CWorkMotorSpeed(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CWorkMotorSpeed();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_WORK_SPEED };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
	afx_msg void OnBnClickedBtnSafety();
	afx_msg void OnBnClickedBtnTestsite();
	afx_msg void OnBnClickedButtonXgem();
};
