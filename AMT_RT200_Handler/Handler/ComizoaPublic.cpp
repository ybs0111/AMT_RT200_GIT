// ComizoaPublic.cpp: implementation of the CComizoaPublic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ComizoaPublic.h"
#include "math.h"
#include "io.h"						// 파일 존재 유무 검사 함수 호출하기 위해서는 반드시 INCLUDE 필요 
#include "Variable.h"
#include "CtlBd_Library.h"
#include "FastechPublic_IO.h"
#include "PublicFunction.h"
#include "Ctlbd_Variable.h"

#ifdef _WIN64
#include ".\\64bit\\Cmmsdk.h"
#include ".\\64bit\\CmmsdkDef.h"
#else
#include ".\\32bit\\Cmmsdk.h"
#include ".\\32bit\\CmmsdkDef.h"
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CComizoaPublic COMI;


////////////////////////////////////////////////////////////
//version:0.1 (2011.1026) 모터관련 기본 함수 정리 
//
////////////////////////////////////////////////////////////


/* 모터관련 기본 alarm file 정리 
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//alarm code 6자리 정의 
//앞자리 0~1까지 2개 : 모터번호 정의 (0~99까지)
//앞자리 2~5까지 4개 : 알람 번호 정의 최대 (0~9999까지)
//
//
///////////////////////////////////////////////////////////////
//기본 알람 종류 설정, 7번 위치 
// 0 : 경고성 알람, 메세지만 출력하고 알람 카운트는 하지 않는다 
// 1 : 장비 알람, 알람 카운트 함 
// 2 : 안전관련 알람, 알람 카운트하지 않는다.
// 3 : ....
//
//
///////////////////////////////////////////////////////////////
// 알람 파트(위치) 정의, 9번 위치, 각 부위별 에러를 확인시 사용한다 
// A, B, C, D, E, F, .......Z
//
//
///////////////////////////////////////////////////////////////
// 알람 메세지 사용한다, 11번 부터 
//  
///////////////////////////////////////////////////////////////

010001 3 A "Motor Parametor Setting Error(01 motor)."
010002 3 A "Motor Move Setting Error01 motor)."
010003 1 A "Motor Home Check Error(01 motor)."
010004 1 A "Motor Alarm Error(01 motor)."
010005 1 A "Motor Power Off Error(01 motor)."
010006 1 A "Motor Move Check Error(01 motor)."
010007 2 A "Motor Move Safety Check Error(01 motor)."
010008 1 A "Motor position minus limit error(01 motor)."
010009 1 A "Motor position plus limit error(01 motor)."
010010 1 A "Motor Interrupt check error(01 motor)."
010011 1 A "Motor Move Time Out Check Error(01 motor)."
010012 1 A "Motor allow position Check Error(01 motor)."
010013 1 A "Motor motion done Check Error(01 motor)."

010015 1 A "Interpolation IxMapAxis setting Error(01 IxMap)."
010016 1 A "Interpolation IxMapAxis check Error  (01 IxMap)."
*/



CComizoaPublic::CComizoaPublic()
{
	int i, j=0;

	for(i=0; i<16; i++) ml_LmIndexNumber[i] = i;  //최대 16개의 ListIndexMap를 가질수 있다 
	for(i=0; i<8; i++)  ml_IxIndexNumber[i] = i;  //최대 8개의 IxIndexMap를 가질 수 있다 
	
	//2011.1024 
	mn_max_move_limit_time = 60000;//30초 //3000ms
	mn_max_home_limit_time = 60000; //1분 
	mn_max_retry_cnt = 3;
}

CComizoaPublic::~CComizoaPublic()
{

}

int CComizoaPublic::Initialize_MotBoard(CString s_file) //보드 초기화 
{
	int nRet = -1;
	long nNumAxes = 0;
	char chPath[100];

	///* 2012.1029 jamestest
	
//커미조아 모터보드 초기화 순서 
//1. 커미조아 라이브러리를 응용프로그램 공간에 로딩한다 
//2. 모터보드(하드웨어)를 로그하고 장치를 초기화 한다   
//3. 모터 스피드를 셋팅한다 cmmCfgSetSpeedPattern(nAxis, nSpeedMode, d_work, d_accel, d_decel);

	nRet = cmmLoadDll(); //라이브러리 로드 - 응용 프로그램의 메모리 공간으로 커미조아 라이브러리 로드 
	if (nRet == TRUE)	//TRUE    1
	{
		nRet = BD_GOOD;
	}
	else
	{
		if(mn_errormsg_debug_mode)
		{
//			sprintf(mc_normal_msg, "cmmLoadDll return error");
			//wsprintfA(mc_normal_msg, "%S", _T("cmmLoadDll return error"));
			clsFunc.OnStringToChar(_T("cmmLoadDll return error"), mc_normal_msg);
			Debug_File_Create(0, mc_normal_msg);
		}
		return BD_ERROR; //MOT_OPEN_FAIL;
	}

	nRet = cmmGnDeviceLoad(cmTRUE, &nNumAxes);//모터보드(하드웨어)를 로그하고 장치를 초기화 한다 
	if (nRet == cmERR_NONE) //수행성공 의미, nNumAxes=모션제어축의 개수를 반환  
	{
		clsFunc.OnStringToChar(s_file, chPath);
		nRet = cmmGnInitFromFile(chPath); //("c:\\AMT7000\\Setting\\amt7000.cme2");
		if(nRet != cmERR_NONE)
		{
			return BD_ERROR; //MOT_OPEN_FAIL;
		}
		mn_motorbd_init_end = BD_YES; 

		mn_totalmotoraxis_number = nNumAxes;

		cmmCfgSetSeqMode(cmSEQM_SKIP_RUN); //cmSEQM_SKIP_RUN, cmSEQM_WAIT_RUN

		nRet = BD_GOOD;
	}
	else
	{
		if(mn_errormsg_debug_mode)
		{
//			sprintf(mc_normal_msg, "cmmGnDeviceLoad return error");
			//wsprintfA(mc_normal_msg, "%S", _T("cmmGnDeviceLoad return error"));
			clsFunc.OnStringToChar(_T("cmmGnDeviceLoad return error"), mc_normal_msg);
			Debug_File_Create(0, mc_normal_msg);
		}
		mn_motorbd_init_end = BD_NO; 
		nRet = BD_ERROR;
	}

	return nRet;
}

void CComizoaPublic::Close_MotBoard() //보드 클로즈 
{
	//cmmUnloadDll(); //라이브러리 언로드 - 응용 프로그램의 메모리 공간으로 부터 라이브러리 언로드 

	//cmmGnDeviceUnload(); //시스템에 설치된 하드웨어 장치를 언로드하고 장치사용을 정지합니다 
}

int CComizoaPublic::Set_Simulation_Mode(int n_axis, int n_simul_mode)
{
//	n_simul_mode => 0:시뮬레이션 모드 비활성, 1:시뮬레이션 모드 활성

	cmmGnSetSimulMode(n_axis, n_simul_mode);//cmCTRL_SEMI_C , 엔코더,(피드백기준) 기준으로 목표좌표를 설정하여 이송합니다
	return BD_GOOD;
} 

//
int CComizoaPublic::Set_CMD_CTL_Mode(int n_axis, int n_cmd_type)
{
//	cmCTRL_OPEN, // Open loop control mode
//	cmCTRL_SEMI_C, // Semi-closed loop control mode (applied only to absolute in-position commands)
//	cmCTRL_FULL_C // Full-closed loop control mode (this is not supported at current version)

	cmmCfgSetCtrlMode(n_axis, n_cmd_type);//cmCTRL_SEMI_C , 엔코더,(피드백기준) 기준으로 목표좌표를 설정하여 이송합니다
	return BD_GOOD;
}


int CComizoaPublic::Set_Motor_IO_Property(long n_Axis, long  n_PropId, long n_OnOff)
{
	int nRet = -1;
	CString strMsg;
/*
	// MIO Property ID //
typedef enum _TCmMioPropId{
	cmALM_LOGIC=0, cmALM_MODE=1, cmCMP_LOGIC=2, cmDR_LOGIC=3, cmEL_LOGIC=4, cmEL_MODE=5, 
	cmERC_LOGIC=6, cmERC_OUT=7, cmEZ_LOGIC=8, cmINP_EN=9, cmINP_LOGIC=10, cmLTC_LOGIC=11, 
	cmLTC_LTC2SRC=12, cmORG_LOGIC=13, cmSD_EN=14, cmSD_LOGIC=15, cmSD_LATCH=16, cmSD_MODE=17, cmSTA_MODE=18,
	cmSTA_TRG=19, cmSTP_MODE=20, cmCLR_CNTR=21, cmCLR_SIGTYPE=22, cmCMP_PWIDTH=23, cmERC_ONTIME=24, cmSVON_LOGIC=25,
}TCmMioPropId;
*/
	//CMM_EXTERN long (WINAPI *cmmCfgSetMioProperty)	(long Axis, long PropId, long PropVal);

	//우선 InPosition On/Off 기능을 사용한다 
	nRet = cmmCfgSetMioProperty(n_Axis, n_PropId, n_OnOff);
	if (nRet == cmERR_NONE)
	{//수행 성공 
		return BD_GOOD;
	}
	else
	{//수행 실패 
		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[axis %d]cmmCfgSetMioProperty return error"), n_Axis);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//			sprintf(mc_normal_msg, "[axis %d]cmmCfgSetMioProperty return error", n_Axis);
			Debug_File_Create(0, mc_normal_msg);
		}
		return BD_ERROR;
	}
	return BD_ERROR;
}


// ******************************************************************************
// 모터 POWER ON 설정 함수                                                       
// -> 사용하는 모터 보드 최대 축수에 따라 파라메터 변경한다                      
// ******************************************************************************
int CComizoaPublic::Set_MotPower(int n_Axis, int n_OnOff) 
{
	int nRet = -1, nRet_2=BD_ERROR;
	char cJamcode[10] = {NULL};
	double dCurrentPos = 0, dGetPos = 0; 	
	CString strMsg;

	if (m_nMotInfo[n_Axis] == MOT_SERVO)
	{
		nRet = cmmGnSetServoOn(n_Axis, n_OnOff);
		if (nRet == cmERR_NONE)
		{	 
			if(n_OnOff == BD_YES) //james 2009.5.15 추가
			{
				Sleep(200);
				dCurrentPos = Get_MotCurrentPos(n_Axis); //엔코드 실제 현재 위치 
			
				//limit 값 셋팅 에러 
				if(dCurrentPos < md_limit_position[n_Axis][0]) //-1 - limit 
				{//010008 E A "Motor position minus limit error(01 motor)."		
					strMsg.Format(_T("%02d0008"), n_Axis);
					//wsprintfA(mc_alarmcode, "%S", strMsg);
					clsFunc.OnStringToChar(strMsg, mc_alarmcode);
					//sprintf(mc_alarmcode, "%02d0008", n_Axis); 
					nRet_2 = BD_ERROR;
				}	
				else if(dCurrentPos > md_limit_position[n_Axis][1]) //+ limit 
				{//010009 E A "Motor position plus limit error(01 motor)."
					strMsg.Format(_T("%02d0009"), n_Axis);
					wsprintfA(mc_alarmcode, "%S", strMsg);
					clsFunc.OnStringToChar(strMsg, mc_alarmcode);
	//				sprintf(mc_alarmcode, "%02d0009", n_Axis);  
					nRet_2 = BD_ERROR;
				}
				else
				{		
					/*20150428 james 사용하지 말자 //COMI.Set_CMD_CTL_Mode(i, cmCTRL_SEMI_C);//cmCTRL_SEMI_C , 엔코더,(피드백기준) 기준으로 목표좌표를 설정하여 이송합니다(커멘드 정보는 무시되고 최종 feedback(엔코더) 위치로 제어가 이루어진다.
					//현재 위치를 엔코더 기준으로 셋팅한다 
					cmmStGetPosition(n_Axis, cmCNT_COMM, &dGetPos);	//현재 커멘드 위치 			
					cmmStSetPosition(n_Axis, cmCNT_COMM, (double)dCurrentPos); //엔코드 위치와 커멘드 위치를 통일 시킨다 
					//cmmStSetPosition(n_Axis, cmCNT_FEED, (double)0);
					*/
					cmmStGetPosition(n_Axis, cmCNT_COMM, &dGetPos);	//20150429 현재 커멘드 위치 	

					if(mn_errormsg_debug_mode)
					{
						strMsg.Format(_T("Axis=%d, CMDPOS[%4.3f] FEEDPOS[%4.3f]"), n_Axis, dGetPos, dCurrentPos);
						//wsprintfA(mc_normal_msg, "%S", strMsg);
						clsFunc.OnStringToChar(strMsg, mc_normal_msg);
	//					sprintf(mc_normal_msg, "Axis=%d, CMDPOS[%4.3f] FEEDPOS[%4.3f]", n_Axis, dGetPos, dCurrentPos);
						Debug_File_Create(0, mc_normal_msg);
					}
					nRet_2 = BD_GOOD;
				}
			} 
			else
			{
				nRet_2 = BD_GOOD;
			}
		

			if(nRet_2 == BD_ERROR)
			{
				if(mn_errormsg_debug_mode)
				{
					strMsg.Format(_T("[axis %d]cmmGnSetServoOn OnOff= %d return error"), n_Axis, n_OnOff);
					//wsprintfA(mc_normal_msg, "%S", strMsg);
					clsFunc.OnStringToChar(strMsg, mc_normal_msg);
		//			sprintf(mc_normal_msg, "[axis %d]cmmGnSetServoOn OnOff= %d return error", n_Axis, n_OnOff);
					Debug_File_Create(0, mc_normal_msg);
				}
			}
		}
	}
	else
	{
		FAS_IO.set_out_bit(st_io_info.oMotPwr[n_Axis], n_OnOff);
	}

	return nRet_2;
}
// ******************************************************************************


// ******************************************************************************
// 2009.8.2                                               
// -> Command 와 Encoder 위치를 비교한다                      
// ******************************************************************************
int CComizoaPublic::Get_Deviation_Position(int n_Axis, double d_AllowPos, double * d_Get_DevPos) 
{
	int nRet = -1;
	char cJamcode[10] = {NULL};
	double d_Cmd=0, d_Feed=0, d_Dev=0, d_CompleteVal=0; 
	CString strMsg;

	if(st_work_info.nSimulationMode == 1)
	{
		return BD_GOOD;
	}



	cmmStGetPosition(n_Axis, cmCNT_COMM, &d_Cmd);	//현재 커멘드 위치 
	cmmStGetPosition(n_Axis, cmCNT_FEED, &d_Feed);	//현재 Feedback 위치 
	cmmStGetPosition(n_Axis, cmCNT_DEV, &d_Dev);	//현재 편차카운트 

	d_CompleteVal =  (double)fabs(d_Cmd - d_Feed);
	//d_CompleteVal = d_Dev;

	if(d_CompleteVal < d_AllowPos) //정상
	{//허용초차보다 작으면 정상 
		d_Get_DevPos[0] = d_Cmd; d_Get_DevPos[1] = d_Feed; d_Get_DevPos[2] = d_Dev; 
		 nRet = BD_GOOD;
	}
	else //if(d_CompleteVal > d_AllowPos) 
	{//허용오차보다 더 커지면 에러  
		d_Get_DevPos[0] = d_Cmd; d_Get_DevPos[1] = d_Feed; d_Get_DevPos[2] = d_Dev; 
		//010012 E A "Motor allow position Check Error(01 motor)."
		strMsg.Format(_T("%02d0012"), n_Axis);
		//wsprintfA(mc_alarmcode, "%S", strMsg);
		clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//		sprintf(mc_alarmcode, "%02d0012", n_Axis);  
		nRet = BD_ERROR;
		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("Axis[%d], CMDPOS[%.3f], FEEDPOS[%4.3f], DEVPOS[%4.3f]"), n_Axis, d_Cmd, d_Feed, d_Dev);
			//wsprintfA(mc_abnormal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//		  sprintf(mc_abnormal_msg,"Axis[%d], CMDPOS[%.3f], FEEDPOS[%4.3f], DEVPOS[%4.3f]", n_Axis, d_Cmd, d_Feed, d_Dev);
 			Debug_File_Create(0, mc_abnormal_msg);
		}
	}

	return nRet;
}
// ******************************************************************************


