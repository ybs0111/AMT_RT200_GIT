nTestEndBin
nTestEndBin
nTestEndBin
nTestEndBin
#include "StdAfx.h"
#include "ClientEcFirst.h"
#include "Variable.h"
#include "AlgMemory.h"
#include "ClientSocket.h"
#include "CtlBd_Library.h"
#include "Ctlbd_Variable.h"
#include "PublicFunction.h"

#define EC_FIRST_CLIENT_MSG_NAME		"EC_FIRST_CLIENT_SHAREMEM"

CClientEcFirst clsEcFirst;

UINT OnEcFirstClientThread(LPVOID lParam);

CClientEcFirst::CClientEcFirst(void)
{
	m_thrHandle = NULL;

	// 공유 메모리 선언...../////////////////////////////////////////////////////////////////////////////////////////
	m_hMemMapEcFirst	= NULL;
	m_pQueueEcFirst		= NULL;

	m_hMemMapEcFirst	= CreateFileMapping(INVALID_HANDLE_VALUE,
										    NULL,
										    PAGE_READWRITE,
										    0,
										    sizeof(EC_F_CLIENT_QUEUE),
										    (LPCWSTR)EC_FIRST_CLIENT_MSG_NAME);
	if (m_hMemMapEcFirst != NULL)
	{	
		if (GetLastError() != ERROR_ALREADY_EXISTS)
		{	
			m_pQueueEcFirst = (P_EC_F_CLIENT_QUEUE)MapViewOfFile(m_hMemMapEcFirst, FILE_MAP_ALL_ACCESS, 0, 0, 0);
			if (m_pQueueEcFirst != NULL)
			{
				m_pQueueEcFirst->m_nFront	= 0;
				m_pQueueEcFirst->m_nRear	= 0;
				m_pQueueEcFirst->m_nCount	= 0;
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_nCommStep = 0;
	m_strHeader = _T("");
	m_nHeader	= 16;
}


CClientEcFirst::~CClientEcFirst(void)
{
	if (m_pQueueEcFirst		!= NULL)	UnmapViewOfFile(m_pQueueEcFirst);	
	if (m_hMemMapEcFirst	!= NULL)	CloseHandle(m_hMemMapEcFirst);
}

void CClientEcFirst::OnOpen()
{
	m_hMemMapEcFirst	= CreateFileMapping(INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(EC_F_CLIENT_QUEUE),
		(LPCWSTR)EC_FIRST_CLIENT_MSG_NAME);
	if (m_hMemMapEcFirst != NULL)
	{	
		if (GetLastError() != ERROR_ALREADY_EXISTS)
		{	
			m_pQueueEcFirst = (P_EC_F_CLIENT_QUEUE)MapViewOfFile(m_hMemMapEcFirst, FILE_MAP_ALL_ACCESS, 0, 0, 0);
			if (m_pQueueEcFirst != NULL)
			{
				m_pQueueEcFirst->m_nFront	= 0;
				m_pQueueEcFirst->m_nRear	= 0;
				m_pQueueEcFirst->m_nCount	= 0;
			}
		}
	}
}


void CClientEcFirst::OnClose()
{
	if (m_pQueueEcFirst		!= NULL)	UnmapViewOfFile(m_pQueueEcFirst);	
	if (m_hMemMapEcFirst	!= NULL)	CloseHandle(m_hMemMapEcFirst);
}

CString	CClientEcFirst::OnCharToString(char chData[], int nLength)
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

void CClientEcFirst::OnStringToChar(CString strData, char chData[])
{
	int nCnt;
	wchar_t *chBuf;

	chBuf = strData.GetBuffer(strData.GetLength());
	nCnt  = WideCharToMultiByte(CP_ACP, 0, chBuf, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, chBuf, -1, chData, nCnt, 0, 0);
}

void CClientEcFirst::OnMessageWrite(CString strMsg)
{
	CString strLog = _T("[HANDLER >> EC SERVER] ");

	if (m_hMemMapEcFirst != NULL && m_pQueueEcFirst != NULL)
	{
		strLog += strMsg;
		clsFunc.OnLogFileAdd(4, strLog);

		OnStringToChar(strMsg, m_pQueueEcFirst->m_chMsg[m_pQueueEcFirst->m_nRear]);

		m_pQueueEcFirst->m_nRear++;
		
		if (m_pQueueEcFirst->m_nRear >= 100) m_pQueueEcFirst->m_nRear = 0;
		
		m_pQueueEcFirst->m_nCount++;
	}
}

CString	CClientEcFirst::OnMessageRead()
{
	CString strData = _T("");
	
	if (m_hMemMapEcFirst!= NULL && m_pQueueEcFirst != NULL)
	{
		if (m_pQueueEcFirst->m_nCount > 0 )
		{
			strData = m_pQueueEcFirst->m_chMsg[m_pQueueEcFirst->m_nFront];
			
			m_pQueueEcFirst->m_nFront++;
			if (m_pQueueEcFirst->m_nFront >= 100) m_pQueueEcFirst->m_nFront = 0;
			
			m_pQueueEcFirst->m_nCount--;
		}
	}

	return strData;
}

UINT OnEcFirstClientThread(LPVOID lParam)
{
	CClientEcFirst *pMsg = (CClientEcFirst *)lParam;

	while(pMsg->m_bThread)
	{
		pMsg->OnCommunication();
	}

	pMsg->m_thrHandle = NULL;

	return 0;
}

void CClientEcFirst::OnDataReceive(CString strMsg)
{
	int nHead;
	int nLength;
	int nPos;

	CString strTemp;
	CString strData;
	CString strFunc;
	CString strLog = _T("[EC SERVER >> HANDLER] ");

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
			OnDataAnalysis(strTemp);
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
				OnDataAnalysis(strTemp);
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
				OnDataAnalysis(strTemp);

				m_nHeaderLength		= 0;
				m_strData			= _T("");
			}
			else
			{
				if (m_nHeaderLength > (m_strData.GetLength() + nLength))
				{
					m_strData		+= strMsg;

					// lot end 예외처리
					strFunc = OnDataItemAnalysis(0, 0, m_strData, _T("FUNCTION_RPY"));
					if (strFunc == _T("SORTER_LOT_END"))
					{
						nLength = m_strData.GetLength();
						if (nLength > 140)
						{
							OnDataAnalysis(m_strData);

							m_nHeaderLength		= 0;
							m_strData			= _T("");
						}
					}
				}
				else
				{
					strTemp		= m_strData + strMsg;

					if (m_nHeaderLength < strTemp.GetLength())
					{
						OnDataAnalysis(strTemp);

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
		strTemp.Format(_T("[EC_FIRST_NETWORK] %s"), strMsg);
		clsMem.OnNormalMessageWrite(strTemp);

		st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력
	}
}


void CClientEcFirst::OnDataAnalysis(CString strMsg)
{
	CString strFunc;
	CString strTemp;
	CString strData;
	CString strRpy;
	CString strEqpModel;

	int i;
	int nPos;
	int nCount = 0;

	strFunc = OnDataItemAnalysis(0, 0, strMsg, _T("FUNCTION_RPY"));

	if (strFunc == _T("BARCODE_RULE"))
	{
	}
	else if (strFunc == _T("PGM_INFO"))
	{
	}
	else if (strFunc == _T("SLOT_END"))
	{
	}
	else if (strFunc == _T("TEST_END"))
	{
		strData = OnDataItemAnalysis(0, 0, strMsg, _T("RESULT"));
		if (strData == _T("PASS"))
		{
			st_track_info.nTestEndBin	= YES;
			m_nTestEndFlag = m_nCommBin = BD_DATA_GOOD;
		}
		else
		{
			strData = OnDataItemAnalysis(0, 0, strMsg, _T("MSG"));
			if (strData != _T(""))
			{
				m_strErrorMsg	= strData.Mid(1, strData.GetLength()-1);
			}
			m_nTestEndFlag = m_nCommBin = BD_DATA_REJECT;
		}
	}
	else if (strFunc == _T("ONLINE_END"))
	{
		strData = OnDataItemAnalysis(0, 0, strMsg, _T("RESULT"));
		if (strData == _T("PASS"))
		{
			m_nOnlineEndFlag = m_nCommBin = BD_DATA_GOOD;
		}
		else
		{
			strData = OnDataItemAnalysis(0, 0, strMsg, _T("MSG"));
			if (strData != _T(""))
			{
				m_strErrorMsg	= strData.Mid(1, strData.GetLength()-1);
			}
			m_nOnlineEndFlag = m_nCommBin = BD_DATA_REJECT;
		}
	}
	else if (strFunc == _T("SCRAP_INFO"))
	{
		strData = OnDataItemAnalysis(0, 0, strMsg, _T("RESULT"));
		if (strData == _T("PASS"))
		{
			st_lot_display_info.nScrapCnt = 0;
			strData = OnDataItemAnalysis(0, 0, strMsg, _T("SCRAP_CNT"));
			//st_lot_display_info.nScrapCnt	= _wtoi(strData);
			nCount	= _wtoi(strData);

			if (nCount > 0)
			{
				for (i=0; i < nCount; i++)
				{
					strTemp.Format(_T("SCRAP_INFO%03d"), i + 1);
					strData = OnDataItemAnalysis(2, 0, strMsg, strTemp);

					strTemp = OnDataItemAnalysis(0, 0, strData, _T("EQPMODEL"));
					strEqpModel = strTemp.Mid(0, strTemp.GetLength()-1);
					if (strEqpModel == _T("IFT"))
					{
						st_lot_display_info.strScrapCode[st_lot_display_info.nScrapCnt]		= OnDataItemAnalysis(0, 0, strData, _T("SCRAP_CODE"));
						st_lot_display_info.strArray[st_lot_display_info.nScrapCnt]			= OnDataItemAnalysis(0, 0, strData, _T("ARRAYSN"));
						st_lot_display_info.strScrapSerial[st_lot_display_info.nScrapCnt]	= OnDataItemAnalysis(0, 0, strData, _T("SERIAL"));
						st_lot_display_info.nScrapCnt++;
					}
				}
			}
			m_nScrapInfoFlag = m_nCommBin = BD_DATA_GOOD;
		}
		else
		{
			strData = OnDataItemAnalysis(0, 0, strMsg, _T("MSG"));
			if (strData != _T(""))
			{
				m_strErrorMsg	= strData.Mid(1, strData.GetLength()-1);
			}
			m_nScrapInfoFlag = m_nCommBin = BD_DATA_REJECT;
		}
	}
	else if (strFunc == _T("SEE_LOT"))
	{
		strData = OnDataItemAnalysis(0, 0, strMsg, _T("RESULT"));
		if (strData == _T("PASS"))
		{
			st_lot_display_info.strLotNo		= OnDataItemAnalysis(0, 0, strMsg, _T("LOTNO"));
			st_lot_display_info.strPartNo		= OnDataItemAnalysis(0, 0, strMsg, _T("PARTNO"));
			st_lot_display_info.strLotState		= OnDataItemAnalysis(0, 0, strMsg, _T("LOTSTATE"));
			st_lot_display_info.strLotType		= OnDataItemAnalysis(0, 0, strMsg, _T("LOTTYPE"));
			st_lot_display_info.strMainState	= OnDataItemAnalysis(0, 0, strMsg, _T("MAINSTATE"));
			st_lot_display_info.strProCid		= OnDataItemAnalysis(0, 0, strMsg, _T("PROCID"));
			st_lot_display_info.nQty			= _wtoi(OnDataItemAnalysis(0, 0, strMsg, _T("QTY")));
			st_lot_display_info.strStep			= OnDataItemAnalysis(0, 0, strMsg, _T("STEPSEQ"));
			st_lot_display_info.strLine			= OnDataItemAnalysis(0, 0, strMsg, _T("LINE"));
			st_lot_display_info.strComplot		= OnDataItemAnalysis(0, 0, strMsg, _T("COMPLOT"));

			m_nSeeLotFlag = m_nCommBin = BD_DATA_GOOD;
		}
		else
		{
			strData = OnDataItemAnalysis(0, 0, strMsg, _T("MSG"));
			if (strData != _T(""))
			{
				m_strErrorMsg	= strData.Mid(1, strData.GetLength()-1);
			}
			m_nSeeLotFlag = m_nCommBin = BD_DATA_REJECT;
		}
	}
	else if (strFunc == _T("DELETE_SCRAP"))
	{
		strData = OnDataItemAnalysis(0, 0, strMsg, _T("RESULT"));
		if (strData == _T("PASS"))
		{
			m_nDelScrapFlg = m_nCommBin = BD_DATA_GOOD;
		}
		else
		{
			strData = OnDataItemAnalysis(0, 0, strMsg, _T("MSG"));
			if (strData != _T(""))
			{
				m_strErrorMsg	= strData.Mid(1, strData.GetLength()-1);
			}
			m_nDelScrapFlg = m_nCommBin = BD_DATA_REJECT;
		}
	}
	else if (strFunc == _T("SORTER_LOT_END"))
	{
		strData = OnDataItemAnalysis(0, 0, strMsg, _T("RESULT"));
		if (strData == _T("PASS"))
		{
			st_track_info.nTestEndBin		= NO;
			st_track_info.strLotNo			= _T("");
			st_track_info.strPartNo			= _T("");
			st_track_info.nIn				= 0;
			st_track_info.nPrimePass		= 0;
			st_track_info.nPrimeReject		= 0;
			st_track_info.nCumPass			= 0;
			st_track_info.nCumReject		= 0;

			if (st_track_info.nStatus == YES)
			{
				st_track_info.nStatus = NO;
			}

			m_nLotEndFlag = m_nCommBin = BD_DATA_GOOD;

			if (st_basic_info.nModeRfid == NO)
			{
				if (st_handler_info.cWndMain != NULL)
				{
					st_handler_info.cWndMain->SendMessage(WM_WORK_COMMAND, MAIN_TRACK_OUT_DELETE_REQ, 0);
				}
			}
		}
		else
		{
			strData = OnDataItemAnalysis(0, 0, strMsg, _T("MSG"));
			if (strData != _T(""))
			{
				m_strErrorMsg	= strData.Mid(1, strData.GetLength()-1);
			}
			m_nLotEndFlag = m_nCommBin = BD_DATA_REJECT;
		}
	}
	else if (strFunc == _T("VERSION_UPDATE"))
	{
		strData = OnDataItemAnalysis(0, 0, strMsg, _T("RESULT"));
		if (strData == _T("PASS"))
		{
			m_nVerFlag = m_nCommBin = BD_DATA_GOOD;
		}
		else
		{
			strData = OnDataItemAnalysis(0, 0, strMsg, _T("MSG"));
			if (strData != _T(""))
			{
				m_strErrorMsg	= strData.Mid(1, strData.GetLength()-1);
			}
			m_nVerFlag = m_nCommBin = BD_DATA_REJECT;
		}
	}
	else if (strFunc == _T("EQP_STATUS_CHANGE"))
	{
		strData = OnDataItemAnalysis(0, 0, strMsg, _T("RESULT"));
		if (strData == _T("PASS"))
		{
			m_nStatusFlag = m_nCommBin = BD_DATA_GOOD;
		}
		else
		{
			strData = OnDataItemAnalysis(0, 0, strMsg, _T("MSG"));
			if (strData != _T(""))
			{
				m_strErrorMsg	= strData.Mid(1, strData.GetLength()-1);
			}
			m_nStatusFlag = m_nCommBin = BD_DATA_REJECT;
		}
	}
	else if (strFunc == _T("EQP_MODE_CHANGE"))
	{
		strData = OnDataItemAnalysis(0, 0, strMsg, _T("RESULT"));
		if (strData == _T("PASS"))
		{
			m_nModeChangFlag = m_nCommBin = BD_DATA_GOOD;
		}
		else
		{
			strData = OnDataItemAnalysis(0, 0, strMsg, _T("MSG"));
			if (strData != _T(""))
			{
				m_strErrorMsg	= strData.Mid(1, strData.GetLength()-1);
			}
			m_nModeChangFlag = m_nCommBin = BD_DATA_REJECT;
		}
	}
	else if (strFunc == _T("FULL_CARRIER_MOVE_REQ"))
	{
		strData = OnDataItemAnalysis(0, 0, strMsg, _T("RESULT"));
		if (strData == _T("PASS"))
		{
			m_nFullCarrierFlag = m_nCommBin = BD_DATA_GOOD;
		}
		else
		{
			strData = OnDataItemAnalysis(0, 0, strMsg, _T("MSG"));
			if (strData != _T(""))
			{
				m_strErrorMsg	= strData.Mid(1, strData.GetLength()-1);
			}
			m_nFullCarrierFlag = m_nCommBin = BD_DATA_REJECT;
		}

		if (st_basic_info.nModeRfid == YES)
		{
			// jtkim 20150508
			if (st_handler_info.cWndMain != NULL)
			{
				st_handler_info.cWndMain->SendMessage(WM_WORK_COMMAND, MAIN_TRACK_OUT_DELETE_REQ, 0);
			}
		}
	}
	else if (strFunc == _T("EMPTY_CARRIER_MOVE_REQ"))
	{
		strData = OnDataItemAnalysis(0, 0, strMsg, _T("RESULT"));
		if (strData == _T("PASS"))
		{
			m_nEmptyCarrierFlag = m_nCommBin = BD_DATA_GOOD;
		}
		else
		{
			strData = OnDataItemAnalysis(0, 0, strMsg, _T("MSG"));
			if (strData != _T(""))
			{
				m_strErrorMsg	= strData.Mid(1, strData.GetLength()-1);
			}
			m_nEmptyCarrierFlag = m_nCommBin = BD_DATA_REJECT;
		}
	}
	else if (strFunc == _T("TOOL_CHANGE"))
	{
		strData = OnDataItemAnalysis(0, 0, strMsg, _T("RESULT"));
		if (strData == _T("PASS"))
		{
			m_nToolChangFlag = m_nCommBin = BD_DATA_GOOD;
		}
		else
		{
			strData = OnDataItemAnalysis(0, 0, strMsg, _T("MSG"));
			if (strData != _T(""))
			{
				m_strErrorMsg	= strData.Mid(1, strData.GetLength()-1);
			}
			m_nToolChangFlag = m_nCommBin = BD_DATA_REJECT;
		}
	}
	else if (strFunc == _T("MATCH_ID"))
	{
		strData = OnDataItemAnalysis(0, 0, strMsg, _T("RESULT"));
		if (strData == _T("PASS"))
		{
			m_nMatchFlag = m_nCommBin = BD_DATA_GOOD;
		}
		else
		{
			strData = OnDataItemAnalysis(0, 0, strMsg, _T("MSG"));
			if (strData != _T(""))
			{
				m_strErrorMsg	= strData.Mid(1, strData.GetLength()-1);
			}
			m_nMatchFlag = m_nCommBin = BD_DATA_REJECT;
		}
	}
	else if (strFunc == _T("DATE_TIME"))
	{
		strData = OnDataItemAnalysis(0, 0, strMsg, _T("RESULT"));
		if (strData == _T("PASS"))
		{
			m_nDateFlag = m_nCommBin = BD_DATA_GOOD;
		}
		else
		{
			strData = OnDataItemAnalysis(0, 0, strMsg, _T("MSG"));
			if (strData != _T(""))
			{
				m_strErrorMsg	= strData.Mid(1, strData.GetLength()-1);
			}
			m_nDateFlag = m_nCommBin = BD_DATA_REJECT;
		}
	}
	else if (strFunc == _T("LABEL_SERIAL_INFO"))
	{
		// jtkim 20150806
		m_strSerial		= _T("");
		m_strPpid		= _T("");
		m_strWwn		= _T("");
		m_strCserial	= _T("");
		m_strPsid		= _T("");

		strData = OnDataItemAnalysis(0, 0, strMsg, _T("RESULT"));
		if (strData == _T("PASS"))
		{
			strData			= OnDataItemAnalysis(2, 0, strMsg, _T("SRL_INFO"));

			m_strSerial		= OnDataItemAnalysis(0, 0, strData, _T("SERIAL"));
			nPos = -1;
			nPos = m_strSerial.Find(_T("-"), 0);
			if (nPos >= 0) m_strSerial = _T("");

			if (m_strSerial != _T(""))
			{
				strTemp = m_strSerial.Mid(m_strSerial.GetLength() - 1, 1);
				if (strTemp == _T(")"))
				{
					strTemp		= m_strSerial.Mid(0, m_strSerial.GetLength() - 1);
					m_strSerial = strTemp;
				}
			}

			
			m_strPpid		= OnDataItemAnalysis(0, 0, strData, _T("PPID"));
			nPos = -1;
			nPos = m_strPpid.Find(_T("-"), 0);
			if (nPos >= 0) m_strPpid = _T("");

			if (m_strPpid != _T(""))
			{
				strTemp = m_strPpid.Mid(m_strPpid.GetLength() - 1, 1);
				if (strTemp == _T(")"))
				{
					strTemp		= m_strPpid.Mid(0, m_strPpid.GetLength() - 1);
					m_strPpid	= strTemp;
				}
			}

			
			m_strWwn		= OnDataItemAnalysis(0, 0, strData, _T("WWN"));
			nPos = -1;
			nPos = m_strWwn.Find(_T("-"), 0);
			if (nPos >= 0) m_strWwn = _T("");
			
			if (m_strWwn != _T(""))
			{
				strTemp = m_strWwn.Mid(m_strWwn.GetLength() - 1, 1);
				if (strTemp == _T(")"))
				{
					strTemp		= m_strWwn.Mid(0, m_strWwn.GetLength() - 1);
					m_strWwn	= strTemp;
				}
			}

			m_strCserial	= OnDataItemAnalysis(0, 0, strData, _T("C_SERIAL"));
			nPos = -1;
			nPos = m_strCserial.Find(_T("-"), 0);
			if (nPos >= 0) m_strCserial = _T("");
			
			if (m_strCserial != _T(""))
			{
				strTemp = m_strCserial.Mid(m_strCserial.GetLength() - 1, 1);
				if (strTemp == _T(")"))
				{
					strTemp			= m_strCserial.Mid(0, m_strCserial.GetLength() - 1);
					m_strCserial	= strTemp;
				}
			}

			m_strPsid		= OnDataItemAnalysis(0, 0, strData, _T("PSID"));
			nPos = -1;
			nPos = m_strPsid.Find(_T("-"), 0);
			if (nPos >= 0) m_strPsid = _T("");

			if (m_strPsid != _T(""))
			{
				strTemp = m_strPsid.Mid(m_strPsid.GetLength() - 1, 1);
				if (strTemp == _T(")"))
				{
					strTemp		= m_strPsid.Mid(0, m_strPsid.GetLength() - 1);
					m_strPsid	= strTemp;
				}
			}

			m_nPcbInfoFlag = m_nCommBin = BD_DATA_GOOD;
		}
		else
		{
			strData = OnDataItemAnalysis(3, 0, strMsg, _T("MSG"));
			if (strData != _T(""))
			{
				m_strErrorMsg	= strData.Mid(0, strData.GetLength()-1);
			}
			m_nPcbInfoFlag = m_nCommBin = BD_DATA_REJECT;
		}
	}
}


CString CClientEcFirst::OnDataItemAnalysis(int nOpt, int nPos, CString strMsg, CString strFind)
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

void CClientEcFirst::OnCommunication()
{
	if (st_handler_info.nRunStatus == dWARNING) return;

	switch (m_nCommStep)
	{
		case 0:
			// 공유메모리에 전송할 데이터가 있으면 
			if (m_pQueueEcFirst->m_nCount <= 0) break;
				
			m_nRetry		= 0;
			m_strSendMsg	= OnMessageRead();

			m_nCommStep = 100;
			break;

		case 100:
			// interface 상태 체크
			if (st_basic_info.nModeInterface == EQP_ON_LINE)
			{
				m_nCommBin	= BD_NONE;

				m_nCommStep = 2000;
			}
			else
			{
				// off line mode 임의로 데이터 생성
				m_dwWaitTime[0] = GetTickCount();

				m_nCommStep = 1000;
			}
			break;

		case 1000:
			m_dwWaitTime[1] = GetTickCount();
			m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];

			if (m_dwWaitTime[2] < 0)
			{
				m_dwWaitTime[0] = GetTickCount();
				break;
			}

			if (m_dwWaitTime[2] > (DWORD)st_recipe_info.nAbortTime)
			{
				m_nCommBin	= BD_DATA_GOOD;

				m_nCommStep = 0;
			}
			break;

		case 2000:
			if (st_client_info[EC_FIRST_NETWORK].nConnect == YES)
			{
				m_nCommStep = 2200;
			}
			else
			{
				m_dwWaitTime[0] = GetTickCount();

				::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_CONNECT, 0);

				m_nCommStep = 2100;
			}
			break;

		case 2100:
			if (st_client_info[EC_FIRST_NETWORK].nConnect == YES)
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

						::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_CLOSE, 0);

						m_dwWaitTime[0] = GetTickCount();
						CTL_Lib.Alarm_Error_Occurrence(20000, dWARNING, _T("610000"));

						m_nCommStep = 0;
					}
					else
					{
						m_dwWaitTime[0] = GetTickCount();

						::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_CLOSE, 0);

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
				m_nCommStep = 100;
			}
			break;

		case 2200:
			m_dwWaitTime[0] = GetTickCount();
			st_client_info[EC_FIRST_NETWORK].strSend = m_strSendMsg;

			::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_SEND, 0);

			m_nCommStep = 2300;
			break;

		case 2300:
			if (m_nCommBin == BD_DATA_GOOD)
			{
				m_dwWaitTime[0] = GetTickCount();
				::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_CLOSE, 0);

				m_nCommStep = 2350;
			}
			else if (m_nCommBin == BD_DATA_REJECT)
			{
				m_dwWaitTime[0] = GetTickCount();
				::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_CLOSE, 0);

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

				if (m_dwWaitTime[2] > 100000)
				{
					m_nRetry++;

					if (m_nRetry > 3)
					{
						m_nCommBin	= BD_RCV_TIMEOUT;

						::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_CLOSE, 0);

						m_dwWaitTime[0] = GetTickCount();
						CTL_Lib.Alarm_Error_Occurrence(20001, dWARNING, _T("610001"));

						m_nCommStep = 0;
					}
					else
					{
						m_dwWaitTime[0] = GetTickCount();
						::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + EC_FIRST_NETWORK, CLIENT_CLOSE, 0);

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
				m_nCommStep = 0;
			}
			break;
	}
}

