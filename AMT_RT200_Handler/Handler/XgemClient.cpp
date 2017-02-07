#include "StdAfx.h"
#include "XgemClient.h"
#include "Variable.h"
#include "PublicFunction.h"
#include "AlgMemory.h"

CXgemClient clsXgem;

CXgemClient::CXgemClient(void)
{
	m_nXgemReq	= BIN_NOTUESD;
	m_strHeader = _T("HD");
	m_nHeader	= 6;
}


CXgemClient::~CXgemClient(void)
{
}

void CXgemClient::OnDataReceive(CString strMsg)
{
	int nHead;
	int nLength;
	int nPos;

	CString strTmp;
	CString strData;
	CString strLog = _T("[XGEM >> HANDLER] ");

	if (m_strHeader == _T(""))
	{
		strTmp = strMsg.Mid(0, m_nHeader);
	}
	else
	{
		nPos	= strMsg.Find(m_strHeader, 0);
		strTmp	= strMsg.Mid(nPos + m_strHeader.GetLength() + 1, m_nHeader);
	}
	nHead	= _wtoi(strTmp);

	// Header 
	if (nHead > 0)
	{
		nLength = strMsg.GetLength();

		if (nHead == nLength)
		{
//			strTmp = strMsg.Mid(m_nHeader, nLength);
			OnDataAnalysis(strMsg);
		}
		else
		{
			if (nHead > nLength)
			{
				m_nHeaderLength		= nHead;
				m_strData			= strMsg.Mid(m_nHeader, nLength);
			}
			else
			{
//				strTmp = strMsg.Mid(m_nHeader, nHead);
				strTmp = strMsg.Mid(0, nHead);
				OnDataAnalysis(strTmp);

				strTmp = strMsg.Mid(nHead + 1, m_nHeader);
				nHead	= _wtoi(strTmp);
				if (nHead > 0)
				{
				}
			}
		}
	}
	else
	{
		nLength = strMsg.GetLength();

		if (m_nHeaderLength > 0)
		{
			if (m_nHeaderLength == (m_strData.GetLength() + nLength))
			{
				strTmp		= m_strData + strMsg;
				OnDataAnalysis(strTmp);

				m_nHeaderLength		= 0;
				m_strData			= _T("");
			}
			else
			{
				if (m_nHeaderLength > (m_strData.GetLength() + nLength))
				{
					m_strData		+= strMsg;
				}
				else
				{
					strTmp		= m_strData + strMsg.Mid(0, nLength - (m_nHeaderLength - m_strData.GetLength()));
					OnDataAnalysis(strTmp);

					nPos	= nLength - (m_nHeaderLength - m_strData.GetLength());
					strData = strMsg.Mid(nPos, nLength - nPos);

					m_nHeaderLength		= 0;
					m_strData			= _T("");

					if (m_strHeader == _T(""))
					{
						strTmp	= strData.Mid(0, m_nHeader);
					}
					else
					{
						nPos	= strData.Find(m_strHeader, 0);
						strTmp	= strData.Mid(nPos + m_strHeader.GetLength() + 1, m_nHeader);
					}
					nHead	= _wtoi(strTmp);

					if (nHead > 0)
					{
						nLength = strData.GetLength() - m_nHeader;

						if (nHead == nLength)
						{
							strTmp = strData.Mid(m_nHeader, nLength);
							OnDataAnalysis(strTmp);
						}
						else
						{
							if (nHead > nLength)
							{
								m_nHeaderLength		= nHead;
								m_strData			= strData.Mid(m_nHeader, nLength);
							}
							else
							{
								strTmp = strData.Mid(m_nHeader, nHead);
								OnDataAnalysis(strTmp);
							}
						}
					}
					else
					{
						m_nHeaderLength		= 0;
						m_strData			= _T("");
					}
				}
			}
		}
	}

	strLog += strMsg;
	clsFunc.OnLogFileAdd(4, strLog);

	if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
	{
		strTmp.Format(_T("[XGEM] %s"), strMsg);

		clsMem.OnNormalMessageWrite(strTmp);
		st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
	}
}


