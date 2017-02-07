#pragma once

class CComm;
class CBarcode_1D
{

public:
	DWORD					m_dwWaitTime[3];

	CString					m_strBarcode[2];
	CString					m_strTrigger;

	int						m_nCommand;
	int						m_nX;
	int						m_nY;
	int						m_nBin;

	void					OnDataSend(CString strData);
	void					OnDataRevice(CString strData);

	CString					OnCharToString(char chData[], int nLength);
	void					OnStringToChar(CString strData, char chData[]);

public:
	CBarcode_1D(void);
	~CBarcode_1D(void);
};
extern CBarcode_1D clsBcr1D;
