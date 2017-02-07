/*******************************************************************************
* [ CmmsdkDef.h ]
* Header file for definitions of constants and data in CMMSDK library
* - Update Data: 2007/01/12
* - Provider: COMIZOA Co., Ltd.
* - Phone: +82-42-936-6500~6
* - Fax  : +82-42-936-6507
* - URL  : http://www.comizoa.co.kr,  http://www.comizoa.com
********************************************************************************/

#ifndef _CMMSDK_DEF_H_
#define _CMMSDK_DEF_H_

// Device ID definition //
#define COMI_SD401	0xB401
#define COMI_SD402	0xB402 
#define COMI_SD403	0xB403 
#define COMI_SD404	0xB404 
#define COMI_SD414	0xB414 
#define COMI_SD424	0xB424 
#define COMI_LX501	0xA501
#define COMI_LX502	0xA502
#define COMI_LX504	0xA504
#define COMI_LX508	0xA508
#define COMI_LX534	0xA534
#define COMI_LX544	0xA544
#define COMI_LX504A	0xA544

// Definition of maximum number of things //
#define CMM_MAX_NUM_MOT_DEV			16 // Maximum number of Motion Devices in one PC
#define CMM_MAX_NUM_DIO_DEV			16 // Maximum number of Digital I/O Devices in one PC
#define CMM_MAX_NUM_AXES			64 // Maximum number of motion axes
#define CMM_MAX_DI_CH				512 // Maximum number of D/I channels
#define CMM_MAX_DO_CH				512 // Maximum number of D/O channels
#define CMM_MAX_STR_LEN_AXIS_TITLE	30 // Maximum string length of axis title
#define CMM_MAX_STR_LEN_DIST_UNIT	30 // Maximum string length of distance unit
#define CMM_MAX_STR_LEN_VEL_UNIT	30 // Maximum string length of velocity unit
#define CMM_MAX_STR_LEN_ERR			128  // Maximum error string length: a buffer to receive error string must be larger than this size.


#ifndef _PCL_REGISTER_DEFINED_
#define _PCL_REGISTER_DEFINED_

// Motion Chip Main-space Address //
typedef enum {COMW, OTPW, BUF0, BUF1}TMCWAddr;
typedef enum {MSTSW, SSTSW}TMCRAddr;

// Motion Chip Registers //
enum {	
	PRMV, PRFL, PRFH, PRUR, PRDR, PRMG, PRDP, PRMD, PRIP, PRUS, PRDS, PRCP5, PRCI, 
	RMV=16, RFL, RFH, RUR, RDR, RMG, RDP, RMD, RIP, RUS, RDS, RFA, RENV1, RENV2, RENV3, RENV4, RENV5, RENV6, RENV7, 
	RCUN1=35, RCUN2, RCUN3, RCUN4, RCMP1, RCMP2, RCMP3, RCMP4, RCMP5, RIRQ, RLTC1, RLTC2, RLTC3, RLTC4, 
	RSTS=49, REST, RIST, RPLS, RSPD, PSDC, 
	RCI=60, RCIC, 
	RIPS=63
};

#endif // _PCL_REGISTER_DEFINED_

//***********************************************************************************************
//								CONSTATNS for User												*
//***********************************************************************************************

#ifndef OFF
 #define OFF	0
#endif

#ifndef ON
 #define ON		1
#endif

// Boolean type definition //
typedef enum _TCmBool{ 
	cmFALSE, cmTRUE
}TCmBool;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MIO Property ID 
// <TCmMioPropId> �� <TCmMioPropIdEx> �� ������ MIO Property ID�� ��Ÿ���� �����Դϴ�. <TCmMioPropIdEx> �������� 
// <TCmMioPropId> ���� �̸��� �ٲپ���� ���Դϴ�. ���� �� ���� ���� �߿��� �ƹ��ų� ����ص� �˴ϴ�.
typedef enum _TCmMioPropId{
	cmALM_LOGIC,	cmALM_MODE,		cmCMP_LOGIC,	cmDR_LOGIC, cmEL_LOGIC,		cmEL_MODE, 
	cmERC_LOGIC,	cmERC_OUT,		cmEZ_LOGIC,		cmINP_EN,	cmINP_LOGIC,	cmLTC_LOGIC, 
	cmLTC_LTC2SRC,	cmORG_LOGIC,	cmSD_EN,		cmSD_LOGIC, cmSD_LATCH,		cmSD_MODE, 
	cmSTA_MODE, 	cmSTA_TRG,		cmSTP_MODE,		cmCLR_CNTR, cmCLR_SIGTYPE,	cmCMP_PWIDTH, 
	cmERC_ONTIME,	cmSVON_LOGIC,	cmERC_OUT_EL,	cmCNT_D_SRC,	cmCNT_G_SRC
}TCmMioPropId;

