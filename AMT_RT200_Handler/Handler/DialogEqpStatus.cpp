// DialogEqpStatus.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "Handler.h"
#include "DialogEqpStatus.h"
#include "afxdialogex.h"
#include "Variable.h"
#include "GradientStatic.h"
#include "Dialog_KeyBoard.h"
#include "Alg_Mysql.h"
#include "PublicFunction.h"
#include "Dialog_Message.h"
#include ".\\Ctrl\\KeyBoard.h"

// CDialogEqpStatus 대화 상자입니다.
#define TM_MC_ENABLE		100
#define TM_MC_DISABLE		200
#define TM_MGZ_IN			300
#define TM_MGZ_OUT			400

IMPLEMENT_DYNAMIC(CDialogEqpStatus, CDialogEx)

	CDialogEqpStatus::CDialogEqpStatus(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogEqpStatus::IDD, pParent)
{

}

CDialogEqpStatus::~CDialogEqpStatus()
{
}

void CDialogEqpStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//	DDX_Control(pDX, IDC_BTN_EXIT,			m_btnExit);
	//	DDX_Control(pDX, IDCANCEL,				m_btnCancel);
	DDX_Control(pDX, IDC_BTN_DIALOG_EQP_EXIT,m_btnExit);
	DDX_Control(pDX, IDC_BTN_DIALOG_EQP_MAGAZINE_IN,m_btnMgzIn);
	DDX_Control(pDX, IDC_BTN_DIALOG_EQP_MAGAZINE_OUT,m_btnMgzOut);
	DDX_Control(pDX, IDC_STATIC_ENABLE,		m_stnEnable);
	DDX_Control(pDX, IDC_STATIC_DISABLE,	m_stnDisable);
	DDX_Control(pDX, IDC_GROUP,				m_groupTool);
	DDX_Control(pDX, IDC_GRID_TOOLLIST,		m_pGridPcbModel);
	DDX_Control(pDX, IDC_STATIC_ABLE,		m_staticAble);
	DDX_Control(pDX, IDC_STATIC_COMMENT,	m_staticComment);
	DDX_Control(pDX, IDC_STATIC_CODE,		m_staticCode);
	DDX_Control(pDX, IDC_TEXT_COMMENT,		m_ctrlComment);
	DDX_Control(pDX, IDC_TEXT_CODE,			m_ctrlCode);
}


BEGIN_MESSAGE_MAP(CDialogEqpStatus, CDialogEx)
	ON_STN_CLICKED(IDC_STATIC_ENABLE, &CDialogEqpStatus::OnStnClickedStaticEnable)
	ON_STN_CLICKED(IDC_STATIC_DISABLE, &CDialogEqpStatus::OnStnClickedStaticDisable)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_NOTIFY(NM_CLICK, IDC_GRID_TOOLLIST, &CDialogEqpStatus::OnToolListClick)
	ON_STN_CLICKED(IDC_TEXT_COMMENT, &CDialogEqpStatus::OnStnClickedTextComment)
	ON_WM_TIMER()
	ON_STN_CLICKED(IDC_TEXT_CODE, &CDialogEqpStatus::OnStnClickedTextCode)
	ON_BN_CLICKED(IDC_BTN_DIALOG_EQP_EXIT, &CDialogEqpStatus::OnBnClickedBtnDialogEqpExit)
	ON_BN_CLICKED(IDC_BTN_DIALOG_EQP_MAGAZINE_IN, &CDialogEqpStatus::OnBnClickedBtnDialogEqpMagazineIn)
	ON_BN_CLICKED(IDC_BTN_DIALOG_EQP_MAGAZINE_OUT, &CDialogEqpStatus::OnBnClickedBtnDialogEqpMagazineOut)
END_MESSAGE_MAP()


// CDialogEqpStatus 메시지 처리기입니다.

