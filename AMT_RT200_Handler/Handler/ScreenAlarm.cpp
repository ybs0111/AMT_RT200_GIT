// ScreenAlarm.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Handler.h"
#include "ScreenAlarm.h"
#include "Variable.h"
#include "PublicFunction.h"
#include "Dialog_Image_View.h"
#include "DialogAlarmIO.h"
#include "Alg_Excel.h"
#include "DialogAlarmMotor.h"
#include "XgemClient.h"
#include "AlgMemory.h"
#include "FastechPublic_IO.h"
#include "Dialog_Message.h"

// CScreenAlarm
#define	THUMBNAIL_WIDTH		80
#define	THUMBNAIL_HEIGHT	60

IMPLEMENT_DYNCREATE(CScreenAlarm, CFormView)

CScreenAlarm::CScreenAlarm()
	: CFormView(CScreenAlarm::IDD)
{
	m_pImageView		= NULL;
	m_strJamCode		= NULL;

	m_pAlarmIO			= NULL;
	m_pAlarmMotor		= NULL;
}

CScreenAlarm::~CScreenAlarm()
{
}

void CScreenAlarm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMAGE_ALARM,			m_imgAlarm);
	DDX_Control(pDX, IDC_MSG_ALARM_PART,		m_msgAlarmPart);
	DDX_Control(pDX, IDC_MSG_ALARM_PART_DATA,	m_msgAlarmPartData);
	DDX_Control(pDX, IDC_MSG_ALARM_CODE,		m_msgAlarmCode);
	DDX_Control(pDX, IDC_MSG_ALARM_CODE_DATA,	m_msgAlarmCodeData);
	DDX_Control(pDX, IDC_GROUP_PREV_ALARM,		m_groupPrevAlarm);
	DDX_Control(pDX, IDC_GROUP_CODE,			m_groupCode);
	DDX_Control(pDX, IDC_BTN_PREV_ALARM_1,		m_btnPrevAlarm[0]);
	DDX_Control(pDX, IDC_BTN_PREV_ALARM_2,		m_btnPrevAlarm[1]);
	DDX_Control(pDX, IDC_BTN_PREV_ALARM_3,		m_btnPrevAlarm[2]);
	DDX_Control(pDX, IDC_BTN_PREV_ALARM_4,		m_btnPrevAlarm[3]);
	DDX_Control(pDX, IDC_BTN_PREV_ALARM_5,		m_btnPrevAlarm[4]);
	DDX_Control(pDX, IDC_BTN_PREV_ALARM_6,		m_btnPrevAlarm[5]);
	DDX_Control(pDX, IDC_BTN_PREV_ALARM_7,		m_btnPrevAlarm[6]);
	DDX_Control(pDX, IDC_BTN_PREV_ALARM_8,		m_btnPrevAlarm[7]);
	DDX_Control(pDX, IDC_BTN_PREV_ALARM_9,		m_btnPrevAlarm[8]);
	DDX_Control(pDX, IDC_BTN_PREV_ALARM_10,		m_btnPrevAlarm[9]);
	DDX_Control(pDX, IDC_CUSTOM_THUMB,			m_listThumb);
	DDX_Control(pDX, IDC_CUSTOM_REPAIR,			m_pGridRepair);
	DDX_Control(pDX, IDC_CHK_YES,				m_chkYes);
	DDX_Control(pDX, IDC_CHK_NO,				m_chkNo);
	DDX_Control(pDX, IDC_BTN_ALARM_IO,			m_btnAlarmIO);
	DDX_Control(pDX, IDC_BTN_ALARM_MOTOR,		m_btnAlarmMotor);
	DDX_Control(pDX, IDC_BTN_ALARM_CLEAN,		m_btnAlarmClean);
	DDX_Control(pDX, IDC_BTN_ALARM_DOOR,		m_btnDoorOpen);
}

BEGIN_MESSAGE_MAP(CScreenAlarm, CFormView)
	ON_WM_ERASEBKGND()
	ON_REGISTERED_MESSAGE( UWM_ON_TNB_LCLICKED, OnImageClicked )
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BTN_PREV_ALARM_1, &CScreenAlarm::OnBnClickedBtnPrevAlarm1)
	ON_BN_CLICKED(IDC_BTN_PREV_ALARM_10, &CScreenAlarm::OnClickedBtnPrevAlarm10)
	ON_BN_CLICKED(IDC_BTN_PREV_ALARM_2, &CScreenAlarm::OnClickedBtnPrevAlarm2)
	ON_BN_CLICKED(IDC_BTN_PREV_ALARM_3, &CScreenAlarm::OnClickedBtnPrevAlarm3)
	ON_BN_CLICKED(IDC_BTN_PREV_ALARM_4, &CScreenAlarm::OnClickedBtnPrevAlarm4)
	ON_BN_CLICKED(IDC_BTN_PREV_ALARM_5, &CScreenAlarm::OnClickedBtnPrevAlarm5)
	ON_BN_CLICKED(IDC_BTN_PREV_ALARM_6, &CScreenAlarm::OnClickedBtnPrevAlarm6)
	ON_BN_CLICKED(IDC_BTN_PREV_ALARM_7, &CScreenAlarm::OnClickedBtnPrevAlarm7)
	ON_BN_CLICKED(IDC_BTN_PREV_ALARM_8, &CScreenAlarm::OnClickedBtnPrevAlarm8)
	ON_BN_CLICKED(IDC_BTN_PREV_ALARM_9, &CScreenAlarm::OnClickedBtnPrevAlarm9)
	ON_BN_CLICKED(IDC_BTN_ALARM_CLEAN, &CScreenAlarm::OnBnClickedBtnAlarmClean)
	ON_BN_CLICKED(IDC_CHK_NO, &CScreenAlarm::OnClickedChkNo)
	ON_BN_CLICKED(IDC_CHK_YES, &CScreenAlarm::OnClickedChkYes)
	ON_BN_CLICKED(IDC_BTN_ALARM_IO, &CScreenAlarm::OnClickedBtnAlarmIo)
	ON_BN_CLICKED(IDC_BTN_ALARM_MOTOR, &CScreenAlarm::OnClickedBtnAlarmMotor)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_DOOR, &CScreenAlarm::OnBnClickedBtnDoor)
	ON_MESSAGE(WM_WORK_COMMAND,			OnAlarmWorkInfoCommand)
END_MESSAGE_MAP()


// CScreenAlarm �����Դϴ�.

#ifdef _DEBUG
void CScreenAlarm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CScreenAlarm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

BOOL CScreenAlarm::DestroyWindow()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
/*	m_imgAlarm.ClearDrawing();
	m_imgAlarm.Delete();

	if(m_pImageView != NULL)
	{
		delete m_pImageView;
		m_pImageView = NULL;
	}

	if (m_strJamCode != NULL)
	{
		delete[] m_strJamCode;
		m_strJamCode = NULL;
	}

	st_handler_info.cWndAlarm = NULL;*/

	if (m_pAlarmIO != NULL)
	{
		m_pAlarmIO->DestroyWindow();

		delete m_pAlarmIO;
		m_pAlarmIO = NULL;
	}

	if (m_pAlarmMotor != NULL)
	{
		m_pAlarmMotor->DestroyWindow();

		delete m_pAlarmMotor;
		m_pAlarmMotor = NULL;
	}
	return CFormView::DestroyWindow();
}


void CScreenAlarm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	OnInitButton();
	OnInitLabel();
	OnInitGroupBox();
	OnGridRepair();

	//�߻��� �˶��� List�� �ִ´�.

	if (st_alarm_info.strCode != _T(""))
	{
		if (st_alarm_info.nAlarmClear == NO)
		{
			// 20140812 jtkim
			clsXgem.OnMcAlarm(st_alarm_info.strCode, 1);

			st_alarm_info.nAlarmClear = YES;
			st_alarm_info.strListJam.AddTail(st_alarm_info.strCode);
		}
	}
	else if (st_alarm_info.strCode == _T(""))
	{
		m_nLastAlarm = (int)st_alarm_info.strListJam.GetCount();
		
		if(m_nLastAlarm == 0)
		{
			st_alarm_info.strCode = _T("999999");
		}
		else
		{
			st_alarm_info.strCode = st_alarm_info.strListJam.GetAt(st_alarm_info.strListJam.FindIndex(m_nLastAlarm - 1));
			
			if(st_alarm_info.strCode == _T(""))
			{
				st_alarm_info.strCode = _T("999999");
			}
		}
	}

	OnAlarmPrevListSet();			// ���� �˶� ����
	OnAlarmDataLoading();
	OnAlarmInfoDisplay(st_alarm_info.strCode);

	st_handler_info.cWndAlarm = this;
}


BOOL CScreenAlarm::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CDC  memDC;                     // �޸� DC
	CBitmap * pOldBitmap = NULL;    // m_BackBitmap���� ���ο� �׸��� DC�� �׸� ��, ���� �׷����� DC(��, Bitmap)�� ����.
	BITMAP bitmapInfo;              // �׸��� ����(m_BackBitmap)
	st_handler_info.m_BackBitmap.GetBitmap(&bitmapInfo); // Bitmap ũ�� ����.

	memDC.CreateCompatibleDC(pDC);  // �޸� DC ����
	pOldBitmap = memDC.SelectObject(&st_handler_info.m_BackBitmap);
	pDC->BitBlt(0, 0, bitmapInfo.bmWidth, bitmapInfo.bmHeight, &memDC, 0, 0, SRCCOPY);   // �޸� DC�� �׸��� �׸�

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();

	return true; // �� �η����� �ƴ� ����� �׷��� �ϱ� ������ true
}