typedef enum _TCmMioPropIdEx{
	cmMPID_ALM_LOGIC,	cmMPID_ALM_MODE,	cmMPID_CMP_LOGIC,	cmMPID_DR_LOGIC,	cmMPID_EL_LOGIC,	cmMPID_EL_MODE, 
	cmMPID_ERC_LOGIC,	cmMPID_ERC_OUT,		cmMPID_EZ_LOGIC,	cmMPID_INP_EN,		cmMPID_INP_LOGIC,	cmMPID_LTC_LOGIC, 
	cmMPID_LTC_LTC2SRC,	cmMPID_ORG_LOGIC,	cmMPID_SD_EN,		cmMPID_SD_LOGIC,	cmMPID_SD_LATCH,	cmMPID_SD_MODE, 
	cmMPID_STA_MODE,	cmMPID_STA_TRG,		cmMPID_STP_MODE,	cmMPID_CLR_CNTR,	cmMPID_CLR_SIGTYPE,	cmMPID_CMP_PWIDTH, 
	cmMPID_ERC_ONTIME,	cmMPID_SVON_LOGIC,	cmMPID_ERC_OUT_EL,	cmMPID_CNT_D_SRC,	cmMPID_CNT_G_SRC,	cmMPID_HOME_ESC_DIS,
	cmMPID_LTC_TRGSRC,	cmMPID_EN_ARDP
}TCmMioPropIdEx;

// Bit order of StReadMioStatuses() return value  //
typedef enum _TCmMioState{
	cmIOST_RDY,	cmIOST_ALM, cmIOST_ELP,		cmIOST_ELN,
	cmIOST_ORG, cmIOST_DIR, cmIOST_RSV1,	cmIOST_PCS, 
	cmIOST_ERC, cmIOST_EZ,	cmIOST_CLR,		cmIOST_LTC,	
	cmIOST_SD,	cmIOST_INP, cmIOST_DRP,		cmIOST_DRN, 
	cmIOST_STA, cmIOST_STP, cmIOST_SVON
}TCmMioState;

// Motion operation status ID //
typedef enum _TCmMotionState{ 
	cmMST_STOP, cmMST_WAIT_DR, cmMST_WAIT_STA, cmMST_WAIT_INSYNC,
	cmMST_WAIT_OTHER, cmMST_WAIT_ERC, cmMST_WAIT_DIR, cmMST_RESERVED1, cmMST_WAIT_PLSR,
	cmMST_IN_RVSSPD, cmMST_IN_INISPD, cmMST_IN_ACC, cmMST_IN_WORKSPD, cmMST_IN_DEC,
	cmMST_WAIT_INP, cmMST_SPARE0
}TCmMotionState;

// Signal logic definition //
typedef enum _TCmSigLogic{ 
	cmLOGIC_A=0 /*Normal open*/, cmLOGIC_B=1/*Normal close*/ 
}TCmSigLogic;

// Axis index definition //
typedef enum _TCmAxis{ 
	cmX1, cmY1, cmZ1, cmU1, cmX2, cmY2, cmZ2, cmU2 
}TCmAxis;

// Definition for axes mask  //
typedef enum _TCmAxisMask{ 
	cmX1_MASK=0x1, cmY1_MASK=0x2, cmZ1_MASK=0x4, cmU1_MASK=0x8,
	cmX2_MASK=0x10, cmY2_MASK=0x20, cmZ2_MASK=0x40, cmU2_MASK=0x80
}TCmAxisMask;

// Encoder and PA/PB input mode definition //
typedef enum _TCmInMode{ 
	cmIMODE_AB1X, cmIMODE_AB2X, cmIMODE_AB4X, cmIMODE_CWCCW, cmIMODE_STEP
}TCmInMode;

// Command output mode definition //
typedef enum _TCmOutMode{ 
	cmOMODE_PDIR0, cmOMODE_PDIR1, cmOMODE_PDIR2, cmOMODE_PDIR3,
	cmOMODE_CWCCW0, cmOMODE_CWCCW1, cmOMODE_CCWCW0, cmOMODE_CCWCW1, 
	cmOMODE_AB1, cmOMODE_AB2
}TCmOutMode;

