// Dialog_KeyPad.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Dialog_KeyPad.h"
#include "afxdialogex.h"
#include "PublicFunction.h"
#include "Variable.h"
#include "Dialog_Message.h"
// CDialog_KeyPad dialog

IMPLEMENT_DYNAMIC(CDialog_KeyPad, CDialog)

CDialog_KeyPad::CDialog_KeyPad(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog_KeyPad::IDD, pParent)
{
	m_pFont	= NULL;

	m_nKeypadMode	= 0;
}

CDialog_KeyPad::~CDialog_KeyPad()
{
}

void CDialog_KeyPad::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSG_OLD_VALUE, m_msgOldValue);
	DDX_Control(pDX, IDC_MSG_NEW_VALUE, m_msgNewValue);
	DDX_Control(pDX, IDC_MSG_KEYPAD_LIMIT, m_msgKeypadLimit);
	DDX_Control(pDX, IDC_GROUP_KEYPAD, m_groupKeypad);
	DDX_Control(pDX, IDC_BTN_PAD_RESET, m_btnPadReset);
	DDX_Control(pDX, IDC_BTN_PAD_DOT, m_btnPadDot);
	DDX_Control(pDX, IDC_BTN_PAD_1001, m_btnPad1001);
	DDX_Control(pDX, IDC_BTN_PAD_1000, m_btnPad1000);
	DDX_Control(pDX, IDC_BTN_PAD_101, m_btnPad101);
	DDX_Control(pDX, IDC_BTN_PAD_100, m_btnPad100);
	DDX_Control(pDX, IDC_BTN_PAD_11, m_btnPad11);
	DDX_Control(pDX, IDC_BTN_PAD_10, m_btnPad10);
	DDX_Control(pDX, IDC_BTN_PAD_9, m_btnPad9);
	DDX_Control(pDX, IDC_BTN_PAD_8, m_btnPad8);
	DDX_Control(pDX, IDC_BTN_PAD_7, m_btnPad7);
	DDX_Control(pDX, IDC_BTN_PAD_6, m_btnPad6);
	DDX_Control(pDX, IDC_BTN_PAD_5, m_btnPad5);
	DDX_Control(pDX, IDC_BTN_PAD_4, m_btnPad4);
	DDX_Control(pDX, IDC_BTN_PAD_3, m_btnPad3);
	DDX_Control(pDX, IDC_BTN_PAD_2, m_btnPad2);
	DDX_Control(pDX, IDC_BTN_PAD_1, m_btnPad1);
	DDX_Control(pDX, IDC_BTN_PAD_0, m_btnPad0);
	DDX_Control(pDX, IDC_BTN_PAD_DEL, m_btnPadDel);
	DDX_Control(pDX, IDC_BTN_PAD_BACK, m_btnPadBack);
	DDX_Control(pDX, IDC_DGT_NEW_VALUE, m_dgtNewValue);
	DDX_Control(pDX, IDC_DGT_OLD_VALUE, m_dgtOldValue);
}


BEGIN_MESSAGE_MAP(CDialog_KeyPad, CDialog)
	ON_BN_CLICKED(IDC_BTN_PAD_0, &CDialog_KeyPad::OnClickedBtnPad0)
	ON_BN_CLICKED(IDC_BTN_PAD_1, &CDialog_KeyPad::OnClickedBtnPad1)
	ON_BN_CLICKED(IDC_BTN_PAD_10, &CDialog_KeyPad::OnClickedBtnPad10)
	ON_BN_CLICKED(IDC_BTN_PAD_100, &CDialog_KeyPad::OnClickedBtnPad100)
	ON_BN_CLICKED(IDC_BTN_PAD_1000, &CDialog_KeyPad::OnClickedBtnPad1000)
	ON_BN_CLICKED(IDC_BTN_PAD_1001, &CDialog_KeyPad::OnClickedBtnPad1001)
	ON_BN_CLICKED(IDC_BTN_PAD_101, &CDialog_KeyPad::OnClickedBtnPad101)
	ON_BN_CLICKED(IDC_BTN_PAD_11, &CDialog_KeyPad::OnClickedBtnPad11)
	ON_BN_CLICKED(IDC_BTN_PAD_2, &CDialog_KeyPad::OnClickedBtnPad2)
	ON_BN_CLICKED(IDC_BTN_PAD_3, &CDialog_KeyPad::OnClickedBtnPad3)
	ON_BN_CLICKED(IDC_BTN_PAD_4, &CDialog_KeyPad::OnClickedBtnPad4)
	ON_BN_CLICKED(IDC_BTN_PAD_5, &CDialog_KeyPad::OnClickedBtnPad5)
	ON_BN_CLICKED(IDC_BTN_PAD_6, &CDialog_KeyPad::OnClickedBtnPad6)
	ON_BN_CLICKED(IDC_BTN_PAD_7, &CDialog_KeyPad::OnClickedBtnPad7)
	ON_BN_CLICKED(IDC_BTN_PAD_8, &CDialog_KeyPad::OnClickedBtnPad8)
	ON_BN_CLICKED(IDC_BTN_PAD_9, &CDialog_KeyPad::OnClickedBtnPad9)
	ON_BN_CLICKED(IDC_BTN_PAD_BACK, &CDialog_KeyPad::OnClickedBtnPadBack)
	ON_BN_CLICKED(IDC_BTN_PAD_DEL, &CDialog_KeyPad::OnClickedBtnPadDel)
	ON_BN_CLICKED(IDC_BTN_PAD_DOT, &CDialog_KeyPad::OnClickedBtnPadDot)
	ON_BN_CLICKED(IDC_BTN_PAD_RESET, &CDialog_KeyPad::OnClickedBtnPadReset)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &CDialog_KeyPad::OnBnClickedOk)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_PAD_SUB, &CDialog_KeyPad::OnBnClickedBtnPadSub)
