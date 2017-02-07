// ComizoaPublic.h: interface for the CComizoaPublic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMIZOAPUBLIC_H__E0F02F5E_1503_4BEB_BE01_56C396CEE734__INCLUDED_)
#define AFX_COMIZOAPUBLIC_H__E0F02F5E_1503_4BEB_BE01_56C396CEE734__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//20150428 james 
#define MOT_STOP_DEC			0   //모터 감속 정지 
#define MOT_STOP_EMG			1   //모터 즉시정지 //20150420 
#define MOT_STOP_DEC_BLOCKING	2   //모터 감속 정지할떄까지 blocking 상태, 다른것 사용불가 

/////////////////////////////////////////////////////////
//DLL 구조로 변경예정 //2011.1007 추가 
/////////////////////////////////////////////////////////
#define BD_MAX_MOTOR		100
#define BD_NO				0		//어떤 조건이 클리어된 상태 
#define BD_YES				1		//어떤 조건을 요청한 상태 

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//motor, i/O 보드등의 리턴값은 이 것만 사용한다  
//////////////////////////////////////////////////////////////////////////////////////////////////////////

#define BD_ERROR	   -1
#define BD_READY		0
#define BD_PROCEED		1
#define BD_GOOD			2
#define BD_RETRY		3
#define BD_SAFETY		4

//MOT Speed Setting  
#define	MOT_SPD_VEL			0
#define	MOT_SPD_ACC			1
#define MOT_SPD_DEC			2
#define MOT_SPD_JOG			3
#define MOT_SPD_ORG			4
#define MOT_SPD_FEEDRATE	5
#define MOT_SPD_INTERACC	6

#define MOT_SPD_CM_CTL		7 //2011.1104 속도를 cm로 제어한다 
#define MOT_SPD_RPM_CTL		8 //속도를 rpm으로 제어 

//motor sensor 
#define MOT_SENS_HOME		0
#define MOT_SENS_ELM		1
#define MOT_SENS_ELP		2
#define MOT_SENS_SD			3 //slow down sensor 

//모터 방향 정의 
#define MOT_PLUS			1
#define MOT_MINUS			0

 
#define MOT_RTY_CNT			4	//파스텍 제어기 관련 문제시 재 시도하는 횟수

#define MOT_MOVE_LIMITTIME	60000	//이동 후 3초이내동작 완료할 것 	 
#define MOT_TIMEOUT			90000   //1분의 time out를 가지고 동작한다  //홈 체크시 사용 

//가동상태 저장 
#define CTL_dSTOP			0
#define CTL_dRUN			1
#define CTL_dIDLE			2
#define CTL_dJAM			3
#define CTL_dLOTEND			4
#define CTL_dINIT			5
#define CTL_dWARNING		6
#define CTL_dLOCK			7
#define CTL_dSELFCHECK		8
#define CTL_dMAINT			9




class CComizoaPublic  
{
public:
	CComizoaPublic();
	virtual ~CComizoaPublic();

	// Attributes
public:
	
	long ml_alarmcode_id;//20150422 james 



	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 2011.1021 
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	int mn_errormsg_debug_mode; //에러 메세지 출력방법 => 0:사용안함 , 1:사용

	int mn_max_move_limit_time;	//모터 동작 리미트시간 설정
	int mn_max_home_limit_time; //홈 체크 리미트 시간 설정 

	int mn_run_status;			//장비 가동상태 정보를 받아온다 	

	int mn_motor_type[BD_MAX_MOTOR]; // jtkim 20130823 step motor comm처리 문제로 추가

	int mn_homechk_flag[BD_MAX_MOTOR]; //홈 체크 완료여부 플레그 BD_YES:완료, BD_NO:홈체크 안함 

	int mn_home_step[BD_MAX_MOTOR]; //home check시 사용하는 step 
	
	int mn_homecheck_method[BD_MAX_MOTOR]; //home check 방법 정의 

	// jtkim 20150501
	int   m_nDoneFlag[BD_MAX_MOTOR];
	DWORD m_dwDoneWaitTime[BD_MAX_MOTOR][3];	   // motion done wait time....


	long ml_motion_move_time[BD_MAX_MOTOR][3]; //동작관련 시간체크 
	long ml_retry_time_wait[BD_MAX_MOTOR][3];  //재시도 동작관련 시간체크 

	int mn_speed_control_flag[BD_MAX_MOTOR]; //speed control flag 확인 YES면 사용, NO면 사용안함 
	
	int mn_max_retry_cnt;					//리트라이할 최대 횟수설정 
	int mn_retry_cnt[BD_MAX_MOTOR];			//모터별 재시도 횟수 저장 

	double md_limit_position[BD_MAX_MOTOR][2];//[0]:+리미트 값정의, [1]:- 리미트값 정의 
	double md_cmdpos_backup[BD_MAX_MOTOR];		//장비 런중 이동한 모터 최종 위치저장 
	double md_spd_vel[BD_MAX_MOTOR][3]; 		//[0]:VEL, [1]:accel,[2]:deceel
	double md_spd_home[BD_MAX_MOTOR];			//home 체크 속도 설정 
	double md_spd_jog[BD_MAX_MOTOR];			//home 체크 속도 설정 
	double md_initial_speed[BD_MAX_MOTOR];			//home 체크 속도 설정 

