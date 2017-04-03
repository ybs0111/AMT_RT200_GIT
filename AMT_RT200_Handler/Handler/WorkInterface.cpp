// WorkInterface.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Handler.h"
#include "WorkInterface.h"
#include "afxdialogex.h"
#include "PublicFunction.h"
#include "Dialog_KeyBoard.h"
#include "Dialog_KeyPad.h"
#include "Dialog_Message.h"
#include "CtlBd_Variable.h"
#include "Comm.h"
#include ".\\Ctrl\\KeyBoard.h"

#include "AlgMemory.h"
// CWorkInterface 대화 상자입니다.
#define TM_BARCODE			100

IMPLEMENT_DYNAMIC(CWorkInterface, CDialog)

CWorkInterface::CWorkInterface(CWnd* pParent /*=NULL*/)
	: CDialog(CWorkInterface::IDD, pParent)
{
//	int i;
	m_nCntBcrNum = 0;
	m_nBarcodePos = 0;
}

CWorkInterface::~CWorkInterface()
{
}

void CWorkInterface::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GROUP_SERIAL_1,			m_groupSerial1);
	DDX_Control(pDX, IDC_GROUP_NETWORK_1,			m_groupClient1);
	DDX_Control(pDX, IDC_GROUP_NETWORK_2,			m_groupClient2);
	DDX_Control(pDX, IDC_GROUP_NETWORK_3,			m_groupClient3);
	DDX_Control(pDX, IDC_GROUP_NETWORK_4,			m_groupClient4);
	DDX_Control(pDX, IDC_GROUP_NETWORK_5,			m_groupClient5);
	DDX_Control(pDX, IDC_GROUP_NETWORK_6,			m_groupClient6);
	DDX_Control(pDX, IDC_GROUP_NETWORK_7,			m_groupClient7);
	DDX_Control(pDX, IDC_GROUP_SERVER_1,			m_groupServer1);
	DDX_Control(pDX, IDC_GROUP_COMMAND,				m_groupCommand);
	DDX_Control(pDX, IDC_GROUP_LOAD_BARCODE,		m_groupLdBcr);
	DDX_Control(pDX, IDC_GROUP_UNLOAD_BARCODE_A,	m_groupUnldBcrA);
	DDX_Control(pDX, IDC_GROUP_UNLOAD_BARCODE_B,	m_groupUnldBcrB);
	DDX_Control(pDX, IDC_MSG_PORT_1,				m_msgPort[0]);
	DDX_Control(pDX, IDC_MSG_RATE_1,				m_msgRate[0]);
	DDX_Control(pDX, IDC_MSG_DATA_1,				m_msgData[0]);
	DDX_Control(pDX, IDC_MSG_STOP_1,				m_msgStop[0]);
	DDX_Control(pDX, IDC_MSG_PARITY_1,				m_msgParity[0]);
	DDX_Control(pDX, IDC_MSG_PORT_2,				m_msgPort[1]);
	DDX_Control(pDX, IDC_MSG_RATE_2,				m_msgRate[1]);
	DDX_Control(pDX, IDC_MSG_DATA_2,				m_msgData[1]);
	DDX_Control(pDX, IDC_MSG_STOP_2,				m_msgStop[1]);
	DDX_Control(pDX, IDC_MSG_PARITY_2,				m_msgParity[1]);
	DDX_Control(pDX, IDC_MSG_PORT_3,				m_msgPort[2]);
	DDX_Control(pDX, IDC_MSG_RATE_3,				m_msgRate[2]);
	DDX_Control(pDX, IDC_MSG_DATA_3,				m_msgData[2]);
	DDX_Control(pDX, IDC_MSG_STOP_3,				m_msgStop[2]);
	DDX_Control(pDX, IDC_MSG_PARITY_3,				m_msgParity[2]);
	DDX_Control(pDX, IDC_MSG_CLIENT_IP_1,			m_msgClientIp1);
	DDX_Control(pDX, IDC_MSG_CLIENT_PORT_1,			m_msgClientPort1);
	DDX_Control(pDX, IDC_MSG_CLIENT_IP_2,			m_msgClientIp2);
	DDX_Control(pDX, IDC_MSG_CLIENT_PORT_2,			m_msgClientPort2);
	DDX_Control(pDX, IDC_MSG_CLIENT_IP_3,			m_msgClientIp3);
	DDX_Control(pDX, IDC_MSG_CLIENT_PORT_3,			m_msgClientPort3);
	DDX_Control(pDX, IDC_MSG_CLIENT_IP_4,			m_msgClientIp4);
	DDX_Control(pDX, IDC_MSG_CLIENT_PORT_4,			m_msgClientPort4);
	DDX_Control(pDX, IDC_MSG_CLIENT_IP_5,			m_msgClientIp5);
	DDX_Control(pDX, IDC_MSG_CLIENT_PORT_5,			m_msgClientPort5);
	DDX_Control(pDX, IDC_MSG_CLIENT_IP_6,			m_msgClientIp6);
	DDX_Control(pDX, IDC_MSG_CLIENT_PORT_6,			m_msgClientPort6);
	DDX_Control(pDX, IDC_MSG_CLIENT_IP_7,			m_msgClientIp7);
	DDX_Control(pDX, IDC_MSG_CLIENT_PORT_7,			m_msgClientPort7);
	DDX_Control(pDX, IDC_MSG_SERVER_PORT_1,			m_msgServerPort1);
	DDX_Control(pDX, IDC_COMBO_PORT_1,				m_cbPort[0]);
	DDX_Control(pDX, IDC_COMBO_RATE_1,				m_cbRate[0]);
	DDX_Control(pDX, IDC_COMBO_DATA_1,				m_cbData[0]);
	DDX_Control(pDX, IDC_COMBO_STOP_1,				m_cbStop[0]);
	DDX_Control(pDX, IDC_COMBO_PARITY_1,			m_cbParity[0]);
	DDX_Control(pDX, IDC_COMBO_PORT_2,				m_cbPort[1]);
	DDX_Control(pDX, IDC_COMBO_RATE_2,				m_cbRate[1]);
	DDX_Control(pDX, IDC_COMBO_DATA_2,				m_cbData[1]);
	DDX_Control(pDX, IDC_COMBO_STOP_2,				m_cbStop[1]);
	DDX_Control(pDX, IDC_COMBO_PARITY_2,			m_cbParity[1]);
	DDX_Control(pDX, IDC_COMBO_PORT_3,				m_cbPort[2]);
	DDX_Control(pDX, IDC_COMBO_RATE_3,				m_cbRate[2]);
	DDX_Control(pDX, IDC_COMBO_DATA_3,				m_cbData[2]);
	DDX_Control(pDX, IDC_COMBO_STOP_3,				m_cbStop[2]);
	DDX_Control(pDX, IDC_COMBO_PARITY_3,			m_cbParity[2]);
	DDX_Control(pDX, IDC_CLIENT_IP_1,				m_clientIp1);
	DDX_Control(pDX, IDC_CLIENT_IP_2,				m_clientIp2);
	DDX_Control(pDX, IDC_CLIENT_IP_3,				m_clientIp3);
	DDX_Control(pDX, IDC_CLIENT_IP_4,				m_clientIp4);
	DDX_Control(pDX, IDC_CLIENT_IP_5,				m_clientIp5);
	DDX_Control(pDX, IDC_CLIENT_IP_6,				m_clientIp6);
	DDX_Control(pDX, IDC_CLIENT_IP_7,				m_clientIp7);
	DDX_Control(pDX, IDC_EDIT_CLIENT_PORT_1,		m_editClientPort1);
	DDX_Control(pDX, IDC_EDIT_CLIENT_PORT_2,		m_editClientPort2);
	DDX_Control(pDX, IDC_EDIT_CLIENT_PORT_3,		m_editClientPort3);
	DDX_Control(pDX, IDC_EDIT_CLIENT_PORT_4,		m_editClientPort4);
	DDX_Control(pDX, IDC_EDIT_CLIENT_PORT_5,		m_editClientPort5);
	DDX_Control(pDX, IDC_EDIT_CLIENT_PORT_6,		m_editClientPort6);
	DDX_Control(pDX, IDC_EDIT_CLIENT_PORT_7,		m_editClientPort7);
	DDX_Control(pDX, IDC_EDIT_SERVER_PORT_1,		m_editServerPort1);
	DDX_Control(pDX, IDC_BTN_APPLY,					m_btnApply);
	DDX_Control(pDX, IDC_BTN_RELOAD,				m_btnReLoad);
	DDX_Control(pDX, IDC_BTN_SERIAL_APPLY_1,		m_btnSerialApply[0]);
	DDX_Control(pDX, IDC_BTN_SERIAL_CONNECT_1,		m_btnSerialConvet[0]);
	DDX_Control(pDX, IDC_BTN_SERIAL_APPLY_2,		m_btnSerialApply[1]);
	DDX_Control(pDX, IDC_BTN_SERIAL_CONNECT_2,		m_btnSerialConvet[1]);
	DDX_Control(pDX, IDC_BTN_SERIAL_APPLY_3,		m_btnSerialApply[2]);
	DDX_Control(pDX, IDC_BTN_SERIAL_CONNECT_3,		m_btnSerialConvet[2]);
	DDX_Control(pDX, IDC_BTN_CLIENT_IP_1,			m_btnClientIpSet[0]);
	DDX_Control(pDX, IDC_BTN_CLIENT_PORT_1,			m_btnClientPortSet[0]);
	DDX_Control(pDX, IDC_BTN_CLIENT_IP_2,			m_btnClientIpSet[1]);
	DDX_Control(pDX, IDC_BTN_CLIENT_PORT_2,			m_btnClientPortSet[1]);
	DDX_Control(pDX, IDC_BTN_CLIENT_IP_3,			m_btnClientIpSet[2]);
	DDX_Control(pDX, IDC_BTN_CLIENT_PORT_3,			m_btnClientPortSet[2]);
	DDX_Control(pDX, IDC_BTN_CLIENT_IP_4,			m_btnClientIpSet[3]);
	DDX_Control(pDX, IDC_BTN_CLIENT_PORT_4,			m_btnClientPortSet[3]);
	DDX_Control(pDX, IDC_BTN_CLIENT_IP_5,			m_btnClientIpSet[4]);
	DDX_Control(pDX, IDC_BTN_CLIENT_PORT_5,			m_btnClientPortSet[4]);
	DDX_Control(pDX, IDC_BTN_CLIENT_IP_6,			m_btnClientIpSet[5]);
	DDX_Control(pDX, IDC_BTN_CLIENT_PORT_6,			m_btnClientPortSet[5]);
	DDX_Control(pDX, IDC_BTN_CLIENT_IP_7,			m_btnClientIpSet[6]);
	DDX_Control(pDX, IDC_BTN_CLIENT_PORT_7,			m_btnClientPortSet[6]);
	//kwlee 2017.0302
	DDX_Control(pDX, IDC_BTN_BCR_1_CLIENT_IP,			m_btnBcrIP[0]);
	DDX_Control(pDX, IDC_BTN_BCR_1_CLIENT_PORT,			m_btnBcrPort[0]);
	DDX_Control(pDX, IDC_BTN_BCR_2_CLIENT_IP,			m_btnBcrIP[1]);
	DDX_Control(pDX, IDC_BTN_BCR_2_CLIENT_PORT,			m_btnBcrPort[1]);
	//
	DDX_Control(pDX, IDC_BTN_SERVER_PORT_1,			m_btnServerPortSet[0]);
	DDX_Control(pDX, IDC_CHECK_UNLOAD_BARCODE_A,	m_btnUnldBcr[0]);
	DDX_Control(pDX, IDC_CHECK_UNLOAD_BARCODE_B,	m_btnUnldBcr[1]);
	DDX_Control(pDX, IDC_LIST_RESPONSE_BARCODE,		m_listBarcode);
	DDX_Control(pDX, IDC_BTN_BCODE_TRIGGER_ON,		m_btnBcodeOn[0]);
	DDX_Control(pDX, IDC_BTN_BCODE_TRIGGER_ON2,		m_btnBcodeOn[1]);
	DDX_Control(pDX, IDC_BTN_BCODE_TRIGGER_ON3,		m_btnBcodeOn[2]);
	DDX_Control(pDX, IDC_BTN_BCODE_TRIGGER_OFF,		m_btnBcodeOff);
	DDX_Control(pDX, IDC_GROUP_BCR_NETWORK_1, m_groupBcr1);
	DDX_Control(pDX, IDC_GROUP_BCR_2_NETWORK, m_groupBcr2);
	DDX_Control(pDX, IDC_MSG_BCR_1_CLIENT_IP, m_msgBcrIp_1);
	DDX_Control(pDX, IDC_MSG_BCR_2_CLIENT_IP, m_msgBcrIp_2);
	DDX_Control(pDX, IDC_BCR_1_CLIENT_IP, m_clientBcrIP1);
	DDX_Control(pDX, IDC_BCR_2_CLIENT_IP, m_clientBcrIP2);
	DDX_Control(pDX, IDC_MSG_BCR_1_CLIENT_PORT, m_msgBcrPort1);
	DDX_Control(pDX, IDC_MSG_BCR_2_CLIENT_PORT, m_msgBcrPort2);
	DDX_Control(pDX, IDC_EDIT_BCR_1_CLIENT_PORT, m_editBcrPort1);
	DDX_Control(pDX, IDC_EDIT_BCR_2_CLIENT_PORT, m_editBcrPort2);
}


