#include "StdAfx.h"
#include "ClientRfid.h"
#include "Variable.h"
#include "AlgMemory.h"
#include "ClientSocket.h"
#include "CtlBd_Library.h"
#include "Ctlbd_Variable.h"
#include "PublicFunction.h"

CClientRfid clsRfid;

UINT OnClientRfidThread(LPVOID lParam);

CClientRfid::CClientRfid(void)
{
	int nRfid;

	nRfid				= RF_CARRIER_ID;
	m_nRfidPos[nRfid]	= 0;
	m_nRfidNum[nRfid]	= 8;

	nRfid				= RF_EQP_ID;
	m_nRfidPos[nRfid]	= 8;
	m_nRfidNum[nRfid]	= 8;

	nRfid				= RF_QTY;
	m_nRfidPos[nRfid]	= 16;
	m_nRfidNum[nRfid]	= 6;

	nRfid				= RF_QTY_T;
	m_nRfidPos[nRfid]	= 22;
	m_nRfidNum[nRfid]	= 6;

	nRfid				= RF_LOT_ID_J;
	m_nRfidPos[nRfid]	= 28;
	m_nRfidNum[nRfid]	= 12;

	nRfid				= RF_STEP;
	m_nRfidPos[nRfid]	= 40;
	m_nRfidNum[nRfid]	= 6;

	nRfid				= RF_LOT_TYPE;
	m_nRfidPos[nRfid]	= 46;
	m_nRfidNum[nRfid]	= 2;

	nRfid				= RF_PART_ID;
	m_nRfidPos[nRfid]	= 48;
	m_nRfidNum[nRfid]	= 28;
	
	nRfid				= RF_PKG_CODE;
	m_nRfidPos[nRfid]	= 76;
	m_nRfidNum[nRfid]	= 3;

	nRfid				= RF_NONE_1;
	m_nRfidPos[nRfid]	= 79;
	m_nRfidNum[nRfid]	= 1;

	nRfid				= RF_LOT_ID_M;
	m_nRfidPos[nRfid]	= 80;
	m_nRfidNum[nRfid]	= 12;

	nRfid				= RF_CNT;
	m_nRfidPos[nRfid]	= 92;
	m_nRfidNum[nRfid]	= 8;

	nRfid				= RF_FLAG;
	m_nRfidPos[nRfid]	= 100;
	m_nRfidNum[nRfid]	= 2;

	nRfid				= RF_REJECT;
	m_nRfidPos[nRfid]	= 102;
	m_nRfidNum[nRfid]	= 2;

	nRfid				= RF_BIN;
	m_nRfidPos[nRfid]	= 104;
	m_nRfidNum[nRfid]	= 2;

	nRfid				= RF_NONE_2;
	m_nRfidPos[nRfid]	= 106;
	m_nRfidNum[nRfid]	= 2;

	nRfid				= RF_TRAY_QTY;
	m_nRfidPos[nRfid]	= 108;
	m_nRfidNum[nRfid]	= 4;

	m_nCommStep			= 0;
}


CClientRfid::~CClientRfid(void)
{
	if (m_thrHandle) 
	{
		m_bThread = false;
		while(WaitForSingleObject(m_thrHandle, 5000) != WAIT_OBJECT_0)
		{
			if (m_thrHandle == NULL) break;
			Sleep(0);
		}
	}
}