int CScreenAlarm::OnAlarmDataLoading()
{
	CString strChkData;
	CString strLoadFile;
	CString strAlarmCurMsg;
	CString strTmp;

	int		nPos;  

	if (st_alarm_info.strCode == _T("999999"))
	{
		m_msgAlarmCodeData.SetWindowText(st_alarm_info.strCode);
		m_msgAlarmPartData.SetWindowText(_T("-"));

		strTmp.Format(_T("[Alarm] (%s)_Miss Comparison"), st_alarm_info.strCode);
		clsFunc.OnLogFileAdd(99, strTmp);
	}
	else
	{
		if (st_alarm_info.strCode != _T(""))
		{
			m_msgAlarmCodeData.SetWindowText(st_alarm_info.strCode);
//			m_msgAlarmPartData.SetWindowText(_T("-"));
			m_msgAlarmPartData.SetWindowText(st_alarm_info.strContent);
		}
	}

	if (st_alarm_display_info.strCurrMsg != _T(""))
	{
		strAlarmCurMsg = _T("\r\n");
		strAlarmCurMsg += _T(" ") + st_alarm_display_info.strCurrMsg;
	}
	
//	((CEdit*)GetDlgItem(IDC_EDIT_ALARM_TITLE))->SetWindowText(_T(str_alarm_cur_msg));      // �˶� Ÿ��Ʋ �޽��� ȭ�� ���

	/* ************************************************************************** */
	/* �߻��� �˶��� ���� ��� ���� �ε��Ѵ�                                      */
	/* ************************************************************************** */
	strChkData = OnAlarmDisplayInfoLoad(st_path_info.strPathAlarm, _T("JamData.TXT"), _T("JamAction.txt"), st_alarm_info.strCode);  /* �߻� �˶� ���� ���� �Լ� */
	nPos = strChkData.Find(_T("Trouble"));
	if (nPos >= 0)  
	{
//		((CEdit*)GetDlgItem(IDC_EDIT_ALARM_REPARE))->SetWindowText(str_chk_data);  /* ���� ���� ȭ�� ��� */
//		return FALSE;
	}
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* �߻��� �˶� ǥ���� �׸� ���� ���� �ε��Ѵ�                                 */
	/* ************************************************************************** */
	nPos = st_alarm_display_info.strBmpFile.Find(_T("."));

	if(nPos < 0)
	{
		st_alarm_display_info.strBmpFile = _T("amt.BMP");
	}

	strLoadFile = st_path_info.strPathAlarm + st_alarm_display_info.strBmpFile;  /* ����� �׸� ���Ͽ� ���� [����]+[���ϸ�] ���� */

//	m_image_alarm.ClearDrawing();  // ���� ��� �׸� ���� ���� ����
	if (!m_imgAlarm.LoadImage(strLoadFile))  // �߻��� �˶��� ���� �׸� �ε�
	{
		st_alarm_info.strBmpFile = "amt.BMP";
		strLoadFile = st_path_info.strPathAlarm + st_alarm_display_info.strBmpFile;  /* ����� �׸� ���Ͽ� ���� [����]+[���ϸ�] ���� */

		if (!m_imgAlarm.LoadImage(strLoadFile))  // �߻��� �˶��� ���� �׸� �ε�
		{
			strChkData = _T("Picture File Name : ") + strLoadFile;
			strChkData += _T("\r\n");
			strChkData += _T("Loading Fail");
			
//			((CEdit*)GetDlgItem(IDC_EDIT_ALARM_REPARE))->SetWindowText(str_chk_data);  // ���� ���� ȭ�� ���
			return FALSE;
		}
	}
	
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* �߻��� �˶��� ���� ������ ȭ�鿡 ǥ���Ѵ�                                  */
	/* ************************************************************************** */
	m_imgAlarm.Invalidate();  // ȭ�� ��� ����

	OnAlarmImageStyle(st_path_info.strPathAlarm);  // �߻��� �˶� ���� ǥ�� ��� ���� �Լ�
	/* ************************************************************************** */

	return TRUE;
}

void CScreenAlarm::OnInitGroupBox()
{
	m_groupPrevAlarm.SetFont(clsFunc.OnLogFont(16));
	m_groupPrevAlarm.SetCatptionTextColor(BLUE_C);
	m_groupPrevAlarm.SetBorderColor(ORANGE_C);
	m_groupPrevAlarm.SetFontBold(TRUE);
	m_groupPrevAlarm.SetBackgroundColor(WINDOW_UP);

	m_groupCode.SetFont(clsFunc.OnLogFont(16));
	m_groupCode.SetCatptionTextColor(BLUE_C);
	m_groupCode.SetBorderColor(ORANGE_C);
	m_groupCode.SetFontBold(TRUE);
	m_groupCode.SetBackgroundColor(WINDOW_UP);
}


void CScreenAlarm::OnInitLabel()
{
	m_msgAlarmPart.SetFont(clsFunc.m_pFont[2]);
	m_msgAlarmPart.SetWindowText(_T("Alarm Part"));
	m_msgAlarmPart.SetCenterText();
	m_msgAlarmPart.SetColor(RGB(219, 229, 241));
	m_msgAlarmPart.SetGradientColor(RGB(219, 229, 241));
	m_msgAlarmPart.SetTextColor(BLACK_L);

	m_msgAlarmPartData.SetFont(clsFunc.m_pFont[0]);
	m_msgAlarmPartData.SetWindowText(_T("-"));
	m_msgAlarmPartData.SetCenterText();
	m_msgAlarmPartData.SetColor(BLACK_L);
	m_msgAlarmPartData.SetGradientColor(BLACK_L);
	m_msgAlarmPartData.SetTextColor(YELLOW_C);

	m_msgAlarmCode.SetFont(clsFunc.m_pFont[2]);
	m_msgAlarmCode.SetWindowText(_T("Alarm Code"));
	m_msgAlarmCode.SetCenterText();
	m_msgAlarmCode.SetColor(RGB(219, 229, 241));
	m_msgAlarmCode.SetGradientColor(RGB(219, 229, 241));
	m_msgAlarmCode.SetTextColor(BLACK_L);

	m_msgAlarmCodeData.SetFont(clsFunc.m_pFont[2]);
	m_msgAlarmCodeData.SetWindowText(_T("-"));
	m_msgAlarmCodeData.SetCenterText();
	m_msgAlarmCodeData.SetColor(BLACK_L);
	m_msgAlarmCodeData.SetGradientColor(BLACK_L);
	m_msgAlarmCodeData.SetTextColor(YELLOW_C);
}

/*
130313 Loader Stopper PCB Off Check
130305 Loader Conveyor PCB In Off Check
050308 Loader Elevator Megazine Check ON
050309 Loader Elevator Megazine Check Off
090303 Megazine Transfer Megazine Check On
090304 Megazine Transfer Megazine Check On
080309 Megazine Buffer Megazine Check On
080310 Megazine Buffer Megazine Check Off
100306 PCB Transfer Loader Pcb Check On
100307 PCB Transfer Loader Pcb Check Off
100308 PCB Transfer UnLoader Pcb Check On
100309 PCB Transfer UnLoader Pcb Check Off
160306 UnLoader Conveyor PCB Check On
160307 UnLoader Conveyor PCB Check Off
160310 UnLoader Conveyor PCB(Output) check On
160311 UnLoader Conveyor PCB(Output) check Off
070303 UnLoader Elevator Megazine Check On
0703034UnLoader Elevator Megazine Check Off
*/
void CScreenAlarm::OnInitButton()
{
	int nAlarm;

	m_btnAlarmClean.SetBitmaps(IDC_BTN_ALARM_CLEAN, IDB_BITMAP_CLEAR_UP, WINDOW_UP, IDB_BITMAP_CLEAR_DN, WINDOW_DN);
	m_btnAlarmClean.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnAlarmClean.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnAlarmClean.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnAlarmClean.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnAlarmClean.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnAlarmClean.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);

	m_btnAlarmIO.SetBitmaps(IDC_BTN_ALARM_CLEAN, IDB_BITMAP_IO_UP, WINDOW_UP, IDB_BITMAP_IO_DN, WINDOW_DN);
	m_btnAlarmIO.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnAlarmIO.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnAlarmIO.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnAlarmIO.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnAlarmIO.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnAlarmIO.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnAlarmIO.ShowWindow(SW_HIDE);

	m_btnAlarmMotor.SetBitmaps(IDC_BTN_ALARM_CLEAN, IDB_BITMAP_MOTOR_UP, WINDOW_UP, IDB_BITMAP_MOTOR_DN, WINDOW_DN);
	m_btnAlarmMotor.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnAlarmMotor.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnAlarmMotor.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnAlarmMotor.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnAlarmMotor.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnAlarmMotor.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnAlarmMotor.ShowWindow(SW_HIDE);

	m_btnDoorOpen.SetBitmaps(IDC_BTN_ALARM_DOOR, IDB_BITMAP_DOOR_OPEN, WINDOW_DN, IDB_BITMAP_DOOR_OPEN, WINDOW_UP);
	m_btnDoorOpen.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnDoorOpen.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnDoorOpen.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnDoorOpen.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnDoorOpen.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnDoorOpen.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnDoorOpen.SetFont(clsFunc.m_pFont[3]);
	m_btnDoorOpen.SetTooltipText(_T("Door Open"));

	m_chkYes.SetBitmaps(IDC_CHK_YES, IDB_BITMAP_REMOVE_YES, WINDOW_UP, IDB_BITMAP_REMOVE_YES, WINDOW_DN);
	m_chkYes.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_chkYes.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_chkYes.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_chkYes.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_chkYes.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_chkYes.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_chkYes.ShowWindow(SW_HIDE);

	m_chkNo.SetBitmaps(IDC_CHK_NO, IDB_BITMAP_REMOVE_NO, WINDOW_UP, IDB_BITMAP_REMOVE_NO, WINDOW_DN);
	m_chkNo.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_chkNo.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_chkNo.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_chkNo.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_chkNo.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_chkNo.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_chkNo.ShowWindow(SW_HIDE);

	if (st_alarm_info.strCode != _T(""))
	{
		nAlarm = _wtoi(st_alarm_info.strCode);
	}
	else 
	{
		return ;
	}