BEGIN_MESSAGE_MAP(CWorkInterface, CDialog)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_SERIAL_APPLY_1,			&CWorkInterface::OnBnClickedBtnSerialApply1)
	ON_BN_CLICKED(IDC_BTN_SERIAL_CONNECT_1,			&CWorkInterface::OnBnClickedBtnSerialConnect1)
	ON_BN_CLICKED(IDC_BTN_CLIENT_IP_1,				&CWorkInterface::OnBnClickedBtnClientIp1)
	ON_BN_CLICKED(IDC_BTN_CLIENT_PORT_1,			&CWorkInterface::OnBnClickedBtnClientPort1)
	ON_BN_CLICKED(IDC_BTN_CLIENT_IP_2,				&CWorkInterface::OnBnClickedBtnClientIp2)
	ON_BN_CLICKED(IDC_BTN_CLIENT_PORT_2,			&CWorkInterface::OnBnClickedBtnClientPort2)
	ON_BN_CLICKED(IDC_BTN_CLIENT_IP_3,				&CWorkInterface::OnBnClickedBtnClientIp3)
	ON_BN_CLICKED(IDC_BTN_CLIENT_PORT_3,			&CWorkInterface::OnBnClickedBtnClientPort3)
	ON_BN_CLICKED(IDC_BTN_SERVER_PORT_1,			&CWorkInterface::OnBnClickedBtnServerPort1)
	ON_BN_CLICKED(IDC_BTN_APPLY,					&CWorkInterface::OnBnClickedBtnApply)
	ON_BN_CLICKED(IDC_BTN_RELOAD,					&CWorkInterface::OnBnClickedBtnReload)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_BCODE_TRIGGER_ON,			&CWorkInterface::OnClickedBtnBcodeTriggerOn)
	ON_BN_CLICKED(IDC_BTN_BCODE_TRIGGER_OFF,		&CWorkInterface::OnClickedBtnBcodeTriggerOff)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_CLIENT_IP_4, &CWorkInterface::OnBnClickedBtnClientIp4)
	ON_BN_CLICKED(IDC_BTN_CLIENT_PORT_4, &CWorkInterface::OnBnClickedBtnClientPort4)
	ON_BN_CLICKED(IDC_BTN_CLIENT_IP_5, &CWorkInterface::OnBnClickedBtnClientIp5)
	ON_BN_CLICKED(IDC_BTN_CLIENT_PORT_5, &CWorkInterface::OnBnClickedBtnClientPort5)
	ON_BN_CLICKED(IDC_BTN_CLIENT_IP_6, &CWorkInterface::OnBnClickedBtnClientIp6)
	ON_BN_CLICKED(IDC_BTN_CLIENT_PORT_6, &CWorkInterface::OnBnClickedBtnClientPort6)
	ON_BN_CLICKED(IDC_BTN_SERIAL_APPLY_2, &CWorkInterface::OnBnClickedBtnSerialApply2)
	ON_BN_CLICKED(IDC_BTN_SERIAL_CONNECT_2, &CWorkInterface::OnBnClickedBtnSerialConnect2)
	ON_BN_CLICKED(IDC_BTN_SERIAL_APPLY_3, &CWorkInterface::OnBnClickedBtnSerialApply3)
	ON_BN_CLICKED(IDC_BTN_SERIAL_CONNECT_3, &CWorkInterface::OnBnClickedBtnSerialConnect3)
	ON_BN_CLICKED(IDC_BTN_BCODE_TRIGGER_ON2, &CWorkInterface::OnBnClickedBtnBcodeTriggerOn2)
	ON_BN_CLICKED(IDC_BTN_BCODE_TRIGGER_ON3, &CWorkInterface::OnBnClickedBtnBcodeTriggerOn3)
	ON_BN_CLICKED(IDC_BTN_CLIENT_IP_7, &CWorkInterface::OnBnClickedBtnClientIp7)
	ON_BN_CLICKED(IDC_BTN_CLIENT_PORT_7, &CWorkInterface::OnBnClickedBtnClientPort7)
	ON_BN_CLICKED(IDC_CHECK_UNLOAD_BARCODE_A, &CWorkInterface::OnBnClickedCheckUnloadBarcodeA)
	ON_BN_CLICKED(IDC_CHECK_UNLOAD_BARCODE_B, &CWorkInterface::OnBnClickedCheckUnloadBarcodeB)
	ON_BN_CLICKED(IDC_BUTTON6, &CWorkInterface::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CWorkInterface::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_READ_1, &CWorkInterface::OnBnClickedButtonRead1)
	ON_BN_CLICKED(IDC_BUTTON_READ_2, &CWorkInterface::OnBnClickedButtonRead2)
	ON_BN_CLICKED(IDC_BUTTON_BCR1, &CWorkInterface::OnBnClickedButtonBcr1)
	ON_BN_CLICKED(IDC_BUTTON_BCR2, &CWorkInterface::OnBnClickedButtonBcr2)
	ON_BN_CLICKED(IDC_BTN_BCR_1_CLIENT_IP, &CWorkInterface::OnBnClickedBtnBcr1ClientIp)
	ON_BN_CLICKED(IDC_BTN_BCR_2_CLIENT_IP, &CWorkInterface::OnBnClickedBtnBcr2ClientIp)
	ON_BN_CLICKED(IDC_BTN_BCR_1_CLIENT_PORT, &CWorkInterface::OnBnClickedBtnBcr1ClientPort)
	ON_BN_CLICKED(IDC_BTN_BCR_2_CLIENT_PORT, &CWorkInterface::OnBnClickedBtnBcr2ClientPort)
END_MESSAGE_MAP()


// CWorkInterface 메시지 처리기입니다.


BOOL CWorkInterface::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
/*
	if (OnDataComparison() == RET_ERROR)
	{
		OnDataHistoryLog();
		OnDataApply();
	}
*/
	
	return CDialog::DestroyWindow();
}


BOOL CWorkInterface::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	OnDataInit();

	OnInitGroupBox();
	OnInitLabel();
	OnInitButton();

	OnInterface_Display();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CWorkInterface::OnInitGroupBox()
{
	m_groupSerial1.SetFont(clsFunc.OnLogFont(16));
	m_groupSerial1.SetCatptionTextColor(BLUE_C);
	m_groupSerial1.SetBorderColor(ORANGE_C);
	m_groupSerial1.SetBackgroundColor(WINDOW_UP1);
	m_groupSerial1.SetFontBold(TRUE);

	m_groupClient1.SetFont(clsFunc.OnLogFont(16));
	m_groupClient1.SetCatptionTextColor(BLUE_C);
	m_groupClient1.SetBorderColor(ORANGE_C);
	m_groupClient1.SetBackgroundColor(WINDOW_UP1);
	m_groupClient1.SetFontBold(TRUE);

	m_groupClient2.SetFont(clsFunc.OnLogFont(16));
	m_groupClient2.SetCatptionTextColor(BLUE_C);
	m_groupClient2.SetBorderColor(ORANGE_C);
	m_groupClient2.SetBackgroundColor(WINDOW_UP1);
	m_groupClient2.SetFontBold(TRUE);

	m_groupClient3.SetFont(clsFunc.OnLogFont(16));
	m_groupClient3.SetCatptionTextColor(BLUE_C);
	m_groupClient3.SetBorderColor(ORANGE_C);
	m_groupClient3.SetBackgroundColor(WINDOW_UP1);
	m_groupClient3.SetFontBold(TRUE);

	m_groupClient4.SetFont(clsFunc.OnLogFont(16));
	m_groupClient4.SetCatptionTextColor(BLUE_C);
	m_groupClient4.SetBorderColor(ORANGE_C);
	m_groupClient4.SetBackgroundColor(WINDOW_UP1);
	m_groupClient4.SetFontBold(TRUE);

	m_groupClient5.SetFont(clsFunc.OnLogFont(16));
	m_groupClient5.SetCatptionTextColor(BLUE_C);
	m_groupClient5.SetBorderColor(ORANGE_C);
	m_groupClient5.SetBackgroundColor(WINDOW_UP1);
	m_groupClient5.SetFontBold(TRUE);

	m_groupClient6.SetFont(clsFunc.OnLogFont(16));
	m_groupClient6.SetCatptionTextColor(BLUE_C);
	m_groupClient6.SetBorderColor(ORANGE_C);
	m_groupClient6.SetBackgroundColor(WINDOW_UP1);
	m_groupClient6.SetFontBold(TRUE);

	m_groupClient7.SetFont(clsFunc.OnLogFont(16));
	m_groupClient7.SetCatptionTextColor(BLUE_C);
	m_groupClient7.SetBorderColor(ORANGE_C);
	m_groupClient7.SetBackgroundColor(WINDOW_UP1);
	m_groupClient7.SetFontBold(TRUE);

	m_groupServer1.SetFont(clsFunc.OnLogFont(16));
	m_groupServer1.SetCatptionTextColor(BLUE_C);
	m_groupServer1.SetBorderColor(ORANGE_C);
	m_groupServer1.SetBackgroundColor(WINDOW_UP1);
	m_groupServer1.SetFontBold(TRUE);

	m_groupCommand.SetFont(clsFunc.OnLogFont(16));
	m_groupCommand.SetCatptionTextColor(BLUE_C);
	m_groupCommand.SetBorderColor(ORANGE_C);
	m_groupCommand.SetBackgroundColor(WINDOW_UP1);
	m_groupCommand.SetFontBold(TRUE);

	m_groupLdBcr.SetFont(clsFunc.OnLogFont(16));
	m_groupLdBcr.SetCatptionTextColor(BLUE_C);
	m_groupLdBcr.SetBorderColor(ORANGE_C);
	m_groupLdBcr.SetBackgroundColor(WINDOW_UP1);
	m_groupLdBcr.SetFontBold(TRUE);

	m_groupUnldBcrA.SetFont(clsFunc.OnLogFont(16));
	m_groupUnldBcrA.SetCatptionTextColor(BLUE_C);
	m_groupUnldBcrA.SetBorderColor(ORANGE_C);
	m_groupUnldBcrA.SetBackgroundColor(WINDOW_UP1);
	m_groupUnldBcrA.SetFontBold(TRUE);

	m_groupUnldBcrB.SetFont(clsFunc.OnLogFont(16));
	m_groupUnldBcrB.SetCatptionTextColor(BLUE_C);
	m_groupUnldBcrB.SetBorderColor(ORANGE_C);
	m_groupUnldBcrB.SetBackgroundColor(WINDOW_UP1);
	m_groupUnldBcrB.SetFontBold(TRUE);

	m_groupBcr1.SetFont(clsFunc.OnLogFont(16));
	m_groupBcr1.SetCatptionTextColor(BLUE_C);
	m_groupBcr1.SetBorderColor(ORANGE_C);
	m_groupBcr1.SetBackgroundColor(WINDOW_UP1);
	m_groupBcr1.SetFontBold(TRUE);

	m_groupBcr2.SetFont(clsFunc.OnLogFont(16));
	m_groupBcr2.SetCatptionTextColor(BLUE_C);
	m_groupBcr2.SetBorderColor(ORANGE_C);
	m_groupBcr2.SetBackgroundColor(WINDOW_UP1);
	m_groupBcr2.SetFontBold(TRUE);

}

