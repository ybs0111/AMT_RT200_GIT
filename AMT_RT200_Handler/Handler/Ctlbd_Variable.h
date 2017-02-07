#ifndef _CTLBD_VARIABLE_h__
#define _CTLBD_VARIABLE_h__

#include "CtlBd_Variable.h"

enum MOTOR_MOVE_METHOD_INFO  //robot site 정보 
{
	ONLY_MOVE_START				=  0,
	ONLY_MOVE_CHECK				    ,
	BOTH_MOVE_FINISH					,
};

//////////////////////////////////////////////////////////
//2015.0102
//////////////////////////////////////////////////////////

#define MAX_MOT_RETRY		3

#define IO_STABLE_WAIT		100 //100ms 안정시간 준다 
#define IO_CONVEYOR_WAIT_LIMIT	10000 //10초 


#define CTL_NO				0		//어떤 조건이 클리어된 상태 
#define CTL_YES				1		//어떤 조건을 요청한 상태 
#define CTL_WORK_END		2		//해당 작업이  모두 완료된 상태  

#define IO_RUN_MODE			0
#define IO_MANUAL_MODE		1 

#define CTL_LOCK			   -1 //요청에 따라 동작을 시작했슴.
#define CTL_CLEAR				0 //아무것도 하지않은 초기상태 
#define CTL_REQ				1 //해당 동작을 요청한 상태 
#define CTL_READY				2 //해당 동작의 요청에 따라 동작을 완료하여 준비된 상태 - 응대한 상태
#define CTL_FREE				3 //요청한 측에서 동작을 모두 완료한 상태로 간섭등을 벗어났을때 상태측이 동작할 수 있도록 자류롭게 데이터를 놓아준다.
#define CTL_CHANGE				4 //목표물을 교체를 요청할때 사용한다(Remove Buffer 교체를 요청할때 사용한다)
#define CTL_SORT                5
#define CTL_JOB_LOTEND          6  //lot end 등 정보셋 상태시 사용 
#define CTL_COMPLETE            7

///////////////////////////////////
// Motor Board Define
//////////////////////////////////
#define MOT_RECOVERY_TIME	10000	//최소한 목표 지점에 있어야 할 최종 시간 
#define MOT_CLASH_TIME		10000	//10초 이상 계속 돌고 있으면 에러 발생 시킨다 
#define MOT_DEBUG			1
//모터 수량 정보 
#define MOT_MAXMOTOR		100 //2015.0303 50
#define MOT_MAX_POS			20

//MOT Speed Setting  
#define	MOT_SPD_VEL			0
#define	MOT_SPD_ACC			1
#define MOT_SPD_DEC			2
#define MOT_SPD_JOG			3
#define MOT_SPD_ORG			4
#define MOT_SPD_FEEDRATE	5
#define MOT_SPD_INTERACC	6

//motor sensor 
#define MOT_SENS_HOME		0
#define MOT_SENS_ELM		1
#define MOT_SENS_ELP		2
#define MOT_SENS_SD			3 //slow down sensor 

//모터 방향 정의 
#define MOT_PLUS			1
#define MOT_MINUS			0

 
//20111027 jtkim #define MOT_RTY_CNT			8	//파스텍 제어기 관련 문제시 재 시도하는 횟수

//20111027 jtkim #define MOT_MOVE_LIMITTIME	900000	//이동 후 3초이내동작 완료할 것 	 
//20111027 jtkim #define MOT_TIMEOUT			900000   //1분의 time out를 가지고 동작한다  //홈 체크시 사용 

//에러등 메세지 지정시사용 
#define MOT_NORMAL_MSG		0
#define MOT_ERR_MSG			1
#define MOT_ERR_CODE		2


#define M_MAX_POS			20
#define MOT_MOTOR_RATIO		1000				// 화면에 보여주는것과 실제 모터 값과의 배율 

#define MOT_LmMAPINDEX			0	//모터 리스트 모션 정의 
#define MOT_IxMAPINDEX			1	//모터 직선 보간 정의 