// ******************************************************************************
// 모터 POWER 상태 리턴 함수                                                     
//  : 리턴 값 [0:POWER OFF    1:POWER ON]                                        
// ******************************************************************************
int CComizoaPublic::Get_MotPower(int n_Axis) 
{
	int  nRet = BD_ERROR;
	long dwOnOff=0;

	if(st_work_info.nSimulationMode == 1)
	{
		return BD_GOOD;
	}

	if (m_nMotInfo[n_Axis] == MOT_SERVO)
	{
		cmmGnGetServoOn(long(n_Axis), &dwOnOff);	
		if (dwOnOff == cmTRUE)  //1 또는 cmTRUE //Servo Motor ON
		{
			nRet = BD_GOOD;
		}
		else //0 또는 cmFALSE //Servo Motor Off
		{
		
			nRet = BD_ERROR;
		}
	}
	else
	{
		if (FAS_IO.get_out_bit(st_io_info.oMotPwr[n_Axis], IO_OFF) == IO_OFF)
		{
			nRet = BD_ERROR;
		}
		else
		{
			nRet = BD_GOOD;
		}
	}

	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// 모터 ALARM CLEAR 함수                                                         
// -> 사용하는 모터 보드 최대 축수에 따라 파라메터 변경한다                      
// ******************************************************************************
int CComizoaPublic::Set_MotAlarmClear(int n_Axis) 
{
	int  nRet = BD_ERROR;
	CString strMsg;

	if(st_work_info.nSimulationMode == 1)
	{
		return BD_GOOD;
	}

	 nRet = cmmGnPulseAlarmRes(n_Axis, BD_YES, 300, TRUE); //FALSE);
	//nRet = cmmGnPulseAlarmRes(n_Axis, cmFALSE, 300, TRUE); 
	if (nRet == cmERR_NONE)
	{
		nRet = BD_GOOD;
	}
	else
	{
		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[axis %d]cmmGnPulseAlarmRes return error"), n_Axis);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//			sprintf(mc_normal_msg, "[axis %d]cmmGnPulseAlarmRes return error", n_Axis);
			Debug_File_Create(0, mc_normal_msg);
		}
		nRet = BD_ERROR;
	}

	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// 모터 ALARM 상태 리턴 함수                                                     
// -> 사용하는 모터 보드 최대 축수에 따라 파라메터 변경한다                      
//  : 리턴 값 [0:ALARM NONE    1:ALARM OCCURED]                                  
// ******************************************************************************
int CComizoaPublic::Get_MotAlarmStatus(int n_Axis) 
{
	int  nRet = BD_ERROR;
	long dwMioState=0;
	BOOL bALM_State = 0;
	CString strMsg;

	if(st_work_info.nSimulationMode == 1)
	{//BD_ERROR == 알람이 발생하지 않은 상태
		return BD_ERROR;
	}

	cmmStReadMioStatuses(n_Axis, &dwMioState);	

	bALM_State = (dwMioState >> cmIOST_ALM) & 0x01;
	
	// 20111212 jtkim
	if(!bALM_State) 
	{//알람발생 
		nRet = BD_GOOD; //알람이 발생한 상태 
	}
	else
	{	//010004 E A "Motor Alarm Error(01 motor)."
		strMsg.Format(_T("%02d0004"), n_Axis);
		//wsprintfA(mc_alarmcode, "%S", strMsg);
		clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//		sprintf(mc_alarmcode, "%02d0004", n_Axis);  
		nRet = BD_ERROR; //알람 발생하지 않은 상태  

		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[axis %d]cmmStReadMioStatuses return alarm error"), n_Axis);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//			sprintf(mc_normal_msg, "[axis %d]cmmStReadMioStatuses return alarm error", n_Axis);
			Debug_File_Create(0, mc_normal_msg);
		}
	}

	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// 모터 이동 정지 함수                                                           
// -> nMode     : 모터 정지 모드 [0:감속 정지    1:진급 정지]                    
// -> nAxis     : 정지할 모터 축                                                 
// -> wEndCheck : 동작 완료 검사 방식                                            
//  : [0:미완료 시점에서 다른 동작 가능     1:완료 시점후에 다른 동작 가능]      
// ******************************************************************************
int CComizoaPublic::Set_MotStop(int n_Mode, int n_Axis) 
{
	int nRet = BD_ERROR;
	int nEndCheck = 0;
	CString strMsg;

	if(st_work_info.nSimulationMode == 1)
	{//BD_ERROR == 알람이 발생하지 않은 상태
		return BD_GOOD;
	}

	if (n_Mode == 0)  // 감속 정지 
	{
		nRet = cmmSxStop(n_Axis, 0, nEndCheck);
	}
	else             // 긴급 정지
	{
		nRet = cmmSxStopEmg(n_Axis);
	}

	if (nRet == cmERR_NONE)
	{
		nRet = BD_GOOD;
	}
	else
	{
		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[axis %d] cmmSxStop return alarm error"), n_Axis);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//			sprintf(mc_normal_msg, "[axis %d] cmmSxStop return alarm error", n_Axis);
			Debug_File_Create(0, mc_normal_msg);
		}

		nRet = BD_ERROR;
	}

	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// 모터 특정 축의 현재 위치 리턴 함수                                            
// -> 이 함수에서는 서브 함수 동작 정상 완료 유무 검사하지 않도록 한다           
//  : 리턴 값으로 사용할 수 있는 값중에 [0이나 -] 모두 포함되기 때문이다         
//  : 함수 동작 실패 시 [-]값 리턴되더라도 이 값이 위치값으로 오해될수 있기 때문 
// ******************************************************************************
double CComizoaPublic::Get_MotCurrentPos(int n_Axis) 
{

	//return 0;
	/*20150429 james 
	int    nRet = BD_ERROR;
	double dGetActualPos = 0;
	CString strMsg;

	if(st_work_info.nSimulationMode == 1)
	{//BD_ERROR == 알람이 발생하지 않은 상태
		return dGetActualPos;
	}

	// jtkm 20130823
	if (0 && m_nMotInfo[n_Axis] == MOT_STEP) //20150428 james step 모터도 cme file 생성시 step motor로 설정하면 자동으로 cmCNT_FEED와 같이 읽어됨 
	{
		// 엔코더 입력이 없으면 command 처리
		nRet = cmmStGetPosition(n_Axis, cmCNT_COMM, &dGetActualPos);
	}
	else
	{
		nRet = cmmStGetPosition(n_Axis, cmCNT_FEED, &dGetActualPos);
	}
	
	if (nRet == cmERR_NONE)
	{
		return dGetActualPos;  // 현재 위치 리턴 
	}
	else
	{
		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[axis %d] cmmStGetPosition return error"), n_Axis);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//			sprintf(mc_normal_msg, "[axis %d] cmmStGetPosition return error", n_Axis);
			Debug_File_Create(0, mc_normal_msg);
		}
		nRet = BD_ERROR;
	}

	return (double)nRet;
	*/

	//20150429 james 수정 
	int    nRet = BD_ERROR;
	double dGetActualPos = 0;
	CString strMsg;
	int i;

	if(st_work_info.nSimulationMode == 1)
	{
		return BD_GOOD;
	}

	for(i=0; i< 3000; i++) //20150422 james 문제 발생시 재시도하여 정상적일때만 리턴하자, 아닐때 에러 값을 가지고 리턴하면 많은 문제가 예상 됨 
	{//약 10초동안 재시도한다 
		nRet = cmmStGetPosition(n_Axis, cmCNT_FEED, &dGetActualPos);
		if (nRet == cmERR_NONE)
		{
			i =  3000; //
			return dGetActualPos;  // 현재 위치 리턴 
		}
		else
		{
			ml_alarmcode_id = nRet;   
			strMsg.Format(_T("Axis[%d], FEEDPOS[%4.3f] Get_MotCurrentPos_cmmStGetPosition_return_Error"), n_Axis, dGetActualPos);
			CTL_Lib.Motor_Error_Occurrence(3, n_Axis, ml_alarmcode_id, strMsg);		 
			nRet = BD_ERROR;		
			Sleep(1); //20150422 james 추가 
		}
	}

	return (double)nRet;
}
// ******************************************************************************

// ******************************************************************************
// 모터 이동 위치 정상 유무 검사 함수                                            
// -> nAxis     : 모터 위치 검사할 특정 축                                       
// -> dDistance : 현재 모터 위치                                                 
// -> nRange    : 모터 위치 허용 오차 범위                                       
// ******************************************************************************
/* 2015.0219 james 
int CComizoaPublic::Check_MotPosRange(int n_Axis, double d_Distance, int n_Range) 
{
	int    nRet = BD_ERROR;
	double dGetActualPos=0;
	double dP = 0, dN = 0;

	if(st_work_info.nSimulationMode == 1)
	{//BD_ERROR == 알람이 발생하지 않은 상태
		return BD_GOOD;
	}

	dN = (double)d_Distance - (double)(n_Range); 
	dP = (double)d_Distance + (double)(n_Range);

	dGetActualPos = Get_MotCurrentPos(n_Axis);  // 모터 특정 축의 현재 위치 리턴 함수 

	if (dGetActualPos >= dN && dGetActualPos <= dP) 	 
	{
		nRet = BD_GOOD;
	}
	else
	{
		nRet = BD_ERROR;
	}

	return nRet;
}*/
// ******************************************************************************



// ******************************************************************************
// 모터 INPOSITION 검사 함수                                                     
// -> 동작중인 경우 시그널 ON 정지하면 시그널 OFF이다                            
// ******************************************************************************
int CComizoaPublic::Get_MotInposition(int n_Axis) 
{
	int nRet = BD_ERROR;
	long lMioState = 0;
	BOOL bInPos_State = 0;

	if(st_work_info.nSimulationMode == 1)
	{// 
		return BD_GOOD;
	}

	nRet = cmmStReadMioStatuses(n_Axis, &lMioState);
	if(nRet == cmERR_NONE)
	{
		bInPos_State = (lMioState >> cmIOST_INP) & 0x01;
		
		// 20111212 jtkim
		//if (bInPos_State == BD_YES)
		if(bInPos_State)
		{
			nRet = BD_GOOD;
		}
		else
		{
			nRet = BD_ERROR;
		}		
	}
	else
	{
		nRet = BD_ERROR;
	}
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// 모터 이동 완료 검사 함수                                                      
// -> iMotionDone : [0:동작 완료    1:동작 중]                                   
// -> 알람 발생 시 INPOSITION 시그널이 발생하지 않는다                           
// ******************************************************************************
int CComizoaPublic::Get_MotMotionDone(int n_Axis, int n_Mode) 
{
	int  nRet = BD_ERROR, nRet_2 =0 ;
	int  nInposition = 0, nMotionDone = -1;
	long dwMotionDone=0;
	int  nServo_On=0;
	double dCurrPos=0;

	long dwMioState=0;
	long dwMotionStatus[3]={0,0,0};
	BOOL bINPO_State = false;

	CString strMsg;

	if(st_work_info.nSimulationMode == 1)
	{ 
		return BD_GOOD;
	}

/*
	//
	cmmHomeGetSuccess(n_Axis, &dwMotionStatus[0]);//1
	cmmStReadMotionState(n_Axis, &dwMotionStatus[1]); //1
	cmmHomeIsBusy(n_Axis, &dwMotionStatus[2]);  //0

	cmmStReadMioStatuses(n_Axis, &dwMioState);
	BOOL INPO_State = (dwMioState >> cmIOST_INP) & 0x01;	
	nRet = cmmSxIsDone(n_Axis, &dwMotionDone);			// Motion Done이냐?
	//nInposition = Get_Mot_Inposition(n_Axis);			// 모터 INPOSITION 검사 함수 
	//제외 nServo_On   = Get_Mot_Power(n_Axis);
	
	if (nRet == cmERR_NONE)
	{
		if(dwMotionStatus[0] == cmTRUE && dwMotionStatus[1] == cmTRUE && dwMotionStatus[2] == cmFALSE && INPO_State == cmTRUE) //2009.7.11 james 변경 이미 cmmSxIsDone 에서 체크 && INPO_State == cmTRUE) 
		{
			return BD_GOOD;
		}
		else
		{
			return BD_RET_PROCEED;
		}
	}
	else 
	{//리턴 값이 에러일때 		
		sprintf(cJamcode, "%02d0011",  n_Axis); //Motor time out error								
		dCurrPos = Get_MotCurrentPos(n_Axis);  // 모터 특정 축의 현재 위치 리턴 함수
		CtlBdFunc.ms_ErrMsg.Format("[MotionDone Return] Axis=%d : Target= %4.3f, CurPos= %4.3f", n_Axis, 0, dCurrPos);
		CtlBdFunc.Send_Error_Message(MOT_ERR_MSG, n_Axis, cJamcode, CtlBdFunc.ms_ErrMsg);
		
		cmmSxStopEmg(n_Axis);				
		return BD_ERROR;
		//}	
	}
	
*/
//	CtlBdFunc.Send_Error_Message(0, n_Axis, "0", "0");

 	
	cmmStReadMioStatuses(n_Axis, &dwMioState);
	bINPO_State = (dwMioState >> cmIOST_INP) & 0x01;

	nRet_2 = cmmSxIsDone(n_Axis, &dwMotionDone);			// Motion Done이냐?
	//nInposition = Get_Mot_Inposition(n_Axis);			// 모터 INPOSITION 검사 함수 
	//제외 nServo_On   = Get_Mot_Power(n_Axis);

	if (nRet_2 == cmERR_NONE)
	{
		if (m_nMotInfo[n_Axis] == MOT_STEP)
		{
			if (dwMotionDone == cmTRUE)
			{
				nRet = BD_GOOD;
			}
			else
			{
				nRet = BD_PROCEED;
			}
		}
		else
		{
			if (n_Mode == -1)
			{
				if (dwMotionDone == cmTRUE && bINPO_State == cmTRUE) //2009.7.11 james 변경 이미 cmmSxIsDone 에서 체크 && INPO_State == cmTRUE) 
				{
					nRet = BD_GOOD;
				}
				else
				{
					nRet = BD_PROCEED;
				}
			}
			else			// Inposition을 보지 않을때...
			{
				if (dwMotionDone == cmTRUE)
				{
					nRet = BD_GOOD;
				}
				else
				{
					nRet = BD_PROCEED;
				}
			}
		}
	}
	else 
	{//리턴 값이 에러일때 
		//010013 E A "Motor motion done Check Error(01 motor)."
		strMsg.Format(_T("%02d0013"), n_Axis);
		//wsprintfA(mc_alarmcode, "%S", strMsg);
		clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//		sprintf(mc_alarmcode, "%02d0013", n_Axis);  
//		cmmSxStopEmg(n_Axis);
		cmmSxStop(n_Axis, FALSE, FALSE); //20150429 james TRUE를 FALSE로 변경하여 Blocking을 방지한다 한다  // 모터 이동 정지 함수 
		//2011.0105 cmmSxStopEmg(n_Axis);				
		nRet = BD_ERROR;

		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[axis %d] cmmSxIsDone return error"), n_Axis);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//			sprintf(mc_normal_msg, "[axis %d] cmmSxIsDone return error", n_Axis);
			Debug_File_Create(0, mc_normal_msg);
		}
		//}	
	}

	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// 모터 ZERO BIT 검사 함수                                                       
// -> 모터 HOMING 시 사용되는 함수                                               
//  : HOMING 완전히 종료되면 해당 BIT가 1로 설정된다                             
// ******************************************************************************
int CComizoaPublic::Get_MotMotionZeroReturn(int n_Axis) 
{
	int nRet = BD_ERROR;
	long dwMioState =0;
	BOOL bORG_State = BD_ERROR;

	if(st_work_info.nSimulationMode == 1)
	{// 
		return BD_GOOD;
	}


	cmmStReadMioStatuses(n_Axis, &dwMioState);
	
	bORG_State = (dwMioState >> cmIOST_ORG) & 0x01;

	// 20111212 jtkim
	//if (bORG_State == BD_YES)
	if(bORG_State)
	{
		nRet = BD_GOOD;
	}
	else
	{
		nRet = BD_ERROR;
	}

	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// 모터 센서 상태 리턴 함수 [EL(+)    EL(-)    HOME]                             
// -> 리턴 플래그 [0:미감지    1:감지]                                           
// ******************************************************************************
int CComizoaPublic::Get_MotIOSensor(int n_Axis, int n_MotSensor) 
{
	int nRet = BD_ERROR;
	BOOL bELN_State = 0;
	BOOL bELP_State = 0;
	BOOL bORG_State = 0;
	BOOL bSD_State = 0; //2011.0105 
	long dwMioState = 0;

	if(st_work_info.nSimulationMode == 1)
	{// 
		return BD_GOOD;
	}

	cmmStReadMioStatuses(n_Axis, &dwMioState);	
	switch(n_MotSensor)
	{
		case MOT_SENS_ELM :  // EL[-] 
			bELN_State = (dwMioState >> cmIOST_ELN) & 0x01;
			
			//if(bELN_State == BD_YES)
			if(bELN_State)
			{
				nRet = BD_GOOD;
			}
			else
			{
				nRet = BD_ERROR;
			}
			break;

		case MOT_SENS_ELP :  // EL[+] 
			bELP_State = (dwMioState >> cmIOST_ELP) & 0x01;
			
			if(bELP_State)
			{
				nRet = BD_GOOD;
			}
			else
			{
				nRet = BD_ERROR;
			}
			break;

		case MOT_SENS_HOME :
			bORG_State = (dwMioState >> cmIOST_ORG) & 0x01;
			
			if(bORG_State)
			{
				nRet = BD_GOOD;
			}
			else
			{
				nRet = BD_ERROR;
			}
			break;

		case MOT_SENS_SD : //2011.0105 james SD센서 조건 추가 
			bSD_State = (dwMioState >> cmIOST_SD) & 0x01;
			
			if(bSD_State)
			{
				nRet = BD_GOOD;
			}
			else
			{
				nRet = BD_ERROR;
			}
			break;

		default :
			nRet = BD_ERROR; //MOT_INVALID_PARAMETER_NUM;
			break;
	}

	return nRet;
}
// ******************************************************************************



int CComizoaPublic::Set_MotInitSpeed(int n_Axis, double d_work)
{
	cmmSxOptSetIniSpeed(n_Axis, d_work);
	return BD_GOOD;
}

//모터의 속도 사용 범위를 정한다(Min ~ Max)
int CComizoaPublic::Set_SpeedRange(int n_Axis, double d_max_pps) //2011.1104 
{//각축의 최소 ~ 최대 속도를 설정합니다.
	//10(min) ~ 655350(max) pps 설정 값 
	// 최대값 설정에 따라 최소값은 자동 계산 
	cmmSxOptSetIniSpeed(n_Axis, d_max_pps);
	return BD_GOOD;
}


int CComizoaPublic::Set_HomeSetConfig(int n_Axis, int n_mode, int n_ezcount, int n_revvel, int n_escdist)
{
	int  nRet = BD_ERROR, nFlag = 0;
	int nErrAxis, nErrReason;
	CString strMsg;

	if(st_work_info.nSimulationMode == 1)
	{// 
		return BD_GOOD;
	}

	nFlag = cmmHomeSetConfig(n_Axis, n_mode, n_ezcount, n_escdist, 0);
	if (nFlag == cmERR_NONE)
	{
		nRet = BD_GOOD;
	}
	else
	{
		nErrAxis = cmmErrParseAxis(nRet);
		nErrReason = cmmErrParseReason(nRet);
		nRet = BD_ERROR;

		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[axis %d] cmmHomeSetConfig return error"), n_Axis);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//			sprintf(mc_normal_msg, "[axis %d] cmmHomeSetConfig return error", n_Axis);
			Debug_File_Create(0, mc_normal_msg);
		}
	}

	return nRet;
}

int CComizoaPublic::Set_MotHomeSpeed(int n_SpeedCase, int n_Axis, double d_work, double d_accel, double d_decel)
{
	int  nRet = BD_ERROR, nFlag = 0;
	int nErrAxis, nErrReason;
	double dAccPPS = 0, dDecPPS = 0;
	CString strMsg;

	if(st_work_info.nSimulationMode == 1)
	{// 
		return BD_GOOD;
	}
	
	// 20111213 jtkim
//	nFlag = cmmHomeSetSpeedPattern(n_Axis, cmSMODE_T, d_work, d_accel, d_decel, 5000);
	if(n_SpeedCase == MOT_SPD_CM_CTL) //2011.1104 
	{
		nFlag = cmmHomeSetSpeedPattern(n_Axis, cmSMODE_T, d_work, d_accel, d_decel, 1);
	}
	else
	{
		nFlag = cmmHomeSetSpeedPattern(n_Axis, cmSMODE_T, d_work, d_accel, d_decel, 5000);
	}

	if (nFlag == cmERR_NONE)
	{
		nRet = BD_GOOD;
	}
	else
	{
		nErrAxis = cmmErrParseAxis(nRet);
		nErrReason = cmmErrParseReason(nRet);
		nRet = BD_ERROR;

		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[axis %d] cmmHomeSetSpeedPattern return error"), n_Axis);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//			sprintf(mc_normal_msg, "[axis %d] cmmHomeSetSpeedPattern return error", n_Axis);
			Debug_File_Create(0, mc_normal_msg);
		}
	}
	return nRet;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//속도 오버라이딩 
///////////////////////////////////////////////////////////////////////////////////////////////
int CComizoaPublic::Set_Override_Speed(int n_Axis, double d_Speed, int n_SpeedMode)  
{
	int  nRet = BD_ERROR, nFlag = 0;
	
	nFlag = cmmCfgSetSpeedPattern(n_Axis, n_SpeedMode, d_Speed, md_spd_vel[n_Axis][1], md_spd_vel[n_Axis][2]);
	
	if (nFlag == cmERR_NONE)
	{
		if (cmmOverrideSpeedSet((long)n_Axis) == cmERR_NONE)
		{
			nRet = BD_GOOD;
		}
		else
		{
			nRet = BD_ERROR;
		}
	}
	else
	{
		nRet = BD_ERROR;
	}
	
	return nRet;
}

int CComizoaPublic::Set_MotUnitSpeed(int n_Axis, double n_UnitSpeed) //2011.1104 추가 
{
	int  nRet = BD_ERROR, nFlag = 0;
	CString strMsg;
	//1cm/이동거리 => 1cm 이동하는데 1000pulse가 필요하면 다음과 같이 cmmCfgSetUnitSpeed(n_Axis, 1000); 하면 된다
	
	//초당 50cm로 이동하려면 다음과 같이 셋팅하면 된다 
	// cmmCfgSetSpeedPattern(n_axis, cmSMODE_C, 50, 0,0);
	// cmmSxMove(n_axis, 10); //50cm/sec의 속도로 10cm 이동
	
	
	nFlag = cmmCfgSetUnitSpeed(n_Axis, n_UnitSpeed);
	if (nFlag == cmERR_NONE) 
	{
		nRet = BD_GOOD;
	}
	else
	{
		nRet = BD_ERROR;

		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[axis %d] cmmCfgSetUnitSpeed return error"), n_Axis);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//			sprintf(mc_normal_msg, "[axis %d] cmmCfgSetUnitSpeed return error", n_Axis);
			Debug_File_Create(0, mc_normal_msg);
		}
	}
	
	return nRet;
}

// ******************************************************************************
// 모터 속도 설정 함수                                                           
// -> nAxis      : 모터 속도 설정할 특정 축                                      
// -> nCommand   : 설정할 모터 속도 종류 [ACC/DEC/JOG 등]                        
// -> lParaValue : 설정할 모터 속도                                              
// ******************************************************************************
int CComizoaPublic::Set_MotSpeed(int n_SpeedCase, int n_Axis, int n_SpeedMode, double d_work, double d_accel_time, double d_decel_time) 
{
	int  nRet = BD_ERROR;
	double dVel = 0, dAccPPS = 0, dDecPPS = 0;
	double dAccTime = 0, dDecTime = 0;	
	int nLimit_Time = 1;
	CString strMsg;

	//n_SpeedCase => MOT_SPD_CM_CTL:속도를 cm로 제어한다, MOT_SPD_RPM_CTL: 속도를 rpm으로 제어 
	//앞으로는 가속 / 감속 값을 ms 단위로 받울 예정이니 다음과 같다 

	/////////////////////////////////////////////////////////////////////////////////////////////
	// MAX Accel / Decel Time 설정 
	/////////////////////////////////////////////////////////////////////////////////////////////
	if (d_accel_time < 10) //min 10 ms 보다 작게 가감속이 이루어 지면 기구적 강성 문제로 제한한다 
	{
 		d_accel_time = 50; //50 ms
	}
	else if (d_accel_time > 1000) //max
	{
 		d_accel_time = 1000;
	}

	if (d_decel_time < 10) //50 ms 보다 작게 가감속이 이루어 지면 기구적 강성 문제로 제한한다 
	{
 		d_decel_time = 50;
	}
	else if (d_decel_time > 1000)//max
	{
 		d_decel_time = 1000;
	}

	if (n_SpeedCase == MOT_SPD_CM_CTL) //2011.1104 
	{	// 이동 속도 설정을 초당 센치미터(cm/sec)로 제어시 사용한다 
		if (d_work < 1 || d_work > 2000) 
		{// 1:초당 1cm , max:초당 2000cm 이동 한계 설정
			d_work = 10;
		}
	}
	else
	{ //기타 다른 방법으로 속도 셋팅시 

	}
	
	dAccPPS = (d_work - md_initial_speed[n_Axis]) / (d_accel_time / 1000);
	dDecPPS = (d_work - md_initial_speed[n_Axis]) / (d_decel_time / 1000);

	

	if (dAccPPS > 1000) //2011.0122 30ms 까지 가감속 처리 13000000) //(dAccPPS > 6500000)
	{
 		dAccPPS = 1000; //2011.0122 30ms 까지 가감속 처리 13000000; //최소 50ms 저장 //6500000;
	}

	if (dDecPPS > 1000) //2011.0122 30ms 까지 가감속 처리 13000000) //(dDecPPS > 6500000)
	{
 		dDecPPS = 1000; //2011.0122 30ms 까지 가감속 처리 13000000; //6500000;
	}

	n_SpeedMode = cmSMODE_T; //동작모드 정의 

	if (n_SpeedCase == MOT_SPD_ORG ) //홈체크시 동작속도 셋팅 
	{			 
		//cmSMODE_KEEP=-1/* Keep previous setting*/, cmSMODE_C=0 /*Constant */, cmSMODE_T /*Trapeziodal*/, cmSMODE_S /*S-curve*/
		//2011.0121 변경 nRet = cmmHomeSetSpeedPattern(n_Axis, n_SpeedMode, d_work, dAccPPS, dDecPPS, 1000);

		if (dVel > 200 ) //최대 홈 체크 속도를 설정한다 
		{//6000 rpm 사용 
			dVel = 2;
		}

		nRet = cmmHomeSetSpeedPattern_T(n_Axis, cmSMODE_T, d_work, d_accel_time/(double)1000, d_decel_time/(double)1000, 1);//2011.0122 1000);

		if (nRet == cmERR_NONE)
		{			
			nRet = BD_GOOD;
		}
		else
		{
			if (mn_errormsg_debug_mode)
			{
				strMsg.Format(_T("[axis %d] cmmHomeSetSpeedPattern return error"), n_Axis);
				//wsprintfA(mc_normal_msg, "%S", strMsg);
				clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//				sprintf(mc_normal_msg, "[axis %d] cmmHomeSetSpeedPattern return error", n_Axis);
				Debug_File_Create(0, mc_normal_msg);
			}
			nRet = BD_ERROR;
		}
	}
	else //정상동작시 속도셋팅모드 
	{
		if (mn_run_status == CTL_dRUN)
		{
			if (mn_runspeed_rate < 10) mn_runspeed_rate = 50;
			dVel = d_work * (double)(((double)mn_runspeed_rate / (double)100)); //d_work
		}
		else
		{
			if (mn_manualspeed_rate < 10) mn_manualspeed_rate = 50;
			dVel = d_work * (double)(((double)mn_manualspeed_rate / (double)100)); //d_work
		}
	 
		if (dVel > 10000 )
		{//6000 rpm 사용 
 			dVel = 10000;
		}

		if(0 && n_Axis == 0)
		{
			nRet = cmmCfgSetSpeedPattern(0, n_SpeedMode, 100000, 600000, 600000);//2011.0122 1000);
		}
		else
		{
	 	  	nRet = cmmCfgSetSpeedPattern_T(n_Axis, n_SpeedMode, dVel, d_accel_time/(double)1000, d_decel_time/(double)1000);//2011.0122 1000);
		}
		


        if (nRet == cmERR_NONE)
		{
			nRet = BD_GOOD;
		}
		else
		{
			if (mn_errormsg_debug_mode)
			{
				strMsg.Format(_T("[axis %d] cmmCfgSetSpeedPattern return error"), n_Axis);
				//wsprintfA(mc_normal_msg, "%S", strMsg);
				clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//				sprintf(mc_normal_msg, "[axis %d] cmmCfgSetSpeedPattern return error", n_Axis);
				Debug_File_Create(0, mc_normal_msg);
			}
			nRet = BD_ERROR;
		}
	}

	return nRet;
}


int CComizoaPublic::Set_MotSpeed_Ratio(int n_Axis, int n_SpeedMode, double d_work_Ratio, double d_accel_Ratio, double d_decel_Ratio) 
{
	int  nRet = BD_ERROR;

	if(d_work_Ratio > 100 || d_work_Ratio < 10)
	{
		d_work_Ratio = (double)10;
	}
	
	if (d_accel_Ratio > 100 || d_accel_Ratio < 10)
	{
		d_accel_Ratio = (double)100;
	}
	
	if (d_decel_Ratio > 100 || d_decel_Ratio < 10)
	{
		d_decel_Ratio = (double)100;
	}

	cmmSxSetSpeedRatio(n_Axis, n_SpeedMode, d_work_Ratio, d_accel_Ratio, d_decel_Ratio);

	nRet = BD_GOOD;
	return nRet;
}
// ******************************************************************************




int CComizoaPublic::Set_MotUnitDist(int n_Axis, double n_UnitDist)
{
	int  nRet = BD_ERROR, nFlag = 0;
	CString strMsg;

	nFlag = cmmCfgSetUnitDist(n_Axis, n_UnitDist);
	if (nFlag == cmERR_NONE) 
	{
		nRet = BD_GOOD;
	}
	else
	{
		nRet = BD_ERROR;

		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[axis %d] cmmCfgSetUnitDist return error"), n_Axis);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//			sprintf(mc_normal_msg, "[axis %d] cmmCfgSetUnitDist return error", n_Axis);
			Debug_File_Create(0, mc_normal_msg);
		}
	}
	
	return nRet;
}




///////////////////////////////////////////////////////////////////////////////////
///// interrupt //////
///////////////////////////////////////////////////////////////////////////////////
int CComizoaPublic::Set_InterruptSetup(int n_Enable, int n_InterruptType)
{// 장비 내부에서 진행해야 한다 mainfrm.cpp에서 작성해야 함 
	int  nRet = BD_ERROR, nFlag = 0;


	if(n_Enable == BD_NO)
	{
		cmmIntHandlerEnable(FALSE); // Disable Interrupt Handling
		return BD_GOOD;
	}

	switch(n_InterruptType)
	{
	case cmIHT_MESSAGE: 
//		cmmIntHandlerSetup(cmIHT_MESSAGE, GetSafeHwnd(), WMU_MotInterrupt, this);
		break;
	case cmIHT_EVENT:
//		cmmIntHandlerSetup(cmIHT_EVENT, g_hEvent, 0, 0);
		break;
	case cmIHT_CALLBACK: 
//		cmmIntHandlerSetup(cmIHT_CALLBACK, InterruptCallback, WMU_MotInterrupt, this);
		break;
	}
	cmmIntHandlerEnable(TRUE); // Enable Interrupt Handling
// 
	return BD_GOOD;
}

int CComizoaPublic::Set_InterruptMask(int n_Axis, long l_Set_InterruptMask[20])
{//cmmIntSetMask
	int  nRet = BD_ERROR, nFlag = 0;
	long lMapMask = 0;
/*
	"Normal Stop", // 0
	"Succesive start", // 1
	"", // 2
	"", // 3
	"Start of acceleration", //4
	"End of acceleration", // 5
	"Start of deceleration", // 6
	"End of deceleration", // 7
	"", // 8
	"", // 9
	"Position error tolerance exceed", // 10
	"General Comparator", // 11
	"Compared trigger", // 12
	"CLR signal resets the counter value", // 13
	"LTC input making counter value latched", // 14
	"ORG input signal ON", // 15
	"SD input signal ON", // 16
	"+DR input changes", // 17
	"-DR input changes" // 18
*/	

	/*2012.1029 jamestest 
	for(i=0; i<19; i++) //i=최대 0~18까지 19가지 임 
	{	
		if(l_Set_InterruptMask[i] == BD_YES)
		{
			lMapMask = lMapMask | (long)pow(2, i);
		}
	}

	nFlag = cmmIntSetMask(n_Axis, lMapMask);
	if (nFlag == cmERR_NONE)
	{

		//lMapMask = 0;
		//cmmIntGetMask(n_Axis, &lMapMask);
		nRet = BD_GOOD;
	}
	else
	{
		nRet = BD_ERROR;
	}*/
	
	return nRet;
}

int CComizoaPublic::Get_InterruptEventData(int n_MaxAxis, long l_Get_InterruptMask[20])
{//cmmIntSetMask
	int  nRet = BD_ERROR, nFlag = 0, i;
	long dwErrState=0, dwEvtState=0;
	long dwFlag1=0, dwFlag2=0;
	int nAxis = 0;
/*
	"Normal Stop", // 0
	"Succesive start", // 1
	"", // 2
	"", // 3
	"Start of acceleration", //4
	"End of acceleration", // 5
	"Start of deceleration", // 6
	"End of deceleration", // 7
	"", // 8
	"", // 9
	"Position error tolerance exceed", // 10
	"General Comparator", // 11
	"Compared trigger", // 12
	"CLR signal resets the counter value", // 13
	"LTC input making counter value latched", // 14
	"ORG input signal ON", // 15
	"SD input signal ON", // 16
	"+DR input changes", // 17
	"-DR input changes" // 18
*/	

///*2012.1029 jamestest 
	//해당축이 Interrupt가 발생했는지 확인 
	cmmIntReadFlag(&dwFlag1, &dwFlag2);

	//인터럽트가 한 축이라도 발생 했는지 확인한다 
	if(n_MaxAxis >= 62 || (dwFlag1 < 1 && dwFlag2 < 1) ) //어느 모터도 인터럽트가 발생하지 않았다
	{
		return BD_ERROR;		
	}

	//어느축이 인터럽트가 발생했는지 확인한다 
	for(i=0; i<n_MaxAxis; i++)
	{
		if(dwFlag1 < 32) //0qjs ahxj ~ 31번 모터까지 총 32축에 대한 인터럽트 확인 가능 
		{
			if(dwFlag1 == pow((double)2, i)) //pow(2, i) 발생한 모터축의 번호와 비교 
			{
				nAxis = i;
				break;
			}
		}
		else  //32번 모터 ~ 63번 모터까지 총 32축에 대한 인터럽트 확인 가능 
		{
			if(dwFlag1 == pow((double)2, i)) //pow(2, i) 발생한 모터축의 번호와 비교 
			{
				nAxis = i;
				break;
			}
		}
	}

	if(mn_interrupt_flag[nAxis] != BD_YES) //인터럽트를 사용한 모터만 인터럽트 발생 여부를 체크 
	{
		return BD_PROCEED; //BD_GOOD;
	}
	
	//인터럽트 에러 상태 확인 
	if(cmmIntReadErrorStatus(nAxis, &dwErrState) == cmERR_NONE && dwErrState < 0)
	{//에러발생 
		return BD_ERROR;	//사용하지 않아도 된다 		
	}
	
	// 어떤 Event Interrupt가 발생하였는지 체크 //
	if(cmmIntReadEventStatus(nAxis, &dwEvtState) == cmERR_NONE  && dwEvtState != 0)
	{
		for(i=0; i<20; i++) // Event interrupt는 상태값의 b0 ~ b19 비트를 사용한다.
		{ 		
			ml_Get_InterruptMask[nAxis][i] = BD_NO;
			if(cmmGetBit(dwEvtState, i))
			{
				ml_Get_InterruptMask[nAxis][i] = BD_YES;  			
			}		 
		}		 
	}
 
	//현재 사용하는 비교문만 체크한다 
	if(ml_Get_InterruptMask[nAxis][12] == BD_YES) //CMP 모드 체크, CMP위치 인터럽트가 안전위치 이상으로 이동하면 발생  
	{
 
		return BD_GOOD; 
	}	 
//*/
	return BD_PROCEED;	
}

