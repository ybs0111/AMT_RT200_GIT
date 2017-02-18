#include "StdAfx.h"
#include "RunRobot.h"
#include "variable.h"
//#include "CV_test.h"
#include "FastechPublic_IO.h"
#include "ComizoaPublic.h"
#include "CtlBd_Library.h"
#include "Ctlbd_Variable.h"
#include "AlgMemory.h"

CRunRobot clsRunRobot;

CRunRobot::CRunRobot(void)
{
	m_nInitStep = 0;
	m_nRunStep =0;
	m_nInterFaceStep = 0;


	m_nRobot_Z = M_PICKERRBT_Z;
	m_nRobot_P = M_PICKERRBT_PITCH;
	m_nRobot_T = M_PICKERRBT_TURN;
	m_nRobot_X = M_PICKERRBT_X;
	m_nRobot_Y = M_PICKERRBT_Y;

	m_lpAxisNum[0] = m_nRobot_Y; //m_lpAxisNum[4];        //현재의 IndexNum에서 사용하는 모터 실재 번호를 가진다
	m_lpAxisNum[1] = m_nRobot_X;
	m_lpAxisNum[2] = m_nRobot_Z;

	m_bSecondPos = false;
	m_PickSecondPOs = false;
	m_nCntBcrNum = -1;
	m_dpTargetPosList[0]  = 0; 
	m_dpTargetPosList[1]  = 0;
	m_dpTargetPosList[2]  = 0;
	
	m_dpSpdRatio[0] = 100; 		//	m_dpSpdRatio[3];        //속도비율 [0]:Vel, [1]:Acc, [2]:Dec
	m_dpSpdRatio[1] = 100;
	m_dpSpdRatio[2] = 100;

	m_nLinearMove_Index	= 0;   //현재 map index
	m_lAxisCnt	= 2;  
	
	m_nJobNextPos = FALSE; //kwlee 2016.1229

	//kwlee 2017.0204
	m_nPrintOutPutCnt = 0; 
	m_nLabelFailCheck = FALSE;
	m_nEmptyCntBufferCnt = 0;
	// 	for (int i = 0; i<2; i++)
	// 	{
	// 		for (int j =0; j<MAX_BUFFER; j++)
	// 		{
	// 			for (int k =0; k< MAX_INFO; k++)
	// 			{
	// 				st_Picker_info.nPickerData[i][j][k]  = 0;
	// 			}
	// 		}
	// 	}
	
	st_sync_info.nBcrReq = CTL_CLEAR;
}


CRunRobot::~CRunRobot(void)
{
}

void CRunRobot::OnThreadRUN()
{
	switch(st_handler_info.nRunStatus)
	{
	case dINIT:
		OnRunInit();
		break;

	case dRUN:
		if (st_sync_info.nInitPickerRbt != INIT_COMPLETE)	break;
		
		OnRobotRun();
		OnFeederInterface();
		
		break;

	case dSTOP:
		break;

	case dWARNING:
		break;

	case dSELFCHECK:
		break;

	case dREINSTATE:
		break;

	case dRECOVERY:
		break;


	default:
		CTL_Lib.m_nStep_Linear[m_nLinearMove_Index] = 0;
		break;
	}
}
void CRunRobot::OnRunInit()
{
	int nRet_1, nRet_2=0, nRet_3=0, nRect[12]={0,};

	double dCurrentPos = 0;

	if (st_sync_info.nInitPickerRbt != INIT_CLEAR)		return;

	switch (m_nInitStep)
	{
		case 0:
			nRet_1 = FAS_IO.get_in_bit(st_io_info.i_TurnRotatCylBackChk, IO_OFF);
			nRet_2 = FAS_IO.get_in_bit(st_io_info.i_TurnRotatCylForChk, IO_OFF);

			if (nRet_1 == IO_OFF && nRet_2 == IO_OFF )
			{
				//502060 1 A "TURN CONV Table Motor Forward Cylinder-Sol Error."
				CTL_Lib.Alarm_Error_Occurrence(100, dWARNING, _T("502060"));
				//m_nInitStep = 0;		
			}
			else
			{
				m_nInitStep = 100;	
			}
			break;

		case 100:

			if(st_sync_info.nInitConv_pitch == YES)
			{
				m_nInitStep = 110;
			}
			break;

		case 110:
			// loader robot picker up 요청
			for (int i =0; i<PICKCNT; i++ )
			{
				m_npTemp_Picker_YesNo[i] = CTL_YES;
			}
		
			OnSetPickerUpDn(0, PICKER_UP, m_npTemp_Picker_YesNo);
			m_nInitStep = 200;
			break;

		case 200:
			// loader robot picker up 체크
			nRet_1 = OnGetPickerUpDn(0, PICKER_UP, m_npTemp_Picker_YesNo);
			if(nRet_1 == RET_GOOD)
			{
				//m_nInitStep = 300;
				m_nInitStep = 1000;
			}
			else if(nRet_1 == RET_ERROR)
			{
				m_nInitStep = 0;

				CTL_Lib.Alarm_Error_Occurrence(101, dWARNING, m_strAlarmCode);
			}
			break;

// 		case 300:
// 			//picker vaccum off
// 			for (int i = 0; i < PICKCNT; i++)
// 			{
// 				FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[i],IO_OFF);
// 				FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[i],IO_OFF);
// 				//if (nRect[i] == IO_ON)
// 				//vaccum 확인 
// 				if (FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[i],IO_OFF) == IO_ON)
// 				{
// 					//505080 1 A "Head Vaccum #1 Off Check Error ."
// 					//505081 1 A "Head Vaccum #1 On Check Error ."
// 					m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_HeadVaccumCheck[i], IO_ON);
// 					CTL_Lib.Alarm_Error_Occurrence(110, dWARNING, m_strAlarmCode);
// 					m_nInitStep = 0;
// 					break;
// 				}
// 				else
// 				{ 
// 					if (i< PICKCNT -1)
// 					{
// 						continue;
// 					}						
// 					m_nInitStep = 1000;								
// 				}
// 			}
// 			
// 			break;
	
		case 1000:
			// loader robot z축 home 이동
			nRet_1 = COMI.HomeCheck_Mot(M_PICKERRBT_Z, MOT_SENS_ELM, MOT_TIMEOUT) ; 

			if(nRet_1 == BD_GOOD)
			{
				m_nInitStep = 2000;
			}
			else if(nRet_1 == BD_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(120, dWARNING, _T("020001"));
				m_nInitStep = 0;
			}
			break;

		case 2000:	// loader robot z축 안전 위치로 이동
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_Z, st_motor_info[M_PICKERRBT_Z].d_pos[ROBOT_Z_SAFETY], COMI.mn_runspeed_rate);

			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				st_sync_info.nInitRbt[M_PICKERRBT_Z] = CTL_YES;
				m_nInitStep = 3000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(130, dWARNING, st_alarm_info.strCode);
				m_nInitStep = 0;
			}
			break;

		case 3000:
			// robot pitch 홈 위치 이동
			nRet_1 = COMI.HomeCheck_Mot(M_PICKERRBT_PITCH, MOT_SENS_ELM, MOT_TIMEOUT) ; 

			if(nRet_1 == BD_GOOD)
			{
				st_sync_info.nInitRbt[M_PICKERRBT_PITCH] = CTL_YES;
				m_nInitStep = 4000;
			}
			else if(nRet_1 == BD_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(140, dWARNING, _T("040001"));
			}
			break;

