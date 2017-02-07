#if !defined(AFX_SERVERCLIENT_H__83BDA1EB_BA16_4020_B0A1_947BEF9B77D6__INCLUDED_)
#define AFX_SERVERCLIENT_H__83BDA1EB_BA16_4020_B0A1_947BEF9B77D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerClient.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CServerClient command target
class CServerSocket;

class CServerClient : public CAsyncSocket
{
public:
	//wchar_t			m_chRev[4096];
	char			m_chRev[8196];

	CString			m_strRev;
	CString			m_strBuff;

	int				m_nLength;
	int				m_nAddr;
	int				m_ClientNum;

	CWnd			*m_pWnd;

	CServerSocket	*m_pSocket;	// 서버 소켓
// Attributes
public:
	void			OnOpen(CWnd *pWnd, int nAddr);
// Operations
public:
	CServerClient(CServerSocket* pSocket);
	virtual ~CServerClient();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerClient)
	public:
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CServerClient)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERCLIENT_H__83BDA1EB_BA16_4020_B0A1_947BEF9B77D6__INCLUDED_)