int CComizoaPublic::Get_InterruptFlag(int n_Axis)
{
	int  nRet = BD_ERROR, nFlag = 0;
	long dwFlag1, dwFlag2;

	cmmIntReadFlag(&dwFlag1, &dwFlag2);

	if(n_Axis < 32)
	{
		nFlag = cmmGetBit(dwFlag1, n_Axis);
	}
	else
	{
		nFlag = cmmGetBit(dwFlag2, (n_Axis - 32));
	}

	return nFlag; //해당 축이 인터럽트가 발생했으면 TRUE(BD_YES) 리턴 
}

///////////////////////////////////////////////////////////////////////////////////////
// ONE POINT COMPARE POSITION SETTING
///////////////////////////////////////////////////////////////////////////////////////
int CComizoaPublic::Set_TriggerSet_OneCompareConfig(int n_Axis, int n_Source, int n_Method, double d_ComparePosition)
{ 
	int  nRet = BD_GOOD, nFlag = 0;

	//n_Mode : 	szCompareSource[4][50] = { 
		//0				"COMMAND Counter",
		//1				"Feedback Counter",
		//2			    "Deviation Counter",
		//3			    "General Counter"

	//CHAR szCompareMethod[6][50] = {
		//0				"Disable",
		//1				"CMP == COUNTER (No direction)",
		//2				"CMP == COUNTER (Only when counting up)",
		//3				"CMP == COUNTER (Only when counting down)",
		//4				"CMP > COUNTER",
		//5				"CMP < COUNTER"
 

	//n_Type : 
	//case ONE_POINT_TRIGGER :
	//case REGULAR_INTERVAL_POINT :
	//case VOLUNTARY_INTERVAL_POINT :

//TCmCntr {cmCNT_COMM/*Command*/, cmCNT_FEED/*Feedback*/, cmCNT_DEV/*Deviation*/,cmCNT_GEN/*General*/, cmCNT_REM/*Remained*/

	nFlag = cmmCmpTrgSetConfig(n_Axis, n_Source, n_Method); //cmmCmpTrgSetConfig) (long Axis, long CmpSrc, long CmpMethod);
	if (nFlag != cmERR_NONE)
	{
		nRet = BD_ERROR; 
	}

	nFlag = cmmCmpTrgSetOneData(n_Axis, d_ComparePosition);
	if (nFlag != cmERR_NONE)
	{
		nRet = BD_ERROR; 
	}

	return nRet;
}
///////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Pick & Place 시의 위험 공간 이동하여 보간공작 List Motiom 
// 2008.9.1 James lee
////////////////////////////////////////////////////////////
int CComizoaPublic::Create_LmIndexMapAxis(long n_MapIndex, long n_AxisCount, long *np_AxisNum) //최대 4축까지 사용할 수 있도록 하자 
{////Lm에서는 n_MapIndexNum 은 최대 0 ~ 15까지 임 
	int  nRet = BD_ERROR, nFlag = 0, i;
	long nMapMask_1 = 0, nMapMask_2 = 0;
	CString strMsg;

	if(st_work_info.nSimulationMode == 1)
	{
		return BD_GOOD;
	}

	for(i=0; i<n_AxisCount; i++) //i=최대 0~4까지 4가지 임 
	{			
		if(np_AxisNum[i] <= 31) nMapMask_1 = nMapMask_1 | (long)pow((double)2, np_AxisNum[i]);
		else				    nMapMask_2 = nMapMask_2 | (long)pow((double)2, np_AxisNum[i]);	 
	}

	nFlag = cmmLmMapAxes(n_MapIndex, nMapMask_1, nMapMask_2); //Map index 번호는 반듯이 0~15까지의 숫자이어야 한다
	if (nFlag == cmERR_NONE)
	{
		nRet = BD_GOOD; //st_coordmot[n_MapIndex].n_indexmap_init = BD_YES;;
	}
	else
	{
		nRet = BD_ERROR; 

		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[n_MapIndex %d] cmmLmMapAxes return error"), n_MapIndex);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//			sprintf(mc_normal_msg, "[n_MapIndex %d] cmmLmMapAxes return error", n_MapIndex);
			Debug_File_Create(0, mc_normal_msg);
		}
	}

	return nRet;
}

int CComizoaPublic::Create_IxIndexMapAxis(long n_MapIndex, long n_AxisCount, long *np_AxisNum) //최대 4축까지 사용할 수 있도록 하자 
{////Ix에서는 n_MapIndexNum 은 최대 0 ~ 7까지 임 
	int  nRet = BD_ERROR, nFlag = 0, i;
	long nMapMask_1 = 0, nMapMask_2 = 0;
	CString strMsg;

	if(st_work_info.nSimulationMode == 1)
	{
		return BD_GOOD;
	}

	for(i=0; i<n_AxisCount; i++) //i=최대 0~3까지 4가지 임 
	{			
		if(np_AxisNum[i] <= 31)  nMapMask_1 = nMapMask_1 | (long)pow((double)2, np_AxisNum[i]);
		else					 nMapMask_2 = nMapMask_2 | (long)pow((double)2, np_AxisNum[i]);	 
	}

	nFlag = cmmIxMapAxes(n_MapIndex, nMapMask_1, nMapMask_2); //Map index 번호는 반드시 0~7까지의 숫자이어야 한다 
	if (nFlag == cmERR_NONE)
	{
		nRet = BD_GOOD; //st_linearmot[n_MapIndex].n_indexmap_init = BD_YES;
	}
	else
	{
		nRet = BD_ERROR; 

		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[n_MapIndex %d] cmmIxMapAxes return error"), n_MapIndex);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//			sprintf(mc_normal_msg, "[n_MapIndex %d] cmmIxMapAxes return error", n_MapIndex);
			Debug_File_Create(0, mc_normal_msg);
		}
	}
	return nRet;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//위치 오버라이딩 
///////////////////////////////////////////////////////////////////////////////////////////////
int CComizoaPublic::Set_Override_Move_Pos(int n_Axis, double d_TargetPos)  
{
	int  nRet = BD_ERROR, nFlag = 0;
	long lp_OverrideFlag = 0; 

	lp_OverrideFlag = 0; 
	nFlag = cmmOverrideMoveTo(n_Axis, d_TargetPos, &lp_OverrideFlag);

	if (nFlag == cmERR_NONE && lp_OverrideFlag == 0)  
	{
		if(lp_OverrideFlag == 0) //1:정상 수행, 0:수행안됨 //목표 위치 값 오버라이드 
		{
			nRet = BD_GOOD;
		}
	}
	else
	{

	}

	return nRet;
}




// ******************************************************************************
// 모터 이동 함수                                                                
// -> int nAxis        : 정의된 모터 ID 번호                                     
// -> int nRunMode     : 모터 MOVE 방식 [1:RUN MODE  2:MANUAL MODE]              
// -> double dDistance : 모터 이동 거리                                          
// -> double dVelocity : 모터 VEL 속도                                           
// ******************************************************************************
int CComizoaPublic::Start_SingleMove(int n_Axis, double d_Distance, int n_SpeedRatio) // (int ratio = 100) //2011.0105 변경 
{
	int  nRet = BD_ERROR, nFlag = 0;
	double d_SpeedVel;
	double dCurrPos = 0;
	long dwMotionDone=0;
	CString strMsg;
	double d_Speed[3];

	if(st_work_info.nSimulationMode == 1)
	{
		return BD_GOOD;
	}


	nRet = CTL_Lib.Motor_SafetyCheck(1, n_Axis, d_Distance);

	if(nRet == BD_ERROR)
	{
		cmmSxStop(n_Axis, FALSE, FALSE);  //2015.0407 james  모터 이동 정지 함수 

		return BD_ERROR;
	}
	else if (nRet == BD_RETRY)
	{
		if(mn_retry_cnt[n_Axis] < mn_max_retry_cnt) //2011.0201 조건 추가 
		{//반복 동작을 진행한다 
			if(mn_retry_cnt[n_Axis] == 0)
			{
				ml_retry_time_wait[n_Axis][0] = GetCurrentTime();
			}

			mn_retry_cnt[n_Axis]++ ;//2011.0224 추가 
			return BD_PROCEED;
		}
		else
		{
			ml_retry_time_wait[n_Axis][1] = GetCurrentTime();
			ml_retry_time_wait[n_Axis][2] = ml_retry_time_wait[n_Axis][1] - ml_retry_time_wait[n_Axis][0];
			if(ml_retry_time_wait[n_Axis][2] < mn_max_move_limit_time) //3초 이하 일떄는 반복 종작을 한다 
			{
				return BD_PROCEED;
			}
			else if(ml_retry_time_wait[n_Axis][2] < 0)
			{
				ml_retry_time_wait[n_Axis][0] = GetCurrentTime();
			}
			//반복 횟수 보다 크면 정상적으로 리턴 내용 처리 
		}

//		cmmSxStopEmg(n_Axis); //2009.6.30 추가 

		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[axis %d] SafetyCheck return error"), n_Axis);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//			sprintf(mc_normal_msg, "[axis %d] SafetyCheck return error", n_Axis);
			Debug_File_Create(0, mc_normal_msg);
		}

		// 20120523 jtkim
//		sprintf(Jamcode, "%02d0006", n_Axis); //on check error
		st_alarm_info.strCode.Format(_T("%02d0006"), n_Axis); 
		st_alarm_info.nCountMode	= 0;	
		st_alarm_info.nTypeMode		= eWARNING;
		st_alarm_info.nAlarmAssignSection = 0;

		return BD_ERROR;
	}


	/* 20150429 james motionstate로 변경 
	nRet = cmmSxIsDone(n_Axis, &dwMotionDone);			// Motion Done이냐?
	if (nRet == cmERR_NONE)
	{
		if(dwMotionDone != cmTRUE) //모션 동작 완료 == 1, 동작중은 0
		{			
//			cmmSxStopEmg(n_Axis); //091012 동작중이면 정지 시키고 다시 동작한다 
			cmmSxStop(n_Axis, FALSE, FALSE);  // 모터 이동 정지 함수 

			return BD_RETRY;

			//010013 E A "Motor motion done Check Error(01 motor)."
			strMsg.Format(_T("%02d0013"), n_Axis);
			//wsprintfA(mc_alarmcode, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//			sprintf(mc_alarmcode, "%02d0013", n_Axis);  
			if(mn_errormsg_debug_mode)
			{
				strMsg.Format(_T("[%d] Start_SingleMove cmmSxIsDone Check Error"), n_Axis);
				//wsprintfA(mc_normal_msg, "%S", strMsg);
				clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//				sprintf(mc_normal_msg, "[%d] Start_SingleMove cmmSxIsDone Check Error", n_Axis);
				Debug_File_Create(0, mc_normal_msg);
			}

//			cmmSxStopEmg(n_Axis);
			cmmSxStop(n_Axis, FALSE, FALSE);  // 모터 이동 정지 함수 
			
			if(mn_errormsg_debug_mode)
			{
				strMsg.Format(_T("[axis %d] cmmSxIsDone return error"), n_Axis);
				//wsprintfA(mc_normal_msg, "%S", strMsg);
				clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//				sprintf(mc_normal_msg, "[axis %d] cmmSxIsDone return error", n_Axis);
				Debug_File_Create(0, mc_normal_msg);
			}

			// 20120523 jtkim
			st_alarm_info.strCode.Format(_T("%02d0013"), n_Axis);
			st_alarm_info.nCountMode	= 0;	
			st_alarm_info.nTypeMode		= eWARNING;
			st_alarm_info.nAlarmAssignSection = 1;

			return BD_ERROR;
		}

		nRet = Get_Deviation_Position(n_Axis, md_allow_value[n_Axis], md_deviation[n_Axis]) ; //0.5 mm	
		if (nRet != BD_GOOD)
		{				
			ml_retry_time_wait[n_Axis][1] = GetCurrentTime();
			ml_retry_time_wait[n_Axis][2] = ml_retry_time_wait[n_Axis][1] - ml_retry_time_wait[n_Axis][0];
			
			if (ml_retry_time_wait[n_Axis][2] < 1000)
			{
				if( ml_retry_time_wait[n_Axis][2] < 0) //2015.0326 james 추가 
				{
					ml_retry_time_wait[n_Axis][0] = GetCurrentTime();
				}
				return BD_RETRY;
			}
			
			dCurrPos = Get_MotCurrentPos(n_Axis);  // 모터 특정 축의 현재 위치 리턴 함수
			
			// jtkim 20130823 확인 
//			if (mn_motor_type[n_Axis] == 0)
			///*2015.0214 james// COMI.Set_CMD_CTL_Mode(i, cmCTRL_SEMI_C);//cmCTRL_SEMI_C , 엔코더,(피드백기준) 기준으로 목표좌표를 설정하여 이송합니다(커멘드 정보는 무시되고 최종 feedback(엔코더) 위치로 제어가 이루어진다.
			//if (m_nMotInfo[n_Axis] == MOT_STEP)
			//{
				cmmStGetPosition(n_Axis, cmCNT_FEED, &dCurrPos);	//현재 커멘드 위치 
			//	cmmStSetPosition(n_Axis, cmCNT_COMM, dCurrPos); //엔코드 위치와 커멘드 위치를 통일 시킨다 
			//}/
			
			if(mn_errormsg_debug_mode)
			{
				//wsprintfA(mc_normal_msg, "Axis=%d, CMDPOS[%4.3f] FEEDPOS[%4.3f]", n_Axis, d_Distance, dCurrPos);
				//Debug_File_Create(0, mc_normal_msg);
			}
		}
	}
	else
	{//함수 리턴 에러 
		if (mn_retry_cnt[n_Axis] > mn_max_retry_cnt)  
		{			
			cmmSxStop(n_Axis, FALSE, FALSE);  // 모터 이동 정지 함수 

			//010001 E A "Motor Parametor Setting Error(01 motor)."
			if(mn_errormsg_debug_mode)
			{
				//wsprintfA(mc_normal_msg, "[%d] cmmSxIsDone start single_return", n_Axis);
				//Debug_File_Create(0, mc_normal_msg);
			}

			// 20120523 jtkim
//			sprintf(Jamcode, "%02d0006", n_Axis); //on check error
			st_alarm_info.strCode.Format(_T("%02d0006"), n_Axis);
			st_alarm_info.nCountMode	= 0;	
			st_alarm_info.nTypeMode		= eWARNING;
			st_alarm_info.nAlarmAssignSection = 2;

			return BD_ERROR;
		}
		else
		{
			mn_retry_cnt[n_Axis]++;  
		}
	}*/

//20150429 james 보완, 정지 상태를 확인하는 커멘드 변경 
	nRet = Check_Motion_State(n_Axis,  cmMST_STOP); 
	if(nRet == BD_GOOD)
	{
		//모터는 정지되어 있는 상태이다 //정상 
	}
	else if(nRet == BD_PROCEED)
	{
		return BD_PROCEED;
	}
	else //if(nRet == BD_ERROR)
	{
		if (mn_retry_cnt[n_Axis] > mn_max_retry_cnt)  
		{			
			cmmSxStop(n_Axis, FALSE, FALSE);  // 모터 이동 정지 함수 

			//010001 E A "Motor Parametor Setting Error(01 motor)."
			if(mn_errormsg_debug_mode)
			{
				//wsprintfA(mc_normal_msg, "[%d] cmmSxIsDone start single_return", n_Axis);
				//Debug_File_Create(0, mc_normal_msg);
			}

			// 20120523 jtkim
//			sprintf(Jamcode, "%02d0006", n_Axis); //on check error
			st_alarm_info.strCode.Format(_T("%02d0006"), n_Axis);
			st_alarm_info.nCountMode	= 0;	
			st_alarm_info.nTypeMode		= eWARNING;
			st_alarm_info.nAlarmAssignSection = 2;

			return BD_ERROR;
		}
		else
		{
			mn_retry_cnt[n_Axis]++;  
			return BD_PROCEED;
		}		
	}

	if(n_SpeedRatio > 10 || n_SpeedRatio < 500) //최소 10% ~ 500% 까지 속도를 가변 할 수 있다 
	{//d_SpeedRatio = 100, 셋팅된 속도 100=100%, 200=200%, 50=50% 
		d_SpeedVel = (double)(md_spd_vel[n_Axis][0] * ((double)n_SpeedRatio / (double)100));//mn_spd_rate_mnu
	}
	else if(n_SpeedRatio >= 0)
	{
		d_SpeedVel = (double)(md_spd_vel[n_Axis][0] * 0.1); //10% 속도로 리미트 셋팅 
	}
	else //if(n_SpeedRatio < 0 && n_SpeedRatio < -500)
	{//- 값의 셋팅은 정속의 / 셋팅 값을 정의한다 
		d_SpeedVel = (double)(md_spd_vel[n_Axis][0] / (double)n_SpeedRatio);
	}

	d_Speed[0] = d_SpeedVel; d_Speed[1] = md_spd_vel[n_Axis][1]; d_Speed[2] = md_spd_vel[n_Axis][2];
	
	/*2015.0326 james 
	if(d_Speed[0] < 1000) //속도가 이값보다 작으면 정해진 속도로 이동하자 
	{	//이미 천천히 움직이는 속도 
//20111212 jtkim		d_Speed[0] = 1000; //d_SpeedVel;
	}*/

	//2015.0326 james 
	if(d_Speed[0] < 0) //속도가 이값보다 작으면 정해진 속도로 이동하자 
	{	//이미 천천히 움직이는 속도 
		d_Speed[0] = 10; //d_SpeedVel;
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//V_0.1.1 nRet = Set_MotSpeed(MOT_SPD_VEL, n_Axis, cmSMODE_T, dp_SpeedVel, st_motor[n_Axis].d_spd_vel[1], st_motor[n_Axis].d_spd_vel[2]);
	nRet = Set_MotSpeed(MOT_SPD_VEL, n_Axis, cmSMODE_T, d_Speed[0], d_Speed[1], d_Speed[2]); //V_0.1.1 
	if(nRet == BD_ERROR)
	{
		//010002 E A "Motor Move Setting Error01 motor)."
		strMsg.Format(_T("%02d0002"), n_Axis);
		//wsprintfA(mc_alarmcode, "%S", strMsg);
		clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//		sprintf(mc_alarmcode, "%02d0002", n_Axis); 
		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[%d] Set_MotSpeed start single_return"), n_Axis);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//			sprintf(mc_normal_msg, "[%d] Set_MotSpeed start single_return", n_Axis);
			Debug_File_Create(0, mc_normal_msg);
		}
		
		// 20120523 jtkim
//		sprintf(Jamcode, "%02d0015", n_Axis); //on check error
		st_alarm_info.strCode.Format(_T("%02d0015"), n_Axis);
		st_alarm_info.nCountMode	= 0;	
		st_alarm_info.nTypeMode		= eWARNING;
		st_alarm_info.nAlarmAssignSection = 3;

		return BD_ERROR;
	}

	nRet = cmmSxSetSpeedRatio(n_Axis, cmSMODE_T, 100,100,100); //cmSMODE_KEEP, 100, 100, 100);	
	if(nRet == BD_ERROR)
	{
		//010002 E A "Motor Move Setting Error01 motor)."
		strMsg.Format(_T("%02d0002"), n_Axis);
		//wsprintfA(mc_alarmcode, "%S", strMsg);
		clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//		sprintf(mc_alarmcode, "%02d0002", n_Axis); 
		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[%d] cmmSxSetSpeedRatio start single_return"), n_Axis);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//			sprintf(mc_normal_msg, "[%d] cmmSxSetSpeedRatio start single_return", n_Axis);
			Debug_File_Create(0, mc_normal_msg);
		} 

		// 20120523 jtkim
//		sprintf(Jamcode, "%02d0015", n_Axis); //on check error
		st_alarm_info.strCode.Format(_T("%02d0015"), n_Axis);
		st_alarm_info.nCountMode	= 0;	
		st_alarm_info.nTypeMode		= eWARNING;
		st_alarm_info.nAlarmAssignSection = 4;

		return BD_ERROR;
	}

	nRet = cmmSxMoveToStart(n_Axis, d_Distance);
	if (nRet == cmERR_NONE)
	{
		ml_motion_move_time[n_Axis][0] = GetCurrentTime();
		
		if(mn_run_status == CTL_dRUN)	//090825 handler가 정상가동중일때만 커멘드 데이터를 백업, 메뉴얼 동작은 스킵 
		{
			md_cmdpos_backup[n_Axis] = d_Distance;
		}

		mn_interrupt_flag[n_Axis] = BD_NO;
		return BD_GOOD;
	}
	else
	{
/* james test 
		nRet = cmmCfgGetSpeedPattern(n_Axis, &lErrorCode, &dRatio[0], &dRatio[1], &dRatio[2]);
		if (nRet != cmERR_NONE) //james test
		{
			return BD_ERROR;
		}
		cmmSxGetSpeedRatio(n_Axis, &lErrorCode, &dRatio[0], &dRatio[1],&dRatio[2]);
*/
		
		nRet = Get_Deviation_Position(n_Axis, (double)md_allow_value[n_Axis], md_deviation[n_Axis])  ; //0.5 mm

		if(nRet == BD_ERROR)
		{			
			if (mn_retry_cnt[n_Axis] > mn_max_retry_cnt)  
			{
				//010002 E A "Motor Move Setting Error01 motor)."
				strMsg.Format(_T("%02d0002"), n_Axis);
				//wsprintfA(mc_alarmcode, "%S", strMsg);
				clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//				sprintf(mc_alarmcode, "%02d0002", n_Axis); 
				if(mn_errormsg_debug_mode)
				{
					strMsg.Format(_T("[%d] Target= %4.3f, CmdPos= %4.3f, FeedPos=%4.3f"), n_Axis, d_Distance, md_deviation[n_Axis][0], md_deviation[n_Axis][1]);
					//wsprintfA(mc_normal_msg, "%S", strMsg);
					clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//					sprintf(mc_normal_msg, "[%d] Target= %4.3f, CmdPos= %4.3f, FeedPos=%4.3f", n_Axis, d_Distance, md_deviation[n_Axis][0], md_deviation[n_Axis][1]);
					Debug_File_Create(0, mc_normal_msg);
				} 
				dCurrPos = Get_MotCurrentPos(n_Axis);  // 모터 특정 축의 현재 위치 리턴 함수				 

//				cmmSxStopEmg(n_Axis);
				cmmSxStop(n_Axis, FALSE, FALSE);  // 모터 이동 정지 함수 
				
				mn_retry_cnt[n_Axis] = 0;

				// 20120523 jtkim
//				sprintf(Jamcode, "%02d0006", n_Axis); //on check error
				st_alarm_info.strCode.Format(_T("%02d0006"), n_Axis);
				st_alarm_info.nCountMode	= 0;	
				st_alarm_info.nTypeMode		= eWARNING;
				st_alarm_info.nAlarmAssignSection = 5;

				return BD_ERROR;
			}
			else
			{
				mn_retry_cnt[n_Axis]++ ;
				return BD_RETRY;								
			}			
		}
	/*			 
		lErrorCode = nRet;
		cmmErrGetLastCode(&lErrorCode);						//마지막 발생한 에러 코드 확인  
		nErrorParseAxis = cmmErrParseAxis(lErrorCode);     //에러를 발생시킨 축 정보를 가져온다 
		nErrorParseReason = cmmErrParseReason(lErrorCode); //error code를 받는다 
		cmmErrGetString(lErrorCode, cErrorMsg, 200);
		
		sMsg.Format("Msg=%S", cErrorMsg);
*/

		//010002 E A "Motor Move Setting Error 01 motor)."
		strMsg.Format(_T("%02d0002"), n_Axis);
		//wsprintfA(mc_alarmcode, "%S", strMsg);
		clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//		sprintf(mc_alarmcode, "%02d0002", n_Axis); 
		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[%d] Target= %4.3f, CurPos= %4.3f"), n_Axis, d_Distance, dCurrPos);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//			sprintf(mc_normal_msg, "[%d] Target= %4.3f, CurPos= %4.3f", n_Axis, d_Distance, dCurrPos);
			Debug_File_Create(0, mc_normal_msg);
		} 

		dCurrPos = Get_MotCurrentPos(n_Axis);  // 모터 특정 축의 현재 위치 리턴 함수
