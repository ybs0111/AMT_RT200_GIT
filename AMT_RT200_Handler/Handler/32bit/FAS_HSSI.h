////////////////////////////////////////////////////////
//		FAS-HSSI Program DLL Version 1.0.0.1
////////////////////////////////////////////////////////

#ifdef __FAS_HSSI_DLL
#define FAS_HSSI	extern "C" _declspec(dllexport)
#else
#define FAS_HSSI	extern "C" _declspec(dllimport)
#pragma comment( lib, "HSSI.lib" )
#endif

#define MAX_PCI_SLOT 8	// 0~7 8��
#define MAX_PORT_NO 4	// 0~3 4��
#define MAX_SLAVE_NO 8	// 0~7 8��

#define TYPE_LBYTE	0x2 // Low Byte
#define TYPE_HBYTE	0x3 // High Byte

#define FAS_STATUS	DWORD // Return Value

#define DLL_VERSION	110

// ��� ������ �̻������ Ȯ���ϴ� ���
// ���� if(FAS_SUCCESS(result))
#define FAS_SUCCESS(fasStatus)	(((fasStatus) == 0) ? TRUE : FALSE)

///////////////////////////////////////////////////////
//		Error Message for function of GetLastError
///////////////////////////////////////////////////////

enum
{
	STATUS_SUCCESS = 0x0,
	STATUS_ERROR_DLL,			// DLL File Error
	STATUS_ERROR_NO_DEVICE,		// �������� ���尡 ���� ��
	STATUS_ERROR_READ,			// Read���� ����
	STATUS_ERROR_WRITE,			// Write���� ����
	STATUS_ERROR_INITIALIZE,	// �ʱ�ȭ ���� (Reset����)
	STATUS_ERROR_COMMAND,		// CMD ��뿡 �����ְų�
								// �ش� Cmd�� �������� �ʴ� Slave Module ��
	STATUS_ERROR_FILE,			// File�� ��������
	STATUS_ERROR_PORT,			// ��Ʈ�� �������� ����
	STATUS_ERROR_SLAVE,			// �����̺갡 ON�� �ƴ�
	STATUS_NOTUSE_COMMAND,		// �������� �ʴ� �����
	STATUS_INVALID_BOARDID,		// �ش� Board�� ���� X
	STATUS_INVALID_PORTNO,		// �ش� Port�� ���� X
	STATUS_INVALID_SLAVEID,		// �ش� Slave�� ���� X,
	STATUS_INVALID_DLL = 0x7fff	// �߸��� DLL�� ���
};

///////////////////////////////////////////////////////
//		Open �Լ��� ���Ǵ� ����
///////////////////////////////////////////////////////

enum
{	
	START_NOTHING = 0x0,	// Digital I/O������ �ǵ帮�� �ʰ� Open
	START_CLEAR,			// Digital I/O�� ��� Digital Input���� �ϰ� Slave�� ��� OFF
	START_LOADDATA,			// ������ ����Ǿ��� ���� �״�� ���
};

///////////////////////////////////////////////////////
//		��żӵ��� ���Ǵ� ����
///////////////////////////////////////////////////////

enum
{
	PORT_SPEED_05 = 0x0, // 0,5 MHz
	PORT_SPEED_10,	// 1 MHz
	PORT_SPEED_20	// 2 MHz
};

///////////////////////////////////////////////////////
//		Slave Tpye�� Ȯ���ϴ� �Լ����� ���ϵǴ� ����
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