	//2015.0219 james int mn_allow_value[BD_MAX_MOTOR];			//이동 완료 후의 허용오차 범위 지정
	double md_allow_value[BD_MAX_MOTOR];			//이동 완료 후의 허용오차 범위 지정
	double md_deviation[BD_MAX_MOTOR][3];		//커멘드와 피드백 위치값 차이 문제로 확인위해 추가 

	int mn_sd_io_property_flag[BD_MAX_MOTOR];	//sd io 설정 사용유무 설정 
	int mn_interrupt_flag[BD_MAX_MOTOR];        //interrupt 사용 유무 설정 
	int ml_Get_InterruptMask[BD_MAX_MOTOR][20]; //inturrupt 설정 
	int mn_runspeed_rate;						//장비 런 스피드 정보를 장비에서 받는다 
	int mn_manualspeed_rate;					//장비 메뉴얼 스피드 정보를 장비에서 받는다 
	int mn_motorbd_init_end;				    //motor board 초기화 여부 

	int mn_totalmotboard_number;				//장비에서 사용하는 모터보드 전체 수량 
	int mn_totalmotoraxis_number;				//장비에서 사용하는 total motor 축수 
	int mn_bd_axis_number[10];					//모터 보드별 축수를 정의한다 
 
	char mc_alarmcode[10];						//모터에서 발생하는 error 값 정의.

	//2011.0910 
	char mc_normal_msg[1000];//CHAR_MAX_SIZE];
	char mc_abnormal_msg[1000];//CHAR_MAX_SIZE];
	////////////////////////////////////////////////

	//linear motor move setting //max 8개 셋팅 
	///////////////////////////////////////////
	int	mn_indexmap_init[8];	   //현재 index map이 초기화가 끝났는지 확인한다 
	long ml_axiscnt[8];            //현재의 IndexNum에서 사용 가능한 모터 수량 최대 4개  
	long mp_axisnum[8][4];        //현재의 IndexNum에서 사용하는 모터 실재 번호를 가진다 
	double md_poslist[8][4];		   //최대 4 포인트를 한번에 이동가능하다 
	double md_spdratio[8][3];        //속도비율 [0]:Vel, [1]:Acc, [2]:Dec

	//2015.0219 james 추가 
	long ml_linear_move_time[8][3]; //동작관련 시간체크 
	int  mn_linear_retry_cnt[8];	   //모터별 재시도 횟수 저장 
	//////////////////////////////

	////////////////////////////////////////////
	// coord move  max 8
	////////////////////////////////////////////
	int		 mn_coord_indexmap_init[8];		//현재 index map이 셋팅이 초기화가 끝났는지 확인한다 
	long	 ml_coord_totalmovecnt[8];		//총 동작할 구간 정의 보간동작도 한번으로 한다 (최대 10회) 
	long     ml_coord_totalaxiscnt[8];        //현재의 IndexNum에서 사용 가능한 모터 수량 
	long     ml_coord_totalaxisnum[8][4];    //현재의 IndexNum에서 사용하는 모터번호를 가진다 
	
	long    ml_coord_IxIndexnum[8][8];	    //최대 0 ~ 7번, 보간 동작시 사용할 수 있는 번호 
	long    ml_coord_subaxiscnt[8][8];	    //현재 index에서 동작할 모터 수량
	long    ml_coord_subaxisnum[8][8][4];    //[10] = index 구간 8회 //[0][0]:첫번째 모터번호, [0][1]:2번째 모터번호  
	double	md_coord_poslist[8][8][4];	    //[10] = 10회구간 , //[0]=앞축의 모터 이동 값(단축시 이것만 사용), [1]=뒤축의 모터 이동 값 (보간 동작시 사용)
	double	md_coord_axisratio[8][8][3];		    //최대 두축의 이동구간 속도 비율 ,[0]:vel, [1]:acc, [2]:dec ratio
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long   ml_LmIndexNumber[16];//[16]=최대 16개의 Map를 가질수 있다
	long   ml_IxIndexNumber[8]; //[8] =최대 8개의 Map를 가질수 있다

	// 20140306 jtkim
	int		m_nMotInfo[BD_MAX_MOTOR];
 
// Operations
public:
	
	int	Check_Motion_State(int n_Axis, long l_MotState);  //20150428

	///////////////////////////////////////////

	int Check_MotPosRange_DirectionLimit(int n_MoveDirection, int n_Axis, double d_ReferencePos, double d_TragetPos, double d_Range) ; //james 2012.0830 안전 조건 확인용 추가 
	int Check_MotPosRange(int n_Axis, double d_Distance, double d_Range) ; //james 2012.0830


