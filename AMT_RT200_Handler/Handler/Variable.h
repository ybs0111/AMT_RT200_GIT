#include <afxtempl.h>

#ifndef _VARIABLE_h__
#define _VARIABLE_h__

#include "Alg_Mysql.h"

// amt8562
//	Console application 프로그램을 위해서 아래 부분을 정의함
typedef unsigned long int				DWORD;
typedef unsigned short int				WORD;
typedef unsigned char					BYTE;
typedef int								BOOL;
typedef void							*HANDLE;

#ifndef PASCAL
#define PASCAL						__stdcall
#endif	// PASCAL

// jtkim 20130520
/*
#ifndef EXPORT
//#define EXPORT
#endif	// EXPORT
*/
//typedef int							BOOL;			// 0(FALSE), 1(TRUE)
typedef signed char						INT8;			// -128 .. 127
typedef unsigned char					UINT8;			// 0 .. 255
typedef signed short int				INT16;			// -32768 .. 32767
typedef unsigned short int				UINT16;			// 0 .. 65535
typedef int								INT32;			// -2,147,483,648 .. 2,147,483,647
typedef unsigned int					UINT32;			// 0 .. 4,294,967,295

// *****************************************************************************
// 사용자 정의 메시지 ID 선언                                                   
// *****************************************************************************
#define WM_FORM_CHANGE			WM_USER+200   // 화면이 전환된 때 해당 전환 상태 정보를 전송하기 위한 사용자 정의 메시지 

#define WM_STATUS_CHANGE		WM_USER+201   // 타이틀 바 화면으로 전송된 메시지 출력시키기 위한 사용자 정의 메시지 추가 

#define WM_LIST_DATA			WM_USER + 6   // 통신 메시지 출력 리스트 대화 상자에 
                                              // 시리얼 포트를 통해 송/수신된 메시지를 출력하기 위한 사용자 정의 메시지 
#define WM_DATA_SEND			WM_USER + 7   // Data를 전송하기 위한 화면에서 정보를 전송하기 위해 정의한 사용자 정의 함수

#define WM_WORK_END				WM_USER + 8   // 테스트 작업 완료 시 정보를 메인 화면에 출력시키기 위한 사용자 정의 메시지 추가 

#define WM_BASIC_APPLY			WM_USER + 10  // BASIC DATA를 APPLY 시키기 위한 사용자 정의 메시지 추가 

#define WM_MAINTENANCE_APPLY	WM_USER + 11  // Maintenance DATA를 APPLY 시키기 위한 사용자 정의 메시지 추가
#define WM_INTERFACE_APPLY		WM_USER + 13
#define	WM_WAITTIME_APPLY		WM_USER + 15  // Wait Time DATA를 APPLY 시키기 위한 사용자 정의 메시지 추가 

#define	WM_MODULESITE_APPLY		WM_USER + 16
#define	WM_HEATSINKSITE_APPLY	WM_USER + 17
#define	WM_MODULEROBOT_APPLY	WM_USER + 18
#define	WM_HEATSINKROBOT_APPLY	WM_USER + 19
#define	WM_DISPENSER_APPLY		WM_USER + 20
#define	WM_MOTORSPEED_APPLY		WM_USER + 21
#define WM_SERIAL_PORT			WM_USER + 25
#define	WM_TESTREFERENCE_MANUAL		WM_USER + 21

#define	WM_LOTOPEN_APPLY		WM_USER + 26
#define	WM_LOTSTART_APPLY		WM_USER + 27

#define WM_DATA_INIT_SAVE       WM_USER + 300 // apply버튼시 저장까지, 해당 화면의 데이터 적용(Init)
#define WM_DATA_CHANGE          WM_USER + 301 // apply버튼시 저장까지, 해당 화면의 데이터 적용(Init)

#define WM_WORK_DISPLAY			WM_USER + 400
#define WM_WORK_COMMAND			WM_USER + 401
#define WM_MAIN_IO_DISPLAY	    WM_USER + 402

#define WM_PCB_CV_IN_MOVE_DRAW_MAIN    WM_USER + 403
#define WM_PCB_CV_TURN_MOVE_DRAW_MAIN  WM_USER + 404
#define WM_PCB_CV_OUT_MOVE_DRAW_MAIN   WM_USER + 405

#define WM_CLIENT_MSG			WM_USER + 500
#define WM_CLIENT_MSG_1			WM_USER + 500
#define WM_CLIENT_MSG_2			WM_USER + 501
#define WM_CLIENT_MSG_3			WM_USER + 502
#define WM_CLIENT_MSG_4			WM_USER + 503
#define WM_CLIENT_MSG_5			WM_USER + 504
#define WM_CLIENT_MSG_6			WM_USER + 505
#define WM_CLIENT_MSG_7			WM_USER + 506
#define WM_CLIENT_MSG_8			WM_USER + 507
#define WM_CLIENT_MSG_9			WM_USER + 508
#define WM_CLIENT_MSG_10		WM_USER + 509

#define WM_ZEBRA_CLIENT_MSG	WM_USER + 509


#define WM_SERVER_MSG			WM_USER + 550
#define WM_SERVER_MSG_1			WM_USER + 550
#define WM_SERVER_MSG_2			WM_USER + 551
#define WM_SERVER_MSG_3			WM_USER + 552
#define WM_SERVER_MSG_4			WM_USER + 553
#define WM_SERVER_MSG_5			WM_USER + 554
#define WM_SERVER_MSG_6			WM_USER + 555
#define WM_SERVER_MSG_7			WM_USER + 556
#define WM_SERVER_MSG_8			WM_USER + 557
#define WM_SERVER_MSG_9			WM_USER + 558
#define WM_SERVER_MSG_10		WM_USER + 559

#define WM_GMS_MSG				WM_USER + 600

#define WM_RIGHT_POS			WM_USER + 601
#define WM_LEFT_POS				WM_USER + 602
#define WM_CONV_STATE			WM_USER + 603
#define WM_BARCODE_MSG 		    WM_USER + 604 

#define CLIENT_CONNECT			0
#define CLIENT_CLOSE			1
#define CLIENT_SEND				2
#define CLIENT_REV				3
#define CLIENT_ACCEPT			4

#define SERVER_CONNECT			0
#define SERVER_CLOSE			1
#define SERVER_SEND				2
#define SERVER_REV				3
#define SERVER_ACCEPT			4

#define INITIAL_INDEX_DUMP		0	
#define INITIAL_TEST_DUMP		1
#define INITIAL_INDEX_TABLE		2

//kwlee 2017.0202
#define BARCODE_TRIGGER_1       1
#define BARCODE_TRIGGER_2       2
// *****************************************************************************

// *****************************************************************************
//  네트웍 메세지 정의 장비쪽에서 BPC로 보내는 메세지.
// *****************************************************************************
#define NW_LOT_START_RPY				1
#define NW_LOT_CANCEL_RPY				2
#define NW_PCBOX_START_RPY				3
#define NW_PCBOX_END_RPY				4
#define NW_PCBOX_CHANGE_RPY				5
#define NW_ENABLE_RPY					6
#define NW_DISABLE_RPY					7
#define NW_SLOT_MASK_RPY				8
#define NW_LOT_START_ENABLE_RPY			9
#define NW_PCBOX_REMOVE_RPY				10
#define NW_PCBOX_ERROR_RPY				11

#define NW_PCBOX_OFF					12
#define NW_UNLOAD_COMPT					13
#define NW_CANCEL_END					14
#define NW_LOT_END						15
#define NW_PCBOX_DISABLE				16
#define NW_VIDEO_TIMEOUT				17
#define NW_HOLD_TIMEOUT					18
#define NW_PCBOX_LOT_INFO				19
#define NW_FAIL_INFO_SEND				20
#define NW_FAIL_REGNUMBER_REQUEST		21
#define NW_FAIL_INFO_SERACH				22
#define NW_FAIL_INFO_SAVE				23
#define NW_FAIL_INFO_DELETE				24
// 새로 추가 됨.
#define NW_LOT_DISPLAY					25
#define NW_EQP_LOT_START				26
#define NW_EQP_LOT_CANCEL				27
#define NW_EQP_LOT_END					28

// *****************************************************************************
//  네트웍 메세지 정의 BPC쪽에서 장비로 보내는 메세지.
// *****************************************************************************
#define NW_LOT_START					29
#define NW_LOT_CANCEL					30
#define NW_PCBOX_START					31
#define NW_PCBOX_END					32
#define NW_PCBOX_CHANGE					33
#define NW_ENABLE						34
#define NW_DISABLE						35
#define NW_SLOT_MASK					36
#define NW_LOT_START_ENABLE				37
#define NW_PCBOX_REMOVE					38
#define NW_PCBOX_ERROR					39

#define NW_PCBOX_LOT_INFO_RPY			40
#define NW_FAIL_INFO_SEND_RPY			41
#define NW_FAIL_REGNUMBER_REQUEST_RPY	42
#define NW_FAIL_INFO_SERACH_RPY			43
#define NW_FAIL_INFO_SAVE_RPY			44
#define NW_FAIL_INFO_DELETE_RPY			45
// 새로 추가 됨
#define NW_LOT_DISPLAY_RPY				46
#define NW_EQP_LOT_START_RPY			47
#define NW_EQP_LOT_CANCEL_RPY			48
#define NW_EQP_LOT_END_RPY				49
// *****************************************************************************

#define NW_PCBOX_ENABLE					50
#define NW_PGM_START					60
//#define NW_FAIL_REGNUMBER_REQUEST		61
// *****************************************************************************
//  List Box 출력용 메세지 정의                                                 
// *****************************************************************************
#define RECEIVE_MSG			1
#define SEND_MSG			2
#define NORMAL_MSG			3
#define ABNORMAL_MSG		4
#define MACHINE_INFO		5
#define DEVICE_INFO			6

// *****************************************************************************
// *****************************************************************************
//  SCREEN_MAIN 출력용 메세지 정의                                                
// *****************************************************************************
#define MAIN_TIMEINFO				100
#define MAIN_COUNT_DISPLAY			101
#define MAIN_LOT_DISPLAY			116 //(conveyor 상태표시)
#define MAIN_BARCODE_DISPLAY		117 //(conveyor 상태표시) // jtkim 20150316

#define MAIN_ROBOT_LEFT_TURN_DISPLAY   118
#define MAIN_ROBOT_RIGHT_TURN_DISPLAY  119
#define MAIN_ROBOT_CENTER_DISPLAY      120
#define MAIN_PCB_WORK				   121
#define MAIN_PCB_IN_WORK               122
#define MAIN_PCB_TURN_WORK             123
#define MAIN_PCB_OUT_WORK              124

#define MAIN_PCB_INFO				200
#define MAIN_PCB_INFO_BACK			201
#define MAIN_PCB_INFO_NEXT			202

#define MAIN_PCB_PICKER_CREATE_MSG	300
#define MAIN_PCB_PICKER_DELETE_MSG	301
#define MAIN_PCB_BUFFER_CREATE_MSG	302
#define MAIN_PCB_BUFFER_DELETE_MSG	303
#define MAIN_PCB_TRAY_CREATE_MSG	304
#define MAIN_PCB_TRAY_DELETE_MSG	305
#define MAIN_PCB_INTER_CREATE_MSG	306
#define MAIN_PCB_INTER_DELETE_MSG	307

#define MAIN_BD_ERR_CREATE_MSG		400
#define MAIN_BD_ERR_DELETE_MSG		401

#define MAIN_MESSAGE_BOX_CREATE_REQ	100
#define MAIN_MESSAGE_BOX_DELETE_REQ	101

#define MAIN_BARCODE_BOX_CREATE_REQ	110
#define MAIN_BARCODE_BOX_DELETE_REQ	111

#define	MAIN_TRACK_OUT_CREATE_REQ	500
#define	MAIN_TRACK_OUT_DELETE_REQ	501

#define MAIN_LOT_HISTORY_CREATE_REQ 600
#define MAIN_LOT_HISTORY_DELETE_REQ 601

#define MAIN_PCB_DATA_CREATE_REQ	700
#define MAIN_PCB_DATA_DELETE_REQ	701

#define MAIN_NOTICE_CREATE_REQ		800
#define MAIN_NOTICE_DELETE_REQ		801

#define MOTOR_IO_CREATE_REQ			900
#define MOTOR_IO_DELETE_REQ			901

#define MAIN_MESSAGE_VIEW_CREATE_REQ 1000
#define MAIN_MESSAGE_VIEW_DELETE_REQ 1001

#define ALARM_IO_CREATE_REQ 100
#define ALARM_IO_DELETE_REQ 101
#define ALARM_MOTOR_CREATE_REQ 200
#define ALARM_MOTOR_DELETE_REQ 201

#define INIT_RECOVERY_COMMAND		0
#define INIT_INITIALIZE_COMMAND		1
#define INIT_COK_EXCHANGE_COMMAND	2 //2015.0407 james COK 교체 작업전에 작업자에게 확인을 받는다 
#define INIT_AUTO_MODE_CHECK        3
#define INIT_COMPLETE_COMMAND       4
#define INIT_SKIP_COMMAND		    5
// *****************************************************************************
//  Title Bar 출력용 메세지 정의                                                
// *****************************************************************************
#define FORM_NAME			1
#define MACHINE_STATUS		2
#define DEVICE_MODE			3
#define INTERFACE_MODE		4
#define LEVEL_MODE			5
#define FILE_MODE			6
#define MODEL_MODE			7
#define COMMUNICATION_MODE	8
#define XGEM_MODE			9
// *****************************************************************************