END_MESSAGE_MAP()


// CDialog_KeyPad message handlers


void CDialog_KeyPad::OnClickedBtnPad0()
{
	// TODO: Add your control notification handler code here
	int nCnt;
	int nDotPos;
//	int nPos;

//	double dVal;

//	if (m_strNewVal.IsEmpty())  return;

	nCnt= m_strNewVal.GetLength();
	
	if (nCnt > 13) return;
/*
	// 소수점 3자리까지만 입력을 받는다. 2K8/01/10/ViboX
	nDotPos = m_strNewVal.Find('.');
	if (nDotPos != -1)
	{
		if (nCnt - nDotPos > 3)	return;
	}

	dVal = _wtof(m_strNewVal);

	if (dVal != 0)
	{
		m_strNewVal +=_T("0");
	}
	else 
	{
		nPos	= m_strNewVal.Find('-');
		nDotPos = m_strNewVal.Find('.');

		if (nPos == -1 && nDotPos == -1)
		{
			m_strNewVal =_T("0");
		}
		else 
		{
			m_strNewVal +=_T("0") ;
		}
	}
*/	
	if (m_nKeypadMode == 0)
	{
		m_strNewVal +=_T("0");
		m_dgtNewValue.SetVal(_wtoi(m_strNewVal));
	}
	else
	{
		nDotPos = m_strNewVal.Find('.');
		if (nDotPos != -1)
		{
			if (nCnt - nDotPos > 3)	return;
		}

		m_strNewVal +=_T("0");
		m_dgtNewValue.SetFloatVal((float)_wtof(m_strNewVal));
	}
}


void CDialog_KeyPad::OnClickedBtnPad1()
{
	// TODO: Add your control notification handler code here
	int nCnt;
	int nDotPos;
//	int nPos;

//	double dVal;

//	if (m_strNewVal.IsEmpty())  return;

	nCnt= m_strNewVal.GetLength();
	
	if (nCnt > 13) return;
/*
	// 소수점 3자리까지만 입력을 받는다. 2K8/01/10/ViboX
	nDotPos = m_strNewVal.Find('.');
	if (nDotPos != -1)
	{
		if (nCnt - nDotPos > 3)	return;
	}

	dVal = _wtof(m_strNewVal);

	if (dVal != 0)
	{
		m_strNewVal +=_T("1");
	}
	else 
	{
		nPos	= m_strNewVal.Find('-');
		nDotPos = m_strNewVal.Find('.');

		if (nPos == -1 && nDotPos == -1)
		{
			m_strNewVal =_T("1");
		}
		else 
		{
			m_strNewVal +=_T("1") ;
		}
	}
	
	if (m_nKeypadMode == 0)
	{
		m_dgtNewValue.SetVal(_wtoi(m_strNewVal));
	}
	else
	{
		m_dgtNewValue.SetFloatVal((float)_wtof(m_strNewVal));
	}
*/
	if (m_nKeypadMode == 0)
	{
		m_strNewVal +=_T("1");
		m_dgtNewValue.SetVal(_wtoi(m_strNewVal));
	}
	else
	{
		nDotPos = m_strNewVal.Find('.');
		if (nDotPos != -1)
		{
			if (nCnt - nDotPos > 3)	return;
		}

		m_strNewVal +=_T("1");
		m_dgtNewValue.SetFloatVal((float)_wtof(m_strNewVal));
	}
}


void CDialog_KeyPad::OnClickedBtnPad10()
{
	// TODO: Add your control notification handler code here
	double dVal;

	if (m_strNewVal.IsEmpty())
	{
		m_strNewVal=_T("0");
	}

	dVal = _wtof(m_strNewVal);
	if (dVal >= 999999990)
	{
		return;
	}

	dVal += 10 ;
	if (m_nKeypadMode == 0)
	{
		m_strNewVal.Format(_T("%d"), (int)dVal);
		m_dgtNewValue.SetVal(_wtoi(m_strNewVal));
	}
	else
	{
		m_strNewVal.Format(_T("%.3f"), dVal);
		m_dgtNewValue.SetFloatVal((float)_wtof(m_strNewVal));
	}
}


