#pragma once

#include "afxcmn.h"
#include ".\\Controls\\PictureEx.h"  // GIF �ε� Ŭ����
#include "GradientStatic.h"  // �׶��̼� �� Ŭ����
#include ".\\Controls\\SuperProgressCtrl.h"  // ���α׷����� Ŭ����
#include "BtnST.h"
#include "LedButton.h"				// LED ��ư ���� Ŭ���� �߰� 
#include "MyBasicData.h" //kwlee 2015.1116
// CScreenInitial �� ���Դϴ�.

class CDialogMessageView;

#define TMR_INIT	710  // �ʱ�ȭ ���� Ÿ�̸�

class CScreenInitial : public CFormView
{
	DECLARE_DYNCREATE(CScreenInitial)
public:
	CLedButton		m_radPg[21];

	CButtonST		m_btnSkip;
	CBitmap			m_bmGround;
	void			OnInitButton();
public:
	CScreenInitial();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CScreenInitial();

	// ���� ������ ���� ���� �� ���� �÷���
	long			m_nInitStep;

	DWORD			m_dwWaitTime[3];

	int				m_nMcStep;
	int				m_nRetry;
	int				m_nProcessStep;  // ���α׷��� ���� ����
	int				m_nProcessStep1;  // ���α׷��� ���� ����
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
	afx_msg LRESULT		OnMainWorkInfoCommand(WPARAM wParam, LPARAM lParam);  // ȭ�� ������ ���ſ� ����� �޽���
	virtual void PostNcDestroy();
};