void CWorkInterface::OnInitLabel()
{
	for(int i=0; i<3; i++)
	{
		m_msgPort[i].SetFont(clsFunc.m_pFont[1]);
		m_msgPort[i].SetWindowText(_T("Port"));
		m_msgPort[i].SetCenterText();
		m_msgPort[i].SetColor(WHITE_C);
		m_msgPort[i].SetGradientColor(ORANGE_C);
		m_msgPort[i].SetTextColor(BLACK_C);

		m_msgRate[i].SetFont(clsFunc.m_pFont[1]);
		m_msgRate[i].SetWindowText(_T("BaudRate"));
		m_msgRate[i].SetCenterText();
		m_msgRate[i].SetColor(WHITE_C);
		m_msgRate[i].SetGradientColor(ORANGE_C);
		m_msgRate[i].SetTextColor(BLACK_C);

		m_msgData[i].SetFont(clsFunc.m_pFont[1]);
		m_msgData[i].SetWindowText(_T("Data"));
		m_msgData[i].SetCenterText();
		m_msgData[i].SetColor(WHITE_C);
		m_msgData[i].SetGradientColor(ORANGE_C);
		m_msgData[i].SetTextColor(BLACK_C);

		m_msgStop[i].SetFont(clsFunc.m_pFont[1]);
		m_msgStop[i].SetWindowText(_T("Stop"));
		m_msgStop[i].SetCenterText();
		m_msgStop[i].SetColor(WHITE_C);
		m_msgStop[i].SetGradientColor(ORANGE_C);
		m_msgStop[i].SetTextColor(BLACK_C);

		m_msgParity[i].SetFont(clsFunc.m_pFont[1]);
		m_msgParity[i].SetWindowText(_T("Parity"));
		m_msgParity[i].SetCenterText();
		m_msgParity[i].SetColor(WHITE_C);
		m_msgParity[i].SetGradientColor(ORANGE_C);
		m_msgParity[i].SetTextColor(BLACK_C);
	}

	m_msgClientIp1.SetFont(clsFunc.m_pFont[1]);
	m_msgClientIp1.SetWindowText(_T("IP"));
	m_msgClientIp1.SetCenterText();
	m_msgClientIp1.SetColor(WHITE_C);
	m_msgClientIp1.SetGradientColor(ORANGE_C);
	m_msgClientIp1.SetTextColor(BLACK_C);

	m_msgClientPort1.SetFont(clsFunc.m_pFont[1]);
	m_msgClientPort1.SetWindowText(_T("Port"));
	m_msgClientPort1.SetCenterText();
	m_msgClientPort1.SetColor(WHITE_C);
	m_msgClientPort1.SetGradientColor(ORANGE_C);
	m_msgClientPort1.SetTextColor(BLACK_C);

	m_msgClientIp2.SetFont(clsFunc.m_pFont[1]);
	m_msgClientIp2.SetWindowText(_T("IP"));
	m_msgClientIp2.SetCenterText();
	m_msgClientIp2.SetColor(WHITE_C);
	m_msgClientIp2.SetGradientColor(ORANGE_C);
	m_msgClientIp2.SetTextColor(BLACK_C);

	m_msgClientPort2.SetFont(clsFunc.m_pFont[1]);
	m_msgClientPort2.SetWindowText(_T("Port"));
	m_msgClientPort2.SetCenterText();
	m_msgClientPort2.SetColor(WHITE_C);
	m_msgClientPort2.SetGradientColor(ORANGE_C);
	m_msgClientPort2.SetTextColor(BLACK_C);

	m_msgClientIp3.SetFont(clsFunc.m_pFont[1]);
	m_msgClientIp3.SetWindowText(_T("IP"));
	m_msgClientIp3.SetCenterText();
	m_msgClientIp3.SetColor(WHITE_C);
	m_msgClientIp3.SetGradientColor(ORANGE_C);
	m_msgClientIp3.SetTextColor(BLACK_C);

	m_msgClientPort3.SetFont(clsFunc.m_pFont[1]);
	m_msgClientPort3.SetWindowText(_T("Port"));
	m_msgClientPort3.SetCenterText();
	m_msgClientPort3.SetColor(WHITE_C);
	m_msgClientPort3.SetGradientColor(ORANGE_C);
	m_msgClientPort3.SetTextColor(BLACK_C);

	m_msgClientIp4.SetFont(clsFunc.m_pFont[1]);
	m_msgClientIp4.SetWindowText(_T("IP"));
	m_msgClientIp4.SetCenterText();
	m_msgClientIp4.SetColor(WHITE_C);
	m_msgClientIp4.SetGradientColor(ORANGE_C);
	m_msgClientIp4.SetTextColor(BLACK_C);

	m_msgClientPort4.SetFont(clsFunc.m_pFont[1]);
	m_msgClientPort4.SetWindowText(_T("Port"));
	m_msgClientPort4.SetCenterText();
	m_msgClientPort4.SetColor(WHITE_C);
	m_msgClientPort4.SetGradientColor(ORANGE_C);
	m_msgClientPort4.SetTextColor(BLACK_C);

	m_msgClientIp5.SetFont(clsFunc.m_pFont[1]);
	m_msgClientIp5.SetWindowText(_T("IP"));
	m_msgClientIp5.SetCenterText();
	m_msgClientIp5.SetColor(WHITE_C);
	m_msgClientIp5.SetGradientColor(ORANGE_C);
	m_msgClientIp5.SetTextColor(BLACK_C);

	m_msgClientPort5.SetFont(clsFunc.m_pFont[1]);
	m_msgClientPort5.SetWindowText(_T("Port"));
	m_msgClientPort5.SetCenterText();
	m_msgClientPort5.SetColor(WHITE_C);
	m_msgClientPort5.SetGradientColor(ORANGE_C);
	m_msgClientPort5.SetTextColor(BLACK_C);

	m_msgClientIp6.SetFont(clsFunc.m_pFont[1]);
	m_msgClientIp6.SetWindowText(_T("IP"));
	m_msgClientIp6.SetCenterText();
	m_msgClientIp6.SetColor(WHITE_C);
	m_msgClientIp6.SetGradientColor(ORANGE_C);
	m_msgClientIp6.SetTextColor(BLACK_C);

	m_msgClientPort6.SetFont(clsFunc.m_pFont[1]);
	m_msgClientPort6.SetWindowText(_T("Port"));
	m_msgClientPort6.SetCenterText();
	m_msgClientPort6.SetColor(WHITE_C);
	m_msgClientPort6.SetGradientColor(ORANGE_C);
	m_msgClientPort6.SetTextColor(BLACK_C);

	m_msgClientIp7.SetFont(clsFunc.m_pFont[1]);
	m_msgClientIp7.SetWindowText(_T("IP"));
	m_msgClientIp7.SetCenterText();
	m_msgClientIp7.SetColor(WHITE_C);
	m_msgClientIp7.SetGradientColor(ORANGE_C);
	m_msgClientIp7.SetTextColor(BLACK_C);

	m_msgClientPort7.SetFont(clsFunc.m_pFont[1]);
	m_msgClientPort7.SetWindowText(_T("Port"));
	m_msgClientPort7.SetCenterText();
	m_msgClientPort7.SetColor(WHITE_C);
	m_msgClientPort7.SetGradientColor(ORANGE_C);
	m_msgClientPort7.SetTextColor(BLACK_C);

	m_msgServerPort1.SetFont(clsFunc.m_pFont[1]);
	m_msgServerPort1.SetWindowText(_T("Port"));
	m_msgServerPort1.SetCenterText();
	m_msgServerPort1.SetColor(WHITE_C);
	m_msgServerPort1.SetGradientColor(ORANGE_C);
	m_msgServerPort1.SetTextColor(BLACK_C);

	//kwlee 2017.0302
	m_msgBcrIp_1.SetFont(clsFunc.m_pFont[1]);
	m_msgBcrIp_1.SetWindowText(_T("IP"));
	m_msgBcrIp_1.SetCenterText();
	m_msgBcrIp_1.SetColor(WHITE_C);
	m_msgBcrIp_1.SetGradientColor(ORANGE_C);
	m_msgBcrIp_1.SetTextColor(BLACK_C);

	m_msgBcrPort1.SetFont(clsFunc.m_pFont[1]);
	m_msgBcrPort1.SetWindowText(_T("Port"));
	m_msgBcrPort1.SetCenterText();
	m_msgBcrPort1.SetColor(WHITE_C);
	m_msgBcrPort1.SetGradientColor(ORANGE_C);
	m_msgBcrPort1.SetTextColor(BLACK_C);

	m_msgBcrIp_2.SetFont(clsFunc.m_pFont[1]);
	m_msgBcrIp_2.SetWindowText(_T("IP"));
	m_msgBcrIp_2.SetCenterText();
	m_msgBcrIp_2.SetColor(WHITE_C);
	m_msgBcrIp_2.SetGradientColor(ORANGE_C);
	m_msgBcrIp_2.SetTextColor(BLACK_C);

	m_msgBcrPort2.SetFont(clsFunc.m_pFont[1]);
	m_msgBcrPort2.SetWindowText(_T("Port"));
	m_msgBcrPort2.SetCenterText();
	m_msgBcrPort2.SetColor(WHITE_C);
	m_msgBcrPort2.SetGradientColor(ORANGE_C);
	m_msgBcrPort2.SetTextColor(BLACK_C);
}


