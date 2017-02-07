////////////////////////////////////////////////////////
//		FAS-HSSI Program DLL Version 1.0.0.1
////////////////////////////////////////////////////////

#ifdef __FAS_HSSI_DLL
#define FAS_HSSI	extern "C" _declspec(dllexport)
#else
#define FAS_HSSI	extern "C" _declspec(dllimport)
#pragma comment( lib, "HSSI.lib" )
#endif

#define MAX_PCI_SLOT 8	// 0~7 8개
#define MAX_PORT_NO 4	// 0~3 4개
#define MAX_SLAVE_NO 8	// 0~7 8개

#define TYPE_LBYTE	0x2 // Low Byte
#define TYPE_HBYTE	0x3 // High Byte

#define FAS_STATUS	DWORD // Return Value

#define DLL_VERSION	110

// 명령 전송이 이상없는지 확인하는 명령
// 사용법 if(FAS_SUCCESS(result))
#define FAS_SUCCESS(fasStatus)	(((fasStatus) == 0) ? TRUE : FALSE)

///////////////////////////////////////////////////////
//		Error Message for function of GetLastError
///////////////////////////////////////////////////////

enum
{
	STATUS_SUCCESS = 0x0,
	STATUS_ERROR_DLL,			// DLL File Error
	STATUS_ERROR_NO_DEVICE,		// 동작중인 보드가 없을 때
	STATUS_ERROR_READ,			// Read동작 실패
	STATUS_ERROR_WRITE,			// Write동작 실패
	STATUS_ERROR_INITIALIZE,	// 초기화 실패 (Reset포함)
	STATUS_ERROR_COMMAND,		// CMD 사용에 문제있거나
								// 해당 Cmd를 지원하지 않는 Slave Module 시
	STATUS_ERROR_FILE,			// File에 문제있음
	STATUS_ERROR_PORT,			// 포트가 존재하지 않음
	STATUS_ERROR_SLAVE,			// 슬레이브가 ON이 아님
	STATUS_NOTUSE_COMMAND,		// 지원하지 않는 명령임
	STATUS_INVALID_BOARDID,		// 해당 Board가 존재 X
	STATUS_INVALID_PORTNO,		// 해당 Port가 존재 X
	STATUS_INVALID_SLAVEID,		// 해당 Slave가 존재 X,
	STATUS_INVALID_DLL = 0x7fff	// 잘못된 DLL일 경우
};

///////////////////////////////////////////////////////
//		Open 함수에 사용되는 변수
///////////////////////////////////////////////////////

enum
{	
	START_NOTHING = 0x0,	// Digital I/O설정을 건드리지 않고 Open
	START_CLEAR,			// Digital I/O를 모두 Digital Input으로 하고 Slave는 모두 OFF
	START_LOADDATA,			// 이전에 저장되었던 값을 그대로 사용
};

///////////////////////////////////////////////////////
//		통신속도에 사용되는 변수
///////////////////////////////////////////////////////

enum
{
	PORT_SPEED_05 = 0x0, // 0,5 MHz
	PORT_SPEED_10,	// 1 MHz
	PORT_SPEED_20	// 2 MHz
};

///////////////////////////////////////////////////////
//		Slave Tpye을 확인하는 함수에서 리턴되는 변수
///////////////////////////////////////////////////////

enum
{
	FAS_HSSI_SIO16 = 0,
	FAS_HSSI_SI16,
	FAS_HSSI_SO16,
	FAS_HSSI_STI16,
	FAS_HSSI_STO16,
	FAS_HSSI_SPO16
};

///////////////////////////////////////////////////////
//
//		Board Control Function
//
///////////////////////////////////////////////////////

