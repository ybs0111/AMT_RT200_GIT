#include "StdAfx.h"
#include "RunConveyor.h"
#include "variable.h"
//#include "CV_test.h"
#include "FastechPublic_IO.h"
#include "CtlBd_Library.h"
#include "Ctlbd_Variable.h"

#include "AlgMemory.h"

CRunConveyor clsRunConveyor;

CRunConveyor::CRunConveyor(void)
{
	for (int i = 0; i<3; i++)
	{
		m_nRunStep[i]		= 0;
	}
	m_nInitStep = 0;
	m_nSmemaStep = 0;
	m_nRobot_Conv = M_TURN_CONV_PITCH;
	st_sync_info.nInitConv_pitch = NO;
}


CRunConveyor::~CRunConveyor(void)
{
}

void CRunConveyor::OnThreadRUN()
{
	Smema_Front();
	Smema_Rear();

 	switch(st_handler_info.nRunStatus)
 	{
	case dINIT:
		OnRunInit();
		break;
			
	case dRUN:
		OnFrontConvRunMove();
		OnTurnConvMove();
		OnRearConvMove();
		break;

	case dSTOP:
		FAS_IO.set_out_bit(st_io_info.o_Front_LabelReq,IO_OFF);
		break;

	case dWARNING:
		FAS_IO.set_out_bit(st_io_info.o_Front_LabelReq,IO_OFF);
		break;

	case dSELFCHECK:
		break;

	case dREINSTATE:
		break;

	case dRECOVERY:
		break;
 	}
}
int CRunConveyor::OnConvPcvCheck()
{

	int nRet[5] = {0,};
	
	nRet[0] = FAS_IO.get_in_bit(st_io_info.i_InConvInChk,	IO_OFF);//ps100 //트레이가 장비에 진입시 감지 됨
	nRet[1] = FAS_IO.get_in_bit(st_io_info.i_InConvPosChk,	IO_OFF);//ps101 //트레이가 장비에 진입시 감지 됨
	nRet[2] = FAS_IO.get_in_bit(st_io_info.i_OutInConvPosChk,	IO_OFF);//ps100 //트레이가 장비에 진입시 감지 됨
	nRet[3] = FAS_IO.get_in_bit(st_io_info.i_TurnConvPosChk,	IO_OFF);//ps101 //트레이가 장비에 진입시 감지 됨
	nRet[4] = FAS_IO.get_in_bit(st_io_info.i_OutConvChk,	IO_OFF);//ps100 //트레이가 장비에 진입시 감지 됨

	if(nRet[0] == IO_OFF && nRet[1] == IO_OFF && nRet[2] == IO_OFF && nRet[3] == IO_OFF && nRet[4] == IO_OFF )
	{
		return RET_GOOD;
		//m_nInitStep = 110;
	}
	else
	{//트레이가 없는지 확인
	//	m_nInitStep = 200;
		//501080 1 A "IN CONV Sensor Off Check Error."
		//501081 1 A "IN CONV Sensor On Check Error."
		//501090 1 A "IN CONV Position Off Check Error."
		//501091 1 A "IN CONV Position On Check Error."

		if	   (nRet[0] == IO_ON) m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_InConvInChk, IO_OFF); //
		else if(nRet[1] == IO_ON) m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_InConvPosChk, IO_OFF);
		else if(nRet[2] == IO_ON) m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_OutInConvPosChk, IO_OFF); //
		else if(nRet[3] == IO_ON) m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_TurnConvPosChk, IO_OFF);
		else   m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_OutConvChk, IO_OFF);

		CTL_Lib.Alarm_Error_Occurrence(200, dWARNING, m_strAlarmCode);		
	//	m_nInitStep = 9000;
	}
	return RET_ERROR;
}

void CRunConveyor::OnPcbDataReset()
{
	for (int i = 0; i<2; i++)
	{
		for (int j = 0; j< PCB_CNT; j++)
		{
			st_Pcb_info.strPcbSerial[i][j] =_T("");
		}
	}
}
void CRunConveyor::Smema_Front()
{
	int nRet[2] = {0,};
	CString strTemp;	

	switch(m_nSmemaStep)
	{
		case 0:
			//초기화
			FAS_IO.set_out_bit(st_io_info.o_Front_LabelReq,IO_OFF);
			FAS_IO.set_out_bit(st_io_info.o_Front_LabelComplete,IO_OFF);
			st_sync_info.nSmema_Front = CTL_NONE;
			m_nSmemaStep = 100;
			break;

		case 100:			
			nRet[0] = FAS_IO.get_in_bit(st_io_info.i_InConvInChk,IO_ON);
			nRet[1] = FAS_IO.get_in_bit(st_io_info.i_InConvPosChk,IO_ON);
			if( nRet[0] == IO_ON && nRet[1] == IO_ON )
			{
				m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_InConvInChk, IO_ON); 
				CTL_Lib.Alarm_Error_Occurrence(304, dWARNING, m_strAlarmCode);				
			}
			else
			{
				m_nSmemaStep = 110;
			}
			break;

		case 110:
			nRet[0] = FAS_IO.get_in_bit(st_io_info.i_InConvInChk,IO_ON);
			nRet[1] = FAS_IO.get_in_bit(st_io_info.i_InConvPosChk,IO_ON);	
			if( nRet[0] == IO_ON || nRet[1] == IO_ON )
			{
				m_lWait_Smema[0] = GetCurrentTime();
				m_nSmemaStep = 200;
			}
			else
			{
				m_nSmemaStep = 1000;
			}
			break;

		case 200:
			m_lWait_Smema[1] = GetCurrentTime();
			m_lWait_Smema[2] = m_lWait_Smema[1] - m_lWait_Smema[0];
			if (m_lWait_Smema[2] <= 0)  m_lWait_Smema[0] = GetCurrentTime();

			nRet[0] = FAS_IO.get_in_bit(st_io_info.i_InConvInChk,IO_ON);
			nRet[1] = FAS_IO.get_in_bit(st_io_info.i_InConvPosChk,IO_ON);	
			if( nRet[0] == IO_ON || nRet[1] == IO_ON )
			{
				if (m_lWait_Smema[2] > 30 )
				{
					m_nSmemaStep = 1100;
				}
			}
			else
			{
				m_nSmemaStep = 1000;
			}
			break;

		case 1000:
			if (st_sync_info.nSmema_Tray_Input_Req == CONV_REQ)
			{
				FAS_IO.set_out_bit(st_io_info.o_Front_LabelReq,IO_ON);
				m_lWait_Smema[0] = GetCurrentTime(); //kwlee 2017.0313
				m_nSmemaStep = 1100;
			}
			break;

		case 1100:
			nRet[0] = FAS_IO.get_in_bit(st_io_info.i_InConvInChk,IO_ON);
			nRet[1] = FAS_IO.get_in_bit(st_io_info.i_InConvPosChk,IO_ON);	
			if( nRet[0] == IO_ON || nRet[1] == IO_ON || st_sync_info.nSmema_Front == CTL_READY)
			{
				m_lWait_Smema[0] = GetCurrentTime();
				m_nSmemaStep = 1200;
			}
			break;

		case 1200:
			m_lWait_Smema[1] = GetCurrentTime();
			m_lWait_Smema[2] = m_lWait_Smema[1] - m_lWait_Smema[0];
			if (m_lWait_Smema[2] <= 0)  m_lWait_Smema[0] = GetCurrentTime();
			nRet[0] = FAS_IO.get_in_bit(st_io_info.i_InConvInChk,IO_ON);
			nRet[1] = FAS_IO.get_in_bit(st_io_info.i_InConvPosChk,IO_ON);	
			if( nRet[0] == IO_ON || nRet[1] == IO_ON || st_sync_info.nSmema_Front == CTL_READY)
			{
				if (m_lWait_Smema[2] > 30)
				{
					FAS_IO.set_out_bit(st_io_info.o_Front_LabelReq,IO_OFF);
					m_nSmemaStep = 2000;
				}
			}
			else
			{
				m_nSmemaStep = 1100;
			}
			break;

		case 2000:
	       /////check check



	
			
			if (m_lWait_Smema[2] > st_wait_info.nLimitWaitTime[WAIT_CONV_REQ])
			{
				//FAS_IO.set_out_bit(st_io_info.o_Front_LabelReq,IO_OFF); //kwlee 2017.0315
				st_sync_info.nSmema_Tray_Input_Req = CONV_READY;
				m_lWait_Smema[0] = GetCurrentTime();
				m_nSmemaStep = 300;
			}



		case 210:
			//보낼 준비가 되면 Ready On
			m_lWait_Smema[1] = GetCurrentTime();
			m_lWait_Smema[2] = m_lWait_Smema[1] - m_lWait_Smema[0];
			if (m_lWait_Smema[2] < 0)
			{
				m_lWait_Smema[0] = GetCurrentTime();
			}

			//nRet[0] = FAS_IO.get_in_bit(st_io_info.i_FrontReady,IO_ON);
			//kwlee 2017.0315
			nRet[0] = FAS_IO.get_in_bit(st_io_info.i_InConvInChk,IO_ON);
			if(nRet[0] == IO_OFF)
			{
				m_nSmemaStep = 200;
			}
			else if( nRet[0] == IO_ON  || st_sync_info.nSmema_Front == CTL_READY)
			{
				if (m_lWait_Smema[2] > st_wait_info.nLimitWaitTime[WAIT_CONV_REQ])
				{
					//FAS_IO.set_out_bit(st_io_info.o_Front_LabelReq,IO_OFF); //kwlee 2017.0315
					st_sync_info.nSmema_Tray_Input_Req = CONV_READY;
					m_lWait_Smema[0] = GetCurrentTime();
					m_nSmemaStep = 300;
				}
			}
// 			if( st_basic_info.nModeDevice == WITHOUT_DVC)
// 			{
// 				st_sync_info.nSmema_Tray_Input_Req = CONV_READY;
// 				m_lWait_Smema[0] = GetCurrentTime();
// 				m_nSmemaStep = 300;
// 			}
			break;

		case 300:
			m_lWait_Smema[1] = GetCurrentTime();
			m_lWait_Smema[2] = m_lWait_Smema[1] - m_lWait_Smema[0];
			if (m_lWait_Smema[2] < 0)
			{
				m_lWait_Smema[0] = GetCurrentTime();
			}
			//nRet[0] = FAS_IO.get_in_bit(st_io_info.i_FrontReady,IO_OFF);
			//kwlee 2017.0315
			nRet[0] = FAS_IO.get_in_bit(st_io_info.i_InConvInChk,IO_OFF);
			if (nRet[0] == IO_ON)
			{
				m_nSmemaStep = 200;
			}
			else if (/*st_basic_info.nModeDevice == WITHOUT_DVC || */
				( nRet[0] == IO_OFF && FAS_IO.get_in_bit(st_io_info.i_InConvPosChk,IO_ON) == IO_ON || st_sync_info.nSmema_Front == CTL_COMPLETE ) )
			{
				if (m_lWait_Smema[2] > st_wait_info.nLimitWaitTime[WAIT_CONV_REQ])
				{
					//FAS_IO.set_out_bit(st_io_info.o_Front_LabelReq,IO_OFF); //kwlee 2017.0315
					m_nSmemaStep = 400;
				}
			}
			break;

		case 400:
			//nRet[0] = FAS_IO.get_in_bit(st_io_info.i_FrontReady,IO_OFF);
			//kwlee 2017.0315
			nRet[0] = FAS_IO.get_in_bit(st_io_info.i_InConvPosChk,IO_OFF);
			if( nRet[0] == IO_OFF)
			{
				m_lWait_Smema[0] = GetCurrentTime();
				m_nSmemaStep = 410;
			}
			//kwlee 2017.0313
// 			if( st_basic_info.nModeDevice == WITHOUT_DVC)
// 			{
// 				m_lWait_Smema[0] = GetCurrentTime();
// 				m_nSmemaStep = 410;
// 			}
			break;

		case 410:
			m_lWait_Smema[1] = GetCurrentTime();
			m_lWait_Smema[2] = m_lWait_Smema[1] - m_lWait_Smema[0];
			if (m_lWait_Smema[2] < 0)
			{
				m_lWait_Smema[0] = GetCurrentTime();
			}

			//nRet[0] = FAS_IO.get_in_bit(st_io_info.i_FrontReady,IO_OFF);
			//kwlee 2017.0315
			nRet[0] = FAS_IO.get_in_bit(st_io_info.i_InConvPosChk,IO_OFF);
			if(nRet[0] == IO_ON)
			{
				m_nSmemaStep = 400;
			}
			else if( nRet[0] == IO_OFF /*|| st_basic_info.nModeDevice == WITHOUT_DVC*/)
			{
				if (m_lWait_Smema[2] > st_wait_info.nLimitWaitTime[WAIT_CONV_REQ])
				{
					m_nSmemaStep = 0;
				}
			}
			break;

		default:
			if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
			{ 
				strTemp.Format(_T("abnomal case Last Step : %d"),m_nSmemaStep);
				clsMem.OnNormalMessageWrite(strTemp);
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
			break;
	}
}
void  CRunConveyor::Smema_Rear()
{
	int nRet[2];
	CString strTemp;

	switch(m_nRearSmemaStep)
	{
	case 0:
		FAS_IO.set_out_bit(st_io_info.o_Rear_Label_Ready,IO_OFF);
		FAS_IO.set_out_bit(st_io_info.o_Rear_Label_Complete,IO_OFF);

		m_nRearSmemaStep = 100;
		break;

	case 100:
		if (st_sync_info.nSmema_Tray_Output_Req == CONV_REQ )
		{
			nRet[0] = FAS_IO.get_in_bit(st_io_info.i_RearReq,IO_ON);
			//Rear 받을 준비
			if (nRet[0] == IO_ON || st_sync_info.nSmema_Rear == CTL_REQ)
			{
				m_lWait_Smema[0] = GetCurrentTime();
				m_nRearSmemaStep = 200;
			}
		}
		break;

	case 200:
		m_lWait_Smema[1] = GetCurrentTime();
		m_lWait_Smema[2] = m_lWait_Smema[1] - m_lWait_Smema[0];

		if (m_lWait_Smema[2] > 1000)
		{
			FAS_IO.set_out_bit(st_io_info.o_Rear_Label_Ready,IO_ON);
			st_sync_info.nSmema_Tray_Output_Req = CONV_READY;
			m_lWait_Smema[0] = GetCurrentTime();
			
			m_nRearSmemaStep = 300;
		}
		else if (m_lWait_Smema[2] < 0)
		{
			m_lWait_Smema[0] = GetCurrentTime();
		}
		break;

	case 300:
		m_lWait_Smema[1] = GetCurrentTime();
		m_lWait_Smema[2] = m_lWait_Smema[1] - m_lWait_Smema[0];

		if (m_lWait_Smema[2] < st_wait_info.nLimitWaitTime[WAIT_CONV_REQ])
		{
			if (st_sync_info.nSmema_Tray_Output_Req == CONV_CLR)
			{
				FAS_IO.set_out_bit(st_io_info.o_Rear_Label_Complete,IO_ON);
				m_lWait_Smema[0] = GetCurrentTime();
				m_nRearSmemaStep = 400;
			}
		}
		break;

	case 400:
		m_lWait_Smema[1] = GetCurrentTime();
		m_lWait_Smema[2] = m_lWait_Smema[1] - m_lWait_Smema[0];

		if (m_lWait_Smema[2] < st_wait_info.nLimitWaitTime[WAIT_CONV_REQ])
		{
			nRet[0] = FAS_IO.get_in_bit(st_io_info.i_RearComplete,IO_ON);

			if (nRet[0] == IO_ON || st_sync_info.nSmema_Rear == CTL_COMPLETE)
			{
				FAS_IO.set_out_bit(st_io_info.o_Rear_Label_Ready,IO_OFF);
				m_lWait_Smema[0] = GetCurrentTime();
				m_nRearSmemaStep = 500;
			}
		}	
		break;

	case 500:
		m_lWait_Smema[1] = GetCurrentTime();
		m_lWait_Smema[2] = m_lWait_Smema[1] - m_lWait_Smema[0];

		if (m_lWait_Smema[2] > 500)
		{
			FAS_IO.set_out_bit(st_io_info.o_Rear_Label_Complete,IO_OFF);
			m_lWait_Smema[0] = GetCurrentTime();
			m_nRearSmemaStep = 100;
		}
		break;		

	default:
		if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
		{ 
			strTemp.Format(_T("abnomal case Last Step : %d"),m_nRearSmemaStep);
			clsMem.OnNormalMessageWrite(strTemp);
			st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}
		break;
	}
}

