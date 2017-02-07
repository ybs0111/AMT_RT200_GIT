#pragma once

class CClientGms
{
public:
	int						m_nHeader;
	int						m_nHeaderLength;
	int						m_nCommStep;
	int						m_nRetry;
	int						m_nTimFlag;
	int						m_nAsdFlag;
	int						m_nPngFlag;

	CString					m_strHeader;
	CString					m_strData;
	CString					m_strSendMsg;
	CString					m_strErrorMsg;

	DWORD					m_dwWaitTime[3];

	HANDLE					m_thrHandle;

	bool					m_bThread;

public:
	void					OnCommunication();
	void					OnMessageWrite(CString strMsg);
	CString					OnMessageRead();
	CString					OnSyncTime();															// ���� <-> TTS VnR �ð� ����ȭ
	CString					OnAutoSendReq();														// �ڵ� ���� ��û
	void					OnDataReceive(CString strMsg);
	void					OnDataAnalysis(CString strMsg);
	CString					OnDataItemAnalysis(int nOpt, int nPos, CString strMsg, CString strFind);
	CString					OnDateFormat(CString strDate);
public:
	CClientGms(void);
	~CClientGms(void);
};
extern CClientGms clsGms;