void CWorkInterface::OnInitButton()
{
	m_btnSerialApply[0].SetBitmaps(IDC_BTN_SERIAL_APPLY_1, IDB_BITMAP_INTERFACE_APPLY_DN, WINDOW_DN1, IDB_BITMAP_INTERFACE_APPLY_UP, WINDOW_UP1);
	m_btnSerialApply[0].SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnSerialApply[0].SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnSerialApply[0].SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnSerialApply[0].SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnSerialApply[0].SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnSerialApply[0].SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnSerialApply[0].SetFont(clsFunc.m_pFont[1]);
	m_btnSerialApply[0].SetTooltipText(_T("1 Serial Data Apply"));

	m_btnSerialConvet[0].SetBitmaps(IDC_BTN_SERIAL_CONNECT_1, IDB_BITMAP_INTERFACE_CONVERT_DN, WINDOW_DN1, IDB_BITMAP_INTERFACE_CONVERT_UP, WINDOW_UP1);
	m_btnSerialConvet[0].SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnSerialConvet[0].SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnSerialConvet[0].SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnSerialConvet[0].SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnSerialConvet[0].SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnSerialConvet[0].SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnSerialConvet[0].SetFont(clsFunc.m_pFont[1]);
	m_btnSerialConvet[0].SetTooltipText(_T("1 Serial Data Converter"));

	m_btnSerialApply[1].SetBitmaps(IDC_BTN_SERIAL_APPLY_1, IDB_BITMAP_INTERFACE_APPLY_DN, WINDOW_DN1, IDB_BITMAP_INTERFACE_APPLY_UP, WINDOW_UP1);
	m_btnSerialApply[1].SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnSerialApply[1].SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnSerialApply[1].SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnSerialApply[1].SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnSerialApply[1].SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnSerialApply[1].SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnSerialApply[1].SetFont(clsFunc.m_pFont[1]);
	m_btnSerialApply[1].SetTooltipText(_T("1 Serial Data Apply"));

	m_btnSerialConvet[1].SetBitmaps(IDC_BTN_SERIAL_CONNECT_1, IDB_BITMAP_INTERFACE_CONVERT_DN, WINDOW_DN1, IDB_BITMAP_INTERFACE_CONVERT_UP, WINDOW_UP1);
	m_btnSerialConvet[1].SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnSerialConvet[1].SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnSerialConvet[1].SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnSerialConvet[1].SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnSerialConvet[1].SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnSerialConvet[1].SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnSerialConvet[1].SetFont(clsFunc.m_pFont[1]);
	m_btnSerialConvet[1].SetTooltipText(_T("1 Serial Data Converter"));

	m_btnSerialApply[2].SetBitmaps(IDC_BTN_SERIAL_APPLY_1, IDB_BITMAP_INTERFACE_APPLY_DN, WINDOW_DN1, IDB_BITMAP_INTERFACE_APPLY_UP, WINDOW_UP1);
	m_btnSerialApply[2].SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnSerialApply[2].SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnSerialApply[2].SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnSerialApply[2].SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnSerialApply[2].SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnSerialApply[2].SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnSerialApply[2].SetFont(clsFunc.m_pFont[1]);
	m_btnSerialApply[2].SetTooltipText(_T("1 Serial Data Apply"));

	m_btnSerialConvet[2].SetBitmaps(IDC_BTN_SERIAL_CONNECT_1, IDB_BITMAP_INTERFACE_CONVERT_DN, WINDOW_DN1, IDB_BITMAP_INTERFACE_CONVERT_UP, WINDOW_UP1);
	m_btnSerialConvet[2].SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnSerialConvet[2].SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnSerialConvet[2].SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnSerialConvet[2].SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnSerialConvet[2].SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnSerialConvet[2].SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnSerialConvet[2].SetFont(clsFunc.m_pFont[1]);
	m_btnSerialConvet[2].SetTooltipText(_T("3 Serial Data Converter"));

	m_btnClientIpSet[0].SetBitmaps(IDC_BTN_CLIENT_IP_1, IDB_BITMAP_RECIPE1, WINDOW_DN1, IDB_BITMAP_RECIPE1, WINDOW_UP1);
	m_btnClientIpSet[0].SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnClientIpSet[0].SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnClientIpSet[0].SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnClientIpSet[0].SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnClientIpSet[0].SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnClientIpSet[0].SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnClientIpSet[0].SetFont(clsFunc.m_pFont[1]);
	m_btnClientIpSet[0].SetTooltipText(_T("1 Client IP Address"));

	m_btnClientIpSet[1].SetBitmaps(IDC_BTN_CLIENT_IP_2, IDB_BITMAP_RECIPE1, WINDOW_DN1, IDB_BITMAP_RECIPE1, WINDOW_UP1);
	m_btnClientIpSet[1].SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnClientIpSet[1].SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnClientIpSet[1].SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnClientIpSet[1].SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnClientIpSet[1].SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnClientIpSet[1].SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnClientIpSet[1].SetFont(clsFunc.m_pFont[1]);
	m_btnClientIpSet[1].SetTooltipText(_T("2 Client IP Address"));

	m_btnClientIpSet[2].SetBitmaps(IDC_BTN_CLIENT_IP_3, IDB_BITMAP_RECIPE1, WINDOW_DN1, IDB_BITMAP_RECIPE1, WINDOW_UP1);
	m_btnClientIpSet[2].SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnClientIpSet[2].SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnClientIpSet[2].SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnClientIpSet[2].SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnClientIpSet[2].SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnClientIpSet[2].SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnClientIpSet[2].SetFont(clsFunc.m_pFont[1]);
	m_btnClientIpSet[2].SetTooltipText(_T("3 Client IP Address"));

	m_btnClientIpSet[3].SetBitmaps(IDC_BTN_CLIENT_IP_4, IDB_BITMAP_RECIPE1, WINDOW_DN1, IDB_BITMAP_RECIPE1, WINDOW_UP1);
	m_btnClientIpSet[3].SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnClientIpSet[3].SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnClientIpSet[3].SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnClientIpSet[3].SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnClientIpSet[3].SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnClientIpSet[3].SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnClientIpSet[3].SetFont(clsFunc.m_pFont[1]);
	m_btnClientIpSet[3].SetTooltipText(_T("Front Client IP Address"));

	m_btnClientIpSet[4].SetBitmaps(IDC_BTN_CLIENT_IP_5, IDB_BITMAP_RECIPE1, WINDOW_DN1, IDB_BITMAP_RECIPE1, WINDOW_UP1);
	m_btnClientIpSet[4].SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnClientIpSet[4].SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnClientIpSet[4].SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnClientIpSet[4].SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnClientIpSet[4].SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnClientIpSet[4].SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnClientIpSet[4].SetFont(clsFunc.m_pFont[1]);
	m_btnClientIpSet[4].SetTooltipText(_T("Rear Client IP Address"));

	m_btnClientIpSet[5].SetBitmaps(IDC_BTN_CLIENT_IP_6, IDB_BITMAP_RECIPE1, WINDOW_DN1, IDB_BITMAP_RECIPE1, WINDOW_UP1);
	m_btnClientIpSet[5].SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnClientIpSet[5].SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnClientIpSet[5].SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnClientIpSet[5].SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnClientIpSet[5].SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnClientIpSet[5].SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnClientIpSet[5].SetFont(clsFunc.m_pFont[1]);
	m_btnClientIpSet[5].SetTooltipText(_T("FTP Client IP Address"));

	m_btnClientIpSet[6].SetBitmaps(IDC_BTN_CLIENT_IP_7, IDB_BITMAP_RECIPE1, WINDOW_DN1, IDB_BITMAP_RECIPE1, WINDOW_UP1);
	m_btnClientIpSet[6].SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnClientIpSet[6].SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnClientIpSet[6].SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnClientIpSet[6].SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnClientIpSet[6].SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnClientIpSet[6].SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnClientIpSet[6].SetFont(clsFunc.m_pFont[1]);
	m_btnClientIpSet[6].SetTooltipText(_T("GMS Client IP Address"));

	m_btnClientPortSet[0].SetBitmaps(IDC_BTN_CLIENT_PORT_1, IDB_BITMAP_RECIPE1, WINDOW_DN1, IDB_BITMAP_RECIPE1, WINDOW_UP1);
	m_btnClientPortSet[0].SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnClientPortSet[0].SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnClientPortSet[0].SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnClientPortSet[0].SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnClientPortSet[0].SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnClientPortSet[0].SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnClientPortSet[0].SetFont(clsFunc.m_pFont[1]);
	m_btnClientPortSet[0].SetTooltipText(_T("1 Client Port"));

	m_btnClientPortSet[1].SetBitmaps(IDC_BTN_CLIENT_PORT_2, IDB_BITMAP_RECIPE1, WINDOW_DN1, IDB_BITMAP_RECIPE1, WINDOW_UP1);
	m_btnClientPortSet[1].SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnClientPortSet[1].SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnClientPortSet[1].SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnClientPortSet[1].SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnClientPortSet[1].SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnClientPortSet[1].SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnClientPortSet[1].SetFont(clsFunc.m_pFont[1]);
	m_btnClientPortSet[1].SetTooltipText(_T("2 Client Port"));

	m_btnClientPortSet[2].SetBitmaps(IDC_BTN_CLIENT_PORT_3, IDB_BITMAP_RECIPE1, WINDOW_DN1, IDB_BITMAP_RECIPE1, WINDOW_UP1);
	m_btnClientPortSet[2].SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnClientPortSet[2].SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnClientPortSet[2].SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnClientPortSet[2].SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnClientPortSet[2].SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnClientPortSet[2].SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnClientPortSet[2].SetFont(clsFunc.m_pFont[1]);
	m_btnClientPortSet[2].SetTooltipText(_T("3 Client Port"));

	m_btnClientPortSet[3].SetBitmaps(IDC_BTN_CLIENT_PORT_4, IDB_BITMAP_RECIPE1, WINDOW_DN1, IDB_BITMAP_RECIPE1, WINDOW_UP1);
	m_btnClientPortSet[3].SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnClientPortSet[3].SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnClientPortSet[3].SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnClientPortSet[3].SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnClientPortSet[3].SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnClientPortSet[3].SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnClientPortSet[3].SetFont(clsFunc.m_pFont[1]);
	m_btnClientPortSet[3].SetTooltipText(_T("Front Server Port"));

	m_btnClientPortSet[4].SetBitmaps(IDC_BTN_CLIENT_PORT_5, IDB_BITMAP_RECIPE1, WINDOW_DN1, IDB_BITMAP_RECIPE1, WINDOW_UP1);
	m_btnClientPortSet[4].SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnClientPortSet[4].SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnClientPortSet[4].SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnClientPortSet[4].SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnClientPortSet[4].SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnClientPortSet[4].SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnClientPortSet[4].SetFont(clsFunc.m_pFont[1]);
	m_btnClientPortSet[4].SetTooltipText(_T("Rear Client Port"));

	m_btnClientPortSet[5].SetBitmaps(IDC_BTN_CLIENT_PORT_6, IDB_BITMAP_RECIPE1, WINDOW_DN1, IDB_BITMAP_RECIPE1, WINDOW_UP1);
	m_btnClientPortSet[5].SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnClientPortSet[5].SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnClientPortSet[5].SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnClientPortSet[5].SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnClientPortSet[5].SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnClientPortSet[5].SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnClientPortSet[5].SetFont(clsFunc.m_pFont[1]);
	m_btnClientPortSet[5].SetTooltipText(_T("FTP Client Port"));

	m_btnClientPortSet[6].SetBitmaps(IDC_BTN_CLIENT_PORT_7, IDB_BITMAP_RECIPE1, WINDOW_DN1, IDB_BITMAP_RECIPE1, WINDOW_UP1);
	m_btnClientPortSet[6].SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnClientPortSet[6].SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnClientPortSet[6].SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnClientPortSet[6].SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnClientPortSet[6].SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnClientPortSet[6].SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnClientPortSet[6].SetFont(clsFunc.m_pFont[1]);
	m_btnClientPortSet[6].SetTooltipText(_T("GMS Client Port"));

	m_btnServerPortSet[0].SetBitmaps(IDC_BTN_SERVER_PORT_1, IDB_BITMAP_RECIPE1, WINDOW_DN1, IDB_BITMAP_RECIPE1, WINDOW_UP1);
	m_btnServerPortSet[0].SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnServerPortSet[0].SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnServerPortSet[0].SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnServerPortSet[0].SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnServerPortSet[0].SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnServerPortSet[0].SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnServerPortSet[0].SetFont(clsFunc.m_pFont[1]);
	m_btnServerPortSet[0].SetTooltipText(_T("Server Port"));

	//kwlee 2017.0302
	m_btnBcrIP[0].SetBitmaps(IDC_BTN_BCR_1_CLIENT_IP, IDB_BITMAP_RECIPE1, WINDOW_DN1, IDB_BITMAP_RECIPE1, WINDOW_UP1);
	m_btnBcrIP[0].SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnBcrIP[0].SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnBcrIP[0].SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnBcrIP[0].SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnBcrIP[0].SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnBcrIP[0].SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnBcrIP[0].SetFont(clsFunc.m_pFont[1]);
	m_btnBcrIP[0].SetTooltipText(_T("Client IP Address"));

	m_btnBcrPort[0].SetBitmaps(IDC_BTN_BCR_1_CLIENT_PORT, IDB_BITMAP_RECIPE1, WINDOW_DN1, IDB_BITMAP_RECIPE1, WINDOW_UP1);
	m_btnBcrPort[0].SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnBcrPort[0].SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnBcrPort[0].SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnBcrPort[0].SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnBcrPort[0].SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnBcrPort[0].SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnBcrPort[0].SetFont(clsFunc.m_pFont[1]);
	m_btnBcrPort[0].SetTooltipText(_T("Client Port"));

	m_btnBcrIP[1].SetBitmaps(IDC_BTN_BCR_2_CLIENT_IP, IDB_BITMAP_RECIPE1, WINDOW_DN1, IDB_BITMAP_RECIPE1, WINDOW_UP1);
	m_btnBcrIP[1].SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnBcrIP[1].SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnBcrIP[1].SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnBcrIP[1].SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnBcrIP[1].SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnBcrIP[1].SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnBcrIP[1].SetFont(clsFunc.m_pFont[1]);
	m_btnBcrIP[1].SetTooltipText(_T("Client IP Address"));

	m_btnBcrPort[1].SetBitmaps(IDC_BTN_BCR_2_CLIENT_PORT, IDB_BITMAP_RECIPE1, WINDOW_DN1, IDB_BITMAP_RECIPE1, WINDOW_UP1);
	m_btnBcrPort[1].SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnBcrPort[1].SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnBcrPort[1].SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnBcrPort[1].SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnBcrPort[1].SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnBcrPort[1].SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnBcrPort[1].SetFont(clsFunc.m_pFont[1]);
	m_btnBcrPort[1].SetTooltipText(_T("Client Port"));
	////

	m_btnApply.SetBitmaps(IDC_BTN_APPLY, IDB_BITMAP_APPLY_DN1, WINDOW_DN1, IDB_BITMAP_APPLY_UP1, WINDOW_UP1);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnApply.SetFont(clsFunc.m_pFont[2]);
	m_btnApply.SetTooltipText(_T("Interface Data Apply"));

	m_btnReLoad.SetBitmaps(IDC_BTN_RELOAD, IDB_BITMAP_RELOAD_DN1, WINDOW_DN1, IDB_BITMAP_RELOAD_UP1, WINDOW_UP1);
	m_btnReLoad.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnReLoad.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnReLoad.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnReLoad.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnReLoad.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnReLoad.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnReLoad.SetFont(clsFunc.m_pFont[2]);
	m_btnReLoad.SetTooltipText(_T("Interface Data ReLoad"));

	m_btnBcodeOn[0].SetBitmaps(IDC_BTN_BCODE_TRIGGER_ON, IDB_BTN_UPLOAD_DN, WINDOW_DN1, IDB_BTN_UPLOAD_UP, WINDOW_UP1);
	m_btnBcodeOn[0].SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnBcodeOn[0].SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnBcodeOn[0].SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnBcodeOn[0].SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnBcodeOn[0].SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnBcodeOn[0].SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnBcodeOn[0].SetFont(clsFunc.m_pFont[2]);
	m_btnBcodeOn[0].SetTooltipText(_T("Trigger On"));

	m_btnBcodeOn[1].SetBitmaps(IDC_BTN_BCODE_TRIGGER_ON2, IDB_BTN_UPLOAD_DN, WINDOW_DN1, IDB_BTN_UPLOAD_UP, WINDOW_UP1);
	m_btnBcodeOn[1].SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnBcodeOn[1].SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnBcodeOn[1].SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnBcodeOn[1].SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnBcodeOn[1].SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnBcodeOn[1].SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnBcodeOn[1].SetFont(clsFunc.m_pFont[2]);
	m_btnBcodeOn[1].SetTooltipText(_T("Trigger On"));

	m_btnBcodeOn[2].SetBitmaps(IDC_BTN_BCODE_TRIGGER_ON3, IDB_BTN_UPLOAD_DN, WINDOW_DN1, IDB_BTN_UPLOAD_UP, WINDOW_UP1);
	m_btnBcodeOn[2].SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnBcodeOn[2].SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnBcodeOn[2].SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnBcodeOn[2].SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnBcodeOn[2].SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnBcodeOn[2].SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnBcodeOn[2].SetFont(clsFunc.m_pFont[2]);
	m_btnBcodeOn[2].SetTooltipText(_T("Trigger On"));

	m_btnBcodeOff.SetBitmaps(IDC_BTN_BCODE_TRIGGER_OFF, IDB_BTN_DOWNLOAD_DN, WINDOW_DN1, IDB_BTN_DOWNLOAD_UP, WINDOW_UP1);
	m_btnBcodeOff.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnBcodeOff.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnBcodeOff.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnBcodeOff.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnBcodeOff.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnBcodeOff.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnBcodeOff.SetFont(clsFunc.m_pFont[2]);
	m_btnBcodeOff.SetTooltipText(_T("Trigger Off"));

	m_btnUnldBcr[0].SetBitmaps(IDC_CHECK_UNLOAD_BARCODE_A, IDB_BITMAP_USED_UP, WINDOW_DN1, IDB_BITMAP_USED_DN, WINDOW_UP1);
	m_btnUnldBcr[0].SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnUnldBcr[0].SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnUnldBcr[0].SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnUnldBcr[0].SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnUnldBcr[0].SetFont(clsFunc.m_pFont[7]);
	m_btnUnldBcr[0].SetCheck(st_recipe_info.bUnldBcrFlag[0]);

	m_btnUnldBcr[1].SetBitmaps(IDC_CHECK_UNLOAD_BARCODE_B, IDB_BITMAP_USED_UP, WINDOW_DN1, IDB_BITMAP_USED_DN, WINDOW_UP1);
	m_btnUnldBcr[1].SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnUnldBcr[1].SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnUnldBcr[1].SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnUnldBcr[1].SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnUnldBcr[1].SetFont(clsFunc.m_pFont[7]);
	m_btnUnldBcr[1].SetCheck(st_recipe_info.bUnldBcrFlag[1]);

	

	if (st_recipe_info.bUnldBcrFlag[0]) m_btnUnldBcr[0].SetWindowTextW(_T("Unload Barcode A Enable"));
	else m_btnUnldBcr[0].SetWindowTextW(_T("Unload Barcode A Disable"));

	if (st_recipe_info.bUnldBcrFlag[1]) m_btnUnldBcr[1].SetWindowTextW(_T("Unload Barcode B Enable"));
	else m_btnUnldBcr[1].SetWindowTextW(_T("Unload Barcode B Disable"));
}			