// *****************************************************************************
// 장비 동작 제어 시 필요한 메시지 정의                                         
// *****************************************************************************
#define TIMECOUNT			5
#define ELEVATOR_STRATUMS	6
// *****************************************************************************
// 라벨 정의                                         
// *****************************************************************************
#define  NONE              0
#define  FAIL              1
#define  GOOD              2

#define  BARCODEREAD       3

#define  BIN               0
#define  EXIST             1
#define  X_POS             2
#define  Y_POS             3
#define  FAILPICK          4

#define  LEFT              0
#define  RIGHT             1




// *************************************************************************
// 장비 동작 제어 시 필요한 메시지 정의                                          
// *************************************************************************
#define dSTOP				0
#define dRUN				1
#define dIDLE				2
#define dJAM				3
#define dLOTEND				4
#define dINIT				5
#define dWARNING			6
#define dLOCK				7
#define dSELFCHECK			8
#define dMAINT				9
#define dREINSTATE			10	//복귀동작중
#define dRECOVERY			11
#define dMOTOR_WARNING		12 //이때는 알람은 발생시키지 않는다(run status를 바꾸지 않는다).thread 에서만 판단하여 알람 발생시킨다 

#define eWARNING			0
#define eRUNNING			1
#define eSTOP				2
#define eERROR				3

#define IO_CLEAR			-1
#define NO					0
#define YES					1
#define COVER_YES			2
#define READY				2
#define PROCEED				3


#define IO_ON				1
#define IO_OFF				0
#define IO_ERROR			2
#define IO_CHK_ERROR		4

#define IO_NONE				0
#define IO_IN				1
#define IO_OUT				2

#define IO_CLAMP			1
#define IO_UNCLAMP			0

#define IO_HOLD				0
#define IO_RELEASE			1

#define IO_DOWN				1
#define IO_UP				0

#define IO_PROCEED			2
#define IO_TIMEOUT			3

#define IO_LEFT				0
#define IO_RIGHT			1

#define IO_CLOSE			0
#define IO_OPEN				1

#define IO_GOOD				10
#define IO_FAIL				11

#define FUN_ING				0
#define FUN_END				1
#define FUN_ERR				2
#define FUN_CTU				3

#define PICKER				0
#define FINGER				1

#define FIRST               0
#define SECOND              1

#define RETRYCNT               3

//2015.0324 james 
#define SOCKET_CONTACT_ON			100
#define SOCKET_CONTACT_OFF			200
////////////////////////////////////////////
//CONVEYOR INTERFACE
#define CONV_CLR              0
#define CONV_REQ              1
#define CONV_READY              2

///////////////////////////////////////////////
#define CONV_IN                0
#define CONV_MID                1
#define CONV_OUT                2

#define CONVEYOR			         0
#define ROBOT			             1
#define CONVEYOR_SEC                 2
#define ROBOT_SEC                    3

// #define WAIT_CONV_IN                0
// #define WAIT_CONV_OUT                1
// #define WAIT_CONV_REQ                2
///////////////////////////////////////////
#define COM_CLEAR            -1
#define COM_NONE             0
#define COM_START            1
#define COM_READY            2
#define COM_READY_CHK        3
#define COM_COMPLETE         4
#define COM_COMPLETE_CHK      5
#define COM_GOOD	          6
#define COM_PROCEED	          7

#define TIMECOUNT			5
#define ELEVATOR_STRATUMS	6
// *************************************************************************

// *************************************************************************
// RS-232C 시리얼 통신 시 사용되는 메시지 정의                                   
// -> MAX_PORT : 기본 포트 2개로 구성되어 있다						             
// *************************************************************************
#define MAX_PORT				2	// 생성할 시리얼 포트 갯수
#define COM_ERROR				-1	// 에러 메시지
#define CRI_BARCODE_PORT		0		// 현재 BCR READ 포트 번호
#define MPCB_BARCODE_PORT		1		// 현재 BCR READ 포트 번호
#define LIGHT_PORT				2
#define A_LIGHT_PORT			3

#define COM_PORT			0
#define COM_BAUDRATE		1
#define COM_DATA			2
#define COM_STOP			3
#define COM_PARITY			4

//#define TESTER_PORT	2		// 현재 Tester Communication 포트 번호
//#define COM_PORT   2	// 현재 사용 중인 시리얼 포트 번호
 
#define BUF_SIZE  1000	// 버퍼 크기
// *************************************************************************

// *************************************************************************
// *************************************************************************
// I/O 모듈 갯수 및 포트 최대 갯수 선언한다                                      
// *************************************************************************
#define MAX_IO				1315

#define MAX_MASTER_IO		1
#define MAX_PORT_IO			4
#define MAX_SLAVE_IO		8
// *************************************************************************
#define M_BARCODE_PORT		0
#define C_BARCODE_PORT		1
// *************************************************************************
//  MOTOR 보드 관련 변수 선언한다                                                
// *************************************************************************
#define MOT_GOOD			1
#define MOT_PROGRESS		2
#define MOT_RETRY			3
#define MOT_ERROR			4

#define PLUS				1
#define MINUS				0
#define RUN_MODE			1
#define MANUAL_MODE			2

#define POWER_OFF			0
#define POWER_ON			1

#define ELM					0
#define ELP					1
#define HOME				2

#define MOT_ACC				0
#define MOT_DEC				1
#define MOT_JOG				2
#define MOT_ORG				3
#define MOT_FEEDRATE		4
#define MOT_INTERACC		5
#define MOT_ORGMETHOD		6
#define MOT_ORGACCDEC		7

#define MOT_FAST			10
#define MOT_SLOW			20
#define TSITE_SOCKET_CNT	48
// *************************************************************************

// *****************************************************************************
//  Main Screen 출력용 메세지 정의                                              
// *****************************************************************************

// *****************************************************************************
///// test bin  result
#define BIN_NOTUESD			0
#define BIN_LDBUFFERBIN		1
#define BIN_RETESTBIN		2
#define BIN_GOODBIN			3
#define BIN_REJECTBIN		4
#define BIN_REJECTBIN_1		5
#define BIN_REJECTBIN_2		6
#define BIN_REJECTBIN_3		7
#define BIN_REJECTBIN_4		8

///////////////////
#define RET_NOWHERE			-1000		// 해당 데이터가 존재하지 않을때 
#define RET_NOSPACE			-2000		// 공간이 없을때 
#define RET_EMPTYDEVICE		-3000		// 디바이스가 모두 없을때 
#define RET_FULLDEVICE		-4000		// 디바이스가 FULL
#define RET_LOADTRAYSKIP	-10000		// loader key skip

#define RET_PICKERALLYES 	-5000	// 피커 정보가 문제일때 
#define RET_PICKERALLNO 	-6000	// 피커 정보가 문제일때 
#define RET_PICKERNOTFIND	-7000	// 빈에 해당하는 정보가 없다

#define BUTTON_MOUSE_DN		1100
#define BUTTON_MOUSE_UP		1000

// GRID TYPE
#define DFC_NOUSE       -1//원래시스템 그리기 사용함(0번컬럼을 원래대로 사용할때 이용하면됨)
#define DFC_STATIC       0
#define DFC_EDIT         1
#define DFC_COMBO        2
#define DFC_CHECK        3
#define DFC_DATE         4
#define DFC_TIME         5
#define DFC_PROGRESS     6
#define DFC_IMAGE        7

#define IMG_LISTHDCTRL_BG _T("배경2.jpg")//_T("smflower.jpg")
#define IMG_PICCTRL_BG _T("배경2.jpg") //_T("smflower.jpg")
#define IMG_LISTHDCTRL_BG1 _T("배경25.gif")//_T("smflower.jpg")
#define IMG_PICCTRL_BG1 _T("배경25.gif") //_T("smflower.jpg")

// 칼라 정의
#define SKY_C					RGB(187, 250, 255)
#define NOR_C					RGB(164, 164, 164)
#define NOR_L					RGB(190, 190, 190)
#define RED_C					RGB(255, 0, 0)
#define RED_L					RGB(255, 150, 150)
#define RED_D					RGB(80, 20, 0)
#define BLUE_C					RGB(0, 0, 255)
#define WHITE_C					RGB(255, 255, 255)
#define BLACK_C					RGB(25, 1, 1)
#define BLACK_L					RGB(62, 55, 55)
#define GRAY					RGB(90, 80, 80)
#define YELLOW_C				RGB(255,255,0)
#define YELLOW_D				RGB(128, 128, 0)
#define YELLOW_L				RGB(255, 255, 193)
#define GREEN_C					RGB(0,255,0)
#define GREEN_L					RGB(150,255,150)
#define GREEN_D					RGB(0,100,20)
#define DSERBLE_BC				RGB(210,210,230)
#define TEXT_BC					RGB(220,230,210)
#define OK_BC					RGB(0, 0, 255)
#define NG_BC					RGB(255, 0, 0)
#define RESULT_BC				RGB(40, 1, 1)
#define ORANGE_C				RGB(0xFF, 0x68, 0x20)
#define BLACK					RGB(0, 0, 0)
#define BLUE_D					RGB(0, 0, 128)
#define BLUE				    RGB(0, 0, 255)
#define PINK					RGB(255, 125, 225)
#define TEXT_BC_1				RGB(240, 240, 240210)
#define BLUE_L					RGB(124, 139, 252)
#define ON_C					RGB(0, 255, 0)
#define OFF_C					RGB(0, 150, 0)
#define NG_C					0x0000FF
#define OK_C					0xFF00
#define BLACK_GC				0x00
#define YELLOW_GC				0xFFFF
#define TEST_BC					RGB(194, 231, 150)
#define IN_BC					RGB(204, 153, 255)

#define DVC_READY_C				RGB(153, 153, 255)
#define DVC_LOAD_C				RGB(239, 214, 198)
#define DVC_NOVIDEO_C			RGB(0,100,20)
#define DVC_PGM_C				RGB(255, 255, 0)
#define DVC_UNLOAD_C			RGB(255, 255, 204)
#define DVC_NONE_C				RGB(150, 150, 150)
#define DVC_START_C				RGB(150, 255, 150)
#define DVC_NO_C				RGB(128, 128, 0)

#define THICKNESS_FAIL			RGB(255,255,0)
#define LENGTH_FAIL				RGB(0xFF, 0x68, 0x20)
#define BARCODE_FAIL			RGB(255, 0, 0)

#define WINDOW_DN				RGB(219, 255, 210)
#define WINDOW_UP				RGB(250, 255, 244)

#define WINDOW_DN1				RGB(219, 255, 210)
#define WINDOW_UP1				RGB(250, 250, 250)

#define BLUE_L					RGB(124, 139, 252)
#define BLACK_GC				0x00
#define YELLOW_GC				0xFFFF
#define PINK_D					RGB(252, 233, 218)
#define COL_PUPLE				RGB(204,102,204)

// board 상태 정보
#define BIN_OFF					RGB(164, 164, 164)
#define BIN_NONE				RGB(62,	55, 55)
#define BIN_LOAD				RGB(187, 250, 255)
#define BIN_READY				RGB(150, 255, 150)
#define BIN_START				RGB(255, 255, 0)
#define BIN_ABORT				RGB(204, 102, 204)
#define BIN_GOOD				RGB(124, 139, 252)
#define BIN_FAIL				RGB(255, 0, 0)
#define BIN_RELOAD				RGB(255, 150, 150)

#define NORMAL_MODE				0
#define THRESHOLD_MODE			1

#define HOMING					1
#define MOVING					2
#define MULTI_MOVING			3
#define POS_MOVING				4

#define RET_ERROR				-1
#define RET_READY				0
#define RET_PROCEED				1
#define RET_GOOD				2
#define RET_RETRY				3
#define RET_SAFETY				4
#define RET_ABORT				5
#define RET_SKIP				6
#define RET_RACKSKIP			7

#define RET_PICKER_NOT_FIND		100  //피커에 더이상 작업할 공간이 없으면 
#define RET_TRAY_NOT_FIND		200  //트레이에 더이상 작업할 공간이 없으면 

/* ***************************************************************************** */
/* 화면 뷰 클래스 ID 설정 부분                                                   */
/* ***************************************************************************** */
#define IDW_SCREEN_MAIN					101
#define IDW_SCREEN_INIT					102

#define IDW_SCREEN_BASIC		 		201

#define IDW_SCREEN_WORK_INFO			301
#define IDW_SCREEN_SET_MAINTENANCE		302
#define IDW_SCREEN_SET_INTERFACE		303
#define IDW_SCREEN_SET_MEASURE			304
#define IDW_SCREEN_SET_RECIPE			305
#define IDW_SCREEN_SET_RFID				306
#define IDW_SCREEN_SET_FTP				307
#define IDW_SCREEN_SET_COK				308
#define IDW_SCREEN_SET_PART_NO			309
#define IDW_SCREEN_IO_FUNCTION			310
#define IDW_SCREEN_TEST_SITE			311

#define IDW_SCREEN_WAIT_TIME			401

#define IDW_SCREEN_MOTOR				501
#define IDW_SCREEN_MOTOR_SPEED			502

#define IDW_SCREEN_LIST					601
#define IDW_SCREEN_LIST_WORK			602
#define IDW_SCREEN_LIST_ALARM			603
#define IDW_SCREEN_LIST_STEP			605
#define IDW_SCREEN_LIST_ERROR			606
#define IDW_SCREEN_LIST_DATA_VIEW		607

#define IDW_SCREEN_IO_MAP				701

#define IDW_SCREEN_LOCK					801

#define IDW_SCREEN_ALARM				901

#define IDW_SCREEN_PGM_INFO				1001

