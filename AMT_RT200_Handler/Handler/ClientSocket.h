#if !defined(AFX_CLIENTSOCKET_H__877C34C3_4FB8_4EDE_B22A_1BA282D77D22__INCLUDED_)
#define AFX_CLIENTSOCKET_H__877C34C3_4FB8_4EDE_B22A_1BA282D77D22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClientSocket.h : header file
//
#include <iostream>
#include <queue>
#include <string>
using namespace std;

class CClientSocket : public CAsyncSocket
{
// Attributes
public:
	bool				m_bConnect;
	char				m_chRev[8196];

	CString				m_strRev;
	CString				m_strBuff;

	int					m_nLength;
	int					m_nAddr;

	CWnd				*m_pWnd;
	void				OnData();
	void				OnOpen(CWnd *pWnd, int nAddr);
// Operations
public:
	CClientSocket();
	virtual ~CClientSocket();
	
// Overrides
public:
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CClientSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
public:
	virtual void OnConnect(int nErrorCode);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTSOCKET_H__877C34C3_4FB8_4EDE_B22A_1BA282D77D22__INCLUDED_)