void CWorkInterface::OnInterface_Display()
{
	CString str_tmp;
	int i;

	for (i=0; i<3; i++)
	{
		m_cbPort[i].SetCurSel(m_nPort[i][1]);
		m_cbRate[i].SetCurSel(m_nRate[i][1]);
		m_cbData[i].SetCurSel(m_nData[i][1]);
		m_cbParity[i].SetCurSel(m_nParity[i][1]);
		m_cbStop[i].SetCurSel(m_nStop[i][1]);
	}

	OnInterfaceIpAddress(m_strClientIp[0][1]);
	m_clientIp1.SetAddress(m_byIp[0], m_byIp[1], m_byIp[2], m_byIp[3]);
	str_tmp.Format(_T("%d"), m_nClientPort[0][1]);
	m_editClientPort1.SetWindowText(str_tmp);

	OnInterfaceIpAddress(m_strClientIp[1][1]);
	m_clientIp2.SetAddress(m_byIp[0], m_byIp[1], m_byIp[2], m_byIp[3]);
	str_tmp.Format(_T("%d"), m_nClientPort[1][1]);
	m_editClientPort2.SetWindowText(str_tmp);

	OnInterfaceIpAddress(m_strClientIp[2][1]);
	m_clientIp3.SetAddress(m_byIp[0], m_byIp[1], m_byIp[2], m_byIp[3]);
	str_tmp.Format(_T("%d"), m_nClientPort[2][1]);
	m_editClientPort3.SetWindowText(str_tmp);

	OnInterfaceIpAddress(m_strClientIp[3][1]);
	m_clientIp4.SetAddress(m_byIp[0], m_byIp[1], m_byIp[2], m_byIp[3]);
	/*str_tmp.Format(_T("%d"), m_nClientPort[3][1]);
	m_editClientPort4.SetWindowText(str_tmp);*/

	OnInterfaceIpAddress(m_strClientIp[4][1]);
	m_clientIp5.SetAddress(m_byIp[0], m_byIp[1], m_byIp[2], m_byIp[3]);
	str_tmp.Format(_T("%d"), m_nClientPort[4][1]);
	m_editClientPort5.SetWindowText(str_tmp);

	OnInterfaceIpAddress(m_strClientIp[6][1]);
	m_clientIp6.SetAddress(m_byIp[0], m_byIp[1], m_byIp[2], m_byIp[3]);
	str_tmp.Format(_T("%d"), m_nClientPort[6][1]);
	m_editClientPort6.SetWindowText(str_tmp);

	//kwlee 2017.0302
	OnInterfaceIpAddress(m_strClientIp[BCR1_NETWORK][1]);
	m_clientBcrIP1.SetAddress(m_byIp[0], m_byIp[1], m_byIp[2], m_byIp[3]);

	str_tmp.Format(_T("%d"), m_nClientPort[BCR1_NETWORK][1]);
	m_editBcrPort1.SetWindowText(str_tmp);

	OnInterfaceIpAddress(m_strClientIp[BCR2_NETWORK][1]);
	m_clientBcrIP2.SetAddress(m_byIp[0], m_byIp[1], m_byIp[2], m_byIp[3]);

	str_tmp.Format(_T("%d"), m_nClientPort[BCR2_NETWORK][1]);
	m_editBcrPort2.SetWindowText(str_tmp);
	//


	//OnInterfaceIpAddress(m_strClientIp[7][1]);
	//m_clientIp7.SetAddress(m_byIp[0], m_byIp[1], m_byIp[2], m_byIp[3]);
	str_tmp.Format(_T("%d"), m_nServerPort[7][1]);
	m_editClientPort7.SetWindowText(str_tmp);

	str_tmp.Format(_T("%d"), m_nServerPort[2][1]);
	m_editServerPort1.SetWindowText(str_tmp);

	str_tmp.Format(_T("%d"), m_nServerPort[3][1]);
	m_editClientPort4.SetWindowText(str_tmp);
}

void CWorkInterface::OnInterfaceIpAddress(CString str_ip)
{
	int nDot1, nDot2, nDot3, nStart;

	nStart = 0;
	nDot1 = str_ip.Find('.');
	nDot2 = str_ip.Find('.', nDot1 + 1);
	nDot3 = str_ip.Find('.', nDot2 + 1);

	m_byIp[0]	= _wtoi(str_ip.Mid(nStart, nDot1 - nStart));
	nStart		= nDot1 + 1;
	m_byIp[1]	= _wtoi(str_ip.Mid(nStart, nDot2 - nStart));
	nStart		= nDot2 + 1;
	m_byIp[2]	= _wtoi(str_ip.Mid(nStart, nDot3 - nStart));
	nStart		= nDot3 + 1;
	m_byIp[3]	= _wtoi(str_ip.Mid(nStart));
}