void CXgemClient::OnDataAnalysis(CString strMsg)
{
	int nLength;
//	int nHead;
	int nPos[2];

	CString strFunc;
	CString strFind;
	CString strRcmd;
	CString strConnect;
	
	strFunc = OnDataItemAnalysis(strMsg, _T("FUNC"));
			
	if (strFunc == _T("INTERFACE"))
	{
		nPos[0] = strMsg.Find(_T("CONNECT="), 0);
		nPos[1] = strMsg.Find(_T(","), nPos[0]);
				
		nLength = (nPos[1] - nPos[0]) - 8;
				
		strConnect	= _T("");
		strConnect = strMsg.Mid(nPos[0] + 8, nLength);
				
		if (strConnect == _T("0"))
		{
			// off line mode
			m_nXgemInterface = MES_OFF_LINE;
			if(st_handler_info.cWndTitle != NULL)  
			{
				st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, XGEM_MODE, m_nXgemInterface);
 			}
		}
		else if(strConnect == _T("1"))
		{
			// on line local mode
			m_nXgemInterface = MES_ON_LINE_LOCAL;
			if(st_handler_info.cWndTitle != NULL)  
			{
				st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, XGEM_MODE, m_nXgemInterface);
 			}
		}
		else if(strConnect == _T("2"))
		{
			// on line remote mode
			m_nXgemInterface = MES_ON_LINE_REMOTE;
			if(st_handler_info.cWndTitle != NULL)  
			{
				st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, XGEM_MODE, m_nXgemInterface);
 			}
		}
	}
	else if (strFunc == _T("RCMD"))
	{
		strRcmd = OnDataItemAnalysis(strMsg, _T("RCMD_NUM"));
		strRcmd.MakeUpper();

		if(strRcmd == _T("START"))
		{
			m_nRunStatus = HOST_RUN;
		}
		else if(strRcmd == _T("STOP"))
		{
			m_nRunStatus = HOST_STOP;
		}
	}
}


CString	CXgemClient::OnDataItemAnalysis(CString strMsg, CString strFind)
{
	int		nPos[2];
	int		nLength;
	CString strData;
	
	nPos[0] = strMsg.Find(strFind, 0);
	nPos[1] = strMsg.Find(_T(","), nPos[0]+1);
	
	nLength	= (nPos[1] - nPos[0]) - (strFind.GetLength() + 1);
	strData = strMsg.Mid(nPos[0] + strFind.GetLength() + 1, nLength);
	
	return strData;
}


void CXgemClient::OnXgemInitialize(CString strPath)
{
	CString strHead, strFunc;

	if (st_client_info[XGEM_NETWORK].nConnect != YES)
	{
		::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + XGEM_NETWORK, CLIENT_CONNECT, 0);

		return;
	}

	strFunc = _T("FUNC=COMMUNICATION,CONFIG=");
	strFunc += strPath; 
	strFunc += _T(",");
	strHead.Format(_T("HD=%06d,"), strFunc.GetLength() + 10);

	st_client_info[XGEM_NETWORK].strSend = strHead + strFunc;
	::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + XGEM_NETWORK, CLIENT_SEND, 0);
}


void CXgemClient::OnMcInterface(int nMode)
{
	CString strHead, strFunc;
	CString strLog = _T("[HANDLER >> XGEM] ");

	if (st_client_info[XGEM_NETWORK].nConnect != YES)
	{
		return;
	}

	switch(nMode)
	{
		case MES_OFF_LINE:
			strFunc = _T("FUNC=OPERATE,CONNECT=0,");
			break;

		case MES_ON_LINE_LOCAL:
			strFunc = _T("FUNC=OPERATE,CONNECT=1,");
			break;

		case MES_ON_LINE_REMOTE:
			strFunc = _T("FUNC=OPERATE,CONNECT=2,");
			break;
	}
	
	strHead.Format(_T("HD=%06d,"), strFunc.GetLength() + 10);

	st_client_info[XGEM_NETWORK].strSend = strHead + strFunc;

	strLog += st_client_info[XGEM_NETWORK].strSend;
	clsFunc.OnLogFileAdd(4, strLog);

	::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + XGEM_NETWORK, CLIENT_SEND, 0);
}


