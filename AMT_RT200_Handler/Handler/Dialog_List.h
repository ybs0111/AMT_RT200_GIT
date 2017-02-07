#pragma once


// CDialog_List dialog
#include "Variable.h"		// 전역 변수 정의 클래스 추가 
#include ".\\Controls\\InitDialogBar.h"		// Initial DialogBar 클래스
#include ".\\Controls\\ClockST.h"			// 디지털 시계 클래스
#include ".\\Controls\\ColorListBox.h"		// 칼라 리스트 박스 클래스

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
	afx_msg LRESULT OnListDataUpdate(WPARAM wParam,LPARAM lParam) ;  // 수신 메시지 리스트 박스 컨트롤에 추가 메시지 선언
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialogBar();
	afx_msg void OnLbnDblclkListData();
};
