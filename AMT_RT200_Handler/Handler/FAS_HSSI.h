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

#define MAX_PCI_SLOT 8	// 0~7 8��
#define MAX_PORT_NO 4	// 0~3 4��
#define MAX_SLAVE_NO 8	// 0~7 8��

#define TYPE_LBYTE	2 // Low Byte
#define TYPE_HBYTE	3 // High Byte

#define FAS_STATUS	DWORD // Return Value

#define DLL_VERSION	120

// ��� ������ �̻������ Ȯ���ϴ� ���
// ���� if(FAS_SUCCESS(result))
#define FAS_SUCCESS(fasStatus)	(((fasStatus) == 0) ? TRUE : FALSE)

///////////////////////////////////////////////////////
//		Error Message for function of GetLastError
///////////////////////////////////////////////////////

typedef enum HSSI_RESULT
{	HSSI_STATUS_SUCCESS = 0x0,
	HSSI_STATUS_ERROR_DLL,           // DLL File Error
	HSSI_STATUS_ERROR_NO_DEVICE,     // �������� ���尡 ���� ��
	HSSI_STATUS_ERROR_READ,          // Read���� ����
	HSSI_STATUS_ERROR_WRITE,         // Write���� ����
	HSSI_STATUS_ERROR_INITIALIZE,    // �ʱ�ȭ ���� (Reset����)
	HSSI_STATUS_ERROR_COMMAND,       // CMD ��뿡 �����ְų�
                                     // �ش� Cmd�� �������� �ʴ� Slave Module ��
	HSSI_STATUS_ERROR_FILE,          // File�� ��������
	HSSI_STATUS_ERROR_PORT,          // ��Ʈ�� �������� ����
	HSSI_STATUS_ERROR_SLAVE,         // �����̺갡 ON�� �ƴ�
	HSSI_STATUS_NOTUSE_COMMAND,      // �������� �ʴ� �����
	HSSI_STATUS_INVALID_BOARDID,     // �ش� Board�� ���� X
	HSSI_STATUS_INVALID_PORTNO,      // �ش� Port�� ���� X
	HSSI_STATUS_INVALID_SLAVEID,     // �ش� Slave�� ���� X,
	HSSI_STATUS_INVALID_PARAMETER,   // �Ķ���� �Է� ����
	HSSI_STATUS_INVALID_DLL = 0x7fff // �߸��� DLL�� ���
} HSSI_RESULT;

///////////////////////////////////////////////////////
//		��żӵ��� ���Ǵ� ����
///////////////////////////////////////////////////////

typedef enum PORT_SPEED
{	PORT_SPEED_05 = 0x0, // 0,5 MHz
	PORT_SPEED_10,	// 1 MHz
	PORT_SPEED_20	// 2 MHz
} PORT_SPEED;

///////////////////////////////////////////////////////
//		Slave Tpye�� Ȯ���ϴ� �Լ����� ���ϵǴ� ����
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

// Master Board�� ����� ������ (�������� Master Board�߿� �ϳ��� ����ǵ� �������� ����)
FAS_HSSI FAS_STATUS WINAPI HSSI_Open ( INT* pCount = NULL );
// Master Board�� ����� ������
FAS_HSSI FAS_STATUS WINAPI HSSI_Close ( void );
// DLL File�� Version�� Ȯ����
//FAS_HSSI FAS_STATUS	WINAPI HSSI_GetDLLVersion(WCHAR* lpVersion, INT nBufferSize);
// ������ Board�� �����Ǿ������� Ȯ�� (�� ������ ī��� ����� �Ǿ����� Ȯ��)
FAS_HSSI FAS_STATUS WINAPI HSSI_GetAttachedMasterBoard ( INT* pBoardNo );//
// �ش� Board�� ��� Port�� �ִ��� Ȯ���ϴ� ����
FAS_HSSI FAS_STATUS WINAPI HSSI_GetAttachedPort ( const INT nMasterID, INT* pnPortNo );
// �ش� Port�� ����Ǿ��ִ� Slave�� Rertun (BCD)
FAS_HSSI FAS_STATUS WINAPI HSSI_GetAttachedSlaveModule ( const INT nMasterID, const INT nPortNo, INT* pSlaveNo);
// Master Board�� �ʱ�ȭ �ϴ� ���
FAS_HSSI FAS_STATUS WINAPI HSSI_ResetBoard (INT nMasterID);

