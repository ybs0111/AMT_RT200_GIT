// AlgMemory.cpp: implementation of the CAlgMemory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "AlgMemory.h"
#include "Variable.h"
#include "PublicFunction.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define NORMAL_MSG_NAME			"NORMAL_SHAREMEM"
#define ABNORMAL_MSG_NAME		"ABNORMAL_SHAREMEM"

CAlgMemory clsMem;

CAlgMemory::CAlgMemory()
{
	m_hMemMapNormal	= m_hMemMapAbNormal= NULL;
	m_pQueueNormal	= m_pQueueAbNormal = NULL;

	OnMemoryOpen();
}

CAlgMemory::~CAlgMemory()
{
	OnMemoryClose();
}


bool CAlgMemory::OnMemoryOpen()
{
	m_hMemMapNormal = CreateFileMapping(INVALID_HANDLE_VALUE,
										 NULL,
										 PAGE_READWRITE,
										 0,
										 sizeof(S_QUEUE),
										 (LPCWSTR)NORMAL_MSG_NAME);
	if (m_hMemMapNormal == NULL)
	{
		return false;
	}
		
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		return false;
	}
		
		
	m_pQueueNormal = (PS_QUEUE)MapViewOfFile(m_hMemMapNormal, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (m_pQueueNormal == NULL)
	{
		return false;
	}

	m_pQueueNormal->m_nFront	= 0;
	m_pQueueNormal->m_nRear		= 0;
	m_pQueueNormal->m_nCount	= 0;

	m_hMemMapAbNormal = CreateFileMapping(INVALID_HANDLE_VALUE,
										 NULL,
										 PAGE_READWRITE,
										 0,
										 sizeof(S_QUEUE),
										 (LPCWSTR)ABNORMAL_MSG_NAME);
	if (m_hMemMapAbNormal == NULL)
	{
		return false;
	}
		
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		return false;
	}
		
		
	m_pQueueAbNormal = (PS_QUEUE)MapViewOfFile(m_hMemMapAbNormal, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (m_pQueueAbNormal == NULL)
	{
		return false;
	}

	m_pQueueAbNormal->m_nFront	= 0;
	m_pQueueAbNormal->m_nRear	= 0;
	m_pQueueAbNormal->m_nCount	= 0;

	return true;
}

void CAlgMemory::OnMemoryClose()
{
	if (m_pQueueNormal != NULL)		UnmapViewOfFile(m_pQueueNormal);	
	if (m_hMemMapNormal != NULL)	CloseHandle(m_hMemMapNormal);

	if (m_pQueueAbNormal != NULL)	UnmapViewOfFile(m_pQueueAbNormal);	
	if (m_hMemMapAbNormal != NULL)	CloseHandle(m_hMemMapAbNormal);
}

void CAlgMemory::OnNormalMessageWrite(CString strMsg)
{
	if (m_hMemMapNormal != NULL && m_pQueueNormal != NULL)
	{
		//clsFunc.OnStringToChar(strMsg, m_pQueueNormal->m_chMsg[m_pQueueNormal->m_nRear]);
		if (strMsg == _T(""))
		{
			return;
		}
		OnNormalStringToChar(strMsg, m_pQueueNormal->m_chMsg[m_pQueueNormal->m_nRear]);

		m_pQueueNormal->m_nRear++;

		if (m_pQueueNormal->m_nRear >= 100) m_pQueueNormal->m_nRear = 0;
	
		m_pQueueNormal->m_nCount++;
	}
}

void CAlgMemory::OnAbNormalMessagWrite(CString strMsg)
{
	if (m_hMemMapAbNormal != NULL && m_pQueueAbNormal != NULL)
	{
		//clsFunc.OnStringToChar(strMsg, m_pQueueAbNormal->m_chMsg[m_pQueueAbNormal->m_nRear]);
		if (strMsg == _T(""))
		{
			return;
		}

		if (m_pQueueAbNormal->m_nRear >= 100) return;

		OnAbNormalStringToChar(strMsg, m_pQueueAbNormal->m_chMsg[m_pQueueAbNormal->m_nRear]);

		m_pQueueAbNormal->m_nRear++;
		
		if (m_pQueueAbNormal->m_nRear >= 100) m_pQueueAbNormal->m_nRear = 0;
		
		m_pQueueAbNormal->m_nCount++;
	}
}


CString CAlgMemory::OnNormalMessageRead()
{
	CString strData = _T("");

	if (m_hMemMapNormal != NULL && m_pQueueNormal != NULL)
	{
		if (m_pQueueNormal->m_nCount > 0 )
		{
//			strData = m_pQueueNormal->m_chMsg[m_pQueueNormal->m_nFront];
			strData = OnNormalCharToString(m_pQueueNormal->m_chMsg[m_pQueueNormal->m_nFront], 5000);
			m_pQueueNormal->m_nFront++;
			if (m_pQueueNormal->m_nFront >= 100) m_pQueueNormal->m_nFront = 0;
			
			m_pQueueNormal->m_nCount--;
		}
	}

	return strData;
}

CString	CAlgMemory::OnAbNormalMessageRead()
{
	CString strData = _T("");
	
	if (m_hMemMapAbNormal != NULL && m_pQueueAbNormal != NULL)
	{
		if (m_pQueueAbNormal->m_nCount > 0 )
		{
			//strData = m_pQueueAbNormal->m_chMsg[m_pQueueAbNormal->m_nFront];
			strData = OnAbNormalCharToString(m_pQueueAbNormal->m_chMsg[m_pQueueAbNormal->m_nFront], 5000);

			m_pQueueAbNormal->m_nFront++;
			if (m_pQueueAbNormal->m_nFront >= 100) m_pQueueAbNormal->m_nFront = 0;
			
			m_pQueueAbNormal->m_nCount--;
		}
	}

	return strData;
}

CString	CAlgMemory::OnAbNormalCharToString(char chData[], int nLength)
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

void CAlgMemory::OnAbNormalStringToChar(CString strData, char chData[])
{
	int nCnt;
	wchar_t *chBuf;

	chBuf = strData.GetBuffer(strData.GetLength());
	nCnt  = WideCharToMultiByte(CP_ACP, 0, chBuf, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, chBuf, -1, chData, nCnt, 0, 0);
}

CString	CAlgMemory::OnNormalCharToString(char chData[], int nLength)
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

void CAlgMemory::OnNormalStringToChar(CString strData, char chData[])
{
	int nCnt;
	wchar_t *chBuf;

	chBuf = strData.GetBuffer(strData.GetLength());
	nCnt  = WideCharToMultiByte(CP_ACP, 0, chBuf, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, chBuf, -1, chData, nCnt, 0, 0);
}