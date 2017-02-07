// Dialog_KeyBoard.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Dialog_KeyBoard.h"
#include "afxdialogex.h"
#include "Variable.h"
#include "PublicFunction.h"


// CDialog_KeyBoard dialog

IMPLEMENT_DYNAMIC(CDialog_KeyBoard, CDialog)

CDialog_KeyBoard::CDialog_KeyBoard(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog_KeyBoard::IDD, pParent)
{

}

CDialog_KeyBoard::~CDialog_KeyBoard()
{
}

void CDialog_KeyBoard::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSG_BOARD, m_msgKeyBoard);
	DDX_Control(pDX, IDC_MSG_BOARD_DATA, m_msgKeyBoardData);
	DDX_Control(pDX, IDC_GROUP_BOARD, m_groupKeyBoard);
	DDX_Control(pDX, IDC_BTN_BOARD_SHARP, m_btnBoardSharp);
	DDX_Control(pDX, IDC_BTN_BOARD_RBRACE_S, m_btnBoardRbraceS);
	DDX_Control(pDX, IDC_BTN_BOARD_RBRACE_B, m_btnBoardRbraceB);
	DDX_Control(pDX, IDC_BTN_BOARD_LINE, m_btnBoardLine);
	DDX_Control(pDX, IDC_BTN_BOARD_LBRACE_S, m_btnBoardLbraceS);
	DDX_Control(pDX, IDC_BTN_BOARD_LBRACE_B, m_btnBoardLbraceB);
	DDX_Control(pDX, IDC_BTN_BOARD_DOT, m_btnBoardDot);
	DDX_Control(pDX, IDC_BTN_BOARD_DOLLAR, m_btnBoardDollar);
	DDX_Control(pDX, IDC_BTN_BOARD_DIV, m_btnBoardDiv);
	DDX_Control(pDX, IDC_BTN_BOARD_DEL, m_btnBoardDel);
	DDX_Control(pDX, IDC_BTN_BOARD_BACK, m_btnBoardBack);
	DDX_Control(pDX, IDC_BTN_BOARD_ADD, m_btnBoardAdd);
	DDX_Control(pDX, IDC_BTN_BOARD_Z, m_btnBoardZ);
	DDX_Control(pDX, IDC_BTN_BOARD_Y, m_btnBoardY);
	DDX_Control(pDX, IDC_BTN_BOARD_X, m_btnBoardX);
	DDX_Control(pDX, IDC_BTN_BOARD_W, m_btnBoardW);
	DDX_Control(pDX, IDC_BTN_BOARD_V, m_btnBoardV);
	DDX_Control(pDX, IDC_BTN_BOARD_U, m_btnBoardU);
	DDX_Control(pDX, IDC_BTN_BOARD_T, m_btnBoardT);
	DDX_Control(pDX, IDC_BTN_BOARD_S, m_btnBoardS);
	DDX_Control(pDX, IDC_BTN_BOARD_R, m_btnBoardR);
	DDX_Control(pDX, IDC_BTN_BOARD_Q, m_btnBoardQ);
	DDX_Control(pDX, IDC_BTN_BOARD_P, m_btnBoardP);
	DDX_Control(pDX, IDC_BTN_BOARD_O, m_btnBoardO);
	DDX_Control(pDX, IDC_BTN_BOARD_N, m_btnBoardN);
	DDX_Control(pDX, IDC_BTN_BOARD_M, m_btnBoardM);
	DDX_Control(pDX, IDC_BTN_BOARD_L, m_btnBoardL);
	DDX_Control(pDX, IDC_BTN_BOARD_K, m_btnBoardK);
	DDX_Control(pDX, IDC_BTN_BOARD_J, m_btnBoardJ);
	DDX_Control(pDX, IDC_BTN_BOARD_I, m_btnBoardI);
	DDX_Control(pDX, IDC_BTN_BOARD_H, m_btnBoardH);
	DDX_Control(pDX, IDC_BTN_BOARD_G, m_btnBoardG);
	DDX_Control(pDX, IDC_BTN_BOARD_F, m_btnBoardF);
	DDX_Control(pDX, IDC_BTN_BOARD_E, m_btnBoardE);
	DDX_Control(pDX, IDC_BTN_BOARD_D, m_btnBoardD);
	DDX_Control(pDX, IDC_BTN_BOARD_C, m_btnBoardC);
	DDX_Control(pDX, IDC_BTN_BOARD_B, m_btnBoardB);
	DDX_Control(pDX, IDC_BTN_BOARD_A, m_btnBoardA);
	DDX_Control(pDX, IDC_BTN_BOARD_9, m_btnBoard9);
	DDX_Control(pDX, IDC_BTN_BOARD_8, m_btnBoard8);
	DDX_Control(pDX, IDC_BTN_BOARD_7, m_btnBoard7);
	DDX_Control(pDX, IDC_BTN_BOARD_6, m_btnBoard6);
	DDX_Control(pDX, IDC_BTN_BOARD_5, m_btnBoard5);
	DDX_Control(pDX, IDC_BTN_BOARD_4, m_btnBoard4);
	DDX_Control(pDX, IDC_BTN_BOARD_3, m_btnBoard3);
	DDX_Control(pDX, IDC_BTN_BOARD_2, m_btnBoard2);
	DDX_Control(pDX, IDC_BTN_BOARD_1, m_btnBoard1);
	DDX_Control(pDX, IDC_BTN_BOARD_0, m_btnBoard0);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Text(pDX, IDC_EDIT_BOARD_DATA, m_strKeyBoardVal);
}