void CDialog_KeyPad::OnClickedBtnPad100()
{
	// TODO: Add your control notification handler code here
	double dVal;

	if (m_strNewVal.IsEmpty())
	{
		m_strNewVal=_T("0");
	}

	dVal = _wtof(m_strNewVal);
	if (dVal >= 999999990)
	{
		return;
	}

	dVal += 100 ;
	if (m_nKeypadMode == 0)
	{
		m_strNewVal.Format(_T("%d"), (int)dVal);
		m_dgtNewValue.SetVal(_wtoi(m_strNewVal));
	}
	else
	{
		m_strNewVal.Format(_T("%.3f"), dVal);
		m_dgtNewValue.SetFloatVal((float)_wtof(m_strNewVal));
	}
}


void CDialog_KeyPad::OnClickedBtnPad1000()
{
	// TODO: Add your control notification handler code here
	double dVal;

	if (m_strNewVal.IsEmpty())
	{
		m_strNewVal=_T("0");
	}

	dVal = _wtof(m_strNewVal);
	if (dVal >= 999999990)
	{
		return;
	}

	dVal += 1000 ;
	if (m_nKeypadMode == 0)
	{
		m_strNewVal.Format(_T("%d"), (int)dVal);
		m_dgtNewValue.SetVal(_wtoi(m_strNewVal));
	}
	else
	{
		m_strNewVal.Format(_T("%.3f"), dVal);
		m_dgtNewValue.SetFloatVal((float)_wtof(m_strNewVal));
	}
}


void CDialog_KeyPad::OnClickedBtnPad1001()
{
	// TODO: Add your control notification handler code here
	double dVal;

	if (m_strNewVal.IsEmpty())
	{
		m_strNewVal=_T("0");
	}

	dVal = _wtof(m_strNewVal);
	if (dVal >= 999999990)
	{
		return;
	}

	dVal -= 1000 ;
	if (m_nKeypadMode == 0)
	{
		m_strNewVal.Format(_T("%d"), (int)dVal);
		m_dgtNewValue.SetVal(_wtoi(m_strNewVal));
	}
	else
	{
		m_strNewVal.Format(_T("%.3f"), dVal);
		m_dgtNewValue.SetFloatVal((float)_wtof(m_strNewVal));
	}
}


void CDialog_KeyPad::OnClickedBtnPad101()
{
	// TODO: Add your control notification handler code here
	double dVal;

	if (m_strNewVal.IsEmpty())
	{
		m_strNewVal=_T("0");
	}

	dVal = _wtof(m_strNewVal);
	if (dVal >= 999999990)
	{
		return;
	}

	dVal -= 100 ;
	if (m_nKeypadMode == 0)
	{
		m_strNewVal.Format(_T("%d"), (int)dVal);
		m_dgtNewValue.SetVal(_wtoi(m_strNewVal));
	}
	else
	{
		m_strNewVal.Format(_T("%.3f"), dVal);
		m_dgtNewValue.SetFloatVal((float)_wtof(m_strNewVal));
	}
}


void CDialog_KeyPad::OnClickedBtnPad11()
{
	// TODO: Add your control notification handler code here
	double dVal;

	if (m_strNewVal.IsEmpty())
	{
		m_strNewVal=_T("0");
	}

	dVal = _wtof(m_strNewVal);
	if (dVal >= 999999990)
	{
		return;
	}

	dVal -= 10 ;
	if (m_nKeypadMode == 0)
	{
		m_strNewVal.Format(_T("%d"), (int)dVal);
		m_dgtNewValue.SetVal(_wtoi(m_strNewVal));
	}
	else
	{
		m_strNewVal.Format(_T("%.3f"), dVal);
		m_dgtNewValue.SetFloatVal((float)_wtof(m_strNewVal));
	}
}


void CDialog_KeyPad::OnClickedBtnPad2()
{
	// TODO: Add your control notification handler code here
	int nCnt;
	int nDotPos;
//	int nPos;

//	double dVal;

//	if (m_strNewVal.IsEmpty())  return;

	nCnt= m_strNewVal.GetLength();
	
	if (nCnt > 13) return;
/*
	// 소수점 3자리까지만 입력을 받는다. 2K8/01/10/ViboX
	nDotPos = m_strNewVal.Find('.');
	if (nDotPos != -1)
	{
		if (nCnt - nDotPos > 3)	return;
	}

	dVal = _wtof(m_strNewVal);

	if (dVal != 0)
	{
		m_strNewVal +=_T("2");
	}
	else 
	{
		nPos	= m_strNewVal.Find('-');
		nDotPos = m_strNewVal.Find('.');

		if (nPos == -1 && nDotPos == -1)
		{
			m_strNewVal =_T("2");
		}
		else 
		{
			m_strNewVal +=_T("2") ;
		}
	}
	
	if (m_nKeypadMode == 0)
	{
		m_dgtNewValue.SetVal(_wtoi(m_strNewVal));
	}
	else
	{
		m_dgtNewValue.SetFloatVal((float)_wtof(m_strNewVal));
	}
*/
	if (m_nKeypadMode == 0)
	{
		m_strNewVal +=_T("2");
		m_dgtNewValue.SetVal(_wtoi(m_strNewVal));
	}
	else
	{
		nDotPos = m_strNewVal.Find('.');
		if (nDotPos != -1)
		{
			if (nCnt - nDotPos > 3)	return;
		}

		m_strNewVal +=_T("2");
		m_dgtNewValue.SetFloatVal((float)_wtof(m_strNewVal));
	}
}


