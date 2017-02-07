#ifndef _CTLBD_VARIABLE_h__
#define _CTLBD_VARIABLE_h__

#include "CtlBd_Variable.h"

enum MOTOR_MOVE_METHOD_INFO  //robot site ���� 
{
	ONLY_MOVE_START				=  0,
	ONLY_MOVE_CHECK				    ,
	BOTH_MOVE_FINISH					,
};

//////////////////////////////////////////////////////////
//2015.0102
//////////////////////////////////////////////////////////

#define MAX_MOT_RETRY		3

#define IO_STABLE_WAIT		100 //100ms �����ð� �ش� 
#define IO_CONVEYOR_WAIT_LIMIT	10000 //10�� 


#define CTL_NO				0		//� ������ Ŭ����� ���� 
#define CTL_YES				1		//� ������ ��û�� ���� 
#define CTL_WORK_END		2		//�ش� �۾���  ��� �Ϸ�� ����  

#define IO_RUN_MODE			0
#define IO_MANUAL_MODE		1 

#define CTL_LOCK			   -1 //��û�� ���� ������ �����߽�.
#define CTL_CLEAR				0 //�ƹ��͵� �������� �ʱ���� 
#define CTL_REQ				1 //�ش� ������ ��û�� ���� 
#define CTL_READY				2 //�ش� ������ ��û�� ���� ������ �Ϸ��Ͽ� �غ�� ���� - ������ ����
#define CTL_FREE				3 //��û�� ������ ������ ��� �Ϸ��� ���·� �������� ������� �������� ������ �� �ֵ��� �ڷ��Ӱ� �����͸� �����ش�.
#define CTL_CHANGE				4 //��ǥ���� ��ü�� ��û�Ҷ� ����Ѵ�(Remove Buffer ��ü�� ��û�Ҷ� ����Ѵ�)
#define CTL_SORT                5
#define CTL_JOB_LOTEND          6  //lot end �� ������ ���½� ��� 
#define CTL_COMPLETE            7

///////////////////////////////////
// Motor Board Define
//////////////////////////////////
#define MOT_RECOVERY_TIME	10000	//�ּ��� ��ǥ ������ �־�� �� ���� �ð� 
#define MOT_CLASH_TIME		10000	//10�� �̻� ��� ���� ������ ���� �߻� ��Ų�� 
#define MOT_DEBUG			1
//���� ���� ���� 
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

//���� ���� ���� 
#define MOT_PLUS			1
#define MOT_MINUS			0

 
//20111027 jtkim #define MOT_RTY_CNT			8	//�Ľ��� ����� ���� ������ �� �õ��ϴ� Ƚ��

//20111027 jtkim #define MOT_MOVE_LIMITTIME	900000	//�̵� �� 3���̳����� �Ϸ��� �� 	 
//20111027 jtkim #define MOT_TIMEOUT			900000   //1���� time out�� ������ �����Ѵ�  //Ȩ üũ�� ��� 

//������ �޼��� �����û�� 
#define MOT_NORMAL_MSG		0
#define MOT_ERR_MSG			1
#define MOT_ERR_CODE		2


#define M_MAX_POS			20
#define MOT_MOTOR_RATIO		1000				// ȭ�鿡 �����ִ°Ͱ� ���� ���� ������ ���� 

#define MOT_LmMAPINDEX			0	//���� ����Ʈ ��� ���� 
#define MOT_IxMAPINDEX			1	//���� ���� ���� ���� 

///////////////////////////////////
// IO Board Define
//////////////////////////////////
//picker ���� 
//#define IO_PICKERINFO		4
//#define IO_PICKERNUM		16
//I/O ���� ���� 
//#define IO_MAXIO			500
//I/O on, Off define  
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//	Console application ���α׷��� ���ؼ� �Ʒ� �κ��� ������
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
//motor, i/O ������� ���ϰ��� �� �͸� ����Ѵ�  
//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CTLBD_RET_ERROR	   -1
#define CTLBD_RET_PROCEED	0
#define CTLBD_RET_GOOD		1
#define CTLBD_RET_RETRY		2
#define CTLBD_RET_SAFETY	3
#define CTLBD_RET_SKIP		5

#define CTL_ERROR			-1			// �Լ� ���� ���� 
#define CTL_PROCEED			0			// �Լ��� �������̴� 
#define CTL_GOOD			1			// ������ �����Ǿ��� �� 
#define CTL_RETRY			2			// �ٽ� ó������ ������ �� �õ��Ѵ� 

