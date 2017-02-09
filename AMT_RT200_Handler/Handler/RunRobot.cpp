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
	m_nInterFaceStep = -1;


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
		//OnFeederInterface();
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
				m_nInitStep = 300;
			}
			else if(nRet_1 == RET_ERROR)
			{
				m_nInitStep = 0;

				CTL_Lib.Alarm_Error_Occurrence(101, dWARNING, m_strAlarmCode);
			}
			break;

		case 300:
			//picker vaccum off
			for (int i = 0; i < PICKCNT; i++)
			{
				FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[i],IO_OFF);

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
				else
				{ 
					if (i< PICKCNT -1)
					{
						continue;
					}						
					m_nInitStep = 1000;								
				}
			}
			break;
	
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

		case 4000:
			nRet_1 = COMI.HomeCheck_Mot(M_PICKERRBT_TURN, MOT_SENS_ELM, MOT_TIMEOUT) ; 

			if(nRet_1 == BD_GOOD)
			{
				st_sync_info.nInitRbt[M_PICKERRBT_TURN] = CTL_YES;
				m_nInitStep = 5000;
			}
			else if(nRet_1 == BD_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(150, dWARNING, _T("060001"));
			}
			break;

		case 5000:		
			nRet_1 = COMI.HomeCheck_Mot(M_PICKERRBT_X, MOT_SENS_ELM, MOT_TIMEOUT) ;
			if(nRet_1 == BD_GOOD)
			{
				st_sync_info.nInitRbt[M_PICKERRBT_X] = CTL_YES;
				m_nInitStep = 6000;
			}
			else if(nRet_1 == BD_ERROR)
			{//010001 0 A "Motor Home Check Error - (Robot X)."
				CTL_Lib.Alarm_Error_Occurrence(160, dWARNING, _T("010001"));
			}
			break;

		case 6000:
			nRet_1 = COMI.HomeCheck_Mot(M_PICKERRBT_Y, MOT_SENS_ELM, MOT_TIMEOUT) ;
			if(nRet_1 == BD_GOOD)
			{
				st_sync_info.nInitRbt[M_PICKERRBT_Y] = CTL_YES;
				m_nInitStep = 7000;
			}
			else if(nRet_1 == BD_ERROR)
			{//000001 0 A "Motor Home Check Error - (Robot Y)."
				CTL_Lib.Alarm_Error_Occurrence(170, dWARNING, _T("000001"));
				m_nInitStep = 0;
			}
			break;

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
				m_nInitStep = 8000;
			}
			else if(nRet_1 == BD_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(190, dWARNING, st_alarm_info.strCode);
				m_nInitStep = 0;
			}
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
void CRunRobot::OnSetPickerUpDn(int  nPickCnt)
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
// 					if (st_basic_info.nPickerSelect[i] == CTL_YES)
// 					{
					clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_YES;
					//}
				}
			}