///////////////////////////////////
// IO Board Define
//////////////////////////////////
//picker 정보 
//#define IO_PICKERINFO		4
//#define IO_PICKERNUM		16
//I/O 수량 정보 
//#define IO_MAXIO			500
//I/O on, Off define  
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//	Console application 프로그램을 위해서 아래 부분을 정의함
typedef unsigned long int				DWORD;
typedef unsigned short int				WORD;
typedef unsigned char					BYTE;
typedef int								BOOL;
typedef void							*HANDLE;

//typedef int							BOOL;			// 0(FALSE), 1(TRUE)
typedef signed char						INT8;			// -128 .. 127
typedef unsigned char					UINT8;			// 0 .. 255
typedef signed short int				INT16;			// -32768 .. 32767
typedef unsigned short int				UINT16;			// 0 .. 65535
typedef int								INT32;			// -2,147,483,648 .. 2,147,483,647
typedef unsigned int					UINT32;			// 0 .. 4,294,967,295
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//motor, i/O 보드등의 리턴값은 이 것만 사용한다  
//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CTLBD_RET_ERROR	   -1
#define CTLBD_RET_PROCEED	0
#define CTLBD_RET_GOOD		1
#define CTLBD_RET_RETRY		2
#define CTLBD_RET_SAFETY	3
#define CTLBD_RET_SKIP		5

#define CTL_ERROR			-1			// 함수 리턴 에러 
#define CTL_PROCEED			0			// 함수가 진행중이다 
#define CTL_GOOD			1			// 조건이 만족되었다 참 
#define CTL_RETRY			2			// 다시 처음부터 조건을 재 시도한다 

//////////////////////////////
//일반 선언 
//////////////////////////////
// ******************************************************************************
// 장비 동작 제어 시 필요한 메시지 정의                                          
// ******************************************************************************

#define DBIT0	0x0001 
#define DBIT1	0x0002 
#define DBIT2	0x0004 
#define DBIT3	0x0008 
#define DBIT4	0x0010 
#define DBIT5	0x0020 
#define DBIT6	0x0040 
#define DBIT7	0x0080 

#define DBIT8	0x0100 
#define DBIT9	0x0200 
#define DBIT10	0x0400 
#define DBIT11	0x0800 
#define DBIT12	0x1000 
#define DBIT13	0x2000 
#define DBIT14	0x4000 
#define DBIT15	0x8000 

#define DBIT16	0x00010000 
#define DBIT17	0x00020000 
#define DBIT18	0x00040000 
#define DBIT19	0x00080000 
#define DBIT20	0x00100000 
#define DBIT21	0x00200000 
#define DBIT22	0x00400000 
#define DBIT23	0x00800000 

#define DBIT24	0x01000000 
#define DBIT25	0x02000000 
#define DBIT26	0x04000000 
#define DBIT27	0x08000000 
#define DBIT28	0x10000000 
#define DBIT29	0x20000000 
#define DBIT30	0x40000000 
#define DBIT31	0x80000000 


////////////////// function ///
//#define NO					0
//#define YES					1

//#define RET_ERROR	   -1
//#define RET_PROCEED		0
//#define RET_GOOD		1
//#define RET_RETRY		2



