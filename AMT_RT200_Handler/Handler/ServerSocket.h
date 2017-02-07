#if !defined(AFX_SERVERSOCKET_H__ED37D999_5F86_452D_B820_3BE62583F1A8__INCLUDED_)
#define AFX_SERVERSOCKET_H__ED37D999_5F86_452D_B820_3BE62583F1A8__INCLUDED_

#include "ServerClient.h"
#define MAX_USER 10
#define WM_ACCEPT_SOCKET WM_USER+1
#define WM_CLOSECLIENT	 WM_USER+3
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerSocket.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CServerSocket command target
#include <iostream>
#include <queue>
#include <string>
using namespace std;

class CServerSocket : public CAsyncSocket
{
// Attributes
public:
	queue<CString>		m_qData;  // 구분자 단위의 문자열 저장 데이터 큐

	CWnd				*m_pWnd;

	int					m_nAddr;
	int					m_nClientCount;
	int					m_nClientUsed[MAX_USER];

	CServerClient		*m_pClient[MAX_USER];
	CServerClient		*GetAcceptSocket(int nClient);

	bool				OnOpen(CWnd *pWnd, int nPort, int nAddr);
	void				OnClose();
	int					OnClientNumber();
	int					OnClientAccept();
	void				OnSendMessage(CString strMsg, int nClient);
	void				OnCloseClient(int nClient);
// Operations
public:
	CServerSocket();
	virtual ~CServerSocket();
	
// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerSocket)
	public:
	virtual void OnAccept(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CServerSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERSOCKET_H__ED37D999_5F86_452D_B820_3BE62583F1A8__INCLUDED_)
