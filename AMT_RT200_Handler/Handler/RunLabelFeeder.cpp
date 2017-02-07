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


void CRunLabelFeeder::OnRunInit()
{

	int nRet[5] = {0,};
	double dCurrentPos = 0;
	//1차년도 사용 안함. 2차년도 사용.
	st_sync_info.nInitRbt[M_LABEL_PITCH] = CTL_YES;	
	st_sync_info.nInitLabel = INIT_READY;
	return;


	if (st_sync_info.nInitLabel != INIT_CLEAR)		return;

	switch (m_nInitStep)
	{

	case 0:

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
		nRet[0] = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_LABEL_PITCH, st_motor_info[M_LABEL_PITCH].d_pos[LABEL_FEEDER_1_SAFETY], COMI.mn_manualspeed_rate);

		if (nRet[0] == BD_GOOD) //좌측으로 이동
		{
			st_sync_info.nInitRbt[M_LABEL_PITCH] = CTL_YES;
			m_nInitStep = 3000;
		}
		else if (nRet[0] == BD_ERROR || nRet[0] == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(207, dWARNING, st_alarm_info.strCode);
			m_nInitStep = 0;
		}
		break;


	case 3000:
		st_sync_info.nInitLabel = INIT_READY;
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
		
		break;
	}
//	OnFeederInterface();
}