void CClientEcFirst::OnEcSLotEnd(tagPCB_INFO pcb_info)
{
	CString strTemp;
	CString strData;
	CString strMsg;
	
	strData = _T("FUNCTION=SLOT_END");
	strData += _T(" ");
	
	strTemp.Format(_T("EQPID=%s"), st_basic_info.strEqp);
	strData += strTemp;
	strData += _T(" ");

	strData += _T("OPERID=AUTO");
	strData += _T(" ");

	strTemp.Format(_T("LOTNO=%s"), pcb_info.strLotNo);
	strData += strTemp;
	strData += _T(" ");

	strTemp.Format(_T("PARTID=%s"), pcb_info.strPartNo);
	strData += strTemp;
	strData += _T(" ");

	strTemp.Format(_T("SERIAL=%s"), pcb_info.strSerialNo);
	strData += strTemp;
	strData += _T(" ");

	if (pcb_info.nBin == BD_DATA_GOOD)
	{
		strData += _T("TESTRESULT=PASS");
		strData += _T(" ");
	}
	else
	{
		strData += _T("TESTRESULT=FAIL");
		strData += _T(" ");
	}

	strTemp.Format(_T("PPID=%s"), pcb_info.strPPID);
	strData += strTemp;
	strData += _T(" ");

	strTemp.Format(_T("WWN=%s"), pcb_info.strWWN);
	strData += strTemp;
	strData += _T(" ");

	strTemp.Format(_T("SCRAP_CODE=%d"), pcb_info.nScrCode);
	strData += strTemp;
	strData += _T(" ");

	strTemp.Format(_T("BIN=%d"), pcb_info.nBin);
	strData += strTemp;
	strData += _T(" ");

	strTemp.Format(_T("SLOT=%d"), pcb_info.nBdNum);
	strData += strTemp;
	strData += _T(" ");

	strTemp.Format(_T("ARRAY_SN=%s"), pcb_info.strArrNo);
	strData += strTemp;
	strData += _T(" ");

	strTemp.Format(_T("C_SERIAL=%s"), pcb_info.strCSerialNo);
	strData += strTemp;
	strData += _T(" ");

	strTemp.Format(_T("PSID=%s"), pcb_info.strPSID);
	strData += strTemp;
	strData += _T(" ");

	strTemp.Format(_T("P_SERIAL=%d"), pcb_info.strPSID);
	strData += strTemp;

	strMsg.Format(_T("%016d%s"), strData.GetLength(), strData);

	OnMessageWrite(strMsg);
}

