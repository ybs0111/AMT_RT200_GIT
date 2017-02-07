// ServerClient.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "ServerClient.h"
#include "ServerSocket.h"
#include "PublicFunction.h"
#include "Variable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerClient

CServerClient::CServerClient(CServerSocket* pSocket)
{
	m_pSocket = pSocket;
}

CServerClient::~CServerClient()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CServerClient, CAsyncSocket)
	//{{AFX_MSG_MAP(CServerClient)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CServerClient member functions

void CServerClient::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_pWnd != NULL)
	{
		::PostMessage(m_pWnd->m_hWnd, WM_SERVER_MSG + m_nAddr, SERVER_CLOSE, this->m_ClientNum);
	}
	CAsyncSocket::OnClose(nErrorCode);
}

void CServerClient::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	CString strMsg;

	m_nLength = Receive(m_chRev, 8196);

	strMsg		= (CString) m_chRev;
	st_server_info[m_nAddr].strRev = strMsg.Mid(0, m_nLength);

	if (m_pWnd != NULL)
	{
		::PostMessage(m_pWnd->m_hWnd, WM_SERVER_MSG + m_nAddr, SERVER_REV, 0);
	}

	CAsyncSocket::OnReceive(nErrorCode);
}

void CServerClient::OnOpen(CWnd *pWnd, int nAddr)
{
	m_pWnd	= pWnd;

	m_nAddr	= nAddr;
}