/*
	if (st_work_info.nSkipStatus[0] == YES)
	{
		m_chkYes.ShowWindow(SW_SHOW);
		m_chkYes.SetCheck(FALSE);

		m_chkNo.EnableWindow(TRUE);
		m_chkNo.ShowWindow(SW_SHOW);

		switch(nAlarm)
		{
			case 50308:
				m_chkYes.SetWindowText(_T("Ld Elevator Megazine Yes"));
				m_chkNo.SetWindowText(_T("Ld Elevator Megazine No"));
				break;

			case 50309:
				m_chkYes.SetWindowText(_T("Ld Elevator Megazine No"));
				m_chkNo.SetWindowText(_T("Ld Elevator Megazine Yes"));
				break;

			case 70303:
				m_chkYes.SetWindowText(_T("UnLd Elevator Megazine Yes"));
				m_chkNo.SetWindowText(_T("UnLd Elevator Megazine No"));
				break;

			case 70304:
				m_chkYes.SetWindowText(_T("UnLd Elevator Megazine No"));
				m_chkNo.SetWindowText(_T("UnLd Elevator Megazine Yes"));
				break;

			case 80309:
				m_chkYes.SetWindowText(_T("Megazine Buffer Megazine Yes"));
				m_chkNo.SetWindowText(_T("Megazine Buffer Megazine No"));
				break;

			case 80310:
				m_chkYes.SetWindowText(_T("Megazine Buffer Megazine No"));
				m_chkNo.SetWindowText(_T("Megazine Buffer Megazine Yes"));
				break;

			case 90303:
				m_chkYes.SetWindowText(_T("Megazine Transfer Megazine Yes"));
				m_chkNo.SetWindowText(_T("Megazine Transfer Megazine No"));
				break;

			case 90304:
				m_chkYes.SetWindowText(_T("Megazine Transfer Megazine No"));
				m_chkNo.SetWindowText(_T("Megazine Transfer Megazine Yes"));
				break;

			case 100306:
				m_chkYes.SetWindowText(_T("Ld Picker PCB Yes"));
				m_chkNo.SetWindowText(_T("Ld Picker PCB No"));
				break;

			case 100307:
				m_chkYes.SetWindowText(_T("Ld Picker PCB No"));
				m_chkNo.SetWindowText(_T("Ld Picker PCB Yes"));
				break;

			case 100308:
				m_chkYes.SetWindowText(_T("UnLd Picker PCB Yes"));
				m_chkNo.SetWindowText(_T("UnLd Picker PCB No"));
				break;

			case 100309:
				m_chkYes.SetWindowText(_T("UnLd Picker PCB No"));
				m_chkNo.SetWindowText(_T("UnLd Picker PCB Yes"));
				break;

			case 130305:
				m_chkYes.SetWindowText(_T("Ld Conveyor PCB In Yes"));
				m_chkNo.SetWindowText(_T("Ld Conveyor PCB In No"));
				break;

			case 130313:
				m_chkYes.SetWindowText(_T("Ld Stopper PCB No"));
				m_chkNo.SetWindowText(_T("Ld Stopper PCB Yes"));
				break;

			case 160306:
				m_chkYes.SetWindowText(_T("UnLd Conveyor PCB Yes"));
				m_chkNo.SetWindowText(_T("UnLd Conveyor PCB No"));
				break;

			case 160307:
				m_chkYes.SetWindowText(_T("UnLd Conveyor PCB No"));
				m_chkNo.SetWindowText(_T("UnLd Conveyor PCB Yes"));
				break;

			case 160310:
				m_chkYes.SetWindowText(_T("UnLd Conveyor PCB Out Yes"));
				m_chkNo.SetWindowText(_T("UnLd Conveyor PCB Out No"));
				break;

			case 160311:
				m_chkYes.SetWindowText(_T("UnLd Conveyor PCB Out No"));
				m_chkNo.SetWindowText(_T("UnLd Conveyor PCB Out Yes"));
				break;
		}
	}
	else
	{
		m_chkYes.SetWindowText(_T("-"));
		m_chkYes.ShowWindow(SW_HIDE);

		m_chkNo.SetWindowText(_T("-"));
		m_chkNo.ShowWindow(SW_HIDE);
	}
*/
	UpdateData(FALSE);
}

void CScreenAlarm::OnAlarmImageStyle(CString strPath)
{
	if (st_alarm_display_info.strStyle == "NONE")
	{
		return;
	}
	else if (st_alarm_display_info.strStyle == "CIRCLE") 
	{
		m_imgAlarm.SetCircleStyle(st_alarm_display_info.nCx, st_alarm_display_info.nCy, st_alarm_display_info.nOffset);
		m_imgAlarm.SetLineStyle(st_alarm_display_info.nLineWidth, RGB(st_alarm_display_info.nR, st_alarm_display_info.nG, st_alarm_display_info.nB));
		return;
	}
	else if (st_alarm_display_info.strStyle == "ARROW_LEFT")
	{
		m_imgAlarm.SetArrowStyle(st_alarm_display_info.nCx, st_alarm_display_info.nCy, CBmpImageST::JS_ARROW_LEFT, strPath, st_alarm_display_info.nOffset);
		return;
	}
	else if (st_alarm_display_info.strStyle == "ARROW_RIGHT")
	{
		m_imgAlarm.SetArrowStyle(st_alarm_display_info.nCx, st_alarm_display_info.nCy, CBmpImageST::JS_ARROW_RIGHT, strPath, st_alarm_display_info.nOffset);
		return;
	}
	else if (st_alarm_display_info.strStyle == "ARROW_UP") 
	{
		m_imgAlarm.SetArrowStyle(st_alarm_display_info.nCx, st_alarm_display_info.nCy, CBmpImageST::JS_ARROW_UP, strPath, st_alarm_display_info.nOffset);
		return;
	}
	else if (st_alarm_display_info.strStyle == "ARROW_DOWN")
	{
		m_imgAlarm.SetArrowStyle(st_alarm_display_info.nCx, st_alarm_display_info.nCy, CBmpImageST::JS_ARROW_DOWN, strPath, st_alarm_display_info.nOffset);
		return;
	}
	else if (st_alarm_display_info.strStyle == "ARROW_LEFTUP")
	{
		m_imgAlarm.SetArrowStyle(st_alarm_display_info.nCx, st_alarm_display_info.nCy, CBmpImageST::JS_ARROW_LEFTUP, strPath, st_alarm_display_info.nOffset);
		return;
	}
	else if (st_alarm_display_info.strStyle == "ARROW_LEFTDOWN")
	{
		m_imgAlarm.SetArrowStyle(st_alarm_display_info.nCx, st_alarm_display_info.nCy, CBmpImageST::JS_ARROW_LEFTDOWN, strPath, st_alarm_display_info.nOffset);
		return;
	}
	else if (st_alarm_display_info.strStyle == "ARROW_RIGHTUP")   
	{
		m_imgAlarm.SetArrowStyle(st_alarm_display_info.nCx, st_alarm_display_info.nCy, CBmpImageST::JS_ARROW_RIGHTUP, strPath, st_alarm_display_info.nOffset);
		return;
	}
	else if (st_alarm_display_info.strStyle == "ARROW_RIGHTDOWN") 
	{
		m_imgAlarm.SetArrowStyle(st_alarm_display_info.nCx, st_alarm_display_info.nCy, CBmpImageST::JS_ARROW_RIGHTDOWN, strPath, st_alarm_display_info.nOffset);
		return;
	}
}

// ******************************************************************************
// �߻� �˶� ���� ���� �Լ�                                                      
// -> strPath     : �˶� ���� ���� ���� ���� ����                                
// -> strDataFile : �˶� ���� ����� File Name(File Name+Ȯ����)                       
// -> strCode     : �߻��� �˶� Code                                             
// ******************************************************************************
CString CScreenAlarm::OnAlarmDisplayInfoLoad(CString strPath, CString strDataFile, CString strActionFile, CString strCode)
{
	CString strFilePath;  // ���� ���� ����
	CString strAlarmChk;  // �Լ� ���� ���� ���� ����
	int nPos;

	strFilePath = strPath;
	if (strFilePath.Right(1) != _T("\\"))  // ���� ��� ������ ���� �˻�
	{
		strFilePath += _T("\\");
	}

	strAlarmChk = OnAlarmRepairDataLoad(strFilePath, strActionFile, strCode);  // �߻��� �˶��� ���� ��ġ ���� ���� �Լ�
	nPos = strAlarmChk.Find(_T("Trouble"));
	if (nPos >= 0)  // ���� �߻�
	{
		return strAlarmChk;
	}

	strAlarmChk = OnAlarmBmpDataLoad(strFilePath, strDataFile, strCode);  // �߻� �˶� ���� ǥ�ÿ� �׸� File Name ���� �Լ�
	nPos = strAlarmChk.Find(_T("Trouble"));
	if (nPos >= 0)  // ���� �߻�
	{
		return strAlarmChk;
	}

	strAlarmChk = OnAlarmStyleDataLoad(strFilePath, strDataFile, strCode);  // �˶� �߻� ���� ǥ�� ���� �� ��� ��ġ ���� �Լ�
	nPos = strAlarmChk.Find(_T("Trouble"));
	if (nPos >= 0)  // ���� �߻�
	{
		return strAlarmChk;
	}

	OnAlarmIODataLoad(strFilePath, _T("JamIO.TXT"), strCode);  // �߻��� �˶��� ���� ��ġ ���� ���� �Լ�
	OnAlarmMotorDataLoad(strFilePath, _T("JamMotor.TXT"), strCode);  // �߻��� �˶��� ���� ��ġ ���� ���� �Լ�
	OnAlarmImageDataLoad(strFilePath, _T("JamImage.TXT"), strCode);  // �߻��� �˶��� ���� ��ġ ���� ���� �Լ�
	return strAlarmChk;
}

// ******************************************************************************
// �߻� �˶� ���� ǥ�ÿ� �׸� File Name ���� �Լ�                                
// -> strPath     : �˶� ǥ�� �׸� ���� ���� ����                                
// -> strDataFile : �˶� ǥ�� �׸� File Name(File Name+Ȯ����)                   
// -> strCode     : �߻��� �˶� Code                                             
// ******************************************************************************
CString CScreenAlarm::OnAlarmBmpDataLoad(CString strPath, CString strDataFile, CString strCode)
{
	CString strReturnData;  // �Լ� ���� �÷���
	CString strFile;  // �˶� ǥ�ÿ� �׸��� ���� [����]+[File Name] ���� ����
	CString strTemp;  // �˶� ǥ�ÿ� �׸� File Name �ӽ� ���� ����

	TCHAR chrBuf[1000];

	strReturnData = _T("OK");  // �Լ� ���� �÷��� �ʱ�ȭ

	strFile = strPath + strDataFile;  // [����]+[File Name] ����

	if (!GetPrivateProfileString(strCode, _T("DEF"), _T(""), chrBuf, 1000, strFile))
	{
		strReturnData = _T("File Name : ") + strFile;
		strReturnData += _T("\r\n");
		strReturnData += _T("[DEF] The information we read. Trouble -> Code : ");
		strReturnData += strCode;

		return strReturnData;
	}

	strTemp.Format(_T("%s"), chrBuf);
	st_alarm_display_info.strBmpFile = strTemp;  // �˶� ǥ�ÿ� �׷� File Name ����*/

	return strReturnData;
}
// ******************************************************************************

