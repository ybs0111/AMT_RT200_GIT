#pragma once

#define R_ON	(RGB(255, 0, 0))
#define R_OFF	(RGB(162, 0, 0))
#define Y_ON	(RGB(255, 255, 0))
#define Y_OFF	(RGB(162, 162, 0))
#define G_ON	(RGB(0, 255, 0))
#define G_OFF	(RGB(0, 162, 0))

#define FLICK	(RGB(190, 220, 255))
// CWorkMaintenance 대화 상자입니다.
#include "Variable.h"
#include "XPGroupBox.h"
#include "xShadeButton.h"	// 조그 버튼 생성 클래스 추가
#include ".\\Controls\\CheckerCtrl.h"	// SITE 클래스.
#include "UniButtonEx.h"	// 유니 버튼 클래스
#include "Digit.h"
#include "GradientStatic.h"
#include "BtnST.h"
//2015.02.09
#include ".\\Union\\Grid\\GridCtrl.h"
#include "XgemClient.h"

class CWorkMaintenance : public CDialog
{
	DECLARE_DYNAMIC(CWorkMaintenance)

public:
	enum 
	{ 
		IDC_LAMP_BASE			= 3000, 
		IDC_LAMP_ROOF			= 3010, 
		IDC_LAMP_POLE			= 3020, 
		IDC_LAMP_LOWER			= 3030, 
		IDC_LAMP_UPPER			= 3040, 
		IDC_LAMP_SEP1			= 3050, 
		IDC_LAMP_SEP2			= 3060, 
		IDC_LAMP_SEP3			= 3070, 
		ID_LAMP_RLAMP			= 3080, 
		ID_LAMP_RFLICKER		= 3090, 
		ID_LAMP_YLAMP			= 3100, 
		ID_LAMP_YFLICKER		= 3110, 
		ID_LAMP_GLAMP			= 3120, 
		ID_LAMP_GFLICKER		= 3130, 
	};
	
	enum 
	{ 
		STOP					= 0,
		RUN						= 1,
		IDLE					= 2,
		ALARM					= 3,
		LOTEND					= 4,
		INIT					= 5,
		WARNING					= 6,
		LOCK					= 7,
		SELFCHECK				= 8
	};
										//장비의 설정들을 저장할 변수 선언
	BOOL						m_bLamp;
	long						m_nWaitTime[3];

	CString						m_strEqp[2];

	int							m_nLampFlickerTime[2];
	int							m_nModeBuzzer[2];
	int							m_nModeLampBlue[2];			//관심장비를 알리는 blue lamp을 켤지? (0:끔, 표시하지 않음, 관심 장비가 아님   1:켬)
	int							m_nModeLampBlueFlicker[2];	//관심장비를 알리는 blue lamp의 상태를 설정 (0:일반 켜짐, 1:깜빡임)
	int							m_nRlamp[2][9];				// RED 램프 상태 플래그 [0:OFF    1:ON    2:FLICKER] //
	int							m_nYlamp[2][9];				// YELLOW 램프 상태 플래그 [0:OFF    1:ON    2:FLICKER] //
	int							m_nGlamp[2][9];				// GREEN 램프 상태 플래그 [0:OFF    1:ON    2:FLICKER] //
	int							m_nPosX[9][2];
	int							m_nPosY[9][2];				// 컨트롤 위치 저장 변수 //
	int							m_nYdiff[9];
	int							m_nYspace[9];				// 출력 영역 저장 변수 //
	int							m_nRunSpeed[2];
	int							m_nManualSpeed[2];
	//2015.02.09
	int							m_nModeXgem[2];
	int							m_nModeXgemRunChk[2];
	int							m_nModeXgemInterface[2];
	//2015.02.09
	double						m_dRejTrayPick[2];			//Reject Tray Pick Offset
	double						m_dRejTrayPlace[2];			//Reject Tray Place Offset

	CRect						m_rectArea[9];  // 컨트롤 여역 크기 저장 변수 //	

	CGradientStatic				m_msgRunSpeed;
	CGradientStatic				m_msgManualSpeed;
	CGradientStatic				m_msgEqp;
	CGradientStatic				m_msgEqpData;
	CGradientStatic				m_msgRejTrayPick;
	CGradientStatic				m_msgRejTrayPlace;