void CClientEcFirst::OnEcPcbInfo(CString strLotId, CString strPartId, CString strSerial)
{
	CString strTemp;
	CString strData;
	CString strMsg;
	
	strData = _T("FUNCTION=LABEL_SERIAL_INFO");
	strData += _T(" ");
	
	strTemp.Format(_T("EQPID=%s"), st_basic_info.strEqp);
	strData += strTemp;
	strData += _T(" ");
/*
	strTemp.Format(_T("LOTID=%s"), strLotId);
	strData += strTemp;
	strData += _T(" ");
*/
	strTemp.Format(_T("PARTID=%s"), strPartId);
	strData += strTemp;
	strData += _T(" ");

	strData += _T("OPERID=AUTO");
	strData += _T(" ");

	strTemp.Format(_T("SERIAL=%s"), strSerial);
	strData += strTemp;

	strMsg.Format(_T("%016d%s"), strData.GetLength(), strData);

	m_nPcbInfoFlag = BD_NONE;

	OnMessageWrite(strMsg);
}

void CClientEcFirst::OnEcTestEnd(CString strLotId, CString strPartId)
{
	CString strTemp;
	CString strData;
	CString strMsg;
	
	strData = _T("FUNCTION=TEST_END");
	strData += _T(" ");
	
	strTemp.Format(_T("EQPID=%s"), st_basic_info.strEqp);
	strData += strTemp;
	strData += _T(" ");

	strTemp.Format(_T("LOTID=%s"), strLotId);
	strData += strTemp;
	strData += _T(" ");

	strTemp.Format(_T("PARTNO=%s"), strPartId);
	strData += strTemp;

	strMsg.Format(_T("%016d%s"), strData.GetLength(), strData);

	m_nTestEndFlag = BD_NONE;

	OnMessageWrite(strMsg);
}

