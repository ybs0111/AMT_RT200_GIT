#include "StdAfx.h"
#include "RunRobot.h"
#include "variable.h"
//#include "CV_test.h"
#include "FastechPublic_IO.h"
#include "ComizoaPublic.h"
#include "CtlBd_Library.h"
#include "Ctlbd_Variable.h"
#include "AlgMemory.h"
#include "PublicFunction.h"

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
	m_nCntBcrNum = 0;
	
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
	m_nPrintOutCheck = FALSE;
	m_nBarcodeReadCheck[0] = FALSE;
	m_nBarcodeReadCheck[1] = FALSE;
	m_nSodimmSecondPlace = FALSE;
	m_nBcrRetryCnt[0] = 0;
	m_nBcrRetryCnt[1] = 0;
	
	//kwlee 2017.0315
	m_nVisionStep = 0; 
	m_nVisionRetry = 0;
	

	m_nPrintOutPutCnt = 39; //kwlee 2017.0324 Test
	for (int i = 0; i< 2; i++)
	{
		for (int j = 0; j< 40; j++)
		{
			st_Buffer_info.nBufferData[i][j][BIN]  = 2;
			st_Buffer_info.nBufferData[i][j][EXIST] = 1;
		}
	}
	st_sync_info.nBcrReq = CTL_CLEAR;
	st_sync_info.nLabelCheckReq = CTL_CLEAR;
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
		OnLabelSensorCheck(); //kwlee 2017.03075
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
				//kwlee 2017.0317
				for (int i =0; i<PICKCNT; i++ )
				{
					m_npTemp_Picker_YesNo[i] = CTL_NO;
				}

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
					m_npTemp_Picker_YesNo[i] = CTL_YES;
				}
				else
				{
					m_npTemp_Picker_YesNo[PICKCNT + 3 - i] = CTL_YES;
				}
			}
			OnSetPickerUpDn(0, PICKER_DN, m_npTemp_Picker_YesNo);
			m_nInitStep = 6800;
			break;

		case 6800:
			nRet_1 = OnGetPickerUpDn(0, PICKER_DN,m_npTemp_Picker_YesNo);
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
			if( m_dwTimeCheck[2] >  300 )
			{

				for (int i =0; i<PICKCNT; i++ )
				{
					if (i < 5)
					{
						FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[i],IO_OFF);
						m_npTemp_Picker_YesNo[i] = CTL_YES;
					}
					else
					{
						FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[PICKCNT + 3 - i],IO_OFF);
						m_npTemp_Picker_YesNo[PICKCNT + 3 - i] = CTL_YES;
					}
				}				
				OnSetPickerUpDn(0, PICKER_UP, m_npTemp_Picker_YesNo);
				m_nInitStep = 6930;
			}
			break;

		case 6930:

			nRet_1 = OnGetPickerUpDn(0, PICKER_UP, m_npTemp_Picker_YesNo);

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
		m_npTemp_Picker_YesNo[i] = CTL_NO;	
	}

	for (int i =0; i < nPickCnt; i++ )
	{
		if(m_bSecondPos == false)
		{
			if(st_basic_info.nPcbType == RDIMM)
			{
				if (i < 3)
				{
					m_npTemp_Picker_YesNo[i] = CTL_YES;
				}
			}
			else /*if (st_basic_info.nPcbType == UDIMM_10)*/
			{
				if (i < 5)
				{
					m_npTemp_Picker_YesNo[i] = CTL_YES;
				}
			}
		}
		else
		{
			if(st_basic_info.nPcbType == RDIMM)
			{ 
				if(i > 2) 
 				{
					m_npTemp_Picker_YesNo[PICKCNT - i - 1] = CTL_YES;	
				}
			}
			else if (st_basic_info.nPcbType == UDIMM_10)
			{ 
				if(i > 4) 
				{
// 					if (st_basic_info.nPickerSelect[PICKCNT - 6 + i] == CTL_YES)
// 					{
					m_npTemp_Picker_YesNo[PICKCNT + 3 - i] = CTL_YES;	
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
					m_npTemp_Picker_YesNo[PICKCNT + 3 - i] = CTL_YES;	
					//}
				}
			}
		}	
	}
	//clsRunRobot.OnSetPickerUpDn(0, PICKER_DN, clsRunRobot.m_npTemp_Picker_YesNo);
}
// void CRunRobot::OnBufferDataTransfer()
// {
// // 	if (m_nJobNextPos == 1)
// // 	{
// 	int nCnt = 0;
// 	for (int i =0; i<2; i++)
// 	{
// 		for (int j =0; j<MAX_BUFFER; j++)
// 		{
// 			nCnt++;
// 			if (st_Buffer_info.strBufferSerial[i][j] != _T(""))
// 			{
// 				if (st_Buffer_info.strBufferSerial[i][nCnt - 1] == _T(""))
// 				{
// 					st_Buffer_info.strBufferSerial[i][nCnt - 1] =  st_Buffer_info.strBufferSerial[i][j];
// 					break;
// 				}
// 			}
// 		}
// 	}
// // 	}
// // 	else
// // 	{
// // 		for (int i =0; i<2; i++)
// // 		{
// // 			for (int j =0; j<MAX_PICKER; j++)
// // 			{
// // 				for (int k =0; j<MAX_INFO; k++)
// // 				{
// // 					if (st_Buffer_info[PICK].strBufferSerial[i][MAX_PICKER + j] != _T(""))
// // 					{
// // 						st_Buffer_info[TEMP].nBufferData[i][j][k] =  st_Buffer_info[PICK].nBufferData[i][MAX_PICKER + j][k];
// // 						st_Buffer_info[TEMP].strBufferSerial[i][j] =  st_Buffer_info[PICK].strBufferSerial[i][MAX_PICKER + j];
// // 
// // 						st_Buffer_info[PICK].nBufferData[i][j][k] = st_Buffer_info[TEMP].nBufferData[i][j][k];
// // 						st_Buffer_info[PICK].strBufferSerial[i][j] = st_Buffer_info[TEMP].strBufferSerial[i][j];
// // 
// // 						st_Buffer_info[TEMP].nBufferData[i][j][k] = 0;
// // 						st_Buffer_info[TEMP].strBufferSerial[i][j] = _T("");
// // 						//m_nEmptyCntBufferCnt++;
// // 					}
// // 				
// // 				}
// // 			}
// // 		}
// // 	}
// }

void CRunRobot::OnResetRejectData()
{
	int nTemp;
	//for (int i = 0; i < m_nPickCnt; i++)
	for (int i = 0; i < MAX_PICKER; i++)
	{
		if (i < 5 && st_Picker_info.nPickerData[0][i][FAILPICK] == YES)
		{
			st_Picker_info.strPickerSerial[0][i] = _T("");
			st_Picker_info.nPickerData[0][i][BIN] = FAIL;
			st_Picker_info.nPickerData[0][i][EXIST] = NO;
		}
		else if (i > 4 && st_Picker_info.nPickerData[0][m_nPickCnt - i][FAILPICK] == YES)
		{
			nTemp = st_Picker_info.nPickerData[1][m_nPickCnt - i][Y_FAIL_POS];
			st_Picker_info.strPickerSerial[1][nTemp] = _T("");
			st_Picker_info.nPickerData[1][nTemp][BIN] = FAIL;
			st_Picker_info.nPickerData[1][nTemp][EXIST] = NO;
		}
		else
		{

		}
	}
}
void CRunRobot::OnDataExchange(int nMode, int nPickPlace)
{
	//kwlee 2017.02
	int nTemp = 0;
	for (int j = 0; j< m_nPickCnt; j++)
	{
		if (nMode == REJECT_MODE)
		{
			if (nPickPlace == PICK)
			{
				if (st_Buffer_info.nBufferData[1][39][BIN] == GOOD && st_Buffer_info.nBufferData[1][39][EXIST] == YES)
				{
					if (j < 5 && st_Picker_info.nPickerData[0][j][FAILPICK] == YES)
					{
// 							st_Picker_info.nPickerData[0][j][k] = st_Buffer_info.nBufferData[1][39][k];
// 							st_Picker_info.strPickerSerial[0][j] = st_Buffer_info.strBufferSerial[1][39];
						
// 							st_Picker_info.nPickerData[0][j][k] = st_Buffer_info.nBufferData[1][39][k];
// 							st_Picker_info.strPickerSerial[0][j] = st_Buffer_info.strBufferSerial[1][39];
						//kwlee 2017.0225
						st_Picker_info.nPickerData[0][j][BIN] = st_Buffer_info.nBufferData[1][39][BIN];
						st_Picker_info.nPickerData[0][j][EXIST] = st_Buffer_info.nBufferData[1][39][EXIST];
						st_Picker_info.nPickerData[0][j][Y_FAIL_POS] = st_Buffer_info.nBufferData[1][39][Y_FAIL_POS];
						st_Picker_info.nPickerData[0][j][FAILPICK] = st_Buffer_info.nBufferData[1][39][FAILPICK];

						st_Picker_info.strPickerSerial[0][j] = st_Buffer_info.strBufferSerial[1][39];

// 						if( k == (MAX_INFO - 1))
// 						{
						st_Picker_info.nPickerData[0][j][FAILPICK] = NO;

						st_Buffer_info.nBufferData[1][39][BIN] = NONE;
						st_Buffer_info.nBufferData[1][39][FAILPICK] = NO;
						st_Buffer_info.nBufferData[1][39][EXIST] = NO;
						st_Buffer_info.strBufferSerial[1][39] = _T("");
						break;
						//
							
					}
					else if (j> 4 && st_Picker_info.nPickerData[1][m_nPickCnt - j][FAILPICK] == YES)
					{
// 						st_Picker_info.nPickerData[1][m_nPickCnt - j][k] = st_Buffer_info.nBufferData[1][39][k];
// 						st_Picker_info.strPickerSerial[1][m_nPickCnt - j] = st_Buffer_info.strBufferSerial[1][39];
							
						//st_Picker_info.nPickerData[1][m_nPickCnt - j][k] = st_Buffer_info.nBufferData[1][39][k];
						st_Picker_info.nPickerData[1][m_nPickCnt - j][BIN] = st_Buffer_info.nBufferData[1][39][BIN];
						st_Picker_info.nPickerData[1][m_nPickCnt - j][EXIST] = st_Buffer_info.nBufferData[1][39][EXIST];
						st_Picker_info.nPickerData[1][m_nPickCnt - j][Y_FAIL_POS] = st_Buffer_info.nBufferData[1][39][Y_FAIL_POS];
						st_Picker_info.nPickerData[1][m_nPickCnt - j][FAILPICK] = st_Buffer_info.nBufferData[1][39][FAILPICK];

						st_Picker_info.strPickerSerial[1][m_nPickCnt - j] = st_Buffer_info.strBufferSerial[1][39];

// 						if( k == (MAX_INFO - 1))
// 						{
						st_Picker_info.nPickerData[1][m_nPickCnt - j][FAILPICK] = NO;

						st_Buffer_info.nBufferData[1][39][BIN] = NONE;
						st_Buffer_info.nBufferData[1][39][FAILPICK] = NO;
						st_Buffer_info.nBufferData[1][39][EXIST] = NO;
						st_Buffer_info.strBufferSerial[1][39] = _T("");
						break;
						//
							
					}
						
				}
				else if (st_Buffer_info.nBufferData[0][39][BIN] == GOOD && st_Buffer_info.nBufferData[0][39][EXIST] == YES)
				{
					if (j < 5 && st_Picker_info.nPickerData[0][j][FAILPICK] == YES)
					{
					//	st_Picker_info.nPickerData[0][j][k] = st_Buffer_info.nBufferData[0][39][k];
						//st_Picker_info.strPickerSerial[0][j] = st_Buffer_info.strBufferSerial[0][39];
						//kwlee 2017.0225
						st_Picker_info.nPickerData[0][j][BIN] = st_Buffer_info.nBufferData[0][39][BIN];
						st_Picker_info.nPickerData[0][j][EXIST] = st_Buffer_info.nBufferData[0][39][EXIST];
						st_Picker_info.nPickerData[0][j][Y_FAIL_POS] = st_Buffer_info.nBufferData[0][39][Y_FAIL_POS];
						st_Picker_info.nPickerData[0][j][FAILPICK] = st_Buffer_info.nBufferData[0][39][FAILPICK];
						st_Picker_info.strPickerSerial[0][j] = st_Buffer_info.strBufferSerial[0][39];

// 						if( k == (MAX_INFO - 1))
// 						{
						st_Picker_info.nPickerData[0][j][FAILPICK] = NO;

						st_Buffer_info.nBufferData[0][39][FAILPICK] = NO;
						st_Buffer_info.nBufferData[0][39][EXIST] = NO;
						st_Buffer_info.strBufferSerial[0][39] = _T("");
						break;
					//}
							
					}
					else if (j> 4 && st_Picker_info.nPickerData[1][m_nPickCnt - j][FAILPICK] == YES)
					{
						//st_Picker_info.nPickerData[1][m_nPickCnt - j][k] = st_Buffer_info.nBufferData[0][39][k];
						//st_Picker_info.strPickerSerial[1][m_nPickCnt - j] = st_Buffer_info.strBufferSerial[0][39];

						st_Picker_info.nPickerData[1][m_nPickCnt - j][BIN] = st_Buffer_info.nBufferData[0][39][BIN];
						st_Picker_info.nPickerData[1][m_nPickCnt - j][EXIST] = st_Buffer_info.nBufferData[0][39][EXIST];
						st_Picker_info.nPickerData[1][m_nPickCnt - j][Y_FAIL_POS] = st_Buffer_info.nBufferData[0][39][Y_FAIL_POS];
						st_Picker_info.nPickerData[1][m_nPickCnt - j][FAILPICK] = st_Buffer_info.nBufferData[0][39][FAILPICK];
						st_Picker_info.strPickerSerial[1][m_nPickCnt - j] = st_Buffer_info.strBufferSerial[0][39];
// 						if( k == (MAX_INFO - 1))
// 						{
						st_Picker_info.nPickerData[1][m_nPickCnt - j][FAILPICK] = NO;

						st_Buffer_info.nBufferData[0][39][FAILPICK] = NO;
						st_Buffer_info.nBufferData[0][39][EXIST] = NO;
						st_Buffer_info.strBufferSerial[0][39] = _T("");
						break;
					//	}
							
					}
					//break;
				}
			}
			else
			{

			}
		}
		else //nMode == NOMAL_MODE
		{
			for (int k =0; k<MAX_INFO; k++)
			{
				if (nPickPlace == PICK)
				{
					if (j < 5)
					{
						if (st_Buffer_info.strBufferSerial[0][39 - j] !=_T(""))
						{
							st_Picker_info.nPickerData[0][j][k] = st_Buffer_info.nBufferData[0][39 - j][k];
							st_Picker_info.strPickerSerial[0][j] = st_Buffer_info.strBufferSerial[0][39 - j];
							st_Picker_info.nPickerFailCnt        = st_Buffer_info.nBufferFailCnt;

							if( k == (MAX_INFO - 1))
							{
								if (st_Picker_info.nPickerData[0][j][k] == st_Buffer_info.nBufferData[0][39 - j][k] && 
									st_Picker_info.strPickerSerial[0][j] == st_Buffer_info.strBufferSerial[0][39 - j] && 
									m_nLabelFailCheck == FALSE)
								{
									st_Buffer_info.nBufferData[0][39 - j][k] = -1;
									st_Buffer_info.strBufferSerial[0][39 - j] = _T("");
									//st_Buffer_info.nBufferFailCnt = 0;	
								}	
							}
						}
					}
					else //j > 4
					{
						if (st_Buffer_info.strBufferSerial[1][39 - (m_nPickCnt - j)] !=_T(""))
						{
							st_Picker_info.nPickerData[1][m_nPickCnt - j][k]  = st_Buffer_info.nBufferData[1][39 - (m_nPickCnt - j)][k];
							st_Picker_info.strPickerSerial[1][m_nPickCnt - j] = st_Buffer_info.strBufferSerial[1][39 - (m_nPickCnt - j)];
							st_Picker_info.nPickerFailCnt					  = st_Buffer_info.nBufferFailCnt;

							if( k == (MAX_INFO - 1))
							{
								if (st_Picker_info.nPickerData[1][m_nPickCnt - j][k] == st_Buffer_info.nBufferData[1][39 - (m_nPickCnt - j)][k]  &&
									st_Picker_info.strPickerSerial[1][m_nPickCnt - j] == st_Buffer_info.strBufferSerial[1][39 - (m_nPickCnt - j)] && 
									m_nLabelFailCheck == FALSE)
								{
									st_Buffer_info.nBufferData[1][39 - (m_nPickCnt - j)][k] = -1;
									st_Buffer_info.strBufferSerial[1][39 - (m_nPickCnt - j)] = _T("");
									//st_Buffer_info.nBufferFailCnt = 0;	
								}
							}
						}
					}
				}
				else if (nPickPlace == PLACE)
				{	
					// kwlee 2017.0220
					if (st_basic_info.nPcbType == SODIMM)
					{
						if (st_sync_info.nConvTurnCheck == FALSE)
						{
							//if (i == 0)
							if (j < 5)
							{
								if (st_Picker_info.strPickerSerial[0][j] != _T(""))
								{ 
									st_Pcb_info.strPcbSerial[0][j] =  st_Picker_info.strPickerSerial[0][j];
									st_Picker_info.strPickerSerial[0][j] = _T("");	
									st_Picker_info.nPickerData[0][j][k] =  -1;	
								}

							}
							else
							{
								if (m_bSecondPos == true)
								{
									//kwlee 2017.0221
									nTemp = MAX_PICKER - (m_nPickCnt - j);
									if (st_Picker_info.strPickerSerial[1][nTemp] != _T(""))
									{
										st_Pcb_info.strPcbSerial[0][j] =  st_Picker_info.strPickerSerial[1][nTemp];
										st_Picker_info.strPickerSerial[1][nTemp] = _T("");
										st_Picker_info.nPickerData[1][nTemp][k] =  -1;
									}
								}
							} 
						}
						else //st_sync_info.nConvTurnCheck == TRUE
						{
							if (j < 5)
							{
								if (st_Picker_info.strPickerSerial[0][j] != _T(""))
								{
									st_Pcb_info.strPcbSerial[1][j] =  st_Picker_info.strPickerSerial[0][j];

									st_Picker_info.strPickerSerial[0][j] = _T("");	
									st_Picker_info.nPickerData[0][j][k] =  -1;
								}
							}
							else
							{
								if (m_bSecondPos == true)
								{
									nTemp = MAX_PICKER - (m_nPickCnt - j);
									if (st_Picker_info.strPickerSerial[1][nTemp] != _T(""))
									{
										st_Pcb_info.strPcbSerial[1][j] =  st_Picker_info.strPickerSerial[1][nTemp];
										st_Picker_info.strPickerSerial[1][nTemp] = _T("");
										st_Picker_info.nPickerData[1][nTemp][k] =  -1;
									}
								}
							}
						}
					}
				}
				else
				{

				}
			}
		}//for = k
	}//for = j
}