void CClientRfid::OnCommunication()
{
	switch (m_nCommStep)
	{
		case 0:
			if (st_basic_info.nModeRfid == YES)
			{
				// rfid 사용모드이면 
				m_nRetry	= 0;
				m_nCommBin	= BD_NONE;

				m_nCommStep = 2000;
			}
			else
			{
				m_bThread	= false;

				m_nCommStep = 0;
			}
			break;

		case 2000:
			if (st_client_info[RFID_NETWORK].nConnect == YES)
			{
				m_nCommStep = 2200;
			}
			else
			{
				m_dwWaitTime[0] = GetTickCount();

				::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + RFID_NETWORK, CLIENT_CONNECT, 0);

				m_nCommStep = 2100;
			}
			break;

		case 2100:
			if (st_client_info[RFID_NETWORK].nConnect == YES)
			{
				m_nCommStep = 2200;
			}
			else
			{
				m_dwWaitTime[1] = GetTickCount();
				m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];

				if (m_dwWaitTime[2] < 0)
				{
					m_dwWaitTime[0] = GetTickCount();
					break;
				}

				if (m_dwWaitTime[2] > 60000)
				{
					m_nRetry++;

					if (m_nRetry > 3)
					{
						m_nCommBin	= BD_CONT_TIMEOUT;

						::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + RFID_NETWORK, CLIENT_CLOSE, 0);

						m_dwWaitTime[0] = GetTickCount();
						CTL_Lib.Alarm_Error_Occurrence(20000, dWARNING, _T("620000"));

						m_nCommStep = 2150;
					}
					else
					{
						m_dwWaitTime[0] = GetTickCount();

						::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + RFID_NETWORK, CLIENT_CLOSE, 0);

						m_nCommStep = 2150;
					}
				}
			}
			break;

		case 2150:
			m_dwWaitTime[1] = GetTickCount();
			m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];

			if (m_dwWaitTime[2] < 0)
			{
				m_dwWaitTime[0] = GetTickCount();
				break;
			}

			if (m_dwWaitTime[2] > WAIT_CLOSE)
			{
				m_bThread	= false;

				m_nCommStep = 0;
			}
			break;

		case 2200:
			m_dwWaitTime[0] = GetTickCount();
			st_client_info[RFID_NETWORK].strSend = m_strSendMsg;

			::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + RFID_NETWORK, CLIENT_SEND, 0);

			m_nCommStep = 2300;
			break;

		case 2300:
			if (m_nCommBin == BIN_GOODBIN)
			{
				m_bThread	= false;

				::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + RFID_NETWORK, CLIENT_CLOSE, 0);

				m_nCommStep = 0;
			}
			else if (m_nCommBin == BIN_REJECTBIN)
			{
				m_dwWaitTime[0] = GetTickCount();
				::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + RFID_NETWORK, CLIENT_CLOSE, 0);

				m_nCommStep = 2350;
			}
			else
			{
				m_dwWaitTime[1] = GetTickCount();
				m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];

				if (m_dwWaitTime[2] < 0)
				{
					m_dwWaitTime[0] = GetTickCount();
					break;
				}

				if (m_dwWaitTime[2] > WAIT_RCV)
				{
					m_nRetry++;

					if (m_nRetry > 3)
					{
						m_nCommBin	= BD_RCV_TIMEOUT;

						::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + RFID_NETWORK, CLIENT_CLOSE, 0);

						m_dwWaitTime[0] = GetTickCount();
						CTL_Lib.Alarm_Error_Occurrence(20001, dWARNING, _T("620001"));

						m_nCommStep = 2150;
					}
					else
					{
						m_dwWaitTime[0] = GetTickCount();
						::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + RFID_NETWORK, CLIENT_CLOSE, 0);

						m_nCommStep = 2150;
					}
				}
			}
			break;

		case 2350:
			m_dwWaitTime[1] = GetTickCount();
			m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];

			if (m_dwWaitTime[2] < 0)
			{
				m_dwWaitTime[0] = GetTickCount();
				break;
			}

			if (m_dwWaitTime[2] > WAIT_CLOSE)
			{
				m_bThread	= false;

				m_nCommStep = 0;
			}
			break;
	}
}

UINT OnClientRfidThread(LPVOID lParam)
{
	CClientRfid *pMsg = (CClientRfid *)lParam;

	CString strTemp;

	while(pMsg->m_bThread)
	{
		pMsg->OnCommunication();
	}

	pMsg->m_thrHandle = NULL;

	return 0;
}

CString	CClientRfid::OnCharToString(char chData[], int nLength)
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

void CClientRfid::OnStringToChar(CString strData, char chData[])
{
	int nCnt;
	wchar_t *chBuf;

	chBuf = strData.GetBuffer(strData.GetLength());
	nCnt  = WideCharToMultiByte(CP_ACP, 0, chBuf, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, chBuf, -1, chData, nCnt, 0, 0);
}

