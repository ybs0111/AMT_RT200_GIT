// ComizoaPublic.h: interface for the CComizoaPublic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMIZOAPUBLIC_H__E0F02F5E_1503_4BEB_BE01_56C396CEE734__INCLUDED_)
#define AFX_COMIZOAPUBLIC_H__E0F02F5E_1503_4BEB_BE01_56C396CEE734__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//20150428 james 
#define MOT_STOP_DEC			0   //���� ���� ���� 
#define MOT_STOP_EMG			1   //���� ������� //20150420 
#define MOT_STOP_DEC_BLOCKING	2   //���� ���� �����ҋ����� blocking ����, �ٸ��� ���Ұ� 

/////////////////////////////////////////////////////////
//DLL ������ ���濹�� //2011.1007 �߰� 
/////////////////////////////////////////////////////////
#define BD_MAX_MOTOR		100
#define BD_NO				0		//� ������ Ŭ����� ���� 
#define BD_YES				1		//� ������ ��û�� ���� 

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//motor, i/O ������� ���ϰ��� �� �͸� ����Ѵ�  
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

#define MOT_SPD_CM_CTL		7 //2011.1104 �ӵ��� cm�� �����Ѵ� 
#define MOT_SPD_RPM_CTL		8 //�ӵ��� rpm���� ���� 

//motor sensor 
#define MOT_SENS_HOME		0
#define MOT_SENS_ELM		1
#define MOT_SENS_ELP		2
#define MOT_SENS_SD			3 //slow down sensor 

//���� ���� ���� 
#define MOT_PLUS			1
#define MOT_MINUS			0

 
#define MOT_RTY_CNT			4	//�Ľ��� ����� ���� ������ �� �õ��ϴ� Ƚ��

#define MOT_MOVE_LIMITTIME	60000	//�̵� �� 3���̳����� �Ϸ��� �� 	 
#define MOT_TIMEOUT			90000   //1���� time out�� ������ �����Ѵ�  //Ȩ üũ�� ��� 