//////////////////////////////
//�Ϲ� ���� 
//////////////////////////////
// ******************************************************************************
// ��� ���� ���� �� �ʿ��� �޽��� ����                                          
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


//���� �� I/O ���ϰ� ���� 
enum
{
	MOT_CHECK_FALSE = -9999,	// -9999	// ��ġ���� �����ؾ� �ϴµ�, �������϶�
	MOT_CHECK_BD_FALSE,			// -9998	// ��ġ���� �����ؾ� �ϴµ�, �������϶�
	MOT_CHECK_AXIS_FALSE,		// -9997	// ��ġ���� �����ؾ� �ϴµ�, �������϶�

	//���ϰ����� ����Ѵ� 
	MOT_FALSE = 0,				// 0		// ������
	MOT_TRUE,					// 1		// ����
	MOT_PROCEED,				// 2		// ������ (���� ��������, ���������� ���� ����.)
	MOT_RETRY,					// 3		// �ݺ������� �����Ѵ�  
	
	MOT_OPEN_FAIL,				// 4		// ����̺� ���� ����.
	MOT_CLOSE_FAIL,				// 5		// ����̺� �ݱ� ����.
	MOT_NOT_OPEN,				// 6		// ���� ���� ����.
	MOT_INVALID_BD_NUM,			// 7		// ���� ��ȣ ����.
	MOT_INVALID_AXIS_NUM,		// 8		// �� ��ȣ ����.
	MOT_INVALID_PARAMETER_NUM,	// 9		// �Ķ���� ����.
	MOT_PARAMETER_RANGE_ERROR,	// 10		// ���� �� ����.
	MOT_ZERO_RETURN_ERROR,		// 11
	MOT_TIMEOUT_ERROR,			// 12		// Ÿ�� �ƿ� ����.
	MOT_UNKNOWN_ERROR,			// 13		// �������� ����.

	MOT_SETTING_ERROR,			// 14		// ���� ����.
	MOT_INIT_ERROR,				// 15		// �ʱ�ȭ ����.
	MOT_READ_ERROR,				// 16		// �ش� �Լ� �б� ����.

	MOT_MOT_ALARM_ERROR,		// 17		// Motor�� �˶��� �ɸ� �����̴�.
	MOT_MOT_POWER_ERROR,		// 18		// Motor�� �Ŀ��� ON���� �ʴ´�.
	MOT_MOT_DONE_ERROR,			// 19		// Motor�� Motion Done�� ���� �ʴ´�.
	MOT_MOT_RETRY,				// 20		// Motor ������ ��õ� �����̴�.
	MOT_MOT_ERROR,				// 21		// Motor ������ ��õ� Ƚ�� �ʰ� ���� 
	MOT_SAFETY_ERROR,			// 22		// ���� ���� ����.

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
	M_DCLDRBT_X	= 0, //0������ 
	M_DCLDRBT_Y,
	M_DCLDRBT_Z,
	M_DCLDRBT_P_X,

	M_DCLDRBT_P_Y,
	M_DCULDRBT_X,
	M_DCULDRBT_Y,
	M_DCULDRBT_Z = 7,
	
	M_DCULDRBT_P_X,	//1������ 
	M_DCULDRBT_P_Y,
	M_DCTESTCONTACT,
	M_INSERTBUF_TOP,

	M_INSERTBUF_BOTTOM,
	M_INSERTRBT_X,
	M_INSERTRBT_Y,
	M_INSERTRBT_Z = 15,  

	M_REMOVERBT_X,	//2������
	M_REMOVERBT_Y,
	M_REMOVERBT_Z,
	M_REMOVEBUF_1,

	M_REMOVEBUF_2,
	M_REMOVEBUF_3,
	M_MANUALBUF_Y,
	M_SPAREMOT_23   = 23,

	M_ULDSORTRBT_X,	//3������
	M_ULDSORTRBT_Y,
	M_ULDSORTRBT_Z,
	M_ULDSORTRBT_P_X,

	M_ULDSORTRBT_P_Y,
	M_ULDBUFRBT_X,	
	M_ULDBUFRBT_Z,	
	M_LDTRANSFER_X = 31, //���� ����ũ ��ȣ 

	M_LDTRANSFER_Z,		//4������    //�̻� ���� ����ũ ��ȣ 
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
	int		n_z_safety_ignore_flag; //2015.0222 james Z�� ���� ������ �����ϴ� ������ �����Ҷ�(XY ���� �̵���üũ ��ŵ, buffer pick&place, test site Y up/dn ���̵��۽� ���� ���� ����Ʈ ���ǿ��� ���۽�)

	int		n_sd_mv_chk; //2015.0212 