void CDialog_KeyPad::OnClickedBtnPad3()
{
	// TODO: Add your control notification handler code here
	int nCnt;
	int nDotPos;
//	int nPos;

//	double dVal;

//	if (m_strNewVal.IsEmpty())  return;

	nCnt= m_strNewVal.GetLength();
	
	if (nCnt > 13) return;
/*
	// 소수점 3자리까지만 입력을 받는다. 2K8/01/10/ViboX
	nDotPos = m_strNewVal.Find('.');
	if (nDotPos != -1)
	{
		if (nCnt - nDotPos > 3)	return;
	}

	dVal = _wtof(m_strNewVal);

	if (dVal != 0)
	{
		m_strNewVal +=_T("3");
	}
	else 
	{
		nPos	= m_strNewVal.Find('-');
		nDotPos = m_strNewVal.Find('.');

		if (nPos == -1 && nDotPos == -1)
		{
			m_strNewVal =_T("3");
		}
		else 
		{
			m_strNewVal +=_T("3") ;
		}
	}
	
	if (m_nKeypadMode == 0)
	{
		m_dgtNewValue.SetVal(_wtoi(m_strNewVal));
	}
	else
	{
		m_dgtNewValue.SetFloatVal((float)_wtof(m_strNewVal));
	}
*/
	if (m_nKeypadMode == 0)
	{
		m_strNewVal +=_T("3");
		m_dgtNewValue.SetVal(_wtoi(m_strNewVal));
	}
	else
	{
		nDotPos = m_strNewVal.Find('.');
		if (nDotPos != -1)
		{
			if (nCnt - nDotPos > 3)	return;
		}

		m_strNewVal +=_T("3");
		m_dgtNewValue.SetFloatVal((float)_wtof(m_strNewVal));
	}
}


void CDialog_KeyPad::OnClickedBtnPad4()
{
	// TODO: Add your control notification handler code here
	int nCnt;
	int nDotPos;
//	int nPos;

//	double dVal;

//	if (m_strNewVal.IsEmpty())  return;

	nCnt= m_strNewVal.GetLength();
	
	if (nCnt > 13) return;
/*
	// 소수점 3자리까지만 입력을 받는다. 2K8/01/10/ViboX
	nDotPos = m_strNewVal.Find('.');
	if (nDotPos != -1)
	{
		if (nCnt - nDotPos > 3)	return;
	}

	dVal = _wtof(m_strNewVal);

	if (dVal != 0)
	{
		m_strNewVal +=_T("4");
	}
	else 
	{
		nPos	= m_strNewVal.Find('-');
		nDotPos = m_strNewVal.Find('.');

		if (nPos == -1 && nDotPos == -1)
		{
			m_strNewVal =_T("4");
		}
		else 
		{
			m_strNewVal +=_T("4") ;
		}
	}
	
	if (m_nKeypadMode == 0)
	{
		m_dgtNewValue.SetVal(_wtoi(m_strNewVal));
	}
	else
	{
		m_dgtNewValue.SetFloatVal((float)_wtof(m_strNewVal));
	}
*/
	if (m_nKeypadMode == 0)
	{
		m_strNewVal +=_T("4");
		m_dgtNewValue.SetVal(_wtoi(m_strNewVal));
	}
	else
	{
		nDotPos = m_strNewVal.Find('.');
		if (nDotPos != -1)
		{
			if (nCnt - nDotPos > 3)	return;
		}

		m_strNewVal +=_T("4");
		m_dgtNewValue.SetFloatVal((float)_wtof(m_strNewVal));
	}
}


void CDialog_KeyPad::OnClickedBtnPad5()
{
	// TODO: Add your control notification handler code here
	int nCnt;
	int nDotPos;
//	int nPos;

//	double dVal;

//	if (m_strNewVal.IsEmpty())  return;

	nCnt= m_strNewVal.GetLength();
	
	if (nCnt > 13) return;
/*
	// 소수점 3자리까지만 입력을 받는다. 2K8/01/10/ViboX
	nDotPos = m_strNewVal.Find('.');
	if (nDotPos != -1)
	{
		if (nCnt - nDotPos > 3)	return;
	}

	dVal = _wtof(m_strNewVal);

	if (dVal != 0)
	{
		m_strNewVal +=_T("5");
	}
	else 
	{
		nPos	= m_strNewVal.Find('-');
		nDotPos = m_strNewVal.Find('.');

		if (nPos == -1 && nDotPos == -1)
		{
			m_strNewVal =_T("5");
		}
		else 
		{
			m_strNewVal +=_T("5") ;
		}
	}
	
	if (m_nKeypadMode == 0)
	{
		m_dgtNewValue.SetVal(_wtoi(m_strNewVal));
	}
	else
	{
		m_dgtNewValue.SetFloatVal((float)_wtof(m_strNewVal));
	}
*/
	if (m_nKeypadMode == 0)
	{
		m_strNewVal +=_T("5");
		m_dgtNewValue.SetVal(_wtoi(m_strNewVal));
	}
	else
	{
		nDotPos = m_strNewVal.Find('.');
		if (nDotPos != -1)
		{
			if (nCnt - nDotPos > 3)	return;
		}

		m_strNewVal +=_T("5");
		m_dgtNewValue.SetFloatVal((float)_wtof(m_strNewVal));
	}
}


