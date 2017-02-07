#include "StdAfx.h"
#include "ServerVnR.h"
#include "Variable.h"
#include "AlgMemory.h"
#include "ServerSocket.h"
#include "CtlBd_Library.h"
#include "Ctlbd_Variable.h"
#include "PublicFunction.h"

CServerVnR clsVnR;

CServerVnR::CServerVnR(void)
{
	int i, j;

	for (i=0; i<10; i++)
	{
		for (j=0; j<10; j++)
		{
			m_dVolt[i][j]		= 0.0f;
			m_dResistance[i][j]	= 0.0f;
		}
	}

	m_nDataCount = 0;
}


CServerVnR::~CServerVnR(void)
{
}

void CServerVnR::OnDataReceive(CString strMsg)
{
	CString strFunc;
	CString strData;
	CString strTemp;
	CString strLog = _T("[VnR >> HANDLER]");

	int	nLength;
	int nPos[3];
	int nMcID;
	int nPort;
	double dVolt, dResistance;

	nLength = strMsg.GetLength();

	strData = strMsg.Mid(1, nLength-1);

	nPos[0] = -1;
	nPos[0] = strData.Find(_T("EOSOK"), 0);

	if (nPos[0] >= 0)
	{
		OnMakeAsd();
	}
	else
	{
		nPos[0] = -1;
		nPos[0] = strData.Find(_T("PNG"), 0);

		if (nPos[0] >= 0)
		{
			OnMakePngok();
		}
		else 
		{
			nPos[0] = -1;
			nPos[0] = strData.Find(_T("EOS"), 0);

			if (nPos[0] >= 0)
			{
				nPos[0] = 0;
				nPos[1] = strData.Find(_T(","), nPos[0]);
				nPos[2] = strData.Find(_T(","), nPos[1] + 1);
				strTemp	= strData.Mid(nPos[1] + 1, nPos[2] - (nPos[1] + 1));
				nMcID	= _wtoi(strTemp);

				nPos[0] = nPos[2];
				nPos[1] = strData.Find(_T(","), nPos[0]);
				nPos[2] = strData.Find(_T(","), nPos[1] + 1);
				strTemp	= strData.Mid(nPos[1] + 1, nPos[2] - (nPos[1] + 1));
				nPort	= _wtoi(strTemp);

				nPos[0]		= nPos[2];
				nPos[1]		= strData.Find(_T(","), nPos[0]);
				nPos[2]		= strData.Find(_T(","), nPos[1] + 1);
				strTemp		= strData.Mid(nPos[1] + 1, nPos[2] - (nPos[1] + 1));
				dVolt		= _wtof(strTemp);

				nPos[0]		= nPos[2];
				nPos[1]		= strData.Find(_T(","), nPos[0]);
				nPos[2]		= strData.Find(_T(","), nPos[1] + 1);
				strTemp		= strData.Mid(nPos[1] + 1, nPos[2] - (nPos[1] + 1));
				dResistance	= _wtof(strTemp);

				if (nPort > 0 && nPort < 10)
				{
					m_dVolt[nPort - 1][m_nDataCount]		= dVolt;
					m_dResistance[nPort - 1][m_nDataCount]	= dResistance;

					m_nDataCount++;
					if (m_nDataCount > 60)
					{
						m_nDataCount = 0;
					}
				}
			}
		}
	}
	strLog += strMsg;
	clsFunc.OnLogFileAdd(4, strLog);
}

void CServerVnR::OnMakeTime()
{
	CTime time = CTime::GetCurrentTime();
	CString strData;

	strData.Format(_T("%cTIM  %04d%02d%02d%02d%02d%02d%c"), 0x02,
													        time.GetYear(),
													        time.GetMonth(),
													        time.GetDay(),
													        time.GetHour(),
													        time.GetMinute(),
													        time.GetSecond(),
													        0x03);
 
	st_server_info[GMS_NETWORK].strSend = strData;

	::SendMessage(st_handler_info.hWnd, WM_SERVER_MSG + GMS_NETWORK, SERVER_SEND, 0);
}

void CServerVnR::OnMakeAsd()
{
	CString strData;
	CString strLog = _T("[HANDLER >> VnR] ");

	strData.Format(_T("%cASD %c"), 0x02,
								   0x03);

	st_server_info[GMS_NETWORK].strSend = strData;

	strLog += strData;
	clsFunc.OnLogFileAdd(4, strLog);

	::SendMessage(st_handler_info.hWnd, WM_SERVER_MSG + GMS_NETWORK, SERVER_SEND, 0);
}

void CServerVnR::OnMakePngok()
{
	CString strData;
	CString strLog = _T("[HANDLER >> VnR] ");

	strData.Format(_T("%cPNGOK%c"), 0x02,
								   0x03);

	st_server_info[GMS_NETWORK].strSend = strData;

	strLog += strData;
	clsFunc.OnLogFileAdd(4, strLog);
	::SendMessage(st_handler_info.hWnd, WM_SERVER_MSG + GMS_NETWORK, SERVER_SEND, 0);
}