// ******************************************************************************
// �˶� �߻� ���� ǥ�� ���� �� ��� ��ġ ���� �Լ�                               
// -> strPath     : ���� ǥ�� ���� ���� ���� ����                                
// -> strDataFile : ���� ǥ�� ���� ���� File Name(File Name+Ȯ����)              
// -> strCode     : �߻��� �˶� Code                                             
// ******************************************************************************
CString CScreenAlarm::OnAlarmStyleDataLoad(CString strPath, CString strDataFile, CString strCode)
{
	CString strChkData;		// �Լ� ���� �÷���
	CString strChkFile;		// �˶� ���� ǥ�� ������ ���� [����]+[File Name] ���� ����
	CString strChk, strChk2;  // �˶� ���� ǥ�� ���� �� ��ġ ���� ����
	int start=0, end, i;		// ���� ������ ��ġ ���� ����
	int max=4;					// ���� ������ �ִ� ���� (ȭ��ǥ ������� �ʱ�ȭ)

	TCHAR chrBuf[1000];

	strChkFile = _T("OK");  // �Լ� ���� �÷��� �ʱ�ȭ

	strChkFile = strPath + strDataFile;  // [����]+[File Name] ����

	if (!GetPrivateProfileString(strCode, _T("STYLE"), _T(""), chrBuf, 1000, strChkFile))
	{
		strChkData = _T("File Name : ") + strChkFile;
		strChkData += _T("\r\n");
		strChkData += _T("[STYLE] The information we read. Trouble -> Code : ");
		strChkData += strCode;

		return strChkData;
	}

	strChk.Format(_T("%s"), chrBuf);

	for(i=0; i<max; i++)
	{
		end = strChk.Find(_T(";"), start);
		if (end == -1)  // ������ ������
		{
			break;
		}
		else            // ������ ����
		{
			strChk2 = strChk.Mid(start, end-start);  // ������ ������ �� �� ������ ����
			strChk2.TrimLeft(' ');
			strChk2.TrimRight(' ');

			if (i == 0)  // �˶� ���� ǥ�� ����
			{
				st_alarm_display_info.strStyle = strChk2;       // �˶� ���� ǥ�� ���� ����
				if (st_alarm_display_info.strStyle == _T("CIRCLE"))  // ���� ǥ�� ���� 8�� ����
				{
					max = 8;    
				}
				else if (st_alarm_display_info.strStyle == _T("NONE"))
				{
					max = 1;
				}
			}
			else
			{
				if (i == 1)
				{
					st_alarm_display_info.nCx = _wtoi(strChk2);         // X�� �ȼ� ��ġ
				}
				else if (i == 2)
				{
					st_alarm_display_info.nCy = _wtoi(strChk2);         // Y�� �ȼ� ��ġ
				}
				else if (i == 3)
				{
					st_alarm_display_info.nOffset = _wtoi(strChk2);     // �� ũ��
				}
				else if (i == 4)
				{
					st_alarm_display_info.nLineWidth = _wtoi(strChk2);  // ���� �β�
				}
				else if (i == 5)
				{
					st_alarm_display_info.nR = _wtoi(strChk2);          // RGB �� R ��
				}
				else if (i == 6)
				{
					st_alarm_display_info.nG = _wtoi(strChk2);          // RGB �� G ��
				}
				else if (i == 7) 
				{
					st_alarm_display_info.nB = _wtoi(strChk2);          // RGB �� B ��
				}
				// **************************************************************
			}

			start = end + 1;  // ���� ������ ��ġ ����
		}
	}

	if (i != max)  // Data Format ����
	{
		strChkData = _T("File Name : ") + strChkFile;
		strChkData += _T("\r\n");
		strChkData += _T("[STYLE] Data Format. Trouble -> Code : ");
		strChkData += strCode;

		return strChkData;
	}

	return strChkData;
}
// ******************************************************************************

// ******************************************************************************
// �߻��� �˶��� ���� ��ġ ���� ���� �Լ�                                        
// -> strPath       : �˶� ��ġ ���� ����� ���� ����                            
// -> strActionFile : �˶� ��ġ ���� ����� File Name(File Name+Ȯ����)          
// -> strCode       : �߻��� �˶� Code                                           
// ******************************************************************************
CString CScreenAlarm::OnAlarmRepairDataLoad(CString strPath, CString strActionFile, CString strCode)
{
	CString strReturnVal;		// �Լ� ���� �÷���
	CString strRepairFile;	// ��ġ ���׿� ���� [����]+[File Name] ���� ����
	CString strActItem;		// ��ġ ���� ���� ��ġ ���� ����
	CString strActTemp;		// ��ġ ���� �ӽ� ���� ����
	int nChkCnt;				// ��ġ ���� ���� ���� ����
	int i; 
	
	TCHAR chrBuf[1000];

	strReturnVal = _T("OK");  // �Լ� ���� �÷��� �ʱ�ȭ

	strRepairFile = strPath + strActionFile;  // [����]+[File Name] ����

	if (!GetPrivateProfileString(strCode, _T("COUNT"), _T("0"), chrBuf, 10, strRepairFile))
	{
		strReturnVal = _T("File Name : ") + strRepairFile;
		strReturnVal += _T("\r\n");
		strReturnVal += _T("[COUNT] The information we read. Trouble -> Code : ");
		strReturnVal += strCode;

		return strReturnVal;
	}

	nChkCnt = _wtoi(chrBuf);  // ���� ��ġ ���� ���� ����
	if (nChkCnt<1)
	{
		strReturnVal = _T("File Name : ") + strRepairFile;
		strReturnVal += _T("\r\n");
		strReturnVal += _T("[COUNT] Data Format Trouble -> Code : ");
		strReturnVal += strCode;

		return strReturnVal;
	}
	else
	{
		st_alarm_display_info.nActionCnt = nChkCnt;
	}

	(st_alarm_display_info.strRepairMsg).Empty();  //  ��ġ ���� ���� ���� �ʱ�ȭ

	for (i=0; i<st_alarm_display_info.nActionCnt; i++)   
	{
		strActItem.Format(_T("%d"), i+1);  // ��ġ ���� ���� ���� ��ġ ����
		if (!GetPrivateProfileString(strCode, (LPCTSTR)strActItem, _T(""), chrBuf, 1000, strRepairFile))
		{
			strReturnVal = _T("File Name : ")+ strRepairFile;
			strReturnVal += _T("\r\n");
			strReturnVal += _T("An action subject. The information we read. Trouble");

			strReturnVal += _T("\r\n");
			strReturnVal += _T("Code : ");
			strReturnVal += strCode;

			strReturnVal += _T("\r\n");
			strReturnVal += _T("Position : ");
			strReturnVal += strActItem;

			return strReturnVal;
		}
		// **********************************************************************

		st_alarm_display_info.strRepairMsg += _T("\r\n");			//��ġ ���׿� �� ù���� ����.

		strActItem.Format(_T(" %s"), chrBuf);  // ��ġ ���� ����
		st_alarm_display_info.strRepairMsg += strActItem;

		OnGridRepairData(i, strActItem);

		if (i != ((st_alarm_display_info.nActionCnt)-1))  
		{
			st_alarm_display_info.strRepairMsg += _T("\r\n");  // ��ġ ���� �ܶ� ������ ����
		}
	}
	// **************************************************************************
	
	return strReturnVal;
}

void CScreenAlarm::OnAlarmIODataLoad(CString strPath, CString strIOFile, CString strCode)
{
	CString strChkData;		// �Լ� ���� �÷���
	CString strChkFile;		// �˶� ���� ǥ�� ������ ���� [����]+[File Name] ���� ����
	CString strChk, strChk2;  // �˶� ���� ǥ�� ���� �� ��ġ ���� ����
	int nPos[2];
	int nLength;
	int i;
	TCHAR chrBuf[1000];

	strChkFile = strPath + strIOFile;  // [����]+[File Name] ����

	m_nInCnt	= 0;
	m_nOutCnt	= 0;

	if (!GetPrivateProfileString(strCode, _T("IN"), _T(""), chrBuf, 1000, strChkFile))
	{
		return;
	}

	strChkData.Format(_T("%s"), chrBuf);
	if (strChkData == _T("NO"))
	{
		return;
	}

	nLength = strChkData.GetLength();
	nPos[0] = 0;

	for (i=0; i<nLength; i++)
	{
		nPos[1] = strChkData.Find(_T(";"), nPos[0]);
		if (nPos[1] < 0) break;
		strChk  = strChkData.Mid(nPos[0], nPos[1] - nPos[0]);

		m_nIn[m_nInCnt] = _wtoi(strChk);
		m_nInCnt++;
		nPos[0] = nPos[1] + 1;
	}

	if (!GetPrivateProfileString(strCode, _T("OUT"), _T(""), chrBuf, 1000, strChkFile))
	{
		if (m_nInCnt > 0)
		{
			m_btnAlarmIO.ShowWindow(SW_SHOW);
		}
		return;
	}

	strChkData.Format(_T("%s"), chrBuf);
	if (strChkData == _T("NO"))
	{
		if (m_nInCnt > 0)
		{
			m_btnAlarmIO.ShowWindow(SW_SHOW);
		}
		return;
	}

	nLength = strChkData.GetLength();
	nPos[0] = 0;

	for (i=0; i<nLength; i++)
	{
		nPos[1] = strChkData.Find(_T(";"), nPos[0]);
		if (nPos[1] < 0) break;
		strChk  = strChkData.Mid(nPos[0], nPos[1] - nPos[0]);

		m_nOut[m_nOutCnt] = _wtoi(strChk);
		m_nOutCnt++;
		nPos[0] = nPos[1] + 1;
	}

	if (m_nInCnt > 0 || m_nOutCnt > 0)
	{
		m_btnAlarmIO.ShowWindow(SW_SHOW);
	}
}


void CScreenAlarm::OnAlarmMotorDataLoad(CString strPath, CString strMotorFile, CString strCode)
{
	CString strChkData;		// �Լ� ���� �÷���
	CString strChkFile;		// �˶� ���� ǥ�� ������ ���� [����]+[File Name] ���� ����
	CString strChk, strChk2;  // �˶� ���� ǥ�� ���� �� ��ġ ���� ����
	int nPos[2];
	int nLength;
	int i;
	TCHAR chrBuf[1000];

	strChkFile = strPath + strMotorFile;  // [����]+[File Name] ����

	m_nMotorCnt = -1;

	if (!GetPrivateProfileString(strCode, _T("MOTOR"), _T(""), chrBuf, 1000, strChkFile))
	{
		return;
	}

	strChkData.Format(_T("%s"), chrBuf);
	if (strChkData == _T("NO"))
	{
		return;
	}

	m_nMotorCnt = 0;
	nLength = strChkData.GetLength();
	nPos[0] = 0;

	for (i=0; i<nLength; i++)
	{
		nPos[1] = strChkData.Find(_T(";"), nPos[0]);
		if (nPos[1] < 0) break;
		strChk  = strChkData.Mid(nPos[0], nPos[1] - nPos[0]);

		m_nMotor = _wtoi(strChk);
		nPos[0] = nPos[1] + 1;
	}

	m_btnAlarmMotor.ShowWindow(SW_SHOW);
}