// Master Board�� ����� ������ (�������� Master Board�߿� �ϳ��� ����ǵ� �������� ����)
FAS_HSSI FAS_STATUS WINAPI HSSI_Open(INT* pCount=NULL, INT nInitialize = START_NOTHING);
// Master Board�� ����� ������
FAS_HSSI FAS_STATUS WINAPI HSSI_Close(BOOL bSetInitialize/*=TRUE*/);
// DLL File�� Version�� Ȯ����
FAS_HSSI FAS_STATUS	WINAPI HSSI_GetDLLVersion(LPCTSTR lpVersion, INT nBufferSize);
// ������ Board�� �����Ǿ������� Ȯ�� (�� ������ ī��� ����� �Ǿ����� Ȯ��)
FAS_HSSI FAS_STATUS WINAPI HSSI_GetAttachedMasterBoard(INT* pBoardNo);//
// �ش� Board�� ��� Port�� �ִ��� Ȯ���ϴ� ����
FAS_HSSI FAS_STATUS WINAPI HSSI_GetAttachedPort(INT nMasterID, INT* pnPortNo);
// �ش� Port�� ����Ǿ��ִ� Slave�� Rertun (BCD)
FAS_HSSI FAS_STATUS WINAPI HSSI_GetAttachedSlaveModule(INT nMasterID, INT nPortNo, INT* pSlaveNo);
// I/O ������ Save �� Load�ϴ� ���
FAS_HSSI FAS_STATUS WINAPI HSSI_SaveConfiguration(LPCTSTR lpszFileName);
FAS_HSSI FAS_STATUS WINAPI HSSI_LoadConfiguration(LPCTSTR lpszFileName);
// Master Board�� �ʱ�ȭ �ϴ� ���
FAS_HSSI FAS_STATUS WINAPI HSSI_ResetBoard(INT nMasterID);

// Port�� ��żӵ��� ������ nType : 0 (0.5MHz), 1 (1MHz), 2�̻� (2MHz)
FAS_HSSI FAS_STATUS WINAPI HSSI_SetCommSpeed (INT nMasterID, INT nPortNo, INT nType);
FAS_HSSI FAS_STATUS WINAPI HSSI_GetCommSpeed (INT nMasterID, INT nPortNo, INT* nType);
// Slave�� On /Off ��Ŵ (bEnable : TRUE (On), FALSE (Off)
FAS_HSSI FAS_STATUS WINAPI HSSI_EnableSlaveModule(INT nMasterID, INT nPortNo, INT nSlaveID, BOOL bEnable);
// ����Ǿ� �ִ� Slave�� ã�Ƽ� ON ��Ŵ
FAS_HSSI FAS_STATUS WINAPI HSSI_PortAutoEnable(INT nMasterID, INT nPortNo);
// Slave Module�� ������ Ȯ��
FAS_HSSI FAS_STATUS WINAPI HSSI_GetSlaveType(INT nMasterID, INT nPortNo, INT nSlaveID, BYTE* btData);
// ������ Slave Module�� Tpye�� ���߾� I/O ������ ����
FAS_HSSI FAS_STATUS WINAPI HSSI_ApplySlaveType(INT nMasterID, INT nPortNo, INT nSlaveID);


///////////////////////////////////////////////////////
//
//		Search Function
//
///////////////////////////////////////////////////////

// Card�� �����ϴ��� Ȯ��
FAS_HSSI FAS_STATUS WINAPI HSSI_IsExistMasterBoard (INT nMasterID);
// Port�� �����ϴ��� Ȯ��
FAS_HSSI FAS_STATUS WINAPI HSSI_IsExistPort (INT nMasterID, INT nPortNo);
// Slave�� �����ϴ��� Ȯ��
FAS_HSSI FAS_STATUS WINAPI HSSI_IsExistSlaveModule (INT nMasterID, INT nPortNo, INT nSlaveID);

///////////////////////////////////////////////////////
//
//		Data Control Function
//
///////////////////////////////////////////////////////

// ���⼭ ���� nType�� ���� Byte���� ���� Byte������ ��Ÿ���ϴ�.

// Dout ���
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadOutputWord (INT nMasterID, INT nPortNo, INT nSlaveID, WORD* pwData);
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadOutputByte (INT nMasterID, INT nPortNo, INT nSlaveID, INT nType, BYTE* pbtData);
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadOutputBit (INT nMasterID, INT nPortNo, INT nSlaveID, INT nBitCount, BOOL* pbData);
FAS_HSSI FAS_STATUS WINAPI HSSI_WriteOutputWord (INT nMasterID, INT nPortNo, INT nSlaveID, WORD wData);
FAS_HSSI FAS_STATUS WINAPI HSSI_WriteOutputByte (INT nMasterID, INT nPortNo, INT nSlaveID, INT nType, BYTE btData);
FAS_HSSI FAS_STATUS WINAPI HSSI_WriteOutputBit (INT nMasterID, INT nPortNo, INT nSlaveID, INT nBitCount, BOOL bData);
// Din ���
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadInputWord (INT nMasterID, INT nPortNo, INT nSlaveID, WORD* pwData);
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadInputByte (INT nMasterID, INT nPortNo, INT nSlaveID, INT nType, BYTE* pbtData);
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadInputBit (INT nMasterID, INT nPortNo, INT nSlaveID, INT nBitCount, BOOL* pbData);