BOOL CDialogEqpStatus::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_nCurRow		=	-1;
	m_nDbRowCnt		=	0;
	m_strComment	=	_T("");
	m_bEnableFlag	=	st_basic_info.bEnableFlag;
	m_ctrlComment.SetWindowTextW(m_strComment);
	m_brushDisable.CreateSolidBrush(RED_C);
	m_brushEnable.CreateSolidBrush(GREEN_C);

	m_strModelName = st_basic_info.strModelName;

	OnInitControls();
	//2015.02.09
	OnInitEqpFile(); 
	//OnInitDB();
	OnInitStatic();

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDialogEqpStatus::OnInitDB()
{
	COleDateTime oledate[2];
	CTime time	= CTime::GetCurrentTime();

	CString strTemp;
	CString strData[5];

	int nPos[2];
	int nCount = -1;
	int i, j;

	m_nCurRow = -1;

	oledate[0].SetDateTime(time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	oledate[1].SetDateTime(time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

	//if (Mysql.m_bFlag == TRUE) return;

	if (Mysql.MySql_Open(8))
	{
		if (Mysql.Database_Select(8, _T("AMT")))
		{
			if (Mysql.Table_Select(8, _T("AMT"), _T("PCB_MODEL_DATA")))
			{
				nCount = Mysql.Record_Date_Search(8, _T("PCB_MODEL_DATA"), _T("DATE"), _T(">"), oledate);
				OnInitGrid(nCount);

				if (nCount > 0)
				{
					for (i=0; i<nCount; i++)
					{
						strTemp = Mysql.Record_Read(8);
						nPos[0] =0;
						for (j=0; j<Mysql.m_nToolTableCnt; j++)
						{
							nPos[1]		= strTemp.Find(_T(","), nPos[0] + 1);
							strData[j]	= strTemp.Mid(nPos[0], (nPos[1] - nPos[0]));
							nPos[0]		= nPos[1] + 1;
						}
						m_pGridPcbModel.SetItemText(i + 1, 0, strData[2]);

						if (strData[2] == m_strModelName)
						{
							m_nCurRow = i + 1;

							m_pGridPcbModel.SetItemBkColour(i + 1, 0, GREEN_C, BLACK_C);
						}
						else
						{
							m_pGridPcbModel.SetItemBkColour(i + 1, 0, RGB(250, 250, 200), BLACK_C);
						}
					}
				}
			}

		}
	}




	if (m_nCurRow < 0)
	{
		m_nCurRow = 1;
		OnInitGrid(m_nCurRow);

		m_pGridPcbModel.SetItemBkColour(m_nCurRow, 0, GREEN_C, BLACK_C);
	}

	Mysql.MySql_Close(8);
}

void CDialogEqpStatus::OnInitGrid(int nCount)
{
	int i,j;
	int nRowCnt;
	CRect rect;

	nRowCnt = nCount + 1;

	m_pGridPcbModel.Refresh();

	m_pGridPcbModel.GetClientRect(&rect);
	m_pGridPcbModel.SetFrameFocusCell(FALSE);
	m_pGridPcbModel.SetTrackFocusCell(FALSE);
	m_pGridPcbModel.EnableSelection(FALSE);
	m_pGridPcbModel.EnableScrollBars(SB_VERT, TRUE);

	m_pGridPcbModel.SetGridLineColor(BLACK_GC);
	m_pGridPcbModel.SetGridLines(2);

	m_pGridPcbModel.SetRowCount(nRowCnt);
	m_pGridPcbModel.SetColumnCount(1);

	m_pGridPcbModel.SetFixedRowCount(0);
	m_pGridPcbModel.SetFixedColumnCount(0);
	m_pGridPcbModel.SetFixedBkColor(RGB(0,0,200));
	m_pGridPcbModel.SetFixedBkColor(RGB(200,200,255));
	m_pGridPcbModel.SetTextBkColor(RGB(150,150,200));

	for (i=0; i<nRowCnt; i++)
	{
		if (i == 0)
		{
			m_pGridPcbModel.SetRowHeight(i, 30);
		}
		else
		{
			m_pGridPcbModel.SetRowHeight(i, 50);
		}
		m_pGridPcbModel.SetColumnWidth(0, 390);

		for (j=0; j<1; j++)
		{
			m_pGridPcbModel.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_pGridPcbModel.SetItemState(i, j, GVIS_READONLY);
			m_pGridPcbModel.SetItemBkColour(i, j, WHITE_C, CLR_DEFAULT);

			if (i==0)
			{
				m_pGridPcbModel.SetItemBkColour(i, j, RGB(150, 150, 150), CLR_DEFAULT);
			}
			else
			{
				m_pGridPcbModel.SetItemBkColour(i, j, RGB(250, 250, 200), BLACK_C);
				m_pGridPcbModel.SetItemText(i, j, _T(""));
			}
			m_pGridPcbModel.SetItemFont(i, j, &clsFunc.OnLogFont(17));
		}
	}
	m_pGridPcbModel.SetItemText(0, 0, _T("Model Name"));
	m_pGridPcbModel.Invalidate(FALSE);
}

void CDialogEqpStatus::OnInitControls()
{
	m_btnExit.SetBitmaps(IDC_BTN_DIALOG_EQP_EXIT, IDB_BITMAP_EXIT, WINDOW_DN, IDB_BITMAP_EXIT, WINDOW_UP);
	m_btnExit.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnExit.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnExit.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnExit.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnExit.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnExit.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnExit.SetFont(clsFunc.m_pFont[3]);
	m_btnExit.SetTooltipText(_T("Exit"));

	m_btnMgzIn.SetBitmaps(IDC_BTN_DIALOG_EQP_MAGAZINE_IN, IDB_NETWORK_ON_UP, WINDOW_DN, IDB_NETWORK_ON_UP, WINDOW_UP);
	m_btnMgzIn.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnMgzIn.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnMgzIn.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnMgzIn.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnMgzIn.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnMgzIn.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnMgzIn.SetFont(clsFunc.m_pFont[3]);
	m_btnMgzIn.SetTooltipText(_T("Magazine In"));

	m_btnMgzOut.SetBitmaps(IDC_BTN_DIALOG_EQP_MAGAZINE_OUT, IDB_NETWORK_ON_UP, WINDOW_DN, IDB_NETWORK_ON_UP, WINDOW_UP);
	m_btnMgzOut.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnMgzOut.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnMgzOut.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnMgzOut.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnMgzOut.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnMgzOut.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnMgzOut.SetFont(clsFunc.m_pFont[3]);
	m_btnMgzOut.SetTooltipText(_T("Magazine Out"));

	m_groupTool.SetFont(clsFunc.OnLogFont(14));
	m_groupTool.SetCatptionTextColor(BLUE_C);
	m_groupTool.SetBorderColor(ORANGE_C);
	m_groupTool.SetFontBold(TRUE);
	m_groupTool.SetBackgroundColor(WINDOW_UP);
}

void CDialogEqpStatus::OnInitStatic()
{
	m_staticAble.SetFont(clsFunc.m_pFont[1]);
	m_staticAble.SetWindowText(_T("Enable/Disable"));
	m_staticAble.SetCenterText();
	m_staticAble.SetColor(WHITE_C);
	m_staticAble.SetGradientColor(ORANGE_C);
	m_staticAble.SetTextColor(BLACK_C);

	m_staticComment.SetFont(clsFunc.m_pFont[1]);
	m_staticComment.SetWindowText(_T("Comment"));
	m_staticComment.SetCenterText();
	m_staticComment.SetColor(WHITE_C);
	m_staticComment.SetGradientColor(ORANGE_C);
	m_staticComment.SetTextColor(BLACK_C);

	m_staticCode.SetFont(clsFunc.m_pFont[1]);
	m_staticCode.SetWindowText(_T("Code"));
	m_staticCode.SetCenterText();
	m_staticCode.SetColor(WHITE_C);
	m_staticCode.SetGradientColor(ORANGE_C);
	m_staticCode.SetTextColor(BLACK_C);
}

void CDialogEqpStatus::OnDisplayList()
{

}

void CDialogEqpStatus::OnStnClickedStaticEnable()
{
	//	m_bEnableFlag = true;

	//	Invalidate(TRUE);
	if (st_handler_info.nMenuLock != FALSE) return;

	m_nRetry			= 0;
	m_nMcEnableStep		= 0;
	st_handler_info.nMenuLock = TRUE;

	SetTimer(TM_MC_ENABLE, 100, NULL);
}


void CDialogEqpStatus::OnStnClickedStaticDisable()
{
	//	m_bEnableFlag = false;

	//	Invalidate(TRUE);
	if (st_handler_info.nMenuLock != FALSE) return;

	m_nRetry			= 0;
	m_nMcDisableStep	= 0;
	st_handler_info.nMenuLock = TRUE;

	SetTimer(TM_MC_DISABLE, 100, NULL);
}


HBRUSH CDialogEqpStatus::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		if (pWnd->GetDlgCtrlID() == IDC_STATIC_ENABLE)
		{
			m_brushEnable.DeleteObject();

			if (m_bEnableFlag) 
			{
				m_brushEnable.CreateSolidBrush(GREEN_C);
				pDC->SetTextColor(BLACK);
				pDC->SetBkColor(GREEN_C);

				return m_brushEnable;
			}
			else 
			{
				m_brushEnable.CreateSolidBrush(GRAY);
				pDC->SetTextColor(WHITE_C);
				pDC->SetBkColor(GRAY);

				return m_brushEnable;
			}
		}
		else if (pWnd->GetDlgCtrlID() == IDC_STATIC_DISABLE)
		{
			m_brushDisable.DeleteObject();

			if (!m_bEnableFlag)
			{
				m_brushDisable.CreateSolidBrush(RED_C);
				pDC->SetTextColor(BLACK);
				pDC->SetBkColor(RED_C);

				return m_brushDisable;
			}
			else 
			{
				m_brushDisable.CreateSolidBrush(GRAY);
				pDC->SetTextColor(WHITE_C);
				pDC->SetBkColor(GRAY);

				return m_brushDisable;
			}
		}
		else if (pWnd->GetDlgCtrlID() == IDC_TEXT_COMMENT)
		{
			pDC->SetBkColor(WHITE_C);
			return (HBRUSH)::GetStockObject(WHITE_BRUSH);
		}
		else
		{
			pDC->SetBkMode(TRANSPARENT);
			return (HBRUSH)::GetStockObject(NULL_BRUSH);
		}
	}
	return hbr;
}