// Master Board와 통신을 시작함 (여러개의 Master Board중에 하나만 연결되도 성공으로 나옴)
FAS_HSSI FAS_STATUS WINAPI HSSI_Open(INT* pCount=NULL, INT nInitialize = START_NOTHING);
// Master Board와 통신을 종료함
FAS_HSSI FAS_STATUS WINAPI HSSI_Close(BOOL bSetInitialize/*=TRUE*/);
// DLL File의 Version을 확인함
FAS_HSSI FAS_STATUS	WINAPI HSSI_GetDLLVersion(LPCTSTR lpVersion, INT nBufferSize);
// 몇장의 Board가 장착되었는지를 확인 (총 몇장의 카드와 통신이 되었는지 확인)
FAS_HSSI FAS_STATUS WINAPI HSSI_GetAttachedMasterBoard(INT* pBoardNo);//
// 해당 Board에 몇개의 Port가 있는지 확인하는 내용
FAS_HSSI FAS_STATUS WINAPI HSSI_GetAttachedPort(INT nMasterID, INT* pnPortNo);
// 해당 Port에 연결되어있는 Slave를 Rertun (BCD)
FAS_HSSI FAS_STATUS WINAPI HSSI_GetAttachedSlaveModule(INT nMasterID, INT nPortNo, INT* pSlaveNo);
// I/O 설정을 Save 및 Load하는 명령
FAS_HSSI FAS_STATUS WINAPI HSSI_SaveConfiguration(LPCTSTR lpszFileName);
FAS_HSSI FAS_STATUS WINAPI HSSI_LoadConfiguration(LPCTSTR lpszFileName);
// Master Board를 초기화 하는 명령
FAS_HSSI FAS_STATUS WINAPI HSSI_ResetBoard(INT nMasterID);

// Port의 통신속도를 조절함 nType : 0 (0.5MHz), 1 (1MHz), 2이상 (2MHz)
FAS_HSSI FAS_STATUS WINAPI HSSI_SetCommSpeed (INT nMasterID, INT nPortNo, INT nType);
FAS_HSSI FAS_STATUS WINAPI HSSI_GetCommSpeed (INT nMasterID, INT nPortNo, INT* nType);
// Slave를 On /Off 시킴 (bEnable : TRUE (On), FALSE (Off)
FAS_HSSI FAS_STATUS WINAPI HSSI_EnableSlaveModule(INT nMasterID, INT nPortNo, INT nSlaveID, BOOL bEnable);
// 연결되어 있는 Slave를 찾아서 ON 시킴
FAS_HSSI FAS_STATUS WINAPI HSSI_PortAutoEnable(INT nMasterID, INT nPortNo);
// Slave Module의 종류를 확인
FAS_HSSI FAS_STATUS WINAPI HSSI_GetSlaveType(INT nMasterID, INT nPortNo, INT nSlaveID, BYTE* btData);
// 지정된 Slave Module의 Tpye에 맞추어 I/O 설정을 변경
FAS_HSSI FAS_STATUS WINAPI HSSI_ApplySlaveType(INT nMasterID, INT nPortNo, INT nSlaveID);


///////////////////////////////////////////////////////
//
//		Search Function
//
///////////////////////////////////////////////////////

// Card가 존재하는지 확인
FAS_HSSI FAS_STATUS WINAPI HSSI_IsExistMasterBoard (INT nMasterID);
// Port가 존재하는지 확인
FAS_HSSI FAS_STATUS WINAPI HSSI_IsExistPort (INT nMasterID, INT nPortNo);
// Slave가 존재하는지 확인
FAS_HSSI FAS_STATUS WINAPI HSSI_IsExistSlaveModule (INT nMasterID, INT nPortNo, INT nSlaveID);

///////////////////////////////////////////////////////
//
//		Data Control Function
//
///////////////////////////////////////////////////////

// 여기서 부터 nType은 상위 Byte인지 하위 Byte인지를 나타냅니다.

// Dout 명령
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadOutputWord (INT nMasterID, INT nPortNo, INT nSlaveID, WORD* pwData);
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadOutputByte (INT nMasterID, INT nPortNo, INT nSlaveID, INT nType, BYTE* pbtData);
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadOutputBit (INT nMasterID, INT nPortNo, INT nSlaveID, INT nBitCount, BOOL* pbData);
FAS_HSSI FAS_STATUS WINAPI HSSI_WriteOutputWord (INT nMasterID, INT nPortNo, INT nSlaveID, WORD wData);
FAS_HSSI FAS_STATUS WINAPI HSSI_WriteOutputByte (INT nMasterID, INT nPortNo, INT nSlaveID, INT nType, BYTE btData);
FAS_HSSI FAS_STATUS WINAPI HSSI_WriteOutputBit (INT nMasterID, INT nPortNo, INT nSlaveID, INT nBitCount, BOOL bData);
// Din 명령
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadInputWord (INT nMasterID, INT nPortNo, INT nSlaveID, WORD* pwData);
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadInputByte (INT nMasterID, INT nPortNo, INT nSlaveID, INT nType, BYTE* pbtData);
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadInputBit (INT nMasterID, INT nPortNo, INT nSlaveID, INT nBitCount, BOOL* pbData);