void CScreenAlarm::OnAlarmImageDataLoad(CString strPath, CString strImageFile, CString strCode)
{
	CString strChkData;		// �Լ� ���� �÷���
	CString strChkFile;		// �˶� ���� ǥ�� ������ ���� [����]+[File Name] ���� ����
	CString strChk, strChk2;  // �˶� ���� ǥ�� ���� �� ��ġ ���� ����

	TCHAR chrBuf[1000];

	strChkFile = strPath + strImageFile;  // [����]+[File Name] ����

	if (!GetPrivateProfileString(strCode, _T("IMAGE"), _T(""), chrBuf, 1000, strChkFile))
	{
		return;
	}

	strChkData.Format(_T("%s"), chrBuf);
	if (strChkData != _T("NO"))
	{
		OnImageThumbnail(strChkData);
	}
}


void CScreenAlarm::OnGridRepair()
{
	m_pGridRepair.SetFrameFocusCell(FALSE);
	m_pGridRepair.SetTrackFocusCell(FALSE);
	m_pGridRepair.EnableSelection(FALSE);

	m_pGridRepair.SetGridLineColor(BLACK_C);
	m_pGridRepair.SetGridLines(1);

	m_pGridRepair.SetRowCount(0);
	m_pGridRepair.SetColumnCount(0);

	m_pGridRepair.SetFixedRowCount(0);
	m_pGridRepair.SetFixedColumnCount(0);
}

void CScreenAlarm::OnGridRepairData(int nCount, CString strRepair)
{
	CString strTmp;

	m_pGridRepair.SetRowCount(nCount + 1); 
	m_pGridRepair.SetColumnCount(1); 

	m_pGridRepair.SetRowHeight(nCount, 25);
	m_pGridRepair.SetColumnWidth(0, 500);

	m_pGridRepair.SetItemFormat(nCount, 0, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	m_pGridRepair.SetItemState(nCount, 0, GVIS_READONLY);
	m_pGridRepair.SetItemBkColour(nCount, 0, WHITE_C, CLR_DEFAULT);

	m_pGridRepair.SetItemBkColour(nCount, 0, RED_L, BLACK_C);
	
	m_pGridRepair.SetItemFont(nCount, 0, &clsFunc.OnLogFont(14));

	strTmp.Format(_T("[%d] : %s"), nCount, strRepair);
	m_pGridRepair.SetItemText(nCount, 0, strTmp);
}


void CScreenAlarm::OnImageThumbnail(CString strPath)
{
 	HANDLE hFind;
    WIN32_FIND_DATA fd;
	CString strFileName;

	m_listThumb.InitializeVariables( 170, 120 );

	strFileName		= st_path_info.strPathAlarm + strPath;
	strFileName		+= _T("\\");
	strFileName		+= "*.*";
   
    if ((hFind = ::FindFirstFile ((LPCTSTR) strFileName, &fd)) != INVALID_HANDLE_VALUE) 
	{
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
		{
			strFileName		= st_path_info.strPathAlarm + strPath;
			strFileName		+= _T("\\");
			strFileName		+= fd.cFileName;
			m_listThumb.Add(strFileName);
        }

        while (::FindNextFile (hFind, &fd)) 
		{
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				strFileName = st_path_info.strPathAlarm + strPath;
				strFileName += _T("\\");
				strFileName += fd.cFileName;
				m_listThumb.Add(strFileName);
			}
        }
		::FindClose(hFind);
    }
	m_listThumb.SetRedraw();
	m_listThumb.Invalidate();
}


LRESULT CScreenAlarm::OnImageClicked( WPARAM wParam, LPARAM  lParam)
{
	CThumbnailButton *pBtn = (CThumbnailButton*) wParam;
	CString m_strMsg;

	if(lParam == L_BUTTON_DOWN)
	{
		if (::IsWindow(pBtn->m_hWnd))
		{
			m_strMsg = pBtn->GetFullpath();

			if(m_pImageView != NULL)
			{
				delete m_pImageView;
				m_pImageView = NULL;
			}

			m_pImageView = new CDialog_Image_View;
			m_pImageView->m_strFileName = m_strMsg;
			m_pImageView->Create();
			m_pImageView->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		if(m_pImageView != NULL)
		{
			delete m_pImageView;
			m_pImageView = NULL;
		}
	}

  return 1;
}


void CScreenAlarm::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if(m_pImageView != NULL)
	{
		delete m_pImageView;
		m_pImageView = NULL;
	}
	CFormView::OnLButtonUp(nFlags, point);
}


void CScreenAlarm::OnBnClickedBtnPrevAlarm1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_listThumb.DeleteAll();
	OnAlarmPrevAlarmDisplay(1);
}

void CScreenAlarm::OnAlarmPrevButtonListSet(int nCount)
{
	int i;
	if (nCount > 10)	nCount = 10;

	for(i=0; i<10; i++)
	{
		m_btnPrevAlarm[i].ShowWindow(FALSE);
	}

	for(i=0; i<nCount; i++)
	{
		m_btnPrevAlarm[i].ShowWindow(TRUE);
	}
}


void CScreenAlarm::OnAlarmPrevListSet()
{
	int i, nCnt;
	
	nCnt = -1;
	nCnt = (int)st_alarm_info.strListJam.GetCount();
	
	if (nCnt == 0) return;

	OnAlarmPrevButtonListSet(nCnt);

	if (m_strJamCode != NULL)
	{
		delete[] m_strJamCode;
		m_strJamCode = NULL;
	}
	
	m_strJamCode = new CString [nCnt];
	
	for (i = 0; i < nCnt; i++)
	{
		m_strJamCode[i] = st_alarm_info.strListJam.GetAt(st_alarm_info.strListJam.FindIndex(i));
	}

	OnAlarmPrevListCaptionSet(nCnt);
}

void CScreenAlarm::OnAlarmPrevListCaptionSet(int nCnt)
{
	if (nCnt < 1)		return;
	if (m_strJamCode[nCnt - 1] != "")	m_btnPrevAlarm[0].SetWindowText(m_strJamCode[nCnt - 1]);

	if (nCnt < 2)		return;
	if (m_strJamCode[nCnt - 2] != "")	m_btnPrevAlarm[1].SetWindowText(m_strJamCode[nCnt - 2]);

	if (nCnt < 3)		return;
	if (m_strJamCode[nCnt - 3] != "")	m_btnPrevAlarm[2].SetWindowText(m_strJamCode[nCnt - 3]);

	if (nCnt < 4)		return;
	if (m_strJamCode[nCnt - 4] != "")	m_btnPrevAlarm[3].SetWindowText(m_strJamCode[nCnt - 4]);

	if (nCnt < 5)		return;
	if (m_strJamCode[nCnt - 5] != "")	m_btnPrevAlarm[4].SetWindowText(m_strJamCode[nCnt - 5]);

	if (nCnt < 6)		return;
	if (m_strJamCode[nCnt - 6] != "")	m_btnPrevAlarm[5].SetWindowText(m_strJamCode[nCnt - 6]);

	if (nCnt < 7)		return;
	if (m_strJamCode[nCnt - 7] != "")	m_btnPrevAlarm[6].SetWindowText(m_strJamCode[nCnt - 7]);

	if (nCnt < 8)		return;
	if (m_strJamCode[nCnt - 8] != "")	m_btnPrevAlarm[7].SetWindowText(m_strJamCode[nCnt - 8]);

	if (nCnt < 9)		return;
	if (m_strJamCode[nCnt - 9] != "")	m_btnPrevAlarm[8].SetWindowText(m_strJamCode[nCnt - 9]);

	if (nCnt < 10)		return;
	if (m_strJamCode[nCnt - 10] != "")	m_btnPrevAlarm[9].SetWindowText(m_strJamCode[nCnt - 10]);
}

void CScreenAlarm::OnClickedBtnPrevAlarm10()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_listThumb.DeleteAll();
	OnAlarmPrevAlarmDisplay(10);
}


void CScreenAlarm::OnClickedBtnPrevAlarm2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_listThumb.DeleteAll();
	OnAlarmPrevAlarmDisplay(2);
}


void CScreenAlarm::OnClickedBtnPrevAlarm3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_listThumb.DeleteAll();
	OnAlarmPrevAlarmDisplay(3);
}


void CScreenAlarm::OnClickedBtnPrevAlarm4()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_listThumb.DeleteAll();
	OnAlarmPrevAlarmDisplay(4);
}


void CScreenAlarm::OnClickedBtnPrevAlarm5()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_listThumb.DeleteAll();
	OnAlarmPrevAlarmDisplay(5);
}


void CScreenAlarm::OnClickedBtnPrevAlarm6()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_listThumb.DeleteAll();
	OnAlarmPrevAlarmDisplay(6);
}


void CScreenAlarm::OnClickedBtnPrevAlarm7()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_listThumb.DeleteAll();
	OnAlarmPrevAlarmDisplay(7);
}


void CScreenAlarm::OnClickedBtnPrevAlarm8()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_listThumb.DeleteAll();
	OnAlarmPrevAlarmDisplay(8);
}


void CScreenAlarm::OnClickedBtnPrevAlarm9()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_listThumb.DeleteAll();
	OnAlarmPrevAlarmDisplay(9);
}


CString CScreenAlarm::OnAlarmPrevAlarmCheck(int nCnt)
{
	int i, cnt = (int)st_alarm_info.strListJam.GetCount();
	
	if (m_strJamCode != NULL)
	{
		delete[] m_strJamCode;
		m_strJamCode = NULL;
	}
	
	if (cnt > 0) m_strJamCode = new CString [cnt];
	
	for (i = 0; i < cnt; i++)
	{
		m_strJamCode[i] = st_alarm_info.strListJam.GetAt(st_alarm_info.strListJam.FindIndex(i));
	}

	if (cnt < nCnt) return _T("999999");		//2015.04.24 sonjs

	return m_strJamCode[cnt - nCnt];
}


