#include "StdAfx.h"
#include "ClientGms.h"
#include "Variable.h"
#include "AlgMemory.h"
#include "PublicFunction.h"
#include "ClientSocket.h"
#include "CtlBd_Library.h"
#include "Ctlbd_Variable.h"

#define GMS_CLIENT_MSG_NAME		"GMS_CLIENT_SHAREMEM"
#define STX						0x02
#define ETX						0x03

CClientGms clsGms;

CClientGms::CClientGms(void)
{
	m_thrHandle = NULL;

	m_nCommStep	=	0;
	m_nRetry	=	0;
	m_nAsdFlag	=	0;
	m_nTimFlag	=	0;
	m_nPngFlag	=	0;

	//ZeroMemory(m_gms_info, sizeof(m_gms_info));
}


CClientGms::~CClientGms(void)
{
}


void CClientGms::OnCommunication()
{
	CString strTemp;
	CTime	time;
	
	switch(m_nCommStep)
	{
		case 0:
			if (m_nTimFlag != TRUE || m_nAsdFlag != TRUE || m_nPngFlag != FALSE)
			{
				if (m_nTimFlag != TRUE)
				{
					if (st_client_info[GMS_NETWORK].nConnect == YES)
					{
						m_nCommStep = 1000;
					}
					else
					{
						m_nRetry		= 0;
						m_dwWaitTime[0] = GetTickCount();

						::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + GMS_NETWORK, CLIENT_CONNECT, 0);

						m_nCommStep = 100;
					}
				}
				if (m_nAsdFlag != TRUE)
				{
					if (st_client_info[GMS_NETWORK].nConnect == YES)
					{
						m_nCommStep = 2000;
					}
					else
					{
						m_nRetry		= 0;
						m_dwWaitTime[0] = GetTickCount();

						::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + GMS_NETWORK, CLIENT_CONNECT, 0);

						m_nCommStep = 100;
					}
				}
				if (m_nPngFlag != FALSE)
				{
					if (st_client_info[GMS_NETWORK].nConnect == YES)
					{
						m_nCommStep = 3000;
					}
					else
					{
						m_nRetry		= 0;
						m_dwWaitTime[0]	= GetTickCount();

						::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + GMS_NETWORK, CLIENT_CONNECT, 0);

						m_nCommStep = 100;
					}
				}
			}

			break;

		case 100:
			if (st_client_info[GMS_NETWORK].nConnect == YES)
			{
				m_nCommStep = 0;
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

				if (m_dwWaitTime[2] > WAIT_CONNECT)
				{
					m_nRetry++;

					if (m_nRetry > 3)
					{
						::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + GMS_NETWORK, CLIENT_CLOSE, 0);

						m_dwWaitTime[0] = GetTickCount();
						CTL_Lib.Alarm_Error_Occurrence(20000, dWARNING, _T("630000"));

						m_nCommStep = 200;
					}
					else
					{
						m_dwWaitTime[0] = GetTickCount();


						m_nCommStep = 200;
					}
				}
			}
			break;

		case 200:
			m_dwWaitTime[1] = GetTickCount();
			m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];

			if (m_dwWaitTime[2] < 0)
			{
				m_dwWaitTime[0] = GetTickCount();
				break;
			}

			if (m_dwWaitTime[2] > WAIT_CLOSE)
			{
				m_nCommStep = 0;
			}
			break;

		case 1000:
			m_nRetry		= 0;
			m_dwWaitTime[0]	=	GetTickCount();

			st_client_info[GMS_NETWORK].strSend	=	OnSyncTime();

			::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + GMS_NETWORK, CLIENT_SEND, 0);

			m_nCommStep	= 1100;
			break;

		case 1100:
			m_dwWaitTime[1] = GetTickCount();
			m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];

			if (m_nTimFlag == TRUE)
			{
				//::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + GMS_NETWORK, CLIENT_CLOSE, 0);

				m_nCommStep = 0;
			}
			else
			{
				if (m_dwWaitTime[2] < 0)
				{
					m_dwWaitTime[0] = GetTickCount();
				}
				else if (m_dwWaitTime[2] >= WAIT_RCV)
				{
					m_nRetry++;

					if (m_nRetry > 3)
					{
						::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + GMS_NETWORK, CLIENT_CLOSE, 0);

						m_dwWaitTime[0] = GetTickCount();
						CTL_Lib.Alarm_Error_Occurrence(20000, dWARNING, _T("630001"));

						m_nCommStep = 1200;
					}
					else
					{
						m_dwWaitTime[0] = GetTickCount();

						::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + GMS_NETWORK, CLIENT_CLOSE, 0);

						m_nCommStep = 1200;
					}
				}
			}
			break;

		case 1200:
			m_dwWaitTime[1] = GetTickCount();
			m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];

			if (m_dwWaitTime[2] < 0)
			{
				m_dwWaitTime[0] = GetTickCount();
				break;
			}

			if (m_dwWaitTime[2] > WAIT_CLOSE)
			{
				m_nCommStep = 0;
			}
			break;

		case 2000:
			m_nRetry		= 0;
			m_dwWaitTime[0]	=	GetTickCount();

			st_client_info[GMS_NETWORK].strSend	=	OnAutoSendReq();

			::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + GMS_NETWORK, CLIENT_SEND, 0);

			m_nCommStep	= 2100;
			break;

		case 2100:
			m_dwWaitTime[1] = GetTickCount();
			m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];

			if (m_nAsdFlag == TRUE)
			{
				//::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + GMS_NETWORK, CLIENT_CLOSE, 0);

				m_nCommStep = 0;
			}
			else
			{
				if (m_dwWaitTime[2] < 0)
				{
					m_dwWaitTime[0] = GetTickCount();
				}
				else if (m_dwWaitTime[2] >= WAIT_RCV)
				{
					m_nRetry++;

					if (m_nRetry > 3)
					{
						if (m_nRetry > 3)
						{
							::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + GMS_NETWORK, CLIENT_CLOSE, 0);

							m_dwWaitTime[0] = GetTickCount();
							CTL_Lib.Alarm_Error_Occurrence(20000, dWARNING, _T("630001"));

							m_nCommStep = 2200;
						}
						else
						{
							m_dwWaitTime[0] = GetTickCount();


							m_nCommStep = 2200;
						}
					}
				}
			}
			break;

		case 2200:
			m_dwWaitTime[1] = GetTickCount();
			m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];

			if (m_dwWaitTime[2] < 0)
			{
				m_dwWaitTime[0] = GetTickCount();
				break;
			}

			if (m_dwWaitTime[2] > WAIT_CLOSE)
			{
				m_nCommStep = 0;
			}
			break;

		case 3000:
			m_nRetry		= 0;
			m_dwWaitTime[0]	=	GetTickCount();

			strTemp.Format(_T("%c%s%c"), STX, _T("PNGOK"), ETX);
			
			st_client_info[GMS_NETWORK].strSend	= strTemp;	
			
			::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + GMS_NETWORK, CLIENT_SEND, 0);

			m_nCommStep	= 3100;

			break;

		case 3100:
			m_nPngFlag		= FALSE;

			m_dwWaitTime[0] = GetTickCount();

			//::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + GMS_NETWORK, CLIENT_CLOSE, 0);

			m_nCommStep = 3200;
			break;

		case 3200:
			m_dwWaitTime[1] = GetTickCount();
			m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];

			if (m_dwWaitTime[2] < 0)
			{
				m_dwWaitTime[0] = GetTickCount();
				break;
			}

			if (m_dwWaitTime[2] > WAIT_CLOSE)
			{
				m_nCommStep = 0;
			}
			break;
	}
}

