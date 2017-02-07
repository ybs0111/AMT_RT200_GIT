// Comm.cpp: implementation of the CComm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Comm.h"
#include "Variable.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CComm, CObject)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern DWORD CommWatchProc(LPVOID lpData);


CComm::CComm()
{
	idComDev	= NULL;
	//bFlowCtrl = FC_XONXOFF;
	bFlowCtrl	= 0x04;
	fConnected	= FALSE;
	m_nCount	= 0;
	m_byEnd		= 13;
	m_bRevFlag	= false;
}

CComm::~CComm()
{
	DestroyComm();
}

//BEGIN_MESSAGE_MAP(CComm, CObject)
//{AFX_MSG_MAP(CComm)
// NOTE - the ClassWizard will add and remove mapping macros here.
//}}AFX_MSG_MAP
//END_MESSAGE_MAP()


// XonOff, 즉 리턴값 더블 설정.
void CComm::SetXonOff(BOOL chk)
{
	fXonXoff = chk;
}

//Com Port를 설정한다.
void CComm::SetComPort(int port, DWORD rate, BYTE bytesize, BYTE stop, BYTE parity)
{
	bPort = port;
	dwBaudRate = rate;
	bByteSize = bytesize;
	bStopBits = stop;
	bParity = parity;
}


void CComm::SetDtrRts(BYTE chk)
{
	bFlowCtrl = chk;
}

// 컴포트 정보를 만든다. 이것을 만들 때 이전에 
// SetComPort(); -> SetXonOff() -> SetDtrRts() 한 다음 설정한다.
BOOL CComm::CreateCommInfo()
{
	osWrite.Offset = 0;
	osWrite.OffsetHigh = 0;
	osRead.Offset = 0;
	osRead.OffsetHigh = 0;

	// 이벤트 창구 설정

	osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if(osRead.hEvent == NULL) return FALSE;

	osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if(NULL == osWrite.hEvent)
	{
		CloseHandle(osRead.hEvent);
		return FALSE;
	}

	return TRUE;
}

//컴포트를 완전히 해제한다.
BOOL CComm::DestroyComm()
{
	if(fConnected) CloseConnection();

	CloseHandle(osRead.hEvent);
	CloseHandle(osWrite.hEvent);

	return (TRUE);
}

//컴포트로부터 데이터를 읽는다.
int CComm::ReadCommBlock(LPSTR lpszBlock, int nMaxLength)
{
	BOOL	fReadStat;
	COMSTAT	ComStat;
	DWORD	dwErrorFlags;
	DWORD	dwLength[2];
	//only try to read number of bytes in queue
	ClearCommError(idComDev, &dwErrorFlags, &ComStat);

	dwLength[0] = min((DWORD) nMaxLength, ComStat.cbInQue);

	if(dwLength[0] > 0)
	{
//		fReadStat = ReadFile(idComDev, lpszBlock, dwLength, &dwLength, &osRead);
		fReadStat = ReadFile(idComDev, lpszBlock, dwLength[0], &dwLength[1], &osRead);

		if(!fReadStat)
		{
			//이곳에 에러를 넣다. 즉 ReadFile했을때 데이터가 제대로 안 나오면 
			//fReadState에 여러 에러 코드를 리턴한다. 이때 복구할 수 있으면 좋지만
			//실질적인 복구가 불가능하다. 따라서, 재송출을 해달라는 메시지를 해주는 것이 좋다.
		}
	}

	return (dwLength[0]);
}

BOOL CComm::WriteCommBlock(LPSTR lpByte, DWORD dwBytesToWrite)
{
	BOOL	fWriteStat;
	DWORD	dwBytesWritten;
	
//	Empty();
//	m_bRevFlag = false;

	fWriteStat = WriteFile(idComDev, lpByte, dwBytesToWrite, &dwBytesWritten, &osWrite);
	if(!fWriteStat)
	{
		//컴포트에 데이터를 제대로 써넣지 못해쓸 경우이다.
		//이때는 어떻게 할까? 그것은 사용자 마음이다. 다시보내고 싶으면 재귀송출을 하면된다.
		//그러나 무한 루프를 돌 수 있다는 점을 주의하자.
	}
	return (TRUE);
}