// 		case 4000:
// 			nRet_1 = COMI.HomeCheck_Mot(M_PICKERRBT_TURN, MOT_SENS_ELM, MOT_TIMEOUT) ; 
// 
// 			if(nRet_1 == BD_GOOD)
// 			{
// 				st_sync_info.nInitRbt[M_PICKERRBT_TURN] = CTL_YES;
// 				m_nInitStep = 5000;
// 			}
// 			else if(nRet_1 == BD_ERROR)
// 			{
// 				CTL_Lib.Alarm_Error_Occurrence(150, dWARNING, _T("060001"));
// 			}
// 			break;

		//case 5000:
		case 4000:
			nRet_1 = COMI.HomeCheck_Mot(M_PICKERRBT_X, MOT_SENS_ELM, MOT_TIMEOUT) ;
			if(nRet_1 == BD_GOOD)
			{
				st_sync_info.nInitRbt[M_PICKERRBT_X] = CTL_YES;
				//m_nInitStep = 6000;
				m_nInitStep = 5000;
			}
			else if(nRet_1 == BD_ERROR)
			{//010001 0 A "Motor Home Check Error - (Robot X)."
				CTL_Lib.Alarm_Error_Occurrence(160, dWARNING, _T("010001"));
			}
			break;

		case 5000:
			nRet_1 = COMI.HomeCheck_Mot(M_PICKERRBT_Y, MOT_SENS_ELM, MOT_TIMEOUT) ;
			if(nRet_1 == BD_GOOD)
			{
				st_sync_info.nInitRbt[M_PICKERRBT_Y] = CTL_YES;
				//m_nInitStep = 7000;
				m_nInitStep = 6000;
			}
			else if(nRet_1 == BD_ERROR)
			{//000001 0 A "Motor Home Check Error - (Robot Y)."
				CTL_Lib.Alarm_Error_Occurrence(170, dWARNING, _T("000001"));
				m_nInitStep = 0;
			}
			break;

		case 6000:
			nRet_1 = COMI.HomeCheck_Mot(M_PICKERRBT_TURN, MOT_SENS_ELM, MOT_TIMEOUT) ; 

			if(nRet_1 == BD_GOOD)
			{
				st_sync_info.nInitRbt[M_PICKERRBT_TURN] = CTL_YES;
				//m_nInitStep = 5000;
				m_nInitStep = 6100;
			}
			else if(nRet_1 == BD_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(150, dWARNING, _T("060001"));
			}
			break;

			///////////////////////////
			//kwlee 2017.0218
		case 6100:	
			//ROBOT TURN
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_TURN, st_motor_info[M_PICKERRBT_TURN].d_pos[PITCH_UP_HEAD_TURN_ROTATOR_0], COMI.mn_runspeed_rate);

			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_nInitStep = 6200;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				CTL_Lib.Alarm_Error_Occurrence(630, dWARNING, st_alarm_info.strCode);
				m_nInitStep = 0;
			}
			break;

		case 6200:	
			////ROBOT PITCH 작업 위치 이동
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_PITCH, st_motor_info[M_PICKERRBT_PITCH].d_pos[PITCH_UP_HEAD_PICK_FEEDER], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				//m_nRunStep = 1000;
				m_nInitStep = 6300;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(640, dWARNING, st_alarm_info.strCode);
				m_nInitStep = 0;
			}
			break;

		case 6300:
			OnSetRejectCylUpDn(IO_RUN_MODE,IO_ON);
			m_nInitStep = 6400;
			break;

		case 6400:
			nRet_1 = OnGetRejectCylUpDn(IO_RUN_MODE,IO_ON);

			if (nRet_1 == RET_GOOD)
			{
				m_nInitStep = 6500;
			}
			else if(nRet_1 == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(650, dWARNING, m_strAlarmCode);
			}
			break;

			//kwlee 2017.0204
		case 6500:
			m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_REJECT];
			m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_REJECT];

			nRet_1 = CTL_Lib.Linear_Move(ONLY_MOVE_START, m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, COMI.mn_runspeed_rate);

			if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
			{
				m_nInitStep = 6600;
			}
			else if(nRet_1 == BD_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(660, dWARNING, st_alarm_info.strCode);
				m_nInitStep = 0;
			}
			break;

		case 6600:
			//label pick 위치 확인
			nRet_1 = CTL_Lib.Linear_Move(ONLY_MOVE_CHECK, m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, COMI.mn_runspeed_rate);
			if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태 
			{						
				m_nInitStep = 6700;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_nInitStep = 6600;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{			
				m_nInitStep = 0;
				CTL_Lib.Alarm_Error_Occurrence(670, dWARNING, st_alarm_info.strCode);
			}
			break;

		case 6700:
			for (int i =0; i<PICKCNT; i++ )
			{
				if (i < 5)
				{
					clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_YES;
				}
				else
				{
					clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT + 3 - i] = CTL_YES;
				}
				
			}
			clsRunRobot.OnSetPickerUpDn(0, PICKER_DN, clsRunRobot.m_npTemp_Picker_YesNo);
			m_nInitStep = 6800;
			break;

		case 6800:
			nRet_1 = clsRunRobot.OnGetPickerUpDn(0, PICKER_DN,clsRunRobot.m_npTemp_Picker_YesNo);
			if(nRet_1 == RET_GOOD)
			{	
				m_nInitStep = 6900;	
			}
			else if(nRet_1 == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(750, dWARNING, m_strAlarmCode);
				m_nInitStep = 0;
			}
			break;
		case 6900:	
			//함수호출
			nRet_1 = COMI.Check_MotPosRange(M_PICKERRBT_X,st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_REJECT],st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_REJECT] + st_motor_info[M_PICKERRBT_X].d_allow);
			nRet_2 = COMI.Check_MotPosRange(M_PICKERRBT_Y, st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_REJECT],st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_REJECT] + st_motor_info[M_PICKERRBT_Y].d_allow);

			if (nRet_1 == BD_GOOD && nRet_2 == BD_GOOD)
			{
				nRet_3 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_Z, st_motor_info[M_PICKERRBT_Z].d_pos[ROBOT_REJECT_PLACE], COMI.mn_runspeed_rate);

				if (nRet_3 == BD_GOOD) //좌측으로 이동
				{
					//m_nRunStep = 900;
					m_nInitStep = 6910;	
				}
				else if (nRet_3 == BD_ERROR || nRet_3 == BD_SAFETY)
				{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
					CTL_Lib.Alarm_Error_Occurrence(680, dWARNING, st_alarm_info.strCode);
					//m_nRunStep = 0;
				}
			}
			else
			{
				m_strAlarmCode.Format(_T("%02d0006"), M_PICKERRBT_Z); //000008 0 A "MOTOR MOVE SAFETY CHECK ERROR -[M_ROBOT_X]."
				CTL_Lib.Alarm_Error_Occurrence(690, dMOTOR_WARNING, m_strAlarmCode);//dMOTOR_WARNING ==> 이때는 알람은 발생시키지 않는다(run status를 바꾸지 않는다).thread 에서만 판단하여 알람 발생시킨다 

				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
				{
					clsMem.OnNormalMessageWrite(_T("Robot Z Place Reject Position Error"));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
				}
			}
			break;

		case 6910:
			for (int i =0; i<PICKCNT; i++ )
			{
				if (i < 5)
				{
					FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[i],IO_OFF);
					FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[i],IO_ON);
				}
				else
				{
					FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[PICKCNT + 3 - i],IO_OFF);
					FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[PICKCNT + 3 - i],IO_ON);
				}
			}
			m_dwTimeCheck[0] = GetCurrentTime();
			m_nInitStep = 6920;	
			break;

		case 6920:
			m_dwTimeCheck[1] = GetCurrentTime();
			m_dwTimeCheck[2] = m_dwTimeCheck[1] - m_dwTimeCheck[0];
			if( m_dwTimeCheck[2] <= 0 ) m_dwTimeCheck[0] = GetCurrentTime();
			if( m_dwTimeCheck[2] >  1000 )
			{

				for (int i =0; i<PICKCNT; i++ )
				{
					if (i < 5)
					{
						FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[i],IO_OFF);
						clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_YES;
					}
					else
					{
						FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[PICKCNT + 3 - i],IO_OFF);
						clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT + 3 - i] = CTL_YES;
					}
				}				
				clsRunRobot.OnSetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);
				m_nInitStep = 6930;
			}
			break;

		case 6930:

			nRet_1 = clsRunRobot.OnGetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);

			if(nRet_1 == RET_GOOD)
			{
				m_nInitStep = 6940;
			}
			else if(nRet_1 == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(650, dWARNING, m_strAlarmCode);
			}
			break;

		case 6940:	
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_Z, st_motor_info[M_PICKERRBT_Z].d_pos[ROBOT_SAFETY], COMI.mn_runspeed_rate);
			//z 축 up
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				//m_nRunStep = 1000;
				m_nInitStep = 6950;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(700, dWARNING, st_alarm_info.strCode);
			}
			break;

		case 6950:
			OnSetRejectCylUpDn(IO_RUN_MODE,IO_OFF);
			m_nInitStep = 6960;
			break;

		case 6960:
			nRet_1 = OnGetRejectCylUpDn(IO_RUN_MODE,IO_OFF);

			if (nRet_1 == RET_GOOD)
			{
				m_nInitStep = 7000;
			}
			else if(nRet_1 == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(650, dWARNING, m_strAlarmCode);
			}
			break;
	
			
			//////////////////////
		case 7000:
			// loader robot x, y축 안전위치로 이동
			m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_SAFETY];
			m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_SAFETY];

			nRet_1 = CTL_Lib.Linear_Move(ONLY_MOVE_START, m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, COMI.mn_manualspeed_rate);

			if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
			{
				m_nInitStep = 7100;
			}
			else if(nRet_1 == BD_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(180, dWARNING, st_alarm_info.strCode);
				m_nInitStep = 0;
			}
			break;

		case 7100:
			m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_SAFETY];
			m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_SAFETY];

			nRet_1 = CTL_Lib.Linear_Move(ONLY_MOVE_CHECK, m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, COMI.mn_manualspeed_rate);

			if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
			{
				m_nInitStep = 7200;
				
			}
			else if(nRet_1 == BD_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(190, dWARNING, st_alarm_info.strCode);
				m_nInitStep = 0;
			}
			break;


		case 7200:
			//picker vaccum off
			for (int i = 0; i < PICKCNT; i++)
			{
				FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[i],IO_OFF);
				FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[i],IO_OFF);
				//if (nRect[i] == IO_ON)
				//vaccum 확인 
				if (FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[i],IO_OFF) == IO_ON)
				{
					//505080 1 A "Head Vaccum #1 Off Check Error ."
					//505081 1 A "Head Vaccum #1 On Check Error ."
					m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_HeadVaccumCheck[i], IO_ON);
					CTL_Lib.Alarm_Error_Occurrence(110, dWARNING, m_strAlarmCode);
					m_nInitStep = 0;
					break;
				}
			}

			m_nInitStep = 8000;
			break;

		case 8000:
			st_sync_info.nInitPickerRbt = INIT_READY;
			m_nInitStep = 0;
			
			break;

		case 9000:
			m_nInitStep = 0;
			st_sync_info.nInitializeError = YES;
			break;
	}
}
void CRunRobot::OnSetLabelPlace(int  nPickCnt)
{
	for (int i = 0; i<PICKCNT; i++)
	{
		clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_NO;	
	}

	for (int i =0; i < nPickCnt; i++ )
	{
		if(m_bSecondPos == false)
		{
			if(st_basic_info.nPcbType == RDIMM)
			{
				if (i < 3)
				{
					clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_YES;
				}
			}
			else /*if (st_basic_info.nPcbType == UDIMM_10)*/
			{
				if (i < 5)
				{
					clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_YES;
				}
			}
		}
		else
		{
			if(st_basic_info.nPcbType == RDIMM)
			{ 
				if(i > 2) 
 				{
					clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT - i - 1] = CTL_YES;	
				}
			}
			else if (st_basic_info.nPcbType == UDIMM_10)
			{ 
				if(i > 4) 
				{
// 					if (st_basic_info.nPickerSelect[PICKCNT - 6 + i] == CTL_YES)
// 					{
					clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT + 3 - i] = CTL_YES;	
					//}
				}
			}
			else //Sodimm, Udimm_9
			{
				//if(m_nPickCnt > (PICKCNT - 6 + i)) 
				if(i > 4) 
				{	
// 					if (st_basic_info.nPickerSelect[PICKCNT - 6 + i] == CTL_YES)
// 					{
					clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT + 3 - i] = CTL_YES;	
					//}
				}
			}
		}	
	}
	//clsRunRobot.OnSetPickerUpDn(0, PICKER_DN, clsRunRobot.m_npTemp_Picker_YesNo);
}
void CRunRobot::OnBufferDataTransfer()
{
// 	if (m_nJobNextPos == 1)
// 	{
	int nCnt = 0;
	for (int i =0; i<2; i++)
	{
		for (int j =0; j<MAX_BUFFER; j++)
		{
			nCnt++;
			if (st_Buffer_info[PICK].strBufferSerial[i][j] != _T(""))
			{
				if (st_Buffer_info[PICK].strBufferSerial[i][nCnt - 1] == _T(""))
				{
					st_Buffer_info[PICK].strBufferSerial[i][nCnt - 1] =  st_Buffer_info[PICK].strBufferSerial[i][j];
					break;
				}
			}
		}
	}
// 	}
// 	else
// 	{
// 		for (int i =0; i<2; i++)
// 		{
// 			for (int j =0; j<MAX_PICKER; j++)
// 			{
// 				for (int k =0; j<MAX_INFO; k++)
// 				{
// 					if (st_Buffer_info[PICK].strBufferSerial[i][MAX_PICKER + j] != _T(""))
// 					{
// 						st_Buffer_info[TEMP].nBufferData[i][j][k] =  st_Buffer_info[PICK].nBufferData[i][MAX_PICKER + j][k];
// 						st_Buffer_info[TEMP].strBufferSerial[i][j] =  st_Buffer_info[PICK].strBufferSerial[i][MAX_PICKER + j];
// 
// 						st_Buffer_info[PICK].nBufferData[i][j][k] = st_Buffer_info[TEMP].nBufferData[i][j][k];
// 						st_Buffer_info[PICK].strBufferSerial[i][j] = st_Buffer_info[TEMP].strBufferSerial[i][j];
// 
// 						st_Buffer_info[TEMP].nBufferData[i][j][k] = 0;
// 						st_Buffer_info[TEMP].strBufferSerial[i][j] = _T("");
// 						//m_nEmptyCntBufferCnt++;
// 					}
// 				
// 				}
// 			}
// 		}
// 	}
}
void CRunRobot::OnDataExchange(int nPickPlace,int nFailCheck)
{

	for (int i =0; i<2; i++)
	{
		for (int j = 0; j< MAX_PICKER; j++)
		{
			for (int k =0; k<MAX_INFO; k++)
			{
				if (nPickPlace == PICK)
				{
					st_Picker_info.nPickerData[i][j][k] = st_Buffer_info[PICK].nBufferData[i][j][k];
					st_Picker_info.strPickerSerial[i][j] = st_Buffer_info[PICK].strBufferSerial[i][j];
					st_Picker_info.nPickerFailCnt        = st_Buffer_info[PICK].nBufferFailCnt;

		// 					if (st_Picker_info.nPickerData[i][j][k] == st_Buffer_info[PICK].nBufferData[i][j][k] && 
		// 						st_Picker_info.strPickerSerial[i][j] == st_Buffer_info[PICK].strBufferSerial[i][j])
					if (st_Picker_info.nPickerData[i][j][k] == st_Buffer_info[PICK].nBufferData[i][j][k] && 
						st_Picker_info.strPickerSerial[i][j] == st_Buffer_info[PICK].strBufferSerial[i][j] && 
						m_nLabelFailCheck == FALSE)
					{

						st_Buffer_info[PICK].nBufferData[i][j][k] = -1;
						st_Buffer_info[PICK].strBufferSerial[i][j] = _T("");
						//st_Buffer_info.nBufferFailCnt = 0;	
					}
				}
				else if (nPickPlace == PLACE)
				{
					st_Buffer_info[PLACE].nBufferData[i][j][k]  = st_Picker_info.nPickerData[i][j][k] ;
					st_Buffer_info[PLACE].strBufferSerial[i][j] = st_Picker_info.strPickerSerial[i][j] ;
					//st_Picker_info.nPickerFailCnt        = st_Buffer_info.nBufferFailCnt;

					// 					if (st_Picker_info.nPickerData[i][j][k] == st_Buffer_info[PICK].nBufferData[i][j][k] && 
					// 						st_Picker_info.strPickerSerial[i][j] == st_Buffer_info[PICK].strBufferSerial[i][j])
					if (st_Buffer_info[PLACE].nBufferData[i][j][k]  ==  st_Picker_info.nPickerData[i][j][k] && 
						st_Buffer_info[PLACE].strBufferSerial[i][j] == st_Picker_info.strPickerSerial[i][j])
						//m_nLabelFailCheck == FALSE

					{
						st_Picker_info.nPickerData[i][j][k]  = -1; 
						st_Picker_info.strPickerSerial[i][j] = _T("");

						//st_Buffer_info[PICK].nBufferData[i][j][k] = -1;
						//st_Buffer_info[PICK].strBufferSerial[i][j] = _T("");
						//st_Buffer_info.nBufferFailCnt = 0;	
					}
				}
				else
				{

				}
			}
		}
	}
}

//kwlee 2017.0204
int CRunRobot::OnGetVaccumgmCheck(int OnOff,int nPickCnt)
{
	int i,nFailCnt;
	int nRet;
	if (OnOff == IO_ON)
	{
		for (i = 0; i < nPickCnt; i++)
		{
			if (st_basic_info.nPcbType == UDIMM_10)
			{
				if( i < 5)
				{
					//FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[i],IO_ON);
					//kwlee 2017.0204
					//if (st_Buffer_info.nBufferData[0][i][BIN] == GOOD)
					if (st_Buffer_info[PICK].nBufferData[0][i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[0][i][EXIST] == YES && st_Buffer_info[PICK].strBufferSerial[0][i] !=_T(""))
					{
						nRet = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[i],IO_ON);
						if (nRet == IO_OFF)
						{
							st_Buffer_info[PICK].nBufferData[0][i][BIN] = FAIL;
							st_Buffer_info[PICK].nBufferData[0][i][EXIST] = NO;
						//	m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_HeadVaccumCheck[i], OnOff);
							//return RET_ERROR;
						}
						
					}
					//else if (st_Buffer_info[PICK].nBufferData[0][i][FAILPICK] == YES)
					else if (st_Picker_info.nPickerData[0][i][FAILPICK] == YES)
					{
						nRet = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[i],IO_ON);
						if (nRet == IO_ON)
						{
							//st_Buffer_info[PICK].nBufferData[0][i][FAILPICK] = NO;	
							//kwlee 2017.0214
							st_Buffer_info[PICK].nBufferData[0][i][FAILPICK] = NO;	
							st_Buffer_info[PICK].nBufferData[0][i][EXIST] = YES;
							st_Buffer_info[PICK].nBufferData[0][i][BIN] = GOOD;
							
							
						}
					}
					//kwlee 2017.0209
// 					else
// 					{
// 						if (st_Buffer_info[PICK].nBufferData[0][i][BIN] == FAIL && st_Buffer_info[PICK].nBufferData[0][i][EXIST] == NO)
// 						{
// 							st_Buffer_info[PICK].nBufferData[0][i][X_POS] = LEFT;
// 							st_Buffer_info[PICK].nBufferData[0][i][Y_POS] = i;
// 							st_Buffer_info[PICK].nBufferData[0][i][FAILPICK] = YES;
// 							//return RET_ERROR;
// 						}
// 					}
				}
				else
				{
					//FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[PICKCNT + 5 - m_nPickerNum],IO_ON);
					//	if(nPickCnt >= (PICKCNT + 3 - i)) 
					//	{
					//FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[PICKCNT + 4 - i],IO_ON);
					//kwlee 2017.0204
					//if (st_Buffer_info.nBufferData[1][nPickCnt - i][BIN] == GOOD)
					//if (st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][EXIST] == YES && st_Buffer_info[PICK].strBufferSerial[1][nPickCnt - i]!= _T(""))
					if (st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][EXIST] == YES && st_Buffer_info[PICK].strBufferSerial[1][nPickCnt - i]!= _T(""))
					{
						nRet = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[PICKCNT + 3 - i],IO_ON);
						if (nRet == IO_OFF)
						{
							st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][BIN] = FAIL;
							st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][EXIST] = NO;
						//	m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_HeadVaccumCheck[PICKCNT + 3 - i], OnOff);
							//return RET_ERROR;
						}	
					}
					else if (st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][FAILPICK] == YES)
					{
						nRet = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[PICKCNT + 3 - i],IO_ON);
						if (nRet == IO_ON)
						{
							//st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][FAILPICK] = NO;
							//kwlee 2017.0214
							st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][FAILPICK] = NO;
							st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][EXIST] = YES;
							st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][BIN] = GOOD;
						}
					}
					//kwlee 2017.0209
// 					else
// 					{
// 						if (st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][BIN] == FAIL && st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][EXIST] == NO)
// 						{
// 							st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][X_POS] = RIGHT;
// 							st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][Y_POS] = i;
// 							st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][FAILPICK] = YES;
// 							//return RET_ERROR;
// 						}
					//}
				}
			}
			else
			{
				if (st_basic_info.nPcbType == RDIMM)
				{
					if (i < 3)
					{
						//FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[i],IO_ON);
						//kwlee 2017.0204
						//if (st_Buffer_info.nBufferData[0][i][BIN] == GOOD)
						if (st_Buffer_info[PICK].nBufferData[0][i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[0][i][EXIST] == YES && st_Buffer_info[PICK].strBufferSerial[0][i] != _T(""))
						{
							nRet = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[i],IO_ON);
							if (nRet == IO_OFF)
							{
								st_Buffer_info[PICK].nBufferData[0][i][BIN] = FAIL;
								st_Buffer_info[PICK].nBufferData[0][i][EXIST] = NO;
								//m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_HeadVaccumCheck[i], OnOff);
								//return RET_ERROR;
							}
						}
						else if (st_Buffer_info[PICK].nBufferData[0][i][FAILPICK] == YES)
						{
							//FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[i],IO_OFF);
							nRet = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[i],IO_ON);
							if (nRet == IO_ON)
							{
								//st_Buffer_info[PICK].nBufferData[0][i][FAILPICK] = NO;
								//kwlee 2017.0214
								st_Buffer_info[PICK].nBufferData[0][i][FAILPICK] = NO;
								st_Buffer_info[PICK].nBufferData[0][i][EXIST] = YES;
								st_Buffer_info[PICK].nBufferData[0][i][BIN] = GOOD;
								
							}
						}
						//kwlee 2017.0209
// 						else
// 						{
// 							if (st_Buffer_info[PICK].nBufferData[0][i][BIN] == FAIL && st_Buffer_info[PICK].nBufferData[0][i][EXIST] == NO)
// 							{
// 								st_Buffer_info[PICK].nBufferData[0][i][X_POS] = RIGHT;
// 								st_Buffer_info[PICK].nBufferData[0][i][Y_POS] = i;
// 								st_Buffer_info[PICK].nBufferData[0][i][FAILPICK] = YES;
// 								//return RET_ERROR;
// 							}
// 						}
					}
					else
					{	
						//FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[PICKCNT - i],IO_ON);
						//kwlee 2017.0204
						//if (st_Buffer_info.nBufferData[1][nPickCnt - i][BIN] == GOOD)
						if (st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][EXIST] == YES && st_Buffer_info[PICK].strBufferSerial[1][nPickCnt - i] != _T("") )
						{
							nRet = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[PICKCNT - i - 1],IO_ON);
							if (nRet == IO_OFF)
							{
								st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][BIN] = FAIL;
								st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][EXIST] = NO;
								//m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_HeadVaccumCheck[PICKCNT - i - 1], OnOff);
								//return RET_ERROR;
							}

						}
						else if (st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][FAILPICK] == YES)
						{
							//FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[PICKCNT - i],IO_OFF);
							nRet = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[PICKCNT - i - 1],IO_ON);
							if (nRet == IO_ON)
							{
								//st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][FAILPICK] = NO;
								//kwlee 2017.0214
								st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][FAILPICK] = NO;
								st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][EXIST] = YES;
								st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][BIN] = GOOD;

							}
						}
						//kwlee 2017.0209