//		cmmSxStopEmg(n_Axis);
		cmmSxStop(n_Axis, FALSE, FALSE);  // 모터 이동 정지 함수 

		// 20120523 jtkim
//		sprintf(Jamcode, "%02d0006", n_Axis); //on check error
		st_alarm_info.strCode.Format(_T("%02d0006"), n_Axis);
		st_alarm_info.nCountMode	= 0;	
		st_alarm_info.nTypeMode		= eWARNING;
		st_alarm_info.nAlarmAssignSection = 6;

		return BD_ERROR;
	}

	return BD_ERROR;
}
// ******************************************************************************

// ******************************************************************************
// 모터 이동 위치 정밀도 검사 함수                                               
// -> int nAxis        : 정의된 모터 ID 번호                                     
// -> double dDistance : 모터 이동 거리                                          
// ******************************************************************************
int CComizoaPublic::Check_SingleMove(int n_Axis, double d_Distance) 
{
	int    nRet=0, nRetDone;
	double dCurrPos[4]={0,0,0,0};
	long dwMotionDone=0;
	CString strMsg;

	if(st_work_info.nSimulationMode == 1)
	{
		return BD_GOOD;
	}

	nRet = CTL_Lib.Motor_SafetyCheck(2, n_Axis, d_Distance);

	if (nRet == BD_ERROR)
	{		
		cmmSxStop(n_Axis, FALSE, FALSE);  //2015.0407 james  모터 이동 정지 함수 

		return BD_ERROR;
	}
	else if (nRet == BD_RETRY)
	{
		if (mn_retry_cnt[n_Axis] > mn_max_retry_cnt)   
		{//반복 동작을 진행한다 
			if(mn_retry_cnt[n_Axis] == 0)
			{
				ml_retry_time_wait[n_Axis][0] = GetCurrentTime();
			}
			mn_retry_cnt[n_Axis]++ ;//2011.0224 추가 
			return BD_PROCEED;
		}
		else
		{
			//반복 횟수 보다 크면 정상적으로 리턴 내용 처리 
			ml_retry_time_wait[n_Axis][1] = GetCurrentTime();
			ml_retry_time_wait[n_Axis][2] = ml_retry_time_wait[n_Axis][1] - ml_retry_time_wait[n_Axis][0];
			if(ml_retry_time_wait[n_Axis][2] < 500) //500ms 이하 일때는 반복 동작을 한다 
			{
				return BD_PROCEED; //2011.0224 추가 적용 //우선 기능 제외 return BD_RET_PROCEED;
			}
			else if(ml_retry_time_wait[n_Axis][2] < 0)
			{
				ml_retry_time_wait[n_Axis][0] = GetCurrentTime();
			}
		}

		mn_retry_cnt[n_Axis] = 0;  
//		cmmSxStopEmg(n_Axis);  
		cmmSxStop(n_Axis, FALSE, FALSE);  // 모터 이동 정지 함수 
		return BD_SAFETY;  
	}	

	//st_motor[n_Axis].d_cmd_feed_allow = 0.1; //0.1.mm 오차발생시 에러 체크 

	ml_motion_move_time[n_Axis][1] = GetCurrentTime();
	ml_motion_move_time[n_Axis][2] = ml_motion_move_time[n_Axis][1] - ml_motion_move_time[n_Axis][0];

	//james test nRet = Get_MotMotionDone(n_Axis);  // 모터 이동 완료 검사 함수 
	nRetDone = cmmSxIsDone(n_Axis, &dwMotionDone);			// Motion Done이냐?
	if (nRetDone == cmERR_NONE)
	{
		if(dwMotionDone == cmTRUE) //모션 동작 완료 == 1, 동작중은 0
		{	
			if(mn_sd_io_property_flag[n_Axis] == BD_YES) 
			{
				COMI.Set_Motor_IO_Property(n_Axis, cmSD_EN, cmFALSE); //cmINP_EN=9 //cmFALSE = 0 INP 비활성, cmTRUE = 1 INP 활성 
			}
			

			nRet = Check_MotPosRange(n_Axis, d_Distance, md_allow_value[n_Axis]); //(int n_Axis, double d_Distance, int n_Range) 
 
			if(nRet == BD_GOOD)
			{
				mn_retry_cnt[n_Axis] = 0; 
				return BD_GOOD;// 
			}
			else
			{
				if (ml_motion_move_time[n_Axis][2] > 2000)//2015.0217 james mn_max_move_limit_time) //MOT_MOVE_LIMITTIME) //모터 동작 완료 상태이니 기다릴 필요 없음 MOT_MOVE_LIMITTIME)
				{	
					if(mn_sd_io_property_flag[n_Axis] == BD_YES) 
					{
						COMI.Set_Motor_IO_Property(n_Axis, cmSD_EN, cmFALSE); //cmINP_EN=9 //cmFALSE = 0 INP 비활성, cmTRUE = 1 INP 활성 
					}

					
					if (mn_retry_cnt[n_Axis] > mn_max_retry_cnt)
					{
						mn_retry_cnt[n_Axis] = 0; 

						dCurrPos[0] = Get_MotCurrentPos(n_Axis);  // 모터 특정 축의 현재 위치 리턴 함수						 
//						cmmSxStopEmg(n_Axis);
						cmmSxStop(n_Axis, FALSE, FALSE);  // 모터 이동 정지 함수 

						//010002 E A "Motor Move Setting Error 01 motor)."
						strMsg.Format(_T("%02d0002"), n_Axis);
						//wsprintfA(mc_alarmcode, "%S", strMsg);
						clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//						sprintf(mc_alarmcode, "%02d0002", n_Axis); 
						if(mn_errormsg_debug_mode)
						{
							strMsg.Format(_T("[%d] Check_MotPosRange Target= %4.3f, CurPos= %4.3f"), n_Axis, d_Distance, dCurrPos[0]);
							//wsprintfA(mc_normal_msg, "%S", strMsg);
							clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//							sprintf(mc_normal_msg, "[%d] Check_MotPosRange Target= %4.3f, CurPos= %4.3f", n_Axis, d_Distance, dCurrPos[0]);
							Debug_File_Create(0, mc_normal_msg);
						} 

						// 20120523 jtkim
//						sprintf(Jamcode, "%02d0002", n_Axis); //on check error
						st_alarm_info.strCode.Format(_T("%02d0002"), n_Axis);
						st_alarm_info.nCountMode	= 0;	
						st_alarm_info.nTypeMode		= eWARNING;
						st_alarm_info.nAlarmAssignSection = 7;

						return BD_ERROR;
					}
					else
					{
						mn_retry_cnt[n_Axis]++ ;
						return BD_RETRY;								
					}
				}
				else if(ml_motion_move_time[n_Axis][2] < 0)
				{
					ml_motion_move_time[n_Axis][0] = GetCurrentTime();
				}
			}
		}
		else 
		{//모터 동작중일때 
			if (ml_motion_move_time[n_Axis][2] > mn_max_move_limit_time)
			{			
				if (mn_retry_cnt[n_Axis] > mn_max_retry_cnt)
				{
					mn_retry_cnt[n_Axis] = 0; 

						dCurrPos[0] = Get_MotCurrentPos(n_Axis);  // 모터 특정 축의 현재 위치 리턴 함수						 
//						cmmSxStopEmg(n_Axis);
						cmmSxStop(n_Axis, FALSE, FALSE);  // 모터 이동 정지 함수 

						//010002 E A "Motor Move Setting Error 01 motor)."
						strMsg.Format(_T("%02d0002"), n_Axis);
						//wsprintfA(mc_alarmcode, "%S", strMsg);
						clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//						sprintf(mc_alarmcode, "%02d0002", n_Axis); 
						if(mn_errormsg_debug_mode)
						{
							strMsg.Format(_T("[%d] Check_MotPosRange Target= %4.3f, CurPos= %4.3f"), n_Axis, d_Distance, dCurrPos[0]);
							//wsprintfA(mc_normal_msg, "%S", strMsg);
							clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//							sprintf(mc_normal_msg, "[%d] Check_MotPosRange Target= %4.3f, CurPos= %4.3f", n_Axis, d_Distance, dCurrPos[0]);
							Debug_File_Create(0, mc_normal_msg);
						}
						
						// 20120523 jtkim
//						sprintf(Jamcode, "%02d0002", n_Axis); //on check error
						st_alarm_info.strCode.Format(_T("%02d0002"), n_Axis); 
						st_alarm_info.nCountMode	= 0;	
						st_alarm_info.nTypeMode		= eWARNING;
						st_alarm_info.nAlarmAssignSection = 8;

						return BD_ERROR;
				}
				else
				{//2011.0305 
					mn_retry_cnt[n_Axis]++ ;
					return BD_RETRY;	
				}
			}	
			else if(ml_motion_move_time[n_Axis][2] < 0)
			{
				ml_motion_move_time[n_Axis][0] = GetCurrentTime();
			}
		}	
	}
	else 
	{//리턴 값이 에러일때 
		if(mn_sd_io_property_flag[n_Axis] == BD_YES)
		{
			COMI.Set_Motor_IO_Property(n_Axis, cmSD_EN, cmFALSE); //cmINP_EN=9 //cmFALSE = 0 INP 비활성, cmTRUE = 1 INP 활성 
		}
		

		if (mn_retry_cnt[n_Axis] > mn_max_retry_cnt)
		{
			mn_retry_cnt[n_Axis] = 0; 

				dCurrPos[0] = Get_MotCurrentPos(n_Axis);  // 모터 특정 축의 현재 위치 리턴 함수						 
//				cmmSxStopEmg(n_Axis);
				cmmSxStop(n_Axis, FALSE, FALSE);  // 모터 이동 정지 함수 

				//010002 E A "Motor Move Setting Error 01 motor)."
				strMsg.Format(_T("%02d0002"), n_Axis);
				//wsprintfA(mc_alarmcode, "%S", strMsg);
				clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//				sprintf(mc_alarmcode, "%02d0002", n_Axis); 
				if(mn_errormsg_debug_mode)
				{
					strMsg.Format(_T("[%d] Check_MotPosRange Target= %4.3f, CurPos= %4.3f"), n_Axis, d_Distance, dCurrPos[0]);
					//wsprintfA(mc_normal_msg, "%S", strMsg);
					clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//					sprintf(mc_normal_msg, "[%d] Check_MotPosRange Target= %4.3f, CurPos= %4.3f", n_Axis, d_Distance, dCurrPos[0]);
					Debug_File_Create(0, mc_normal_msg);
				} 

				// 20120523 jtkim
//				sprintf(Jamcode, "%02d0002", n_Axis); //on check error
				st_alarm_info.strCode.Format(_T("%02d0002"), n_Axis);
				st_alarm_info.nCountMode	= 0;	
				st_alarm_info.nTypeMode		= eWARNING;
				st_alarm_info.nAlarmAssignSection = 9;

				return BD_ERROR;
		}
		else
		{//2011.0305 
			mn_retry_cnt[n_Axis]++ ;
			return BD_RETRY;	
		}
	}
	
	return BD_PROCEED;
}

// ******************************************************************************

// ******************************************************************************
// Linear Move  다중 모터 이동 함수                                                           
// ******************************************************************************
int CComizoaPublic::Start_LinearMove(int n_MapIndex, double *dp_PosList, double *dp_SpdRatio)  //n_MapIndex는 미리 셋팅되어 있어야 한다 
{//2축 이상 동작 
 
	int    i, nRet=-1;
	double dTargetPos[4]={0,};
	long nListIsDone = 0;
	double dp_Vel[4]={0,0,0, 0};//2015.0326 james 최대 4축까지 보검 동작 가능하여 변경 dp_Vel[3]={0,0,0};
	CString strMsg;
//	char Jamcode[10];

	if(st_work_info.nSimulationMode == 1)
	{// 
		return BD_GOOD;
	}



	if(mn_indexmap_init[n_MapIndex] == NO || n_MapIndex >= 8) 
	{//초기화 되지 않은 상태 
		nRet = Create_IxIndexMapAxis(n_MapIndex, ml_axiscnt[n_MapIndex], mp_axisnum[n_MapIndex]);
		if(nRet == BD_GOOD && n_MapIndex < 8)
		{
			mn_indexmap_init[n_MapIndex]  = BD_YES;
		}
		else //if(nRet == BD_ERROR)
		{
			//010015 E A "Interpolation IxMapAxis setting Error(01 motor)."
			strMsg.Format(_T("%02d0015"), n_MapIndex);
			//wsprintfA(mc_alarmcode, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//			sprintf(mc_alarmcode, "%02d0015", n_MapIndex); 
			if(mn_errormsg_debug_mode)
			{
				strMsg.Format(_T("[%d] Create_IxIndexMapAxis return error"), n_MapIndex);
				//wsprintfA(mc_normal_msg, "%S", strMsg);
				clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//				sprintf(mc_normal_msg, "[%d] Create_IxIndexMapAxis return error", n_MapIndex);
				Debug_File_Create(0, mc_normal_msg);
			}

			// 20120523 jtkim
//			sprintf(Jamcode, "%02d0006", mp_axisnum[n_MapIndex][0]); //on check error
			st_alarm_info.strCode.Format(_T("%02d0006"), mp_axisnum[n_MapIndex][0]);
			st_alarm_info.nCountMode	= 0;	
			st_alarm_info.nTypeMode		= eWARNING;
			st_alarm_info.nAlarmAssignSection = 10;

			return BD_ERROR;  
		}
	}

	if (ml_axiscnt[n_MapIndex] < 2 || ml_axiscnt[n_MapIndex] > 4) //2015.0326 james 최대 4축까지 동작 가등하다 
	{
		//010015 E A "Interpolation IxMapAxis setting Error(01 motor)."
		strMsg.Format(_T("%02d0015"), mp_axisnum[n_MapIndex][0]);
		//wsprintfA(mc_alarmcode, "%S", strMsg);
		clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//		sprintf(mc_alarmcode, "%02d0015", n_MapIndex); 
		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[%d] ml_axiscnt[n_MapIndex] < 2 return error"), n_MapIndex);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//			sprintf(mc_normal_msg, "[%d] ml_axiscnt[n_MapIndex] < 2 return error", n_MapIndex);
			Debug_File_Create(0, mc_normal_msg);
		}

		// 20120523 jtkim
//		sprintf(Jamcode, "%02d0015", mp_axisnum[n_MapIndex][0]); //on check error
		st_alarm_info.strCode.Format(_T("%02d0015"), mp_axisnum[n_MapIndex][0]);
		st_alarm_info.nCountMode	= 0;	
		st_alarm_info.nTypeMode		= eWARNING;
		st_alarm_info.nAlarmAssignSection = 11;

		return BD_ERROR;
	}

	for(i=0; i<ml_axiscnt[n_MapIndex]; i++)
	{
		nRet = CTL_Lib.Motor_SafetyCheck(1, mp_axisnum[n_MapIndex][i], dp_PosList[i]);
		if (nRet == BD_ERROR)
		{	//safety error 는 해당 함수에서 코드 정리 
			cmmIxStopEmg(n_MapIndex);			
			// 20120523 jtkim
//			sprintf(Jamcode, "%02d0006", mp_axisnum[n_MapIndex][i]); //on check error
			//st_alarm_info.strCode.Format(_T("%02d0006"), mp_axisnum[n_MapIndex][i]);
			st_alarm_info.nCountMode	= 0;	
			st_alarm_info.nTypeMode		= eWARNING;
			st_alarm_info.nAlarmAssignSection = 12;

			return BD_ERROR;

		}
		else if(nRet == BD_RETRY)  
		{
			return BD_RETRY;
		}
//   		Set_MotSpeed(MOT_SPD_VEL, st_linearmot[n_MapIndex].lp_AxisNum[i], cmSMODE_S,
//   			st_motor[st_linearmot[n_MapIndex].lp_AxisNum[i]].d_spd_vel[0], 
//   			st_motor[st_linearmot[n_MapIndex].lp_AxisNum[i]].d_spd_vel[1], 
//   			st_motor[st_linearmot[n_MapIndex].lp_AxisNum[i]].d_spd_vel[2]);

		dTargetPos[i] = dp_PosList[i];
	}

	//2015.0326 james 수정 각각의 모터 속도 셋팅
	for(i = 0; i < ml_axiscnt[n_MapIndex]; i++)
	{
		dp_Vel[i] = (double)(md_spd_vel[mp_axisnum[n_MapIndex][i]][0] * ((double)dp_SpdRatio[0] / (double)100));
		nRet = Set_MotSpeed(MOT_SPD_VEL, mp_axisnum[n_MapIndex][i], cmSMODE_T, 
			dp_Vel[i], md_spd_vel[mp_axisnum[n_MapIndex][i]][1], md_spd_vel[mp_axisnum[n_MapIndex][i]][2]);  //((double)n_SpeedRatio / (double)100)) 

		/* 2015.0223 james 
		nRet = Set_MotSpeed(MOT_SPD_VEL, mp_axisnum[n_MapIndex][0], cmSMODE_T, 
			md_spd_vel[mp_axisnum[n_MapIndex][0]][0], md_spd_vel[mp_axisnum[n_MapIndex][0]][1], md_spd_vel[mp_axisnum[n_MapIndex][0]][2]);  
			*/
		if(nRet == BD_ERROR)
		{
			//010015 E A "Interpolation IxMapAxis setting Error(01 motor)."
			strMsg.Format(_T("%02d0015"), n_MapIndex);
			//wsprintfA(mc_alarmcode, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_alarmcode);
			//		sprintf(mc_alarmcode, "%02d0015", n_MapIndex); 
			if(mn_errormsg_debug_mode)
			{
				strMsg.Format(_T("[%d][%d] Create_IxIndexMapAxis Set_MotSpeed_0 return error"), n_MapIndex, mp_axisnum[n_MapIndex][i]);
				//wsprintfA(mc_normal_msg, "%S", strMsg);
				clsFunc.OnStringToChar(strMsg, mc_normal_msg);
				//			sprintf(mc_normal_msg, "[%d] Create_IxIndexMapAxis Set_MotSpeed_0 return error", n_MapIndex);
				Debug_File_Create(0, mc_normal_msg);
			}

			// 20120523 jtkim
			//		sprintf(Jamcode, "%02d0006", mp_axisnum[n_MapIndex][0]); //on check error
			st_alarm_info.strCode.Format(_T("%02d0006"), mp_axisnum[n_MapIndex][i]);
			st_alarm_info.nCountMode = 0;	
			st_alarm_info.nTypeMode = eWARNING;
			st_alarm_info.nAlarmAssignSection = 13;

			return BD_ERROR;
		}
	}

/*3025.0326 james 
	dp_Vel[0] = (double)(md_spd_vel[mp_axisnum[n_MapIndex][0]][0] * ((double)dp_SpdRatio[0] / (double)100));

	nRet = Set_MotSpeed(MOT_SPD_VEL, mp_axisnum[n_MapIndex][0], cmSMODE_T, 
		dp_Vel[0], md_spd_vel[mp_axisnum[n_MapIndex][0]][1], md_spd_vel[mp_axisnum[n_MapIndex][0]][2]);  //((double)n_SpeedRatio / (double)100)) 

	 
	if(nRet == BD_ERROR)
	{
		//010015 E A "Interpolation IxMapAxis setting Error(01 motor)."
		strMsg.Format(_T("%02d0015"), n_MapIndex);
		//wsprintfA(mc_alarmcode, "%S", strMsg);
		clsFunc.OnStringToChar(strMsg, mc_alarmcode);
		//		sprintf(mc_alarmcode, "%02d0015", n_MapIndex); 
		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[%d] Create_IxIndexMapAxis Set_MotSpeed_0 return error"), n_MapIndex);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
			//			sprintf(mc_normal_msg, "[%d] Create_IxIndexMapAxis Set_MotSpeed_0 return error", n_MapIndex);
			Debug_File_Create(0, mc_normal_msg);
		}

		// 20120523 jtkim
		//		sprintf(Jamcode, "%02d0006", mp_axisnum[n_MapIndex][0]); //on check error
		st_alarm_info.strCode.Format(_T("%02d0006"), mp_axisnum[n_MapIndex][0]);
		st_alarm_info.nCountMode = 0;	
		st_alarm_info.nTypeMode = eWARNING;
		st_alarm_info.nAlarmAssignSection = 13;

		return BD_ERROR;
	}

	dp_Vel[0] = (double)(md_spd_vel[mp_axisnum[n_MapIndex][1]][0] * ((double)dp_SpdRatio[0] / (double)100));
	nRet = Set_MotSpeed(MOT_SPD_VEL, mp_axisnum[n_MapIndex][1], cmSMODE_T, 
		dp_Vel[0], md_spd_vel[mp_axisnum[n_MapIndex][1]][1], md_spd_vel[mp_axisnum[n_MapIndex][1]][2]); 
	
	 
	if(nRet == BD_ERROR)
	{
		//500000 D A "Interpolation IxMapAxis 0번 Setting Error."
		strMsg.Format(_T("5000%02dr"), n_MapIndex);
		clsFunc.OnStringToChar(strMsg, mc_alarmcode);
		//		sprintf(mc_alarmcode, "5000%02d", n_MapIndex); 
		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[%d] Create_IxIndexMapAxis Set_MotSpeed_1 return error"), n_MapIndex);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
			//			sprintf(mc_normal_msg, "[%d] Create_IxIndexMapAxis Set_MotSpeed_1 return error", n_MapIndex);
			Debug_File_Create(0, mc_normal_msg);
		}

		// 20120523 jtkim
		//		sprintf(Jamcode, "%02d0015", mp_axisnum[n_MapIndex][0]); //on check error
		st_alarm_info.strCode.Format(_T("%02d0015"), mp_axisnum[n_MapIndex][0]);
		st_alarm_info.nCountMode	= 0;	
		st_alarm_info.nTypeMode		= eWARNING;
		st_alarm_info.nAlarmAssignSection = 14;

		return BD_ERROR;
	}
	*/
	
// IsVertorSpeed : cmmIxSetSpeedPattern함수의 인자이며, TRUE로 설정했을 경우에는 벡터스피드 모드로, FALSE로 설정했을 경우에는 마스터스피드 모드로 설정됩니다
	dp_SpdRatio[0] = dp_SpdRatio[0] * ((double)mn_runspeed_rate / (double)100); //james test 090311
	//cmmIxSetSpeedPattern)	(long MapIndex, long IsVectorSpeed, long SpeedMode, double Vel, double Acc, double Dec);  	
#if 1	
	nRet = cmmIxSetSpeedPattern(n_MapIndex, cmFALSE, cmSMODE_T, dp_SpdRatio[0], dp_SpdRatio[1], dp_SpdRatio[2]);//cmFALSE = Master Speed로 셋팅되면 % 로 Speed가 설정된다 
#else
	nRet = cmmIxSetSpeedPattern(n_MapIndex, cmTRUE, cmSMODE_T, 450000, 4500000, 4500000); //vertorspeed
#endif
  	if (nRet != cmERR_NONE) //정상
	{
		//010015 E A "Interpolation IxMapAxis setting Error(01 motor)."
		strMsg.Format(_T("%02d0015"), mp_axisnum[n_MapIndex][0]);
		//wsprintfA(mc_alarmcode, "%S", strMsg);
		clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//		sprintf(mc_alarmcode, "%02d0015", n_MapIndex); 
		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[%d] cmmIxSetSpeedPattern return error"), n_MapIndex);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//			sprintf(mc_normal_msg, "[%d] cmmIxSetSpeedPattern return error", n_MapIndex);
			Debug_File_Create(0, mc_normal_msg);
		}

		// 20120523 jtkim
//		sprintf(Jamcode, "%02d0006", mp_axisnum[n_MapIndex][0]); //on check error
		st_alarm_info.strCode.Format(_T("%02d0006"), mp_axisnum[n_MapIndex][0]);
		st_alarm_info.nCountMode = 0;	
		st_alarm_info.nTypeMode = eWARNING;
		st_alarm_info.nAlarmAssignSection = 15;

		cmmIxStopEmg(n_MapIndex);

		return BD_ERROR;
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//모션 동작 완료 여부를 확인한다 
	nRet = cmmIxIsDone(n_MapIndex, &nListIsDone);
	if(nRet == cmERR_NONE) 
	{//함수 수행 성공 
		if(nListIsDone != cmTRUE) //0:리스트 모션이 동작중인 상태, 1: 모션이 완료된 상태 
		{
			cmmIxStopEmg(n_MapIndex);//2015.0219 james
			return 0;//2015.0219 james

			ml_linear_move_time[n_MapIndex][1] = GetCurrentTime();
			ml_linear_move_time[n_MapIndex][2] = ml_linear_move_time[n_MapIndex][1] - ml_linear_move_time[n_MapIndex][0];
			if (ml_linear_move_time[n_MapIndex][2] > 2000) //090609 MOT_MOVE_LIMITTIME)
			{
				mn_linear_retry_cnt[n_MapIndex] = 0;  
				cmmIxStopEmg(n_MapIndex);
			 
				//010015 E A "Interpolation IxMapAxis setting Error(01 motor)."
				strMsg.Format(_T("%02d0015"), n_MapIndex);
				//wsprintfA(mc_alarmcode, "%S", strMsg);
				clsFunc.OnStringToChar(strMsg, mc_alarmcode);
	//			sprintf(mc_alarmcode, "%02d0015", n_MapIndex); 

				if(mn_errormsg_debug_mode)
				{
					strMsg.Format(_T("[%d] cmmIxIsDone return error"), n_MapIndex);
					//wsprintfA(mc_normal_msg, "%S", strMsg);
					clsFunc.OnStringToChar(strMsg, mc_normal_msg);
	//				sprintf(mc_normal_msg, "[%d] cmmIxIsDone return error", n_MapIndex);
					Debug_File_Create(0, mc_normal_msg);
				}

				// 20120523 jtkim
	//			sprintf(Jamcode, "%02d0015", mp_axisnum[n_MapIndex][0]); //on check error
				st_alarm_info.strCode.Format(_T("%02d0015"), mp_axisnum[n_MapIndex][0]);
				st_alarm_info.nCountMode = 0;	
				st_alarm_info.nTypeMode = eWARNING;
				st_alarm_info.nAlarmAssignSection = 16;

				return BD_ERROR;
			}
			else
			{
				return BD_RETRY;
			}
		}
	}
	else
	{//리턴값이 정상이 아니다 
		//010015 E A "Interpolation IxMapAxis setting Error(01 motor)."
 		cmmIxStopEmg(n_MapIndex);

		strMsg.Format(_T("%02d0015"), n_MapIndex);
		//wsprintfA(mc_alarmcode, "%S", strMsg);
		clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//		sprintf(mc_alarmcode, "%02d0015", n_MapIndex);
		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[%d] cmmIxIsDone return error"), n_MapIndex);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//			sprintf(mc_normal_msg, "[%d] cmmIxIsDone return error", n_MapIndex);
			Debug_File_Create(0, mc_normal_msg);
		}
		mn_linear_retry_cnt[n_MapIndex] = 0; 

		// 20120523 jtkim