void CScreenAlarm::OnAlarmPrevAlarmDisplay(int nAlarmPos)
{
	st_alarm_info.strCode = OnAlarmPrevAlarmCheck(nAlarmPos);

	m_imgAlarm.ClearDrawing();												// ���� ��� �׸� ���� ���� ����
	

	/* ************************************************************************** */
	/* ���� �߻��� �˶��� ���� �޽��� ���� �����Ѵ�                               */
	/* ************************************************************************** */
//	mcls_alarm_load.On_Alarm_Info_Set_to_Variable(alarm.str_code);		// �߻��� �˶� �޽��� ���� ������ �����ϴ� �Լ�
	/* ************************************************************************** */

	OnAlarmDataLoading();												// ���� �߻��� �˶��� ���� �׸� �� ��ġ ���� ���� �ε� �Լ�
}


void CScreenAlarm::OnBnClickedBtnAlarmClean()
{
	// Alarm Clean Btn Click �� 
	// ��� ���¸� STOP���� �����Ѵ�. 
	int nAlarm;
	CString strTemp;

	if (st_handler_info.nRunStatus != dSTOP)
	{
		clsFunc.OnSound(IO_OFF);

		st_handler_info.nRunStatus = dSTOP;
	}

	if (st_alarm_info.strCode != _T(""))
	{
		nAlarm = _wtoi(st_alarm_info.strCode);
	}

	st_work_info.nRemoveYesNo[0]	= m_nRemoveYesNo[0];
	// Alarm Reset Ŭ�� �� Reset ���º��� ���� < RUN S/W Ŭ���� Ȯ�εǴ� �����̴�. >
	st_alarm_info.nResetStatus = NO;

	if (st_work_info.nRemoveInfo[0] == YES)
	{
		strTemp.Format(_T("CODE : %s METHOT :%d"), st_alarm_info.strCode, st_work_info.nRemoveYesNo[0]);

		if (st_handler_info.cWndList != NULL)
		{
			//clsFunc.OnStringToChar(strTemp, st_other_info.cNormalMsg);
			clsMem.OnNormalMessageWrite(strTemp);
			st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}
	}
	// Alarm Reset Ŭ�� �� ȭ���� �������� ����ǹǷ� FALSE�� ��������. < RUN S/W Ŭ���� Ȯ�εǴ� �����̴�. >
	st_handler_info.nAlarmScreen = FALSE;

	// ��� ȭ���� ���� ȭ������ �̵��Ѵ�. 
	::PostMessage(st_handler_info.hWnd, WM_FORM_CHANGE, SCREEN_MAIN, 0);  
}


void CScreenAlarm::OnClickedChkNo()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	if (m_chkYes.GetCheck())
	{
		m_chkYes.SetCheck(FALSE);
	}

	if (!m_chkNo.GetCheck())
	{
		m_chkYes.SetCheck(TRUE);
	}

	m_nRemoveYesNo[0] = NO;

	UpdateData(FALSE);
}


void CScreenAlarm::OnClickedChkYes()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	if (m_chkNo.GetCheck())
	{
		m_chkNo.SetCheck(FALSE);
	}

	if (!m_chkYes.GetCheck())
	{
		m_chkNo.SetCheck(TRUE);
	}

	m_nRemoveYesNo[0] = YES;

	UpdateData(FALSE);
}


void CScreenAlarm::OnClickedBtnAlarmIo()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	/*CDialogAlarmIO		dlgIO;
	CString				strTemp;
	int					i;
	int					nBoard, nPort, nSlave, nBit;
	
//	m_strIOBitDescrip[i][j][k][l]
	dlgIO.m_nInCnt		= m_nInCnt;
	dlgIO.m_nOutCnt		= m_nOutCnt;

	// jtkim 20160111
	st_handler_info.nDlgLock = TRUE;

	if (m_nInCnt > 0 && m_nInCnt < 16)
	{
//		dlgIO.m_nInCnt		= m_nInCnt;
		for (i=0; i<m_nInCnt; i++)
		{
			if (m_nIn[i] >= 0)
			{
				nBoard			= m_nIn[i] / 4000;
				nPort			= (m_nIn[i] % 4000) / 1000;
				nSlave			= ((m_nIn[i] % 4000) % 1000) / 100;
				nBit			= (((m_nIn[i] % 4000) % 1000) % 100) % 16;

				if (nBoard < 0 || nPort < 0 || nSlave < 0 || nBit < 0)
				{
					st_handler_info.nDlgLock = FALSE;
					return;
				}
				if (nBoard > 2 || nPort > 3 || nSlave > 7 || nBit > 15)
				{
					strTemp.Format(_T("[Debug_RepairIO]%d:%d:%d:%d:%d:%d"), i, m_nIn[i], nBoard, nPort, nSlave, nBit);
					clsFunc.OnLogFileAdd(3, strTemp);

					st_handler_info.nDlgLock = FALSE;
					return;
				}

				dlgIO.m_nIn[i]	= m_nIn[i];
				dlgIO.m_strIn[i]= clsExcel.m_strIOBitDescrip[nBoard][nPort][nSlave][nBit];
			}
			else
			{
				dlgIO.m_nIn[i]		= -1;
				dlgIO.m_strIn[i]	= _T("None");
			}
		}
	}

	if (m_nOutCnt > 0 && m_nOutCnt < 16)
	{
//		dlgIO.m_nOutCnt		= m_nOutCnt;
		for (i=0; i<m_nOutCnt; i++)
		{
			if (m_nOut[i] >= 0)
			{
				nBoard			= m_nOut[i] / 4000;
				nPort			= (m_nOut[i] % 4000) / 1000;
				nSlave			= ((m_nOut[i] % 4000) % 1000) / 100;
				nBit			= (((m_nOut[i] % 4000) % 1000) % 100) % 16;

				if (nBoard < 0 || nPort < 0 || nSlave < 0 || nBit < 0)
				{
					st_handler_info.nDlgLock = FALSE;
					return;
				}
				if (nBoard > 2 || nPort > 3 || nSlave > 7 || nBit > 15)
				{
					strTemp.Format(_T("[Debug_RepairIO]%d:%d:%d:%d:%d:%d"), i, m_nOut[i], nBoard, nPort, nSlave, nBit);
					clsFunc.OnLogFileAdd(3, strTemp);
					st_handler_info.nDlgLock = FALSE;
					return;
				}

				dlgIO.m_nOut[i]		= m_nOut[i];
				dlgIO.m_strOut[i]	= clsExcel.m_strIOBitDescrip[nBoard][nPort][nSlave][nBit];
			}
			else
			{
				dlgIO.m_nOut[i]		= -1;
				dlgIO.m_strOut[i]	= _T("None");
			}
		}
	}

	if (m_nOutCnt < 0 || m_nInCnt < 0)
	{
		st_handler_info.nDlgLock = FALSE;
		return;
	}

	if (m_nOutCnt > 16 || m_nInCnt > 16)
	{
		st_handler_info.nDlgLock = FALSE;
		return;
	}

	dlgIO.DoModal();*/
	if (st_handler_info.cWndAlarm != NULL)
	{
		st_handler_info.cWndAlarm->SendMessage(WM_WORK_COMMAND, ALARM_IO_CREATE_REQ, 0);
	}
}


void CScreenAlarm::OnClickedBtnAlarmMotor()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
/*	CDialogAlarmMotor		dlgMotor;
	int i, j;

	st_handler_info.nDlgLock = TRUE;

	for (i=0; i<clsExcel.m_nPartCnt; i++)
	{
		if(clsExcel.m_nPartAxisCnt[i] > 0)
		{
			for(j=0; j<clsExcel.m_nPartAxisCnt[i]; j++)
			{
				if ( m_nMotor == clsExcel.m_nPartAxis[i][j])
				{
					dlgMotor.m_strAxisName			= clsExcel.m_strPartAxisName[i][j];
					dlgMotor.m_nAxis				= clsExcel.m_nPartAxis[i][j];
					dlgMotor.m_nAxisLimitCnt		= clsExcel.m_nAxisLimitCnt[i][j];	
					dlgMotor.m_nAxisLimitInfo[0]	= clsExcel.m_nAxisLimitInfo[i][j][0];
					dlgMotor.m_nAxisLimitInfo[1]	= clsExcel.m_nAxisLimitInfo[i][j][1];
					dlgMotor.m_nAxisLimitInfo[2]	= clsExcel.m_nAxisLimitInfo[i][j][2];
				}
			}
		}
	}

	dlgMotor.DoModal();*/
	if (st_handler_info.cWndAlarm != NULL)
	{
		st_handler_info.cWndAlarm->SendMessage(WM_WORK_COMMAND, ALARM_MOTOR_CREATE_REQ, 0);
	}
}


void CScreenAlarm::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	clsFunc.OnSound(IO_OFF);

	st_work_info.nRemoveYesNo[0]	= m_nRemoveYesNo[0];
	// Alarm Reset Ŭ�� �� Reset ���º��� ���� < RUN S/W Ŭ���� Ȯ�εǴ� �����̴�. >
	if (st_alarm_info.nResetStatus != NO)
	{
		st_alarm_info.nResetStatus = NO;
	}
	
	// Alarm Reset Ŭ�� �� ȭ���� �������� ����ǹǷ� FALSE�� ��������. < RUN S/W Ŭ���� Ȯ�εǴ� �����̴�. >
	if (st_handler_info.nAlarmScreen != FALSE)
	{
		st_handler_info.nAlarmScreen = FALSE;
	}

	m_imgAlarm.ClearDrawing();
	m_imgAlarm.Delete();

	if(m_pImageView != NULL)
	{
		delete m_pImageView;
		m_pImageView = NULL;
	}

	if (m_strJamCode != NULL)
	{
		delete[] m_strJamCode;
		m_strJamCode = NULL;
	}

	if (st_handler_info.nMenuLock != FALSE)
	{
		st_handler_info.nMenuLock = FALSE;
	}

	st_handler_info.cWndAlarm = NULL;
}


void CScreenAlarm::PostNcDestroy()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	/*	m_imgAlarm.ClearDrawing();
	m_imgAlarm.Delete();

	if(m_pImageView != NULL)
	{
		delete m_pImageView;
		m_pImageView = NULL;
	}

	if (m_strJamCode != NULL)
	{
		delete[] m_strJamCode;
		m_strJamCode = NULL;
	}

	st_handler_info.cWndAlarm = NULL;*/
	CFormView::PostNcDestroy();
}


