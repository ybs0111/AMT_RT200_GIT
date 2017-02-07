#pragma once


// CDialog_List dialog
#include "Variable.h"		// ���� ���� ���� Ŭ���� �߰� 
#include ".\\Controls\\InitDialogBar.h"		// Initial DialogBar Ŭ����
#include ".\\Controls\\ClockST.h"			// ������ �ð� Ŭ����
#include ".\\Controls\\ColorListBox.h"		// Į�� ����Ʈ �ڽ� Ŭ����

class CDialog_List : public CInitDialogBar
{
	DECLARE_DYNAMIC(CDialog_List)

public:
	CInternetSession	m_pFtp;
	long			m_lNormalWaitTime[3];
	long			m_lAbNormalWaitTime[3];

	CString			m_strOldMsg;

	CClockST		m_listClock;
	CColorListBox	m_listData;

	void			OnInitDigit();
	void			OnListAbnormalMsgDisplay();
	void			OnListNormalMsgDisplay();
	void			OnListMsgClear();
public:
	CDialog_List();   // standard constructor
	virtual ~CDialog_List();

// Dialog Data
	enum { IDD = IDD_DIALOG_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg LRESULT OnListDataUpdate(WPARAM wParam,LPARAM lParam) ;  // ���� �޽��� ����Ʈ �ڽ� ��Ʈ�ѿ� �߰� �޽��� ����
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialogBar();
	afx_msg void OnLbnDblclkListData();
};
