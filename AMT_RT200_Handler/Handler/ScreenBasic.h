#pragma once



// CScreenBasic 폼 뷰입니다.

#include "Variable.h"
#include "XPGroupBox.h"
#include ".\\Union\\Grid\\GridCtrl.h"
#include ".\\Union\\Grid\\GridURLCell.h"
#include ".\\Union\\Grid\\GridCellCombo.h"
#include ".\\Union\\Grid\\GridCellCheck.h"
#include ".\\Union\\Grid\\GridCellNumeric.h"
#include ".\\Union\\Grid\\GridCellDateTime.h"
#include "BtnST.h"
#include "GradientStatic.h"
#include "Digit.h"
#include "afxwin.h"



class CScreenBasic : public CFormView
{
	DECLARE_DYNCREATE(CScreenBasic)

public:
	int					m_nDeviceName;					// 선택된 디바이스의 리스트 위치 저장 변수
	int					m_nOldDeviceName;
	int					m_nModeDevice[2];
	int					m_nModeInterface[2];
	int					m_nModeSecsGem[2];
	int					m_nModeWork[2];
	int					m_nModeFrontSmema[2];
	int					m_nModeRearSmema[2];
	int					m_nModeRfid[2];
	int					m_nBarcodeErrorSkip[2]; //kwlee 2017.0204
	//kwlee 2016.0323
	int					m_nColCnt[2];
	int					m_nRowCnt[2];
	int                 m_RobotPickPos[2];
	int                 m_RobotPlacePos[2];
	int                 m_nVisionErrorCnt[2]; //kwlee 2017.0202
	int                 m_nBarCodeReadPos[2];

	int                 mn_Size;
	int					m_nPcbTurn;
	int					m_nRobotPickMode;
	int					m_nRobotPlaceMode;

	
	bool                mbGridCheck;
	/////
	bool                mb_GridInit;
	CString				m_strTempDevice;				// 선택된 디바이스 종류 임시 저장 변수
	CString				m_strLoadFile;					// 로딩할 파일에 대한 [폴더]+[파일명] 저장 변수
	CString				m_strDeviceName[2];
	CString				m_strModelName[2];

	CButtonST			m_btnModelCreate;
	CButtonST			m_btnModelDelete;
	CButtonST			m_btnApply;
	CButtonST			m_btnReLoad;

	CXPGroupBox m_groupPcbSetting;
	CGradientStatic m_msgPcbRowCnt;
	CGradientStatic m_msgPcbColCnt;
	CDigit m_dgtPcbRowCnt;
	CDigit m_dgtPcbColCnt;

	CXPGroupBox			m_groupModelInfo;
	CXPGroupBox			m_groupDeviceMode;

	CXPGroupBox			m_groupProcess;
	CXPGroupBox			m_groupRobotPickPos;
	CXPGroupBox			m_groupRobotPlacePos;


	CGridCtrl			m_pGridDevice;
	CGridCtrl			m_pGridModel;
	CGridCtrl			m_pGridHifix;
	CGridCtrl			m_GridPcbType;
	CButtonST			m_btnPcbCreate;
	CButtonST			m_BtnSodimm;
	CButtonST			m_BtnRdimm;
	CButtonST			m_BtnUdimm_9;
	CButtonST			m_BtnUdimm_10;
	
	void				OnInitGroupBox();
	void                OnInitPickerSelect();
	void				OnInitLabel();
	void				OnInitButton();
	void				OnInitDigit();
	void				OnInitGridDeviceMode();
	void				OnInitGridModelList();
	void				OnDataHistoryLog();
	void				OnDeviceColorChange(int nSelectRow, int nSelectCol);
	void				OnDataRecovery();
	int					OnDataComparison();
	void				OnDataApply();
	void				OnDataBackup();
	void				OnDataInit();
	int					OnInitModelList(CString strFileName);
	int					OnAddItem(int nIndex, WIN32_FIND_DATA *pFd);
	void				OnDisplayModelList(int nMode, int nCount, CString strFileName);
	bool				OnModelDeviceCheck(CString strFileName);
	bool				OnModelCreateDeviceCheck(CString strFileName);

	//kwlee 2016.0323
	void                OnInitPcbType();
	void                OnSetPos();
	int                 OnPickerNum();
public:
	CScreenBasic();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CScreenBasic();

public:
	enum { IDD = IDD_SCREEN_BASIC };
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
	virtual void OnInitialUpdate();
	virtual BOOL DestroyWindow();
	afx_msg void OnDeviceCellClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	//afx_msg void OnWorkModeCellClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnModelCellClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnModelCellRClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnClickedBtnModelCreate();
	afx_msg void OnClickedBtnModelDelete();
	afx_msg void OnClickedBtnReload();
	afx_msg void OnClickedBtnApply();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void PostNcDestroy();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	
	afx_msg void OnStnClickedDgtPcbRowCnt();
	afx_msg void OnStnClickedDgtPcbColCnt();
	afx_msg void OnBnClickedRadioPcbTurn();
	afx_msg void OnBnClickedRadioPcbNoturn();

	afx_msg void OnBnClickedBtnPcbCreate();
	afx_msg void OnPcbCellClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnPickerSelect(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnBnClickedBtnSodimm();
	afx_msg void OnBnClickedBtnRdimm();
	afx_msg void OnBnClickedBtnUdimm9();
	afx_msg void OnBnClickedBtnUdimm10();
	
	afx_msg void OnBnClickedRadioPlacePos0();
	afx_msg void OnBnClickedRadioPickPos0();
	afx_msg void OnRadioPickPos90();
	afx_msg void OnRadioPickPosMin90();
	afx_msg void OnRadioPlacePos90();
	afx_msg void OnRadioPlacePosMin90();

	afx_msg void OnBnClickedRadioBasicPickCenter();
	afx_msg void OnRadioBasicPickRight();
	afx_msg void OnRadioBasicPickLeft();
	afx_msg void OnBnClickedRadioBasicPlaceCenter();
	afx_msg void OnRadioBasicPlaceRight();
	afx_msg void OnRadioBasicPlaceLeft();
	CXPGroupBox m_group_Picker_Select;
	CGridCtrl m_GridPickerSelect;
	CDigit m_dgtVisionErrorCnt;
	afx_msg void OnStnClickedDgtVisionErrorCnt();
	CGradientStatic m_msgVisionErrorCnt;
	CGradientStatic m_msgBarcodeReadPos;
	CDigit m_dgtBarcodeReadPos;
	afx_msg void OnStnClickedDgtBarcodeReadPos();
};