BOOL CDialogEqpStatus::OnEraseBkgnd(CDC* pDC)
{
	CDC  memDC;                     // 메모리 DC
	CBitmap * pOldBitmap = NULL;    // m_BackBitmap으로 새로운 그림을 DC에 그릴 때, 이전 그려졌던 DC(즉, Bitmap)을 저장.
	BITMAP bitmapInfo;              // 그림의 정보(m_BackBitmap)
	st_handler_info.m_BackBitmap.GetBitmap(&bitmapInfo); // Bitmap 크기 구함.

	memDC.CreateCompatibleDC(pDC);  // 메모리 DC 생성
	pOldBitmap = memDC.SelectObject(&st_handler_info.m_BackBitmap);
	pDC->BitBlt(0, 0, bitmapInfo.bmWidth, bitmapInfo.bmHeight, &memDC, 0, 0, SRCCOPY);   // 메모리 DC에 그림을 그림

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();

	return true; // 흰 부러쉬가 아닌 배경을 그려야 하기 때문에 true
}


BOOL CDialogEqpStatus::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_brushDisable.DeleteObject();
	m_brushEnable.DeleteObject();


	return CDialogEx::DestroyWindow();
}


void CDialogEqpStatus::OnToolListClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	int nRow, nCol;
	int nResponse;

	CDialog_Message dlgMsg;

	NM_GRIDVIEW* pItem	= (NM_GRIDVIEW*) pNotifyStruct;

	nRow				= pItem->iRow;
	nCol				= pItem->iColumn;

	if(nRow < 1) return ; 

	if (nRow != m_nCurRow)
	{
		dlgMsg.m_nMessageType	= 1;
		dlgMsg.m_strMessage.Format(_T("[%s] Model\r\nDo you want a clicked model?"), m_pGridPcbModel.GetItemText(nRow, nCol));

		nResponse = dlgMsg.DoModal();

		if (nResponse == IDOK)
		{
			m_strModelName = m_pGridPcbModel.GetItemText(nRow, nCol);
			m_pGridPcbModel.SetItemBkColour(nRow, 0, GREEN_C, BLACK_C);

			if (m_nCurRow > 0)
			{
				m_pGridPcbModel.SetItemBkColour(m_nCurRow, 0, RGB(250, 250, 200), BLACK_C);
			}

			m_nCurRow = nRow;
		}
	}

	m_pGridPcbModel.Invalidate(FALSE);
}

