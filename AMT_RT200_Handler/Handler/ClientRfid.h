#pragma once
class CClientRfid
{
public:
	CString					m_strData;
	CString					m_strRfidData[20];

	DWORD					m_dwWaitTime[3];

	int						m_nRfidPos[50];
	int						m_nRfidNum[50];

	void					OnCommunication();
	CString					OnCharToString(char chData[], int nLength);
	void					OnStringToChar(CString strData, char chData[]);
	void					OnDataReceive(CString strData);
	bool					OnMakeWriteCode(int nCode, int nChann, CString strCode);
	bool					OnMakeReadCode(int nCode, int nChann);
	bool					OnMakeWriteCodeAll(int nChann, CString strCode[20]);
	bool					OnMakeReadCodeAll(int nChann);
	int						OnHexToDecimal(CString strHex);

public:
	bool					m_bThread;

	int						m_nRetry;
	int						m_nCommStep;
	int						m_nCommBin;

	CString					m_strSendMsg;
	CString					m_strErrorMsg;

	HANDLE					m_thrHandle;

	CClientRfid(void);
	~CClientRfid(void);
};
extern CClientRfid clsRfid;