// 						else
// 						{
// 							if (st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][BIN] == FAIL && st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][EXIST] == NO)
// 							{
// 								st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][X_POS] = RIGHT;
// 								st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][Y_POS] = i;
// 								st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][FAILPICK] = YES;
// 								//return RET_ERROR;
// 							}
// 						}
					}
				}
				else //Sodimm, Udimm_9
				{
					if (i <5)
					{
						//FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[i],IO_ON);	
						//kwlee 2017.0204
						//if (st_Buffer_info.nBufferData[0][i][BIN] == GOOD)
						if (st_Buffer_info[PICK].nBufferData[0][i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[0][i][EXIST] == YES && st_Buffer_info[PICK].strBufferSerial[0][i]!= _T(""))
						{
							nRet = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[i],IO_ON);	
							if (nRet == IO_OFF)
							{
								st_Buffer_info[PICK].nBufferData[0][i][BIN] = FAIL;
								st_Buffer_info[PICK].nBufferData[0][i][EXIST] = NO;
								//m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_HeadVaccumCheck[i], OnOff);
								//return RET_ERROR;
							}
						}
						else if (st_Buffer_info[PICK].nBufferData[0][i][FAILPICK] == YES)
						{
							nRet = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[i],IO_ON);
							if (nRet == IO_ON)
							{
								//st_Buffer_info[PICK].nBufferData[0][i][FAILPICK] = NO;
								//kwlee 2017.0214
								st_Buffer_info[PICK].nBufferData[0][i][FAILPICK] = NO;
								st_Buffer_info[PICK].nBufferData[0][i][EXIST] = YES;
								st_Buffer_info[PICK].nBufferData[0][i][BIN] = GOOD;
							}
						}
						//kwlee 2017.0209
// 						else
// 						{
// 							if (st_Buffer_info[PICK].nBufferData[0][i][BIN] == FAIL && st_Buffer_info[PICK].nBufferData[0][i][EXIST] == NO)
// 							{
// 								
// 								st_Buffer_info[PICK].nBufferData[0][i][X_POS] = RIGHT;
// 								st_Buffer_info[PICK].nBufferData[0][i][Y_POS] = i;
// 								st_Buffer_info[PICK].nBufferData[0][i][FAILPICK] = YES;
// 								//return RET_ERROR;
// 							}
// 						}
					}
					else
					{
						//FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[PICKCNT + 3 - i],IO_ON);
						//kwlee 2017.0204
						//if (st_Buffer_info.nBufferData[1][nPickCnt - i][BIN] == GOOD)
						if (st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][EXIST] == YES && 
							st_Buffer_info[PICK].strBufferSerial[1][nPickCnt - i] !=_T(""))
						{
							nRet = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[PICKCNT + 3 - i],IO_ON);
							if (nRet == IO_OFF)
							{
								//st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][BIN] = FAIL;
								//st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][EXIST] = NO;
								st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][BIN] = FAIL;
								st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][EXIST] = NO;
								//m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_HeadVaccumCheck[PICKCNT + 3 - i], OnOff);
								//return RET_ERROR;
							}
						}
						else if (st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][FAILPICK] == YES)
						{
							//FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[PICKCNT + 3 - i],IO_OFF);
							nRet = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[PICKCNT + 3 - i],IO_ON);
							if (nRet == IO_ON)
							{
								///st_Buffer_info[PICK].nBufferData[1][TOTAL_PICK - i][FAILPICK] = NO;
								//kwlee 2017.0214
								st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][FAILPICK] = NO;
								st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][EXIST] = YES;
								st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][BIN] = GOOD;
							}
						}
						//kwlee 2017.0209
// 						else
// 						{
// 							if (st_Buffer_info[PICK].nBufferData[1][TOTAL_PICK - i][BIN] == FAIL && st_Buffer_info[PICK].nBufferData[1][TOTAL_PICK - i][EXIST] == NO)
// 							{
// 								st_Buffer_info[PICK].nBufferData[1][TOTAL_PICK - i][X_POS] = RIGHT;
// 								st_Buffer_info[PICK].nBufferData[1][TOTAL_PICK - i][Y_POS] = i;
// 								st_Buffer_info[PICK].nBufferData[1][TOTAL_PICK - i][FAILPICK] = YES;
// 							//	return RET_ERROR;
// 							}
// 						}
					}
				}
			}
		}
		//kwlee 2017.0209
		nFailCnt = 0;
		for (int i = 0; i<2; i++)
		{
			for (int j =0; j < MAX_PICKER; j++)
			{
				if (st_Buffer_info[PICK].nBufferData[i][j][BIN] == FAIL && st_Buffer_info[PICK].nBufferData[i][j][EXIST] == NO)
				{
					st_Buffer_info[PICK].nBufferData[i][j][X_POS] = i;
					st_Buffer_info[PICK].nBufferData[i][j][Y_POS] = j;
					st_Buffer_info[PICK].nBufferData[i][j][FAILPICK] = YES;
					nFailCnt++;
				}
			}
		}

		if (nFailCnt > 0)
		{
			st_Buffer_info[PICK].nBufferFailCnt = nFailCnt;
			return RET_ERROR;
		}
		else
		{
			return RET_GOOD;
		}
		////
	}
	else //OnOff == IO_OFF
	{
		for (int i =0; i<TOTAL_PICK; i++ )
		{
			if(m_bSecondPos == false)
			{
				if(st_basic_info.nPcbType == RDIMM)
				{
					if (i < 3)
					{
						nRet = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[i],IO_OFF);
						if (nRet == IO_ON)
						{
							m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_HeadVaccumCheck[i], OnOff);
							return RET_ERROR;
						}
					}
				}
				else if (st_basic_info.nPcbType == UDIMM_10)
				{
					if (i < 5)
					{

						nRet = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[i],IO_OFF);
						if (nRet == IO_ON)
						{
							m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_HeadVaccumCheck[i], OnOff);
							return RET_ERROR;
						}
					}
				}
			}
			else
			{
				if(st_basic_info.nPcbType == RDIMM)
				{
					//if(m_nPickCnt > (PICKCNT - i)) 
					if(i > 2) 
					{
						nRet = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[PICKCNT - i - 1],IO_OFF);
						if (nRet == IO_ON)
						{
							m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_HeadVaccumCheck[PICKCNT - i - 1], OnOff);
							return RET_ERROR;
						}
					}
				}
				else if (st_basic_info.nPcbType == UDIMM_10)
				{
					//if (st_basic_info.nPickerSelect[PICKCNT - 6 + i] == CTL_YES)
					if (i > 4)
					{
						nRet = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[PICKCNT + 3 - i],IO_OFF);
						if (nRet == IO_ON)
						{
							m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_HeadVaccumCheck[PICKCNT + 3 - i], OnOff);
							return RET_ERROR;
						}
					}
				}
				else //Sodimm, Udimm_9
				{
					//if(m_nPickCnt > (PICKCNT - 6 + i))
					if(i > 4)
					{
						// 						if (st_basic_info.nPickerSelect[PICKCNT - 6 + i] == CTL_YES)
						// 						{
						nRet = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[PICKCNT + 3 - i],IO_OFF);
						if (nRet == IO_ON)
						{
							m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_HeadVaccumCheck[PICKCNT + 3 - i], OnOff);
							return RET_ERROR;
						}
						//}	
					}
				}
			}
		}
	}
	return RET_GOOD;
}
/////
void CRunRobot::OnSetLabelPick(int nMode, int nPickCnt)
{
	int i;
	for (i =0; i<PICKCNT; i++ )
	{
		clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_NO;
	}

	//for (int i =0; i<PICKCNT; i++)
	//for (i =0; i<nPickCnt; i++)
	for (i =0; i<nPickCnt; i++)
	{
		if (nMode == 1)
		{
			//if ((st_Picker_info.nPickerData[0][i][X_POS] == LEFT && st_Picker_info.nPickerData[0][i][FAILPICK] == YES) || 
				//(st_Picker_info.nPickerData[1][nPickCnt - i][X_POS] == RIGHT && st_Picker_info.nPickerData[1][nPickCnt - i][FAILPICK] == YES))
			//{
			//	if (i < 5 && st_Buffer_info[PICK].nBufferData[0][i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[0][i][EXIST] == YES)
				if (i < 5 && st_Picker_info.nPickerData[0][i][BIN] == FAIL && st_Picker_info.nPickerData[0][i][EXIST] == NO)
				{
					//if (st_Buffer_info[PICK].nBufferData[0][i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[0][i][EXIST] == YES)
					//if (st_Buffer_info[PICK].nBufferData[0][i][BIN] == FAIL && st_Buffer_info[PICK].nBufferData[0][i][EXIST] == NO)
// 					if (st_Picker_info.nPickerData[0][i][BIN] == FAIL && st_Picker_info.nPickerData[0][i][EXIST] == NO)
// 					{	
						//clsRunRobot.m_npTemp_Picker_YesNo[st_Picker_info.nPickerData[0][i][Y_POS]] = CTL_YES;
						
						clsRunRobot.m_npTemp_Picker_YesNo[st_Picker_info.nPickerData[0][i][Y_POS]] = CTL_YES;
											//}
					
					//st_Picker_info.nPickerData[0][st_Picker_info.nPickerData[0][i][Y_POS]][FAILPICK] = NO;
				//	break;
				}
				//else if (i > 4 && st_Buffer_info[PICK].nBufferData[1][i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[1][i][EXIST] == YES)
				else if ( i > 4 && st_Picker_info.nPickerData[1][nPickCnt - i][BIN] == FAIL && st_Picker_info.nPickerData[1][nPickCnt - i][EXIST] == NO)
				{
					//if (st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][EXIST] == YES)
					//if (st_Buffer_info[PICK].nBufferData[1][i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[1][i][EXIST] == YES)
// 					if (st_Picker_info.nPickerData[1][nPickCnt - i][BIN] == FAIL && st_Picker_info.nPickerData[1][nPickCnt - i][EXIST] == NO)
// 					{	
					
					//clsRunRobot.m_npTemp_Picker_YesNo[MAX_PICKER + st_Picker_info.nPickerData[1][TOTAL_PICK - i][Y_POS]] = CTL_YES;
					clsRunRobot.m_npTemp_Picker_YesNo[MAX_PICKER + st_Picker_info.nPickerData[1][nPickCnt - i][Y_POS]] = CTL_YES;
				
					//}
					//st_Picker_info.nPickerData[1][st_Picker_info.nPickerData[1][i][Y_POS]][FAILPICK] = NO;
				//	break;
				}
				else
				{

				}
			//}
		}
		else
		{
			if (st_basic_info.nPcbType == UDIMM_10)
			{
				if(i < 5)
				{
					//clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_YES;
					//kwlee 2017.0204
					//if (st_Buffer_info.nBufferData[0][i][BIN] == GOOD)
					if (st_Buffer_info[PICK].nBufferData[0][i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[0][i][EXIST] == YES)
					{
						clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_YES;
					}
					else
					{
						clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_NO;
					}
				}
				else
				{
					//clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT + 3 - i] = CTL_YES;
					//kwlee 2017.0204
					//if (st_Buffer_info.nBufferData[1][nPickCnt - i][BIN] == GOOD)
					if (st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][EXIST] == YES)
					{
						clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT + 3 - i] = CTL_YES;
					}
					else
					{
						clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT + 3 - i] = CTL_NO;
					}
				}
			}
			else
			{

				if (st_basic_info.nPcbType == RDIMM)
				{
					if (i < 3)
					{
						/*clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_YES;	*/
						//kwlee 2017.0204
						//if (st_Buffer_info.nBufferData[0][i][BIN] == GOOD)
						if (st_Buffer_info[PICK].nBufferData[0][i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[0][i][EXIST] == YES)
						{
							clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_YES;	
						}
						else
						{
							clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_NO;	
						}
					}
					else
					{
						//clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT - i] = CTL_YES;
						//if (st_Buffer_info.nBufferData[1][nPickCnt - i][BIN] == GOOD)
						if (st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][EXIST] == YES)
						{
							clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT - i] = CTL_YES;
						}
						else
						{
							clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT - i] = CTL_NO;
						}
					}
				}
				else //Sodimm, Udimm_9
				{
					if (i <5)
					{
						//clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_YES;
						//kwlee 2017.0204
						//if (st_Buffer_info.nBufferData[0][i][BIN] == GOOD)
						if (st_Buffer_info[PICK].nBufferData[0][i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[0][i][EXIST] == YES)
						{
							clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_YES;
						}
						else
						{
							clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_NO;
						}
					}
					else
					{
						//clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT + 3 - i] = CTL_YES;	
						//kwlee 2017.0204
						//if (st_Buffer_info.nBufferData[1][nPickCnt - i][BIN] == GOOD)
						//if (st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][EXIST] == YES)
						if (st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][EXIST] == YES)
						{
							clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT + 3 - i] = CTL_YES;	
						}
						else
						{
							clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT + 3 - i] = CTL_NO;	
						}
					}
				}
			}
		}
	}
	
}
void CRunRobot::OnBlowSet(int nMode,int nPickCnt,int OnOff)
{

	if (OnOff == IO_ON)
	{
		for (int i =0; i<nPickCnt; i++ )
		{	
			if (nMode == 1)
			{
				if ((st_Picker_info.nPickerData[0][i][X_POS] == LEFT && st_Picker_info.nPickerData[0][i][FAILPICK] == YES) || 
					(st_Picker_info.nPickerData[1][nPickCnt - i][X_POS] == RIGHT && st_Picker_info.nPickerData[1][nPickCnt - i][FAILPICK] == YES))
				{
					if (i < 5 &&st_Picker_info.nPickerData[0][i][X_POS] == LEFT)
					{
						//if (st_Buffer_info[PICK].nBufferData[0][i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[0][i][EXIST] == YES)
						if (st_Picker_info.nPickerData[0][i][BIN] == FAIL && st_Picker_info.nPickerData[0][i][EXIST] == NO)
						{
							FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[st_Picker_info.nPickerData[0][i][Y_POS]],IO_ON);
						}
						//break;
					}
					else if (i > 4 && st_Picker_info.nPickerData[1][nPickCnt - i][X_POS] == RIGHT)
					{
						//if (st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][EXIST] == YES)
						//if (st_Buffer_info[PICK].nBufferData[1][i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[1][i][EXIST] == YES)
						if (st_Picker_info.nPickerData[1][nPickCnt - i][BIN] == FAIL && st_Picker_info.nPickerData[1][nPickCnt - i][EXIST] == NO)
						{
							FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[MAX_PICKER + st_Picker_info.nPickerData[1][nPickCnt - i][Y_POS]],IO_ON);
						}
						//break;
					}
					else
					{

					}
				}
			}
			else
			{
				if(m_bSecondPos == false)
				{
					if(st_basic_info.nPcbType == RDIMM)
					{
						if (i < 3)
						{
	// 						if (st_basic_info.nPickerSelect[i] == CTL_YES)
	// 						{
							FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[i],IO_ON);
							//FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[i],IO_ON);
							//}
						}
					}
					else//Sodimm,Udimm_9
					{
						if (i < 5)
						{
							FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[i],IO_ON);
						}
					}
				}
				else
				{
					if(st_basic_info.nPcbType == RDIMM)
					{
						//if(m_nPickCnt > (PICKCNT - i)) 
						if(i > 2) 
						{
	// 						if (st_basic_info.nPickerSelect[PICKCNT - i] == CTL_YES)
	// 						{
							FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[PICKCNT - i - 1],IO_ON);
	//						}
						}
					}
					else if (st_basic_info.nPcbType == UDIMM_10)
					{
						//if(m_nPickCnt >= (PICKCNT - 6 + i)) 
						if (i > 4)
						{
	// 						if (st_basic_info.nPickerSelect[PICKCNT - 6 + i] == CTL_YES)
	// 						{
							FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[PICKCNT + 3 - i],IO_ON);
							//}
						}
					}
					else
					{
						//if(m_nPickCnt > (PICKCNT - 6 + i)) 
						if (i > 4)
						{
	// 						if (st_basic_info.nPickerSelect[PICKCNT - 6 + i] == CTL_YES)
	// 						{
							//FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[PICKCNT + 4 - i],IO_ON);
							FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[PICKCNT + 3 - i],IO_ON);
							//}
						}
					}
				}
			}
		}//for
	}
	else //OnOff == IO_OFF
	{
		for (int i =0; i<nPickCnt; i++ )
		{
			if (nMode == 1)
			{
				if ((st_Picker_info.nPickerData[0][i][X_POS] == LEFT && st_Picker_info.nPickerData[0][i][FAILPICK] == YES) || 
					(st_Picker_info.nPickerData[1][nPickCnt - i][X_POS] == RIGHT && st_Picker_info.nPickerData[1][nPickCnt - i][FAILPICK] == YES))
				{
					if (i < 5 &&st_Picker_info.nPickerData[0][i][X_POS] == LEFT)
					{
						//if (st_Buffer_info[PICK].nBufferData[0][i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[0][i][EXIST] == YES)
						if (st_Picker_info.nPickerData[0][i][BIN] == FAIL && st_Picker_info.nPickerData[0][i][EXIST] == NO)
						{
							FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[st_Picker_info.nPickerData[0][i][Y_POS]],IO_ON);
						}
						//break;
					}
					else if (i > 4 && st_Picker_info.nPickerData[1][nPickCnt - i][X_POS] == RIGHT)
					{
						//if (st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][EXIST] == YES)
						//if (st_Buffer_info[PICK].nBufferData[1][i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[1][i][EXIST] == YES)
						if (st_Picker_info.nPickerData[1][nPickCnt - i][BIN] == FAIL && st_Picker_info.nPickerData[1][nPickCnt - i][EXIST] == NO)
						{
							FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[MAX_PICKER + st_Picker_info.nPickerData[1][nPickCnt - i][Y_POS]],IO_ON);
						}
						//break;
					}
					else
					{

					}
				}
			}
			else
			{
				if(m_bSecondPos == false)
				{
					if(st_basic_info.nPcbType == RDIMM)
					{
						if (i < 3)
						{
	// 						if (st_basic_info.nPickerSelect[i] == CTL_YES)
	// 						{
							FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[i],IO_OFF);
							//}
						}
					}
					else //Sodimm,Udimm_9
					{
						if (i < 5)
						{
	// 						if (st_basic_info.nPickerSelect[i] == CTL_YES)
	// 						{
							FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[i],IO_OFF);
							//}
						}
					}
				}
				else
				{
					if(st_basic_info.nPcbType == RDIMM)
					{
						//if(m_nPickCnt > (PICKCNT - i)) 
						{
	// 						if (st_basic_info.nPickerSelect[PICKCNT - i] == CTL_YES)
	// 						{
							FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[PICKCNT - i - 1],IO_OFF);
							//}
						}
					}
					else if (st_basic_info.nPcbType == UDIMM_10)
					{
						//if(m_nPickCnt >= (PICKCNT - 6 + i)) 
						{
	// 						if (st_basic_info.nPickerSelect[PICKCNT - 6 + i] == CTL_YES)
	// 						{
							FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[PICKCNT + 3 - i],IO_OFF);
							//}
									
						}
					}
					else
					{
						//if(m_nPickCnt > (PICKCNT - 6 + i)) 
						{
	// 						if (st_basic_info.nPickerSelect[PICKCNT - 6 + i] == CTL_YES)
	// 						{
							FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[PICKCNT + 3 - i],IO_OFF);
							//}
							//clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT - i ] = CTL_YES;	
						}
					}
				}
			}
		}//for	
	}
}