#define SCREEN_MAIN						0
#define SCREEN_INITIAL					1
#define SCREEN_BASIC					2
#define SCREEN_SETTING					3
#define SCREEN_MOTOR					4
#define SCREEN_IO						5
#define SCREEN_LIST						6
#define SCREEN_ALARM					7
#define SCREEN_LOCK						8

// Yasunaga 관련 Define
#define Y_INITIALIZE					0//초기화 0미완료: , 1:완료상태
#define Y_MODELSELECT					1//모델 선택 0:미완료, 1: 완료상태
#define Y_RETURNTOORIGIN				2//원점복귀 0:미완료,1:완료상태
#define Y_AUTOMODE						3//자동 모드 0:OFF, 1:ON
#define Y_CALIBRATION					4//calibration  0:미완료, 1:완료상태
#define Y_PACKAGEDATA					5//패키지 데이터0:미완료, 1:송신완료상태
#define Y_AXISDATA						6//축데이터0:미완료, 1: 설정완료상태

#define L_BUTTON_DOWN					0
#define L_BUTTON_UP						1

#define MACHINE_MANUAL					0
#define MACHINE_AUTO					1

#define LOT_CLEAR						0 //2015.0303 james -1
#define LOT_START						1
#define LOT_END_START					2
#define LOT_END							3

#define CTL_NONE						0

//kwlee 2016.1229
#define SODIMM                          18
#define RDIMM                           6
#define UDIMM_9                         9
#define UDIMM_10                        10

// #define SODIMM                          1
// #define RDIMM                           2
// #define UDIMM_9                         3
// #define UDIMM_10                        4
//

#define PCB_TURN                        0
#define PCB_NO_TURN                     1

#define FRONT	                        1
#define REAR	                        2

#define HOST_WAIT						0
#define HOST_STOP						1
#define HOST_RUN						2

#define MAXMOTOR						7
#define PICKCNT							12
#define MAX_PICKCNT						20
//kwlee 2017.0204
#define PICK                            0
#define PLACE                           1
#define TEMP                            2

#define MAX_PICKER                      5
#define TOTAL_PICK                      10
#define MAX_INFO                        5
#define MAX_BUFFER                      30


#define BUFF_PICK_POS	                0
#define LABEL_PLACE_POS                 1
//2015.1005
#define  WM_DAY1_DATA					1
#define  WM_DAY3_DATA					2
#define  WM_DAY7_DATA					3
#define  WM_MONTH1_DATA					4
#define  WM_MONTH2_DATA					5
#define  WM_MONTH3_DATA					6
#define  WM_MONTHCAL_DATA				7
#define  WM_FILLRATIO_DATA				8
#define  WM_NOW_DATA					9


#define CONVEYOR_IN					0
#define IN_CONVEYOR_POSITION		1
#define IN_CONVEYOR_STOPPER_UP		2
#define IN_CONVEYOR_STOPPER_DOWN	3
#define OUT_IN_POSITION				4
#define TURN_CONVEYOR_POSITION		5
#define TURN_STOPPER_UP				6
#define TURN_STOPPER_DOWN			7
#define OUT_CONVEYOR				8
#define OUT_CONVEYOR_STOPPER_UP		9
#define OUT_CONVEYOR_STOPPER_DOWN  10


#define CLS_BCR_PRINTER1			0

enum PICKERUPDN
{
	PICKER_UP = 0,
	PICKER_DN,
};

enum NET_BCRPRINT_RECEIVED
{
	NVR_NONE,
	NVR_PASS,
	NVR_FAIL,
};

enum NETCLIENT
{
	CLS_ECSERVER	= 0,
	CLS_XGEM,
};

struct tagHANDLER_INFO
{
	CBitmap m_TitleBackBitmap;
	CBitmap m_BackBitmap;
	CBitmap m_BackDlgBitmap;

	BOOL bProgramExit;				//프로그램이 빠져나가는건지 확인한다.
	// **************************************************************************
	// LOT 처리 관련 플래그 설정 멤버 변수                                       
	// **************************************************************************
	bool bLoaderOnFlag;				// 로더 On : True    로더 Off : False
	bool bAllLotStartFlag;			// LOT 처음 시작 시 설정 됨
	// **************************************************************************

	int nMenuNum;					// 각 화면에 설정된 화면 번호 저장 변수 
	int nAlarmScreen;				// 현재 알람 화면이 출력되어져 있는지에 대한 정보 저장 변수 
	int nLoadState;					// 현재 로딩 버튼 관련 대화 상자 출력 여부 플래그 저장 변수 
	int nRetestRequest;				// 리젝 자재로 리테스트 하는건지 물어본다.
	// *************************************************************************
	// 원래 위치 복귀 기능 사용 시 사용되는 변수 선언                           
	// *************************************************************************
	int nHomingRequest[MAXMOTOR];	// 강제 Home Check 요청 플래그 설정 변수 
	int nMenuLock;					// 메뉴 사용 가능 상태 플래그 
	int nSystemLock;				// SYSTEM LOCK 상태 플래그
	int nSwitchLock;				// Dialog가 떠 있는 상태 플래그 (Start Switch Lock)	
	int nMaintMode;					// 메인트 모드 설정 플래그
	int nLevelMaint;				// 메인트 레벨(Level 1) 설정 플래그
	int nLevelTeach;				// 티칭 레벨(Level 2) 설정 플래그
	int nLevelAdmin;				// 관리자 레벨(Level 3) 설정 플래그
	int nLevelSpeed;
	int nIoBoardInitial;			// IO 보드 초기화 작업 완료 플래그
	int nMotorBoardInitial;			// 모션 보드 초기화 작업 완료 플래그
	int nMatroxBoardInitial;		// matrox 초기화
	int nIsAllSiteInitEnd;			// 모터 초기화 작업 완료 확인 플래그
	int nRunStatus;
	int nIdleStatus;
	int nManualSpeed;
	int nRunSpeed;
	int nMachineMode;
	int nLotStatus;					// Lot start 시작유무....
	int	nInitRetry;					// initialize retry 작업 유무.......
	int nInitialSuccess;			// Initialize 작업 성공유무.......	
	int nInitialError;
	
	bool bEqpStatus;
	bool bInOutCheck[2]; //kwlee 2015.1126

	double dCurrCmdPos[MAXMOTOR];	// 현재 모터 위치 정보 저장 변수 
	// *************************************************************************

	// *************************************************************************
	// 화면에 대한 핸들 정보 저장 변수 선언                                     
	// *************************************************************************
	HWND hWnd;						// 메인 프레임 핸들 저장 변수

	CWnd *cWndMain;					// 메인 화면 핸들 저장 변수
	CWnd *cWndBasic;				// BASIC 화면 핸들 저장 변수
	CWnd *cWndInitial;
	CWnd *cWndMaintenance;			// Maintenance 화면 핸들 저장 변수	
	CWnd *cWndWaitTime;				// Wait Time 화면 핸들 저장 변수
	CWnd *cWndMotorSpeed;			// Motor Speed 화면 핸들 저장 변수
	CWnd *cWndIO;					// IO 화면 핸들 저장 변수
	CWnd *cWndMotor;
	CWnd *cWndTitle;				// 타이틀 출력 화면 핸들 저장 변수
	CWnd *cWndList;					// 리스트 출력 화면 핸들 저장 변수
	CWnd *cWndAlarm;
	CWnd *cWndLock;
	CWnd *cWndWork;
	CWnd *cWndFtp;
	// *************************************************************************

	CString	strUserId;
	
	CTime		tRef;
	
	CTimeSpan	tRun;				// Run Time
	CTimeSpan	tJam;				// Jam Time
	CTimeSpan	tStop;				// Stop Time
	CTimeSpan	tMaint;				// Maint Time
	CTimeSpan	tDRun;				// Run Time
	CTimeSpan	tDJam;				// Jam Time
	CTimeSpan	tDStop;				// Stop Time
	CTimeSpan	tDMaint;			// Maint Time
	CTimeSpan	tWork[20][2];		// Work Time
	// jtkim 20150529
	CTimeSpan	tRunUph;
	CString		strHourUph;

	COleDateTime tCreate;
	COleDateTime tUph;

	int			nPgmVer;
	int			nFtpVer;			// jtkim 20150530 
	int			nVerUpdate;			// jtkim 20150604
	int			nAutoLine;			// jtkim 20150627

	//2015.1005
	CTimeSpan m_tDRef;
	CTimeSpan m_tDRdown[TSITE_SOCKET_CNT];
	CTimeSpan m_tDUserStop[TSITE_SOCKET_CNT];
	CTimeSpan m_tDStop[TSITE_SOCKET_CNT];
	CTimeSpan m_tDInStandby[TSITE_SOCKET_CNT];
	///////
	CTimeSpan m_tDtest[TSITE_SOCKET_CNT];
	CTimeSpan m_tDSokOff[TSITE_SOCKET_CNT];
	CTimeSpan m_tDOutStandby[TSITE_SOCKET_CNT];
	//kwlee 2015.1126
	CTimeSpan m_tStopTime[2][TSITE_SOCKET_CNT];

	int				mn_Pass[TSITE_SOCKET_CNT];
	int				mn_Fail[TSITE_SOCKET_CNT];
	int				mn_Input[TSITE_SOCKET_CNT];
	int             mn_Output[TSITE_SOCKET_CNT];
	int             mn_Yield[TSITE_SOCKET_CNT];


	int mn_allnewlot;
// 	int mn_InstandyBy[TSITE_SOCKET_CNT];
// 	int mn_InstandyBy_1[TSITE_SOCKET_CNT];
// 	int mn_OutstandyBy[TSITE_SOCKET_CNT];

	int mn_userstop;
	bool isTestRun[TSITE_SOCKET_CNT];
	CString mstr_DirPath;


	// jtkim 20160111
	int nDlgLock;
};
extern  tagHANDLER_INFO  st_handler_info;
// *****************************************************************************
// *************************************************************************
// RS-232C 시리얼 통신 관련 저장 구조체 선언                                     
// *************************************************************************
struct tagSERIAL_INFO
{
	int	nConnect[MAX_PORT];
	int nSndChk[MAX_PORT];				// 메시지 송신 완료 플래그
	int nRecChk[MAX_PORT];				// 메시지 수신 완료 플래그
	
	CString strPortChk[MAX_PORT];		// 시리얼 포트 생성 정보 저장 변수

	CString strSnd[MAX_PORT];			// 송신 메시지 저장 변수
	CString strRec[MAX_PORT];			// 수신 메시지 저장 변수

	CString strCommSnd[MAX_PORT];		// 리스트 화면 출력 송신 메시지 저장 변수
	CString strCommRec[MAX_PORT];		// 리스트 화면 출력 수신 메시지 저장 변수
	CString strCommErr[MAX_PORT];		// 리스트 화면 출력 에러 메시지 저장 변수

	int nSerialPort[10];
	int nSerialBaudrate[10];
	int nSerialData[10];
	int nSerialParity[10];
	int nSerialStop[10];

	
};
extern  tagSERIAL_INFO  st_serial_info;
// *************************************************************************

#define _MAX_ALARM_CNT		2000

// *************************************************************************
// 발생한 알람 관련 데이터 저장 구조체 선언                                      
// *************************************************************************
struct tagALARM_INFO
{
	// **************************************************************************
	// 알람 부위 표시 마크 [출력 위치], [크기], [칼라] 저장 변수                 
	// **************************************************************************
	int nOffset;
	int nCy;
	int nCx;
	int nLineWidth;
	int nR;
	int nG;
	int nB;
	// **************************************************************************

	// **************************************************************************
	// 파일에 저장된 알람 정보 저장 변수                                         
	// **************************************************************************
	CString strEContent[_MAX_ALARM_CNT];  // 파일에서 로딩한 한 라인 정보 저장 변수
	CString strECode[_MAX_ALARM_CNT];     // 알람 코드 저장 변수
	CString strEState[_MAX_ALARM_CNT];    // 알람 상태 저장 변수
	CString strEPart[_MAX_ALARM_CNT];     // 알람 발생 부위 저장 변수
	CString strEMsg[_MAX_ALARM_CNT];      // 공백 제거 알람 메시지 저장 변수
	// **************************************************************************

	// **************************************************************************
	// 화면에 출력할 알람 정보 저장 변수                                         
	// **************************************************************************
	int nActionCnt;            // 출력할 알람 조치 사항 갯수 저장 변수
	CString strStyle;           // 알람 출력 형태 저장 변수
	CString strBmpFile;        // 알람 표시 그림 파일 저장 변수
	CString strRepairMsg;      // 알람 조치 사항 저장 변수
	// **************************************************************************

	CStringList strListJam;		// 발생한 Jam Code의 리스트
	CString strCode;			// 발생한 알람 코드 저장 변수
	CString strPcode;			// 발생한 알람 코드 저장 변수
	CString strCurrMsg;
	CString strCurPart;
	CString strContent;

	int nCountMode;				// 알람 카운트 플래그 (0:알람 카운트 미진행    1:알람 카운트 진행)
	int nTypeMode;				// 알람 종류 저장 변수 (0:경고성    1:진행성    2:장비 정지)

	long stl_cur_alarm_time;	// 알람 발생 시간 저장 변수
	int stl_prev_code;			// 바로 전 발생한 알람 코드 저장 변수
	long stl_prev_alarm_time;	// 바로 전 발생한 알람 발생 시간 저장 변수