//		sprintf(Jamcode, "%02d0006", mp_axisnum[n_MapIndex][0]); //on check error
		st_alarm_info.strCode.Format(_T("%02d0006"), mp_axisnum[n_MapIndex][0]);
		st_alarm_info.nCountMode = 0;	
		st_alarm_info.nTypeMode = eWARNING;
		st_alarm_info.nAlarmAssignSection = 17;

		return BD_ERROR;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	nRet = cmmIxLineToStart(n_MapIndex, dTargetPos);	
	if (nRet == cmERR_NONE)
	{
		ml_linear_move_time[n_MapIndex][0] = GetCurrentTime();
		for(i=0; i<ml_axiscnt[n_MapIndex]; i++)
		{
			if(mn_run_status == CTL_dRUN)	//090825 handler가 정상가동중일때만 커멘드 데이터를 백업, 메뉴얼 동작은 스킵 
			{
				md_cmdpos_backup[mp_axisnum[n_MapIndex][i]] = dp_PosList[i];
			}
		}

		return BD_GOOD;
	}
	else
	{
		strMsg.Format(_T("%02d0015"), mp_axisnum[n_MapIndex][0]);
		//wsprintfA(mc_alarmcode, "%S", strMsg);
		clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//		sprintf(mc_alarmcode, "%02d0015", n_MapIndex);
		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[%d] cmmIxLineToStart return error"), n_MapIndex);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//			sprintf(mc_normal_msg, "[%d] cmmIxLineToStart return error", n_MapIndex);
			Debug_File_Create(0, mc_normal_msg);
		}		
 		cmmIxStopEmg(n_MapIndex);
		 

		// 20120523 jtkim
//		sprintf(Jamcode, "%02d0006", mp_axisnum[n_MapIndex][0]); //on check error
		st_alarm_info.strCode.Format(_T("%02d0006"), mp_axisnum[n_MapIndex][0]);
		st_alarm_info.nCountMode = 0;	
		st_alarm_info.nTypeMode = eWARNING;
		st_alarm_info.nAlarmAssignSection = 18;

		mn_linear_retry_cnt[n_MapIndex] = 0;

		return BD_ERROR;
	}

	return BD_PROCEED;  
}


int CComizoaPublic::Check_LinearMove(int n_MapIndex, double *dp_PosList) 
{
	int nRet=0, i=0, nError=0;
	long nListIsDone = 0;
	int  nCurrPosEndCnt = 0;
	double dCurrPos[2] = {0,0};
	double dGap[2] = {0,0};
	CString strMsg;
//	char Jamcode[10];

	if(st_work_info.nSimulationMode == 1)
	{// 
		return BD_GOOD;
	}

	for(i=0; i<ml_axiscnt[n_MapIndex]; i++)
	{
		nRet = CTL_Lib.Motor_SafetyCheck(2, mp_axisnum[n_MapIndex][i], dp_PosList[i]);
		if (nRet == BD_ERROR)
		{
 			cmmIxStopEmg(n_MapIndex); //2009.06.30 추가 		 

			return BD_ERROR;
		}
		else if(nRet == BD_RETRY) //091216 추가 
		{
			return BD_RETRY;
		}
	}

	nRet = cmmIxIsDone(n_MapIndex, &nListIsDone);
	if(nRet == cmERR_NONE) //함수 수행 성공 
	{
		ml_linear_move_time[n_MapIndex][1] = GetCurrentTime();
		ml_linear_move_time[n_MapIndex][2] = ml_linear_move_time[n_MapIndex][1] - ml_linear_move_time[n_MapIndex][0];
		if(ml_linear_move_time[n_MapIndex][2] < 0)
		{
			ml_linear_move_time[n_MapIndex][0] = GetCurrentTime();
			ml_linear_move_time[n_MapIndex][2] = 0;
		}

		if(nListIsDone == cmTRUE) //0:리스트 모션이 동작중인 상태, 1: 모션이 완료된 상태 
		{
			nCurrPosEndCnt = 0;
			for(i=0; i<ml_axiscnt[n_MapIndex]; i++) //최대 한번에 4축 보간동작으로 이동가능 
			{
				nRet = Check_MotPosRange(mp_axisnum[n_MapIndex][i], dp_PosList[i], md_allow_value[mp_axisnum[n_MapIndex][i]]); //(int n_Axis, double d_Distance, int n_Range) 
				if(nRet == BD_GOOD)
				{					
					nCurrPosEndCnt++;
				}
				else
				{
					nError = i;
				}
			}			
			if(nCurrPosEndCnt >= ml_axiscnt[n_MapIndex]) //james test
			{
				ml_linear_move_time[n_MapIndex][0] = GetCurrentTime();
				mn_linear_retry_cnt[n_MapIndex] = 0;  
				return BD_GOOD;
			}
			else
			{//보간동작 완료했으나 위치가 문제인 상태 
				 
				if (ml_linear_move_time[n_MapIndex][2] > 2000) //090609 MOT_MOVE_LIMITTIME)
				{
					if(mn_linear_retry_cnt[n_MapIndex] > mn_max_retry_cnt)
					{
						mn_linear_retry_cnt[n_MapIndex] = 0; 

						strMsg.Format(_T("%02d0006"), mp_axisnum[n_MapIndex][nError]);
						//wsprintfA(mc_alarmcode, "%S", strMsg);
						clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//						sprintf(mc_alarmcode, "%02d0015", n_MapIndex);
						if(mn_errormsg_debug_mode)
						{
							strMsg.Format(_T("[%d] cmmIxIsDone return_1 error"), n_MapIndex);
							//wsprintfA(mc_normal_msg, "%S", strMsg);
							clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//							sprintf(mc_normal_msg, "[%d] cmmIxIsDone return_1 error", n_MapIndex);
							Debug_File_Create(0, mc_normal_msg);
						}

 						cmmIxStopEmg(n_MapIndex);
						 

						// 20120523 jtkim
//						sprintf(Jamcode, "%02d0015", mp_axisnum[n_MapIndex][0]); //on check error
// 						st_alarm_info.strCode.Format(_T("%02d0015"), mp_axisnum[n_MapIndex][0]);
// 						st_alarm_info.nCountMode	= 0;	
// 						st_alarm_info.nTypeMode		= eWARNING;
// 						st_alarm_info.nAlarmAssignSection = 19;

						mn_linear_retry_cnt[n_MapIndex] = 0; 

						return BD_ERROR;
					}
					else
					{					
						mn_linear_retry_cnt[n_MapIndex]++;
						return BD_RETRY;
					}
				}
			} 
		}
		else //0 이면 완료되지 않은 상태 
		{//아직 동작 중인 상태 
			if(ml_linear_move_time[n_MapIndex][2] > mn_max_move_limit_time) //5초 이상 동작하지 못했으면 에러 
			{				
				mn_linear_retry_cnt[n_MapIndex] = 0; 

				strMsg.Format(_T("%02d0015"), n_MapIndex);
				//wsprintfA(mc_alarmcode, "%S", strMsg);
				clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//				sprintf(mc_alarmcode, "%02d0015", n_MapIndex);
				if(mn_errormsg_debug_mode)
				{
					strMsg.Format(_T("[%d] cmmIxIsDone return_2 error"), n_MapIndex);
					//wsprintfA(mc_normal_msg, "%S", strMsg);
					clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//					sprintf(mc_normal_msg, "[%d] cmmIxIsDone return_2 error", n_MapIndex);
					Debug_File_Create(0, mc_normal_msg);
				}

 				cmmIxStopEmg(n_MapIndex);			 

				// 20120523 jtkim
//				sprintf(Jamcode, "%02d0006", mp_axisnum[n_MapIndex][0]); //on check error
				st_alarm_info.strCode.Format(_T("%02d0015"), mp_axisnum[n_MapIndex][0]);
				st_alarm_info.nCountMode	= 0;	
				st_alarm_info.nTypeMode		= eWARNING;
				st_alarm_info.nAlarmAssignSection = 20;

				return BD_ERROR;
			}
			else
			{
				return BD_PROCEED; 
			}
		}
	}
	else
	{ //리턴 에러  
		mn_linear_retry_cnt[n_MapIndex] = 0; 

		strMsg.Format(_T("%02d0015"), mp_axisnum[n_MapIndex][0]);
		//wsprintfA(mc_alarmcode, "%S", strMsg);
		clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//		sprintf(mc_alarmcode, "%02d0015", n_MapIndex);
		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[%d] cmmIxIsDone return_3 error"), n_MapIndex);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//			sprintf(mc_normal_msg, "[%d] cmmIxIsDone return_3 error", n_MapIndex);
			Debug_File_Create(0, mc_normal_msg);
		}

     	cmmIxStopEmg(n_MapIndex);
		
		// 20120523 jtkim
//		sprintf(Jamcode, "%02d0015", mp_axisnum[n_MapIndex][0]); //on check error
		st_alarm_info.strCode.Format(_T("%02d0015"), mp_axisnum[n_MapIndex][0]); 
		st_alarm_info.nCountMode = 0;	
		st_alarm_info.nTypeMode = eWARNING;
		st_alarm_info.nAlarmAssignSection = 21;

		return BD_ERROR;
	}

	return BD_PROCEED; //MOT_PROCEED;
}

/* 2015.0219 james 수정
int CComizoaPublic::Start_LinearMove(int n_MapIndex, double *dp_PosList, double *dp_SpdRatio)  //n_MapIndex는 미리 셋팅되어 있어야 한다 
{//2축 동작 
//	int    nSafetyCheck = 0, Ret, i=0;
	int    i, nRet=-1;
	double dTargetPos[4]={0,};
	long nListIsDone = 0;
	double dp_Vel[3]={0,0,0};
	CString strMsg;
//	char Jamcode[10];

	if(st_work_info.nSimulationMode == 1)
	{// 
		return BD_GOOD;
	}

	if(mn_indexmap_init[n_MapIndex] == NO || n_MapIndex >= 8) 
	{//초기화 되지 않은 상태 
		nRet = Create_IxIndexMapAxis(n_MapIndex, ml_axiscnt[n_MapIndex], mp_axisnum[n_MapIndex]);
		if(nRet == BD_GOOD)
		{
			mn_indexmap_init[n_MapIndex]  = BD_YES;
		}
		else //if(nRet == BD_ERROR)
		{
			//010015 E A "Interpolation IxMapAxis setting Error(01 motor)."
			strMsg.Format(_T("%02d0015"), n_MapIndex);
			//wsprintfA(mc_alarmcode, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//			sprintf(mc_alarmcode, "%02d0015", n_MapIndex); 
			if(mn_errormsg_debug_mode)
			{
				strMsg.Format(_T("[%d] Create_IxIndexMapAxis return error"), n_MapIndex);
				//wsprintfA(mc_normal_msg, "%S", strMsg);
				clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//				sprintf(mc_normal_msg, "[%d] Create_IxIndexMapAxis return error", n_MapIndex);
				Debug_File_Create(0, mc_normal_msg);
			}

			// 20120523 jtkim
//			sprintf(Jamcode, "%02d0006", mp_axisnum[n_MapIndex][0]); //on check error
			st_alarm_info.strCode.Format(_T("%02d0006"), mp_axisnum[n_MapIndex][0]);
			st_alarm_info.nCountMode	= 0;	
			st_alarm_info.nTypeMode		= eWARNING;
			st_alarm_info.nAlarmAssignSection = 10;

			return BD_ERROR;  
		}
	}

	if(ml_axiscnt[n_MapIndex] < 2)
	{
		//010015 E A "Interpolation IxMapAxis setting Error(01 motor)."
		strMsg.Format(_T("%02d0015"), n_MapIndex);
		//wsprintfA(mc_alarmcode, "%S", strMsg);
		clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//		sprintf(mc_alarmcode, "%02d0015", n_MapIndex); 
		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[%d] ml_axiscnt[n_MapIndex] < 2 return error"), n_MapIndex);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//			sprintf(mc_normal_msg, "[%d] ml_axiscnt[n_MapIndex] < 2 return error", n_MapIndex);
			Debug_File_Create(0, mc_normal_msg);
		}

		// 20120523 jtkim
//		sprintf(Jamcode, "%02d0015", mp_axisnum[n_MapIndex][0]); //on check error
		st_alarm_info.strCode.Format(_T("%02d0015"), mp_axisnum[n_MapIndex][0]);
		st_alarm_info.nCountMode	= 0;	
		st_alarm_info.nTypeMode		= eWARNING;
		st_alarm_info.nAlarmAssignSection = 11;

		return BD_ERROR;
	}

	for(i=0; i<ml_axiscnt[n_MapIndex]; i++)
	{
		nRet = CTL_Lib.Motor_SafetyCheck(1, mp_axisnum[n_MapIndex][i], dp_PosList[i]);
		if (nRet == BD_ERROR)
		{	//safety error 는 해당 함수에서 코드 정리 
//			cmmIxStopEmg(n_MapIndex);
			cmmSxStop(mp_axisnum[n_MapIndex][i], FALSE, TRUE);  // 모터 이동 정지 함수 
			
			// 20120523 jtkim
//			sprintf(Jamcode, "%02d0006", mp_axisnum[n_MapIndex][i]); //on check error
			st_alarm_info.strCode.Format(_T("%02d0006"), mp_axisnum[n_MapIndex][i]);
			st_alarm_info.nCountMode	= 0;	
			st_alarm_info.nTypeMode		= eWARNING;
			st_alarm_info.nAlarmAssignSection = 12;

			return BD_ERROR;

		}
		else if(nRet == BD_RETRY)  
		{
			return BD_RETRY;
		}
//   		Set_MotSpeed(MOT_SPD_VEL, st_linearmot[n_MapIndex].lp_AxisNum[i], cmSMODE_S,
//   			st_motor[st_linearmot[n_MapIndex].lp_AxisNum[i]].d_spd_vel[0], 
//   			st_motor[st_linearmot[n_MapIndex].lp_AxisNum[i]].d_spd_vel[1], 
//   			st_motor[st_linearmot[n_MapIndex].lp_AxisNum[i]].d_spd_vel[2]);

		dTargetPos[i] = dp_PosList[i];
	}

	nRet = Set_MotSpeed(MOT_SPD_VEL, mp_axisnum[n_MapIndex][0], cmSMODE_T, 
		md_spd_vel[mp_axisnum[n_MapIndex][0]][0], md_spd_vel[mp_axisnum[n_MapIndex][0]][1], md_spd_vel[mp_axisnum[n_MapIndex][0]][2]);  
	if(nRet == BD_ERROR)
	{
		//010015 E A "Interpolation IxMapAxis setting Error(01 motor)."
		strMsg.Format(_T("%02d0015"), n_MapIndex);
		//wsprintfA(mc_alarmcode, "%S", strMsg);
		clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//		sprintf(mc_alarmcode, "%02d0015", n_MapIndex); 
		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[%d] Create_IxIndexMapAxis Set_MotSpeed_0 return error"), n_MapIndex);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//			sprintf(mc_normal_msg, "[%d] Create_IxIndexMapAxis Set_MotSpeed_0 return error", n_MapIndex);
			Debug_File_Create(0, mc_normal_msg);
		}

		// 20120523 jtkim
//		sprintf(Jamcode, "%02d0006", mp_axisnum[n_MapIndex][0]); //on check error
		st_alarm_info.strCode.Format(_T("%02d0006"), mp_axisnum[n_MapIndex][0]);
		st_alarm_info.nCountMode = 0;	
		st_alarm_info.nTypeMode = eWARNING;
		st_alarm_info.nAlarmAssignSection = 13;

		return BD_ERROR;
	}
	
	nRet = Set_MotSpeed(MOT_SPD_VEL, mp_axisnum[n_MapIndex][0], cmSMODE_T, 
		md_spd_vel[mp_axisnum[n_MapIndex][0]][0], md_spd_vel[mp_axisnum[n_MapIndex][0]][1], md_spd_vel[mp_axisnum[n_MapIndex][0]][2]);  
	if(nRet == BD_ERROR)
	{
		//500000 D A "Interpolation IxMapAxis 0번 Setting Error."
		strMsg.Format(_T("5000%02dr"), n_MapIndex);
		clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//		sprintf(mc_alarmcode, "5000%02d", n_MapIndex); 
		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[%d] Create_IxIndexMapAxis Set_MotSpeed_1 return error"), n_MapIndex);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//			sprintf(mc_normal_msg, "[%d] Create_IxIndexMapAxis Set_MotSpeed_1 return error", n_MapIndex);
			Debug_File_Create(0, mc_normal_msg);
		}

		// 20120523 jtkim
//		sprintf(Jamcode, "%02d0015", mp_axisnum[n_MapIndex][0]); //on check error
		st_alarm_info.strCode.Format(_T("%02d0015"), mp_axisnum[n_MapIndex][0]);
		st_alarm_info.nCountMode	= 0;	
		st_alarm_info.nTypeMode		= eWARNING;
		st_alarm_info.nAlarmAssignSection = 14;

		return BD_ERROR;
	}

//	dp_SpdRatio[0] = dp_SpdRatio[0] * (double)((mn_runspeed_rate / 100.)); //james test 090311
	dp_SpdRatio[0] = dp_SpdRatio[0] * ((double)mn_runspeed_rate / (double)100); //james test 090311
	//cmmIxSetSpeedPattern)	(long MapIndex, long IsVectorSpeed, long SpeedMode, double Vel, double Acc, double Dec);
  	
#if 1	
	nRet = cmmIxSetSpeedPattern(n_MapIndex, cmFALSE, cmSMODE_T, dp_SpdRatio[0], dp_SpdRatio[1], dp_SpdRatio[2]);
#else
	nRet = cmmIxSetSpeedPattern(n_MapIndex, cmTRUE, cmSMODE_T, 450000, 4500000, 4500000);
#endif
  	if (nRet != cmERR_NONE)
	{
		//010015 E A "Interpolation IxMapAxis setting Error(01 motor)."
		strMsg.Format(_T("%02d0015"), n_MapIndex);
		//wsprintfA(mc_alarmcode, "%S", strMsg);
		clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//		sprintf(mc_alarmcode, "%02d0015", n_MapIndex); 
		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[%d] cmmIxSetSpeedPattern return error"), n_MapIndex);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//			sprintf(mc_normal_msg, "[%d] cmmIxSetSpeedPattern return error", n_MapIndex);
			Debug_File_Create(0, mc_normal_msg);
		}

		// 20120523 jtkim
//		sprintf(Jamcode, "%02d0006", mp_axisnum[n_MapIndex][0]); //on check error
		st_alarm_info.strCode.Format(_T("%02d0006"), mp_axisnum[n_MapIndex][0]);
		st_alarm_info.nCountMode = 0;	
		st_alarm_info.nTypeMode = eWARNING;
		st_alarm_info.nAlarmAssignSection = 15;

		return BD_ERROR;
	}
	//Set_MotSpeed_Ratio(int n_Axis, int n_SpeedMode, double d_work_Ratio, double d_accel_Ratio, double d_decel_Ratio) 
	//cmFALSE = Master Speed로 셋팅되면 % 로 Speed가 설정된다 

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//모션 동작 완료 여부를 확인한다 
	nRet = cmmIxIsDone(n_MapIndex, &nListIsDone);

	if(nRet == cmERR_NONE) 
	{//함수 수행 성공 
		if(nListIsDone != cmTRUE) //0:리스트 모션이 동작중인 상태, 1: 모션이 완료된 상태 
		{
			return 0;

			ml_motion_move_time[mp_axisnum[n_MapIndex][0]][1] = GetCurrentTime();
			ml_motion_move_time[mp_axisnum[n_MapIndex][0]][2] = ml_motion_move_time[mp_axisnum[n_MapIndex][0]][1] - ml_motion_move_time[mp_axisnum[n_MapIndex][0]][0];
			if (ml_motion_move_time[mp_axisnum[n_MapIndex][0]][2] > 2000) //090609 MOT_MOVE_LIMITTIME)
			{

				mn_retry_cnt[mp_axisnum[n_MapIndex][0]] = 0; 
	//			cmmIxStopEmg(n_MapIndex);
				cmmIxStopEmg(n_MapIndex);
			 
			 

			


				//010015 E A "Interpolation IxMapAxis setting Error(01 motor)."
				strMsg.Format(_T("%02d0015"), n_MapIndex);
				//wsprintfA(mc_alarmcode, "%S", strMsg);
				clsFunc.OnStringToChar(strMsg, mc_alarmcode);
	//			sprintf(mc_alarmcode, "%02d0015", n_MapIndex); 

				if(mn_errormsg_debug_mode)
				{
					strMsg.Format(_T("[%d] cmmIxIsDone return error"), n_MapIndex);
					//wsprintfA(mc_normal_msg, "%S", strMsg);
					clsFunc.OnStringToChar(strMsg, mc_normal_msg);
	//				sprintf(mc_normal_msg, "[%d] cmmIxIsDone return error", n_MapIndex);
					Debug_File_Create(0, mc_normal_msg);
				}

				// 20120523 jtkim
	//			sprintf(Jamcode, "%02d0015", mp_axisnum[n_MapIndex][0]); //on check error
				st_alarm_info.strCode.Format(_T("%02d0015"), mp_axisnum[n_MapIndex][0]);
				st_alarm_info.nCountMode = 0;	
				st_alarm_info.nTypeMode = eWARNING;
				st_alarm_info.nAlarmAssignSection = 16;

				return BD_ERROR;
			}
			else
			{
				return BD_RETRY;
			}
		}
	}
	else
	{//리턴값이 정상이 아니다 
		//010015 E A "Interpolation IxMapAxis setting Error(01 motor)."
//		cmmIxStopEmg(n_MapIndex);

		for(i=0; i<ml_axiscnt[n_MapIndex]; i++)
		{
			cmmSxStop(mp_axisnum[n_MapIndex][i], FALSE, TRUE);  // 모터 이동 정지 함수 
		}

		strMsg.Format(_T("%02d0015"), n_MapIndex);
		//wsprintfA(mc_alarmcode, "%S", strMsg);
		clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//		sprintf(mc_alarmcode, "%02d0015", n_MapIndex);
		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[%d] cmmIxIsDone return error"), n_MapIndex);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//			sprintf(mc_normal_msg, "[%d] cmmIxIsDone return error", n_MapIndex);
			Debug_File_Create(0, mc_normal_msg);
		}
		mn_retry_cnt[mp_axisnum[n_MapIndex][0]] = 0; 

		// 20120523 jtkim
//		sprintf(Jamcode, "%02d0006", mp_axisnum[n_MapIndex][0]); //on check error
		st_alarm_info.strCode.Format(_T("%02d0006"), mp_axisnum[n_MapIndex][0]);
		st_alarm_info.nCountMode = 0;	
		st_alarm_info.nTypeMode = eWARNING;
		st_alarm_info.nAlarmAssignSection = 17;

		return BD_ERROR;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	nRet = cmmIxLineToStart(n_MapIndex, dTargetPos);	
	if (nRet == cmERR_NONE)
	{
		ml_motion_move_time[mp_axisnum[n_MapIndex][0]][0] = GetCurrentTime();
		for(i=0; i<ml_axiscnt[n_MapIndex]; i++)
		{
			if(mn_run_status == CTL_dRUN)	//090825 handler가 정상가동중일때만 커멘드 데이터를 백업, 메뉴얼 동작은 스킵 
			{
				md_cmdpos_backup[mp_axisnum[n_MapIndex][i]] = dp_PosList[i];
			}
		}

		return BD_GOOD;
	}
	else
	{
		strMsg.Format(_T("%02d0015"), n_MapIndex);
		//wsprintfA(mc_alarmcode, "%S", strMsg);
		clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//		sprintf(mc_alarmcode, "%02d0015", n_MapIndex);
		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[%d] cmmIxLineToStart return error"), n_MapIndex);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//			sprintf(mc_normal_msg, "[%d] cmmIxLineToStart return error", n_MapIndex);
			Debug_File_Create(0, mc_normal_msg);
		}		

//		cmmIxStopEmg(n_MapIndex);
		for(i=0; i<ml_axiscnt[n_MapIndex]; i++)
		{
			cmmSxStop(mp_axisnum[n_MapIndex][i], FALSE, TRUE);  // 모터 이동 정지 함수 
		}

		// 20120523 jtkim
//		sprintf(Jamcode, "%02d0006", mp_axisnum[n_MapIndex][0]); //on check error
		st_alarm_info.strCode.Format(_T("%02d0006"), mp_axisnum[n_MapIndex][0]);
		st_alarm_info.nCountMode = 0;	
		st_alarm_info.nTypeMode = eWARNING;
		st_alarm_info.nAlarmAssignSection = 18;

		return BD_ERROR;
	}

	return BD_PROCEED;  
}
 
int CComizoaPublic::Check_LinearMove(int n_MapIndex, double *dp_PosList) 
{
	int nRet=0, i=0;
	long nListIsDone = 0;
	int  nCurrPosEndCnt = 0;
	double dCurrPos[2] = {0,0};
	double dGap[2] = {0,0};
	CString strMsg;
//	char Jamcode[10];

	if(st_work_info.nSimulationMode == 1)
	{// 
		return BD_GOOD;
	}

	for(i=0; i<ml_axiscnt[n_MapIndex]; i++)
	{
		nRet = CTL_Lib.Motor_SafetyCheck(2, mp_axisnum[n_MapIndex][i], dp_PosList[i]);
		if (nRet == BD_ERROR)
		{
//			cmmIxStopEmg(n_MapIndex); //2009.06.30 추가 
			cmmSxStop(mp_axisnum[n_MapIndex][i], FALSE, TRUE);  // 모터 이동 정지 함수 

			return BD_ERROR;
		}
		else if(nRet == BD_RETRY) //091216 추가 
		{
			return BD_RETRY;
		}
	}

	nRet = cmmIxIsDone(n_MapIndex, &nListIsDone);
	if(nRet == cmERR_NONE) //함수 수행 성공 
	{
		ml_motion_move_time[mp_axisnum[n_MapIndex][0]][1] = GetCurrentTime();
		ml_motion_move_time[mp_axisnum[n_MapIndex][0]][2] = ml_motion_move_time[mp_axisnum[n_MapIndex][0]][1] - ml_motion_move_time[mp_axisnum[n_MapIndex][0]][0];

		if(nListIsDone == cmTRUE) //0:리스트 모션이 동작중인 상태, 1: 모션이 완료된 상태 
		{
			nCurrPosEndCnt = 0;
			for(i=0; i<ml_axiscnt[n_MapIndex]; i++) //최대 한번에 4축 보간동작으로 이동가능 
			{
				nRet = Check_MotPosRange(mp_axisnum[n_MapIndex][i], dp_PosList[i], mn_allow_value[mp_axisnum[n_MapIndex][0]]); //(int n_Axis, double d_Distance, int n_Range) 

				if(nRet == BD_GOOD)
				{					
					nCurrPosEndCnt++;
				}
			}			

			if(nCurrPosEndCnt >= ml_axiscnt[n_MapIndex]) //james test
			{
				mn_retry_cnt[mp_axisnum[n_MapIndex][0]] = 0; 

				return BD_GOOD;
			}
			else
			{//보간동작 완료했으나 위치가 문제인 상태 
				 
				if (ml_motion_move_time[mp_axisnum[n_MapIndex][0]][2] > 2000) //090609 MOT_MOVE_LIMITTIME)
				{
					if(mn_retry_cnt[mp_axisnum[n_MapIndex][0]] > mn_max_retry_cnt)
					{
						mn_retry_cnt[mp_axisnum[n_MapIndex][0]] = 0; 

						strMsg.Format(_T("%02d0015"), n_MapIndex);
						//wsprintfA(mc_alarmcode, "%S", strMsg);
						clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//						sprintf(mc_alarmcode, "%02d0015", n_MapIndex);
						if(mn_errormsg_debug_mode)
						{
							strMsg.Format(_T("[%d] cmmIxIsDone return_1 error"), n_MapIndex);
							//wsprintfA(mc_normal_msg, "%S", strMsg);
							clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//							sprintf(mc_normal_msg, "[%d] cmmIxIsDone return_1 error", n_MapIndex);
							Debug_File_Create(0, mc_normal_msg);
						}

//						cmmIxStopEmg(n_MapIndex);
						for(i=0; i<ml_axiscnt[n_MapIndex]; i++)
						{
							cmmSxStop(mp_axisnum[n_MapIndex][i], FALSE, TRUE);  // 모터 이동 정지 함수 
						}

						// 20120523 jtkim
//						sprintf(Jamcode, "%02d0015", mp_axisnum[n_MapIndex][0]); //on check error
						st_alarm_info.strCode.Format(_T("%02d0015"), mp_axisnum[n_MapIndex][0]);
						st_alarm_info.nCountMode	= 0;	
						st_alarm_info.nTypeMode		= eWARNING;
						st_alarm_info.nAlarmAssignSection = 19;

						return BD_ERROR;
					}
					else
					{					
						mn_retry_cnt[mp_axisnum[n_MapIndex][0]]++;
						return BD_RETRY;
					}
				}
			} 
		}
		else //0 이면 완료되지 않은 상태 
		{//아직 동작 중인 상태 
			if(ml_motion_move_time[mp_axisnum[n_MapIndex][0]][2] > mn_max_move_limit_time) //5초 이상 동작하지 못했으면 에러 
			{				
				mn_retry_cnt[mp_axisnum[n_MapIndex][0]] = 0; 

				strMsg.Format(_T("%02d0015"), n_MapIndex);
				//wsprintfA(mc_alarmcode, "%S", strMsg);
				clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//				sprintf(mc_alarmcode, "%02d0015", n_MapIndex);
				if(mn_errormsg_debug_mode)
				{
					strMsg.Format(_T("[%d] cmmIxIsDone return_2 error"), n_MapIndex);
					//wsprintfA(mc_normal_msg, "%S", strMsg);
					clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//					sprintf(mc_normal_msg, "[%d] cmmIxIsDone return_2 error", n_MapIndex);
					Debug_File_Create(0, mc_normal_msg);
				}

//				cmmIxStopEmg(n_MapIndex);
				for(i=0; i<ml_axiscnt[n_MapIndex]; i++)
				{
					cmmSxStop(mp_axisnum[n_MapIndex][i], FALSE, TRUE);  // 모터 이동 정지 함수 
				}

				// 20120523 jtkim
//				sprintf(Jamcode, "%02d0006", mp_axisnum[n_MapIndex][0]); //on check error
				st_alarm_info.strCode.Format(_T("%02d0015"), mp_axisnum[n_MapIndex][0]);
				st_alarm_info.nCountMode	= 0;	
				st_alarm_info.nTypeMode		= eWARNING;
				st_alarm_info.nAlarmAssignSection = 20;

				return BD_ERROR;
			}
			else
			{
				return BD_PROCEED; 
			}
		}
	}
	else
	{ //리턴 에러  
		mn_retry_cnt[mp_axisnum[n_MapIndex][0]] = 0; 

		strMsg.Format(_T("%02d0015"), n_MapIndex);
		//wsprintfA(mc_alarmcode, "%S", strMsg);
		clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//		sprintf(mc_alarmcode, "%02d0015", n_MapIndex);
		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[%d] cmmIxIsDone return_3 error"), n_MapIndex);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//			sprintf(mc_normal_msg, "[%d] cmmIxIsDone return_3 error", n_MapIndex);
			Debug_File_Create(0, mc_normal_msg);
		}

//		cmmIxStopEmg(n_MapIndex);
		for(i=0; i<ml_axiscnt[n_MapIndex]; i++)
		{
			cmmSxStop(mp_axisnum[n_MapIndex][i], FALSE, TRUE);  // 모터 이동 정지 함수 
		}

		// 20120523 jtkim
//		sprintf(Jamcode, "%02d0015", mp_axisnum[n_MapIndex][0]); //on check error
		st_alarm_info.strCode.Format(_T("%02d0015"), mp_axisnum[n_MapIndex][0]); 
		st_alarm_info.nCountMode = 0;	
		st_alarm_info.nTypeMode = eWARNING;
		st_alarm_info.nAlarmAssignSection = 21;

		return BD_ERROR;
	}

	return BD_PROCEED; //MOT_PROCEED;
}*/

