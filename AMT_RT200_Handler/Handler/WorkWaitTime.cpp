// WorkWaitTime.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Handler.h"
#include "WorkWaitTime.h"
#include "afxdialogex.h"
#include "Variable.h"
#include "Alg_Excel.h"
#include "PublicFunction.h"
#include "Dialog_KeyPad.h"
#include "Dialog_Message.h"
#include ".\\Ctrl\\KeyBoard.h"

// CWorkWaitTime ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CWorkWaitTime, CDialog)

CWorkWaitTime::CWorkWaitTime(CWnd* pParent /*=NULL*/)
	: CDialog(CWorkWaitTime::IDD, pParent)
{

}

CWorkWaitTime::~CWorkWaitTime()
{
}

void CWorkWaitTime::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM_WAITTIME,		m_pGridWaitTime);
	DDX_Control(pDX, IDC_GROUP_COMMAND,			m_groupCommand);
	DDX_Control(pDX, IDC_GROUP_WAITTIME,		m_groupWaitTime);
	DDX_Control(pDX, IDC_BTN_APPLY,				m_btnApply);
	DDX_Control(pDX, IDC_BTN_RELOAD,			m_btnReLoad);
}


BEGIN_MESSAGE_MAP(CWorkWaitTime, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_CUSTOM_WAITTIME,	OnCellClick)
	ON_BN_CLICKED(IDC_BTN_APPLY,				&CWorkWaitTime::OnClickedBtnApply)
	ON_BN_CLICKED(IDC_BTN_RELOAD,				&CWorkWaitTime::OnClickedBtnReload)
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CWorkWaitTime �޽��� ó�����Դϴ�.


BOOL CWorkWaitTime::DestroyWindow()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (OnDataComparison() == RET_ERROR)
	{
		OnDataHistoryLog();
		OnDataApply();
	}

	return CDialog::DestroyWindow();
}


BOOL CWorkWaitTime::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	OnDataInit();

	OnInitGroupBox();
	OnInitLabel();
	OnInitButton();
	OnInitGridWaitTime();
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CWorkWaitTime::OnInitGroupBox()
{
	m_groupCommand.SetFont(clsFunc.OnLogFont(16));
	m_groupCommand.SetCatptionTextColor(BLUE_C);
	m_groupCommand.SetBorderColor(ORANGE_C);
	m_groupCommand.SetBackgroundColor(WINDOW_UP1);
	m_groupCommand.SetFontBold(TRUE);

	m_groupWaitTime.SetFont(clsFunc.OnLogFont(16));
	m_groupWaitTime.SetCatptionTextColor(BLUE_C);
	m_groupWaitTime.SetBorderColor(ORANGE_C);
	m_groupWaitTime.SetBackgroundColor(WINDOW_UP1);
	m_groupWaitTime.SetFontBold(TRUE);
}


void CWorkWaitTime::OnInitLabel()
{
}


void CWorkWaitTime::OnInitButton()
{
	m_btnApply.SetBitmaps(IDC_BTN_APPLY, IDB_BITMAP_APPLY_DN1, WINDOW_DN1, IDB_BITMAP_APPLY_UP1, WINDOW_UP1);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnApply.SetFont(clsFunc.m_pFont[2]);
	m_btnApply.SetTooltipText(_T("Wait Time Data Apply"));

	m_btnReLoad.SetBitmaps(IDC_BTN_RELOAD, IDB_BITMAP_RELOAD_DN1, WINDOW_DN1, IDB_BITMAP_RELOAD_UP1, WINDOW_UP1);
	m_btnReLoad.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN1);
	m_btnReLoad.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP1);
	m_btnReLoad.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP1);
	m_btnReLoad.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnReLoad.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnReLoad.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnReLoad.SetFont(clsFunc.m_pFont[2]);
	m_btnReLoad.SetTooltipText(_T("Wait Time Data ReLoad"));
}


void CWorkWaitTime::OnDataHistoryLog()
{
	int i;
	CString strMsg;

	for(i=0; i<m_nWaitTimeMaxCnt; i++)
	{
		if(m_nOnWaitTime[i][0]		!= m_nOnWaitTime[i][1])
		{
			strMsg.Format(_T("[%s] On WaitTime Change %d -> %d"), m_strWaitTimeLabel[i],
																  m_nOnWaitTime[i][0], m_nOnWaitTime[i][1]);
			clsFunc.OnLogFileAdd(0, strMsg);
		}

		if(m_nOffWaitTime[i][0]		!= m_nOffWaitTime[i][1])
		{
			strMsg.Format(_T("[%s] Off WaitTime Change %d -> %d"), m_strWaitTimeLabel[i],
																  m_nOffWaitTime[i][0], m_nOffWaitTime[i][1]);
			clsFunc.OnLogFileAdd(0, strMsg);
		}

		if(m_nLimitWaitTime[i][0]	!= m_nLimitWaitTime[i][1])
		{
			strMsg.Format(_T("[%s] Limit WaitTime Change %d -> %d"), m_strWaitTimeLabel[i],
																  m_nLimitWaitTime[i][0], m_nLimitWaitTime[i][1]);
			clsFunc.OnLogFileAdd(0, strMsg);
		}
	}
}


