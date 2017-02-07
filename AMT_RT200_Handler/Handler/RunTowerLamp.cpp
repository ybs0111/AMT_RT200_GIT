// RunTowerLamp.cpp: implementation of the CRunTowerLamp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RunTowerLamp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define LAMP_START			0
#define LAMP_STOP			1
#define LAMP_RESET			2
#define LAMP_BUZZER			3

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRunTowerLamp	clsTowerLamp;

CRunTowerLamp::CRunTowerLamp()
{
	m_nLampStep		= 0;
	m_nOldStatus	= 0;
}

CRunTowerLamp::~CRunTowerLamp()
{

}

//==================================================================//
// ������ ����
//==================================================================//
void CRunTowerLamp::OnThreadRun() 
{
	//==============================================================//
	// I/O ���� ����� ȭ�� ��µ� ��� ����
	// : �������� ������ I/O ȭ�鿡�� Ÿ�� ���� ��Ʈ���� �Ұ���
	// : �� �����忡�� ����ؼ� Ÿ�� ���� ��� �������� ����
	//==============================================================//
	if (st_handler_info.nMenuNum == IDW_SCREEN_IO_MAP)	
	{
		return;
	}

// 	if (st_handler_info.nRunStatus == dJAM || st_handler_info.nRunStatus == dWARNING)
// 	{
// 		FAS_IO.set_out_bit(st_io_info.o_StopLamp, IO_ON);
// 		FAS_IO.set_out_bit(st_io_info.o_StartLamp, IO_OFF);
// 	}

	if(m_nOldStatus != st_handler_info.nRunStatus)
	{
		m_nOldStatus	= st_handler_info.nRunStatus;
		m_nLampStep		= 0;
	}

	switch(st_handler_info.nRunStatus)
	{
		case dINIT:
			OnRunInit();
			break;
			
		case dSTOP:
		case dRUN:
		case dIDLE:
		case dJAM:
		case dLOTEND:
		case dWARNING:
		case dLOCK:
		case dSELFCHECK:
		case dMAINT:
		case dREINSTATE:
		case dRECOVERY:
			OnTowerLamp();
			break;

		default:
			
			break;
	}
}


void CRunTowerLamp::OnTowerLamp(int nLamp)
{
	switch(nLamp)
	{
		case 0:
			// ====================================================================================
			//  RED Ÿ�� ���� ��� �����Ѵ�												
			// ====================================================================================
			FAS_IO.set_out_bit(st_io_info.o_TwRedLamp, IO_ON);
			// ====================================================================================

			// ====================================================================================
			//  YELLOW Ÿ�� ���� ��� �����Ѵ�											
			// ====================================================================================
			FAS_IO.set_out_bit(st_io_info.o_TwYellowLamp, IO_OFF);
			// ====================================================================================

			// ====================================================================================
			//  GREEN Ÿ�� ���� ��� �����Ѵ�
			// ====================================================================================
			FAS_IO.set_out_bit(st_io_info.o_TwGreenLamp, IO_OFF);
			// ====================================================================================
			break;
		case 1:
			// ====================================================================================
			//  RED Ÿ�� ���� ��� �����Ѵ�												
			// ====================================================================================
			FAS_IO.set_out_bit(st_io_info.o_TwRedLamp, IO_OFF);
			// ====================================================================================

			// ====================================================================================
			//  YELLOW Ÿ�� ���� ��� �����Ѵ�											
			// ====================================================================================
			FAS_IO.set_out_bit(st_io_info.o_TwYellowLamp, IO_ON);
			// ====================================================================================

			// ====================================================================================
			//  GREEN Ÿ�� ���� ��� �����Ѵ�
			// ====================================================================================
			FAS_IO.set_out_bit(st_io_info.o_TwGreenLamp, IO_OFF);
			// ====================================================================================
			break;
		case 2:
			// ====================================================================================
			//  RED Ÿ�� ���� ��� �����Ѵ�		
			// ====================================================================================
			FAS_IO.set_out_bit(st_io_info.o_TwRedLamp, IO_OFF);
			// ====================================================================================

			// ====================================================================================
			//  YELLOW Ÿ�� ���� ��� �����Ѵ�											
			// ====================================================================================
			FAS_IO.set_out_bit(st_io_info.o_TwYellowLamp, IO_OFF);
			// ====================================================================================

			// ====================================================================================
			//  GREEN Ÿ�� ���� ��� �����Ѵ�
			// ====================================================================================
			FAS_IO.set_out_bit(st_io_info.o_TwGreenLamp, IO_ON);
			// ====================================================================================
			break;
		default:
			// ====================================================================================
			//  RED Ÿ�� ���� ��� �����Ѵ�												
			// ====================================================================================
			FAS_IO.set_out_bit(st_io_info.o_TwRedLamp, IO_OFF);
			// ====================================================================================

			// ====================================================================================
			//  YELLOW Ÿ�� ���� ��� �����Ѵ�											
			// ====================================================================================
			FAS_IO.set_out_bit(st_io_info.o_TwYellowLamp, IO_OFF);
			// ====================================================================================

			// ====================================================================================
			//  GREEN Ÿ�� ���� ��� �����Ѵ�
			// ====================================================================================
			FAS_IO.set_out_bit(st_io_info.o_TwGreenLamp, IO_OFF);
			// ====================================================================================
			break;
	}
}

