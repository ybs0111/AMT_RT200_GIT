#pragma once


// CDialog_Title ��ȭ �����Դϴ�.
#include "InitDialogBar.h" // Initial Dialog Bar ���� Ŭ���� �߰�
#include "GradientStatic.h" // �׶��̼� Į�� �ؽ�Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "PictureEx.h"		// GIF ������ �ε��ϱ� ���� Ŭ���� �߰� 
#include "VLabel.h"			// Į�� �ؽ�Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "BtnST.h"

class CDialog_Title : public CInitDialogBar
{
	DECLARE_DYNAMIC(CDialog_Title)

public:
	BOOL			m_bInlineMode;
	int				m_nInlineMode;
	CBitmap			m_btmBk;

	void			OnInitButton();
	void			OnInit_Label();
	void			OnInit_Picture();
	void			OnTitleFormNameChange(int nViewID);
	void			OnTitleStatusChange(int nStatus);
	void			OnTitleDeviceModeDisplay(int nDevice);
	void			OnTitleModelNameDisplay();
	void			OnTitleInterfaceDisplay(int nInterfaceMode);
	void			OnTitleMesDisplay(int nMesMode);
	
	CDialog_Title();   // ǥ�� �������Դϴ�.
	virtual ~CDialog_Title();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_TITLE };

protected:
	virtual		void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	afx_msg		LRESULT OnStatusDataUpdate(WPARAM wParam,LPARAM lParam) ; 
	DECLARE_MESSAGE_MAP()
public:
	CVLabel				m_msgFormName;
	CGradientStatic		m_msgInterfaceModeInfo;
	CGradientStatic		m_msgDeviceModeInfo;
	CGradientStatic		m_msgModelName;
	CGradientStatic		m_msgStatusInfo;
	CGradientStatic		m_msgMesModeInfo;
	CGradientStatic		m_msgOtherInfo;			//Abort, Retest ����

	CPictureEx			m_gifTitleLogo;

	virtual BOOL OnInitDialogBar();
	virtual BOOL DestroyWindow();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//	afx_msg void OnBtnVision();
//	afx_msg void OnYasunagaCalibration();
//	afx_msg void OnYasunagaEnviroment();
//	afx_msg void OnYasunagaInspection();
//	afx_msg void OnYasunagaManual();
//	afx_msg void OnYasunagaModel();
//	afx_msg void OnYasunagaModify();
//	afx_msg void OnYasunagaParameter();
//	afx_msg void OnYasunagaReport();
//	afx_msg void OnYasunagaSelect();
//	afx_msg void OnYasunagaSpec();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnStnClickedMsgMesAbortRetest();
};