	int nAlarmOccured;			// 알람 발생 횟수 저장 변수
	int nAlarmOccuredDay;		// 알람 발생 횟수 저장 변수
	int nArmLotContinue;		// LOT 계속 진행 플래그
	int nResetStatus;			// Alarm Reset Status
	int nSkipMode;
	int nMotorAlarm;			// MOTOR관련 알람은 한번만 발생하도록 하기 위함!!  --> run시도이전엔 다시 울리지 말자!
	int nAlarmAssignSection;	//알람이 발생한 곳을 조금더 쉽게 찾기 위함
	int nCurState;
	int nAlarmClear;
	int nAlarmNum;

	int nAlarmCountClear;			// alarm 정보 clear
};	
extern  tagALARM_INFO  st_alarm_info;
// *************************************************************************

// *************************************************************************
// 알람 데이터 저장 구조체 선언                                                  
// *************************************************************************
struct tagALARM_DISPLAY_INFO
{
	// **************************************************************************
	// 알람 부위 표시 마크 [출력 위치], [크기], [칼라] 저장 변수                 
	// **************************************************************************
	int nOffset;
	int nCy;
	int nCx;
	int nLineWidth;
	int nR;
	int nG;
	int nB;
	// **************************************************************************

	// **************************************************************************
	// 화면에 출력할 알람 정보 저장 변수                                         
	// **************************************************************************
	int nActionCnt;            // 출력할 알람 조치 사항 갯수 저장 변수
	CString strStyle;           // 알람 출력 형태 저장 변수
	CString strBmpFile;        // 알람 표시 그림 파일 저장 변수
	CString strRepairMsg;      // 알람 조치 사항 저장 변수
	// **************************************************************************

	// **************************************************************************
	// 파일에 저장된 알람 정보 저장 변수                                         
	// **************************************************************************
	CString strContent[2000];  // 파일에서 로딩한 한 라인 정보 저장 변수
	CString strCode[2000];     // 알람 코드 저장 변수
	CString strState[2000];    // 알람 상태 저장 변수
	CString strPart[2000];     // 알람 발생 부위 저장 변수
	CString strMsg[2000];      // 공백 제거 알람 메시지 저장 변수
	// **************************************************************************

	// **************************************************************************
	// 현재 발생한 알람 저장 변수                                                
	// **************************************************************************
	int nCurrState;             // 알람 저장 여부 플래그
	CString strCurrMsg;         // 발생한 알람 메시지 저장 변수
	// **************************************************************************
};
extern  tagALARM_DISPLAY_INFO  st_alarm_display_info;


// **********************************************************************************
// 베이직 화면 환경 설정 정보 저장 구조체 선언                                   
// **********************************************************************************
struct tagBASIC_INFO
{
	//int		nModeTesterDisable; //teser 사용유/무(0:tester사용, 1:사용하지 않음(소켓오프여부 보지않음)

	int		nUldGoodTrayStack_Count; //언로딩 스태ㅔ커에 적재하는 셋팅 수량 (단독 장비 사용시 적용)
	int     nPickerErrorSkipMode; //0:스킵없이 에러, 1:자동으로 스킵하면서 이동 
	int		nCtrlMode;				// [Title Bar 상태 표시] < ONLINE, REMOTE, LOCAL.. 표시	> 
	int		nModeDevice;			// [Title Bar 상태 표시] < WHIT/WHIT OUT ㅡ MODE 표시	>
	int		nModeWork;				// [Title Bar 상태 표시] < 작업 모드 설정 표시			>
	int		nModeXgem;
	int		nModeInterface;
	int		nModeJigStack;			// unloader jig stack 방식.....
	int		nModeRfid;				// rfid 사용유무.....
	int		nModeBarcode;			// barcode read 유무....
	int		nModeLdBcr;				// Load Barcode Read 유무...
	int		nModeUnLdBcr;			// unloader barcode read 유무.....
	int		nRetry;					// retry 수량....
	int		nPartialPickCount;
	int		nModeJigLevel;
	int		nModeFrontSmema;
	int		nModeRearSmema;
	int		nModeFtpGms;			// GMS FTP 전송유무.....
	int		nVnRTime;
	int		nPcbArray;				// 연배열 수량.....
	int		nModeHook;
	int		nModeXgemRunChk;		// 20140806 jtkim..... rcmd check.....
	int		nModeXgemInterface;		// xgem interface [0] off line [1] on line local [2] on line remote.....
	int		nPartNoDgt;
	int     nBarcodeErrorSkip;      //Error Skip 사용 유무 kwlee 2017.0204
	bool	bEnableFlag;

	CString strDeviceName;			// [Title Bar 상태 표시] < 장비에서 설정한 DEVICE 표시	>
	CString strModelName;			// [Title Bar 상태 표시] < 비젼에서 설정한 JOB 정보 표시 >

	CString strNotice;				// Notice...

	CString	strEqp;					// eqp id.........
	CString strOperID;				// operator id.....
	CString strToolName;			// tool name.....
	CString strFtpUser;				// ftp 사용자명......
	CString strFtpPass;				// ftp 암호......
	CString strDevice;
	CString strVnrSpec[2][8];
	CString strVnrUcl[2][8];
	CString strVnrLcl[2][8];

	//2015.0302 이후 추가분 
	int		nMp_Docking_Sensor_Disable; //2015.0302 CTL_YES: M/P를 붙이지 않은 상태로 가동 
	int		nRotatorBuffer_IDCheck_Sensor_Disable; //2015.0302 CTL_YES: M/P를 붙이지 않은 상태로 가동 

	CString strHifixType[10];

	int nLastMdlCount;	// jtkim 20150501 last mdl count....
	int nBcrRead;		// jtkim 20150508
	int nBcrFail;		// jtkim 20150514 김창훈씨 요청

	CString strCodePath;
	// jtkim 20150529
	CString strScrapName;
	CString strScrapHead;

	CString strProcess;	// jtkim 20150623 in-line

	// jtkim 20150721
	int nSocketCount;

	// jtkim 20150930
	int nRecipeYesNo;	// recipe download.....
	int nBarcodeBlock;

	// jtkim 20151229
	int nRetestCount;
	//kwlee 2016.0325
	int nRowCnt;
	int nColCnt;
	int nVisionErrorCnt; //kwlee 2017.0202

	int nPcbType;
	int nPcbTurnEnable;
	int nRobotPickPos;
	int nRobotPlacePos;
	int nLeftSize;
	int nPcbCnt;

	int nPickerSelect[12];
	int nCellPos[2][6];

} ;
extern  tagBASIC_INFO  st_basic_info;
// ******************************************************************************

// **********************************************************************************
// 레시피 화면 환경 설정 정보 저장 구조체 선언                                   
// **********************************************************************************
struct tagRECIPE_INFO
{
	int		nTestRetest_Count;
	int		nBufferRotator_TestSite_Degrees; // ld/unld 같이 사용 . 0:0도, 1:90도, 2:180도, 3:270도 까지 셋팅 가능 
	int		nBufferRotator_PlateSite_Degrees; // ld/uld 같이 사용 . 무조건 0도로 셋팅이다 
	int		nFailCnt;						//연속 Fail일때 Socket Off 되는 카운트.
	// jtkim 20151017
	int		nBufferRotator_Barcode_Degrees; 
	int Buffer_DvcCheckSensor_Mode;    //버퍼 디바이스체크 센서 사용 유/무 
	int nCOKBuff_IDNum_Type;			////2015.0305 위부터 1로 변경 4.:2.5" reverse, 3:2.5" Front, 2:Msata, 1:pogo type //현재 작업할 COK ID 정보, buffer id와 공유한다, (COK Buff, Rotator Buff, Test site COK 정보가 이 번호와 같이 동일해야 한다 	

	int nTsiteUse[2][2][12];		// test site socket on/off 정보....[0]좌측(0)/우측[1] [0]하단(0)상단(1) [0]SLOT
	int	nTrayY;						// tray Y 정보.......
	int nTrayX;						// tray X 정보.......

	int nRetestTray_X; //2015.0222 retest tray를 사용시 X 방향으로 사용할 트레이의 정보
	int nRejectTray_X; //2015.0222 reject tray를 사용시 X 방향으로 사용할 트레이의 정보

	int nBufferY;					//buffer y 정보 
	int nBufferX;					//buffer x 정보
	int nHifix;						// hifix 번호.....
	int	nAbortTime;					// test timeout......
	int nCokType[4];

	double dTrayPitch_Y;			//트레이 피치간격 
	double dTrayPitch_X;			//트레이 피치간격 
	double dBufferPitch_Y;			//버퍼 피치간격 
	double dBufferPitch_X;			//버퍼 피치간격 
	double dTestPitch_X;			//버퍼 피치간격 

	double dCOKBuffPitch_X;			//COK 버퍼 피치간격 

	//2015.0206 모터를 X 방향으로 변경 double dLdUldPickerPitch_Y;		//picker pitch 
	double dLdUldPickerPitch_X;		//picker pitch 
	double dTestPickerPitch_X;		//picker pitch 

	double dTrayYOffset;			// tray Y pitch.......
	double dTrayXOffset;			// tray X pitch.......
	double dTSiteOffsetX;			// test site x offset.....

	double dRejTrayPick;			// Reject Tray Pick Offset...
	double dRejTrayPlace;			// Reject Tray Place Offset...

	CString strHifix;				// hifix name.....
	CString strCokTypeName[8];

	bool	bCokChk[8][3];

	BOOL	bUnldBcrFlag[2];		// Unload 단 Barcode A, B 사용유무 플래그

	int		nBcrBank;				// barcode bank......

	double	dTsiteOffsetY[12];	// test site offset y.......

	CString strPartNo;

	// jtkim
	int		nAbortPer;
	int		nIndividualPick;

	double	d15TOffset;

	//kwlee 2017.0116
	double dPickerRowOffset;
	double dPickerColOffset;
	double dPcbRowOffset;
	double dPcbColOffset;
	double dLabelRowOffset;
	double dLabelColOffset;


};
extern tagRECIPE_INFO	st_recipe_info;
// ******************************************************************************

// *****************************************************************************
//  Tower Lamp 화면에 대한 정보 저장 구조체 변수 선언                           
// *****************************************************************************
struct tagLAMP_INFO
{
	int nTFlick;
	int nLampR[9];				// RED 램프 상태 저장 변수
	int nLampY[9];				// YELLOW 램프 상태 저장 변수
	int nLampG[9];				// GREEN 램프 상태 저장 변수
	int nLampB[9];				// GREEN 램프 상태 저장 변수

	int nInterest;				// BLUE 램프 상태 저장 변수

	CString strEquipNo;			// 장비 호기 저장 변수
	CString strEquipCode;		// 장비 코드 저장 변수

	int nBuzzerMode;			// 부저 사용 모드 저장 변수
	int nStatusBuzzerRing;		// 사운드 울리고 있는 상태 변수 
	long lBuzzerOffWait[3];		// 사운드 일정 시간 울리고 멈추도록 하는 변수 

	int nLampWaitTime;			// 램프 FLACKER 시간 저장 변수

	int nModeLampBlue;			// 관심장비 설정 램프 사용여부
	int nModeLampBlueFlicker;	// 관심장비 설정 램프 FLACKER 여부
};
extern  tagLAMP_INFO st_lamp_info;
// *****************************************************************************


//kwlee 2017.0204
struct tagPICKER_DATA_INFO
{
	int nPickerData[2][MAX_PICKER][MAX_INFO]; //0 : BinData 1 : Yes/No 2: X_Pos 3: y: Pos
	int nPickerFailCnt;

	CString strPickerSerial[2][30];
};
extern tagPICKER_DATA_INFO  st_Picker_info;
//

struct tagBUFFER_DATA_INFO
{
	int nBufferData[2][MAX_BUFFER][MAX_INFO]; //0 : BinData 1 : Yes/No 2: X_Pos 3: y: Pos
	int nOutPutCnt; //프린터 출력 카운트
	CString strBufferSerial[2][MAX_BUFFER];
	int nBufferFailCnt;
	
};
extern tagBUFFER_DATA_INFO  st_Buffer_info[3];

enum LAMP_CTRL
{
	LAMP_ON		= 1,
	LAMP_OFF	= 2,
	LAMP_FLICK	= 3,
};

struct tagIO_INFO
{
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	// Module No 00
	///////////////////////////////////////////////////////////////////////////////////////////////
	int		o_ZaxisBreak;					// S0000
	int		o_TurnMotorDirection;			// S0001
	int		o_ResetLamp;					// S0002
	int		o_StartLamp;					// S0003
	int		o_StopLamp;						// S0004
	int     o_AlarmClrLamp;                 // S0005									
	int     o_BuzzerOff;                    // S0006									
	int     o_LabelFeederInterFace3;        // S0007

	int		i_AutoChk;						//PS0000
	int		i_DoorLockChk;					//PS0001
	int		i_ResetChk;						//PS0002
	int		i_StartChk;						//PS0003
	int		i_StopChk;   					//PS0004
	int		i_BuzzOffChk;   				//PS0005
	int		i_EmoSwChk;     				//PS0006
	int		i_AirDownSwChk; 				//PS0007

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Module No 01
	///////////////////////////////////////////////////////////////////////////////////////////////
	int		o_TwGreenLamp;					// S0100
	int		o_TwYellowLamp;					// S0101
	int		o_TwRedLamp;					// S0102
	int		o_BuzzOn;						// S0103
	int		o_InConvMotorOn;				// S0104
	int		o_InConvPosStopperUp;			// S0105
	int		o_InConvPosStopperDw;			// S0106
	int		o_TurnPosUp; 					// S0107

	int		i_InConvInChk;					//PS0100
	int		i_InConvPosChk;					//PS0101
	int		i_InConvPosUpChk;				//PS0102
	//PS0103

