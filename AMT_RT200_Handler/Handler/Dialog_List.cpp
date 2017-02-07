// Dialog_List.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Dialog_List.h"
#include "afxdialogex.h"
#include "Variable.h"
#include "PublicFunction.h"
#include "AlgMemory.h"
// CDialog_List dialog

IMPLEMENT_DYNAMIC(CDialog_List, CInitDialogBar)

CDialog_List::CDialog_List()
			: CInitDialogBar()
{

}

CDialog_List::~CDialog_List()
{
}

void CDialog_List::DoDataExchange(CDataExchange* pDX)
{
	CInitDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CLOCK, m_listClock);
	DDX_Control(pDX, IDC_LIST_DATA, m_listData);
}


BEGIN_MESSAGE_MAP(CDialog_List, CInitDialogBar)
	ON_MESSAGE(WM_LIST_DATA, OnListDataUpdate)  // ���� �޽��� ����Ʈ �ڽ� ��Ʈ�ѿ� �߰� �޽��� ����
	ON_LBN_DBLCLK(IDC_LIST_DATA, &CDialog_List::OnLbnDblclkListData)
END_MESSAGE_MAP()


// CDialog_List message handlers


BOOL CDialog_List::OnInitDialogBar()
{
	CInitDialogBar::OnInitDialogBar();

	// TODO:  Add extra initialization here
	m_strOldMsg = _T("Default"); // 20140710 sjs

	OnInitDigit();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_List::OnInitDigit()
{
	m_listClock.Start(IDB_CLOCKST_PANE, IDB_CLOCKST_BIG, IDB_CLOCKST_SMALL);
}

// ******************************************************************************
//  ����Ʈ �ڽ� ��Ʈ�ѿ� ���� �޽��� �߰���Ű�� ����� ���� �Լ�                 
// ******************************************************************************
LRESULT CDialog_List:: OnListDataUpdate(WPARAM wParam,LPARAM lParam)
{
	OnListMsgClear();					// ����Ʈ �ڽ� ��Ʈ�ѿ� �߰��� ��� ������ ���� �Լ�

	switch(lParam)
	{
		case NORMAL_MSG:						// ���� ����
			OnListNormalMsgDisplay();		// ����Ʈ �ڽ� ��Ʈ�ѿ� ���� �޽��� �߰� �Լ�
			break;

		case ABNORMAL_MSG:						// ������ ����
			OnListAbnormalMsgDisplay();		// ����Ʈ �ڽ� ��Ʈ�ѿ� ���� �޽��� �߰� �Լ�
			break;		
	}

	return 0 ;
}

void CDialog_List::OnListNormalMsgDisplay()
{
	int nIndex;	
	CString strMsg = _T("");

//	strMsg.Format(_T("%c"), st_other_info.cNormalMsg);
//	strMsg = clsFunc.OnCharToString(st_other_info.cNormalMsg, sizeof(st_other_info.cNormalMsg));
	strMsg = clsMem.OnNormalMessageRead();

	if (m_strOldMsg == strMsg)
	{
		//2011.0504 �߰� ���� �޼����� ������ ���� ó�� 
		m_lNormalWaitTime[1] = GetCurrentTime();
		m_lNormalWaitTime[2] = m_lNormalWaitTime[1] - m_lNormalWaitTime[0];
		
		if(m_lNormalWaitTime[2] < 0)
		{
			m_lNormalWaitTime[0] = GetTickCount();
			return;
		}
		if (m_lNormalWaitTime[2] > 10000) 
		{
			//OK ���Ϸ� ���� ���� 
		}
	}
	
	if (strMsg != _T(""))
	{
		m_lNormalWaitTime[0]	= GetCurrentTime();
		
		m_strOldMsg				= strMsg;
	}
	else
	{
//		strMsg = _T("����� �޽����� �����ϴ�.");
		return;
	}
	
	nIndex = m_listData.AddString(strMsg, RGB(0, 0, 255));  // ���� �Էµ� ���� ����Ʈ �ڽ� ��Ʈ�ѿ� �߰� 
	m_listData.SetCurSel(nIndex);
	
	clsFunc.OnLogFileAdd(99, strMsg);
}

void CDialog_List::OnListAbnormalMsgDisplay()
{
	int nIndex;	
	CString strMsg = _T("");

//	strMsg.Format(_T("%s"), st_other_info.cAbnormalMsg);
//	strMsg = clsFunc.OnCharToString(st_other_info.cAbnormalMsg, sizeof(st_other_info.cAbnormalMsg));
	strMsg = clsMem.OnAbNormalMessageRead();
	if (m_strOldMsg == strMsg)
	{
		//2011.0504 �߰� ���� �޼����� ������ ���� ó�� 
		m_lAbNormalWaitTime[1] = GetCurrentTime();
		m_lAbNormalWaitTime[2] = m_lAbNormalWaitTime[1] - m_lAbNormalWaitTime[0];
		
		if (m_lAbNormalWaitTime[2] < 0)
		{
			m_lAbNormalWaitTime[0] = GetTickCount();
			return;
		}
		else if (m_lAbNormalWaitTime[2] > 1000) 
		{
			//OK ���Ϸ� ���� ����
		}
	}
	
	if (strMsg != _T(""))
	{
		m_lAbNormalWaitTime[0]	= GetCurrentTime();
		
		m_strOldMsg				= strMsg;
	}
	else
	{
//		strMsg = _T("����� �޽����� �����ϴ�.");
		return;
	}
	
	nIndex = m_listData.AddString(strMsg, RGB(255, 0, 0));  // ���� �Էµ� ���� ����Ʈ �ڽ� ��Ʈ�ѿ� �߰� 
	m_listData.SetCurSel(nIndex);
	
	clsFunc.OnLogFileAdd(99, strMsg);
}

void CDialog_List::OnListMsgClear()
{
	int nIndex = m_listData.GetCount();

	if (nIndex < 100) return;

	m_listData.ResetContent();   // ����Ʈ �ڽ� ��Ʈ�ѿ� �߰��� ��� ������ ���� 
}

void CDialog_List::OnLbnDblclkListData()
{
	CString mstr_cur_year, mstr_cur_month, mstr_cur_day, str_display_time; // ���� ��, ��, �� ���� ���������� ��ȯ�Ͽ� ������ ���� 
	int mn_cur_year, mn_cur_month, mn_cur_day; // ���� ��, ��, �� ���� ���� ���� 
	CString mstr_file_name;		// ���������� ������ ���� �̸� ���� ���� 
	CString mstr_create_file;	// �˶� ���� ������ ���Ͽ� ���� [����]+[���ϸ�]+[Ȯ����] ���� ���� 
	COleDateTime time_cur;		// �˻��� �ð� ���� ���� ���� 

	// **************************************************************************
	// ���� �̸����� ����� ��¥ ������ ��´�                                   
	// **************************************************************************
	time_cur = COleDateTime::GetCurrentTime();  // ���� �ð� ������ ��´�. 

	mn_cur_year = time_cur.GetYear();  
	mn_cur_month = time_cur.GetMonth();  
	mn_cur_day = time_cur.GetDay();  

	// **************************************************************************
	// ��¥ ������ ���������� ��ȯ�Ͽ� ������ �����Ѵ�                           
	// **************************************************************************
	mstr_cur_year.Format(_T("%04d"), mn_cur_year);
	mstr_cur_month.Format(_T("%02d"), mn_cur_month);
	mstr_cur_day.Format(_T("%02d"), mn_cur_day);

	mstr_file_name = _T("TO") + mstr_cur_year;
	mstr_file_name += mstr_cur_month; 
	mstr_file_name += mstr_cur_day; 
	mstr_create_file = st_path_info.strTotal + mstr_file_name;
	mstr_create_file += _T(".TXT");

	::ShellExecute(NULL, NULL, _T("NotePAD.exe"), mstr_create_file, NULL, SW_SHOWNORMAL);// TODO: Add your control notification handler code here
}