// ******************************************************************************

// ******************************************************************************
// 다중 모터 이동 함수                                                           
// ******************************************************************************
int CComizoaPublic::Start_MultiMove(long l_AxisCnt, long *lp_AxisNum, double *dp_PosList, double *dp_SpdRatio)//long l_AxisCnt, long *lp_Axis, double *dp_AbsPos, int n_SpeedRatio) 
{//2축 동작 
//	int    nSafetyCheck = 0, Ret, i=0;
	int    i, nRet=-1;
	double dTargetPos[4]={0,};
	CString strMsg;

	if(st_work_info.nSimulationMode == 1)
	{// 
		return BD_GOOD;
	}
	
	
	for(i=0; i<l_AxisCnt; i++)
	{
		nRet = CTL_Lib.Motor_SafetyCheck(1, lp_AxisNum[i], dp_PosList[i]);

		if (nRet == BD_ERROR)
		{
//			cmmSxStopEmg(lp_AxisNum[i]);
			cmmSxStop(lp_AxisNum[i], FALSE, FALSE);  // 모터 이동 정지 함수 
			return BD_ERROR;
		}
		else if(nRet == BD_RETRY) //091216 추가 
		{
			return BD_RETRY;
		}
	}

	for(i=0; i<l_AxisCnt; i++)
	{
		nRet = Set_MotSpeed(MOT_SPD_VEL, lp_AxisNum[i], cmSMODE_T, md_spd_vel[lp_AxisNum[i]][0], md_spd_vel[lp_AxisNum[i]][1], md_spd_vel[lp_AxisNum[i]][2]);
		
		if(nRet == BD_ERROR)
		{//010002 E A "Motor Move Setting Error01 motor)."
			strMsg.Format(_T("%02d0002"), lp_AxisNum[i]);
			//wsprintfA(mc_alarmcode, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//			sprintf(mc_alarmcode, "%02d0002", lp_AxisNum[i]);
			if(mn_errormsg_debug_mode)
			{
				strMsg.Format(_T("[axis=%d] multi move speed return error"), lp_AxisNum[i]);
				//wsprintfA(mc_normal_msg, "%S", strMsg);
				clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//				sprintf(mc_normal_msg, "[axis=%d] multi move speed return error", lp_AxisNum[i]);
				Debug_File_Create(0, mc_normal_msg);
			}
			return BD_ERROR;
		}

		Set_MotSpeed_Ratio(lp_AxisNum[i], cmSMODE_T, dp_SpdRatio[0], dp_SpdRatio[1], dp_SpdRatio[2]);//V_0.1.4 090726 cmSMODE_KEEP 
		
		dTargetPos[i] = dp_PosList[i];
	}

	nRet = cmmMxMoveToStart(l_AxisCnt, lp_AxisNum, dTargetPos);	
	if (nRet == cmERR_NONE)
	{
		
		for(i=0; i<l_AxisCnt; i++)
		{
			if(mn_run_status == CTL_dRUN)	//090825 handler가 정상가동중일때만 커멘드 데이터를 백업, 메뉴얼 동작은 스킵 
			{
				md_cmdpos_backup[lp_AxisNum[i]] = dp_PosList[i];
			}
		}
		ml_motion_move_time[lp_AxisNum[0]][0] = GetCurrentTime();

		return BD_GOOD;
	}
	else
	{
		strMsg.Format(_T("%02d0002"), lp_AxisNum[0]);
		//wsprintfA(mc_alarmcode, "%S", strMsg);
		clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//		sprintf(mc_alarmcode, "%02d0002", lp_AxisNum[0]);
		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[axis=%d] cmmMxMoveToStart return error"), lp_AxisNum[0]);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//			sprintf(mc_normal_msg, "[axis=%d] cmmMxMoveToStart return error", lp_AxisNum[0]);
			Debug_File_Create(0, mc_normal_msg);
		}
		return BD_ERROR;
	}

	return BD_PROCEED;
}
// ******************************************************************************
// 다중 모터 이동 위치 정밀도 검사 함수                                          
// -> WORD wBoardID      : 정의된 모터 보드 ID 번호                              
// -> int *npAxis        : 모터 축 저장 배열에 대한 포인터                       
// -> double *dpAbsPos   : 모터 이동 거리 저장 배열에 대한 포인터                
// ******************************************************************************
int CComizoaPublic::Check_MultiMove(long l_AxisCnt, long *lp_AxisNum, double *dp_PosList) 
{
	int    i, nRet=0;
	double dCurrPos[4]={0,};
	long nListIsDone = 0;
	int nCurrPosEndCnt;
	CString strMsg;

	if(st_work_info.nSimulationMode == 1)
	{// 
		return BD_GOOD;
	}

	for(i=0; i<l_AxisCnt; i++)
	{
		nRet = 1; //2012.1029 jamestest CTL_Lib.Motor_SafetyCheck(2, lp_AxisNum[i], dp_PosList[i]);
		if (nRet == BD_ERROR)
		{
//			cmmSxStopEmg(lp_AxisNum[i]);
			cmmSxStop(lp_AxisNum[i], FALSE, FALSE);  // 모터 이동 정지 함수 
			return BD_ERROR; //MOT_SAFETY_ERROR;
		}	
		else if(nRet == BD_RETRY) //091216 추가 
		{
			return BD_RETRY;
		}
	}

	ml_motion_move_time[lp_AxisNum[0]][1] = GetCurrentTime();
	ml_motion_move_time[lp_AxisNum[0]][2] = ml_motion_move_time[lp_AxisNum[0]][1] - ml_motion_move_time[lp_AxisNum[0]][0];

	nRet = cmmMxIsDone(l_AxisCnt, lp_AxisNum, &nListIsDone);
	if(nRet == cmERR_NONE) //수행 성공 
	{
		if(nListIsDone == cmTRUE) //0:리스트 모션이 동작중인 상태, 1: 모션이 완료된 상태 
		{
			nCurrPosEndCnt = 0;
			for(i=0; i<l_AxisCnt; i++) //최대 한버에 4축 보간동작으로 이동가능 
			{
				nRet = Check_MotPosRange(lp_AxisNum[i], dp_PosList[i], md_allow_value[lp_AxisNum[i]]); //(int n_Axis, double d_Distance, int n_Range) 

				if(nRet == BD_GOOD)
				{					
					nCurrPosEndCnt++;
				}
			}

			if(nCurrPosEndCnt == l_AxisCnt)
			{
				mn_retry_cnt[lp_AxisNum[0]] =  0;
				return BD_GOOD;
			}
			else
			{//보간동작 완료 
				if (ml_motion_move_time[lp_AxisNum[0]][2] > 2000) //090609 MOT_MOVE_LIMITTIME)
				{
					if(mn_retry_cnt[lp_AxisNum[0]] > mn_max_retry_cnt)
					{
						mn_retry_cnt[lp_AxisNum[0]] =  0;

						//010006 E A "Motor Move Check Error(01 motor)."
						strMsg.Format(_T("%02d0006"), lp_AxisNum[0]);
						//wsprintfA(mc_alarmcode, "%S", strMsg);
						clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//						sprintf(mc_alarmcode, "%02d0006", lp_AxisNum[0]);
						if(mn_errormsg_debug_mode)
						{
							strMsg.Format(_T("[axis=%d] cmmMxIsDone return error"), lp_AxisNum[0]);
							//wsprintfA(mc_normal_msg, "%S", strMsg);
							clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//							sprintf(mc_normal_msg, "[axis=%d] cmmMxIsDone return error", lp_AxisNum[0]);
							Debug_File_Create(0, mc_normal_msg);
						}
						return BD_ERROR;
					}
					else
					{
						mn_retry_cnt[lp_AxisNum[0]]++ ;
						return BD_RETRY;								
					}			
				}					
			} 
		}
		else //0 이면 완료되지 않은 상태 
		{	//동작 중인 상태 		
			if(ml_motion_move_time[lp_AxisNum[0]][2] > mn_max_move_limit_time) //MOT_MOVE_LIMITTIME초 이상 동작하지 못했으면 에러 
			{
				mn_retry_cnt[lp_AxisNum[0]] =  0;

				//010006 E A "Motor Move Check Error(01 motor)."
				strMsg.Format(_T("%02d0006"), lp_AxisNum[0]);
				//wsprintfA(mc_alarmcode, "%S", strMsg);
				clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//				sprintf(mc_alarmcode, "%02d0006", lp_AxisNum[0]);
				if(mn_errormsg_debug_mode)
				{
					strMsg.Format(_T("[axis=%d] cmmMxIsDone nListIsDone return error"), lp_AxisNum[0]);
					//wsprintfA(mc_normal_msg, "%S", strMsg);
					clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//					sprintf(mc_normal_msg, "[axis=%d] cmmMxIsDone nListIsDone return error", lp_AxisNum[0]);
					Debug_File_Create(0, mc_normal_msg);
				}
				return BD_ERROR;
			}
			else
			{
				nRet = BD_PROCEED;
			}
		}
	}
	else
	{ //음수이면 에러 
		//010006 E A "Motor Move Check Error(01 motor)."
		strMsg.Format(_T("%02d0006"), lp_AxisNum[0]);
		//wsprintfA(mc_alarmcode, "%S", strMsg);
		clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//		sprintf(mc_alarmcode, "%02d0006", lp_AxisNum[0]);
		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[axis=%d] cmmMxIsDone return_0 error"), lp_AxisNum[0]);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//			sprintf(mc_normal_msg, "[axis=%d] cmmMxIsDone return_0 error", lp_AxisNum[0]);
			Debug_File_Create(0, mc_normal_msg);
		}
		return BD_ERROR;
	}

	return BD_PROCEED;
}
// ******************************************************************************


///////////////////////////////////////////////////////////////////////////////////////////////////
// 연속 보간 동작은 보드당 0~3, 4~7까지 4축 씩만 가능한다 
///////////////////////////////////////////////////////////////////////////////////////////////////
int CComizoaPublic::Start_CoordinatedMove(long n_LmIndexNum) //연속보간 모드 
{
	int i,j;
	int nRet =0;
	double dSpeedRatio[3] = {100,100,100}; 
	CString strMsg;

	if(st_work_info.nSimulationMode == 1)
	{// 
		return BD_GOOD;
	}

//Index Map 번호 및 셋팅 
	nRet = Create_LmIndexMapAxis(n_LmIndexNum, ml_coord_totalaxiscnt[n_LmIndexNum], ml_coord_totalaxisnum[n_LmIndexNum]); //l_TotalAxisCnt = 최대 4축까지 사용할 수 있도록 하자 
	if(nRet == BD_GOOD)
	{				
		mn_coord_indexmap_init[n_LmIndexNum] = BD_YES;
	}
	else //if(nRet == BD_ERROR)
	{//Index Map 작성 에러 
		//cmmLmAbortMotion(n_LmIndexNum); //동작중 에러 발생시 강제 종료 시킨다 
		mn_coord_indexmap_init[n_LmIndexNum] = BD_NO;

		//010002 E A "Motor Move Setting Error01 motor)."
		strMsg.Format(_T("%02d0002"), ml_coord_totalaxisnum[n_LmIndexNum][0]);
		//wsprintfA(mc_alarmcode, "%S", strMsg);
		clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//		sprintf(mc_alarmcode, "%02d0002", ml_coord_totalaxisnum[n_LmIndexNum][0]);
		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[axis=%d] Create_LmIndexMapAxis return_0 error"), ml_coord_totalaxisnum[n_LmIndexNum][0]);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//			sprintf(mc_normal_msg, "[axis=%d] Create_LmIndexMapAxis return_0 error", ml_coord_totalaxisnum[n_LmIndexNum][0]);
			Debug_File_Create(0, mc_normal_msg);
		}
		return BD_ERROR;  
	}


	if(mn_coord_indexmap_init[n_LmIndexNum] != BD_YES || n_LmIndexNum >= 8) 
	{//초기화 되지 않은 상태 
		//cmmLmAbortMotion(n_LmIndexNum); //동작중 에러 발생시 강제 종료 시킨다 
		//010002 E A "Motor Move Setting Error01 motor)."
		strMsg.Format(_T("%02d0002"), ml_coord_totalaxisnum[n_LmIndexNum][0]);
		//wsprintfA(mc_alarmcode, "%S", strMsg);
		clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//		sprintf(mc_alarmcode, "%02d0002", ml_coord_totalaxisnum[n_LmIndexNum][0]);
		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[axis=%d] Create_LmIndexMapAxis return_0 error"), ml_coord_totalaxisnum[n_LmIndexNum][0]);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//			sprintf(mc_normal_msg, "[axis=%d] Create_LmIndexMapAxis return_0 error", ml_coord_totalaxisnum[n_LmIndexNum][0]);
			Debug_File_Create(0, mc_normal_msg);
		}
		return BD_ERROR;  
	}

	
	nRet = cmmLmBeginList(n_LmIndexNum); 
	if (nRet != cmERR_NONE)
	{ 
		//cmmLmAbortMotion(n_LmIndexNum); //동작중 에러 발생시 강제 종료 시킨다 
		//010002 E A "Motor Move Setting Error01 motor)."
		strMsg.Format(_T("%02d0002"), ml_coord_totalaxisnum[n_LmIndexNum][0]);
		//wsprintfA(mc_alarmcode, "%S", strMsg);
		clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//		sprintf(mc_alarmcode, "%02d0002", ml_coord_totalaxisnum[n_LmIndexNum][0]);
		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[axis=%d] cmmLmBeginList return_0 error"), ml_coord_totalaxisnum[n_LmIndexNum][0]);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//			sprintf(mc_normal_msg, "[axis=%d] cmmLmBeginList return_0 error", ml_coord_totalaxisnum[n_LmIndexNum][0]);
			Debug_File_Create(0, mc_normal_msg);
		}
		return BD_ERROR;  
	}

	for(i=0; i<ml_coord_totalaxiscnt[n_LmIndexNum]; i++) //단축이든 보간이든 한번 동작을 1회로 보아 이동하는 횟수를 지정한다  
	{
		if(ml_coord_subaxiscnt[n_LmIndexNum][i] == 1) //단축 이동한다는 플레그 
		{	
			nRet = Start_SingleMove(ml_coord_subaxisnum[n_LmIndexNum][i][0], md_coord_poslist[n_LmIndexNum][i][0]); //double *dp_SpeedRatio) 

			if (nRet != BD_YES)//cmERR_NONE)
			{
				//cmmLmAbortMotion(n_LmIndexNum); //동작중 에러 발생시 강제 종료 시킨다 
				//010002 E A "Motor Move Setting Error01 motor)."
				strMsg.Format(_T("%02d0002"), ml_coord_subaxisnum[n_LmIndexNum][i][0]);
				//wsprintfA(mc_alarmcode, "%S", strMsg);
				clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//				sprintf(mc_alarmcode, "%02d0002", ml_coord_subaxisnum[n_LmIndexNum][i][0]);
				if(mn_errormsg_debug_mode)
				{
					strMsg.Format(_T("[axis=%d] coord Start_SingleMove return_0 error"), ml_coord_subaxisnum[n_LmIndexNum][i][0]);
					//wsprintfA(mc_normal_msg, "%S", strMsg);
					clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//					sprintf(mc_normal_msg, "[axis=%d] coord Start_SingleMove return_0 error",ml_coord_subaxisnum[n_LmIndexNum][i][0]);
					Debug_File_Create(0, mc_normal_msg);
				}
			}
		}
		else if(ml_coord_subaxiscnt[n_LmIndexNum][i] > 1) //2축 이상 보간동작으로 이동한다는 플레그 
		{ 
			//Axis Mask 정보를 미리 구하여 선언해 놓아야 한다  
			//cmmIxMapAxes(n_IxIndexNum, nMapMask[0], nMapMask[1]);
			//LmIndexMap_Create(int n_LmMode, int n_MapIndexNum, int n_AxisCount, int n_AxisNum[4]) //최대 4축까지 사용할 수 있도록 하자 

			ml_axiscnt[n_LmIndexNum] = ml_coord_subaxiscnt[n_LmIndexNum][i];

			for(j=0; j<ml_axiscnt[n_LmIndexNum]; j++)
			{
				mp_axisnum[n_LmIndexNum][j] = ml_coord_subaxisnum[n_LmIndexNum][i][j];
				//st_linearmot[n_LmIndexNum].lp_AxisNum[1] = st_coordmot[n_LmIndexNum].lp_SubAxisNum[i];
			}

			nRet = Start_LinearMove(n_LmIndexNum, md_coord_poslist[n_LmIndexNum][i], dSpeedRatio);
			if(nRet != BD_GOOD)
			{
				cmmLmAbortMotion(n_LmIndexNum); //동작중 에러 발생시 강제 종료 시킨다 

				//010002 E A "Motor Move Setting Error01 motor)."
				strMsg.Format(_T("%02d0002"), ml_coord_subaxisnum[n_LmIndexNum][i][0]);
				//wsprintfA(mc_alarmcode, "%S", strMsg);
				clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//				sprintf(mc_alarmcode, "%02d0002", ml_coord_subaxisnum[n_LmIndexNum][i][0]);
				if(mn_errormsg_debug_mode)
				{
					strMsg.Format(_T("[axis=%d] coord Start_LinearMove return_0 error"), ml_coord_subaxisnum[n_LmIndexNum][i][0]);
					//wsprintfA(mc_normal_msg, "%S", strMsg);
					clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//					sprintf(mc_normal_msg, "[axis=%d] coord Start_LinearMove return_0 error",ml_coord_subaxisnum[n_LmIndexNum][i][0]);
					Debug_File_Create(0, mc_normal_msg);
				}
				return BD_ERROR; 
			}
/*
			//Index Map 번호 및 셋팅 
			nRet = Create_IxIndexMapAxis(n_LmIndexNum, st_coordmot[n_LmIndexNum].lp_SubAxisCnt[i], st_coordmot[n_LmIndexNum].lp_SubAxisNum[i]); //n_AxisCnt = 최대 4축까지 사용할 수 있도록 하자 
			if(nRet == BD_GOOD)
			{
				st_coordmot[n_LmIndexNum].n_indexmap_init = BD_YES;
			}
			else //if(nRet == BD_ERROR)
			{//Index Map 작성 에러 
				sprintf(cJamcode, "%02d0046",  st_coordmot[st_coordmot[n_LmIndexNum].lp_IxIndexNum[i]].lp_IxIndexNum[i]); //000046 1 70 "List Motion_0 LmIndexMap  Start Return Check Error."
				CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, cJamcode);

				nErrAxis = cmmErrParseAxis(nRet);
				nErrReason = cmmErrParseReason(nRet);
				CtlBdFunc.ms_ErrMsg.Format("[MOVE ERROR] %02d Axis : %d", nErrAxis, nErrReason);
				CtlBdFunc.Send_Error_Message(MOT_ERR_MSG, CtlBdFunc.ms_ErrMsg);
				return BD_ERROR;  
			}
			
			if(st_coordmot[n_LmIndexNum].n_indexmap_init != BD_YES || n_LmIndexNum >= 8)  //0~7
			{//초기화 되지 않은 상태 
				sprintf(cJamcode, "%02d0025",  st_coordmot[n_LmIndexNum].lp_IxIndexNum[i]); //000025 1 70 "Linear Motion_0 IxIndexMap Create Check Error."
				CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, cJamcode);
				return BD_ERROR;  
			}

			nRet = cmmIxSetSpeedPattern(st_coordmot[n_LmIndexNum].lp_IxIndexNum[i], cmFALSE, cmSMODE_S, 100, 100, 100); //정속, 가속, 감속 ratio 
			if (nRet != cmERR_NONE)	{CtlBdFunc.Send_Error_Message(0, "cmmIxSetSpeedPattern return Error"); return BD_ERROR; }			

			nRet = cmmIxLineToStart(n_LmIndexNum, st_coordmot[n_LmIndexNum].dp_PosList[i]);
			if (nRet != cmERR_NONE)
			{
				sprintf(cJamcode, "%02d0020",  st_coordmot[n_LmIndexNum].lp_IxIndexNum[i]); //000020 1 70 "Linear Motion_0 Start Error."
				CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, cJamcode);
				nErrAxis = cmmErrParseAxis(nRet);
				nErrReason = cmmErrParseReason(nRet);
				CtlBdFunc.ms_ErrMsg.Format("[MOVE ERROR] %02d Axis : %d", nErrAxis, nErrReason);
				CtlBdFunc.Send_Error_Message(MOT_ERR_MSG, CtlBdFunc.ms_ErrMsg);
				 return BD_ERROR; 
			}
*/
		}
		else 
		{
			//AfxMessageBox("List Motion Motor Axis define Error");
			//cmmLmAbortMotion(n_LmIndexNum); //동작중 에러 발생시 강제 종료 시킨다 

			//010002 E A "Motor Move Setting Error01 motor)."
			strMsg.Format(_T("%02d0002"), ml_coord_subaxisnum[n_LmIndexNum][i][0]);
			//wsprintfA(mc_alarmcode, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//			sprintf(mc_alarmcode, "%02d0002", ml_coord_subaxisnum[n_LmIndexNum][i][0]);
			if(mn_errormsg_debug_mode)
			{
				strMsg.Format(_T("[axis=%d] coord Start_LinearMove return_0 error"), ml_coord_subaxisnum[n_LmIndexNum][i][0]);
				//wsprintfA(mc_normal_msg, "%S", strMsg);
				clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//				sprintf(mc_normal_msg, "[axis=%d] coord Start_LinearMove return_0 error",ml_coord_subaxisnum[n_LmIndexNum][i][0]);
				Debug_File_Create(0, mc_normal_msg);
			}
			return BD_ERROR; 
		}
	}

	nRet = cmmLmEndList(n_LmIndexNum); //map 설정 완료 
	if (nRet != cmERR_NONE)
	{
		cmmLmAbortMotion(n_LmIndexNum); //동작중 에러 발생시 강제 종료 시킨다 

		//010002 E A "Motor Move Setting Error01 motor)."
		strMsg.Format(_T("%02d0002"), ml_coord_subaxisnum[n_LmIndexNum][0][0]);
		//wsprintfA(mc_alarmcode, "%S", strMsg);
		clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//		sprintf(mc_alarmcode, "%02d0002", ml_coord_subaxisnum[n_LmIndexNum][0][0]);
		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[axis=%d] coord cmmLmEndList return_0 error"), ml_coord_subaxisnum[n_LmIndexNum][i][0]);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//			sprintf(mc_normal_msg, "[axis=%d] coord cmmLmEndList return_0 error",ml_coord_subaxisnum[n_LmIndexNum][i][0]);
			Debug_File_Create(0, mc_normal_msg);
		}
		return BD_ERROR; 
	}

	nRet = cmmLmStartMotion(n_LmIndexNum); //지금부터 이동 시작 i 
	if (nRet == cmERR_NONE) //에러없이 모두동작 완료 
	{	//동작한 모터의 커멘드 정보를 저장한다 
		for(i=0; i<ml_coord_totalmovecnt[n_LmIndexNum]; i++) //단축이든 보간이든 한번 동작을 1회로 보아 이동하는 횟수를 지정한다  
		{
			if(ml_coord_subaxiscnt[n_LmIndexNum][i] == 1) //단축 이동한다는 플레그 
			{
				md_cmdpos_backup[ml_coord_subaxisnum[n_LmIndexNum][i][0]] = md_coord_poslist[n_LmIndexNum][i][0];			
			}
			else if(ml_coord_subaxiscnt[n_LmIndexNum][i] > 1) //2축 이상의 보간동작으로 이동한다는 플레그 
			{
				for(j=0; j<ml_coord_subaxiscnt[n_LmIndexNum][i]; j++)
				{
					md_cmdpos_backup[ml_coord_subaxisnum[n_LmIndexNum][i][j]] = md_coord_poslist[n_LmIndexNum][i][j];
				}				 
			}
		}				
	}
	else
	{		
		//010002 E A "Motor Move Setting Error01 motor)."
		strMsg.Format(_T("%02d0002"), ml_coord_subaxisnum[n_LmIndexNum][0][0]);
		//wsprintfA(mc_alarmcode, "%S", strMsg);
		clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//		sprintf(mc_alarmcode, "%02d0002", ml_coord_subaxisnum[n_LmIndexNum][0][0]);
		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[axis=%d] coord cmmLmEndList return_0 error"), ml_coord_subaxisnum[n_LmIndexNum][0][0]);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//			sprintf(mc_normal_msg, "[axis=%d] coord cmmLmEndList return_0 error",ml_coord_subaxisnum[n_LmIndexNum][0][0]);
			Debug_File_Create(0, mc_normal_msg);
		}

		cmmLmAbortMotion(n_LmIndexNum); //동작중 에러 발생시 강제 종료 시킨다 
		return BD_ERROR; 
	}

	ml_motion_move_time[ml_coord_subaxisnum[n_LmIndexNum][0][0]][0] = GetCurrentTime();

	//st_coordmot[n_LmIndexNum].n_indexmap_init = BD_YES; //james test 
	return BD_GOOD;
} 

