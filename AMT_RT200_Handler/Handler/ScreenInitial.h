#pragma once

#include "afxcmn.h"
#include ".\\Controls\\PictureEx.h"  // GIF 로딩 클래스
#include "GradientStatic.h"  // 그라데이션 라벨 클래스
#include ".\\Controls\\SuperProgressCtrl.h"  // 프로그레스바 클래스
#include "BtnST.h"
#include "LedButton.h"				// LED 버튼 생성 클래스 추가 
#include "MyBasicData.h" //kwlee 2015.1116
// CScreenInitial 폼 뷰입니다.

class CDialogMessageView;

#define TMR_INIT	710  // 초기화 진행 타이머

class CScreenInitial : public CFormView
{
	DECLARE_DYNCREATE(CScreenInitial)
public:
	CLedButton		m_radPg[21];

	CButtonST		m_btnSkip;
	CBitmap			m_bmGround;
	void			OnInitButton();
public:
	CScreenInitial();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CScreenInitial();

	// 스텝 데이터 저장 변수 및 리턴 플래그
	long			m_nInitStep;

	DWORD			m_dwWaitTime[3];

	int				m_nMcStep;
	int				m_nRetry;
	int				m_nProcessStep;  // 프로그레스 진행 상태
	int				m_nProcessStep1;  // 프로그레스 진행 상태
	bool            mb_MsgCheck;
	CDialogMessageView		*m_pMsgView;

	void			OnMessageView();
	int				OnMcEnable();
	void			OnRecoveryInit();
	void			OnReadyInit();
	void			OnInitProgress();
	void			OnInitLabel();
	void			OnInitRadio();
	int				OnInitExcution();
	void			OnInitChangeStatus(int nNum);
public:
	enum { IDD = IDD_SCREEN_INITIAL };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	CSuperProgressCtrl	m_ctrlProgress;
	CSuperProgressCtrl	m_ctrlProgress1;

public:
	CPictureEx			m_pgifInitializing;
	CGradientStatic		m_msgInitial;

	virtual void		OnInitialUpdate();
	afx_msg void		OnDestroy();
	afx_msg void		OnTimer(UINT_PTR nIDEvent);
	virtual BOOL		DestroyWindow();
	afx_msg void		OnClickedBtnSkip();
	afx_msg BOOL		OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT		OnMainWorkInfoCommand(WPARAM wParam, LPARAM lParam);  // 화면 데이터 갱신용 사용자 메시지
	virtual void PostNcDestroy();
};