// 			else
// 			{
// 				if (i < 6)
// 				{
// 					if (st_basic_info.nPickerSelect[i] == CTL_YES)
// 					{
// 						clsRunRobot.m_npTemp_Picker_YesNo[i] = CTL_YES;
// 					}
// 				}
// 			}
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
					clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT + 4 - i] = CTL_YES;	
					//}
				}
			}
		}	
	}
	//clsRunRobot.OnSetPickerUpDn(0, PICKER_DN, clsRunRobot.m_npTemp_Picker_YesNo);
}
void CRunRobot::OnBufferDataTransfer()
{
	for (int i =0; i<2; i++)
	{
		for (int j =0; j<MAX_BUFFER; j++)
		{
			for (int k =0; j<MAX_INFO; k++)
			{
				if (st_Buffer_info[PICK].strBufferSerial[i][MAX_PICKER + j] != _T(""))
				{
					st_Buffer_info[TEMP].nBufferData[i][j][k] =  st_Buffer_info[PICK].nBufferData[i][m_nPrintOutPutCnt + j][k];
					st_Buffer_info[TEMP].strBufferSerial[i][j] =  st_Buffer_info[PICK].strBufferSerial[i][m_nPrintOutPutCnt + j];

					st_Buffer_info[PICK].nBufferData[i][j][k] = st_Buffer_info[TEMP].nBufferData[i][j][k];
					st_Buffer_info[PICK].strBufferSerial[i][j] = st_Buffer_info[TEMP].strBufferSerial[i][j];
					m_nEmptyCntBuffer++;
				}
				else
				{
					break;
				}
			}
		}
	}
}
void CRunRobot::OnDataExchange(int nPickPlace,int nFailCheck)
{
	if (nPickPlace == PICK)
	{
		for (int i =0; i<2; i++)
		{
			for (int j = 0; j< MAX_PICKER; j++)
			{
				for (int k =0; k<MAX_INFO; k++)
				{
					st_Picker_info.nPickerData[i][j][k] = st_Buffer_info[PICK].nBufferData[i][j][k];
					st_Picker_info.strPickerSerial[i][j] = st_Buffer_info[PICK].strBufferSerial[i][j];
					//st_Picker_info.nPickerFailCnt        = st_Buffer_info.nBufferFailCnt;

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
			}
		}
	}
	else if (nPickPlace == PLACE)
	{
		for (int i =0; i<2; i++)
		{
			for (int j = 0; j< MAX_PICKER; j++)
			{
				for (int k =0; k<MAX_INFO; k++)
				{
					st_Buffer_info[PLACE].nBufferData[i][j][k]  = st_Picker_info.nPickerData[i][j][k] ;
					st_Buffer_info[PLACE].strBufferSerial[i][j] = st_Picker_info.strPickerSerial[i][j] ;
					//st_Picker_info.nPickerFailCnt        = st_Buffer_info.nBufferFailCnt;

					// 					if (st_Picker_info.nPickerData[i][j][k] == st_Buffer_info[PICK].nBufferData[i][j][k] && 
					// 						st_Picker_info.strPickerSerial[i][j] == st_Buffer_info[PICK].strBufferSerial[i][j])
					if (st_Buffer_info[PLACE].nBufferData[i][j][k]  ==  st_Picker_info.nPickerData[i][j][k] && 
						 st_Buffer_info[PLACE].strBufferSerial[i][j] == st_Picker_info.strPickerSerial[i][j]
						)
						//m_nLabelFailCheck == FALSE
						
					{
						st_Picker_info.nPickerData[i][j][k]  = -1; 
						st_Picker_info.strPickerSerial[i][j] = _T("");

						//st_Buffer_info[PICK].nBufferData[i][j][k] = -1;
						//st_Buffer_info[PICK].strBufferSerial[i][j] = _T("");
						//st_Buffer_info.nBufferFailCnt = 0;	
					}
				}
			}
		}
	}
	else
	{

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
							m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_HeadVaccumCheck[i], OnOff);
							return RET_ERROR;
						}

					}
					else if (st_Buffer_info[PICK].nBufferData[0][i][FAILPICK] == YES)
					{
						nRet = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[i],IO_ON);
						if (nRet == IO_ON)
						{
							st_Buffer_info[PICK].nBufferData[0][i][FAILPICK] = NO;		
						}
					}
					// 					else
					// 					{
					// 						nRet = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[i],IO_OFF);
					// 
					// 						if (nRet == IO_ON)
					// 						{
					// 							return RET_ERROR;
					// 						}
					// 						else
					// 						{
					// 							return RET_GOOD;
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
					if (st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][EXIST] == YES && st_Buffer_info[PICK].strBufferSerial[1][nPickCnt - i]!= _T(""))
					{
						nRet = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[PICKCNT + 3 - i],IO_ON);
						if (nRet == IO_OFF)
						{
							st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][BIN] = FAIL;
							st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][EXIST] = NO;
							m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_HeadVaccumCheck[PICKCNT + 3 - i], OnOff);
							return RET_ERROR;
						}	
					}
					else if (st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][FAILPICK] == YES)
					{
						nRet = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[PICKCNT + 3 - i],IO_ON);
						if (nRet == IO_ON)
						{
							st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][FAILPICK] = NO;
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
						if (st_Buffer_info[PICK].nBufferData[0][i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[0][i][EXIST] == YES && st_Buffer_info[PICK].strBufferSerial[0][i] != _T(""))
						{
							nRet = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[i],IO_ON);
							if (nRet == IO_OFF)
							{
								st_Buffer_info[PICK].nBufferData[0][i][BIN] = FAIL;
								st_Buffer_info[PICK].nBufferData[0][i][EXIST] = NO;
								m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_HeadVaccumCheck[i], OnOff);
								return RET_ERROR;
							}

						}
						else if (st_Buffer_info[PICK].nBufferData[0][i][FAILPICK] == YES)
						{
							//FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[i],IO_OFF);
							nRet = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[i],IO_ON);
							if (nRet == IO_ON)
							{
								st_Buffer_info[PICK].nBufferData[0][i][FAILPICK] = NO;
							}
						}
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
								m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_HeadVaccumCheck[PICKCNT - i - 1], OnOff);
								return RET_ERROR;
							}

						}
						else if (st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][FAILPICK] == YES)
						{
							//FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[PICKCNT - i],IO_OFF);
							nRet = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[PICKCNT - i - 1],IO_ON);
							if (nRet == IO_ON)
							{
								st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][FAILPICK] = NO;
							}
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
						if (st_Buffer_info[PICK].nBufferData[0][i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[0][i][EXIST] == YES && st_Buffer_info[PICK].strBufferSerial[0][i]!= _T(""))
						{
							nRet = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[i],IO_ON);	
							if (nRet == IO_OFF)
							{
								st_Buffer_info[PICK].nBufferData[0][i][BIN] = FAIL;
								st_Buffer_info[PICK].nBufferData[0][i][EXIST] = NO;
								m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_HeadVaccumCheck[i], OnOff);
								return RET_ERROR;
							}
						}
						else if (st_Buffer_info[PICK].nBufferData[0][i][FAILPICK] == YES)
						{
							nRet = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[i],IO_ON);
							if (nRet == IO_ON)
							{
								st_Buffer_info[PICK].nBufferData[0][i][FAILPICK] = NO;
							}
						}
					}
					else
					{
						//FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[PICKCNT + 3 - i],IO_ON);
						//kwlee 2017.0204
						//if (st_Buffer_info.nBufferData[1][nPickCnt - i][BIN] == GOOD)
						if (st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][EXIST] == YES && st_Buffer_info[PICK].strBufferSerial[1][nPickCnt - i] !=_T(""))
						{
							nRet = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[PICKCNT + 3 - i],IO_ON);
							if (nRet == IO_OFF)
							{
								st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][BIN] = FAIL;
								st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][EXIST] = NO;
								m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_HeadVaccumCheck[PICKCNT + 3 - i], OnOff);
								return RET_ERROR;
							}
						}
						else if (st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][FAILPICK] == YES)
						{
							//FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[PICKCNT + 3 - i],IO_OFF);
							nRet = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[PICKCNT + 3 - i],IO_ON);
							if (nRet == IO_ON)
							{
								st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][FAILPICK] = NO;
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
	for (i =0; i<nPickCnt; i++)
	{
		if (nMode == 1)
		{
			//if ((st_Picker_info.nPickerData[0][i][X_POS] == LEFT && st_Picker_info.nPickerData[0][i][FAILPICK] == YES) || 
				//(st_Picker_info.nPickerData[1][nPickCnt - i][X_POS] == RIGHT && st_Picker_info.nPickerData[1][nPickCnt - i][FAILPICK] == YES))
			//{
				if (i < 5 && st_Buffer_info[PICK].nBufferData[0][i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[0][i][EXIST] == YES)
				{
					//if (st_Buffer_info[PICK].nBufferData[0][i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[0][i][EXIST] == YES)
					//if (st_Buffer_info[PICK].nBufferData[0][i][BIN] == FAIL && st_Buffer_info[PICK].nBufferData[0][i][EXIST] == NO)
					if (st_Picker_info.nPickerData[0][i][BIN] == FAIL && st_Picker_info.nPickerData[0][i][EXIST] == NO)
					{
						clsRunRobot.m_npTemp_Picker_YesNo[st_Picker_info.nPickerData[0][i][Y_POS]] = CTL_YES;
					}
					
					//st_Picker_info.nPickerData[0][st_Picker_info.nPickerData[0][i][Y_POS]][FAILPICK] = NO;
				//	break;
				}
				else if (i > 4 && st_Buffer_info[PICK].nBufferData[1][i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[1][i][EXIST] == YES)
				{
					//if (st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][EXIST] == YES)
					//if (st_Buffer_info[PICK].nBufferData[1][i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[1][i][EXIST] == YES)
					if (st_Picker_info.nPickerData[1][nPickCnt - i][BIN] == FAIL && st_Picker_info.nPickerData[1][nPickCnt - i][EXIST] == NO)
					{	
						clsRunRobot.m_npTemp_Picker_YesNo[MAX_PICKER + st_Picker_info.nPickerData[1][nPickCnt - i][Y_POS]] = CTL_YES;
					}
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
void CRunRobot::OnBlowSet(int OnOff)
{

	if (OnOff == IO_ON)
	{
		for (int i =0; i<m_nPickCnt; i++ )
		{
			if(m_bSecondPos == false)
			{
				if(st_basic_info.nPcbType == RDIMM)
				{
					if (i < 3)
					{
// 						if (st_basic_info.nPickerSelect[i] == CTL_YES)
// 						{

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
						FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[PICKCNT + 4 - i],IO_ON);
						//}
					}
				}
			}
// 			if (i< m_nPickCnt - 1)
// 			{
// 				continue;
// 			}

		}
	}
	else //OnOff == IO_OFF
	{
		for (int i =0; i<m_nPickCnt; i++ )
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
						FAS_IO.set_out_bit(st_io_info.o_HeadVaccumOff[PICKCNT + 4 - i],IO_OFF);
						//}
						//clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT - i ] = CTL_YES;	
					}
				}
			}
			//if (i< PICKCNT -1)
// 			if (i< m_nPickCnt - 1)
// 			{
// 				continue;
// 			}
		}	
	}
}