/*


//모터 및 I/O 리턴값 정의 
enum
{
	MOT_CHECK_FALSE = -9999,	// -9999	// 위치값을 리턴해야 하는데, 비정상일때
	MOT_CHECK_BD_FALSE,			// -9998	// 위치값을 리턴해야 하는데, 비정상일때
	MOT_CHECK_AXIS_FALSE,		// -9997	// 위치값을 리턴해야 하는데, 비정상일때

	//리턴값으로 사용한다 
	MOT_FALSE = 0,				// 0		// 비정상
	MOT_TRUE,					// 1		// 정상
	MOT_PROCEED,				// 2		// 동작중 (아직 정상인지, 비정상인지 판정 못함.)
	MOT_RETRY,					// 3		// 반복동작을 진행한다  
	
	MOT_OPEN_FAIL,				// 4		// 드라이브 열기 에러.
	MOT_CLOSE_FAIL,				// 5		// 드라이브 닫기 에러.
	MOT_NOT_OPEN,				// 6		// 열지 않음 에러.
	MOT_INVALID_BD_NUM,			// 7		// 보드 번호 에러.
	MOT_INVALID_AXIS_NUM,		// 8		// 축 번호 에러.
	MOT_INVALID_PARAMETER_NUM,	// 9		// 파라미터 에러.
	MOT_PARAMETER_RANGE_ERROR,	// 10		// 값의 폭 에러.
	MOT_ZERO_RETURN_ERROR,		// 11
	MOT_TIMEOUT_ERROR,			// 12		// 타임 아웃 에러.
	MOT_UNKNOWN_ERROR,			// 13		// 이유없는 에러.

	MOT_SETTING_ERROR,			// 14		// 설정 에러.
	MOT_INIT_ERROR,				// 15		// 초기화 에러.
	MOT_READ_ERROR,				// 16		// 해당 함수 읽기 에러.

	MOT_MOT_ALARM_ERROR,		// 17		// Motor가 알람에 걸린 상태이다.
	MOT_MOT_POWER_ERROR,		// 18		// Motor가 파워가 ON되지 않는다.
	MOT_MOT_DONE_ERROR,			// 19		// Motor가 Motion Done이 되지 않는다.
	MOT_MOT_RETRY,				// 20		// Motor 동작중 재시도 상태이다.
	MOT_MOT_ERROR,				// 21		// Motor 동작중 재시도 횟수 초과 에러 
	MOT_SAFETY_ERROR,			// 22		// 안전 관련 에러.

	IO_BOARD_OPEN_FAILED = 100, 
	IO_SLAVE_ENABLE_FAILED,
	IO_MASTER_SEARCH_FAILED,
	IO_PORT_SEARCH_FAILED,
	IO_SLAVE_SEARCH_FAILED,
	IO_SPEED_SET_FAILED,
	IO_SPEED_GET_FAILED
};
*/
/*
enum	//MOTOR NUMBER DEFINE
{
	M_DCLDRBT_X	= 0, //0번보드 
	M_DCLDRBT_Y,
	M_DCLDRBT_Z,
	M_DCLDRBT_P_X,

	M_DCLDRBT_P_Y,
	M_DCULDRBT_X,
	M_DCULDRBT_Y,
	M_DCULDRBT_Z = 7,
	
	M_DCULDRBT_P_X,	//1번보드 
	M_DCULDRBT_P_Y,
	M_DCTESTCONTACT,
	M_INSERTBUF_TOP,

	M_INSERTBUF_BOTTOM,
	M_INSERTRBT_X,
	M_INSERTRBT_Y,
	M_INSERTRBT_Z = 15,  

	M_REMOVERBT_X,	//2번보드
	M_REMOVERBT_Y,
	M_REMOVERBT_Z,
	M_REMOVEBUF_1,

	M_REMOVEBUF_2,
	M_REMOVEBUF_3,
	M_MANUALBUF_Y,
	M_SPAREMOT_23   = 23,

	M_ULDSORTRBT_X,	//3번보드
	M_ULDSORTRBT_Y,
	M_ULDSORTRBT_Z,
	M_ULDSORTRBT_P_X,

	M_ULDSORTRBT_P_Y,
	M_ULDBUFRBT_X,	
	M_ULDBUFRBT_Z,	
	M_LDTRANSFER_X = 31, //하위 마스크 번호 

	M_LDTRANSFER_Z,		//4번보드    //이상 상위 마스크 번호 
	M_EMPTYTRANSFER_X,			
	M_EMPTYTRANSFER_Z, 
	M_SPAREMOT_35  = 35,

	M_ULDTRANSFER_X,
	M_ULDTRANSFER_Z,
	M_BIBTABLE_Y,
	M_BIBCLAMP_1,

	M_BIBCLAMP_2,
	M_RACKELE_Z,
};

*/

#define	MOT_SERVO		0
#define MOT_STEP		1

struct tagMOTOR_INFO 
{   	 
	int		n_z_safety_ignore_flag; //2015.0222 james Z축 안전 조건을 무시하는 동작을 진행할때(XY 모터 이동시체크 스킵, buffer pick&place, test site Y up/dn 사이동작시 같은 같은 사이트 조건에서 동작시)

	int		n_sd_mv_chk; //2015.0212 