int CComizoaPublic::Check_CoordinatedMove(int n_LmIndexNum)
{
	long nRet=0, nDoneRet=1, i=0;
	long lListIsDone;
	int  nMotCntNum = 0, nCurrPosYes = 0;
	double dCurrPos[4]={0,0,0,0};
	CString sErrMsg;
	CString strMsg;

	if(st_work_info.nSimulationMode == 1)
	{// 
		return BD_GOOD;
	}

	ml_motion_move_time[ml_coord_subaxisnum[n_LmIndexNum][0][0]][1] = GetCurrentTime(); 
	ml_motion_move_time[ml_coord_subaxisnum[n_LmIndexNum][0][0]][2] = ml_motion_move_time[ml_coord_subaxisnum[n_LmIndexNum][0][0]][1]- ml_motion_move_time[ml_coord_subaxisnum[n_LmIndexNum][0][0]][0];

	nDoneRet = cmmLmIsDone(n_LmIndexNum, &lListIsDone); //james test lListIsDone 내용 확인할 것 
	//if(nDoneRet == cmERR_NONE) //명령 수행 성공 
	if(lListIsDone == cmTRUE)
	{
		if(lListIsDone == cmTRUE) //BD_YES) //0:리스트 모션이 동작중인 상태, 1: 모션이 완료된 상태 
		{
			for(i=0; i<ml_coord_totalaxiscnt[n_LmIndexNum]; i++) //최대 한버에 4축 보간동작으로 이동가능 
			{
				nRet = Check_MotPosRange(ml_coord_totalaxisnum[n_LmIndexNum][i], md_cmdpos_backup[ml_coord_totalaxisnum[n_LmIndexNum][i]],md_allow_value[ml_coord_totalaxisnum[n_LmIndexNum][i]]); //(int n_Axis, double d_Distance, int n_Range) 

				if(nRet == BD_GOOD)
				{ 					
					nCurrPosYes++;
					dCurrPos[i]++;
				}
				else
				{

					dCurrPos[i]--;

				}
			}
			//james test 
			//if(nCurrPosYes >= st_coordmot[n_LmIndexNum].l_TotalAxisCnt - 2)
			if(nCurrPosYes >= ml_coord_totalaxiscnt[n_LmIndexNum])
			{
				mn_retry_cnt[ml_coord_totalaxisnum[n_LmIndexNum][0]] = 0;
				return BD_GOOD;
			}
			else
			{//보간동작 완료 
				if(ml_motion_move_time[ml_coord_subaxisnum[n_LmIndexNum][0][0]][2] > 2000) //090609 MOT_MOVE_LIMITTIME)
				{
					if(mn_retry_cnt[ml_coord_subaxisnum[n_LmIndexNum][0][0]] > mn_max_retry_cnt)
					{
						//010006 E A "Motor Move Check Error(01 motor)."
						strMsg.Format(_T("%02d0006"), ml_coord_subaxisnum[n_LmIndexNum][0][0]);
						//wsprintfA(mc_alarmcode, "%S", strMsg);
						clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//						sprintf(mc_alarmcode, "%02d0006", ml_coord_subaxisnum[n_LmIndexNum][0][0]);
						if(mn_errormsg_debug_mode)
						{
							strMsg.Format(_T("[axis=%d] coord cmmLmEndList return_0 error"), ml_coord_subaxisnum[n_LmIndexNum][0][0]);
							//wsprintfA(mc_normal_msg, "%S", strMsg);
							clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//							sprintf(mc_normal_msg, "[axis=%d] coord cmmLmEndList return_0 error",ml_coord_subaxisnum[n_LmIndexNum][0][0]);
							Debug_File_Create(0, mc_normal_msg);
						}

						cmmLmAbortMotion(n_LmIndexNum); //동작중 에러 발생시 강제 종료 시킨다 
						return BD_ERROR;
					}
					else
					{
						mn_retry_cnt[ml_coord_subaxisnum[n_LmIndexNum][0][0]]++;
						cmmLmAbortMotion(n_LmIndexNum); //동작중 에러 발생시 강제 종료 시킨다 

						return BD_RETRY;
					}		
				}	
				else
				{
					for(i=0; i<ml_coord_totalaxiscnt[n_LmIndexNum]; i++) //최대 한버에 4축 보간동작으로 이동가능 
					{
						nRet = Check_MotPosRange(ml_coord_totalaxisnum[n_LmIndexNum][i], md_cmdpos_backup[ml_coord_totalaxisnum[n_LmIndexNum][i]], md_allow_value[ml_coord_totalaxisnum[n_LmIndexNum][i]]); //(int n_Axis, double d_Distance, int n_Range) 
					}			
				}
			} 
		}
		else //0 이면 완료되지 않은 상태 
		{	//동작 중인 상태 		
			if(ml_motion_move_time[ml_coord_subaxisnum[n_LmIndexNum][0][0]][2] > mn_max_move_limit_time) //5초 이상 동작하지 못했으면 에러 
			{
				for(i=0; i<ml_coord_totalaxiscnt[n_LmIndexNum]; i++) //최대 한버에 4축 보간동작으로 이동가능 
				{
					nRet = Check_MotPosRange(ml_coord_totalaxisnum[n_LmIndexNum][i], md_cmdpos_backup[ml_coord_totalaxisnum[n_LmIndexNum][i]], md_allow_value[ml_coord_totalaxisnum[n_LmIndexNum][i]]); //(int n_Axis, double d_Distance, int n_Range) 

					if(nRet == BD_GOOD)
					{ 
						nCurrPosYes++;
					}
					else
					{
						
						dCurrPos[0]++;
						
					}
				}

				//010006 E A "Motor Move Check Error(01 motor)."
				strMsg.Format(_T("%02d0006"), ml_coord_subaxisnum[n_LmIndexNum][0][0]);
				//wsprintfA(mc_alarmcode, "%S", strMsg);
				clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//				sprintf(mc_alarmcode, "%02d0006", ml_coord_subaxisnum[n_LmIndexNum][0][0]);
				if(mn_errormsg_debug_mode)
				{
					strMsg.Format(_T("[axis=%d] coord cmmLmEndList return_0 error"), ml_coord_subaxisnum[n_LmIndexNum][0][0]);
					//wsprintfA(mc_normal_msg, "%S", strMsg);
					clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//					sprintf(mc_normal_msg, "[axis=%d] coord cmmLmEndList return_0 error",ml_coord_subaxisnum[n_LmIndexNum][0][0]);
					Debug_File_Create(0, mc_normal_msg);
				}
				cmmLmAbortMotion(n_LmIndexNum); //동작중 에러 발생시 강제 종료 시킨다 

				return BD_ERROR;						
			}
		}
	}
	else
	{ //음수이면 에러

		//010006 E A "Motor Move Check Error(01 motor)."
		strMsg.Format(_T("%02d0006"), ml_coord_subaxisnum[n_LmIndexNum][0][0]);
		//wsprintfA(mc_alarmcode, "%S", strMsg);
		clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//		sprintf(mc_alarmcode, "%02d0006", ml_coord_subaxisnum[n_LmIndexNum][0][0]);
		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[axis=%d] coord cmmLmEndList return_0 error"), ml_coord_subaxisnum[n_LmIndexNum][0][0]);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//			sprintf(mc_normal_msg, "[axis=%d] coord cmmLmEndList return_0 error",ml_coord_subaxisnum[n_LmIndexNum][0][0]);
			Debug_File_Create(0, mc_normal_msg);
		}
		cmmLmAbortMotion(n_LmIndexNum); //동작중 에러 발생시 강제 종료 시킨다 
		return BD_ERROR;						
	}

	return BD_PROCEED;
}

// ******************************************************************************
// 다중 모터 HOMING 작업 진행 함수                                               
// ******************************************************************************
int CComizoaPublic::HomeCheck_Mot(int n_Axis, int n_HomeMode, int n_TimeOut) 
{
	int  nRet =-1, nRet_1, nRet_2, nRet_3, nFuncRet;
	long IsEnable=0;
	double LimitN=0, LimitP=0;
	CString strMsg;
	int i;

	if(st_work_info.nSimulationMode == 1)
	{// 
		return BD_GOOD;
	}

//	CTL_Lib.SD_Sensor_Enable(0, M_EMPTY_TRAY_ELV, CTL_NO); //sd sensor clear
//	CTL_Lib.SD_Sensor_Enable(0, M_GOOD_TRAY_ELV, CTL_NO); //sd sensor clear


	nFuncRet = BD_PROCEED;
	n_HomeMode = mn_homecheck_method[n_Axis];

	if (n_HomeMode == 1 || n_HomeMode == 7) //org
	{	
		n_HomeMode = 0;
	}
	else if (n_HomeMode == 6)//el-
	{
		n_HomeMode = 1;
	}
	
	nRet = CTL_Lib.Motor_SafetyCheck(0, n_Axis, 0);
	if (nRet == BD_ERROR)
	{
		//2011.0127 삭제 mn_homechk_flag = BD_NO;   
//		Set_MotStop(1, n_Axis);
		for (i=0; i<13; i++)
		{
			cmmSxStop(i, FALSE, FALSE);  // 모터 이동 정지 함수 
		}
		mn_home_step[n_Axis] = 0; //2011.0129 추가 

		return BD_ERROR;
	}
	else if(nRet == BD_RETRY) //091216 추가 
	{
		return BD_RETRY;
	}

//	cmmHomeSetConfig(n_Axis, 6, 0, 10, 0);
//	cmmHomeSetPosClrMode(n_Axis, 0);
	//cmmCfgSetMioProperty(m_nAxis, cmORG_LOGIC, m_nOrgLogic);
	//cmmCfgSetMioProperty(m_nAxis, cmEZ_LOGIC, m_nEzLogic);
	//cmmCfgSetMioProperty(m_nAxis, cmERC_OUT, m_bErcOut);  
	//cmmHomeSetSpeedPattern(m_nAxis, m_nSpeedMode, m_fWorkSpeed, m_fAcc, m_fDec, m_fRvsSpeed);


	switch(mn_home_step[n_Axis])
	{
		case 0 :
			if(1 || mn_sd_io_property_flag[n_Axis] == BD_YES) 
			{
				COMI.Set_Motor_IO_Property(n_Axis, cmSD_EN, cmFALSE); //cmINP_EN=9 //cmFALSE = 0 INP 비활성, cmTRUE = 1 INP 활성 
			}

		//	CTL_Lib.SD_Sensor_Enable(0, n_Axis, CTL_NO); //sd sensor clear //james 20150606 add, sd sensor clear 

//			cmmSxStopEmg(n_Axis);
			cmmSxStop(n_Axis, FALSE, FALSE);  // 모터 이동 정지 함수 
			// 20111213 jtkim
//			COMI.Set_MotSpeed(MOT_SPD_ORG, n_Axis, cmSMODE_T, md_spd_home[n_Axis], 100, 100) ; //st_motor[i].d_spd_home[0] * 10, st_motor[i].d_spd_home[0] * 10);
			Set_MotHomeSpeed(MOT_SPD_CM_CTL, n_Axis, md_spd_home[n_Axis], 100, 100) ; //st_motor[i].d_spd_home[0] * 10, st_motor[i].d_spd_home[0] * 10);
			cmmCfgSetSoftLimit(n_Axis, FALSE, md_limit_position[n_Axis][0], md_limit_position[n_Axis][1]); //S/W limit 해제 
			mn_home_step[n_Axis] = 100;
			break;

		case 100:
			if (Get_MotAlarmStatus(n_Axis) == BD_GOOD)  // BD_ERROR는 알람이 발생하지 않은 상태 
			{
				mn_home_step[n_Axis] = 200;
			}
			else
			{
				mn_home_step[n_Axis] = 110;
			}
			break;

		case 110 :
			if ( Set_MotAlarmClear(n_Axis) == BD_GOOD)  // 모터 ALARM CLEAR 함수
			{
				mn_home_step[n_Axis]  = 100;
			}
			else
			{
				if (mn_retry_cnt[n_Axis] > mn_max_retry_cnt)
				{
					mn_retry_cnt[n_Axis] = 0;						
					mn_home_step[n_Axis] = 0;
					//2011.0127 삭제 st_motor[n_Axis].b_homechk_flag = BD_NO;
					nFuncRet = BD_ERROR;

					//010004 E A "Motor Alarm Error(01 motor)."
					strMsg.Format(_T("%02d0004"), n_Axis);
					//wsprintfA(mc_alarmcode, "%S", strMsg);
					clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//					sprintf(mc_alarmcode, "%02d0004", n_Axis);
					if(mn_errormsg_debug_mode)
					{
						strMsg.Format(_T("[axis=%d] org Set_MotAlarmClear return error"), n_Axis);
						//wsprintfA(mc_normal_msg, "%S", strMsg);
						clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//						sprintf(mc_normal_msg, "[axis=%d] org Set_MotAlarmClear return error",n_Axis);
						Debug_File_Create(0, mc_normal_msg);
					}
				}
				else
				{
					mn_retry_cnt[n_Axis]++ ;
					mn_home_step[n_Axis] = 110;
				}
			}
			break;

		case 200 :
			if ( Get_MotPower(n_Axis) == BD_GOOD )  
			{// 모터 POWER 상태 ON
				mn_home_step[n_Axis] = 300;
			}
			else
			{
				mn_home_step[n_Axis] = 210;
			}
			break;

		case 210 :
			if ( Set_MotPower(n_Axis, BD_YES) == BD_GOOD )  // 모터 POWER ON 설정 함수
			{
				mn_home_step[n_Axis]  = 200;
			}
			else
			{
				if (mn_retry_cnt[n_Axis] > mn_max_retry_cnt)
				{
					mn_retry_cnt[n_Axis] = 0;
					mn_home_step[n_Axis] = 0;
					//2011.0127 삭제 st_motor[n_Axis].b_homechk_flag = BD_NO;
					nFuncRet = BD_ERROR;

					//010005 E A "Motor Power Off Error(01 motor)."
					strMsg.Format(_T("%02d0005"), n_Axis);
					//wsprintfA(mc_alarmcode, "%S", strMsg);
					clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//					sprintf(mc_alarmcode, "%02d0005", n_Axis);
					if(mn_errormsg_debug_mode)
					{
						strMsg.Format(_T("[axis=%d] org Set_MotAlarmClear return error"), n_Axis);
						//wsprintfA(mc_normal_msg, "%S", strMsg);
						clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//						sprintf(mc_normal_msg, "[axis=%d] org Set_MotAlarmClear return error",n_Axis);
						Debug_File_Create(0, mc_normal_msg);
					}
				}
				else
				{
					mn_retry_cnt[n_Axis]++ ;
					mn_home_step[n_Axis] = 210;
				}
			}
			break;

		case 300 :  // 모터 HOMING 
			nRet = cmmHomeMoveStart(n_Axis, cmFALSE);
			if (nRet == cmERR_NONE)
			{
				ml_motion_move_time[n_Axis][0] = GetCurrentTime();
				mn_home_step[n_Axis] = 310;
			}
			else
			{
				if (mn_retry_cnt[n_Axis] > mn_max_retry_cnt)
				{				
					Set_MotStop(1, n_Axis);  // FAS 모터 이동 정지 함수
					mn_home_step[n_Axis] = 0;
					mn_retry_cnt[n_Axis] = 0;
					nFuncRet = BD_ERROR;

					//010003 E A "Motor Home Check Error(01 motor)."
					strMsg.Format(_T("%02d0003"), n_Axis);
					//wsprintfA(mc_alarmcode, "%S", strMsg);
					clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//					sprintf(mc_alarmcode, "%02d0003", n_Axis);
					if(mn_errormsg_debug_mode)
					{
						strMsg.Format(_T("[axis=%d] org Set_MotAlarmClear return error"), n_Axis);
						//wsprintfA(mc_normal_msg, "%S", strMsg);
						clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//						sprintf(mc_normal_msg, "[axis=%d] org Set_MotAlarmClear return error",n_Axis);
						Debug_File_Create(0, mc_normal_msg);
					} 
				}
				else
				{
					mn_retry_cnt[n_Axis]++ ;
					mn_home_step[n_Axis] = 300;
				}
			}
			break;	

		case 310 :  // MOTION DONE 검사 		
			nRet_1 = Get_MotMotionDone(n_Axis);        // 모터 이동 완료 검사 함수 //홈 체크시는 쓰면 안됨,  무조건 멈추면 YES 됨 
			nRet_2 = Get_MotMotionZeroReturn(n_Axis);  // 모터 ZERO BIT 검사 함수 
			cmmHomeGetSuccess(n_Axis, &IsEnable);
			if(IsEnable == cmTRUE) nRet_3 = BD_GOOD;
			else nRet_3 = BD_ERROR;

	//		nRet_3 = BD_GOOD;

			if(n_HomeMode == MOT_SENS_HOME) //home sensor로 홈 체크하는 모드이면 
			{
				if (nRet_1 == BD_GOOD && nRet_2 == BD_GOOD && nRet_3 == BD_GOOD)
				{
					ml_motion_move_time[n_Axis][0] = GetCurrentTime();
					mn_home_step[n_Axis] = 320;
				}	
				else if(nRet_1 == BD_GOOD) //2011.0209 추가 홈 체크 리턴이 나오지 않을때 
				{
					mn_retry_cnt[n_Axis]++ ;
 					mn_home_step[n_Axis] = 301;
				}
			}
			else if(n_HomeMode == MOT_SENS_ELM) //-EL sensor로 홈 체크하는 모드이면 
			{//st_motor[n_Axis].n_homecheck_method == 6)	// -EL로 홈체크 하는 모터일 경우에는 ZeroReturn은 보지 않는다.
				if (nRet_1 == BD_GOOD && nRet_3 == BD_GOOD) //20150428 james if (nRet_3 == BD_GOOD && nRet_3 == BD_GOOD)
				{
					mn_home_step[n_Axis] = 400;				
				}
			}

			ml_motion_move_time[n_Axis][1] = GetCurrentTime();
			ml_motion_move_time[n_Axis][2] = ml_motion_move_time[n_Axis][1] - ml_motion_move_time[n_Axis][0];
			
			if (ml_motion_move_time[n_Axis][2] >= n_TimeOut)
			{
				if (mn_retry_cnt[n_Axis] > mn_max_retry_cnt)
				{				
					Set_MotStop(1, n_Axis);  // FAS 모터 이동 정지 함수
					mn_retry_cnt[n_Axis] = 0;
					mn_home_step[n_Axis] = 0;
					//2011.0127 삭제 st_motor[n_Axis].b_homechk_flag = BD_NO;
					nFuncRet = BD_ERROR;

					//010003 E A "Motor Home Check Error(01 motor)."
					strMsg.Format(_T("%02d0003"), n_Axis);
					//wsprintfA(mc_alarmcode, "%S", strMsg);'
					clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//					sprintf(mc_alarmcode, "%02d0003", n_Axis);
					if(mn_errormsg_debug_mode)
					{
						strMsg.Format(_T("[axis=%d %d %d %d] org cmmHomeGetSuccess return error"), n_Axis, nRet_1, nRet_2, nRet_3);
						//wsprintfA(mc_normal_msg, "%S", strMsg);
						clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//						sprintf(mc_normal_msg, "[axis=%d %d %d %d] org cmmHomeGetSuccess return error",n_Axis, nRet_1, nRet_2, nRet_3);
						Debug_File_Create(0, mc_normal_msg);
					} 
				}
				else
				{
					mn_retry_cnt[n_Axis]++;
	//				mn_home_step[n_Axis] = 300;
				}
			}	
			else if(nRet_1 == 1 && ml_motion_move_time[n_Axis][2] > mn_max_home_limit_time) //2012.0831 추가 
			{
				Set_MotStop(1, n_Axis);  // FAS 모터 이동 정지 함수
				mn_retry_cnt[n_Axis] = 0;
				mn_home_step[n_Axis] = 0;
				//2011.0127 삭제 st_motor[n_Axis].b_homechk_flag = BD_NO;
				nFuncRet = BD_ERROR;

				//010003 E A "Motor Home Check Error(01 motor)."
				strMsg.Format(_T("%02d0003"), n_Axis);
				//wsprintfA(mc_alarmcode, "%S", strMsg);
				clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//					sprintf(mc_alarmcode, "%02d0003", n_Axis);
				if(mn_errormsg_debug_mode)
				{
					strMsg.Format(_T("[axis=%d %d %d %d] org cmmHomeGetSuccess return error"), n_Axis, nRet_1, nRet_2, nRet_3);
					//wsprintfA(mc_normal_msg, "%S", strMsg);
					clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//						sprintf(mc_normal_msg, "[axis=%d %d %d %d] org cmmHomeGetSuccess return error",n_Axis, nRet_1, nRet_2, nRet_3);
					Debug_File_Create(0, mc_normal_msg);
				} 
			}
			break;	

		case 301: //2011.0209 추가 트레이 트랜스퍼 Z축 break가 제대로 on/off 되지 안아 모터 이동 및 홈체크 못함 
			Set_MotAlarmClear(n_Axis);
			mn_home_step[n_Axis] = 0;
			break;

		case 320 :  // HOME 센서 검사 
			if (Get_MotIOSensor(n_Axis, MOT_SENS_HOME) == BD_GOOD)  // 모터 센서 상태 검사 함수 [EL(+)    EL(-)    HOME]
			{
				mn_home_step[n_Axis] = 400;
			}
			else
			{
				// 20111212 jtkim
				//ml_motion_move_time[n_Axis][0] = GetCurrentTime();
				ml_motion_move_time[n_Axis][1] = GetCurrentTime();
				ml_motion_move_time[n_Axis][2] = ml_motion_move_time[n_Axis][1] - ml_motion_move_time[n_Axis][0];
				if (ml_motion_move_time[n_Axis][2] > 500)
				{
					Set_MotStop(1, n_Axis);  // 모터 이동 정지 함수
					mn_retry_cnt[n_Axis] = 0;
					mn_home_step[n_Axis] = 0;
					//2011.0127 삭제 st_motor[n_Axis].b_homechk_flag = BD_NO;
					nFuncRet = BD_ERROR;

					//010003 E A "Motor Home Check Error(01 motor)."
					strMsg.Format(_T("%02d0003"), n_Axis);
					//wsprintfA(mc_alarmcode, "%S", strMsg);
					clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//					sprintf(mc_alarmcode, "%02d0003", n_Axis);
					if(mn_errormsg_debug_mode)
					{
						strMsg.Format(_T("[axis=%d ] org MOT_SENS_HOME return error"), n_Axis);
						//wsprintfA(mc_normal_msg, "%S", strMsg);
						clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//						sprintf(mc_normal_msg, "[axis=%d ] org MOT_SENS_HOME return error",n_Axis);
						Debug_File_Create(0, mc_normal_msg);
					} 
				}			
			}
			break;

		case 400:
			ml_motion_move_time[n_Axis][0] = GetCurrentTime();
			mn_home_step[n_Axis] = 410;
			break;


		case 410 :  // 모터 커맨드 위치 초기화 
			// 20111212 jtkim
			//ml_motion_move_time[n_Axis][0] = GetCurrentTime();
			ml_motion_move_time[n_Axis][1] = GetCurrentTime();
			ml_motion_move_time[n_Axis][2] = ml_motion_move_time[n_Axis][1] - ml_motion_move_time[n_Axis][0];
			if (ml_motion_move_time[n_Axis][2] > 1000) //1초후에 정지한 상태에서 엔코더 0로 클리어 
			{
				cmmStSetPosition(n_Axis, cmCNT_COMM, (double)0);
				cmmStSetPosition(n_Axis, cmCNT_FEED, (double)0);

				if(mn_errormsg_debug_mode)
				{
					strMsg.Format(_T("[axis=%d ] org check end ok"), n_Axis);
					//wsprintfA(mc_normal_msg, "%S", strMsg);
					clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//					sprintf(mc_normal_msg, "[axis=%d ] org check end ok",n_Axis);
					Debug_File_Create(0, mc_normal_msg);
				} 

				mn_home_step[n_Axis] = 500;
			}		 
			break;

		case 500 :  // HOMING 완료 
	 		cmmCfgSetSoftLimit(n_Axis, TRUE, md_limit_position[n_Axis][0], md_limit_position[n_Axis][1]); //S/W limit 설정  
 			mn_retry_cnt[n_Axis] = 0;
			mn_home_step[n_Axis] = 0;
			mn_homechk_flag[n_Axis] = BD_YES;  

			cmmSxStop(n_Axis, FALSE, FALSE);  // 모터 이동 정지 함수 
			nFuncRet = BD_GOOD;
			break;
	}

	return nFuncRet;  
}