	//���� ��ǥ ����(��κ� Z��)�� �̵����� ������ ����(��κ� X,Y ��) �� �̵� ��ġ�� Ȯ���� �� �̻��� ������ ���� �����Ѵ� 
	int     n_position_recovery_retry_cnt; //090824 ´ �߻��� �ڵ� ���� ������ ���� ��� 
	long    l_position_recovery_wait_time[3]; 

	//���� ƼĪ���� define�Ͽ� ��ġ���� �����Ѵ� 
	long    l_clash_wait_time[3]; //090823 	
	
	double d_rub;

	int n_ws_acc;		// ���� ���� ���� ���� ����
	int n_ws_dec;		// ���� ���� ���� ���� ����
	double d_ws_vel;	// ���� ���� ���� ���� ����

	double  d_deviation[3]; //090803 �߰� Ŀ���� �ǵ�� ��ġ�� ���� ������ Ȯ������ �߰� 
    double	d_pos[M_MAX_POS];
	double	d_pos_c[M_MAX_POS];
	double	d_move_pulse_to_mm;	
    int		n_retry_cnt;                    // ���� �̵� �ݺ� ���� ���� ���� 

	double  d_cmd_feed_allow;	//�⺻ 0.5 mm���� �߻��� ó�� 
	//int		n_allow;						// ���� �̵��� ��� ���� ���� ���� ���� 
	double  d_allow; //2015.0107 // ���� �̵��� ��� ���� ���� ���� ���� 
	double	d_mm_for_pulse;					// 1mm �̵� �� ��� �޽� �� ���� ����
	
	double  d_initial_speed;				// ��/���� ���� �ӵ�(�ʱ� �ӵ�)

	double  d_spd_max[3];					// jong 20101206 �߰� 
	double  d_accel[3];
	double  d_decel[3];
    double	d_spd_vel[3];					//[0]:���� ����, [1]:����, [2]:���� 
    double	d_spd_home[3];					// ���� Ȩüũ [0]:���� ����, [1]:����, [2]:���� 
    double	d_spd_jog[3];					// ���� JOG �ӵ� ���� ���� ���� [0]:���� ����, [1]:����, [2]:����
	double  d_spd_manual[3];
	double  d_spd_ratio[3]; //����, ��/���� ���� 10 ~ 100 %

	int	    n_per_home;
	int 	n_per_jog;
	int     n_per_vel;  
	int		n_per_manual;

	double	d_cmdpos_backup; 
	int		n_homecheck_method;				//6 = EL-�� Ȩ üũ�Ѵ� 
	int		b_homechk_flag; 
 	int		n_home_step;
	int		n_initialize;					//thread �ʱ�ȭ �Ϸ�� ���ȴ� 

	long    l_waittime[3];					//time out�� ����Ʈ �簣�� üũ�Ѵ� 

	int		n_home_state;

	//���ͷ�Ʈ ���� ���� ���� 
	int		n_interrupt_flag;	//interrupt ��� ��/�� �÷��� 

	int		n_interrupt_source;  //���ͷ�Ʈ ó�� ���� 
	    //n_Mode : 	szCompareSource[4][50] = { 
		//0				"COMMAND Counter",
		//1				"Feedback Counter",
		//2			    "Deviation Counter",
		//3			    "General Counter"

	int		n_interrupt_method; //����ϴ� ���� ����  
		//CHAR szCompareMethod[6][50] = {
		//0				"Disable",
		//1				"CMP == COUNTER (No direction)",
		//2				"CMP == COUNTER (Only when counting up)",
		//3				"CMP == COUNTER (Only when counting down)",
		//4				"CMP > COUNTER",
		//5				"CMP < COUNTER"
	
	double	n_interrupt_position; //���ͷ�Ʈ �߻� ��ġ 

	int     n_InterruptMask_Enable_Flag;  //���ͷ�Ʈ�� �� ��尡 CTL_YES�� �� ���¿����� �÷��׸� �����Ű�� 
	int		n_InterruptMask_Disable_Flag; //���ͷ�Ʈ ����� �ѹ� ��������� �ʾҴٴ� �÷���(DC Fail ���縦 ����Ҷ� ���� ������ �߰�)

	long	l_Set_InterruptMask[20]; //[50]=�ִ� ���� ��, [19]=0~18 ������ ���ͷ�Ʈ ����ũ ���� ���� ���� 
	long	l_Get_InterruptMask[20]; //[50]=�ִ� ���� ��, [19]=0~18 ������ ���ͷ�Ʈ ����ũ ���� �о���δ�  
	