void CWorkInterface::OnDataHistoryLog()
{
	CString strMsg;
	int i = 0;

	for(i = 0; i < 10; i++)
	{
		if (m_strClientIp[i][1]			!= m_strClientIp[i][0])
		{
			strMsg.Format(_T("[Interface] %d Client IP Change %s -> %s"), i + 1, m_strClientIp[i][0], m_strClientIp[i][1]);
			clsFunc.OnLogFileAdd(0, strMsg);
		}

		if (m_nClientPort[i][1]			!= m_nClientPort[i][0])
		{
			strMsg.Format(_T("[Interface] %d Client Port Change %d -> %d"), i + 1, m_nClientPort[i][0], m_nClientPort[i][1]);
			clsFunc.OnLogFileAdd(0, strMsg);
		}

		if (m_nServerPort[i][1]			!= m_nServerPort[i][0])
		{
			strMsg.Format(_T("[Interface] %d Server Port Change %d -> %d"), i + 1, m_nServerPort[i][0], m_nServerPort[i][1]);
			clsFunc.OnLogFileAdd(0, strMsg);
		}

		if (m_nPort[i][1]				!= m_nPort[i][0])
		{
			strMsg.Format(_T("[Interface] %d Serial Port Change %d -> %d"), i + 1, m_nPort[i][0], m_nPort[i][1]);
			clsFunc.OnLogFileAdd(0, strMsg);
		}

		if (m_nRate[i][1]				!= m_nRate[i][0])
		{
			strMsg.Format(_T("[Interface] %d Baudrate Change %d -> %d"), i + 1, m_nRate[i][0], m_nRate[i][1]);
			clsFunc.OnLogFileAdd(0, strMsg);
		}

		if (m_nData[i][1]				!= m_nData[i][0])
		{
			strMsg.Format(_T("[Interface] %d Data Bit Change %d -> %d"), i + 1, m_nData[i][0], m_nData[i][1]);
			clsFunc.OnLogFileAdd(0, strMsg);
		}

		if (m_nParity[i][1]				!= m_nParity[i][0])
		{
			strMsg.Format(_T("[Interface] %d Parity Bit Change %d -> %d"), i + 1, m_nParity[i][0], m_nParity[i][1]);
			clsFunc.OnLogFileAdd(0, strMsg);
		}

		if (m_nStop[i][1]				!= m_nStop[i][0])
		{
			strMsg.Format(_T("[Interface] %d Stop Bit Change %d -> %d"), i + 1, m_nStop[i][0], m_nStop[i][1]);
			clsFunc.OnLogFileAdd(0, strMsg);
		}
	}
	if (m_bUnldBcrFlag[0][1]			!= m_bUnldBcrFlag[0][0])
	{
		strMsg.Format(_T("[Interface]  Unload Barcode A Flag Change %d -> %d"), m_bUnldBcrFlag[0][1], m_bUnldBcrFlag[0][0]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}
	if (m_bUnldBcrFlag[1][1]			!= m_bUnldBcrFlag[1][0])
	{
		strMsg.Format(_T("[Interface]  Unload Barcode B Flag Change %d -> %d"), m_bUnldBcrFlag[1][1], m_bUnldBcrFlag[1][0]);
		clsFunc.OnLogFileAdd(0, strMsg);
	}
}


void CWorkInterface::OnDataRecovery()
{
	int i = 0;

	for(i = 0; i < 10; i++)
	{
		m_strClientIp[i][1]			= m_strClientIp[i][0];
		m_nClientPort[i][1]			= m_nClientPort[i][0];

		m_nServerPort[i][1]			= m_nServerPort[i][0];

		m_nPort[i][1]				= m_nPort[i][0];
		m_nRate[i][1]				= m_nRate[i][0];
		m_nData[i][1]				= m_nData[i][0];
		m_nParity[i][1]				= m_nParity[i][0];
		m_nStop[i][1]				= m_nStop[i][0];
	}
	m_bUnldBcrFlag[0][1]			= m_bUnldBcrFlag[0][0];
	m_bUnldBcrFlag[1][1]			= m_bUnldBcrFlag[1][0];
}


int CWorkInterface::OnDataComparison()
{
	int i = 0;

	for(i = 0; i < 10; i++)
	{
		if (m_strClientIp[i][1]			!= m_strClientIp[i][0])
		{
			return RET_ERROR;
		}

		if (m_nClientPort[i][1]			!= m_nClientPort[i][0])
		{
			return RET_ERROR;
		}

		if (m_nServerPort[i][1]			!= m_nServerPort[i][0])
		{
			return RET_ERROR;
		}

		if (m_nPort[i][1]				!= m_nPort[i][0])
		{
			return RET_ERROR;
		}

		if (m_nRate[i][1]				!= m_nRate[i][0])
		{
			return RET_ERROR;
		}

		if (m_nData[i][1]				!= m_nData[i][0])
		{
			return RET_ERROR;
		}

		if (m_nParity[i][1]				!= m_nParity[i][0])
		{
			return RET_ERROR;
		}

		if (m_nStop[i][1]				!= m_nStop[i][0])
		{
			return RET_ERROR;
		}
	}
	if (m_bUnldBcrFlag[0][1]			!= m_bUnldBcrFlag[0][0])	return RET_ERROR;
	if (m_bUnldBcrFlag[1][1]			!= m_bUnldBcrFlag[1][0])	return RET_ERROR;

	return RET_GOOD;
}


void CWorkInterface::OnDataApply()
{
	int i;

	for(i = 0; i < 10; i++)
	{
		st_client_info[i].strIp = m_strClientIp[i][1];
		st_client_info[i].nPort = m_nClientPort[i][1];

		st_server_info[i].nPort = m_nServerPort[i][1];

		st_serial_info.nSerialPort[i]		= ConverterToData(COM_PORT, m_nPort[i][1]);
		st_serial_info.nSerialData[i]		= ConverterToData(COM_DATA, m_nData[i][1]);
		st_serial_info.nSerialStop[i]		= ConverterToData(COM_STOP, m_nStop[i][1]);
		st_serial_info.nSerialBaudrate[i]	= ConverterToData(COM_BAUDRATE, m_nRate[i][1]);
		st_serial_info.nSerialParity[i]		= ConverterToData(COM_PARITY, m_nParity[i][1]);
	}
	st_recipe_info.bUnldBcrFlag[0]	=	m_bUnldBcrFlag[0][1];
	st_recipe_info.bUnldBcrFlag[1]	=	m_bUnldBcrFlag[1][1];
}


void CWorkInterface::OnDataBackup()
{
	int i = 0;

	for(i = 0; i < 10; i++)
	{
		m_strClientIp[i][0]			= m_strClientIp[i][1];
		m_nClientPort[i][0]			= m_nClientPort[i][1];

		m_nServerPort[i][0]			= m_nServerPort[i][1];

		m_nPort[i][0]				= m_nPort[i][1];
		m_nRate[i][0]				= m_nRate[i][1];
		m_nData[i][0]				= m_nData[i][1];
		m_nParity[i][0]				= m_nParity[i][1];
		m_nStop[i][0]				= m_nStop[i][1];
	}
	m_bUnldBcrFlag[0][0]			= m_bUnldBcrFlag[0][1];
	m_bUnldBcrFlag[1][0]			= m_bUnldBcrFlag[1][1];
}


void CWorkInterface::OnDataInit()
{
	int pos;

	for(int i = 0; i < 10; i++)
	{
		m_strClientIp[i][1] = st_client_info[i].strIp;

		m_nClientPort[i][1] = st_client_info[i].nPort;

		m_nServerPort[i][1] = st_server_info[i].nPort;

		pos = st_serial_info.nSerialPort[i];
		m_nPort[i][1] = ConverterToPos(COM_PORT, pos);

		pos = st_serial_info.nSerialBaudrate[i];
		m_nRate[i][1] = ConverterToPos(COM_BAUDRATE, pos);

		pos = st_serial_info.nSerialData[i];
		m_nData[i][1] = ConverterToPos(COM_DATA, pos);

		pos = st_serial_info.nSerialParity[i];
		m_nParity[i][1] = ConverterToPos(COM_PARITY, pos);

		pos = st_serial_info.nSerialStop[i];
		m_nStop[i][1] = ConverterToPos(COM_STOP, pos);
	}
	m_bUnldBcrFlag[0][1]	=	st_recipe_info.bUnldBcrFlag[0];
	m_bUnldBcrFlag[1][1]	=	st_recipe_info.bUnldBcrFlag[1];

	OnDataBackup();
}

BOOL CWorkInterface::OnEraseBkgnd(CDC* pDC)
{
	CDC  memDC;                     // 메모리 DC
	CBitmap * pOldBitmap = NULL;    // m_BackBitmap으로 새로운 그림을 DC에 그릴 때, 이전 그려졌던 DC(즉, Bitmap)을 저장.
	BITMAP bitmapInfo;              // 그림의 정보(m_BackBitmap)
	st_handler_info.m_BackDlgBitmap.GetBitmap(&bitmapInfo); // Bitmap 크기 구함.

	memDC.CreateCompatibleDC(pDC);  // 메모리 DC 생성
	pOldBitmap = memDC.SelectObject(&st_handler_info.m_BackDlgBitmap);
	pDC->BitBlt(0, 0, bitmapInfo.bmWidth, bitmapInfo.bmHeight, &memDC, 0, 0, SRCCOPY);   // 메모리 DC에 그림을 그림

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	return true; // 흰 부러쉬가 아닌 배경을 그려야 하기 때문에 true
}


void CWorkInterface::OnBnClickedBtnSerialApply1()
{
	UpdateData(TRUE);

	m_nPort[0][1]			= m_cbPort[0].GetCurSel();
	m_nRate[0][1]			= m_cbRate[0].GetCurSel();
	m_nData[0][1]			= m_cbData[0].GetCurSel();
	m_nStop[0][1]			= m_cbStop[0].GetCurSel();
	m_nParity[0][1]			= m_cbParity[0].GetCurSel();
}

int CWorkInterface::ConverterToData(int mode, int pos)
{
	int Ret;

	switch(mode)
	{
		case COM_PORT:
			switch(pos)
			{
				case 0:
					Ret = 1;
					break;
				case 1:
					Ret = 2;
					break;
				case 2:
					Ret = 3;
					break;
				case 3:
					Ret = 4;
					break;
				case 4:
					Ret = 5;
					break;
				case 5:
					Ret = 6;
					break;
				case 6:
					Ret = 7;
					break;
				case 7:
					Ret = 8;
					break;
			}
			break;
		case COM_BAUDRATE:
			switch(pos)
			{
				case 0:
					Ret = 2400;
					break;
				case 1:
					Ret = 4800;
					break;
				case 2:
					Ret = 9600;
					break;
				case 3:
					Ret = 14400;
					break;
				case 4:
					Ret = 19200;
					break;
				case 5:
					Ret = 38400;
					break;
				case 6:
					Ret = 57600;
					break;
				case 7:
					Ret = 115200;
					break;
			}
			break;
		case COM_DATA:
			switch(pos)
			{
				case 0:
					Ret = 5;
					break;
				case 1:
					Ret = 6;
					break;
				case 2:
					Ret = 7;
					break;
				case 3:
					Ret = 8;
					break;
				}
				break;
			case COM_STOP:
				switch(pos)
				{
					case 0:
						Ret = ONESTOPBIT;
						break;
					case 1:
						Ret = TWOSTOPBITS;
						break;
				}
				break;
			case COM_PARITY:
				switch(pos)
				{
					case 0:
						Ret = EVENPARITY;  // Even
						break;
					case 1:
						Ret = ODDPARITY;  // Odd
						break;
					case 2:
						Ret = NOPARITY;  // None
						break;
			}
			break;
	}

	return Ret;
}

int CWorkInterface::ConverterToPos(int mode, int data)
{
	int Ret = 0;

	switch(mode)
	{
		case COM_PORT:
			switch(data)
			{
				case 1:
					Ret = 0;
					break;
				case 2:
					Ret = 1;
					break;
				case 3:
					Ret = 2;
					break;
				case 4:
					Ret = 3;
					break;
				case 5:
					Ret = 4;
					break;
				case 6:
					Ret = 5;
					break;
				case 7:
					Ret = 6;
					break;
				case 8:
					Ret = 7;
					break;
			}
			break;
		case COM_BAUDRATE:
			switch(data)
			{
				case 2400:
					Ret = 0;
					break;
				case 4800:
					Ret = 1;
					break;
				case 9600:
					Ret = 2;
					break;
				case 14400:
					Ret = 3;
					break;
				case 19200:
					Ret = 4;
					break;
				case 38400:
					Ret = 5;
					break;
				case 57600:
					Ret = 6;
					break;
				case 115200:
					Ret = 7;
					break;
			}
			break;
		case COM_DATA:
			switch(data)
			{
				case 5:
					Ret = 0;
					break;
				case 6:
					Ret = 1;
					break;
				case 7:
					Ret = 2;
					break;
				case 8:
					Ret = 3;
					break;
			}
			break;
		case COM_STOP:
			switch(data)
			{
				case ONESTOPBIT:
					Ret = 0;
					break;
				case TWOSTOPBITS:
					Ret = 1;
					break;
			}
			break;
		case COM_PARITY:
			switch(data)
			{
				case EVENPARITY:
					Ret = 0;  // Even
					break;
				case ODDPARITY:
					Ret = 1;  // Odd
					break;
				case NOPARITY:
					Ret = 2;  // None
					break;
			}
			break;
	}

	return Ret;
}

void CWorkInterface::OnBnClickedBtnSerialConnect1()
{
	// jtkim 20150626

	st_serial_info.nSerialPort[0]		= ConverterToData(COM_PORT, m_nPort[0][1]);
	st_serial_info.nSerialData[0]		= ConverterToData(COM_DATA, m_nData[0][1]);
	st_serial_info.nSerialStop[0]		= ConverterToData(COM_STOP, m_nStop[0][1]);
	st_serial_info.nSerialBaudrate[0]	= ConverterToData(COM_BAUDRATE, m_nRate[0][1]);
	st_serial_info.nSerialParity[0]		= ConverterToData(COM_PARITY, m_nParity[0][1]);
}


void CWorkInterface::OnBnClickedBtnClientIp1()
{
	CString strKey;

	KeyBoard(&strKey);

	if (strKey == _T("")) return;
	m_strClientIp[0][1] = strKey;
	m_strClientIp[0][1].MakeUpper();
	m_strClientIp[0][1].TrimLeft(' ');               
	m_strClientIp[0][1].TrimRight(' ');

	OnInterfaceIpAddress(m_strClientIp[0][1]);

	m_clientIp1.SetAddress(m_byIp[0], m_byIp[1], m_byIp[2], m_byIp[3]);
}


void CWorkInterface::OnBnClickedBtnClientPort1()
{
	CString strTemp;

	int nKey = m_nClientPort[0][1];

	KeyPadI(1, 100000, &nKey);

	m_nClientPort[0][1] = nKey;
	strTemp.Format(_T("%d"), nKey);

	m_editClientPort1.SetWindowText(strTemp);
}


void CWorkInterface::OnBnClickedBtnClientIp2()
{
	CString strKey;

	KeyBoard(&strKey);

	if (strKey == _T("")) return;

	m_strClientIp[1][1] = strKey;
	m_strClientIp[1][1].MakeUpper();
	m_strClientIp[1][1].TrimLeft(' ');               
	m_strClientIp[1][1].TrimRight(' ');

	OnInterfaceIpAddress(m_strClientIp[1][1]);

	m_clientIp2.SetAddress(m_byIp[0], m_byIp[1], m_byIp[2], m_byIp[3]);
}


void CWorkInterface::OnBnClickedBtnClientPort2()
{
	CString strTemp;

	int nKey = m_nClientPort[1][1];

	KeyPadI(1, 100000, &nKey);

	m_nClientPort[1][1] = nKey;
	strTemp.Format(_T("%d"), nKey);

	m_editClientPort2.SetWindowText(strTemp);
}


void CWorkInterface::OnBnClickedBtnClientIp3()
{
	CString strKey;

	KeyBoard(&strKey);

	if (strKey == _T("")) return;

	m_strClientIp[PRINTER_NETWORK][1] = strKey;

	m_strClientIp[PRINTER_NETWORK][1].MakeUpper();
	m_strClientIp[PRINTER_NETWORK][1].TrimLeft(' ');               
	m_strClientIp[PRINTER_NETWORK][1].TrimRight(' ');

	OnInterfaceIpAddress(m_strClientIp[PRINTER_NETWORK][1]);

	m_clientIp3.SetAddress(m_byIp[0], m_byIp[1], m_byIp[2], m_byIp[3]);
}


void CWorkInterface::OnBnClickedBtnClientPort3()
{
	CString strTemp;

	int nKey = m_nClientPort[PRINTER_NETWORK][1];

	KeyPadI(1, 100000, &nKey);

	m_nClientPort[PRINTER_NETWORK][1] = nKey;
	strTemp.Format(_T("%d"), nKey);

	m_editClientPort3.SetWindowText(strTemp);
}

void CWorkInterface::OnBnClickedBtnClientIp4()
{
	CString strKey;

	KeyBoard(&strKey);

	if (strKey == _T("")) return;

	m_strClientIp[3][1] = strKey;

	m_strClientIp[3][1].MakeUpper();
	m_strClientIp[3][1].TrimLeft(' ');               
	m_strClientIp[3][1].TrimRight(' ');

	OnInterfaceIpAddress(m_strClientIp[3][1]);

	m_clientIp4.SetAddress(m_byIp[0], m_byIp[1], m_byIp[2], m_byIp[3]);
}


void CWorkInterface::OnBnClickedBtnClientPort4()
{
	CString strTemp;

	int nKey = m_nServerPort[3][1];

	KeyPadI(1, 100000, &nKey);

	m_nServerPort[3][1] = nKey;
	strTemp.Format(_T("%d"), nKey);

	m_editClientPort4.SetWindowText(strTemp);
}


void CWorkInterface::OnBnClickedBtnClientIp5()
{
	CString strKey;

	KeyBoard(&strKey);

	if (strKey == _T("")) return;

	m_strClientIp[4][1] = strKey;

	m_strClientIp[4][1].MakeUpper();
	m_strClientIp[4][1].TrimLeft(' ');               
	m_strClientIp[4][1].TrimRight(' ');

	OnInterfaceIpAddress(m_strClientIp[4][1]);

	m_clientIp5.SetAddress(m_byIp[0], m_byIp[1], m_byIp[2], m_byIp[3]);
}


void CWorkInterface::OnBnClickedBtnClientPort5()
{
	CString strTemp;

	int nKey = m_nClientPort[4][1];

	KeyPadI(1, 100000, &nKey);

	m_nClientPort[4][1] = nKey;
	strTemp.Format(_T("%d"), nKey);

	m_editClientPort5.SetWindowText(strTemp);
}

void CWorkInterface::OnBnClickedBtnServerPort1()
{
	CString strTemp;

	int nKey = m_nServerPort[2][1];

	KeyPadI(1, 100000, &nKey);

	m_nServerPort[2][1] = nKey;
	strTemp.Format(_T("%d"), nKey);

	m_editServerPort1.SetWindowText(strTemp);
}

void CWorkInterface::OnBnClickedBtnClientIp6()
{
	CString strKey;

	KeyBoard(&strKey);

	if (strKey == _T("")) return;

	m_strClientIp[6][1] = strKey;

	m_strClientIp[6][1].MakeUpper();
	m_strClientIp[6][1].TrimLeft(' ');               
	m_strClientIp[6][1].TrimRight(' ');

	OnInterfaceIpAddress(m_strClientIp[6][1]);

	m_clientIp6.SetAddress(m_byIp[0], m_byIp[1], m_byIp[2], m_byIp[3]);
}


void CWorkInterface::OnBnClickedBtnClientPort6()
{
	CString strTemp;

	int nKey = m_nClientPort[6][1];

	KeyPadI(1, 100000, &nKey);

	m_nClientPort[6][1] = nKey;
	strTemp.Format(_T("%d"), nKey);

	m_editClientPort6.SetWindowText(strTemp);
}

void CWorkInterface::OnBnClickedBtnClientIp7()
{
	CString strKey;

	KeyBoard(&strKey);

	if (strKey == _T("")) return;

	m_strClientIp[7][1] = strKey;

	m_strClientIp[7][1].MakeUpper();
	m_strClientIp[7][1].TrimLeft(' ');               
	m_strClientIp[7][1].TrimRight(' ');

	OnInterfaceIpAddress(m_strClientIp[7][1]);

	m_clientIp7.SetAddress(m_byIp[0], m_byIp[1], m_byIp[2], m_byIp[3]);
}


void CWorkInterface::OnBnClickedBtnClientPort7()
{
	CString strTemp;

	int nKey = m_nServerPort[7][1];

	KeyPadI(1, 100000, &nKey);

	m_nServerPort[7][1] = nKey;
	strTemp.Format(_T("%d"), nKey);

	m_editClientPort7.SetWindowText(strTemp);
}


void CWorkInterface::OnBnClickedBtnApply()
{
	CDialog_Message		dlgMsg;
	int					nResponse;

	if (OnDataComparison() == RET_ERROR)
	{
		dlgMsg.m_nMessageType	= 1;
		dlgMsg.m_strMessage		= _T("Do you want to change the existing value to the current value?");

		nResponse =  (int)dlgMsg.DoModal();

		if (nResponse == IDOK)
		{
			if (OnDataComparison() == RET_ERROR)
			{
				OnDataHistoryLog();
				OnDataApply();
				OnDataBackup();
			}
		}
	}
}


void CWorkInterface::OnBnClickedBtnReload()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnDataRecovery();
	OnInterface_Display();
}