// Control Mode //
typedef enum _TCmCtrlMode{ 
	cmCTRL_OPEN, // Open loop control mode
	cmCTRL_SEMI_C, // Semi-closed loop control mode (applied only to absolute in-position commands)
	cmCTRL_FULL_C // Full-closed loop control mode (this is not supported at current version)
}TCmCtrlMode;
			
// (Linear)Operation direction //	
typedef enum _TCmDir{
	cmDIR_N /*(-)Dir*/, cmDIR_P /*(+)Dir*/
}TCmDir;

// Counter name //
typedef enum _TCmCntr { 
	cmCNT_COMM/*Command*/, cmCNT_FEED/*Feedback*/, cmCNT_DEV/*Deviation*/, 
	cmCNT_GEN/*General*/, cmCNT_REM/*Remained*/
}TCmCntr;

// Speed mode index definition //	
typedef enum _TCmSpeedMode{ 
	cmSMODE_KEEP=-1/* Keep previous setting*/, cmSMODE_C=0 /*Constant */, cmSMODE_T /*Trapeziodal*/, cmSMODE_S /*S-curve*/
}TCmSpeedMode;

// Arc operation direction //
typedef enum _TCmArcDir{ 
	cmARC_CW, cmARC_CCW
}TCmArcDir;

// Compare Method //
typedef enum _TCmCmpMethod{
	cmDISABLE, cmEQ_BIDIR, cmEQ_PDIR, cmEQ_NDIR, cmLESS/*Cnt<Data*/, cmGREATER/*Cnt>Data*/
}TCmCmpMethod;

// Action when general comparator met the condition //
typedef enum _TCmCmpAction{
	cmEVNT_ONLY, cmEVNT_IS, cmEVNT_DS, cmEVNT_SPDCHG
}TCmCmpAction;

// Backlash/Slip correction mode //
typedef enum _TCmCorrMode{
	cmCORR_DIS, // Disable correction 
	cmCORR_BACK, // Backlash correction mode 
	cmCORR_SLIP // Slip correction mode
}TCmCorrMode;

typedef enum _TCmExtOptionId{
	cmEXOPT_SET_USE_PREREG
}TCmExtOptionId;

// Interrupt Handler Type //
typedef enum _TCmIntHandlerType{
	cmIHT_MESSAGE=0, cmIHT_EVENT, cmIHT_CALLBACK
}TCmIntHandlerType;

// Interrupt Handler Type //
typedef enum _TCmStringID{
	cmSTR_AXIS_NAME, cmSTR_DIST_UNIT, cmSTR_SPEED_UNIT
}TCmStringID;

// Sequence Mode //
typedef enum _TCmSeqMode{
	cmSEQM_SKIP_RUN, cmSEQM_WAIT_RUN
}TCmSeqMode;

// Map Type //
typedef enum _TCmDevMapType{
	cmDMAP_MOTION, cmDMAP_DIO, cmDMAP_ALL
}TCmDevMapType;

// SetFilterAB�� ��� //
typedef enum _TCmABFilter{
	cmAB_ENC, cmAB_PULSAR
}TCmABFilter;

// Axis Capability ID //
typedef enum _TCmAxisCapID{
	cmCAPX_CMD_DIR=0, /* Command direction change function */
	cmCAPX_EL_MAN_SET, /* -/+EL Manual control function */
	cmCAPX_CMP_HIGH /* High-speed Compare Output function */
}TCmAxisCapID;

typedef enum _TCmHomePosClrMode{
	cmHPCM_M0, // ORG(/EL/EZ) ��ȣ�� �߻��� �� COMMAND & FEEDBACK ��ġ�� 0���� Ŭ�����Ѵ�.
	cmHPCM_M1, // �������͸� ��� �Ϸ��ϰ� ���� COMMAND & FEEDBACK ��ġ�� ��� 0���� Ŭ�����Ѵ�.
	cmHPCM_M2,  // �������͸� ��� �Ϸ��ϰ� ���� FEEDBACK ��ġ�� �״�� �ΰ� COMMAND ��ġ�� FEEDBACK ��ġ�� ��ġ��Ų��.
	cmHPCM_KEEP // �������͸� ��� �Ϸ��ϴ��� COMMAND & FEEDBACK ��ġ�� Ŭ�������� �ʴ´�.
}TCmHomePosClrMode;