int CComizoaPublic::Start_JogMove(long l_AxisNum, long l_Dir)//2011.0112 , double d_SpeedRatio[3])
{
	int  nRet = 0; 
	CString strMsg;

	if(st_work_info.nSimulationMode == 1)
	{// 
		return BD_GOOD;
	}
	
	nRet = CTL_Lib.Motor_SafetyCheck(3, l_AxisNum, 0);
	if (nRet != BD_GOOD)
	{
		if(nRet == BD_RETRY) //091216 추가 
		{
			return BD_RETRY;
		}

		return BD_ERROR; //MOT_MOT_RETRY, BD_ERROR; MOT_SAFETY_ERROR;
	}
/*
	nRet = Set_MotSpeed(MOT_SPD_VEL, l_AxisNum, cmSMODE_KEEP, d_SpeedRatio[0], st_motor[l_AxisNum].d_spd_vel[1], st_motor[l_AxisNum].d_spd_vel[2]);
	if(nRet == CTL_ERROR)
	{
		sprintf(cJamcode, "%02d0004",  l_AxisNum); alarm.str_code = _T(cJamcode); //091216//000004 1 00 "Load Index Motor Move Setting Error."
		CtlBdFunc.ms_ErrMsg.Format("[Start_JogMove] Check Error");				
		CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, l_AxisNum, cJamcode, CtlBdFunc.ms_ErrMsg);
		return BD_ERROR;
	}	
*/
	if(mn_sd_io_property_flag[l_AxisNum] == BD_YES) //2011.0104 SD 센서 추가관련 기능 추가 
	{//2011.0104 james 기능 추가 
		COMI.Set_Motor_IO_Property(l_AxisNum, cmSD_EN, cmFALSE);	//cmSD_EN=14 //cmFALSE = 0 SD 비활성, cmTRUE = 1 SD 활성 
	}

	
	nRet = cmmSxVMoveStart(l_AxisNum, l_Dir);
	if (nRet == cmERR_NONE)
	{
		return BD_GOOD;
	}
	else
	{
		//010002 E A "Motor Move Setting Error01 motor)."
		strMsg.Format(_T("%02d0002"), l_AxisNum);
		//wsprintfA(mc_alarmcode, "%S", strMsg);
		clsFunc.OnStringToChar(strMsg, mc_alarmcode);
//		sprintf(mc_alarmcode, "%02d0002", l_AxisNum);
		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[axis=%d] cmmSxVMoveStart return error"), l_AxisNum);
			//wsprintfA(mc_normal_msg, "%S", strMsg);
			clsFunc.OnStringToChar(strMsg, mc_normal_msg);
//			sprintf(mc_normal_msg, "[axis=%d ] cmmSxVMoveStart return error",l_AxisNum);
			Debug_File_Create(0, mc_normal_msg);
		} 
		return BD_ERROR;
	}
	
	return BD_ERROR;
}




int CComizoaPublic::Debug_File_Create(int n_mode, char* s_msg) //2012.1029 jamestest  CString s_msg)
{
//2011.0417	sing.Lock(); //2011.0417 
	CFileFind filefind;
	CString mstr_cur_year, mstr_cur_month, mstr_cur_day, str_display_time; // 현재 년, 월, 일 정보 문자형으로 변환하여 저장할 변수 
	int mn_cur_year, mn_cur_month, mn_cur_day, mn_cur_hour; // 현재 년, 월, 일 정보 저장 변수 
	CString mstr_file_name;		// 마지막으로 생성된 파일 이름 저장 변수 
	CString mstr_create_file;	// 알람 정보 저장할 파일에 대한 [폴더]+[파일명]+[확장자] 설정 변수 
	CString mstr_list_name, mstr_temp_data;  // 각 부분별 알람 발생 횟수 정보 저장할 로그 파일 정보 저장 변수 
	CString mstr_content;		// 한 라인에 출력할 문자열 정보 저장 변수 
	COleDateTime time_cur;		// 검사할 시간 정보 저장 변수 
	CTime m_time_current;		// 간략한 헝식의 현재 시간 정보 저장 변수
	char chr_buf[20]={0,};
	int mn_existence;			// 파일 존재 유무 설정 플래그 
	char fileName[256]={0,};			// 검색할 파일 정보 설정 함수 
	FILE  *fp = NULL;					// 파일에 대한 포인터 설정 변수 

	CString BackupName;
	CString mstr_cur_hour, mstr_cur_min, mstr_cur_sec;
	CString strTemp;

	int nRet=BD_GOOD;

	if (0) //2012.1029 jamestest s_msg.IsEmpty() == TRUE)  
	{
		return BD_ERROR;
	}

	// **************************************************************************
	// 파일 이름으로 사용할 날짜 정보를 얻는다                                   
	// **************************************************************************
	time_cur = COleDateTime::GetCurrentTime();  // 현재 시간 정보를 얻는다. 

	m_time_current = CTime::GetCurrentTime() ;  // 간략한 형식의 현재 시간 정보 얻는다. 

	mn_cur_year = time_cur.GetYear();  

	mn_cur_month = time_cur.GetMonth();  

    mn_cur_day = time_cur.GetDay();  

	mn_cur_hour = time_cur.GetHour();
	// **************************************************************************

	// **************************************************************************
	// 날짜 정보를 문자형으로 변환하여 변수에 설정한다                           
	// **************************************************************************
//	mstr_cur_year = LPCTSTR(_itoa( mn_cur_year, chr_buf, 10 ));  
	mstr_cur_year.Format(_T("%04d"), mn_cur_year);
	mstr_cur_month.Format(_T("%02d"), mn_cur_month);
	mstr_cur_day.Format(_T("%02d"), mn_cur_day);

	// **************************************************************************
	// 현재 시간 정보 얻는다                                                     
	// **************************************************************************
	mn_cur_hour = time_cur.GetHour();				// 현재 시간 정보를 설정한다. 
//2012.1029 jamestest mstr_cur_hour.Format("%d",time_cur.GetHour());	// 현재 시간 정보를 설정한다.
//2012.1029 jamestest mstr_cur_min.Format("%d",time_cur.GetMinute());	// 현재 분 정보를 설정한다. 
//2012.1029 jamestest mstr_cur_sec.Format("%d",time_cur.GetSecond());	// 현재 초 정보를 설정한다. 
	str_display_time = m_time_current.Format("%c");	// 리스트 파일에 출력할 타이틀 시간 정보 설정 
	// **************************************************************************


	mstr_file_name =  mstr_cur_year;
	mstr_file_name += mstr_cur_month; 
	mstr_file_name += mstr_cur_day; 
	mstr_create_file = _T("C:\\AMT_LOG\\Motor\\") + mstr_file_name;
	mstr_create_file += _T(".TXT");

	//wsprintfA(fileName, "%S", mstr_create_file);
	clsFunc.OnStringToChar(mstr_create_file, fileName);

	mn_existence = filefind.FindFile(mstr_create_file,0);

	nRet = FileSizeCheck(mstr_create_file, 1048576, BD_YES); //size and rename
	if(nRet == 1) //file size over
	{
//2012.1029 jamestest		BackupName = mstr_create_file + mstr_cur_hour + mstr_cur_min +  mstr_cur_sec + ".bak" ;
//2012.1029 jamestest		rename(mstr_create_file, BackupName);
	}

	//if (fp = fopen(fileName, "a+")) == NULL)  
	fopen_s(&fp, fileName, "a+");
	if(!fp)
 	{
		//AfxMessageBox("File open failure!..");
		//wsprintfA(mc_normal_msg, "%S", _T("File open failure!.._4"));
		clsFunc.OnStringToChar(_T("File open failure!.._4"), mc_normal_msg);
//		sprintf(mc_normal_msg, "File open failure!.._4");
		return BD_ERROR;
	}

	// **************************************************************************
	// 로그 파일에 현재 발생한 알람 정보 저장한다                                
	// **************************************************************************
	
	mstr_content = str_display_time;//2012.1029 jamestest

	mstr_content += _T(" : ");
	strTemp = clsFunc.OnCharToString(s_msg, 1000);
	mstr_content += strTemp;
	mstr_content += _T("\n");
//	_snprintf(fileName, sizeof(fileName), "%S", mstr_content);
	//wsprintfA(fileName, "%S", mstr_content);
	clsFunc.OnStringToChar(mstr_content, fileName);
//	fprintf(fp,_T("%S\n"),mstr_content) ;
	fprintf(fp,fileName) ;

	if (ferror(fp))  
	{
		//wsprintfA(mc_normal_msg, "%S", _T("Debug_File_Create ferror return error"));
		clsFunc.OnStringToChar(_T("Debug_File_Create ferror return error"), mc_normal_msg);
//		sprintf(mc_normal_msg,"Debug_File_Create ferror return error");	
		clearerr(fp); //2011.0418 

		fclose(fp) ;  //2011.0417  파일 종료
		return BD_ERROR;
	}
	fclose(fp);  // 파일을 종료한다.

	return nRet;
}

int CComizoaPublic::FileSizeCheck(CString s_filename, long l_size, int n_check)
{
	CFileFind finder;
	long Len=0, flag=0;
	BOOL bContinue;
	
	if(bContinue = finder.FindFile(s_filename))
	{	
		if (n_check == BD_YES)			// Size를 체크하는것이면...
		{
			finder.FindFile(s_filename);
			finder.FindNextFile();
			finder.GetFileName();
			Len = (long)finder.GetLength();
			if(Len > l_size)
			{
				flag = 1;
			}
			finder.Close();	
		}
		else						// Size를 확인하는것이면...
		{
			finder.FindFile(s_filename);
			finder.FindNextFile();
			finder.GetFileName();
			Len = (long)finder.GetLength();
			
			flag = Len;
			
			finder.Close();	
		}
	}
	
	return flag;
}

int CComizoaPublic::Start_Continue(int n_axis, double d_work)
{
	if(Get_MotPower(n_axis) == BD_GOOD)
	{
		Set_MotSpeed(MOT_SPD_CM_CTL, n_axis, cmSMODE_KEEP, d_work, 100, 100);		// 축의 속도를 변경한다.
					
		if(cmmSxVMoveStart(n_axis, 1) != cmERR_NONE)
		{
			return BD_ERROR;
		}

		return BD_GOOD;
	}
	else
	{
		if(Set_MotPower(n_axis, TRUE) == BD_ERROR)
		{
			return BD_ERROR;
		}

		Sleep(100);

		Set_MotSpeed(MOT_SPD_JOG, n_axis, cmSMODE_KEEP, md_spd_jog[n_axis], 100, 100);		// 축의 속도를 변경한다.
					
		if(cmmSxVMoveStart(n_axis, 1) != cmERR_NONE)
		{
			return BD_ERROR;
		}

		return BD_GOOD;
	}
}

void CComizoaPublic::Stop_Continue(int n_axis)
{
	cmmSxStop(n_axis, FALSE, FALSE);  // 모터 이동 정지 함수 
}

int CComizoaPublic::Set_MotJogSpeed(int n_SpeedCase, int n_Axis, double d_work, double d_accel, double d_decel)
{
	int  nRet = BD_ERROR, nFlag = 0;
	double dVel = 0;
	CString strMsg;

	if(st_work_info.nSimulationMode == 1)
	{// 
		return BD_GOOD;
	}
	
	if(n_SpeedCase == MOT_SPD_CM_CTL) //2011.1104 
	{
		if(d_work  > 0.5)
		{
			dVel = d_work / 2;
		}
		else
		{
			dVel = d_work;
		}
		
	}
	else
	{
		if(dVel > 1000000)
		{//6000 rpm 사용 
			dVel = (double)10000;
		}
	}

	//2011.0121 변경 nRet = cmmCfgSetSpeedPattern(n_Axis, n_SpeedMode, dVel, dAccPPS, dDecPPS);//cmSMODE_T
	nRet = cmmCfgSetSpeedPattern(n_Axis, cmSMODE_T, dVel, d_accel, d_decel);
    if (nRet == cmERR_NONE)
	{
		nRet = BD_GOOD;
	}
	else
	{
		if(mn_errormsg_debug_mode)
		{
			strMsg.Format(_T("[axis %d] cmmCfgSetSpeedPattern return error"), n_Axis);
			//wsprintfA(mc_normal_msg, "%S", _T("Debug_File_Create ferror return error"));
			clsFunc.OnStringToChar(_T("Debug_File_Create ferror return error"), mc_normal_msg);
//			sprintf(mc_normal_msg, "[axis %d] cmmCfgSetSpeedPattern return error", n_Axis);
			Debug_File_Create(0, mc_normal_msg);
		}
		nRet = BD_ERROR;
	}

	return nRet;
}


int CComizoaPublic::Check_MotPosRange(int n_Axis, double d_Distance, double d_Range) 
{
	int    nRet = BD_ERROR;
	double dGetActualPos=0;
	double dP = 0, dN = 0;
	
	if (st_work_info.nSimulationMode == 1)
	{//BD_ERROR == 알람이 발생하지 않은 상태
		return BD_GOOD;
	}
	
	dN = (double)d_Distance - (double)(d_Range); 
	dP = (double)d_Distance + (double)(d_Range);
	
		dGetActualPos = Get_MotCurrentPos(n_Axis);  // 모터 특정 축의 현재 위치 리턴 함수 
	//dGetActualPos = g_comiMgr.Get_MotCurrentPos(n_Axis);
	
	if (dGetActualPos > dN && dGetActualPos < dP) 	 
	{
		nRet = BD_GOOD;
	}
	else
	{
		nRet = BD_ERROR;
	}
	
	return nRet;
}


//2015.0223 james 모터 안전위치 및 target 위치 이동전에 조건을 확인하기 위해 추가 
int CComizoaPublic::Check_MotPosRange_DirectionLimit(int n_MoveDirection, int n_Axis, double d_ReferencePos, double d_TragetPos, double d_Range) 
{	//n_Direction -1: - 방향으로 range 만큼 조건이 허락된다, 1: + 방향으로 range 만큼 이동가능 조건이허락 된다.
	int    nRet_1 = BD_ERROR, nRet_2 = BD_ERROR;
	double dGetActualPos=0;
	double dRP = 0, dRN = 0;
	double dTP = 0, dTN = 0;
	double dP = 0, dN = 0;
	int dCal = 0;
	
	if (st_work_info.nSimulationMode == 1)
	{//BD_ERROR == 알람이 발생하지 않은 상태
		return BD_GOOD;
	}

	dGetActualPos = Get_MotCurrentPos(n_Axis);  // 모터 특정 축의 현재 위치 리턴 함수 

	dCal = (int)(d_ReferencePos - d_TragetPos);
	if(dCal <= 0) 
	{//- 방향에서 + 방향으로 이동예정  (d_ReferencePos 위치가 target 보다 작은 위치에 있다 
		dN = d_ReferencePos - d_Range; //
		dP = d_TragetPos    + d_Range;
	}
	else //if(dCal > 0)
	{//+ 방향에서 - 방향으로 이동예정
		dN = d_TragetPos		- d_Range; //
		dP = d_ReferencePos   + d_Range;
	}

	if(dN < dGetActualPos && dP > dGetActualPos)
	{
		nRet_1 = BD_GOOD;
	}
	else
	{
		nRet_1 = BD_ERROR;
	}
	/*	
	dGetActualPos = Get_MotCurrentPos(n_Axis);  // 모터 특정 축의 현재 위치 리턴 함수 
	//작업 시작전 기준 위치값 체크
	dRN = (double)d_ReferencePos - (double)(d_Range); 
	dRP = (double)d_ReferencePos + (double)(d_Range);	 

	//이동할 목표 위치의 최종 위치값 
	dTN = (double)d_TragetPos - (double)(d_Range); 
	dTP = (double)d_TragetPos + (double)(d_Range);
	if(n_MoveDirection == 1) //현재 위치기준 + 방향으로만 조건 체크 
	{		
		if (dGetActualPos >= dRN && dGetActualPos <= dTP) 	 //+ 방향으로 이동 목적 
		{//모터 위치가 현재 위치보다는 커야하고, 목표 우치보다는 작아야 한다 
			nRet_1 = BD_GOOD;
		}
		else
		{
			nRet_1 = BD_ERROR;
		}

	}
	else if(n_MoveDirection == -1) //현재 위치기준 - 방향으로만 조건 체크 
	{
		if (dGetActualPos <= dRP && dGetActualPos >= dTN) 	 //- 방향으로 이동 목적 
		{//모터 위치가 현재 위치보다는 작고, 가야될 목표 위치보다는 커야 한다
			nRet_1 = BD_GOOD;
		}
		else
		{
			nRet_1 = BD_ERROR;
		}
	} 
	else
	{
		nRet_1 = BD_ERROR;
	}	*/
	return nRet_1;
}

 

//***********************************************************************************************
// MOTOR STATE (모터 동작상태 반환)	
// 모터 정지, 이동등 각종 모터 상태를 체크시 사용 
// 20150428 james 추가 ////////////
//***********************************************************************************************
int CComizoaPublic::Check_Motion_State(int n_Axis, long l_State) 
{
	int    nRet = BD_ERROR;
	long	   lMotionState = 0;
	CString strMsg;
	
	if(st_work_info.nSimulationMode == 1)
	{
		return BD_GOOD;
	}
	

	nRet = cmmStReadMotionState(n_Axis, &lMotionState);  //20150417 모터 이동전에는 상태체크로 모터 상태를 체크하기로 한다 
	
	if (nRet == cmERR_NONE)
	{//정상적으로 처리됨 
		if (m_nDoneFlag[n_Axis] == YES) m_nDoneFlag[n_Axis] = NO;

		if(lMotionState == l_State) //모터 상태가 체크하려는 상태와 같은 상태면 참.
		{
			nRet = BD_GOOD;
		}
		else if(0 > lMotionState) //음수 값이면 에러처리이다 
		{
			// jtkim 20150608
			//ml_alarmcode_id = nRet;    
			//CTL_Lib.Motor_Error_Occurrence(3, n_Axis, ml_alarmcode_id, _T("Check_Motion_State_cmmStReadMotionState_lMotionStateValue_Error"));	
			if(st_motor_info[n_Axis].n_sd_mv_chk == 1) //sd를 조건을 확인하기 위해 이동한다 
			{
				ml_alarmcode_id = nRet;    
				CTL_Lib.Motor_Error_Occurrence(3, n_Axis, ml_alarmcode_id, _T("Check_Motion_State_cmmStReadMotionState_lMotionStateValue_Error"));
			}

			return BD_ERROR;
		}
		else //그 이상이면 동작 중이다 
		{
			nRet = BD_PROCEED; //체크 중이다 
		}
	}
	else
	{
		//20150507 추가
		if(st_motor_info[n_Axis].n_sd_mv_chk == 1) //sd를 조건을 확인하기 위해 이동한다 
		{
			if(nRet == cmERR_STOP_BY_SD)//-5012
			{
				if(l_State == cmMST_STOP)
				{
					cmmSxStop(n_Axis, FALSE, FALSE);  // 모터 이동 정지 함수 
					return BD_GOOD;
				}
			}
		}

		// jtkim 20150501
		if (m_nDoneFlag[n_Axis] == NO)
		{
			m_nDoneFlag[n_Axis] = YES;
			m_dwDoneWaitTime[n_Axis][0] = GetTickCount();

			 return BD_PROCEED; //체크 중이다 
		}
		else
		{
			m_dwDoneWaitTime[n_Axis][1] = GetTickCount();
			m_dwDoneWaitTime[n_Axis][2] = m_dwDoneWaitTime[n_Axis][1] - m_dwDoneWaitTime[n_Axis][0];

			if (m_dwDoneWaitTime[n_Axis][2] <= (DWORD)0)
			{
				m_dwDoneWaitTime[n_Axis][0] = GetTickCount();

				return BD_PROCEED; //체크 중이다 
			}

			if (m_dwDoneWaitTime[n_Axis][2] > (DWORD)50)
			{
				ml_alarmcode_id = nRet;    
				CTL_Lib.Motor_Error_Occurrence(3, n_Axis, ml_alarmcode_id, _T("Check_Motion_State_cmmStReadMotionState_Return_Error"));	
				
				m_nDoneFlag[n_Axis] = NO;

				return BD_ERROR;
			}
		}
	}	
	return nRet;
}