	int		i_TurnPosUpChk;					//PS0104
	int		i_TurnPosDwChk;					//PS0105
	int		i_TurnConvStopperForChk;		//PS0106
	int		i_TurnConvStopperBackChk;		//PS0107

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Module No 02
	///////////////////////////////////////////////////////////////////////////////////////////////
	int		o_TurnPosDw;					//S0200
	int		o_OutConvMotorOn;				//S0201
	int		o_OutPosStopperUp;				//S0202
	int		o_OutPosStopperDw;				//S0203
	int		o_TablePcbMovCw;				//S0204
	int		o_TablePcbMovCcw;				//S0205
	int		o_TableMotorForCyl;				//S0206
	int		o_TableMotorBackCyl;			//S0207

	int		i_OutConvChk;					//PS0200
	int		i_OutInConvPosChk;				//PS0201
	int		i_OutConvPosUpChk;				//PS0202
	int		i_TurnConvPosChk;				//PS0203
	//PS0204
	//PS0205
	int		i_TurnRotatCylForChk;			//PS0206	
	int		i_TurnRotatCylBackChk;			//PS0207	

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Module No 03
	///////////////////////////////////////////////////////////////////////////////////////////////
	int		o_HeadVacOff1;					// S0300
	int		o_HeadVacOff2;					// S0301
	int		o_HeadVacOff3;					// S0302
	int		o_HeadVacOff4;					// S0303
	int		o_HeadVacOff5;					// S0304
	int		o_HeadVacOff6; 					// S0305									
	int		o_HeadVacOn1;                   // S0306									
	int		o_HeadVacOn2;                   // S0307									
	int		o_HeadVacOn3;					// S0308
	int		o_HeadVacOn4;					// S0309
	int		o_HeadVacOn5;					// S0310
	int		o_HeadVacOn6;					// S0311
	///////////////////////////////////////////////////////////////////////////////////////////////
	// Module No 04
	///////////////////////////////////////////////////////////////////////////////////////////////
	int		o_HeadVacOff7;					// S0400
	int		o_HeadVacOff8;					// S0401
	int		o_HeadVacOff9;					// S0402
	int		o_HeadVacOff10;					// S0403
	int		o_HeadVacOff11;					// S0404
	int		o_HeadVacOff12; 				// S0405									
	int		o_HeadVacOn7;                   // S0406									
	int		o_HeadVacOn8;                   // S0407									
	int		o_HeadVacOn9;					// S0408
	int		o_HeadVacOn10;					// S0409
	int		o_HeadVacOn11;					// S0410
	int		o_HeadVacOn12;					// S0411
	///////////////////////////////////////////////////////////////////////////////////////////////
	// Module No 05
	///////////////////////////////////////////////////////////////////////////////////////////////
	int		o_HeadCylUpDw1;				// S0500
	int		o_HeadCylUpDw2;				// S0501
	int		o_HeadCylUpDw3;				// S0502
	int		o_HeadCylUpDw4;				// S0503
	int		o_HeadCylUpDw5;				// S0504
	int     o_HeadCylUpDw6;              // S0505									
	int     o_HeadCylUpDw7;              // S0506									
	int     o_HeadCylUpDw8;		        // S0507

	int		i_HeadVacOnChk1;				//PS0500
	int		i_HeadVacOnChk2;				//PS0501
	int		i_HeadVacOnChk3;				//PS0502
	int		i_HeadVacOnChk4;				//PS0503
	int		i_HeadVacOnChk5;   				//PS0504
	int		i_HeadVacOnChk6;   				//PS0505
	//PS0506
	//PS0507
	///////////////////////////////////////////////////////////////////////////////////////////////
	// Module No 06
	///////////////////////////////////////////////////////////////////////////////////////////////
	int		o_HeadCylUpDw9;					// S0600
	int		o_HeadCylUpDw10;				// S0601
	int		o_HeadCylUpDw11;				// S0602
	int		o_HeadCylUpDw12;				// S0603
	// S0604
	// S0605
	// S0606
	// S0607
	int		i_HeadVacOnChk7;				//PS0600
	int		i_HeadVacOnChk8;				//PS0601
	int		i_HeadVacOnChk9;				//PS0602
	int		i_HeadVacOnChk10;				//PS0603
	int		i_HeadVacOnChk11;				//PS0604
	int		i_HeadVacOnChk12;				//PS0605 
	//PS0606
	//PS0607		
	///////////////////////////////////////////////////////////////////////////////////////////////
	// Module No 07
	///////////////////////////////////////////////////////////////////////////////////////////////
	int		o_LabelFeederMotorOn;							// S0700
	int		o_LabelFeederClmpOn1;							// S0701
	int		o_LabelFeederClmpOn2;							// S0702
	// S0703
	int		o_PcReady;										//S0704
	int		o_PcComplete;									//S0705
	int		o_TableTurnCylFor;								//S0706
	int		o_TableTurnCylBack;								//S0707

	int		i_LabelFeederProductChk1;						//PS0700
	int		i_LabelFeederProductChk2;						//PS0701
	//PS0702
	int		i_LabelFeederStopperChk1;						//PS0703
	//PS0704
	int		i_LfReqChk;										//PS0705
	int		i_LfCompChk;									//PS0706
	int		i_LfAlarmChk;									//PS0707

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Module No 10
	///////////////////////////////////////////////////////////////////////////////////////////////
// 	int		o_LabelInterface1;				//S1000
// 	int		o_LabelInterface2;				//S1001
	
	int		o_Front_LabelReq;				//S1000
	int		o_Front_LabelComplete;	 		//S1001
	int		o_Rear_Label_Ready;				//S1002
	int		o_Rear_Label_Complete;			//S1003
	int		o_LabelInterface5;				//S1004
	int		o_LabelInterface6;				//S1005 
	int		o_LabelInterface7;				//S1006 

	int		i_LabelFeederProductChk1_1;		//PS1000
	int		i_LabelFeederProductChk2_1;		//PS1001
	int		i_FrontReady; 					//PS1002
	int		i_FrontComplete;				//PS1003
	int     i_RearReq;			            //PS1004
	int     i_RearComplete;                 //PS1005 

	int     o_HeadCylUpDn[PICKCNT];
	int     i_HeadVaccumCheck[PICKCNT];
	int     o_HeadVaccum[PICKCNT];
	int     o_HeadVaccumOff[PICKCNT];

	int		oMotPwr[MAXMOTOR];

	
};
extern  tagIO_INFO  st_io_info;
// *****************************************************************************

// *************************************************************************
// 파일 경로명 저장 구조체 변수 선언                                             
// *************************************************************************
struct tagPATH_INFO
{
	CString strBasic;			// 기본 셋팅 정보 저장 폴더+파일명 설정 변수

	CString strPathDvc;			// 디바이스별 티칭 정보 저장 파일 생성 폴더 설정 변수
	CString strSocket;			// 소켓별...
	CString strPathAlarm;		// 출력 알람 정보 존재 폴더 설정 변수
	CString strPathOp;			// 오퍼레이터 정보 저장 폴더 설정 변수
	CString strPathTray;		// Tray별 정보 저장 파일 생성 폴더 설정 변수

	CString strPathFile;		// 파일 생성 폴더 설정 변수
	CString strPathBack;		// BACKUP 파일 생성 폴더 설정 변수

	CString strPathRecipe;
	CString strPathFtpGms;		// ftp path 경로.....

	CString strPathRecovery;	// recoverydata........

	CString strFileBasic;		// 기본 셋팅 정보 저장 파일명 설정 변수
	CString strFileWait;		// 대기 시간 정보 저장 파일명 설정 변수
	CString strFileMotor;

	CString strOperation;		//Operation Log 파일 폴더 설정.
	CString strInterface;		//Interface Log 파일 폴더 설정.
	CString strMachine;			//Machine Log 파일 폴더 설정.
	CString strTotal;			//Total Log 파일 폴더 설정.
	CString strData;			//Data Log 파일 폴더 설정.
	CString strAlarmDay;
	CString strAlarmMonth;

	CString strIoMap;
	CString strPartIoMap;
	CString strIoPartMap;
	CString strMotorAxisMap;
	CString strMotorPartMap;
	CString strWaitTimeMap;

	CString strTool; // 20140811 jtkim

	// jtkim 20150414
	CString strBarcode;
	CString strBarcodeNg;

	// jtkim 20150607
	CString strPathLotWork;
	CString strPathLotPcb;

	// jtkim 20150721
	CString strPathCycle;

	// jtkim 20160107
	CString strPathTestSite;
};
extern  tagPATH_INFO st_path_info;
// *************************************************************************

struct tagWORK_INFO
{
	int		nPgmInfo;
	int		nSimulationMode;
	int		nMessageInfo;
	int		nStatusEqp;
	int		nRearSafetyChk;
	int		nTowerStatus[5];

	int		nRemoveInfo[10];		// alarm skip 정보.....
	int		nRemoveYesNo[10];		// alarm skip 유무.....

	int		nLfTsiteBin[2][12];
	int		nRiTsiteBin[2][12];

	int		nBcrX;					// barcode x 좌표.....
	int     nBcrY;					// barcode y 좌표.....
	int		nBdReadyChk;			//

	// jtkim 20150625 tray skip
	int		nUnLdCvyTrayRemove;

	CString strBarcode[2];

	int		nOldHifix;

	DWORD	dwCycleTime[3];

	double	dCurrPos[100];

	// jtkim 20150719
	int		nFtpWork;

	// jtkim 20150721
	DWORD	dwTrayCycle;
	DWORD   dwLotCycle;
	int		nCycleCount;
	int		nCycleYesNo;
	int		nTsiteCycleYesNo;

	// jtkim 20150818 cycle time관련 추가
	DWORD	dwFrontTime[3];
	DWORD	dwFrontSmema[3];

	DWORD	dwRearTime[3];
	DWORD	dwRearSmema[3];

	// jtkim 20150902
	CString strHifix[10];

	// daily cycle time
	double	dDailyCycle;

	CTime	tLotStart;
	CTime   tLotEnd;

	CString strSocketOff;
	CString strSocketOn;
};
extern  tagWORK_INFO  st_work_info;

// *************************************************************************
// 기타 정보 저장 구조체 선언                                                    
// *************************************************************************
struct tagOTHER_INFO
{
	CString strOpMsg;			// 리스트 화면 출력 동작 에러 메시지 저장 변수
	CString strNormalMsg;		// 리스트 화면 출력 동작 에러 메시지 저장 변수
	CString strAbnormalMsg;		// 리스트 화면 출력 동작 에러 메시지 저장 변수
	CString strSndMsg;			// 리스트 화면 테스터 SEND 메시지 저장 변수
	CString strRcvMsg;			// 리스트 화면 테스터 RECIVE 메시지 저장 변수
	CString strBcrRcvMsg;		// 리스트 화면 바코드 RECIVE 메시지 저장 변수
	CString strBoxMsg;

	CString strFallacyMsg;		// 오류 메시지 대화 상자 타이틀 저장 변수
	CString strConfirmMsg;		// 확인 메시지 대화 상자 타이틀 저장 변수
	CString strTypingMsg;		// 키보드 대화 상자 타이틀 저장 변수

	CString strKeypadMsg;		// 키패드 대화 상자 타이틀 저장 변수
	CString strPadVal;			// 키패드 대화 상자에 출력할 데이터 저장 변수

	CString strPadHighLimit;
	CString strPadLowLimit;

	CString strPassLevel;		// 암호 레벨 저장 변수
	CString strPassword;		// 암호 저장 변수
	
	CString strParentMenu;		// 주메뉴 이름 저장 변수

	CString strMsgBuf[10];

	char cNormalMsg[4096];
	char cAbnormalMsg[4096];
	char cOpMsg[4096];

	int nBuzYesNo;				// jtkim 20150410
	int nConfirmMsg;
	int nConfirmType;

	int nSocket[3];				// Socket Comment 20150804...
	////////////////////////////////2K10/02/09/ViboX////////////////////////////////////////
	int		nPrevStep[100];			// 이전 step (변경 되었는지 보기 위함)
	CString	strHistoryStep[100];		// step_history 약 50개에 해당하는 step을 저장
	CString strLastChangeDate[100];	// 최종 step으로 변경되는 시점의 시간
};
extern  tagOTHER_INFO  st_other_info;
// *************************************************************************

// *************************************************************************

#define INIT_CLEAR					0
#define INIT_READY					1
#define INIT_COMPLETE				2

#define WORK_CLEAR					0



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


enum TEST_SITE_HIGH_LOW_INFO  //test site high / low 정보 
{
	 TESTSITE_LOW							=  0,
     TESTSITE_HIGH						    , 
};

enum THREAD_SYNC_VARIBLE_SITE_INFO  //위치별 트레이 존재 유무를 위치별로 정의해 놓음  
{
	// tray site

};




//#define LOADTRAY_PALTE_NUM			2
//#define UNLOADTRAY_PLATE_NUM       2

//#define TESTROBOT_INFO_REQ			2	//0:req 응답, 1:작업 시작위치  

//
//#define MAX_ROBOT_SITE				3 //0:load robot, 1:unload robot, 2:test robot 
//#define PICKER_DATA_INFO			5//[0]:dvc 유/무, [1]:빈정보, [2]:retest cnt, [3]:screen 빈 정보, [4]:현재 디바이스 유/무 상태 임시 저장, 또는 보완정보 확인
//#define MAX_PICKER_PARA				4



struct tagSYNC_INFO
{
	