void CRunRobot::OnBarcodeReadCheck()
{
	int nRet = 0;
	int nCnt = 0;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < MAX_PICKER; j++)
		{
			if (st_Buffer_info[PICK].nBufferData[i][j][BIN] == GOOD && st_Buffer_info[PICK].strBufferSerial[i][j] !=_T(""))
			{
				st_Buffer_info[PICK].nBufferData[i][j][EXIST] = YES;
			}
			else
			{
				st_Buffer_info[PICK].nBufferData[i][j][EXIST] = NO;
				//nCnt++;
			}
		}
	}
}
//kwlee 2017.0202
int CRunRobot::OnPrinterFeeder(int nCnt, int nFailCheck)
{
	m_nCntBcrNum++;
	if (st_Buffer_info[0].strBufferSerial[0][nCnt] == _T("") &&  st_Buffer_info[0].strBufferSerial[1][nCnt] != _T(""))
	{
		st_Buffer_info[0].strBufferSerial[0][nCnt] = st_Buffer_info[0].strBufferSerial[0][nCnt];
		st_Buffer_info[0].strBufferSerial[1][nCnt] = st_Buffer_info[0].strBufferSerial[1][nCnt - 1];
	}

	st_Buffer_info[0].strBufferSerial[0][nCnt].Format(_T("1111_%d"),m_nCntBcrNum);
	st_Buffer_info[0].strBufferSerial[1][nCnt].Format(_T("2222_%d"),m_nCntBcrNum);

	
	return RET_GOOD;
}



void CRunRobot::OnVaccummSet(int nMode, int nPickCnt, int OnOff)
{
	int i;
	if (OnOff == IO_ON)
	{
		for (i = 0; i < nPickCnt; i++)
		{
			if (nMode == 1)
			{
				if ((st_Picker_info.nPickerData[0][i][X_POS] == LEFT && st_Picker_info.nPickerData[0][i][FAILPICK] == YES) || 
					(st_Picker_info.nPickerData[1][nPickCnt - i][X_POS] == RIGHT && st_Picker_info.nPickerData[1][nPickCnt - i][FAILPICK] == YES))
				{
					if (i < 5 &&st_Picker_info.nPickerData[0][i][X_POS] == LEFT)
					{
						//if (st_Buffer_info[PICK].nBufferData[0][i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[0][i][EXIST] == YES)
						if (st_Picker_info.nPickerData[0][i][BIN] == FAIL && st_Picker_info.nPickerData[0][i][EXIST] == NO)
						{
							FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[st_Picker_info.nPickerData[0][i][Y_POS]],IO_ON);
						}
						//break;
					}
					else if (i > 4 && st_Picker_info.nPickerData[1][nPickCnt -i][X_POS] == RIGHT)
					{
						//if (st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][EXIST] == YES)
						//if (st_Buffer_info[PICK].nBufferData[1][i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[1][i][EXIST] == YES)
						if (st_Picker_info.nPickerData[1][nPickCnt -i][BIN] == FAIL && st_Picker_info.nPickerData[1][nPickCnt -i][EXIST] == NO)
						{
							FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[MAX_PICKER + st_Picker_info.nPickerData[1][nPickCnt - i][Y_POS]],IO_ON);
						}
						//break;
					}
					else
					{

					}
				}
			}
			else
			{
				if (st_basic_info.nPcbType == UDIMM_10)
				{
					if( i < 5)
					{
						//FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[i],IO_ON);
						//kwlee 2017.0204
						//if (st_Buffer_info.nBufferData[0][i][BIN] == GOOD)
						if (st_Buffer_info[PICK].nBufferData[0][i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[0][i][EXIST] == YES)
						{
							FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[i],IO_ON);
						}
						else
						{
							FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[i],IO_OFF);
						}
					}
					else
					{
						//FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[PICKCNT + 5 - m_nPickerNum],IO_ON);
						if(nPickCnt >= (PICKCNT + 3 - i)) 
						{
							//FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[PICKCNT + 4 - i],IO_ON);
							//kwlee 2017.0204
							//if (st_Buffer_info.nBufferData[1][nPickCnt - i][BIN] == GOOD)
							if (st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][EXIST] == YES)
							{
								FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[PICKCNT + 3 - i],IO_ON);
							}
							else
							{
								FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[PICKCNT + 3 - i],IO_OFF);
							}
						}
					}
				}
				else
				{

					if (st_basic_info.nPcbType == RDIMM)
					{
						if (i < 3)
						{
							//FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[i],IO_ON);
							//kwlee 2017.0204
							//if (st_Buffer_info.nBufferData[0][i][BIN] == GOOD)
							if (st_Buffer_info[PICK].nBufferData[0][i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[0][i][EXIST] == YES)
							{
								FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[i],IO_ON);
							}
							else
							{
								FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[i],IO_OFF);
							}
						}
						else
						{	
							//FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[PICKCNT - i],IO_ON);
							//kwlee 2017.0204
							//if (st_Buffer_info.nBufferData[1][nPickCnt - i][BIN] == GOOD)
							if (st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][EXIST] == YES)
							{
								FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[PICKCNT - i -1],IO_ON);
							}
							else
							{
								FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[PICKCNT - i -1],IO_OFF);
							}
						
						}
					}
					else //Sodimm, Udimm_9
					{
				
						if (i <5)
						{
							//FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[i],IO_ON);	
							//kwlee 2017.0204
							//if (st_Buffer_info.nBufferData[0][i][BIN] == GOOD)
							if (st_Buffer_info[PICK].nBufferData[0][i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[0][i][EXIST] == YES)
							{
								FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[i],IO_ON);	
							}
							else
							{
								FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[i],IO_OFF);	
							}
						}
						else
						{
							//FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[PICKCNT + 3 - i],IO_ON);
							//kwlee 2017.0204
							//if (st_Buffer_info.nBufferData[1][nPickCnt - i][BIN] == GOOD)
							if (st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][EXIST] == YES)
							{
								FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[PICKCNT + 3 - i],IO_ON);
							}
							else
							{
								FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[PICKCNT + 3 - i],IO_OFF);
							}
						
						}
					}
				}
			}
		}
	}
	else //OnOff == IO_OFF
	{
		for (int i =0; i<nPickCnt; i++ )
		{
			// kwlee 2017.0211
			if (nMode == 1)
			{
				if ((st_Picker_info.nPickerData[0][i][X_POS] == LEFT && st_Picker_info.nPickerData[0][i][FAILPICK] == YES) || 
					(st_Picker_info.nPickerData[1][nPickCnt - i][X_POS] == RIGHT && st_Picker_info.nPickerData[1][nPickCnt - i][FAILPICK] == YES))
				{
					if (i < 5 &&st_Picker_info.nPickerData[0][i][X_POS] == LEFT)
					{
						//if (st_Buffer_info[PICK].nBufferData[0][i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[0][i][EXIST] == YES)
						if (st_Picker_info.nPickerData[0][i][BIN] == FAIL && st_Picker_info.nPickerData[0][i][EXIST] == NO)
						{
							FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[st_Picker_info.nPickerData[0][i][Y_POS]],IO_OFF);
						}
						//break;
					}
					else if (i > 4 && st_Picker_info.nPickerData[1][nPickCnt - i][X_POS] == RIGHT)
					{
						//if (st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][EXIST] == YES)
						//if (st_Buffer_info[PICK].nBufferData[1][i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[1][i][EXIST] == YES)
						if (st_Picker_info.nPickerData[1][i][BIN] == FAIL && st_Picker_info.nPickerData[1][nPickCnt - i][EXIST] == NO)
						{
							FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[MAX_PICKER + st_Picker_info.nPickerData[1][nPickCnt - i][Y_POS]],IO_OFF);
						}
						//break;
					}
					else
					{

					}
				}
			}
			else
			{
				if(m_bSecondPos == false)
				{
					if(st_basic_info.nPcbType == RDIMM)
					{
						if (i < 3)
						{

							FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[i],IO_OFF);
						}
					}
					else if (st_basic_info.nPcbType == UDIMM_10)
					{
						if (i < 5)
						{
						
							FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[i],IO_OFF);
						
						}
					}
				}
				else
				{
					if(st_basic_info.nPcbType == RDIMM)
					{
						//if(m_nPickCnt > (PICKCNT - i)) 
						if(i > 2) 
						{
							FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[PICKCNT - i - 1],IO_OFF);
						}
					}
					else if (st_basic_info.nPcbType == UDIMM_10)
					{
						//if (st_basic_info.nPickerSelect[PICKCNT - 6 + i] == CTL_YES)
						if (i > 4)
						{
							FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[PICKCNT + 3 - i],IO_OFF);
						}
					}
					else //Sodimm, Udimm_9
					{
						//if(m_nPickCnt > (PICKCNT - 6 + i))
						if(i > 4)
						{
	// 						if (st_basic_info.nPickerSelect[PICKCNT - 6 + i] == CTL_YES)
	// 						{
							FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[PICKCNT + 3 - i],IO_OFF);
							//}	
						}
					}
				}
			}
		}
	}
	
}
// void CRunRobot::OnPickVaccummSet()
// {
// 	int i;
// 	for (i = 0; i < m_nPickerNum; i++)
// 	{
// 		if (st_basic_info.nPcbType == UDIMM_10)
// 		{
// 			if( i < 5)
// 			{
// 				FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[i],IO_ON);
// 			}
// 			else
// 			{
// 				//FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[PICKCNT + 5 - m_nPickerNum],IO_ON);
// 				if(m_nPickCnt >= (PICKCNT + 4 - i)) 
// 				{
// 					FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[PICKCNT + 4 - i],IO_ON);
// 				}
// 			}
// 		}
// 		else
// 		{
// 
// 			if (st_basic_info.nPcbType == RDIMM)
// 			{
// 				if (i < 3)
// 				{
// 					FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[i],IO_ON);
// 				}
// 				else
// 				{	
// 					FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[PICKCNT - i],IO_ON);
// 				}
// 			}
// 			else //Sodimm, Udimm_9
// 			{
// // 				if (i <6)
// // 				{
// // 					FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[i],IO_ON);
// // 				}
// // 				else
// // 				{
// // 					FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[PICKCNT + 5 - i],IO_ON);
// // 				}
// 
// 				if (i <5)
// 				{
// 					FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[i],IO_ON);
// 				}
// 				else
// 				{
// 					//FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[PICKCNT + 4 - i],IO_ON);
// 					FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[PICKCNT + 3 - i],IO_ON);
// 				}
// 			}
// 		}
// 	}
// }

void CRunRobot::OnGetPickCheck(int nMode,int nPickCnt)
{
	int i;
	for (i =0; i<PICKCNT; i++ )
	{
		clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_NO;
	}

	//for (int i =0; i<PICKCNT; i++)
	for (i =0; i<nPickCnt; i++)
	{
		if (nMode == 1)
		{

		}
		else
		{
			if (st_basic_info.nPcbType == UDIMM_10)
			{
				if(i<5)
				{
					//clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_YES;
					//kwlee 2017.0204
					//if (st_Buffer_info.nBufferData[0][i][BIN] == GOOD)
					if (st_Buffer_info[PICK].nBufferData[0][i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[0][i][EXIST] == YES)
					{
						clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_YES;
					}
					else
					{
						clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_NO;
					}
				}
				else
				{
					//clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT + 3 - i] = CTL_YES;
					//if (st_Buffer_info.nBufferData[1][10 - i][BIN] == GOOD)
					if (st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][EXIST] == YES)
					{
						clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT + 3 - i] = CTL_YES;
					}
					else
					{
						clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT + 3 - i] = CTL_NO;
					}
				}
			}
			else
			{
				if (st_basic_info.nPcbType == RDIMM)
				{
					if (i < 3)
					{
						//clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_YES;
						//kwlee 2017.0204
						//if (st_Buffer_info.nBufferData[0][i][BIN] == GOOD)
						if (st_Buffer_info[PICK].nBufferData[0][i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[0][i][EXIST] == YES)
						{
							clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_YES;
						}
						else
						{
							clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_NO;
						}
					}
					else
					{
						//clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT - i] = CTL_YES;	
						//kwlee 2017.0204
						if (st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][EXIST] == YES)
						{
							clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT - i] = CTL_YES;
						}
						else
						{
							clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT - i] = CTL_NO;
						}
					}
				}
				else //Sodimm, Udimm_9
				{
					if (i <5)
					{
						//clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_YES;
						//kwlee 2017.0204
						if (st_Buffer_info[PICK].nBufferData[0][i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[0][i][EXIST] == YES)
						{
							clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_YES;
						}
						else
						{
							clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_NO;
						}
					}
					else
					{
						//clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT + 5 - i] = CTL_YES;	
						//kwlee 2017.0204
						if (st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][EXIST] == YES)
						{
							clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT + 3 - i] = CTL_YES;	
						}
						else
						{
							clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT + 3 - i] = CTL_NO;
						}
					}
				}
			}
		}
	}

	//nRet_1 = clsRunRobot.OnGetPickerUpDn(0, PICKER_DN,clsRunRobot.m_npTemp_Picker_YesNo);
