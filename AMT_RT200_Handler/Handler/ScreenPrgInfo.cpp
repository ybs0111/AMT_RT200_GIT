// ScreenPrgInfo.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Handler.h"
#include "ScreenPrgInfo.h"
#include "Variable.h"
#include "PublicFunction.h"

// CScreenPrgInfo

IMPLEMENT_DYNCREATE(CScreenPrgInfo, CFormView)

CScreenPrgInfo::CScreenPrgInfo()
	: CFormView(CScreenPrgInfo::IDD)
{
	m_nExelSelect = 0;
}

CScreenPrgInfo::~CScreenPrgInfo()
{
}

void CScreenPrgInfo::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM_INPUT_PART_IO,	m_pGridExelSheet);
}

BEGIN_MESSAGE_MAP(CScreenPrgInfo, CFormView)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CScreenPrgInfo 진단입니다.

#ifdef _DEBUG
void CScreenPrgInfo::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CScreenPrgInfo::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CScreenPrgInfo 메시지 처리기입니다.


void CScreenPrgInfo::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// 버튼의 스타일을 지정한다. 
	OnInitButton();

	// 선택된 Exel 정보를 가져온다. 
	// 최초 Open 시 0번 I/O 화면 전보 가져온다. 
	OnExelReadInfo(m_nExelSelect);

	// ExelReadInfo에서 가져온 정보고 Grid 화면을 그려준다. 
	OnInitGridExelSheet(m_nExelSelect);

	// 선택된 Exel 정보와 같이 Exel 내용도 가져왔으므로 화면에 갱신한다. 
	OnGridDataUpdate();
}

void CScreenPrgInfo::OnInitButton()
{

}

void CScreenPrgInfo::OnExelReadInfo(int nExelSelect)
{
	switch(nExelSelect)
	{
	// I/O 정보를 가져온다. 
	case 0:
		/*clsExcel.m_strAxisName*/
		break;

	// I/O Part 정보를 가져온다. 
	case 1:
		break;

	// Motor Axis 정보를 가져온다. 
	case 2: 
		break;

	// Motor Part 정보를 가져온다. 
	case 3:
		break;

	// Wait Time 정보를 가져온다. 
	case 4:
		break;
	}
}

void CScreenPrgInfo::OnInitGridExelSheet(int nExelSelect)
{
	// 현재 선택된 Exel 정보로 Grid의 틀과 색상을 그려준다. 
	switch(nExelSelect)
	{
	// I/O 정보를 가져온다. 
	case 0:
		OnGridInitIOInfo();
		break;

	// I/O Part 정보를 가져온다. 
	case 1:
		OnGridInitPartIOInfo();
		break;

	// Motor Axis 정보를 가져온다. 
	case 2: 
		OnGridInitMotorAxisInfo();
		break;

	// Motor Part 정보를 가져온다. 
	case 3:
		OnGridInitMotorPartInfo();
		break;

	// Wait Time 정보를 가져온다. 
	case 4:
		OnGridInitWaitTimeInfo();
		break;
	}
}

void CScreenPrgInfo::OnDestroy()
{
	CFormView::OnDestroy();

}