int CRunRobot::OnBarcodeReadCheck()
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
				nCnt++;
			}
		}
	}

	if (nCnt > 0)
	{
		st_Buffer_info[PICK].nBufferFailCnt = nCnt;
		nRet = BD_ERROR;
	}
	else
	{
		nRet = BD_GOOD;
	}
	return nRet;
}
//kwlee 2017.0202
int CRunRobot::OnPrinterFeeder(int nCnt, int nFailCheck)
{

	CString strMsg;

// 	if (FAS_IO.get_in_bit(st_io_info.i_LfAlarmChk,IO_ON) == IO_ON)
// 	{	
// 		//507151 1 A "Label Feeder LF_ALARM On Check Error."
// 		m_strAlarmCode.Format(_T("507151"));
// 		CTL_Lib.Alarm_Error_Occurrence(404, dWARNING, m_strAlarmCode);
// 		return RET_ERROR;
// 	}
	//Printer Alarm 시 알람
	///
	
	switch(m_nFeederStep)
	{
		case 0:

			m_nFeederStep = 100;
		break;

		case 100:
			m_nFeederStep = 200;
		break;

		case 200:
			m_nFeederStep = 300;
			break;

	}
	

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
					(st_Picker_info.nPickerData[1][i][X_POS] == RIGHT && st_Picker_info.nPickerData[1][nPickCnt - i][FAILPICK] == YES))
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
					else if (i > 4 && st_Picker_info.nPickerData[1][i][X_POS] == RIGHT)
					{
						//if (st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[1][nPickCnt - i][EXIST] == YES)
						//if (st_Buffer_info[PICK].nBufferData[1][i][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[1][i][EXIST] == YES)
						if (st_Picker_info.nPickerData[1][i][BIN] == FAIL && st_Picker_info.nPickerData[1][i][EXIST] == NO)
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
						FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[PICKCNT + 4 - i],IO_OFF);
						//}	
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
	if (FAS_IO.get_in_bit(st_io_info.i_LfAlarmChk,IO_ON) == IO_ON)
	{	
		//507151 1 A "Label Feeder LF_ALARM On Check Error."
		m_strAlarmCode.Format(_T("507151"));
		CTL_Lib.Alarm_Error_Occurrence(404, dWARNING, m_strAlarmCode);
		return RET_ERROR;
	}

	switch(m_nInterFaceStep)
	{
	case COM_CLEAR:
		FAS_IO.set_out_bit(st_io_info.o_PcComplete,IO_ON);

		if (FAS_IO.get_in_bit(st_io_info.i_LfCompChk,IO_ON) == IO_OFF)
		{
			FAS_IO.set_out_bit(st_io_info.o_PcComplete,IO_OFF);
			FAS_IO.set_out_bit(st_io_info.o_PcReady,IO_OFF);
		}
		m_nInterFaceStep = COM_NONE;
		break;

	case COM_START:
		nRet = FAS_IO.get_in_bit(st_io_info.i_LfReqChk,IO_ON);

		if(nRet == IO_OFF)
		{
			//507130 1 A "Label Feeder LF_REQUEST Off Check Error."
			m_strAlarmCode.Format(_T("507130")); 
			CTL_Lib.Alarm_Error_Occurrence(404, dWARNING, m_strAlarmCode);
		}
		else
		{
			FAS_IO.set_out_bit(st_io_info.o_PcComplete,IO_OFF);
			FAS_IO.set_out_bit(st_io_info.o_PcReady,IO_OFF);
			m_nInterFaceStep = COM_READY;
		}
		break;

	case COM_READY:
		FAS_IO.set_out_bit(st_io_info.o_PcReady,IO_ON);
		m_lFeederWaitTime[0] = GetCurrentTime();
		break;

	case COM_READY_CHK:
		
		m_lFeederWaitTime[1] = GetCurrentTime();
		m_lFeederWaitTime[2] = m_lFeederWaitTime[1] - m_lFeederWaitTime[0];

		if (m_lFeederWaitTime[2] > 500)
		{
			nRet = FAS_IO.get_in_bit(st_io_info.i_LfCompChk,IO_ON);
			if (nRet == IO_ON)
			{
				m_lFeederWaitTime[0] = GetCurrentTime();
				FAS_IO.set_out_bit(st_io_info.o_PcReady,IO_OFF);
				m_nInterFaceStep = COM_COMPLETE;
			}
			else
			{
				m_lFeederWaitTime[1] = GetCurrentTime();
				m_lFeederWaitTime[2] = m_lFeederWaitTime[1] - m_lFeederWaitTime[0];

				if (m_lFeederWaitTime[2]> st_wait_info.dLimitWaitTime[WAIT_FEEDER_REPLY])
				{
					//507141 1 A "Label Feeder LF_COMPLETE On Check Error."
					m_strAlarmCode.Format(_T("507141")); 
					CTL_Lib.Alarm_Error_Occurrence(404, dWARNING, m_strAlarmCode);
				}
			}
		}
		break;

	case COM_COMPLETE:
		m_lFeederWaitTime[1] = GetCurrentTime();
		m_lFeederWaitTime[2] = m_lFeederWaitTime[1] - m_lFeederWaitTime[0];
		if (m_lFeederWaitTime[2] < 0) m_lFeederWaitTime[0] = GetCurrentTime();
		
		if (m_lFeederWaitTime[2] > 100)
		{
			FAS_IO.set_out_bit(st_io_info.o_PcComplete,IO_ON);
			m_nInterFaceStep = COM_COMPLETE_CHK;
		}
		break;

	case COM_COMPLETE_CHK:
		nRet = FAS_IO.get_in_bit(st_io_info.i_LfCompChk,IO_OFF);

		if (nRet == IO_OFF)
		{
			FAS_IO.set_out_bit(st_io_info.o_PcComplete,IO_OFF);
		}
		else
		{
			m_lFeederWaitTime[1] = GetCurrentTime();
			m_lFeederWaitTime[2] = m_lFeederWaitTime[1] - m_lFeederWaitTime[0];

			if (m_lFeederWaitTime[2]> st_wait_info.dLimitWaitTime[WAIT_FEEDER_REPLY])
			{
				//507140 1 A "Label Feeder LF_COMPLETE Off Check Error."
				m_strAlarmCode.Format(_T("507140")); 
				CTL_Lib.Alarm_Error_Occurrence(404, dWARNING, m_strAlarmCode);
			}
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

					if (m_dwPickUpDnWaitTime[nMode][2] > (DWORD)st_wait_info.dOnWaitTime[WAIT_PICKER_UP_DN])
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

					if (m_dwPickUpDnWaitTime[nMode][2] > (DWORD)st_wait_info.dLimitWaitTime[WAIT_PICKER_UP_DN])
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

					if(m_dwPickUpDnWaitTime[nMode][2] > (DWORD)st_wait_info.dOnWaitTime[WAIT_PICKER_UP_DN])
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

					if (m_dwPickUpDnWaitTime[nMode][2] > (DWORD)st_wait_info.dLimitWaitTime[WAIT_PICKER_UP_DN])
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

	int nRet_1, nRet_2=0, nRet_3=0, nRect[12]={0,},nCnt;
	int nRet;
	double dCurrentPos = 0;
	CString str;
	//m_nRunStep = 0;
	
	m_dPitch_Y = (st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST] - st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_END])/5;
	m_dPitch_x = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_FIRST] - st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_END];

	m_lpAxisNum[0] = M_PICKERRBT_Y;
	m_lpAxisNum[1] = M_PICKERRBT_X;

	if (st_basic_info.nPcbType == SODIMM)
	{
		m_nPickCnt = st_basic_info.nColCnt * (st_basic_info.nRowCnt/2);
	}
	else
	{
		m_nPickCnt = st_basic_info.nColCnt * st_basic_info.nRowCnt;
	}
	//m_nPickCnt = st_basic_info.nPcbCnt;
	if (FAS_IO.get_in_bit(st_io_info.i_LfAlarmChk,IO_ON) == IO_ON)
	{	
		//507151 1 A "Label Feeder LF_ALARM On Check Error."
		m_strAlarmCode.Format(_T("507151"));
		CTL_Lib.Alarm_Error_Occurrence(600, dWARNING, m_strAlarmCode);
	//	m_nRunStep = 0;

		return;
	}