void CClientRfid::OnDataReceive(CString strData)
{
	int i, j;
	int nLength;
	int nVal;
	int nPos[2];
	char chData[1024];
	CString strFunc;
	CString strTemp;
	CString strHex;
	CString strHead[17] = {_T("CARRIER"),	_T("EQP"),		_T("QTY"),	_T("QTY_T"),	_T("LOT_ID_J"),
						   _T("STEP"),		_T("LOT_TYPE"), _T("PART"), _T("PKG_CODE"), _T("NONE_1"),
						   _T("LOT_ID_M"),	_T("CNT"),		_T("FLAG"), _T("REJECT"),	_T("BIN"),
						   _T("NONE_2"),	_T("TRAY_QTY")};
	CString strLog = _T("[RFID >> HANDLER] ");

	nLength		= strData.GetLength();

	if (nLength > 1024) return;

	OnStringToChar(strData, chData);

	if (chData[nLength-1] == 0x03)
	{
		m_strData += strData;

		strFunc = m_strData.Mid(3, 2);

		if (strFunc == _T("80") || strFunc == _T("81"))
		{
			for (i=0; i<17; i++)
			{
				nPos[0] = (m_nRfidPos[i] * 2) + 5;
				nPos[1] = m_nRfidNum[i] * 2;

				strTemp  = m_strData.Mid(nPos[0], nPos[1]);
				m_strRfidData[i] = _T("");
				for (j=0; j<m_nRfidNum[i] * 2; j+=2)
				{
					nVal = OnHexToDecimal(strTemp.Mid(j, 2));
					strHex.Format(_T("%c"), nVal);

					if (strHex != _T(" "))
					{
						m_strRfidData[i] += strHex;
					}
				}

				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
				{
					strTemp.Format(_T("[RFID_READ_%s] %s"), strHead[i], m_strRfidData[i]);
					clsMem.OnNormalMessageWrite(strTemp);
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
				}

			}
			m_nCommBin = BIN_GOODBIN;
		}
		else if(strFunc == _T("90") || strFunc == _T("91"))
		{
			if (chData[0] == 0x06)
			{
				m_nCommBin = BIN_GOODBIN;
			}
		}
		else
		{
			m_nCommBin = BIN_REJECTBIN;
		}

		m_strData = _T("");
	}
	else
	{
		m_strData += strData;
	}

	strLog += strData;
	clsFunc.OnLogFileAdd(4, strLog);
}


int	CClientRfid::OnHexToDecimal(CString strHex)
{
	char chHex[10];
	int val;

	OnStringToChar(strHex, chHex);

	chHex[2] = '\0';
	val = strtoul(chHex, NULL, 16);

	return val;
}