BEGIN_MESSAGE_MAP(CDialog_KeyBoard, CDialog)
//	ON_COMMAND(IDC_BTN_APPLY, &CDialog_KeyBoard::OnBtnApply)
//	ON_COMMAND(IDC_BTN_APPLY, &CDialog_KeyBoard::OnBtnApply)
	ON_BN_CLICKED(IDC_BTN_BOARD_0, &CDialog_KeyBoard::OnClickedBtnBoard0)
	ON_BN_CLICKED(IDC_BTN_BOARD_1, &CDialog_KeyBoard::OnClickedBtnBoard1)
	ON_BN_CLICKED(IDC_BTN_BOARD_2, &CDialog_KeyBoard::OnClickedBtnBoard2)
	ON_BN_CLICKED(IDC_BTN_BOARD_3, &CDialog_KeyBoard::OnClickedBtnBoard3)
	ON_BN_CLICKED(IDC_BTN_BOARD_4, &CDialog_KeyBoard::OnClickedBtnBoard4)
	ON_BN_CLICKED(IDC_BTN_BOARD_5, &CDialog_KeyBoard::OnClickedBtnBoard5)
	ON_BN_CLICKED(IDC_BTN_BOARD_6, &CDialog_KeyBoard::OnClickedBtnBoard6)
	ON_BN_CLICKED(IDC_BTN_BOARD_7, &CDialog_KeyBoard::OnClickedBtnBoard7)
	ON_BN_CLICKED(IDC_BTN_BOARD_8, &CDialog_KeyBoard::OnClickedBtnBoard8)
	ON_BN_CLICKED(IDC_BTN_BOARD_9, &CDialog_KeyBoard::OnClickedBtnBoard9)
	ON_BN_CLICKED(IDC_BTN_BOARD_A, &CDialog_KeyBoard::OnClickedBtnBoardA)
	ON_BN_CLICKED(IDC_BTN_BOARD_ADD, &CDialog_KeyBoard::OnClickedBtnBoardAdd)
	ON_BN_CLICKED(IDC_BTN_BOARD_B, &CDialog_KeyBoard::OnClickedBtnBoardB)
	ON_BN_CLICKED(IDC_BTN_BOARD_BACK, &CDialog_KeyBoard::OnClickedBtnBoardBack)
	ON_BN_CLICKED(IDC_BTN_BOARD_C, &CDialog_KeyBoard::OnClickedBtnBoardC)
	ON_BN_CLICKED(IDC_BTN_BOARD_D, &CDialog_KeyBoard::OnClickedBtnBoardD)
	ON_BN_CLICKED(IDC_BTN_BOARD_DEL, &CDialog_KeyBoard::OnClickedBtnBoardDel)
	ON_BN_CLICKED(IDC_BTN_BOARD_DIV, &CDialog_KeyBoard::OnClickedBtnBoardDiv)
	ON_BN_CLICKED(IDC_BTN_BOARD_DOLLAR, &CDialog_KeyBoard::OnClickedBtnBoardDollar)
	ON_BN_CLICKED(IDC_BTN_BOARD_DOT, &CDialog_KeyBoard::OnClickedBtnBoardDot)
	ON_BN_CLICKED(IDC_BTN_BOARD_E, &CDialog_KeyBoard::OnClickedBtnBoardE)
	ON_BN_CLICKED(IDC_BTN_BOARD_F, &CDialog_KeyBoard::OnClickedBtnBoardF)
	ON_BN_CLICKED(IDC_BTN_BOARD_G, &CDialog_KeyBoard::OnClickedBtnBoardG)
	ON_BN_CLICKED(IDC_BTN_BOARD_H, &CDialog_KeyBoard::OnClickedBtnBoardH)
	ON_BN_CLICKED(IDC_BTN_BOARD_I, &CDialog_KeyBoard::OnClickedBtnBoardI)
	ON_BN_CLICKED(IDC_BTN_BOARD_J, &CDialog_KeyBoard::OnClickedBtnBoardJ)
	ON_BN_CLICKED(IDC_BTN_BOARD_K, &CDialog_KeyBoard::OnClickedBtnBoardK)
	ON_BN_CLICKED(IDC_BTN_BOARD_L, &CDialog_KeyBoard::OnClickedBtnBoardL)
	ON_BN_CLICKED(IDC_BTN_BOARD_LBRACE_B, &CDialog_KeyBoard::OnClickedBtnBoardLbraceB)
	ON_BN_CLICKED(IDC_BTN_BOARD_LBRACE_S, &CDialog_KeyBoard::OnClickedBtnBoardLbraceS)
	ON_BN_CLICKED(IDC_BTN_BOARD_LINE, &CDialog_KeyBoard::OnClickedBtnBoardLine)
	ON_BN_CLICKED(IDC_BTN_BOARD_M, &CDialog_KeyBoard::OnClickedBtnBoardM)
	ON_BN_CLICKED(IDC_BTN_BOARD_N, &CDialog_KeyBoard::OnClickedBtnBoardN)
	ON_BN_CLICKED(IDC_BTN_BOARD_O, &CDialog_KeyBoard::OnClickedBtnBoardO)
	ON_BN_CLICKED(IDC_BTN_BOARD_P, &CDialog_KeyBoard::OnClickedBtnBoardP)
	ON_BN_CLICKED(IDC_BTN_BOARD_Q, &CDialog_KeyBoard::OnClickedBtnBoardQ)
	ON_BN_CLICKED(IDC_BTN_BOARD_R, &CDialog_KeyBoard::OnClickedBtnBoardR)
	ON_BN_CLICKED(IDC_BTN_BOARD_RBRACE_B, &CDialog_KeyBoard::OnClickedBtnBoardRbraceB)
	ON_BN_CLICKED(IDC_BTN_BOARD_RBRACE_S, &CDialog_KeyBoard::OnClickedBtnBoardRbraceS)
	ON_BN_CLICKED(IDC_BTN_BOARD_S, &CDialog_KeyBoard::OnClickedBtnBoardS)
	ON_BN_CLICKED(IDC_BTN_BOARD_SHARP, &CDialog_KeyBoard::OnClickedBtnBoardSharp)
	ON_BN_CLICKED(IDC_BTN_BOARD_T, &CDialog_KeyBoard::OnClickedBtnBoardT)
	ON_BN_CLICKED(IDC_BTN_BOARD_U, &CDialog_KeyBoard::OnClickedBtnBoardU)
	ON_BN_CLICKED(IDC_BTN_BOARD_V, &CDialog_KeyBoard::OnClickedBtnBoardV)
	ON_BN_CLICKED(IDC_BTN_BOARD_W, &CDialog_KeyBoard::OnClickedBtnBoardW)
	ON_BN_CLICKED(IDC_BTN_BOARD_X, &CDialog_KeyBoard::OnClickedBtnBoardX)
	ON_BN_CLICKED(IDC_BTN_BOARD_Y, &CDialog_KeyBoard::OnClickedBtnBoardY)
	ON_BN_CLICKED(IDC_BTN_BOARD_Z, &CDialog_KeyBoard::OnClickedBtnBoardZ)