void CClientGms::OnMessageWrite(CString strMsg)
{

}

CString	CClientGms::OnMessageRead()
{
	CString strData = _T("");

	

	return strData;
}

void CClientGms::OnDataReceive(CString strMsg)
{
	int nHead =0;
	int nLength =0;
	int nPos =0;
	int nCh =0;
	int	i = 0;

	CString strTemp;
	CString strData;
	CString	strBuff[10];
	CString strLog = _T("");
	
	strMsg.TrimLeft(STX);
	strMsg.TrimRight(ETX);

	if (strMsg.GetLength() > 5)
	{
		strData	=	strMsg.Mid(0, 3);

		if (strData	==	_T("EOS"))
		{
			for (i=0; i<6; i++)
			{
				AfxExtractSubString(strBuff[i], strMsg, i, _T(','));
			}

			nCh			=	_wtoi(strBuff[2]);

			strTemp		=	OnDateFormat(strBuff[5]);
			
			st_gms_info.strEqpID[nCh-1]		=	strBuff[1];
			st_gms_info.strValVolt[nCh-1]	=	strBuff[3];
			st_gms_info.strValRes[nCh-1]	=	strBuff[4];
			st_gms_info.strDateTime[nCh-1]	=	strTemp;

			st_handler_info.cWndFtp->SendMessage(WM_WORK_DISPLAY, nCh-1);
		}
	}
	else
	{
		if (strMsg == _T("EOSOK"))
		{
			m_nTimFlag	=	TRUE;
			m_nAsdFlag	=	FALSE;
		}
		else if (strMsg == _T("ASDOK"))
		{
			m_nAsdFlag	=	TRUE;
		}
		else if (strMsg == _T("PNG  "))
		{
			m_nPngFlag	=	TRUE;
		}
		else
		{

		}
	}

	strData.Format(_T("[TTS_VNR_NETWORK] %s Receive"), strMsg);
	//clsFunc.OnStringToChar(strMsg, st_other_info.cNormalMsg);
	clsMem.OnNormalMessageWrite(strData);
	st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
}

void CClientGms::OnDataAnalysis(CString strMsg)
{

}

CString CClientGms::OnDataItemAnalysis(int nOpt, int nPos, CString strMsg, CString strFind)
{
	CString strData = _T("");
	CString strHead;
//	int length;
//	int pos[4];

	return strData;
}

// 설비 <-> TTS VnR 시간 동기화
CString CClientGms::OnSyncTime()
{
	CString strData, strTemp;
	CTime	time;

	time	= CTime::GetCurrentTime();	
		
	strTemp.Format(_T("TIM  %04d%02d%02d%02d%02d%02d"),	time.GetYear(), time.GetMonth(), time.GetDay(), 
													time.GetHour(), time.GetMinute(), time.GetSecond());
	
	strData.Format(_T("%c%s%c"), STX, strTemp, ETX);
		
	return strData;
}


// 자동 전송 요청
CString CClientGms::OnAutoSendReq()
{
	CString strData;
		
	strData.Format(_T("%c%s%c"), STX, _T("ASD  "), ETX);

	return strData;
}

CString CClientGms::OnDateFormat(CString strDate)
{
	CString strTemp;

	strDate.Insert(4, _T("-"));
	strDate.Insert(7, _T("-"));
	strDate.Insert(10, _T(" "));
	strDate.Insert(13, _T(":"));
	strDate.Insert(16, _T(":"));

	return strDate;
}