// Sync mode //
typedef enum _TCmSyncMode{
	cmSYNC_DISABLE,
	cmSYNC_INT_SYNC,
	cmSYNC_OTHER_STOP
}TCmSyncMode;

// Internal sync. conditions //
typedef enum _TCmIntSyncCond{
	cmISYNC_ACC_STA, // 0: at start of acceleration
	cmISYNC_ACC_END, // 1: at end of acceleration
	cmISYNC_DEC_STA, // 2: at start of deceleration
	cmISYNC_DEC_END, // 3: at end of deceleration
	cmISYNC_SLN, // 4: when (-)software limit met
	cmISYNC_SLP, // 5: when (+)software limit met
	cmISYNC_GCMP, // 6: when General Comparator condition is satisfied
	cmISYNC_TCMP, // 7: when Trigger Comparator condition is satisfied
}TCmIntSyncCond;

////////////////////////////////////////////////////////////////////////////////////////////
// TCmLmxSeqMode: �̼۸���� �����Ϸ� �ϴµ� LMX Buffer�� �̹� �� ���ִ� ��쿡 ��� ó��
// ������ ���� ����� ���̵�.
// -. cmLMX_SEQM_SKIP_RUN: 'cmERR_LMX_ADD_ITEM_FAIL'������ �߻��ϰ� �Լ��� �ٷ� ��ȯ�ȴ�.
// -. cmLMX_SEQM_WAIT_RUN: LMX ���ۿ� free space�� ���� ������ ����ϰ� �ִٰ� free space��
//    ����� �����ϰ� �Լ��� ��ȯ�ȴ�.
typedef enum _TCmLmxSeqMode{
	cmLMX_SEQM_SKIP_RUN, 
	cmLMX_SEQM_WAIT_RUN
}TCmLmxSeqMode;


////////////////////////////////////////////////////////////////////////////////////////////
// TCmLmxStsId: cmmLmxGetSts() �Լ��� ���ؼ� �о�� status ID ���� ����
typedef enum _TCmLmxStsId{
	cmLMX_STARTED, // Lmx ����� Ȱ��ȭ �Ǿ������� ��Ÿ���� status 
	cmLMX_BUSY, // Lmx�� ���� ������ �̼��� ���� �������� ��Ÿ���� status
	cmLMX_FREE_SPACE, // Lmx ������ ���� ����. ��ȯ�Ǵ� ���� ����Ʈ ������ �ƴ϶� ����� �� �ִ� ������ ���̴�.
	cmLMX_RUN_ITEM_NO, // ���� �̼۵ǰ� �ְų� ������ �̼۵� Item�� ��ȣ.
	cmLMX_RUN_ITEM_ID, // ���� �̼۵ǰ� �ְų� ������ �̼۵� Item�� ���̵�(���̵�� cmmLmxSetNextItemId() �Լ��� ���ؼ� �����Ѵ�)
	cmLMX_LAST_SET_ITEM_ID, // ���������� ����� Item�� ���̵�.
	//cmLMX_LAST_SET_ITEM_NO  // ���������� ����� Item�� ���̵�[���Ŀ� �߰�����].
}TCmLmxStsId;

////////////////////////////////////////////////////////////////////////////////////////////
// Debug-log level ID
typedef enum _TCmLogLevel{
	cmLOG_LEVEL_DISABLED, // disable logging
	cmLOG_LEVEL_ERROR,	// log only when error occurs
	cmLOG_LEVEL_COMM,	// log command functions and error (including error occurs)
	cmLOG_LEVEL_GET,	// log get functions, command functions and error (except cmmSxIsDone(), cmmStGetPostion(), ...)
	cmLOG_LEVEL_ALL		// log all functions
}TCmDbgLogLevel;

////////////////////////////////////////////////////////////////////////////////////////////
// Debug-log level ID
typedef enum _TCmManLogMode{
	cmMAN_LOG_DISABLED, // no effect on manual log action
	cmMAN_LOG_SKIP,		// skip logging until cmmDlogExitManCtl() is called
	cmMAN_LOG_ADD		// log any functions regardless of log level until cmmDlogExitManCtl() is called.
	                    // But, cmMAN_LOG_ADD has no effect if log level is 0 (disabled)
}TCmManLogMode;