	//최종 목표 지점(대부분 Z축)을 이동전에 연관된 모터(대부분 X,Y 축) 의 이동 위치를 확인한 후 이상이 없으면 다음 동작한다 
	int     n_position_recovery_retry_cnt; //090824 쨈 발생전 자동 복구 동작을 위해 사용 
	long    l_position_recovery_wait_time[3]; 

	//모터 티칭값을 define하여 위치값을 정의한다 
	long    l_clash_wait_time[3]; //090823 	
	
	double d_rub;

	int n_ws_acc;		// 모터 가속 정보 저장 변수
	int n_ws_dec;		// 모터 감속 정보 저장 변수
	double d_ws_vel;	// 모터 정속 정보 저장 변수

	double  d_deviation[3]; //090803 추가 커멘드와 피드백 위치값 차이 문제로 확인위해 추가 
    double	d_pos[M_MAX_POS];
	double	d_pos_c[M_MAX_POS];
	double	d_move_pulse_to_mm;	
    int		n_retry_cnt;                    // 모터 이동 반복 정보 저장 변수 

	double  d_cmd_feed_allow;	//기본 0.5 mm차이 발생시 처리 
	//int		n_allow;						// 모터 이동시 허용 오차 정보 저장 변수 
	double  d_allow; //2015.0107 // 모터 이동시 허용 오차 정보 저장 변수 
	double	d_mm_for_pulse;					// 1mm 이동 시 출력 펄스 수 저장 변수
	
	double  d_initial_speed;				// 가/감속 시작 속도(초기 속도)

	double  d_spd_max[3];					// jong 20101206 추가 
	double  d_accel[3];
	double  d_decel[3];
    double	d_spd_vel[3];					//[0]:모터 정속, [1]:가속, [2]:감속 
    double	d_spd_home[3];					// 모터 홈체크 [0]:모터 정속, [1]:가속, [2]:감속 
    double	d_spd_jog[3];					// 모터 JOG 속도 정보 저장 변수 [0]:모터 정속, [1]:가속, [2]:감속
	double  d_spd_manual[3];
	double  d_spd_ratio[3]; //정속, 가/감속 비율 10 ~ 100 %

	int	    n_per_home;
	int 	n_per_jog;
	int     n_per_vel;  
	int		n_per_manual;

	double	d_cmdpos_backup; 
	int		n_homecheck_method;				//6 = EL-로 홈 체크한다 
	int		b_homechk_flag; 
 	int		n_home_step;
	int		n_initialize;					//thread 초기화 완료시 사용된다 

	long    l_waittime[3];					//time out등 리미트 사간을 체크한다 

	int		n_home_state;

	//인터럽트 관련 셋팅 변수 
	int		n_interrupt_flag;	//interrupt 사용 유/무 플레그 

	int		n_interrupt_source;  //인터럽트 처리 조건 
	    //n_Mode : 	szCompareSource[4][50] = { 
		//0				"COMMAND Counter",
		//1				"Feedback Counter",
		//2			    "Deviation Counter",
		//3			    "General Counter"

	int		n_interrupt_method; //사용하는 조건 정의  
		//CHAR szCompareMethod[6][50] = {
		//0				"Disable",
		//1				"CMP == COUNTER (No direction)",
		//2				"CMP == COUNTER (Only when counting up)",
		//3				"CMP == COUNTER (Only when counting down)",
		//4				"CMP > COUNTER",
		//5				"CMP < COUNTER"
	
	double	n_interrupt_position; //인터럽트 발생 위치 

	int     n_InterruptMask_Enable_Flag;  //인터럽트는 이 모드가 CTL_YES가 된 상태에서만 플레그를 변경시키자 
	int		n_InterruptMask_Disable_Flag; //인터럽트 기능을 한번 ㅁ사용하지 않았다는 플레그(DC Fail 자재를 사용할때 안전 문제로 추가)

	long	l_Set_InterruptMask[20]; //[50]=최대 모터 수, [19]=0~18 까지의 인터럽트 마스크 정보 셋팅 저장 
	long	l_Get_InterruptMask[20]; //[50]=최대 모터 수, [19]=0~18 까지의 인터럽트 마스크 정보 읽어들인다  
	
	double d_limit_position[2];		// 0:-, 1:+
	double d_offset_pos[2];			// 0:집기, 1:놓기