void CWorkWaitTime::OnDataRecovery()
{
	int i;

	for(i=0; i<m_nWaitTimeMaxCnt; i++)
	{
		m_nOnWaitTime[i][1]		= m_nOnWaitTime[i][0];
		m_nOffWaitTime[i][1]	= m_nOffWaitTime[i][0];
		m_nLimitWaitTime[i][1]	= m_nLimitWaitTime[i][0];
	}
}


int	CWorkWaitTime::OnDataComparison()
{
	int i;

	for(i=0; i<m_nWaitTimeMaxCnt; i++)
	{
		if(m_nOnWaitTime[i][0]		!= m_nOnWaitTime[i][1])		return RET_ERROR;
		if(m_nOffWaitTime[i][0]		!= m_nOffWaitTime[i][1])	return RET_ERROR;
		if(m_nLimitWaitTime[i][0]	!= m_nLimitWaitTime[i][1])  return RET_ERROR;
	}

	return RET_GOOD;
}


void CWorkWaitTime::OnDataApply()
{
	int i;

	for(i=0; i<m_nWaitTimeMaxCnt; i++)
	{
		st_wait_info.nOnWaitTime[i]		= m_nOnWaitTime[i][1];
		st_wait_info.nOffWaitTime[i]	= m_nOffWaitTime[i][1];
		st_wait_info.nLimitWaitTime[i]	= m_nLimitWaitTime[i][1];
	}
}


void CWorkWaitTime::OnDataBackup()
{
	int i;

	for(i=0; i<m_nWaitTimeMaxCnt; i++)
	{
		m_nOnWaitTime[i][0]		= m_nOnWaitTime[i][1];
		m_nOffWaitTime[i][0]	= m_nOffWaitTime[i][1];
		m_nLimitWaitTime[i][0]	= m_nLimitWaitTime[i][1];
	}
}


void CWorkWaitTime::OnDataInit()
{
	int i;

	m_nWaitTimeMaxCnt		= clsExcel.m_nWaitTimeMaxCnt;

	for(i=0; i<m_nWaitTimeMaxCnt; i++)
	{
		m_strWaitTimeLabel[i]	= clsExcel.m_strWaitTimeLabel[i];

		m_nOnWaitTime[i][1]		= st_wait_info.nOnWaitTime[i];
		m_nOffWaitTime[i][1]	= st_wait_info.nOffWaitTime[i];
		m_nLimitWaitTime[i][1]	= st_wait_info.nLimitWaitTime[i];
	}
	OnDataBackup();
}



void CWorkWaitTime::OnClickedBtnApply()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CDialog_Message  dlgMsg;
	int nResponse;

	if (OnDataComparison() == RET_ERROR)
	{
		dlgMsg.m_nMessageType	= 1;
		dlgMsg.m_strMessage		= _T("Do you want to change the existing value to the current value?");

		nResponse =  (int)dlgMsg.DoModal();

		if (nResponse == IDOK)
		{
			if (OnDataComparison() == RET_ERROR)
			{
				OnDataHistoryLog();
				OnDataApply();
				OnDataBackup();
			}
		}
	}
}


void CWorkWaitTime::OnClickedBtnReload()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnDataRecovery();
	if(OnDataComparison() == RET_ERROR)
	{
		OnInitGridWaitTime();
	}
}