void CXgemClient::OnMcRecipeCreate(int nMode, CString strRecipe, CString strPath)
{
	CString strHead, strFunc;
	CString strTmp;
	CString strLog = _T("[HANDLER >> XGEM] ");

	if (st_client_info[XGEM_NETWORK].nConnect != YES)
	{
		return;
	}

	strFunc = _T("FUNC=PPID");
	strFunc	+= _T(",");
	strTmp.Format(_T("PP_NAME=%s"), strRecipe);
	strFunc	+= strTmp;
	strFunc	+= _T(",");
	strFunc += _T("METHOD=1");
	strFunc	+= _T(",");
	strTmp.Format(_T("PP_PATH=%s"), strPath);
	strFunc	+= strTmp;
	strFunc	+= _T(",");

	strHead.Format(_T("HD=%06d,"), strFunc.GetLength() + 10);

	st_client_info[XGEM_NETWORK].strSend = strHead + strFunc;

	m_strOldRecipe = strRecipe;

	strLog += st_client_info[XGEM_NETWORK].strSend;
	clsFunc.OnLogFileAdd(4, strLog);
	::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + XGEM_NETWORK, CLIENT_SEND, 0);
}


void CXgemClient::OnMcRecipeSelete(CString strRecipe)
{
	CString strHead, strFunc;
	CString strTmp;
	CString strLog = _T("[HANDLER >> XGEM] ");

	if (st_client_info[XGEM_NETWORK].nConnect != YES)
	{
		return;
	}

	strFunc = _T("FUNC=CEID,CEID_NUM=1100,REPORT=1");
	strFunc += _T(",");
	strTmp.Format(_T("REPORT=1,REP_01={CNT=02,[DA_01=01,%s],[DA_02=01,%s]},"), st_basic_info.strEqp, strRecipe);
	strFunc += strTmp;
	strHead.Format(_T("HD=%06d,"), strFunc.GetLength() + 10);

	st_client_info[XGEM_NETWORK].strSend = strHead + strFunc;

	strLog += st_client_info[XGEM_NETWORK].strSend;
	clsFunc.OnLogFileAdd(4, strLog);
	::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + XGEM_NETWORK, CLIENT_SEND, 0);
}


void CXgemClient::OnMcStatus(int nStatus)
{
	CString strHead, strFunc;
	CString strTmp;
	CString strStatus;
	CString strLog = _T("[HANDLER >> XGEM] ");

	if (st_client_info[XGEM_NETWORK].nConnect != YES)
	{
		return;
	}

	switch (nStatus)
	{
		case dSTOP:
			strStatus = _T("0");
			break;

		case dRUN:
			strStatus = _T("1");
			break;

		case dIDLE:
			strStatus = _T("4");
			break;

		case dWARNING:
			strStatus = _T("2");
			break;
	}

	strFunc = _T("FUNC=CEID,CEID_NUM=1001,REPORT=1");
		strFunc += _T(",");
		strTmp.Format(_T("REPORT=1,REP_01={CNT=02,[DA_01=01,%s],[DA_02=01,%s]},"), st_basic_info.strEqp, strStatus);
		strFunc += strTmp;
		strHead.Format(_T("HD=%06d,"), strFunc.GetLength() + 10);
	
		st_client_info[XGEM_NETWORK].strSend = strHead + strFunc;
	
		strLog += st_client_info[XGEM_NETWORK].strSend;
		clsFunc.OnLogFileAdd(4, strLog);
		::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + XGEM_NETWORK, CLIENT_SEND, 0);
}


void CXgemClient::OnMcLotStart(CString strLotNo, CString strPartNo)
{
	CString strHead, strFunc;
	CString strTmp;
	CString strLog = _T("[HANDLER >> XGEM] ");

	if (st_client_info[XGEM_NETWORK].nConnect != YES)
	{
		return;
	}

	strFunc = _T("FUNC=CEID,CEID_NUM=1002,REPORT=1");
	strFunc += _T(",");
	strTmp.Format(_T("REP_01={CNT=03,[DA_01=01,%s],[DA_02=01,%s],[DA_03=01,%s]},"), st_basic_info.strEqp, strPartNo, strLotNo);
	strFunc += strTmp;
	strHead.Format(_T("HD=%06d,"), strFunc.GetLength() + 10);

	st_client_info[XGEM_NETWORK].strSend = strHead + strFunc;

	strLog += st_client_info[XGEM_NETWORK].strSend;
	clsFunc.OnLogFileAdd(4, strLog);
	::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + XGEM_NETWORK, CLIENT_SEND, 0);
}


