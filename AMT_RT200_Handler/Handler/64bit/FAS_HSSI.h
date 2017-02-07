////////////////////////////////////////////////////////
//		FAS-HSSI Program DLL Version 1.2
////////////////////////////////////////////////////////
#pragma once

#ifdef __FAS_HSSI_DLL
#define FAS_HSSI	extern "C" _declspec(dllexport)
#else
#define FAS_HSSI	extern "C" _declspec(dllimport)
#pragma comment( lib, "FAS_HSSI.lib" )
#endif

#define MAX_PCI_SLOT 8	// 0~7 8개
#define MAX_PORT_NO 4	// 0~3 4개
#define MAX_SLAVE_NO 8	// 0~7 8개

#define TYPE_LBYTE	2 // Low Byte
#define TYPE_HBYTE	3 // High Byte

#define FAS_STATUS	DWORD // Return Value

#define DLL_VERSION	120

// 명령 전송이 이상없는지 확인하는 명령
// 사용법 if(FAS_SUCCESS(result))
#define FAS_SUCCESS(fasStatus)	(((fasStatus) == 0) ? TRUE : FALSE)

///////////////////////////////////////////////////////
//		Error Message for function of GetLastError
///////////////////////////////////////////////////////

typedef enum HSSI_RESULT
{	HSSI_STATUS_SUCCESS = 0x0,
	HSSI_STATUS_ERROR_DLL,           // DLL File Error
	HSSI_STATUS_ERROR_NO_DEVICE,     // 동작중인 보드가 없을 때
	HSSI_STATUS_ERROR_READ,          // Read동작 실패
	HSSI_STATUS_ERROR_WRITE,         // Write동작 실패
	HSSI_STATUS_ERROR_INITIALIZE,    // 초기화 실패 (Reset포함)
	HSSI_STATUS_ERROR_COMMAND,       // CMD 사용에 문제있거나
                                     // 해당 Cmd를 지원하지 않는 Slave Module 시
	HSSI_STATUS_ERROR_FILE,          // File에 문제있음
	HSSI_STATUS_ERROR_PORT,          // 포트가 존재하지 않음
	HSSI_STATUS_ERROR_SLAVE,         // 슬레이브가 ON이 아님
	HSSI_STATUS_NOTUSE_COMMAND,      // 지원하지 않는 명령임
	HSSI_STATUS_INVALID_BOARDID,     // 해당 Board가 존재 X
	HSSI_STATUS_INVALID_PORTNO,      // 해당 Port가 존재 X
	HSSI_STATUS_INVALID_SLAVEID,     // 해당 Slave가 존재 X,
	HSSI_STATUS_INVALID_PARAMETER,   // 파라미터 입력 에러
	HSSI_STATUS_INVALID_DLL = 0x7fff // 잘못된 DLL일 경우
} HSSI_RESULT;

///////////////////////////////////////////////////////
//		통신속도에 사용되는 변수
///////////////////////////////////////////////////////

typedef enum PORT_SPEED
{	PORT_SPEED_05 = 0x0, // 0,5 MHz
	PORT_SPEED_10,	// 1 MHz
	PORT_SPEED_20	// 2 MHz
} PORT_SPEED;

///////////////////////////////////////////////////////
//		Slave Tpye을 확인하는 함수에서 리턴되는 변수
///////////////////////////////////////////////////////

typedef enum SLAVE_ID
{	FAS_HSSI_SIO16 = 0,
	FAS_HSSI_SI16,
	FAS_HSSI_SO16,
	FAS_HSSI_STI16,
	FAS_HSSI_STO16,
	FAS_HSSI_SPO16
} SLAVE_ID;

///////////////////////////////////////////////////////
//
//		Board Control Function
//
///////////////////////////////////////////////////////

// Master Board와 통신을 시작함 (여러개의 Master Board중에 하나만 연결되도 성공으로 나옴)
FAS_HSSI FAS_STATUS WINAPI HSSI_Open ( INT* pCount = NULL );
// Master Board와 통신을 종료함
FAS_HSSI FAS_STATUS WINAPI HSSI_Close ( void );
// DLL File의 Version을 확인함
//FAS_HSSI FAS_STATUS	WINAPI HSSI_GetDLLVersion(WCHAR* lpVersion, INT nBufferSize);
// 몇장의 Board가 장착되었는지를 확인 (총 몇장의 카드와 통신이 되었는지 확인)
FAS_HSSI FAS_STATUS WINAPI HSSI_GetAttachedMasterBoard ( INT* pBoardNo );//
// 해당 Board에 몇개의 Port가 있는지 확인하는 내용
FAS_HSSI FAS_STATUS WINAPI HSSI_GetAttachedPort ( const INT nMasterID, INT* pnPortNo );
// 해당 Port에 연결되어있는 Slave를 Rertun (BCD)
FAS_HSSI FAS_STATUS WINAPI HSSI_GetAttachedSlaveModule ( const INT nMasterID, const INT nPortNo, INT* pSlaveNo);
// Master Board를 초기화 하는 명령
FAS_HSSI FAS_STATUS WINAPI HSSI_ResetBoard (INT nMasterID);