void CWorkInterface::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (OnDataComparison() == RET_ERROR)
	{
		OnDataHistoryLog();
		OnDataApply();
	}
	CDialog::PostNcDestroy();
}


void CWorkInterface::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (OnDataComparison() == RET_ERROR)
	{
		OnDataHistoryLog();
		OnDataApply();
	}
}


void CWorkInterface::OnClickedBtnBcodeTriggerOn()
{
	if (st_handler_info.nMenuLock) return;

	st_handler_info.nMenuLock = TRUE;

	m_nBarcodePos	= 0;
	m_nBarcodeStep	= 0;

	SetTimer(TM_BARCODE, 100, NULL);
}


void CWorkInterface::OnClickedBtnBcodeTriggerOff()
{
	if (st_handler_info.nMenuLock) return;

	st_handler_info.nMenuLock = TRUE;

	m_nBarcodeStep = 0;
	SetTimer(TM_BARCODE, 100, NULL);
}


int	CWorkInterface::OnBarcodeLoader()
{
	return RET_PROCEED;
}


// int	CWorkInterface::OnBarcodeUnLoaderA()
// {
// 	BYTE byData[100];
// 
// 	int nLength;
// 
// 	CString strTemp;
// 	CString strBarcode;
// 
// 	switch (m_nBarcodeStep)
// 	{
// 	case 0:
// 		if (clsBcrUnLdA.m_bConnect)
// 		{
// 			m_nBarcodeStep = 100;
// 		}
// 		else
// 		{
// 			if (clsBcrUnLdA.OnOpen(st_serial_info.nSerialPort[UNLD_BCR_A_PORT],		st_serial_info.nSerialBaudrate[UNLD_BCR_A_PORT],
// 				st_serial_info.nSerialParity[UNLD_BCR_A_PORT],	st_serial_info.nSerialData[UNLD_BCR_A_PORT],
// 				st_serial_info.nSerialStop[UNLD_BCR_A_PORT],		0x0d))
// 			{
// 				m_nBarcodeStep = 100;
// 			}
// 			else
// 			{
// 				m_nBarcodeStep = 0;
// 				m_listBarcode.AddString(_T("Not Connected"));
// 
// 				return RET_ERROR;
// 			}
// 		}
// 		break;
// 
// 	case 100:
// 		m_dwWaitTime[0] = GetTickCount();
// 
// 		strTemp.Format(_T("LON%02d"), st_recipe_info.nBcrBank);
// 		clsBcrUnLdA.OnDataSend(strTemp);
// 
// 		m_nBarcodeStep = 200;
// 		break;
// 
// 	case 200:
// 		if (clsBcrUnLdA.m_pCom->m_bRevFlag)
// 		{
// 			nLength = clsBcrUnLdA.m_pCom->m_nLength;
// 			if (nLength > 0 )
// 			{
// 				if (nLength > 100)
// 				{
// 					clsBcrUnLdA.m_pCom->Empty();
// 
// 					m_nBarcodeStep = 100;
// 					break;
// 				}
// 				clsBcrUnLdA.m_pCom->ReadData(byData, nLength);
// 				strTemp		= clsFunc.OnCharToString((char*)byData, sizeof(byData));
// 				strBarcode	= strTemp.Mid(0, nLength -1);
// 
// 				m_listBarcode.AddString(strBarcode);
// 
// 				m_nBarcodeStep = 0;
// 
// 				return RET_GOOD;
// 			}
// 			else
// 			{
// 				m_nBarcodeStep = 0;
// 			}
// 		}
// 		else
// 		{
// 			m_dwWaitTime[1] = GetTickCount();
// 			m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];
// 
// 			if (m_dwWaitTime[2] <= 0)
// 			{
// 				m_dwWaitTime[0] = GetTickCount();
// 
// 				break;
// 			}
// 
// 			if (m_dwWaitTime[2] > (DWORD) 5000)
// 			{
// 				m_nBarcodeStep = 0;
// 				m_listBarcode.AddString(_T("Barcode Receive Fail"));
// 
// 				return RET_ERROR;
// 			}
// 		}
// 		break;
// 	}
// 	return RET_PROCEED;
// }
// 
// 
// 	
// int	CWorkInterface::OnBarcodeUnLoaderB()
// {
// 	BYTE byData[100];
// 
// 	int nLength;
// 
// 	CString strTemp;
// 	CString strBarcode;
// 
// 	switch (m_nBarcodeStep)
// 	{
// 	case 0:
// 		if (clsBcrUnLdA.m_bConnect)
// 		{
// 			m_nBarcodeStep = 100;
// 		}
// 		else
// 		{
// 			if (clsBcrUnLdB.OnOpen(st_serial_info.nSerialPort[UNLD_BCR_B_PORT],		st_serial_info.nSerialBaudrate[UNLD_BCR_B_PORT],
// 				st_serial_info.nSerialParity[UNLD_BCR_B_PORT],	st_serial_info.nSerialData[UNLD_BCR_B_PORT],
// 				st_serial_info.nSerialStop[UNLD_BCR_B_PORT],		0x0d))
// 			{
// 				m_nBarcodeStep = 100;
// 			}
// 			else
// 			{
// 				m_nBarcodeStep = 0;
// 				m_listBarcode.AddString(_T("Not Connected"));
// 
// 				return RET_ERROR;
// 			}
// 		}
// 		break;
// 
// 	case 100:
// 		m_dwWaitTime[0] = GetTickCount();
// 
// 		//strTemp.Format(_T("LON%02d"), st_recipe_info.nBcrBank);
// 		strTemp.Format(_T("%cR%c"), 0x02, 0x03);
// 		clsBcrUnLdB.OnDataSend(strTemp);
// 		clsMem.OnNormalMessageWrite(strTemp);
// 		m_nBarcodeStep = 200;
// 		break;
// 
// 	case 200:
// 		if (clsBcrUnLdB.m_pCom->m_bRevFlag)
// 		{
// 			nLength = clsBcrUnLdB.m_pCom->m_nLength;
// 			if (nLength > 0 )
// 			{
// 				if (nLength > 100)
// 				{
// 					clsBcrUnLdB.m_pCom->Empty();
// 
// 					m_nBarcodeStep = 100;
// 					break;
// 				}
// 				clsBcrUnLdB.m_pCom->ReadData(byData, nLength);
// 				strTemp		= clsFunc.OnCharToString((char*)byData, sizeof(byData));
// 				strBarcode	= strTemp.Mid(0, nLength -1);
// 
// 				m_listBarcode.AddString(strBarcode);
// 				m_nBarcodeStep = 0;
// 
// 				return RET_GOOD;
// 			}
// 			else
// 			{
// 				m_nBarcodeStep = 0;
// 			}
// 		}
// 		else
// 		{
// 			m_dwWaitTime[1] = GetTickCount();
// 			m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];
// 
// 			if (m_dwWaitTime[2] <= 0)
// 			{
// 				m_dwWaitTime[0] = GetTickCount();
// 
// 				break;
// 			}
// 
// 			if (m_dwWaitTime[2] > (DWORD) 5000)
// 			{
// 				m_nBarcodeStep = 0;
// 				m_listBarcode.AddString(_T("Barcode Receive Fail"));
// 
// 				return RET_ERROR;
// 			}
// 		}
// 		break;
// 	}
// 
// 	return RET_PROCEED;
// }

int	CWorkInterface::OnRfidRead()
{
/*	switch (m_nRfidStep)
	{
	case 0:
		if (st_client_info[RFID_NETWORK].nConnect == NO)
		{
			// 상위서버와 연결중이면
			m_nRfidStep = 100;
		}
		else
		{
			// 상위서버와 연결이 안된 상태이면
			m_nRfidStep = 300;
		}
		break;

		case 100:
			// 상위 서버에 클라이언트 연결 요청
			m_dwWaitTime[0] = GetTickCount();

			//::PostMessage(st_handler_info.hWnd, WM_CLIENT_MSG + RFID_NETWORK, CLIENT_CONNECT, 0);
			::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + RFID_NETWORK, CLIENT_CONNECT, 0);

			m_nRfidStep = 200;
			break;

		case 200:
			if (st_client_info[RFID_NETWORK].nConnect == YES)
			{
				// 상위서버와 클라이언트가 연결 되었으면 
				m_nRfidStep = 300;
			}
			else
			{
				m_dwWaitTime[1] = GetTickCount();
				m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];

				if (m_dwWaitTime[2] <= 0)
				{
					m_dwWaitTime[0] = GetTickCount();
					break;
				}

				if (m_dwWaitTime[2] > (DWORD)5000)
				{
					// 5초동안 연결되지 않으면 
					m_nRfidStep = 0;

					return RET_ERROR;
				}
			}
			break;

		case 300:
			m_dwWaitTime[0]								= GetTickCount();

			clsRfid.m_nRfidReq							= BIN_NOTUESD;
			st_client_info[RFID_NETWORK].strSend		= clsRfid.OnMakeReadCodeAll(1);
			
			//::PostMessage(st_handler_info.hWnd, WM_CLIENT_MSG + RFID_NETWORK, CLIENT_SEND, 0);
			::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + RFID_NETWORK, CLIENT_SEND, 0);

			m_nRfidStep = 400;
			break;

		case 400:
			// 데이터 송신 결과 확인 작업
			if (clsRfid.m_nRfidReq == BIN_GOODBIN)
			{

				return RET_GOOD;
			}
			else if (clsRfid.m_nRfidReq == BIN_REJECTBIN)
			{
				m_nRfidStep = 0;

				return RET_ERROR;
			}
			else
			{
				m_dwWaitTime[1] = GetTickCount();
				m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];

				if (m_dwWaitTime[2] <= 0)
				{
					m_dwWaitTime[0] = GetTickCount();
					break;
				}

				if (m_dwWaitTime[2] > (DWORD)5000)
				{
					m_nRfidStep = 0;

					return RET_ERROR;
				}
			}
			break;
	}
	*/
	return RET_PROCEED;
}


int	CWorkInterface::OnRfidWrite()
{
/*	switch (m_nRfidStep)
	{
		case 0:
			if (st_client_info[RFID_NETWORK].nConnect == NO)
			{
				// 상위서버와 연결중이면
				m_nRfidStep = 100;
			}
			else
			{
				// 상위서버와 연결이 안된 상태이면
				m_nRfidStep = 300;
			}
			break;

		case 100:
			// 상위 서버에 클라이언트 연결 요청
			m_dwWaitTime[0] = GetTickCount();

			//::PostMessage(st_handler_info.hWnd, WM_CLIENT_MSG + RFID_NETWORK, CLIENT_CONNECT, 0);
			::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + RFID_NETWORK, CLIENT_CONNECT, 0);

			m_nRfidStep = 200;
			break;

		case 200:
			if (st_client_info[RFID_NETWORK].nConnect == YES)
			{
				// 상위서버와 클라이언트가 연결 되었으면 
				m_nRfidStep = 300;
			}
			else
			{
				m_dwWaitTime[1] = GetTickCount();
				m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];

				if (m_dwWaitTime[2] <= 0)
				{
					m_dwWaitTime[0] = GetTickCount();
					break;
				}

				if (m_dwWaitTime[2] > (DWORD)5000)
				{
					// 5초동안 연결되지 않으면 
					m_nRfidStep = 0;

					return RET_ERROR;
				}
			}
			break;

		case 300:
			m_dwWaitTime[0]								= GetTickCount();

			clsRfid.m_nRfidReq							= BIN_NOTUESD;
			st_client_info[RFID_NETWORK].strSend		= clsRfid.OnMakeWriteCodeAll(1, m_strRfidData);
//			st_client_info[RFID_NETWORK].strSend		= clsRfid.OnMakeWriteCode(1, 1, _T("AAAA"));

			//::PostMessage(st_handler_info.hWnd, WM_CLIENT_MSG + RFID_NETWORK, CLIENT_SEND, 0);
			::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + RFID_NETWORK, CLIENT_SEND, 0);

			m_nRfidStep = 400;
			break;

		case 400:
			// 데이터 송신 결과 확인 작업
			if (clsRfid.m_nRfidReq == BIN_GOODBIN)
			{
				m_nRfidStep = 0;

				return RET_GOOD;
			}
			else if (clsRfid.m_nRfidReq == BIN_REJECTBIN)
			{
				m_nRfidStep = 0;

				return RET_ERROR;
			}
			else
			{
				m_dwWaitTime[1] = GetTickCount();
				m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];

				if (m_dwWaitTime[2] <= 0)
				{
					m_dwWaitTime[0] = GetTickCount();
					break;
				}

				if (m_dwWaitTime[2] > (DWORD)10000)
				{
					m_nRfidStep = 0;

					return RET_ERROR;
				}
			}
			break;
	}*/

	return RET_PROCEED;
}