//	return nRet_1;
}

void CRunRobot::OnSetPickerUpDn(int nMode, int nOnOff, int *npPickerInfo)
{
	int i;
	int nNum = PICKCNT;

	if (nMode > 1) return;

	for (i=0; i<nNum; i++)
	{
		if (npPickerInfo[i] == YES)
		{
			FAS_IO.set_out_bit(st_io_info.o_HeadCylUpDn[i], nOnOff);
			m_bPickUpDnFlag[nMode]			= false;
			m_dwPickUpDnWaitTime[nMode][0]	= GetCurrentTime();
		}
	}
}
void CRunRobot::OnFeederReq(int nStep)
{
	m_nInterFaceStep = nStep;
}
int CRunRobot::OnFeederInterface()
{
	int nRet;
	CString strMsg;
// 	if (FAS_IO.get_in_bit(st_io_info.i_LfAlarmChk,IO_ON) == IO_ON)
// 	{	
// 		//507151 1 A "Label Feeder LF_ALARM On Check Error."
// 		m_strAlarmCode.Format(_T("507151"));
// 		CTL_Lib.Alarm_Error_Occurrence(404, dWARNING, m_strAlarmCode);
// 		return RET_ERROR;
// 	}

	switch(m_nInterFaceStep)
	{
	case 0:
		//프린터 출력 요청.
		//nRet = FAS_IO.get_in_bit(st_io_info.i_LabelFeederProductChk1,IO_ON);
		//if (nRet != IO_ON && m_nPrintOutPutCnt <= 39)
		//{
			m_nInterFaceStep = 100;
		//}
		break;

	case 100:
		if( st_sync_info.nBcrReq == CTL_REQ)
		{
			st_sync_info.nBcrReq = CTL_READY;
			m_nInterFaceStep = 200;
		}
		break;

	case 200:
		nRet = OnPrinterFeeder(m_nPrintOutPutCnt,m_nLabelFailCheck); //Buffer 저장.
		if (nRet == RET_GOOD)
		{
			//출력
			m_nInterFaceStep = 1000;
		}
		else
		{
			CTL_Lib.Alarm_Error_Occurrence(720, dWARNING, m_strAlarmCode);
		}
		break;

	case 1000:
		//출력
		nRet = FAS_IO.get_in_bit(st_io_info.i_LabelFeederProductChk1,IO_ON);
		if (nRet == IO_ON && m_nPrintOutPutCnt >= 39)
		{
			m_dwTimeCheck[0] = GetCurrentTime();
			m_nInterFaceStep = 2000;
		}
		else
		{
			if (m_nPrintOutPutCnt == 16)
			{
				::PostMessage(st_handler_info.hWnd, WM_BARCODE_MSG, BARCODE_TRIGGER_1, 0);
				::PostMessage(st_handler_info.hWnd, WM_BARCODE_MSG, BARCODE_TRIGGER_2, 0);
			}
			m_nPrintOutPutCnt++;
			m_nInterFaceStep = 200;
		}
		break;



	case 2000:
		m_lFeederWaitTime[1] = GetCurrentTime();
		m_lFeederWaitTime[2] = m_lFeederWaitTime[1] - m_lFeederWaitTime[0];
		if (m_lFeederWaitTime[2] < 0) m_lFeederWaitTime[0] = GetCurrentTime();
		
		if (m_lFeederWaitTime[2] > 100)
		{
			st_sync_info.nPrinterStatus = BD_READY;
			st_sync_info.nBcrReq = CTL_CHANGE;
			m_nInterFaceStep = NONE;	
		}
		
		break;
	}
	return RET_PROCEED;
}

int CRunRobot::OnGetPickerUpDn(int nMode, int nOnOff, int *npPickerInfo)
{
	int i;
	int m_nCnt = 0;
	
	int nRet[PICKCNT];
	int nNum = PICKCNT;
	for(int i = 0; i<nNum; i++)
		nRet[i] = NO;

	CString strAlarm;

	for (i=0; i<nNum; i++)
	{
		if (npPickerInfo[i] == YES)
		{
			if (nOnOff == PICKER_UP)//IO_OFF
			{
				if (m_bPickUpDnFlag[nMode] == false && 
					FAS_IO.get_out_bit(st_io_info.o_HeadCylUpDn[i],	IO_OFF)	== IO_OFF)	
				{
					m_bPickUpDnFlag[nMode]			= true;
					m_dwPickUpDnWaitTime[nMode][0]	= GetCurrentTime();
				}
				else if (m_bPickUpDnFlag[nMode] == true &&
					FAS_IO.get_out_bit(st_io_info.o_HeadCylUpDn[i],	IO_OFF)	== IO_OFF)
				{
					m_dwPickUpDnWaitTime[nMode][1] = GetCurrentTime();
					m_dwPickUpDnWaitTime[nMode][2] = m_dwPickUpDnWaitTime[nMode][1] - m_dwPickUpDnWaitTime[nMode][0];

					if (m_dwPickUpDnWaitTime[nMode][2] <= 0)
					{
						m_dwPickUpDnWaitTime[nMode][0] = GetCurrentTime();
						return RET_PROCEED;
					}

					if (m_dwPickUpDnWaitTime[nMode][2] > (DWORD)st_wait_info.nOnWaitTime[WAIT_PICKER_UP_DN])
					{
						nRet[i] = YES;
					}
				}
				else
				{
					m_dwPickUpDnWaitTime[nMode][1] = GetCurrentTime();
					m_dwPickUpDnWaitTime[nMode][2] = m_dwPickUpDnWaitTime[nMode][1] - m_dwPickUpDnWaitTime[nMode][0];

					if (m_dwPickUpDnWaitTime[nMode][2] <= 0)
					{
						m_dwPickUpDnWaitTime[nMode][0] = GetCurrentTime();
						return RET_PROCEED;
					}

					if (m_dwPickUpDnWaitTime[nMode][2] > (DWORD)st_wait_info.nLimitWaitTime[WAIT_PICKER_UP_DN])
					{
						//505000 "Head Cylinder
						m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.o_HeadCylUpDn[i], nOnOff);
						//CTL_Lib.Alarm_Error_Occurrence(200, dWARNING, m_strAlarmCode);

						return RET_ERROR;
					}
				}
			}
			else//PICKER_DN IO_DN
			{
				if (m_bPickUpDnFlag[nMode] == false &&
				FAS_IO.get_out_bit(st_io_info.o_HeadCylUpDn[i],	IO_ON)	== IO_ON)
				{
					m_bPickUpDnFlag[nMode]			= true;
					m_dwPickUpDnWaitTime[nMode][0]	= GetCurrentTime();
				}
				else if (m_bPickUpDnFlag[nMode] == true &&
					FAS_IO.get_out_bit(st_io_info.o_HeadCylUpDn[i],	IO_ON)	== IO_ON)
				{
					m_dwPickUpDnWaitTime[nMode][1]	= GetCurrentTime();
					m_dwPickUpDnWaitTime[nMode][2]	= m_dwPickUpDnWaitTime[nMode][1] - m_dwPickUpDnWaitTime[nMode][0];

					if (m_dwPickUpDnWaitTime[nMode][2] <= 0)
					{
						m_dwPickUpDnWaitTime[nMode][0]	= GetCurrentTime();
						return RET_PROCEED;
					}

					if(m_dwPickUpDnWaitTime[nMode][2] > (DWORD)st_wait_info.nOnWaitTime[WAIT_PICKER_UP_DN])
					{
						nRet[i] = YES;
					}
				}
				else
				{
					m_dwPickUpDnWaitTime[nMode][1]	= GetCurrentTime();
					m_dwPickUpDnWaitTime[nMode][2]	= m_dwPickUpDnWaitTime[nMode][1] - m_dwPickUpDnWaitTime[nMode][0];

					if (m_dwPickUpDnWaitTime[nMode][2] <= 0)
					{
						m_dwPickUpDnWaitTime[nMode][0]	= GetCurrentTime();
						return RET_PROCEED;
					}

					if (m_dwPickUpDnWaitTime[nMode][2] > (DWORD)st_wait_info.nLimitWaitTime[WAIT_PICKER_UP_DN])
					{
						//505000 "Head Cylinder
						m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.o_HeadCylUpDn[i], nOnOff);
						//CTL_Lib.Alarm_Error_Occurrence(201, dWARNING, m_strAlarmCode);

						return RET_ERROR;
					}
				}
			}
		}
		else
		{
			nRet[i] = YES;
		}
	}

	m_nCnt = 0;
 	for (int i =0; i<nNum;i++)
 	{
 		if (nRet[i] != YES)
 		{
			break;
 		}
 		else
 		{
			m_nCnt++;	
 		} 		
 	}

	if( m_nCnt >= PICKCNT)
	{
		return RET_GOOD;
	}

//  	if (nRet[0] == YES && nRet[1] == YES) return RET_GOOD;
// 	return RET_PROCEED;
	return RET_PROCEED;
}

void CRunRobot::OnRobotRun()
{
	//return;//Test 이후 해제

	int nRet_1, nRet_2=0, nRet_3=0, nRect[12]={0,};
	int nRet,nTemp = 0;
	double dCurrentPos = 0;
	CString str;
	bool bOnePicker;
	//m_nRunStep = 0;
	
	m_dPitch_Y = (st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST] - st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_END])/5;
	m_dPitch_x = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_FIRST] - st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_END];

	m_lpAxisNum[0] = M_PICKERRBT_Y;
	m_lpAxisNum[1] = M_PICKERRBT_X;
	nTemp = st_basic_info.nColCnt * st_basic_info.nRowCnt;
	//if (st_basic_info.nPcbType == SODIMM)
	if (nTemp == SODIMM)
	{
		m_nPickCnt = st_basic_info.nColCnt * (st_basic_info.nRowCnt/2);
		st_basic_info.nPcbType = SODIMM;
	}
	else
	{
		m_nPickCnt = st_basic_info.nColCnt * st_basic_info.nRowCnt;
	}
	//m_nPickCnt = st_basic_info.nPcbCnt;
	//kwlee 2017.0209
// 	if (FAS_IO.get_in_bit(st_io_info.i_LfAlarmChk,IO_ON) == IO_ON)
// 	{	
// 		//507151 1 A "Label Feeder LF_ALARM On Check Error."
// 		m_strAlarmCode.Format(_T("507151"));
// 		CTL_Lib.Alarm_Error_Occurrence(600, dWARNING, m_strAlarmCode);
// 	//	m_nRunStep = 0;
// 
// 		return;
// 	}
	if(st_sync_info.TurnConvJobReady[CONVEYOR] == CTL_COMPLETE)
		st_sync_info.TurnConvJobReady[ROBOT] = CTL_NONE;

	switch (m_nRunStep)
	{
	case 0:
		//picker vaccum off
// 		for (int i = 0; i < PICKCNT; i++)
// 		{
// 			FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[i],IO_OFF);
// 
// 			//if (nRect[i] == IO_ON)
// 			//vaccum 확인 
// 			if (FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[i],IO_OFF) == IO_ON)
// 			{
// 				//505080 1 A "Head Vaccum #1 Off Check Error ."
// 				//505081 1 A "Head Vaccum #1 On Check Error ."
// 				m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_HeadVaccumCheck[i], IO_ON);
// 				CTL_Lib.Alarm_Error_Occurrence(610, dWARNING, m_strAlarmCode);
// 				m_nRunStep = 0;
// 				break;
// 			}
// 			else
// 			{ 
// 				if (i< PICKCNT -1)
// 				{
// 					continue;
// 				}						
// 				m_nRunStep = 100;								
// 			}
// 		}
		//kwlee 2017.0213
		m_nRunStep = 100;								
		break;

	case 100:
		// loader robot picker up 요청
		for (int i =0; i<PICKCNT; i++ )
		{
			clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_YES;
		}
		//picker up
		/*st_sync_info.TurnConvJobReady[ROBOT] = CTL_NONE;*/
		clsRunRobot.OnSetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);
		m_nRunStep = 200;
		break;

	case 200:	
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_Z, st_motor_info[M_PICKERRBT_Z].d_pos[ROBOT_SAFETY], COMI.mn_runspeed_rate);
		//z 축 up
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			m_nRunStep = 300;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(5200, dWARNING, st_alarm_info.strCode);
			m_nRunStep = 200;
		}
		break;
		//////////////////////////////

	case 300:	
		//ROBOT TURN
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_TURN, st_motor_info[M_PICKERRBT_TURN].d_pos[PITCH_UP_HEAD_TURN_ROTATOR_0], COMI.mn_runspeed_rate);

		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			m_nRunStep = 400;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			CTL_Lib.Alarm_Error_Occurrence(630, dWARNING, st_alarm_info.strCode);
			m_nRunStep = 300;
		}
		break;

	case 400:	
		////ROBOT PITCH 작업 위치 이동
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_PITCH, st_motor_info[M_PICKERRBT_PITCH].d_pos[PITCH_UP_HEAD_PICK_FEEDER], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			//m_nRunStep = 1000;
			m_nRunStep = 500;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(640, dWARNING, st_alarm_info.strCode);
			m_nRunStep = 400;
		}
		break;
		/////////////////////////////////////////////////////////////////////////////////////////

	case 500:
		// picker up 체크
		nRet_1 = clsRunRobot.OnGetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);
		if(nRet_1 == RET_GOOD)
		{
			//m_nRunStep = 600;
			m_nRunStep = 510;
		}
		else if(nRet_1 == RET_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(650, dWARNING, m_strAlarmCode);
		}
		break;

	case 510:
// 		OnSetRejectCylUpDn(IO_RUN_MODE,IO_ON);
// 		m_nRunStep = 520;
		//kwlee 2017.0218
		if(m_nLabelFailCheck == TRUE)
		{
			OnSetRejectCylUpDn(IO_RUN_MODE,IO_ON);
			m_nRunStep = 520;
		}
		else
		{
			m_nRunStep = 810;
		}
		
		break;

	case 520:
		nRet = OnGetRejectCylUpDn(IO_RUN_MODE,IO_ON);

		if (nRet == RET_GOOD)
		{
			m_nRunStep = 600;
		}
		else if(nRet == RET_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(650, dWARNING, m_strAlarmCode);
		}
		break;

		//kwlee 2017.0204
	case 600:
		m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_REJECT];
		m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_REJECT];
		
		//label pick 위치 이동
		nRet_1 = CTL_Lib.Linear_Move(ONLY_MOVE_START, m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, COMI.mn_runspeed_rate);

		if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
		{
			//m_nRunStep = 3000;
			//kwlee 2017.0204
			//m_nRunStep = 1200;
			m_nRunStep = 700;
		}
		else if(nRet_1 == BD_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(660, dWARNING, st_alarm_info.strCode);
		}
		break;

	case 700:
		//label pick 위치 확인
		nRet = CTL_Lib.Linear_Move(ONLY_MOVE_CHECK, m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, COMI.mn_runspeed_rate);
		if(nRet == BD_GOOD) //정상적으로 완료된 상태 
		{						
			//m_nRunStep = 4000;
			//m_nRunStep = 800;
			m_nRunStep = 710;
		}
		else if (nRet == BD_RETRY)
		{
			m_nRunStep = 600;
		}
		else if (nRet == BD_ERROR || nRet == BD_SAFETY)
		{			
			m_nRunStep = 600;
			CTL_Lib.Alarm_Error_Occurrence(670, dWARNING, st_alarm_info.strCode);
		}
		break;

	case 710:
// 		if (m_nLabelFailCheck == TRUE)
// 		{
// 			OnSetLabelPick(1,m_nPickCnt);
// 			//clsRunRobot.OnSetPickerUpDn(0, PICKER_DN, clsRunRobot.m_npTemp_Picker_YesNo);
// 		}
// 		else
// 		{		
// 			for (int i =0; i<m_nPickCnt; i++ )
// 			{
// 				//clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_YES;
// 				if(i<5)
// 				{
// 					clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_YES;
// 				}
// 				else if (i > 4)
// 				{
// 					clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT + 3 - i] = CTL_YES;
// 				}
// 			}
			//picker up
			/*st_sync_info.TurnConvJobReady[ROBOT] = CTL_NONE;*/
			//clsRunRobot.OnSetPickerUpDn(0, PICKER_DN, clsRunRobot.m_npTemp_Picker_YesNo);
		//m_dwTimeCheck[0] = GetCurrentTime();
	//	}

		//kwlee 2017.0218
		OnSetLabelPick(1,m_nPickCnt);
		clsRunRobot.OnSetPickerUpDn(0, PICKER_DN, clsRunRobot.m_npTemp_Picker_YesNo);
		m_nRunStep = 720;
		break;

	case 720:
// 		m_dwTimeCheck[1] = GetCurrentTime();
// 		m_dwTimeCheck[2] = m_dwTimeCheck[1] - m_dwTimeCheck[0];
// 		if( m_dwTimeCheck[2] <= 0 ) m_dwTimeCheck[0] = GetCurrentTime();
// 		if( m_dwTimeCheck[2] >  1000 )
// 		{
	
		nRet_1 = clsRunRobot.OnGetPickerUpDn(0, PICKER_DN,clsRunRobot.m_npTemp_Picker_YesNo);
		if(nRet_1 == RET_GOOD)
		{	
			m_nRunStep = 800;	
		}
		else if(nRet_1 == RET_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(750, dWARNING, m_strAlarmCode);
		}
		break;

	case 800:	
		//함수호출
		nRet_1 = COMI.Check_MotPosRange(M_PICKERRBT_X,st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_REJECT],st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_REJECT] + st_motor_info[M_PICKERRBT_X].d_allow);
		nRet_2 = COMI.Check_MotPosRange(M_PICKERRBT_Y, st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_REJECT],st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_REJECT] + st_motor_info[M_PICKERRBT_Y].d_allow);

		if (nRet_1 == BD_GOOD && nRet_2 == BD_GOOD)
		{
			nRet_3 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_Z, st_motor_info[M_PICKERRBT_Z].d_pos[ROBOT_REJECT_PLACE], COMI.mn_runspeed_rate);

			if (nRet_3 == BD_GOOD) //좌측으로 이동
			{
				//m_nRunStep = 900;
				m_nRunStep = 810;
			}
			else if (nRet_3 == BD_ERROR || nRet_3 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(680, dWARNING, st_alarm_info.strCode);
				//m_nRunStep = 0;
			}
		}
		else
		{
			m_strAlarmCode.Format(_T("%02d0006"), M_PICKERRBT_Z); //000008 0 A "MOTOR MOVE SAFETY CHECK ERROR -[M_ROBOT_X]."
			CTL_Lib.Alarm_Error_Occurrence(690, dMOTOR_WARNING, m_strAlarmCode);//dMOTOR_WARNING ==> 이때는 알람은 발생시키지 않는다(run status를 바꾸지 않는다).thread 에서만 판단하여 알람 발생시킨다 

			if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
			{
				clsMem.OnNormalMessageWrite(_T("Robot Z Place Reject Position Error"));
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}
		break;

	case 810:
		if (m_nLabelFailCheck == TRUE)
		{
			OnVaccummSet(1,m_nPickCnt,IO_OFF);
			OnBlowSet(1,m_nPickCnt,IO_ON);
		}
		else
		{
			
			OnVaccummSet(0,m_nPickCnt,IO_OFF);
			OnBlowSet(0,m_nPickCnt,IO_ON);
		}
		m_dwTimeCheck[0] = GetCurrentTime();
		m_nRunStep = 820;
		break;

	case 820:
		m_dwTimeCheck[1] = GetCurrentTime();
		m_dwTimeCheck[2] = m_dwTimeCheck[1] - m_dwTimeCheck[0];
		if( m_dwTimeCheck[2] <= 0 ) m_dwTimeCheck[0] = GetCurrentTime();
		if( m_dwTimeCheck[2] >  1000 )
		{
// 			clsRunRobot.OnSetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);
// 			OnBlowSet(0,IO_OFF);
// 			m_nRunStep = 830;
			//kwlee 2017.0214
			if (m_nLabelFailCheck == TRUE)
			{
				OnBlowSet(1,m_nPickCnt,IO_OFF);
			}
			else
			{
				OnBlowSet(0,m_nPickCnt,IO_OFF);
			}
			clsRunRobot.OnSetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);
			m_nRunStep = 830;
		}
		break;

	case 830:
		
		nRet_1 = clsRunRobot.OnGetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);

		if(nRet_1 == RET_GOOD)
		{
			m_nRunStep = 900;
		}
		else if(nRet_1 == RET_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(650, dWARNING, m_strAlarmCode);
		}
		break;
		
	case 900:	
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_Z, st_motor_info[M_PICKERRBT_Z].d_pos[ROBOT_SAFETY], COMI.mn_runspeed_rate);
		//z 축 up
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			//m_nRunStep = 1000;
			m_nRunStep = 910;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(700, dWARNING, st_alarm_info.strCode);
		}
		break;

	case 910:
		OnSetRejectCylUpDn(IO_RUN_MODE,IO_OFF);
		m_nRunStep = 920;
		break;

	case 920:
		nRet = OnGetRejectCylUpDn(IO_RUN_MODE,IO_OFF);

		if (nRet == RET_GOOD)
		{
			//m_nRunStep = 600;
			m_nRunStep = 1000;
		}
		else if(nRet == RET_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(650, dWARNING, m_strAlarmCode);
		}
		break;

	case 1000:
// 		m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST];
// 		m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_FIRST];
		//kwlee 2017.0213
		bOnePicker = false;
		if ( m_nLabelFailCheck == TRUE)
		{
			if (st_Buffer_info[PICK].nBufferData[1][0][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[1][0][EXIST] == YES)
			{	
				bOnePicker = true;
// 				for (int i = 0; i< 2; i++)
// 				{
					//for (int j = 0; j < MAX_PICKER; j++)
				for (int j = 0; j < MAX_PICKER; j++)
				{
					//if (st_Buffer_info[PICK].nBufferData[0][j][X_POS] == LEFT)
					if (st_Picker_info.nPickerData[0][j][X_POS] == LEFT)
					{
						m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_END];
						m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST]  - (st_Picker_info.nPickerData[0][j][Y_POS] * m_dPitch_Y);
						break;
					}
					else if (st_Picker_info.nPickerData[1][j][X_POS] == RIGHT)
					{
						m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_FIRST];
						m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST]  - (st_Picker_info.nPickerData[1][MAX_PICKER - j][Y_POS] * m_dPitch_Y);
						break;
					}
					else
					{

					}
				}
				//}
			}
			//kwlee 2017.0214
			else
			{
				m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST];
				m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_FIRST];
			}
		}
		else
		{
			m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST];
			m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_FIRST];
		}
		//label pick 위치 이동
		nRet_1 = CTL_Lib.Linear_Move(ONLY_MOVE_START, m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, COMI.mn_runspeed_rate);

		if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
		{
			//m_nRunStep = 2100;
			//kwlee 2017.0213
			if (bOnePicker == true)
			{
				m_dwTimeCheck[0] = GetCurrentTime();
				m_nRunStep = 4000;
			}
			else
			{
				m_nRunStep = 2100;
			}
		}
		else if(nRet_1 == BD_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(710, dWARNING, st_alarm_info.strCode);
		//	m_nRunStep = 0;
		}
		break;

		//kwlee 2017.0210
	case 2100:
		if (st_basic_info.nModeDevice == WITHOUT_DVC)
		{
			m_nRunStep = 3000;
		}
		else
		{		
			if ( m_nLabelFailCheck == TRUE)
			{			
				if (st_Picker_info.nPickerFailCnt != 1)
				{
					OnBufferDataTransfer();
				}
			}
			m_nRunStep = 3000;
		}
		break;

	case 3000:
		//label pick 위치 확인
		nRet = CTL_Lib.Linear_Move(ONLY_MOVE_CHECK, m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, COMI.mn_runspeed_rate);
		if(nRet == BD_GOOD) //정상적으로 완료된 상태 
		{						
			//m_nRunStep = 4000;
			m_nRunStep = 3100;
		}
		else if (nRet == BD_RETRY)
		{
			m_nRunStep = 2000;
		}
		else if (nRet == BD_ERROR || nRet == BD_SAFETY)
		{			
			m_nRunStep = 2000;
			CTL_Lib.Alarm_Error_Occurrence(720, dWARNING, st_alarm_info.strCode);
		}
		break;
		
		//kwlee 2017.0119 
		//1. Print 출력 Cnt 증가.
		//2. Serial Data 무조건 저장.
		//3. 로봇이 다른 작업 중 출력 할수 있도록 수정 해야함.
		//4. Fail 시 Buffer 관리 추가 해야 함.
	case 3100:
		if ( 0 && st_basic_info.nModeDevice == WITHOUT_DVC)
		{
			m_nRunStep = 3200;
		}
		else
		{
			if( st_sync_info.nBcrReq == CTL_CHANGE || st_sync_info.nBcrReq == CTL_CLEAR)
			{
				st_sync_info.nBcrReq = CTL_REQ;
			}
			m_dwTimeCheck[0] = GetCurrentTime();
			//OnFeederReq(COM_START);// kwlee 2017.0218
			m_nRunStep = 3200;
// 			nRet = OnPrinterFeeder(m_nPrintOutPutCnt,m_nLabelFailCheck);
// 			if (nRet == RET_GOOD)
// 			{
// 				//프린터 정상 출력 시 
// 				m_nPrintOutPutCnt++;
// 				m_nRunStep = 3200;
// 				//st_Buffer_info[PICK].nOutPutCnt = m_nPrintOutPutCnt;
// 			}
// 			else
// 			{
// 				//프린터 정상 출력 하지 않으면 알람.
// 				CTL_Lib.Alarm_Error_Occurrence(720, dWARNING, m_strAlarmCode);
// 			}
		}
		//m_nRunStep = 3200;
		break;

		//kwlee 2017.0119 Barcode Read
		//1. 바코드 Read
		//2. 양품인지 확인.
	case 3200:
		//if (FAS_IO.get_in_bit(st_io_info.i_LabelFeederProductChk1,IO_ON) && m_nPrintOutPutCnt >= 16)
		//kwlee 2017.0204
// 		if (st_basic_info.nModeDevice == WITHOUT_DVC)
// 		{
// 			m_dwTimeCheck[0] = GetCurrentTime();
// 			m_nRunStep = 4000;
// 		}
// 		else
// 		{
			
// 			nRet = FAS_IO.get_in_bit(st_io_info.i_LabelFeederProductChk1,IO_ON);
// 
// 			if (nRet == IO_ON && m_nPrintOutPutCnt >= 16)
// 			{
// 				m_dwTimeCheck[0] = GetCurrentTime();
// 				m_nRunStep = 4000;
// 			}
// 			else
// 			{
// 				if (m_nPrintOutPutCnt > 3) //Barcode Read 위치 확인.
// 				{
// 					::PostMessage(st_handler_info.hWnd, WM_BARCODE_MSG, BARCODE_TRIGGER_1, 0);
// 					::PostMessage(st_handler_info.hWnd, WM_BARCODE_MSG, BARCODE_TRIGGER_2, 0);
// 				}
// 				m_nRunStep = 3100;
// 			}


		//}
		//kwlee 2017.0218
		m_dwTimeCheck[1] = GetCurrentTime();
		m_dwTimeCheck[2] = m_dwTimeCheck[1] - m_dwTimeCheck[0];
		if( m_dwTimeCheck[2] <= 0 ) m_dwTimeCheck[0] = GetCurrentTime();
		if( m_dwTimeCheck[2] >  1000)
		{
			if (st_sync_info.nPrinterStatus == BD_READY && st_sync_info.nBcrReq == CTL_CHANGE)
			{
				m_nRunStep = 4000;
			}
		}
		break;

	case 4000:
		m_dwTimeCheck[1] = GetCurrentTime();
		m_dwTimeCheck[2] = m_dwTimeCheck[1] - m_dwTimeCheck[0];
		if( m_dwTimeCheck[2] <= 0 ) m_dwTimeCheck[0] = GetCurrentTime();
		if( m_dwTimeCheck[2] >  500)
		{
// 			//Label Pick 동작..
// 			OnSetLabelPick(0,m_nPickCnt); 
// 			clsRunRobot.OnSetPickerUpDn(0, PICKER_DN, clsRunRobot.m_npTemp_Picker_YesNo);
// 			m_nRunStep = 5000;
			//kwlee 2017.0204
			if (m_nLabelFailCheck == TRUE )
			{
				//Label Pick 동작..
				//if (st_Picker_info.nPickerFailCnt == 1)
				if (st_Buffer_info[PICK].nBufferData[1][0][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[1][0][EXIST] == YES)
				{
					//OnSetLabelPick(1,m_nPickCnt); 
					//kwlee 2017.0214
					for (int i = 0; i<TOTAL_PICK; i++)
					{
						if (i < 5 && st_Picker_info.nPickerData[0][i][BIN] == FAIL && st_Picker_info.nPickerData[0][i][EXIST] == NO)
						{
							clsRunRobot.m_npTemp_Picker_YesNo[st_Picker_info.nPickerData[0][i][Y_POS]] = CTL_YES;
							break;
						}
						else if ( i > 4 && st_Picker_info.nPickerData[1][TOTAL_PICK - i][BIN] == FAIL && st_Picker_info.nPickerData[1][TOTAL_PICK - i][EXIST] == NO)
						{
							clsRunRobot.m_npTemp_Picker_YesNo[MAX_PICKER + st_Picker_info.nPickerData[1][TOTAL_PICK - i][Y_POS]] = CTL_YES;
							break;
						}
					}
					clsRunRobot.OnSetPickerUpDn(0, PICKER_DN, clsRunRobot.m_npTemp_Picker_YesNo);
					break;
				}
				else
				{
					OnBarcodeReadCheck();
					OnSetLabelPick(1,m_nPickCnt); 
					clsRunRobot.OnSetPickerUpDn(0, PICKER_DN, clsRunRobot.m_npTemp_Picker_YesNo);
				}
			}
			else
			{
				//Label Pick 동작..
				OnBarcodeReadCheck();
				OnSetLabelPick(0,m_nPickCnt); 
				clsRunRobot.OnSetPickerUpDn(0, PICKER_DN, clsRunRobot.m_npTemp_Picker_YesNo);
			}
			m_nRunStep = 5000;
		}
		break;
		
	case 5000:
		//2017.0116
		//vaccum On
		//OnVaccummSet(0,m_nPickCnt,IO_ON);
		//kwlee 2017.0204
		if (m_nLabelFailCheck == TRUE)
		{
			OnVaccummSet(1,m_nPickCnt,IO_ON);
			m_nRunStep = 7000;
		}
		else
		{
			OnVaccummSet(0,m_nPickCnt,IO_ON);
			m_nRunStep = 6000;
		}
		break;

	case 6000:	
		//함수호출
		nRet_1 = COMI.Check_MotPosRange(M_PICKERRBT_X,st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_FIRST],st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_FIRST] + st_motor_info[M_PICKERRBT_X].d_allow);
		nRet_2 = COMI.Check_MotPosRange(M_PICKERRBT_Y, st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST],st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST] + st_motor_info[M_PICKERRBT_Y].d_allow);	
		
		if (nRet_1 == BD_GOOD && nRet_2 == BD_GOOD)
		{
			nRet_3 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_Z, st_motor_info[M_PICKERRBT_Z].d_pos[ROBOT_FEEDER_PICK_FIRST], COMI.mn_runspeed_rate);

			if (nRet_3 == BD_GOOD) //좌측으로 이동
			{
				m_nRunStep = 7000;
			}
			else if (nRet_3 == BD_ERROR || nRet_3 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(730, dWARNING, st_alarm_info.strCode);
				//m_nRunStep = 0;
			}
		}
		else
		{
			m_strAlarmCode.Format(_T("%02d0006"), M_PICKERRBT_Z); //000008 0 A "MOTOR MOVE SAFETY CHECK ERROR -[M_ROBOT_X]."
			CTL_Lib.Alarm_Error_Occurrence(740, dMOTOR_WARNING, m_strAlarmCode);//dMOTOR_WARNING ==> 이때는 알람은 발생시키지 않는다(run status를 바꾸지 않는다).thread 에서만 판단하여 알람 발생시킨다 

			if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
			{
				clsMem.OnNormalMessageWrite(_T("Robot Z Place Position Error"));
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}
		break;

	case 7000:
		//picker down
		//OnGetPickCheck(m_nPickCnt); //kwlee 2017.0204
		nRet_1 = clsRunRobot.OnGetPickerUpDn(0, PICKER_DN,clsRunRobot.m_npTemp_Picker_YesNo);
		if(nRet_1 == RET_GOOD)
		{
			mn_Retry = 0;
			m_nRunStep = 7100;	
			//kwlee 2017.0202 사용 않함
			//OnFeederReq(COM_START);//label Pick &발행 요청.
		}
		else if(nRet_1 == RET_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(750, dWARNING, m_strAlarmCode);
		}
		break;

	case 7100:
		clsRunRobot.OnSetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);
		m_nRunStep = 7110;
		break;

	case 7110:
		//clsRunRobot.OnSetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);
		//m_nRunStep = 7120;
		nRet_1 = clsRunRobot.OnGetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);

		if(nRet_1 == RET_GOOD)
		{
			m_nRunStep = 7300;
			mn_Retry = 0;
		}
		break;

	case 7300:	
		//z축 up
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_Z, st_motor_info[M_PICKERRBT_Z].d_pos[ROBOT_SAFETY], COMI.mn_runspeed_rate);

		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			m_nRunStep = 7310;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(760, dWARNING, st_alarm_info.strCode);
			m_nRunStep = 7300;
		}
		break;

		///////////////////////////////////////////////////////////////////
	case 7310:
		if ( 0 && st_basic_info.nModeDevice == WITHOUT_DVC)
		{
			m_nRunStep = 13000;
			m_nLabelFailCheck = FALSE;
		}
		else
		{
			nRet = OnGetVaccumgmCheck(IO_ON,m_nPickCnt);
			if(nRet == RET_GOOD)
			{
				m_nLabelFailCheck = FALSE;
				m_nPrintOutPutCnt = m_nPrintOutPutCnt - MAX_PICKER;
				OnDataExchange(PICK,m_nLabelFailCheck);
				st_sync_info.nBcrReq = CTL_REQ;
				m_nRunStep = 13000;
			}
			else
			{
				m_nLabelFailCheck = TRUE;
				//st_Buffer_info[PICK].nBufferFailCnt = nCnt;
				m_nPrintOutPutCnt = m_nPrintOutPutCnt - MAX_PICKER;
				OnDataExchange(PICK,m_nLabelFailCheck);
				st_sync_info.nBcrReq = CTL_REQ;
				//kwlee 2017.0211
				//m_nRunStep = 7320;
				m_nRunStep = 510;
			}
		}
		break;

		//kwlee 2017.0213
