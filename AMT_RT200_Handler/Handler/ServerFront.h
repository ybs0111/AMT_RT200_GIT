#pragma once

typedef struct _FRONT_SV_QUEUE_
{
	int			m_nRear;
	int			m_nFront;
	int			m_nCount;

	char		m_chMsg[100][5000];
} FRONT_SV_QUEUE, *P_FRONT_SV_QUEUE;

//class CFtpComm;

class CServerFront
{
public:
//	CFtpComm			*m_pFtp;

	HANDLE				m_hMemMapFront;
	P_FRONT_SV_QUEUE	m_pQueueFront;

	int					m_nNewLotChk;
	int					m_nHeader;
	int					m_nHeaderLength;

	CString				m_strHeader;
	CString				m_strData;

	void				OnCommunication();
	CString				OnCharToString(char chData[], int nLength);
	void				OnStringToChar(CString strData, char chData[]);
	void				OnMessageWrite(CString strMsg);
	CString				OnMessageRead();
	void				OnDataReceive(CString strMsg);
	void				OnDataAnalysis(CString strMsg);
	CString				OnDataItemAnalysis(int nOpt, int nPos, CString strMsg, CString strFind);

public:
	CServerFront(void);
	~CServerFront(void);
};
extern CServerFront clsFront;