void CRunTowerLamp::OnSwitchLamp(int nLamp)
{
	
	switch(nLamp)
	{
		case 0:
			FAS_IO.set_out_bit(st_io_info.o_StartLamp,	IO_ON);
			FAS_IO.set_out_bit(st_io_info.o_StopLamp,	IO_OFF);
			FAS_IO.set_out_bit(st_io_info.o_AlarmClrLamp,IO_OFF);
			FAS_IO.set_out_bit(st_io_info.o_ResetLamp,	IO_OFF);
			break;

		case 1:
			FAS_IO.set_out_bit(st_io_info.o_StartLamp,	IO_OFF);
			FAS_IO.set_out_bit(st_io_info.o_StopLamp,	IO_ON);
			FAS_IO.set_out_bit(st_io_info.o_AlarmClrLamp,IO_OFF);
			FAS_IO.set_out_bit(st_io_info.o_ResetLamp,	IO_OFF);
			break;
		case 2:
			FAS_IO.set_out_bit(st_io_info.o_StartLamp,	IO_OFF);
			FAS_IO.set_out_bit(st_io_info.o_StopLamp,	IO_OFF);
			FAS_IO.set_out_bit(st_io_info.o_AlarmClrLamp,IO_ON);
			FAS_IO.set_out_bit(st_io_info.o_ResetLamp,	IO_OFF);
			break;
		case 3:
			FAS_IO.set_out_bit(st_io_info.o_StartLamp,	IO_OFF);
			FAS_IO.set_out_bit(st_io_info.o_StopLamp,	IO_OFF);
			FAS_IO.set_out_bit(st_io_info.o_AlarmClrLamp,IO_OFF);
			FAS_IO.set_out_bit(st_io_info.o_ResetLamp,	IO_ON);
			break;
		case 4:
			FAS_IO.set_out_bit(st_io_info.o_StartLamp,	IO_OFF);
			FAS_IO.set_out_bit(st_io_info.o_StopLamp,	IO_OFF);
			FAS_IO.set_out_bit(st_io_info.o_AlarmClrLamp,IO_OFF);
			FAS_IO.set_out_bit(st_io_info.o_ResetLamp,	IO_OFF);
			break;
		default:
			FAS_IO.set_out_bit(st_io_info.o_StartLamp,	IO_OFF);
			FAS_IO.set_out_bit(st_io_info.o_StopLamp,	IO_OFF);
			FAS_IO.set_out_bit(st_io_info.o_AlarmClrLamp,IO_OFF);
			FAS_IO.set_out_bit(st_io_info.o_ResetLamp,	IO_OFF);
			break;
	}
}

