#pragma once


// CDialogAlarmIO ��ȭ �����Դϴ�.
#include ".\\Union\\Grid\\GridCtrl.h"
#include ".\\Union\\Grid\\GridURLCell.h"
#include ".\\Union\\Grid\\GridCellCombo.h"
#include ".\\Union\\Grid\\GridCellCheck.h"
#include ".\\Union\\Grid\\GridCellNumeric.h"
#include ".\\Union\\Grid\\GridCellDateTime.h"
#include "BtnST.h"

class CDialogAlarmIO : public CDialog
{
	DECLARE_DYNAMIC(CDialogAlarmIO)
public:
	BOOL			Create();
	void			OnInputCheck();
	void			OnOutputCheck();
	void			OnInitGridInput();
	void			OnInitGridOutput();
	void			OnInitButton();

	CBitmap			m_btmBk;

	int				m_nInCnt;
	int				m_nOutCnt;

	int				m_nIn[16];
	int				m_nOut[16];

	CString			m_strIn[16];
	CString			m_strOut[16];

	CGridCtrl		m_pGridInput;
	CGridCtrl		m_pGridOutput;

	CButtonST		m_btnExit;
public:
	CDialogAlarmIO(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogAlarmIO();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_ALARM_IO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClickedBtnExit();
	afx_msg void OnCellOutputClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
