#pragma once


// CDialog_Pass_Check dialog
#include "GradientStatic.h"
#include "XPGroupBox.h"

/* ****************************************************************************** */
/* 암호 정보 로딩 및 저장 클래스 추가                                             */
/* ****************************************************************************** */
#include "MyBasicData.h"           
/* ****************************************************************************** */
#include "PublicFunction.h"
#include "BtnST.h"

class CDialog_Pass_Check : public CDialog
{
	DECLARE_DYNAMIC(CDialog_Pass_Check)

public:
	/* ************************************************************************** */
	/* 암호 정보 로딩/저장 클래스 변수 선언                                       */
	/* ************************************************************************** */
	CMyBasicData mcls_pass_chk;

	CGradientStatic	m_msgTitlePass;
	CGradientStatic	m_msgPass;
	CXPGroupBox	m_groupPass;

	CButtonST	m_btnPassAdd;
	CButtonST	m_btnPassSharp;
	CButtonST	m_btnPassRbraceS;
	CButtonST	m_btnPassRbraceB;
	CButtonST	m_btnPassLine;
	CButtonST	m_btnPassLbraceS;
	CButtonST	m_btnPassLbraceB;
	CButtonST	m_btnPassDot;
	CButtonST	m_btnPassDollar;
	CButtonST	m_btnPassDiv;
	CButtonST	m_btnPassDel;
	CButtonST	m_btnPassBack;
	CButtonST	m_btnPassZ;
	CButtonST	m_btnPassY;
	CButtonST	m_btnPassX;
	CButtonST	m_btnPassW;
	CButtonST	m_btnPassV;
	CButtonST	m_btnPassU;
	CButtonST	m_btnPassT;
	CButtonST	m_btnPassS;
	CButtonST	m_btnPassR;
	CButtonST	m_btnPassQ;
	CButtonST	m_btnPassP;
	CButtonST	m_btnPassO;
	CButtonST	m_btnPassN;
	CButtonST	m_btnPassM;
	CButtonST	m_btnPassL;
	CButtonST	m_btnPassK;
	CButtonST	m_btnPassJ;
	CButtonST	m_btnPassI;
	CButtonST	m_btnPassH;
	CButtonST	m_btnPassG;
	CButtonST	m_btnPassF;
	CButtonST	m_btnPassE;
	CButtonST	m_btnPassD;
	CButtonST	m_btnPassC;
	CButtonST	m_btnPassB;
	CButtonST	m_btnPassA;
	CButtonST	m_btnPass9;
	CButtonST	m_btnPass8;
	CButtonST	m_btnPass7;
	CButtonST	m_btnPass6;
	CButtonST	m_btnPass5;
	CButtonST	m_btnPass4;
	CButtonST	m_btnPass3;
	CButtonST	m_btnPass2;
	CButtonST	m_btnPass1;
	CButtonST	m_btnPass0;
	CButtonST	m_btnOk;
	CButtonST	m_btnCancel;

	int			m_nPassLevel;
	CString		m_strPassword;
	CString		m_strTemp;		// 설정 암호 임시 저장 변수 
	CString		m_strSystemLock;// 시스템 롹 임시 저장 변수
	CString		m_strLevel1;	// 설정 암호 Level 1 임시 저장 변수 
	CString		m_strLevel2;	// 설정 암호 Level 2 임시 저장 변수 
	CString     m_strAdmin;

	/* ************************************************************************** */
	/* 에디트 박스에 입력된 암호 체크 내부 변수 선언                              */
	/* ************************************************************************** */
	CString mstr_pass_chk;		// 에디트 박스에 입력된 암호 정보 저장 변수 
	CString mstr_temp_chk;		// 설정 암호 임시 저장 변수 

	void			OnPassChkPassDataLoad();
	bool			OnPassChkLengthCheck();
	void			OnInitGroupBox();
	void			OnInitLabel();
public:
	CDialog_Pass_Check(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialog_Pass_Check();

// Dialog Data
	enum { IDD = IDD_DIALOG_PASS_CHECK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickedBtnPassChk0();
	afx_msg void OnClickedBtnPassChk1();
	afx_msg void OnClickedBtnPassChk2();
	afx_msg void OnClickedBtnPassChk3();
	afx_msg void OnClickedBtnPassChk4();
	afx_msg void OnClickedBtnPassChk5();
	afx_msg void OnClickedBtnPassChk6();
	afx_msg void OnClickedBtnPassChk7();
	afx_msg void OnClickedBtnPassChk8();
	afx_msg void OnClickedBtnPassChk9();
	afx_msg void OnClickedBtnPassChkA();
	afx_msg void OnClickedBtnPassChkAdd();
	afx_msg void OnClickedBtnPassChkB();
	afx_msg void OnClickedBtnPassChkBack();
	afx_msg void OnClickedBtnPassChkC();
	afx_msg void OnClickedBtnPassChkD();
	afx_msg void OnClickedBtnPassChkDel();
	afx_msg void OnClickedBtnPassChkDiv();
	afx_msg void OnClickedBtnPassChkDollar();
	afx_msg void OnClickedBtnPassChkDot();
	afx_msg void OnClickedBtnPassChkE();
	afx_msg void OnClickedBtnPassChkF();
	afx_msg void OnClickedBtnPassChkG();
	afx_msg void OnClickedBtnPassChkH();
	afx_msg void OnClickedBtnPassChkI();
	afx_msg void OnClickedBtnPassChkJ();
	afx_msg void OnClickedBtnPassChkK();
	afx_msg void OnClickedBtnPassChkL();
	afx_msg void OnClickedBtnPassChkLbraceB();
	afx_msg void OnClickedBtnPassChkLbraceS();
	afx_msg void OnClickedBtnPassChkLine();
	afx_msg void OnClickedBtnPassChkM();
	afx_msg void OnClickedBtnPassChkN();
	afx_msg void OnClickedBtnPassChkO();
	afx_msg void OnClickedBtnPassChkP();
	afx_msg void OnClickedBtnPassChkQ();
	afx_msg void OnClickedBtnPassChkR();
	afx_msg void OnClickedBtnPassChkRbraceB();
	afx_msg void OnClickedBtnPassChkRbraceS();
	afx_msg void OnClickedBtnPassChkS();
	afx_msg void OnClickedBtnPassChkSharp();
	afx_msg void OnClickedBtnPassChkT();
	afx_msg void OnClickedBtnPassChkU();
	afx_msg void OnClickedBtnPassChkV();
	afx_msg void OnClickedBtnPassChkW();
	afx_msg void OnClickedBtnPassChkX();
	afx_msg void OnClickedBtnPassChkY();
	afx_msg void OnClickedBtnPassChkZ();
	afx_msg void OnBnClickedOk();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedCancel();
};