void CDialogEqpStatus::OnStnClickedTextComment()
{
	CString strKey;

	KeyBoard(&strKey);

	m_strComment = strKey;
	m_ctrlComment.SetWindowTextW(m_strComment);
}

void CDialogEqpStatus::OnStnClickedTextCode()
{
	CString strKey;

	KeyBoard(&strKey);

	m_strCode = strKey;
	m_ctrlCode.SetWindowTextW(m_strCode);
}


int	CDialogEqpStatus::OnMcEnable()
{
	switch (m_nMcEnableStep)
	{
	case 0:
		// 작업중인 모드체크
		if (st_basic_info.nModeInterface == EQP_ON_LINE)
		{
			// 작업모드가 on line 모드이면
			m_dwWaitTime[0] = GetTickCount();

			m_nMcEnableStep = 100;
		}
		else
		{
			// off line 모드이면 
			m_nMcEnableStep = 0;

			return RET_GOOD;
		}
		break;

	case 100:
		// datacenter client 현재 상태 체크
		if (st_client_info[EC_FIRST_NETWORK].nConnect == NO)
		{
			// 클라이언트 대기중이면 
			m_nMcEnableStep = 200;
		}
		else
		{
			m_dwWaitTime[1] = GetTickCount();
			m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];

			if (m_dwWaitTime[2] <= 0)
			{
				m_dwWaitTime[0] = GetTickCount();
				break;
			}

			if (m_dwWaitTime[2] > 10000)
			{
				// 5초동안 대기해서 강제 종료
				m_dwWaitTime[1] = GetTickCount();
				//::PostMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_CLOSE, 0);
				::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_CLOSE, 0);
			}
		}
		break;

	case 200:
		// client 연결 요청 
		m_dwWaitTime[0] = GetTickCount();
		//::PostMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_CONNECT, 0);
		::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_CONNECT, 0);

		m_nMcEnableStep = 300;
		break;

	case 300:
		// client 연결상태 체크
		if (st_client_info[EC_FIRST_NETWORK].nConnect == YES)
		{
			// 현재 연결중이면 데이터 전송으로 
			m_nMcEnableStep = 400;
		}
		else 
		{
			m_dwWaitTime[1] = GetTickCount();
			m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];

			if (m_dwWaitTime[2] <= 0)
			{
				m_dwWaitTime[0] = GetTickCount();
				break;
			}

			if (m_dwWaitTime[2] > 5000)
			{
				m_nRetry++;
				if (m_nRetry < st_basic_info.nRetry)
				{
					m_nMcEnableStep = 0;
				}
				else
				{
					return RET_ERROR;
				}
			}
		}
		break;

	case 400:
		m_dwWaitTime[0]								= GetTickCount();

