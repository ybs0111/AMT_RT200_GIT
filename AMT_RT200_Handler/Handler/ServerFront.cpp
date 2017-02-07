#include "StdAfx.h"
#include "ServerFront.h"
#include "Variable.h"
#include "AlgMemory.h"
#include "ServerSocket.h"
#include "CtlBd_Library.h"
#include "Ctlbd_Variable.h"
#include "PublicFunction.h"
#include "XgemClient.h"
#include "FtpComm.h"
#include "MyBasicData.h"
#include "RunHandlerCheck.h"

CServerFront clsFront;
#define FRONT_SERVER_MSG_NAME		"FRONT_SERVER_SHAREMEM"

CServerFront::CServerFront(void)
{
	// 공유 메모리 선언...../////////////////////////////////////////////////////////////////////////////////////////
	m_hMemMapFront	= NULL;
	m_pQueueFront	= NULL;

	m_hMemMapFront	= CreateFileMapping(INVALID_HANDLE_VALUE,
										NULL,
										PAGE_READWRITE,
										0,
										sizeof(FRONT_SV_QUEUE),
										(LPCWSTR)FRONT_SERVER_MSG_NAME);
	if (m_hMemMapFront != NULL)
	{	
		if (GetLastError() != ERROR_ALREADY_EXISTS)
		{
			m_pQueueFront = (P_FRONT_SV_QUEUE)MapViewOfFile(m_hMemMapFront, FILE_MAP_ALL_ACCESS, 0, 0, 0);
			if (m_pQueueFront != NULL)
			{
				m_pQueueFront->m_nFront	= 0;
				m_pQueueFront->m_nRear	= 0;
				m_pQueueFront->m_nCount	= 0;
			}
		}
	}

	m_nHeader	=	16;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// jtkim 20150514 ftp scrap code
//	m_pFtp = NULL;
//	m_pFtp = new CFtpComm;
//	m_pFtp->OnScrapCodeDownload(0, st_path_info.strPathFtpGms, _T("RETEST_RULE.TXT"));
}


CServerFront::~CServerFront(void)
{
	if (m_pQueueFront != NULL)	UnmapViewOfFile(m_pQueueFront);	
	if (m_hMemMapFront != NULL) CloseHandle(m_hMemMapFront);
}

CString	CServerFront::OnCharToString(char chData[], int nLength)
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

void CServerFront::OnStringToChar(CString strData, char chData[])
{
	int nCnt;
	wchar_t *chBuf;

	chBuf = strData.GetBuffer(strData.GetLength());
	nCnt  = WideCharToMultiByte(CP_ACP, 0, chBuf, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, chBuf, -1, chData, nCnt, 0, 0);
}

void CServerFront::OnMessageWrite(CString strMsg)
{
	CString strLog = _T("[HANDLER >> FRONT] ");

	if (m_hMemMapFront != NULL && m_pQueueFront != NULL)
	{
		strLog += strMsg;
		clsFunc.OnLogFileAdd(4, strLog);

		OnStringToChar(strMsg, m_pQueueFront->m_chMsg[m_pQueueFront->m_nRear]);

		m_pQueueFront->m_nRear++;
		
		if (m_pQueueFront->m_nRear >= 100) m_pQueueFront->m_nRear = 0;
		
		m_pQueueFront->m_nCount++;
	}
}

CString	CServerFront::OnMessageRead()
{
	CString strData = _T("");
	
	if (m_hMemMapFront != NULL && m_pQueueFront != NULL)
	{
		if (m_pQueueFront->m_nCount > 0 )
		{
			strData = m_pQueueFront->m_chMsg[m_pQueueFront->m_nFront];
			
			m_pQueueFront->m_nFront++;
			if (m_pQueueFront->m_nFront >= 100) m_pQueueFront->m_nFront = 0;
			
			m_pQueueFront->m_nCount--;
		}
	}

	return strData;
}