	int			nInitRbt[MAXMOTOR];
	int			nInitializeError;
	int			nLotEnd[50];					// lot end flag 상태.....
	int			nInitPickerRbt;
	int			nInitConveyor;
	int         nInitLabel;
	///////////////////////////////////////////////////
	int         nInitRobot;
	int         nInitConv_pitch;
	int         nInitRobot_Pitch;
	int         nInitRobotTurn;
	int         nInitLdRbt;
	int			nInitializeSuccess;
	int 		nSmema_Tray_Input_Req; //lable 장비 
	int 		nSmema_Tray_Output_Req; //AOI Vision 장비
	int 		nFrontTrayIn;
	int 		nMidTrayIn;
	int 		nRearTrayIn;
	int         TurnConvJobReady[4];

	int         nSmema_Front;
	int         nSmema_Rear;

	
};
extern tagSYNC_INFO	st_sync_info;




struct tagVARIABLE_INFO
{
	int			nForce_LOTEND;//2015.0224 james 강제로 lot end를 진행시에 사용한다 

	int			nRetestBin_OutputCount_Full_Flag; //retest bin를 처리해야하는 구간이다. (로딩 피커는 로딩 자재를 미리 집고있지 말고 리테스트 자재가 공급되기를 기다린다 )

	int			nTestDvcOutFlag; //현재 lot이 테스트가 끝나 언로딩 중이다. 빈 트레이를 언로딩 플레이트에 공급해야 한다 
	
	int			nTester_MP_Docking_End_Flag[2]; //left, right 
	int			nCokBuffer_Change_End_Flag; //100: cok가 교체를 시작한다, 200: COK 교체가 완료된 상태(마지막 실린더 작업만 남음), 0:cok 교체 작업 모두완료 

	//2015.0104 int nTray_Exist_Info[TRAY_EXIST_POSITION_INFO_MAX_SITE]; //트레이 존재 유무를 셋팅한다 
	int			nEmptyStackerSite_LastMovePos_Num; //2015.0111 1:
	double       dEmptyStackerSite_LastMovePos_Backup; //각 위치에 있을때의 백업 위치 

	int			nUnloadStackerSite_LastMovePos_Num; //2015.0111 1:
	double       dUnloadStackerSite_LastMovePos_Backup; //각 위치에 있을때의 백업 위치 

	//int			nTrayRunMode_StandAlone; //트레이를 뒷 공정으로 보내지않고 언로딩 스태커에 적재하는 모드 

	int			nBuffer_TestSite_RotatorDegree; //테스트 사이트에서 필요한 각도 ==> 0, 90, 18, 270도 회전 가능
	int			nBuffer_TraySite_RotatorDegree; //트레이 사이트에서 필요한 각도 ==> 무조건 0도 일것이나 혹시 몰라 추가 

	//
	int			npRotatorBuffer_Mapping_Num_Y[10][10];
	int			npRotatorBuffer_Mapping_Num_X[10][10];

 
	//////
	//LOT 정보 관련 변수, Max 10개까지 선언은 했지만 시용은 최대 3개 까지만 진행 가능하게 하고, 3개 이상일때는 Input smema 사이트에서 대기하게 하자  
	//int			nLotWork_Count; //현재 장비 내에서 진행중인 lot의 수량, Max 10개까지 선언은 했지만 시용은 최대 3개 까지만 진행 가능하게 하고, 3개 이상일때는 Input smema 사이트에서 대기하게 하자  
	//int			nLot_LastModule[10]; //각 LOT의 LAST MODULE 정보
    //CString     strLot_Name[10];     //각 LOT의 NAME
	//CString		strCurrent_LotName; //현재 최초로 작업중이던 최신 lot 의 정보(nLot_LastModule[0] 번째 배열의 정보와 같아야 한다 )
	
	//lot end하면서 이 정보를 변환하여 현재 lot의 정보를 한번에 변경한다 
	int			nLotExchange_CurrentLotNum; //0,1 의 정보를 가진다 0 -> 1 -> 0 무한 반복 (현재 작업중인 lot의 정보를 update 하는 역활을 함 

	//CString		strReservation_LotName; //예약 lot 저장 가능.

	////// 아래변수는 2015.0225 이후의 변수 
	int			nLdbuffer_TestSite_MoveReady_Flag; //로딩버퍼에  자재가 준비되어 테스트 사이트로 이송해 대기 중이다. 이떄 테스트 로봇은 로딩할 소켓이있으면 버퍼의 자재부터 먼저 처리한다 
	int			nRotatorBuffer_PlateTrayReferencePoint_Degrees; //205.0227 테스트 사이트에서 Turn한 위치를 tray plate위치의 기준점을 (0도)다시 돌리는데 필요한 degrees 위치값 (/0(0도), 1(90도),2(180도),3(270도) 일때 0으로 갈수 있는 값)

	int			nTestSite_LastSSDPlace_Pos; //2015.0302 마지막 테스트 사이트에놓은 위치 (0 - 47개의 소켓 정보)

	int			nTestSiteRobot_Z_RejectTrayMove_Safety_Action_Flag[10]; //2015.0303 reject tray 교체시 안전 위치 체크 및 이동위치 

	int			nTemp_Spare_Flag[100]; //2015.0303 변수 설정이 없어 프로그램 실행중 사용하는 임시 변수  

	int			nTestSite_SSD_Place_OneCycle_Flag; //2015.0309 Current Lot 각 사이트 전체 작업한 횟수(0- 11까지 12개 사이트)
	 
	// jtkim 20151017
	int			nRotatorBuffer_Barcode_Degrees;
	//2015.0312
	//st_var 변수 
	//int n_Contact_Start_Flag[2][THREAD_MAX_SITE][2][12]; //작업 지정 정보(test robot에서 지정)
	//int n_Contact_Start_Status[THREAD_MAX_SITE][2][12];//작업 진행여부
	int n_Contact_WorkSite_Info[2][48];//작업 가능 위치정보
	int n_Contact_Start_Pos[2];//작업 시작 위치
	int n_Contact_End_Pos[2];//작업 완료 위치
	int n_Contact_WorkCnt[2];//작업 할 소켓 수량
	int n_Contact_Site[2]; //left/right test site
	int n_Contact_TopBtm[2]; //left/right test site
	int n_Contact_XPos[2]; //left/right test site

	int n_Socket_Conatct_Work_Mode_Flag[2]; 

	//2015.0314 추가 
	int n_RetestBinTray_LoadSite_Supply_Count;
	int n_RetestBinTray_LoadSite_MoveTo_Start_Flag;

	int n_NextLot_TestSiteSocket_Work_Enable_Count;

	//2015.0322 추가 
	int n_LotEnd_LastRetetstCount_Dvc_Flag[2]; //[0]:마지막 retest 자재인지, [1]:retest 남은 자재수량 

	//2015.0323 
	int n_SocketOff_Seperator_Mode_Flag; //4개 소케중 0 또는 2만 시작 우치가 될 수 있다, 즉 2개 단위로 소켓 오프, 1:4개의 소켓중에는 하나 단위로 소켓 오프 가능
	
	//2015.0324 james test socket auto socket on/off 처리시 사용 
	int n_Auto_Socket_Open_WorkPos_Num;
	int n_Auto_Socket_Close_WorkPos_Num;
	

	// jtkim 20150826 
	int nPlateFlag;			// unloading plate 1.....
	int nRejectFlag;		// reject stacker....
	int nRetestFlag;		// retest.....
};
extern tagVARIABLE_INFO	st_var;




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 2015.0102 
#define MAX_GMS_CHANNEL					8

struct tagCLIENT_INFO
{
	CString		strIp;			// ip address......
	
	int			nPort;			// port.....
	int			nConnect;		// client 연결상태......
	int			nSendLength;	// 송신 문자 길이......
	int			nRevLength;		// 수신 문자 길이......
	int			nClientReady;	// 데이터 전송중...
	int			nRevInfo;		// client 종료 방법..... (0:바로 1: 수신후)

	char		chRev[8196];	// 수신내용....
	CString     strRev;
	char		chSend[8196];   // 송신내용....
	CString		strSend;
};
extern tagCLIENT_INFO st_client_info[20];

struct tagSERVER_INFO
{
	CString		strIp;			// ip address......
	
	int			nPort;			// port.....
	int			nConnect;		// client 연결상태......
	int			nSendLength;	// 송신 문자 길이......
	int			nRevLength;		// 수신 문자 길이......
	
	char		chRev[8196];	// 수신내용....
	CString     strRev;
	char		chSend[8196];   // 송신내용....
	CString		strSend;
};
extern tagSERVER_INFO st_server_info[20];

struct tagRS232_INFO
{
	int			nSerialPort[10];		// serial port....
	int			nSerialBaudrate[10];	// serial baudrate....
	int			nSerialData[10];		// serial data....
	int			nSerialParity[10];		// serial parity.....
	int			nSerialStop[10];		// serial stop....
};
extern tagRS232_INFO st_rs232_info;

struct tagGMS_INFO
{
	CString		strValVolt[MAX_GMS_CHANNEL];
	CString		strValRes[MAX_GMS_CHANNEL];
	CString		strEqpID[MAX_GMS_CHANNEL];
	CString		strDateTime[MAX_GMS_CHANNEL];

	CStringArray	ArValVolt[MAX_GMS_CHANNEL];
	CStringArray	ArValRes[MAX_GMS_CHANNEL];

	COleDateTime oleTime[MAX_GMS_CHANNEL];
};
extern tagGMS_INFO	st_gms_info;

#define RCVR_CLEAR						0
#define RCVR_PICK_UP_COMMAND			1
#define RCVR_PICK_UP_COMPLETE			2
#define RCVR_MOVE_COMMAND				3
#define RCVR_MOVE_COMPLETE				4

#define MOVE_FORWARD					0
#define MOVE_BACKWARD					1

#define LOW_SPEED						0
#define HIGH_SPEED						1



struct	tagRECOVERY_INFO
{
	double	dRecoveryPos[100];	// 각축의 recovery position......

	int		nRecoveryStep[100];	// recovery step......
	int		nRecoveryFlag[100];	// recovery 완료 여부........
	
	int		nMcRecoveryUsed;
	int		nMcDataRecovery;

	// jtkim 실린더 recovery 변수
	int		nRecoveryCyl[100];
};
extern tagRECOVERY_INFO st_recovery_info;

//#define MAX_PICKER				4
#define MAX_BUF_ROW_Y				10
#define MAX_BUF_COL_X				10
#define MAX_TRAY_ROW			20
#define MAX_TRAY_COL			20
#define MAX_SITE_PICKER		4
#define MAX_SITE				12

#define LD_ULD_PICKER_PARA			2
#define TEST_PICKER_PARA			4

#define VACUUM_WAIT_TIME			100  //100ms 까지 체크시간 확인 

#define MAX_RETEST_COUNT			5

struct tagPCB_INFO
{
	int nTestBdStart;						//테스트 시작과 끝을 알수 있다 
	int nEnable;							//해당 위치 사용유/무 
	int nYesNo;								// ssd 유무.....
	int nBin;								// ssd 테스트 결과.....
	int nRetestCnt;							// retest 테스트 횟수......
	int nRetest;
	int	nBinNum;							// ssd 테스트 결과.....
	int	nRetestSite[MAX_RETEST_COUNT];		// retest 실행한 site 번호....
	int nBdNum;								// 마지막 테스트 테스트 Board 번호.......
	int nScrCode;							// scrap code.....
	int nCOK_IDNum;							//cok 정보 1~7까지 
	int nCOK_YesNo;							//cok 존재 유/무 셋팅 
	int nStatusChang;						// 상태정보 변경.........
	int nOldBin;							// barcode....
	DWORD dwBdTime;							// test time......

	CString strLotNo;						// pcb Lot number.....
	CString strSerialNo;					// Lot에서 pcb의 순서번호......
	CString strCSerialNo2;
	CString strBarcode;						// pcb 2D barcode......
	CString strPartNo;						// part no.....
	CString strArrNo;						// array serial....
	CString strPPID;						// ppid.....
	CString strPSID;						// psid....
	CString strWWN;							// wwn.....
	CString strCSerialNo;					// c_serial......
	CString strBin;							// strBin......
	// jtkim 20150707
	CString strOldSerial;

	// jtkim 20150805
	CString strComment;

	COleDateTime tStart;					// 테스트 시작시간...
	// jtkim 20151027
	CTime tStandBy;
	// jtkim 20151028
	int nReworkNg;							// rework mode check......

	// jtkim 20160106
	int nCntInfo;							// count info......

	// jtkim 20160108
	DWORD dwCycle[10];						// log add...
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PCB_info
{
	int nPcbSerial[2][10];
	int nPcbType;
	int	nPcbTurn;
	int nPcbSelect[2][10];
	int nLeftSize;
	int m_arrayID;
	CString strPcbSerial[2][10];

};

extern PCB_info st_Pcb_info;


//////////////////////////////////////////////////////////////////////////////////////////////
//2015.0106 전체 plate의 사이트의 트레이 자재 정보를 관리한다 
//////////////////////////////////////////////////////////////////////////////////////////////

//2015.0316 james COK
struct tagCOK_INFO
{ 
	int nWorkEnable;						//해당 위치 사용 유/무, 사용하면 1, 사용하지 않으면 0임
	int nCOK_IDNum;							//cok 정보 1~7까지 
	int nYesNo[24];							// cok 유무.....	
};

//2015.0316 james COK
struct tagCOKBUFF_SITE_INFO
{
	// jtkim 20150330
	int	nCokCount; // cok 수량.......