void CDialog_KeyPad::OnClickedBtnPad6()
{
	// TODO: Add your control notification handler code here
	int nCnt;
	int nDotPos;
//	int nPos;

//	double dVal;

//	if (m_strNewVal.IsEmpty())  return;

	nCnt= m_strNewVal.GetLength();
	
	if (nCnt > 13) return;
/*
	// 소수점 3자리까지만 입력을 받는다. 2K8/01/10/ViboX
	nDotPos = m_strNewVal.Find('.');
	if (nDotPos != -1)
	{
		if (nCnt - nDotPos > 3)	return;
	}

	dVal = _wtof(m_strNewVal);

	if (dVal != 0)
	{
		m_strNewVal +=_T("6");
	}
	else 
	{
		nPos	= m_strNewVal.Find('-');
		nDotPos = m_strNewVal.Find('.');

		if (nPos == -1 && nDotPos == -1)
		{
			m_strNewVal =_T("6");
		}
		else 
		{
			m_strNewVal +=_T("6") ;
		}
	}
	
	if (m_nKeypadMode == 0)
	{
		m_dgtNewValue.SetVal(_wtoi(m_strNewVal));
	}
	else
	{
		m_dgtNewValue.SetFloatVal((float)_wtof(m_strNewVal));
	}
*/
	if (m_nKeypadMode == 0)
	{
		m_strNewVal +=_T("6");
		m_dgtNewValue.SetVal(_wtoi(m_strNewVal));
	}
	else
	{
		nDotPos = m_strNewVal.Find('.');
		if (nDotPos != -1)
		{
			if (nCnt - nDotPos > 3)	return;
		}

		m_strNewVal +=_T("6");
		m_dgtNewValue.SetFloatVal((float)_wtof(m_strNewVal));
	}
}


void CDialog_KeyPad::OnClickedBtnPad7()
{
	// TODO: Add your control notification handler code here
	int nCnt;
	int nDotPos;
//	int nPos;

//	double dVal;

//	if (m_strNewVal.IsEmpty())  return;

	nCnt= m_strNewVal.GetLength();
	
	if (nCnt > 13) return;
/*
	// 소수점 3자리까지만 입력을 받는다. 2K8/01/10/ViboX
	nDotPos = m_strNewVal.Find('.');
	if (nDotPos != -1)
	{
		if (nCnt - nDotPos > 3)	return;
	}

	dVal = _wtof(m_strNewVal);

	if (dVal != 0)
	{
		m_strNewVal +=_T("7");
	}
	else 
	{
		nPos	= m_strNewVal.Find('-');
		nDotPos = m_strNewVal.Find('.');

		if (nPos == -1 && nDotPos == -1)
		{
			m_strNewVal =_T("7");
		}
		else 
		{
			m_strNewVal +=_T("7") ;
		}
	}
	
	if (m_nKeypadMode == 0)
	{
		m_dgtNewValue.SetVal(_wtoi(m_strNewVal));
	}
	else
	{
		m_dgtNewValue.SetFloatVal((float)_wtof(m_strNewVal));
	}
*/
	if (m_nKeypadMode == 0)
	{
		m_strNewVal +=_T("7");
		m_dgtNewValue.SetVal(_wtoi(m_strNewVal));
	}
	else
	{
		nDotPos = m_strNewVal.Find('.');
		if (nDotPos != -1)
		{
			if (nCnt - nDotPos > 3)	return;
		}

		m_strNewVal +=_T("7");
		m_dgtNewValue.SetFloatVal((float)_wtof(m_strNewVal));
	}
}


void CDialog_KeyPad::OnClickedBtnPad8()
{
	// TODO: Add your control notification handler code here
	int nCnt;
	int nDotPos;
//	int nPos;

//	double dVal;

//	if (m_strNewVal.IsEmpty())  return;

	nCnt= m_strNewVal.GetLength();
	
	if (nCnt > 13) return;
/*
	// 소수점 3자리까지만 입력을 받는다. 2K8/01/10/ViboX
	nDotPos = m_strNewVal.Find('.');
	if (nDotPos != -1)
	{
		if (nCnt - nDotPos > 3)	return;
	}

	dVal = _wtof(m_strNewVal);

	if (dVal != 0)
	{
		m_strNewVal +=_T("8");
	}
	else 
	{
		nPos	= m_strNewVal.Find('-');
		nDotPos = m_strNewVal.Find('.');

		if (nPos == -1 && nDotPos == -1)
		{
			m_strNewVal =_T("8");
		}
		else 
		{
			m_strNewVal +=_T("8") ;
		}
	}
	
	if (m_nKeypadMode == 0)
	{
		m_dgtNewValue.SetVal(_wtoi(m_strNewVal));
	}
	else
	{
		m_dgtNewValue.SetFloatVal((float)_wtof(m_strNewVal));
	}
*/
	if (m_nKeypadMode == 0)
	{
		m_strNewVal +=_T("8");
		m_dgtNewValue.SetVal(_wtoi(m_strNewVal));
	}
	else
	{
		nDotPos = m_strNewVal.Find('.');
		if (nDotPos != -1)
		{
			if (nCnt - nDotPos > 3)	return;
		}

		m_strNewVal +=_T("8");
		m_dgtNewValue.SetFloatVal((float)_wtof(m_strNewVal));
	}
}