// Latct ���� ���
FAS_HSSI FAS_STATUS WINAPI HSSI_SetLatchWord (INT nMasterID, INT nPortNo, INT nSlaveID, WORD wData);
FAS_HSSI FAS_STATUS WINAPI HSSI_SetLatchByte (INT nMasterID, INT nPortNo, INT nSlaveID, INT nType, BYTE btData);
FAS_HSSI FAS_STATUS WINAPI HSSI_SetLatchBit (INT nMasterID, INT nPortNo, INT nSlaveID, INT nBitCount, BOOL bData);
// Latch Ȯ�� ���
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadLatchWord (INT nMasterID, INT nPortNo, INT nSlaveID, WORD* pwData);
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadLatchByte (INT nMasterID, INT nPortNo, INT nSlaveID, INT nType, BYTE* pbtData);
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadLatchBit (INT nMasterID, INT nPortNo, INT nSlaveID, INT nBitCount, BOOL* pbData);
// Latch Ȯ�� ���
FAS_HSSI FAS_STATUS WINAPI HSSI_ResetLatchWord (INT nMasterID, INT nPortNo, INT nSlaveID, WORD wData);
FAS_HSSI FAS_STATUS WINAPI HSSI_ResetLatchByte (INT nMasterID, INT nPortNo, INT nSlaveID, INT nType, BYTE btData);
FAS_HSSI FAS_STATUS WINAPI HSSI_ResetLatchBit (INT nMasterID, INT nPortNo, INT nSlaveID, INT nBitCount, BOOL bData);

// Direction ���� ���
FAS_HSSI FAS_STATUS WINAPI HSSI_WriteDirectionWord (INT nMasterID, INT nPortNo, INT nSlaveID, WORD wData);
FAS_HSSI FAS_STATUS WINAPI HSSI_WriteDirectionByte (INT nMasterID, INT nPortNo, INT nSlaveID, INT nType, BYTE btData);
FAS_HSSI FAS_STATUS WINAPI HSSI_WriteDirectionBit (INT nMasterID, INT nPortNo, INT nSlaveID, INT nBitCount, BOOL bData);
// Direction Ȯ�� ���
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadDirectionWord (INT nMasterID, INT nPortNo, INT nSlaveID, WORD* pwData);
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadDirectionByte (INT nMasterID, INT nPortNo, INT nSlaveID, INT nType, BYTE* pbtData);
FAS_HSSI FAS_STATUS WINAPI HSSI_ReadDirectionBit (INT nMasterID, INT nPortNo, INT nSlaveID, INT nBitCount, BOOL* pbData);
// Direction ���� ��� (Dout)
FAS_HSSI FAS_STATUS WINAPI HSSI_SetOutWord(INT nMasterID, INT nPortNo, INT nSlaveID, WORD wData);
FAS_HSSI FAS_STATUS WINAPI HSSI_SetOutByte(INT nMasterID, INT nPortNo, INT nSlaveID, INT nType, BYTE btData);
FAS_HSSI FAS_STATUS WINAPI HSSI_SetOutBit(INT nMasterID, INT nPortNo, INT nSlaveID, INT nBitCount, BOOL bData = TRUE);
// Direction ���� ��� (Din)
FAS_HSSI FAS_STATUS WINAPI HSSI_SetInWord(INT nMasterID, INT nPortNo, INT nSlaveID, WORD wData);
FAS_HSSI FAS_STATUS WINAPI HSSI_SetInByte(INT nMasterID, INT nPortNo, INT nSlaveID, INT nType, BYTE btData);
FAS_HSSI FAS_STATUS WINAPI HSSI_SetInBit(INT nMasterID, INT nPortNo, INT nSlaveID, INT nBitCount, BOOL bData = TRUE);