void CClientEcFirst::OnEcOnlineEnd(tagLOT_DISPLAY_INFO lot_display)
{
	CString strTemp;
	CString strData;
	CString strMsg;

	int i;

	strData = _T("FUNCTION=ONLINE_END");
	strData += _T(" ");
	
	strTemp.Format(_T("EQPID=%s"), st_basic_info.strEqp);
	strData += strTemp;
	strData += _T(" ");

	strData += _T("EQPMODEL=IFT");
	strData += _T(" ");

	// jtkim 20150627
	//if (st_basic_info.nModeRearSmema == YES)
	if (st_handler_info.nAutoLine == 1 && st_lot_info[LOT_CURR].nTrayRunMode_StandAlone == NO)
	{
		strData += _T("AUTOLINE=YES");
	}
	else
	{
		strData += _T("AUTOLINE=NO");
	}
	strData += _T(" ");

	strTemp.Format(_T("LOTID=%s"), lot_display.strLotNo);
	strData += strTemp;
	strData += _T(" ");

	strData += _T("OPERID=AUTO");
	strData += _T(" ");

	if (lot_display.nScrapCnt > 0)
	{
		strTemp.Format(_T("SCRAP_CNT=%d"), lot_display.nScrapCnt);
		strData += strTemp;
		strData += _T(" ");

		for (i=0; i<lot_display.nScrapCnt; i++)
		{
			strTemp.Format(_T("SCRAP_INFO%03d=(SCRAP_CODE=%s ARRAYSN=%s SERIAL=%s)"), i + 1,
																					  lot_display.strScrapCode[i],
																					  lot_display.strArray[i],
																					  lot_display.strScrapSerial[i]);
			strData += strTemp;
			strData += _T(" ");
		}
	}
	else
	{
		strData += _T("SCRAP_CNT=0");
	}

	strMsg.Format(_T("%016d%s"), strData.GetLength(), strData);

	m_nOnlineEndFlag = BD_NONE;

	OnMessageWrite(strMsg);
}