void CDialog_KeyPad::OnClickedBtnPad9()
{
	// TODO: Add your control notification handler code here
	int nCnt;
	int nDotPos;
//	int nPos;

//	double dVal;

//	if (m_strNewVal.IsEmpty())  return;

	nCnt= m_strNewVal.GetLength();
	
	if (nCnt > 13) return;
/*
	// 소수점 3자리까지만 입력을 받는다. 2K8/01/10/ViboX
	nDotPos = m_strNewVal.Find('.');
	if (nDotPos != -1)
	{
		if (nCnt - nDotPos > 3)	return;
	}

	dVal = _wtof(m_strNewVal);

	if (dVal != 0)
	{
		m_strNewVal +=_T("9");
	}
	else 
	{
		nPos	= m_strNewVal.Find('-');
		nDotPos = m_strNewVal.Find('.');

		if (nPos == -1 && nDotPos == -1)
		{
			m_strNewVal =_T("9");
		}
		else 
		{
			m_strNewVal +=_T("9") ;
		}
	}

	if (m_nKeypadMode == 0)
	{
		m_dgtNewValue.SetVal(_wtoi(m_strNewVal));
	}
	else
	{
		m_dgtNewValue.SetFloatVal((float)_wtof(m_strNewVal));
	}
*/
	if (m_nKeypadMode == 0)
	{
		m_strNewVal +=_T("9");
		m_dgtNewValue.SetVal(_wtoi(m_strNewVal));
	}
	else
	{
		nDotPos = m_strNewVal.Find('.');
		if (nDotPos != -1)
		{
			if (nCnt - nDotPos > 3)	return;
		}

		m_strNewVal +=_T("9");
		m_dgtNewValue.SetFloatVal((float)_wtof(m_strNewVal));
	}
}


void CDialog_KeyPad::OnClickedBtnPadBack()
{
	// TODO: Add your control notification handler code here
	int nCnt;
	int nPos;

	if (m_strNewVal ==_T("") || m_strNewVal ==_T("0")) return ;  // 현재 입력 값이 공백이거나 0이면 리턴

	nCnt = m_strNewVal.GetLength();

	if (nCnt == 1) 
	{
		nPos = m_strNewVal.Find('-');

		if (nPos != -1)  return;
		else
		{
			m_strNewVal.Empty();
			if (m_nKeypadMode == 0)
			{
				m_dgtNewValue.SetVal(0);
			}
			else
			{
				m_dgtNewValue.SetFloatVal(0.0);
			}
		}
	}
	else 
	{
		CString s(m_strNewVal);
		m_strNewVal=s.Left(nCnt-1);

		if (m_nKeypadMode == 0)
		{
			m_dgtNewValue.SetVal(_wtoi(m_strNewVal));
		}
		else
		{
			m_dgtNewValue.SetFloatVal((float)_wtof(m_strNewVal));
		}
	}
}


void CDialog_KeyPad::OnClickedBtnPadDel()
{
	// TODO: Add your control notification handler code here
	m_strNewVal = ""; //.Empty();

	if (m_nKeypadMode == 0)
	{
		m_dgtNewValue.SetVal(0);
	}
	else
	{
		m_dgtNewValue.SetFloatVal(0.0);
	}
}


void CDialog_KeyPad::OnClickedBtnPadDot()
{
	// TODO: Add your control notification handler code here
	double dVal;

//	int nDotPos;

	if (m_strNewVal.IsEmpty())  m_strNewVal=_T("0");
	
	dVal = _wtof(m_strNewVal);

	if (dVal >= 999999990) return;
/*
	// 소수점이 있으면 또 안찍힌다. 2K8/01/10/ViboX
	nDotPos = m_strNewVal.Find('.');
	if (nDotPos != -1)	return;
	
	if (dVal == 0)
	{
		m_strNewVal = _T("0.");
	}
	else
	{
		m_strNewVal += _T(".");
	}
*/
	if (m_nKeypadMode == 0)
	{
		m_dgtNewValue.SetVal(_wtoi(m_strNewVal));
	}
	else
	{
		m_strNewVal += _T(".");
		m_dgtNewValue.SetFloatVal((float)_wtof(m_strNewVal));
	}
}


void CDialog_KeyPad::OnClickedBtnPadReset()
{
	// TODO: Add your control notification handler code here
	double dVal;
	m_strNewVal = m_strOldVal;
	
	if (!m_strNewVal.IsEmpty())  m_strNewVal =_T("0");  // 공백 정보가 전달된 경우 0으로 초기값 설정 
	
	dVal = _wtof(m_strNewVal);					// Limit 넘어 간 경우에는 기존 OLD로 넣는다.
	
	if (dVal > 999999990 || dVal == 0)
	{
		m_strNewVal =_T("0");
		if (m_nKeypadMode == 0)
		{
			m_dgtNewValue.SetVal(0);
		}
		else
		{
			m_dgtNewValue.SetFloatVal(0.0);
		}
	}
	else  
	{
		if (m_nKeypadMode == 0)
		{
			m_dgtNewValue.SetVal(_wtoi(m_strNewVal));
		}
		else
		{
			m_dgtNewValue.SetFloatVal((float)_wtof(m_strNewVal));
		}
	}
}


