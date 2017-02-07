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

	HANDLE	idComDev ; // 컴포트 디바이스 연결 핸들
	BOOL	fConnected ; // 컴포트가 연결되면 1로 설정
	BYTE	byend;
	BYTE	abIn[MAXBLOCK+1] ; // 컴포트에서 들어오는 데이터
	HWND	m_hwnd; // 메세지를 전달할 위도우 플래그

	// 메시지를 보낼 위도우 플래그를 설정한다.
	void SetHwnd(HWND hwnd);
	// 읽은 데이터를 버퍼에 저장한다.
	void SetReadData(LPSTR data);
	// 연결을 해제한다.
	BOOL CloseConnection();
	// 포트를 연결한다.
	BOOL SetupConnection();
	// 컴포트를 열고 연결을 시도한다.
	BOOL OpenComPort();
	// 컴포트에 데이터를 넣는다.
	BOOL WriteCommBlock(LPSTR, DWORD);
	// 컴포트에서 데이터를 받는다.
	int ReadCommBlock(LPSTR, int);
	// Comm Port를 해제한다.
	BOOL DestroyComm();
	// Com Port를 만든다.
	BOOL CreateCommInfo();
	// Dtr Rts 설정.
	void SetDtrRts(BYTE chk);
	// 컴포트를 설정함.
	void SetComPort(int port, DWORD rate, BYTE bytesize, BYTE stop, BYTE parity);
	// XonOff 설정
	void SetXonOff(BOOL chk);

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
	int		m_nRear;   // 읽어간 데이터 번호
	int		m_nFront;  // 들어온 데이터 번호
	int		m_nLength;
	bool	m_bRevFlag;
	void	Empty();

	// Generated message map functions
	// DECLARE_MESSAGE_MAP()
protected:

};

#endif // !defined(AFX_COMM_H__10748263_8A1C_11D4_9B25_A7CF3E9F990A__INCLUDED_)