//kwlee 2017.0204
int CRunRobot::OnGetVaccumgmCheck(int OnOff,int nPickCnt)
{
	int i,nFailCnt = 0;
	int nRet;
	if (OnOff == IO_ON)
	{
		for (i = 0; i < nPickCnt; i++)
		{
			if (i <5)
			{
				//kwlee 2017.0221
				if (st_Picker_info.nPickerData[0][i][BIN] == GOOD && st_Picker_info.nPickerData[0][i][EXIST] == YES && 
					st_Picker_info.strPickerSerial[0][i]!= _T("") && st_Picker_info.nPickerData[0][i][FAILPICK] == NO)
				{
					nRet = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[i],IO_ON);	
					if (nRet == IO_OFF && st_basic_info.nModeDevice == WITH_DVC)
					{
						//kwlee 2017.0221
						st_Picker_info.nPickerData[0][i][BIN] = FAIL;
						st_Picker_info.nPickerData[0][i][EXIST] = NO;
					}
				}
				else if (st_Picker_info.nPickerData[0][i][BIN] == GOOD && st_Picker_info.nPickerData[0][i][EXIST] == YES && 
					st_Picker_info.strPickerSerial[0][i]!= _T("") && st_Picker_info.nPickerData[0][i][FAILPICK] == YES)
				{
					nRet = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[i],IO_ON);
					if (nRet == IO_ON)
					{
						//kwlee 2017.0221
						st_Picker_info.nPickerData[0][i][FAILPICK] = NO;
						st_Picker_info.nPickerData[0][i][EXIST] = YES;
						st_Picker_info.nPickerData[0][i][BIN] = GOOD;
					}
				}
				//kwlee 2017.0221
				else
				{
					nRet = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[i],IO_ON);	
					if (nRet == IO_OFF && st_basic_info.nModeDevice == WITH_DVC)
					{
						//kwlee 2017.0221
						st_Picker_info.nPickerData[0][i][BIN] = FAIL;
						st_Picker_info.nPickerData[0][i][EXIST] = NO;
					}
				}				
			}
			else if(i > 4)
			{
				//kwlee 2017.0221
// 				if (st_Picker_info.nPickerData[1][MAX_PICKER - i][BIN] == GOOD && st_Picker_info.nPickerData[1][MAX_PICKER - i][EXIST] == YES && 
// 					st_Picker_info.strPickerSerial[1][MAX_PICKER - i] !=_T("") && st_Picker_info.nPickerData[1][MAX_PICKER - i][FAILPICK] == NO)
				//kwlee 2017.0317
				if (st_Picker_info.nPickerData[1][nPickCnt - i][BIN] == GOOD && st_Picker_info.nPickerData[1][nPickCnt - i][EXIST] == YES && 
					st_Picker_info.strPickerSerial[1][nPickCnt - i] !=_T("") && st_Picker_info.nPickerData[1][nPickCnt - i][FAILPICK] == NO)
				{
					nRet = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[PICKCNT + 3 - i],IO_ON);
					if (nRet == IO_OFF && st_basic_info.nModeDevice == WITH_DVC)
					{
						//kwlee 2017.0221
// 						st_Picker_info.nPickerData[1][MAX_PICKER - i][BIN] = FAIL;
// 						st_Picker_info.nPickerData[1][MAX_PICKER - i][EXIST] = NO;
						//kwlee 2017.0317
						st_Picker_info.nPickerData[1][nPickCnt - i][BIN] = FAIL;
						st_Picker_info.nPickerData[1][nPickCnt - i][EXIST] = NO;
						//m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_HeadVaccumCheck[PICKCNT + 3 - i], OnOff);
						//return RET_ERROR;
					}
				}