	CDigit						m_dgtFlickerTime;
	CDigit						m_dgtRunSpeed;
	CDigit						m_dgtManualSpeed;
	CDigit						m_dgtRejTrayPick;
	CDigit						m_dgtRejTrayPlace;

	CXPGroupBox					m_groupSpeed;
	CXPGroupBox					m_groupLamp;
	CXPGroupBox					m_groupLampWarning;
	CXPGroupBox					m_groupLampStop;
	CXPGroupBox					m_groupLampRun;
	CXPGroupBox					m_groupLampLotend;
	CXPGroupBox					m_groupLampLock;
	CXPGroupBox					m_groupLampFlickerTime;
	CXPGroupBox					m_groupLampAlarm;
	CXPGroupBox					m_groupLampIdle;
	CXPGroupBox					m_groupBuzzer;
	CXPGroupBox					m_groupPassword;
	CXPGroupBox					m_groupCommand;
	CXPGroupBox					m_groupEqpInfo;
	CXPGroupBox					m_groupOffset;
	CXPGroupBox					m_groupXgemInfo; //2015.02.09
	CGridCtrl					m_pGridXgem; //2015.02.09

	CStatic						m_stLampWarning;
	CStatic						m_stLampStop;
	CStatic						m_stLampRun;
	CStatic						m_stLampLotend;
	CStatic						m_stLampLock;
	CStatic						m_stLampAlarm;
	CStatic						m_stLampIdle;

	CUniButtonEx				m_btnBase[9];
	CUniButtonEx				m_btnRoof[9];
	CUniButtonEx				m_btnPole[9];
	CUniButtonEx				m_btnLower[9];
	CUniButtonEx				m_btnUpper[9];

	CUniButtonEx				m_btnStep1[9];
	CUniButtonEx				m_btnStep2[9];
	CUniButtonEx				m_btnStep3[9];
	CUniButtonEx				m_btnRlamp[9];
	CUniButtonEx				m_btnYlamp[9];
	CUniButtonEx				m_btnGlamp[9];

	CButtonST					m_btnApply;
	CButtonST					m_btnReLoad;
	CButtonST					m_btnBuzzer;
	CButtonST					m_btnPasswordSystem;
	CButtonST					m_btnPasswordLevel1;
	CButtonST					m_btnPasswordLevel2;		
	CButtonST					m_btnCylinder;

	void						OnInitGroupBox();
	void						OnInitLabel();
	void						OnInitDigit();
	void						OnInitButton();
	void						OnDataHistoryLog();
	void						OnDataRecovery();
	int							OnDataComparison();
	void						OnDataApply();
	void						OnDataBackup();
	void						OnDataInit();
	void						OnLampDrawButton(int i);
	void						OnLampRedStateChange(int nDx);
	void						OnLampGreenStateChange(int nDx);
	void						OnLampYellowStateChange(int nDx);
	void						OnInitGridXgem();//2015.02.09
	int						OnXgemInterface(); //2015.02.09
public:
	CWorkMaintenance(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CWorkMaintenance();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_WORK_MAINTENANCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnRLampClick(UINT);
	afx_msg void OnYLampClick(UINT);
	afx_msg void OnGLampClick(UINT);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedBtnApply();
	afx_msg void OnBnClickedBtnSmPasswordChangeSLock();
	afx_msg void OnBnClickedBtnSmPasswordChangeLevle1();
	afx_msg void OnBnClickedBtnSmPasswordChangeLevel2();
	afx_msg void OnClickedBtnBuzzer();
	afx_msg void OnStnClickedDgtSmLampFlickerTime();
	afx_msg void OnClickedDgtRunSpeed();
	afx_msg void OnClickedDgtManualSpeed();
	virtual void PostNcDestroy();
	afx_msg void OnDestroy();
	afx_msg void OnStnClickedMsgMaintenanceEqpData();
	afx_msg void OnStnClickedDgtRejTrayPickOffset();
	afx_msg void OnStnClickedDgtRejTrayPlaceOffset();
	afx_msg void OnXgemCellClick(NMHDR *pNotifyStruct, LRESULT* pResult); //2015.02.09
	afx_msg void OnBnClickedBtnMaintCylinder();
};