typedef struct {
	double fCmndPos, fFeedPos;
	double fCmndVel, fFeedVel;
	int	nMst;
	UINT dwMio;
	UINT dwReserved[2];
}TAxisMstPack;

typedef struct{
	UINT nScanCount;
	TAxisMstPack Axis[64];
}TCmMstAll;


//***********************************************************************************************
//								FUNCTION Prototype												*
//***********************************************************************************************

// Interrupt Handler Callback Function Prototype //
typedef void (WINAPI *TIntHandler) (LPVOID lParam); 


//***********************************************************************************************
//								Structures														*
//***********************************************************************************************

//------------------- Motion Device Informations -------------------------------------------------------//

// Informations of a motion device //
typedef struct{
	USHORT wDeviceId, wDevInstance;
	short wSlot;
	USHORT wNumAxes, wIniAxis;
	USHORT wDiNum, wDoNum;
	void*  pDevClass; // �� ������ ���̺귯�� ���ο����� ����ϴ� �����̴�.
}TMotDevInfo;

// Informations of all actually installed motion devices //
typedef struct{
	USHORT wNumDevs; // Number of installed motion devices
	TMotDevInfo Dev[CMM_MAX_NUM_MOT_DEV];
}TMotDevEnum;

// Informations of motion devices defined by user //
typedef struct{
	USHORT wNumDevs;
	TMotDevInfo Dev[CMM_MAX_NUM_MOT_DEV];
}TMotDevMap;

//------------------- DIO Device Informations -----------------------------------------------------------//

// Informations of a DIO device //
typedef struct{
	USHORT	wDeviceId, wDevInstance;
	short 	wSlot;
	USHORT	wDiNum, wDiIniChan;
	USHORT	wDoNum, wDoIniChan;
	BOOL	bMotionDevice; 
	HANDLE	Handle; // �����ġ�ΰ�쿡�� CMotionDevice Ŭ���� ������, SD4xx ��ġ�� ��쿡�� Device Handle
}TDioDevInfo;

// Informations of all actually installed DIO devices //
typedef struct{
	USHORT wNumDevs;
	TDioDevInfo Dev[CMM_MAX_NUM_DIO_DEV];
}TDioDevEnum;

// Informations of DIO devices defined by user//
typedef struct{
	USHORT wNumDevs, wNumAllDiChan, wNumAllDoChan;
	TDioDevInfo Dev[CMM_MAX_NUM_DIO_DEV];
}TDioDevMap;

//-------------------- MOTION CONFIGURATION ------------------------------------------------------------//
typedef union{
	struct{
		// B0 ~ B3 //
		ULONG ALM_Logic: 1;		
		ULONG ALM_Mode : 1;
		ULONG CMP_Logic: 1;
		ULONG DR_Logic: 1;
		// B4 ~ B7 //
		ULONG EL_Logic: 1;
		ULONG EL_Mode: 1;
		ULONG ERC_Logic: 1;
		ULONG ERC_OutAtHome: 1;
		// B8 ~ B11 //
		ULONG EZ_Logic: 1;
		ULONG INP_Enable: 1;
		ULONG INP_Logic: 1;
		ULONG LTC_Logic: 1;
		// B12 ~ B15 //
		ULONG LTC_Ltc2Src: 1;
		ULONG ORG_Logic: 1;
		ULONG SD_Enable: 1;
		ULONG SD_Logic: 1;
		// B16 ~ B19 //
		ULONG SD_Latch: 1;
		ULONG SD_Mode: 1;
		ULONG STA_Mode: 1;
		ULONG STA_TrgType: 1;
		// B20 ~ B21 //
		ULONG STP_Mode_Old: 1; // ����� ���� �ʴ� ��Ʈ��
		ULONG SVON_Logic: 1;
        ULONG STP_Mode: 2;
		ULONG Reserved: 8;
	}b;
	ULONG v;
}TCmeMio1;

typedef struct {
	short CLR_CntrSel;
	short CLR_SigType;
	long CMP_PulseWidth;
	short ERC_OnTime;
}TCmeMio2;

typedef struct{
	long HomeMode;
	long EzCount;
	double EscDist;
	double Offset;
	long nVMode;
	double fVel, fAcc, fDec, fRevVel;
}TCmeHome;