void CClientEcFirst::OnEcScrapInfo(CString strLotId)
{
	CString strTemp;
	CString strData;
	CString strMsg;

	strData = _T("FUNCTION=SCRAP_INFO");
	strData += _T(" ");
	
	strTemp.Format(_T("EQPID=%s"), st_basic_info.strEqp);
	strData += strTemp;
	strData += _T(" ");

	strTemp.Format(_T("LOTID=%s"), strLotId);
	strData += strTemp;
	strData += _T(" ");

	// jtkim 20150627
	//if (st_basic_info.nModeRearSmema == YES)
	if (st_handler_info.nAutoLine == 1 && st_lot_info[LOT_CURR].nTrayRunMode_StandAlone == NO)
	{
		strData += _T("AUTOLINE=YES");
	}
	else
	{
		strData += _T("AUTOLINE=NO");
	}

	strMsg.Format(_T("%016d%s"), strData.GetLength(), strData);

	m_nScrapInfoFlag = BD_NONE;

	OnMessageWrite(strMsg);
}

void CClientEcFirst::OnEcSeeLot(CString strLotId)
{
	CString strTemp;
	CString strData;
	CString strMsg;

	strData = _T("FUNCTION=SEE_LOT");
	strData += _T(" ");

	strTemp.Format(_T("LOTNO=%s"), strLotId);
	strData += strTemp;
	strData += _T(" ");

	strTemp.Format(_T("EQPID=%s"), st_basic_info.strEqp);
	strData += strTemp;
	strData += _T(" ");

	strData += _T("OPERID=AUTO");

	strMsg.Format(_T("%016d%s"), strData.GetLength(), strData);

	m_nSeeLotFlag = BD_NONE;

	OnMessageWrite(strMsg);
}