void CRunTowerLamp::OnRunInit()
{
	switch(m_nLampStep)
	{
		case 0 :
			OnTowerLamp(0);
			OnSwitchLamp(0);
			// ========================================================================
			// Ÿ�� ���� ON/OFF ���� �����ϱ� ���Ͽ� ���� �ð� ��´�					
			// -> ȭ�� ���� �ð���ŭ ON/OFF �ð� �����Ѵ�								
			// ========================================================================
			m_lWaitTime[0] = GetCurrentTime();
			m_nLampStep++;
			// ========================================================================
			break;
		case 1 :
			// ========================================================================
			// ȭ�� ���� ���� ON/OFF �ð����� ����Ѵ�									
			// -> st_maintenance.i_lamp_wait_time : ���� ON/OFF ���� �ð� ���� ����			
			// ========================================================================
			m_lWaitTime[1] = GetCurrentTime();
			m_lWaitTime[2] = m_lWaitTime[1] - m_lWaitTime[0];

			if(m_lWaitTime[2] < 0)
			{
				m_lWaitTime[0] = GetCurrentTime();
				break;
			}
			else if(m_lWaitTime[2] > st_lamp_info.nLampWaitTime)
			{
				m_nLampStep++;
			}
			// ========================================================================
			break;
		case 2 :
			OnTowerLamp(1);
			OnSwitchLamp(1);
			// ========================================================================
			// Ÿ�� ���� ON/OFF ���� �����ϱ� ���Ͽ� ���� �ð� ��´�					
			// -> ȭ�� ���� �ð���ŭ ON/OFF �ð� �����Ѵ�								
			// ========================================================================
			m_lWaitTime[0] = GetCurrentTime();
			m_nLampStep++;
			// ========================================================================
			break;
		case 3 :
			// ========================================================================
			// ȭ�� ���� ���� ON/OFF �ð����� ����Ѵ�									
			// -> st_maintenance.i_lamp_wait_time : ���� ON/OFF ���� �ð� ���� ����			
			// ========================================================================
			m_lWaitTime[1] = GetCurrentTime();
			m_lWaitTime[2] = m_lWaitTime[1] - m_lWaitTime[0];
				
			if(m_lWaitTime[2] < 0)
			{
				m_lWaitTime[0] = GetCurrentTime();
				break;
			}
			else if(m_lWaitTime[2] > st_lamp_info.nLampWaitTime)
			{
				m_nLampStep++;
			}
			// ========================================================================
			break;
		case 4:
			OnTowerLamp(2);
			OnSwitchLamp(2);
			// ========================================================================
			// Ÿ�� ���� ON/OFF ���� �����ϱ� ���Ͽ� ���� �ð� ��´�					
			// -> ȭ�� ���� �ð���ŭ ON/OFF �ð� �����Ѵ�								
			// ========================================================================
			m_lWaitTime[0] = GetCurrentTime();
			m_nLampStep++;
			// ========================================================================
			break;
		case 5:
			// ========================================================================
			// ȭ�� ���� ���� ON/OFF �ð����� ����Ѵ�									
			// -> st_maintenance.i_lamp_wait_time : ���� ON/OFF ���� �ð� ���� ����			
			// ========================================================================
			m_lWaitTime[1] = GetCurrentTime();
			m_lWaitTime[2] = m_lWaitTime[1] - m_lWaitTime[0];
				
			if(m_lWaitTime[2] < 0)
			{
				m_lWaitTime[0] = GetCurrentTime();
				break;
			}
			else if(m_lWaitTime[2] > st_lamp_info.nLampWaitTime)
			{
				m_nLampStep++;
			}
			// ========================================================================
			break;
		case 6:
			OnTowerLamp(1);
			OnSwitchLamp(3);
			// ========================================================================
			// Ÿ�� ���� ON/OFF ���� �����ϱ� ���Ͽ� ���� �ð� ��´�					
			// -> ȭ�� ���� �ð���ŭ ON/OFF �ð� �����Ѵ�								
			// ========================================================================
			m_lWaitTime[0] = GetCurrentTime();
			m_nLampStep++;
			// ========================================================================
			break;
		case 7:
			// ========================================================================
			// ȭ�� ���� ���� ON/OFF �ð����� ����Ѵ�								   
			// -> st_maintenance.i_lamp_wait_time : ���� ON/OFF ���� �ð� ���� ����			
			// ========================================================================
			m_lWaitTime[1] = GetCurrentTime();
			m_lWaitTime[2] = m_lWaitTime[1] - m_lWaitTime[0];
				
			if(m_lWaitTime[2] < 0)
			{
				m_lWaitTime[0] = GetCurrentTime();
				break;
			}
			else if(m_lWaitTime[2] > st_lamp_info.nLampWaitTime)
			{
				m_nLampStep++;
			}
			// ========================================================================
			break;
		case 8:
			OnTowerLamp(0);
			OnSwitchLamp(4);
			// ========================================================================
			// Ÿ�� ���� ON/OFF ���� �����ϱ� ���Ͽ� ���� �ð� ��´�					
			// -> ȭ�� ���� �ð���ŭ ON/OFF �ð� �����Ѵ�								
			// ========================================================================
			m_lWaitTime[0] = GetCurrentTime();
			m_nLampStep++;
			// ========================================================================
			break;
		case 9:
			// ========================================================================
			// ȭ�� ���� ���� ON/OFF �ð����� ����Ѵ�									
			// -> st_maintenance.i_lamp_wait_time : ���� ON/OFF ���� �ð� ���� ����			
			// ========================================================================
			m_lWaitTime[1] = GetCurrentTime();
			m_lWaitTime[2] = m_lWaitTime[1] - m_lWaitTime[0];
				
			if(m_lWaitTime[2] < 0)
			{
				m_lWaitTime[0] = GetCurrentTime();
				break;
			}
			else if(m_lWaitTime[2] > st_lamp_info.nLampWaitTime)
			{
				m_nLampStep++;
			}
			// ========================================================================
			break;
		case 10:
			OnTowerLamp(1);
			OnSwitchLamp(3);
			// ========================================================================
			// Ÿ�� ���� ON/OFF ���� �����ϱ� ���Ͽ� ���� �ð� ��´�					
			// -> ȭ�� ���� �ð���ŭ ON/OFF �ð� �����Ѵ�								
			// ========================================================================
			m_lWaitTime[0] = GetCurrentTime();
			m_nLampStep++;
			// ========================================================================
			break;
		case 11:
			// ========================================================================
			// ȭ�� ���� ���� ON/OFF �ð����� ����Ѵ�									
			// -> st_maintenance.i_lamp_wait_time : ���� ON/OFF ���� �ð� ���� ����			
			// ========================================================================
			m_lWaitTime[1] = GetCurrentTime();
			m_lWaitTime[2] = m_lWaitTime[1] - m_lWaitTime[0];
				
			if(m_lWaitTime[2] < 0)
			{
				m_lWaitTime[0] = GetCurrentTime();
				break;
			}
			else if(m_lWaitTime[2] > st_lamp_info.nLampWaitTime)
			{
				m_nLampStep++;
			}
			// ========================================================================
			break;
		case 12:
			OnTowerLamp(2);
			OnSwitchLamp(2);
			// ========================================================================
			// Ÿ�� ���� ON/OFF ���� �����ϱ� ���Ͽ� ���� �ð� ��´�					
			// -> ȭ�� ���� �ð���ŭ ON/OFF �ð� �����Ѵ�								
			// ========================================================================
			m_lWaitTime[0] = GetCurrentTime();
			m_nLampStep++;
			// ========================================================================
			break;
		case 13:
			// ========================================================================
			// ȭ�� ���� ���� ON/OFF �ð����� ����Ѵ�									
			// -> st_maintenance.i_lamp_wait_time : ���� ON/OFF ���� �ð� ���� ����			
			// ========================================================================
			m_lWaitTime[1] = GetCurrentTime();
			m_lWaitTime[2] = m_lWaitTime[1] - m_lWaitTime[0];
				
			if(m_lWaitTime[2] < 0)
			{
				m_lWaitTime[0] = GetCurrentTime();
				break;
			}
			else if(m_lWaitTime[2] > st_lamp_info.nLampWaitTime)
			{
				m_nLampStep++;
			}
			// ========================================================================
			break;
		case 14:
			OnTowerLamp(1);
			OnSwitchLamp(1);
			// ========================================================================
			// Ÿ�� ���� ON/OFF ���� �����ϱ� ���Ͽ� ���� �ð� ��´�					
			// -> ȭ�� ���� �ð���ŭ ON/OFF �ð� �����Ѵ�								
			// ========================================================================
			m_lWaitTime[0] = GetCurrentTime();
			m_nLampStep++;
			// ========================================================================
			break;
		case 15:
			// ========================================================================
			// ȭ�� ���� ���� ON/OFF �ð����� ����Ѵ�									
			// -> st_maintenance.i_lamp_wait_time : ���� ON/OFF ���� �ð� ���� ����			
			// ========================================================================
			m_lWaitTime[1] = GetCurrentTime();
			m_lWaitTime[2] = m_lWaitTime[1] - m_lWaitTime[0];
				
			if(m_lWaitTime[2] < 0)
			{
				m_lWaitTime[0] = GetCurrentTime();
				break;
			}
			else if(m_lWaitTime[2] > st_lamp_info.nLampWaitTime)
			{
				m_nLampStep = 0;
			}
			// ========================================================================
			break;
	}
}