void CServerFront::OnDataReceive(CString strMsg)
{
	int nHead;
	int nLength;
	int nPos;

	CString strTemp;
	CString strData;
	CString strLog = _T("[FRONT >> HANDLER] ");

	if (m_strHeader == _T(""))
	{
		strTemp = strMsg.Mid(0, m_nHeader);
	}
	else
	{
		nPos	= strMsg.Find(m_strHeader, 0);
		strTemp	= strMsg.Mid(nPos + m_strHeader.GetLength() + 1, m_nHeader);
	}
	nHead	= _wtoi(strTemp);

	// Header 
	if (nHead > 0)
	{
		nLength = strMsg.GetLength() - m_nHeader;

		if (nHead == nLength)
		{
			strTemp = strMsg.Mid(m_nHeader, nLength);
			//OnDataAnalysis(strTemp);
			OnMessageWrite(strTemp);
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
				strTemp = strMsg.Mid(m_nHeader, nHead);
				//OnDataAnalysis(strTemp);
				OnMessageWrite(strTemp);
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
				strTemp		= m_strData + strMsg;
				//OnDataAnalysis(strTemp);
				OnMessageWrite(strTemp);

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
					strTemp		= m_strData + strMsg;

					if (m_nHeaderLength < strTemp.GetLength())
					{
						OnDataAnalysis(strTemp);

						nPos	= nLength - (m_nHeaderLength - m_strData.GetLength());
						strData = strMsg.Mid(nPos, nLength - nPos);

						m_nHeaderLength		= 0;
						m_strData			= _T("");
					}
					else
					{
						m_strData += strTemp;
						if (m_strData.GetLength() > 4096)
						{
							m_nHeaderLength		= 0;
							m_strData			= _T("");
						}
					}
/*
					if (m_strHeader == _T(""))
					{
						strTemp	= strData.Mid(0, m_nHeader);
					}
					else
					{
						nPos	= strData.Find(m_strHeader, 0);
						strTemp	= strData.Mid(nPos + m_strHeader.GetLength() + 1, m_nHeader);
					}
					nHead	= _wtoi(strTemp);

					if (nHead > 0)
					{
						nLength = strData.GetLength() - m_nHeader;

						if (nHead == nLength)
						{
							strTemp = strData.Mid(m_nHeader, nLength);
							//OnDataAnalysis(strTemp);
							OnMessageWrite(strTemp);
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
								strTemp = strData.Mid(m_nHeader, nHead);
								//OnDataAnalysis(strTemp);
								OnMessageWrite(strTemp);
							}
						}
					}
					else
					{
						m_nHeaderLength		= 0;
						m_strData			= _T("");
					}
*/
				}
			}
		}
	}

	strLog += strMsg;
	clsFunc.OnLogFileAdd(4, strLog);

	if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
	{
		strTemp.Format(_T("[FRONT_NETWORK] %s"), strMsg);
		clsMem.OnNormalMessageWrite(strTemp);

		st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
	}
}