// 				else if (st_Picker_info.nPickerData[1][MAX_PICKER - i][BIN] == GOOD && st_Picker_info.nPickerData[1][MAX_PICKER - i][EXIST] == YES && 
// 					st_Picker_info.strPickerSerial[1][MAX_PICKER - i] !=_T("") && st_Picker_info.nPickerData[1][MAX_PICKER - i][FAILPICK] == YES)
				//kwlee 2017.0317
				else if (st_Picker_info.nPickerData[1][nPickCnt - i][BIN] == GOOD && st_Picker_info.nPickerData[1][nPickCnt - i][EXIST] == YES && 
					st_Picker_info.strPickerSerial[1][nPickCnt - i] !=_T("") && st_Picker_info.nPickerData[1][nPickCnt - i][FAILPICK] == YES)
				{
					//FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[PICKCNT + 3 - i],IO_OFF);
					nRet = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[PICKCNT + 3 - i],IO_ON);
					if (nRet == IO_ON)
					{
						//kwlee 2017.0221
// 						st_Picker_info.nPickerData[1][MAX_PICKER - i][FAILPICK] = NO;
// 						st_Picker_info.nPickerData[1][MAX_PICKER - i][EXIST] = YES;
// 						st_Picker_info.nPickerData[1][MAX_PICKER - i][BIN] = GOOD;
						//kwlee 2017.0317
						st_Picker_info.nPickerData[1][nPickCnt - i][FAILPICK] = NO;
						st_Picker_info.nPickerData[1][nPickCnt - i][EXIST] = YES;
						st_Picker_info.nPickerData[1][nPickCnt - i][BIN] = GOOD;
					}
				}
				//kwlee 2017.0221
				else
				{
					nRet = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[PICKCNT + 3 - i],IO_ON);
					if (nRet == IO_OFF && st_basic_info.nModeDevice == WITH_DVC)
					{
						//kwlee 2017.0221
// 						st_Picker_info.nPickerData[1][MAX_PICKER - i][BIN] = FAIL;
// 						st_Picker_info.nPickerData[1][MAX_PICKER - i][EXIST] = NO;
						//kwlee 2017.0317
						st_Picker_info.nPickerData[1][nPickCnt - i][BIN] = FAIL;
						st_Picker_info.nPickerData[1][nPickCnt - i][EXIST] = NO;
						//m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_HeadVaccumCheck[PICKCNT + 3 - i], OnOff);
						//return RET_ERROR;
					}
				}
			}
		}
		
		//kwlee 2017.0220
		for (int j =0; j < m_nPickCnt; j++)
		{
			//kwlee 2017.0221
			if (j < 5)
			{
// 					if (st_Picker_info.nPickerData[0][j][BIN] == FAIL && st_Picker_info.nPickerData[0][j][EXIST] == YES ||
// 						st_Picker_info.nPickerData[0][j][EXIST] == NO)
				//kwlee 2017.0225
				if (st_Picker_info.nPickerData[0][j][BIN] == FAIL && st_Picker_info.nPickerData[0][j][EXIST] == YES ||
					st_Picker_info.nPickerData[0][j][BIN] == -1 && st_Picker_info.nPickerData[0][j][EXIST] == YES ||
					st_Picker_info.nPickerData[0][j][EXIST] == NO)
				{
					//st_Picker_info.nPickerData[i][j][X_FAIL_POS] = i;
					st_Picker_info.nPickerData[0][j][BIN] = FAIL;
					st_Picker_info.nPickerData[0][j][Y_FAIL_POS] = j;
					st_Picker_info.nPickerData[0][j][FAILPICK] = YES;
					nFailCnt++;
				}
			}
			else if(j > 4)
			{
				//kwlee 2017.0225
				if(st_Picker_info.nPickerData[1][m_nPickCnt - j][BIN] == FAIL && st_Picker_info.nPickerData[1][m_nPickCnt - j][EXIST] == YES ||
				st_Picker_info.nPickerData[1][m_nPickCnt - j][BIN] == -1 && st_Picker_info.nPickerData[1][m_nPickCnt - j][EXIST] == YES ||
				st_Picker_info.nPickerData[1][m_nPickCnt - j][EXIST] == NO)
				{
					st_Picker_info.nPickerData[1][m_nPickCnt - j][BIN] = FAIL;
					st_Picker_info.nPickerData[1][m_nPickCnt - j][Y_FAIL_POS] = m_nPickCnt - j;
					st_Picker_info.nPickerData[1][m_nPickCnt - j][FAILPICK] = YES;
					nFailCnt++;
				}
			}
		}
				
		if (nFailCnt > 0)
		{
			st_Picker_info.nPickerFailCnt = nFailCnt;
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
	int nTemp = 0;

	
	for (i =0; i<PICKCNT; i++ )
	{
		m_npTemp_Picker_YesNo[i] = CTL_NO;
	}
	
	i =0; 
	for (i =0; i<nPickCnt; i++)
	{
		if (nMode == 1)
		{		
			if (i < 5 && ( st_Picker_info.nPickerData[0][i][BIN] == FAIL /*|| st_Picker_info.nPickerData[0][i][EXIST] == NO*/ ) )
			{
				m_npTemp_Picker_YesNo[st_Picker_info.nPickerData[0][i][Y_FAIL_POS]] = CTL_YES;
			}
			else if ( i > 4 && ( st_Picker_info.nPickerData[1][nPickCnt - i][BIN] == FAIL /*|| st_Picker_info.nPickerData[1][nPickCnt - i][EXIST] == NO*/))
			{
				//kwlee 2017.0221
				nTemp = MAX_PICKER + st_Picker_info.nPickerData[1][nPickCnt  - i][Y_FAIL_POS]; 
				m_npTemp_Picker_YesNo[nTemp + 1] = CTL_YES;
			}
			else
			{

			}
		}
		else
		{

			if (i <5)
			{
				//clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_YES;
				//kwlee 2017.0204
				//if (st_Buffer_info.nBufferData[0][i][BIN] == GOOD)
				if (st_Buffer_info.nBufferData[0][39 - i][BIN] == GOOD && st_Buffer_info.nBufferData[0][39 - i][EXIST] == YES)
				{
					m_npTemp_Picker_YesNo[i] = CTL_YES;
				}
				else
				{
					m_npTemp_Picker_YesNo[i] = CTL_NO;
				}
				
			}
			else if (i > 4)
			{
				//clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT + 3 - i] = CTL_YES;	
				//kwlee 2017.0204
				
				if (st_Buffer_info.nBufferData[1][39 - (m_nPickCnt - i)][BIN] == GOOD && st_Buffer_info.nBufferData[1][39 - (m_nPickCnt - i)][EXIST] == YES)
				{
					m_npTemp_Picker_YesNo[PICKCNT + 3 - i] = CTL_YES;	
				}
				else
				{
					m_npTemp_Picker_YesNo[PICKCNT + 3 - i] = CTL_NO;	
				}
			}
			else
			{

			}
				//}
			//}
		}
	}
}
void CRunRobot::OnBlowSet(int nMode,int nPickCnt,int OnOff)
{

	int nTemp = 0;

	if (OnOff == IO_ON)
	{
		for (int i =0; i<nPickCnt; i++ )
		{	
			if (nMode == 1)
			{
				if (i <5 && st_Picker_info.nPickerData[0][i][FAILPICK] == YES)	
				{
// 					if (st_Picker_info.nPickerData[0][i][BIN] == FAIL && st_Picker_info.nPickerData[0][i][EXIST] == NO)
// 					{
					FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[st_Picker_info.nPickerData[0][i][Y_FAIL_POS]],IO_ON);
				//	}
					//break;
				}
				//else if (i > 4 && st_Picker_info.nPickerData[1][MAX_PICKER - (nPickCnt - i)][X_FAIL_POS] == RIGHT)
				//kwlee 2017.0221
				else if(i > 4 && st_Picker_info.nPickerData[1][nPickCnt - i][FAILPICK] == YES)
				{
					//kwlee 2017.0221
// 					if (st_Picker_info.nPickerData[1][MAX_PICKER - (nPickCnt - i)][BIN] == FAIL && st_Picker_info.nPickerData[1][MAX_PICKER - (nPickCnt - i)][EXIST] == NO)
// 					{
					nTemp = MAX_PICKER + st_Picker_info.nPickerData[1][MAX_PICKER - (nPickCnt - i)][Y_FAIL_POS];
					FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[nTemp + 1],IO_ON);
					//}
						
				}
				else
				{

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
				//kwlee 2017.0221
				if (i <5 && st_Picker_info.nPickerData[0][i][FAILPICK] == YES)
				{
					FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[st_Picker_info.nPickerData[0][i][Y_FAIL_POS]],IO_OFF);
				
				}
				else if( i > 4 && st_Picker_info.nPickerData[1][nPickCnt - i][FAILPICK] == YES)
				{

					nTemp = MAX_PICKER + st_Picker_info.nPickerData[1][MAX_PICKER - (nPickCnt - i)][Y_FAIL_POS];
					FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[nTemp + 1],IO_OFF);
			
				}
				else
				{

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
							FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[i],IO_OFF);
						}
					}
					else //Sodimm,Udimm_9
					{
						if (i < 5)
						{
							FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[i],IO_OFF);
						}
					}
				}
				else
				{
					if(st_basic_info.nPcbType == RDIMM)
					{
					
						FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[PICKCNT - i - 1],IO_OFF);
						
					}
					else if (st_basic_info.nPcbType == UDIMM_10)
					{

						FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[PICKCNT + 3 - i],IO_OFF);
						
					}
					else
					{
						FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[PICKCNT + 3 - i],IO_OFF);	
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
			if (st_Buffer_info.nBufferData[i][39 - j][BIN] == GOOD && st_Buffer_info.strBufferSerial[i][39 - j] !=_T(""))
			{
				st_Buffer_info.nBufferData[i][39  - j][EXIST] = YES;
			}
		}
	}
}
int CRunRobot::OnBufferDataTransfer(int nCnt)
{
	//bool bDataShift;

	//bDataShift  = false;
	for (int i =0; i<2; i++)
	{	
		for (int j = 0; j<nCnt; j++)
		{	
			if (st_Buffer_info.strBufferSerial[i][j] != _T(""))
			{
				if ( j > 0 && j < 39 )
				{
					if (st_Buffer_info.strBufferSerial[i][j - 1] == _T(""))
					{
						st_Buffer_info.strBufferSerial[i][j - 1] = st_Buffer_info.strBufferSerial[i][j];
						st_Buffer_info.strBufferSerial[i][j] = _T(""); 
						//bDataShift = true;
					}
				}
			}
		}
	}
	return BD_GOOD;
}

//kwlee 2017.0202
int CRunRobot::OnPrinterFeeder(int nCnt, int nFailCheck)
{
	int nRet = 0;
	CString strFormat;
	int nFuncRet = RET_PROCEED;

	int max = MAX_BUFFER;

	if( nCnt >= 0 && nCnt <= 39)
	{
		for (int i =0; i<2; i++)
		{
			for (int j = 39; j>= 1; j--)
			{
				if (st_Buffer_info.strBufferSerial[i][j - 1] != _T(""))
 				{
					st_Buffer_info.strBufferSerial[i][j] = st_Buffer_info.strBufferSerial[i][j - 1];
					//st_Buffer_info.nBufferData[i][j][EXIST] = st_Buffer_info.nBufferData[i][j - 1][EXIST];
					//st_Buffer_info.nBufferData[i][j][BIN] = st_Buffer_info.nBufferData[i][j - 1][BIN];
				}
			}
		}
	}

	if (nCnt >= 0  && nCnt <= 39)
	{
  		st_Buffer_info.strBufferSerial[0][0].Format(_T("%d"),m_nCntBcrNum);
  		st_Buffer_info.strBufferSerial[1][0].Format(_T("%d"),m_nCntBcrNum);

		st_sync_info.nLabelRecv = CTL_NO;

		strFormat.Format(_T("^XA^LH500,025^FO10,10^BY3^B3N,N,150,Y,N^FD%s^FS^FO1400,10^BY3^B3N,N,150,Y,N^FD%s^FS^XZ"),st_Buffer_info.strBufferSerial[0][0],st_Buffer_info.strBufferSerial[1][0]);
		(clsFunc.m_pZebra[0])->OnPrintOutput(2, PRINTER_NETWORK, 0, 0,  strFormat);
		m_nCntBcrNum++;		
		nFuncRet = RET_GOOD;
	}
	else if (nCnt >= 39)
	{		
		st_sync_info.nLabelRecv = CTL_YES;
		nFuncRet = RET_GOOD;
	}
	return nFuncRet;
}
int CRunRobot::OnVisionDataCheck(int nPos)
{
	int nFailCnt;
	int nVisionData;
	int nRet = CTL_PROCEED;

	switch(m_nVisionStep)
	{
		case 0:
			m_nVisionStep = 100;
			break;

		case 100:
			//::SendMessage( st_handler_info.hWnd, WM_MAINFRAME_WORK, MAIN_TEACH_VISION,0 );
			//kwlee 2017.0327
			if (nPos > 0)
			{
				::SendMessage(st_handler_info.hWnd, WM_MAINFRAME_WORK, MAIN_TEACH_VISION,nPos);
				m_nVisionStep = 200;
			}
			break;

		case 200:
			nFailCnt = 0;
			for (int i = 0; i< 10; i++)
			{
				nVisionData = _ttoi(st_Vision_info.strVisionData[i]);
				if (nVisionData == VISION_FAIL)
				{
					nFailCnt++;
				}
			}

			if (nFailCnt > 0)
			{
				if (m_nVisionRetry > st_basic_info.nVisionErrorCnt)
				{
					m_nVisionRetry = 0;
					m_nVisionStep = 0;
					nRet = CTL_ERROR;
				}
				else
				{
					m_nVisionStep = 100;
					m_nVisionRetry++;
				}
			}
			else
			{
				nRet = CTL_GOOD;
			}
			break;
		}
	return nRet;
}
void CRunRobot::OnLabelSensorCheck()
{
	int nRet = 0;
	switch (m_nCheckStep)
	{
	case 0:
		if(st_sync_info.nLabelCheckReq == CTL_REQ)
		{
			m_nCheckStep = 1000;
		}
		break;

	case  1000:
		nRet = FAS_IO.get_in_bit(st_io_info.i_LabelFeederProductChk1_1,IO_ON);
		m_nCurrLabelChk = nRet;
		m_dwCheckOnOffTime[0] = GetCurrentTime();
		m_nCheckStep = 1050;
		break;

	case 1050:
		//if( GetCurrentTime() - m_dwCheckOnOffTime[0] <= 0) m_dwCheckOnOffTime[0] = GetCurrentTime();
		m_dwCheckOnOffTime[1] = GetCurrentTime();
		m_dwCheckOnOffTime[2] = m_dwCheckOnOffTime[1] - m_dwCheckOnOffTime[0];

		if (m_dwCheckOnOffTime[2] <= 0)
		{
			m_dwCheckOnOffTime[0] = GetCurrentTime();
		}
		
		nRet = FAS_IO.get_in_bit(st_io_info.i_LabelFeederProductChk1_1,IO_ON);
		//if( m_nCurrLabelChk == nRet)
		if( m_nCurrLabelChk == nRet)
		{
			//if( GetCurrentTime() - m_dwCheckOnOffTime > 20 )
			if (m_dwCheckOnOffTime[2] > 20)
			{
				m_nCheckStep = 1100;
			}
		}
		else
		{
			m_nCheckStep = 1000;

		}
		break;

	case 1100:
		nRet = FAS_IO.get_in_bit(st_io_info.i_LabelFeederProductChk1_1,IO_ON);
		if(1 || m_nCurrLabelChk != nRet)
		{
			m_nCurrLabelChk = nRet;
			m_dwCheckOnOffTime[0] = GetCurrentTime();
			m_nCheckStep = 1200;
		}
		//kwlee 2017.0309
		else
		{
			m_nCheckStep = 1000;
		}
		break;

	case 1200:
		//if( GetCurrentTime() - m_dwCheckOnOffTime <= 0) m_dwCheckOnOffTime = GetCurrentTime();
		nRet = FAS_IO.get_in_bit(st_io_info.i_LabelFeederProductChk1_1,IO_ON);
		m_dwCheckOnOffTime[1] = GetCurrentTime();
		m_dwCheckOnOffTime[2] = m_dwCheckOnOffTime[1] - m_dwCheckOnOffTime[0];

		if (m_dwCheckOnOffTime[2] <= 0)
		{
			m_dwCheckOnOffTime[0] = GetCurrentTime();
		}

		if( m_nCurrLabelChk == nRet)
		{
			//if( GetCurrentTime() - m_dwCheckOnOffTime > 30 )
			if (m_dwCheckOnOffTime[2] > 30)
			{
				m_nCheckStep = 1300;
			}
		}
		else
		{
			m_nCheckStep = 1100;
		}
		break;

	case 1300:
		m_nCheckStep = 0;
		st_sync_info.nLabelCheckReq = CTL_READY;
		break;

	}
}

