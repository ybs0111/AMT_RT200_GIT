#pragma once


#include "Variable.h"
#include "XPGroupBox.h"
#include "xShadeButton.h"	// 조그 버튼 생성 클래스 추가
#include ".\\Union\\Grid\\GridCtrl.h"
#include ".\\Union\\Grid\\GridURLCell.h"
#include ".\\Union\\Grid\\GridCellCombo.h"
#include ".\\Union\\Grid\\GridCellCheck.h"
#include ".\\Union\\Grid\\GridCellNumeric.h"
#include ".\\Union\\Grid\\GridCellDateTime.h"
#include "Picture.h"
#include "BtnST.h"

// CScreenMotor 폼 뷰입니다.
class CDialogManualIO;

class CScreenMotor : public CFormView
{
	DECLARE_DYNCREATE(CScreenMotor)
public:	
	CDialogManualIO		*m_pIO;
	bool				m_bEmergency;	// ESTOP 요청 플래그 초기화

	CString				m_strCmdMotor;

	double				m_dMotorPos[50][2];
	double				m_dMotorGotoSpd;
	double				m_dDistance;
	double				m_dMotorGotoPos;
	
	int					m_nPositionMove;
	int					m_nHomeStep;
	int					m_nMoveStep;
	int					m_nCmdMotorNum;
	int					m_nCmdNum;
	int					m_nMoveMode;
	int					m_nMotorPartNum;
	int					m_nMotorPartAxisNum;
	int					m_nCellRowPos;	// 현재 선택된 Cell Row 위치
	int					m_nCellColPos;	// 현재 선택된 Cell Col 위치
	int					m_nImgPos;

	CGridCtrl			m_pGridMotPosition;
	CGridCtrl			m_pGridMotStatus;
	CImageList			*m_pImagelist;
	CPicture			m_imgPart;
	CPicture			m_imgPosition;

	CButtonST			m_btnApply;
	CButtonST			m_btnPartIO;
	CButtonST			m_btnEmergency;
	CButtonST			m_btnManualIO;
	CButtonST			m_btnManualFunction; //2015.02.13
	CXPGroupBox			m_groupPartList;
	CXPGroupBox			m_groupMotorList;

	CTreeCtrl			m_treeMotorList;

	CButtonST			m_btnHome;
	CButtonST			m_btnLeft;
	CButtonST			m_btnRight;

	void				OnInitGroupBox();
	void				OnInitTree();
	void				OnInitGridMotorPosition(int nPartNum, int nAxisNum);
	void				OnInitGridMotorStatus(int nPartNum, int nAxisNum);
	void				OnInitButton();
	void				OnInitRadio();
	void				OnImageLoad(int nMode, CString strFilename);
	void				OnButtonControl(bool bStatus);
	void				OnMotorMoveStepClear();
	void				OnMotorPositionCheck();
	void				OnMotorStatusCheck();
	void				OnMotorCompletion(int nRow, int nCol);
	int					OnMotorMovingExcution();
	int					OnMotorHomingExcution();
	int					OnMotorStatusCheck(int nMotor, int nMode);
	void				OnDataHistoryLog(int nPartNum, int nAxisNum);
	void				OnDataRecovery(int nPartNum, int nAxisNum);
	int					OnDataComparison(int nPartNum, int nAxisNum);
	void				OnDataApply(int nPartNum, int nAxisNum);
	void				OnDataBackup(int nPartNum, int nAxisNum);
	void				OnDataInit(int nPartNum, int nAxisNum);

public:
	CScreenMotor();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CScreenMotor();

public:
	enum { IDD = IDD_SCREEN_MOTOR };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedGroupIoList();
	virtual BOOL DestroyWindow();
	virtual void OnInitialUpdate();
	afx_msg void OnClickedBtnEmergency();
	afx_msg void OnSelchangedTreeMotorList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPaint();
	afx_msg LRESULT OnUserLButtonDown(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserLButtonUp(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMotorCellClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnStatusCellClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedBtnApply();
	afx_msg void OnBnClickedBtnManualFunction(); //2015.02.13
	afx_msg void OnBnClickedBtnPartIo();
	virtual void PostNcDestroy();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnManualIo();
	afx_msg LRESULT	OnMotorWorkInfoCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedBtnLeftUnclamp();
};