// 	if(st_sync_info.TurnConvJobReady[CONVEYOR] == CTL_COMPLETE)
// 		st_sync_info.TurnConvJobReady[ROBOT] = CTL_NONE;

	switch (m_nRunStep)
	{
	case 0:
		//picker vaccum off
		for (int i = 0; i < PICKCNT; i++)
		{
			FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[i],IO_OFF);

			//if (nRect[i] == IO_ON)
			//vaccum 확인 
			if (FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[i],IO_OFF) == IO_ON)
			{
				//505080 1 A "Head Vaccum #1 Off Check Error ."
				//505081 1 A "Head Vaccum #1 On Check Error ."
				m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_HeadVaccumCheck[i], IO_ON);
				CTL_Lib.Alarm_Error_Occurrence(610, dWARNING, m_strAlarmCode);
				m_nRunStep = 0;
				break;
			}
			else
			{ 
				if (i< PICKCNT -1)
				{
					continue;
				}						
				m_nRunStep = 100;								
			}
		}
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
			CTL_Lib.Alarm_Error_Occurrence(620, dWARNING, st_alarm_info.strCode);
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
			m_nRunStep = 1000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(640, dWARNING, st_alarm_info.strCode);
			m_nRunStep = 400;
		}
		break;
		/////////////////////////////////////////////////////////////////////////////////////////

	case 1000:
		// picker up 체크
		nRet_1 = clsRunRobot.OnGetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);

		if(nRet_1 == RET_GOOD)
		{
			//m_nRunStep = 2000;
			//kwlee 2017.0204
			m_nRunStep = 1100;
		}
		else if(nRet_1 == RET_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(650, dWARNING, m_strAlarmCode);
		}
		break;

		//kwlee 2017.0204
	case 1100:
		//kwlee 2017.0116
		m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_REJECT];
		m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_REJECT];
		////

		//label pick 위치 이동
		nRet_1 = CTL_Lib.Linear_Move(ONLY_MOVE_START, m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, COMI.mn_runspeed_rate);

		if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
		{
			//m_nRunStep = 3000;
			//kwlee 2017.0204
			m_nRunStep = 1200;
		}
		else if(nRet_1 == BD_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(660, dWARNING, st_alarm_info.strCode);
			//	m_nRunStep = 0;
		}
		break;

	case 1200:
		//label pick 위치 확인
		nRet = CTL_Lib.Linear_Move(ONLY_MOVE_CHECK, m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, COMI.mn_runspeed_rate);
		if(nRet == BD_GOOD) //정상적으로 완료된 상태 
		{						
			//m_nRunStep = 4000;
			m_nRunStep = 1300;
		}
		else if (nRet == BD_RETRY)
		{
			m_nRunStep = 1100;
		}
		else if (nRet == BD_ERROR || nRet == BD_SAFETY)
		{			
			m_nRunStep = 1100;
			CTL_Lib.Alarm_Error_Occurrence(670, dWARNING, st_alarm_info.strCode);
		}
		break;

	case 1300:	
		//함수호출
		nRet_1 = COMI.Check_MotPosRange(M_PICKERRBT_X,st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_REJECT],st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_REJECT] + st_motor_info[M_PICKERRBT_X].d_allow);
		nRet_2 = COMI.Check_MotPosRange(M_PICKERRBT_Y, st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_REJECT],st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_REJECT] + st_motor_info[M_PICKERRBT_Y].d_allow);

		if (nRet_1 == BD_GOOD && nRet_2 == BD_GOOD)
		{
			nRet_3 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_Z, st_motor_info[M_PICKERRBT_Z].d_pos[ROBOT_REJECT_PLACE], COMI.mn_runspeed_rate);

			if (nRet_3 == BD_GOOD) //좌측으로 이동
			{
				m_nRunStep = 1400;
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
		/////
	case 1400:	
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_Z, st_motor_info[M_PICKERRBT_Z].d_pos[ROBOT_SAFETY], COMI.mn_runspeed_rate);
		//z 축 up
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			m_nRunStep = 2000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(700, dWARNING, st_alarm_info.strCode);
		}
		break;

	case 2000:
		//kwlee 2017.0116
		m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST];
		m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_FIRST];
		////

		//label pick 위치 이동
		nRet_1 = CTL_Lib.Linear_Move(ONLY_MOVE_START, m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, COMI.mn_runspeed_rate);

		if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
		{
			m_nRunStep = 3000;
		}
		else if(nRet_1 == BD_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(710, dWARNING, st_alarm_info.strCode);
		//	m_nRunStep = 0;
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
// 		if( m_nLabelFailCheck == TRUE)
// 		{
			//m_nRunStep = 3200;
		//kwlee 2017.0204 
		if ( m_nLabelFailCheck == TRUE)
		{
			if (st_Picker_info.nPickerFailCnt == 1)
			{

			}
			else
			{
				OnBufferDataTransfer();
			}
		}
		nRet = OnPrinterFeeder(m_nPrintOutPutCnt,m_nLabelFailCheck);
		if (nRet == RET_GOOD)
		{
			m_nPrintOutPutCnt++;
			//st_Buffer_info[PICK].nOutPutCnt = m_nPrintOutPutCnt;
		}
		m_nRunStep = 3200;
		break;

		//kwlee 2017.0119 Barcode Read
		//1. 바코드 Read
		//2. 양품인지 확인.
	case 3200:
		//if (FAS_IO.get_in_bit(st_io_info.i_LabelFeederProductChk1,IO_ON) && m_nPrintOutPutCnt >= 16)
		//kwlee 2017.0204
		if ((m_nLabelFailCheck == TRUE && st_Picker_info.nPickerFailCnt == 1) || 
			(FAS_IO.get_in_bit(st_io_info.i_LabelFeederProductChk1,IO_ON) && m_nPrintOutPutCnt >= 16))
		{	
			m_dwTimeCheck[0] = GetCurrentTime();
			//st_Buffer_info[PICK].nBufferFailCnt = 0;
			m_nRunStep = 4000;
		}
		else
		{
			if (m_nPrintOutPutCnt > 3) //Barcode Read 위치 확인.
			{
				::PostMessage(st_handler_info.hWnd, WM_BARCODE_MSG, BARCODE_TRIGGER_1, 0);
				::PostMessage(st_handler_info.hWnd, WM_BARCODE_MSG, BARCODE_TRIGGER_2, 0);
			}
			m_nRunStep = 3100;
		}
		break;

	case 4000:
		m_dwTimeCheck[1] = GetCurrentTime();
		m_dwTimeCheck[2] = m_dwTimeCheck[1] - m_dwTimeCheck[0];
		if( m_dwTimeCheck[2] <= 0 ) m_dwTimeCheck[0] = GetCurrentTime();
		if( m_dwTimeCheck[2] >  1000 )
		{
// 			//Label Pick 동작..
// 			OnSetLabelPick(0,m_nPickCnt); 
// 			clsRunRobot.OnSetPickerUpDn(0, PICKER_DN, clsRunRobot.m_npTemp_Picker_YesNo);
// 			m_nRunStep = 5000;
			//kwlee 2017.0204
			if (m_nLabelFailCheck == TRUE )
			{
				//Label Pick 동작..
				if (st_Picker_info.nPickerFailCnt == 1)
				{
					OnSetLabelPick(1,m_nPickCnt); 
					clsRunRobot.OnSetPickerUpDn(0, PICKER_DN, clsRunRobot.m_npTemp_Picker_YesNo);
				}
				else
				{
					nRet = OnBarcodeReadCheck();
					if (nRet == RET_GOOD)
					{
						OnSetLabelPick(1,m_nPickCnt); 
						clsRunRobot.OnSetPickerUpDn(0, PICKER_DN, clsRunRobot.m_npTemp_Picker_YesNo);
					}
				}
			}
			else
			{
				nRet = OnBarcodeReadCheck();
				if (nRet == RET_GOOD)
				{

				}
				else
				{
					m_nLabelFailCheck = TRUE;
				}
				//Label Pick 동작..
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
		}
		else
		{
			OnVaccummSet(0,m_nPickCnt,IO_ON);
		}
		m_nRunStep = 6000;
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

// 	case 7010:
// 		nRet_3 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, M_PICKERRBT_Z, st_motor_info[M_PICKERRBT_Z].d_pos[ROBOT_FEEDER_PICK_FIRST] + (mn_Retry*0.2), COMI.mn_runspeed_rate);
// 
// 		if (nRet_3 == BD_GOOD) //좌측으로 이동
// 		{
// 			m_nRunStep = 7100;
// 		}
// 		else if (nRet_3 == BD_ERROR || nRet_3 == BD_SAFETY)
// 		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
// 			CTL_Lib.Alarm_Error_Occurrence(700, dWARNING, st_alarm_info.strCode);
// 			//m_nRunStep = 0;
// 		}
// 		break;

	case 7100:
		//label 부착 확인
// 		if (st_basic_info.nPcbType == UDIMM_10)
// 		{
// 			if( m_nPickerNum < 5)
// 			{
// 				nRect[0] = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[m_nPickerNum],IO_ON);
// 			}
// 			else
// 			{
// 				nRect[0] = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[PICKCNT + 4 - m_nPickerNum],IO_ON);
// 			}
// 		}
// 		else
// 		{
// 			if( m_nPickerNum < 6)
// 			{
// 				nRect[0] = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[m_nPickerNum],IO_ON);
// 			}
// 			else
// 			{
// 				nRect[0] = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[PICKCNT + 5 - m_nPickerNum],IO_ON);
// 			} 
// 		}
		//////////////////////수정 예정
// 		for (int i = 0; i < m_nPickerNum; i++)
// 		{
// 			if (st_basic_info.nPcbType == UDIMM_10)
// 			{
// 				if( i < 5)
// 				{
// 					FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[i],IO_ON);
// 				}
// 				else
// 				{
// 					//FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[PICKCNT + 5 - m_nPickerNum],IO_ON);
// 					if(m_nPickCnt >= (PICKCNT + 4 - i)) 
// 					{
// 						FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[PICKCNT + 4 - i],IO_ON);
// 					}
// 				}
// 			}
// 			else
// 			{
// 
// 				if (st_basic_info.nPcbType == RDIMM)
// 				{
// 					if (i < 3)
// 					{
// 						FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[i],IO_ON);
// 					}
// 					else
// 					{	
// 						FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[PICKCNT - i],IO_ON);
// 					}
// 				}
// 				else //Sodimm, Udimm_9
// 				{
// 					if (i <6)
// 					{
// 						FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[i],IO_ON);
// 					}
// 					else
// 					{
// 						FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[PICKCNT + 5 - i],IO_ON);
// 					}
// 				}
// 			}
// 		}
// 		//////////////////
// 		if (mn_Retry >= 3)
// 		{
// 			CTL_Lib.Alarm_Error_Occurrence(700, dWARNING, m_strAlarmCode);
// 			mn_Retry = 0;
// 		}
// 		else
// 		{
		m_nRunStep = 7110;
		clsRunRobot.OnSetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);
		m_dwTimeCheck[0] = GetCurrentTime();
	//	}
		//mn_Retry = 0;
		//}
		break;

	case 7110:
		m_dwTimeCheck[1] = GetCurrentTime();
		m_dwTimeCheck[2] = m_dwTimeCheck[1] - m_dwTimeCheck[0];
		if( m_dwTimeCheck[2] <= 0 ) m_dwTimeCheck[0] = GetCurrentTime();
		if( m_dwTimeCheck[2] >  st_wait_info.dOnWaitTime[WAIT_PICKER_UP_DN] )
		{
			//clsRunRobot.OnSetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);
			//m_nRunStep = 7120;
			nRet_1 = clsRunRobot.OnGetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);

			if(nRet_1 == RET_GOOD)
			{
				m_nRunStep = 7300;
				mn_Retry = 0;
			}
// 			else
// 			{
// 				m_nRunStep = 7100;
// 				mn_Retry++;
// 			}
		}
		break;

	//case 7120:
// 		nRet_1 = clsRunRobot.OnGetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);
// 
// 		if(nRet_1 == RET_GOOD)
// 		{
// 			if (st_basic_info.nPcbType == UDIMM_10)
// 			{
// 				if( m_nPickerNum < 5)
// 				{
// 					nRect[0] = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[m_nPickerNum],IO_ON);
// 				}
// 				else
// 				{
// 					nRect[0] = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[PICKCNT + 4 - m_nPickerNum],IO_ON);
// 				}
// 			}
// 			else
// 			{
// 				if( m_nPickerNum < 6)
// 				{
// 					nRect[0] = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[m_nPickerNum],IO_ON);
// 				}
// 				else
// 				{
// 					nRect[0] = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[PICKCNT + 5 - m_nPickerNum],IO_ON);
// 				} 
// 			}
			////////////수정 예정
// 			for (int i = 0; i < m_nPickerNum; i++)
// 			{
// 				if (st_basic_info.nPcbType == UDIMM_10)
// 				{
// 					if( i < 5)
// 					{
// 						FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[i],IO_ON);
// 					}
// 					else
// 					{
// 						//FAS_IO.set_out_bit(st_io_info.o_HeadVaccum[PICKCNT + 5 - m_nPickerNum],IO_ON);
// 						if(m_nPickCnt >= (PICKCNT + 4 - i)) 
// 						{
// 							FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[PICKCNT + 4 - i],IO_ON);
// 						}
// 					}
// 				}
// 				else
// 				{
// 
// 					if (st_basic_info.nPcbType == RDIMM)
// 					{
// 						if (i < 3)
// 						{
// 							FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[i],IO_ON);
// 						}
// 						else
// 						{	
// 							FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[PICKCNT - i],IO_ON);
// 						}
// 					}
// 					else //Sodimm, Udimm_9
// 					{
// 						if (i <6)
// 						{
// 							FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[i],IO_ON);
// 						}
// 						else
// 						{
// 							FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[PICKCNT + 5 - i],IO_ON);
// 						}
// 					}
// 				}
// 			}
			/*OnFeederReq(COM_START);//label Pick &발행 요청.*/