void CRunRobot::OnVaccummSet(int nMode, int nPickCnt, int OnOff)
{
	int i;
	int nTemp = 0;
	if (OnOff == IO_ON)
	{
		for (i = 0; i < nPickCnt; i++)
		{
			if (nMode == 1)
			{
// 				if ((st_Picker_info.nPickerData[0][i][X_FAIL_POS] == LEFT && st_Picker_info.nPickerData[0][i][FAILPICK] == YES) || 
// 					(st_Picker_info.nPickerData[1][nPickCnt - i][X_FAIL_POS] == RIGHT && st_Picker_info.nPickerData[1][nPickCnt - i][FAILPICK] == YES))
// 				{
					//if (i < 5 &&st_Picker_info.nPickerData[0][i][X_FAIL_POS] == LEFT)
				if (i < 5 && st_Picker_info.nPickerData[0][i][FAILPICK] == YES)
				{
					//if (st_Buffer_info[PICK].nBufferData[0][i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[0][i][EXIST] == YES)
					//if (st_Picker_info.nPickerData[0][i][BIN] == FAIL && st_Picker_info.nPickerData[0][i][EXIST] == YES)
					//{
					FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[st_Picker_info.nPickerData[0][i][Y_FAIL_POS]],IO_ON);
// 							st_Picker_info.strPickerSerial[0][i] = _T("");
// 							st_Picker_info.nPickerData[0][i][BIN] = -1;
// 							st_Picker_info.nPickerData[0][i][EXIST] = NO;

					//}
					//break;
				}
				//else if (i > 4 && st_Picker_info.nPickerData[1][nPickCnt -i][X_POS] == RIGHT)
				//kwlee 20177.0221
				//else if (i > 4 && st_Picker_info.nPickerData[1][MAX_PICKER - (nPickCnt -i)][X_FAIL_POS] == RIGHT)
				else if (i > 4 && st_Picker_info.nPickerData[1][nPickCnt - i][FAILPICK] == YES)
				{
					//kwlee 2017.0221
// 					if (st_Picker_info.nPickerData[1][MAX_PICKER - (nPickCnt - i)][BIN] == FAIL && st_Picker_info.nPickerData[1][MAX_PICKER - (nPickCnt -i)][EXIST] == YES)
// 					{
						nTemp = MAX_PICKER + st_Picker_info.nPickerData[1][MAX_PICKER - (nPickCnt - i)][Y_FAIL_POS];	
						FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[nTemp + 1],IO_ON);
// 							st_Picker_info.strPickerSerial[1][nTemp + 1] = _T("");
// 							st_Picker_info.nPickerData[1][nTemp + 1][BIN] = -1;
// 							st_Picker_info.nPickerData[1][nTemp + 1][EXIST] = NO;
					//}
					//break;
						
				}
				else
				{

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
						if (st_Buffer_info.nBufferData[0][i][BIN] == GOOD && st_Buffer_info.nBufferData[0][i][EXIST] == YES)
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
							if (st_Buffer_info.nBufferData[1][nPickCnt - i][BIN] == GOOD && st_Buffer_info.nBufferData[1][nPickCnt - i][EXIST] == YES)
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
							if (st_Buffer_info.nBufferData[0][i][BIN] == GOOD && st_Buffer_info.nBufferData[0][i][EXIST] == YES)
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
							if (st_Buffer_info.nBufferData[1][nPickCnt - i][BIN] == GOOD && st_Buffer_info.nBufferData[1][nPickCnt - i][EXIST] == YES)
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
							//if (st_Buffer_info.nBufferData[0][i][BIN] == GOOD && st_Buffer_info.nBufferData[0][i][EXIST] == YES)
							//kwlee 2017.0317
// 							if (st_Buffer_info.nBufferData[0][39 - i][BIN] == GOOD && st_Buffer_info.nBufferData[0][39 - i][EXIST] == YES)
// 							{
// 								FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[i],IO_ON);	
// 							}
// 							else
// 							{
// 								FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[i],IO_OFF);	
// 							}
							//kwlee 2017.0322
							FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[i],IO_ON);	
						}
						else if (i > 4)
						{
							//FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[PICKCNT + 3 - i],IO_ON);
							//kwlee 2017.0204
							//if (st_Buffer_info.nBufferData[1][nPickCnt - i][BIN] == GOOD)
// 							if (st_Buffer_info.nBufferData[1][39 - (nPickCnt - i)][BIN] == GOOD && st_Buffer_info.nBufferData[1][39 - (nPickCnt - i)][EXIST] == YES)
// 							{
// 								FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[PICKCNT + 3 - i],IO_ON);
// 							}
// 							else
// 							{
// 								FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[PICKCNT + 3 - i],IO_OFF);
// 							}
							//kwlee 2017.0322
							FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[PICKCNT + 3 - i],IO_ON);
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
				if ((st_Picker_info.nPickerData[0][i][X_FAIL_POS] == LEFT && st_Picker_info.nPickerData[0][i][FAILPICK] == YES) || 
					(st_Picker_info.nPickerData[1][i][X_FAIL_POS] == RIGHT && st_Picker_info.nPickerData[1][i][FAILPICK] == YES))
				{
					if (st_Picker_info.nPickerData[0][i][X_FAIL_POS] == LEFT)
					{
						//if (st_Buffer_info[PICK].nBufferData[0][i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[0][i][EXIST] == YES)
						if (st_Picker_info.nPickerData[0][i][BIN] == FAIL && st_Picker_info.nPickerData[0][i][EXIST] == YES)
						{
							FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[st_Picker_info.nPickerData[0][i][Y_FAIL_POS]],IO_OFF);
							st_Picker_info.strPickerSerial[0][i] = _T("");
							st_Picker_info.nPickerData[0][i][BIN] = -1;
							st_Picker_info.nPickerData[0][i][EXIST] = NO;
						}
						//break;
					}
					//else if (i > 4 && st_Picker_info.nPickerData[1][nPickCnt - i][X_POS] == RIGHT)
					//kwlee 2017.0221
					else if (st_Picker_info.nPickerData[1][i][X_FAIL_POS] == RIGHT)
					{
						//kwlee 2017.0221
						if (st_Picker_info.nPickerData[1][i][BIN] == FAIL && st_Picker_info.nPickerData[1][i][EXIST] == YES)
						{
							nTemp = MAX_PICKER + st_Picker_info.nPickerData[1][MAX_PICKER - i][Y_FAIL_POS];
							FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[nTemp + 1],IO_OFF);
							st_Picker_info.strPickerSerial[1][nTemp + 1] = _T("");
							st_Picker_info.nPickerData[1][nTemp + 1][BIN] = -1;
							st_Picker_info.nPickerData[1][nTemp + 1][EXIST] = NO;
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
					else //Sodimm, Udimm_9
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
		m_npTemp_Picker_YesNo[i] = CTL_NO;
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
					if (st_Buffer_info.nBufferData[0][i][BIN] == GOOD && st_Buffer_info.nBufferData[0][i][EXIST] == YES)
					{
						m_npTemp_Picker_YesNo[i] = CTL_YES;
					}
					else
					{
						m_npTemp_Picker_YesNo[i] = CTL_NO;
					}
				}
				else
				{
					//clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT + 3 - i] = CTL_YES;
					//if (st_Buffer_info.nBufferData[1][10 - i][BIN] == GOOD)
					if (st_Buffer_info.nBufferData[1][nPickCnt - i][BIN] == GOOD && st_Buffer_info.nBufferData[1][nPickCnt - i][EXIST] == YES)
					{
						m_npTemp_Picker_YesNo[PICKCNT + 3 - i] = CTL_YES;
					}
					else
					{
						m_npTemp_Picker_YesNo[PICKCNT + 3 - i] = CTL_NO;
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
						if (st_Buffer_info.nBufferData[0][i][BIN] == GOOD && st_Buffer_info.nBufferData[0][i][EXIST] == YES)
						{
							m_npTemp_Picker_YesNo[i] = CTL_YES;
						}
						else
						{
							m_npTemp_Picker_YesNo[i] = CTL_NO;
						}
					}
					else
					{
						//clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT - i] = CTL_YES;	
						//kwlee 2017.0204
						if (st_Buffer_info.nBufferData[1][nPickCnt - i][BIN] == GOOD && st_Buffer_info.nBufferData[1][nPickCnt - i][EXIST] == YES)
						{
							m_npTemp_Picker_YesNo[PICKCNT - i] = CTL_YES;
						}
						else
						{
							m_npTemp_Picker_YesNo[PICKCNT - i] = CTL_NO;
						}
					}
				}
				else //Sodimm, Udimm_9
				{
					if (i <5)
					{
						//clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_YES;
						//kwlee 2017.0204
						//if (st_Buffer_info.nBufferData[0][i][BIN] == GOOD && st_Buffer_info.nBufferData[0][i][EXIST] == YES)
						//kwle 2017.0317
						if (st_Buffer_info.nBufferData[0][39 - i][BIN] == GOOD && st_Buffer_info.nBufferData[0][39 - i][EXIST] == YES)
						{
							m_npTemp_Picker_YesNo[i] = CTL_YES;
						}
						else
						{
							m_npTemp_Picker_YesNo[i] = CTL_NO;
						}
					}
					else if (i > 4)
					{
						//clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT + 5 - i] = CTL_YES;	
						//kwlee 2017.0204
						if (st_Buffer_info.nBufferData[1][39 - (nPickCnt - i)][BIN] == GOOD && st_Buffer_info.nBufferData[1][39 - (nPickCnt - i)][EXIST] == YES)
						{
							m_npTemp_Picker_YesNo[PICKCNT + 3 - i] = CTL_YES;	
						}
						else
						{
							m_npTemp_Picker_YesNo[PICKCNT + 3 - i] = CTL_NO;
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

	for (i=0; i<11; i++)
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
	int nRet = 0;
	CString strMsg;
	int nBcrReadPos = 0;
	
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
// 		if( st_sync_info.nBcrReq == CTL_REQ)
// 		{
// 			st_sync_info.nBcrReq = CTL_READY;
// 			m_nInterFaceStep = 200;
// 		}
		//kwlee 2017.0302
		if(st_sync_info.nBcrReq == CTL_REQ)
		{
			if ( 0 && st_basic_info.nModeDevice == WITHOUT_DVC)
			{
				m_dwBcrTime[0] = GetTickCount();
				m_nInterFaceStep = 110;
			}
			else
			{
				m_dwBcrTime[0] = GetTickCount();
				m_nInterFaceStep = 111;
			}
		}
		break;

	case  110:
		//kwlee 2017.0310
		if (st_client_info[BCR1_NETWORK].nConnect == NO)
		{
			::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + BCR1_NETWORK, CLIENT_CONNECT, 0);
		}
		if (st_client_info[BCR2_NETWORK].nConnect == NO)
		{
			::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + BCR2_NETWORK, CLIENT_CONNECT, 0);
		}
		if (st_client_info[PRINTER_NETWORK].nConnect == NO)
		{
			::SendMessage( st_handler_info.hWnd, WM_CLIENT_MSG_3, CLIENT_CONNECT, 0);
		}

		m_dwBcrTime[0] = GetTickCount();
		m_nInterFaceStep = 111;
		break;

	case 111:
		if (0 && st_basic_info.nModeDevice == WITHOUT_DVC)
		{
			st_sync_info.nBcrReq = CTL_READY;
			m_nInterFaceStep = 200;
		}
		else
		{
			if (st_client_info[BCR1_NETWORK].nConnect == YES && st_client_info[BCR2_NETWORK].nConnect == YES &&
				st_client_info[PRINTER_NETWORK].nConnect == YES)
			{
				st_sync_info.nBcrReq = CTL_READY;
				m_nInterFaceStep = 200;
			}
			else 
			{
				m_dwBcrTime[1] = GetTickCount();
				m_dwBcrTime[2] = m_dwBcrTime[1] - m_dwBcrTime[0];

				if (m_dwBcrTime[2] <= (DWORD)0)
				{
					m_dwBcrTime[0] = GetTickCount();
					break;
				}
				
				//if (m_dwBcrTime[2] > 1000)
				//kwlee 2017.0320
				if (m_dwBcrTime[2] > 300)
				{
					m_nInterFaceStep = 110;
				}
			}
		}
		break;

	case 200:
		if (st_client_info[PRINTER_NETWORK].nConnect == NO || st_client_info[BCR1_NETWORK].nConnect == NO || 
			st_client_info[BCR2_NETWORK].nConnect == NO)
		{
			m_nInterFaceStep = 110;
		}
		else
		{
			m_nInterFaceStep = 210;
		}
		break;

	case 210:
		//Printer 출력
		nRet = OnPrinterFeeder(m_nPrintOutPutCnt,m_nLabelFailCheck); //Buffer 저장.
		if (nRet == RET_GOOD)
		{
			m_dwBcrTime[0] = GetTickCount(); //kwlee 2017.0310
			m_nInterFaceStep = 300;
		}
		else if (nRet == RET_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(720, dWARNING, m_strAlarmCode);
		}
		break;

	case 300:
		m_dwBcrTime[1] = GetTickCount();
		m_dwBcrTime[2] = m_dwBcrTime[1] - m_dwBcrTime[0];

		if (m_dwBcrTime[2] <= (DWORD)0)
		{
			m_dwBcrTime[0] = GetTickCount();
			break;
		}
		if (m_dwBcrTime[2] > 500)
		{
			if(1 || st_sync_info.nLabelRecv == CTL_YES )
			{
				m_nPrintOutCheck = TRUE;
				st_sync_info.nLabelCheckReq = CTL_REQ;
				m_nInterFaceStep = 400;
			}
		}
		break;

	//모터를 끄고 라벨을 출력완료했으니
	//피더를움직이자
	case 400:
		st_sync_info.nLabelRbt_Dvc_Req[0] = CTL_REQ;
		st_sync_info.nLabelRbt_Dvc_Req[1] = BCR_LOAD;
		m_nInterFaceStep = 500;
		break;

	case 500:
		if( st_sync_info.nLabelRbt_Dvc_Req[0] == CTL_READY)
		{
			st_sync_info.nLabelRbt_Dvc_Req[0] = CTL_CLEAR;
			//m_nInterFaceStep = 600;
			//kwlee 20170310
			//FAS_IO.set_out_bit(st_io_info.o_LabelStopperCylinder,IO_OFF);
			m_nInterFaceStep = 1000;
		}
		break;

// 	case 600: //모터 이동을 완료했으니 모터 전원을 ON하자
// 		FAS_IO.set_out_bit(st_io_info.o_LabelStopperCylinder,IO_OFF);
// 		if( OnBcrConveyorOnOff( IO_ON ) == RET_GOOD )
// 		{
// 			m_nInterFaceStep = 1000;
// 		}
// 		break;

	case 1000:
		//출력 확인.
		//BCR Read
		//Print 출력후 넘어온 값 Parsing "Complete" 이면 변수 셋팅
		if (m_nPrintOutCheck == TRUE)
		{
// 			if (st_basic_info.nModeDevice == WITHOUT_DVC)
// 			{
// 				st_Buffer_info.nBufferData[0][0][EXIST] = YES;
// 				st_Buffer_info.nBufferData[1][0][EXIST] = YES;
// 			}
			//kwlee 2017.0313
			st_Buffer_info.nBufferData[0][0][EXIST] = YES;
			st_Buffer_info.nBufferData[1][0][EXIST] = YES;

			m_nPrintOutCheck = FALSE;
			m_dwLabelOutTime[0] = GetTickCount(); //kwlee 2017.0307

		//	nRet = FAS_IO.get_in_bit(st_io_info.i_LabelFeederProductChk1,IO_ON);

			if (1/*nRet == IO_ON && m_nPrintOutPutCnt >= 39*/)
			{
				if (m_nPrintOutPutCnt > st_basic_info.nBarcodeReadPos)
				{
					if (st_basic_info.nModeDevice == WITH_DVC)
					{
						::PostMessage(st_handler_info.hWnd, WM_CLIENT_MSG + BCR1_NETWORK, CLIENT_SEND, 0);
						::PostMessage(st_handler_info.hWnd, WM_CLIENT_MSG + BCR2_NETWORK, CLIENT_SEND, 0);
					}
					if (1 || st_basic_info.nModeDevice == WITHOUT_DVC)
					{
						m_nBarcodeReadCheck[0] = TRUE;
						m_nBarcodeReadCheck[1] = TRUE;
						m_strBarcode[0] = st_Buffer_info.strBufferSerial[0][st_basic_info.nBarcodeReadPos];
						m_strBarcode[1] = st_Buffer_info.strBufferSerial[1][st_basic_info.nBarcodeReadPos];
					}
				}
			}
			else
			{
				if (m_nPrintOutPutCnt >= st_basic_info.nBarcodeReadPos)
				{
					::PostMessage(st_handler_info.hWnd, WM_CLIENT_MSG + BCR1_NETWORK, CLIENT_SEND, 0);
					::PostMessage(st_handler_info.hWnd, WM_CLIENT_MSG + BCR2_NETWORK, CLIENT_SEND, 0);

					if (st_basic_info.nModeDevice == WITHOUT_DVC)
					{
						m_nBarcodeReadCheck[0] = TRUE;
						m_nBarcodeReadCheck[1] = TRUE;
						m_strBarcode[0] = st_Buffer_info.strBufferSerial[0][st_basic_info.nBarcodeReadPos];
						m_strBarcode[1] = st_Buffer_info.strBufferSerial[1][st_basic_info.nBarcodeReadPos];
					}
				}
				m_nPrintOutPutCnt++;
			}
			m_nInterFaceStep = 1100;
		}
		break;

		//BCR Read 확인.
	case 1100:
		//kwlee 2017.0307
		m_dwLabelOutTime[1] = GetCurrentTime();
		m_dwLabelOutTime[2] = m_dwLabelOutTime[1] - m_dwLabelOutTime[0];

		if (m_dwLabelOutTime[2] <= 0)
		{
			m_dwLabelOutTime[0] = GetCurrentTime();
		}

		if (m_dwLabelOutTime[2] > 500)
		{
			for (int i = 0; i<2; i++)
			{
				if (m_nBarcodeReadCheck[i] == TRUE)
				{
					if (1 || st_Buffer_info.strBufferSerial[i][st_basic_info.nBarcodeReadPos] == m_strBarcode[i] && 
						 st_Buffer_info.strBufferSerial[i][st_basic_info.nBarcodeReadPos] !=_T("") && m_strBarcode[i] != _T(""))
					{
						st_Buffer_info.nBufferData[i][st_basic_info.nBarcodeReadPos][BIN] = GOOD;
					}
					else
					{
						st_Buffer_info.nBufferData[i][st_basic_info.nBarcodeReadPos][BIN] = FAIL;
					}
					m_nBarcodeReadCheck[i] = FALSE;
				}
			}
			//kwlee 2017.0309
			if( m_nPrintOutPutCnt >= 0 && m_nPrintOutPutCnt <= 39)
			{
				for (int i =0; i<2; i++)
				{
					for (int j = 39; j>= 1; j--)
					{
						if (st_Buffer_info.strBufferSerial[i][j - 1] != _T(""))
						{
							//st_Buffer_info.strBufferSerial[i][j] = st_Buffer_info.strBufferSerial[i][j - 1];
							st_Buffer_info.nBufferData[i][j][EXIST] = st_Buffer_info.nBufferData[i][j - 1][EXIST];
							st_Buffer_info.nBufferData[i][j][BIN] = st_Buffer_info.nBufferData[i][j - 1][BIN];
						}
					}
				}
			}

			if (m_nPrintOutPutCnt < 40)
				m_nPrintOutPutCnt++;
			//kwlee 2017.0302
	// 		nRet = FAS_IO.get_in_bit(st_io_info.i_LabelFeederProductChk1,IO_ON);
	// 		if (nRet == IO_ON && m_nPrintOutPutCnt > 39)
			//kwlee 2017.0307
			if (st_sync_info.nLabelCheckReq == CTL_READY && m_nPrintOutPutCnt >= 40)
			{
				m_dwTimeCheck[0] = GetCurrentTime();
				m_nInterFaceStep = 2000;
			}
			else
			{
				m_nInterFaceStep = 200;
			}
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
	CString strAlarm;
	int nRet[PICKCNT];
	int nNum = 11;

	for(int i = 0; i<nNum; i++)
		nRet[i] = NO;

	

	for (i=0; i<11; i++)
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

	if( m_nCnt >= 11)
	{
		return RET_GOOD;
	}

//  	if (nRet[0] == YES && nRet[1] == YES) return RET_GOOD;
// 	return RET_PROCEED;
	return RET_PROCEED;
}

void CRunRobot::OnRobotRun()
{

	int nRet_1, nRet_2=0, nRet_3=0, nRect[12]={0,};
	int nRet,nTemp = 0;
	double dCurrentPos = 0;
	CString str;
	int nPosCheck = 0;
	
	m_dPitch_Y = (st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST] - st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_END])/4;
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
		for (int i =0; i<PICKCNT; i++ )
		{
			m_npTemp_Picker_YesNo[i] = NO;
		}
		m_nRunStep = 100;								
		break;

	case 100:
		// loader robot picker up 요청
		for (int i =0; i<PICKCNT; i++ )
		{
			m_npTemp_Picker_YesNo[i] = YES;
		}
		//picker up
		/*st_sync_info.TurnConvJobReady[ROBOT] = CTL_NONE;*/
		OnSetPickerUpDn(0, PICKER_UP, m_npTemp_Picker_YesNo);
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
		nRet_1 = OnGetPickerUpDn(0, PICKER_UP, m_npTemp_Picker_YesNo);
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

	case 600:
		m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_REJECT];
		m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_REJECT];
		
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
		//kwlee 2017.0218
		OnSetLabelPick(1,m_nPickCnt);
		OnSetPickerUpDn(0, PICKER_DN, m_npTemp_Picker_YesNo);
		m_nRunStep = 720;
		break;

	case 715:
		OnSetPickerUpDn(0, PICKER_DN, m_npTemp_Picker_YesNo);
		m_nRunStep = 720;
		break;

	case 720:
// 		m_dwTimeCheck[1] = GetCurrentTime();
// 		m_dwTimeCheck[2] = m_dwTimeCheck[1] - m_dwTimeCheck[0];
// 		if( m_dwTimeCheck[2] <= 0 ) m_dwTimeCheck[0] = GetCurrentTime();
// 		if( m_dwTimeCheck[2] >  1000 )
// 		{
	
		nRet_1 = OnGetPickerUpDn(0, PICKER_DN,m_npTemp_Picker_YesNo);
		if(nRet_1 == RET_GOOD)
		{	
			m_nRunStep = 800;	
		}
		else if(nRet_1 == RET_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(750, dWARNING, m_strAlarmCode);
			m_nRunStep = 715;
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
		}
		else
		{
			OnVaccummSet(0,m_nPickCnt,IO_OFF);
		}
		
		m_nRunStep = 820;
		break;

	case 820:
		if (m_nLabelFailCheck == TRUE)
		{
			OnBlowSet(1,m_nPickCnt,IO_ON);
		}
		else
		{
			OnBlowSet(0,m_nPickCnt,IO_ON);
		}
		m_dwTimeCheck[0] = GetCurrentTime();
		m_nRunStep = 830;
		break;

	case 830:
		m_dwTimeCheck[1] = GetCurrentTime();
		m_dwTimeCheck[2] = m_dwTimeCheck[1] - m_dwTimeCheck[0];
		if( m_dwTimeCheck[2] <= 0 ) m_dwTimeCheck[0] = GetCurrentTime();
		if( m_dwTimeCheck[2] >  300 )
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
			OnSetPickerUpDn(0, PICKER_UP, m_npTemp_Picker_YesNo);
			m_nRunStep = 840;
		}
		break;

	case 840:
		nRet_1 = OnGetPickerUpDn(0, PICKER_UP, m_npTemp_Picker_YesNo);

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
			m_nRunStep = 930;
		}
		else if(nRet == RET_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(650, dWARNING, m_strAlarmCode);
			m_nRunStep = 910;
		}
		break;
		
	case 930:
		//버리고 데이터 삭제...
		if ( m_nLabelFailCheck == TRUE)
		{
			OnResetRejectData();
		}
		m_nRunStep = 1000;
		break;
		//REJECT BCR PLACE 후 
	case 1000:
		//kwlee 2017.0213
		if ( m_nLabelFailCheck == TRUE)
		{
			if (st_sync_info.nBcrReq != CTL_CHANGE ) 
			{
				break;
			}

			if(st_Buffer_info.nBufferData[1][39][BIN] == GOOD && st_Buffer_info.nBufferData[1][39][EXIST] == YES)
			{	
				for (int j = 0; j < m_nPickCnt; j++)
				{
					//if (st_Buffer_info[PICK].nBufferData[0][j][X_POS] == LEFT)
					if (j <5 && st_Picker_info.nPickerData[0][j][FAILPICK] == YES)
					{
						m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST]  - (st_Picker_info.nPickerData[0][j][Y_FAIL_POS] * m_dPitch_Y);
						m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_END];
						break;
					}
					else if (j > 4 && st_Picker_info.nPickerData[1][m_nPickCnt - j][FAILPICK] == YES)
					{	
						//m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST]  - (st_Picker_info.nPickerData[1][MAX_PICKER - j][Y_POS] * m_dPitch_Y);
						m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST]  - (st_Picker_info.nPickerData[1][m_nPickCnt - j][Y_FAIL_POS] * m_dPitch_Y);
						m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_FIRST];
						break;
					}
					else
					{

					}
				}
				m_nRunStep = 3800;
				break;
			}
			//else
			//kwlee 2017.0224
			else if (st_Buffer_info.nBufferData[0][39][BIN] == GOOD && st_Buffer_info.nBufferData[0][39][EXIST] == YES)
			{
				//m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST];
				//m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_FIRST];
				//kwlee 2017.0224

				for (int j = 0; j < m_nPickCnt; j++)
				{
					//if (st_Buffer_info[PICK].nBufferData[0][j][X_POS] == LEFT)
					if (j <5 && st_Picker_info.nPickerData[0][j][FAILPICK] == YES)
					{
						m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST]  - (st_Picker_info.nPickerData[0][j][Y_FAIL_POS] * m_dPitch_Y);
						m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_FIRST];
						break;
					}
					else if (j > 4 && st_Picker_info.nPickerData[1][m_nPickCnt - j][FAILPICK] == YES)
					{	
						//m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST]  - (st_Picker_info.nPickerData[1][MAX_PICKER - j][Y_POS] * m_dPitch_Y);
						m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST]  - (st_Picker_info.nPickerData[1][m_nPickCnt - j][Y_FAIL_POS] * m_dPitch_Y);
						//m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_END] - st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_FIRST];
						m_dpTargetPosList[1] = 0.962;
						
						break;
					}
					else
					{

					}
				}
				m_nRunStep = 3800;
				break;
			}
			//kwlee 2017.0310
			else
			{
				m_nRunStep = 7320;
			}
		}
		else
		{
			m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST];
			m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_FIRST];
			m_nRunStep = 2100;
		}
		//m_nRunStep = 2100;
		break;

		//kwlee 2017.0210
	case 2100:
		//nRet_1 = CTL_Lib.Linear_Move(ONLY_MOVE_START, m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, COMI.mn_runspeed_rate);
		//kwlee 2017.0323
		nRet_1 = CTL_Lib.Linear_Move(BOTH_MOVE_FINISH, m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, COMI.mn_runspeed_rate);

		if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
		{
			///m_nRunStep = 3000;
			//kwlee 2017.0323
			//m_nRunStep = 3010;
			//kwlee 2017.0329
			m_nRunStep = 3100;
		}
		else if(nRet_1 == BD_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(710, dWARNING, st_alarm_info.strCode);
		}
		break;
		//kwlee 2017.0323