bool	CClientRfid::OnMakeWriteCodeAll(int nChann, CString strCode[20])
{
	int		i;
	int		nLength;
	int		nSum = 0;
	int		nBcc;

	CString strRfid = _T("");
	CString strMake = _T("");
	CString strLength;
	CString strTemp;
	CString strData;
	CString strTag;
	CString strHead[17] = {_T("CARRIER"),	_T("EQP"),		_T("QTY"),	_T("QTY_T"),	_T("LOT_ID_J"),
						   _T("STEP"),		_T("LOT_TYPE"), _T("PART"), _T("PKG_CODE"), _T("NONE_1"),
						   _T("LOT_ID_M"),	_T("CNT"),		_T("FLAG"), _T("REJECT"),	_T("BIN"),
						   _T("NONE_2"),	_T("TRAY_QTY")};
	CString strLog = _T("[HANDLER >> RFID] ");

	BYTE	byBuf[1024];

	m_strData	= _T("");
	
	if (m_bThread) return false;
	
	m_bThread	= true;
	m_nCommBin	= BD_NONE;

	strLength.Format(_T("%02x%02x"), m_nRfidPos[RF_EQP_ID], 104);
	strMake.Format(_T("%c01%d%s"), 0x05, 90 + (nChann - 1), strLength);

	/*if (strCode[0].GetLength() > 8)
	{
		strTemp.Format(_T("%-8s"), strCode[0].Mid(0, 8));
	}
	else
	{
		strTemp.Format(_T("%-8s"), strCode[0]);
	}
	strData += strTemp;*/

	if (strCode[1].GetLength() > 8)
	{
		strTemp.Format(_T("%-8s"), strCode[1].Mid(0, 8));
	}
	else
	{
		strTemp.Format(_T("%-8s"), strCode[1]);
	}
	strData += strTemp;
	if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
	{
		strTag.Format(_T("[RFID_WRITE_%s] %s"), strHead[RF_EQP_ID], strTemp);
		clsMem.OnNormalMessageWrite(strTag);
		st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
	}

	if (strCode[2].GetLength() > 6)
	{
		strTemp.Format(_T("%-6s"), strCode[2].Mid(0, 6));
	}
	else
	{
		strTemp.Format(_T("%-6s"), strCode[2]);
	}
	strData += strTemp;
	if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
	{
		strTag.Format(_T("[RFID_WRITE_%s] %s"), strHead[RF_QTY], strTemp);
		clsMem.OnNormalMessageWrite(strTag);
		st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
	}

	if (strCode[3].GetLength() > 6)
	{
		strTemp.Format(_T("%-6s"), strCode[3].Mid(0, 6));
	}
	else
	{
		strTemp.Format(_T("%-6s"), strCode[3]);
	}
	strData += strTemp;
	if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
	{
		strTag.Format(_T("[RFID_WRITE_%s] %s"), strHead[RF_QTY_T], strTemp);
		clsMem.OnNormalMessageWrite(strTag);
		st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
	}

	if (strCode[4].GetLength() > 12)
	{
		strTemp.Format(_T("%-12s"), strCode[4].Mid(0, 12));
	}
	else
	{
		strTemp.Format(_T("%-12s"), strCode[4]);
	}
	strData += strTemp;
	if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
	{
		strTag.Format(_T("[RFID_WRITE_%s] %s"), strHead[RF_LOT_ID_J], strTemp);
		clsMem.OnNormalMessageWrite(strTag);
		st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
	}

	if (strCode[5].GetLength() > 6)
	{
		strTemp.Format(_T("%-6s"), strCode[5].Mid(0, 6));
	}
	else
	{
		strTemp.Format(_T("%-6s"), strCode[5]);
	}
	strData += strTemp;
	if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
	{
		strTag.Format(_T("[RFID_WRITE_%s] %s"), strHead[RF_STEP], strTemp);
		clsMem.OnNormalMessageWrite(strTag);
		st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
	}

	if (strCode[6].GetLength() > 2)
	{
		strTemp.Format(_T("%-2s"), strCode[6].Mid(0, 2));
	}
	else
	{
		strTemp.Format(_T("%-2s"), strCode[6]);
	}
	strData += strTemp;
	if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
	{
		strTag.Format(_T("[RFID_WRITE_%s] %s"), strHead[RF_LOT_TYPE], strTemp);
		clsMem.OnNormalMessageWrite(strTag);
		st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
	}

	if (strCode[7].GetLength() > 28)
	{
		strTemp.Format(_T("%-28s"), strCode[7].Mid(0, 28));
	}
	else
	{
		strTemp.Format(_T("%-28s"), strCode[7]);
	}
	strData += strTemp;
	if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
	{
		strTag.Format(_T("[RFID_WRITE_%s] %s"), strHead[RF_PART_ID], strTemp);
		clsMem.OnNormalMessageWrite(strTag);
		st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
	}

	if (strCode[8].GetLength() > 1)
	{
		strTemp.Format(_T("%-1s"), strCode[8].Mid(0, 1));
	}
	else
	{
		strTemp.Format(_T("%-1s"), strCode[8]);
	}
	strData += strTemp;
	if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
	{
		strTag.Format(_T("[RFID_WRITE_%s] %s"), strHead[RF_PKG_CODE], strTemp);
		clsMem.OnNormalMessageWrite(strTag);
		st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
	}

	if (strCode[9].GetLength() > 3)
	{
		strTemp.Format(_T("%-3s"), strCode[9].Mid(0, 3));
	}
	else
	{
		strTemp.Format(_T("%-3s"), strCode[9]);
	}
	strData += strTemp;
	if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
	{
		strTag.Format(_T("[RFID_WRITE_%s] %s"), strHead[RF_NONE_1], strTemp);
		clsMem.OnNormalMessageWrite(strTag);
		st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
	}

	if (strCode[10].GetLength() > 12)
	{
		strTemp.Format(_T("%-12s"), strCode[10].Mid(0, 12));
	}
	else
	{
		strTemp.Format(_T("%-12s"), strCode[10]);
	}
	strData += strTemp;
	if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
	{
		strTag.Format(_T("[RFID_WRITE_%s] %s"), strHead[RF_LOT_ID_M], strTemp);
		clsMem.OnNormalMessageWrite(strTag);
		st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
	}

	if (strCode[11].GetLength() > 8)
	{
		strTemp.Format(_T("%-8s"), strCode[11].Mid(0, 8));
	}
	else
	{
		strTemp.Format(_T("%-8s"), strCode[11]);
	}
	strData += strTemp;
	if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
	{
		strTag.Format(_T("[RFID_WRITE_%s] %s"), strHead[RF_CNT], strTemp);
		clsMem.OnNormalMessageWrite(strTag);
		st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
	}

	if (strCode[12].GetLength() > 2)
	{
		strTemp.Format(_T("%-2s"), strCode[12].Mid(0, 2));
	}
	else
	{
		strTemp.Format(_T("%-2s"), strCode[12]);
	}
	strData += strTemp;
	if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
	{
		strTag.Format(_T("[RFID_WRITE_%s] %s"), strHead[RF_FLAG], strTemp);
		clsMem.OnNormalMessageWrite(strTag);
		st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
	}

	if (strCode[13].GetLength() > 2)
	{
		strTemp.Format(_T("%-2s"), strCode[13].Mid(0, 2));
	}
	else
	{
		strTemp.Format(_T("%-2s"), strCode[13]);
	}
	strData += strTemp;
	if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
	{
		strTag.Format(_T("[RFID_WRITE_%s] %s"), strHead[RF_REJECT], strTemp);
		clsMem.OnNormalMessageWrite(strTag);
		st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
	}

	if (strCode[14].GetLength() > 2)
	{
		strTemp.Format(_T("%-2s"), strCode[14].Mid(0, 2));
	}
	else
	{
		strTemp.Format(_T("%-2s"), strCode[14]);
	}
	strData += strTemp;
	if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
	{
		strTag.Format(_T("[RFID_WRITE_%s] %s"), strHead[RF_BIN], strTemp);
		clsMem.OnNormalMessageWrite(strTag);
		st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
	}

	if (strCode[15].GetLength() > 2)
	{
		strTemp.Format(_T("%-2s"), strCode[15].Mid(0, 2));
	}
	else
	{
		strTemp.Format(_T("%-2s"), strCode[15]);
	}
	strData += strTemp;
	if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
	{
		strTag.Format(_T("[RFID_WRITE_%s] %s"), strHead[RF_NONE_2], strTemp);
		clsMem.OnNormalMessageWrite(strTag);
		st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
	}

	if (strCode[16].GetLength() > 4)
	{
		strTemp.Format(_T("%-4s"), strCode[16].Mid(0, 4));
	}
	else
	{
		strTemp.Format(_T("%-4s"), strCode[16]);
	}
	strData += strTemp;
	if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
	{
		strTag.Format(_T("[RFID_WRITE_%s] %s"), strHead[RF_TRAY_QTY], strTemp);
		clsMem.OnNormalMessageWrite(strTag);
		st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
	}

	OnStringToChar(strData, (char*) byBuf);

	nLength = strData.GetLength();
	for(i=0; i<nLength; i++)
	{
		strTemp.Format(_T("%x"), byBuf[i]);
		strMake += strTemp;
	}
	
	nLength = strMake.GetLength();
	OnStringToChar(strMake, (char*) byBuf);

	for(i=0; i<nLength; i++)
	{
		nSum += byBuf[i];
	}

	nBcc = (nSum % 256);
	strTemp.Format(_T("%02x"), nBcc);
	strTemp.MakeUpper();
	nLength = strTemp.GetLength();

	strRfid.Format(_T("%s%s"), strMake, strTemp.Mid(nLength - 2, 2));
	m_strSendMsg = strRfid;

	strLog += strRfid;
	clsFunc.OnLogFileAdd(4, strLog);

	m_thrHandle = AfxBeginThread(OnClientRfidThread, this);

	return true;
}