void CDialog_KeyPad::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
}


BOOL CDialog_KeyPad::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_pFont != NULL)
	{
		delete m_pFont;
		m_pFont = NULL;
	}
	return CDialog::DestroyWindow();
}

void CDialog_KeyPad::OnInitDigit()
{
	float fVal;
	int nVal;

//	m_strNewVal = m_strKeypadVal;	// 전역 변수에 저장된 값을 클래스 변수에 설정
	m_strOldVal = m_strKeypadVal;	// 전역 변수에 저장된 값을 클래스 변수에 설정

	if (m_nKeypadMode == 0)
	{
		nVal = _wtoi(m_strKeypadVal);

		m_dgtNewValue.SetStyle(CDigit::DS_INT, 13, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
		m_dgtNewValue.SetVal(0);

		m_dgtOldValue.SetStyle(CDigit::DS_INT, 13, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
		m_dgtOldValue.SetVal(nVal);
	}
	else
	{
		fVal = (float)_wtof(m_strKeypadVal);

		m_dgtNewValue.SetStyle(CDigit::DS_FLOAT3, 13, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
		m_dgtNewValue.SetFloatVal(0.0);

		m_dgtOldValue.SetStyle(CDigit::DS_FLOAT3, 13, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
		m_dgtOldValue.SetFloatVal(fVal);
	}
}

BOOL CDialog_KeyPad::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	// **************************************************************************
	// 컨트롤에 설정할 폰트 생성한다                                             
	// **************************************************************************
	m_pFont = NULL;
	m_pFont = new CFont;
	m_pFont->CreateFont(12,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,_T("MS Sans Serif"));

	OnInitDigit();
	OnInitGroupBox();
	OnInitLabel();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDialog_KeyPad::OnInitLabel()
{
	CString strTmp;

	m_msgKeypadLimit.SetFont(m_pFont);
	strTmp.Format(_T("Input Limit : %s ~ %s"), m_strKeypadLowLimit, m_strKeypadHighLimit);
	m_msgKeypadLimit.SetWindowText(strTmp);
	m_msgKeypadLimit.SetCenterText();
	m_msgKeypadLimit.SetColor(RGB(0,0,255));
	m_msgKeypadLimit.SetGradientColor(RGB(0,0,0));
	m_msgKeypadLimit.SetTextColor(RGB(255,255,255));

	m_msgOldValue.SetFont(m_pFont);
	m_msgOldValue.SetWindowText(_T("Prev."));
	m_msgOldValue.SetCenterText();
	m_msgOldValue.SetColor(RGB(0,0,255));
	m_msgOldValue.SetGradientColor(RGB(0,0,0));
	m_msgOldValue.SetTextColor(RGB(255,255,255));

	m_msgNewValue.SetFont(m_pFont);
	m_msgNewValue.SetWindowText(_T("New"));
	m_msgNewValue.SetCenterText();
	m_msgNewValue.SetColor(RGB(0,0,255));
	m_msgNewValue.SetGradientColor(RGB(0,0,0));
	m_msgNewValue.SetTextColor(RGB(255,255,255));
}

void CDialog_KeyPad::OnInitGroupBox()
{
	m_groupKeypad.SetFont(clsFunc.OnLogFont(16));
	m_groupKeypad.SetCatptionTextColor(BLUE_C);
	m_groupKeypad.SetBorderColor(ORANGE_C);
	m_groupKeypad.SetFontBold(TRUE);
	m_groupKeypad.SetBackgroundColor(WINDOW_UP1);
}

void CDialog_KeyPad::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog_Message		dlgMsg;
	int					n_response;				// 대화 상자 리턴 플래그
	int					n_error_flag = FALSE;
	double				dVal;

	n_error_flag = TRUE;

	if (m_nKeypadMode == 0)
	{
		if (_wtoi(m_strNewVal) > _wtoi(m_strKeypadHighLimit))
		{
			dlgMsg.m_nMessageType	= 0;
			dlgMsg.m_strMessage		= _T("Input Data is Limit Over.");
			dlgMsg.DoModal();

			n_error_flag = FALSE;
		}
		else if (_wtoi(m_strNewVal) < _wtoi(m_strKeypadLowLimit))
		{
			dlgMsg.m_nMessageType	= 0;
			dlgMsg.m_strMessage		= _T("Input Data is Limit Under.");
			dlgMsg.DoModal();
			n_error_flag = FALSE;
		}
	}
	else
	{
		if (_wtof(m_strNewVal) > _wtof(m_strKeypadHighLimit))
		{
			dlgMsg.m_nMessageType	= 0;
			dlgMsg.m_strMessage		= _T("Input Data is Limit Over.");
			dlgMsg.DoModal();
			n_error_flag = FALSE;
		}
		else if (_wtof(m_strNewVal) < _wtof(m_strKeypadLowLimit))
		{
			dlgMsg.m_nMessageType	= 0;
			dlgMsg.m_strMessage		= _T("Input Data is Limit Under.");
			dlgMsg.DoModal();
			n_error_flag = FALSE;
		}
	}

	if (n_error_flag == TRUE)
	{
		dlgMsg.m_nMessageType	= 1;
		dlgMsg.m_strMessage		= _T("Do you want to change the existing value to the current value?");
		n_response =  (int)dlgMsg.DoModal();

		if (n_response == IDOK)
		{
			if (m_strNewVal.IsEmpty())  m_strNewVal =_T("0");  // 공백 정보가 전달된 경우 0으로 초기값 설정 

			dVal = _wtof(m_strNewVal);					// Limit 넘어 간 경우에는 기존 OLD로 넣는다.
				
			if (dVal > 999999990 || dVal == 0)
			{
				m_strNewVal =_T("0");
				if (m_nKeypadMode == 0)
				{
					m_dgtNewValue.SetVal(0);
				}
				else
				{
					m_dgtNewValue.SetFloatVal(0.0);
				}

				CDialog::OnOK();
			}
			else  
			{
				if (m_nKeypadMode == 0)
				{
					m_dgtNewValue.SetVal(_wtoi(m_strNewVal));
				}
				else
				{
					m_dgtNewValue.SetFloatVal((float)_wtof(m_strNewVal));
				}
				CDialog::OnOK();
			}
		//	return;	
		}
		else
		{
			CDialog::OnCancel();
		}
	}
	else
	{
		return;
	}

///	CDialog::OnOK();
}


BOOL CDialog_KeyPad::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_NUMPAD0 || pMsg->wParam == '0')			{OnClickedBtnPad0(); return true;}
		else if (pMsg->wParam == VK_NUMPAD1 || pMsg->wParam == '1')		{OnClickedBtnPad1(); return true;}
		else if (pMsg->wParam == VK_NUMPAD2 || pMsg->wParam == '2')		{OnClickedBtnPad2(); return true;}
		else if (pMsg->wParam == VK_NUMPAD3 || pMsg->wParam == '3')		{OnClickedBtnPad3(); return true;}
		else if (pMsg->wParam == VK_NUMPAD4 || pMsg->wParam == '4')		{OnClickedBtnPad4(); return true;}
		else if (pMsg->wParam == VK_NUMPAD5 || pMsg->wParam == '5')		{OnClickedBtnPad5(); return true;}
		else if (pMsg->wParam == VK_NUMPAD6 || pMsg->wParam == '6')		{OnClickedBtnPad6(); return true;}
		else if (pMsg->wParam == VK_NUMPAD7 || pMsg->wParam == '7')		{OnClickedBtnPad7(); return true;}
		else if (pMsg->wParam == VK_NUMPAD8 || pMsg->wParam == '8')		{OnClickedBtnPad8(); return true;}
		else if (pMsg->wParam == VK_NUMPAD9 || pMsg->wParam == '9')		{OnClickedBtnPad9(); return true;}
		else if ((pMsg->wParam == VK_DECIMAL || pMsg->wParam == 0xBE) && m_nKeypadMode == 1)	{OnClickedBtnPadDot(); return true;}
		else if (pMsg->wParam == VK_BACK)								{OnClickedBtnPadBack(); return true;}
		else if (pMsg->wParam == VK_DELETE)								{OnClickedBtnPadDel(); return true;}
		else if (pMsg->wParam == VK_RETURN)								{OnBnClickedOk(); return true;}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CDialog_KeyPad::OnEraseBkgnd(CDC* pDC)
{
	CDC  memDC;                     // 메모리 DC
	CBitmap * pOldBitmap = NULL;    // m_BackBitmap으로 새로운 그림을 DC에 그릴 때, 이전 그려졌던 DC(즉, Bitmap)을 저장.
	BITMAP bitmapInfo;              // 그림의 정보(m_BackBitmap)
	st_handler_info.m_BackDlgBitmap.GetBitmap(&bitmapInfo); // Bitmap 크기 구함.

	memDC.CreateCompatibleDC(pDC);  // 메모리 DC 생성
	pOldBitmap = memDC.SelectObject(&st_handler_info.m_BackDlgBitmap);
	pDC->BitBlt(0, 0, bitmapInfo.bmWidth, bitmapInfo.bmHeight, &memDC, 0, 0, SRCCOPY);   // 메모리 DC에 그림을 그림

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	return true; // 흰 부러쉬가 아닌 배경을 그려야 하기 때문에 true
}


void CDialog_KeyPad::OnBnClickedBtnPadSub()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nCnt;
//	int nDotPos;
//	int nPos;

//	double dVal;

	nCnt= m_strNewVal.GetLength();
	
	if (nCnt > 1) return;

	m_strNewVal =_T("-") + m_strNewVal;

	if (m_nKeypadMode == 0)
	{
		m_dgtNewValue.SetVal(_wtoi(m_strNewVal));
	}
	else
	{
		m_dgtNewValue.SetFloatVal((float)_wtof(m_strNewVal));
	}
}