// 			m_dwTimeCheck[0] = GetCurrentTime();
// 			m_nRunStep = 7300;
// 		}
// 		break;

		// 		case 7200:
		// 			//picker up
		// 			if( GetIntefaceInfo(COM_READY) == TRUE)
		// 			{
		// 				OnFeederReq(COM_READY_CHK); //Label Feeder로 Complete 신호 보냄
		// 				m_RunRelayMoveStep = 7210;
		// 				m_dwTimeCheck[0] = GetCurrentTime();
		// 			}
		// 			else
		// 			{
		// 				m_dwTimeCheck[1] = GetCurrentTime();
		// 				m_dwTimeCheck[2] = m_dwTimeCheck[1] - m_dwTimeCheck[0];
		// 				if( m_dwTimeCheck[2] <= 0 ) m_dwTimeCheck[0] = GetCurrentTime();
		// 				if( m_dwTimeCheck[2] > 30000)
		// 				{//손으로 라벨 제거
		// 					//507140 1 A "Label Feeder LF_COMPLETE Off Check Error."
		// 					m_strAlarmCode.Format(_T("507140")); 
		// 					CTL_Lib.Alarm_Error_Occurrence(414, dWARNING, m_strAlarmCode);
		// 					clsRunRobot.OnSetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);
		// 					/*SetInterfaceStep(COM_NONE);*/
		// 					m_RunRelayMoveStep = 7100;
		// 					break;
		// 				}
		// 			}
		// 
		// 			break;
		// 
		// 		case 7210:
		// 			m_dwTimeCheck[1] = GetCurrentTime();
		// 			m_dwTimeCheck[2] = m_dwTimeCheck[1] - m_dwTimeCheck[0];
		// 			if( m_dwTimeCheck[2] <= 0 ) m_dwTimeCheck[0] = GetCurrentTime();
		// 			if( m_dwTimeCheck[2] > 30000)
		// 			{//손으로 라벨 제거
		// 				//507140 1 A "Label Feeder LF_COMPLETE Off Check Error."
		// 				m_strAlarmCode.Format(_T("507140")); 
		// 				CTL_Lib.Alarm_Error_Occurrence(414, dWARNING, m_strAlarmCode);
		// 				clsRunRobot.OnSetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);
		// 				/*SetInterfaceStep(COM_NONE);*/
		// 				m_RunRelayMoveStep = 7000;
		// 				break;
		// 			}
		// 			if ( GetIntefaceInfo(COM_COMPLETE_CHK) == TRUE )
		// 			{		
		// 				if( m_nPickerNum < 6)
		// 				{
		// 					clsRunRobot.m_npTemp_Picker_YesNo[m_nPickerNum] = CTL_YES;
		// 				}
		// 				else
		// 				{
		// 					clsRunRobot.m_npTemp_Picker_YesNo[PICKCNT + 5 - m_nPickerNum] = CTL_YES;
		// 				}
		// 				/*clsRunRobot.OnSetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);*/
		// 
		// 				m_RunRelayMoveStep = 7300;
		// 				//1ch vlfdy
		// 				SetInterfaceStep(COM_NONE);
		// 			}
		// 
		// 			break;
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
		//picker up
		
// 		if( GetIntefaceInfo(COM_READY) == TRUE)
// 		{
// 			OnFeederReq(COM_READY_CHK); //Label Feeder로 Complete 신호 보냄
// 			m_dwTimeCheck[0] = GetCurrentTime();
// 			m_nRunStep = 7320;
// 		}
// 		else
// 		{
// 			m_dwTimeCheck[1] = GetCurrentTime();
// 			m_dwTimeCheck[2] = m_dwTimeCheck[1] - m_dwTimeCheck[0];
// 			if( m_dwTimeCheck[2] <= 0 ) m_dwTimeCheck[0] = GetCurrentTime();
// 			if( m_dwTimeCheck[2] > 30000)
// 			{//손으로 라벨 제거
// 				//507140 1 A "Label Feeder LF_COMPLETE Off Check Error."
// 				m_strAlarmCode.Format(_T("507140")); 
// 				CTL_Lib.Alarm_Error_Occurrence(720, dWARNING, m_strAlarmCode);
// 				clsRunRobot.OnSetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);
// 				/*SetInterfaceStep(COM_NONE);*/
// 		//		clsRunRobot.OnFeederReq(COM_CLEAR);
// 				//m_nRunStep = 7100;
// 			//	m_nRunStep = 0;
// 				break;
// 			}
// 		}
		//m_nRunStep = 7320;
// 		if (nCnt > 0)
// 		{
// 			st_Picker_info.nPickerFailCnt = nCnt;
// 		}

		//1.정면 오른쪽 첫번째 Label 확인 
		//2.정상이면 Pick Up 후 Label 출력 요청
		//3. 아니면 그냥 출력 요청.
		//BD_GOOD, EXIST == YES 이면
		//kwlee 2017.0204
		
