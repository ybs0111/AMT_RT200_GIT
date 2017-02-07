// ClientSocket.cpp : implementation file
//

#include "stdafx.h"
//#include "Handler.h"
#include "ClientSocket.h"
#include "Variable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientSocket


CClientSocket::CClientSocket()
{
	m_bConnect	= false;
	m_pWnd		= NULL;
}

CClientSocket::~CClientSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CClientSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CClientSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CClientSocket member functions


void CClientSocket::OnReceive(int nErrorCode) 
{
	CString strMsg;

	m_nLength = Receive(m_chRev, 8196);

	strMsg		= (CString) m_chRev;
	st_client_info.strRev = strMsg.Mid(0, m_nLength);

	if (m_pWnd != NULL)
	{
		::SendMessage(m_pWnd->m_hWnd, WM_CLIENT_MSG, CLIENT_REV, 0);
	}

	CAsyncSocket::OnReceive(nErrorCode);
}

void CClientSocket::OnOpen(CWnd *pWnd, int nAddr)
{
	m_pWnd	= pWnd;
	m_nAddr	= nAddr;
}

void CClientSocket::OnClose(int nErrorCode) 
{
	if (m_pWnd != NULL)
	{
		::SendMessage(m_pWnd->m_hWnd, WM_CLIENT_MSG, CLIENT_CLOSE, 0);
	}

	CAsyncSocket::OnClose(nErrorCode);
}


void CClientSocket::OnData()
{
//	PostMessage(m_pWnd->m_hWnd, WM_WORK_COMMAND, 0, 0);

	if (m_pWnd != NULL)
	{
//		m_pWnd->SendMessage(WM_WORK_COMMAND, 0, 0);
	}
}


void CClientSocket::OnConnect(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (m_pWnd != NULL)
	{
		::SendMessage(m_pWnd->m_hWnd, WM_CLIENT_MSG, CLIENT_ACCEPT, nErrorCode);
	}

	CAsyncSocket::OnConnect(nErrorCode);
}


void CClientSocket::OnAccept(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	CAsyncSocket::OnAccept(nErrorCode);
}
