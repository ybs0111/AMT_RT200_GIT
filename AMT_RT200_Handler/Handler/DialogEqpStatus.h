#pragma once

#include ".\\Union\\Grid\\GridCtrl.h"
#include ".\\Union\\Grid\\GridURLCell.h"
#include ".\\Union\\Grid\\GridCellCombo.h"
#include ".\\Union\\Grid\\GridCellCheck.h"
#include ".\\Union\\Grid\\GridCellNumeric.h"
#include ".\\Union\\Grid\\GridCellDateTime.h"
#include "GradientStatic.h" 
#include "XPGroupBox.h"
#include "BtnSt.h"
// CDialogEqpStatus 대화 상자입니다.
class CDialogEqpStatus : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogEqpStatus)

public:
	CDialogEqpStatus(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogEqpStatus();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_EQP_STATUS };

public:
	int			OnMagazineIn();
	int			OnMagazineOut();
	int			OnMcEnable();
	int			OnMcDisable();
	void		OnInitDB();
	void		OnInitControls();
	void		OnInitGrid(int nCount);
	void		OnDisplayList();
	void		OnInitStatic();
	void		OnInitEqpFile();//2015.02.09


public:
	CString			m_strModelName;

	DWORD			m_dwWaitTime[3];
	int				m_nRetry;
	int				m_nMcEnableStep;
	int				m_nMcDisableStep;
	int				m_nMgzStep;

	bool			m_bEnableFlag;

	CBrush			m_brushEnable;
	CBrush			m_brushDisable;

	//	CBitmapButton	m_btnOK;
	//	CBitmapButton	m_btnCancel;

	//	CButtonST		m_btnAdd;
	//	CButtonST		m_btnDel;

	CButtonST		m_btnExit;
	CButtonST		m_btnMgzIn;
	CButtonST		m_btnMgzOut;

	CStatic			m_stnEnable;
	CStatic			m_stnDisable;

	CString			m_strToolName;
	CString			m_strPartNo;
	CString			m_strComment;
	CString			m_strCode;

	CXPGroupBox		m_groupTool;

	CGridCtrl		m_pGridPcbModel;

	CGradientStatic	m_staticAble;
	CGradientStatic m_staticComment;
	CGradientStatic	m_staticCode;

	CStatic			m_ctrlComment;
	CStatic			m_ctrlCode;

	int				m_nDbRowCnt;
	int				m_nCurRow;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedStaticEnable();
	afx_msg void OnStnClickedStaticDisable();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL DestroyWindow();
	afx_msg void OnToolListClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnStnClickedTextComment();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnStnClickedTextCode();
	afx_msg void OnBnClickedBtnDialogEqpExit();
	afx_msg void OnBnClickedBtnDialogEqpMagazineIn();
	afx_msg void OnBnClickedBtnDialogEqpMagazineOut();
	
};