// Port의 통신속도를 조절함 nType : 0 (0.5MHz), 1 (1MHz), 2이상 (2MHz)
FAS_HSSI FAS_STATUS WINAPI HSSI_SetCommSpeed ( const INT nMasterID, const INT nPortNo, const INT nType);
FAS_HSSI FAS_STATUS WINAPI HSSI_GetCommSpeed ( const INT nMasterID, const INT nPortNo, INT* nType);
// Slave를 On /Off 시킴 (bEnable : TRUE (On), FALSE (Off)
FAS_HSSI FAS_STATUS WINAPI HSSI_EnableSlaveModule ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const BOOL bEnable);
// 연결되어 있는 Slave를 찾아서 ON 시킴
FAS_HSSI FAS_STATUS WINAPI HSSI_PortAutoEnable ( const INT nMasterID, const INT nPortNo );
// Slave Module의 종류를 확인
FAS_HSSI FAS_STATUS WINAPI HSSI_GetSlaveType( const INT nMasterID, const INT nPortNo, const INT nSlaveID, BYTE* btData);
// 지정된 Slave Module의 Tpye에 맞추어 I/O 설정을 변경
FAS_HSSI FAS_STATUS WINAPI HSSI_ApplySlaveType ( const INT nMasterID, const INT nPortNo, const INT nSlaveID);


///////////////////////////////////////////////////////
//
//		Search Function
//
///////////////////////////////////////////////////////

// Card가 존재하는지 확인
FAS_HSSI FAS_STATUS WINAPI HSSI_IsExistMasterBoard ( const INT nMasterID );
// Port가 존재하는지 확인
FAS_HSSI FAS_STATUS WINAPI HSSI_IsExistPort ( const INT nMasterID, const INT nPortNo);
// Slave가 존재하는지 확인
FAS_HSSI FAS_STATUS WINAPI HSSI_IsExistSlaveModule ( const INT nMasterID, const INT nPortNo, const INT nSlaveID );

///////////////////////////////////////////////////////
//
//		Data Control Function
//
///////////////////////////////////////////////////////

// 여기서 부터 nType은 상위 Byte인지 하위 Byte인지를 나타냅니다.

// Dout 명령
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadOutputWord  ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, WORD* pwData);
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadOutputByte  ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nType,     BYTE* pbtData);
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadOutputBit   ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nBitCount, BOOL* pbData);
FAS_HSSI FAS_STATUS WINAPI HSSI_WriteOutputWord ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const WORD wData);
FAS_HSSI FAS_STATUS WINAPI HSSI_WriteOutputByte ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nType,     const BYTE btData);
FAS_HSSI FAS_STATUS WINAPI HSSI_WriteOutputBit  ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nBitCount, const BOOL bData);
// Din 명령
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadInputWord ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, WORD* pwData);
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadInputByte ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nType,     BYTE* pbtData);
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadInputBit  ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nBitCount, BOOL* pbData);

// Latct 동작 명령
FAS_HSSI FAS_STATUS WINAPI HSSI_SetLatchWord ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const WORD wData);
FAS_HSSI FAS_STATUS WINAPI HSSI_SetLatchByte ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nType,     const BYTE btData);
FAS_HSSI FAS_STATUS WINAPI HSSI_SetLatchBit  ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nBitCount, const BOOL bData);
// Latch 확인 명령
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadLatchWord ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, WORD* pwData);
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadLatchByte ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nType,     BYTE* pbtData);
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadLatchBit  ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nBitCount, BOOL* pbData);
// Latch 확인 명령
FAS_HSSI FAS_STATUS WINAPI HSSI_ResetLatchWord ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const WORD wData);
FAS_HSSI FAS_STATUS WINAPI HSSI_ResetLatchByte ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nType,     const BYTE btData);
FAS_HSSI FAS_STATUS WINAPI HSSI_ResetLatchBit  ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nBitCount, const BOOL bData);

// Direction 쓰기 명령
FAS_HSSI FAS_STATUS WINAPI HSSI_WriteDirectionWord ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const WORD wData);
FAS_HSSI FAS_STATUS WINAPI HSSI_WriteDirectionByte ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nType,     const BYTE btData);
FAS_HSSI FAS_STATUS WINAPI HSSI_WriteDirectionBit  ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nBitCount, const BOOL bData );
// Direction 확인 명령
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadDirectionWord ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, WORD* pwData );
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadDirectionByte ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nType,     BYTE* pbtData );
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadDirectionBit  ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nBitCount, BOOL* pbData );
// Direction 설정 명령 (Dout)
FAS_HSSI FAS_STATUS WINAPI HSSI_SetOutWord ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const WORD wData );
FAS_HSSI FAS_STATUS WINAPI HSSI_SetOutByte ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nType,     const BYTE btData );
FAS_HSSI FAS_STATUS WINAPI HSSI_SetOutBit  ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nBitCount, const BOOL bData = TRUE );
// Direction 설정 명령 (Din)
FAS_HSSI FAS_STATUS WINAPI HSSI_SetInWord ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const WORD wData );
FAS_HSSI FAS_STATUS WINAPI HSSI_SetInByte ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nType,     const BYTE btData );
FAS_HSSI FAS_STATUS WINAPI HSSI_SetInBit  ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nBitCount, const BOOL bData = TRUE );