void CWorkWaitTime::OnInitGridWaitTime()
{
	CString strHeader[4] = {_T("WaitTime Name"), _T("On(ms)"), _T("Off(ms)"), _T("Limit(ms)")};
	CString strTmp;
	int i, j;
	
	m_pGridWaitTime.SetFrameFocusCell(FALSE);
	m_pGridWaitTime.SetTrackFocusCell(FALSE);
	m_pGridWaitTime.EnableSelection(FALSE);

	m_pGridWaitTime.SetGridLineColor(BLACK_C);
	m_pGridWaitTime.SetGridLines(1);

	m_pGridWaitTime.SetRowCount(m_nWaitTimeMaxCnt + 1);
	m_pGridWaitTime.SetColumnCount(4);

	m_pGridWaitTime.SetFixedRowCount(0);
	m_pGridWaitTime.SetFixedColumnCount(0);
	m_pGridWaitTime.SetFixedBkColor(RGB(0,0,200));
	m_pGridWaitTime.SetFixedBkColor(RGB(200,200,255));
	m_pGridWaitTime.SetTextBkColor(RGB(150,150,200));

	for (i=0; i<m_nWaitTimeMaxCnt+1; i++) 
	{
		m_pGridWaitTime.SetRowHeight(i, 41);

		for (j=0; j<4; j++) 
		{
			switch (j)
			{
				case 0:
					if (m_nWaitTimeMaxCnt > 13)
					{
						m_pGridWaitTime.SetColumnWidth(j, 290);
					}
					else
					{
						m_pGridWaitTime.SetColumnWidth(j, 305);
					}
					break;

				case 1:
					m_pGridWaitTime.SetColumnWidth(j, 190);
					break;

				case 2:
					m_pGridWaitTime.SetColumnWidth(j, 190);
					break;

				case 3:
					m_pGridWaitTime.SetColumnWidth(j, 190);
					break;
			}

			m_pGridWaitTime.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_pGridWaitTime.SetItemState(i, j, GVIS_READONLY);
			m_pGridWaitTime.SetItemBkColour(i, j, WHITE_C, CLR_DEFAULT);
		}
	}

	for (i=0; i<m_nWaitTimeMaxCnt+1; i++) 
	{
		for (j=0; j<4; j++) 
		{
			if(i == 0)
			{
				m_pGridWaitTime.SetItemBkColour(i, j, RGB(219, 229, 241), BLACK_L);
				m_pGridWaitTime.SetItemFont(i, j, &clsFunc.OnLogFont(17));
				m_pGridWaitTime.SetItemText(i, j,strHeader[j]);
			}
			else
			{
				switch (j)
				{
					case 0:
						m_pGridWaitTime.SetItemBkColour(i, j, RGB(219, 229, 241), BLACK_L);
						m_pGridWaitTime.SetItemFont(i, j, &clsFunc.OnLogFont(17));
						m_pGridWaitTime.SetItemText(i, j, m_strWaitTimeLabel[i - 1]);
						break;

					case 1:
						if(clsExcel.m_nOnUsed[i -1])
						{
							m_pGridWaitTime.SetItemBkColour(i, j, GREEN_L, BLACK_C);
							m_pGridWaitTime.SetItemFont(i, j, &clsFunc.OnLogFont(17));
							strTmp.Format(_T("%d"), m_nOnWaitTime[i - 1][1]);
							m_pGridWaitTime.SetItemText(i, j, strTmp);
						}
						else
						{
							m_pGridWaitTime.SetItemBkColour(i, j, DSERBLE_BC, BLACK_C);
							m_pGridWaitTime.SetItemFont(i, j, &clsFunc.OnLogFont(25));
							m_pGridWaitTime.SetItemText(i, j, _T("--"));
						}
						break;

					case 2:
						if(clsExcel.m_nOffUsed[i -1])
						{
							m_pGridWaitTime.SetItemBkColour(i, j, RED_L, BLACK_C);
							m_pGridWaitTime.SetItemFont(i, j, &clsFunc.OnLogFont(17));
							strTmp.Format(_T("%d"), m_nOffWaitTime[i - 1][1]);
							m_pGridWaitTime.SetItemText(i, j, strTmp);
						}
						else
						{
							m_pGridWaitTime.SetItemBkColour(i, j, DSERBLE_BC, BLACK_C);
							m_pGridWaitTime.SetItemFont(i, j, &clsFunc.OnLogFont(25));
							m_pGridWaitTime.SetItemText(i, j, _T("--"));
						}
						break;

					case 3:
						if(clsExcel.m_nLimitUsed[i -1])
						{
							m_pGridWaitTime.SetItemBkColour(i, j, YELLOW_L, BLACK_C);
							m_pGridWaitTime.SetItemFont(i, j, &clsFunc.OnLogFont(17));
							strTmp.Format(_T("%d"), m_nLimitWaitTime[i - 1][1]);
							m_pGridWaitTime.SetItemText(i, j, strTmp);
						}
						else
						{
							m_pGridWaitTime.SetItemBkColour(i, j, DSERBLE_BC, BLACK_C);
							m_pGridWaitTime.SetItemFont(i, j, &clsFunc.OnLogFont(25));
							m_pGridWaitTime.SetItemText(i, j, _T("--"));
						}
						break;
				}
			}
		}
	}

	m_pGridWaitTime.Invalidate(FALSE);
}

