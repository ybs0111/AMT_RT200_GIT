#pragma once

#include "XPGroupBox.h"
#include "xShadeButton.h"	// 조그 버튼 생성 클래스 추가
#include ".\\Union\\Grid\\GridCtrl.h"
#include ".\\Union\\Grid\\GridURLCell.h"
#include ".\\Union\\Grid\\GridCellCombo.h"
#include ".\\Union\\Grid\\GridCellCheck.h"
#include ".\\Union\\Grid\\GridCellNumeric.h"
#include ".\\Union\\Grid\\GridCellDateTime.h"
#include "Digit.h"
#include "BtnST.h"
#include "Variable.h"
#include "GradientStatic.h"
// #include "d:\work\amt_program\amt_rt200(라벨러)\13.버전별프로그램\amt_rt200\handler\handler\union\grid\gridctrl.h"
// #include "d:\work\amt_program\amt_rt200(라벨러)\13.버전별프로그램\amt_rt200\handler\handler\union\grid\gridctrl.h"
// CDialogManualIO 대화 상자입니다.

#define TMR_IO_CONTINUE			  100
#define TM_ROBOT_MOVE			  200
#define TM_ROBOT_RELAY_MOVE       300
#define TM_ROBOT_PICK_PLACE       400
#define TM_ROBOT_PICK_UP		      500
#define TM_ROBOT_PICK_DN			  600
#define TM_PICKER				  700
#define TM_LABELFEEDER			  800

class CDialogManualIO : public CDialog
{
	DECLARE_DYNAMIC(CDialogManualIO)

public:
	CButtonST		m_btnExit;
	CButtonST		m_btnDoor;
	CButtonST		m_btnLeftMpClamp;
	CButtonST		m_btnLeftMpUnClamp;
	CButtonST		m_btnRightMpClamp;
	CButtonST		m_btnRightMpUnClamp;
	CButtonST		m_btnPickerPlace;

	CButtonST        m_btnManualAction1;
	CButtonST        m_btnManualAction10;
	CButtonST        m_btnManualAction100;
	CButtonST        m_btnManualEmergency;
	CButtonST		 m_btnManualExit;
	
	CXPGroupBox		m_groupTurnCovRuntime;
	CXPGroupBox		m_groupPickerRuntime;
	CXPGroupBox		m_group_TurnConveyor;
	CXPGroupBox		m_groupRbotPicker;

	CGridCtrl		m_pGridRuntime;
	CButtonST		m_btnPickUp;
	CButtonST		m_btnMove;
	CButtonST		m_btnPlace;
	CButtonST		m_btnRelay;
	CGridCtrl		m_pGridRobotRunTime;
	CDigit			m_Pitch_x;
	CDigit			m_Pitch_Y;
	CDigit			m_WaitTime;

	CGradientStatic m_msgPitchX;
	CGradientStatic m_msgPitchY;

	int             m_nManualActionMode;
	int             m_nManualWaitTime;
	int             m_nPickerNum;
	double          m_dPitch_x;
	double          m_dPitch_Y;
	int             m_nManualStep;
	long            m_lWaitTime[3];
	long            m_lFeederWaitTime[3];
	long            m_lVaccumOffTime[3];
	int             m_Retry;
	int				m_nCnt;
	int             m_nPickCnt;
	int             m_nPlaceCnt;
	CBitmap			m_bmGround;

	DWORD			m_dwWaitTime[3];

	DWORD           m_dwTimeCheck[3];
	double			m_dpTargetPosList[4];
	int				m_nManualActionCount[2];
	bool            m_bEmergencyStop;
	bool            m_bSecondPos;
	int             m_RunMoveStep;
	int             m_RunRelayMoveStep;
	int             m_RunPickPlaceStep;
	int             m_RunPickUpStep;
	int             mn_Num;

	int             m_RunPickDnStep;
	int             m_nRobot_Z;
	int             m_nRobot_X;
	int             m_nRobot_Y;
	int             m_nRobot_P;
	int             m_nRobot_T;
	int				m_nLinearMove_Index; 
	int             m_RunPickerStep;
	int             m_nInterFaceStep;
	int             mn_Retry;
	long				m_lAxisCnt;            //현재의 IndexNum에서 사용 가능한 모터 수량 최대 4개  
	long				m_lpAxisNum[4];        //현재의 IndexNum에서 사용하는 모터 실재 번호를 가진다 

	
	CString         m_strAlarmCode;

	BOOL			Create();
	void			OnInitButton();
	void			OnIoActionMode(int nMode);
	void            OnInitDigit();
	int             OnCylinderExcution();
	void            OnInitGrid();
	void            OnInitLabel();

	int             OnRunPicker();

	void            OnTurnConvRuntTimeGrid();
	void            OnPickerRunTimeGrid();
	void            OnInitGroup();
	int             OnRobotMove();
	int             OnRobotRelayMove();
	int             OnRobotPickPlace();
	int             OnRobotPickUp();
	int             OnRobotPickDn();
	int	           OnFeederInterface();
	void           OnFeederReq(int nStep);

	int            GetIntefaceInfo(int mStep) { return m_nInterFaceStep == mStep; }
	void           SetInterfaceStep(int mStep) { m_nInterFaceStep = mStep; }
	

public:
	CDialogManualIO(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogManualIO();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MANUAL_IO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnBnClickedCheckManualAction1();
	afx_msg void OnBnClickedCheckManualAction10();
	afx_msg void OnBnClickedCheckManualAction100();
	afx_msg void OnBnClickedBtnManualEmergency();

	
	afx_msg void OnStnClickedWaitTime();
	afx_msg void OnBnClickedBtnExt();
	
	afx_msg void OnBnClickedBtnPick();
	afx_msg void OnBnClickedBtnMove();
	afx_msg void OnBnClickedBtnPlace();
	afx_msg void OnBnClickedBtnRelay();
	
	afx_msg void OnBnClickedBtnPickPlace();
	
	CButtonST m_btnPicker;
	afx_msg void OnBnClickedBtnPicker();
// 	afx_msg void OnStnClickedPitchY();
// 	afx_msg void OnStnClickedPitchX();
	afx_msg void OnClickedBtnManualEmergency2();
};
