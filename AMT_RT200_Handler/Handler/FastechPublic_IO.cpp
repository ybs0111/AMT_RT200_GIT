// FastechPublic_IO.cpp: implementation of the CFastechPublic_IO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FastechPublic_IO.h"
#include "math.h"
#include "Variable.h"
#include "PublicFunction.h"
//#include "io.h"     // ���� ���� ���� �˻� �Լ� ȣ���ϱ� ���ؼ��� �ݵ�� INCLUDE �ʿ� 
// jtkim 20130625
#ifdef _WIN64
	#include ".\\64bit\\FAS_HSSI.h"
#else
	#include ".\\32bit\\FAS_HSSI.h"
#endif
#include "AlgMemory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFastechPublic_IO  FAS_IO;  

CFastechPublic_IO::CFastechPublic_IO()
{
}

CFastechPublic_IO::~CFastechPublic_IO()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
//2011.1019 james lee WORD ������ I/O�� �����Ѵ�
//Ver 0.0.1_2011.1019
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////


//2011.1019 james //Word ������ ����� ����ϴ� �Լ� 
BOOL CFastechPublic_IO::get_in_bit_wait(int nNum, int nIO, int nOnOff, int nTime)  //2011.1019 Simulation ���۱��� �������� onoff �߰� 
{
	if(st_work_info.nSimulationMode == 1) //Simulation ����̸� ���α׷��� �����ؾ� �ϴ� I/O ���¸� ���ϴ� ������ ������ �ش� 
	{		
		return n_in[nIO] = nOnOff;
	}

	m_dwWaitTime[nNum][nIO][1] = GetTickCount();
	m_dwWaitTime[nNum][nIO][2] = m_dwWaitTime[nNum][nIO][1] -  m_dwWaitTime[nNum][nIO][0];

	if (m_dwWaitTime[nNum][nIO][2] <= (DWORD)0 || m_dwWaitTime[nNum][nIO][0] <= (DWORD)0)
	{
		m_nWaitIO[nNum][nIO]		= n_in[nIO];		// jtkim 20160125
		m_dwWaitTime[nNum][nIO][0]	= GetTickCount();

		return -1;
	}

	if (m_bWaitRetry[nNum][nIO])
	{
		m_dwWaitTime[nNum][nIO][0]	= 0;
		m_bWaitRetry[nNum][nIO]		= FALSE;

		return n_in[nIO]; 
	}
	else
	{
		if (m_dwWaitTime[nNum][nIO][2] > (DWORD)nTime)
		{
			if (m_nWaitIO[nNum][nIO] == n_in[nIO])
			{
				m_dwWaitTime[nNum][nIO][0] = 0;

				return n_in[nIO]; 
			}
			m_bWaitRetry[nNum][nIO] = TRUE;
		}
	}

	return -1;
}


//2011.1019 james //Word ������ ����� ����ϴ� �Լ� 
BOOL CFastechPublic_IO::get_in_bit(int n_IONo, int n_OnOff)  //2011.1019 Simulation ���۱��� �������� onoff �߰� 
{
/*
	CString sMsg;
	int     nRet=0;
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0, nBitNo = 0;
	BOOL    bData=0;
*/
	if(st_work_info.nSimulationMode == 1) //Simulation ����̸� ���α׷��� �����ؾ� �ϴ� I/O ���¸� ���ϴ� ������ ������ �ش� 
	{		
		return n_out[n_IONo] = n_OnOff;
	}

	return n_in[n_IONo]; 
}

int CFastechPublic_IO::set_out_bit(int n_IONo, BOOL b_OnOff) 
{
/*
	CString sMsg;
	int     nRet=0;
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0, nBitNo=0;
*/

	n_out[n_IONo] = b_OnOff;
//	return HSSI_STATUS_SUCCESS;
// jtkim 20130625
#ifdef _WIN64
	return HSSI_STATUS_SUCCESS;
#else
	return STATUS_SUCCESS;
#endif


	//���ϴ� ������� �ʴ´� 
	/*
	nMasterID = Search_IO_BoardInformation(0, n_IONo);  // HSSI ���� ������ Ȯ�� �� �˻� �Լ�
	nPortNo   = Search_IO_BoardInformation(1, n_IONo);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo);
	nBitNo    = Search_IO_BoardInformation(3, n_IONo);

	nRet = HSSI_WriteOutputBit(nMasterID, nPortNo, nSlaveNo, nBitNo, b_OnOff);  // SLAVE I/O ��� ��� ��ȣ �߻� �Լ� [BIT ����]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;*/
}

BOOL CFastechPublic_IO::get_out_bit(int n_IONo, int n_OnOff)
{
/*
	CString sMsg;
	int     nRet=0;	
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0,nBitNo=0;
	BOOL    bData;
*/

	if(st_work_info.nSimulationMode == 1) //Simulation ����̸� ���α׷��� �����ؾ� �ϴ� I/O ���¸� ���ϴ� ������ ������ �ش� 
	{		
		return n_out[n_IONo] = n_OnOff;
	}

	return n_out[n_IONo];
	//���ϴ� ������� �ʴ´� 
	/*
	nMasterID = Search_IO_BoardInformation(0, n_IONo);  // HSSI ���� ������ Ȯ�� �� �˻� �Լ�
	nPortNo   = Search_IO_BoardInformation(1, n_IONo);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo);
	nBitNo    = Search_IO_BoardInformation(3, n_IONo);
	
	nRet = HSSI_ReadOutputBit(nMasterID, nPortNo, nSlaveNo, nBitNo, &bData);  // SLAVE I/O ��� �Է� ��ȣ READ �Լ� [BIT ����]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		bData = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}	
	return bData;*/
}