void CWorkWaitTime::OnCellClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	int nRow, nCol;
	int i;
	int nKey;

	CString strTmp;
	CString strMsg;

	NM_GRIDVIEW* pItem	= (NM_GRIDVIEW*) pNotifyStruct;

	nRow				= pItem->iRow;
	nCol				= pItem->iColumn;

	if(nCol < 1) return;

	if(nRow == 0)
	{
		switch (nCol)
		{
			case 1:
				nKey = m_nOnWaitTime[0][1];

				KeyPadI(0, 5000, &nKey);

				for (i=0; i<m_nWaitTimeMaxCnt; i++)
				{
					m_nOnWaitTime[i][1] = nKey;

					strTmp.Format(_T("%0.2f"), nKey);
					m_pGridWaitTime.SetItemText(i + 1, nCol, strTmp);
				}
				break;

			case 2:
				nKey = m_nOffWaitTime[0][1];

				KeyPadI(0, 5000, &nKey);

				for (i=0; i<m_nWaitTimeMaxCnt; i++)
				{
					m_nOffWaitTime[i][1] = nKey;

					strTmp.Format(_T("%d"), nKey);
					m_pGridWaitTime.SetItemText(i + 1, nCol, strTmp);
				}
				break;

			case 3:
				nKey = m_nLimitWaitTime[0][1];

				KeyPadI(0, 20000, &nKey);

				for (i=0; i<m_nWaitTimeMaxCnt; i++)
				{
					m_nLimitWaitTime[i][1] = nKey;

					strTmp.Format(_T("%d"), nKey);
					m_pGridWaitTime.SetItemText(i + 1, nCol, strTmp);
				}
				break;
		}
	}
	else
	{
		switch (nCol)
		{
			case 1:
				if(clsExcel.m_nOnUsed[nRow -1] != 0)
				{
					nKey = m_nOnWaitTime[nRow - 1][1];

					KeyPadI(0, 5000, &nKey);

					m_nOnWaitTime[nRow - 1][1] = nKey;

					strTmp.Format(_T("%d"), nKey);
					m_pGridWaitTime.SetItemText(nRow, nCol, strTmp);
				}
				break;

			case 2:
				if(clsExcel.m_nOffUsed[nRow -1] != 0)
				{
					nKey = m_nOffWaitTime[nRow - 1][1];

					KeyPadI(0, 5000, &nKey);

					m_nOffWaitTime[nRow - 1][1] = nKey;

					strTmp.Format(_T("%d"), nKey);
					m_pGridWaitTime.SetItemText(nRow, nCol, strTmp);
				}
				break;

			case 3:
				if(clsExcel.m_nLimitUsed[nRow - 1] != 0)
				{
					nKey = m_nLimitWaitTime[nRow - 1][1];

					KeyPadI(0, 200000, &nKey);

					m_nLimitWaitTime[nRow - 1][1] = nKey;

					strTmp.Format(_T("%d"), nKey);
					m_pGridWaitTime.SetItemText(nRow, nCol, strTmp);
				}
				break;
		}
	}

	m_pGridWaitTime.Invalidate(FALSE);
}

BOOL CWorkWaitTime::OnEraseBkgnd(CDC* pDC)
{
	CDC  memDC;                     // �޸� DC
	CBitmap * pOldBitmap = NULL;    // m_BackBitmap���� ���ο� �׸��� DC�� �׸� ��, ���� �׷����� DC(��, Bitmap)�� ����.
	BITMAP bitmapInfo;              // �׸��� ����(m_BackBitmap)
	st_handler_info.m_BackDlgBitmap.GetBitmap(&bitmapInfo); // Bitmap ũ�� ����.

	memDC.CreateCompatibleDC(pDC);  // �޸� DC ����
	pOldBitmap = memDC.SelectObject(&st_handler_info.m_BackDlgBitmap);
	pDC->BitBlt(0, 0, bitmapInfo.bmWidth, bitmapInfo.bmHeight, &memDC, 0, 0, SRCCOPY);   // �޸� DC�� �׸��� �׸�

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	return true; // �� �η����� �ƴ� ����� �׷��� �ϱ� ������ true
}


void CWorkWaitTime::PostNcDestroy()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	/*	if (OnDataComparison() == RET_ERROR)
	{
		OnDataHistoryLog();
		OnDataApply();
	}*/
	CDialog::PostNcDestroy();
}


void CWorkWaitTime::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (OnDataComparison() == RET_ERROR)
	{
		OnDataHistoryLog();
		OnDataApply();
	}
}