// 		if (m_nLabelFailCheck == TRUE)
// 		{
// 			nRet = OnGetVaccumgmCheck(IO_ON,m_nPickCnt);
// 			if (nRet == RET_GOOD)
// 			{
// 				m_nLabelFailCheck = FALSE;
// 			}
// 		}
// 		else
// 		{
// 			nRet = OnGetVaccumgmCheck(IO_ON,m_nPickCnt);
// 			if (nRet == RET_ERROR)
// 			{
// 				CTL_Lib.Alarm_Error_Occurrence(770, dWARNING, m_strAlarmCode);
// 				break;
// 			}
// 		}
		
// 		if (nRet == RET_ERROR)
// 		{
			//CTL_Lib.Alarm_Error_Occurrence(770, dWARNING, m_strAlarmCode);
			//break;
// 		}
// 		else
// 		{
// 
// 		}
		 OnGetVaccumgmCheck(IO_ON,m_nPickCnt);
		nCnt = 0;
// 		for ( int i =0; i< 2; i++)
// 		{
			//for (int j =0; j< MAX_PICKER; j++)
			for (int j =0; j< TOTAL_PICK; j++)
			{
				//if (st_Buffer_info.nBufferData[i][j][BIN] == FAIL && st_Buffer_info.nBufferData[i][j][EXIST] == NO && st_Buffer_info.nBufferData[i][j][FAILPICK] == NO)
				// 				if (st_Buffer_info[PICK].nBufferData[i][j][BIN] == FAIL && st_Buffer_info[PICK].nBufferData[i][j][EXIST] == NO)
				// 				{
				// 					st_Buffer_info[PICK].nBufferData[i][j][X_POS] = i;
				// 					st_Buffer_info[PICK].nBufferData[i][j][Y_POS] = j;
				// 					st_Buffer_info[PICK].nBufferData[i][j][FAILPICK] = YES;
				// 					
				// 				}

				if (j < 5)
				{
					if (st_Buffer_info[PICK].nBufferData[0][j][BIN] == FAIL && st_Buffer_info[PICK].nBufferData[0][j][EXIST] == NO)
					{
						st_Buffer_info[PICK].nBufferData[0][j][X_POS] = LEFT;
						st_Buffer_info[PICK].nBufferData[0][j][Y_POS] = j;
						st_Buffer_info[PICK].nBufferData[0][j][FAILPICK] = YES;

					}
					nCnt++;
				}
				else
				{
					if (st_Buffer_info[PICK].nBufferData[1][TOTAL_PICK - j][BIN] == FAIL && st_Buffer_info[PICK].nBufferData[1][TOTAL_PICK - j][EXIST] == NO)
					{
						st_Buffer_info[PICK].nBufferData[1][TOTAL_PICK - j][X_POS] = RIGHT;
						st_Buffer_info[PICK].nBufferData[1][TOTAL_PICK - j][Y_POS] = j;
						st_Buffer_info[PICK].nBufferData[1][TOTAL_PICK - j][FAILPICK] = YES;

					}
					nCnt++;
				}
			}
		//}
			st_Buffer_info[PICK].nBufferFailCnt = nCnt;
			m_nPrintOutPutCnt = m_nPrintOutPutCnt - MAX_PICKER;
			OnDataExchange(PICK,m_nLabelFailCheck);

			if(nCnt == 0)
			{
				m_nLabelFailCheck = FALSE;
			}
			else
			{
// 		if (m_nLabelFailCheck == TRUE && nCnt > 0)
// 		{
				if (st_Buffer_info[PICK].nBufferData[1][0][BIN] == GOOD && st_Buffer_info[PICK].nBufferData[1][0][EXIST] == YES)
				{	
	// 				for (int i = 0; i< 2; i++)
	// 				{
						//for (int j = 0; j < MAX_PICKER; j++)
						for (int j = 0; j < MAX_PICKER; j++)
						{
							//if (st_Buffer_info.nBufferData[i][j][BIN] == FAIL && st_Buffer_info.nBufferData[i][j][EXIST] == NO)
							//{
							 if (st_Buffer_info[PICK].nBufferData[0][j][X_POS] == LEFT)
							{
								m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_END];
								m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST]  - (st_Picker_info.nPickerData[0][j][Y_POS] * m_dPitch_Y);
								//m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST]  - (st_Buffer_info[PICK].nBufferData[i][j][Y_POS] * m_dPitch_Y);
								//m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST]  - (st_Buffer_info[PICK].nBufferData[0][j][Y_POS] * m_dPitch_Y);
								break;
							}
							else if (st_Buffer_info[PICK].nBufferData[1][j][X_POS] == RIGHT)
							{
								m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_FEEDER_PICK_FIRST];
								m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST]  - (st_Picker_info.nPickerData[1][MAX_PICKER - j][Y_POS] * m_dPitch_Y);
								//m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_FEEDER_PICK_FIRST]  - (st_Buffer_info[PICK].nBufferData[1][m_nPickCnt - j][Y_POS] * m_dPitch_Y);
								break;
							}
							else
							{

							}
							//}
						}
					//}
					m_nRunStep = 7320;
				}
				else
				{
					m_nRunStep = 3100;
				}
			}
		//}
// 		else
// 		{
// 			
// 			//Label Fail 없으므로 정상 진행.
// 			//m_nRunStep = 7320;
// 			m_nLabelFailCheck = FALSE; 
// 			m_nRunStep = 7500;
// 		}		
		break;