void CClientEcFirst::OnEcDeleteScrap(CString strLotId, CString strSerial, CString strArray)
{
	CString strTemp;
	CString strData;
	CString strMsg;

	strData = _T("FUNCTION=DELETE_SCRAP");
	strData += _T(" ");

	strTemp.Format(_T("EQPID=%s"), st_basic_info.strEqp);
	strData += strTemp;
	strData += _T(" ");

	// jtkim 20150627
	//if (st_basic_info.nModeRearSmema == YES)
	if (st_handler_info.nAutoLine == 1 && st_lot_info[LOT_CURR].nTrayRunMode_StandAlone == NO)
	{
		strData += _T("AUTOLINE=YES");
	}
	else
	{
		strData += _T("AUTOLINE=NO");
	}
	strData += _T(" ");

	strTemp.Format(_T("LOTID=%s"), strLotId);
	strData += strTemp;
	strData += _T(" ");

	strTemp.Format(_T("SERIAL=%s"), strSerial);
	strData += strTemp;
	strData += _T(" ");

	strTemp.Format(_T("ARRAYSN=%s"), strArray);
	strData += strTemp;

	strMsg.Format(_T("%016d%s"), strData.GetLength(), strData);

	m_nDelScrapFlg = BD_NONE;

	OnMessageWrite(strMsg);
}