// 	case 3000:
// 		//label pick 위치 확인
// 		nRet = CTL_Lib.Linear_Move(ONLY_MOVE_CHECK, m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, COMI.mn_runspeed_rate);
// 		if(nRet == BD_GOOD) //정상적으로 완료된 상태 
// 		{						
// 			//m_nRunStep = 3100;
// 			//kwlee 2017.0307
// 			m_nRunStep = 3010;
// 		}
// 		else if (nRet == BD_RETRY)
// 		{
// 			m_nRunStep = 2100;
// 		}
// 		else if (nRet == BD_ERROR || nRet == BD_SAFETY)
// 		{			
// 			m_nRunStep = 2100;
// 			CTL_Lib.Alarm_Error_Occurrence(720, dWARNING, st_alarm_info.strCode);
// 		}
// 		break;
		
		
		//kwlee 20170329
// 	case 3010:
// 		OnSetFeederClampCylUpDn(0,IO_OFF);
// 		m_nRunStep = 3020;
// 		break;
// 
// 	case 3020:
// 		nRet = OnGetFeederClampCylUpDn(0,IO_OFF);
// 		if (nRet == RET_GOOD)
// 		{
// 			m_dwStopperCylWaitTime[0] = GetCurrentTime();
// 			m_nRunStep = 3100;
// 		}
// 		else if (nRet == RET_ERROR)
// 		{
// 			CTL_Lib.Alarm_Error_Occurrence(721, dWARNING, st_alarm_info.strCode);
// 			m_nRunStep = 3010;
// 		}
// 		break;
// 		
// 	case 3030:
// 		m_dwStopperCylWaitTime[1] = GetCurrentTime();
// 		 m_dwStopperCylWaitTime[2] = m_dwStopperCylWaitTime[1] - m_dwStopperCylWaitTime[0];
// 
// 		if (m_dwStopperCylWaitTime[2] <= 0)
// 		{
// 			m_dwStopperCylWaitTime[0] = GetCurrentTime();
// 		}
// 		if (m_dwStopperCylWaitTime[2] > 500)
// 		{
// 			nRet = FAS_IO.get_in_bit(st_io_info.i_LabelStopperCylinderChk,IO_OFF);
// 			if (nRet == IO_OFF)
// 			{
// 				m_nRunStep = 3100;
// 			}
// 			else
// 			{
// 				CTL_Lib.Alarm_Error_Occurrence(721, dWARNING, st_alarm_info.strCode);
// 				m_nRunStep = 3020;
// 			}
// 		}
// 		break;


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
// 			if( st_sync_info.nBcrReq == CTL_CHANGE || st_sync_info.nBcrReq == CTL_CLEAR)
// 			{
// 				st_sync_info.nBcrReq = CTL_REQ;
// 			}
			if (m_nLabelFailCheck == FALSE)
			{
				if( st_sync_info.nBcrReq == CTL_CHANGE || st_sync_info.nBcrReq == CTL_CLEAR)
				{
					st_sync_info.nBcrReq = CTL_REQ;
				}
			}
 			m_dwTimeCheck[0] = GetCurrentTime();
// 			m_nRunStep = 3110;
			//kwlee 2017.0320
			m_nRunStep = 3200;
			
		}
		break;

	case 3110:
		m_dwTimeCheck[1] = GetCurrentTime();
		m_dwTimeCheck[2] = m_dwTimeCheck[1] - m_dwTimeCheck[0];
		if( m_dwTimeCheck[2] <= 0 ) m_dwTimeCheck[0] = GetCurrentTime();
		if( m_dwTimeCheck[2] >  300)
		{
			m_dwTimeCheck[0] = GetCurrentTime();
			m_nRunStep = 3200;
		}
		break;

		//kwlee 2017.0119 Barcode Read
		//1. 바코드 Read
		//2. 양품인지 확인.
	case 3200:
		//kwlee 2017.0218
		m_dwTimeCheck[1] = GetCurrentTime();
		m_dwTimeCheck[2] = m_dwTimeCheck[1] - m_dwTimeCheck[0];
		if( m_dwTimeCheck[2] <= 0 ) m_dwTimeCheck[0] = GetCurrentTime();
		if( m_dwTimeCheck[2] >  300)
		{
			if (st_sync_info.nPrinterStatus == BD_READY && st_sync_info.nBcrReq == CTL_CHANGE)
			{
				//kwlee 2017.0318
				//m_nRunStep = 3210;
				m_nRunStep = 4000;
			}
		}
		break;

		//kwlee 2017.0307
	case 3210:
		OnSetFeederClampCylUpDn(0,IO_ON);
		m_nRunStep = 3220;
		break;

	case 3220:
		nRet = OnGetFeederClampCylUpDn(0,IO_ON);
		if (nRet == RET_GOOD)
		{
			m_dwStopperCylWaitTime[0] = GetCurrentTime();
			//kwlee 2017.0310
			//FAS_IO.set_out_bit(st_io_info.o_LabelStopperCylinder, IO_ON);
			//m_nRunStep = 3230;
			m_nRunStep = 4000;
		}
		else if (nRet == RET_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(721, dWARNING, st_alarm_info.strCode);
			m_nRunStep = 3210;
		}
		break;

	case 3230:
		m_dwStopperCylWaitTime[1] = GetCurrentTime();
		m_dwStopperCylWaitTime[2] = m_dwStopperCylWaitTime[1] - m_dwStopperCylWaitTime[0];

		if (m_dwStopperCylWaitTime[2] <= 0)
		{
			m_dwStopperCylWaitTime[0] = GetCurrentTime();
		}
		if (m_dwStopperCylWaitTime[2] > 300)
		{
			nRet = FAS_IO.get_in_bit(st_io_info.i_LabelStopperCylinderChk,IO_ON);
			if (nRet == IO_ON)
			{
				m_nRunStep = 4000;
			}
			else
			{
				CTL_Lib.Alarm_Error_Occurrence(721, dWARNING, st_alarm_info.strCode);
				m_nRunStep = 3020;
			}
		}
		break;

	case 3800:
		//nRet_1 = CTL_Lib.Linear_Move(ONLY_MOVE_START, m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, COMI.mn_runspeed_rate);
		//kwlee 2017.0323
		nRet_1 = CTL_Lib.Linear_Move(BOTH_MOVE_FINISH, m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, COMI.mn_runspeed_rate);

		if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
		{
			//m_nRunStep = 2100;
			//kwlee 2017.0213
			//m_nRunStep = 3900;
			//kwlee 2017.0323
			m_nRunStep = 4000;
		}
		else if(nRet_1 == BD_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(710, dWARNING, st_alarm_info.strCode);
			//	m_nRunStep = 0;
		}
		break;
	//kwleee 2017.0323
// 	case  3900:
// 		nRet = CTL_Lib.Linear_Move(ONLY_MOVE_CHECK, m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, COMI.mn_runspeed_rate);
// 		if(nRet == BD_GOOD) //정상적으로 완료된 상태 
// 		{						
// 			m_nRunStep = 4000;
// 		}
// 		else if (nRet == BD_RETRY)
// 		{
// 			m_nRunStep = 3800;
// 		}
// 		else if (nRet == BD_ERROR || nRet == BD_SAFETY)
// 		{			
// 			m_nRunStep = 3800;
// 			CTL_Lib.Alarm_Error_Occurrence(720, dWARNING, st_alarm_info.strCode);
// 		}
// 		break;

	case 4000:
		m_bLabelFeederNum = false;
		if (m_nLabelFailCheck == TRUE )
		{
			for (int i =0; i<PICKCNT; i++ )
			{
				m_npTemp_Picker_YesNo[i] = CTL_NO;
			}

			if (st_sync_info.nPrinterStatus	== BD_READY && st_sync_info.nBcrReq == CTL_CHANGE)
			//Label Pick 동작..
			{
				if (st_Buffer_info.nBufferData[1][39][BIN] == GOOD && st_Buffer_info.nBufferData[1][39][EXIST] == YES)
				{
					//kwlee 2017.0221
					for (int i = 0; i<m_nPickCnt; i++)
					{
						//if (i < 5 && st_Picker_info.nPickerData[0][i][BIN] == FAIL && st_Picker_info.nPickerData[0][i][EXIST] == NO)
						if (i < 5 && st_Picker_info.nPickerData[0][i][FAILPICK] == YES)
						{
 							m_npTemp_Picker_YesNo[st_Picker_info.nPickerData[0][i][Y_FAIL_POS]] = CTL_YES;
							OnSetPickerUpDn(0, PICKER_DN, m_npTemp_Picker_YesNo);
							m_bLabelFeederNum = true;
							break;
						}
						//else if ( i > 4 && st_Picker_info.nPickerData[1][m_nPickCnt - i][BIN] == FAIL && st_Picker_info.nPickerData[1][m_nPickCnt - i][EXIST] == NO)
						else if ( i > 4 && st_Picker_info.nPickerData[1][m_nPickCnt - i][FAILPICK] == YES)
						{
 							m_npTemp_Picker_YesNo[MAX_PICKER + st_Picker_info.nPickerData[1][m_nPickCnt - i][Y_FAIL_POS] + 1] = CTL_YES;
 							OnSetPickerUpDn(0, PICKER_DN,m_npTemp_Picker_YesNo);
							m_bLabelFeederNum = true;
							break;
						}						
					}
					m_nRunStep = 5000;
				}
				//kwlee 2017.0224
				else if (st_Buffer_info.nBufferData[0][39][BIN] == GOOD && st_Buffer_info.nBufferData[0][39][EXIST] == YES)
				{
					for (int i = 0; i<m_nPickCnt; i++)
					{
						//if (i < 5 && st_Picker_info.nPickerData[0][i][BIN] == FAIL && st_Picker_info.nPickerData[0][i][EXIST] == NO)
						if (i < 5 && st_Picker_info.nPickerData[0][i][FAILPICK] == YES)
						{
							m_npTemp_Picker_YesNo[st_Picker_info.nPickerData[0][i][Y_FAIL_POS]] = CTL_YES;
							OnSetPickerUpDn(0, PICKER_DN, m_npTemp_Picker_YesNo);
							m_bLabelFeederNum = true;
							break;
						}
						//else if ( i > 4 && st_Picker_info.nPickerData[1][m_nPickCnt - i][BIN] == FAIL && st_Picker_info.nPickerData[1][m_nPickCnt - i][EXIST] == NO)
						else if ( i > 4 && st_Picker_info.nPickerData[1][m_nPickCnt - i][FAILPICK] == YES)
						{
							m_npTemp_Picker_YesNo[MAX_PICKER + st_Picker_info.nPickerData[1][m_nPickCnt - i][Y_FAIL_POS] + 1] = CTL_YES;
							OnSetPickerUpDn(0, PICKER_DN, m_npTemp_Picker_YesNo);
							m_bLabelFeederNum = true;
							break;
						}
					}
					m_nRunStep = 5000;
				}
				else
				{
					m_nRunStep = 7320;
				}
			}
// 			else
// 			{
// 				OnBarcodeReadCheck();
// 				OnSetLabelPick(1,m_nPickCnt); 
// 				clsRunRobot.OnSetPickerUpDn(0, PICKER_DN, clsRunRobot.m_npTemp_Picker_YesNo);
// 			}	
		}
		else
		{
			//Label Pick 동작..
			OnBarcodeReadCheck();
			OnSetLabelPick(0,m_nPickCnt); 
			OnSetPickerUpDn(0, PICKER_DN, m_npTemp_Picker_YesNo);
			m_nRunStep = 5000;
		}
			
		//}
		break;
		
	case 5000:
		//2017.0116
		//vaccum On
		//OnVaccummSet(0,m_nPickCnt,IO_ON);
		//kwlee 2017.0204
		if (m_nLabelFailCheck == TRUE)
		{
			OnBlowSet(1,m_nPickCnt,IO_OFF);
		}
		else
		{
			OnBlowSet(0,m_nPickCnt,IO_OFF);
		}
		m_dwStopperCylWaitTime[0] = GetCurrentTime();
		m_nRunStep = 5100;
		break;

	case 5100:
		m_dwStopperCylWaitTime[1] = GetCurrentTime();
		m_dwStopperCylWaitTime[2] = m_dwStopperCylWaitTime[1] - m_dwStopperCylWaitTime[0];

		if (m_dwStopperCylWaitTime[2] <= 0)
		{
			m_dwStopperCylWaitTime[0] = GetCurrentTime();
		}
		if (m_dwStopperCylWaitTime[2] > 300)
		{
			if (m_nLabelFailCheck == TRUE)
			{
				OnVaccummSet(1,m_nPickCnt,IO_ON);
			}
			else
			{
				OnVaccummSet(0,m_nPickCnt,IO_ON);
			}
			m_nRunStep = 6000;
		}
	
		break;

	case 6000:	
		//함수호출
		nRet_1 = COMI.Check_MotPosRange(M_PICKERRBT_X,st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_FIRST],150);
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
		nRet_1 = OnGetPickerUpDn(0, PICKER_DN,m_npTemp_Picker_YesNo);
		if(nRet_1 == RET_GOOD)
		{
			mn_Retry = 0;
			//m_nRunStep = 7010;	
			//kwlee 2017.0310
			//FAS_IO.set_out_bit(st_io_info.o_LabelStopperCylinder,IO_OFF);
			m_nRunStep = 7020;
		}
		else if(nRet_1 == RET_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(750, dWARNING, m_strAlarmCode);
		}
		break;
		//Label Feeder Motor 동작 추가.

		//일단 AC파워를 켜고 PITCH를 움직이자