typedef struct{
	char szAxisName[CMM_MAX_STR_LEN_AXIS_TITLE+1];
	char szDistText[CMM_MAX_STR_LEN_DIST_UNIT+1];
	char szVelText[CMM_MAX_STR_LEN_VEL_UNIT+1];
	TCmeMio1 Mio1;
	TCmeMio2 Mio2;
	double fInOutRatio;
	double fUnitDist, fUnitSpeed, fMaxSpeed;
	// Pulse output mode //
	struct{
		UINT nMode:		4; // Output Mode Index
		UINT bReverse:	2; // Reverse CW/CCW, this is valid when nMode is 4 or 5(two pulse mode).
		UINT Reserved:	26;
	}OutMode;
	// Encoder input mode //
	struct{
		UINT nMode:		4; // Encoder input mode index
		UINT bReverse:	2; // Reverse encoder direction
		UINT bApplyFilter: 1; // Apply Filter to EA/EB input
		UINT Reserved:	25;
	}InMode;
	// Software limit config //
	struct{
		short IsEnable;
		double LimitN, LimitP;
	}SoftLimit;
	// Slip/Backlash correction config //
	struct{
		USHORT Mode, CntrMask;
		double Amount, Vel;
	}Correction;

	// FH Correction config //
	struct{
		BOOL bEnable;
		double Ratio;
	}VworkCorr;

	ULONG dwIntMask; // Event Interrupt Mask

	// Etc. config //
	struct{
		UINT bFilterEn: 1;
		UINT nCtrlMode: 4;
		UINT bSimulMode: 1;
		UINT bFilterPAPB: 1;
		UINT nElnManMode: 2; // 0=>Sensor �Է� ���, 1=>���� OFF, 2=>���� ON
		UINT nElpManMode: 2; // 0=>Sensor �Է� ���, 1=>���� OFF, 2=>���� ON
		UINT Reserved: 21;
	}Etc1;

	TCmeHome Home;
	long nVMode;
	double fVelIni, fVelWork, fAcc, fDec;
	long nHPCM; // Home Position Clear Mode [Added at 2007/01/12]
	DWORD dwReserved[255];
}TCmeAxis;

//------------------------------------------------------------------------------------------------------//
typedef struct{
	struct{
		BOOL bEnable;
		ULONG dwInterval;
	}ActSpdChk;
	BOOL bShowErrorAuto;
	struct{
		SHORT nLevel;
		char szFilePath[MAX_PATH];
	}Dlog;
	DWORD dwReserved[100];
}TCmeGeneral;

//--------------------------------- CME FILE STRUCTURE (ALL) --------------------------------------------//

typedef struct{
	struct{
		UINT Major:	16;
		UINT Minor: 16;
	}Version;
	DWORD dwReserved[100];
}TCmeFileHeader;

typedef struct{
	TMotDevMap DevMap_Mot;
	TDioDevMap DevMap_Dio;
	DWORD dwDiLogics[16], dwDoLogics[16];
	TCmeAxis Axis[64];
	TCmeGeneral Gen;
}TCmeData_V2;

//***********************************************************************************************
//									ERROR CODE DEFINITIONs										*
//***********************************************************************************************

#define cmERR_NONE					0
#define cmERR_MEM_ALLOC_FAIL		-290	// Memory allocation fail
#define cmERR_GLOBAL_MEM_FAIL		-292	// Global memory allocation fail
#define cmERR_ISR_CONNEC_FAIL 		-310	// ISR registration fail
#define cmERR_DIVIDE_BY_ZERO		-400	// Cause divide by zero error 
#define cmERR_WORNG_NUM_DATA		-500	// Number of data is too small or too big
#define cmERR_VER_MISMATCH			-600 	// Version(of file or device) mismatch

