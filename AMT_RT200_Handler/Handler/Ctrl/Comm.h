// Comm.h: interface for the CComm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMM_H__10748263_8A1C_11D4_9B25_A7CF3E9F990A__INCLUDED_)
#define AFX_COMM_H__10748263_8A1C_11D4_9B25_A7CF3E9F990A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define MAXBLOCK 80
#define MAXBLOCK 1000
#define MAXPORTS 8

// Flow control flags
//#define FC_DTRDSR  0X01
//#define FC_RTSCTS  0X02
//#define FC_XONXOFF 0X04

// ascii definitions
//#define ASCII_BEL		0X07
//#define ASCII_BS		0x08
//#define ASCII_LF		0X0A
//#define ASCII_CR		0X0D
//#define ASCII_XON		0X11
//#define ASCII_XOFF		0X13
#define WM_RECEIVEDATA	WM_USER+10

// golbal stuff
// function prototypes (private)

#define STX		0x02
#define ETX		0x03
#define ACK		0x06
#define NACK	0x15
#define ENQ		0x05

class CComm : public CObject 
{
	DECLARE_DYNCREATE(CComm)
public:
	BYTE	m_byEnd;

	HANDLE	idComDev ; // ����Ʈ ����̽� ���� �ڵ�
	BOOL	fConnected ; // ����Ʈ�� ����Ǹ� 1�� ����
	BYTE	byend;
	BYTE	abIn[MAXBLOCK+1] ; // ����Ʈ���� ������ ������
	HWND	m_hwnd; // �޼����� ������ ������ �÷���

	// �޽����� ���� ������ �÷��׸� �����Ѵ�.
	void SetHwnd(HWND hwnd);
	// ���� �����͸� ���ۿ� �����Ѵ�.
	void SetReadData(LPSTR data);
	// ������ �����Ѵ�.
	BOOL CloseConnection();
	// ��Ʈ�� �����Ѵ�.
	BOOL SetupConnection();
	// ����Ʈ�� ���� ������ �õ��Ѵ�.
	BOOL OpenComPort();
	// ����Ʈ�� �����͸� �ִ´�.
	BOOL WriteCommBlock(LPSTR, DWORD);
	// ����Ʈ���� �����͸� �޴´�.
	int ReadCommBlock(LPSTR, int);
	// Comm Port�� �����Ѵ�.
	BOOL DestroyComm();
	// Com Port�� �����.
	BOOL CreateCommInfo();
	// Dtr Rts ����.
	void SetDtrRts(BYTE chk);
	// ����Ʈ�� ������.
	void SetComPort(int port, DWORD rate, BYTE bytesize, BYTE stop, BYTE parity);
	// XonOff ����
	void SetXonOff(BOOL chk);

	CString				OnCharToString(char chData[], int nLength);
	void				OnStringToChar(CString strData, char chData[]);

	CComm();
	virtual ~CComm();

	// Attributes
public:
	BYTE		 bPort;
	BOOL		 fXonXoff;
	BYTE		 bByteSize;
	BYTE		 bFlowCtrl;
	BYTE		 bParity;
	BYTE		 bStopBits;
	DWORD		 dwBaudRate;
	HANDLE		 hWatchTread;
	HWND		 hTermWnd;
	DWORD		 dwThreadID;
	OVERLAPPED	 osWrite;
	OVERLAPPED	 osRead;

	// Operations
public:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComm)
	//}}AFX_VIRTUAL

	// Implementation
public:
	//	CBR_19200;//dwBaudRate; 
	//	8;//bByteSize;  
	//	EVENPARITY;//bParity; 
	//	TWOSTOPBITS;//bStopBits;
	BOOL	OpenSerial(int nPort, int nBaudRate, int nData,int nStopBit,int nParity);
	int		ReadData(BYTE *data, int nLength);
	BOOL	isReady(int size);
	int		m_nCount;
	int		m_nRear;   // �о ������ ��ȣ
	int		m_nFront;  // ���� ������ ��ȣ
	int		m_nLength;
	bool	m_bRevFlag;
	void	Empty();

	// Generated message map functions
	// DECLARE_MESSAGE_MAP()
protected:

};

#endif // !defined(AFX_COMM_H__10748263_8A1C_11D4_9B25_A7CF3E9F990A__INCLUDED_)