//�������� ���� 
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
	int mn_errormsg_debug_mode; //���� �޼��� ��¹�� => 0:������ , 1:���

	int mn_max_move_limit_time;	//���� ���� ����Ʈ�ð� ����
	int mn_max_home_limit_time; //Ȩ üũ ����Ʈ �ð� ���� 

	int mn_run_status;			//��� �������� ������ �޾ƿ´� 	

	int mn_motor_type[BD_MAX_MOTOR]; // jtkim 20130823 step motor commó�� ������ �߰�

	int mn_homechk_flag[BD_MAX_MOTOR]; //Ȩ üũ �ϷῩ�� �÷��� BD_YES:�Ϸ�, BD_NO:Ȩüũ ���� 

	int mn_home_step[BD_MAX_MOTOR]; //home check�� ����ϴ� step 
	
	int mn_homecheck_method[BD_MAX_MOTOR]; //home check ��� ���� 

	// jtkim 20150501
	int   m_nDoneFlag[BD_MAX_MOTOR];
	DWORD m_dwDoneWaitTime[BD_MAX_MOTOR][3];	   // motion done wait time....


	long ml_motion_move_time[BD_MAX_MOTOR][3]; //���۰��� �ð�üũ 
	long ml_retry_time_wait[BD_MAX_MOTOR][3];  //��õ� ���۰��� �ð�üũ 

	int mn_speed_control_flag[BD_MAX_MOTOR]; //speed control flag Ȯ�� YES�� ���, NO�� ������ 
	
	int mn_max_retry_cnt;					//��Ʈ������ �ִ� Ƚ������ 
	int mn_retry_cnt[BD_MAX_MOTOR];			//���ͺ� ��õ� Ƚ�� ���� 

	double md_limit_position[BD_MAX_MOTOR][2];//[0]:+����Ʈ ������, [1]:- ����Ʈ�� ���� 
	double md_cmdpos_backup[BD_MAX_MOTOR];		//��� ���� �̵��� ���� ���� ��ġ���� 
	double md_spd_vel[BD_MAX_MOTOR][3]; 		//[0]:VEL, [1]:accel,[2]:deceel
	double md_spd_home[BD_MAX_MOTOR];			//home üũ �ӵ� ���� 
	double md_spd_jog[BD_MAX_MOTOR];			//home üũ �ӵ� ���� 
	double md_initial_speed[BD_MAX_MOTOR];			//home üũ �ӵ� ���� 

	//2015.0219 james int mn_allow_value[BD_MAX_MOTOR];			//�̵� �Ϸ� ���� ������ ���� ����
	double md_allow_value[BD_MAX_MOTOR];			//�̵� �Ϸ� ���� ������ ���� ����
	double md_deviation[BD_MAX_MOTOR][3];		//Ŀ���� �ǵ�� ��ġ�� ���� ������ Ȯ������ �߰� 

	int mn_sd_io_property_flag[BD_MAX_MOTOR];	//sd io ���� ������� ���� 
	int mn_interrupt_flag[BD_MAX_MOTOR];        //interrupt ��� ���� ���� 
	int ml_Get_InterruptMask[BD_MAX_MOTOR][20]; //inturrupt ���� 
	int mn_runspeed_rate;						//��� �� ���ǵ� ������ ��񿡼� �޴´� 
	int mn_manualspeed_rate;					//��� �޴��� ���ǵ� ������ ��񿡼� �޴´� 
	int mn_motorbd_init_end;				    //motor board �ʱ�ȭ ���� 

	int mn_totalmotboard_number;				//��񿡼� ����ϴ� ���ͺ��� ��ü ���� 
	int mn_totalmotoraxis_number;				//��񿡼� ����ϴ� total motor ��� 
	int mn_bd_axis_number[10];					//���� ���庰 ����� �����Ѵ� 
 
	char mc_alarmcode[10];						//���Ϳ��� �߻��ϴ� error �� ����.

	//2011.0910 
	char mc_normal_msg[1000];//CHAR_MAX_SIZE];
	char mc_abnormal_msg[1000];//CHAR_MAX_SIZE];
	////////////////////////////////////////////////

	//linear motor move setting //max 8�� ���� 
	///////////////////////////////////////////
	int	mn_indexmap_init[8];	   //���� index map�� �ʱ�ȭ�� �������� Ȯ���Ѵ� 
	long ml_axiscnt[8];            //������ IndexNum���� ��� ������ ���� ���� �ִ� 4��  
	long mp_axisnum[8][4];        //������ IndexNum���� ����ϴ� ���� ���� ��ȣ�� ������ 
	double md_poslist[8][4];		   //�ִ� 4 ����Ʈ�� �ѹ��� �̵������ϴ� 
	double md_spdratio[8][3];        //�ӵ����� [0]:Vel, [1]:Acc, [2]:Dec

	//2015.0219 james �߰� 
	long ml_linear_move_time[8][3]; //���۰��� �ð�üũ 
	int  mn_linear_retry_cnt[8];	   //���ͺ� ��õ� Ƚ�� ���� 
	//////////////////////////////

	////////////////////////////////////////////
	// coord move  max 8
	////////////////////////////////////////////
	int		 mn_coord_indexmap_init[8];		//���� index map�� ������ �ʱ�ȭ�� �������� Ȯ���Ѵ� 
	long	 ml_coord_totalmovecnt[8];		//�� ������ ���� ���� �������۵� �ѹ����� �Ѵ� (�ִ� 10ȸ) 
	long     ml_coord_totalaxiscnt[8];        //������ IndexNum���� ��� ������ ���� ���� 
	long     ml_coord_totalaxisnum[8][4];    //������ IndexNum���� ����ϴ� ���͹�ȣ�� ������ 
	
	long    ml_coord_IxIndexnum[8][8];	    //�ִ� 0 ~ 7��, ���� ���۽� ����� �� �ִ� ��ȣ 
	long    ml_coord_subaxiscnt[8][8];	    //���� index���� ������ ���� ����
	long    ml_coord_subaxisnum[8][8][4];    //[10] = index ���� 8ȸ //[0][0]:ù��° ���͹�ȣ, [0][1]:2��° ���͹�ȣ  
	double	md_coord_poslist[8][8][4];	    //[10] = 10ȸ���� , //[0]=������ ���� �̵� ��(����� �̰͸� ���), [1]=������ ���� �̵� �� (���� ���۽� ���)
	double	md_coord_axisratio[8][8][3];		    //�ִ� ������ �̵����� �ӵ� ���� ,[0]:vel, [1]:acc, [2]:dec ratio
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long   ml_LmIndexNumber[16];//[16]=�ִ� 16���� Map�� ������ �ִ�
	long   ml_IxIndexNumber[8]; //[8] =�ִ� 8���� Map�� ������ �ִ�

	// 20140306 jtkim
	int		m_nMotInfo[BD_MAX_MOTOR];
 
// Operations
public:
	
	int	Check_Motion_State(int n_Axis, long l_MotState);  //20150428

	///////////////////////////////////////////

	int Check_MotPosRange_DirectionLimit(int n_MoveDirection, int n_Axis, double d_ReferencePos, double d_TragetPos, double d_Range) ; //james 2012.0830 ���� ���� Ȯ�ο� �߰� 
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

	//2011.1104 �߰� 
	int Set_SpeedRange(int n_Axis, double d_max_pps); //2011.1104 	//������ �ӵ� ��� ������ ���Ѵ�(Min ~ Max)
	int Set_MotUnitSpeed(int n_Axis, double n_UnitSpeed); //2011.1104 �߰� //2011.1104 �߰� �ӵ��� �ʴ� �̵��Ÿ��� ���� ���� 

	int Set_Override_Move_Pos(int n_Axis, double d_TargetPos);
	
	int Get_Deviation_Position(int n_Axis, double d_AllowPos, double * d_Get_DevPos);
	int Set_Motor_IO_Property(long n_Axis, long  n_PropId, long n_OnOff); //090719 �߰� 
	
	int Start_JogMove(long l_AxisNum, long l_Dir); //2011.0119 , double dp_SpeedRatio[3]);
	int Initialize_MotBoard(CString s_file);
	void Close_MotBoard();//���� Ŭ���� 

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

	int Create_LmIndexMapAxis(long n_MapIndexNum, long n_AxisCount, long *np_AxisNum); //�ִ� 4����� ����� �� �ֵ��� ���� 
	int Create_IxIndexMapAxis(long n_MapIndexNum, long n_AxisCount, long *np_AxisNum); //�ִ� 4����� ����� �� �ֵ��� ���� 

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
