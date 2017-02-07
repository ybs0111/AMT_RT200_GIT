#pragma once

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
#include "Digit.h"

class CScreenIO : public CFormView
{
	DECLARE_DYNCREATE(CScreenIO)
private:

public:
	bool				m_bEmergency;
	bool				m_bIOAction;

	int					m_nIOMaster;				// ���� ���õ� IO ������ ��ȣ
	int					m_nIOPort;					// ���� ���õ� IO ��Ʈ ��ȣ
	int					m_nIOSlave;					// ���� ���õ� IO �����̺� ��ȣ
	int					m_nIOActionCount[2];			// IO ���� ī����
	int					m_nOutputInfo[16];
	int					m_nIOActionStep;
	int					m_nContinueWaitTime;

	long				m_lWaitTime[3];

	CDigit				m_dgtWaitTime;

	CImageList			*m_pImagelist;

	CButtonST			m_btnAction1;
	CButtonST			m_btnAction10;
	CButtonST			m_btnAction100;
	CButtonST			m_btnEmergency;

	CGridCtrl			m_pGridIo;

	int					m_nActionMode;

	CTreeCtrl			m_treeIoList;

	CXPGroupBox			m_groupIoList;
	CXPGroupBox			m_groupAction;

	void				OnDisplayIO(int nMaster, int nPort, int nSlave);
	void				OnInitGroupBox();
	void				OnInitTree();
	void				OnInitGridIo();
	void				OnInitButton();
	void				OnInitRadio();
	void				OnInitDigit();
	void				OnIoInputCheck();
	void				OnIoOutputCheck();
	void				OnIoActionMode(int nMode);
	int					OnIoCylinderExcution();
public:
	CScreenIO();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CScreenIO();

public:
	enum { IDD = IDD_SCREEN_IO };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	virtual BOOL DestroyWindow();
	afx_msg void OnSelchangedTreeLoList(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnRadAction10();
//	afx_msg void OnRadAction100();
	afx_msg void OnCellClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClickedBtnEmergency();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//	afx_msg void OnRadAction1();
	afx_msg void OnClickedCheckAction1();
	afx_msg void OnClickedCheckAction10();
	afx_msg void OnClickedCheckAction100();
	afx_msg void OnClickedDgtWaitTime();
	virtual void PostNcDestroy();
	afx_msg void OnDestroy();
};