//		clsDataClient.m_nMcReq						= BIN_NOTUESD;
//		st_client_info[EC_FIRST_NETWORK].strSend	= clsDataClient.OnMcEnable(m_strCode, m_strComment);
		//			st_client_info[EC_FIRST_NETWORK].strSend	= clsDataClient.OnLotQty(_T("2000000000L1MZ7WD480142400001736"));

		//::PostMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_SEND, 0);
//		::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_SEND, 0);

		m_nMcEnableStep = 500;
		break;

	case 500:
		// 데이터 송신 결과 확인 작업
/*		if (clsDataClient.m_nMcReq == BIN_GOODBIN)
		{
			m_nMcEnableStep = 0;

			return RET_GOOD;
		}
		else if (clsDataClient.m_nMcReq == BIN_REJECTBIN)
		{
			m_nRetry++;
			if (m_nRetry < st_basic_info.nRetry)
			{
				m_nMcEnableStep = 100;
			}
			else
			{
				m_nMcEnableStep = 0;

				return RET_ERROR;
			}
		}
		else
		{
			m_dwWaitTime[1] = GetTickCount();
			m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];

			if (m_dwWaitTime[2] <= 0)
			{
				m_dwWaitTime[0] = GetTickCount();
				break;
			}

			if (m_dwWaitTime[2] > 10000)
			{
				m_nRetry++;
				if (m_nRetry < st_basic_info.nRetry)
				{
					m_nMcEnableStep = 100;
				}
				else
				{
					m_nMcEnableStep = 0;

					return RET_ERROR;
				}
			}
		}*/
		break;
	}

	return RET_PROCEED;
}


int	CDialogEqpStatus::OnMcDisable()
{
	switch (m_nMcDisableStep)
	{
	case 0:
		// 작업중인 모드체크
		if (st_basic_info.nModeInterface == EQP_ON_LINE)
		{
			// 작업모드가 on line 모드이면
			m_dwWaitTime[0] = GetTickCount();

			m_nMcDisableStep = 100;
		}
		else
		{
			// off line 모드이면 
			m_nMcDisableStep = 0;

			return RET_GOOD;
		}
		break;

	case 100:
		// datacenter client 현재 상태 체크
		if (st_client_info[EC_FIRST_NETWORK].nConnect == NO)
		{
			// 클라이언트 대기중이면 
			m_nMcDisableStep = 200;
		}
		else
		{
			m_dwWaitTime[1] = GetTickCount();
			m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];

			if (m_dwWaitTime[2] <= 0)
			{
				m_dwWaitTime[0] = GetTickCount();
				break;
			}

			if (m_dwWaitTime[2] > 10000)
			{
				// 5초동안 대기해서 강제 종료
				m_dwWaitTime[1] = GetTickCount();
				//::PostMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_CLOSE, 0);
				::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_CLOSE, 0);
			}
		}
		break;

	case 200:
		// client 연결 요청 
		m_dwWaitTime[0] = GetTickCount();
		//::PostMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_CONNECT, 0);
		::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_CONNECT, 0);

		m_nMcDisableStep = 300;
		break;

	case 300:
		// client 연결상태 체크
		if (st_client_info[EC_FIRST_NETWORK].nConnect == YES)
		{
			// 현재 연결중이면 데이터 전송으로 
			m_nMcDisableStep = 400;
		}
		else 
		{
			m_dwWaitTime[1] = GetTickCount();
			m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];

			if (m_dwWaitTime[2] <= 0)
			{
				m_dwWaitTime[0] = GetTickCount();
				break;
			}

			if (m_dwWaitTime[2] > 5000)
			{
				m_nRetry++;
				if (m_nRetry < st_basic_info.nRetry)
				{
					m_nMcDisableStep = 0;
				}
				else
				{
					return RET_ERROR;
				}
			}
		}
		break;

	case 400:
		m_dwWaitTime[0]								= GetTickCount();