// 컴포트를 열고 연결을 시도한다.
BOOL CComm::OpenComPort()
{
	char szPort[15];
	BOOL fRetVal;
	CString strComm;
	COMMTIMEOUTS CommTimeOuts;

	if(bPort > MAXPORTS) 
	{
//		clsFunc.OnStringToChar(_T("\\\\.\\TELNET"), szPort);
#ifdef UNICODE
					OnStringToChar(_T("\\\\.\\TELNET"), szPort);
#else
					sprintf(szPort, _T("\\\\.\\TELNET"));
#endif
	}
	else
	{
		strComm.Format(_T("COM%d"), bPort);
	}

	// COMM device를 형식으로 연결한다.

	idComDev = CreateFile( strComm, 
						   GENERIC_READ | GENERIC_WRITE, 
						   0, 
						   NULL,
						   OPEN_EXISTING, 
						   FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 
						   NULL);

	if(idComDev == (HANDLE) -1)
	{
		return (FALSE);
	}
	else
	{
		//컴포트에서 데이터를 교환하는 방법을 char 단위를 기본으로 설정하자.
		SetCommMask(idComDev, EV_RXCHAR);
		SetupComm(idComDev, 4096, 4096);
		//디바이스에 쓰레기가 있을지 모르니까 깨끗이 청소를 하자.
		PurgeComm(idComDev, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
		m_nFront = 0;
		m_nRear = 0;

		CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
		CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
		CommTimeOuts.ReadTotalTimeoutConstant = 1000;
		CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
		CommTimeOuts.WriteTotalTimeoutConstant = 1000;

		SetCommTimeouts(idComDev, &CommTimeOuts);
	}

	fRetVal = SetupConnection();

	if(fRetVal) //연결이 되었다면 fRetVal TRUE이므로
	{
		fConnected = TRUE;
		// 프로시저를 CommWatchProc에 연결하니까 나중에 데이터가 왔다갔다 하면 모든 내용은 CommWatchProc가 담당한다.
		AfxBeginThread((AFX_THREADPROC)CommWatchProc, (LPVOID)this);
	}
	else
	{
		fConnected = FALSE;
		CloseHandle(idComDev);
	}

	return (fRetVal);
}

// 파일로 설정된 컴포트와 실질 포트를 여결시킨다.
//SetupConnection이전에 CreateComPort를 해주어야 한다.
BOOL CComm::SetupConnection()
{
	BOOL	fRetVal;
//	BYTE	bSet;
	DCB		dcb;

	dcb.DCBlength = sizeof(DCB);

	GetCommState(idComDev, &dcb); //dcb의 기본값을 받는다.

	//이 부분을 수정해야 한다.
	dcb.BaudRate		= dwBaudRate; 
	dcb.ByteSize		= bByteSize;  
	dcb.Parity			= bParity; 
	dcb.StopBits		= bStopBits;

	dcb.fOutxDsrFlow	= 0; //Dsr Flow
	dcb.fDtrControl		= DTR_CONTROL_ENABLE;
	dcb.fOutxCtsFlow	= 0;
	dcb.fRtsControl		= RTS_CONTROL_ENABLE;
	dcb.fInX			= dcb.fOutX;
	//dcb.XonChar = ASCII_XON;
	//dcb.XoffChar = ASCII_XOFF;
	dcb.XonChar			= 0x11;
	dcb.XoffChar		= 0x13;
	dcb.XonLim			= 100;
	dcb.XoffLim			= 100;
	dcb.fBinary			= TRUE;
	dcb.fParity			= TRUE;

	fRetVal = SetCommState(idComDev, &dcb);
	DWORD dRet = GetLastError();

	return (fRetVal);
}

//연결을 닫는다.
BOOL CComm::CloseConnection()
{
	// set connected flag to FALSE

	fConnected = FALSE;

	//disable event notification and wait for thread to halt
	
	SetCommMask(idComDev, 0);

	EscapeCommFunction(idComDev, CLRDTR);

	PurgeComm(idComDev, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

	CloseHandle(idComDev);

	return (TRUE);
}

//데이터를 읽고 데이터를 읽었다는 메시지를 리턴한다.
void CComm::SetReadData(LPSTR data)
{
	abIn[m_nFront++] = data[0];
	if (data[0] == m_byEnd)
	{
		m_nLength	= m_nFront;

		m_bRevFlag	= true;
	}
	else
	{
		// jtkim barcode
		if (m_nFront > 200)
		{
			Empty();
		}
	}
}

//메시지를 전달할 hwnd 설정
void CComm::SetHwnd(HWND hwnd)
{
	m_hwnd=hwnd;
}

//--------------------------------------------------------------------------
// 통신을 하는 프로시저, 즉 데잍가 들어왔을 때 감시하는 루틴.
// 이 루틴은 OpenComPort 함수 실행시 프로시저로 연결됨.
//--------------------------------------------------------------------------
DWORD CommWatchProc(LPVOID lpData)
{
	DWORD		dwEvtMask;
	OVERLAPPED	os;
	CComm*		npComm = (CComm*) lpData;
	char		InData[MAXBLOCK+1];

	int			nLength;
	int			nTotLen = 0;
	int			i;

	//idCommDev라는 핸들에 아무런 컴포트가 안 붙어 있으면 에러 리턴
	if(npComm == NULL || !npComm->IsKindOf(RUNTIME_CLASS(CComm) ) ) return (DWORD) (-1);

	memset(&os, 0, sizeof(OVERLAPPED) );

	os.hEvent = CreateEvent(NULL,    // no security
							TRUE,    // explicit reset req
							FALSE,   // initial event reset
							NULL);   // no name

	if(os.hEvent == NULL)
	{
		return (FALSE);
	}

	if(!SetCommMask(npComm->idComDev, EV_RXCHAR)) return FALSE;

	while(npComm->fConnected)
	{
		dwEvtMask = 0;
		WaitCommEvent(npComm->idComDev, &dwEvtMask, NULL);

		if((dwEvtMask & EV_RXCHAR) == EV_RXCHAR)
		{
			if (npComm->m_byEnd == 0x00)
			{
				do
				{
					if (nLength = npComm->ReadCommBlock((LPSTR)InData, MAXBLOCK))//MAXBLOCK))
					{
						for (i=0; i<nLength; i++)
						{
							npComm->abIn[i] = InData[i];
						}
						npComm->m_nFront	= nLength;
						npComm->m_bRevFlag	= true;
					}
				}while(nLength > 0);
			}
			else
			{
				do
				{
					nLength = npComm->ReadCommBlock((LPSTR)InData, 1);
					if (nLength > 0)
					{
						npComm->SetReadData(InData);
					}
				}while(nLength > 0);
			}
		}
	}

	CloseHandle(os.hEvent);
//	npComm->idComDev = NULL;

	return TRUE;
}


void CComm::Empty()
{
	m_bRevFlag = false;

	PurgeComm(idComDev, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);

	m_nFront = 0;
	m_nRear = 0;
}

BOOL CComm::isReady(int size)
{
	if(m_nFront-m_nRear >= size) return TRUE;
	else return FALSE;
}

BOOL CComm::ReadData(BYTE *data, int nLength)
{
//	if (!m_bRevFlag) return m_nLength;
	int i;

	if(m_nFront-m_nRear < nLength)
	{
		for(i=0; i<m_nFront; i++)
		{
			data[i] = abIn[m_nRear++ % MAXBLOCK];
		}

		m_nLength	= 0;
		m_bRevFlag	= false;

		return FALSE;
	}

	for(i=0; i<nLength; i++)
	{
		data[i] = abIn[m_nRear++ % MAXBLOCK];
	}

	m_nLength	= 0;
	m_bRevFlag	= false;

	return TRUE;
}
//	CBR_19200;//dwBaudRate; 
//	8;//bByteSize;  
//	EVENPARITY;//bParity; 
//	TWOSTOPBITS;//bStopBits;
BOOL CComm::OpenSerial(int nPort, int nBaudRate, int nData,int nStopBit,int nParity)
{
	BOOL bRet;

	SetComPort(nPort,nBaudRate,nData,nStopBit,nParity);
	CreateCommInfo();
	bRet = OpenComPort(); 

	return bRet;
}

CString	CComm::OnCharToString(char chData[], int nLength)
{
	CString strTmp;
	BSTR	buf;
	int		nCnt;

	nCnt	= MultiByteToWideChar(CP_ACP, 0, chData, nLength, NULL, NULL);
	buf		= SysAllocStringLen(NULL, nCnt);
	MultiByteToWideChar(CP_ACP, 0, chData, nLength, buf, nCnt);
	strTmp.Format(_T("%s"), buf);

	SysFreeString(buf);

	return strTmp;
}

void CComm::OnStringToChar(CString strData, char chData[])
{
	int nCnt;
	wchar_t *chBuf;

	chBuf = strData.GetBuffer(strData.GetLength());
	nCnt  = WideCharToMultiByte(CP_ACP, 0, chBuf, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, chBuf, -1, chData, nCnt, 0, 0);
}
