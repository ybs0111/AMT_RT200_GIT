#pragma once


// CDialog_KeyBoard dialog
#include "XPGroupBox.h"
#include "GradientStatic.h"
#include "BtnST.h"

class CDialog_KeyBoard : public CDialog
{
	DECLARE_DYNAMIC(CDialog_KeyBoard)

public:
	/* ************************************************************************** */
	/* 타이틀 정보 임시 저장 변수 선언                                            */
	/* ************************************************************************** */
	CString m_strKeyBoardVal;
	/* ************************************************************************** */

	CGradientStatic	m_msgKeyBoard;
	CGradientStatic	m_msgKeyBoardData;
	CXPGroupBox		m_groupKeyBoard;
	CButtonST		m_btnBoardSharp;
	CButtonST		m_btnBoardRbraceS;
	CButtonST		m_btnBoardRbraceB;
	CButtonST		m_btnBoardLine;
	CButtonST		m_btnBoardLbraceS;
	CButtonST		m_btnBoardLbraceB;
	CButtonST		m_btnBoardDot;
	CButtonST		m_btnBoardDollar;
	CButtonST		m_btnBoardDiv;
	CButtonST		m_btnBoardDel;
	CButtonST		m_btnBoardBack;
	CButtonST		m_btnBoardAdd;
	CButtonST		m_btnBoardZ;
	CButtonST		m_btnBoardY;
	CButtonST		m_btnBoardX;
	CButtonST		m_btnBoardW;
	CButtonST		m_btnBoardV;
	CButtonST		m_btnBoardU;
	CButtonST		m_btnBoardT;
	CButtonST		m_btnBoardS;
	CButtonST		m_btnBoardR;
	CButtonST		m_btnBoardQ;
	CButtonST		m_btnBoardP;
	CButtonST		m_btnBoardO;
	CButtonST		m_btnBoardN;
	CButtonST		m_btnBoardM;
	CButtonST		m_btnBoardL;
	CButtonST		m_btnBoardK;
	CButtonST		m_btnBoardJ;
	CButtonST		m_btnBoardI;
	CButtonST		m_btnBoardH;
	CButtonST		m_btnBoardG;
	CButtonST		m_btnBoardF;
	CButtonST		m_btnBoardE;
	CButtonST		m_btnBoardD;
	CButtonST		m_btnBoardC;
	CButtonST		m_btnBoardB;
	CButtonST		m_btnBoardA;
	CButtonST		m_btnBoard9;
	CButtonST		m_btnBoard8;
	CButtonST		m_btnBoard7;
	CButtonST		m_btnBoard6;
	CButtonST		m_btnBoard5;
	CButtonST		m_btnBoard4;
	CButtonST		m_btnBoard3;
	CButtonST		m_btnBoard2;
	CButtonST		m_btnBoard1;
	CButtonST		m_btnBoard0;
	CButtonST		m_btnOk;
	CButtonST		m_btnCancel;

	void			OnInitGroupBox();
	void			OnInitLabel();
	bool			OnKeyboardLengthCheck(CString strData);
public:
	CDialog_KeyBoard(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialog_KeyBoard();

// Dialog Data
	enum { IDD = IDD_DIALOG_KEYBOARD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnBtnApply();
//	afx_msg void OnBtnApply();
	afx_msg void OnClickedBtnBoard0();
	afx_msg void OnClickedBtnBoard1();
	afx_msg void OnClickedBtnBoard2();
	afx_msg void OnClickedBtnBoard3();
	afx_msg void OnClickedBtnBoard4();
	afx_msg void OnClickedBtnBoard5();
	afx_msg void OnClickedBtnBoard6();
	afx_msg void OnClickedBtnBoard7();
	afx_msg void OnClickedBtnBoard8();
	afx_msg void OnClickedBtnBoard9();
	afx_msg void OnClickedBtnBoardA();
	afx_msg void OnClickedBtnBoardAdd();
	afx_msg void OnClickedBtnBoardB();
	afx_msg void OnClickedBtnBoardBack();
	afx_msg void OnClickedBtnBoardC();
	afx_msg void OnClickedBtnBoardD();
	afx_msg void OnClickedBtnBoardDel();
	afx_msg void OnClickedBtnBoardDiv();
	afx_msg void OnClickedBtnBoardDollar();
	afx_msg void OnClickedBtnBoardDot();
	afx_msg void OnClickedBtnBoardE();
	afx_msg void OnClickedBtnBoardF();
	afx_msg void OnClickedBtnBoardG();
	afx_msg void OnClickedBtnBoardH();
	afx_msg void OnClickedBtnBoardI();
	afx_msg void OnClickedBtnBoardJ();
	afx_msg void OnClickedBtnBoardK();
	afx_msg void OnClickedBtnBoardL();
	afx_msg void OnClickedBtnBoardLbraceB();
	afx_msg void OnClickedBtnBoardLbraceS();
	afx_msg void OnClickedBtnBoardLine();
	afx_msg void OnClickedBtnBoardM();
	afx_msg void OnClickedBtnBoardN();
	afx_msg void OnClickedBtnBoardO();
	afx_msg void OnClickedBtnBoardP();
	afx_msg void OnClickedBtnBoardQ();
	afx_msg void OnClickedBtnBoardR();
	afx_msg void OnClickedBtnBoardRbraceB();
	afx_msg void OnClickedBtnBoardRbraceS();
	afx_msg void OnClickedBtnBoardS();
	afx_msg void OnClickedBtnBoardSharp();
	afx_msg void OnClickedBtnBoardT();
	afx_msg void OnClickedBtnBoardU();
	afx_msg void OnClickedBtnBoardV();
	afx_msg void OnClickedBtnBoardW();
	afx_msg void OnClickedBtnBoardX();
	afx_msg void OnClickedBtnBoardY();
	afx_msg void OnClickedBtnBoardZ();
//	afx_msg void OnBtnCancel();
//	afx_msg void OnBtnApply();
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedOk();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedCancel();
};