//		clsDataClient.m_nMcReq						= BIN_NOTUESD;
//		st_client_info[EC_FIRST_NETWORK].strSend	= clsDataClient.OnMcDisable(m_strCode, m_strComment);

		//::PostMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_SEND, 0);
//		::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_SEND, 0);

		m_nMcDisableStep = 500;
		break;

	case 500:
		// 데이터 송신 결과 확인 작업
/*		if (clsDataClient.m_nMcReq == BIN_GOODBIN)
		{
			m_nMcDisableStep = 0;

			return RET_GOOD;
		}
		else if (clsDataClient.m_nMcReq == BIN_REJECTBIN)
		{
			m_nRetry++;
			if (m_nRetry < st_basic_info.nRetry)
			{
				m_nMcDisableStep = 100;
			}
			else
			{
				m_nMcDisableStep = 0;

				return RET_ERROR;
			}
		}
		else
		{
			m_dwWaitTime[1] = GetTickCount();
			m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];

			if (m_dwWaitTime[2] <= 0)
			{
				m_dwWaitTime[0] = GetTickCount();
				break;
			}

			if (m_dwWaitTime[2] > 10000)
			{
				m_nRetry++;
				if (m_nRetry < st_basic_info.nRetry)
				{
					m_nMcDisableStep = 100;
				}
				else
				{
					m_nMcDisableStep = 0;

					return RET_ERROR;
				}
			}
		}*/
		break;
	}

	return RET_PROCEED;
}


int	CDialogEqpStatus::OnMagazineIn()
{
	switch (m_nMgzStep)
	{
	case 0:
		// 작업중인 모드체크
		if (st_basic_info.nModeInterface == EQP_ON_LINE)
		{
			// 작업모드가 on line 모드이면
			m_dwWaitTime[0] = GetTickCount();

			m_nMgzStep = 100;
		}
		else
		{
			// off line 모드이면 
			m_nMgzStep = 0;

			return RET_GOOD;
		}
		break;

	case 100:
		// datacenter client 현재 상태 체크
		if (st_client_info[EC_FIRST_NETWORK].nConnect == NO)
		{
			// 클라이언트 대기중이면 
			m_nMgzStep = 200;
		}
		else
		{
			m_dwWaitTime[1] = GetTickCount();
			m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];

			if (m_dwWaitTime[2] <= 0)
			{
				m_dwWaitTime[0] = GetTickCount();
				break;
			}

			if (m_dwWaitTime[2] > 10000)
			{
				// 5초동안 대기해서 강제 종료
				m_dwWaitTime[1] = GetTickCount();
				//::PostMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_CLOSE, 0);
				::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_CLOSE, 0);
			}
		}
		break;

	case 200:
		// client 연결 요청 
		m_dwWaitTime[0] = GetTickCount();
		//::PostMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_CONNECT, 0);
		::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_CONNECT, 0);

		m_nMgzStep = 300;
		break;

	case 300:
		// client 연결상태 체크
		if (st_client_info[EC_FIRST_NETWORK].nConnect == YES)
		{
			// 현재 연결중이면 데이터 전송으로 
			m_nMgzStep = 400;
		}
		else 
		{
			m_dwWaitTime[1] = GetTickCount();
			m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];

			if (m_dwWaitTime[2] <= 0)
			{
				m_dwWaitTime[0] = GetTickCount();
				break;
			}

			if (m_dwWaitTime[2] > 5000)
			{
				m_nRetry++;
				if (m_nRetry < st_basic_info.nRetry)
				{
					m_nMgzStep = 0;
				}
				else
				{
					return RET_ERROR;
				}
			}
		}
		break;

	case 400:
		m_dwWaitTime[0]								= GetTickCount();

//		clsDataClient.m_nMgzInReq					= BIN_NOTUESD;
//		st_client_info[EC_FIRST_NETWORK].strSend	= clsDataClient.OnFullMagazineInReq();
		//			clsDataClient.m_nLotQtyReq					= BIN_NOTUESD;
		//			st_client_info[EC_FIRST_NETWORK].strSend	= clsDataClient.OnLotQty(_T("2000000000L2MZJPU256142600000931"));
		//::PostMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_SEND, 0);