// 	case 7010:
// 		FAS_IO.set_out_bit(st_io_info.o_LabelStopperCylinder,IO_OFF);
// 		if( OnBcrConveyorOnOff( IO_ON ) == RET_GOOD )
// 		{
// 			m_nRunStep = 7020;
// 		}
// 		break;

	case 7020:
		st_sync_info.nLabelRbt_Dvc_Req[0] = CTL_REQ;
		st_sync_info.nLabelRbt_Dvc_Req[1] = BCR_UNLOAD;
		if( m_bLabelFeederNum == true )	st_sync_info.nLabelRbt_Dvc_Req[2] = 1;
		else							st_sync_info.nLabelRbt_Dvc_Req[2] = 5;
		
		m_dwTimeCheck[0] = GetCurrentTime(); //kwlee 2017.0323 .
		m_nRunStep = 7030;
		//kwlee 2017.0323
		//m_nRunStep = 7100;
		break;

	case 7030:
// 		if( st_sync_info.nLabelRbt_Dvc_Req[0] == CTL_READY )
// 		{
// 			st_sync_info.nLabelRbt_Dvc_Req[0] = CTL_CLEAR; 
// 			m_nRunStep = 7100;
// 		}
		//kwlee 2017.0323
		m_dwTimeCheck[1] = GetCurrentTime();
		m_dwTimeCheck[2] = m_dwTimeCheck[1] - m_dwTimeCheck[0];

		if (m_dwTimeCheck[2] < 0)
		{
			m_dwTimeCheck[0] = GetCurrentTime();
		}

		if (m_dwTimeCheck[2] > 300)
		{
			if( st_sync_info.nLabelRbt_Dvc_Req[0] == CTL_READY )
			{
				st_sync_info.nLabelRbt_Dvc_Req[0] = CTL_CLEAR; 
				m_nRunStep = 7100;
			}
		}
		
		break;

	case 7100:
		OnSetPickerUpDn(0, PICKER_UP, m_npTemp_Picker_YesNo);
 		m_nRunStep = 7110;
		
		break;

	case 7110:
		//clsRunRobot.OnSetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);
		//m_nRunStep = 7120;
		nRet_1 = OnGetPickerUpDn(0, PICKER_UP, m_npTemp_Picker_YesNo);
	
		if(nRet_1 == RET_GOOD)
		{	
			//st_sync_info.nLabelRbt_Dvc_Req[0] = CTL_CLEAR; //kwlee 2017.0318
 			//m_nRunStep = 7300;
			//kwlee 2017.0324
			m_nRunStep = 7120;
 			mn_Retry = 0;
		}
		break;

	case 7120:	
		//z축 up
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_Z, st_motor_info[M_PICKERRBT_Z].d_pos[ROBOT_SAFETY], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			//st_sync_info.nLabelRbt_Dvc_Req[0] = CTL_CLEAR; //kwlee 2017.0323
			//m_nRunStep = 7310;
			m_dwTacTimeCheck[0] = GetCurrentTime(); //kwlee 2017.0328
			m_nRunStep = 7130;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(760, dWARNING, st_alarm_info.strCode);
			m_nRunStep = 7120;
		}
		break;

		//kwlee 2017.0324
	case 7130:
		st_sync_info.nLabelRbt_Dvc_Req[0] = CTL_REQ;
		st_sync_info.nLabelRbt_Dvc_Req[1] = BCR_LOAD;
		m_nRunStep = 7140;
		break;

	case 7140:
		if( st_sync_info.nLabelRbt_Dvc_Req[0] == CTL_READY)
		{
			st_sync_info.nLabelRbt_Dvc_Req[0] = CTL_CLEAR;
			m_nRunStep = 7310;
		}
		break;
		///

	case 7310: //kwlee 2017.0323
		if (m_nLabelFailCheck ==  TRUE)
		{
			OnDataExchange(REJECT_MODE,PICK);
		}
		else
		{
			OnDataExchange(NORMAL_MODE,PICK);
		}
	
		m_nRunStep = 7320;
		break;
	
	case 7320: 
		if ( 0 && st_basic_info.nModeDevice == WITHOUT_DVC)
		{
			m_nRunStep = 14000;
		}
		else
		{
			nRet = OnGetVaccumgmCheck(IO_ON,m_nPickCnt);
			if(1 || nRet == RET_GOOD)
			{
				m_nLabelFailCheck = FALSE;
				nTemp = 0;
				for (int i =0; i< 2; i++)
				{
					for (int j = 0; j< MAX_PICKER; j++)
					{
						if ((st_Buffer_info.strBufferSerial[i][39 - j] == _T("") && st_Buffer_info.nBufferData[i][39 - j][EXIST] == YES) ||
							(st_Buffer_info.nBufferData[i][39 - j][BIN] == FAIL || st_Buffer_info.nBufferData[i][39 - j][EXIST] == NO) )
						{
							nTemp++;
						}
					}
				}
				if (nTemp >= m_nPickCnt)
				{
					m_nPrintOutPutCnt = m_nPrintOutPutCnt - MAX_PICKER;		
					st_sync_info.nBcrReq = CTL_REQ;
				}
				else if (((st_Buffer_info.nBufferData[1][39][BIN] == FAIL && st_Buffer_info.nBufferData[1][39][EXIST] == YES) || st_Buffer_info.nBufferData[1][39][EXIST] == NO) || 
					((st_Buffer_info.nBufferData[0][39][BIN] == FAIL && st_Buffer_info.nBufferData[0][39][EXIST] == YES) || st_Buffer_info.nBufferData[0][39][EXIST] == NO))
				{
					m_nPrintOutPutCnt = m_nPrintOutPutCnt  - 1;
					st_sync_info.nBcrReq = CTL_REQ;
				}
				///m_nRunStep = 13000;
				//kwlee 2017.0327
				m_nRunStep = 14000;
			}
			else
			{
				//버리는 동작 .
				if (m_nLabelFailCheck == FALSE)
				{
					if(m_bLabelFeederNum == true)
					{
						m_nPrintOutPutCnt = m_nPrintOutPutCnt - 1;		
						st_sync_info.nBcrReq = CTL_REQ;
					}
					else
					{
						m_nPrintOutPutCnt = m_nPrintOutPutCnt - MAX_PICKER;		
						st_sync_info.nBcrReq = CTL_REQ;
					}
					m_bLabelFeederNum = false;

					m_nLabelFailCheck = TRUE;
					m_nRunStep = 510;	
					break;
				}
				else
				{
					nTemp = 0;
					for (int i =0; i< 2; i++)
					{
						for (int j = 0; j< MAX_PICKER; j++)
						{
							if ((st_Buffer_info.strBufferSerial[i][39 - j] == _T("") && st_Buffer_info.nBufferData[i][39 - j][EXIST] == YES) ||
								(st_Buffer_info.nBufferData[i][39 - j][BIN] == NONE || st_Buffer_info.nBufferData[i][39 - j][EXIST] == NO || st_Buffer_info.nBufferData[i][39 - j][BIN] == FAIL))							//kwlee 2017.022
							{
								nTemp++;
							}
						}
					}
					if (nTemp >= m_nPickCnt )
					{	
						m_nPrintOutPutCnt = m_nPrintOutPutCnt - MAX_PICKER;		
						if (m_nPrintOutPutCnt < 0 || m_nPrintOutPutCnt < 35)
						{
							m_nPrintOutPutCnt = 35;
						}
						st_sync_info.nBcrReq = CTL_REQ;
					}
					else if (((nTemp <=2 && st_Buffer_info.nBufferData[1][39][BIN] == FAIL && st_Buffer_info.nBufferData[1][39][EXIST] == YES) || st_Buffer_info.nBufferData[1][39][EXIST] == NO || st_Buffer_info.nBufferData[1][39][BIN] == NONE) && 
						((nTemp<= 2 && st_Buffer_info.nBufferData[0][39][BIN] == FAIL && st_Buffer_info.nBufferData[0][39][EXIST] == YES) || st_Buffer_info.nBufferData[0][39][EXIST] == NO || st_Buffer_info.nBufferData[0][39][BIN] == NONE))
					{
						m_nPrintOutPutCnt = m_nPrintOutPutCnt  - 1;
						st_sync_info.nBcrReq = CTL_REQ;
					}
					//m_nRunStep = 1000;
					m_nRunStep = 510;
				}
			}
		}
		break;
		//kwlee 2017.0327
// 	case 13000:	
// 		//ROBOT TURN
// 		if (m_nSodimmSecondPlace == TRUE)
// 		{
// 			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_TURN, st_motor_info[M_PICKERRBT_TURN].d_pos[PITCH_UP_HEAD_TURN_ROTATOR_L_90], COMI.mn_runspeed_rate);
// 
// 			if (nRet_1 == BD_GOOD) //좌측으로 이동
// 			{
// 				m_nRunStep = 14000;
// 			}
// 			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
// 			{
// 				//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
// 				CTL_Lib.Alarm_Error_Occurrence(810, dWARNING, st_alarm_info.strCode);
// 				m_nRunStep = 13000;
// 			}
// 		}
// 		else
// 		{
// 			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_TURN, st_motor_info[M_PICKERRBT_TURN].d_pos[PITCH_UP_HEAD_TURN_ROTATOR_R_90], COMI.mn_runspeed_rate);
// 
// 			if (nRet_1 == BD_GOOD) //좌측으로 이동
// 			{
// 				m_nRunStep = 14000;
// 			}
// 			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
// 			{
// 				//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
// 				CTL_Lib.Alarm_Error_Occurrence(810, dWARNING, st_alarm_info.strCode);
// 				m_nRunStep = 13000;
// 
// 			}
// 		}
// 		break;

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
// 				if (st_sync_info.TurnConvJobReady[CONVEYOR] == CTL_READY) //kwlee 2017.0320
// 				{
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
// 					m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_FIRST];
// 					m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_FIRST];

					if (m_nSodimmSecondPlace == TRUE)
					{
						m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_THIRD];
						m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_THIRD];
					}
					else
					{
						m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_FIRST];
						m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_FIRST];
					}
				}
			}
			else //m_bSecondPos == true
			{
				m_lTurnConvWaitTime[1] = GetCurrentTime();
				m_lTurnConvWaitTime[2] = m_lTurnConvWaitTime[1] - m_lTurnConvWaitTime[0];

				//if (m_lTurnConvWaitTime[2] > 1000 )
				if (m_lTurnConvWaitTime[2] > 300 )
				{
// 					if (st_sync_info.TurnConvJobReady[CONVEYOR] == CTL_READY)
// 					{
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
// 						m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_SECOND];
// 						m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_SECOND];

						//kwlee 2017.0308
						if (m_nSodimmSecondPlace == TRUE)
						{
							m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_FORTH];
							m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_FORTH];
								
						}
						else
						{
							m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_SECOND];
							m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_SECOND];
						}
					}
				//	}
				}	
			}
			m_lpAxisNum[0] = M_PICKERRBT_Y; //m_lpAxisNum[4];        //현재의 IndexNum에서 사용하는 모터 실재 번호를 가진다 
			m_lpAxisNum[1] = M_PICKERRBT_X;


// 			nRet_1 = CTL_Lib.Linear_Move(ONLY_MOVE_START, m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, COMI.mn_manualspeed_rate);
// 
// 			if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
// 			{
// 				m_nRunStep = 15000;
// 			}
// 			else if(nRet_1 == BD_ERROR)
// 			{
// 				CTL_Lib.Alarm_Error_Occurrence(820, dWARNING, st_alarm_info.strCode);
// 				//m_nRunStep = 0;
// 			}
			//kwlee 2017.0320
			if (st_sync_info.TurnConvJobReady[CONVEYOR] == CTL_READY) //kwlee 2017.0320
			{
				nRet_1 = CTL_Lib.Linear_Move(ONLY_MOVE_START, m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, COMI.mn_manualspeed_rate);

				if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
				{
					//m_nRunStep = 15000;
					//kwlee 2017.0327
					m_nRunStep = 14100;
				}
				else if(nRet_1 == BD_ERROR)
				{
					CTL_Lib.Alarm_Error_Occurrence(820, dWARNING, st_alarm_info.strCode);
					//m_nRunStep = 0;
				}
			}
			break;

		//kwlee 2017.0327
		case 14100:	
			//ROBOT TURN
			if (m_nSodimmSecondPlace == TRUE)
			{
				nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_TURN, st_motor_info[M_PICKERRBT_TURN].d_pos[PITCH_UP_HEAD_TURN_ROTATOR_L_90], COMI.mn_runspeed_rate);

				if (nRet_1 == BD_GOOD) //좌측으로 이동
				{
					//m_nRunStep = 14000;
					//kwlee 2017.0327
					m_nRunStep = 15000;
				}
				else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
				{
					//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
					CTL_Lib.Alarm_Error_Occurrence(810, dWARNING, st_alarm_info.strCode);
					//m_nRunStep = 13000;
				}
			}
			else
			{
				nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_TURN, st_motor_info[M_PICKERRBT_TURN].d_pos[PITCH_UP_HEAD_TURN_ROTATOR_R_90], COMI.mn_runspeed_rate);
				if (nRet_1 == BD_GOOD) //좌측으로 이동
				{
					//m_nRunStep = 14000;
					//kwlee 2017.0327
					m_nRunStep = 15000;
				}
				else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
				{
					//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
					CTL_Lib.Alarm_Error_Occurrence(810, dWARNING, st_alarm_info.strCode);
					//m_nRunStep = 13000;

				}
			}
			break;

		//kwlee 2017.0327
	//case 16000:
	case 15000:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_PITCH, st_motor_info[M_PICKERRBT_PITCH].d_pos[PITCH_UP_HEAD_PLACE_PCB], COMI.mn_manualspeed_rate);

		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			//m_nRunStep = 16100;
			//kwlee 2017.0327
			m_nRunStep = 15100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(840, dWARNING, st_alarm_info.strCode);
			//m_nRunStep = 0;
		}
		break;

	case 15100:
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
		//kwlee 2017.0327