int CFastechPublic_IO::IO_ReadWrite(int n_bd_num, int n_port_num, int n_slave_num, int n_rw_mode)
{//n_rw_mode : -1:������ �ʱ�ȭ, 0=> initialize i/o ���� read, 1:input, 2:output
	WORD	wData = 0;
	int     i=0, nRet=0, nBoardID=-1, nPortNo=0, nSlaveNo=0, nIO_Num = 0;
 
//#define MAX_PCI_SLOT 8	// 0~7 8��
//#define MAX_PORT_NO 4	// 0~3 4��
//#define MAX_SLAVE_NO 8	// 0~7 8��


	//���� ������ �̿��Ͽ� io port ��ȣ�� �����Ѵ� 
	if(n_bd_num < 0 || n_bd_num > MAX_PCI_SLOT - 6) //MAX_PCI_SLOT - 1)
	{//	����� ���� 8����� ��밡���ϳ� ���� MAX�� 2������ �Ѵ� 
		AfxMessageBox(_T("IO Board [0 ~ 1] Max Num Over"));//[0 ~ 7] Max Num Over");
		return BD_ERROR;
	}
	else nBoardID = n_bd_num * 4000;

	if(n_port_num < 0 || n_port_num > MAX_PORT_NO - 1)
	{
		AfxMessageBox(_T("IO Board Port [0 ~ 3] Max Num Over"));
		return BD_ERROR;
	}
	else nPortNo   = n_port_num * 1000;

	if(n_slave_num < 0 || n_slave_num > MAX_SLAVE_NO - 1)
	{
		AfxMessageBox(_T("IO Board Slave [0 ~ 7] Max Num Over"));
		return BD_ERROR;
	}
	else nSlaveNo   = n_slave_num * 100;
	
	
    nIO_Num = nBoardID + nPortNo + nSlaveNo;
	if(nIO_Num > HSSI_MAX_IO - 1) //#define HSSI_MAX_IO		8716 //max io board 2�� ������ �����Ͽ� ������� //max io board 4�� ������ ó���Ѵ� 
	{
		AfxMessageBox(_T("IO Num [8715] Max Num Over"));
		return BD_ERROR;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//��� I/O port�� define �� �����Ʈ �ʱ�ȭ �� I/O ���� �������� �� ��Ʈ�� ���¸� �о� ������ �����Ͽ� �ش� 
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if(n_rw_mode == -1) //input, output port ������� ó�� �ѹ� �ʱ�ȭ �� �� 
	{
		for(i=0; i<HSSI_MAX_IO; i++) //��Ʈ ��� ���� �ʱ�ȭ 
		{
			n_in[i]  = 0;//-1;
			n_out[i] = -1;
		}

		////////////////////////////////////////////////////////////////
		// �Ʒ� ������ AMT8312 I/O define ���� ���� ���뿡 ���� ������ �����ߴ� 
		// �� �κ��� ��񺰷� Ʋ���� ������ Ʋ���� �� �� 
		////////////////////////////////////////////////////////////////

		//output ����ϴ� ���� 0 ���� �� �ʱ�ȭ �Ͽ� ��밡���ϰ� �Ѵ� 
		n_out[0]=0; n_out[1]=0;  n_out[15]=0;	//slave 0 0x8003);
		n_out[0]=0; n_out[1]=0;   //			//slave 1 0x0003)
		n_out[0]=0; n_out[1]=0; n_out[2]=0; n_out[3]=0; //slave 2 0x000f)
		n_out[0]=0; n_out[1]=0; n_out[2]=0; n_out[3]=0; //slave 3 0x000f)
		n_out[0]=0; n_out[1]=0; n_out[2]=0; n_out[3]=0; n_out[4]=0; n_out[5]=0; n_out[6]=0; n_out[7]=0; //slave 4 0x00ff)
		n_out[0]=0; n_out[1]=0; n_out[2]=0; n_out[3]=0; n_out[4]=0; n_out[5]=0; n_out[6]=0; n_out[7]=0; n_out[8]=0; n_out[9]=0; //slave 5 0x03ff)
		n_out[0]=0; n_out[1]=0;   //slave 6 0x0003)
		n_out[0]=0; n_out[1]=0; n_out[2]=0; n_out[3]=0; n_out[4]=0; n_out[5]=0; n_out[6]=0; n_out[7]=0; n_out[8]=0; n_out[9]=0; //slave 7 0x03ff)

	}

	if(st_work_info.nSimulationMode == 1) //Simulation ����̸� ���α׷��� �����ؾ� �ϴ� I/O ���¸� ���ϴ� ������ ������ �ش� 
	{//������ ���ķ��̼� ���� ���α׷� �׽�Ʈ�� ������ ������ ������ ������ Ŭ���� �Ǵ� �����Ѵ� 
		return BD_GOOD; 
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// ��� �ʱ�ȭ�� ó���ѹ� Input �� output port�� ���¸� �޴´� 
	//////////////////////////////////////////////////////////////////////////////////////////////////
	if(n_rw_mode == 0) //initialize total input/ output read mode 
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//���� ���� ���� �ѹ� �ʱ�ȭ
		// in/out ������ -1�̸� ������� �ʴ� ��Ʈ�̰�, 0 �Ǵ� 1�̸� ���������� ��񿡼� �ÿ��ϴ� �����̴� 
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		

		// ************************************************************************** //
		// �Է�(��� ��ȣ 00��)								                          //
		// ************************************************************************** //
		nRet = HSSI_ReadInputWord(n_bd_num, n_port_num, n_slave_num, &wData); //(iMasterID, iPortNo, iSlaveNo, &wData); 
//		if(nRet != HSSI_STATUS_SUCCESS){return BD_ERROR;};//error
// jtkim 20130625
#ifdef _WIN64
		if(nRet != HSSI_STATUS_SUCCESS){return BD_ERROR;};//error
#else
		if(nRet != STATUS_SUCCESS){return BD_ERROR;};//error
#endif
		n_in[nIO_Num + 0]	= (wData & DBIT0)   ? 1 : 0 ;		// PS000 (���: 0	��Ʈ: 0) 
		n_in[nIO_Num + 1]	= (wData & DBIT1)   ? 1 : 0 ;		// PS001 (���: 0	��Ʈ: 1) 
		n_in[nIO_Num + 2]	= (wData & DBIT2)   ? 1 : 0 ;		// PS002 (���: 0	��Ʈ: 2) 
		n_in[nIO_Num + 3]	= (wData & DBIT3)   ? 1 : 0 ;		// PS003 (���: 0	��Ʈ: 3) 
		n_in[nIO_Num + 4]	= (wData & DBIT4)   ? 1 : 0 ;		// PS004 (���: 0	��Ʈ: 4)
		n_in[nIO_Num + 5]	= (wData & DBIT5)   ? 1 : 0 ;		// PS005 (���: 0	��Ʈ: 5) 
		n_in[nIO_Num + 6]	= (wData & DBIT6)   ? 1 : 0 ;		// PS006 (���: 0	��Ʈ: 6) 
		n_in[nIO_Num + 7]	= (wData & DBIT7)   ? 1 : 0 ;		// PS007 (���: 0	��Ʈ: 7) 
		n_in[nIO_Num + 8]	= (wData & DBIT8)   ? 1 : 0 ;		// PS008 (���: 0	��Ʈ: 8) 
		n_in[nIO_Num + 9]	= (wData & DBIT9)   ? 1 : 0 ;		// PS009 (���: 0	��Ʈ: 9) 
		n_in[nIO_Num + 10]	= (wData & DBIT10)  ? 1 : 0 ;		// PS010 (���: 0	��Ʈ: 10) 
		n_in[nIO_Num + 11]	= (wData & DBIT11)  ? 1 : 0 ;		// PS011 (���: 0	��Ʈ: 11) 
		n_in[nIO_Num + 12]	= (wData & DBIT12)  ? 1 : 0 ;		// PS012 (���: 0	��Ʈ: 12) 
		n_in[nIO_Num + 13]	= (wData & DBIT13)  ? 1 : 0 ;		// PS013 (���: 0	��Ʈ: 13) 
		n_in[nIO_Num + 14]	= (wData & DBIT14)  ? 1 : 0 ;		// PS014 (���: 0	��Ʈ: 14) 
		n_in[nIO_Num + 15]	= (wData & DBIT15)  ? 1 : 0 ;		// PS015 (���: 0	��Ʈ: 15) 

		// ************************************************************************** //
		// ���(��� ��ȣ 00��)								                          //
		// ************************************************************************** //
		nRet = HSSI_ReadOutputWord(n_bd_num, n_port_num, n_slave_num, &wData); //(iMasterID, iPortNo, iSlaveNo, &wData); 
// jtkim 20130625
//		if(nRet != HSSI_STATUS_SUCCESS){return BD_ERROR;};//error
#ifdef _WIN64
		if(nRet != HSSI_STATUS_SUCCESS){return BD_ERROR;};//error
#else
		if(nRet != STATUS_SUCCESS){return BD_ERROR;};//error
#endif
		for(i=0; i<16; i++)//io port 16��
		{
			if(n_out[nIO_Num + i] == 0 || n_out[nIO_Num + i] == 1) //0:off, 1:on
			{
				if     (i == 0)	n_out[nIO_Num + 0]		= (wData & DBIT0)   ? 1 : 0 ;		// S000 (���: 0	��Ʈ: 0) 
				else if(i == 1)	n_out[nIO_Num + 1]		= (wData & DBIT1)   ? 1 : 0 ;		// S001 (���: 0	��Ʈ: 1) 
				else if(i == 2) n_out[nIO_Num + 2]		= (wData & DBIT2)   ? 1 : 0 ;		// S002 (���: 0	��Ʈ: 2) 
				else if(i == 3) n_out[nIO_Num + 3]		= (wData & DBIT3)   ? 1 : 0 ;		// S003 (���: 0	��Ʈ: 3) 
				else if(i == 4) n_out[nIO_Num + 4]		= (wData & DBIT4)   ? 1 : 0 ;		// S004 (���: 0	��Ʈ: 4)
				else if(i == 5) n_out[nIO_Num + 5]		= (wData & DBIT5)   ? 1 : 0 ;		// S005 (���: 0	��Ʈ: 5) 
				else if(i == 6) n_out[nIO_Num + 6]		= (wData & DBIT6)   ? 1 : 0 ;		// S006 (���: 0	��Ʈ: 6) 
				else if(i == 7) n_out[nIO_Num + 7]		= (wData & DBIT7)   ? 1 : 0 ;		// S007 (���: 0	��Ʈ: 7) 
				else if(i == 8) n_out[nIO_Num + 8]		= (wData & DBIT8)   ? 1 : 0 ;		// S008 (���: 0	��Ʈ: 8) 
				else if(i == 9) n_out[nIO_Num + 9]		= (wData & DBIT9)   ? 1 : 0 ;		// S009 (���: 0	��Ʈ: 9) 
				else if(i == 10) n_out[nIO_Num + 10]	= (wData & DBIT10)  ? 1 : 0 ;		// S010 (���: 0	��Ʈ: 10) 
				else if(i == 11) n_out[nIO_Num + 11]	= (wData & DBIT11)  ? 1 : 0 ;		// S011 (���: 0	��Ʈ: 11) 
				else if(i == 12) n_out[nIO_Num + 12]	= (wData & DBIT12)  ? 1 : 0 ;		// S012 (���: 0	��Ʈ: 12) 
				else if(i == 13) n_out[nIO_Num + 13]	= (wData & DBIT13)  ? 1 : 0 ;		// S013 (���: 0	��Ʈ: 13) 
				else if(i == 14) n_out[nIO_Num + 14]	= (wData & DBIT14)  ? 1 : 0 ;		// S014 (���: 0	��Ʈ: 14) 
				else if(i == 15) n_out[nIO_Num + 15]	= (wData & DBIT15)  ? 1 : 0 ;		// S015 (���: 0	��Ʈ: 15) 
			}
		}
	}	

	if(n_rw_mode == 1 || n_rw_mode == 2) //input read mode
	{
		// ************************************************************************** //
		// �Է�(��� ��ȣ 00��)								                          //
		// ************************************************************************** //
		nRet = HSSI_ReadInputWord(n_bd_num, n_port_num, n_slave_num, &wData); //(iMasterID, iPortNo, iSlaveNo, &wData); 
// jtkim 20130625
//		if(nRet != HSSI_STATUS_SUCCESS){return BD_ERROR;};//error
#ifdef _WIN64
		if(nRet != HSSI_STATUS_SUCCESS){return BD_ERROR;};//error
#else
		if(nRet != STATUS_SUCCESS){return BD_ERROR;};//error
#endif
		n_in[nIO_Num + 0]	= (wData & DBIT0)   ? 1 : 0 ;		// PS000 (���: 0	��Ʈ: 0) 
		n_in[nIO_Num + 1]	= (wData & DBIT1)   ? 1 : 0 ;		// PS001 (���: 0	��Ʈ: 1) 
		n_in[nIO_Num + 2]	= (wData & DBIT2)   ? 1 : 0 ;		// PS002 (���: 0	��Ʈ: 2) 
		n_in[nIO_Num + 3]	= (wData & DBIT3)   ? 1 : 0 ;		// PS003 (���: 0	��Ʈ: 3) 
		n_in[nIO_Num + 4]	= (wData & DBIT4)   ? 1 : 0 ;		// PS004 (���: 0	��Ʈ: 4)
		n_in[nIO_Num + 5]	= (wData & DBIT5)   ? 1 : 0 ;		// PS005 (���: 0	��Ʈ: 5) 
		n_in[nIO_Num + 6]	= (wData & DBIT6)   ? 1 : 0 ;		// PS006 (���: 0	��Ʈ: 6) 
		n_in[nIO_Num + 7]	= (wData & DBIT7)   ? 1 : 0 ;		// PS007 (���: 0	��Ʈ: 7) 
		n_in[nIO_Num + 8]	= (wData & DBIT8)   ? 1 : 0 ;		// PS008 (���: 0	��Ʈ: 8) 
		n_in[nIO_Num + 9]	= (wData & DBIT9)   ? 1 : 0 ;		// PS009 (���: 0	��Ʈ: 9) 
		n_in[nIO_Num + 10]	= (wData & DBIT10)  ? 1 : 0 ;		// PS010 (���: 0	��Ʈ: 10) 
		n_in[nIO_Num + 11]	= (wData & DBIT11)  ? 1 : 0 ;		// PS011 (���: 0	��Ʈ: 11) 
		n_in[nIO_Num + 12]	= (wData & DBIT12)  ? 1 : 0 ;		// PS012 (���: 0	��Ʈ: 12) 
		n_in[nIO_Num + 13]	= (wData & DBIT13)  ? 1 : 0 ;		// PS013 (���: 0	��Ʈ: 13) 
		n_in[nIO_Num + 14]	= (wData & DBIT14)  ? 1 : 0 ;		// PS014 (���: 0	��Ʈ: 14) 
		n_in[nIO_Num + 15]	= (wData & DBIT15)  ? 1 : 0 ;		// PS015 (���: 0	��Ʈ: 15) 
	}

	if(n_rw_mode == 1 || n_rw_mode == 2) //output mode
	{
		// ************************************************************************** //
		// �Է�(��� ��ȣ 00��)								                          //
		// ************************************************************************** //
		wData = 0;
		for(i=0; i<16; i++)//io port 16��
		{
			if(n_out[nIO_Num + i] == 0 || n_out[nIO_Num + i] == 1) //0:off, 1:on
			{
				if     (i == 0)  wData += n_out[nIO_Num + i]		* DBIT0;  // S000 (���: 0	��Ʈ: 0)
				else if(i == 1)  wData += n_out[nIO_Num + i]		* DBIT1;  // S001 (���: 0	��Ʈ: 1)
				else if(i == 2)  wData += n_out[nIO_Num + i]		* DBIT2;  // S002 (���: 0	��Ʈ: 2)
				else if(i == 3)  wData += n_out[nIO_Num + i]		* DBIT3;  // S003 (���: 0	��Ʈ: 3)
				else if(i == 4)  wData += n_out[nIO_Num + i]		* DBIT4;  // S004 (���: 0	��Ʈ: 4)
				else if(i == 5)  wData += n_out[nIO_Num + i]		* DBIT5;  // S005 (���: 0	��Ʈ: 5)
				else if(i == 6)  wData += n_out[nIO_Num + i]		* DBIT6;  // S006 (���: 0	��Ʈ: 6)
				else if(i == 7)  wData += n_out[nIO_Num + i]		* DBIT7;  // S007 (���: 0	��Ʈ: 7)
				else if(i == 8)  wData += n_out[nIO_Num + i]		* DBIT8;  // S008 (���: 0	��Ʈ: 8)
				else if(i == 9)  wData += n_out[nIO_Num + i]		* DBIT9;  // S009 (���: 0	��Ʈ: 9)
				else if(i == 10) wData += n_out[nIO_Num + i]		* DBIT10; // S010 (���: 0	��Ʈ: 10)
				else if(i == 11) wData += n_out[nIO_Num + i]		* DBIT11; // S011 (���: 0	��Ʈ: 11)
				else if(i == 12) wData += n_out[nIO_Num + i]		* DBIT12; // S012 (���: 0	��Ʈ: 12)
				else if(i == 13) wData += n_out[nIO_Num + i]		* DBIT13; // S013 (���: 0	��Ʈ: 13)
				else if(i == 14) wData += n_out[nIO_Num + i]		* DBIT14; // S014 (���: 0	��Ʈ: 14)
				else if(i == 15) wData += n_out[nIO_Num + i]		* DBIT15; // S015 (���: 0	��Ʈ: 15)
				

			}
		}

		nRet = HSSI_WriteOutputWord(n_bd_num, n_port_num, n_slave_num, wData);//(iMasterID, iPortNo, iSlaveNo, wData); //TRUE = enable, FALSE = Dasible
// jtkim 20130625
//		if(nRet != HSSI_STATUS_SUCCESS){return BD_ERROR;};//error 
#ifdef _WIN64
		if(nRet != HSSI_STATUS_SUCCESS){return BD_ERROR;};//error
#else
		if(nRet != STATUS_SUCCESS){return BD_ERROR;};//error
#endif
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//2011.1019 james lee WORD ������ I/O�� �����Ѵ�
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





// ******************************************************************************
// I/O ���� ���̺귯�� �ʱ�ȭ �Լ�                                                   
// ******************************************************************************
// ******************************************************************************
// I/O ���� ���� �Լ�                                                            
// ******************************************************************************
int CFastechPublic_IO::Set_IO_BoardOpen(INT* pn_MasterNo) 
{
	CString sMsg;
	int     nRet=0;
	INT     nMasterNo=0;

#ifdef _WIN64
	nRet = HSSI_Open(&nMasterNo);  // HSSI �ʱ�ȭ �Լ� 
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		*pn_MasterNo = nMasterNo;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
#else
	nRet = HSSI_Open(&nMasterNo, START_NOTHING);  // HSSI �ʱ�ȭ �Լ� 
	if (nRet == STATUS_SUCCESS)
	{
		*pn_MasterNo = nMasterNo;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
#endif

	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// I/O ���� ���� �Լ�                                                            
// ******************************************************************************
int CFastechPublic_IO::Set_IO_BoardClose(bool b_CloseMethod) 
{
	CString sMsg;
	int     nRet=0;

#ifdef _WIN64
	nRet = HSSI_Close();  // HSSI ���ҽ� ���� �Լ� 
	if (nRet == HSSI_STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
#else
	nRet = HSSI_Close(TRUE);  // HSSI ���ҽ� ���� �Լ� 
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
#endif
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ����� ����� ���� �Լ� [WORD ����]                                 
// ******************************************************************************
int CFastechPublic_IO::Set_IO_DefineWord(int n_MasterID, int n_PortNo, int n_SlaveNo, WORD w_Data) 
{
	CString sMsg;
	int     nRet=0;

	nRet = HSSI_WriteDirectionWord(n_MasterID, n_PortNo, n_SlaveNo, w_Data);  // SLAVE I/O ����� ����� ���� �Լ� [WORD ����]
// jtkim 20130625
/*
	if (nRet == HSSI_STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
*/
#ifdef _WIN64
	if (nRet == HSSI_STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
#else
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
#endif
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ����� ����� ���� �Լ� [BYTE ����]                                 
// ******************************************************************************
int CFastechPublic_IO::Set_IO_DefineByte(int n_MasterID, int n_PortNo, int n_SlaveNo, int n_Type, BYTE bt_Data) 
{
	CString sMsg;
	int     nRet=0;

	nRet = HSSI_WriteDirectionByte(n_MasterID, n_PortNo, n_SlaveNo, n_Type, bt_Data);  // SLAVE I/O ����� ����� ���� �Լ� [BYTE ����]
// jtkim 20130625
/*
	if (nRet == HSSI_STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
*/
#ifdef _WIN64
	if (nRet == HSSI_STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
#else
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
#endif
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ����� ����� ���� �Լ� [BIT ����]                                  
// ******************************************************************************
int CFastechPublic_IO::Set_IO_DefineBit(int n_IONo, BOOL b_Data) 
{
	CString sMsg;
	int     nRet=0;
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0, nBitNo=0;

	nMasterID = Search_IO_BoardInformation(0, n_IONo);  // HSSI ���� ������ Ȯ�� �� �˻� �Լ�
	nPortNo   = Search_IO_BoardInformation(1, n_IONo);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo);
	nBitNo    = Search_IO_BoardInformation(3, n_IONo);

	nRet = HSSI_WriteDirectionBit(nMasterID, nPortNo, nSlaveNo, nBitNo, b_Data);  // SLAVE I/O ����� ����� ���� �Լ� [BIT ����]
// jtkim 20130625
/*
	if (nRet == HSSI_STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
*/
#ifdef _WIN64
	if (nRet == HSSI_STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
#else
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
#endif
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ����� ����� ���� ���� ���� [WORD ����]                            
// ******************************************************************************
int CFastechPublic_IO::Get_IO_DefineWord(int n_MasterID, int n_PortNo, int n_SlaveNo, WORD* pw_Data) 
{
	CString sMsg;
	int     nRet=0;
	WORD    wData=0;

	nRet = HSSI_ReadDirectionWord(n_MasterID, n_PortNo, n_SlaveNo, &wData);  // SLAVE I/O ����� ����� ���� ���� ���� [WORD ����]
// jtkim 20130625
/*
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		*pw_Data = wData;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
*/
#ifdef _WIN64
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		*pw_Data = wData;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
#else
	if (nRet == STATUS_SUCCESS)
	{
		*pw_Data = wData;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
#endif
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ����� ����� ���� ���� ���� [BYTE ����]                            
// ******************************************************************************
int CFastechPublic_IO::Get_IO_DefineByte(int n_MasterID, INT n_PortNo, int n_SlaveNo, int n_Type, BYTE* pb_Data) 
{
	CString sMsg;
	int     nRet=0;
	BYTE    bData=0;

	nRet = HSSI_ReadDirectionByte(n_MasterID, n_PortNo, n_SlaveNo, n_Type, &bData);  // SLAVE I/O ����� ����� ���� ���� ���� [BYTE ����]
// jtkim 20130625
/*
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		*pb_Data = bData;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
*/
#ifdef _WIN64
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		*pb_Data = bData;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
#else
	if (nRet == STATUS_SUCCESS)
	{
		*pb_Data = bData;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
#endif
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ����� ����� ���� ���� ���� [BIT ����]                             
// ******************************************************************************
int CFastechPublic_IO::Get_IO_DefineBit(int n_IONo, BOOL* pb_Data) 
{
	CString sMsg;
	int     nRet=0;
	BOOL    bData;
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0, nBitNo=0;

	nMasterID = Search_IO_BoardInformation(0, n_IONo);  // HSSI ���� ������ Ȯ�� �� �˻� �Լ�
	nPortNo   = Search_IO_BoardInformation(1, n_IONo);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo);
	nBitNo    = Search_IO_BoardInformation(3, n_IONo);

	nRet = HSSI_ReadDirectionBit(nMasterID, nPortNo, nSlaveNo, nBitNo, &bData);  // SLAVE I/O ����� ����� ���� ���� ���� [BIT ����]
// jtkim 20130625
/*
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		*pb_Data = bData;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
*/
#ifdef _WIN64
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		*pb_Data = bData;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
#else
	if (nRet == STATUS_SUCCESS)
	{
		*pb_Data = bData;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
#endif
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// PC�� ����� ������ ���� ���� �˻� �Լ�                                        
// ******************************************************************************
int CFastechPublic_IO::Search_IO_Master(INT* pn_MasterNo) 
{
	CString sMsg;
	int     nRet=0;
	INT     nMasterNo=0;

	nRet = HSSI_GetAttachedMasterBoard(&nMasterNo);  // ���� PC�� ����Ǿ� �ִ� ������ ���� ���� ���� �Լ�
// jtkim 20130625
/*
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		*pn_MasterNo = nMasterNo;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
*/
#ifdef _WIN64
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		*pn_MasterNo = nMasterNo;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
#else
	if (nRet == STATUS_SUCCESS)
	{
		*pn_MasterNo = nMasterNo;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
#endif
	return nRet;
}
// ******************************************************************************


// ******************************************************************************
// ������ ���忡 ����� PORT ���� �˻� �Լ�                                      
// ******************************************************************************
int CFastechPublic_IO::Search_IO_Port(int n_MasterID, INT* pn_PortNo) 
{
	CString sMsg;
	int     nRet=0;
	INT     nPortNo=0;

	nRet = HSSI_GetAttachedPort(n_MasterID, &nPortNo);  // ������ ������ ���� �����ϴ� PORT ���� ���� �Լ�
// jtkim 20130625
/*
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		*pn_PortNo = nPortNo;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
*/
#ifdef _WIN64
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		*pn_PortNo = nPortNo;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
#else
	if (nRet == STATUS_SUCCESS)
	{
		*pn_PortNo = nPortNo;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
#endif
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// PORT�� ����� SLAVE �˻� �Լ�                                                 
// ******************************************************************************
int CFastechPublic_IO::Search_IO_Slave(int n_MasterID, int n_PortNo, INT* pn_SlaveNo) 
{
	CString sMsg;
	int     nRet=0;	
	INT     nSlaveNo=0;

	nRet = HSSI_GetAttachedSlaveModule(n_MasterID, n_PortNo, &nSlaveNo);  // PORT�� ����� SLAVE I/O ��� �˻� �Լ�
// jtkim 20130625
/*
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		*pn_SlaveNo = nSlaveNo;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
*/
#ifdef _WIN64
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		*pn_SlaveNo = nSlaveNo;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
#else
	if (nRet == STATUS_SUCCESS)
	{
		*pn_SlaveNo = nSlaveNo;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
#endif
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O MODULE�� ENABLE ���·� ������ ���忡 ����Ǿ� �ִ��� �˻� �Լ�      
// ******************************************************************************
int CFastechPublic_IO::Check_IO_SlaveStatus(int n_MasterID, int n_PortNo, int n_SlaveNo) 
{
	CString sMsg;
	int     nRet=0;

	nRet = HSSI_IsExistSlaveModule(n_MasterID, n_PortNo, n_SlaveNo);  // SLAVE I/O MODULE�� ENABLE ���·� ������ ���忡 ����Ǿ� �ִ��� �˻� �Լ�
// jtkim 20130625
/*
	if (nRet == HSSI_STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
*/
#ifdef _WIN64
	if (nRet == HSSI_STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
#else
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
#endif
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// PORT�� ����� SLAVE [ENABLE/DISABLE] ���� �˻� �Լ�                           
// ******************************************************************************
BOOL CFastechPublic_IO::Check_IO_Slave(int n_MasterID, int n_PortNo, int n_SlaveID) 
{
	int nSlaveInfo;

	FAS_STATUS  fasResult;

	// **************************************************************************
	// nSlaveInfo : �������κ��� ���� ��Ʈ �������� 8���� BIT���� �� SLAVE����   
	//              ���� ���°� ����� BCD�� ����ִ�                            
	// -> BIT 1   : SLAVE I/O ����� ����Ǿ� ENABLE�� ����                      
	// -> BIT 0   : SLAVE I/O ����� ������� �ʾ� DISABLE�� ����                
	// **************************************************************************
	fasResult = HSSI_GetAttachedSlaveModule(n_MasterID, n_PortNo, &nSlaveInfo);  // PORT�� ����� SLAVE I/O ��� �˻� �Լ�
	if (((nSlaveInfo >> n_SlaveID) & 0x1) != 1)
	{
		return BD_ERROR;
	}
	// **************************************************************************

	return BD_GOOD;
}
// ******************************************************************************

// ******************************************************************************
// PORT�� SLAVE I/O ������ ��� �ӵ� ���� �Լ�                                 
// -> PORT_SPEED_05 : 0.5 Mbps                                                   
// -> PORT_SPEED_10 : 1.0 Mbps                                                   
// -> PORT_SPEED_20 : 2.0 Mbps                                                   
// ******************************************************************************
int CFastechPublic_IO::Set_IO_HSSISpeed(int n_MasterID, int n_PortNo, int n_Type) 
{
	CString sMsg;
	int     nRet=0, nFlag = BD_ERROR;

	nRet = HSSI_SetCommSpeed(n_MasterID, n_PortNo, n_Type);  // �ش� PORT�� I/O ��� ������ ��� �ӵ� ���� �Լ�
// jtkim 20130625
/*
	if (nRet == HSSI_STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
*/
#ifdef _WIN64
	if (nRet == HSSI_STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
#else
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
#endif
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// PORT�� SLAVE I/O ������ ��� �ӵ� ���� �Լ�                                 
// -> PORT_SPEED_05 : 0.5 Mbps                                                   
// -> PORT_SPEED_10 : 1.0 Mbps                                                   
// -> PORT_SPEED_20 : 2.0 Mbps                                                   
// ******************************************************************************
int CFastechPublic_IO::Get_IO_HSSISpeed(int n_MasterID, int n_PortNo, INT* pn_Type) 
{
	CString sMsg;
	int     nRet=0;
	INT     nType;

	nRet = HSSI_GetCommSpeed(n_MasterID, n_PortNo, &nType);  // �ش� PORT�� I/O ��� ������ ��� �ӵ� ���� �Լ�
// jtkim 20130625
/*
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		*pn_Type = nType;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
*/
#ifdef _WIN64
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		*pn_Type = nType;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
#else
	if (nRet == STATUS_SUCCESS)
	{
		*pn_Type = nType;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
#endif
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// ���� ���� ���� �˻� �Լ�                                                      
// ******************************************************************************
int CFastechPublic_IO::Check_IO_BoardExist(int n_MaxSlot, int n_MaxPort, int n_MaxSlave) 
{
	int nMasterID, nPortNo, nSlaveID;
	int nRet=0;

	while(1)
	{
		for(nMasterID=0; nMasterID<n_MaxSlot; nMasterID++)
		{
			nRet = Check_IO_MasterStatus(nMasterID);  // ������ ���� ���� ���� �˻� �Լ�
			if (nRet != BD_GOOD)
			{
				continue ;
			}

			for(nPortNo=0; nPortNo<n_MaxPort; nPortNo)
			{
				nRet = Check_IO_PortStatus(nMasterID, nPortNo);  // PORT ���� ���� �� ���� ���� �˻� �Լ�
				if (nRet != BD_GOOD)
				{
					continue ;
				}

				for(nSlaveID=0; nSlaveID<n_MaxSlave; nSlaveID++)
				{
					nRet = Check_IO_SlaveStatus(nMasterID, nPortNo, nSlaveID);  // SLAVE I/O MODULE�� ENABLE ���·� ������ ���忡 ����Ǿ� �ִ��� �˻� �Լ�
					if (nRet != BD_GOOD)
					{
						continue ;
					}
				}
			}
		}
	}

	return 0;
}
// ******************************************************************************

// ******************************************************************************
// PORT�� ����� ��� SLAVE I/O ��� �ڵ� ENABLE �Լ�                            
// ******************************************************************************
int CFastechPublic_IO::Set_IO_PortAutoEnable(int n_MasterID, int n_PortNo) 
{
	CString sMsg;
	int     nRet=0;

	nRet = HSSI_PortAutoEnable(n_MasterID, n_PortNo);  // �ش� PORT�� ����� ��� SLAVE I/O ��� �ڵ� ENABLE �Լ�
// jtkim 20130625
/*
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		nRet = BD_ERROR;
	}
*/
#ifdef _WIN64
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		nRet = BD_ERROR;
	}
#else
	if (nRet == STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		nRet = BD_ERROR;
	}
#endif
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ��� ���� ENABLE/DISABLE ���� �Լ�                                  
// ******************************************************************************
int CFastechPublic_IO::Set_IO_SlaveEnable(int n_MasterID, int n_PortNo, int n_SlaveNo, BOOL b_Enable) 
{
	CString sMsg;
	int     nRet=0;

	nRet = HSSI_EnableSlaveModule(n_MasterID, n_PortNo, n_SlaveNo, b_Enable);  // �ش� SLAVE I/O ��� ���� ENABLE/DISABLE ���� �Լ�
// jtkim 20130625
/*
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		nRet = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
*/
#ifdef _WIN64
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		nRet = BD_ERROR;
	}
#else
	if (nRet == STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		nRet = BD_ERROR;
	}
#endif
	return nRet;
}
// ******************************************************************************


// ******************************************************************************

// ******************************************************************************
// Ư�� ��ġ BIT�� �˻� �Լ�                                                     
// ******************************************************************************
BOOL CFastechPublic_IO::Check_Bit(long n_Value, int n_BitPos) 
{
	unsigned long nMask = 0x0001;  // ����ũ BIT ���� 
	nMask <<= n_BitPos;  // ������ BIT ��ġ�� �̵� [SHIFT] 

	return (n_Value & nMask);  // �ش� BIT ���� �� ���� 
}
// ******************************************************************************

// ******************************************************************************
// Ư�� ��ġ BIT ���� �Լ�                                                       
// ******************************************************************************
void CFastechPublic_IO::Set_Bit(long &n_Value, int n_Bit, BOOL b_On) 
{
	unsigned long nMask = 0x0001;  // ����ũ BIT ���� 
	nMask <<= n_Bit;  // ������ BIT ��ġ�� �̵� [SHIFT] 

	if ( b_On )
	{
		n_Value |= nMask;
	}
	else
	{
		n_Value &= ~nMask;
	}
}
// ******************************************************************************

// ******************************************************************************
// HSSI ���� ������ Ȯ�� �� �˻� �Լ�                                            
// -> �Է� �Ķ���ʹ� �ϵ��� �������� �ԷµǾ�� �Ѵ�                            
//  : I/O MAP ���� �����Ѵ�                                                      
//  : I/O MAP ������ 4�ڸ��� �����ȴ�                                            
// ******************************************************************************
int CFastechPublic_IO::Search_IO_BoardInformation(int n_Type, int n_Data) 
{
	int nFlag     = BD_ERROR;
	int nBardData = 0;

	// **************************************************************************
	// I/O PORT�� ���� �ִ� ������� 7127�� �����Ѵ�                             
	// -> �ִ� ��� : ������ ���� 2���� ���� [I/O PORT�� 2048]                   
	// **************************************************************************
	if (n_Data > HSSI_MAX_IO) //HSSI_MAX_IO = 7715
		return BD_ERROR;
	// **************************************************************************

	switch(n_Type)
	{
	case 0 :  // ������ ���� 
		// **********************************************************************
		// 0[0, 1, 2, 3]    1[4, 5, 6, 7]    2[8, 9, 10, 11]    3[12, 13, 14, 15]
		// -> ������ ���� 1�忡 �ִ� 4 PORT ����ȴ�                             
		// **********************************************************************
		nBardData = n_Data / 4000;
		// **********************************************************************
		break;

	case 1 :  // PORT 
		// **********************************************************************
		// I/O MAP ��ȣ�� 1000���� ������ ���� PORT ��ȣ�� �ǵ��� ���ǵǾ� �ִ�  
		// -> PORT ���� ������ �ִ� [0 - 15] �̴�                                
		// -> ������ ����� �ִ� 2������ �����Ѵ�                                
		// **********************************************************************
		nBardData = n_Data / 1000;

		if (nBardData>3)
			nBardData -= 4;
		// **********************************************************************
		break;

	case 2 :  // SLAVE 
		// **********************************************************************
		// �� PORT�� ����� SLAVE ��ȣ�� �����Ѵ�                                
		// -> PORT �� �ִ� [0 - 7] ����ȴ�                                      
		// **********************************************************************
		nBardData = n_Data     % 1000;
		nBardData = nBardData / 100;
		// **********************************************************************
		break;

	case 3 :  // I/O BIT 
		// **********************************************************************
		// �ش� I/O BIT ��ġ �����Ѵ�                                            
		// -> I/O BIT�� �ִ� [0 - 15] ������ �� �ִ�                             
		//  : IN/OUT ���´� ���Ƿ� ���� �����Ѵ�                                 
		// **********************************************************************
		nBardData = n_Data     % 1000;
		nBardData = nBardData % 100;
		// **********************************************************************
		break;

	default :
		return BD_ERROR;
		break;
	}

	return nBardData;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ����� �Է��� LATCH ���� ���� �Լ� [WORD ����]                    
// ******************************************************************************
int CFastechPublic_IO::Set_IO_LatchWord(int n_MasterID, int n_PortNo, int n_SlaveNo, WORD w_Data) 
{
	CString sMsg;
	int     nRet=0;

	nRet = HSSI_SetLatchWord(n_MasterID, n_PortNo, n_SlaveNo, w_Data);  // SLAVE I/O ����� �Է��� LATCH ���� ���� �Լ� [WORD ����]
// jtkim 20130625
/*
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		nRet = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
*/
#ifdef _WIN64
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		nRet = BD_ERROR;
	}
#else
	if (nRet == STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		nRet = BD_ERROR;
	}
#endif
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ����� �Է��� LATCH ���� ���� �Լ� [BYTE ����]                    
// ******************************************************************************
int CFastechPublic_IO::Set_IO_LatchByte(int n_MasterID, int n_PortNo, int n_SlaveNo, int n_Type, BYTE bt_Data) 
{
	CString sMsg;
	int     nRet=0;

	nRet = HSSI_SetLatchByte(n_MasterID, n_PortNo, n_SlaveNo, n_Type, bt_Data);  // SLAVE I/O ����� �Է��� LATCH ���� ���� �Լ� [BYTE ����]
// jtkim 20130625
/*
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		nRet = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
*/
#ifdef _WIN64
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		nRet = BD_ERROR;
	}
#else
	if (nRet == STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		nRet = BD_ERROR;
	}
#endif
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ����� �Է��� LATCH ���� ���� �Լ� [BIT ����]                     
// ******************************************************************************
int CFastechPublic_IO::Set_IO_LatchBit(int n_IONo, BOOL b_Data) 
{
	CString sMsg;
	int     nRet=0;	
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0, nBitNo=0;

	nMasterID = Search_IO_BoardInformation(0, n_IONo);  // HSSI ���� ������ Ȯ�� �� �˻� �Լ�
	nPortNo   = Search_IO_BoardInformation(1, n_IONo);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo);
	nBitNo    = Search_IO_BoardInformation(3, n_IONo);

	nRet = HSSI_SetLatchBit(nMasterID, nPortNo, nSlaveNo, nBitNo, b_Data);  // SLAVE I/O ����� �Է��� LATCH ���� ���� �Լ� [BIT ����]
// jtkim 20130625
/*
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		nRet = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
*/
#ifdef _WIN64
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		nRet = BD_ERROR;
	}
#else
	if (nRet == STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		nRet = BD_ERROR;
	}
#endif
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ����� LATCH ���� ���� �Լ� [WORD ����]                             
// ******************************************************************************
WORD CFastechPublic_IO::Get_IO_LatchWord(int n_MasterID, int n_PortNo, int n_SlaveNo) 
{
	CString sMsg;
	int     nRet=0;
	
	WORD    wData=0;

	nRet = HSSI_ReadLatchWord(n_MasterID, n_PortNo, n_SlaveNo, &wData);  // SLAVE I/O ����� LATCH ���� ���� �Լ� [WORD ����]
// jtkim 20130625
/*
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		//wData�� ������ ���� //wData = BD_GOOD;
	}
	else
	{
		wData = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
*/
#ifdef _WIN64
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		wData = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		wData = BD_ERROR;
	}
#else
	if (nRet == STATUS_SUCCESS)
	{
		wData = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		wData = BD_ERROR;
	}
#endif
	return wData;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ����� LATCH ���� ���� �Լ� [BYTE ����]                             
// ******************************************************************************
BYTE CFastechPublic_IO::Get_IO_LatchByte(int n_MasterID, INT n_PortNo, int n_SlaveNo, int n_Type) 
{
	CString sMsg;
	int     nRet=0;	
	BYTE    bData=0;

	nRet = HSSI_ReadLatchByte(n_MasterID, n_PortNo, n_SlaveNo, n_Type, &bData);  // SLAVE I/O ����� LATCH ���� ���� �Լ� [BYTE ����]
// jtkim 20130625
/*
	if (nRet == HSSI_STATUS_SUCCESS)
	{	 
	}
	else
	{
		bData = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
*/
#ifdef _WIN64
	if (nRet == HSSI_STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		bData = BD_ERROR;
	}
#else
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		bData = BD_ERROR;
	}
#endif
	return bData;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ����� LATCH ���� ���� �Լ� [BIT ����]                              
// ******************************************************************************
BOOL CFastechPublic_IO::Get_IO_LatchBit(int n_IONo) 
{
	CString sMsg;
	int     nRet=0;
	BOOL    bData=0;
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0, nBitNo=0;

	nMasterID = Search_IO_BoardInformation(0, n_IONo);  // HSSI ���� ������ Ȯ�� �� �˻� �Լ�
	nPortNo   = Search_IO_BoardInformation(1, n_IONo);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo);
	nBitNo    = Search_IO_BoardInformation(3, n_IONo);

	nRet = HSSI_ReadLatchBit(nMasterID, nPortNo, nSlaveNo, nBitNo, &bData);  // SLAVE I/O ����� LATCH ���� ���� �Լ� [BIT ����]
// jtkim 20130625
/*
	if (nRet == HSSI_STATUS_SUCCESS)
	{
	}
	else
	{
		bData = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
*/
#ifdef _WIN64
	if (nRet == HSSI_STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		bData = BD_ERROR;
	}
#else
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		bData = BD_ERROR;
	}
#endif
	return bData;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ����� LATCH ���� ���� �Լ� [WORD ����]                             
// ******************************************************************************
int CFastechPublic_IO::Reset_IO_LatchWord(int n_MasterID, int n_PortNo, int n_SlaveNo, WORD w_Data) 
{
	CString sMsg;
	int     nRet=0;

	nRet = HSSI_ResetLatchWord(n_MasterID, n_PortNo, n_SlaveNo, w_Data);  // SLAVE I/O ����� LATCH ���� ���� �Լ� [WORD ����]
// jtkim 20130625
/*
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		nRet = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
*/
#ifdef _WIN64
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		nRet = BD_ERROR;
	}
#else
	if (nRet == STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		nRet = BD_ERROR;
	}
#endif
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ����� LATCH ���� ���� �Լ� [BYTE ����]                             
// ******************************************************************************
int CFastechPublic_IO::Reset_IO_LatchByte(int n_MasterID, int n_PortNo, int n_SlaveNo, int n_Type, BYTE bt_Data) 
{
	CString sMsg;
	int     nRet=0;

	nRet = HSSI_ResetLatchByte(n_MasterID, n_PortNo, n_SlaveNo, n_Type, bt_Data);  // SLAVE I/O ����� LATCH ���� ���� �Լ� [BYTE ����]
// jtkim 20130625
/*
	if (nRet == HSSI_STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
*/
#ifdef _WIN64
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		nRet = BD_ERROR;
	}
#else
	if (nRet == STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		nRet = BD_ERROR;
	}
#endif
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ����� LATCH ���� ���� �Լ� [BIT ����]                              
// ******************************************************************************
int CFastechPublic_IO::Reset_IO_LatchBit(int n_IONo, BOOL b_Data) 
{
	CString sMsg;
	int     nRet=0;
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0, nBitNo=0;

	nMasterID = Search_IO_BoardInformation(0, n_IONo);  // HSSI ���� ������ Ȯ�� �� �˻� �Լ�
	nPortNo   = Search_IO_BoardInformation(1, n_IONo);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo);
	nBitNo    = Search_IO_BoardInformation(3, n_IONo);

	nRet = HSSI_SetLatchBit(nMasterID, nPortNo, nSlaveNo, nBitNo, b_Data);  // SLAVE I/O ����� LATCH ���� ���� �Լ� [BIT ����]
// jtkim 20310625
/*
	if (nRet == HSSI_STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
*/
#ifdef _WIN64
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		nRet = BD_ERROR;
	}
#else
	if (nRet == STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		nRet = BD_ERROR;
	}
#endif
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// ������ ���� ���� ���� �˻� �Լ�                                               
// ******************************************************************************
int CFastechPublic_IO::Check_IO_MasterStatus(INT n_MasterID) 
{
	CString sMsg;
	int     nRet=0;

	nRet = HSSI_IsExistMasterBoard(n_MasterID);  // ������ ���� ���� ���� �˻� �Լ�
// jtkim 20130625
/*
	if (nRet == HSSI_STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
*/
#ifdef _WIN64
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		nRet = BD_ERROR;
	}
#else
	if (nRet == STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		nRet = BD_ERROR;
	}
#endif
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// PORT ���� ���� �� ���� ���� �˻� �Լ�                                         
// ******************************************************************************
int CFastechPublic_IO::Check_IO_PortStatus(int n_MasterID, int n_PortNo) 
{
	CString sMsg;
	int     nRet=0;

	nRet = HSSI_IsExistPort(n_MasterID, n_PortNo);  // PORT ���� ���� �� ���� ���� �˻� �Լ�
// jtkim 20130625
/*
	if (nRet == HSSI_STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
*/
#ifdef _WIN64
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		nRet = BD_ERROR;
	}
#else
	if (nRet == STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		nRet = BD_ERROR;
	}
#endif
	return nRet;
}
// ******************************************************************************


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//I/O�� ���� �����Ѵ�  james Lee 2008.9.5
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// ******************************************************************************
// SLAVE I/O ��� ��� ��ȣ �߻� �Լ� [WORD ����]                                
// ******************************************************************************
int CFastechPublic_IO::Set_Out_Word(int n_IONo, WORD w_Data) 
{
	CString sMsg;
	int     nRet=0;	
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0;

	nMasterID = Search_IO_BoardInformation(0, n_IONo * 100);  // HSSI ���� ������ Ȯ�� �� �˻� �Լ�
	nPortNo   = Search_IO_BoardInformation(1, n_IONo * 100);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo * 100);

	nRet = HSSI_WriteOutputWord(nMasterID, nPortNo, nSlaveNo, w_Data);  // SLAVE I/O ��� ��� ��ȣ �߻� �Լ� [WORD ����]
// jtkim 20130625
/*
	if (nRet == HSSI_STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
*/
#ifdef _WIN64
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		nRet = BD_ERROR;
	}
#else
	if (nRet == STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		nRet = BD_ERROR;
	}
#endif
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ��� ��� ��ȣ �߻� �Լ� [BYTE ����]                                
// ******************************************************************************
int CFastechPublic_IO::Set_Out_Byte(int n_IONo, int n_Type, BYTE bt_Data) 
{
	CString sMsg;
	int     nRet=0;	
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0;

	nMasterID = Search_IO_BoardInformation(0, n_IONo * 100);  // HSSI ���� ������ Ȯ�� �� �˻� �Լ�
	nPortNo   = Search_IO_BoardInformation(1, n_IONo * 100);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo * 100);

	nRet = HSSI_WriteOutputByte(nMasterID, nPortNo, nSlaveNo, n_Type, bt_Data);  // SLAVE I/O ��� ��� ��ȣ �߻� �Լ� [BYTE ����]
// jtkim 20130625
/*
	if (nRet == HSSI_STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
*/
#ifdef _WIN64
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		nRet = BD_ERROR;
	}
#else
	if (nRet == STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		nRet = BD_ERROR;
	}
#endif
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ��� ��� ��ȣ �߻� �Լ� [BIT ����]                                 
// ******************************************************************************
int CFastechPublic_IO::Set_Out_Bit(int n_IONo, BOOL b_OnOff) 
{
	
	CString sMsg;
	int     nRet=0;
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0, nBitNo=0;

	nMasterID = Search_IO_BoardInformation(0, n_IONo);  // HSSI ���� ������ Ȯ�� �� �˻� �Լ�
	nPortNo   = Search_IO_BoardInformation(1, n_IONo);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo);
	nBitNo    = Search_IO_BoardInformation(3, n_IONo);

	nRet = HSSI_WriteOutputBit(nMasterID, nPortNo, nSlaveNo, nBitNo, b_OnOff);  // SLAVE I/O ��� ��� ��ȣ �߻� �Լ� [BIT ����]
// jtkim 20130625
/*
	if (nRet == HSSI_STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
*/
#ifdef _WIN64
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		nRet = BD_ERROR;
	}
#else
	if (nRet == STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		nRet = BD_ERROR;
	}
#endif
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ��� �Է� ��ȣ READ �Լ� [WORD ����]                                
// ******************************************************************************
WORD CFastechPublic_IO::Get_In_Word(int n_IONo) 
{
	CString sMsg;
	int     nRet=0;
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0;
	WORD    wData=0;

	nMasterID = Search_IO_BoardInformation(0, n_IONo * 100);  // HSSI ���� ������ Ȯ�� �� �˻� �Լ�
	nPortNo   = Search_IO_BoardInformation(1, n_IONo * 100);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo * 100);

	nRet = HSSI_ReadInputWord(nMasterID, nPortNo, nSlaveNo, &wData);  // SLAVE I/O ��� �Է� ��ȣ READ �Լ� [WORD ����]
// jtkim 20130625
/*
	if (nRet == HSSI_STATUS_SUCCESS)
	{
	}
	else
	{
		wData = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
*/
#ifdef _WIN64
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		wData = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		wData = BD_ERROR;
	}
#else
	if (nRet == STATUS_SUCCESS)
	{
		wData = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		wData = BD_ERROR;
	}
#endif
	return wData;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ��� ��� ��ȣ READ �Լ� [WORD ����]                                
// ******************************************************************************
WORD CFastechPublic_IO::Get_Out_Word(int n_IONo)
{
	CString sMsg;
	int     nRet=0;
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0;
	WORD    wData=0;
	
	nMasterID = Search_IO_BoardInformation(0, n_IONo * 100);  // HSSI ���� ������ Ȯ�� �� �˻� �Լ�
	nPortNo   = Search_IO_BoardInformation(1, n_IONo * 100);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo * 100);
	
	nRet = HSSI_ReadOutputWord(nMasterID, nPortNo, nSlaveNo, &wData);  // SLAVE I/O ��� �Է� ��ȣ READ �Լ� [WORD ����]
// jtkim 20130625
/*
	if (nRet == HSSI_STATUS_SUCCESS)
	{
	}
	else
	{
		wData = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
*/
#ifdef _WIN64
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		wData = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		wData = BD_ERROR;
	}
#else
	if (nRet == STATUS_SUCCESS)
	{
		wData = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		wData = BD_ERROR;
	}
#endif
	return wData;	
}

// ******************************************************************************
// SLAVE I/O ��� �Է� ��ȣ READ �Լ� [BYTE ����]                                
// ******************************************************************************
BYTE CFastechPublic_IO::Get_In_Byte(int n_IONo, int n_Type) 
{
	CString sMsg;
	int     nRet=0;
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0;
	BYTE    btData=0;

	nMasterID = Search_IO_BoardInformation(0, n_IONo * 100);  // HSSI ���� ������ Ȯ�� �� �˻� �Լ�
	nPortNo   = Search_IO_BoardInformation(1, n_IONo * 100);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo * 100);

	nRet = HSSI_ReadInputByte(nMasterID, nPortNo, nSlaveNo, n_Type, &btData);  // SLAVE I/O ��� �Է� ��ȣ READ �Լ� [BYTE ����]
// jtkim 20130625
/*
	if (nRet == HSSI_STATUS_SUCCESS)
	{
	}
	else
	{
		btData = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
*/
#ifdef _WIN64
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		btData = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		btData = BD_ERROR;
	}
#else
	if (nRet == STATUS_SUCCESS)
	{
		btData = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		btData = BD_ERROR;
	}
#endif
	return btData;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ��� ��� ��ȣ READ �Լ� [BYTE ����]                                
// ******************************************************************************
BYTE CFastechPublic_IO::Get_Out_Byte(int n_IONo, int n_Type)
{
	CString sMsg;
	int     nRet=0;
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0;
	BYTE    btData=0;
	
	nMasterID = Search_IO_BoardInformation(0, n_IONo * 100);  // HSSI ���� ������ Ȯ�� �� �˻� �Լ�
	nPortNo   = Search_IO_BoardInformation(1, n_IONo * 100);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo * 100);
	
	nRet = HSSI_ReadOutputByte(nMasterID, nPortNo, nSlaveNo, n_Type, &btData);  // SLAVE I/O ��� �Է� ��ȣ READ �Լ� [BYTE ����]
// jtkim 20130625
/*
	if (nRet == HSSI_STATUS_SUCCESS)
	{
	}
	else
	{
		btData = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
*/
#ifdef _WIN64
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		btData = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		btData = BD_ERROR;
	}
#else
	if (nRet == STATUS_SUCCESS)
	{
		btData = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		btData = BD_ERROR;
	}
#endif
	return btData;
}

// ******************************************************************************
// SLAVE I/O ��� �Է� ��ȣ READ �Լ� [BIT ����]                                 
// ******************************************************************************
BOOL CFastechPublic_IO::Get_In_Bit(int n_IONo) 
{
	CString sMsg;
	int     nRet=0;
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0, nBitNo = 0;
	BOOL    bData;

	nMasterID = Search_IO_BoardInformation(0, n_IONo);  // HSSI ���� ������ Ȯ�� �� �˻� �Լ�
	nPortNo   = Search_IO_BoardInformation(1, n_IONo);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo);
	nBitNo    = Search_IO_BoardInformation(3, n_IONo);

	nRet = HSSI_ReadInputBit(nMasterID, nPortNo, nSlaveNo, nBitNo, &bData);  // SLAVE I/O ��� �Է� ��ȣ READ �Լ� [BIT ����]
// jtkim 20130625
/*
	if (nRet == HSSI_STATUS_SUCCESS)
	{
	}
	else
	{
		bData = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
*/
#ifdef _WIN64
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		bData = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		bData = BD_ERROR;
	}
#else
	if (nRet == STATUS_SUCCESS)
	{
		bData = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		bData = BD_ERROR;
	}
#endif
	return bData;
}




// ******************************************************************************
// SLAVE I/O ��� ��� ��ȣ READ �Լ� [BIT ����]                                 
// ******************************************************************************
BOOL CFastechPublic_IO::Get_Out_Bit(int n_IONo)
{
	CString sMsg;
	int     nRet=0;	
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0,nBitNo=0;
	BOOL    bData;
	
	nMasterID = Search_IO_BoardInformation(0, n_IONo);  // HSSI ���� ������ Ȯ�� �� �˻� �Լ�
	nPortNo   = Search_IO_BoardInformation(1, n_IONo);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo);
	nBitNo    = Search_IO_BoardInformation(3, n_IONo);
	
	nRet = HSSI_ReadOutputBit(nMasterID, nPortNo, nSlaveNo, nBitNo, &bData);  // SLAVE I/O ��� �Է� ��ȣ READ �Լ� [BIT ����]
// jtkim 20130625
/*
	if (nRet == HSSI_STATUS_SUCCESS)
	{
	}
	else
	{
		bData = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
*/
#ifdef _WIN64
	if (nRet == HSSI_STATUS_SUCCESS)
	{
		bData = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		bData = BD_ERROR;
	}
#else
	if (nRet == STATUS_SUCCESS)
	{
		bData = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		bData = BD_ERROR;
	}
#endif
	return bData;
}
// ******************************************************************************




// ******************************************************************************
// HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�                                        
// -> nErrorNum                : �߻��� ���� ��ȣ                                
// -> STATUS_SUCCESS = 0x0     : ���� ����                                       
// -> STATUS_ERROR_NO_DEVICE   : ���� ���� ���� ������                           
// -> STATUS_ERROR_READ        : READ ���� ����                                  
// -> STATUS_ERROR_WRITE       : WRITE ���� ����                                 
// -> STATUS_ERROR_INITIALIZE  : �ʱ�ȭ ���� [RESET ����]                        
// -> STATUS_ERROR_COMMAND     : CMD ��뿡 ���� �߻�                            
// -> STATUS_ERROR_FILE        : FILE�� ���� �߻�                                
// -> STATUS_ERROR_PORT        : PORT ������                                     
// -> STATUS_ERROR_SLAVE       : SLAVE�� ON �ƴ�                                 
// -> STATUS_NOTUSE_COMMAND    : �������� �ʴ� ��ɾ�                            
// -> STATUS_INVALID_BOARDID   : �ش� ���� ������                                
// -> STATUS_INVALID_PORTNO    : �ش� PORT ������                                
// -> STATUS_INVALID_SLAVEID   : �ش� SLAVE ������                               
// -> STATUS_INVALID_PARAMETER : �Ķ���� �Է� ����                              
// -> STATUS_INVALID_DLL       : �߸��� DLL ���                                 
// ******************************************************************************
char* CFastechPublic_IO::ErrorOccurInfo(int nErrorNum) 
{
	CString sErrorMsg;  // ���� �޽��� �ӽ� ���� ���� 

	static char cMsg[60]={0,};
// jtkim 20130625
/*
	if (nErrorNum == HSSI_STATUS_ERROR_NO_DEVICE)
	{
		sErrorMsg = _T("Board not EXIST");
	}
	else if (nErrorNum == HSSI_STATUS_ERROR_READ)
	{
		sErrorMsg = _T("READ failure");
	}
	else if (nErrorNum == HSSI_STATUS_ERROR_WRITE)
	{
		sErrorMsg = _T("WRITE failure");
	}
	else if (nErrorNum == HSSI_STATUS_ERROR_INITIALIZE)
	{
		sErrorMsg = _T("INIT Failure [RESET Inclusion]");
	}
	else if (nErrorNum == HSSI_STATUS_ERROR_COMMAND)
	{
		sErrorMsg = _T("Err Occured [CMD]");
	}
	else if (nErrorNum == HSSI_STATUS_ERROR_FILE)
	{
		sErrorMsg = _T("Err Occured [FILE]");
	}
	else if (nErrorNum == HSSI_STATUS_ERROR_PORT)
	{
		sErrorMsg = _T("PORT not EXIST");
	}
	else if (nErrorNum == HSSI_STATUS_ERROR_SLAVE)
	{
		sErrorMsg = _T("SLAVE not [ON]");
	}
	else if (nErrorNum == HSSI_STATUS_NOTUSE_COMMAND)
	{
		sErrorMsg = _T("Unknown Command");
	}
	else if (nErrorNum == HSSI_STATUS_INVALID_BOARDID)
	{
		sErrorMsg = _T("Board not EXIST");
	}
	else if (nErrorNum == HSSI_STATUS_INVALID_PORTNO)
	{
		sErrorMsg = _T("PORT not EXIST");
	}
	else if (nErrorNum == HSSI_STATUS_INVALID_SLAVEID)
	{
		sErrorMsg = _T("SLAVE not EXIST");
	}
	else if (nErrorNum == HSSI_STATUS_INVALID_PARAMETER)
	{
		sErrorMsg = _T("Input ERR [Parameter]");
	}
	else if (nErrorNum == HSSI_STATUS_INVALID_DLL)
	{
		sErrorMsg = _T("Wrong DLL Used");
	}
	else
	{
		sErrorMsg.Format(_T("Unknown HSSI Err Code = %d"), nErrorNum);
	}
*/
#ifdef _WIN64
	if (nErrorNum == HSSI_STATUS_ERROR_NO_DEVICE)
	{
		sErrorMsg = _T("Board not EXIST");
	}
	else if (nErrorNum == HSSI_STATUS_ERROR_READ)
	{
		sErrorMsg = _T("READ failure");
	}
	else if (nErrorNum == HSSI_STATUS_ERROR_WRITE)
	{
		sErrorMsg = _T("WRITE failure");
	}
	else if (nErrorNum == HSSI_STATUS_ERROR_INITIALIZE)
	{
		sErrorMsg = _T("INIT Failure [RESET Inclusion]");
	}
	else if (nErrorNum == HSSI_STATUS_ERROR_COMMAND)
	{
		sErrorMsg = _T("Err Occured [CMD]");
	}
	else if (nErrorNum == HSSI_STATUS_ERROR_FILE)
	{
		sErrorMsg = _T("Err Occured [FILE]");
	}
	else if (nErrorNum == HSSI_STATUS_ERROR_PORT)
	{
		sErrorMsg = _T("PORT not EXIST");
	}
	else if (nErrorNum == HSSI_STATUS_ERROR_SLAVE)
	{
		sErrorMsg = _T("SLAVE not [ON]");
	}
	else if (nErrorNum == HSSI_STATUS_NOTUSE_COMMAND)
	{
		sErrorMsg = _T("Unknown Command");
	}
	else if (nErrorNum == HSSI_STATUS_INVALID_BOARDID)
	{
		sErrorMsg = _T("Board not EXIST");
	}
	else if (nErrorNum == HSSI_STATUS_INVALID_PORTNO)
	{
		sErrorMsg = _T("PORT not EXIST");
	}
	else if (nErrorNum == HSSI_STATUS_INVALID_SLAVEID)
	{
		sErrorMsg = _T("SLAVE not EXIST");
	}
	else if (nErrorNum == HSSI_STATUS_INVALID_PARAMETER)
	{
		sErrorMsg = _T("Input ERR [Parameter]");
	}
	else if (nErrorNum == HSSI_STATUS_INVALID_DLL)
	{
		sErrorMsg = _T("Wrong DLL Used");
	}
	else
	{
		sErrorMsg.Format(_T("Unknown HSSI Err Code = %d"), nErrorNum);
	}
#else
	if (nErrorNum == STATUS_ERROR_NO_DEVICE)
	{
		sErrorMsg = _T("Board not EXIST");
	}
	else if (nErrorNum == STATUS_ERROR_READ)
	{
		sErrorMsg = _T("READ failure");
	}
	else if (nErrorNum == STATUS_ERROR_WRITE)
	{
		sErrorMsg = _T("WRITE failure");
	}
	else if (nErrorNum == STATUS_ERROR_INITIALIZE)
	{
		sErrorMsg = _T("INIT Failure [RESET Inclusion]");
	}
	else if (nErrorNum == STATUS_ERROR_COMMAND)
	{
		sErrorMsg = _T("Err Occured [CMD]");
	}
	else if (nErrorNum == STATUS_ERROR_FILE)
	{
		sErrorMsg = _T("Err Occured [FILE]");
	}
	else if (nErrorNum == STATUS_ERROR_PORT)
	{
		sErrorMsg = _T("PORT not EXIST");
	}
	else if (nErrorNum == STATUS_ERROR_SLAVE)
	{
		sErrorMsg = _T("SLAVE not [ON]");
	}
	else if (nErrorNum == STATUS_NOTUSE_COMMAND)
	{
		sErrorMsg = _T("Unknown Command");
	}
	else if (nErrorNum == STATUS_INVALID_BOARDID)
	{
		sErrorMsg = _T("Board not EXIST");
	}
	else if (nErrorNum == STATUS_INVALID_PORTNO)
	{
		sErrorMsg = _T("PORT not EXIST");
	}
	else if (nErrorNum == STATUS_INVALID_SLAVEID)
	{
		sErrorMsg = _T("SLAVE not EXIST");
	}
	else if (nErrorNum == STATUS_INVALID_PARAMETER)
	{
		sErrorMsg = _T("Input ERR [Parameter]");
	}
	else if (nErrorNum == STATUS_INVALID_DLL)
	{
		sErrorMsg = _T("Wrong DLL Used");
	}
	else
	{
		sErrorMsg.Format(_T("Unknown HSSI Err Code = %d"), nErrorNum);
	}
#endif
//	strncpy(cMsg, sErrorMsg, sizeof(cMsg)-1);
//	_snprintf(cMsg, sizeof(cMsg), "%S", sErrorMsg);
	//wsprintfA(cMsg, "%S", sErrorMsg);
	clsFunc.OnStringToChar(sErrorMsg, cMsg);
	cMsg[sizeof(cMsg)-1] = '\0'; //2011.0518

	return cMsg;
}
// ******************************************************************************

/*
int CFastechPublic_IO::Check_IO_Port(int n_io, int n_time, int n_flag)
{
	int Ret = CTLBD_READY;	// �Լ� ���� �÷��� ���� 
	
	if (n_time == 0)	// ��� �ð����� �ٷ� ���ϰ��� �޾ƾ� �Ѵ�.
	{
		if (Get_IO_InportBit(n_io) == TRUE)
		{
			Ret = CTLBD_YES;
		}
		else	Ret = CTLBD_NO;
	}
	else				// ���� �ð����� Ȯ���ϰ�, ���ϰ��� �޴´�.
	{
		if (IOFlag[n_io] != true)
		{
			IOTime[n_io][0]	= GetCurrentTime();
			IOFlag[n_io] = true;
		}
		else if (Get_IO_InportBit(n_io) == TRUE && IOFlag[n_io] == true)
		{
			IOTime[n_io][1] = GetCurrentTime();
			IOTime[n_io][2] = IOTime[n_io][1] - IOTime[n_io][0];
			
			if (IOTime[n_io][2] >= n_time)
			{
				IOFlag[n_io] = false;
				Ret = CTLBD_YES;
			}
		}
		else if (Get_IO_InportBit(n_io) == FALSE && IOFlag[n_io] == true)
		{
			IOTime[n_io][1] = GetCurrentTime();
			IOTime[n_io][2] = IOTime[n_io][1] - IOTime[n_io][0];
			
			if (IOTime[n_io][2] >= n_time)
			{
				IOFlag[n_io] = false;
				Ret = CTLBD_NO;
			}
		}
	}
	
	return Ret;
}

int CFastechPublic_IO::Check_IO_Port_Yield(int n_io, int n_time, int n_yield)
{
	int Ret = CTLBD_READY;	// �Լ� ���� �÷��� ���� 
	
	if (IOYFlag[n_io] != true)
	{
		IOYTime[n_io][0] = GetCurrentTime();
		IOYFlag[n_io] = true;
		// Yield Check �ʱ�ȭ. 2K6/05/18/ViboX
		IOYCount[n_io][0] = 0;					// false
		IOYCount[n_io][1] = 0;					// true
		IOYCount[n_io][2] = 0;					// total
	}
	else if (Get_IO_InportBit(n_io) == TRUE && IOYFlag[n_io] == true)
	{
		IOYCount[n_io][1]++;
	}
	else if (Get_IO_InportBit(n_io) == FALSE && IOYFlag[n_io] == true)
	{
		IOYCount[n_io][0]++;
	}
	
	if (IOYFlag[n_io] == true)
	{
		IOYTime[n_io][1] = GetCurrentTime();
		IOYTime[n_io][2] = IOYTime[n_io][1] - IOYTime[n_io][0];
		
		if (IOYTime[n_io][2] >= n_time)
		{
			
			IOYCount[n_io][2] = IOYCount[n_io][1] + IOYCount[n_io][0];
			
			if (IOYCount[n_io][2] <= 0)
			{
				return CTLBD_NO;
			}
			IOYYield[n_io] = (float)IOYCount[n_io][1] / IOYCount[n_io][2];
			
			if (IOYYield[n_io] >= n_yield)
			{
				Ret = CTLBD_YES;
			}
			else
			{
				Ret = CTLBD_NO;
			}
			IOYFlag[n_io] = false;
		}
	}
	return Ret;
}

*/

int CFastechPublic_IO::Initialize_Board()
{
	
	int i = 0,j = 0, k = 0;
	INT HSSI_speed = 0;
	int Ret = 0;
	INT pMasterNo=0;
	INT nReturn = 0;

	int nBd = 1; //master ID
	int nPort[2] = {2,0};
	//int nPort = 2;
	int nSlave[1][2];

	nSlave[0][0] = 8;
	nSlave[0][1] = 8;
	// 	nSlave[0][2] = 8;
	// 	nSlave[0][3] = 8;
	// 
	// 	nSlave[1][0] = 4;
	// 	nSlave[1][1] = 0;
	// 
	// 	nSlave[2][0] = 8;
	// 	nSlave[2][1] = 0;


	CString strMsg;
	/////////////////�ʱ⿡ �ʱ�ȭ�� //////////////////////////////////////////////////////////////////////
	Ret = FAS_IO.Set_IO_BoardOpen(&pMasterNo); //START_LOADDATA); //START_NOTHING); //START_CLEAR);
	
	Search_IO_Port(0, &nReturn);
	
#ifdef _WIN64
	if (Ret != HSSI_STATUS_SUCCESS)
	{
		m_nIoBoardInitialFlag	= BD_NO;
		
		return RET_ERROR;
	}
#else 
	if (st_work_info.nSimulationMode == 1) //Simulation ����̸� ���α׷��� �����ؾ� �ϴ� I/O ���¸� ���ϴ� ������ ������ �ش� 
	{
		m_nIoBoardInitialFlag	= BD_YES;
		
		return RET_GOOD;
	}
	else
	{
		if (Ret != STATUS_SUCCESS)
		{
			m_nIoBoardInitialFlag	= BD_NO;
			
			return RET_ERROR;
		}
	}
#endif
/*	for(i=0; i<2; i++)
	{
		Ret = FAS_IO.Set_IO_PortAutoEnable(0, i);
		
		if (Ret != BD_GOOD)
		{
			if (st_handler_info.cWndList != NULL)  // ����Ʈ �� ȭ�� ����
			{
//				st_other_info.strAbnormalMsg.Format("#0 Card, %d Port, not enable", i);
				strMsg.Format(_T("#0 Card, %d Port, not enable"), i);

				//clsFunc.OnStringToChar(strMsg, st_other_info.cAbnormalMsg);
				clsMem.OnAbNormalMessagWrite(strMsg);
				st_handler_info.cWndList->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
			}
			
			m_nIoBoardInitialFlag	= BD_NO;
			return RET_ERROR;
		}
	}*/

	for(i=0; i<nBd; i++)
	{
		for(j=0; j<nPort[i]; j++)
		{
			Ret = FAS_IO.Set_IO_PortAutoEnable(i, j);
		
			if (Ret != BD_GOOD)
			{
				if (st_handler_info.cWndList != NULL)  // ����Ʈ �� ȭ�� ����
				{
					strMsg.Format(_T("#%d Card, %d Port, not enable"), i, j);

					clsMem.OnAbNormalMessagWrite(strMsg);
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
				}
			
				m_nIoBoardInitialFlag	= BD_NO;
				return RET_ERROR;
			}
		}
	}
	

	
	for(i = 0; i < nBd; i++)
	{
		for(j = 0; j < nPort[i]; j++)
		{
			Ret = FAS_IO.Set_IO_HSSISpeed(i, j, PORT_SPEED_05);
			Ret = FAS_IO.Get_IO_HSSISpeed(i, j, &HSSI_speed);

//			for(k = 0; k < 8; k++)
			for(k = 0; k < nSlave[i][j]; k++)
			{
				Ret = FAS_IO.Set_IO_SlaveEnable( i, j, k, TRUE);
			
				if(Ret != BD_GOOD)
				{
					if (st_handler_info.cWndList != NULL)  // ����Ʈ �� ȭ�� ����
					{
	//					st_other_info.strAbnormalMsg.Format("#0 Card, Port%d, Slave #%d, not enable", i, j);
						strMsg.Format(_T("#%d Card, Port%d, Slave #%d, not enable"), i, j, k);

						//clsFunc.OnStringToChar(strMsg, st_other_info.cAbnormalMsg);
						clsMem.OnAbNormalMessagWrite(strMsg);
						st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
					}
					i = -1;
				
					if (j < 1)
					{
						j++;
					}
					else
					{
	//					st_handler.n_io_board_initial = FALSE;
						m_nIoBoardInitialFlag	= BD_NO;
						return RET_ERROR;
					}
					continue;
				}
			
				if (st_handler_info.cWndList != NULL)  // ����Ʈ �� ȭ�� ����
				{
					strMsg.Format(_T("#%d Card, Port %d, Slave #%d, enable success"), i, j, k);

	//				clsFunc.OnStringToChar(strMsg, st_other_info.cAbnormalMsg);
					clsMem.OnNormalMessageWrite(strMsg);
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ���� ���� ��� ��û
				}
				Sleep(100);
			}
		}
	}
	
/*	
	for(i = 0; i < 4; i++)
	{
		// KILEE [2012.07.02]
		//Ret = FAS_IO.Set_IO_HSSISpeed(0, i, PORT_SPEED_10);
		Ret = FAS_IO.Set_IO_HSSISpeed(0, i, PORT_SPEED_05);
		Ret = FAS_IO.Get_IO_HSSISpeed(0, i, &HSSI_speed);
	}
*/	
	// port 0
	FAS_IO.Set_IO_DefineWord(0, 0, 0, 0x00ff); //0�� Master, 0�� ��Ʈ, 0�� �����̺� , 16bit ����			-- Part I/O ���� �Ϸ�
	FAS_IO.Set_IO_DefineWord(0, 0, 1, 0x00ff); //0�� Master, 0�� ��Ʈ, 1�� �����̺� , 16bit ����			-- Part I/O ���� �Ϸ�
	FAS_IO.Set_IO_DefineWord(0, 0, 2, 0x00ff); //0�� Master, 0�� ��Ʈ, 2�� �����̺� , 16bit ����			-- Part I/O ���� �Ϸ�
	FAS_IO.Set_IO_DefineWord(0, 0, 3, 0xffff); //0�� Master, 0�� ��Ʈ, 3�� �����̺� , 16bit ����			-- Part I/O ���� �Ϸ�
	FAS_IO.Set_IO_DefineWord(0, 0, 4, 0xffff); //0�� Master, 0�� ��Ʈ, 4�� �����̺� , 16bit ����			-- Part I/O ���� �Ϸ�
	FAS_IO.Set_IO_DefineWord(0, 0, 5, 0x00ff); //0�� Master, 0�� ��Ʈ, 5�� �����̺� , 16bit ����			-- Part I/O ���� �Ϸ�
	FAS_IO.Set_IO_DefineWord(0, 0, 6, 0x00ff); //0�� Master, 0�� ��Ʈ, 6�� �����̺� , 16bit ����			-- Part I/O ���� �Ϸ�
	FAS_IO.Set_IO_DefineWord(0, 0, 7, 0x00ff); //0�� Master, 0�� ��Ʈ, 7�� �����̺� , 16bit ����			-- Part I/O ���� �Ϸ�	
// 
// 	// port 1
 	FAS_IO.Set_IO_DefineWord(0, 1, 0, 0x00ff); //0�� Master, 1�� ��Ʈ, 0�� �����̺� , 16bit ����			
	// 	FAS_IO.Set_IO_DefineWord(0, 1, 1, 0x003f); //0�� Master, 1�� ��Ʈ, 1�� �����̺� , 16bit ����
	// 	FAS_IO.Set_IO_DefineWord(0, 1, 2, 0x003f); //0�� Master, 1�� ��Ʈ, 2�� �����̺� , 16bit ����
	// 	FAS_IO.Set_IO_DefineWord(0, 1, 3, 0x0000); //0�� Master, 1�� ��Ʈ, 3�� �����̺� , 16bit ����
	// 	FAS_IO.Set_IO_DefineWord(0, 1, 4, 0x00ff); //0�� Master, 1�� ��Ʈ, 4�� �����̺� , 16bit ����
	// 	FAS_IO.Set_IO_DefineWord(0, 1, 5, 0x00ff); //0�� Master, 1�� ��Ʈ, 5�� �����̺� , 16bit ����
	// 	FAS_IO.Set_IO_DefineWord(0, 1, 6, 0x7fff); //0�� Master, 1�� ��Ʈ, 6�� �����̺� , 16bit ����
	// 	FAS_IO.Set_IO_DefineWord(0, 1, 7, 0x0000); //0�� Master, 1�� ��Ʈ, 7�� �����̺� , 16bit ����
	// 
	// 	// port 2
	// 	FAS_IO.Set_IO_DefineWord(0, 2, 0, 0x00ff); //0�� Master, 2�� ��Ʈ, 0�� �����̺� , 16bit ����			
	// 	FAS_IO.Set_IO_DefineWord(0, 2, 1, 0x07ff); //0�� Master, 2�� ��Ʈ, 1�� �����̺� , 16bit ����
	// 	FAS_IO.Set_IO_DefineWord(0, 2, 2, 0x0000); //0�� Master, 2�� ��Ʈ, 2�� �����̺� , 16bit ����
	// 	FAS_IO.Set_IO_DefineWord(0, 2, 3, 0x0000); //0�� Master, 2�� ��Ʈ, 3�� �����̺� , 16bit ����
	// 	FAS_IO.Set_IO_DefineWord(0, 2, 4, 0x00ff); //0�� Master, 2�� ��Ʈ, 4�� �����̺� , 16bit ����
	// 	FAS_IO.Set_IO_DefineWord(0, 2, 5, 0x07ff); //0�� Master, 2�� ��Ʈ, 5�� �����̺� , 16bit ����
	// 	FAS_IO.Set_IO_DefineWord(0, 2, 6, 0x0000); //0�� Master, 2�� ��Ʈ, 6�� �����̺� , 16bit ����
	// 	FAS_IO.Set_IO_DefineWord(0, 2, 7, 0x0000); //0�� Master, 2�� ��Ʈ, 7�� �����̺� , 16bit ����
	// 
	// 	// port 3
	// 	FAS_IO.Set_IO_DefineWord(0, 3, 0, 0x07ff); //0�� Master, 3�� ��Ʈ, 0�� �����̺� , 16bit ����			
	// 	FAS_IO.Set_IO_DefineWord(0, 3, 1, 0x0000); //0�� Master, 3�� ��Ʈ, 1�� �����̺� , 16bit ����
	// 	FAS_IO.Set_IO_DefineWord(0, 3, 2, 0x0000); //0�� Master, 3�� ��Ʈ, 2�� �����̺� , 16bit ����
	// 	FAS_IO.Set_IO_DefineWord(0, 3, 3, 0x00ff); //0�� Master, 3�� ��Ʈ, 3�� �����̺� , 16bit ����
	// 	FAS_IO.Set_IO_DefineWord(0, 3, 4, 0x07ff); //0�� Master, 3�� ��Ʈ, 4�� �����̺� , 16bit ����
	// 	FAS_IO.Set_IO_DefineWord(0, 3, 5, 0x0000); //0�� Master, 3�� ��Ʈ, 5�� �����̺� , 16bit ����
	// 	FAS_IO.Set_IO_DefineWord(0, 3, 6, 0x0000); //0�� Master, 3�� ��Ʈ, 6�� �����̺� , 16bit ����
	// 	FAS_IO.Set_IO_DefineWord(0, 3, 7, 0x00ff); //0�� Master, 3�� ��Ʈ, 7�� �����̺� , 16bit ����
	// 
	// 	// master 1 port 0
	// 	FAS_IO.Set_IO_DefineWord(1, 0, 0, 0x003f); //1�� Master, 0�� ��Ʈ, 0�� �����̺� , 16bit ����			
	// 	FAS_IO.Set_IO_DefineWord(1, 0, 1, 0x00ff); //1�� Master, 0�� ��Ʈ, 1�� �����̺� , 16bit ����
	// 	FAS_IO.Set_IO_DefineWord(1, 0, 2, 0x000f); //1�� Master, 0�� ��Ʈ, 2�� �����̺� , 16bit ����
	// 	FAS_IO.Set_IO_DefineWord(1, 0, 3, 0x00ff); //1�� Master, 0�� ��Ʈ, 3�� �����̺� , 16bit ����
	// 
	// 	// master 2 port 0		//pogo Cylinder ����..1ȣ�⸸ ����
	// 	FAS_IO.Set_IO_DefineWord(2, 0, 0, 0xffff);
	// 	FAS_IO.Set_IO_DefineWord(2, 0, 1, 0x00ff);
	// 	FAS_IO.Set_IO_DefineWord(2, 0, 2, 0x0000);
	// 	FAS_IO.Set_IO_DefineWord(2, 0, 3, 0xffff);
	// 	FAS_IO.Set_IO_DefineWord(2, 0, 4, 0x00ff);
	// 	FAS_IO.Set_IO_DefineWord(2, 0, 5, 0x0000);

	m_nIoBoardInitialFlag	= BD_YES;

	return RET_GOOD;
}


void CFastechPublic_IO::Initialize_Map()
{
	//int i, j;
	
	/////kwlee 2016.0323 ���� ����
	///////////////////////////////////////////////////////////////////////////////////////////////
	// Module No 00
	///////////////////////////////////////////////////////////////////////////////////////////////
	 	st_io_info.o_ZaxisBreak					= 0;			// S0000
	 	st_io_info.o_TurnMotorDirection			= 1;			// S0001
	 	st_io_info.o_ResetLamp					= 2;			// S0002
	 	st_io_info.o_StartLamp					= 3;			// S0003
	 	st_io_info.o_StopLamp					= 4;			// S0004
	 	st_io_info.o_AlarmClrLamp               = 5;			// S0005									
	 	st_io_info.o_BuzzerOff					= 6; 			// S0006									
	 	st_io_info.o_LabelFeederInterFace3      = 7;			// S0007
	 
	 	st_io_info.i_AutoChk				= 8;			//PS0000
	 	st_io_info.i_DoorLockChk			= 9;			//PS0001
		st_io_info.i_ResetChk				= 10;			//PS0002
	 	st_io_info.i_StartChk				= 11;			//PS0003
	 	st_io_info.i_StopChk				= 12;			//PS0004
		st_io_info.i_BuzzOffChk				= 13;			//PS0005
	 	st_io_info.i_EmoSwChk				= 14;			//PS0006
	 	st_io_info.i_AirDownSwChk			= 15;			//PS0007
	// 
	// 	///////////////////////////////////////////////////////////////////////////////////////////////
	// 	// Module No 01
	// 	//////////////////////////////////////////////////////////////////////////////////////////////
	 	st_io_info.o_TwGreenLamp			= 100;		// S0100
	 	st_io_info.o_TwYellowLamp			= 101;		// S0101
	 	st_io_info.o_TwRedLamp				= 102;		// S0102
	 	st_io_info.o_BuzzOn					= 103;		// S0103
	 	st_io_info.o_InConvMotorOn			= 104;		// S0104
	 	st_io_info.o_InConvPosStopperUp		= 105;		// S0105
	 	st_io_info.o_InConvPosStopperDw		= 106;		// S0106
	 	st_io_info.o_TurnPosUp				= 107;		// S0107
	 
		st_io_info.i_InConvInChk				= 108;		//PS0100	
	 	st_io_info.i_InConvPosChk				= 109;		//PS0101	
	 	st_io_info.i_InConvPosUpChk				= 110;		//PS0102
	// 	//PS0103
	 	st_io_info.i_TurnPosUpChk				= 112;		//PS0104	
	 	st_io_info.i_TurnPosDwChk				= 113;		//PS0105	
	 	st_io_info.i_TurnConvStopperForChk		= 114;		//PS0106	
	 	st_io_info.i_TurnConvStopperBackChk		= 115;		//PS0107	
	// 
	// 	///////////////////////////////////////////////////////////////////////////////////////////////
	// 	// Module No 02
	// 	///////////////////////////////////////////////////////////////////////////////////////////////
	 	st_io_info.o_TurnPosDw						= 200;		//S0200
	 	st_io_info.o_OutConvMotorOn					= 201;		//S0201
	 	st_io_info.o_OutPosStopperUp				= 202;		//S0202
	 	st_io_info.o_OutPosStopperDw				= 203;		//S0203
	 	st_io_info.o_TablePcbMovCw					= 204;		//S0204
	 	st_io_info.o_TablePcbMovCcw					= 205;		//S0205
	 	st_io_info.o_TableMotorForCyl				= 206;		//S0206
	 	st_io_info.o_TableMotorBackCyl				= 207;		//S0207
	// 
	 	st_io_info.i_OutConvChk						= 208;		//PS0200
	 	st_io_info.i_OutInConvPosChk				= 209;		//PS0201
	 	st_io_info.i_OutConvPosUpChk				= 210;		//PS0202
	 	st_io_info.i_TurnConvPosChk					= 211;		//PS0203
	// 	//PS0204
	// 	//PS0205
	 	st_io_info.i_TurnRotatCylForChk				= 214;		//PS0206
	 	st_io_info.i_TurnRotatCylBackChk		    = 215;		//PS0207
	// 
	// 
	// 
	// 	///////////////////////////////////////////////////////////////////////////////////////////////
	// 	// Module No 03
	// 	///////////////////////////////////////////////////////////////////////////////////////////////
	 	st_io_info.o_HeadVacOff1		= 300;		// S0300
	 	st_io_info.o_HeadVacOff2		= 301;		// S0301
	 	st_io_info.o_HeadVacOff3		= 302;		// S0302
	 	st_io_info.o_HeadVacOff4		= 303;		// S0303
	 	st_io_info.o_HeadVacOff5		= 304;		// S0304
	 	st_io_info.o_HeadVacOff6		= 305;		// S0305
	 	st_io_info.o_HeadVacOn1			= 306;		// S0306
	 	st_io_info.o_HeadVacOn2			= 307;		// S0307
	 	st_io_info.o_HeadVacOn3			= 308;		// S0308
	 	st_io_info.o_HeadVacOn4			= 309;		// S0309
	 	st_io_info.o_HeadVacOn5			= 310;		// S0310
	 	st_io_info.o_HeadVacOn6			= 311;		// S0311
	// 
	// 	///////////////////////////////////////////////////////////////////////////////////////////////
	// 	// Module No 04
	// 	///////////////////////////////////////////////////////////////////////////////////////////////
		st_io_info.o_HeadVacOff7		= 400;					// S0400
	 	st_io_info.o_HeadVacOff8		= 401;					// S0401
	 	st_io_info.o_HeadVacOff9		= 402;					// S0402
	 	st_io_info.o_HeadVacOff10		= 403;					// S0403	
	 	st_io_info.o_HeadVacOff11       = 404;					// S0404
		st_io_info.o_HeadVacOff12		= 405;					// S0405
	 	st_io_info.o_HeadVacOn7			= 406;					// S0406
	 	st_io_info.o_HeadVacOn8			= 407;					// S0407
	 	st_io_info.o_HeadVacOn9			= 408;					// S0408
	 	st_io_info.o_HeadVacOn10		= 409;					// S0409
		st_io_info.o_HeadVacOn11		= 410;					// S0410
	 	st_io_info.o_HeadVacOn12		= 411;					// S0411
	// 
	// 
	// 	///////////////////////////////////////////////////////////////////////////////////////////////
	// 	// Module No 05
	// 	///////////////////////////////////////////////////////////////////////////////////////////////
	 	st_io_info.o_HeadCylUpDw1			= 500;		// S0500	
	 	st_io_info.o_HeadCylUpDw2				= 501;		// S0501	
	 	st_io_info.o_HeadCylUpDw3				= 502;		// S0502	
	 	st_io_info.o_HeadCylUpDw4				= 503;		// S0503	
	 	st_io_info.o_HeadCylUpDw5				= 504;		// S0504		  									
	 	st_io_info.o_HeadCylUpDw6				= 505;		// S0505
	 	st_io_info.o_HeadCylUpDw7				= 506;		// S0506
	 	st_io_info.o_HeadCylUpDw8				= 507;		// S0507
	// 
	 	st_io_info.i_HeadVacOnChk1			= 508;		//PS0500
	 	st_io_info.i_HeadVacOnChk2			= 509;		//PS0501
	 	st_io_info.i_HeadVacOnChk3			= 510;		//PS0502
	 	st_io_info.i_HeadVacOnChk4			= 511;		//PS0503
	 	st_io_info.i_HeadVacOnChk5			= 512;		//PS0504
	 	st_io_info.i_HeadVacOnChk6			= 513;		//PS0505
	// 	//PS0506
	// 	//PS0507
	// 	///////////////////////////////////////////////////////////////////////////////////////////////
	// 	// Module No 06
	// 	///////////////////////////////////////////////////////////////////////////////////////////////
	 	st_io_info.o_HeadCylUpDw9		= 600;		// S0600
	 	st_io_info.o_HeadCylUpDw10		= 601;		// S0601
	 	st_io_info.o_HeadCylUpDw11		= 602;		// S0602
	 	st_io_info.o_HeadCylUpDw12		= 603;		// S0603
	// 	// S0604
	// 	// S0605
	// 	// S0606
	// 	// S0607		  
	 	st_io_info.i_HeadVacOnChk7		= 608;		//PS0600
	 	st_io_info.i_HeadVacOnChk8		= 609;		//PS0601
		st_io_info.i_HeadVacOnChk9		= 610;		//PS0602
	 	st_io_info.i_HeadVacOnChk10		= 611;		//PS0603
	 	st_io_info.i_HeadVacOnChk11		= 612;		//PS0604
	 	st_io_info.i_HeadVacOnChk12		= 613;      //PS0605		 
	// 	//PS0606
	// 	//PS0607
	// 
	// 	///////////////////////////////////////////////////////////////////////////////////////////////
	// 	// Module No 07
	// 	///////////////////////////////////////////////////////////////////////////////////////////////
	 	st_io_info.o_LabelFeederMotorOn			= 700;		// S0700	
	 	st_io_info.o_LabelFeederClmpOn1			= 701;		// S0701
	 	st_io_info.o_LabelFeederClmpOn2			= 702;		// S0702
	// 	// S0703
	 	st_io_info.o_PcReady					= 704;		//S0704
	 	st_io_info.o_PcComplete					= 705;		//S0705		
	 	st_io_info.o_TableTurnCylFor			= 706;		//S0706		
	 	st_io_info.o_TableTurnCylBack			= 707;		//S0707	
	// 
	 	st_io_info.i_LabelFeederProductChk1		= 708;						//PS0700
	 	st_io_info.i_LabelFeederProductChk2		= 709;						//PS0701
	// 	//PS0702
	 	st_io_info.i_LabelFeederStopperChk1		= 711;						//PS0703
	// 	//PS0704
	 	st_io_info.i_LfReqChk					= 713;		//PS0705
	 	st_io_info.i_LfCompChk					= 714;		//PS0706
	 	st_io_info.i_LfAlarmChk					= 715;		//PS0707
	// 
	// 	///////////////////////////////////////////////////////////////////////////////////////////////
	// 	// Module No 10
	// 	///////////////////////////////////////////////////////////////////////////////////////////////
	 	st_io_info.o_Front_LabelReq						= 1000;		// S1000	
	 	st_io_info.o_Front_LabelComplete				= 1001;		// S1001
	 	st_io_info.o_Rear_Label_Ready					= 1002;		// S1002
		st_io_info.o_Rear_Label_Complete				= 1003;		// S1003
	 	st_io_info.o_LabelInterface5					= 1004;		// S1004
	 	st_io_info.o_LabelInterface6					= 1005;		// S1005
	 	st_io_info.o_LabelInterface7					= 1006;		// S1006
	// 	// S1007
	 	st_io_info.i_LabelFeederProductChk1_1			= 1008;		//PS1000	
	 	st_io_info.i_LabelFeederProductChk2_1			= 1009;		//PS1001	
		st_io_info.i_FrontReady							= 1010;     //PS1002
		st_io_info.i_FrontComplete		                = 1011;     //PS1003
		st_io_info.i_RearReq							= 1012;     //PS1002
		st_io_info.i_RearComplete		                = 1013;     //PS1003


		st_io_info.o_HeadCylUpDn[0]						 = st_io_info.o_HeadCylUpDw1;
		st_io_info.o_HeadCylUpDn[1]						 = st_io_info.o_HeadCylUpDw2;
		st_io_info.o_HeadCylUpDn[2]						 = st_io_info.o_HeadCylUpDw3;
		st_io_info.o_HeadCylUpDn[3]						 = st_io_info.o_HeadCylUpDw4;
		st_io_info.o_HeadCylUpDn[4]						 = st_io_info.o_HeadCylUpDw5;
		st_io_info.o_HeadCylUpDn[5]						 = st_io_info.o_HeadCylUpDw6;
		st_io_info.o_HeadCylUpDn[6]						 = st_io_info.o_HeadCylUpDw7;
		st_io_info.o_HeadCylUpDn[7]						 = st_io_info.o_HeadCylUpDw8;
		st_io_info.o_HeadCylUpDn[8]						 = st_io_info.o_HeadCylUpDw9;
		st_io_info.o_HeadCylUpDn[9]						 = st_io_info.o_HeadCylUpDw10;
		st_io_info.o_HeadCylUpDn[10]                     = st_io_info.o_HeadCylUpDw11;
		st_io_info.o_HeadCylUpDn[11]                     = st_io_info.o_HeadCylUpDw12;

		st_io_info.i_HeadVaccumCheck[0]                             = st_io_info.i_HeadVacOnChk1;
		st_io_info.i_HeadVaccumCheck[1]                             = st_io_info.i_HeadVacOnChk2;
		st_io_info.i_HeadVaccumCheck[2]                             = st_io_info.i_HeadVacOnChk3;
		st_io_info.i_HeadVaccumCheck[3]                             = st_io_info.i_HeadVacOnChk4;
		st_io_info.i_HeadVaccumCheck[4]                             = st_io_info.i_HeadVacOnChk5;
		st_io_info.i_HeadVaccumCheck[5]                             = st_io_info.i_HeadVacOnChk6;
		st_io_info.i_HeadVaccumCheck[6]                             = st_io_info.i_HeadVacOnChk7;
		st_io_info.i_HeadVaccumCheck[7]                             = st_io_info.i_HeadVacOnChk8;
		st_io_info.i_HeadVaccumCheck[8]                             = st_io_info.i_HeadVacOnChk9;
		st_io_info.i_HeadVaccumCheck[9]                             = st_io_info.i_HeadVacOnChk10;
		st_io_info.i_HeadVaccumCheck[10]                            = st_io_info.i_HeadVacOnChk11;
		st_io_info.i_HeadVaccumCheck[11]                            = st_io_info.i_HeadVacOnChk12;

		st_io_info.o_HeadVaccum[0]                             = st_io_info.o_HeadVacOn1;
		st_io_info.o_HeadVaccum[1]                             = st_io_info.o_HeadVacOn2;
		st_io_info.o_HeadVaccum[2]                             = st_io_info.o_HeadVacOn3;
		st_io_info.o_HeadVaccum[3]                             = st_io_info.o_HeadVacOn4;
		st_io_info.o_HeadVaccum[4]                             = st_io_info.o_HeadVacOn5;
		st_io_info.o_HeadVaccum[5]                             = st_io_info.o_HeadVacOn6;
		st_io_info.o_HeadVaccum[6]                             = st_io_info.o_HeadVacOn7;
		st_io_info.o_HeadVaccum[7]                             = st_io_info.o_HeadVacOn8;
		st_io_info.o_HeadVaccum[8]                             = st_io_info.o_HeadVacOn9;
		st_io_info.o_HeadVaccum[9]                             = st_io_info.o_HeadVacOn10;
		st_io_info.o_HeadVaccum[10]                            = st_io_info.o_HeadVacOn11;
		st_io_info.o_HeadVaccum[11]                            = st_io_info.o_HeadVacOn12;

		st_io_info.o_HeadVaccumOff[0]                           = st_io_info.o_HeadVacOff1;
		st_io_info.o_HeadVaccumOff[1]                           = st_io_info.o_HeadVacOff2;
		st_io_info.o_HeadVaccumOff[2]                           = st_io_info.o_HeadVacOff3;
		st_io_info.o_HeadVaccumOff[3]                           = st_io_info.o_HeadVacOff4;
		st_io_info.o_HeadVaccumOff[4]                           = st_io_info.o_HeadVacOff5;
		st_io_info.o_HeadVaccumOff[5]                           = st_io_info.o_HeadVacOff6;
		st_io_info.o_HeadVaccumOff[6]                           = st_io_info.o_HeadVacOff7;
		st_io_info.o_HeadVaccumOff[7]                           = st_io_info.o_HeadVacOff8;
		st_io_info.o_HeadVaccumOff[8]                           = st_io_info.o_HeadVacOff9;
		st_io_info.o_HeadVaccumOff[9]                           = st_io_info.o_HeadVacOff10;
		st_io_info.o_HeadVaccumOff[10]                          = st_io_info.o_HeadVacOff11;
		st_io_info.o_HeadVaccumOff[11]                          = st_io_info.o_HeadVacOff12;

}


// ******************************************************************************
//  �Ǹ��� ���۽�Ű��  �Լ� (�ܵ�)                                               
// ******************************************************************************
void CFastechPublic_IO::OnCylinderAction(int n_cyl, int n_act)
{
	//Set_Out_Bit(n_cyl, n_act);
	set_out_bit(n_cyl, n_act);
	
	CYLTime[n_cyl][0] = GetCurrentTime();
	
	CYLFlag[n_cyl] = false;
}

// ******************************************************************************
//  �Ǹ��� ���۽�Ű��  �Լ� (������)                                             
// ******************************************************************************
void CFastechPublic_IO::OnCylinderAction(int n_cyl, int n_cyl2, int n_act)
{
	//Set_Out_Bit(n_cyl, n_act);
	//Set_Out_Bit(n_cyl2, !n_act);
	set_out_bit(n_cyl, n_act);
	set_out_bit(n_cyl2, !n_act);
	
	CYLTime[n_cyl][0] = GetCurrentTime();
	
	CYLFlag[n_cyl] = false;
}

// ******************************************************************************
//  �Ǹ��� ���� ��Ų�� Ȯ���ϴ� �Լ� (���� �ϳ���)                               
// ******************************************************************************
int CFastechPublic_IO::OnCylinderActionCheck(int n_cyl, int n_sen, int n_act, int n_wait, int n_limit)
{
	int Ret = RET_PROCEED;  // �Լ� ���� �÷��� ���� 
	
	if (get_in_bit(n_sen, n_act) == n_act && CYLFlag[n_cyl] == false)
	{
		CYLTime[n_cyl][0]	= GetCurrentTime();
		CYLFlag[n_cyl] = true;
	}
	else if (get_in_bit(n_sen, n_act) == n_act && CYLFlag[n_cyl] == true)
	{
		CYLTime[n_cyl][1] = GetCurrentTime();
		CYLTime[n_cyl][2] = CYLTime[n_cyl][1] - CYLTime[n_cyl][0];
		
		if (CYLTime[n_cyl][2] >= n_wait)
		{
			Ret = RET_GOOD;
		}
	}
	else
	{
		CYLTime[n_cyl][1] = GetCurrentTime();
		CYLTime[n_cyl][2] = CYLTime[n_cyl][1] - CYLTime[n_cyl][0];
		
		if (CYLTime[n_cyl][2] >= n_limit)
		{
			Ret = RET_ERROR;
		}
	}
	return Ret;
}

// ******************************************************************************
//  �Ǹ��� ���� ��Ų�� Ȯ���ϴ� �Լ� (���� �ΰ���)                               
// ******************************************************************************
int CFastechPublic_IO::OnCylinderActionCheck(int n_cyl, int n_sen1, int n_sen2, int n_act, int n_wait, int n_limit)
{
	int Ret = RET_PROCEED;  // �Լ� ���� �÷��� ���� 
	
	if (get_in_bit(n_sen1, n_act) == n_act && 
		get_in_bit(n_sen2, !n_act) == !n_act && 
		CYLFlag[n_cyl] == false)
	{
		CYLTime[n_cyl][0]	= GetCurrentTime();
		CYLFlag[n_cyl] = true;
	}
	else if (get_in_bit(n_sen1, n_act) == n_act && 
		get_in_bit(n_sen2, !n_act) == !n_act && 
		CYLFlag[n_cyl] == true)
	{
		CYLTime[n_cyl][1] = GetCurrentTime();
		CYLTime[n_cyl][2] = CYLTime[n_cyl][1] - CYLTime[n_cyl][0];
		
		if (CYLTime[n_cyl][2] >= n_wait)
		{
			Ret = RET_GOOD;
		}
	}
	else
	{
		CYLTime[n_cyl][1] = GetCurrentTime();
		CYLTime[n_cyl][2] = CYLTime[n_cyl][1] - CYLTime[n_cyl][0];
		
		if (CYLTime[n_cyl][2] >= n_limit)
		{
			Ret = RET_ERROR;
		}
	}
	return Ret;
}