//		::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_SEND, 0);

		m_nMgzStep = 500;
		break;

	case 500:
		// 데이터 송신 결과 확인 작업
/*		if (clsDataClient.m_nMgzInReq == BIN_GOODBIN)
			//if (clsDataClient.m_nLotQtyReq == BIN_GOODBIN)
		{
			m_nMgzStep = 0;

			return RET_GOOD;
		}
		else if (clsDataClient.m_nMgzInReq == BIN_REJECTBIN)
			//else if (clsDataClient.m_nLotQtyReq == BIN_REJECTBIN)
		{
			m_nRetry++;
			if (m_nRetry < st_basic_info.nRetry)
			{
				m_nMgzStep = 100;
			}
			else
			{
				m_nMcDisableStep = 0;

				return RET_ERROR;
			}
		}
		else
		{
			m_dwWaitTime[1] = GetTickCount();
			m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];

			if (m_dwWaitTime[2] <= 0)
			{
				m_dwWaitTime[0] = GetTickCount();
				break;
			}

			if (m_dwWaitTime[2] > 5000)
			{
				m_nRetry++;
				if (m_nRetry < st_basic_info.nRetry)
				{
					m_nMgzStep = 100;
				}
				else
				{
					m_nMgzStep = 0;

					return RET_ERROR;
				}
			}
		}*/
		break;
	}

	return RET_PROCEED;
}


int	CDialogEqpStatus::OnMagazineOut()
{
	switch (m_nMgzStep)
	{
	case 0:
		// 작업중인 모드체크
		if (st_basic_info.nModeInterface == EQP_ON_LINE)
		{
			// 작업모드가 on line 모드이면
			m_dwWaitTime[0] = GetTickCount();

			m_nMgzStep = 100;
		}
		else
		{
			// off line 모드이면 
			m_nMgzStep = 0;

			return RET_GOOD;
		}
		break;

	case 100:
		// datacenter client 현재 상태 체크
		if (st_client_info[EC_FIRST_NETWORK].nConnect == NO)
		{
			// 클라이언트 대기중이면 
			m_nMgzStep = 200;
		}
		else
		{
			m_dwWaitTime[1] = GetTickCount();
			m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];

			if (m_dwWaitTime[2] <= 0)
			{
				m_dwWaitTime[0] = GetTickCount();
				break;
			}

			if (m_dwWaitTime[2] > 10000)
			{
				// 5초동안 대기해서 강제 종료
				m_dwWaitTime[1] = GetTickCount();
				//::PostMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_CLOSE, 0);
				::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_CLOSE, 0);
			}
		}
		break;

	case 200:
		// client 연결 요청 
		m_dwWaitTime[0] = GetTickCount();
		//::PostMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_CONNECT, 0);
		::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_CONNECT, 0);

		m_nMgzStep = 300;
		break;

	case 300:
		// client 연결상태 체크
		if (st_client_info[EC_FIRST_NETWORK].nConnect == YES)
		{
			// 현재 연결중이면 데이터 전송으로 
			m_nMgzStep = 400;
		}
		else 
		{
			m_dwWaitTime[1] = GetTickCount();
			m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];

			if (m_dwWaitTime[2] <= 0)
			{
				m_dwWaitTime[0] = GetTickCount();
				break;
			}

			if (m_dwWaitTime[2] > 5000)
			{
				m_nRetry++;
				if (m_nRetry < st_basic_info.nRetry)
				{
					m_nMgzStep = 0;
				}
				else
				{
					return RET_ERROR;
				}
			}
		}
		break;

	case 400:
		m_dwWaitTime[0]								= GetTickCount();

//		clsDataClient.m_nMgzOutReq					= BIN_NOTUESD;


		//::PostMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_SEND, 0);
//		::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_SEND, 0);

		m_nMgzStep = 500;
		break;

	case 500:
		// 데이터 송신 결과 확인 작업
/*		if (clsDataClient.m_nMgzOutReq == BIN_GOODBIN)
		{
			m_nMgzStep = 0;

			return RET_GOOD;
		}
		else if (clsDataClient.m_nMgzOutReq == BIN_REJECTBIN)
		{
			m_nRetry++;
			if (m_nRetry < st_basic_info.nRetry)
			{
				m_nMgzStep = 100;
			}
			else
			{
				m_nMcDisableStep = 0;

				return RET_ERROR;
			}
		}
		else
		{
			m_dwWaitTime[1] = GetTickCount();
			m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];

			if (m_dwWaitTime[2] <= 0)
			{
				m_dwWaitTime[0] = GetTickCount();
				break;
			}

			if (m_dwWaitTime[2] > 10000)
			{
				m_nRetry++;
				if (m_nRetry < st_basic_info.nRetry)
				{
					m_nMgzStep = 100;
				}
				else
				{
					m_nMgzStep = 0;

					return RET_ERROR;
				}
			}
		}*/
		break;
	}

	return RET_PROCEED;
}


