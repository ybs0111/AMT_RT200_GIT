// ServerSocket.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "ServerSocket.h"
#include "Variable.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerSocket

CServerSocket::CServerSocket()
{
	for(int i=0; i<MAX_USER; i++)
	{
		m_pClient[i] = NULL;
	}
}

CServerSocket::~CServerSocket()
{
	OnClose();
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CServerSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CServerSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CServerSocket member functions


void CServerSocket::OnAccept(int nErrorCode) 
{
	int nClient;

	nClient = OnClientNumber();

	if (nClient < 0) return;

	m_pClient[nClient] = new CServerClient(this);
	Accept(*m_pClient[nClient]);

	m_pClient[nClient]->OnOpen(m_pWnd, m_nAddr);
	m_pClient[nClient]->m_ClientNum = nClient;
	//m_pClient[nClient]->m_pWnd		= this->m_pWnd;
	//m_pClient[nClient]->m_ClientNum = nClient;
	//m_pClient[nClient]->m_nAddr		= m_nAddr;

	m_nClientUsed[nClient] = 1;

	if (m_pWnd != NULL)
	{
		::PostMessage(m_pWnd->m_hWnd, WM_SERVER_MSG + m_nAddr, SERVER_ACCEPT, nClient);
	}

	CAsyncSocket::OnAccept(nErrorCode);
}


bool CServerSocket::OnOpen(CWnd *pWnd, int nPort, int nAddr)
{
	m_nClientCount = 0;
	memset(m_nClientUsed, 0, sizeof(m_nClientUsed));

	if (this->Create(nPort))
	{
		this->Listen();

		if (pWnd != NULL)
		{
			m_pWnd = pWnd;
		}
		else
		{
			m_pWnd = NULL;
		}

		m_nAddr = nAddr;
		return true;
	}

	return false;
}

CServerClient *CServerSocket::GetAcceptSocket(int nClient)
{
	return m_pClient[nClient];


}
void CServerSocket::OnSendMessage(CString strMsg, int nClient)
{
	if (m_nClientUsed[nClient])
	{
		m_pClient[nClient]->Send(strMsg, strMsg.GetLength());
	}
}

void CServerSocket::OnCloseClient(int nClient)
{
	if (m_pClient[nClient] != NULL)
	{
		delete m_pClient[nClient];
		m_pClient[nClient] = NULL;
	}
}

void CServerSocket::OnClose()
{
	for(int i=0; i<MAX_USER; i++)
	{
		if (m_pClient[i] != NULL)
		{
			delete m_pClient[i];
			m_pClient[i] = NULL;
		}
	}
}


int CServerSocket::OnClientNumber()
{
	int nClinet = -1;
	int i;

	for(i=0; i<MAX_USER; i++)
	{
		if (m_pClient[i] == NULL)
		{
			nClinet = i;
			break;
		}
	}

	return nClinet;
}

int CServerSocket::OnClientAccept()
{
	int i;
	int nClient = -1;;

	for(i=0; i<MAX_USER; i++)
	{
		if (m_pClient[i] != NULL)
		{
			nClient = i;
			break;
		}
	}

	return nClient;
}