void CScreenAlarm::OnAlarmInfoDisplay(CString strCode)
{
	int nAlarmCode;
	int i;

	for (i=0; i<10; i++)
	{
		m_nRemoveYesNo[i] = NO;
	}

	nAlarmCode = _wtoi(strCode);


	m_chkNo.ShowWindow(FALSE);
	m_chkYes.ShowWindow(FALSE);

	switch (nAlarmCode)
	{
		case 10315:
			m_chkNo.SetCheck(TRUE);
			m_chkNo.SetWindowText(_T("Magazine Remove No"));
			m_chkNo.ShowWindow(TRUE);

			m_chkYes.SetCheck(FALSE);
			m_chkYes.SetWindowText(_T("Magazine Remove Yes"));
			m_chkYes.ShowWindow(TRUE);
			break;

		case 10316:
			m_chkNo.SetCheck(TRUE);
			m_chkNo.SetWindowText(_T("Magazine Remove No"));
			m_chkNo.ShowWindow(TRUE);

			m_chkYes.SetCheck(FALSE);
			m_chkYes.SetWindowText(_T("Magazine Remove Yes"));
			m_chkYes.ShowWindow(TRUE);
			break;

		case 10317:
			m_chkNo.SetCheck(TRUE);
			m_chkNo.SetWindowText(_T("Magazine Remove No"));
			m_chkNo.ShowWindow(TRUE);

			m_chkYes.SetCheck(FALSE);
			m_chkYes.SetWindowText(_T("Magazine Remove Yes"));
			m_chkYes.ShowWindow(TRUE);
			break;

		case 10318:
			m_chkNo.SetCheck(TRUE);
			m_chkNo.SetWindowText(_T("Magazine Remove No"));
			m_chkNo.ShowWindow(TRUE);

			m_chkYes.SetCheck(FALSE);
			m_chkYes.SetWindowText(_T("Magazine Remove Yes"));
			m_chkYes.ShowWindow(TRUE);
			break;

		case 10319:
			m_chkNo.SetCheck(TRUE);
			m_chkNo.SetWindowText(_T("Magazine Remove No"));
			m_chkNo.ShowWindow(TRUE);

			m_chkYes.SetCheck(FALSE);
			m_chkYes.SetWindowText(_T("Magazine Remove Yes"));
			m_chkYes.ShowWindow(TRUE);
			break;

		case 10320:
			m_chkNo.SetCheck(TRUE);
			m_chkNo.SetWindowText(_T("Magazine Remove No"));
			m_chkNo.ShowWindow(TRUE);

			m_chkYes.SetCheck(FALSE);
			m_chkYes.SetWindowText(_T("Magazine Remove Yes"));
			m_chkYes.ShowWindow(TRUE);
			break;

		case 50309:
			m_chkNo.SetCheck(TRUE);
			m_chkNo.SetWindowText(_T("PCB Remove No"));
			m_chkNo.ShowWindow(TRUE);

			m_chkYes.SetCheck(FALSE);
			m_chkYes.SetWindowText(_T("PCB Remove Yes"));
			m_chkYes.ShowWindow(TRUE);
			break;

		case 50310:
			m_chkNo.SetCheck(TRUE);
			m_chkNo.SetWindowText(_T("PCB Remove No"));
			m_chkNo.ShowWindow(TRUE);

			m_chkYes.SetCheck(FALSE);
			m_chkYes.SetWindowText(_T("PCB Remove Yes"));
			m_chkYes.ShowWindow(TRUE);
			break;

		case 50311:
			m_chkNo.SetCheck(TRUE);
			m_chkNo.SetWindowText(_T("PCB Remove No"));
			m_chkNo.ShowWindow(TRUE);

			m_chkYes.SetCheck(FALSE);
			m_chkYes.SetWindowText(_T("PCB Remove Yes"));
			m_chkYes.ShowWindow(TRUE);
			break;

		case 50312:
			m_chkNo.SetCheck(TRUE);
			m_chkNo.SetWindowText(_T("PCB Remove No"));
			m_chkNo.ShowWindow(TRUE);

			m_chkYes.SetCheck(FALSE);
			m_chkYes.SetWindowText(_T("PCB Remove Yes"));
			m_chkYes.ShowWindow(TRUE);
			break;

		case 70300:
			m_chkNo.SetCheck(TRUE);
			m_chkNo.SetWindowText(_T("PCB Remove No"));
			m_chkNo.ShowWindow(TRUE);

			m_chkYes.SetCheck(FALSE);
			m_chkYes.SetWindowText(_T("PCB Remove Yes"));
			m_chkYes.ShowWindow(TRUE);
			break;

		case 70301:
			m_chkNo.SetCheck(TRUE);
			m_chkNo.SetWindowText(_T("PCB Remove No"));
			m_chkNo.ShowWindow(TRUE);

			m_chkYes.SetCheck(FALSE);
			m_chkYes.SetWindowText(_T("PCB Remove Yes"));
			m_chkYes.ShowWindow(TRUE);
			break;

		case 70302:
			m_chkNo.SetCheck(TRUE);
			m_chkNo.SetWindowText(_T("PCB Remove No"));
			m_chkNo.ShowWindow(TRUE);

			m_chkYes.SetCheck(FALSE);
			m_chkYes.SetWindowText(_T("PCB Remove Yes"));
			m_chkYes.ShowWindow(TRUE);
			break;

		case 70303:
			m_chkNo.SetCheck(TRUE);
			m_chkNo.SetWindowText(_T("PCB Remove No"));
			m_chkNo.ShowWindow(TRUE);

			m_chkYes.SetCheck(FALSE);
			m_chkYes.SetWindowText(_T("PCB Remove Yes"));
			m_chkYes.ShowWindow(TRUE);
			break;

		case 70306:
			m_chkNo.SetCheck(TRUE);
			m_chkNo.SetWindowText(_T("PCB Remove No"));
			m_chkNo.ShowWindow(TRUE);

			m_chkYes.SetCheck(FALSE);
			m_chkYes.SetWindowText(_T("PCB Remove Yes"));
			m_chkYes.ShowWindow(TRUE);
			break;

		case 70307:
			m_chkNo.SetCheck(TRUE);
			m_chkNo.SetWindowText(_T("PCB Remove No"));
			m_chkNo.ShowWindow(TRUE);

			m_chkYes.SetCheck(FALSE);
			m_chkYes.SetWindowText(_T("PCB Remove Yes"));
			m_chkYes.ShowWindow(TRUE);
			break;

		case 120320:
			/*m_chkNo.SetCheck(TRUE);
			m_chkNo.SetWindowText(_T("SD ���� üũ Yes"));
			m_chkNo.ShowWindow(TRUE);

			m_chkYes.SetCheck(FALSE);
			m_chkYes.SetWindowText(_T("SD ���� üũ No"));
			m_chkYes.ShowWindow(TRUE);*/
			break;

		case 150304:
			m_chkNo.SetCheck(TRUE);
			m_chkNo.SetWindowText(_T("Jig Remove No"));
			m_chkNo.ShowWindow(TRUE);

			m_chkYes.SetCheck(FALSE);
			m_chkYes.SetWindowText(_T("Jig Remove Yes"));
			m_chkYes.ShowWindow(TRUE);
			break;

		case 150305:
			m_chkNo.SetCheck(TRUE);
			m_chkNo.SetWindowText(_T("Jig Remove No"));
			m_chkNo.ShowWindow(TRUE);

			m_chkYes.SetCheck(FALSE);
			m_chkYes.SetWindowText(_T("Jig Remove Yes"));
			m_chkYes.ShowWindow(TRUE);
			break;

		case 150306:
			m_chkNo.SetCheck(TRUE);
			m_chkNo.SetWindowText(_T("Jig Remove No"));
			m_chkNo.ShowWindow(TRUE);

			m_chkYes.SetCheck(FALSE);
			m_chkYes.SetWindowText(_T("Jig Remove Yes"));
			m_chkYes.ShowWindow(TRUE);
			break;

		case 150307:
			m_chkNo.SetCheck(TRUE);
			m_chkNo.SetWindowText(_T("Jig Remove No"));
			m_chkNo.ShowWindow(TRUE);

			m_chkYes.SetCheck(FALSE);
			m_chkYes.SetWindowText(_T("Jig Remove Yes"));
			m_chkYes.ShowWindow(TRUE);
			break;

		case 160311:
			m_chkNo.SetCheck(TRUE);
			m_chkNo.SetWindowText(_T("Jig Remove No"));
			m_chkNo.ShowWindow(TRUE);

			m_chkYes.SetCheck(FALSE);
			m_chkYes.SetWindowText(_T("Jig Remove Yes"));
			m_chkYes.ShowWindow(TRUE);
			break;

		case 160312:
			m_chkNo.SetCheck(TRUE);
			m_chkNo.SetWindowText(_T("Jig Remove No"));
			m_chkNo.ShowWindow(TRUE);

			m_chkYes.SetCheck(FALSE);
			m_chkYes.SetWindowText(_T("Jig Remove Yes"));
			m_chkYes.ShowWindow(TRUE);
			break;

		case 160313:
			m_chkNo.SetCheck(TRUE);
			m_chkNo.SetWindowText(_T("Jig Remove No"));
			m_chkNo.ShowWindow(TRUE);

			m_chkYes.SetCheck(FALSE);
			m_chkYes.SetWindowText(_T("Jig Remove Yes"));
			m_chkYes.ShowWindow(TRUE);
			break;

		case 160314:
			m_chkNo.SetCheck(TRUE);
			m_chkNo.SetWindowText(_T("Jig Remove No"));
			m_chkNo.ShowWindow(TRUE);

			m_chkYes.SetCheck(FALSE);
			m_chkYes.SetWindowText(_T("Jig Remove Yes"));
			m_chkYes.ShowWindow(TRUE);
			break;

		case 550004:
			m_chkNo.SetCheck(TRUE);
			m_chkNo.SetWindowText(_T("PCB ����"));
			m_chkNo.ShowWindow(TRUE);

			m_chkYes.SetCheck(FALSE);
			m_chkYes.SetWindowText(_T("PCB ����"));
			m_chkYes.ShowWindow(TRUE);
			break;
			
		case 550005:
			m_chkNo.SetCheck(TRUE);
			m_chkNo.SetWindowText(_T("Magazine ����"));
			m_chkNo.ShowWindow(TRUE);

			m_chkYes.SetCheck(FALSE);
			m_chkYes.SetWindowText(_T("Magazine ����"));
			m_chkYes.ShowWindow(TRUE);
			break;

		case 570003:
			m_chkNo.SetCheck(TRUE);
			m_chkNo.SetWindowText(_T("Magazine ����"));
			m_chkNo.ShowWindow(TRUE);

			m_chkYes.SetCheck(FALSE);
			m_chkYes.SetWindowText(_T("Magazine ����"));
			m_chkYes.ShowWindow(TRUE);
			break;

		case 650002:
			m_chkNo.SetCheck(TRUE);
			m_chkNo.SetWindowText(_T("UnLoader Tray Remove No"));
			m_chkNo.ShowWindow(TRUE);

			m_chkYes.SetCheck(FALSE);
			m_chkYes.SetWindowText(_T("UnLoader Tray Remove Yes"));
			m_chkYes.ShowWindow(TRUE);
			break;

		case 811207:
			m_chkNo.SetCheck(TRUE);
			m_chkNo.SetWindowText(_T("UnLoader Tray Remove No"));
			m_chkNo.ShowWindow(TRUE);

			m_chkYes.SetCheck(FALSE);
			m_chkYes.SetWindowText(_T("UnLoader Tray Remove Yes"));
			m_chkYes.ShowWindow(TRUE);
			break;

		case 990001:
			m_chkNo.SetCheck(TRUE);
			m_chkNo.SetWindowText(_T("Loader Tray Remove No"));
			m_chkNo.ShowWindow(TRUE);

			m_chkYes.SetCheck(FALSE);
			m_chkYes.SetWindowText(_T("Loader Tray Remove Yes"));
			m_chkYes.ShowWindow(TRUE);
			break;
	}
}