void CClientEcFirst::OnEcLotEnd(CString strLotId)
{
	CString strTemp;
	CString strData;
	CString strMsg;

	strData = _T("FUNCTION=SORTER_LOT_END");
	strData += _T(" ");

	strTemp.Format(_T("LOTID=%s"), strLotId);
	strData += strTemp;
	strData += _T(" ");

	strTemp.Format(_T("EQPID=%s"), st_basic_info.strEqp);
	strData += strTemp;
	strData += _T(" ");

	// jtkim 20150627
	//if (st_basic_info.nModeRearSmema == YES)
	if (st_handler_info.nAutoLine == 1 && st_lot_info[LOT_CURR].nTrayRunMode_StandAlone == NO)
	{
		strData += _T("AUTOLINE=YES");
	}
	else
	{
		strData += _T("AUTOLINE=NO");
	}

	strMsg.Format(_T("%016d%s"), strData.GetLength(), strData);

	m_nLotEndFlag = BD_NONE;

	OnMessageWrite(strMsg);
}

void CClientEcFirst::OnEqpVersionUpdate(CString strDescrip)
{
	CString strTemp;
	CString strData;
	CString strMsg;

	strData = _T("FUNCTION=VERSION_UPDATE");
	strData += _T(" ");

	strTemp.Format(_T("EQPID=%s"), st_basic_info.strEqp);
	strData += strTemp;
	strData += _T(" ");

	strData += _T("EQPMODEL=ROUTER");
	strData += _T(" ");

	strTemp.Format(_T("DESC=%s"), strDescrip);
	strData += strTemp;

	strMsg.Format(_T("%016d%s"), strData.GetLength(), strData);

	m_nVerFlag = BD_NONE;

	OnMessageWrite(strMsg);
}