void CRunTowerLamp::OnTowerLamp()
{
	int nStatus;

	if (st_handler_info.nRunStatus == dRUN)
	{
		nStatus = st_handler_info.nRunStatus;
		if(st_handler_info.nIdleStatus == YES)
		{
			nStatus = dIDLE;
		}
	}
	else 
	{
		nStatus = st_handler_info.nRunStatus;
	}

// 	switch(m_nLampStep)
// 	{
// 		case 0 :
// 			// ************************************************************************
// 			//  RED Ÿ�� ���� ��� �����Ѵ�												
// 			// ************************************************************************
// 			if (st_lamp_info.nLampR[nStatus] == 1)       // RED Ÿ�� ���� ON
// 			{
// 				FAS_IO.set_out_bit(st_io_info.o_TwLamp[2], IO_ON);
// 			}
// 			else if (st_lamp_info.nLampR[nStatus] == 0)  // RED Ÿ�� ���� OFF
// 			{
// 				FAS_IO.set_out_bit(st_io_info.o_TwLamp[2], IO_OFF);
// 			}
// 			else if (st_lamp_info.nLampR[nStatus] == 2)  // RED Ÿ�� ���� FLICK
// 			{
// 				FAS_IO.set_out_bit(st_io_info.o_TwLamp[2], IO_ON);
// 			}
// 
// 			if (st_handler_info.nRunStatus == dRUN)
// 			{
// 				if (st_work_info.nTowerStatus[0] == YES || st_work_info.nTowerStatus[1] == YES || 
// 					st_work_info.nTowerStatus[2] == YES || st_work_info.nTowerStatus[3] == YES)
// 				{
// 					FAS_IO.set_out_bit(st_io_info.o_TwLamp[2], IO_ON);
// 				}
// 			}
// 			// ************************************************************************
// 
// 			// ************************************************************************
// 			//  GREEN Ÿ�� ���� ��� �����Ѵ�													
// 			// ************************************************************************
// 			if (st_lamp_info.nLampG[nStatus] == 1)       // GREEN Ÿ�� ���� ON
// 			{
// 				FAS_IO.set_out_bit(st_io_info.o_TwLamp[0], IO_ON);
// 			}
// 			else if (st_lamp_info.nLampG[nStatus] == 0)  // GREEN Ÿ�� ���� OFF
// 			{
// 				FAS_IO.set_out_bit(st_io_info.o_TwLamp[0], IO_OFF);
// 			}
// 			else if (st_lamp_info.nLampG[nStatus] == 2)  // GREEN Ÿ�� ���� FLICK
// 			{
// 				FAS_IO.set_out_bit(st_io_info.o_TwLamp[0], IO_ON);
// 			}
// 			// ************************************************************************
// 
// 			// ************************************************************************
// 			//  YELLOW Ÿ�� ���� ��� �����Ѵ�											
// 			// ************************************************************************
// 			if (st_lamp_info.nLampY[nStatus] == 1)       // YELLOW Ÿ�� ���� ON
// 			{
// 				FAS_IO.set_out_bit(st_io_info.o_TwLamp[1], IO_ON);
// 			}
// 			else if (st_lamp_info.nLampY[nStatus] == 0)  // YELLOW Ÿ�� ���� OFF
// 			{
// 				FAS_IO.set_out_bit(st_io_info.o_TwLamp[1], IO_OFF);
// 			}
// 			else if (st_lamp_info.nLampY[nStatus] == 2)  // YELLOW Ÿ�� ���� FLICK
// 			{
// 				FAS_IO.set_out_bit(st_io_info.o_TwLamp[1], IO_ON);
// 			}
// 			// ************************************************************************
// 
// 			// ************************************************************************
// 			// Ÿ�� ���� ON/OFF ���� �����ϱ� ���Ͽ� ���� �ð� ��´�					
// 			// -> ȭ�� ���� �ð���ŭ ON/OFF �ð� �����Ѵ�								
// 			// ************************************************************************
// 			m_lWaitTime[0] = GetCurrentTime();
// 			m_nLampStep++;
// 			// ************************************************************************
// 			break;
// 
// 		case 1 :
// 			// ************************************************************************
// 			// ȭ�� ���� ���� ON/OFF �ð����� ����Ѵ�									
// 			// -> st_lamp.mn_lamp_wait_time : ���� ON/OFF ���� �ð� ���� ����			
// 			// ************************************************************************
// 			m_lWaitTime[1] = GetCurrentTime();
// 			m_lWaitTime[2] = m_lWaitTime[1] - m_lWaitTime[0];
// 
// 			if (m_lWaitTime[2] < 0)
// 			{
// 				m_lWaitTime[0] = GetCurrentTime();
// 				break;
// 			}
// 			if (m_lWaitTime[2] > st_lamp_info.nLampWaitTime)
// 			{
// 				m_nLampStep++;
// 			}
// 			break;
// 
// 		case 2 :
// 			// ************************************************************************
// 			//  RED Ÿ�� ���� ��� �����Ѵ�												
// 			// ************************************************************************
// 			if (st_lamp_info.nLampR[nStatus] == 1)       // RED Ÿ�� ���� ON
// 			{
// 				FAS_IO.set_out_bit(st_io_info.o_TwLamp[2], IO_ON);
// 			}
// 			else if (st_lamp_info.nLampR[nStatus] == 0)  // RED Ÿ�� ���� OFF
// 			{
// 				FAS_IO.set_out_bit(st_io_info.o_TwLamp[2], IO_OFF);
// 			}
// 			else if (st_lamp_info.nLampR[nStatus] == 2)  // RED Ÿ�� ���� FLICK
// 			{
// 				FAS_IO.set_out_bit(st_io_info.o_TwLamp[2], IO_OFF);
// 			}
// 
// 			if (st_handler_info.nRunStatus == dRUN)
// 			{
// 				if (st_work_info.nTowerStatus[0] == YES || st_work_info.nTowerStatus[1] == YES || 
// 					st_work_info.nTowerStatus[2] == YES || st_work_info.nTowerStatus[3] == YES)
// 				{
// 					FAS_IO.set_out_bit(st_io_info.o_TwLamp[2], IO_OFF);
// 				}
// 			}
// 			// ************************************************************************
// 
// 			// ************************************************************************
// 			//  GREEN Ÿ�� ���� ��� �����Ѵ�											
// 			// ************************************************************************
// 			if (st_lamp_info.nLampG[nStatus] == 1)       // GREEN Ÿ�� ���� ON
// 			{
// 				FAS_IO.set_out_bit(st_io_info.o_TwLamp[0], IO_ON);
// 			}
// 			else if (st_lamp_info.nLampG[nStatus] == 0)  // GREEN Ÿ�� ���� OFF
// 			{
// 				FAS_IO.set_out_bit(st_io_info.o_TwLamp[0], IO_OFF);
// 			}
// 			else if (st_lamp_info.nLampG[nStatus] == 2)  // GREEN Ÿ�� ���� FLICK
// 			{
// 				FAS_IO.set_out_bit(st_io_info.o_TwLamp[0], IO_OFF);
// 			}
// 			// ************************************************************************
// 
// 			// ************************************************************************
// 			//  YELLOW Ÿ�� ���� ��� �����Ѵ�											
// 			// ************************************************************************
// 			if (st_lamp_info.nLampY[nStatus] == 1)       // YELLOW Ÿ�� ���� ON
// 			{
// 				FAS_IO.set_out_bit(st_io_info.o_TwLamp[1], IO_ON);
// 			}
// 			else if (st_lamp_info.nLampY[nStatus] == 0)  // YELLOW Ÿ�� ���� OFF
// 			{
// 				FAS_IO.set_out_bit(st_io_info.o_TwLamp[1], IO_OFF);
// 			}
// 			else if (st_lamp_info.nLampY[nStatus] == 2)  // YELLOW Ÿ�� ���� FLICK
// 			{
// 				FAS_IO.set_out_bit(st_io_info.o_TwLamp[1], IO_OFF);
// 			}
// 			// ************************************************************************
// 
// 			// ************************************************************************
// 			// Ÿ�� ���� ON/OFF ���� �����ϱ� ���Ͽ� ���� �ð� ��´�					
// 			// -> ȭ�� ���� �ð���ŭ ON/OFF �ð� �����Ѵ�								
// 			// ************************************************************************
// 			m_lWaitTime[0] = GetCurrentTime();
// 			m_nLampStep++;
// 			// ************************************************************************
// 			break;
// 		case 3 :
// 			// ************************************************************************
// 			// ȭ�� ���� ���� ON/OFF �ð����� ����Ѵ�									
// 			// -> st_lamp.mn_lamp_wait_time : ���� ON/OFF ���� �ð� ���� ����			
// 			// ************************************************************************
// 			m_lWaitTime[1] = GetCurrentTime();
// 			m_lWaitTime[2] = m_lWaitTime[1] - m_lWaitTime[0];
// 
// 			if (m_lWaitTime[2] < 0)
// 			{
// 				m_lWaitTime[0] = GetCurrentTime();
// 				break;
// 			}
// 
// 			if (m_lWaitTime[2] > st_lamp_info.nLampWaitTime)
// 			{
// 				m_nLampStep = 0;
// 			}
// 			break;
// 		default:
// 			m_nLampStep = 0;
// 			break;
// 	}
}
