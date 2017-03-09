#pragma once


// CWorkInterface 대화 상자입니다.
#include "Variable.h"
#include "XPGroupBox.h"
#include "xShadeButton.h"	// 조그 버튼 생성 클래스 추가
#include "GradientStatic.h"  // 그라데이션 라벨 클래스
#include "BtnST.h"
#include "afxcmn.h"
#include "afxwin.h"

class CWorkInterface : public CDialog
{
	DECLARE_DYNAMIC(CWorkInterface)
public:
	DWORD			m_dwWaitTime[3];

	BYTE			m_byIp[4];		// 0: FTP, 1: TAMS

	int				m_nBarcodePos;
	int				m_nBarcodeStep;
	int				m_nRfidStep;
	int				m_nLightValue;
	int				m_nClientCnt;
	int				m_nServerCnt;
	int				m_nClientPort[10][2];
	int				m_nServerPort[10][2];
	int				m_nPort[10][2];
	int				m_nData[10][2];
	int				m_nParity[10][2];
	int				m_nStop[10][2];
	int				m_nRate[10][2];
	BOOL			m_bUnldBcrFlag[2][2];

	CString			m_strClientIp[10][2];
	CString			m_strRfidData[20];
	int             m_nCntBcrNum;
	void			OnInterfaceIpAddress(CString strIp);
	int				ConverterToData(int nMode, int nPos);
	int				ConverterToPos(int nMode, int Ndata);
	int				OnBarcodeLoader();
	int				OnBarcodeUnLoaderA();
 	int				OnBarcodeUnLoaderB();