#define cmERR_INVALID_DEVICE_ID		-1010 // Invalid device id => Load Device �Ǵ� SetDeviceId()����...
#define cmERR_INVALID_HANDLE		-1020
#define cmERR_UNSUPORTED_FUNC		-1030 
#define cmERR_INVALID_PARAMETER		-1101
#define cmERR_INVALID_CHANNEL		-1105
#define cmERR_INVALID_INPUT_RANGE	-1111 // Invalid range value (AI, AO)
#define cmERR_INVALID_FREQ_RANGE	-1121 // Invalid input or output frequency
#define	cmERR_FILE_CREATE_FAIL		-1501 // File create fail 
#define cmERR_FILE_OPEN_FAIL		-1511 // File open fail
#define cmERR_FILE_READ_FAIL		-1522 // File reading fail
#define cmERR_EVENT_CREATE_FAIL		-1550 // Event handle creation fail			
#define cmERR_INT_INSTANCE_FAIL		-1560 // Interrupt event instance creation fail
#define	cmERR_DITHREAD_CRE			-1570 // D/I state change monitor thread creation fail
#define cmERR_BUFFER_SMALL			-1580 // Buffer size is too small
#define cmERR_HIGH_TIMER_UNSUPP		-1590 // The installed hardware does not support a high-resolution performance counter (cmmUtlDelayMicroSec() function fails)
#define cmERR_OUT_OF_RANGE			-1600 // The range of some parameter is out of range
#define cmERR_INVALID_BUFFER		-1610 // buffer pointer is NULL
#define cmERR_INVALID_BUF_SIZE		-1620 // buffer size if too big
#define cmERR_LTCQ_ERROR_BASE		-1630  
#define cmERR_LTCQ_NULL_BUFFER		-1631 // Queue buffer is not allocated (refer to cmmLtcQue_SetSize() function)
#define cmERR_LTCQ_BUF_EMPTY		-1632 // There's no new latch data in the queue berffer
#define cmERR_LTCQ_INVALID_INDEX	-1633 // 'Index' argument of cmmLtcQue_PeekAt() function is invalid 

#define cmERR_ON_MOTION				-5001
#define cmERR_STOP_BY_SLP			-5002 // Abnormally stopped by positive soft limit
#define cmERR_STOP_BY_SLN			-5003 // Abnormally stopped by negative soft limit
#define cmERR_STOP_BY_CMP3			-5004 // Abnormally stopped by comparator3
#define cmERR_STOP_BY_CMP4			-5005 // Abnormally stopped by comparator4
#define cmERR_STOP_BY_CMP5			-5006 // Abnormally stopped by comparator5
#define cmERR_STOP_BY_ELP			-5007 // Abnormally stopped by (+) external limit
#define cmERR_STOP_BY_ELN			-5008 // Abnormally stopped by (-) external limit
#define cmERR_STOP_BY_ALM			-5009 // Abnormally stopped by alarm input signal
#define	cmERR_STOP_BY_CSTP			-5010 // Abnormally stopped by CSTP input signal
#define cmERR_STOP_BY_CEMG			-5011 // Abnormally stopped by CEMG input signal
#define cmERR_STOP_BY_SD			-5012 // Abnormally stopped by SD input signal
#define cmERR_STOP_BY_DERROR		-5013 // Abnormally stopped by operation data error
#define cmERR_STOP_BY_IP			-5014 // Abnormally stopped by other axis error during interpolation
#define cmERR_STOP_BY_PO			-5015 // An overflow occurred in the PA/PB input buffer
#define cmERR_STOP_BY_AO			-5016 // Out of range position counter during interpolation
#define cmERR_STOP_BY_EE			-5017 // An EA/EB input error occurred (does not stop)
#define cmERR_STOP_BY_PE			-5018 // An PA/PB input error occurred (does not stop)
#define cmERR_STOP_BY_SLVERR		-5019 // Abnormally stopped because slave axis has been stopped
#define cmERR_STOP_BY_SEMG			-5020 // Abnormally stopped by software emergency setting

#define cmERR_MOT_MAOMODE			-5110 // Master output mode is not CW/CCW mode // Master/Slave ���۽ÿ� Master output��尡 CW/CCW��尡 �ƴϴ�.
#define cmERR_MOT_SLAVE_SET			-5120 // Slave start fail (Motion state�� "Wait for Pulsar Input"���� ������ �ʴ´�.
#define cmERR_SPEED_RANGE_OVER		-5130
#define cmERR_INVALID_SPEED_SET		-5140 // Speed setting value is not valid
#define cmERR_INVALID_IXMAP			-5150 // Invalid interpolation map
#define cmERR_INVALID_LMMAP			-5160 // Invalid List-Motion Map 
#define cmERR_MOT_SEQ_SKIPPED		-5170 // Motion command is skipped because the axis is already running.  
#define cmERR_CMPIX_INVALID_MAP		-5180 // Interpolated position compare output map is not valid
#define cmERR_INVALID_ARC_POS		-5190 // Position data for circular interpolation is invalid
#define cmERR_LMX_ADD_ITEM_FAIL		-5200 // failed to add an job item to "extend list motion"
#define cmERR_LMX_IS_NOT_ACTIVE		-5300 // 'Extended ListMotion' is not active.

#define	cmERR_UNKNOWN				-9999

#endif
