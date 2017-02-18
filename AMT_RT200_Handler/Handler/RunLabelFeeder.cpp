#include "StdAfx.h"
#include "RunLabelFeeder.h"
#include "variable.h"
//#include "CV_test.h"
#include "FastechPublic_IO.h"
#include "ComizoaPublic.h"
#include "CtlBd_Library.h"
#include "Ctlbd_Variable.h"

CRunLabelFeeder clsRunLabelFeeder;

CRunLabelFeeder::CRunLabelFeeder(void)
{
	
}


CRunLabelFeeder::~CRunLabelFeeder(void)
{
}

void CRunLabelFeeder::OnRunFeeder()
{

}
void CRunLabelFeeder::OnRunInit()
{

	int nRet[5] = {0,};
	int nRect = 0;
	double dCurrentPos = 0;
	//1차년도 사용 안함. 2차년도 사용.
	//kwlee 2017.0217
// 	st_sync_info.nInitRbt[M_LABEL_PITCH] = CTL_YES;	
// 	st_sync_info.nInitLabel = INIT_READY;
// 	return;


	if (st_sync_info.nInitLabel != INIT_CLEAR)		return;

	switch (m_nInitStep)
	{

	case 0:  
		m_nInitStep = 100;
		break;
	
	case 100:
		OnSetRejectCyl(IO_RUN_MODE,IO_OFF);
		m_nInitStep = 200;
		break;
	case 200:
		nRect =  OnGetRejectCyl(IO_RUN_MODE,IO_OFF);

		if (nRect == RET_GOOD)
		{
			m_nInitStep = 210;
		}
		else if (nRect == RET_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(107, dWARNING, m_strAlarmCode);
		}
		break;

	case 210:
		FAS_IO.set_out_bit(st_io_info.o_LabelFeederClmpOff,IO_ON);
		FAS_IO.set_out_bit(st_io_info.o_LabelFeederClmpOn,IO_OFF);
		m_nInitStep = 300;
		break;

	case 300:
		FAS_IO.set_out_bit(st_io_info.o_LabelStopperCylinder,IO_OFF);
		m_nInitStep = 1000;
		break;

	case 1000:
		nRet[0] = COMI.HomeCheck_Mot(M_LABEL_PITCH, MOT_SENS_ELM, MOT_TIMEOUT) ; 

		if(nRet[0] == BD_GOOD)
		{
			m_nInitStep = 2000;
		}
		else if(nRet[0] == BD_ERROR)
		{
			//Motor Home Check Error - (Label Feeder Pitch)
			CTL_Lib.Alarm_Error_Occurrence(206, dWARNING, _T("050007"));
			m_nInitStep = 0;
		}
		break;

	case 2000:
		nRet[0] = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_LABEL_PITCH, st_motor_info[M_LABEL_PITCH].d_pos[LABEL_FEEDER_1_READY], COMI.mn_manualspeed_rate);

		if (nRet[0] == BD_GOOD) //좌측으로 이동
		{
			st_sync_info.nInitRbt[M_LABEL_PITCH] = CTL_YES;
			//m_nInitStep = 3000;
			m_nInitStep = 2100;
			
		}
		else if (nRet[0] == BD_ERROR || nRet[0] == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(207, dWARNING, st_alarm_info.strCode);
			m_nInitStep = 9000;
		}
		break;

	case 2100:
		
		FAS_IO.set_out_bit(st_io_info.o_LabelFeederMotorOn,IO_ON);
		m_dwWFeederWaitTime[0] = GetCurrentTime();
		m_nInitStep = 2200;
		//kwlee 2017.0217 test
		//m_nInitStep = 2200;
		break;

	case 2200:
		m_dwWFeederWaitTime[1] = GetCurrentTime();
		m_dwWFeederWaitTime[2] = m_dwWFeederWaitTime[1] - m_dwWFeederWaitTime[0];

		if (m_dwWFeederWaitTime[2] <= 0)
		{
			m_dwWFeederWaitTime[0] = GetCurrentTime();
		}

		if (m_dwWFeederWaitTime[2] > 3000)
		{
			FAS_IO.set_out_bit(st_io_info.o_LabelFeederMotorOn,IO_OFF);
			m_nInitStep = 2300;
		}
		//kwlee 2017.0217 test
	//	m_nInitStep = 2300;
		break;

	case 2300:
		FAS_IO.set_out_bit(st_io_info.o_LabelFeederClmpOff,IO_OFF);
		FAS_IO.set_out_bit(st_io_info.o_LabelFeederClmpOn,IO_ON);
		m_nInitStep =2400;
		break;

	case 2400:
		FAS_IO.set_out_bit(st_io_info.o_LabelStopperCylinder,IO_ON);
		m_nInitStep = 2500;
		break;

	case 2500:
		//case 2300:
		st_sync_info.nInitLabel = INIT_READY;
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

void CRunLabelFeeder::OnThreadRUN()
{
	
	switch(st_handler_info.nRunStatus)
	{

	case dINIT:
		OnRunInit();
		break;

	case dRUN:
		OnRunFeeder();
		break;
	}
//	OnFeederInterface();
}
void CRunLabelFeeder::OnSetRejectCyl(int nMode, int nUpDn)
{
	if (nMode > 1) return;

	m_bCylFlag[nMode]			= false;
	m_dwWRejectCylWaitTime[nMode][0]	= GetCurrentTime();

	if (nUpDn == IO_ON)
	{
		FAS_IO.set_out_bit(st_io_info.o_LabelRejectCylinderUp, nUpDn); //S0105 Up
		FAS_IO.set_out_bit(st_io_info.o_LabelRejectCylinderDn, !nUpDn);

	}
	else
	{
		FAS_IO.set_out_bit(st_io_info.o_LabelRejectCylinderUp, nUpDn); //S0106 Dw
		FAS_IO.set_out_bit(st_io_info.o_LabelRejectCylinderDn, !nUpDn);
	}


}

int CRunLabelFeeder::OnGetRejectCyl(int nMode, int nUpDn)
{
	int nWaitTime = WAIT_PICKER_UP_DN;
	int nRet;
	if (nUpDn == IO_OFF)
	{
		if (m_bCylFlag[nMode] == false &&
			FAS_IO.get_in_bit(st_io_info.i_LabelRejectCylinderUpCheck,	IO_OFF)	== IO_OFF &&  
			FAS_IO.get_in_bit(st_io_info.i_LabelRejectCylinderDnCheck,	IO_ON)	== IO_ON)
		{
			m_bCylFlag[nMode]		= true;
			m_dwWRejectCylWaitTime[nMode][0]	= GetCurrentTime();
		}
		else if (m_bCylFlag[nMode] == true &&
			FAS_IO.get_in_bit(st_io_info.i_LabelRejectCylinderUpCheck,	IO_OFF)	== IO_OFF && 
			FAS_IO.get_in_bit(st_io_info.i_LabelRejectCylinderDnCheck,	IO_ON)	== IO_ON)
		{
			m_dwWRejectCylWaitTime[nMode][1] = GetCurrentTime();
			m_dwWRejectCylWaitTime[nMode][2] = m_dwWRejectCylWaitTime[nMode][1] - m_dwWRejectCylWaitTime[nMode][0];

			if (m_dwWRejectCylWaitTime[nMode][2] <= 0)
			{
				m_dwWRejectCylWaitTime[nMode][0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwWRejectCylWaitTime[nMode][2] > (DWORD)st_wait_info.nOnWaitTime[nWaitTime])
			{
				return RET_GOOD;
			}
		}
		else
		{
			m_dwWRejectCylWaitTime[nMode][1] = GetCurrentTime();
			m_dwWRejectCylWaitTime[nMode][2] = m_dwWRejectCylWaitTime[nMode][1] - m_dwWRejectCylWaitTime[nMode][0];

			if (m_dwWRejectCylWaitTime[nMode][2] <= 0)
			{
				m_dwWRejectCylWaitTime[nMode][0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwWRejectCylWaitTime[nMode][2] > (DWORD)st_wait_info.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_LabelRejectCylinderUpCheck, nUpDn); 
				//CTL_Lib.Alarm_Error_Occurrence(304, dWARNING, strAlarm);

				return RET_ERROR;
			}
		}
	}
	else//nUpDn == IO_ON
	{
		if (m_bCylFlag[nMode] == false && 
			FAS_IO.get_in_bit(st_io_info.i_LabelRejectCylinderUpCheck,IO_ON) == IO_ON &&
			FAS_IO.get_in_bit(st_io_info.i_LabelRejectCylinderDnCheck,IO_OFF) == IO_OFF)
		{
			m_bCylFlag[nMode]			= true;
			m_dwWRejectCylWaitTime[nMode][0]	= GetCurrentTime();
		}
		else if (m_bCylFlag[nMode] == true && 
			FAS_IO.get_in_bit(st_io_info.i_LabelRejectCylinderUpCheck,IO_ON) == IO_ON &&
			FAS_IO.get_in_bit(st_io_info.i_LabelRejectCylinderDnCheck,IO_OFF) == IO_OFF)
		{
			m_dwWRejectCylWaitTime[nMode][1]	= GetCurrentTime();
			m_dwWRejectCylWaitTime[nMode][2]	= m_dwWRejectCylWaitTime[nMode][1] - m_dwWRejectCylWaitTime[nMode][0];

			if (m_dwWRejectCylWaitTime[nMode][2] <= 0)
			{
				m_dwWRejectCylWaitTime[nMode][0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if(m_dwWRejectCylWaitTime[nMode][2] > (DWORD)st_wait_info.nOnWaitTime[nWaitTime])
			{
				return RET_GOOD;
			}
		}
		else
		{
			m_dwWRejectCylWaitTime[nMode][1]	= GetCurrentTime();
			m_dwWRejectCylWaitTime[nMode][2]	= m_dwWRejectCylWaitTime[nMode][1] - m_dwWRejectCylWaitTime[nMode][0];

			if (m_dwWRejectCylWaitTime[nMode][2] <= 0)
			{
				m_dwWRejectCylWaitTime[nMode][0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwWRejectCylWaitTime[nMode][2] > (DWORD)st_wait_info.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_LabelRejectCylinderUpCheck, nUpDn); 
				//CTL_Lib.Alarm_Error_Occurrence(305, dWARNING, strAlarm);
				return RET_ERROR;
			}
		}
	}
	return RET_PROCEED;

}