bool	CClientRfid::OnMakeReadCodeAll(int nChann)
{
	int		i;
	int		nLength;
	int		nSum = 0;
	int		nBcc;

	CString strRfid = _T("");
	CString strMake = _T("");
	CString strLength;
	CString strTemp;
	CString strData;
	CString strLog = _T("[HANDLER >> RFID] ");

	BYTE	byBuf[100];

	m_strData	= _T("");
	
	if (m_bThread) return false;
	
	m_bThread	= true;
	m_nCommBin	= BD_NONE;

	strLength.Format(_T("%02x%02x"), m_nRfidPos[RF_CARRIER_ID],  112);
	strMake.Format(_T("%c01%2d%s"), 0x05, 80 + (nChann - 1), strLength);

	nLength = strMake.GetLength();
	OnStringToChar(strMake, (char*) byBuf);

	for(i=0; i<nLength; i++)
	{
		nSum += byBuf[i];
	}

	nBcc = (nSum % 256);
	strTemp.Format(_T("%x"), nSum);
	strTemp.MakeUpper();

	nLength = strTemp.GetLength();

	strRfid.Format(_T("%s%s"), strMake, strTemp.Mid(nLength - 2, 2));
	m_strSendMsg = strRfid;

	strLog += strRfid;
	clsFunc.OnLogFileAdd(4, strLog);

	m_thrHandle = AfxBeginThread(OnClientRfidThread, this);

	return true;
}

