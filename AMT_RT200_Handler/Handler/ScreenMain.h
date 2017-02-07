#pragma once

#include ".\\Controls\\Picture.h"  // 이미지 로딩 클래스
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
#include "GradientStatic.h"
#include "MyBasicData.h"
#include "afxwin.h"



// CScreenMain 폼 뷰입니다.

class CScreenMain : public CFormView
{
	DECLARE_DYNCREATE(CScreenMain)

public:
	int						m_nTime;
	int                     mn_Size;
	bool                    mb_Check[12];
public:
	CScreenMain();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CScreenMain();

public:
	enum { IDD = IDD_SCREEN_MAIN };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	CXPGroupBox			m_groupTimeInfo;
	CXPGroupBox			m_groupWorkInfo;

	CXPGroupBox m_group_Conv;
	CXPGroupBox m_group_OutConv;
	CXPGroupBox m_group_TurnConv;
	CXPGroupBox m_group_InConv;
	CXPGroupBox m_group_Robot;
	CXPGroupBox m_group_Feeder;
	CButtonST m_Check_CV_In;
	CButtonST m_Check_In_pos;
	CButtonST m_Check_Out_InPos;
	CButtonST m_Check_Turn_Pos;
	CButtonST m_Check_Out_Pos;
	CButtonST m_Check_Out_Up;
	CButtonST m_Check_Out_Dw;
	CButtonST m_Check_TurnUp;
	CButtonST m_Check_TurnDw;
	CButtonST m_Check_InUp;
	CButtonST m_Check_InDw;

	CGradientStatic m_msg_Cv_In;
	CGradientStatic m_msg_In_Pos;
	CGradientStatic m_msg_OutIn_Pos;
	CGradientStatic m_msg_Turn_Pos;
	CGradientStatic m_msg_Out_Pos;
	CGradientStatic m_msg_In_Up;
	CGradientStatic m_msg_In_Down;
	CGradientStatic m_msg_Turn_Up;
	CGradientStatic m_msg_Turn_Dw;
	CGradientStatic m_msg_Out_Up;
	CGradientStatic m_msg_Out_Dw;
	CGradientStatic m_msg_Right_Turn;
	CGradientStatic m_msg_Left_Turn;

	CGridCtrl m_pGridWorkInfoC;
	CGridCtrl m_pGridWorkInfoN;
	CGridCtrl m_pGridTimeInfo;
	
	CGridCtrl m_pGridRobotTurnPos;
	CGridCtrl m_pGridLeftTurn;
	CGridCtrl m_pGridYieldL;
	CGridCtrl m_pGridYieldD;
	CGridCtrl m_pGridFrontSmema;
	CGridCtrl m_pGridRearSmema;
	CGridCtrl m_GridTm;
	CGridCtrl m_GridTm_2;
	CGridCtrl m_GridTm_3;

	CImageList			*m_pImagelist;

	CBitmap				m_bmGround;

	DWORD				m_dwAnimateWaitTime[3];

	// jtkim 20150316
	void				OnMainCountDisplay();
	void				OnMainTimeDisplay();
	void                OnMainPcbWork();
	
	void                OnMainPcbReverse();

	void				OnInitButton();
	void				OnInitGroupBox();
	void				OnInitLabel();
	void				OnInitDigit();
	void				OnInitGridWorkInfoCurr();
	void				OnInitGridWorkInfoNext();
	void				OnInitGridTimeInfo(int nMode);
	void				OnMainDisplay();
	
	void                OnInitGridPickerTurnPos();
	void				OnInitGridYieldLot();
	void				OnInitGridYieldDaily();

	void                OnMainRobotLeftTurnDisplay();
	void                OnMainRobotRightTurnDisplay();
	void                OnMainRobotCenterDisplay();

	void                OnMainLotDisplay();

	void                OnInitGridFrontSmema();
	void                OnInitGridRearSmema();

	void				OnMainDisplayFrontSmema();
	void				OnMainDisplayRearSmema();

	void				SetTrayPos(int ildx);
	void				TurnPositionGrid();
	void				ReversePositionGrid();
	void                OninitTrayMove();
	void				SetIO_OnOff(int nID,int Onoff);
	int                 GetCustomTrayMove(int iIdx);
	int                 m_nTemp;
	bool                mbTurn;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	virtual BOOL DestroyWindow();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg LRESULT OnMainWorkInfoDisplay(WPARAM wParam, LPARAM lParam);  // 화면 데이터 갱신용 사용자 메시지
	afx_msg LRESULT	OnMainWorkInfoCommand(WPARAM wParam, LPARAM lParam);
	

	afx_msg LRESULT	OnMainCvInMoveDisplay(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnMainCvTurnMoveDisplay(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnMainCvOutMoveDisplay(WPARAM wParam, LPARAM lParam);

	
	

	afx_msg LRESULT	OnMainIODisplay(WPARAM wParam, LPARAM lParam); 
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	
	virtual void PostNcDestroy();
	afx_msg void OnDestroy();
	afx_msg void OnClickedBtnMainAnimate();
	afx_msg void OnClickedBtnMainData();
	afx_msg void OnBnClickedButtonTrackOut();
	afx_msg void OnTimeCellClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnWorkInfoCurrCellClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	


	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnCvIn();
	afx_msg void OnBnClickedBtnCvInPos();
	afx_msg void OnBnClickedBtnCvInUpdw();
	afx_msg void OnBnClickedBtnTurnPos();
	afx_msg void OnBnClickedBtnTurnUpDw();
	afx_msg void OnBnClickedBtnOutInPos();
	afx_msg void OnBnClickedBtnOutPos();
	afx_msg void OnBnClickedBtnOutPosUpDw();

	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnStop2();
	afx_msg void OnBnClickedBtnStop3();
	afx_msg void OnBnClickedBtnStart2();
	afx_msg void OnBnClickedBtnStart3();
	afx_msg void OnBnClickedBtnStart4();
	afx_msg void OnBnClickedBtnFrontSmema();
	afx_msg void OnBnClickedBtnMidConvReq();
	afx_msg void OnBnClickedBtnCvInPos2();
	afx_msg void OnBnClickedBtnOutPosUpDw2();
	afx_msg void OnBnClickedBtnOutPosUpDw3();
	afx_msg void OnBnClickedBtnCvInPos3();
	afx_msg void OnBnClickedBtnOutPosUpDw5();
};