void CScreenAlarm::OnBnClickedBtnDoor()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//if (FAS_IO.get_in_bit(st_io_info.i_FrontSelectSwChk, IO_ON) == IO_OFF ||
	//	FAS_IO.get_in_bit(st_io_info.i_RearSelectSwChk, IO_ON)	== IO_OFF)
	//{
/*	if (FAS_IO.get_in_bit(st_io_info.i_ManualModeChk, IO_OFF) == IO_ON)
	{
 		FAS_IO.set_out_bit(st_io_info.o_DoorLock, IO_OFF);
	}
	else
	{
		if (st_handler_info.cWndMain != NULL)
		{
			st_other_info.strBoxMsg = _T("Please Check Manual Key Status.");
			st_handler_info.cWndMain->SendMessage(WM_WORK_COMMAND, MAIN_MESSAGE_BOX_CREATE_REQ, 0);
		}
	}*/
	CDialog_Message dlgMsg;

// 	if (FAS_IO.get_in_bit(st_io_info.i_FrontSelectSwChk, IO_ON) == IO_OFF ||
// 		FAS_IO.get_in_bit(st_io_info.i_RearSelectSwChk1, IO_ON)	== IO_OFF ||
// 		FAS_IO.get_in_bit(st_io_info.i_RearSelectSwChk2, IO_ON)	== IO_OFF)
// 	{
// 		//if (FAS_IO.get_in_bit(st_io_info.i_ManualModeChk, IO_OFF) == IO_ON)
// 		//{
// 		FAS_IO.set_out_bit(st_io_info.o_DoorLock, IO_OFF);
// 	}
// 	else
// 	{
// /*		if (FAS_IO.get_in_bit(st_io_info.i_ManualModeChk, IO_OFF) == IO_ON)
// 		{
// 			FAS_IO.set_out_bit(st_io_info.o_DoorLock, IO_OFF);
// 		}
// 		else
// 		{
// 			dlgMsg.m_nMessageType	= 0;
// 			dlgMsg.m_strMessage		= _T("Please Check Manual Key Status.");
// 
// 			dlgMsg.DoModal();
// 		}*/
// 		dlgMsg.m_nMessageType	= 0;
// 		dlgMsg.m_strMessage		= _T("Please Check Manual Key Status.");
// 
// 		dlgMsg.DoModal();
// 	}
}

LRESULT CScreenAlarm::OnAlarmWorkInfoCommand(WPARAM wParam, LPARAM lParam) 
{
	CString				strTemp;
	int					i, j;
	int					nBoard, nPort, nSlave, nBit;

	switch (wParam)
	{
		case ALARM_IO_CREATE_REQ:
			if (m_nInCnt > 0 || m_nOutCnt > 0)
			{
				if (m_pAlarmIO == NULL)
				{
					m_pAlarmIO			= new CDialogAlarmIO;

					m_pAlarmIO->m_nInCnt		= m_nInCnt;
					m_pAlarmIO->m_nOutCnt		= m_nOutCnt;
					//m_pAlarmIO->Create();
					//m_pAlarmIO->ShowWindow(SW_SHOW);
				}
				else
				{
					m_pAlarmIO->ShowWindow(SW_SHOW);

					return 0;
				}
			}

			if (m_nInCnt > 0 && m_nInCnt < 16)
			{
				for (i=0; i<m_nInCnt; i++)
				{
					if (m_nIn[i] >= 0)
					{
						nBoard			= m_nIn[i] / 4000;
						nPort			= (m_nIn[i] % 4000) / 1000;
						nSlave			= ((m_nIn[i] % 4000) % 1000) / 100;
						nBit			= (((m_nIn[i] % 4000) % 1000) % 100) % 16;

						if (nBoard < 0 || nPort < 0 || nSlave < 0 || nBit < 0)
						{
							if (m_pAlarmIO != NULL)
							{
								m_pAlarmIO->DestroyWindow();

								delete m_pAlarmIO;
								m_pAlarmIO = NULL;
							}

							return 0;
						}
						if (nBoard > 2 || nPort > 3 || nSlave > 7 || nBit > 15)
						{
							strTemp.Format(_T("[Debug_RepairIO]%d:%d:%d:%d:%d:%d"), i, m_nIn[i], nBoard, nPort, nSlave, nBit);
							clsFunc.OnLogFileAdd(3, strTemp);

							if (m_pAlarmIO != NULL)
							{
								m_pAlarmIO->DestroyWindow();

								delete m_pAlarmIO;
								m_pAlarmIO = NULL;
							}

							return 0;
						}

						m_pAlarmIO->m_nIn[i]	= m_nIn[i];
						m_pAlarmIO->m_strIn[i]= clsExcel.m_strIOBitDescrip[nBoard][nPort][nSlave][nBit];
					}
					else
					{
						m_pAlarmIO->m_nIn[i]	= -1;
						m_pAlarmIO->m_strIn[i]	= _T("None");
					}
				}
			}

			if (m_nOutCnt > 0 && m_nOutCnt < 16)
			{
				//		dlgIO.m_nOutCnt		= m_nOutCnt;
				for (i=0; i<m_nOutCnt; i++)
				{
					if (m_nOut[i] >= 0)
					{
						nBoard			= m_nOut[i] / 4000;
						nPort			= (m_nOut[i] % 4000) / 1000;
						nSlave			= ((m_nOut[i] % 4000) % 1000) / 100;
						nBit			= (((m_nOut[i] % 4000) % 1000) % 100) % 16;

						if (nBoard < 0 || nPort < 0 || nSlave < 0 || nBit < 0)
						{
							if (m_pAlarmIO != NULL)
							{
								m_pAlarmIO->DestroyWindow();

								delete m_pAlarmIO;
								m_pAlarmIO = NULL;
							}

							return 0;
						}
						if (nBoard > 2 || nPort > 3 || nSlave > 7 || nBit > 15)
						{
							strTemp.Format(_T("[Debug_RepairIO]%d:%d:%d:%d:%d:%d"), i, m_nOut[i], nBoard, nPort, nSlave, nBit);
							clsFunc.OnLogFileAdd(3, strTemp);
							
							if (m_pAlarmIO != NULL)
							{
								m_pAlarmIO->DestroyWindow();

								delete m_pAlarmIO;
								m_pAlarmIO = NULL;
							}
							return 0;
						}

						m_pAlarmIO->m_nOut[i]		= m_nOut[i];
						m_pAlarmIO->m_strOut[i]	= clsExcel.m_strIOBitDescrip[nBoard][nPort][nSlave][nBit];
					}
					else
					{
						m_pAlarmIO->m_nOut[i]		= -1;
						m_pAlarmIO->m_strOut[i]	= _T("None");
					}
				}
			}

			if (m_pAlarmIO != NULL)
			{
				m_pAlarmIO->Create();
				m_pAlarmIO->ShowWindow(SW_SHOW);
			}
			break;

		case ALARM_IO_DELETE_REQ:
			if (m_pAlarmIO != NULL)
			{
				m_pAlarmIO->DestroyWindow();

				delete m_pAlarmIO;
				m_pAlarmIO = NULL;
			}
			break;

		case ALARM_MOTOR_CREATE_REQ:
			if (m_pAlarmMotor == NULL)
			{
				m_pAlarmMotor			= new CDialogAlarmMotor;

				for (i=0; i<clsExcel.m_nPartCnt; i++)
				{
					if(clsExcel.m_nPartAxisCnt[i] > 0)
					{
						for(j=0; j<clsExcel.m_nPartAxisCnt[i]; j++)
						{
							if ( m_nMotor == clsExcel.m_nPartAxis[i][j])
							{
								m_pAlarmMotor->m_strAxisName		= clsExcel.m_strPartAxisName[i][j];
								m_pAlarmMotor->m_nAxis				= clsExcel.m_nPartAxis[i][j];
								m_pAlarmMotor->m_nAxisLimitCnt		= clsExcel.m_nAxisLimitCnt[i][j];	
								m_pAlarmMotor->m_nAxisLimitInfo[0]	= clsExcel.m_nAxisLimitInfo[i][j][0];
								m_pAlarmMotor->m_nAxisLimitInfo[1]	= clsExcel.m_nAxisLimitInfo[i][j][1];
								m_pAlarmMotor->m_nAxisLimitInfo[2]	= clsExcel.m_nAxisLimitInfo[i][j][2];
							}
						}
					}
				}
				m_pAlarmMotor->Create();
				m_pAlarmMotor->ShowWindow(SW_SHOW);
			}
			else
			{
				m_pAlarmMotor->ShowWindow(SW_SHOW);
			}
			break;

		case ALARM_MOTOR_DELETE_REQ:
			if (m_pAlarmMotor != NULL)
			{
				m_pAlarmMotor->DestroyWindow();

				delete m_pAlarmMotor;
				m_pAlarmMotor = NULL;
			}
			break;
	}

	return 0;
}