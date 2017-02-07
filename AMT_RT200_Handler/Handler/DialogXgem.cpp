// DialogXgem.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Handler.h"
#include "DialogXgem.h"
#include "afxdialogex.h"
#include "XgemClient.h"
#include "Variable.h"
// CDialogXgem 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogXgem, CDialog)

CDialogXgem::CDialogXgem(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogXgem::IDD, pParent)
{

	m_strAlarm = _T("");
	m_strLotNo = _T("");
	m_strPartNo = _T("");
}

CDialogXgem::~CDialogXgem()
{
}

void CDialogXgem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ALARM, m_strAlarm);
	DDX_Text(pDX, IDC_EDIT_LOT_NO, m_strLotNo);
	DDX_Text(pDX, IDC_EDIT_PART_NO, m_strPartNo);
}


BEGIN_MESSAGE_MAP(CDialogXgem, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_START, &CDialogXgem::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_END, &CDialogXgem::OnBnClickedButtonEnd)
	ON_BN_CLICKED(IDC_BUTTON_ALARM_ON, &CDialogXgem::OnBnClickedButtonAlarmOn)
	ON_BN_CLICKED(IDC_BUTTON_ALARM_OFF, &CDialogXgem::OnBnClickedButtonAlarmOff)
	ON_BN_CLICKED(IDC_BUTTON_RECIPE, &CDialogXgem::OnBnClickedButtonRecipe)
	ON_BN_CLICKED(IDC_BUTTON_BOARD, &CDialogXgem::OnBnClickedButtonBoard)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CDialogXgem::OnBnClickedButtonExit)
END_MESSAGE_MAP()


// CDialogXgem 메시지 처리기입니다.


BOOL CDialogXgem::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ¿©±â¿¡ Ãß°¡ ÃÊ±âÈ­ ÀÛ¾÷À» Ãß°¡ÇÕ´Ï´Ù.

	return TRUE;  // return TRUE unless you set the focus to a control
	// ¿¹¿Ü: OCX ¼Ó¼º ÆäÀÌÁö´Â FALSE¸¦ ¹ÝÈ¯ÇØ¾ß ÇÕ´Ï´Ù.
}


void CDialogXgem::OnBnClickedButtonStart()
{
	// TODO: ¿©±â¿¡ ÄÁÆ®·Ñ ¾Ë¸² Ã³¸®±â ÄÚµå¸¦ Ãß°¡ÇÕ´Ï´Ù.
	UpdateData(TRUE);

	if (m_strLotNo == _T("") || m_strPartNo == _T("")) return;

	clsXgem.OnMcLotStart(m_strLotNo, m_strPartNo);
}


void CDialogXgem::OnBnClickedButtonEnd()
{
	// TODO: ¿©±â¿¡ ÄÁÆ®·Ñ ¾Ë¸² Ã³¸®±â ÄÚµå¸¦ Ãß°¡ÇÕ´Ï´Ù.
	COleDateTimeSpan time;
	UpdateData(TRUE);

	if (m_strLotNo == _T("") || m_strPartNo == _T("")) return;

	clsXgem.OnMcLotEnd(m_strLotNo, m_strPartNo, 100, 90, 10, time);
}


void CDialogXgem::OnBnClickedButtonAlarmOn()
{
	// TODO: ¿©±â¿¡ ÄÁÆ®·Ñ ¾Ë¸² Ã³¸®±â ÄÚµå¸¦ Ãß°¡ÇÕ´Ï´Ù.
	UpdateData(TRUE);

	if (m_strAlarm == _T("")) return;

	clsXgem.OnMcAlarm(m_strAlarm, 1);
}


void CDialogXgem::OnBnClickedButtonAlarmOff()
{
	// TODO: ¿©±â¿¡ ÄÁÆ®·Ñ ¾Ë¸² Ã³¸®±â ÄÚµå¸¦ Ãß°¡ÇÕ´Ï´Ù.
	UpdateData(TRUE);

	if (m_strAlarm == _T("")) return;

	clsXgem.OnMcAlarm(m_strAlarm, 1);
}


void CDialogXgem::OnBnClickedButtonRecipe()
{
	// TODO: ¿©±â¿¡ ÄÁÆ®·Ñ ¾Ë¸² Ã³¸®±â ÄÚµå¸¦ Ãß°¡ÇÕ´Ï´Ù.
	clsXgem.OnMcRecipeCreate(0, st_basic_info.strDeviceName, st_path_info.strPathRecipe);
}


void CDialogXgem::OnBnClickedButtonBoard()
{
	// TODO: ¿©±â¿¡ ÄÁÆ®·Ñ ¾Ë¸² Ã³¸®±â ÄÚµå¸¦ Ãß°¡ÇÕ´Ï´Ù.
	CString strTemp;

	UpdateData(TRUE);

	if (m_strLotNo == _T("") || m_strPartNo == _T("")) return;

	strTemp.Format(_T("TRACE_DATA_%s.TXT"), m_strLotNo);

	clsXgem.OnMcRecipeCreate(1, strTemp, st_path_info.strPathRecipe);
}


void CDialogXgem::OnBnClickedButtonExit()
{
	// TODO: ¿©±â¿¡ ÄÁÆ®·Ñ ¾Ë¸² Ã³¸®±â ÄÚµå¸¦ Ãß°¡ÇÕ´Ï´Ù.
	CDialog::OnOK();
}