	int Set_MotJogSpeed(int n_SpeedCase, int n_Axis, double d_work, double d_accel, double d_decel);
	void Stop_Continue(int n_axis);
	int Start_Continue(int n_axis, double d_work);
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 2011.1021 
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	int Debug_File_Create(int n_mode,  char* s_msg); //2012.1029 jamestest  CString s_msg)CString s_msg);
	int FileSizeCheck(CString s_filename, long l_size, int n_check);

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	int Set_MotInitSpeed(int n_Axis, double d_work);
	int Set_MotHomeSpeed(int n_SpeedCase, int n_Axis, double d_work, double d_accel, double d_decel);
	int Set_MotSpeed(int n_SpeedCase, int n_Axis, int n_SpeedMode, double d_work, double d_accel, double d_decel);
	int Set_MotSpeed_Ratio(int n_Axis, int n_SpeedMode, double d_work_Ratio, double d_accel_Ratio, double d_decel_Ratio) ;
	int Set_Override_Speed(int n_Axis, double d_Speed, int n_SpeedMode);

	//2011.1104 추가 
	int Set_SpeedRange(int n_Axis, double d_max_pps); //2011.1104 	//모터의 속도 사용 범위를 정한다(Min ~ Max)
	int Set_MotUnitSpeed(int n_Axis, double n_UnitSpeed); //2011.1104 추가 //2011.1104 추가 속도를 초당 이동거리로 설정 가능 

	int Set_Override_Move_Pos(int n_Axis, double d_TargetPos);
	
	int Get_Deviation_Position(int n_Axis, double d_AllowPos, double * d_Get_DevPos);
	int Set_Motor_IO_Property(long n_Axis, long  n_PropId, long n_OnOff); //090719 추가 
	
	int Start_JogMove(long l_AxisNum, long l_Dir); //2011.0119 , double dp_SpeedRatio[3]);
	int Initialize_MotBoard(CString s_file);
	void Close_MotBoard();//보드 클로즈 

	int Set_CMD_CTL_Mode(int n_axis, int n_cmd_type);

	int Set_Simulation_Mode(int n_axis, int n_simul_mode);

	int Set_MotPower(int n_Axis, int n_OnOff);
	int Get_MotPower(int n_Axis);
	int Set_MotAlarmClear(int n_Axis);
	int Get_MotAlarmStatus(int n_Axis);
	int Set_MotStop(int n_Mode, int n_Axis);

	double Get_MotCurrentPos(int n_Axis);
	//2015.0219 int Check_MotPosRange(int n_Axis, double d_Distance, int n_Range);

	int Get_MotInposition(int n_Axis);
	int Get_MotMotionDone(int n_Axis, int n_Mode = -1);
	int Get_MotMotionZeroReturn(int n_Axis);
	int Get_MotIOSensor(int n_Axis, int n_MotSensor);

	int Set_HomeSetConfig(int n_Axis, int n_mode, int n_ezcount, int n_revvel, int n_escdist);
	
	int Set_MotUnitDist(int n_Axis, double n_UnitDist);

///////////////////////////////////////////////////////////////////////////////////
	int Set_InterruptSetup(int n_Enable, int n_InterruptType);
	int Set_InterruptMask(int n_Axis, long l_InterruptMask[20]);
	int Get_InterruptEventData(int n_MaxAxis, long l_InterruptMask[20]);
	int Get_InterruptFlag(int n_Axis);

	int Set_TriggerSet_OneCompareConfig(int n_Axis, int n_Source, int n_Method, double d_ComparePosition);
///////////////////////////////////////////////////////////////////////////////////

	int Create_LmIndexMapAxis(long n_MapIndexNum, long n_AxisCount, long *np_AxisNum); //최대 4축까지 사용할 수 있도록 하자 
	int Create_IxIndexMapAxis(long n_MapIndexNum, long n_AxisCount, long *np_AxisNum); //최대 4축까지 사용할 수 있도록 하자 

	int Start_SingleMove(int n_Axis, double d_Distance, int d_SpeedRatio = 100);
//	int Start_SingleMove(int n_Axis, double d_Distance, double *dp_SpeedVel); //double *dp_SpeedRatio);
	int Check_SingleMove(int n_Axis, double d_Distance);

	int Start_LinearMove(int n_MapIndex, double *dp_PosList, double *dp_SpdRatio);
	int Check_LinearMove(int n_MapIndex, double *dp_PosList);

	int Start_MultiMove(long l_AxisCnt, long *lp_AxisNum, double *dp_PosList, double *dp_SpdRatio);
	int Check_MultiMove(long l_AxisCnt, long *lp_AxisNum, double *dp_PosList);	

	int Start_CoordinatedMove(long n_LmIndexNum);
	int Check_CoordinatedMove(int n_LmIndexNum);

	int HomeCheck_Mot(int n_Axis, int n_HomeMode, int n_TimeOut);

private:
	//long lMotorPowerWait[3];
	//int nMotorPowerFlag;
};
extern CComizoaPublic COMI;

#endif // !defined(AFX_COMIZOAPUBLIC_H__E0F02F5E_1503_4BEB_BE01_56C396CEE734__INCLUDED_)