// 	case 7320:
// 		m_dwTimeCheck[1] = GetCurrentTime();
// 		m_dwTimeCheck[2] = m_dwTimeCheck[1] - m_dwTimeCheck[0];
// 		if( m_dwTimeCheck[2] <= 0 ) m_dwTimeCheck[0] = GetCurrentTime();
// 		if( m_dwTimeCheck[2] > 30000)
// 		{//손으로 라벨 제거
// 			//507140 1 A "Label Feeder LF_COMPLETE Off Check Error."
// 			m_strAlarmCode.Format(_T("507140")); 
// 			CTL_Lib.Alarm_Error_Occurrence(730, dWARNING, m_strAlarmCode);
// 			clsRunRobot.OnSetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);
// 			/*SetInterfaceStep(COM_NONE);*/
// 			
// 			m_nRunStep = 7100;
// 			break;
// 		}
// 		if ( GetIntefaceInfo(COM_COMPLETE_CHK) == TRUE )
// 		{
// 			//////////////
// 			//kwlee 2017.0116
// 			OnSetLabelPick(m_nPickCnt);
// 			clsRunRobot.OnSetPickerUpDn(0, PICKER_DN, clsRunRobot.m_npTemp_Picker_YesNo);
// 			m_nRunStep = 7400;
// 		}
// 
// 		break;
		/////////////////////////////////////////////////////////
		//kwlee 2017.0204
		case 7320:
			nRet_1 = CTL_Lib.Linear_Move(ONLY_MOVE_START, m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, COMI.mn_manualspeed_rate);
			if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
			{
				m_nRunStep = 7330;
			}
			else if(nRet_1 == BD_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(780, dWARNING, st_alarm_info.strCode);
			}
			break;

		case 7330:
			nRet = CTL_Lib.Linear_Move(ONLY_MOVE_CHECK, m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, COMI.mn_manualspeed_rate);
			if(nRet == BD_GOOD) //정상적으로 완료된 상태 
			{						
				//m_RunMoveStep = 0;
				m_nRunStep = 7340;
			}
			else if (nRet == BD_RETRY)
			{
				m_nRunStep = 7320;
			}
			else if (nRet == BD_ERROR || nRet == BD_SAFETY)
			{			
				m_nRunStep = 7320;
				CTL_Lib.Alarm_Error_Occurrence(790, dWARNING, st_alarm_info.strCode);
			}
			break;

		case 7340:
			// 			OnSetLabelPick(1, m_nPickCnt);
			// 			clsRunRobot.OnSetPickerUpDn(0, PICKER_DN, clsRunRobot.m_npTemp_Picker_YesNo);
			// 			m_nRunStep = 7350;
			//kwlee 2017.0204
			if (m_nLabelFailCheck == TRUE)
			{
				//m_nRunStep = 3100;
				m_nRunStep = 2000;
			}
			else
			{
				//모두 집었다.
				//정상 이다.
				//m_nRunStep = 7500;
				m_nRunStep = 9000;
			}
			break;

// 		case 7350:
// 			OnVaccummSet(1,m_nPickCnt,IO_ON);
// 			m_nRunStep = 7360;
// 			break;

// 		case 7360:
// 			// picker up 체크
// 		//	OnGetPickCheck(m_nPickCnt);
// 			//nRet_1 = clsRunRobot.OnGetPickerUpDn(0, PICKER_UP,clsRunRobot.m_npTemp_Picker_YesNo);
// 			nRet_1 = clsRunRobot.OnGetPickerUpDn(0, PICKER_DN,clsRunRobot.m_npTemp_Picker_YesNo);
// 			if(nRet_1 == RET_GOOD)
// 			{
// 				//m_nRunStep = 7500;
// 				m_nRunStep = 7370;
// 			}
// 			else if(nRet_1 == RET_ERROR)
// 			{
// 				CTL_Lib.Alarm_Error_Occurrence(800, dWARNING, m_strAlarmCode);
// 			}
// 			break;

	
	case 7500:
// 		if (st_basic_info.nPcbType == UDIMM_9 || st_basic_info.nPcbType == UDIMM_10 || 
// 			st_basic_info.nPcbType == SODIMM || st_basic_info.nPcbType == RDIMM)
// 		{
// 			if (st_basic_info.nPcbType == RDIMM || st_basic_info.nPcbType == UDIMM_10)
// 			{
// 				if( m_nPickerNum >= ( m_nPickCnt ))
// 				{
// 					m_nRunStep = 9000;
// 					//m_PickSecondPOs = false;	
// 				}
// 				else
// 				{
// 					m_nRunStep = 100;
// 					//m_PickSecondPOs = true;
// 				}
// 			}
// 			else
// 			{
// 				if( m_nPickerNum >= ( PICKCNT-1 ))
// 				{
// 					m_nRunStep = 9000;	
// 					m_PickSecondPOs = false;	
// 				}
// 				else
// 				{
// 					m_nRunStep = 100;
// 					m_PickSecondPOs = true;
// 				}
// 			}
			//kwlee 2016.1229
			//m_nPickerNum++;

			//kwlee 2017.0111
			
			//m_nRunStep = 9000;
			//m_nLabelFailCheck = FALSE;
		///	OnFeederReq(COM_CLEAR);
		//}
		break;

		////////////////////////////////////////////////////
		//MOVE
	case 9000:
		for (int i = 0; i < PICKCNT; i++)
		{
			nRect[i] = FAS_IO.get_in_bit(st_io_info.i_HeadVaccumCheck[i],IO_ON);
			// 				if (nRect[i] == IO_OFF)
			// 				{
			// 					//505080 1 A "Head Vaccum #1 Off Check Error ."
			// 					//505081 1 A "Head Vaccum #1 On Check Error ."
			// 					m_strAlarmCode.Format(_T("5%04d%d"), st_io_info.i_HeadVaccumCheck[i], IO_OFF);
			// 					CTL_Lib.Alarm_Error_Occurrence(402, dWARNING, m_strAlarmCode);
			// 					m_RunRelayMoveStep = 0;
			// 					break;
			// 				}
			// 				else
			{ 
			//	if (i< PICKCNT -1)
				if (i< m_nPickCnt -1)
				{
					continue;
				}	
				m_nRunStep = 13000;		
			}
		}
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
		m_lTurnConvWaitTime[1] = GetCurrentTime();
		m_lTurnConvWaitTime[2] = m_lTurnConvWaitTime[1] - m_lTurnConvWaitTime[0];

		if (m_lTurnConvWaitTime[2] > 1000 )
		{
			if (st_sync_info.TurnConvJobReady[CONVEYOR] == CTL_READY)
			{
				if(st_basic_info.nPcbType == RDIMM )
				{
					if (m_bSecondPos == false)
					{
						m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_FIRST];
						m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_FIRST];
					}
					else
					{
						m_dpTargetPosList[0] = st_motor_info[M_PICKERRBT_Y].d_pos[ROBOT_Y_PLACE_THIRD];
						m_dpTargetPosList[1] = st_motor_info[M_PICKERRBT_X].d_pos[ROBOT_X_PLACE_THIRD];
					}
				}
				else if (st_basic_info.nPcbType == UDIMM_10)
				{
					if (m_bSecondPos == false)
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
				else
				{
					if (m_bSecondPos == false)
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
			}
		}
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
		OnSetPickerUpDn(m_nPickCnt);
		clsRunRobot.OnSetPickerUpDn(0, PICKER_DN, clsRunRobot.m_npTemp_Picker_YesNo);
		m_nRunStep = 17000;
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
		OnBlowSet(IO_ON);
		m_lVaccumOffTime[0] = GetCurrentTime();
		m_nRunStep = 20000;
		break;

	case 20000:
		// loader robot picker up 요청
		//picker up
		OnSetPickerUpDn(m_nPickCnt);
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

		if (m_lVaccumOffTime[2] > st_wait_info.dOffWaitTime[WAIT_PICKER_VACCUM])
		{
			OnBlowSet(IO_OFF);
		}
		m_nRunStep = 22000;
		break;

	case 22000:
		// picker up 체크
		nRet_1 = clsRunRobot.OnGetPickerUpDn(0, PICKER_UP, clsRunRobot.m_npTemp_Picker_YesNo);

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