void CClientEcFirst::OnEqpStatusChange(CString strMode, CString strCode, CString strDescrip)
{
	CString strTemp;
	CString strData;
	CString strMsg;

	strData = _T("FUNCTION=EQP_STATUS_CHANGE");
	strData += _T(" ");

	strTemp.Format(_T("EQPID=%s"), st_basic_info.strEqp);
	strData += strTemp;
	strData += _T(" ");

	strData += _T("OPERID=AUTO");
	strData += _T(" ");

	strTemp.Format(_T("MODE=%s"), strMode);
	strData += strTemp;
	strData += _T(" ");

	strTemp.Format(_T("CODE=%s"), strCode);
	strData += strTemp;
	strData += _T(" ");

	strTemp.Format(_T("COMMEND=%s"), strDescrip);
	strData += strTemp;
	strData += _T(" ");

	if (strMode == _T("ENABLE"))
	{
		strData += _T("ENABLEOK=Y");
	}
	else
	{
		strData += _T("ENABLEOK=N");
	}

	strMsg.Format(_T("%016d%s"), strData.GetLength(), strData);

	m_nStatusFlag = BD_NONE;

	OnMessageWrite(strMsg);
}

void CClientEcFirst::OnEqpModeChange(CString strMode)
{
	CString strTemp;
	CString strData;
	CString strMsg;

	strData = _T("FUNCTION=EQP_MODE_CHANGE");
	strData += _T(" ");

	strTemp.Format(_T("EQPID=%s"), st_basic_info.strEqp);
	strData += strTemp;
	strData += _T(" ");

	strData += _T("OPERID=AUTO");
	strData += _T(" ");

	strTemp.Format(_T("EQPMODE=%s"), strMode);
	strData += strTemp;

	strMsg.Format(_T("%016d%s"), strData.GetLength(), strData);

	m_nModeChangFlag = BD_NONE;

	OnMessageWrite(strMsg);
}

void CClientEcFirst::OnEqpFullCarrierMove(CString srtCarrierId)
{
	CString strTemp;
	CString strData;
	CString strMsg;

	strData = _T("FUNCTION=FULL_CARRIER_MOVE_REQ");
	strData += _T(" ");

	strTemp.Format(_T("EQPID=%s"), st_basic_info.strEqp);
	strData += strTemp;
	strData += _T(" ");

	strData += _T("LOTID=EMAGAZINE");
	strData += _T(" ");

	strData += _T("PORTID=AROUT-02_I1");
	strData += _T(" ");

	strTemp.Format(_T("CARRIERID=%s"), srtCarrierId);
	strData += strTemp;

	strMsg.Format(_T("%016d%s"), strData.GetLength(), strData);

	m_nFullCarrierFlag = BD_NONE;

	OnMessageWrite(strMsg);
}

void CClientEcFirst::OnEqpEmptyCarrierMove()
{
	CString strTemp;
	CString strData;
	CString strMsg;

	strData = _T("FUNCTION=EMPTY_CARRIER_MOVE_REQ");
	strData += _T(" ");

	strTemp.Format(_T("EQPID=%s"), st_basic_info.strEqp);
	strData += strTemp;
	strData += _T(" ");

	strData += _T("LOTID=");
	strData += _T(" ");

	strData += _T("PORTID=AROUT-01_I1");
	strData += _T(" ");

	strData += _T("CARRIERID=");
	strData += _T(" ");

	strData += _T("FEEDERID=");
	strData += _T(" ");

	strData += _T("MATCODE=");
	strData += _T(" ");

	strData += _T("QTY=");

	strMsg.Format(_T("%016d%s"), strData.GetLength(), strData);

	m_nEmptyCarrierFlag = BD_NONE;

	OnMessageWrite(strMsg);
}

void CClientEcFirst::OnEqpToolChange(CString strToolId)
{
	CString strTemp;
	CString strData;
	CString strMsg;

	strData = _T("FUNCTION=TOOL_CHANGE");
	strData += _T(" ");

	strTemp.Format(_T("EQPID=%s"), st_basic_info.strEqp);
	strData += strTemp;
	strData += _T(" ");

	strTemp.Format(_T("TOOID=%s"), strToolId);
	strData += strTemp;

	strMsg.Format(_T("%016d%s"), strData.GetLength(), strData);

	m_nToolChangFlag = BD_NONE;

	OnMessageWrite(strMsg);
}

void CClientEcFirst::OnEqpMatch(CString strLotId, CString strCarrierId)
{
	CString strTemp;
	CString strData;
	CString strMsg;

	strData = _T("FUNCTION=MATCH_ID");
	strData += _T(" ");

	strTemp.Format(_T("EQPID=%s"), st_basic_info.strEqp);
	strData += strTemp;
	strData += _T(" ");

	strTemp.Format(_T("CARRIER_ID=%s"), strCarrierId);
	strData += strTemp;
	strData += _T(" ");

	strTemp.Format(_T("LOT_ID=%s"), strLotId);
	strData += strTemp;

	strMsg.Format(_T("%016d%s"), strData.GetLength(), strData);

	m_nMatchFlag = BD_NONE;

	OnMessageWrite(strMsg);
}

void CClientEcFirst::OnEqpDate()
{
	CString strTemp;
	CString strData;
	CString strMsg;

	strData = _T("FUNCTION=DATE_TIME");
	strData += _T(" ");

	strTemp.Format(_T("EQPID=%s"), st_basic_info.strEqp);
	strData += strTemp;

	strMsg.Format(_T("%016d%s"), strData.GetLength(), strData);

	m_nDateFlag = BD_NONE;

	OnMessageWrite(strMsg);
}