// 		case 7320:
// 			//kwlee 2017.0210
// 			m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_REJECT];
// 			m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_REJECT];
// 			//
// 			nRet_1 = CTL_Lib.Linear_Move(ONLY_MOVE_START, m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, COMI.mn_manualspeed_rate);
// 			if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
// 			{
// 				m_nRunStep = 7330;
// 			}
// 			else if(nRet_1 == BD_ERROR)
// 			{
// 				CTL_Lib.Alarm_Error_Occurrence(780, dWARNING, st_alarm_info.strCode);
// 			}
// 			break;
// 
// 		case 7330:
// 			nRet = CTL_Lib.Linear_Move(ONLY_MOVE_CHECK, m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, COMI.mn_manualspeed_rate);
// 			if(nRet == BD_GOOD) //정상적으로 완료된 상태 
// 			{						
// 				//m_RunMoveStep = 0;
// 				m_nRunStep = 7340;
// 			}
// 			else if (nRet == BD_RETRY)
// 			{
// 				m_nRunStep = 7320;
// 			}
// 			else if (nRet == BD_ERROR || nRet == BD_SAFETY)
// 			{			
// 				m_nRunStep = 7320;
// 				CTL_Lib.Alarm_Error_Occurrence(790, dWARNING, st_alarm_info.strCode);
// 			}
// 			break;
// 
// 		case 7340:
// 			//kwlee 2017.0212
// 			OnSetLabelPick(1,m_nPickCnt); 
// 			clsRunRobot.OnSetPickerUpDn(0, PICKER_DN, clsRunRobot.m_npTemp_Picker_YesNo);
// 			m_dwTimeCheck[0] = GetCurrentTime();
// 			m_nRunStep = 7350;
// 			break;
// 			
// 
//  		case 7350:
// 			m_dwTimeCheck[1] = GetCurrentTime();
// 			m_dwTimeCheck[2] = m_dwTimeCheck[1] - m_dwTimeCheck[0];
// 			if( m_dwTimeCheck[2] <= 0 ) m_dwTimeCheck[0] = GetCurrentTime();
// 			if( m_dwTimeCheck[2] >  st_wait_info.dOnWaitTime[WAIT_PICKER_UP_DN] )
// 			{
// 				//clsRunRobot.OnSetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);
// 				//m_nRunStep = 7120;
// 				nRet_1 = clsRunRobot.OnGetPickerUpDn(0,PICKER_DN, clsRunRobot.m_npTemp_Picker_YesNo);
// 
// 				if(nRet_1 == RET_GOOD)
// 				{
// 					m_nRunStep = 7360;
// 					mn_Retry = 0;
// 				}
// 			}
//  			break;
// 
// 	
// 		case 7360:
// 			nRet_1 = COMI.Check_MotPosRange(M_PICKERRBT_X,st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_FIRST],st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_REJECT] + st_motor_info[M_PICKERRBT_X].d_allow);
// 			nRet_2 = COMI.Check_MotPosRange(M_PICKERRBT_Y, st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST],st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_REJECT] + st_motor_info[M_PICKERRBT_Y].d_allow);	
// 
// 			if (nRet_1 == BD_GOOD && nRet_2 == BD_GOOD)
// 			{
// 				nRet_3 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_Z, st_motor_info[M_PICKERRBT_Z].d_pos[ROBOT_REJECT_PLACE], COMI.mn_runspeed_rate);
// 
// 				if (nRet_3 == BD_GOOD) //좌측으로 이동
// 				{
// 					m_nRunStep = 7370;
// 				}
// 				else if (nRet_3 == BD_ERROR || nRet_3 == BD_SAFETY)
// 				{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
// 					CTL_Lib.Alarm_Error_Occurrence(730, dWARNING, st_alarm_info.strCode);
// 					//m_nRunStep = 0;
// 				}
// 			}
// 			else
// 			{
// 				m_strAlarmCode.Format(_T("%02d0006"), M_PICKERRBT_Z); //000008 0 A "MOTOR MOVE SAFETY CHECK ERROR -[M_ROBOT_X]."
// 				CTL_Lib.Alarm_Error_Occurrence(740, dMOTOR_WARNING, m_strAlarmCode);//dMOTOR_WARNING ==> 이때는 알람은 발생시키지 않는다(run status를 바꾸지 않는다).thread 에서만 판단하여 알람 발생시킨다 
// 
// 				if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
// 				{
// 					clsMem.OnNormalMessageWrite(_T("Robot Z Reject Place Position Error"));
// 					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
// 				}
// 			}
// 			break;
// 
// 			case 7370:
// 			clsRunRobot.OnSetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);
// 			m_dwTimeCheck[0] = GetCurrentTime();
// 			m_nRunStep = 7380;
// 			break;
// 
// 		case 7380:
// 			m_dwTimeCheck[1] = GetCurrentTime();
// 			m_dwTimeCheck[2] = m_dwTimeCheck[1] - m_dwTimeCheck[0];
// 			if( m_dwTimeCheck[2] <= 0 ) m_dwTimeCheck[0] = GetCurrentTime();
// 			if( m_dwTimeCheck[2] >  st_wait_info.dOnWaitTime[WAIT_PICKER_VACCUM] )
// 			{
// 				//clsRunRobot.OnSetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);
// 				//m_nRunStep = 7120;
// 				nRet_1 = clsRunRobot.OnGetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);
// 
// 				if(nRet_1 == RET_GOOD)
// 				{
// // 					m_nRunStep = 7390;
// // 					mn_Retry = 0;
// 
// 					//kwlee 2017.0211
// 					nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_Z, st_motor_info[M_PICKERRBT_Z].d_pos[ROBOT_SAFETY], COMI.mn_runspeed_rate);
// 
// 					if (nRet_1 == BD_GOOD) //좌측으로 이동
// 					{
// 						m_nRunStep = 7390;
// 					}
// 					else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
// 					{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
// 						CTL_Lib.Alarm_Error_Occurrence(760, dWARNING, st_alarm_info.strCode);
// 						//m_nRunStep = 7300;
// 					}	
// 				}
// 			}
// 			break;
// 
// 		case 7390:
// 			OnVaccummSet(1,m_nPickCnt,IO_OFF);
// 			//m_dwTimeCheck[0] = GetCurrentTime();
// 			m_nRunStep = 7400;
// 			break;
// 
// 		case 7400:
// 
// 			OnBlowSet(1,IO_ON);
// 			m_dwTimeCheck[0] = GetCurrentTime();
// 			m_nRunStep = 7410;
// 			break;
// 
// 		case 7410:
// 			m_dwTimeCheck[1] = GetCurrentTime();
// 			m_dwTimeCheck[2] = m_dwTimeCheck[1] - m_dwTimeCheck[0];
// 			if( m_dwTimeCheck[2] <= 0 ) m_dwTimeCheck[0] = GetCurrentTime();
// 			if( m_dwTimeCheck[2] >  st_wait_info.dOnWaitTime[WAIT_PICKER_VACCUM] )
// 			{
// 				OnBlowSet(1,IO_OFF);
// 				m_nRunStep = 7420;
// 			}
// 			break;
			//////////////////
// 		case 7420:
// 			if (st_Buffer_info[PICK].nBufferData[1][0][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[1][0][EXIST] == YES)
// 			{	
// 				for (int i = 0; i< 2; i++)
// 				{
// 					//for (int j = 0; j < MAX_PICKER; j++)
// 					for (int j = 0; j < MAX_PICKER; j++)
// 					{
// 						//if (st_Buffer_info.nBufferData[i][j][BIN] == FAIL && st_Buffer_info.nBufferData[i][j][EXIST] == NO)
// 						//{
// 						 if (st_Buffer_info[PICK].nBufferData[0][j][X_POS] == LEFT)
// 						{
// 							m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_END];
// 							m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST]  - (st_Picker_info.nPickerData[0][j][Y_POS] * m_dPitch_Y);
// 							//m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST]  - (st_Buffer_info[PICK].nBufferData[i][j][Y_POS] * m_dPitch_Y);
// 							//m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST]  - (st_Buffer_info[PICK].nBufferData[0][j][Y_POS] * m_dPitch_Y);
// 							break;
// 						}
// 						else if (st_Buffer_info[PICK].nBufferData[1][j][X_POS] == RIGHT)
// 						{
// 							m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_FIRST];
// 							m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST]  - (st_Picker_info.nPickerData[1][MAX_PICKER - j][Y_POS] * m_dPitch_Y);
// 							//m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST]  - (st_Buffer_info[PICK].nBufferData[1][m_nPickCnt - j][Y_POS] * m_dPitch_Y);
// 							break;
// 						}
// 						else
// 						{
// 			
// 						}
// 						//}
// 					}
// 				}
// 				//m_nRunStep = 7330;
// 				m_nRunStep = 7440;
// 			}
// 			else
// 			{
// 				//m_nRunStep = 3100;
// 				m_nRunStep = 2000;
// 			}
// 			break;

		//case 7440:
// 			nRet_1 = CTL_Lib.Linear_Move(ONLY_MOVE_START, m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, COMI.mn_manualspeed_rate);
// 			if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
// 			{
// 				m_nRunStep = 7450;
// 			}
// 			else if(nRet_1 == BD_ERROR)
// 			{
// 				CTL_Lib.Alarm_Error_Occurrence(780, dWARNING, st_alarm_info.strCode);
// 			}
// 
// 
// 			break;
// 
// 		case 7450:
// 			OnSetLabelPick(1, m_nPickCnt);
// 			clsRunRobot.OnSetPickerUpDn(0, PICKER_DN, clsRunRobot.m_npTemp_Picker_YesNo);
// 			m_dwTimeCheck[0] = GetCurrentTime();
// 			m_nRunStep = 7460;
// 			break;
// 
// 		case 7460:
// 			m_dwTimeCheck[1] = GetCurrentTime();
// 			m_dwTimeCheck[2] = m_dwTimeCheck[1] - m_dwTimeCheck[0];
// 			if( m_dwTimeCheck[2] <= 0 ) m_dwTimeCheck[0] = GetCurrentTime();
// 			if( m_dwTimeCheck[2] >  st_wait_info.dOnWaitTime[WAIT_PICKER_UP_DN] )
// 			{
// 				//clsRunRobot.OnSetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);
// 				//m_nRunStep = 7120;
// 				nRet_1 = clsRunRobot.OnGetPickerUpDn(0,PICKER_DN, clsRunRobot.m_npTemp_Picker_YesNo);
// 
// 				if(nRet_1 == RET_GOOD)
// 				{
// 					m_nRunStep = 7500;
// 				}
// 			}
// 			break;
		

	case 7500:
// 		if (m_nLabelFailCheck == TRUE)
// 		{
// 			//m_nRunStep = 3100;
// 			m_nRunStep = 1400;
// 		}
// 		else
// 		{
// 			//모두 집었다.
// 			//정상 이다.
// 			//m_nRunStep = 7500;
// 			m_nRunStep = 9000;
// 		}
		break;


		////////////////////////////////////////////////////
		//MOVE
	case 9000:
// 		for (int i = 0; i < PICKCNT; i++)
// 		{
// 			nRect[i] = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[i],IO_ON);
// 			if (nRect[i] == IO_ON)
// 			{ 
// 			//	if (i< PICKCNT -1)
// 
// 				if (i< m_nPickCnt -1)
// 				{
// 					continue;
// 				}	
// 				m_nRunStep = 13000;		
// 			}
// 		}
	
		break;

	case 13000:	
		//ROBOT TURN
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_TURN, st_motor_info[M_PICKERRBT_TURN].d_pos[PITCH_UP_HEAD_TURN_ROTATOR_R_90], COMI.mn_runspeed_rate);

		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			m_nRunStep = 14000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(810, dWARNING, st_alarm_info.strCode);
			m_nRunStep = 13000;
			
		}
		break;

// 	case 13400:
// 		nRect[0] = FAS_IO.get_in_bit(st_io_info.i_TurnRotatCylForChk,IO_ON);
// 		nRect[1] = FAS_IO.get_in_bit(st_io_info.i_TurnRotatCylBackChk,IO_ON);
// 		m_lTurnConvWaitTime[1] = GetCurrentTime();
// 		m_lTurnConvWaitTime[2] = m_lTurnConvWaitTime[1] - m_lTurnConvWaitTime[0];
// 		
// 		if (m_lTurnConvWaitTime[2] > 500)
// 		{
// 			if (nRect[0] == IO_OFF && nRect[1] == IO_OFF)
// 			{
// 				//502060 1 A "TURN CONV Table Motor Forward Cylinder-Sol Error."
// 				CTL_Lib.Alarm_Error_Occurrence(260, dWARNING, _T("502060"));
// 			}
// 			else
// 			{
// 				m_nRunStep = 14000;
// 			}
// 
// 		}
// 		break;
	
		///////////////////////////////////
		//붙이는 위치 이동.
	case 14000:
// 		m_lTurnConvWaitTime[1] = GetCurrentTime();
// 		m_lTurnConvWaitTime[2] = m_lTurnConvWaitTime[1] - m_lTurnConvWaitTime[0];
// 
// 		if (m_lTurnConvWaitTime[2] > 1000 )
// 		{
// 			if (st_sync_info.TurnConvJobReady[CONVEYOR] == CTL_READY)
// 			{
// 				if(st_basic_info.nPcbType == RDIMM )
// 				{
// 					if (m_bSecondPos == false)
// 					{
// 						m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_FIRST];
// 						m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_FIRST];
// 					}
// 					else
// 					{
// 						m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_THIRD];
// 						m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_THIRD];
// 					}
// 				}
// 				else if (st_basic_info.nPcbType == UDIMM_10)
// 				{
// 					if (m_bSecondPos == false)
// 					{
// 						m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_FIRST];
// 						m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_FIRST];
// 					}
// 					else
// 					{
// 						m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_FIRST];
// 						m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_FIRST];
// 					}
// 				}
// 				else
// 				{
// 					if (m_bSecondPos == false)
// 					{
// 						m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_FIRST];
// 						m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_FIRST];
// 					}
// 					else
// 					{
// 						m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_SECOND];
// 						m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_SECOND];
// 					}
// 				}

			if (m_bSecondPos == false)
			{
				if(st_basic_info.nPcbType == RDIMM )
				{
					m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_FIRST];
					m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_FIRST];
				}
				else if (st_basic_info.nPcbType == UDIMM_10)
				{
					m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_FIRST];
					m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_FIRST];
				}
				else
				{
					m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_FIRST];
					m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_FIRST];
				}
			}
			else //m_bSecondPos == true
			{
				m_lTurnConvWaitTime[1] = GetCurrentTime();
				m_lTurnConvWaitTime[2] = m_lTurnConvWaitTime[1] - m_lTurnConvWaitTime[0];

				if (m_lTurnConvWaitTime[2] > 1000 )
				{
					if (st_sync_info.TurnConvJobReady[CONVEYOR] == CTL_READY)
					{
						if(st_basic_info.nPcbType == RDIMM )
						{
							m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_THIRD];
							m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_THIRD];
						}
						else if (st_basic_info.nPcbType == UDIMM_10)
						{
							m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_FIRST];
							m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_FIRST];
						}
						else
						{
							m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_SECOND];
							m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_SECOND];
						}
					}
				}
			}
			m_lpAxisNum[0] = M_PICKERRBT_Y; //m_lpAxisNum[4];        //현재의 IndexNum에서 사용하는 모터 실재 번호를 가진다 
			m_lpAxisNum[1] = M_PICKERRBT_X;

			nRet_1 = CTL_Lib.Linear_Move(ONLY_MOVE_START, m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, COMI.mn_manualspeed_rate);

			if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
			{
				m_nRunStep = 15000;
			}
			else if(nRet_1 == BD_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(820, dWARNING, st_alarm_info.strCode);
				//m_nRunStep = 0;
			}
			//}
		//}
		break;

	case 15000:
		nRet = CTL_Lib.Linear_Move(ONLY_MOVE_CHECK, m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, COMI.mn_manualspeed_rate);
		if(nRet == BD_GOOD) //정상적으로 완료된 상태 
		{						
			m_nRunStep = 16000;
		}
		else if(nRet == BD_ERROR)
		{			
			m_nRunStep = 14000;
			CTL_Lib.Alarm_Error_Occurrence(830, dWARNING, st_alarm_info.strCode);
		}			
		break;

		/////////////////////////////////////////////////////////////////////////////////////////////
		////ROBOT PITCH 작업 위치 이동
	case 16000:	
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_PITCH, st_motor_info[M_PICKERRBT_PITCH].d_pos[PITCH_UP_HEAD_PLACE_PCB], COMI.mn_manualspeed_rate);

		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			m_nRunStep = 16100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(840, dWARNING, st_alarm_info.strCode);
			//m_nRunStep = 0;
		}
		break;

		//////////////////////////////
		//붙이는 작업 
	case 16100:
		OnSetLabelPlace(m_nPickCnt);
		clsRunRobot.OnSetPickerUpDn(0, PICKER_DN, clsRunRobot.m_npTemp_Picker_YesNo);
		m_nRunStep = 17000;
		//m_nRunStep = 16200;
		break;


	case 17000:	
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_Z, st_motor_info[M_PICKERRBT_Z].d_pos[ROBOT_Z_PLACE], COMI.mn_manualspeed_rate);

		if (nRet_1 == BD_GOOD) 
		{
			m_nRunStep = 18000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(860, dWARNING, st_alarm_info.strCode);
			//m_nRunStep = 0;
		}
		break;

	case 18000:
		// loader robot picker Dw 체크
		nRet_1 = clsRunRobot.OnGetPickerUpDn(0, PICKER_DN, clsRunRobot.m_npTemp_Picker_YesNo);

		if(nRet_1 == RET_GOOD)
		{
			m_nRunStep = 19000;
		}
		else if(nRet_1 == RET_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(870, dWARNING, m_strAlarmCode);
		}
		break;

	case 19000:
		//vaccumm Off
		OnVaccummSet(0,m_nPickCnt,IO_OFF);
		m_nRunStep = 19100;
		break;

	case 19100:
		//Blow
		OnBlowSet(0,m_nPickCnt,IO_ON);
		m_lVaccumOffTime[0] = GetCurrentTime();
		m_nRunStep = 20000;
		break;

	case 20000:
		// loader robot picker up 요청
		//picker up
		OnSetLabelPlace(m_nPickCnt);
		clsRunRobot.OnSetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);
		m_nRunStep = 21000;
		break;

	case 21000:	
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_Z, st_motor_info[M_PICKERRBT_Z].d_pos[ROBOT_SAFETY], COMI.mn_manualspeed_rate);
		//z 축 up
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			m_nRunStep = 21100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(880, dWARNING, st_alarm_info.strCode);
			m_nRunStep = 21000;
		}
		break;

	case 21100:
		m_lVaccumOffTime[1] = GetCurrentTime();
		m_lVaccumOffTime[2] = m_lVaccumOffTime[1] - m_lVaccumOffTime[0];

		if (m_lVaccumOffTime[2] > st_wait_info.nOffWaitTime[WAIT_PICKER_VACCUM])
		{
			OnBlowSet(0,m_nPickCnt,IO_OFF);
		}
		m_nRunStep = 22000;
		break;

	case 22000:
		// picker up 체크
		nRet_1 = clsRunRobot.OnGetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);
		OnDataExchange(PLACE,m_nLabelFailCheck);
		if(nRet_1 == RET_GOOD)
		{
			if(m_bSecondPos == false)
			{
				//m_nRunStep = 13200;
				if (st_basic_info.nPcbType == RDIMM || st_basic_info.nPcbType == UDIMM_10)
				{
					m_nRunStep = 23000;
				}
				else
				{
					m_nRunStep = 14000;
				}

				m_bSecondPos = true;
			}
			else
			{
				if (st_basic_info.nPcbType == RDIMM || st_basic_info.nPcbType == UDIMM_10)
				{
					m_nRdimmJobState = CTL_COMPLETE;
				}
				
				m_nRunStep = 23000;
				m_nPickerNum = 0;
				m_bSecondPos = false;
				m_nLinearMove_Index = 0;
				CTL_Lib.m_nStep_Linear[m_nLinearMove_Index] = 0;
			}

			for (int i = 0; i<PICKCNT; i++)
			{
				clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_NO;	
			}
		}
		else if(nRet_1 == RET_ERROR)
		{
			//	m_RunRelayMoveStep = 0;
			CTL_Lib.Alarm_Error_Occurrence(890, dWARNING, m_strAlarmCode);
		}
		break;

		//////////////////////////////
		//Safety Postion 위치 이동 대기.
	case 23000:
		m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_SAFETY];
		m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_SAFETY];

		m_lpAxisNum[0] = M_PICKERRBT_Y; //m_lpAxisNum[4];        //현재의 IndexNum에서 사용하는 모터 실재 번호를 가진다 
		m_lpAxisNum[1] = M_PICKERRBT_X;

		nRet_1 = CTL_Lib.Linear_Move(ONLY_MOVE_START, m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, COMI.mn_runspeed_rate);

		if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
		{
			m_nRunStep = 23100;
		}
		else if(nRet_1 == BD_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(900, dWARNING, st_alarm_info.strCode);
			//m_nRunStep = 0;
		}
		break;
		//kwlee 2017.0203
		//////Vision Read 위치

		///////
	case 23100:
		//Safety 위치 이동 완료
		nRet = CTL_Lib.Linear_Move(ONLY_MOVE_CHECK, m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, COMI.mn_runspeed_rate);
		if(nRet == BD_GOOD) //정상적으로 완료된 상태 
		{						
			m_lTurnConvWaitTime[0] = GetCurrentTime();

			
			if (st_basic_info.nPcbType == RDIMM || st_basic_info.nPcbType == UDIMM_10)
			{
				if(m_nRdimmJobState == CTL_COMPLETE)
				{	
					m_nRdimmJobState = CTL_NONE;
					m_nRunStep = 0;
				}
				else
				{
					if (st_sync_info.TurnConvJobReady[CONVEYOR] == CTL_COMPLETE)
					{
						m_nRunStep = 14000;
					}
				}
			}
			else
			{
				m_nRunStep = 0;
			}

			for (int i = 0; i<PICKCNT; i++)
			{
				clsRunRobot.m_npTemp_Picker_YesNo[i] =
				FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[i],IO_OFF);
				FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[i],IO_OFF);
			}
			clsRunRobot.OnSetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);

			st_sync_info.TurnConvJobReady[ROBOT] = CTL_READY;
		}
		else if(nRet == BD_ERROR)
		{			
			m_nRunStep = 13200;
			CTL_Lib.Alarm_Error_Occurrence(910, dWARNING, st_alarm_info.strCode);
		}			
		break;
	}
	/*return RET_PROCEED;*/
}


//kwlee 2017.0216
// void CRunRobot::OnSetRejectCylUpDn(int nMode, int nUpDn)
// {
// 	if (nMode > 1) return;
// 
// 	m_bRejectCylUpDnFlag[nMode]			= false;
// 	m_dwCylWaitTime[nMode][0]	= GetCurrentTime();
// 
// 	if (nUpDn == IO_ON)
// 	{
// 		FAS_IO.set_out_bit(st_io_info.o_LabelRejectCylinderUp, nUpDn); //S0105 Up
// 		FAS_IO.set_out_bit(st_io_info.o_LabelRejectCylinderDn, !nUpDn);
// 
// 	}
// 	else
// 	{
// 		FAS_IO.set_out_bit(st_io_info.o_LabelRejectCylinderDn, !nUpDn); //S0106 Dw
// 		FAS_IO.set_out_bit(st_io_info.o_LabelRejectCylinderUp, nUpDn);
// 	}
// 
// }
// int CRunRobot::OnGetRejectCylUpDn(int nMode, int nUpDn)
// {
// 	int nWaitTime = WAIT_PICKER_UP_DN;
// 
// 	if (nUpDn == IO_OFF)
// 	{
// 		if (m_bRejectCylUpDnFlag[nMode] == false &&
// 			FAS_IO.get_in_bit(st_io_info.i_LabelRejectCylinderDnCheck,	IO_OFF)	== IO_OFF )
// 		{
// 			m_bRejectCylUpDnFlag[nMode]		= true;
// 			m_dwCylWaitTime[nMode][0]	= GetCurrentTime();
// 		}
// 		else if (m_bRejectCylUpDnFlag[nMode] == true &&
// 			FAS_IO.get_in_bit(st_io_info.i_LabelRejectCylinderDnCheck,	IO_OFF)	== IO_OFF )
// 		{
// 			m_dwCylWaitTime[nMode][1] = GetCurrentTime();
// 			m_dwCylWaitTime[nMode][2] = m_dwCylWaitTime[nMode][1] - m_dwCylWaitTime[nMode][0];
// 
// 			if (m_dwCylWaitTime[nMode][2] <= 0)
// 			{
// 				m_dwCylWaitTime[nMode][0] = GetCurrentTime();
// 				return RET_PROCEED;
// 			}
// 
// 			if (m_dwCylWaitTime[nMode][2] > (DWORD)st_wait_info.nOnWaitTime[nWaitTime])
// 			{
// 				return RET_GOOD;
// 			}
// 		}
// 		else
// 		{
// 			m_dwCylWaitTime[nMode][1] = GetCurrentTime();
// 			m_dwCylWaitTime[nMode][2] = m_dwCylWaitTime[nMode][1] - m_dwCylWaitTime[nMode][0];
// 
// 			if (m_dwCylWaitTime[nMode][2] <= 0)
// 			{
// 				m_dwCylWaitTime[nMode][0] = GetCurrentTime();
// 				return RET_PROCEED;
// 			}
// 
// 			if (m_dwCylWaitTime[nMode][2] > (DWORD)st_wait_info.nLimitWaitTime[nWaitTime])
// 			{
// 
// 				m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_LabelRejectCylinderDnCheck, nUpDn); 
// 				//CTL_Lib.Alarm_Error_Occurrence(304, dWARNING, strAlarm);
// 				return RET_ERROR;
// 			}
// 		}
// 	}
// 	else
// 	{
// 		if (m_bRejectCylUpDnFlag[nMode] == false &&
// 			FAS_IO.get_in_bit(st_io_info.i_LabelRejectCylinderUpCheck,	IO_ON)	== IO_ON )
// 		{
// 			m_bRejectCylUpDnFlag[nMode]			= true;
// 			m_dwCylWaitTime[nMode][0]	= GetCurrentTime();
// 		}
// 		else if (m_bRejectCylUpDnFlag[nMode] == true &&
// 			FAS_IO.get_in_bit(st_io_info.i_LabelRejectCylinderUpCheck,	IO_ON)	== IO_ON )
// 		{
// 			m_dwCylWaitTime[nMode][1]	= GetCurrentTime();
// 			m_dwCylWaitTime[nMode][2]	= m_dwCylWaitTime[nMode][1] - m_dwCylWaitTime[nMode][0];
// 
// 			if (m_dwCylWaitTime[nMode][2] <= 0)
// 			{
// 				m_dwCylWaitTime[nMode][0]	= GetCurrentTime();
// 				return RET_PROCEED;
// 			}
// 
// 			if(m_dwCylWaitTime[nMode][2] > (DWORD)st_wait_info.nOnWaitTime[nWaitTime])
// 			{
// 				return RET_GOOD;
// 			}
// 		}
// 		else
// 		{
// 			m_dwCylWaitTime[nMode][1]	= GetCurrentTime();
// 			m_dwCylWaitTime[nMode][2]	= m_dwCylWaitTime[nMode][1] - m_dwCylWaitTime[nMode][0];
// 
// 			if (m_dwCylWaitTime[nMode][2] <= 0)
// 			{
// 				m_dwCylWaitTime[nMode][0]	= GetCurrentTime();
// 				return RET_PROCEED;
// 			}
// 			if (m_dwCylWaitTime[nMode][2] > (DWORD)st_wait_info.nLimitWaitTime[nWaitTime])
// 			{
// 				m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_LabelRejectCylinderUpCheck, nUpDn); 
// 				//CTL_Lib.Alarm_Error_Occurrence(305, dWARNING, strAlarm);
// 				return RET_ERROR;
// 			}
// 		}
// 	}
// 	return RET_PROCEED;
// 
// }

void CRunRobot::OnSetRejectCylUpDn(int nMode, int nUpDn)
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

int CRunRobot::OnGetRejectCylUpDn(int nMode, int nUpDn)
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