#pragma once


// CDialog_KeyPad dialog
#include "XPGroupBox.h"
#include "GradientStatic.h"
#include "Digit.h"
#include "BtnST.h"

class CDialog_KeyPad : public CDialog
{
	DECLARE_DYNAMIC(CDialog_KeyPad)
public:
	CFont		*m_pFont;
	// ******************************************************************************
	// 키패드 관련 변수 선언                                                         
	// ******************************************************************************
	CString m_strOldVal;	// 기존의 키패드 값을 문자형으로 저장하기 위한 변수 
	CString m_strNewVal;	// 새로 입력된 키패드 값을 문자형으로 저장하기 위한 변수 
	CString m_strKeypadVal;
	CString m_strKeypadHighLimit;
	CString m_strKeypadLowLimit;

	int		m_nKeypadMode;

	CGradientStatic	m_msgKeypadLimit;
	CGradientStatic	m_msgOldValue;
	CGradientStatic	m_msgNewValue;

	CXPGroupBox	m_groupKeypad;
	CButtonST	m_btnPadReset;
	CButtonST	m_btnPadDot;
	CButtonST	m_btnPad1001;
	CButtonST	m_btnPad1000;
	CButtonST	m_btnPad101;
	CButtonST	m_btnPad100;
	CButtonST	m_btnPad11;
	CButtonST	m_btnPad10;
	CButtonST	m_btnPad9;
	CButtonST	m_btnPad8;
	CButtonST	m_btnPad7;
	CButtonST	m_btnPad6;
	CButtonST	m_btnPad5;
	CButtonST	m_btnPad4;
	CButtonST	m_btnPad3;
	CButtonST	m_btnPad2;
	CButtonST	m_btnPad1;
	CButtonST	m_btnPad0;
	CButtonST	m_btnPadDel;
	CButtonST	m_btnPadBack;

	CDigit		m_dgtNewValue;
	CDigit		m_dgtOldValue;

	void			OnInitDigit();
	void			OnInitLabel();
	void			OnInitGroupBox();
public:
	CDialog_KeyPad(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialog_KeyPad();

// Dialog Data
	enum { IDD = IDD_DIALOG_KEYPAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedBtnPad0();
	afx_msg void OnClickedBtnPad1();
	afx_msg void OnClickedBtnPad10();
	afx_msg void OnClickedBtnPad100();
	afx_msg void OnClickedBtnPad1000();
	afx_msg void OnClickedBtnPad1001();
	afx_msg void OnClickedBtnPad101();
	afx_msg void OnClickedBtnPad11();
	afx_msg void OnClickedBtnPad2();
	afx_msg void OnClickedBtnPad3();
	afx_msg void OnClickedBtnPad4();
	afx_msg void OnClickedBtnPad5();
	afx_msg void OnClickedBtnPad6();
	afx_msg void OnClickedBtnPad7();
	afx_msg void OnClickedBtnPad8();
	afx_msg void OnClickedBtnPad9();
	afx_msg void OnClickedBtnPadBack();
	afx_msg void OnClickedBtnPadDel();
	afx_msg void OnClickedBtnPadDot();
	afx_msg void OnClickedBtnPadReset();
	afx_msg void OnPaint();
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedBtnPadSub();
};