	//2009.5.20 추가 //각각 위치별 gap를 주기 위해 추가 
	double d_tray_gap_x;
	double d_tray_gap_y;

	double d_buff_gap_x;
	double d_buff_gap_y;

	//[0]:보귀 동작시 해당 위치 백업, [1]:복귀 동작을 수행할 모터 선정(YES, NO), [2]:현재 위치가 변경된 모터, 이 모터는 복귀 동작을 해야 한다
	double d_Reinstatement_Pos[3]; 

	//091216 
	int n_retry_time_flag;
	long l_retry_time_wait[3];

	// 20140306 jtkim
	int		nMotorInfo;		// motor 정보 [0] SERVO [1] step motor
};
extern  tagMOTOR_INFO  st_motor_info[MOT_MAXMOTOR];

/*
struct st_ctlbd_param 
{
	long l_LmIndexmotion_time[16][3]; //이동시간 체크 
	long l_IxIndexmotion_time[8][3];  //이동시간 체크 
	long l_Singlemotion_time[MOT_MAXMOTOR][3]; //단축 모터의 이동위치 체크시간 

	int n_motorbd_init_end; //보드 초기화 완료 
	int n_iobd_init_end;    //i/o board 초기화 완료 

	int n_TotalMotBoard_Number;		//이 장비의 전체 모터보드 수량
	int n_TotalMotorAxis_Number;	//전체 모터축의 수량 
	int n_MotorAxis_Number[10];	//최대 10개까지 보드 사용가능  //각각의 모터보드에 있는 축의 수량 
//	int MotorBoard_Axis_Number[11];//[0]:0보드의 모터축수, [1]:1보드의 모터 축수, .....  [10]:전체 모터보드의 수량  

	//2009.6.9 추가 
	int n_motmove_limit_time; //기본 5초, n_motmove_limit_time = MOT_MOVE_LIMITTIME * ((100 - speedrate) + 1)

} ;
extern  st_ctlbd_param  st_ctlbd;
extern  st_ctlbd_param	st_reco_ctlbd;  */

struct st_linearmotion_param
{	
	int		 n_indexmap_init;	   //현재 index map이 초기화가 끝났는지 확인한다 
	long     l_AxisCnt;            //현재의 IndexNum에서 사용 가능한 모터 수량 최대 4개  
	long		lp_AxisNum[4];        //현재의 IndexNum에서 사용하는 모터 실재 번호를 가진다 
	double	 dp_PosList[4];		   //최대 4 포인트를 한번에 이동가능하다 
	double   dp_SpdRatio[3];        //속도비율 [0]:Vel, [1]:Acc, [2]:Dec
};
extern st_linearmotion_param st_linearmot[8];	 //[8] = LmIndexMap Number 0~ 7까지 가능  

struct st_coordmotion_param
{
	int		 n_indexmap_init;		//현재 index map이 셋팅이 초기화가 끝났는지 확인한다 
	long	 l_TotalMoveCnt;		//총 동작할 구간 정의 보간동작도 한번으로 한다 (최대 10회) 
	long     l_TotalAxisCnt;        //현재의 IndexNum에서 사용 가능한 모터 수량 
	long     lp_TotalAxisNum[4];    //현재의 IndexNum에서 사용하는 모터번호를 가진다 
	
	long    lp_IxIndexNum[8];	    //최대 0 ~ 7번, 보간 동작시 사용할 수 있는 번호 
	long    lp_SubAxisCnt[8];	    //현재 index에서 동작할 모터 수량
	long    lp_SubAxisNum[8][4];    //[10] = index 구간 10회 //[0][0]:첫번째 모터번호, [0][1]:2번째 모터번호  
	double	dp_PosList[8][4];	    //[10] = 10회구간 , //[0]=앞축의 모터 이동 값(단축시 이것만 사용), [1]=뒤축의 모터 이동 값 (보간 동작시 사용)
	double	dp_AxisRatio[8][3];		    //최대 두축의 이동구간 속도 비율 ,[0]:vel, [1]:acc, [2]:dec ratio
};
extern st_coordmotion_param st_coordmot[16];	 //[16] = LmIndexMap Number 0~ 15까지 가능  

#endif