//	ON_COMMAND(IDC_BTN_CANCEL, &CDialog_KeyBoard::OnBtnCancel)
//	ON_COMMAND(IDC_BTN_APPLY, &CDialog_KeyBoard::OnBtnApply)
ON_BN_CLICKED(IDOK, &CDialog_KeyBoard::OnBnClickedOk)
ON_WM_ERASEBKGND()
ON_BN_CLICKED(IDCANCEL, &CDialog_KeyBoard::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDialog_KeyBoard message handlers


void CDialog_KeyBoard::OnClickedBtnBoard0()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("0");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoard1()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("1");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoard2()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("2");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoard3()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("3");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoard4()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("4");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoard5()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("5");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoard6()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("6");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoard7()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("7");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoard8()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("8");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoard9()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("9");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoardA()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("A");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoardAdd()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("+");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoardB()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("B");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoardBack()
{
	// TODO: Add your control notification handler code here
	int nCnt;

	UpdateData(TRUE);

	nCnt = m_strKeyBoardVal.GetLength();
	
	if (nCnt == 0)
	{
		return;
	}
	else 
	{
		m_strKeyBoardVal = m_strKeyBoardVal.Left(nCnt-1);
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoardC()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("C");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoardD()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("D");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoardDel()
{
	// TODO: Add your control notification handler code here
	m_strKeyBoardVal.Empty();
	//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
	UpdateData(FALSE);
}


void CDialog_KeyBoard::OnClickedBtnBoardDiv()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("/");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoardDollar()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("$");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoardDot()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T(".");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoardE()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("E");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoardF()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("F");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoardG()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("G");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoardH()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("H");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoardI()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("I");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoardJ()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("J");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoardK()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("K");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoardL()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("L");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoardLbraceB()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("[");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoardLbraceS()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("(");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoardLine()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("-");
		////m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoardM()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("M");
		//////m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoardN()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("N");
		//////m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoardO()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("O");
		////m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoardP()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("P");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoardQ()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("Q");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoardR()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("R");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoardRbraceB()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("]");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoardRbraceS()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T(")");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoardS()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("S");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoardSharp()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("#");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoardT()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("T");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoardU()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("U");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoardV()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("V");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoardW()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("W");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoardX()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("X");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoardY()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("Y");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


void CDialog_KeyBoard::OnClickedBtnBoardZ()
{
	// TODO: Add your control notification handler code here
	if(!OnKeyboardLengthCheck(m_strKeyBoardVal))
	{
		return;   // 입력 디바이스 정보 Length 체크 함수
	}
	else 
	{
		m_strKeyBoardVal +=_T("Z");
		//m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
		UpdateData(FALSE);
	}
}


//void CDialog_KeyBoard::OnBtnCancel()
//{
//	// TODO: Add your command handler code here
//}


//void CDialog_KeyBoard::OnBtnApply()
//{
//	// TODO: Add your command handler code here
//}


BOOL CDialog_KeyBoard::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::DestroyWindow();
}


BOOL CDialog_KeyBoard::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	OnInitGroupBox();
	OnInitLabel();
	m_strKeyBoardVal.Empty();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CDialog_KeyBoard::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::PreTranslateMessage(pMsg);
}

void CDialog_KeyBoard::OnInitLabel()
{
	CString strTmp;

	m_msgKeyBoard.SetFont(clsFunc.m_pFont[1]);
	m_msgKeyBoard.SetWindowText(_T("Input : String Data~"));
	m_msgKeyBoard.SetCenterText();
	m_msgKeyBoard.SetColor(RGB(0,0,255));
	m_msgKeyBoard.SetGradientColor(RGB(0,0,0));
	m_msgKeyBoard.SetTextColor(RGB(255,255,255));

// 	m_msgKeyBoardData.SetFont(clsFunc.m_pFont[1]);
// 	m_msgKeyBoardData.SetWindowText(m_strKeyBoardVal);
// 	m_msgKeyBoardData.SetCenterText();
// 	m_msgKeyBoardData.SetColor(RGB(255,255,255));
// 	m_msgKeyBoardData.SetGradientColor(RGB(255,255,255));
// 	m_msgKeyBoardData.SetTextColor(RGB(0,0,0));
}

void CDialog_KeyBoard::OnInitGroupBox()
{
	m_groupKeyBoard.SetFont(clsFunc.OnLogFont(16));
	m_groupKeyBoard.SetCatptionTextColor(BLUE_C);
	m_groupKeyBoard.SetBorderColor(ORANGE_C);
	m_groupKeyBoard.SetFontBold(TRUE);
	m_groupKeyBoard.SetBackgroundColor(WINDOW_UP1);
}

bool CDialog_KeyBoard::OnKeyboardLengthCheck(CString strData)
{
	int nCnt;

	/*nCnt = strData.GetLength();*/
	UpdateData(TRUE);

	nCnt	=	m_strKeyBoardVal.GetLength();
	
	if (nCnt > 40) return false;

	return true;
}

void CDialog_KeyBoard::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}


BOOL CDialog_KeyBoard::OnEraseBkgnd(CDC* pDC)
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


void CDialog_KeyBoard::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnCancel();
}