	double d_limit_position[2];		// 0:-, 1:+
	double d_offset_pos[2];			// 0:����, 1:����

	//2009.5.20 �߰� //���� ��ġ�� gap�� �ֱ� ���� �߰� 
	double d_tray_gap_x;
	double d_tray_gap_y;

	double d_buff_gap_x;
	double d_buff_gap_y;

	//[0]:���� ���۽� �ش� ��ġ ���, [1]:���� ������ ������ ���� ����(YES, NO), [2]:���� ��ġ�� ����� ����, �� ���ʹ� ���� ������ �ؾ� �Ѵ�
	double d_Reinstatement_Pos[3]; 

	//091216 
	int n_retry_time_flag;
	long l_retry_time_wait[3];

	// 20140306 jtkim
	int		nMotorInfo;		// motor ���� [0] SERVO [1] step motor
};
extern  tagMOTOR_INFO  st_motor_info[MOT_MAXMOTOR];

/*
struct st_ctlbd_param 
{
	long l_LmIndexmotion_time[16][3]; //�̵��ð� üũ 
	long l_IxIndexmotion_time[8][3];  //�̵��ð� üũ 
	long l_Singlemotion_time[MOT_MAXMOTOR][3]; //���� ������ �̵���ġ üũ�ð� 

	int n_motorbd_init_end; //���� �ʱ�ȭ �Ϸ� 
	int n_iobd_init_end;    //i/o board �ʱ�ȭ �Ϸ� 

	int n_TotalMotBoard_Number;		//�� ����� ��ü ���ͺ��� ����
	int n_TotalMotorAxis_Number;	//��ü �������� ���� 
	int n_MotorAxis_Number[10];	//�ִ� 10������ ���� ��밡��  //������ ���ͺ��忡 �ִ� ���� ���� 
//	int MotorBoard_Axis_Number[11];//[0]:0������ �������, [1]:1������ ���� ���, .....  [10]:��ü ���ͺ����� ����  

	//2009.6.9 �߰� 
	int n_motmove_limit_time; //�⺻ 5��, n_motmove_limit_time = MOT_MOVE_LIMITTIME * ((100 - speedrate) + 1)

} ;
extern  st_ctlbd_param  st_ctlbd;
extern  st_ctlbd_param	st_reco_ctlbd;  */

struct st_linearmotion_param
{	
	int		 n_indexmap_init;	   //���� index map�� �ʱ�ȭ�� �������� Ȯ���Ѵ� 
	long     l_AxisCnt;            //������ IndexNum���� ��� ������ ���� ���� �ִ� 4��  
	long		lp_AxisNum[4];        //������ IndexNum���� ����ϴ� ���� ���� ��ȣ�� ������ 
	double	 dp_PosList[4];		   //�ִ� 4 ����Ʈ�� �ѹ��� �̵������ϴ� 
	double   dp_SpdRatio[3];        //�ӵ����� [0]:Vel, [1]:Acc, [2]:Dec
};
extern st_linearmotion_param st_linearmot[8];	 //[8] = LmIndexMap Number 0~ 7���� ����  

struct st_coordmotion_param
{
	int		 n_indexmap_init;		//���� index map�� ������ �ʱ�ȭ�� �������� Ȯ���Ѵ� 
	long	 l_TotalMoveCnt;		//�� ������ ���� ���� �������۵� �ѹ����� �Ѵ� (�ִ� 10ȸ) 
	long     l_TotalAxisCnt;        //������ IndexNum���� ��� ������ ���� ���� 
	long     lp_TotalAxisNum[4];    //������ IndexNum���� ����ϴ� ���͹�ȣ�� ������ 
	
	long    lp_IxIndexNum[8];	    //�ִ� 0 ~ 7��, ���� ���۽� ����� �� �ִ� ��ȣ 
	long    lp_SubAxisCnt[8];	    //���� index���� ������ ���� ����
	long    lp_SubAxisNum[8][4];    //[10] = index ���� 10ȸ //[0][0]:ù��° ���͹�ȣ, [0][1]:2��° ���͹�ȣ  
	double	dp_PosList[8][4];	    //[10] = 10ȸ���� , //[0]=������ ���� �̵� ��(����� �̰͸� ���), [1]=������ ���� �̵� �� (���� ���۽� ���)
	double	dp_AxisRatio[8][3];		    //�ִ� ������ �̵����� �ӵ� ���� ,[0]:vel, [1]:acc, [2]:dec ratio
};
extern st_coordmotion_param st_coordmot[16];	 //[16] = LmIndexMap Number 0~ 15���� ����  

#endif