void CDialogEqpStatus::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int nRet;

	switch (nIDEvent)
	{
	case TM_MC_ENABLE:
		nRet = OnMcEnable();

		if (nRet == RET_GOOD)
		{
			KillTimer(TM_MC_ENABLE);

			st_handler_info.nMenuLock = FALSE;

			m_bEnableFlag = true;
			Invalidate(TRUE);
		}
		else if (nRet == RET_ERROR)
		{
			st_handler_info.nMenuLock = FALSE;

			KillTimer(TM_MC_ENABLE);
		}
		break;

	case TM_MC_DISABLE:
		nRet = OnMcDisable();

		if (nRet == RET_GOOD)
		{
			KillTimer(TM_MC_DISABLE);

			st_handler_info.nMenuLock = FALSE;

			m_bEnableFlag = false;
			Invalidate(TRUE);
		}
		else if (nRet == RET_ERROR)
		{
			st_handler_info.nMenuLock = FALSE;

			KillTimer(TM_MC_DISABLE);
		}
		break;

	case TM_MGZ_IN:
		nRet = OnMagazineIn();

		if (nRet == RET_GOOD)
		{
			KillTimer(TM_MGZ_IN);

			st_handler_info.nMenuLock = FALSE;

			m_bEnableFlag = false;
			Invalidate(TRUE);
		}
		else if (nRet == RET_ERROR)
		{
			st_handler_info.nMenuLock = FALSE;

			KillTimer(TM_MGZ_IN);
		}
		break;

	case TM_MGZ_OUT:
		nRet = OnMagazineOut();

		if (nRet == RET_GOOD)
		{
			KillTimer(TM_MGZ_OUT);

			st_handler_info.nMenuLock = FALSE;

			m_bEnableFlag = false;
			Invalidate(TRUE);
		}
		else if (nRet == RET_ERROR)
		{
			st_handler_info.nMenuLock = FALSE;

			KillTimer(TM_MGZ_OUT);
		}
		break;
	}
	st_basic_info.bEnableFlag = m_bEnableFlag;

	CDialogEx::OnTimer(nIDEvent);
}




void CDialogEqpStatus::OnBnClickedBtnDialogEqpExit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (st_handler_info.nMenuLock == TRUE) return;

	CDialog::OnOK();
}


void CDialogEqpStatus::OnBnClickedBtnDialogEqpMagazineIn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (st_handler_info.nMenuLock != FALSE) return;

	m_nRetry		= 0;
	m_nMgzStep		= 0;
	st_handler_info.nMenuLock = TRUE;

	SetTimer(TM_MGZ_IN, 100, NULL);
}


void CDialogEqpStatus::OnBnClickedBtnDialogEqpMagazineOut()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (st_handler_info.nMenuLock != FALSE) return;

	m_nRetry	= 0;
	m_nMgzStep	= 0;
	st_handler_info.nMenuLock = TRUE;

	SetTimer(TM_MGZ_OUT, 100, NULL);
}
//2015.02.09
void CDialogEqpStatus::OnInitEqpFile()
{
	int i;
	int nChkCnt;	
	CString str_temp, str_name;
	char ch_data[10];
	//	char ch_tmp;
	CString strLoadFile;
	// 	CString strReadData;  // 파일에 저장된 한 라인 정보 저장 변수
	CString strTempData[10];


	strLoadFile = _T("d:\\AMT8562\\Setting\\EqpFile.txt"); 
	:: GetPrivateProfileString(_T("EQP_STATUS"), _T("COUNT"), _T("0"), (LPWSTR)ch_data, 10, strLoadFile);

	nChkCnt = atoi(ch_data);
	OnInitGrid(nChkCnt);

	for(i=0; i<nChkCnt; i++)
	{
		str_name.Format(_T("%d"), i+1);
		:: GetPrivateProfileString(_T("EQP_STATUS"), str_name, _T(""), (LPWSTR)ch_data, 10, strLoadFile);

		str_temp.Format(_T("%s"), ch_data);
		strTempData[i] = str_temp;
		m_pGridPcbModel.SetItemText( i+1, 0, strTempData[i]);
	}
}