void CWorkInterface::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int nRet;

	switch (nIDEvent)
	{
		case TM_BARCODE:
			switch (m_nBarcodePos)
			{
				case 1:
				//	nRet = OnBarcodeUnLoaderA();
					break;

				case 2:
				//	nRet = OnBarcodeUnLoaderB();
					break;
			}

			if (nRet == RET_GOOD)
			{
				KillTimer(TM_BARCODE);

				if (st_handler_info.nMenuLock != FALSE)
				{
					st_handler_info.nMenuLock = FALSE;
				}
			}
			else if (nRet == RET_ERROR)
			{
				KillTimer(TM_BARCODE);

				if (st_handler_info.nMenuLock != FALSE)
				{
					st_handler_info.nMenuLock = FALSE;
				}
			}
			break;
	}

	CDialog::OnTimer(nIDEvent);
}

void CWorkInterface::OnBnClickedBtnSerialApply2()
{
	UpdateData(TRUE);

	m_nPort[1][1]			= m_cbPort[1].GetCurSel();
	m_nRate[1][1]			= m_cbRate[1].GetCurSel();
	m_nData[1][1]			= m_cbData[1].GetCurSel();
	m_nStop[1][1]			= m_cbStop[1].GetCurSel();
	m_nParity[1][1]			= m_cbParity[1].GetCurSel();
}


void CWorkInterface::OnBnClickedBtnSerialConnect2()
{
//	if (clsBcrUnLdA.m_bConnect) return;

	st_serial_info.nSerialPort[1]		= ConverterToData(COM_PORT, m_nPort[1][1]);
	st_serial_info.nSerialData[1]		= ConverterToData(COM_DATA, m_nData[1][1]);
	st_serial_info.nSerialStop[1]		= ConverterToData(COM_STOP, m_nStop[1][1]);
	st_serial_info.nSerialBaudrate[1]	= ConverterToData(COM_BAUDRATE, m_nRate[1][1]);
	st_serial_info.nSerialParity[1]		= ConverterToData(COM_PARITY, m_nParity[1][1]);

	//clsBcrUnLdA.OnClose();

// 	//clsBcrUnLdA.OnOpen(st_serial_info.nSerialPort[UNLD_BCR_A_PORT],		st_serial_info.nSerialBaudrate[UNLD_BCR_A_PORT],
// 		st_serial_info.nSerialParity[UNLD_BCR_A_PORT],	st_serial_info.nSerialData[UNLD_BCR_A_PORT],
// 		st_serial_info.nSerialStop[UNLD_BCR_A_PORT],		0x0d);
}


void CWorkInterface::OnBnClickedBtnSerialApply3()
{
	UpdateData(TRUE);

	m_nPort[2][1]			= m_cbPort[2].GetCurSel();
	m_nRate[2][1]			= m_cbRate[2].GetCurSel();
	m_nData[2][1]			= m_cbData[2].GetCurSel();
	m_nStop[2][1]			= m_cbStop[2].GetCurSel();
	m_nParity[2][1]			= m_cbParity[2].GetCurSel();
}


void CWorkInterface::OnBnClickedBtnSerialConnect3()
{
	//if (clsBcrUnLdB.m_bConnect) return;

	st_serial_info.nSerialPort[2]		= ConverterToData(COM_PORT, m_nPort[2][1]);
	st_serial_info.nSerialData[2]		= ConverterToData(COM_DATA, m_nData[2][1]);
	st_serial_info.nSerialStop[2]		= ConverterToData(COM_STOP, m_nStop[2][1]);
	st_serial_info.nSerialBaudrate[2]	= ConverterToData(COM_BAUDRATE, m_nRate[2][1]);
	st_serial_info.nSerialParity[2]		= ConverterToData(COM_PARITY, m_nParity[2][1]);

	//clsBcrUnLdB.OnClose();

// 	clsBcrUnLdB.OnOpen(st_serial_info.nSerialPort[UNLD_BCR_B_PORT],		st_serial_info.nSerialBaudrate[UNLD_BCR_B_PORT],
// 		st_serial_info.nSerialParity[UNLD_BCR_B_PORT],	st_serial_info.nSerialData[UNLD_BCR_B_PORT],
// 		st_serial_info.nSerialStop[UNLD_BCR_B_PORT],		0x0d);
}


void CWorkInterface::OnBnClickedBtnBcodeTriggerOn2()
{
	if (st_handler_info.nMenuLock) return;

	st_handler_info.nMenuLock = TRUE;

	m_nBarcodePos	= 1;
	m_nBarcodeStep	= 0;

	SetTimer(TM_BARCODE, 100, NULL);

	
}


void CWorkInterface::OnBnClickedBtnBcodeTriggerOn3()
{
	if (st_handler_info.nMenuLock) return;

	st_handler_info.nMenuLock = TRUE;

	m_nBarcodePos	= 2;
	m_nBarcodeStep	= 0;

	SetTimer(TM_BARCODE, 100, NULL);


}




void CWorkInterface::OnBnClickedCheckUnloadBarcodeA()
{
	CDialog_Message	dlgMsg;

	int nRet;
	
	if (m_btnUnldBcr[0].GetCheck())
	{
		dlgMsg.m_nMessageType	=	1;
		dlgMsg.m_strMessage		=	_T("Do you want a Enable Unload Barcode A?");

		nRet	=	dlgMsg.DoModal();

		if (nRet == IDOK)
		{
			m_bUnldBcrFlag[0][1]	=	TRUE;

			m_btnUnldBcr[0].SetWindowTextW(_T("Unload Barcode A Enable"));
		}
		else
		{
			m_btnUnldBcr[0].SetCheck(m_bUnldBcrFlag[0][0]);

			m_btnUnldBcr[0].SetWindowTextW(_T("Unload Barcode A Disable"));
		}
	}
	else
	{
		dlgMsg.m_nMessageType	=	1;
		dlgMsg.m_strMessage		=	_T("Do you want a Disable Unload Barcode A?");

		nRet	=	dlgMsg.DoModal();

		if (nRet == IDOK)
		{
			m_bUnldBcrFlag[0][1]	=	FALSE;

			m_btnUnldBcr[0].SetWindowTextW(_T("Unload Barcode A Disable"));
		}
		else
		{
			m_btnUnldBcr[0].SetCheck(m_bUnldBcrFlag[0][0]);

			m_btnUnldBcr[0].SetWindowTextW(_T("Unload Barcode A Enable"));
		}
	}
}


void CWorkInterface::OnBnClickedCheckUnloadBarcodeB()
{
	CDialog_Message	dlgMsg;

	int nRet;

	if (m_btnUnldBcr[1].GetCheck())
	{
		dlgMsg.m_nMessageType	=	1;
		dlgMsg.m_strMessage		=	_T("Do you want a Enable Unload Barcode B?");

		nRet	=	dlgMsg.DoModal();

		if (nRet == IDOK)
		{
			m_bUnldBcrFlag[1][1]	=	TRUE;

			m_btnUnldBcr[1].SetWindowTextW(_T("Unload Barcode B Enable"));
		}
		else
		{
			m_btnUnldBcr[1].SetCheck(m_bUnldBcrFlag[1][0]);
		}
	}
	else
	{
		dlgMsg.m_nMessageType	=	1;
		dlgMsg.m_strMessage		=	_T("Do you want a Disable Unload Barcode B?");

		nRet	=	dlgMsg.DoModal();

		if (nRet == IDOK)
		{
			m_bUnldBcrFlag[1][1]	=	FALSE;

			m_btnUnldBcr[1].SetWindowTextW(_T("Unload Barcode B Disable"));
		}
		else
		{
			m_btnUnldBcr[1].SetCheck(m_bUnldBcrFlag[1][0]);
		}
	}
}


void CWorkInterface::OnBnClickedButton6()
{
	CString strFormat;
	CString strTmp1,strTmp2;

	if (st_client_info[PRINTER_NETWORK].nConnect == CTL_YES)
	{
		m_nCntBcrNum++;

		 (clsFunc.m_pZebra[0])->m_nPrintStatusBin = NVR_NONE; 
		// CString strFormat = _T("^XA^LH500,047^FO10,10^BY3^B3N,N,150,Y,N^FD123^FS^FO1100,10^BY3^B3N,N,150,Y,N^FD456^FS^XZ");//2Nm150,Y,N,N^FD456^FS^XZ");
		 //kwlee 2017.0306
		 strTmp1.Format(_T("A%d"),m_nCntBcrNum);
		 strTmp2.Format(_T("B%d"),m_nCntBcrNum);

		// strFormat.Format(_T("^XA^LH500,045^FO10,10^BY3^B3N,N,150,Y,N^FD%s^FS^FO1100,10^BY3^B3N,N,150,Y,N^FD%s^FS^XZ"),strTmp1,strTmp2);
		 //kwlee
		 strFormat.Format(_T("^XA^LH500,045^FO10,10^BY3^B3N,N,150,Y,N^FD%s^FS^FO1400,10^BY3^B3N,N,150,Y,N^FD%s^FS^XZ"),strTmp1,strTmp2);
		

		 //strFormat = _T("^XA^SEE:UHANGUL.DAT^FS^CWJ,E:KFONT3.FNT^FS^CI26^FO110,110,^FD123YBS^FS^FO110,10^FS^FDABCYOON^FS^PQ1^XZ");
		 (clsFunc.m_pZebra[0])->OnPrintOutput(2, PRINTER_NETWORK, 0, 0,  strFormat);
	}	
}


void CWorkInterface::OnBnClickedButtonConnect()
{
	::SendMessage( st_handler_info.hWnd, WM_CLIENT_MSG_3, CLIENT_CONNECT, 0);
}


void CWorkInterface::OnBnClickedButtonRead1()
{
	CString strTemp;

// 	strTemp.Format(_T("%cR%c"), 0x02, 0x03);
// 	st_client_info[EC_FIRST_NETWORK].strSend = strTemp;
	::SendMessage( st_handler_info.hWnd, WM_CLIENT_MSG_8, CLIENT_SEND, 0);	
}


void CWorkInterface::OnBnClickedButtonRead2()
{
	CString strTemp;

// 	strTemp.Format(_T("%cR%c"), 0x02, 0x03);
// 	st_client_info[XGEM_NETWORK].strSend = strTemp;
	::SendMessage( st_handler_info.hWnd, WM_CLIENT_MSG_9, CLIENT_SEND, 0);	
}


void CWorkInterface::OnBnClickedButtonBcr1()
{
	::SendMessage( st_handler_info.hWnd, WM_CLIENT_MSG_8, CLIENT_CONNECT, 0);
}


void CWorkInterface::OnBnClickedButtonBcr2()
{
	::SendMessage( st_handler_info.hWnd, WM_CLIENT_MSG_9, CLIENT_CONNECT, 0);
}


void CWorkInterface::OnBnClickedBtnBcr1ClientIp()
{
	CString strKey;

	KeyBoard(&strKey);

	if (strKey == _T("")) return;

	m_strClientIp[BCR1_NETWORK][1] = strKey;
	m_strClientIp[BCR1_NETWORK][1].MakeUpper();
	m_strClientIp[BCR1_NETWORK][1].TrimLeft(' ');               
	m_strClientIp[BCR1_NETWORK][1].TrimRight(' ');

	OnInterfaceIpAddress(m_strClientIp[BCR1_NETWORK][1]);

	m_clientBcrIP1.SetAddress(m_byIp[0], m_byIp[1], m_byIp[2], m_byIp[3]);
}


void CWorkInterface::OnBnClickedBtnBcr2ClientIp()
{
	CString strKey;

	KeyBoard(&strKey);

	if (strKey == _T("")) return;

	m_strClientIp[BCR2_NETWORK][1] = strKey;
	m_strClientIp[BCR2_NETWORK][1].MakeUpper();
	m_strClientIp[BCR2_NETWORK][1].TrimLeft(' ');               
	m_strClientIp[BCR2_NETWORK][1].TrimRight(' ');

	OnInterfaceIpAddress(m_strClientIp[BCR2_NETWORK][1]);

	m_clientBcrIP2.SetAddress(m_byIp[0], m_byIp[1], m_byIp[2], m_byIp[3]);
}


void CWorkInterface::OnBnClickedBtnBcr1ClientPort()
{
	CString strTemp;

	int nKey = m_nClientPort[BCR1_NETWORK][1];

	KeyPadI(1, 100000, &nKey);

	m_nClientPort[BCR1_NETWORK][1] = nKey;
	strTemp.Format(_T("%d"), nKey);

	m_editBcrPort1.SetWindowText(strTemp);
}


void CWorkInterface::OnBnClickedBtnBcr2ClientPort()
{
	CString strTemp;

	int nKey = m_nClientPort[BCR2_NETWORK][1];

	KeyPadI(1, 100000, &nKey);

	m_nClientPort[BCR2_NETWORK][1] = nKey;
	strTemp.Format(_T("%d"), nKey);

	m_editBcrPort2.SetWindowText(strTemp);
}