// Port�� ��żӵ��� ������ nType : 0 (0.5MHz), 1 (1MHz), 2�̻� (2MHz)
FAS_HSSI FAS_STATUS WINAPI HSSI_SetCommSpeed ( const INT nMasterID, const INT nPortNo, const INT nType);
FAS_HSSI FAS_STATUS WINAPI HSSI_GetCommSpeed ( const INT nMasterID, const INT nPortNo, INT* nType);
// Slave�� On /Off ��Ŵ (bEnable : TRUE (On), FALSE (Off)
FAS_HSSI FAS_STATUS WINAPI HSSI_EnableSlaveModule ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const BOOL bEnable);
// ����Ǿ� �ִ� Slave�� ã�Ƽ� ON ��Ŵ
FAS_HSSI FAS_STATUS WINAPI HSSI_PortAutoEnable ( const INT nMasterID, const INT nPortNo );
// Slave Module�� ������ Ȯ��
FAS_HSSI FAS_STATUS WINAPI HSSI_GetSlaveType( const INT nMasterID, const INT nPortNo, const INT nSlaveID, BYTE* btData);
// ������ Slave Module�� Tpye�� ���߾� I/O ������ ����
FAS_HSSI FAS_STATUS WINAPI HSSI_ApplySlaveType ( const INT nMasterID, const INT nPortNo, const INT nSlaveID);


///////////////////////////////////////////////////////
//
//		Search Function
//
///////////////////////////////////////////////////////

// Card�� �����ϴ��� Ȯ��
FAS_HSSI FAS_STATUS WINAPI HSSI_IsExistMasterBoard ( const INT nMasterID );
// Port�� �����ϴ��� Ȯ��
FAS_HSSI FAS_STATUS WINAPI HSSI_IsExistPort ( const INT nMasterID, const INT nPortNo);
// Slave�� �����ϴ��� Ȯ��
FAS_HSSI FAS_STATUS WINAPI HSSI_IsExistSlaveModule ( const INT nMasterID, const INT nPortNo, const INT nSlaveID );

///////////////////////////////////////////////////////
//
//		Data Control Function
//
///////////////////////////////////////////////////////

// ���⼭ ���� nType�� ���� Byte���� ���� Byte������ ��Ÿ���ϴ�.

// Dout ���
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadOutputWord  ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, WORD* pwData);
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadOutputByte  ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nType,     BYTE* pbtData);
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadOutputBit   ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nBitCount, BOOL* pbData);
FAS_HSSI FAS_STATUS WINAPI HSSI_WriteOutputWord ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const WORD wData);
FAS_HSSI FAS_STATUS WINAPI HSSI_WriteOutputByte ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nType,     const BYTE btData);
FAS_HSSI FAS_STATUS WINAPI HSSI_WriteOutputBit  ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nBitCount, const BOOL bData);
// Din ���
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadInputWord ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, WORD* pwData);
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadInputByte ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nType,     BYTE* pbtData);
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadInputBit  ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nBitCount, BOOL* pbData);

// Latct ���� ���
FAS_HSSI FAS_STATUS WINAPI HSSI_SetLatchWord ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const WORD wData);
FAS_HSSI FAS_STATUS WINAPI HSSI_SetLatchByte ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nType,     const BYTE btData);
FAS_HSSI FAS_STATUS WINAPI HSSI_SetLatchBit  ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nBitCount, const BOOL bData);
// Latch Ȯ�� ���
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadLatchWord ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, WORD* pwData);
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadLatchByte ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nType,     BYTE* pbtData);
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadLatchBit  ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nBitCount, BOOL* pbData);
// Latch Ȯ�� ���
FAS_HSSI FAS_STATUS WINAPI HSSI_ResetLatchWord ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const WORD wData);
FAS_HSSI FAS_STATUS WINAPI HSSI_ResetLatchByte ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nType,     const BYTE btData);
FAS_HSSI FAS_STATUS WINAPI HSSI_ResetLatchBit  ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nBitCount, const BOOL bData);

// Direction ���� ���
FAS_HSSI FAS_STATUS WINAPI HSSI_WriteDirectionWord ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const WORD wData);
FAS_HSSI FAS_STATUS WINAPI HSSI_WriteDirectionByte ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nType,     const BYTE btData);
FAS_HSSI FAS_STATUS WINAPI HSSI_WriteDirectionBit  ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nBitCount, const BOOL bData );
// Direction Ȯ�� ���
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadDirectionWord ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, WORD* pwData );
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadDirectionByte ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nType,     BYTE* pbtData );
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadDirectionBit  ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nBitCount, BOOL* pbData );
// Direction ���� ��� (Dout)
FAS_HSSI FAS_STATUS WINAPI HSSI_SetOutWord ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const WORD wData );
FAS_HSSI FAS_STATUS WINAPI HSSI_SetOutByte ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nType,     const BYTE btData );
FAS_HSSI FAS_STATUS WINAPI HSSI_SetOutBit  ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nBitCount, const BOOL bData = TRUE );
// Direction ���� ��� (Din)
FAS_HSSI FAS_STATUS WINAPI HSSI_SetInWord ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const WORD wData );
FAS_HSSI FAS_STATUS WINAPI HSSI_SetInByte ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nType,     const BYTE btData );
FAS_HSSI FAS_STATUS WINAPI HSSI_SetInBit  ( const INT nMasterID, const INT nPortNo, const INT nSlaveID, const INT nBitCount, const BOOL bData = TRUE );