void CServerFront::OnDataAnalysis(CString strMsg)
{
	CString strFunc;
	CString strTemp;
	CString strData;
	CString strRpy;
	CString strLotID;
	CString strPartID;
	CString strEqp;
	CString strSerial;
	CString strProcess;
	CString strTray;
	CString strEmpty;
	CString strLast;
	CString strProperty;
	CString strBypass;
	CString strArray;
	CString strPPID;
	CString strWWN;
	CString strCSerial;
	CString strPSID;
	CString strBin;
	CString strMode;
	CString strCSerial2;
	CString strRecipe;

	int nX = 0, nY = 0;

	if(st_var.nCokBuffer_Change_End_Flag != 0) //2015.0407 james COK 교체 작업이 시작 않했을때만 들어온다 
	{
		if (st_handler_info.cWndList != NULL)  
		{
			clsMem.OnNormalMessageWrite(_T("COK Change Status..."));
			st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
		}
		return;
	}

	strFunc = OnDataItemAnalysis(0, 0, strMsg, _T("FUNCTION"));

	if (strFunc == _T("NEW_LOT_IN"))
	{
		
	}
	else if (strFunc == _T("BUFFER_IN"))
	{
		
	}
	else if (strFunc == _T("LOT_FINISH"))
	{
	
	}
	else if (strFunc == _T("RECIPE_INFO"))
	{
		// jtkim auto recipe 변경작업
		// 20160122
		if (st_lot_info[LOT_CURR].nLotStatus != LOT_CLEAR && st_lot_info[LOT_NEXT].nLotStatus != LOT_CLEAR)
		{
			strData = strMsg;
			strData.Replace(_T("FUNCTION"), _T("FUNCTION_RPY"));
			strData += _T(" ");
			strData = _T("RESULT=FAIL");
			strData += _T(" ");
			strData += _T("MSG=LOT START");
		}
		else
		{
			strPartID	= OnDataItemAnalysis(0, 0, strMsg, _T("PARTID"));
			strRecipe	= clsFunc.OnRecipeCheck(strPartID);

			if (strRecipe == _T(""))
			{
				strData = strMsg;
				strData.Replace(_T("FUNCTION"), _T("FUNCTION_RPY"));
				strData += _T(" ");
				strData = _T("RESULT=FAIL");
				strData += _T(" ");
				strData += _T("MSG=RECIPE FAIL");

				if (st_handler_info.cWndMain != NULL)
				{
					st_other_info.strBoxMsg = _T("Recipe Check Error. \r\n Plase Change The Recipe");
					st_handler_info.cWndMain->SendMessage(WM_WORK_COMMAND, MAIN_MESSAGE_BOX_CREATE_REQ, 0);
				}

				clsFunc.OnMCStop();
			}
			else
			{
				strData = strMsg;
				strData.Replace(_T("FUNCTION"), _T("FUNCTION_RPY"));
				strData += _T(" ");
				strData = _T("RESULT=PASS");

				if (st_basic_info.strDeviceName != strRecipe)
				{
					st_basic_info.strDeviceName	= strRecipe;
			
					clsBasic.OnBasic_Data_Load(1);
					clsBasic.OnModuleRobot_Teach_Data_Load(st_basic_info.strDeviceName);
					clsBasic.OnMaintenance_Data_Load();
					clsBasic.OnWaitTime_Data_Load();
					clsBasic.OnMotorSpeed_Set_Data_Load();
					clsBasic.OnInterface_Data_Load();
					clsBasic.OnRecipe_Data_Load(st_basic_info.strDeviceName);

					if (st_handler_info.nRunStatus == dRUN)
					{
						clsFunc.OnMCStop();
					}
				}

				if (st_handler_info.cWndTitle != NULL)
				{
					st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, MODEL_MODE, 0);
				}
			}
		}

		strRpy.Format(_T("%016d"), strData.GetLength());
		st_server_info[FRONT_NETWORK].strSend = strRpy;
		::SendMessage(st_handler_info.hWnd, WM_SERVER_MSG + FRONT_NETWORK, SERVER_SEND, 0);

		st_server_info[FRONT_NETWORK].strSend = strData;
		::SendMessage(st_handler_info.hWnd, WM_SERVER_MSG + FRONT_NETWORK, SERVER_SEND, 0);
	}
}


CString CServerFront::OnDataItemAnalysis(int nOpt, int nPos, CString strMsg, CString strFind)
{
	CString strData = _T("");
	CString strHead;
	int length;
	int pos[4];

	length = strMsg.GetLength();

	pos[0] = -1;
	pos[0] = strMsg.Find(strFind, nPos);

	if(pos[0] < 0) return strData;

	pos[1] = -1;
	pos[1] = strMsg.Find(_T("="), pos[0]+1);
	if(pos[1] < 0) return strData;

	pos[2] = -1;
	pos[2] = strMsg.Find(_T(" "), pos[0]+1);

	if(nOpt == 0)
	{
		if(pos[2] == -1)
		{
			strData = strMsg.Mid(pos[1]+1, length-(pos[1]+1));	
		}
		else
		{
			strData = strMsg.Mid(pos[1]+1, pos[2]-(pos[1]+1));
		}
	}
	else if(nOpt == 1)
	{
		strData = strMsg.Mid(pos[0], length-(pos[0]));
	}
	else if(nOpt == 2)
	{
		pos[2] = -1;
		pos[2] = strMsg.Find(_T(")"), pos[1]+1);

		strData = strMsg.Mid(pos[1]+1, pos[2]-(pos[1]));
	}
	else if (nOpt == 3)
	{
		strHead.Format(_T("%c"), '"');

		pos[2] = strMsg.Find(strHead, 0);
		pos[3] = strMsg.Find(strHead, pos[2]+1);

		if (pos[2] > 0 && pos[3] > 0)
		{
			strData = strMsg.Mid(pos[2]+1, pos[3]-(pos[2]) - 1);
		}
		else
		{
			strData = _T("Message Fail");
		}
	}

	return strData;
}

void CServerFront::OnCommunication()
{
	CString strMsg;

	if (m_pQueueFront != NULL)
	{
		if (m_pQueueFront->m_nCount > 0)
		{
			strMsg	= OnMessageRead();

			OnDataAnalysis(strMsg);
		}
	}
}