bool CClientRfid::OnMakeReadCode(int nCode, int nChann)
{
	int		i;
	int		nLength;
	int		nSum = 0;
	int		nBcc;

	CString strRfid = _T("");
	CString strMake = _T("");
	CString strLength;
	CString strTemp;
	CString strData;
	CString strLog = _T("[HANDLER >> RFID] ");

	BYTE	byBuf[100];

	m_strData	= _T("");

	if (m_bThread) return false;

	m_bThread	= true;
	m_nCommBin	= BD_NONE;

	strLength.Format(_T("%02x%02x"), m_nRfidPos[nCode], m_nRfidNum[nCode]);
	strMake.Format(_T("%c01%2d%s"), 0x05, 80 + (nChann - 1), strLength);

	nLength = strMake.GetLength();
	OnStringToChar(strMake, (char*) byBuf);

	for(i=0; i<nLength; i++)
	{
		nSum += byBuf[i];
	}

	nBcc = (nSum % 256);
	strTemp.Format(_T("%x"), nSum);
	strTemp.MakeUpper();

	nLength = strTemp.GetLength();

	strRfid.Format(_T("%s%s"), strMake, strTemp.Mid(nLength - 2, 2));
	m_strSendMsg = strRfid;

	strLog += strRfid;
	clsFunc.OnLogFileAdd(4, strLog);

	m_thrHandle = AfxBeginThread(OnClientRfidThread, this);

	return true;
}


bool CClientRfid::OnMakeWriteCode(int nCode, int nChann, CString strCode)
{
	int		i;
	int		nLength;
	int		nSum = 0;
	int		nBcc;

	CString strRfid = _T("");
	CString strMake = _T("");
	CString strLength;
	CString strTemp;
	CString strData;
	CString strLog = _T("[HANDLER >> RFID]");

	BYTE	byBuf[1024];

	m_strData	= _T("");
	
	if (m_bThread) return false;
	
	m_bThread	= true;
	m_nCommBin	= BD_NONE;

	strLength.Format(_T("%02x%02x"), m_nRfidPos[nCode], m_nRfidNum[nCode]);
	strMake.Format(_T("%c01%d%s"), 0x05, 90 + (nChann - 1), strLength);

	if(strCode.GetLength() > m_nRfidNum[nCode])
	{
		strData = strCode.Mid(0, m_nRfidNum[nCode]);
	}
	else
	{
		strData = strCode;
	}
	strTemp.Format(_T("%-8s"), strData);
	OnStringToChar(strTemp, (char*) byBuf);

	for(i=0; i<m_nRfidNum[nCode]; i++)
	{
		strTemp.Format(_T("%x"), byBuf[i]);
		strMake += strTemp;
	}
	
	nLength = strMake.GetLength();
	OnStringToChar(strMake, (char*) byBuf);

	for(i=0; i<nLength; i++)
	{
		nSum += byBuf[i];
	}

	nBcc = (nSum % 256);
	strTemp.Format(_T("%x"), nBcc);
	strTemp.MakeUpper();
	nLength = strTemp.GetLength();

	strRfid.Format(_T("%s%s"), strMake, strTemp.Mid(nLength - 2, 2));
	m_strSendMsg = strRfid;

	strLog += strRfid;
	clsFunc.OnLogFileAdd(4, strLog);

	m_thrHandle = AfxBeginThread(OnClientRfidThread, this);

	return true;
}