	int				OnRfidRead();
	int				OnRfidWrite();
	void			OnInitGroupBox();
	void			OnInitLabel();
	void			OnInitButton();
	void			OnInterface_Display();
	void			OnDataHistoryLog();
	void			OnDataRecovery();
	int				OnDataComparison();
	void			OnDataApply();
	void			OnDataBackup();
	void			OnDataInit();
public:
	CWorkInterface(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CWorkInterface();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_WORK_INTERFACE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	CXPGroupBox		m_groupSerial1;
	CXPGroupBox		m_groupClient1;
	CXPGroupBox		m_groupClient2;
	CXPGroupBox		m_groupClient3;
	CXPGroupBox		m_groupClient4;
	CXPGroupBox		m_groupClient5;
	CXPGroupBox		m_groupClient6;
	CXPGroupBox		m_groupClient7;
	CXPGroupBox		m_groupServer1;
	CXPGroupBox		m_groupCommand;
	CXPGroupBox		m_groupLdBcr;
	CXPGroupBox		m_groupUnldBcrA;
	CXPGroupBox		m_groupUnldBcrB;
	CXPGroupBox		m_groupBcr1;
	CXPGroupBox		m_groupBcr2;

	CGradientStatic m_msgPort[5];
	CGradientStatic m_msgRate[5];
	CGradientStatic m_msgData[5];
	CGradientStatic m_msgStop[5];
	CGradientStatic m_msgParity[5];

	CGradientStatic m_msgClientIp1;
	CGradientStatic m_msgClientPort1;
	CGradientStatic m_msgClientIp2;
	CGradientStatic m_msgClientPort2;
	CGradientStatic m_msgClientIp3;
	CGradientStatic m_msgClientPort3;
	CGradientStatic m_msgClientIp4;
	CGradientStatic m_msgClientPort4;
	CGradientStatic m_msgClientIp5;
	CGradientStatic m_msgClientPort5;
	CGradientStatic m_msgClientIp6;
	CGradientStatic m_msgClientPort6;
	CGradientStatic m_msgClientIp7;
	CGradientStatic m_msgClientPort7;
	CGradientStatic m_msgServerPort1;
	//kwlee 2017.0302
	CGradientStatic m_msgBcrIp_1;
	CGradientStatic m_msgBcrIp_2;
	CGradientStatic m_msgBcrPort1;
	CGradientStatic m_msgBcrPort2;

	CButtonST		m_btnSerialApply[5];
	CButtonST		m_btnSerialConvet[5];
	CButtonST		m_btnClientIpSet[7];
	CButtonST		m_btnClientPortSet[7];
	CButtonST		m_btnServerPortSet[3];
	CButtonST		m_btnApply;
	CButtonST		m_btnReLoad;
	CButtonST		m_btnBcodeOn[3];
	CButtonST		m_btnBcodeOff;
	CButtonST		m_btnUnldBcr[2];
	//kwlee 2017.0302
	CButtonST       m_btnBcrIP[2];
	CButtonST       m_btnBcrPort[2];
	
	CListBox		m_listBarcode;

	CComboBox		m_cbPort[5];
	CComboBox		m_cbRate[5];
	CComboBox		m_cbData[5];
	CComboBox		m_cbStop[5];
	CComboBox		m_cbParity[5];

	CIPAddressCtrl	m_clientIp1;
	CIPAddressCtrl	m_clientIp2;
	CIPAddressCtrl	m_clientIp3;
	CIPAddressCtrl	m_clientIp4;
	CIPAddressCtrl	m_clientIp5;
	CIPAddressCtrl	m_clientIp6;
	CIPAddressCtrl	m_clientIp7;
	CIPAddressCtrl m_clientBcrIP1;
	CIPAddressCtrl m_clientBcrIP2;

	CEdit			m_editClientPort1;
	CEdit			m_editClientPort2;
	CEdit			m_editClientPort3;
	CEdit			m_editClientPort4;
	CEdit			m_editClientPort5;
	CEdit			m_editClientPort6;
	CEdit			m_editClientPort7;
	CEdit			m_editServerPort1;
	CEdit			m_editBcrPort1;
	CEdit			m_editBcrPort2;

	afx_msg void OnBnClickedBtnSerialApply1();
	afx_msg void OnBnClickedBtnSerialConnect1();
	afx_msg void OnBnClickedBtnClientIp1();
	afx_msg void OnBnClickedBtnClientPort1();
	afx_msg void OnBnClickedBtnClientIp2();
	afx_msg void OnBnClickedBtnClientPort2();
	afx_msg void OnBnClickedBtnClientIp3();
	afx_msg void OnBnClickedBtnClientPort3();
	afx_msg void OnBnClickedBtnServerPort1();
	afx_msg void OnBnClickedBtnApply();
	afx_msg void OnBnClickedBtnReload();
	virtual void PostNcDestroy();
	afx_msg void OnDestroy();
	afx_msg void OnClickedBtnBcodeTriggerOn();
	afx_msg void OnClickedBtnBcodeTriggerOff();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnClientIp4();
	afx_msg void OnBnClickedBtnClientPort4();
	afx_msg void OnBnClickedBtnClientIp5();
	afx_msg void OnBnClickedBtnClientPort5();
	afx_msg void OnBnClickedBtnClientIp6();
	afx_msg void OnBnClickedBtnClientPort6();
	afx_msg void OnBnClickedBtnSerialApply2();
	afx_msg void OnBnClickedBtnSerialConnect2();
	afx_msg void OnBnClickedBtnSerialApply3();
	afx_msg void OnBnClickedBtnSerialConnect3();
	afx_msg void OnBnClickedBtnBcodeTriggerOn2();
	afx_msg void OnBnClickedBtnBcodeTriggerOn3();
	afx_msg void OnBnClickedBtnClientIp7();
	afx_msg void OnBnClickedBtnClientPort7();
	afx_msg void OnBnClickedCheckUnloadBarcodeA();
	afx_msg void OnBnClickedCheckUnloadBarcodeB();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonRead1();
	afx_msg void OnBnClickedButtonRead2();
	afx_msg void OnBnClickedButtonBcr1();
	afx_msg void OnBnClickedButtonBcr2();
	
	afx_msg void OnBnClickedBtnBcr1ClientIp();
	afx_msg void OnBnClickedBtnBcr2ClientIp();
	afx_msg void OnBnClickedBtnBcr1ClientPort();
	afx_msg void OnBnClickedBtnBcr2ClientPort();
};