int CRunConveyor::OnConvWaitTime(int nConv, int nWaitMode)
{
	m_dwConveyorWaitTime[nConv][1] = GetCurrentTime();
	m_dwConveyorWaitTime[nConv][2] = m_dwConveyorWaitTime[nConv][1] - m_dwConveyorWaitTime[nConv][0];

	if (m_dwConveyorWaitTime[nConv][2] <= 0)
	{
		m_dwConveyorWaitTime[nConv][0] = GetCurrentTime();
	}

	if (m_dwConveyorWaitTime[nConv][2] > (DWORD)st_wait_info.nLimitWaitTime[nWaitMode])
	{
		return RET_ERROR;
	}

	return RET_PROCEED;
	
}
void CRunConveyor::OnSetInConvStopperUpDn(int nMode, int nUpDn)
{
	if (nMode > 1) return;

	m_bWStopperFlag[nMode]			= false;
	m_dwWStopperWaitTime[nMode][0]	= GetCurrentTime();

	if (nUpDn == IO_ON)
	{
		FAS_IO.set_out_bit(st_io_info.o_InConvPosStopperUp, nUpDn); //S0105 Up
		FAS_IO.set_out_bit(st_io_info.o_InConvPosStopperDw, !nUpDn);	
	}
	else
	{
		FAS_IO.set_out_bit(st_io_info.o_InConvPosStopperDw, !nUpDn); //S0106 Dw
		FAS_IO.set_out_bit(st_io_info.o_InConvPosStopperUp, nUpDn);
	}
	

}
int CRunConveyor::OnGetInConvStopperUpDn(int nMode, int nUpDn)
{
	int nWaitTime = WAIT_IN_CONV_STOPPER;

	if (nUpDn == IO_OFF)
	{
		if (m_bWStopperFlag[nMode] == false &&
			FAS_IO.get_in_bit(st_io_info.i_InConvPosUpChk,	IO_OFF)	== IO_OFF )
		{
			m_bWStopperFlag[nMode]		= true;
			m_dwWStopperWaitTime[nMode][0]	= GetCurrentTime();
		}
		else if (m_bWStopperFlag[nMode] == true &&
			FAS_IO.get_in_bit(st_io_info.i_InConvPosUpChk,	IO_OFF)	== IO_OFF )
		{
			m_dwWStopperWaitTime[nMode][1] = GetCurrentTime();
			m_dwWStopperWaitTime[nMode][2] = m_dwWStopperWaitTime[nMode][1] - m_dwWStopperWaitTime[nMode][0];

			if (m_dwWStopperWaitTime[nMode][2] <= 0)
			{
				m_dwWStopperWaitTime[nMode][0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwWStopperWaitTime[nMode][2] > (DWORD)st_wait_info.nOnWaitTime[nWaitTime])
			{
				return RET_GOOD;
			}
		}
		else
		{
			m_dwWStopperWaitTime[nMode][1] = GetCurrentTime();
			m_dwWStopperWaitTime[nMode][2] = m_dwWStopperWaitTime[nMode][1] - m_dwWStopperWaitTime[nMode][0];

			if (m_dwWStopperWaitTime[nMode][2] <= 0)
			{
				m_dwWStopperWaitTime[nMode][0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwWStopperWaitTime[nMode][2] > (DWORD)st_wait_info.nLimitWaitTime[nWaitTime])
			{

				m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_InConvPosUpChk, nUpDn); 
				//CTL_Lib.Alarm_Error_Occurrence(304, dWARNING, strAlarm);

				return RET_ERROR;
			}
		}
	}
	else
	{
		if (m_bWStopperFlag[nMode] == false &&
			FAS_IO.get_in_bit(st_io_info.i_InConvPosUpChk,	IO_ON)	== IO_ON )
		{
			m_bWStopperFlag[nMode]			= true;
			m_dwWStopperWaitTime[nMode][0]	= GetCurrentTime();
		}
		else if (m_bWStopperFlag[nMode] == true &&
			FAS_IO.get_in_bit(st_io_info.i_InConvPosUpChk,	IO_ON)	== IO_ON )
		{
			m_dwWStopperWaitTime[nMode][1]	= GetCurrentTime();
			m_dwWStopperWaitTime[nMode][2]	= m_dwWStopperWaitTime[nMode][1] - m_dwWStopperWaitTime[nMode][0];

			if (m_dwWStopperWaitTime[nMode][2] <= 0)
			{
				m_dwWStopperWaitTime[nMode][0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if(m_dwWStopperWaitTime[nMode][2] > (DWORD)st_wait_info.nOnWaitTime[nWaitTime])
			{
				return RET_GOOD;
			}
		}
		else
		{
			m_dwWStopperWaitTime[nMode][1]	= GetCurrentTime();
			m_dwWStopperWaitTime[nMode][2]	= m_dwWStopperWaitTime[nMode][1] - m_dwWStopperWaitTime[nMode][0];

			if (m_dwWStopperWaitTime[nMode][2] <= 0)
			{
				m_dwWStopperWaitTime[nMode][0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwWStopperWaitTime[nMode][2] > (DWORD)st_wait_info.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_InConvPosUpChk, nUpDn); 
				//CTL_Lib.Alarm_Error_Occurrence(305, dWARNING, strAlarm);
				return RET_ERROR;
			}
		}
	}

	return RET_PROCEED;

}

void CRunConveyor::OnSetTurnConvStopperUpDn(int nMode, int nUpDn)
{
	if (nMode > 1) return;

	m_bWStopperFlag[nMode]			= false;
	m_dwWStopperWaitTime[nMode][0]	= GetCurrentTime();

	if (nUpDn == IO_ON)
	{
		FAS_IO.set_out_bit(st_io_info.o_TurnPosUp, nUpDn); //s0107 Up
		FAS_IO.set_out_bit(st_io_info.o_TurnPosDw, !nUpDn);
	}
	else
	{
		FAS_IO.set_out_bit(st_io_info.o_TurnPosDw, !nUpDn); //s0200 Up
		FAS_IO.set_out_bit(st_io_info.o_TurnPosUp, nUpDn);
	}
	

}
int CRunConveyor::OnGetTurnConvStopperUpDn(int nMode, int nUpDn)
{
	int nWaitTime = WAIT_TURN_CONV_STOPPER;

	if (nUpDn == IO_OFF)
	{
		if (m_bWStopperFlag[nMode] == false &&
			FAS_IO.get_in_bit(st_io_info.i_TurnPosUpChk,	IO_OFF)	== IO_OFF &&
			FAS_IO.get_in_bit(st_io_info.i_TurnPosDwChk,	IO_ON)	== IO_ON)
		{
			m_bWStopperFlag[nMode]		= true;
			m_dwWStopperWaitTime[nMode][0]	= GetCurrentTime();
		}
		else if (m_bWStopperFlag[nMode] == true &&
			FAS_IO.get_in_bit(st_io_info.i_TurnPosUpChk,	IO_OFF)	== IO_OFF &&
			FAS_IO.get_in_bit(st_io_info.i_TurnPosDwChk,	IO_ON)	== IO_ON)
		{
			m_dwWStopperWaitTime[nMode][1] = GetCurrentTime();
			m_dwWStopperWaitTime[nMode][2] = m_dwWStopperWaitTime[nMode][1] - m_dwWStopperWaitTime[nMode][0];

			if (m_dwWStopperWaitTime[nMode][2] <= 0)
			{
				m_dwWStopperWaitTime[nMode][0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwWStopperWaitTime[nMode][2] > (DWORD)st_wait_info.nOnWaitTime[nWaitTime])
			{
				return RET_GOOD;
			}
		}
		else
		{
			m_dwWStopperWaitTime[nMode][1] = GetCurrentTime();
			m_dwWStopperWaitTime[nMode][2] = m_dwWStopperWaitTime[nMode][1] - m_dwWStopperWaitTime[nMode][0];

			if (m_dwWStopperWaitTime[nMode][2] <= 0)
			{
				m_dwWStopperWaitTime[nMode][0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwWStopperWaitTime[nMode][2] > (DWORD)st_wait_info.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_TurnPosUpChk, nUpDn); 
				//CTL_Lib.Alarm_Error_Occurrence(304, dWARNING, strAlarm);

				return RET_ERROR;
			}
		}
	}
	else
	{
		if (m_bWStopperFlag[nMode] == false &&
			FAS_IO.get_in_bit(st_io_info.i_TurnPosUpChk,	IO_ON)	== IO_ON &&
			FAS_IO.get_in_bit(st_io_info.i_TurnPosDwChk,	IO_OFF)	== IO_OFF)
		{
			m_bWStopperFlag[nMode]			= true;
			m_dwWStopperWaitTime[nMode][0]	= GetCurrentTime();
		}
		else if (m_bWStopperFlag[nMode] == true &&
			FAS_IO.get_in_bit(st_io_info.i_TurnPosUpChk,	IO_ON)	== IO_ON &&
			FAS_IO.get_in_bit(st_io_info.i_TurnPosDwChk,	IO_OFF)	== IO_OFF)
		{
			m_dwWStopperWaitTime[nMode][1]	= GetCurrentTime();
			m_dwWStopperWaitTime[nMode][2]	= m_dwWStopperWaitTime[nMode][1] - m_dwWStopperWaitTime[nMode][0];

			if (m_dwWStopperWaitTime[nMode][2] <= 0)
			{
				m_dwWStopperWaitTime[nMode][0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if(m_dwWStopperWaitTime[nMode][2] > (DWORD)st_wait_info.nOnWaitTime[nWaitTime])
			{
				return RET_GOOD;
			}
		}
		else
		{
			m_dwWStopperWaitTime[nMode][1]	= GetCurrentTime();
			m_dwWStopperWaitTime[nMode][2]	= m_dwWStopperWaitTime[nMode][1] - m_dwWStopperWaitTime[nMode][0];

			if (m_dwWStopperWaitTime[nMode][2] <= 0)
			{
				m_dwWStopperWaitTime[nMode][0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwWStopperWaitTime[nMode][2] > (DWORD)st_wait_info.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_TurnPosDwChk, nUpDn); 
				//CTL_Lib.Alarm_Error_Occurrence(305, dWARNING, strAlarm);

				return RET_ERROR;
			}
		}
	}

	return RET_PROCEED;

}

void CRunConveyor::OnSetOutConvStopperUpDn(int nMode, int nUpDn)
{
	if (nMode > 1) return;

	m_bWStopperFlag[nMode]			= false;
	m_dwWStopperWaitTime[nMode][0]	= GetCurrentTime();

	if (nUpDn == IO_ON)
	{
		FAS_IO.set_out_bit(st_io_info.o_OutPosStopperUp, nUpDn); //S0202 Up
		FAS_IO.set_out_bit(st_io_info.o_OutPosStopperDw, !nUpDn);
	}
	else
	{
		FAS_IO.set_out_bit(st_io_info.o_OutPosStopperUp, nUpDn);
		FAS_IO.set_out_bit(st_io_info.o_OutPosStopperDw, !nUpDn); //S0203 Up
	}
	

}
int CRunConveyor::OnGetOutConvStopperUpDn(int nMode, int nUpDn)
{
	int nWaitTime = WAIT_OUT_CONV_STOPPER;
	
	if (nUpDn == IO_OFF)
	{
		if (m_bWStopperFlag[nMode] == false &&
			FAS_IO.get_in_bit(st_io_info.i_OutConvPosUpChk,	IO_ON)	== IO_OFF )
		{
			m_bWStopperFlag[nMode]		= true;
			m_dwWStopperWaitTime[nMode][0]	= GetCurrentTime();
		}
		else if (m_bWStopperFlag[nMode] == true &&
			FAS_IO.get_in_bit(st_io_info.i_OutConvPosUpChk,	IO_ON)	== IO_OFF )
		{
			m_dwWStopperWaitTime[nMode][1] = GetCurrentTime();
			m_dwWStopperWaitTime[nMode][2] = m_dwWStopperWaitTime[nMode][1] - m_dwWStopperWaitTime[nMode][0];

			if (m_dwWStopperWaitTime[nMode][2] <= 0)
			{
				m_dwWStopperWaitTime[nMode][0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwWStopperWaitTime[nMode][2] > (DWORD)st_wait_info.nOnWaitTime[nWaitTime])
			{
				return RET_GOOD;
			}
		}
		else
		{
			m_dwWStopperWaitTime[nMode][1] = GetCurrentTime();
			m_dwWStopperWaitTime[nMode][2] = m_dwWStopperWaitTime[nMode][1] - m_dwWStopperWaitTime[nMode][0];

			if (m_dwWStopperWaitTime[nMode][2] <= 0)
			{
				m_dwWStopperWaitTime[nMode][0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwWStopperWaitTime[nMode][2] > (DWORD)st_wait_info.nLimitWaitTime[nWaitTime])
			{
				//502020  "OUT CONV Position Stopper Up-Sol Error."
				m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_OutConvPosUpChk, nUpDn); 
				//CTL_Lib.Alarm_Error_Occurrence(304, dWARNING, strAlarm);

				return RET_ERROR;
			}
		}
	}
	else
	{
		
		if (m_bWStopperFlag[nMode] == false &&
			FAS_IO.get_in_bit(st_io_info.i_OutConvPosUpChk, IO_ON) == IO_ON )
		{
			m_bWStopperFlag[nMode]			= true;
			m_dwWStopperWaitTime[nMode][0]	= GetCurrentTime();
		}
		else if (m_bWStopperFlag[nMode] == true &&
			FAS_IO.get_in_bit(st_io_info.i_OutConvPosUpChk,	IO_ON)	== IO_ON )
		{
			m_dwWStopperWaitTime[nMode][1]	= GetCurrentTime();
			m_dwWStopperWaitTime[nMode][2]	= m_dwWStopperWaitTime[nMode][1] - m_dwWStopperWaitTime[nMode][0];

			if (m_dwWStopperWaitTime[nMode][2] <= 0)
			{
				m_dwWStopperWaitTime[nMode][0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if(m_dwWStopperWaitTime[nMode][2] > (DWORD)st_wait_info.nOnWaitTime[nWaitTime])
			{
				return RET_GOOD;
			}
		}
		else
		{
			m_dwWStopperWaitTime[nMode][1]	= GetCurrentTime();
			m_dwWStopperWaitTime[nMode][2]	= m_dwWStopperWaitTime[nMode][1] - m_dwWStopperWaitTime[nMode][0];

			if (m_dwWStopperWaitTime[nMode][2] <= 0)
			{
				m_dwWStopperWaitTime[nMode][0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwWStopperWaitTime[nMode][2] > (DWORD)st_wait_info.nLimitWaitTime[nWaitTime])
			{
				//502020  "OUT CONV Position Stopper Up-Sol Error."
				m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_OutConvPosUpChk, nUpDn); 
				//CTL_Lib.Alarm_Error_Occurrence(305, dWARNING, strAlarm);

				return RET_ERROR;
			}
		}
	}

	return RET_PROCEED;

}

void CRunConveyor::OnSetTableMotorCylOnOff(int nMode, int nUpDn)
{
	if (nMode > 1) return;

	m_bCylFlag[nMode]			= false;
	m_dwCylWaitTime[nMode][0]	= GetCurrentTime();
// 	int		o_TableMotorForCyl;				//S0206
// 	int		o_TableMotorBackCyl;			//S0207

	if (nUpDn == IO_ON)
	{
		FAS_IO.set_out_bit(st_io_info.o_TableMotorForCyl, nUpDn); //S0206 forward
		FAS_IO.set_out_bit(st_io_info.o_TableMotorBackCyl, !nUpDn);
	}
	else
	{
		
		FAS_IO.set_out_bit(st_io_info.o_TableMotorBackCyl,!nUpDn); //S0207 Backward
		FAS_IO.set_out_bit(st_io_info.o_TableMotorForCyl, nUpDn);
	}


}


int CRunConveyor::OnGetTableMotorCylOnOff(int nMode, int nUpDn)
{
	int nWaitTime = WAIT_TURN_CONV_MOTOR_CYL;

	if (nUpDn == IO_OFF)
	{
		if (m_bCylFlag[nMode] == false &&
			FAS_IO.get_in_bit(st_io_info.i_TurnConvStopperForChk,	IO_OFF)	== IO_OFF &&
			FAS_IO.get_in_bit(st_io_info.i_TurnConvStopperBackChk,	IO_ON)	== IO_ON)
		{
			m_bCylFlag[nMode]		= true;
			m_dwCylWaitTime[nMode][0]	= GetCurrentTime();
		}
		else if (m_bCylFlag[nMode] == true &&
			FAS_IO.get_in_bit(st_io_info.i_TurnConvStopperForChk,	IO_OFF)	== IO_OFF &&
			FAS_IO.get_in_bit(st_io_info.i_TurnConvStopperBackChk,	IO_ON)	== IO_ON)
		{
			m_dwCylWaitTime[nMode][1] = GetCurrentTime();
			m_dwCylWaitTime[nMode][2] = m_dwCylWaitTime[nMode][1] - m_dwCylWaitTime[nMode][0];

			if (m_dwCylWaitTime[nMode][2] <= 0)
			{
				m_dwCylWaitTime[nMode][0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwCylWaitTime[nMode][2] > (DWORD)st_wait_info.nOnWaitTime[nWaitTime])
			{
				return RET_GOOD;
			}
		}
		else
		{
			m_dwCylWaitTime[nMode][1] = GetCurrentTime();
			m_dwCylWaitTime[nMode][2] = m_dwCylWaitTime[nMode][1] - m_dwCylWaitTime[nMode][0];

			if (m_dwCylWaitTime[nMode][2] <= 0)
			{
				m_dwCylWaitTime[nMode][0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwCylWaitTime[nMode][2] > (DWORD)st_wait_info.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_TurnConvStopperBackChk, nUpDn); 
				//CTL_Lib.Alarm_Error_Occurrence(304, dWARNING, strAlarm);

				return RET_ERROR;
			}
		}
	}
	else
	{
		if (m_bCylFlag[nMode] == false &&
			FAS_IO.get_in_bit(st_io_info.i_TurnConvStopperForChk,	IO_ON)	== IO_ON&&
			FAS_IO.get_in_bit(st_io_info.i_TurnConvStopperBackChk,	IO_OFF)	== IO_OFF)
		{
			m_bCylFlag[nMode]			= true;
			m_dwCylWaitTime[nMode][0]	= GetCurrentTime();
		}
		else if (m_bCylFlag[nMode] == true &&
			FAS_IO.get_in_bit(st_io_info.i_TurnConvStopperForChk,	IO_ON)	== IO_ON&&
			FAS_IO.get_in_bit(st_io_info.i_TurnConvStopperBackChk,	IO_OFF)	== IO_OFF)
		{
			m_dwCylWaitTime[nMode][1]	= GetCurrentTime();
			m_dwCylWaitTime[nMode][2]	= m_dwCylWaitTime[nMode][1] - m_dwCylWaitTime[nMode][0];

			if (m_dwCylWaitTime[nMode][2] <= 0)
			{
				m_dwCylWaitTime[nMode][0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if(m_dwCylWaitTime[nMode][2] > (DWORD)st_wait_info.nOnWaitTime[nWaitTime])
			{
				return RET_GOOD;
			}
		}
		else
		{
			m_dwCylWaitTime[nMode][1]	= GetCurrentTime();
			m_dwCylWaitTime[nMode][2]	= m_dwCylWaitTime[nMode][1] - m_dwCylWaitTime[nMode][0];

			if (m_dwCylWaitTime[nMode][2] <= 0)
			{
				m_dwCylWaitTime[nMode][0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwCylWaitTime[nMode][2] > (DWORD)st_wait_info.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_TurnConvStopperForChk, nUpDn); 
				//CTL_Lib.Alarm_Error_Occurrence(305, dWARNING, strAlarm);

				return RET_ERROR;
			}
		}
	}

	return RET_PROCEED;

}

void CRunConveyor::OnSetTableTurnCylOnOff(int nMode, int nUpDn)
{
	if (nMode > 1) return;

	m_bCylFlag[nMode]			= false;
	m_dwCylWaitTime[nMode][0]	= GetCurrentTime();
	// 	int		o_TableMotorForCyl;				//S0206
	// 	int		o_TableMotorBackCyl;			//S0207

	if (nUpDn == IO_ON)
	{
		FAS_IO.set_out_bit(st_io_info.o_TableTurnCylFor, nUpDn); //S0706 forward
		FAS_IO.set_out_bit(st_io_info.o_TableTurnCylBack, !nUpDn);
	}
	else
	{

		FAS_IO.set_out_bit(st_io_info.o_TableTurnCylFor,nUpDn); //S0707 Backward
		FAS_IO.set_out_bit(st_io_info.o_TableTurnCylBack, !nUpDn);
	}


}


int CRunConveyor::OnGetTableTurnCylOnOff(int nMode, int nUpDn)
{
	int nWaitTime = WAIT_CONV_TURN;

	if (nUpDn == IO_OFF)
	{
		if (m_bCylFlag[nMode] == false &&
			FAS_IO.get_in_bit(st_io_info.i_TurnRotatCylForChk,	IO_OFF)	== IO_OFF &&
			FAS_IO.get_in_bit(st_io_info.i_TurnRotatCylBackChk,	IO_ON)	== IO_ON)
		{
			m_bCylFlag[nMode]		= true;
			m_dwCylWaitTime[nMode][0]	= GetCurrentTime();
		}
		else if (m_bCylFlag[nMode] == true &&
			FAS_IO.get_in_bit(st_io_info.i_TurnRotatCylForChk,	IO_OFF)	== IO_OFF &&
			FAS_IO.get_in_bit(st_io_info.i_TurnRotatCylBackChk,	IO_ON)	== IO_ON)
		{
			m_dwCylWaitTime[nMode][1] = GetCurrentTime();
			m_dwCylWaitTime[nMode][2] = m_dwCylWaitTime[nMode][1] - m_dwCylWaitTime[nMode][0];

			if (m_dwCylWaitTime[nMode][2] <= 0)
			{
				m_dwCylWaitTime[nMode][0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwCylWaitTime[nMode][2] > (DWORD)st_wait_info.nOnWaitTime[nWaitTime])
			{
				return RET_GOOD;
			}
		}
		else
		{
			m_dwCylWaitTime[nMode][1] = GetCurrentTime();
			m_dwCylWaitTime[nMode][2] = m_dwCylWaitTime[nMode][1] - m_dwCylWaitTime[nMode][0];

			if (m_dwCylWaitTime[nMode][2] <= 0)
			{
				m_dwCylWaitTime[nMode][0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwCylWaitTime[nMode][2] > (DWORD)st_wait_info.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_TurnRotatCylBackChk, nUpDn); 
				//CTL_Lib.Alarm_Error_Occurrence(304, dWARNING, strAlarm);

				return RET_ERROR;
			}
		}
	}
	else
	{
		if (m_bCylFlag[nMode] == false &&
			FAS_IO.get_in_bit(st_io_info.i_TurnRotatCylForChk,	IO_ON)	== IO_ON&&
			FAS_IO.get_in_bit(st_io_info.i_TurnRotatCylBackChk,	IO_OFF)	== IO_OFF)
		{
			m_bCylFlag[nMode]			= true;
			m_dwCylWaitTime[nMode][0]	= GetCurrentTime();
		}
		else if (m_bCylFlag[nMode] == true &&
			FAS_IO.get_in_bit(st_io_info.i_TurnRotatCylForChk,	IO_ON)	== IO_ON&&
			FAS_IO.get_in_bit(st_io_info.i_TurnRotatCylBackChk,	IO_OFF)	== IO_OFF)
		{
			m_dwCylWaitTime[nMode][1]	= GetCurrentTime();
			m_dwCylWaitTime[nMode][2]	= m_dwCylWaitTime[nMode][1] - m_dwCylWaitTime[nMode][0];

			if (m_dwCylWaitTime[nMode][2] <= 0)
			{
				m_dwCylWaitTime[nMode][0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if(m_dwCylWaitTime[nMode][2] > (DWORD)st_wait_info.nOnWaitTime[nWaitTime])
			{
				return RET_GOOD;
			}
		}
		else
		{
			m_dwCylWaitTime[nMode][1]	= GetCurrentTime();
			m_dwCylWaitTime[nMode][2]	= m_dwCylWaitTime[nMode][1] - m_dwCylWaitTime[nMode][0];

			if (m_dwCylWaitTime[nMode][2] <= 0)
			{
				m_dwCylWaitTime[nMode][0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwCylWaitTime[nMode][2] > (DWORD)st_wait_info.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_TurnRotatCylForChk, nUpDn); 
				//CTL_Lib.Alarm_Error_Occurrence(305, dWARNING, strAlarm);

				return RET_ERROR;
			}
		}
	}

	return RET_PROCEED;

}

void CRunConveyor::OnRunInit()
{
	int nRet[5] = {0,};
	if (st_sync_info.nInitConveyor != INIT_CLEAR)		return;		//INIT_CLEAR 일때만 초기화 작업을 한다. 초기화가 끝나면 INIT_CLEAR -> INIT_READY가 되기 떄문에...

	nRet[0] = OnConvPcvCheck();

	if (nRet[0] == RET_ERROR)
	{
		st_handler_info.nInitialError = YES;
		m_nInitStep = 0;
		return;
	}
	switch (m_nInitStep)
	{
		case 0:
			
			m_nInitStep = 100;
			break;

// 		case 100:
// 			nRet[0] = FAS_IO.get_in_bit(st_io_info.i_InConvInChk,	IO_OFF);//ps100 //트레이가 장비에 진입시 감지 됨
// 			nRet[1] = FAS_IO.get_in_bit(st_io_info.i_InConvPosChk,	IO_OFF);//ps101 //트레이가 장비에 진입시 감지 됨
// 			nRet[2] = FAS_IO.get_in_bit(st_io_info.i_OutInConvPosChk,	IO_OFF);//ps100 //트레이가 장비에 진입시 감지 됨
// 			nRet[3] = FAS_IO.get_in_bit(st_io_info.i_TurnConvPosChk,	IO_OFF);//ps101 //트레이가 장비에 진입시 감지 됨
// 			nRet[4] = FAS_IO.get_in_bit(st_io_info.i_OutConvChk,	IO_OFF);//ps100 //트레이가 장비에 진입시 감지 됨
// 
// 			if(nRet[0] == IO_OFF && nRet[1] == IO_OFF && nRet[2] == IO_OFF && nRet[3] == IO_OFF && nRet[4] == IO_OFF )
// 			{
// 				m_nInitStep = 110;
// 			}
// 			else
// 			{//트레이가 없는지 확인
// 				m_nInitStep = 200;
// 				//501080 1 A "IN CONV Sensor Off Check Error."
// 				//501081 1 A "IN CONV Sensor On Check Error."
// 				//501090 1 A "IN CONV Position Off Check Error."
// 				//501091 1 A "IN CONV Position On Check Error."
// 				
// 				if	   (nRet[0] == IO_ON) m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_InConvInChk, IO_OFF); //
// 				else if(nRet[1] == IO_ON) m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_InConvPosChk, IO_OFF);
// 				else if(nRet[2] == IO_ON) m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_OutInConvPosChk, IO_OFF); //
// 				else if(nRet[3] == IO_ON) m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_TurnConvPosChk, IO_OFF);
// 				else   m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_OutConvChk, IO_OFF);
// 
// 				CTL_Lib.Alarm_Error_Occurrence(200, dWARNING, m_strAlarmCode);		
// 				m_nInitStep = 9000;
// 			}
// 			break;
		case 100:
			nRet[0] = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Conv, st_motor_info[m_nRobot_Conv].d_pos[TURN_CONVEYOR_REVERSE_SAFETY], COMI.mn_manualspeed_rate);

			if (nRet[0] == BD_GOOD) //좌측으로 이동
			{
				m_nInitStep = 120;
			}
			else if (nRet[0] == BD_ERROR || nRet[0] == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(201, dWARNING, st_alarm_info.strCode);
				m_nInitStep = 9000;
			}
			break;

		case 120:
			OnSetTableMotorCylOnOff(IO_RUN_MODE, IO_ON);

			st_sync_info.nInitConv_pitch = YES;
			m_nInitStep = 130;
			break;

		case 130:
			nRet[0] = OnGetTableMotorCylOnOff(IO_RUN_MODE, IO_ON);
			if(nRet[0] == RET_GOOD)
			{
				m_nInitStep = 200;
			}
			else if(nRet[0] == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(210, dWARNING, m_strAlarmCode);
				m_nInitStep = 9000;
			}
			break;

		case 200:

			OnSetOutConvStopperUpDn(IO_RUN_MODE, IO_ON);
			
			//st_sync_info.nInitConv_pitch = YES;

			m_nInitStep = 210;
			break;

		case 210:
			nRet[0] = OnGetOutConvStopperUpDn(IO_RUN_MODE, IO_ON);

			if(nRet[0] == RET_GOOD)
			{
				FAS_IO.set_out_bit(st_io_info.o_InConvMotorOn,IO_ON);

				FAS_IO.set_out_bit(st_io_info.o_TurnMotorDirection,IO_ON);
				
				FAS_IO.set_out_bit(st_io_info.o_TablePcbMovCw,IO_OFF);
				FAS_IO.set_out_bit(st_io_info.o_TablePcbMovCcw,IO_ON);

				FAS_IO.set_out_bit(st_io_info.o_OutConvMotorOn,IO_ON);
				
				m_lMotorOnTimeCheck[0] = GetCurrentTime();
				m_nInitStep = 220;
			}
			else if(nRet[0] == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(220, dWARNING, m_strAlarmCode);
				m_nInitStep = 9000;
			}
			break;

		case 220:
			m_lMotorOnTimeCheck[1] = GetCurrentTime();
			m_lMotorOnTimeCheck[2] = m_lMotorOnTimeCheck[1] - m_lMotorOnTimeCheck[0];

			if (m_lMotorOnTimeCheck[2] <= 0)
			{
				m_lMotorOnTimeCheck[0] = GetCurrentTime();
			}

			if (m_lMotorOnTimeCheck[2] > 5000)
			{
				
				if (FAS_IO.get_in_bit(st_io_info.i_OutConvChk,IO_ON) == IO_ON)
				{						
					FAS_IO.set_out_bit(st_io_info.o_TurnMotorDirection,IO_OFF);
					FAS_IO.set_out_bit(st_io_info.o_InConvMotorOn,IO_OFF);
					FAS_IO.set_out_bit(st_io_info.o_OutConvMotorOn,IO_OFF);

					m_nInitStep = 300;	
				}
				else
				{
					FAS_IO.set_out_bit(st_io_info.o_TurnMotorDirection,IO_OFF);
					FAS_IO.set_out_bit(st_io_info.o_InConvMotorOn,IO_OFF);
					FAS_IO.set_out_bit(st_io_info.o_OutConvMotorOn,IO_OFF);

					m_nInitStep = 300;

				}
		
			}
			break;

		case 300:
			if( st_sync_info.nInitPickerRbt == INIT_COMPLETE )
			{
				m_nInitStep = 1000;
			}
			break;

		case 1000:
			OnSetInConvStopperUpDn(IO_RUN_MODE, IO_OFF); //Stopper up //초기화 동작 
			m_nInitStep = 1100;
			break;
			
		case 1100:
			nRet[0] = OnGetInConvStopperUpDn(IO_RUN_MODE, IO_OFF);
			if(nRet[0] == RET_GOOD)
			{
				m_nInitStep = 2000;
			}
			else if(nRet[0] == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(230, dWARNING, m_strAlarmCode);
				m_nInitStep = 9000;
			}
			break;

		case 2000:
			OnSetTurnConvStopperUpDn(IO_RUN_MODE, IO_OFF);
			m_nInitStep = 2100;
			break;

		case 2100:
			nRet[0] = OnGetTurnConvStopperUpDn(IO_RUN_MODE, IO_OFF);
			if(nRet[0] == RET_GOOD)
			{
				 m_nInitStep = 3000;
			}
			else if(nRet[0] == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(240, dWARNING, m_strAlarmCode);
				m_nInitStep = 9000;
			}
			break;

		case 3000:
			OnSetTableMotorCylOnOff(IO_RUN_MODE, IO_OFF);
			m_nInitStep = 3100;
			break;

		case 3100:
			nRet[0] = OnGetTableMotorCylOnOff(IO_RUN_MODE, IO_OFF);
			if(nRet[0] == RET_GOOD)
			{
				m_nInitStep = 3150;
			}
			else if(nRet[0] == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(250, dWARNING, m_strAlarmCode);
				m_nInitStep = 9000;
			}
			break;

		case 3150:
			FAS_IO.set_out_bit(st_io_info.o_TableTurnCylFor, IO_ON);
			FAS_IO.set_out_bit(st_io_info.o_TableTurnCylBack, IO_OFF);
			m_nInitStep = 3200;
			break;

		case 3200:
			nRet[0] = FAS_IO.get_in_bit(st_io_info.i_TurnRotatCylBackChk, IO_OFF);
			nRet[1] = FAS_IO.get_in_bit(st_io_info.i_TurnRotatCylForChk, IO_OFF);

			if (nRet[0] == IO_OFF && nRet[1] == IO_OFF )
			{
				//502060 1 A "TURN CONV Table Motor Forward Cylinder-Sol Error."
				CTL_Lib.Alarm_Error_Occurrence(260, dWARNING, _T("502060"));
				m_nInitStep = 9000;
				//m_nInitStep = 0;
			}
			else
			{
				m_nInitStep = 4000;	
			}
			break;

		case 4000:
			OnSetOutConvStopperUpDn(IO_RUN_MODE, IO_OFF);
			m_nInitStep = 4100;
			break;

		case 4100:
			nRet[0] = OnGetOutConvStopperUpDn(IO_RUN_MODE, IO_OFF);
			if(nRet[0] == RET_GOOD)
			{
				m_nInitStep = 5000;
			}
			else if(nRet[0] == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(270, dWARNING, m_strAlarmCode);
				m_nInitStep = 9000;
			}
			break;

		case 5000:
			
			nRet[0] = COMI.HomeCheck_Mot(m_nRobot_Conv, MOT_SENS_ELM, MOT_TIMEOUT) ; 

			if(nRet[0] == BD_GOOD)
			{
				m_nInitStep = 5100;

			}
			else if(nRet[0] == BD_ERROR)
			{
				//Motor Home Check Error - (TURN CONV PITCH)
				CTL_Lib.Alarm_Error_Occurrence(280, dWARNING, _T("030007"));
				m_nInitStep = 9000;
			}
			break;

		case 5100:
			nRet[0] = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Conv, st_motor_info[m_nRobot_Conv].d_pos[TURN_CONVEYOR_REVERSE_SAFETY], COMI.mn_manualspeed_rate);

			if (nRet[0] == BD_GOOD) //좌측으로 이동
			{
				st_sync_info.nInitRbt[m_nRobot_Conv] = CTL_YES;
				m_nInitStep = 8000;
			}
			else if (nRet[0] == BD_ERROR || nRet[0] == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(290, dWARNING, st_alarm_info.strCode);
				m_nInitStep = 9000;
			}
			break;

		case 8000:
			st_sync_info.nInitConveyor = INIT_READY;
			m_nInitStep = 0;
			break;

		case 9000:
			st_handler_info.nInitialError = YES;
			m_nInitStep = 0;
			break;

		default :
			break;
	}

}

void CRunConveyor::OnFrontConvRunMove()
{
	int nRet[5] = {0,};
	int nRet_1;
	 
	switch(m_nRunStep[CONV_IN])
	{
		case 0:
			m_nRunStep[CONV_IN] = 100;
			break;

		case 100:
// 			if (st_sync_info.nSmema_Tray_Input_Req == CONV_REQ)
// 			{
				//트레이가 없는지 확인
				nRet[0] = FAS_IO.get_in_bit(st_io_info.i_InConvInChk,	IO_OFF);//ps100 //트레이가 장비에 진입시 감지 됨
				nRet[1] = FAS_IO.get_in_bit(st_io_info.i_InConvPosChk,	IO_OFF);//ps101 //트레이가 장비에 진입시 감지 됨
				nRet[2] = FAS_IO.get_in_bit(st_io_info.i_InConvPosUpChk,IO_OFF);
				
				if(nRet[0] == IO_OFF && nRet[1] == IO_OFF && nRet[2] == IO_OFF)
				{
					m_nRunStep[CONV_IN] = 110;
				}
				else
				{
					//501080 1 A "IN CONV Sensor Off Check Error."
					//501081 1 A "IN CONV Sensor On Check Error."
					//501090 1 A "IN CONV Position Off Check Error."
					//501091 1 A "IN CONV Position On Check Error."
					//501100 1 A "IN CONV Position Up Off Check Error."
					//501101 1 A "IN CONV Position Up On Check Error."
					if	   (nRet[0] == IO_ON) m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_InConvInChk, IO_OFF); //
					else if(nRet[1] == IO_ON) m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_InConvPosChk, IO_OFF);
					else if(nRet[2] == IO_ON) m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_InConvPosUpChk, IO_OFF);
					CTL_Lib.Alarm_Error_Occurrence(300, dWARNING, m_strAlarmCode);		
				}		
			//}		
			break;
			
		case 110:
// 			st_sync_info.nSmema_Tray_Input_Req = CONV_REQ; //Front 설비 투입 요청에 대한 응답
// 			m_nRunStep[CONV_IN] = 120;
			
			if (st_sync_info.TurnConvJobReady[CONVEYOR] == CTL_NONE )
			{
				//st_handler_info.cWndMain->PostMessage(WM_PCB_CV_IN_MOVE_DRAW_MAIN,CONV_IN_READY,0); //kwlee 2017.0220
				st_sync_info.nSmema_Tray_Input_Req = CONV_REQ; //Front 설비 투입 요청에 대한 응답
				m_nRunStep[CONV_IN] = 120;
			}
			break;
		
		case 120:
			OnSetInConvStopperUpDn(IO_RUN_MODE, IO_ON);
			m_nRunStep[CONV_IN] = 1000;
			break;

		case 1000:
			nRet[0] = OnGetInConvStopperUpDn(IO_RUN_MODE, IO_ON);

			if(nRet[0] == RET_GOOD)
			{
				/*st_sync_info.nSmema_Tray_Input_Req = CONV_READY; //Front 설비 투입 요청에 대한 응답*/
				m_nRunStep[CONV_IN] = 1100;
			}
			else if(nRet[0] == RET_ERROR)
			{
				//501100 1 A "IN CONV Position Up Off Check Error."
				//501101 1 A "IN CONV Position Up On Check Error."
			//	m_strAlarmCode.Format(_T("5%d%04d"), IO_ON, st_io_info.i_InConvPosUpChk);
				CTL_Lib.Alarm_Error_Occurrence(310, dWARNING, m_strAlarmCode);

			}
			break;

		case 1100:
// 			if (st_basic_info.nModeDevice == WITHOUT_DVC)
// 			{
// 				st_handler_info.cWndMain->PostMessage(WM_PCB_CV_IN_MOVE_DRAW_MAIN,CONV_IN_READY,0); //kwlee 2017.0220
// 				FAS_IO.set_out_bit(st_io_info.o_InConvMotorOn,IO_ON);
// 				m_dwConveyorWaitTime[CONV_IN][0] = GetCurrentTime();
// 				m_nRunStep[CONV_IN] = 1200;
// 			}
// 			else
// 			{
// 				if (st_sync_info.nSmema_Tray_Input_Req == CONV_READY)
// 				{
// 					st_handler_info.cWndMain->PostMessage(WM_PCB_CV_IN_MOVE_DRAW_MAIN,CONV_IN_READY,0); //kwlee 2017.0220
// 					FAS_IO.set_out_bit(st_io_info.o_InConvMotorOn,IO_ON);
// 					m_dwConveyorWaitTime[CONV_IN][0] = GetCurrentTime();
// 					m_nRunStep[CONV_IN] = 1200;
// 				}
// 			}
			//kwlee 2017.0313
			if (st_sync_info.nSmema_Tray_Input_Req == CONV_READY)
			{
				st_handler_info.cWndMain->PostMessage(WM_PCB_CV_IN_MOVE_DRAW_MAIN,CONV_IN_READY,0); //kwlee 2017.0220
				FAS_IO.set_out_bit(st_io_info.o_InConvMotorOn,IO_ON);
				m_dwConveyorWaitTime[CONV_IN][0] = GetCurrentTime();
				m_nRunStep[CONV_IN] = 1200;
			}
			break;

		case 1200:
// 			if (st_basic_info.nModeDevice == WITHOUT_DVC)
// 			{
// 				st_handler_info.cWndMain->PostMessage(WM_PCB_CV_IN_MOVE_DRAW_MAIN,CONV_PCB_IN_ING,0); //kwlee 2017.0220
// 				m_dwConveyorWaitTime[CONV_IN][0] = GetCurrentTime();				
// 				m_nRunStep[CONV_IN] = 1300;
// 			}
// 			else
// 			{
// 				if (FAS_IO.get_in_bit(st_io_info.i_InConvPosChk,IO_ON) == IO_ON )
// 				{
// 					st_handler_info.cWndMain->PostMessage(WM_PCB_CV_IN_MOVE_DRAW_MAIN,CONV_PCB_IN_ING,0); //kwlee 2017.0220
// 					st_sync_info.nSmema_Tray_Input_Req = CONV_CLR;
// 					m_dwConveyorWaitTime[CONV_IN][0] = GetCurrentTime();				
// 					m_nRunStep[CONV_IN] = 1300;
// 				}
// 				else
// 				{
// 					nRet_1 = OnConvWaitTime(CONV_IN,WAIT_CONV_IN);
// 					if(nRet_1 == RET_ERROR)
// 					{
// 						//501090 1 A "IN CONV Position Off Check Error."
// 						//501091 1 A "IN CONV Position On Check Error."
// 						m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_InConvPosChk, IO_OFF);
// 						CTL_Lib.Alarm_Error_Occurrence(320, dWARNING, m_strAlarmCode);
// 
// 						FAS_IO.set_out_bit(st_io_info.o_InConvMotorOn,IO_OFF);
// 						m_nRunStep[CONV_IN] = 1100;
// 					}
// 				}
// 			}
			//kwlee 2017.0313
			if (FAS_IO.get_in_bit(st_io_info.i_InConvPosChk,IO_ON) == IO_ON )
			{
				st_handler_info.cWndMain->PostMessage(WM_PCB_CV_IN_MOVE_DRAW_MAIN,CONV_PCB_IN_ING,0); //kwlee 2017.0220
				st_sync_info.nSmema_Tray_Input_Req = CONV_CLR;
				m_dwConveyorWaitTime[CONV_IN][0] = GetCurrentTime();				
				m_nRunStep[CONV_IN] = 1300;
			}
			else
			{
				nRet_1 = OnConvWaitTime(CONV_IN,WAIT_CONV_IN);
				if(nRet_1 == RET_ERROR)
				{
					//501090 1 A "IN CONV Position Off Check Error."
					//501091 1 A "IN CONV Position On Check Error."
					m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_InConvPosChk, IO_OFF);
					CTL_Lib.Alarm_Error_Occurrence(320, dWARNING, m_strAlarmCode);

					FAS_IO.set_out_bit(st_io_info.o_InConvMotorOn,IO_OFF);
					m_nRunStep[CONV_IN] = 1100;
				}
			}
			break;

		case 1300:
			m_dwConveyorWaitTime[CONV_IN][1] = GetCurrentTime();
			m_dwConveyorWaitTime[CONV_IN][2] = m_dwConveyorWaitTime[CONV_IN][1] - m_dwConveyorWaitTime[CONV_IN][0];

			if (m_dwConveyorWaitTime[CONV_IN][2] <= 0)
			{
				m_dwConveyorWaitTime[CONV_IN][0] = GetCurrentTime();
			}
			if (m_dwConveyorWaitTime[CONV_IN][2] > (DWORD)st_wait_info.nOnWaitTime[WAIT_CONV_IN])
			{
// 				if (st_basic_info.nModeDevice == WITHOUT_DVC)
// 				{	
// 					FAS_IO.set_out_bit(st_io_info.o_InConvMotorOn,IO_OFF);
// 					m_dwConveyorWaitTime[CONV_IN][0] = GetCurrentTime();
// 					m_nRunStep[CONV_IN] = 1400;
// 				}
// 				else
// 				{
// 					if (FAS_IO.get_in_bit(st_io_info.i_InConvPosChk,IO_ON) == IO_ON)
// 					{
// 						FAS_IO.set_out_bit(st_io_info.o_InConvMotorOn,IO_OFF);
// 						m_dwConveyorWaitTime[CONV_IN][0] = GetCurrentTime();
// 						m_nRunStep[CONV_IN] = 1400;
// 					}
// 					else
// 					{
// 						m_nRunStep[CONV_IN] = 1200;
// 					}
// 				}
				//kwlee 2017.0313
				if (FAS_IO.get_in_bit(st_io_info.i_InConvPosChk,IO_ON) == IO_ON)
				{
					FAS_IO.set_out_bit(st_io_info.o_InConvMotorOn,IO_OFF);
					m_dwConveyorWaitTime[CONV_IN][0] = GetCurrentTime();
					m_nRunStep[CONV_IN] = 1400;
				}
				else
				{
					m_nRunStep[CONV_IN] = 1200;
				}
			}
			break;

		case 1400:
			m_dwConveyorWaitTime[CONV_IN][1] = GetCurrentTime();
			m_dwConveyorWaitTime[CONV_IN][2] = m_dwConveyorWaitTime[CONV_IN][1] - m_dwConveyorWaitTime[CONV_IN][0];

			if (m_dwConveyorWaitTime[CONV_IN][2] <= 0)
			{
				m_dwConveyorWaitTime[CONV_IN][0] = GetCurrentTime();
			}
			if (m_dwConveyorWaitTime[CONV_IN][2] > (DWORD)st_wait_info.nOffWaitTime[WAIT_CONV_IN]/*[IO_OFF]*/)
			{
				m_nRunStep[CONV_IN] = 1500;
			}
			break;

		case 1500:
			OnSetInConvStopperUpDn(IO_RUN_MODE, IO_OFF);
			m_nRunStep[CONV_IN] = 1600;
			break;

		case 1600:
			nRet[0] = OnGetInConvStopperUpDn(IO_RUN_MODE, IO_OFF);

			if(nRet[0] == RET_GOOD)
			{
				st_sync_info.nFrontTrayIn = CONV_REQ; //middle conv 넘김 요청 상태
				m_nRunStep[CONV_IN] = 1700;
			}
			else if(nRet[0] == RET_ERROR)
			{
				//501100 1 A "IN CONV Position Up Off Check Error."
				//501101 1 A "IN CONV Position Up On Check Error."
				//	m_strAlarmCode.Format(_T("5%d%04d"), IO_OFF, st_io_info.i_InConvPosUpChk);
				CTL_Lib.Alarm_Error_Occurrence(330, dWARNING, m_strAlarmCode);
			}
			break;

		case 1700:
			if (st_sync_info.TurnConvJobReady[CONVEYOR] == CTL_NONE)
			{
// 				if (st_basic_info.nModeDevice == WITHOUT_DVC)
// 				{
// 					if (st_sync_info.nMidTrayIn == CTL_REQ) //Middle 에서 Conv 투입 준비 신호
// 					{    
// 						
// 						FAS_IO.set_out_bit(st_io_info.o_InConvMotorOn,IO_ON);
// 						m_nRunStep[CONV_IN] = 1710;
// 					}
// 				}
// 				else
// 				{
// 					if (st_sync_info.nMidTrayIn == CTL_REQ) //Middle 에서 Conv 투입 준비 신호
// 					{      
// 						FAS_IO.set_out_bit(st_io_info.o_InConvMotorOn,IO_ON);
// 						m_nRunStep[CONV_IN] = 1710;
// 					}
// 					else
// 					{
// 						if (FAS_IO.get_in_bit(st_io_info.i_InConvPosChk,IO_OFF) == IO_OFF)
// 						{
// 							nRet_1 = OnConvWaitTime(CONV_IN,WAIT_CONV_REQ);
// 							if(nRet_1 == RET_ERROR)
// 							{
// 								//501090 1 A "IN CONV Position Off Check Error."
// 								//501091 1 A "IN CONV Position On Check Error."
// 								m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_InConvPosChk, IO_OFF);
// 								CTL_Lib.Alarm_Error_Occurrence(340, dWARNING, m_strAlarmCode);
// 								m_nRunStep[CONV_IN] = 1600;
// 							}
// 						}	
// 					}
// 				}
				//kwlee 2017.0313
				if (st_sync_info.nMidTrayIn == CTL_REQ) //Middle 에서 Conv 투입 준비 신호
				{      
					FAS_IO.set_out_bit(st_io_info.o_InConvMotorOn,IO_ON);
					m_nRunStep[CONV_IN] = 1710;
				}
				else
				{
					if (FAS_IO.get_in_bit(st_io_info.i_InConvPosChk,IO_OFF) == IO_OFF)
					{
						nRet_1 = OnConvWaitTime(CONV_IN,WAIT_CONV_REQ);
						if(nRet_1 == RET_ERROR)
						{
							//501090 1 A "IN CONV Position Off Check Error."
							//501091 1 A "IN CONV Position On Check Error."
							m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_InConvPosChk, IO_OFF);
							CTL_Lib.Alarm_Error_Occurrence(340, dWARNING, m_strAlarmCode);
							m_nRunStep[CONV_IN] = 1600;
						}
					}	
				}
			}
			break;

		case 1710:

			if( FAS_IO.get_in_bit(st_io_info.i_InConvPosUpChk,IO_OFF)	== IO_OFF )
			{
				m_nRunStep[CONV_IN] = 1900;
			}
			else
			{
				m_nRunStep[CONV_IN] = 1800;
			}
			break;

		case 1800:
			OnSetInConvStopperUpDn(IO_RUN_MODE, IO_OFF);
			m_nRunStep[CONV_IN] = 1900;
			break;


		case 1900:
			nRet[0] = OnGetInConvStopperUpDn(IO_RUN_MODE, IO_OFF);

			if(nRet[0] == RET_GOOD)
			{
				m_dwConveyorWaitTime[CONV_IN][0] = GetCurrentTime();
				m_nRunStep[CONV_IN] = 2000;
			}
			else if(nRet[0] == RET_ERROR)
			{
				//501100 1 A "IN CONV Position Up Off Check Error."
				//501101 1 A "IN CONV Position Up On Check Error."
			//	m_strAlarmCode.Format(_T("5%d%04d"), IO_OFF, st_io_info.i_InConvPosUpChk);
				CTL_Lib.Alarm_Error_Occurrence(350, dWARNING, m_strAlarmCode);
			}
			break;

		case 2000:
// 			if (st_basic_info.nModeDevice == WITHOUT_DVC)
// 			{
// 				//
// 				m_dwConveyorWaitTime[CONV_IN][0] = GetCurrentTime();
// 				m_nRunStep[CONV_IN] = 3000;
// 			}
// 			else
// 			{
// 				if (FAS_IO.get_in_bit(st_io_info.i_InConvPosChk,IO_ON) == IO_OFF)
// 				{
// 					m_dwConveyorWaitTime[CONV_IN][0] = GetCurrentTime();
// 					m_nRunStep[CONV_IN] = 3000;
// 				}
// 				else
// 				{
// 					nRet_1 = OnConvWaitTime(CONV_IN,WAIT_CONV_OUT);
// 					if(nRet_1 == RET_ERROR)
// 					{
// 						//501090 1 A "IN CONV Position Off Check Error."
// 						//501091 1 A "IN CONV Position On Check Error."
// 						m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_InConvPosChk, IO_ON);
// 						CTL_Lib.Alarm_Error_Occurrence(360, dWARNING, m_strAlarmCode);
// 
// 						FAS_IO.set_out_bit(st_io_info.o_InConvMotorOn,IO_OFF);
// 						//	m_nRunStep[CONV_IN] = 0;
// 					}
// 				}
// 			}
			//kwlee 2017.0313
			if (FAS_IO.get_in_bit(st_io_info.i_InConvPosChk,IO_ON) == IO_OFF)
			{
				m_dwConveyorWaitTime[CONV_IN][0] = GetCurrentTime();
				m_nRunStep[CONV_IN] = 3000;
			}
			else
			{
				nRet_1 = OnConvWaitTime(CONV_IN,WAIT_CONV_OUT);
				if(nRet_1 == RET_ERROR)
				{
					//501090 1 A "IN CONV Position Off Check Error."
					//501091 1 A "IN CONV Position On Check Error."
					m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_InConvPosChk, IO_ON);
					CTL_Lib.Alarm_Error_Occurrence(360, dWARNING, m_strAlarmCode);

					FAS_IO.set_out_bit(st_io_info.o_InConvMotorOn,IO_OFF);
					//	m_nRunStep[CONV_IN] = 0;
				}
			}
			break;

		case 3000:
			m_dwConveyorWaitTime[CONV_IN][1] = GetCurrentTime();
			m_dwConveyorWaitTime[CONV_IN][2] = m_dwConveyorWaitTime[CONV_IN][1] - m_dwConveyorWaitTime[CONV_IN][0];

			if (m_dwConveyorWaitTime[CONV_IN][2] <= 0)
			{
				m_dwConveyorWaitTime[CONV_IN][0] = GetCurrentTime();
			}

			if (m_dwConveyorWaitTime[CONV_IN][2] < 1000)
			{
// 				if (st_basic_info.nModeDevice == WITHOUT_DVC)
// 				{
// 					FAS_IO.set_out_bit(st_io_info.o_InConvMotorOn,IO_OFF);
// 					st_sync_info.nFrontTrayIn = CTL_NO;
// 					st_sync_info.nMidTrayIn = CTL_NO;
// 					m_nRunStep[CONV_IN] = 0;
// 				}
// 				else
// 				{
// 					if (FAS_IO.get_in_bit(st_io_info.i_InConvPosChk,IO_ON) == IO_ON)
// 					{
// 						m_nRunStep[CONV_IN] = 2000;
// 					}
// 					else
// 					{
// 						FAS_IO.set_out_bit(st_io_info.o_InConvMotorOn,IO_OFF);
// 						st_sync_info.nFrontTrayIn = CTL_NO;
// 						st_sync_info.nMidTrayIn = CTL_NO;
// 						m_nRunStep[CONV_IN] = 0;
// 					}
// 				}
				//kwlee 2017.0313
				if (FAS_IO.get_in_bit(st_io_info.i_InConvPosChk,IO_ON) == IO_ON)
				{
					m_nRunStep[CONV_IN] = 2000;
				}
				else
				{
					FAS_IO.set_out_bit(st_io_info.o_InConvMotorOn,IO_OFF);
					st_sync_info.nFrontTrayIn = CTL_NO;
					st_sync_info.nMidTrayIn = CTL_NO;
					m_nRunStep[CONV_IN] = 0;
				}
			}
			break;
	}
}


void CRunConveyor::OnTurnConvMove()
{
	int nRet[7] = {0,};
	int nRet_1;
	double d_CurPos[30] = {0,};

	
	switch(m_nRunStep[CONV_MID])
	{
	case 0:
		m_nRunStep[CONV_MID] = 100;

		break;

	case 100:
		if (st_sync_info.nFrontTrayIn == CONV_REQ)
		{
			//트레이가 없는지 확인
			nRet[0] = FAS_IO.get_in_bit(st_io_info.i_TurnConvPosChk, IO_OFF);

			if(nRet[0] == IO_OFF)
			{
				m_nRunStep[CONV_MID] = 200;
			}
			else
			{
				//502110 1 A "TURN CONV  Position Sensor Off Check Error."
				//502111 1 A "TURN CONV  Position Sensor On Check Error."
				m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_TurnConvPosChk, IO_OFF);
				CTL_Lib.Alarm_Error_Occurrence(400, dWARNING, m_strAlarmCode);		
			}		
		}		
		break;

	case 200:
		d_CurPos[0] = COMI.Get_MotCurrentPos(M_TURN_CONV_PITCH);

		if (d_CurPos[0] > st_motor_info[M_TURN_CONV_PITCH].d_pos[TURN_CONVEYOR_REVERSE_SAFETY] + st_motor_info[M_TURN_CONV_PITCH].d_allow)
		{
			m_nRunStep[CONV_MID] = 1000;
		}
		else
		{
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_TURN_CONV_PITCH, st_motor_info[M_TURN_CONV_PITCH].d_pos[TURN_CONVEYOR_REVERSE_SAFETY], COMI.mn_runspeed_rate);

			if (nRet_1 == BD_GOOD)
			{
				m_nRunStep[CONV_MID] = 1000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				m_strAlarmCode.Format(_T("%02d0006"), M_TURN_CONV_PITCH); //000008 0 A "MOTOR MOVE SAFETY CHECK ERROR -[M_ROBOT_X]."
				CTL_Lib.Alarm_Error_Occurrence(401, dWARNING, m_strAlarmCode);//dMOTOR_WARNING ==> 이때는 알람은 발생시키지 않는다(run status를 바꾸지 않는다).thread 에서만 판단하여 알람 발생시킨다 
				//m_nRunStep[CONV_MID] = 0;
			}
		}
		break;

	case 1000:	
		nRet[0] = FAS_IO.get_in_bit(st_io_info.i_TurnRotatCylBackChk, IO_OFF);
		nRet[1] = FAS_IO.get_in_bit(st_io_info.i_TurnRotatCylForChk, IO_ON);

		if (nRet[0] == IO_OFF && nRet[1] == IO_ON )
		{
			m_nRunStep[CONV_MID] = 1100;
		}
		else
		{
			//502060 1 A "TURN CONV Table Motor Forward Cylinder-Sol Error."
			CTL_Lib.Alarm_Error_Occurrence(410, dWARNING, _T("502060"));
			//m_nInitStep = 0;
		}
		break;

	case 1100:
		OnSetTurnConvStopperUpDn(IO_RUN_MODE, IO_OFF);
		m_nRunStep[CONV_MID] = 1200;
		break;


	case 1200:

		nRet[0] = OnGetTurnConvStopperUpDn(IO_RUN_MODE, IO_OFF);
		if(nRet[0] == RET_GOOD)
		{
			m_nRunStep[CONV_MID] = 1210;
		}
		else if(nRet[0] == RET_ERROR)
		{
			//502020 1 A "OUT CONV Position Stopper Up-Sol Error."
			//502030 1 A "OUT CONV Position Stopper Down-Sol Error."
			CTL_Lib.Alarm_Error_Occurrence(420, dWARNING, m_strAlarmCode);
		}
		break;

	case 1210:
		if( FAS_IO.get_in_bit(st_io_info.i_TurnConvStopperForChk, IO_ON)	==  IO_ON &&
			FAS_IO.get_in_bit(st_io_info.i_TurnConvStopperBackChk,IO_OFF)	== IO_OFF )
		{
			m_nRunStep[CONV_MID] = 1500;
		}
		else
		{
			m_nRunStep[CONV_MID] = 1300;
		}
		break;

	case 1300:
		OnSetTableMotorCylOnOff(IO_RUN_MODE, IO_OFF);
		m_nRunStep[CONV_MID] = 1400;
		break;

	case 1400:
		nRet[0] = OnGetTableMotorCylOnOff(IO_RUN_MODE, IO_OFF);
		if(nRet[0] == RET_GOOD)
		{
			m_nRunStep[CONV_MID] = 1500;
		
		}
		else if(nRet[0] == RET_ERROR)
		{
			//502060 1 A "TURN CONV Table Motor Forward Cylinder-Sol Error."
			//502070 1 A "TURN CONV Table Motor Backward Cylinder-Sol Error."
			CTL_Lib.Alarm_Error_Occurrence(430, dWARNING, m_strAlarmCode);
		}
		break;
		//투입 가능 한지 확인 완료 
		//////////////////////////////////////////////////////////////////////////
	case 1500:
		OnSetTurnConvStopperUpDn(IO_RUN_MODE, IO_ON);
		st_sync_info.nMidTrayIn = CTL_REQ;
		m_nRunStep[CONV_MID] = 1600;
		break;

	case 1600:
		
		nRet[0] = OnGetTurnConvStopperUpDn(IO_RUN_MODE, IO_ON);

		if(nRet[0] == RET_GOOD)
		{
			//st_sync_info.nMidTrayIn = CTL_READY;
			
			m_nRunStep[CONV_MID] = 1700;
		}
		else if(nRet[0] == RET_ERROR)
		{
			//501100 1 A "IN CONV Position Up Off Check Error."
			//501101 1 A "IN CONV Position Up On Check Error."
			//	m_strAlarmCode.Format(_T("5%d%04d"), IO_ON, st_io_info.i_InConvPosUpChk);
			CTL_Lib.Alarm_Error_Occurrence(440, dWARNING, m_strAlarmCode);
		}
		
		break;
		//////////////////////////////////////////
		//MOTOR CYLINDER FORWARD 
	case 1700:
		OnSetTableMotorCylOnOff(IO_RUN_MODE, IO_ON);
		m_nRunStep[CONV_MID] = 1800;
		break;

	case 1800:
		nRet[0] = OnGetTableMotorCylOnOff(IO_RUN_MODE, IO_ON);
		if(nRet[0] == RET_GOOD)
		{
			m_nRunStep[CONV_MID] = 1900;
		}
		else if(nRet[0] == RET_ERROR)
		{
			//502060 1 A "TURN CONV Table Motor Forward Cylinder-Sol Error."
			//502070 1 A "TURN CONV Table Motor Backward Cylinder-Sol Error."
			CTL_Lib.Alarm_Error_Occurrence(450, dWARNING, m_strAlarmCode);
		}
		break;
		/////////////////////////////////////////////////
		// TURN MOTOR ON, CCW
	case 1900:
		FAS_IO.set_out_bit(st_io_info.o_TurnMotorDirection,IO_ON);
		FAS_IO.set_out_bit(st_io_info.o_TablePcbMovCcw,IO_ON);
		FAS_IO.set_out_bit(st_io_info.o_TablePcbMovCw,IO_OFF);
		//st_handler_info.cWndMain->PostMessage(WM_PCB_CV_IN_MOVE_DRAW_MAIN,TURN_PCB_CONV_IN,0); //kwlee 2017.0220
		m_dwConveyorWaitTime[CONV_MID][0] = GetCurrentTime();

		m_nRunStep[CONV_MID] = 2000;
		break;

	case 2000:
		//Turn Conv Pos 센서 확인
// 		if (st_basic_info.nModeDevice == WITHOUT_DVC)
// 		{
// 			m_dwConveyorWaitTime[CONV_MID][0] = GetCurrentTime();				
// 			m_nRunStep[CONV_MID] = 2100;
// 		}
// 		else
// 		{
// 			if (FAS_IO.get_in_bit(st_io_info.i_TurnConvPosChk,IO_ON) == IO_ON)
// 			{
// 				m_dwConveyorWaitTime[CONV_MID][0] = GetCurrentTime();				
// 				m_nRunStep[CONV_MID] = 2100;
// 			}
// 			else
// 			{
// 				//	m_dwConveyorWaitTime[CONV_MID][0] = GetCurrentTime();
// 				//m_nRunStep[CONV_MID] = 2010;
// 
// 				nRet_1 = OnConvWaitTime(CONV_MID,WAIT_CONV_IN);
// 
// 				if(nRet_1 == RET_ERROR)
// 				{
// 					//502110 1 A "TURN CONV  Position Sensor Off Check Error."
// 					//502111 1 A "TURN CONV  Position Sensor On Check Error."
// 					m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_TurnConvPosChk, IO_OFF);
// 					//	CTL_Lib.Alarm_Error_Occurrence(460, dWARNING, m_strAlarmCode);
// 					m_dwConveyorWaitTime[CONV_MID][0] = GetCurrentTime();
// 					m_nErrorStep = m_nRunStep[CONV_MID];
// 					m_nRunStep[CONV_MID] = 5000;
// 				}
// 			}
// 		}
		//kwlee 2017.0313
		if (FAS_IO.get_in_bit(st_io_info.i_TurnConvPosChk,IO_ON) == IO_ON)
		{
			m_dwConveyorWaitTime[CONV_MID][0] = GetCurrentTime();				
			m_nRunStep[CONV_MID] = 2100;
		}
		else
		{
			//	m_dwConveyorWaitTime[CONV_MID][0] = GetCurrentTime();
			//m_nRunStep[CONV_MID] = 2010;

			nRet_1 = OnConvWaitTime(CONV_MID,WAIT_CONV_IN);

			if(nRet_1 == RET_ERROR)
			{
				//502110 1 A "TURN CONV  Position Sensor Off Check Error."
				//502111 1 A "TURN CONV  Position Sensor On Check Error."
				m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_TurnConvPosChk, IO_OFF);
				//	CTL_Lib.Alarm_Error_Occurrence(460, dWARNING, m_strAlarmCode);
				m_dwConveyorWaitTime[CONV_MID][0] = GetCurrentTime();
				m_nErrorStep = m_nRunStep[CONV_MID];
				m_nRunStep[CONV_MID] = 5000;
			}
		}

		break;
		///////////////////////////////////////////////////////////

	case 2100:
		m_dwConveyorWaitTime[CONV_MID][1] = GetCurrentTime();
		m_dwConveyorWaitTime[CONV_MID][2] = m_dwConveyorWaitTime[CONV_MID][1] - m_dwConveyorWaitTime[CONV_MID][0];

		if (m_dwConveyorWaitTime[CONV_MID][2] <= 0)
		{
			m_dwConveyorWaitTime[CONV_MID][0] = GetCurrentTime();
		}
		//Turn Pos 감지 시간 확인
		if (m_dwConveyorWaitTime[CONV_MID][2] > (DWORD)st_wait_info.nOnWaitTime[WAIT_CONV_IN])
		{
// 			if (st_basic_info.nModeDevice == WITHOUT_DVC)
// 			{
// 				st_handler_info.cWndMain->PostMessage(WM_PCB_CV_TURN_MOVE_DRAW_MAIN,TURN_PCB_CONV_WORK_POS,0); //kwlee 2017.0220
// 				FAS_IO.set_out_bit(st_io_info.o_TurnMotorDirection,IO_OFF);
// 				m_dwConveyorWaitTime[CONV_MID][0] = GetCurrentTime();
// 				m_nRunStep[CONV_MID] = 2200;
// 				st_sync_info.nMidTrayIn = CTL_NO;
// 			}
// 			else
// 			{
// 				if (FAS_IO.get_in_bit(st_io_info.i_TurnConvPosChk,IO_ON) == IO_ON)
// 				{
// 					
// 					FAS_IO.set_out_bit(st_io_info.o_TurnMotorDirection,IO_OFF);
// 
// 					m_dwConveyorWaitTime[CONV_MID][0] = GetCurrentTime();
// 					st_handler_info.cWndMain->PostMessage(WM_PCB_CV_TURN_MOVE_DRAW_MAIN,TURN_PCB_CONV_WORK_POS,0); //kwlee 2017.0220
// 					m_nRunStep[CONV_MID] = 2200;
// 
// 					st_sync_info.nMidTrayIn = CTL_NO;
// 				}
// 				else
// 				{
// 					m_nRunStep[CONV_MID] = 2000;
// 				}
// 			}
			//kwlee 2017.0313
			if (FAS_IO.get_in_bit(st_io_info.i_TurnConvPosChk,IO_ON) == IO_ON)
			{
				FAS_IO.set_out_bit(st_io_info.o_TurnMotorDirection,IO_OFF);

				m_dwConveyorWaitTime[CONV_MID][0] = GetCurrentTime();
				st_handler_info.cWndMain->PostMessage(WM_PCB_CV_TURN_MOVE_DRAW_MAIN,TURN_PCB_CONV_WORK_POS,0); //kwlee 2017.0220
				m_nRunStep[CONV_MID] = 2200;

				st_sync_info.nMidTrayIn = CTL_NO;
			}
			else
			{
				m_nRunStep[CONV_MID] = 2000;
			}
		}
		break;

	case 2200:
		m_dwConveyorWaitTime[CONV_MID][1] = GetCurrentTime();
		m_dwConveyorWaitTime[CONV_MID][2] = m_dwConveyorWaitTime[CONV_MID][1] - m_dwConveyorWaitTime[CONV_MID][0];

		if (m_dwConveyorWaitTime[CONV_MID][2] <= 0)
		{
			m_dwConveyorWaitTime[CONV_MID][0] = GetCurrentTime();
		}
		if (m_dwConveyorWaitTime[CONV_MID][2] > (DWORD)st_wait_info.nOffWaitTime[WAIT_CONV_IN]/*[IO_OFF]*/)
		{
			m_nRunStep[CONV_MID] = 2300;
		}
		break;

	case 2300: 
		/////////////////////////////////////////////////////////
		//Picker 작업 완료 대기
		//Table Turn 유무 확인
		//Turn Conv 준비
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_TURN_CONV_PITCH, st_motor_info[M_TURN_CONV_PITCH].d_pos[TURN_CONVEYOR_WORK], COMI.mn_runspeed_rate);

		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			//nRet_2 = OnTurnConvPosCheck();
			st_handler_info.cWndMain->PostMessage(WM_PCB_CV_TURN_MOVE_DRAW_MAIN,TURN_PCB_CONV_TURN_POS,0);
			st_sync_info.TurnConvJobReady[CONVEYOR] = CTL_READY;
			m_nRunStep[CONV_MID] = 2310;
			
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(750, dWARNING, st_alarm_info.strCode);
		//	m_nRunStep[CONV_MID] = 0;
		}
		break;

	case 2310:
		//nRet_2 = OnTurnConvPosCheck();
		OnSetTableMotorCylOnOff(IO_RUN_MODE, IO_OFF);
		m_nRunStep[CONV_MID] = 2400;
		break;

	case 2400:
		//nRet_2 = OnTurnConvPosCheck();
		nRet[0] = OnGetTableMotorCylOnOff(IO_RUN_MODE, IO_OFF);
		if(nRet[0] == RET_GOOD)
		{
			m_nRunStep[CONV_MID] = 2500;

		}
		else if(nRet[0] == RET_ERROR)
		{
			//502060 1 A "TURN CONV Table Motor Forward Cylinder-Sol Error."
			//502070 1 A "TURN CONV Table Motor Backward Cylinder-Sol Error."
			CTL_Lib.Alarm_Error_Occurrence(203, dWARNING, m_strAlarmCode);
		}
		break;

	case 2500:
		//Turn Conv Stopper Down 시킨다.
		//nRet_2 = OnTurnConvPosCheck();
		OnSetTurnConvStopperUpDn(IO_RUN_MODE, IO_OFF);
		m_nRunStep[CONV_MID] = 2600;
		break;

	case 2600:
		//nRet_2 = OnTurnConvPosCheck();
		nRet[0] = OnGetTurnConvStopperUpDn(IO_RUN_MODE, IO_OFF);

		if(nRet[0] == RET_GOOD)
		{
			//m_nRunStep[CONV_MID] = 2610;
			m_nRunStep[CONV_MID] = 2610;
		}
		else if(nRet[0] == RET_ERROR)
		{
			//501130 1 A "TURN CONV Position Down Off Check Error."
			//501131 1 A "TURN CONV Position Down On Check Error."
			CTL_Lib.Alarm_Error_Occurrence(470, dWARNING, m_strAlarmCode);
		}
		break;
		
		////////////////////////////////////////////
		//로봇이 안전 위치로 빠지면 Conv Turn 시킨다.
	case 2610:
		nRet_1 = OnTurnConvPosCheck();

// 		if (st_basic_info.nModeDevice == WITHOUT_DVC)
// 		{
// 			if (nRet_1 == RET_GOOD)
// 			{
// 				if (st_sync_info.TurnConvJobReady[ROBOT] == CTL_READY)
// 				{
// 					st_sync_info.nConvTurnCheck = TRUE; //kwlee 2017.0220
// 					st_handler_info.cWndMain->PostMessage(WM_PCB_CV_TURN_MOVE_DRAW_MAIN,TURN_PCB_CONV_TURN,0); //kwlee 2017.0220
// 					OnSetTableTurnCylOnOff(IO_RUN_MODE,IO_OFF);
// 					m_nRunStep[CONV_MID] = 2630;
// 				}
// 			}
// 		}
// 		else
// 		{
// 			if (nRet_1 == RET_RETRY)
// 			{
// 				m_nRunStep[CONV_MID] = 1500;
// 			}
// 			else if (nRet_1 == RET_GOOD)
// 			{
// 				if (st_sync_info.TurnConvJobReady[ROBOT] == CTL_READY)
// 				{
// 					OnSetTableTurnCylOnOff(IO_RUN_MODE,IO_OFF);
// 					m_nRunStep[CONV_MID] = 2630;
// 				}
// 			}
// 			else if (nRet_1 == RET_ERROR)
// 			{
// 				//m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_TurnConvPosChk, IO_OFF);
// 				CTL_Lib.Alarm_Error_Occurrence(460, dWARNING, st_alarm_info.strCode);
// 				// 			m_dwConveyorWaitTime[CONV_MID][0] = GetCurrentTime();
// 				// 			m_nErrorStep = m_nRunStep[CONV_MID];
// 				// 			m_nRunStep[CONV_MID] = 5000;
// 			}
// 		}
		//kwlee 2017.0313
		if (nRet_1 == RET_RETRY)
		{
			m_nRunStep[CONV_MID] = 1500;
		}
		else if (nRet_1 == RET_GOOD)
		{
			if (st_sync_info.TurnConvJobReady[ROBOT] == CTL_READY)
			{
				OnSetTableTurnCylOnOff(IO_RUN_MODE,IO_OFF);
				m_nRunStep[CONV_MID] = 2630;
			}
		}
		else if (nRet_1 == RET_ERROR)
		{
			//m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_TurnConvPosChk, IO_OFF);
			CTL_Lib.Alarm_Error_Occurrence(460, dWARNING, st_alarm_info.strCode);
			// 			m_dwConveyorWaitTime[CONV_MID][0] = GetCurrentTime();
			// 			m_nErrorStep = m_nRunStep[CONV_MID];
			// 			m_nRunStep[CONV_MID] = 5000;
		}
		break;

	case 2630:
		nRet[0] = OnGetTableTurnCylOnOff(IO_RUN_MODE, IO_OFF);

		if(nRet[0] == RET_GOOD)
		{
			if (st_basic_info.nPcbTurnEnable == PCB_TURN || st_basic_info.nPcbType == UDIMM_10 || 
				st_basic_info.nPcbType == SODIMM || st_basic_info.nPcbType == RDIMM)
			{
				st_handler_info.cWndMain->PostMessage(WM_PCB_CV_TURN_MOVE_DRAW_MAIN,TURN_PCB_CONV_REVERSE,0); //kwlee 2017.0220
				st_sync_info.TurnConvJobReady[CONVEYOR] = CTL_COMPLETE;
				m_nRunStep[CONV_MID] = 2640;
			}
			else
			{
				m_nRunStep[CONV_MID] = 2900;
			}
		}
		else if(nRet[0] == RET_ERROR)
		{
			//501130 1 A "TURN CONV Position Down Off Check Error."
			//501131 1 A "TURN CONV Position Down On Check Error."
			CTL_Lib.Alarm_Error_Occurrence(470, dWARNING, m_strAlarmCode);
		}
		break;
		///////////////////////////////////////////////////////

	case 2640:
		//if (st_sync_info.TurnConvJobReady[ROBOT] == CTL_COMPLETE)
	//	{
		//////////////////////////////
		//Turn Conv Pitch Safety 위치 이동.
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_TURN_CONV_PITCH, st_motor_info[M_TURN_CONV_PITCH].d_pos[TURN_CONVEYOR_REVERSE_SAFETY], COMI.mn_runspeed_rate);

		if (nRet_1 == BD_GOOD) 
		{
			//m_nRunStep[CONV_MID] = 2700;
			//m_nRunStep[CONV_MID] = 1500;
			m_nRunStep[CONV_MID] = 2650;

			st_sync_info.TurnConvJobReady[CONVEYOR] = CTL_NONE;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(750, dWARNING, st_alarm_info.strCode);
		//	m_nRunStep[CONV_MID] = 0;
		}
	//	}
		////////////////////////////////////////////////////////
		break;

	case 2650:
		OnSetTurnConvStopperUpDn(IO_RUN_MODE, IO_ON);
		m_nRunStep[CONV_MID] = 2660;
			
		break;

	case 2660:
		nRet[0] = OnGetTurnConvStopperUpDn(IO_RUN_MODE, IO_ON);

		if(nRet[0] == RET_GOOD)
		{
			m_nRunStep[CONV_MID] = 2700;
		}
		else if(nRet[0] == RET_ERROR)
		{
			//501100 1 A "IN CONV Position Up Off Check Error."
			//501101 1 A "IN CONV Position Up On Check Error."
			//	m_strAlarmCode.Format(_T("5%d%04d"), IO_ON, st_io_info.i_InConvPosUpChk);
			CTL_Lib.Alarm_Error_Occurrence(440, dWARNING, m_strAlarmCode);
		}
		break;

	case 2700:
		//Motor Cylinder For
		OnSetTableMotorCylOnOff(IO_RUN_MODE,IO_ON);
		m_nRunStep[CONV_MID] = 2800;
		break;

	case 2800:
		nRet[0] = OnGetTableMotorCylOnOff(IO_RUN_MODE, IO_ON);

		if(nRet[0] == RET_GOOD)
		{
			/*st_sync_info.nMidTrayIn = CONV_REQ;*/
			//m_nRunStep[CONV_MID] = 2900;
			m_nRunStep[CONV_MID] = 2810;
		}
		else if(nRet[0] == RET_ERROR)
		{
			//502060 1 A "TURN CONV Table Motor Forward Cylinder-Sol Error."
			//502070 1 A "TURN CONV Table Motor Backward Cylinder-Sol Error."
			CTL_Lib.Alarm_Error_Occurrence(203, dWARNING, m_strAlarmCode);
		}
		break;

		/////////////////////////////////////////////////
		// TURN MOTOR ON, CCW
	case 2810:
		FAS_IO.set_out_bit(st_io_info.o_TurnMotorDirection,IO_ON);
		FAS_IO.set_out_bit(st_io_info.o_TablePcbMovCcw,IO_ON);
		FAS_IO.set_out_bit(st_io_info.o_TablePcbMovCw,IO_OFF);

		m_dwConveyorWaitTime[CONV_MID][0] = GetCurrentTime();

		m_nRunStep[CONV_MID] = 2820;
		break;

	case 2820:
		//Turn Conv Pos 센서 확인
// 		if (st_basic_info.nModeDevice == WITHOUT_DVC)
// 		{
// 			//st_handler_info.cWndMain->PostMessage(WM_PCB_CV_TURN_MOVE_DRAW_MAIN,TURN_PCB_CONV_WORK_POS,0);
// 			m_dwConveyorWaitTime[CONV_MID][0] = GetCurrentTime();				
// 			m_nRunStep[CONV_MID] = 2830;
// 		}
// 		else
// 		{
// 			if (FAS_IO.get_in_bit(st_io_info.i_TurnConvPosChk,IO_ON) == IO_ON)
// 			{
// 				//
// 				m_dwConveyorWaitTime[CONV_MID][0] = GetCurrentTime();				
// 				m_nRunStep[CONV_MID] = 2830;
// 			}
// 			else
// 			{
// 				nRet_1 = OnConvWaitTime(CONV_MID,WAIT_CONV_IN);
// 				if(nRet_1 == RET_ERROR)
// 				{
// 					//502110 1 A "TURN CONV  Position Sensor Off Check Error."
// 					//502111 1 A "TURN CONV  Position Sensor On Check Error."
// 					m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_TurnConvPosChk, IO_OFF);
// 					//	CTL_Lib.Alarm_Error_Occurrence(460, dWARNING, m_strAlarmCode);
// 					m_nErrorStep = m_nRunStep[CONV_MID];
// 					m_nRunStep[CONV_MID] = 5000;
// 					//m_nRunStep[CONV_MID] = 5000;
// 				}
// 			}
// 		}
		//kwlee 2017.0313
		if (FAS_IO.get_in_bit(st_io_info.i_TurnConvPosChk,IO_ON) == IO_ON)
		{
			//
			m_dwConveyorWaitTime[CONV_MID][0] = GetCurrentTime();				
			m_nRunStep[CONV_MID] = 2830;
		}
		else
		{
			nRet_1 = OnConvWaitTime(CONV_MID,WAIT_CONV_IN);
			if(nRet_1 == RET_ERROR)
			{
				//502110 1 A "TURN CONV  Position Sensor Off Check Error."
				//502111 1 A "TURN CONV  Position Sensor On Check Error."
				m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_TurnConvPosChk, IO_OFF);
				//	CTL_Lib.Alarm_Error_Occurrence(460, dWARNING, m_strAlarmCode);
				m_nErrorStep = m_nRunStep[CONV_MID];
				m_nRunStep[CONV_MID] = 5000;
				//m_nRunStep[CONV_MID] = 5000;
			}
		}
		break;

	case 2830:
		m_dwConveyorWaitTime[CONV_MID][1] = GetCurrentTime();
		m_dwConveyorWaitTime[CONV_MID][2] = m_dwConveyorWaitTime[CONV_MID][1] - m_dwConveyorWaitTime[CONV_MID][0];

		if (m_dwConveyorWaitTime[CONV_MID][2] <= 0)
		{
			m_dwConveyorWaitTime[CONV_MID][0] = GetCurrentTime();
		}
		//Turn Pos 감지 시간 확인
		if (m_dwConveyorWaitTime[CONV_MID][2] > (DWORD)st_wait_info.nOnWaitTime[WAIT_CONV_IN])
		{
// 			if (st_basic_info.nModeDevice == WITHOUT_DVC)
// 			{
// 				//st_handler_info.cWndMain->PostMessage(WM_PCB_CV_TURN_MOVE_DRAW_MAIN,TURN_PCB_CONV_REVERSE,0);
// 				//st_handler_info.cWndMain->PostMessage(WM_PCB_CV_TURN_MOVE_DRAW_MAIN,TURN_PCB_CONV_WORK_POS,0); //kwlee 2017.0220
// 				FAS_IO.set_out_bit(st_io_info.o_TurnMotorDirection,IO_OFF);
// 				m_dwConveyorWaitTime[CONV_MID][0] = GetCurrentTime();
// 				//st_sync_info.nMidTrayIn = CTL_NO;
// 				m_nRunStep[CONV_MID] = 2840;
// 			}
// 			else
// 			{
// 				if (FAS_IO.get_in_bit(st_io_info.i_TurnConvPosChk,IO_ON) == IO_ON)
// 				{
// 					//st_handler_info.cWndMain->PostMessage(WM_PCB_CV_TURN_MOVE_DRAW_MAIN,TURN_PCB_CONV_WORK_POS,0);
// 					//st_handler_info.cWndMain->PostMessage(WM_PCB_CV_TURN_MOVE_DRAW_MAIN,TURN_PCB_CONV_REVERSE,0);
// 					FAS_IO.set_out_bit(st_io_info.o_TurnMotorDirection,IO_OFF);
// 					m_dwConveyorWaitTime[CONV_MID][0] = GetCurrentTime();
// 					//st_sync_info.nMidTrayIn = CTL_NO;
// 					m_nRunStep[CONV_MID] = 2840;
// 				}
// 				else
// 				{
// 					m_nRunStep[CONV_MID] = 2820;
// 				}
// 			}
			//kwlee 2017.0313
			if (FAS_IO.get_in_bit(st_io_info.i_TurnConvPosChk,IO_ON) == IO_ON)
			{
				//st_handler_info.cWndMain->PostMessage(WM_PCB_CV_TURN_MOVE_DRAW_MAIN,TURN_PCB_CONV_WORK_POS,0);
				//st_handler_info.cWndMain->PostMessage(WM_PCB_CV_TURN_MOVE_DRAW_MAIN,TURN_PCB_CONV_REVERSE,0);
				FAS_IO.set_out_bit(st_io_info.o_TurnMotorDirection,IO_OFF);
				m_dwConveyorWaitTime[CONV_MID][0] = GetCurrentTime();
				//st_sync_info.nMidTrayIn = CTL_NO;
				m_nRunStep[CONV_MID] = 2840;
			}
			else
			{
				m_nRunStep[CONV_MID] = 2820;
			}
		}
		break;

	case 2840:
		m_dwConveyorWaitTime[CONV_MID][1] = GetCurrentTime();
		m_dwConveyorWaitTime[CONV_MID][2] = m_dwConveyorWaitTime[CONV_MID][1] - m_dwConveyorWaitTime[CONV_MID][0];

		if (m_dwConveyorWaitTime[CONV_MID][2] <= 0)
		{
			m_dwConveyorWaitTime[CONV_MID][0] = GetCurrentTime();
		}
		if (m_dwConveyorWaitTime[CONV_MID][2] > (DWORD)st_wait_info.nOffWaitTime[WAIT_CONV_IN]/*[IO_OFF]*/)
		{
			m_nRunStep[CONV_MID] = 2850;
		}
		break;

	case 2850:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_TURN_CONV_PITCH, st_motor_info[M_TURN_CONV_PITCH].d_pos[TURN_CONVEYOR_WORK], COMI.mn_runspeed_rate);

		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			st_sync_info.TurnConvJobReady[CONVEYOR] = CTL_READY;
			m_nRunStep[CONV_MID] = 2860;

		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(750, dWARNING, st_alarm_info.strCode);
			//m_nRunStep[CONV_MID] = 0;
		}
		break;

	case 2860:
		OnSetTurnConvStopperUpDn(IO_RUN_MODE, IO_OFF);
		m_nRunStep[CONV_MID] = 2870;
		break;

	case 2870:
		nRet[0] = OnGetTurnConvStopperUpDn(IO_RUN_MODE, IO_OFF);

		if(nRet[0] == RET_GOOD)
		{
			m_nRunStep[CONV_MID] = 2880;
		}
		else if(nRet[0] == RET_ERROR)
		{
			//501100 1 A "IN CONV Position Up Off Check Error."
			//501101 1 A "IN CONV Position Up On Check Error."
			//	m_strAlarmCode.Format(_T("5%d%04d"), IO_ON, st_io_info.i_InConvPosUpChk);
			CTL_Lib.Alarm_Error_Occurrence(440, dWARNING, m_strAlarmCode);
		}
		break;

	case 2880:
		//Motor Cylinder For
		OnSetTableMotorCylOnOff(IO_RUN_MODE,IO_OFF);
		m_nRunStep[CONV_MID] = 2890;
		break;

	case 2890:
		nRet[0] = OnGetTableMotorCylOnOff(IO_RUN_MODE, IO_OFF);

		if(nRet[0] == RET_GOOD)
		{
			/*st_sync_info.nMidTrayIn = CONV_REQ;*/
			//m_nRunStep[CONV_MID] = 2900;
			m_nRunStep[CONV_MID] = 2900;
		}
		else if(nRet[0] == RET_ERROR)
		{
			//502060 1 A "TURN CONV Table Motor Forward Cylinder-Sol Error."
			//502070 1 A "TURN CONV Table Motor Backward Cylinder-Sol Error."
			CTL_Lib.Alarm_Error_Occurrence(203, dWARNING, m_strAlarmCode);
		}
		break;


	case 2900:
		////////////////////////////////////////////
		//Conv Turn Forward
		/*m_TurnConvPosCheck[SECOND] = CTL_NO;*/
		nRet_1 = OnTurnConvPosCheck();
		
		if (nRet_1 == RET_RETRY)
		{
			m_nRunStep[CONV_MID] = 2650;
		}
		else if (nRet_1 == RET_GOOD)
		{
			if (st_sync_info.TurnConvJobReady[ROBOT] == CTL_READY)
			{
				st_sync_info.nConvTurnCheck = FALSE; //kwlee 2017.0220
				st_handler_info.cWndMain->PostMessage(WM_PCB_CV_TURN_MOVE_DRAW_MAIN,TURN_PCB_CONV_TURN,0); //kwlee 2017.0220
				st_sync_info.TurnConvJobReady[CONVEYOR] = CTL_COMPLETE;
				OnSetTableTurnCylOnOff(IO_RUN_MODE,IO_ON);
				m_nRunStep[CONV_MID] = 2910;
			}
		}
		else
		{
			//m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_TurnConvPosChk, IO_OFF);
			CTL_Lib.Alarm_Error_Occurrence(460, dWARNING, st_alarm_info.strCode);
// 			m_dwConveyorWaitTime[CONV_MID][0] = GetCurrentTime();
// 			m_nErrorStep = m_nRunStep[CONV_MID];
// 			m_nRunStep[CONV_MID] = 5000;
		}
		break;

	case 2910:
		nRet[0] = OnGetTableTurnCylOnOff(IO_RUN_MODE, IO_ON);

		if(nRet[0] == RET_GOOD)
		{
			//st_sync_info.TurnConvJobReady[CONVEYOR] = CTL_COMPLETE;
			m_nRunStep[CONV_MID] = 2920;
		}
		else if(nRet[0] == RET_ERROR)
		{
			//501130 1 A "TURN CONV Position Down Off Check Error."
			//501131 1 A "TURN CONV Position Down On Check Error."
			CTL_Lib.Alarm_Error_Occurrence(470, dWARNING, m_strAlarmCode);
			
		}
		break;

	case 2920:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_TURN_CONV_PITCH, st_motor_info[M_TURN_CONV_PITCH].d_pos[TURN_CONVEYOR_REVERSE_SAFETY], COMI.mn_runspeed_rate);

		if (nRet_1 == BD_GOOD) 
		{
			st_handler_info.cWndMain->PostMessage(WM_PCB_CV_TURN_MOVE_DRAW_MAIN,OUT_CONV_IN_ING,0); //kwlee 2017.0220
			//m_nRunStep[CONV_MID] = 2700;
			//m_nRunStep[CONV_MID] = 1500;
			m_nRunStep[CONV_MID] = 2930;
			m_dwConveyorWaitTime[CONV_MID][0] = GetCurrentTime();
			
			st_sync_info.TurnConvJobReady[CONVEYOR] = CTL_NONE;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(750, dWARNING, st_alarm_info.strCode);
		//	m_nRunStep[CONV_MID] = 0;
		}
		break;

	case 2930:
		//Motor Cylinder For
		OnSetTableMotorCylOnOff(IO_RUN_MODE,IO_ON);
		m_nRunStep[CONV_MID] = 2940;
		break;

	case 2940:
		nRet[0] = OnGetTableMotorCylOnOff(IO_RUN_MODE, IO_ON);

		if(nRet[0] == RET_GOOD)
		{
			/*st_sync_info.nMidTrayIn = CONV_REQ;*/
			//m_nRunStep[CONV_MID] = 2900;
			m_nRunStep[CONV_MID] = 2950;
		}
		else if(nRet[0] == RET_ERROR)
		{
			//502060 1 A "TURN CONV Table Motor Forward Cylinder-Sol Error."
			//502070 1 A "TURN CONV Table Motor Backward Cylinder-Sol Error."
			CTL_Lib.Alarm_Error_Occurrence(203, dWARNING, m_strAlarmCode);
		}
		break;
		/////////////////////////////
		///////////////////////////////////////////////////////////////////
	case 2950:
		//if (st_sync_info.nRearTrayIn == CONV_READY) //Rear Conv //투입 요청 신호
		if (st_sync_info.nRearTrayIn == CONV_REQ)
		{
			FAS_IO.set_out_bit(st_io_info.o_TurnMotorDirection,IO_ON);
			FAS_IO.set_out_bit(st_io_info.o_TablePcbMovCcw,IO_ON);
			FAS_IO.set_out_bit(st_io_info.o_TablePcbMovCw,IO_OFF);
		
			st_sync_info.nMidTrayIn = CTL_READY;
			
			m_nRunStep[CONV_MID] = 3000;
		}
		else
		{
			nRet_1 = OnConvWaitTime(CONV_MID,WAIT_CONV_REQ);
			if(nRet_1 == RET_ERROR)
			{
				//502110 1 A "TURN CONV  Position Sensor Off Check Error."
				//502111 1 A "TURN CONV  Position Sensor On Check Error."
				m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_TurnConvPosChk, IO_OFF);
				CTL_Lib.Alarm_Error_Occurrence(480, dWARNING, m_strAlarmCode);
				m_nErrorStep = m_nRunStep[CONV_MID];
				m_nRunStep[CONV_MID] = 5000;
			//	m_nRunStep[CONV_MID] = 5000;
			}
		}
		break;

	case 3000:
		OnSetTurnConvStopperUpDn(IO_RUN_MODE, IO_OFF);
		/*st_sync_info.nMidTrayIn = CONV_REQ;*/
		//st_sync_info.nMidTrayIn = CTL_READY;
		m_nRunStep[CONV_MID] = 3100;
		break;

	case 3100:
		nRet[0] = OnGetTurnConvStopperUpDn(IO_RUN_MODE, IO_OFF);

		if(nRet[0] == RET_GOOD)
		{
			m_dwConveyorWaitTime[CONV_MID][0] = GetCurrentTime();
			m_nRunStep[CONV_MID] = 3200;
			
		}
		else if(nRet[0] == RET_ERROR)
		{
			//501130 1 A "TURN CONV Position Down Off Check Error."
			//501131 1 A "TURN CONV Position Down On Check Error."
			CTL_Lib.Alarm_Error_Occurrence(201, dWARNING, m_strAlarmCode);
		}
		break;

	case 3200:
// 		if (st_basic_info.nModeDevice == WITHOUT_DVC)
// 		{
// 			m_dwConveyorWaitTime[CONV_MID][0] = GetCurrentTime();
// 			m_nRunStep[CONV_MID] = 3300;
// 		}
// 		else
// 		{
// 			if (FAS_IO.get_in_bit(st_io_info.i_TurnConvPosChk,IO_ON) == IO_OFF)
// 			{
// 				m_dwConveyorWaitTime[CONV_MID][0] = GetCurrentTime();
// 				m_nRunStep[CONV_MID] = 3300;
// 			}
// 			else
// 			{
// 				nRet_1 = OnConvWaitTime(CONV_MID,WAIT_CONV_OUT);
// 				if(nRet_1 == RET_ERROR)
// 				{
// 					//502110 1 A "TURN CONV  Position Sensor Off Check Error."
// 					//502111 1 A "TURN CONV  Position Sensor On Check Error."
// 					m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_TurnConvPosChk, IO_ON);
// 					//CTL_Lib.Alarm_Error_Occurrence(490, dWARNING, m_strAlarmCode);
// 					m_nErrorStep = m_nRunStep[CONV_MID];
// 					m_nRunStep[CONV_MID] = 5000;
// 				}
// 			}
// 		}
		//kwlee 2017.0313
		if (FAS_IO.get_in_bit(st_io_info.i_TurnConvPosChk,IO_ON) == IO_OFF)
		{
			m_dwConveyorWaitTime[CONV_MID][0] = GetCurrentTime();
			m_nRunStep[CONV_MID] = 3300;
		}
		else
		{
			nRet_1 = OnConvWaitTime(CONV_MID,WAIT_CONV_OUT);
			if(nRet_1 == RET_ERROR)
			{
				//502110 1 A "TURN CONV  Position Sensor Off Check Error."
				//502111 1 A "TURN CONV  Position Sensor On Check Error."
				m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_TurnConvPosChk, IO_ON);
				//CTL_Lib.Alarm_Error_Occurrence(490, dWARNING, m_strAlarmCode);
				m_nErrorStep = m_nRunStep[CONV_MID];
				m_nRunStep[CONV_MID] = 5000;
			}
		}
		break;

	case 3300:
		m_dwConveyorWaitTime[CONV_MID][1] = GetCurrentTime();
		m_dwConveyorWaitTime[CONV_MID][2] = m_dwConveyorWaitTime[CONV_MID][1] - m_dwConveyorWaitTime[CONV_MID][0];

		if (m_dwConveyorWaitTime[CONV_MID][2] <= 0)
		{
			m_dwConveyorWaitTime[CONV_MID][0] = GetCurrentTime();
		}

		if (m_dwConveyorWaitTime[CONV_MID][2] > 1000)
		{
// 			if (st_basic_info.nModeDevice == WITHOUT_DVC)
// 			{
// 				st_sync_info.nRearTrayIn = CTL_NO;
// 				FAS_IO.set_out_bit(st_io_info.o_TurnMotorDirection,IO_OFF);
// 				st_sync_info.nMidTrayIn = CTL_NO;
// 				m_nRunStep[CONV_MID] = 0;
// 			}
// 			else
// 			{
// 				if (FAS_IO.get_in_bit(st_io_info.i_TurnConvPosChk,IO_ON) == IO_ON)
// 				{
// 					m_nRunStep[CONV_MID] = 3200;
// 				}
// 				else
// 				{
// 					//	st_handler_info.cWndMain->PostMessage(WM_PCB_CV_TURN_MOVE_DRAW_MAIN,CONV_IN_READY,0); //kwlee 2017.0220
// 					st_sync_info.nRearTrayIn = CTL_NO;
// 					FAS_IO.set_out_bit(st_io_info.o_TurnMotorDirection,IO_OFF);
// 					st_sync_info.nMidTrayIn = CTL_NO;
// 					m_nRunStep[CONV_MID] = 0;
// 				}
// 			}
			//kwlee 2013.0313
			if (FAS_IO.get_in_bit(st_io_info.i_TurnConvPosChk,IO_ON) == IO_ON)
			{
				m_nRunStep[CONV_MID] = 3200;
			}
			else
			{
				//	st_handler_info.cWndMain->PostMessage(WM_PCB_CV_TURN_MOVE_DRAW_MAIN,CONV_IN_READY,0); //kwlee 2017.0220
				st_sync_info.nRearTrayIn = CTL_NO;
				FAS_IO.set_out_bit(st_io_info.o_TurnMotorDirection,IO_OFF);
				st_sync_info.nMidTrayIn = CTL_NO;
				m_nRunStep[CONV_MID] = 0;
			}
		}
		break;

		////////////////////////////////////////
	case 5000:
		FAS_IO.set_out_bit(st_io_info.o_TurnMotorDirection,IO_OFF);
		m_nRunStep[CONV_MID] = 5100;
		break;

	case 5100:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_TURN_CONV_PITCH, st_motor_info[M_TURN_CONV_PITCH].d_pos[TURN_CONVEYOR_REVERSE_SAFETY], COMI.mn_runspeed_rate);

		if (nRet_1 == BD_GOOD) 
		{
			//m_nRunStep[CONV_MID] = 2700;
			//m_nRunStep[CONV_MID] = 1500;
			m_nRunStep[CONV_MID] = 5200;
		
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(750, dWARNING, st_alarm_info.strCode);
			//	m_nRunStep[CONV_MID] = 0;
		}
		break;

	case 5200:
		OnSetTurnConvStopperUpDn(IO_RUN_MODE, IO_OFF);
		m_nRunStep[CONV_MID] = 5300;
		break;

	case 5300:
		nRet[0] = OnGetTurnConvStopperUpDn(IO_RUN_MODE, IO_OFF);

		if(nRet[0] == RET_GOOD)
		{
			m_nRunStep[CONV_MID] = m_nErrorStep;
			CTL_Lib.Alarm_Error_Occurrence(440, dWARNING, m_strAlarmCode);
		}
		else if(nRet[0] == RET_ERROR)
		{
			//501100 1 A "IN CONV Position Up Off Check Error."
			//501101 1 A "IN CONV Position Up On Check Error."
			//	m_strAlarmCode.Format(_T("5%d%04d"), IO_ON, st_io_info.i_InConvPosUpChk);
			CTL_Lib.Alarm_Error_Occurrence(440, dWARNING, m_strAlarmCode);
		}
		break;
		///////////////////////////////////////////////////////////
	
	}

}
int CRunConveyor::OnTurnConvPosCheck()
{
	////////////////////////////////////////
	int nRet_1 = 0;
	int nRec[8] = {0,};
	int nRet = 0;
	// 	if (st_basic_info.nModeDevice == WITHOUT_DVC)
	// 	{
	// 		nRec[0] = COMI.Check_MotPosRange(M_PICKERRBT_X,st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_SAFETY],st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_SAFETY] + st_motor_info[M_PICKERRBT_X].d_allow);
	// 		nRec[1] = COMI.Check_MotPosRange(M_PICKERRBT_Y, st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_SAFETY],st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_SAFETY] + st_motor_info[M_PICKERRBT_Y].d_allow);
	// 		if (nRec[0] == BD_GOOD && nRec[1] == BD_GOOD)
	// 		{
	// 			return RET_GOOD;
	// 		}
	// 		else
	// 		{
	// 			return RET_ERROR;
	//  		}
	// 	}
	// 	else
	// 	{
	// 		nRet_1 = FAS_IO.get_in_bit(st_io_info.i_TurnConvPosChk,IO_ON);
	// 
	// 		if (nRet_1 == IO_OFF)
	// 		{
	// 			nRec[0] = COMI.Check_MotPosRange(M_PICKERRBT_X,st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_FIRST],st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_FIRST] + st_motor_info[M_PICKERRBT_X].d_allow);
	// 			nRec[1] = COMI.Check_MotPosRange(M_PICKERRBT_Y, st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_FIRST],st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_FIRST] + st_motor_info[M_PICKERRBT_Y].d_allow);
	// 			
	// 			nRec[2] = COMI.Check_MotPosRange(M_PICKERRBT_X,st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_SECOND],st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_SECOND] + st_motor_info[M_PICKERRBT_X].d_allow);
	// 			nRec[3] = COMI.Check_MotPosRange(M_PICKERRBT_Y, st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_SECOND],st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_SECOND] + st_motor_info[M_PICKERRBT_Y].d_allow);
	// 			
	// 			nRec[4] = COMI.Check_MotPosRange(M_PICKERRBT_X,st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_THIRD],st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_THIRD] + st_motor_info[M_PICKERRBT_X].d_allow);
	// 			nRec[5] = COMI.Check_MotPosRange(M_PICKERRBT_Y, st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_THIRD],st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_THIRD] + st_motor_info[M_PICKERRBT_Y].d_allow);
	// 			
	// 			nRec[6] = COMI.Check_MotPosRange(M_PICKERRBT_X,st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_FORTH],st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_FORTH] + st_motor_info[M_PICKERRBT_X].d_allow);
	// 			nRec[7] = COMI.Check_MotPosRange(M_PICKERRBT_Y, st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_FORTH],st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_FORTH] + st_motor_info[M_PICKERRBT_Y].d_allow);
	// 
	// 			if ((nRec[0] == BD_GOOD && nRec[1] == BD_GOOD) || (nRec[2] == BD_GOOD && nRec[3] == BD_GOOD) || 
	// 				(nRec[4] == BD_GOOD && nRec[5] == BD_GOOD) || (nRec[6] == BD_GOOD && nRec[7] == BD_GOOD))
	// 				
	// // 			nRec[0] = COMI.Check_MotPosRange(M_PICKERRBT_X,st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_SAFETY],st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_SAFETY] + st_motor_info[M_PICKERRBT_X].d_allow);
	// // 			nRec[1] = COMI.Check_MotPosRange(M_PICKERRBT_Y, st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_SAFETY],st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_SAFETY] + st_motor_info[M_PICKERRBT_Y].d_allow);
	// // 
	// // 			if (nRec[0] != BD_GOOD && nRec[1] != BD_GOOD)
	// 			{
	// 				return RET_ERROR;
	// 			}
	// 			else
	// 			{
	// 				nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_TURN_CONV_PITCH, st_motor_info[M_TURN_CONV_PITCH].d_pos[TURN_CONVEYOR_REVERSE_SAFETY], COMI.mn_runspeed_rate);
	// 
	// 				if (nRet_1 == BD_GOOD) 
	// 				{
	// 					//m_nRunStep[CONV_MID] = 2700;
	// 					//m_nRunStep[CONV_MID] = 1500;
	// 
	// 					return RET_RETRY;
	// 				}
	// 				else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
	// 				{
	// 					//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
	// 					CTL_Lib.Alarm_Error_Occurrence(750, dWARNING, st_alarm_info.strCode);
	// 					//	m_nRunStep[CONV_MID] = 0;
	// 					return RET_ERROR;
	// 				}
	// 			}
	// 		}
	// 	}
	//kwlee 2017.0313

		nRec[0] = COMI.Check_MotPosRange(M_PICKERRBT_X,st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_SAFETY],st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_SAFETY] + st_motor_info[M_PICKERRBT_X].d_allow);
		nRec[1] = COMI.Check_MotPosRange(M_PICKERRBT_Y, st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_SAFETY],st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_SAFETY] + st_motor_info[M_PICKERRBT_Y].d_allow);
		if (nRec[0] == BD_GOOD && nRec[1] == BD_GOOD)
		{
			return RET_GOOD;
		}
		else
		{
			return RET_ERROR;
		}

	nRet_1 = FAS_IO.get_in_bit(st_io_info.i_TurnConvPosChk,IO_ON);
	if (nRet_1 == IO_OFF)
	{
		nRec[0] = COMI.Check_MotPosRange(M_PICKERRBT_X,st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_FIRST],st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_FIRST] + st_motor_info[M_PICKERRBT_X].d_allow);
		nRec[1] = COMI.Check_MotPosRange(M_PICKERRBT_Y, st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_FIRST],st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_FIRST] + st_motor_info[M_PICKERRBT_Y].d_allow);

		nRec[2] = COMI.Check_MotPosRange(M_PICKERRBT_X,st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_SECOND],st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_SECOND] + st_motor_info[M_PICKERRBT_X].d_allow);
		nRec[3] = COMI.Check_MotPosRange(M_PICKERRBT_Y, st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_SECOND],st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_SECOND] + st_motor_info[M_PICKERRBT_Y].d_allow);

		nRec[4] = COMI.Check_MotPosRange(M_PICKERRBT_X,st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_THIRD],st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_THIRD] + st_motor_info[M_PICKERRBT_X].d_allow);
		nRec[5] = COMI.Check_MotPosRange(M_PICKERRBT_Y, st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_THIRD],st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_THIRD] + st_motor_info[M_PICKERRBT_Y].d_allow);

		nRec[6] = COMI.Check_MotPosRange(M_PICKERRBT_X,st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_FORTH],st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_FORTH] + st_motor_info[M_PICKERRBT_X].d_allow);
		nRec[7] = COMI.Check_MotPosRange(M_PICKERRBT_Y, st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_FORTH],st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_FORTH] + st_motor_info[M_PICKERRBT_Y].d_allow);

		if ((nRec[0] == BD_GOOD && nRec[1] == BD_GOOD) || (nRec[2] == BD_GOOD && nRec[3] == BD_GOOD) || 
			(nRec[4] == BD_GOOD && nRec[5] == BD_GOOD) || (nRec[6] == BD_GOOD && nRec[7] == BD_GOOD))

			// 			nRec[0] = COMI.Check_MotPosRange(M_PICKERRBT_X,st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_SAFETY],st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_SAFETY] + st_motor_info[M_PICKERRBT_X].d_allow);
			// 			nRec[1] = COMI.Check_MotPosRange(M_PICKERRBT_Y, st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_SAFETY],st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_SAFETY] + st_motor_info[M_PICKERRBT_Y].d_allow);
			// 
			// 			if (nRec[0] != BD_GOOD && nRec[1] != BD_GOOD)
		{
			return RET_ERROR;
		}
		else
 		{			

// 			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_TURN_CONV_PITCH, st_motor_info[M_TURN_CONV_PITCH].d_pos[TURN_CONVEYOR_REVERSE_SAFETY], COMI.mn_runspeed_rate);
// 			if (nRet_1 == BD_GOOD) 
// 			{
// 				//m_nRunStep[CONV_MID] = 2700;
// 				//m_nRunStep[CONV_MID] = 1500;
// 
// 				return RET_RETRY;
// 				
// 			}
// 			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
// 			{
// 				//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
// 				CTL_Lib.Alarm_Error_Occurrence(750, dWARNING, st_alarm_info.strCode);
// 				//	m_nRunStep[CONV_MID] = 0;
// 				return RET_ERROR;
// 				
// 			}
			//kwlee 2017.0317
			return RET_RETRY;
		}
	}
}
void CRunConveyor::OnRearConvMove()
{
	int nRet[5] = {0,};
	int nRet_1;

	switch(m_nRunStep[CONV_OUT])
	{
	case 0:
		m_nRunStep[CONV_OUT] = 100;
		break;

	case 100:
// 		if (st_sync_info.nMidTrayIn == CTL_READY)
// 		{
			//트레이가 없는지 확인
			nRet[0] = FAS_IO.get_in_bit(st_io_info.i_OutConvChk,	IO_OFF);//ps100 //트레이가 장비에 진입시 감지 됨
			nRet[1] = FAS_IO.get_in_bit(st_io_info.i_OutConvPosUpChk,IO_OFF);//ps101 //트레이가 장비에 진입시 감지 됨
			nRet[2] = FAS_IO.get_in_bit(st_io_info.i_OutInConvPosChk,IO_OFF);

			if(nRet[0] == IO_OFF && nRet[1] == IO_OFF && nRet[2] == IO_OFF)
			{
				m_nRunStep[CONV_OUT] = 110;
				st_sync_info.nRearTrayIn = CONV_REQ;
			}
			else
			{
				//502080 1 A "OUT CONV Sensor Off Check Error."
				//502081 1 A "OUT CONV Sensor On Check Error."
				//502090 1 A "OUT CONV In Position Sensor Off Check Error."
				//502091 1 A "OUT CONV In Position Sensor On Check Error.
				//502100 1 A "OUT CONV  Position Up Sensor Off Check Error."
				//502101 1 A "OUT CONV  Position Up Sensor On Check Error."
			
				if	   (nRet[0] == IO_ON) m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_OutConvChk, IO_OFF); //
				else if(nRet[1] == IO_ON) m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_OutConvPosUpChk, IO_OFF);
				else if(nRet[2] == IO_ON) m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_OutInConvPosChk, IO_OFF);
				CTL_Lib.Alarm_Error_Occurrence(500, dWARNING, m_strAlarmCode);		
		//	}		
		}		
		break;

	case 110:
		if (st_sync_info.nMidTrayIn == CTL_READY)
		{
			OnSetOutConvStopperUpDn(IO_RUN_MODE, IO_ON);
			m_nRunStep[CONV_OUT] = 1000;
		}
		break;

	case 1000:
		nRet[0] = OnGetOutConvStopperUpDn(IO_RUN_MODE, IO_ON);

		if(nRet[0] == RET_GOOD)
		{
			
			//st_sync_info.nRearTrayIn = CONV_READY;
			
			m_nRunStep[CONV_OUT] = 1100;
		}
		else if(nRet[0] == RET_ERROR)
		{
			//502100 1 A "OUT CONV  Position Up Sensor Off Check Error."
			//502101 1 A "OUT CONV  Position Up Sensor On Check Error."
			
			CTL_Lib.Alarm_Error_Occurrence(510, dWARNING, m_strAlarmCode);
		}
		break;

	case 1100:
		FAS_IO.set_out_bit(st_io_info.o_OutConvMotorOn,IO_ON);

		m_dwConveyorWaitTime[CONV_OUT][0] = GetCurrentTime();
		st_handler_info.cWndMain->PostMessage(WM_PCB_CV_OUT_MOVE_DRAW_MAIN,OUT_CONV_OUT_POS,0); //kwlee 2017.0220
		m_nRunStep[CONV_OUT] = 1200;
		break;

	case 1200:
// 		if (st_basic_info.nModeDevice == WITHOUT_DVC)
// 		{
// 			st_handler_info.cWndMain->PostMessage(WM_PCB_CV_OUT_MOVE_DRAW_MAIN,OUT_CONV_OUT_POS,0); //kwlee 2017.0220
// 			m_dwConveyorWaitTime[CONV_OUT][0] = GetCurrentTime();				
// 			m_nRunStep[CONV_OUT] = 1300;
// 		}
// 		else
// 		{
// 			if (FAS_IO.get_in_bit(st_io_info.i_OutConvChk,IO_ON) == IO_ON)
// 			{
// 				st_handler_info.cWndMain->PostMessage(WM_PCB_CV_OUT_MOVE_DRAW_MAIN,OUT_CONV_OUT_POS,0); //kwlee 2017.0220
// 				m_dwConveyorWaitTime[CONV_OUT][0] = GetCurrentTime();				
// 				m_nRunStep[CONV_OUT] = 1300;
// 			}
// 			else
// 			{
// 				nRet_1 = OnConvWaitTime(CONV_OUT,WAIT_CONV_IN);
// 				if(nRet_1 == RET_ERROR)
// 				{
// 					//502080 1 A "OUT CONV Sensor Off Check Error."
// 					//502081 1 A "OUT CONV Sensor On Check Error."
// 					m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_OutConvChk, IO_OFF);
// 					CTL_Lib.Alarm_Error_Occurrence(520, dWARNING, m_strAlarmCode);
// 
// 					FAS_IO.set_out_bit(st_io_info.o_OutConvMotorOn,IO_OFF);
// 					//m_nRunStep[CONV_OUT] = 0;
// 				}
// 			}
// 		}
		//kwlee 2017.0313
		if (FAS_IO.get_in_bit(st_io_info.i_OutConvChk,IO_ON) == IO_ON)
		{
			st_handler_info.cWndMain->PostMessage(WM_PCB_CV_OUT_MOVE_DRAW_MAIN,OUT_CONV_OUT_POS,0); //kwlee 2017.0220
			m_dwConveyorWaitTime[CONV_OUT][0] = GetCurrentTime();				
			m_nRunStep[CONV_OUT] = 1300;
		}
		else
		{
			nRet_1 = OnConvWaitTime(CONV_OUT,WAIT_CONV_IN);
			if(nRet_1 == RET_ERROR)
			{
				//502080 1 A "OUT CONV Sensor Off Check Error."
				//502081 1 A "OUT CONV Sensor On Check Error."
				m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_OutConvChk, IO_OFF);
				CTL_Lib.Alarm_Error_Occurrence(520, dWARNING, m_strAlarmCode);

				FAS_IO.set_out_bit(st_io_info.o_OutConvMotorOn,IO_OFF);
				//m_nRunStep[CONV_OUT] = 0;
			}
		}
		break;

	case 1300:
		m_dwConveyorWaitTime[CONV_OUT][1] = GetCurrentTime();
		m_dwConveyorWaitTime[CONV_OUT][2] = m_dwConveyorWaitTime[CONV_OUT][1] - m_dwConveyorWaitTime[CONV_OUT][0];

		if (m_dwConveyorWaitTime[CONV_OUT][2] <= 0)
		{
			m_dwConveyorWaitTime[CONV_OUT][0] = GetCurrentTime();
		}
		if (m_dwConveyorWaitTime[CONV_OUT][2] > (DWORD)st_wait_info.nOnWaitTime[WAIT_CONV_IN])
		{
// 			if (st_basic_info.nModeDevice == WITHOUT_DVC)
// 			{
// 				st_sync_info.nSmema_Tray_Output_Req = CTL_REQ;
// 				st_handler_info.cWndMain->PostMessage(WM_PCB_CV_OUT_MOVE_DRAW_MAIN,OUT_CONV_OUT,0); //kwlee 2017.0220
// 				FAS_IO.set_out_bit(st_io_info.o_OutConvMotorOn,IO_OFF);
// 				m_dwConveyorWaitTime[CONV_OUT][0] = GetCurrentTime();
// 				m_nRunStep[CONV_OUT] = 1400;
// 			}
// 			else
// 			{
// 				if (FAS_IO.get_in_bit(st_io_info.i_OutConvChk,IO_ON) == IO_ON)
// 				{
// 					// Rear Conv 보낼 준비 완료
// 					st_sync_info.nSmema_Tray_Output_Req = CTL_REQ;
// 					st_handler_info.cWndMain->PostMessage(WM_PCB_CV_OUT_MOVE_DRAW_MAIN,OUT_CONV_OUT,0); //kwlee 2017.0220
// 					FAS_IO.set_out_bit(st_io_info.o_OutConvMotorOn,IO_OFF);
// 					m_dwConveyorWaitTime[CONV_OUT][0] = GetCurrentTime();
// 					m_nRunStep[CONV_OUT] = 1400;
// 				}
// 				else
// 				{
// 					m_nRunStep[CONV_OUT] = 1200;
// 				}
// 			}
			//kwlee 2017.0313
			if (FAS_IO.get_in_bit(st_io_info.i_OutConvChk,IO_ON) == IO_ON)
			{
				// Rear Conv 보낼 준비 완료
				st_sync_info.nSmema_Tray_Output_Req = CTL_REQ;
				st_handler_info.cWndMain->PostMessage(WM_PCB_CV_OUT_MOVE_DRAW_MAIN,OUT_CONV_OUT,0); //kwlee 2017.0220
				FAS_IO.set_out_bit(st_io_info.o_OutConvMotorOn,IO_OFF);
				m_dwConveyorWaitTime[CONV_OUT][0] = GetCurrentTime();
				m_nRunStep[CONV_OUT] = 1400;
			}
			else
			{
				m_nRunStep[CONV_OUT] = 1200;
			}
		}
		break;

	case 1400:
		m_dwConveyorWaitTime[CONV_OUT][1] = GetCurrentTime();
		m_dwConveyorWaitTime[CONV_OUT][2] = m_dwConveyorWaitTime[CONV_OUT][1] - m_dwConveyorWaitTime[CONV_OUT][0];

		if (m_dwConveyorWaitTime[CONV_OUT][2] <= 0)
		{
			m_dwConveyorWaitTime[CONV_OUT][0] = GetCurrentTime();
		}
		if (m_dwConveyorWaitTime[CONV_OUT][2] > (DWORD)st_wait_info.nOffWaitTime[WAIT_CONV_IN]/*[IO_OFF]*/)
		{
			m_nRunStep[CONV_OUT] = 1500;
		}
		break;

	case 1500:
		OnSetOutConvStopperUpDn(IO_RUN_MODE, IO_OFF);
		m_nRunStep[CONV_OUT] = 1600;
		break;

	case 1600:
		nRet[0] = OnGetOutConvStopperUpDn(IO_RUN_MODE, IO_OFF);

		if(nRet[0] == RET_GOOD)
		{
			m_nRunStep[CONV_OUT] = 1700;
		}
		else if(nRet[0] == RET_ERROR)
		{
			//502100 1 A "OUT CONV  Position Up Sensor Off Check Error."
			//502101 1 A "OUT CONV  Position Up Sensor On Check Error."
			CTL_Lib.Alarm_Error_Occurrence(530, dWARNING, m_strAlarmCode);
		}
		break;

	case 1700:
// 		if (st_basic_info.nModeDevice == WITHOUT_DVC)
// 		{
// 			st_handler_info.cWndMain->PostMessage(WM_PCB_CV_OUT_MOVE_DRAW_MAIN,CONV_IN_READY,0);
// 			FAS_IO.set_out_bit(st_io_info.o_OutConvMotorOn,IO_ON);
// 			m_nRunStep[CONV_OUT] = 2000;
// 			OnPcbDataReset();
// 			m_dwConveyorWaitTime[CONV_OUT][0] = GetCurrentTime();
// 		}
// 		else
// 		{
// 			if (st_sync_info.nSmema_Tray_Output_Req == CONV_READY) // 뒷설비에서 PCB 투입 요청 신호
// 			{
// 				st_handler_info.cWndMain->PostMessage(WM_PCB_CV_OUT_MOVE_DRAW_MAIN,CONV_IN_READY,0);
// 				OnPcbDataReset();
// 				FAS_IO.set_out_bit(st_io_info.o_OutConvMotorOn,IO_ON);
// 				m_nRunStep[CONV_OUT] = 2000;
// 				m_dwConveyorWaitTime[CONV_OUT][0] = GetCurrentTime();
// 			}
// 		}
		//kwlee 2017.0313
		if (st_sync_info.nSmema_Tray_Output_Req == CONV_READY) // 뒷설비에서 PCB 투입 요청 신호
		{
			st_handler_info.cWndMain->PostMessage(WM_PCB_CV_OUT_MOVE_DRAW_MAIN,CONV_IN_READY,0);
			OnPcbDataReset();
			FAS_IO.set_out_bit(st_io_info.o_OutConvMotorOn,IO_ON);
			m_nRunStep[CONV_OUT] = 2000;
			m_dwConveyorWaitTime[CONV_OUT][0] = GetCurrentTime();
		}
		break;

	case 2000:
		m_dwConveyorWaitTime[CONV_OUT][1] = GetCurrentTime();

		m_dwConveyorWaitTime[CONV_OUT][2] = m_dwConveyorWaitTime[CONV_OUT][1] - m_dwConveyorWaitTime[CONV_OUT][0];
		if (m_dwConveyorWaitTime[CONV_OUT][2] < (double)st_wait_info.nLimitWaitTime[WAIT_CONV_OUT])
		{
			if (FAS_IO.get_in_bit(st_io_info.i_OutConvChk,IO_ON) == IO_OFF)
			{
				m_dwConveyorWaitTime[CONV_OUT][0] = GetCurrentTime();
				st_sync_info.nSmema_Tray_Output_Req = CONV_CLR;
				m_nRunStep[CONV_OUT] = 3000;
			}
		}
		else
		{
			nRet_1 = OnConvWaitTime(CONV_OUT,WAIT_CONV_IN);
			if(nRet_1 == RET_ERROR)
			{
				m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_OutConvChk, IO_ON);
				CTL_Lib.Alarm_Error_Occurrence(560, dWARNING, m_strAlarmCode);

				FAS_IO.set_out_bit(st_io_info.o_OutConvMotorOn,IO_OFF);
				//m_nRunStep[CONV_OUT] = 0;
			}
		}
		break;

	case 3000:
		m_dwConveyorWaitTime[CONV_OUT][1] = GetCurrentTime();
		m_dwConveyorWaitTime[CONV_OUT][2] = m_dwConveyorWaitTime[CONV_OUT][1] - m_dwConveyorWaitTime[CONV_OUT][0];

		if (m_dwConveyorWaitTime[CONV_OUT][2] <= 0)
		{
			m_dwConveyorWaitTime[CONV_OUT][0] = GetCurrentTime();
		}

		if (m_dwConveyorWaitTime[CONV_OUT][2] > 1000)
		{
			if (FAS_IO.get_in_bit(st_io_info.i_OutConvChk,IO_ON) == IO_ON)
			{
				m_nRunStep[CONV_OUT] = 2000;
			}
			else
			{
				//st_handler_info.cWndMain->PostMessage(WM_PCB_CV_OUT_MOVE_DRAW_MAIN,CONV_IN_READY,0);
				FAS_IO.set_out_bit(st_io_info.o_OutConvMotorOn,IO_OFF);
				st_sync_info.nRearTrayIn = CTL_NO;
				m_nRunStep[CONV_OUT] = 0;
			}
		}
		break;

// 	case 5000:
// 		break;
	}

}