// Latct 동작 명령
FAS_HSSI FAS_STATUS WINAPI HSSI_SetLatchWord (INT nMasterID, INT nPortNo, INT nSlaveID, WORD wData);
FAS_HSSI FAS_STATUS WINAPI HSSI_SetLatchByte (INT nMasterID, INT nPortNo, INT nSlaveID, INT nType, BYTE btData);
FAS_HSSI FAS_STATUS WINAPI HSSI_SetLatchBit (INT nMasterID, INT nPortNo, INT nSlaveID, INT nBitCount, BOOL bData);
// Latch 확인 명령
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadLatchWord (INT nMasterID, INT nPortNo, INT nSlaveID, WORD* pwData);
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadLatchByte (INT nMasterID, INT nPortNo, INT nSlaveID, INT nType, BYTE* pbtData);
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadLatchBit (INT nMasterID, INT nPortNo, INT nSlaveID, INT nBitCount, BOOL* pbData);
// Latch 확인 명령
FAS_HSSI FAS_STATUS WINAPI HSSI_ResetLatchWord (INT nMasterID, INT nPortNo, INT nSlaveID, WORD wData);
FAS_HSSI FAS_STATUS WINAPI HSSI_ResetLatchByte (INT nMasterID, INT nPortNo, INT nSlaveID, INT nType, BYTE btData);
FAS_HSSI FAS_STATUS WINAPI HSSI_ResetLatchBit (INT nMasterID, INT nPortNo, INT nSlaveID, INT nBitCount, BOOL bData);

// Direction 쓰기 명령
FAS_HSSI FAS_STATUS WINAPI HSSI_WriteDirectionWord (INT nMasterID, INT nPortNo, INT nSlaveID, WORD wData);
FAS_HSSI FAS_STATUS WINAPI HSSI_WriteDirectionByte (INT nMasterID, INT nPortNo, INT nSlaveID, INT nType, BYTE btData);
FAS_HSSI FAS_STATUS WINAPI HSSI_WriteDirectionBit (INT nMasterID, INT nPortNo, INT nSlaveID, INT nBitCount, BOOL bData);
// Direction 확인 명령
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadDirectionWord (INT nMasterID, INT nPortNo, INT nSlaveID, WORD* pwData);
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadDirectionByte (INT nMasterID, INT nPortNo, INT nSlaveID, INT nType, BYTE* pbtData);
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadDirectionBit (INT nMasterID, INT nPortNo, INT nSlaveID, INT nBitCount, BOOL* pbData);
// Direction 설정 명령 (Dout)
FAS_HSSI FAS_STATUS WINAPI HSSI_SetOutWord(INT nMasterID, INT nPortNo, INT nSlaveID, WORD wData);
FAS_HSSI FAS_STATUS WINAPI HSSI_SetOutByte(INT nMasterID, INT nPortNo, INT nSlaveID, INT nType, BYTE btData);
FAS_HSSI FAS_STATUS WINAPI HSSI_SetOutBit(INT nMasterID, INT nPortNo, INT nSlaveID, INT nBitCount, BOOL bData = TRUE);
// Direction 설정 명령 (Din)
FAS_HSSI FAS_STATUS WINAPI HSSI_SetInWord(INT nMasterID, INT nPortNo, INT nSlaveID, WORD wData);
FAS_HSSI FAS_STATUS WINAPI HSSI_SetInByte(INT nMasterID, INT nPortNo, INT nSlaveID, INT nType, BYTE btData);
FAS_HSSI FAS_STATUS WINAPI HSSI_SetInBit(INT nMasterID, INT nPortNo, INT nSlaveID, INT nBitCount, BOOL bData = TRUE);
