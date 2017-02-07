// Comm.cpp: implementation of the CComm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Comm.h"
#include "Variable.h"
#include "PublicFunction.h"

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


// XonOff, �� ���ϰ� ���� ����.
void CComm::SetXonOff(BOOL chk)
{
	fXonXoff = chk;
}

//Com Port�� �����Ѵ�.
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

// ����Ʈ ������ �����. �̰��� ���� �� ������ 
// SetComPort(); -> SetXonOff() -> SetDtrRts() �� ���� �����Ѵ�.
BOOL CComm::CreateCommInfo()
{
	osWrite.Offset = 0;
	osWrite.OffsetHigh = 0;
	osRead.Offset = 0;
	osRead.OffsetHigh = 0;

	// �̺�Ʈ â�� ����

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

//����Ʈ�� ������ �����Ѵ�.
BOOL CComm::DestroyComm()
{
	if(fConnected) CloseConnection();

	CloseHandle(osRead.hEvent);
	CloseHandle(osWrite.hEvent);

	return (TRUE);
}

//����Ʈ�κ��� �����͸� �д´�.
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
			//�̰��� ������ �ִ�. �� ReadFile������ �����Ͱ� ����� �� ������ 
			//fReadState�� ���� ���� �ڵ带 �����Ѵ�. �̶� ������ �� ������ ������
			//�������� ������ �Ұ����ϴ�. ����, ������� �ش޶�� �޽����� ���ִ� ���� ����.
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
		//����Ʈ�� �����͸� ����� ����� ���ؾ� ����̴�.
		//�̶��� ��� �ұ�? �װ��� ����� �����̴�. �ٽú����� ������ ��ͼ����� �ϸ�ȴ�.
		//�׷��� ���� ������ �� �� �ִٴ� ���� ��������.
	}
	return (TRUE);
}

// ����Ʈ�� ���� ������ �õ��Ѵ�.
BOOL CComm::OpenComPort()
{
	char szPort[15];
	BOOL fRetVal;
	CString strComm;
	COMMTIMEOUTS CommTimeOuts;

	if(bPort > MAXPORTS) 
	{
		clsFunc.OnStringToChar(_T("\\\\.\\TELNET"), szPort);
	}
	else
	{
		strComm.Format(_T("COM%d"), bPort);
	}

	// COMM device�� �������� �����Ѵ�.

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
		//����Ʈ���� �����͸� ��ȯ�ϴ� ����� char ������ �⺻���� ��������.
		SetCommMask(idComDev, EV_RXCHAR);
		SetupComm(idComDev, 4096, 4096);
		//����̽��� �����Ⱑ ������ �𸣴ϱ� ������ û�Ҹ� ����.
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

	if(fRetVal) //������ �Ǿ��ٸ� fRetVal TRUE�̹Ƿ�
	{
		fConnected = TRUE;
		// ���ν����� CommWatchProc�� �����ϴϱ� ���߿� �����Ͱ� �Դٰ��� �ϸ� ��� ������ CommWatchProc�� ����Ѵ�.
		AfxBeginThread((AFX_THREADPROC)CommWatchProc, (LPVOID)this);
	}
	else
	{
		fConnected = FALSE;
		CloseHandle(idComDev);
	}

	return (fRetVal);
}

// ���Ϸ� ������ ����Ʈ�� ���� ��Ʈ�� �����Ų��.
//SetupConnection������ CreateComPort�� ���־�� �Ѵ�.
BOOL CComm::SetupConnection()
{
	BOOL	fRetVal;
//	BYTE	bSet;
	DCB		dcb;

	dcb.DCBlength = sizeof(DCB);

	GetCommState(idComDev, &dcb); //dcb�� �⺻���� �޴´�.

	//�� �κ��� �����ؾ� �Ѵ�.
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

//������ �ݴ´�.
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

//�����͸� �а� �����͸� �о��ٴ� �޽����� �����Ѵ�.
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

//�޽����� ������ hwnd ����
void CComm::SetHwnd(HWND hwnd)
{
	m_hwnd=hwnd;
}

//--------------------------------------------------------------------------
// ����� �ϴ� ���ν���, �� ���氡 ������ �� �����ϴ� ��ƾ.
// �� ��ƾ�� OpenComPort �Լ� ����� ���ν����� �����.
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

	//idCommDev��� �ڵ鿡 �ƹ��� ����Ʈ�� �� �پ� ������ ���� ����
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
			if (npComm->m_byEnd == ' ')
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
//						npComm->m_bRevFlag	= true;
					}
				}while(nLength > 0);
			}
		}
	}

	CloseHandle(os.hEvent);
	npComm->idComDev = NULL;

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
