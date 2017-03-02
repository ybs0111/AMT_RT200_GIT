#include "StdAfx.h"
#include "Barcode_2D.h"
#include "PublicFunction.h"
#include "Variable.h"
#include "Comm.h"
#include "AlgMemory.h"
#include "CtlBd_Library.h"

UINT OnBarcode2DMessage(LPVOID lParam);

CBarcode_2D clsBcr2D;

CBarcode_2D::CBarcode_2D(void)
{
	
}


CBarcode_2D::~CBarcode_2D(void)
{

}


CString	CBarcode_2D::OnCharToString(char chData[], int nLength)
{
	CString strTmp;
	BSTR	buf;
	int		nCnt;

	nCnt	= MultiByteToWideChar(CP_ACP, 0, chData, nLength, NULL, NULL);
	buf		= SysAllocStringLen(NULL, nCnt);
	MultiByteToWideChar(CP_ACP, 0, chData, nLength, buf, nCnt);
	strTmp.Format(_T("%s"), buf);

	SysFreeString(buf);

	return strTmp;
}

void CBarcode_2D::OnStringToChar(CString strData, char chData[])
{
	int nCnt;
	wchar_t *chBuf;

	chBuf = strData.GetBuffer(strData.GetLength());
	nCnt  = WideCharToMultiByte(CP_ACP, 0, chBuf, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, chBuf, -1, chData, nCnt, 0, 0);
}


void CBarcode_2D::OnDataSend(CString strData)
{
	m_nBin = m_nCommand = BD_NONE;
	m_strTrigger = strData;

	//::SendMessage(st_handler_info.hWnd, WM_BARCODE_MSG, BARCODE_TRIGGER_1, 0);
}

void CBarcode_2D::OnDataRevice(CString strData)
{
	int nPos[3];
	int nLength;

	CString strBarcode[2];
	CString strTemp;
	CString strCode;

	strBarcode[0]	= strBarcode[1]		= _T("");
	m_strBarcode[0] = m_strBarcode[0]	= _T("");

	nLength = strData.GetLength();

	nPos[0] = 0;
	nPos[1] = -1;
	nPos[1] = strData.Find(_T("Data"), nPos[0]);

	strCode = strData.Mid(nPos[1] + 7, nLength - (nPos[1] + 8));

	if (strCode == _T(""))
	{
		 m_strBarcode[0] = m_strBarcode[1] = _T("ERROR");
		
		clsMem.OnNormalMessageWrite(strData);
	}
	else if (strCode == _T("ERROR"))
	{
		m_strBarcode[0] = m_strBarcode[1] = _T("ERROR");
		
		clsMem.OnNormalMessageWrite(strData);
	}
	else
	{
		nPos[0] = 0;
		nPos[1] = -1;
		nPos[1] = strCode.Find(_T(","), nPos[0]);

		if (nPos[0] > 0)
		{
			strBarcode[0] = strCode.Mid(nPos[0], nPos[1]);
			strBarcode[1] = strCode.Mid(nPos[1] + 1, nLength - (nPos[1] + 1));

			nLength = strBarcode[0].GetLength();
			if (nLength == 22)
			{
				// ibm serial
				strTemp = strBarcode[0].Mid(0, 3);
				if (strTemp == _T("11S"))
				{
					m_strBarcode[0] = strBarcode[0];
				}
			}
			else if (nLength == 24)
			{
				// vendor serial
				strTemp = strBarcode[0].Mid(0, 2);
				if (strTemp == _T("KR"))
				{
					strTemp.Replace(_T(" "), _T("_"));
					m_strBarcode[1] = strBarcode[0];
				}
			}

			nLength = strBarcode[1].GetLength();
			if (nLength == 22)
			{
				// ibm serial
				strTemp = strBarcode[1].Mid(0, 3);
				if (strTemp == _T("11S"))
				{
					m_strBarcode[0] = strBarcode[1];
				}
			}
			else if (nLength == 24)
			{
				// vendor serial
				strTemp = strBarcode[1].Mid(0, 2);
				if (strTemp == _T("KR"))
				{
					strTemp.Replace(_T(" "), _T("_"));
					m_strBarcode[1] = strBarcode[1];
				}
			}
		}
		else
		{
			strBarcode[0] = strCode;

			nLength = strBarcode[0].GetLength();
			if (nLength == 22)
			{
				// ibm serial
				strTemp = strBarcode[0].Mid(0, 3);
				if (strTemp == _T("11S"))
				{
					m_strBarcode[0] = strBarcode[0];
				}
			}
			else if (nLength == 24)
			{
				// vendor serial
				strTemp = strBarcode[0].Mid(0, 2);
				if (strTemp == _T("KR"))
				{
					strTemp.Replace(_T(" "), _T("_"));
					m_strBarcode[1] = strBarcode[0];
				}
			}
		}
	}
}
