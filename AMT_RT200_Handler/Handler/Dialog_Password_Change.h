#pragma once


// CDialog_Password_Change 대화 상자입니다.
#include "XPGroupBox.h"
#include "GradientStatic.h"
#include "MyBasicData.h"
#include "BtnST.h"


class CDialog_Password_Change : public CDialog
{
	DECLARE_DYNAMIC(CDialog_Password_Change)

public:
	CBitmap						m_btmBk;
	int							m_nPassState;

	CString						m_strOldData;	// 기존 암호 정보 저장 변수 
	CString						m_strNewData;	// 새로 입력된 암호 정보 저장 변수 
	CString						m_strVerifyData; // 확인 암호 정보 저장 변수 

	CGradientStatic				m_msgPassChg;

	CXPGroupBox					m_groupKeyBoard;

	CButtonST					m_btnOld;
	CButtonST					m_btnNew;
	CButtonST					m_btnVerify;
	CButtonST					m_btnBoardSharp;
	CButtonST					m_btnBoardRbraceS;
	CButtonST					m_btnBoardRbraceB;
	CButtonST					m_btnBoardLine;
	CButtonST					m_btnBoardLbraceS;
	CButtonST					m_btnBoardLbraceB;
	CButtonST					m_btnBoardDot;
	CButtonST					m_btnBoardDollar;
	CButtonST					m_btnBoardDiv;
	CButtonST					m_btnBoardDel;
	CButtonST					m_btnBoardBack;
	CButtonST					m_btnBoardAdd;
	CButtonST					m_btnBoardZ;
	CButtonST					m_btnBoardY;
	CButtonST					m_btnBoardX;
	CButtonST					m_btnBoardW;
	CButtonST					m_btnBoardV;
	CButtonST					m_btnBoardU;
	CButtonST					m_btnBoardT;
	CButtonST					m_btnBoardS;
	CButtonST					m_btnBoardR;
	CButtonST					m_btnBoardQ;
	CButtonST					m_btnBoardP;
	CButtonST					m_btnBoardO;
	CButtonST					m_btnBoardN;
	CButtonST					m_btnBoardM;
	CButtonST					m_btnBoardL;
	CButtonST					m_btnBoardK;
	CButtonST					m_btnBoardJ;
	CButtonST					m_btnBoardI;
	CButtonST					m_btnBoardH;
	CButtonST					m_btnBoardG;
	CButtonST					m_btnBoardF;
	CButtonST					m_btnBoardE;
	CButtonST					m_btnBoardD;
	CButtonST					m_btnBoardC;
	CButtonST					m_btnBoardB;
	CButtonST					m_btnBoardA;
	CButtonST					m_btnBoard9;
	CButtonST					m_btnBoard8;
	CButtonST					m_btnBoard7;
	CButtonST					m_btnBoard6;
	CButtonST					m_btnBoard5;
	CButtonST					m_btnBoard4;
	CButtonST					m_btnBoard3;
	CButtonST					m_btnBoard2;
	CButtonST					m_btnBoard1;
	CButtonST					m_btnBoard0;
	CButtonST					m_btnOk;
	CButtonST					m_btnCancel;


	BOOL						OnPassChangeLengthCheck(int nCase);
	void						OnInitLabel();
	void						OnInitGroupBox();
	void						OnInitButton();
	void						OnPassChangeErrorDisplay(CString strText, CString strLevel);
	void						OnPassChangeRadioStateSet(int nState);
	void						OnPassChangePasswordFocus(int nDx);
public:
	CDialog_Password_Change(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialog_Password_Change();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_PASSWORD_CHANGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickedBtnPassChg0();
	afx_msg void OnClickedBtnPassChg1();
	afx_msg void OnClickedBtnPassChg2();
	afx_msg void OnClickedBtnPassChg3();
	afx_msg void OnClickedBtnPassChg4();
	afx_msg void OnClickedBtnPassChg5();
	afx_msg void OnClickedBtnPassChg6();
	afx_msg void OnClickedBtnPassChg7();
	afx_msg void OnClickedBtnPassChg8();
	afx_msg void OnClickedBtnPassChg9();
	afx_msg void OnClickedBtnPassChgA();
	afx_msg void OnClickedBtnPassChgAdd();
	afx_msg void OnClickedBtnPassChgB();
	afx_msg void OnClickedBtnPassChgBack();
	afx_msg void OnClickedBtnPassChgC();
	afx_msg void OnClickedBtnPassChgD();
	afx_msg void OnClickedBtnPassChgDel();
	afx_msg void OnClickedBtnPassChgDiv();
	afx_msg void OnClickedBtnPassChgDollar();
	afx_msg void OnClickedBtnPassChgDot();
	afx_msg void OnClickedBtnPassChgE();
	afx_msg void OnClickedBtnPassChgF();
	afx_msg void OnClickedBtnPassChgG();
	afx_msg void OnClickedBtnPassChgH();
	afx_msg void OnClickedBtnPassChgI();
	afx_msg void OnClickedBtnPassChgJ();
	afx_msg void OnClickedBtnPassChgK();
	afx_msg void OnClickedBtnPassChgL();
	afx_msg void OnClickedBtnPassChgLbraceB();
	afx_msg void OnClickedBtnPassChgLbraceS();
	afx_msg void OnClickedBtnPassChgLine();
	afx_msg void OnClickedBtnPassChgM();
	afx_msg void OnClickedBtnPassChgN();
	afx_msg void OnClickedBtnPassChgO();
	afx_msg void OnClickedBtnPassChgP();
	afx_msg void OnClickedBtnPassChgQ();
	afx_msg void OnClickedBtnPassChgR();
	afx_msg void OnClickedBtnPassChgRbraceB();
	afx_msg void OnClickedBtnPassChgRbraceS();
	afx_msg void OnClickedBtnPassChgS();
	afx_msg void OnClickedBtnPassChgSharp();
	afx_msg void OnClickedBtnPassChgT();
	afx_msg void OnClickedBtnPassChgU();
	afx_msg void OnClickedBtnPassChgW();
	afx_msg void OnClickedBtnPassChgV();
	afx_msg void OnClickedBtnPassChgX();
	afx_msg void OnClickedBtnPassChgY();
	afx_msg void OnClickedBtnPassChgZ();
//	afx_msg void OnClickedRadioPassNew();
//	afx_msg void OnClickedRadioPassOld();
//	afx_msg void OnClickedRadioPassVerify();
	afx_msg void OnBnClickedOk();
	afx_msg void OnClickedCheckNew();
	afx_msg void OnClickedCheckOld();
	afx_msg void OnClickedCheckVerify();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
