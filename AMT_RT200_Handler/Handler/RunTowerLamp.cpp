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
// 쓰레드 동작
//==================================================================//
void CRunTowerLamp::OnThreadRun() 
{
	//==============================================================//
	// I/O 상태 디버깅 화면 출력된 경우 리턴
	// : 리턴하지 않으면 I/O 화면에서 타워 팸프 컨트롤이 불가능
	// : 이 쓰레드에서 계속해서 타워 램프 출력 내보내기 때문
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
			//  RED 타워 램프 출력 제어한다												
			// ====================================================================================
			FAS_IO.set_out_bit(st_io_info.o_TwRedLamp, IO_ON);
			// ====================================================================================

			// ====================================================================================
			//  YELLOW 타워 램프 출력 제어한다											
			// ====================================================================================
			FAS_IO.set_out_bit(st_io_info.o_TwYellowLamp, IO_OFF);
			// ====================================================================================

			// ====================================================================================
			//  GREEN 타워 램프 출력 제어한다
			// ====================================================================================
			FAS_IO.set_out_bit(st_io_info.o_TwGreenLamp, IO_OFF);
			// ====================================================================================
			break;
		case 1:
			// ====================================================================================
			//  RED 타워 램프 출력 제어한다												
			// ====================================================================================
			FAS_IO.set_out_bit(st_io_info.o_TwRedLamp, IO_OFF);
			// ====================================================================================

			// ====================================================================================
			//  YELLOW 타워 램프 출력 제어한다											
			// ====================================================================================
			FAS_IO.set_out_bit(st_io_info.o_TwYellowLamp, IO_ON);
			// ====================================================================================

			// ====================================================================================
			//  GREEN 타워 램프 출력 제어한다
			// ====================================================================================
			FAS_IO.set_out_bit(st_io_info.o_TwGreenLamp, IO_OFF);
			// ====================================================================================
			break;
		case 2:
			// ====================================================================================
			//  RED 타워 램프 출력 제어한다		
			// ====================================================================================
			FAS_IO.set_out_bit(st_io_info.o_TwRedLamp, IO_OFF);
			// ====================================================================================

			// ====================================================================================
			//  YELLOW 타워 램프 출력 제어한다											
			// ====================================================================================
			FAS_IO.set_out_bit(st_io_info.o_TwYellowLamp, IO_OFF);
			// ====================================================================================

			// ====================================================================================
			//  GREEN 타워 램프 출력 제어한다
			// ====================================================================================
			FAS_IO.set_out_bit(st_io_info.o_TwGreenLamp, IO_ON);
			// ====================================================================================
			break;
		default:
			// ====================================================================================
			//  RED 타워 램프 출력 제어한다												
			// ====================================================================================
			FAS_IO.set_out_bit(st_io_info.o_TwRedLamp, IO_OFF);
			// ====================================================================================

			// ====================================================================================
			//  YELLOW 타워 램프 출력 제어한다											
			// ====================================================================================
			FAS_IO.set_out_bit(st_io_info.o_TwYellowLamp, IO_OFF);
			// ====================================================================================

			// ====================================================================================
			//  GREEN 타워 램프 출력 제어한다
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
			// 타워 램프 ON/OFF 간격 제어하기 위하여 현재 시간 얻는다					
			// -> 화면 설정 시간만큼 ON/OFF 시간 유지한다								
			// ========================================================================
			m_lWaitTime[0] = GetCurrentTime();
			m_nLampStep++;
			// ========================================================================
			break;
		case 1 :
			// ========================================================================
			// 화면 설정 램프 ON/OFF 시간동안 대기한다									
			// -> st_maintenance.i_lamp_wait_time : 램프 ON/OFF 유지 시간 저장 변수			
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
			// 타워 램프 ON/OFF 간격 제어하기 위하여 현재 시간 얻는다					
			// -> 화면 설정 시간만큼 ON/OFF 시간 유지한다								
			// ========================================================================
			m_lWaitTime[0] = GetCurrentTime();
			m_nLampStep++;
			// ========================================================================
			break;
		case 3 :
			// ========================================================================
			// 화면 설정 램프 ON/OFF 시간동안 대기한다									
			// -> st_maintenance.i_lamp_wait_time : 램프 ON/OFF 유지 시간 저장 변수			
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
			// 타워 램프 ON/OFF 간격 제어하기 위하여 현재 시간 얻는다					
			// -> 화면 설정 시간만큼 ON/OFF 시간 유지한다								
			// ========================================================================
			m_lWaitTime[0] = GetCurrentTime();
			m_nLampStep++;
			// ========================================================================
			break;
		case 5:
			// ========================================================================
			// 화면 설정 램프 ON/OFF 시간동안 대기한다									
			// -> st_maintenance.i_lamp_wait_time : 램프 ON/OFF 유지 시간 저장 변수			
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
			// 타워 램프 ON/OFF 간격 제어하기 위하여 현재 시간 얻는다					
			// -> 화면 설정 시간만큼 ON/OFF 시간 유지한다								
			// ========================================================================
			m_lWaitTime[0] = GetCurrentTime();
			m_nLampStep++;
			// ========================================================================
			break;
		case 7:
			// ========================================================================
			// 화면 설정 램프 ON/OFF 시간동안 대기한다								   
			// -> st_maintenance.i_lamp_wait_time : 램프 ON/OFF 유지 시간 저장 변수			
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
			// 타워 램프 ON/OFF 간격 제어하기 위하여 현재 시간 얻는다					
			// -> 화면 설정 시간만큼 ON/OFF 시간 유지한다								
			// ========================================================================
			m_lWaitTime[0] = GetCurrentTime();
			m_nLampStep++;
			// ========================================================================
			break;
		case 9:
			// ========================================================================
			// 화면 설정 램프 ON/OFF 시간동안 대기한다									
			// -> st_maintenance.i_lamp_wait_time : 램프 ON/OFF 유지 시간 저장 변수			
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
			// 타워 램프 ON/OFF 간격 제어하기 위하여 현재 시간 얻는다					
			// -> 화면 설정 시간만큼 ON/OFF 시간 유지한다								
			// ========================================================================
			m_lWaitTime[0] = GetCurrentTime();
			m_nLampStep++;
			// ========================================================================
			break;
		case 11:
			// ========================================================================
			// 화면 설정 램프 ON/OFF 시간동안 대기한다									
			// -> st_maintenance.i_lamp_wait_time : 램프 ON/OFF 유지 시간 저장 변수			
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
			// 타워 램프 ON/OFF 간격 제어하기 위하여 현재 시간 얻는다					
			// -> 화면 설정 시간만큼 ON/OFF 시간 유지한다								
			// ========================================================================
			m_lWaitTime[0] = GetCurrentTime();
			m_nLampStep++;
			// ========================================================================
			break;
		case 13:
			// ========================================================================
			// 화면 설정 램프 ON/OFF 시간동안 대기한다									
			// -> st_maintenance.i_lamp_wait_time : 램프 ON/OFF 유지 시간 저장 변수			
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
			// 타워 램프 ON/OFF 간격 제어하기 위하여 현재 시간 얻는다					
			// -> 화면 설정 시간만큼 ON/OFF 시간 유지한다								
			// ========================================================================
			m_lWaitTime[0] = GetCurrentTime();
			m_nLampStep++;
			// ========================================================================
			break;
		case 15:
			// ========================================================================
			// 화면 설정 램프 ON/OFF 시간동안 대기한다									
			// -> st_maintenance.i_lamp_wait_time : 램프 ON/OFF 유지 시간 저장 변수			
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
// 			//  RED 타워 램프 출력 제어한다												
// 			// ************************************************************************
// 			if (st_lamp_info.nLampR[nStatus] == 1)       // RED 타워 램프 ON
// 			{
// 				FAS_IO.set_out_bit(st_io_info.o_TwLamp[2], IO_ON);
// 			}
// 			else if (st_lamp_info.nLampR[nStatus] == 0)  // RED 타워 램프 OFF
// 			{
// 				FAS_IO.set_out_bit(st_io_info.o_TwLamp[2], IO_OFF);
// 			}
// 			else if (st_lamp_info.nLampR[nStatus] == 2)  // RED 타워 램프 FLICK
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
// 			//  GREEN 타워 램프 출력 제어한다													
// 			// ************************************************************************
// 			if (st_lamp_info.nLampG[nStatus] == 1)       // GREEN 타워 램프 ON
// 			{
// 				FAS_IO.set_out_bit(st_io_info.o_TwLamp[0], IO_ON);
// 			}
// 			else if (st_lamp_info.nLampG[nStatus] == 0)  // GREEN 타워 램프 OFF
// 			{
// 				FAS_IO.set_out_bit(st_io_info.o_TwLamp[0], IO_OFF);
// 			}
// 			else if (st_lamp_info.nLampG[nStatus] == 2)  // GREEN 타워 램프 FLICK
// 			{
// 				FAS_IO.set_out_bit(st_io_info.o_TwLamp[0], IO_ON);
// 			}
// 			// ************************************************************************
// 
// 			// ************************************************************************
// 			//  YELLOW 타워 램프 출력 제어한다											
// 			// ************************************************************************
// 			if (st_lamp_info.nLampY[nStatus] == 1)       // YELLOW 타워 램프 ON
// 			{
// 				FAS_IO.set_out_bit(st_io_info.o_TwLamp[1], IO_ON);
// 			}
// 			else if (st_lamp_info.nLampY[nStatus] == 0)  // YELLOW 타워 램프 OFF
// 			{
// 				FAS_IO.set_out_bit(st_io_info.o_TwLamp[1], IO_OFF);
// 			}
// 			else if (st_lamp_info.nLampY[nStatus] == 2)  // YELLOW 타워 램프 FLICK
// 			{
// 				FAS_IO.set_out_bit(st_io_info.o_TwLamp[1], IO_ON);
// 			}
// 			// ************************************************************************
// 
// 			// ************************************************************************
// 			// 타워 램프 ON/OFF 간격 제어하기 위하여 현재 시간 얻는다					
// 			// -> 화면 설정 시간만큼 ON/OFF 시간 유지한다								
// 			// ************************************************************************
// 			m_lWaitTime[0] = GetCurrentTime();
// 			m_nLampStep++;
// 			// ************************************************************************
// 			break;
// 
// 		case 1 :
// 			// ************************************************************************
// 			// 화면 설정 램프 ON/OFF 시간동안 대기한다									
// 			// -> st_lamp.mn_lamp_wait_time : 램프 ON/OFF 유지 시간 저장 변수			
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
// 			//  RED 타워 램프 출력 제어한다												
// 			// ************************************************************************
// 			if (st_lamp_info.nLampR[nStatus] == 1)       // RED 타워 램프 ON
// 			{
// 				FAS_IO.set_out_bit(st_io_info.o_TwLamp[2], IO_ON);
// 			}
// 			else if (st_lamp_info.nLampR[nStatus] == 0)  // RED 타워 램프 OFF
// 			{
// 				FAS_IO.set_out_bit(st_io_info.o_TwLamp[2], IO_OFF);
// 			}
// 			else if (st_lamp_info.nLampR[nStatus] == 2)  // RED 타워 램프 FLICK
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
// 			//  GREEN 타워 램프 출력 제어한다											
// 			// ************************************************************************
// 			if (st_lamp_info.nLampG[nStatus] == 1)       // GREEN 타워 램프 ON
// 			{
// 				FAS_IO.set_out_bit(st_io_info.o_TwLamp[0], IO_ON);
// 			}
// 			else if (st_lamp_info.nLampG[nStatus] == 0)  // GREEN 타워 램프 OFF
// 			{
// 				FAS_IO.set_out_bit(st_io_info.o_TwLamp[0], IO_OFF);
// 			}
// 			else if (st_lamp_info.nLampG[nStatus] == 2)  // GREEN 타워 램프 FLICK
// 			{
// 				FAS_IO.set_out_bit(st_io_info.o_TwLamp[0], IO_OFF);
// 			}
// 			// ************************************************************************
// 
// 			// ************************************************************************
// 			//  YELLOW 타워 램프 출력 제어한다											
// 			// ************************************************************************
// 			if (st_lamp_info.nLampY[nStatus] == 1)       // YELLOW 타워 램프 ON
// 			{
// 				FAS_IO.set_out_bit(st_io_info.o_TwLamp[1], IO_ON);
// 			}
// 			else if (st_lamp_info.nLampY[nStatus] == 0)  // YELLOW 타워 램프 OFF
// 			{
// 				FAS_IO.set_out_bit(st_io_info.o_TwLamp[1], IO_OFF);
// 			}
// 			else if (st_lamp_info.nLampY[nStatus] == 2)  // YELLOW 타워 램프 FLICK
// 			{
// 				FAS_IO.set_out_bit(st_io_info.o_TwLamp[1], IO_OFF);
// 			}
// 			// ************************************************************************
// 
// 			// ************************************************************************
// 			// 타워 램프 ON/OFF 간격 제어하기 위하여 현재 시간 얻는다					
// 			// -> 화면 설정 시간만큼 ON/OFF 시간 유지한다								
// 			// ************************************************************************
// 			m_lWaitTime[0] = GetCurrentTime();
// 			m_nLampStep++;
// 			// ************************************************************************
// 			break;
// 		case 3 :
// 			// ************************************************************************
// 			// 화면 설정 램프 ON/OFF 시간동안 대기한다									
// 			// -> st_lamp.mn_lamp_wait_time : 램프 ON/OFF 유지 시간 저장 변수			
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