	tagCOK_INFO st_pcb_info; // pcb 구조체 변수.....
};
extern tagCOKBUFF_SITE_INFO st_Cok_Buff_info[4]; //cok 4개 사이트 정보 


struct tagTEST_SITE_SOCKET_FIND_INFO
{////npGetSiteInfo  ==>  [0]:start socket pos(0~47), [1]end socket pos(0~47), [2]:left, right site 정보(0,1), [3]:socket start Y Pos(0~1, btm, top 구분), [4]:socket start X Pos(0~11, 12개 정보 위치)
	int nWork_Flag; //0:작업 불가능, 1:로딩 작업가능, 2:언로딩 작업가능 
	int nWork_Cnt; //작업 가능한 수량
	int nPickerStart_Num; //0 또는 2 //0:1번 피커부터 집을때, 2: 3번 피커부터 집을때 
	int nAll_FirstPos; //시작 위치(0 ~ 47 전체 소켓 정보)
	int nAll_EndPos; //end socket pos(0~47)
	int nLeftRight_Site; //0:left, 1:right test m/p
	int nBtmTop_Y_Pos; //소켓 y 위치 0: btm, 1:top
	int nX_Pos; 	 //0~11 까지 12개 위치
	int nX_FixPos; //각 작업 사이트의 고정된 시작 위치
	int nBinCnt[3]; //[0]:good 수량, [1]:retest수량, [2]:reject 수량  //2015.0222추가 james 

	//2015.0324 james socket on/off를 분리하여 동작하게 한다 
	int nSocket_OnOff_Start;	//작업 시작 여부 
	int nSocket_AssyWorkStart_Pos;	//작업 시작 위치(0~11 총 12개 구간)
	int n_Socket_AllWorkStart_Pos; //전체 소켓중 작업시작 소켓 위치(0 ~ 47까지 총 48개 소켓 위치)
	int npSocket_YesNo[4];		//4개 사이트의 동작 플레그 

	
};
extern tagTEST_SITE_SOCKET_FIND_INFO st_test_socket_info[12]; //0~47까지의 소켓을 12가지로 구간을 나눠 관리 
extern tagTEST_SITE_SOCKET_FIND_INFO st_test_socket_status[12]; //2015.0222 james 소켓의 상태를 확인할때 사용. 고유 소켓정보 작업 변경까지 유지  //0~47까지의 소켓을 12가지로 구간을 나눠 관리, thj 

//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* 2015.0108
// loader picker 구조체
struct tagLD_PICKER_INFO
{
	tagPCB_INFO st_pcb_info; // pcb 구조체 변수.....
};
extern tagLD_PICKER_INFO st_ld_picker_info[MAX_PICKER];

// loader buffer 구조체
// [0] load picker -> buffer [1] buffer -> test site robot
struct tagLD_BUF_INFO
{
	tagPCB_INFO st_pcb_info[MAX_BUF_ROW][MAX_BUF_COL]; // pcb 구조체 변수.....
};
extern tagLD_BUF_INFO st_ld_buf_info[2];

// unloader picker 구조체
struct tagUNLD_PICKER_INFO
{
	tagPCB_INFO st_pcb_info; // pcb 구조체 변수.....
};
extern tagUNLD_PICKER_INFO st_unld_picker_info[MAX_PICKER];

// unloader buffer 구조체
// [0] unload picker <-> buffer [1] buffer <-> test site robot
struct tagUNLD_BUF_INFO
{
	tagPCB_INFO st_pcb_info[MAX_BUF_ROW][MAX_BUF_COL]; // pcb 구조체 변수.....
};
extern tagUNLD_BUF_INFO st_unld_buf_info[2];



// loader tray 구조체
struct tagLD_TRAY_INFO
{
	tagPCB_INFO st_pcb_info[MAX_TRAY_ROW][MAX_TRAY_COL]; // pcb 구조체 변수.....
};
extern tagLD_TRAY_INFO st_ld_tray_info[2];

// unloader tray 구조체
struct tagUNLD_TRAY_INFO
{
	tagPCB_INFO st_pcb_info[MAX_TRAY_ROW][MAX_TRAY_COL]; // pcb 구조체 변수.....
};
extern tagUNLD_TRAY_INFO st_unld_tray_info[2];

// reject buffer 구조체
struct tagREJECT_BUFFER_INFO
{
	tagPCB_INFO st_pcb_info[MAX_TRAY_ROW][MAX_TRAY_COL]; // pcb 구조체 변수.....
};
extern tagREJECT_BUFFER_INFO st_reject_buf_info[2];

// retest buffer 구조체
struct tagRETEST_BUFFER_INFO
{
	tagPCB_INFO st_pcb_info[MAX_TRAY_ROW][MAX_TRAY_COL]; // pcb 구조체 변수.....
};
extern tagRETEST_BUFFER_INFO st_retest_buf_info;


// test site robot 구조체
struct tagTEST_SITE_RBT_INFO
{
	tagPCB_INFO st_pcb_info[MAX_SITE_PICKER]; // pcb 구조체 변수.....
};
extern tagTEST_SITE_RBT_INFO st_test_site_rbt_info;
*/

struct tagTRACK_INFO
{
	CString	strLotNo;
	CString strPartNo;

	long	nIn;
	long	nPrimePass;
	long	nPrimeReject;
	long	nCumPass;
	long	nCumReject;

	int		nTestEndBin;
	int		nStatus;		// track 상태........
};
extern struct tagTRACK_INFO st_track_info;

enum PICKER_PICKPLACE_INFO
{
	 PICKER_PICK_MODE	= 0,
	 PICKER_PLACE_MODE	   ,
};

#define	BD_READY_CHK_TIME	20		// board ready 명령후 완료체크 시간

enum BOARD_INFO
{
	BD_NONE							= 0,
	BD_START						,
	BD_READY_CHK 					,
	BD_READY_OK						,
	BD_COMPLETE                                ,
	BD_SLOT_OFF						,
	BD_SLOT_ON						,
	BD_DATA_GOOD					= 200,
	BD_DATA_REJECT					,
	BD_DATA_ABORT					,
	BD_DATA_RETEST					, //2015.0120
	BD_CONT_TIMEOUT					,
	BD_SEND_TIMEOUT					,
	BD_RCV_TIMEOUT					,
	BD_PCB_CHK_TIMEOUT				,
	BD_DATA_ABORT_CHK				, // 20150819
};

// test site board 정보
struct tagBOARD_INFO
{
	long nBdTestCnt[12];			// board 테스트 횟수.....
	long nBdPassCnt[12];			// pass 횟수......
	long nBdInfo[12];				// board 정보...... 0(Clear) 1(Start) 2.......
	long nBdBin[12];				// board 결과 정보...... 
	long nBdFailContiCnt[12];		// 연속 fail 수량.....

	CString strBinHistory[12];		// jtkim 20150320......
	CString strSocketComment[12];	// Socket Off Comment 20150804
};
extern tagBOARD_INFO st_bd_info[2][2];

enum LOT_INFO
{
	LOT_CURR						= 0,
	LOT_NEXT						,
	LOT_CURR_BACKUP					, //2015.0302 james LOT_CURR을 lot end 시 클리어전에 백업 데이터를 받는다 
};
// lot 정보.....
struct tagLOT_INFO
{
	/*
	//////
	//LOT 정보 관련 변수, Max 10개까지 선언은 했지만 시용은 최대 3개 까지만 진행 가능하게 하고, 3개 이상일때는 Input smema 사이트에서 대기하게 하자  
	int			nLotWork_Count; //현재 장비 내에서 진행중인 lot의 수량, Max 10개까지 선언은 했지만 시용은 최대 3개 까지만 진행 가능하게 하고, 3개 이상일때는 Input smema 사이트에서 대기하게 하자  
	int			nLot_LastModule[10]; //각 LOT의 LAST MODULE 정보
    CString     strLot_Name[10];     //각 LOT의 NAME
	CString		strCurrent_LotName; //현재 최초로 작업중이던 최신 lot 의 정보(nLot_LastModule[0] 번째 배열의 정보와 같아야 한다 )
	*/
	CString strLotNo;				// lot no......
	CString strPartNo;				// part no......
	CString strProcess;				// process id.....
	CString strProPerty;			// property end....
	CString strBypass;				// bypass.....

	
	int     nLotStatus; //0:clear, 1:set
//	int		nLotEnd[THREAD_MAX_SITE]; //해당 사이트 lot end 플레그 lkw
	int		nNewLotIn;				//[0]Label 장비 new lot in , [1]:AOI 장비 정보
	int		nLastModule;			// last module....//[0]Label 장비 new lot in , [1]:AOI 장비 정보
	int		nNewLotComplete;		// new lot check 상태.....
	int		nTotalCnt;				// total count....
	int		nLane;					// ......
	int		nLdTrayCnt;				// loading tray 수량.....
	int		nUnLdTrayCnt;			// unloading tray 
	int		nBinSortCnt[3];			//0:good, 1:reject, 2:retest 수량 //2015.0225 james 
	int		nLdCnt;
	int		nTrayRunMode_StandAlone; //트레이를 뒷 공정으로 보내지않고 언로딩 스태커에 적재하는 모드 
	int		nScrapCode;				// scrap code check.....

	COleDateTime	tStart;			// jtkim 20150408

	// jtkim 20160108
	CTime	tLotStart;

	// jtkim 20160124
	int		nAbortTime;
	int		nAbortTotal;
	int		nAbortCount;
	int		nAbortUsed;
};
extern tagLOT_INFO st_lot_info[3]; //2015.0302 james [2] -> [3] 

struct tagANIMATE_INFO
{
	double	dMaxPos[30];		// animate 최대 pulse.....
	double  dMovePos[30];		// 현재 motor pulse .......
	double	dMotorDistance[30];
	double	dMoveOffset[30];

	int		nMoveDistance[30];
	int		nMoveOffset[30];
	int		nImgNum[30];		// image 번호......
	int		nDistance[30];

	bool	bTrayFlag[9];		// Conveyor Assy 위의 Tray 표시 Flag 0=LdCvyFr 1=LdCvyRe 2=LdRailFr 3=LdRailRe 4=EmptyStk 5=UnldRail 6=UnldElv 7=UnldCvy 8=MvCvy;
};
extern tagANIMATE_INFO st_animate_info;

struct tagCOUNT_INFO
{
	int nCokBufferCount[4]; //cok buffer의 cok 수량 정보
	int nPcbQty;
	int nBadPcbQty;
	int	nPcbLotMoveCount[10];		// 공급된 Pcb 수량......
	int	nJigCount;					// jig 수량......
	int	nPcbLotMove;				// Lot Count.......
	int	nLatestUph[24];				// Uph.......

	int nSSD_Input[2];				//loading tray [0]:lot 수량, [1]:DAILY 수량 
	int nTest_Site[2][2][12];		//test site 작업 수량 
	int nSSD_Output[2][20];			//언로딩 빈 수량 //[0]:lot 수량, [1]:DAILY 수량 

	int nEmptyStackerTray;			//적재된 트레이 수량 
	int nUnldStackerTray;			//트레이 수량 
	int nRejectTray;				// reject tray수량....

	// count 표시하기 위해
	long nInCount[2][2];			// [0][0]:lot 수량, [0][1]:lot 수량 [1][0]:DAILY 수량 
	long nPassCount[2][2];			// 양품수량.....
	long nPrimeCount[2][2];			// nPrimeCount........
	long nRejectCount[2][2];
	// jtkim 20150307
	long nPrimeRejectCount[2][2];

	int nUph;
	int nDailyUph;
	// jtkim 20160106
	int nUphCnt;

	// jtkim 20160111
	int nSocketStart;
	int nSocketOff;
	int nSocketOn;

	double dDailyPer;
	double dHourPer;

	int nHourSocket;
};
extern struct tagCOUNT_INFO st_count_info;


struct tagWORKER_INFO
{
	CString strWorker_Name;
	CString strWorker_Crem;
	CString strWorker_Num;
};
extern struct tagWORKER_INFO st_worker_info;

struct tagPART_INFO
{
	CString strPartNo[30];
	CString strType[30];
	CString strDate[30];

	int nDataCnt;
};
extern struct tagPART_INFO st_part_info;

struct tagTYPE_INFO
{
	CString strType[30];
	CString strDate[30];

	int nDataCnt;
};
extern struct tagTYPE_INFO st_type_info;

struct tagLOT_DISPLAY_INFO
{
	CString strLotNo;
	CString strPartNo;
	CString strLotState;
	CString strLotType;
	CString strMainState;
	CString strProCid;
	CString strStep;
	CString strComplot;
	CString strLine;
	CString strScrapSerial[1000];
	CString strScrapCode[1000];
	CString strArray[1000];
	