void CXgemClient::OnMcLotEnd(CString strLotNo, CString strPartNo, int nTotal, int nPass, int nFail, COleDateTimeSpan time)
{
	CString strHead, strFunc;
	CString strTmp;
	CString strData;
	CString strLog = _T("[HANDLER >> XGEM] ");

	if (st_client_info[XGEM_NETWORK].nConnect != YES)
	{
		return;
	}

	strFunc = _T("FUNC=CEID,CEID_NUM=1003,REPORT=1");
	strFunc += _T(",");
	
//	strTmp.Format(_T("REP_01={CNT=07,[DA_01=01,%s],[DA_02=01,%s],[DA_03=01,%s],[DA_04=01,%d],[DA_05=01,%d],[DA_06=01,%d],[DA_07=01,%d]},"), st_basic_info.strEqp,
//																																			strPartNo, 
//																																			strLotNo,
//																																			time.GetTotalMinutes(),
//																																			nTotal,
//																																			nPass,
//			
//																																nFail);
	strTmp.Format(_T("REP_01={CNT=07,[DA_01=01,%s]"), st_basic_info.strEqp);
	strFunc += strTmp;
	strFunc += _T(",");

	strTmp.Format(_T("[DA_02=01,%s]"), strPartNo);
	strFunc += strTmp;
	strFunc += _T(",");

	strTmp.Format(_T("[DA_03=01,%s]"), strLotNo);
	strFunc += strTmp;
	strFunc += _T(",");

	strTmp.Format(_T("[DA_04=01,%d]"), time.GetTotalMinutes());
	strFunc += strTmp;
	strFunc += _T(",");

	strTmp.Format(_T("[DA_05=01,%d]"), nTotal);
	strFunc += strTmp;
	strFunc += _T(",");

	strTmp.Format(_T("[DA_06=01,%d]"), nPass);
	strFunc += strTmp;
	strFunc += _T(",");

	strTmp.Format(_T("[DA_07=01,%d]},"), nFail);
	strFunc += strTmp;

	strHead.Format(_T("HD=%06d,"), strFunc.GetLength() + 10);

	

	st_client_info[XGEM_NETWORK].strSend = strHead + strFunc;

	strLog += st_client_info[XGEM_NETWORK].strSend;
	clsFunc.OnLogFileAdd(4, strLog);
	::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + XGEM_NETWORK, CLIENT_SEND, 0);
}


void CXgemClient::OnMcAlarm(CString strCode, int nStatus)
{
	CString strHead, strFunc;
	CString strLog = _T("[HANDLER >> XGEM] ");

	if (st_client_info[XGEM_NETWORK].nConnect != YES)
	{
		return;
	}
	
	strFunc.Format(_T("FUNC=ALARM,CODE=%s,STATUS=%d,"), strCode,nStatus);
	strHead.Format(_T("HD=%06d,"), strFunc.GetLength() + 10);
	
	st_client_info[XGEM_NETWORK].strSend = strHead + strFunc;

	strLog += st_client_info[XGEM_NETWORK].strSend;
	clsFunc.OnLogFileAdd(4, strLog);
	::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + XGEM_NETWORK, CLIENT_SEND, 0);
}


void CXgemClient::OnMcPCBInfo(CString strBarcode)
{
	CString strHead, strFunc;
	CString strTmp;
	CString strLog = _T("[HANDLER >> XGEM] ");

	CTime time;

	if (st_client_info[XGEM_NETWORK].nConnect != YES)
	{
		return;
	}

	time = CTime::GetCurrentTime();

	strFunc = _T("FUNC=CEID,CEID_NUM=1003,REPORT=1");
	strFunc += _T(",");
	strTmp.Format(_T("REPORT=1,REP_01={CNT=03,[DA_01=01,%04d%02d%02d%02d%02d%02d%02d],[DA_02=01,%s],[DA_03=01,%s]},"), time.GetYear(),
																													   time.GetMonth(),
																													   time.GetDay(), 
																													   time.GetHour(),
																													   time.GetMinute(),
																													   time.GetSecond(), 
																													   0,
																													   st_basic_info.strEqp,
																													   strBarcode);
	strFunc += strTmp;
	strHead.Format(_T("HD=%06d,"), strFunc.GetLength() + 10);
	st_client_info[XGEM_NETWORK].strSend = strHead + strFunc;

	strLog += st_client_info[XGEM_NETWORK].strSend;
	clsFunc.OnLogFileAdd(4, strLog);
	::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + XGEM_NETWORK, CLIENT_SEND, 0);
}