// 	case 16000:	
// 		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_PITCH, st_motor_info[M_PICKERRBT_PITCH].d_pos[PITCH_UP_HEAD_PLACE_PCB], COMI.mn_manualspeed_rate);
// 
// 		if (nRet_1 == BD_GOOD) //좌측으로 이동
// 		{
// 			m_nRunStep = 16100;
// 		}
// 		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
// 		{
// 			//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
// 			CTL_Lib.Alarm_Error_Occurrence(840, dWARNING, st_alarm_info.strCode);
// 			//m_nRunStep = 0;
// 		}
// 		break;

		//////////////////////////////
		//붙이는 작업 
	//case 16100:
		//kwlee 2017.0327
	case 16000:
		OnSetLabelPlace(m_nPickCnt);
		OnSetPickerUpDn(0, PICKER_DN, m_npTemp_Picker_YesNo);
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
		nRet_1 = OnGetPickerUpDn(0, PICKER_DN, m_npTemp_Picker_YesNo);

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
		OnSetPickerUpDn(0, PICKER_UP, m_npTemp_Picker_YesNo);
		m_nRunStep = 21000;
		break;

	case 21000:	
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_Z, st_motor_info[M_PICKERRBT_Z].d_pos[ROBOT_SAFETY], COMI.mn_manualspeed_rate);
		//z 축 up
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			//kwlee 2017.0328
		
		
			////
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
		nRet_1 = OnGetPickerUpDn(0, PICKER_UP, m_npTemp_Picker_YesNo);
		OnDataExchange(NORMAL_MODE, PLACE);
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
				//kwlee 2017.0309
				if (st_basic_info.nPcbType == RDIMM || st_basic_info.nPcbType == UDIMM_10)
				{
					m_nRdimmJobState = CTL_COMPLETE;
				}

				//kwlee 2017.0327
// 				if (st_basic_info.nPcbType == SODIMM && m_nSodimmSecondPlace == FALSE)
// 				{
// 					m_nSodimmSecondPlace = TRUE;
// 				}
// 				else
// 				{
// 					m_nSodimmSecondPlace = FALSE;
// 				}
				m_dwTacTimeCheck[1] = GetCurrentTime();
				m_dwTacTimeCheck[2] = m_dwTacTimeCheck[1] - m_dwTacTimeCheck[0];

				if (m_nSodimmSecondPlace == FALSE)
				{
					str.Format(_T("[FIRST LABEL PLACE TIME] :%.1f"),(double)m_dwTacTimeCheck[2]/1000);
					clsMem.OnNormalMessageWrite(str);
				}
				else
				{
					str.Format(_T("[SECOND LABEL PLACE TIME] :%.1f"),(double)m_dwTacTimeCheck[2]/1000);
					clsMem.OnNormalMessageWrite(str);
				}
				
				m_nRunStep = 23000;
				m_nPickerNum = 0;
				m_bSecondPos = false;
				m_nLinearMove_Index = 0;
				CTL_Lib.m_nStep_Linear[m_nLinearMove_Index] = 0;
			}

			for (int i = 0; i<PICKCNT; i++)
			{
				m_npTemp_Picker_YesNo[i] = CTL_NO;	
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
		//kwlee 2017.0315
		//nRet_1 = CTL_Lib.Linear_Move(BOTH_MOVE_FINISH, m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, COMI.mn_runspeed_rate);

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
		
		
	//case 23200:	
		//kwlee 2017.0327
		case 23100:	
		//ROBOT TURN
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_TURN, st_motor_info[M_PICKERRBT_TURN].d_pos[PITCH_UP_HEAD_TURN_ROTATOR_0], COMI.mn_runspeed_rate);

		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			//m_nRunStep = 23300;
			//kwlee 2017.0327
			m_nRunStep = 23200;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			CTL_Lib.Alarm_Error_Occurrence(630, dWARNING, st_alarm_info.strCode);
		}
		break;

	case 23200:	
		////ROBOT PITCH 작업 위치 이동
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_PITCH, st_motor_info[M_PICKERRBT_PITCH].d_pos[PITCH_UP_HEAD_PICK_FEEDER], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			//m_nRunStep = 1000;
			//kwlee 2017.0328
// 			if (st_basic_info.nPcbType == SODIMM && m_nSodimmSecondPlace == FALSE)
// 			{
// 				m_nSodimmSecondPlace = TRUE;
// 			}
// 			else
// 			{
// 				m_nSodimmSecondPlace = FALSE;
// 			}

			m_nRunStep = 23300;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(640, dWARNING, st_alarm_info.strCode);
		}
		break;

		///////
	case 23300:
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
						st_sync_info.TurnConvJobReady[ROBOT] = CTL_NONE; //kwlee 2017.0318
						m_nRunStep = 14000;
					}
				}
			}
			else
			{

				if (st_basic_info.nPcbType == SODIMM && m_nSodimmSecondPlace == FALSE)
				{
					m_nSodimmSecondPlace = TRUE;
					//nPosCheck = FRONT;
				}
				else
				{
					m_nSodimmSecondPlace = FALSE;
				//	nPosCheck = REAR;
				}

				m_nRunStep = 1000;

				//kwlee 2017.0328	
// 				nRet_1 = OnVisionDataCheck(nPosCheck);
// 				if (nRet_1 == CTL_GOOD)
// 				{
// 					//m_nRunStep = 23200;
// 					//kwlee 2017.0327
// 					m_nRunStep = 1000;
// 				}
// 				else if (nRet_1 == CTL_ERROR)
// 				{
// 					//str.Format(_T("Vision Data Fail"));
// 					//AfxMessageBox(str);
// 					//m_nRunStep = 23200;
// 					//kwlee 2017.0327
// 					m_nRunStep = 1000;
// 				}
			}

			for (int i = 0; i<PICKCNT; i++)
			{
				m_npTemp_Picker_YesNo[i] = NO;
				FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[i],IO_OFF);
				FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[i],IO_OFF);
			}
			OnSetPickerUpDn(0, PICKER_UP, m_npTemp_Picker_YesNo);

			st_sync_info.TurnConvJobReady[ROBOT] = CTL_READY;
		}
		else if(nRet == BD_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(900, dWARNING, st_alarm_info.strCode);
			m_nRunStep = 23000;
		}
		break;
		 //kwlee 2017.0318
// 	case 23200:	
// 		//ROBOT TURN
// 		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_TURN, st_motor_info[M_PICKERRBT_TURN].d_pos[PITCH_UP_HEAD_TURN_ROTATOR_0], COMI.mn_runspeed_rate);
// 
// 		if (nRet_1 == BD_GOOD) //좌측으로 이동
// 		{
// 			m_nRunStep = 23300;
// 		}
// 		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
// 		{
// 			CTL_Lib.Alarm_Error_Occurrence(630, dWARNING, st_alarm_info.strCode);
// 		}
// 		break;

// 	case 23300:	
// 		////ROBOT PITCH 작업 위치 이동
// 		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_PITCH, st_motor_info[M_PICKERRBT_PITCH].d_pos[PITCH_UP_HEAD_PICK_FEEDER], COMI.mn_runspeed_rate);
// 		if (nRet_1 == BD_GOOD) //좌측으로 이동
// 		{
// 			m_nRunStep = 1000;
// 			
// 		}
// 		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
// 		{
// 			//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
// 			CTL_Lib.Alarm_Error_Occurrence(640, dWARNING, st_alarm_info.strCode);
// 		}
// 		break;
	}
	/*return RET_PROCEED;*/
}


int CRunRobot::OnBcrConveyorOnOff(int nOnOff)
{
	int nFuncRet = RET_PROCEED;
	switch(m_nConvStep)
	{
	case 0:
		FAS_IO.set_out_bit(st_io_info.o_LabelFeederMotorOn, nOnOff);
		m_dwBcrConvMotorWaitTime[0] = GetTickCount();
		m_nConvStep = 100;
		break;

	case 100:
		m_dwBcrConvMotorWaitTime[1] = GetTickCount();
		m_dwBcrConvMotorWaitTime[2] = m_dwBcrConvMotorWaitTime[1] - m_dwBcrConvMotorWaitTime[0];
		if( m_dwBcrConvMotorWaitTime[2] <= 0 ) m_dwBcrConvMotorWaitTime[0] = GetTickCount();
		if( m_dwBcrConvMotorWaitTime[2] > 2000 )
		{
			m_nConvStep = 200;
		}
		break;

	case 200:
		nFuncRet = RET_GOOD;
		m_nConvStep = 0;
		break;

	}

	return nFuncRet;
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
//	int nRet;
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

//kwlee 2017.0307
void CRunRobot::OnSetFeederClampCylUpDn(int nMode, int nOnOff)
{
	if (nMode > 1) return;

	m_bCylFlag[nMode]			= false;
	m_dwClampUpDnWaitTime[nMode][0]	= GetCurrentTime();

	if (nOnOff == IO_ON)
	{
		FAS_IO.set_out_bit(st_io_info.o_LabelFeederClmpOn, nOnOff); //S0105 Up
		FAS_IO.set_out_bit(st_io_info.o_LabelFeederClmpOff, !nOnOff);

	}
	else
	{
		FAS_IO.set_out_bit(st_io_info.o_LabelFeederClmpOn, nOnOff); //S0106 Dw
		FAS_IO.set_out_bit(st_io_info.o_LabelFeederClmpOff, !nOnOff);
	}


}

int CRunRobot::OnGetFeederClampCylUpDn(int nMode, int nOnOff)
{
	int nWaitTime = WAIT_PICKER_UP_DN;
	int nRet;
	if (nOnOff == IO_OFF)
	{
		nRet = FAS_IO.get_in_bit(st_io_info.i_LabelFeederStopperChk1,	IO_ON);

		if (m_bCylFlag[nMode] == false &&
			FAS_IO.get_in_bit(st_io_info.i_LabelFeederStopperChk1,	IO_ON)	== IO_OFF )
		{
			m_bCylFlag[nMode]		= true;
			m_dwClampUpDnWaitTime[nMode][0]	= GetCurrentTime();
		}
		else if (m_bCylFlag[nMode] == true &&
			FAS_IO.get_in_bit(st_io_info.i_LabelFeederStopperChk1,	IO_ON)	== IO_OFF )
		{
			m_dwClampUpDnWaitTime[nMode][1] = GetCurrentTime();
			m_dwClampUpDnWaitTime[nMode][2] = m_dwClampUpDnWaitTime[nMode][1] - m_dwClampUpDnWaitTime[nMode][0];

			if (m_dwClampUpDnWaitTime[nMode][2] <= 0)
			{
				m_dwClampUpDnWaitTime[nMode][0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwClampUpDnWaitTime[nMode][2] > (DWORD)st_wait_info.nOnWaitTime[nWaitTime])
			{
				return RET_GOOD;
			}
		}
		else
		{
			m_dwClampUpDnWaitTime[nMode][1] = GetCurrentTime();
			m_dwClampUpDnWaitTime[nMode][2] = m_dwClampUpDnWaitTime[nMode][1] - m_dwClampUpDnWaitTime[nMode][0];

			if (m_dwClampUpDnWaitTime[nMode][2] <= 0)
			{
				m_dwClampUpDnWaitTime[nMode][0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwClampUpDnWaitTime[nMode][2] > (DWORD)st_wait_info.nLimitWaitTime[nWaitTime])
			{
				//502020  "OUT CONV Position Stopper Up-Sol Error."
				m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_LabelFeederStopperChk1, nOnOff); 
				//CTL_Lib.Alarm_Error_Occurrence(304, dWARNING, strAlarm);

				return RET_ERROR;
			}
		}
	}
	else
	{
		if (m_bCylFlag[nMode] == false &&
			FAS_IO.get_in_bit(st_io_info.i_LabelFeederStopperChk1, IO_ON) == IO_ON )
		{
			m_bCylFlag[nMode]			= true;
			m_dwClampUpDnWaitTime[nMode][0]	= GetCurrentTime();
		}
		else if (m_bCylFlag[nMode] == true &&
			FAS_IO.get_in_bit(st_io_info.i_LabelFeederStopperChk1,	IO_ON)	== IO_ON )
		{
			m_dwClampUpDnWaitTime[nMode][1]	= GetCurrentTime();
			m_dwClampUpDnWaitTime[nMode][2]	= m_dwClampUpDnWaitTime[nMode][1] - m_dwClampUpDnWaitTime[nMode][0];

			if (m_dwClampUpDnWaitTime[nMode][2] <= 0)
			{
				m_dwClampUpDnWaitTime[nMode][0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if(m_dwClampUpDnWaitTime[nMode][2] > (DWORD)st_wait_info.nOnWaitTime[nWaitTime])
			{
				return RET_GOOD;
			}
		}
		else
		{
			m_dwClampUpDnWaitTime[nMode][1]	= GetCurrentTime();
			m_dwClampUpDnWaitTime[nMode][2]	= m_dwClampUpDnWaitTime[nMode][1] - m_dwClampUpDnWaitTime[nMode][0];

			if (m_dwClampUpDnWaitTime[nMode][2] <= 0)
			{
				m_dwClampUpDnWaitTime[nMode][0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwClampUpDnWaitTime[nMode][2] > (DWORD)st_wait_info.nLimitWaitTime[nWaitTime])
			{
				//502020  "OUT CONV Position Stopper Up-Sol Error."
				m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_LabelFeederStopperChk1, nOnOff); 
				//CTL_Lib.Alarm_Error_Occurrence(305, dWARNING, strAlarm);

				return RET_ERROR;
			}
		}
	}

	return RET_PROCEED;
}