	int nQty;
	int nScrapCnt;
};
extern struct tagLOT_DISPLAY_INFO st_lot_display_info;

struct tagLOT_HISTORY_INFO
{
	CString strLotNo;
	CString strPartNo;
	int		nTotalCnt;
	int		nPassCnt;
};
extern struct tagLOT_HISTORY_INFO st_lot_history_info;


enum ANI_PART_NUM
{
	ANI_LF_TSITE		=	0,
	ANI_RI_TSITE		,
	ANI_CVY_ASSY		,
	ANI_LD_BUFF			,
	ANI_LD_RBT_X		,
	ANI_LD_RBT_Y		,
	ANI_UNLD_BUFF		,
	ANI_UNLD_RBT_X		,
	ANI_UNLD_RBT_Y		,
	ANI_RETEST_BUFF		,
	ANI_REJECT_BUFF		,
	ANI_COK_BUFF		,
	ANI_TSITE_RBT_X		,
	ANI_TSITE_RBT_Y		,
	ANI_LD_CVY_FRONT	,	
	ANI_LD_CVY_REAR		,
	ANI_LD_RAIL_FRONT	,
	ANI_LD_RAIL_REAR	,
	ANI_EMPTY_STACKER	,
	ANI_UNLD_RAIL		,
	ANI_UNLD_ELV		,
	ANI_UNLD_CVY		,
	ANI_MV_CVY			,
};

enum WAIT_TIME
{
	WAIT_PICKER_UP_DN						 = 0,
	WAIT_IN_CONV_STOPPER,
	WAIT_TURN_CONV_STOPPER,
	WAIT_OUT_CONV_STOPPER,
	WAIT_CONV_TURN,
	WAIT_TURN_CONV_MOTOR_CYL,
	WAIT_PICKER_VACCUM,
	WAIT_FEEDER_REPLY,
	WAIT_CONV_IN ,
	WAIT_CONV_OUT,
	WAIT_CONV_REQ,
	MAX_WAIT_TIME,
};

// *************************************************************************
// I/O 동작 대기 시간 및 리미트 시간 저장 구조체 선언                            
// *************************************************************************
struct tagWAIT_INFO
{
	double dOnWaitTime[MAX_WAIT_TIME];
	double dOffWaitTime[MAX_WAIT_TIME];
	double dLimitWaitTime[MAX_WAIT_TIME];

	int nGripOnOffWaitTime;
	int nPickUpDnWaitTime;
};
extern  tagWAIT_INFO  st_wait_info;




struct tagSCRAP_CODE
{
	int		m_nScrapCode[10000][2]; //scrap code data.....
	int		m_nScrapBin;			// scrap code bin......
};
extern tagSCRAP_CODE st_code_info[2];


//2015.1005
struct tagdbtime_database
{
	int				mndb;

	COleDateTime	mole_date;
	char            str_slot[100];

	CTimeSpan		mn_Ref_time[TSITE_SOCKET_CNT];
	CTimeSpan		mn_run_time[TSITE_SOCKET_CNT];
	CTimeSpan		mn_test_time[TSITE_SOCKET_CNT];
	float			mf_Run[TSITE_SOCKET_CNT];
	float           mf_test[TSITE_SOCKET_CNT];
	CTimeSpan       mn_lt_Rundown[TSITE_SOCKET_CNT];
	CTimeSpan       mn_lt_Socketoff[TSITE_SOCKET_CNT];
	CTimeSpan       mn_lt_instandby[TSITE_SOCKET_CNT];
	CTimeSpan       mn_lt_outReady[TSITE_SOCKET_CNT];
	CTimeSpan       mn_lt_stop[TSITE_SOCKET_CNT];
	CTimeSpan       mn_lt_User_stop[TSITE_SOCKET_CNT];

	float           mf_lr_Rd[TSITE_SOCKET_CNT];
	float           mf_lr_SocketOff[TSITE_SOCKET_CNT];
	float           mf_lr_instandby[TSITE_SOCKET_CNT];
	float           mf_lr_outReady[TSITE_SOCKET_CNT];
	float           mf_lr_stop[TSITE_SOCKET_CNT];
	float           mf_lr_userstop[TSITE_SOCKET_CNT];

	int             mn_Output[TSITE_SOCKET_CNT];
	double          md_Yield[TSITE_SOCKET_CNT];	
	int				mn_Pass[TSITE_SOCKET_CNT];
	int				mn_Fail[TSITE_SOCKET_CNT];
	int				mn_Input[TSITE_SOCKET_CNT];

};
extern struct tagdbtime_database	st_dbtime;

struct tagDB_time_param 
{
	CTimeSpan		n_Db_time[TSITE_SOCKET_CNT][TSITE_SOCKET_CNT];
	int				mn_Pass[TSITE_SOCKET_CNT];
	int				mn_Fail[TSITE_SOCKET_CNT];
	int				mn_Input[TSITE_SOCKET_CNT];
	int             mn_Output[TSITE_SOCKET_CNT];
	float           mn_Yield[TSITE_SOCKET_CNT];
};
extern  tagDB_time_param  st_DB_time;

struct tagBuffer_DB_database
{
	int				mndb;

	COleDateTime	mole_date;
	char            str_slot[100];

	CTimeSpan		mn_Ref_time[48];
	CTimeSpan		mn_run_time[48];
	CTimeSpan		mn_test_time[48];
	float			mf_Run[48];
	float           mf_test[48];
	CTimeSpan       mn_lt_Rundown[48];
	CTimeSpan       mn_lt_Socketoff[48];
	CTimeSpan       mn_lt_instandby[48];
	CTimeSpan       mn_lt_outReady[48];
	CTimeSpan       mn_lt_stop[48];
	CTimeSpan       mn_lt_User_stop[48];

	float           mf_lr_Rd[20];
	float           mf_lr_SocketOff[48];
	float           mf_lr_instandby[48];
	float           mf_lr_outReady[48];
	float           mf_lr_stop[48];
	float           mf_lr_userstop[48];

	int             mn_Output[48];
	double          md_Yield[48];
	int				mn_Pass[48];
	int				mn_Fail[48];
	int				mn_Input[48];

};
extern struct tagBuffer_DB_database	st_BufferTemp;

struct tagdbTimeTmp_database
{
	char            str_startDate[100];
	char            str_EndDate[100];
	char            str_slot[100];

	CTimeSpan		mn_Ref_time;
	CTimeSpan		mn_run_time;
	CTimeSpan		mn_test_time;
	float			mf_Run;
	float           mf_test;
	CTimeSpan       mn_lt_Rundown;
	CTimeSpan       mn_lt_Socketoff;
	CTimeSpan       mn_lt_instandby;
	CTimeSpan       mn_lt_outReady;
	CTimeSpan       mn_lt_stop;
	CTimeSpan       mn_lt_User_stop;

	float           mf_lr_Rd;
	float           mf_lr_SocketOff;
	float           mf_lr_instandby;
	float           mf_lr_outReady;
	float           mf_lr_stop;
	float           mf_lr_userstop;

	int             mn_Output;
	int             mn_Pass;
	double          md_Yield;
	int             mn_Cnt;
	int             mn_Interval;

};
extern struct tagdbTimeTmp_database	*st_dbTimeTmp;


struct st_barcode_param 
{
	int nApplyButton[5];
	int mn_labelset_sd[4];
	int mn_labelset_lt[4];
	int mn_labelset_lhx[4];
	int mn_labelset_lhy[4];
	int mn_Prt_Mode[4];
	int mn_Prt_complete_use[4];

	bool b_print_complete[MAX_PORT];
	int n_barcode_hs_cnt;
	int n_barcode_hs_paper_out_error[MAX_PORT];
	int n_barcode_hs_pause_error[MAX_PORT];
	int n_barcode_hs_head_up_error[MAX_PORT];
	int n_barcode_hs_ribbon_out_error[MAX_PORT];
	int n_barcode_hs_print_mode[MAX_PORT];

	CString new_barcode_info[2];//2014,1125 ysj

} ;
extern  st_barcode_param  st_barcode;
//////////////



enum EQP_MCMD
{
	EQP_OFF_LINE					= 1,
	EQP_ON_LINE						= 4,
};

enum MES_MCMD
{
	MES_OFF_LINE					= 0,
	MES_ON_LINE_LOCAL				= 4,
	MES_ON_LINE_REMOTE				= 5,
};


enum LINK_STATUS
{
	LINK_DISCONNECT					= 0,
	LINK_CONNECT					= 1,
};


enum PROC_STATE
{
	PROC_UNKNOWN					= 0,
	PROC_IDLE						,
	PROC_RUN						,
	PROC_DOWN						,
	PROC_MAINT						,
};

enum  THREAD_PART
{
	PART_MGZ_ELV					= 0,
	PART_PCB_SHUTTLE				,
	PART_PCB_ROBOT					,
	PART_JIG_ROBOT					,
	PART_JIG_ELV					,
	PART_JIG_MOVE_CVY				,
	PART_JIG_SPRT_CVY				,
	PART_MGZ_IN_CVY					,
	PART_MGZ_OUT_CVY				,
	PART_REJECT_TH					,
};


enum EQ_WITH_MODE_STATE
{
	WITHOUT_DVC						= 0,
	WITH_DVC						= 1,
	WITHOUT_TRAY					= 3,
};


enum EQ_WORK_MODE_STATE
{
	WORK_BYPASS						= 0,
	WORK_JIG						= 1,
	WORK_MAGAZINE					= 2,
};

#define DN							0
#define UP							1

#define MOVE_FORWARD				0
#define MOVE_BACKWARD				1

///////////////////////////////////////////////////////
//kwlee
enum CONV_WAIT_TIME
{
	WAIT_CONV_STOPPER_UP_DN						 = 0,
	WAIT_CYL_FOR_BACK						,
	//MAX_WAIT_TIME							,
};


enum LABEL_FEEDER_1_POS
{
	LABEL_FEEDER_1_SAFETY		  = 0,
	LABEL_FEEDER_1_WORK		, 
	 
};

enum PITCH_UP_HEAD_PICK_POS
{
	PITCH_UP_HEAD_SAFETY		  = 0,
	PITCH_UP_HEAD_PICK_FEEDER		, 
	PITCH_UP_HEAD_PLACE_PCB		, 
};

enum PITCH_UP_HEAD_TURN_POS
{
	PITCH_UP_HEAD_TURN_ROTATOR_SAFETY  = 0,
	PITCH_UP_HEAD_TURN_ROTATOR_0			,	
	PITCH_UP_HEAD_TURN_ROTATOR_L_90			,
	PITCH_UP_HEAD_TURN_ROTATOR_R_90			,
};

enum ROBOT_POS
{
	ROBOT_SAFETY = 0,
	ROBOT_FEEDER_PICK_FIRST		, 
	ROBOT_PLACE_FIRST		, 
	ROBOT_REJECT_PLACE,
};
enum ROBOT_X_POS
{
	ROBOT_X_SAFETY		  = 0,
	ROBOT_X_FEEDER_PICK_FIRST		, 
	ROBOT_X_PLACE_FIRST		, 
	ROBOT_X_PLACE_SECOND,
	ROBOT_X_FEEDER_PICK_END,
	ROBOT_X_PLACE_THIRD,
	ROBOT_X_PLACE_FORTH,
	ROBOT_X_PLACE_REJECT, //kwlee 2017.0204
};

enum ROBOT_Y_POS
{
	ROBOT_Y_SAFETY		  = 0,
	ROBOT_Y_FEEDER_PICK_FIRST		, 
	ROBOT_Y_PLACE_FIRST		, 
	ROBOT_Y_PLACE_SECOND,
	ROBOT_Y_FEEDER_PICK_END,
	ROBOT_Y_PLACE_THIRD,
	ROBOT_Y_PLACE_FORTH,
	ROBOT_Y_PLACE_REJECT, //kwlee 2017.0204
};

enum ROBOT_Z_POS
{
	ROBOT_Z_SAFETY		  = 0,
	ROBOT_Z_FEEDER_PICK		, 
	ROBOT_Z_PLACE		,  
	ROBOT_Z_REJECT_PLACE,
};

enum TURN_CONVEYOR_REVERSE_POS
{
	TURN_CONVEYOR_REVERSE_SAFETY		  = 0,
	TURN_CONVEYOR_WORK			, 
	
};

enum LABEL_FEEDER_2_POS
{
	LABEL_FEEDER_2_SAFETY		  = 0,
	LABEL_FEEDER_2_P_LIMIT		, 
	LABEL_FEEDER_2_M_LIMIT		, 
};
//kwlee
////////////////////////////////////////////////////////////////////////////
enum MOTOR_NUM
{
	M_PICKERRBT_Y					= 0,
	M_PICKERRBT_X,
	M_PICKERRBT_Z,
	M_TURN_CONV_PITCH,
	M_PICKERRBT_PITCH,
	M_LABEL_PITCH,
	M_PICKERRBT_TURN,
	M_MAX_MOTOR_NUM					, 
};






enum RFID_POS
{
	RF_CARRIER_ID					= 0,
	RF_EQP_ID						,
	RF_QTY							,
	RF_QTY_T						,
	RF_LOT_ID_J						, 
	RF_STEP							,
	RF_LOT_TYPE						,
	RF_PART_ID						,
	RF_PKG_CODE						,
	RF_NONE_1						,
	RF_LOT_ID_M						,
	RF_CNT							,
	RF_FLAG							,
	RF_REJECT						,
	RF_BIN							,
	RF_NONE_2						,
	RF_TRAY_QTY						,
};

enum COMM_PORT
{
	LD_BCR_PORT						= 0,
	UNLD_BCR_A_PORT					,
	UNLD_BCR_B_PORT					,
};

#define WAIT_CONNECT					5000
#define WAIT_RCV						30000
#define WAIT_CLOSE						300

enum NETWORK_NUM
{
	EC_FIRST_NETWORK				= 0,
	XGEM_NETWORK					,
	INTERFACE_NETWORK				,
	FRONT_NETWORK					,
	REAR_NETWORK					,
	RFID_NETWORK					,
	FTP_NETWORK						,
	GMS_NETWORK						,
	EC_SECOND_NETWORK				,
	PRINTER_NETWORK ,
};

//2015.1005
enum ENUM_FILLRATIO
{

	REF_TIME,
	RUN_TIME,
	TEST_TIME,
	RATIO_RUN,
	RATIO_TEST,
	RUN_DOWN,
	SOCKETOFF,
	INSTANBY,
	OUTREADY,
	STOP,
	USERSTOP,
	OUTPUT,
	YIELD,		
};
///////////////////

#define SLOT_DISABLE				0
#define SLOT_ENABLE					1


#endif