void CScreenPrgInfo::OnGridInitIOInfo()
{
	//m_pGridExelShit
	CString strTmp;
	int i, j;

	m_pGridExelSheet.SetFrameFocusCell(FALSE);
	m_pGridExelSheet.SetTrackFocusCell(FALSE);
	m_pGridExelSheet.EnableSelection(FALSE);

	m_pGridExelSheet.SetGridLineColor(BLACK_C);
	m_pGridExelSheet.SetGridLines(1);

	m_pGridExelSheet.SetRowCount(19);
	m_pGridExelSheet.SetColumnCount(6);

	m_pGridExelSheet.SetFixedRowCount(0);
	m_pGridExelSheet.SetFixedColumnCount(0);
	m_pGridExelSheet.SetFixedBkColor(RGB(0,0,200));
	m_pGridExelSheet.SetFixedBkColor(RGB(200,200,255));
	m_pGridExelSheet.SetTextBkColor(RGB(150,150,200));

	for (i=0; i<19; i++) 
	{
		m_pGridExelSheet.SetRowHeight(i, 41);

		for (j=0; j<6; j++) 
		{
			switch (j)
			{
			case 0:
				m_pGridExelSheet.SetColumnWidth(j, 102);
				break;

			case 1:
				m_pGridExelSheet.SetColumnWidth(j, 130);
				break;

			case 2:
				m_pGridExelSheet.SetColumnWidth(j, 120);
				break;

			case 3:
				m_pGridExelSheet.SetColumnWidth(j, 50);
				break;

			case 4:
				m_pGridExelSheet.SetColumnWidth(j, 120);
				break;

			case 5:
				m_pGridExelSheet.SetColumnWidth(j, 380);
				break;
			}

			m_pGridExelSheet.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_pGridExelSheet.SetItemState(i, j, GVIS_READONLY);
			m_pGridExelSheet.SetItemBkColour(i, j, WHITE_C, CLR_DEFAULT);
		}
	}

	// IO MAP Header ////////////////////////////////////////////////////////////////////////////////////////////// 
	m_pGridExelSheet.MergeCells(0, 0, 0, 5);
	m_pGridExelSheet.SetItemBkColour(0, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridExelSheet.SetItemFont(0, 0, &clsFunc.OnLogFont(32));
	m_pGridExelSheet.SetItemText(0, 0, _T("I/O Map Design Sheet"));
				 
	m_pGridExelSheet.MergeCells(1, 0, 1, 2);
	m_pGridExelSheet.SetItemBkColour(1, 0, RGB(242, 242, 242), BLACK_L);
	m_pGridExelSheet.SetItemFont(1, 0, &clsFunc.OnLogFont(20));
	m_pGridExelSheet.SetItemText(1, 0, _T("Master : [0] Port : [0] Slave : [0]"));
				 
	m_pGridExelSheet.MergeCells(1, 3, 1, 4);
	m_pGridExelSheet.SetItemBkColour(1, 3, RGB(252, 213, 180), BLACK_L);
	m_pGridExelSheet.SetItemFont(1, 3, &clsFunc.OnLogFont(20));
	m_pGridExelSheet.SetItemText(1, 3, _T("Ass'Y"));
				 
	m_pGridExelSheet.SetItemBkColour(1, 5, RGB(242, 242, 242), BLACK_L);
	m_pGridExelSheet.SetItemFont(1, 5, &clsFunc.OnLogFont(20));
	m_pGridExelSheet.SetItemText(1, 5, _T("-"));
				 
	m_pGridExelSheet.SetItemBkColour(2, 0, RGB(219, 229, 241), BLACK_L);
	m_pGridExelSheet.SetItemFont(2, 0, &clsFunc.OnLogFont(20));
	m_pGridExelSheet.SetItemText(2, 0, _T("No."));
				 
	m_pGridExelSheet.SetItemBkColour(2, 1, RGB(219, 229, 241), BLACK_L);
	m_pGridExelSheet.SetItemFont(2, 1, &clsFunc.OnLogFont(20));
	m_pGridExelSheet.SetItemText(2, 1, _T("Label"));
	
	m_pGridExelSheet.SetItemBkColour(2, 2, RGB(219, 229, 241), BLACK_L);
	m_pGridExelSheet.SetItemFont(2, 2, &clsFunc.OnLogFont(20));
	m_pGridExelSheet.SetItemText(2, 2, _T("I/O"));
	
	m_pGridExelSheet.MergeCells(2, 3, 2, 4);
	m_pGridExelSheet.SetItemBkColour(2, 3, RGB(219, 229, 241), BLACK_L);
	m_pGridExelSheet.SetItemFont(2, 3, &clsFunc.OnLogFont(20));
	m_pGridExelSheet.SetItemText(2, 3, _T("Ststus"));
	
	m_pGridExelSheet.SetItemBkColour(2, 5, RGB(219, 229, 241), BLACK_L);
	m_pGridExelSheet.SetItemFont(2, 5, &clsFunc.OnLogFont(20));
	m_pGridExelSheet.SetItemText(2, 5, _T("Description"));

	for (i=0; i<16; i++) 
	{
		strTmp.Format(_T("%d"), i+1);
		m_pGridExelSheet.SetItemBkColour(i + 3, 0, RGB(79, 79, 225), WHITE_C);
		m_pGridExelSheet.SetItemFont(i + 3, 0, &clsFunc.OnLogFont(20));
		m_pGridExelSheet.SetItemText(i + 3, 0, strTmp);
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void CScreenPrgInfo::OnGridInitPartIOInfo()
{
	//m_pGridExelShit
}

void CScreenPrgInfo::OnGridInitMotorAxisInfo()
{
	//m_pGridExelShit
}

void CScreenPrgInfo::OnGridInitMotorPartInfo()
{
	//m_pGridExelShit
}

void CScreenPrgInfo::OnGridInitWaitTimeInfo()
{
	//m_pGridExelShit
}

void CScreenPrgInfo::OnGridDataUpdate()
{
}


void CScreenPrgInfo::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CFormView::PostNcDestroy();
}
