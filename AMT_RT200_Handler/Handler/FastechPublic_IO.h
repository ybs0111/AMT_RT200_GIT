// FastechPublic_IO.h: interface for the CFastechPublic_IO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FASTECHPUBLIC_IO_H__C22EE9E3_BCDD_43ED_B749_CDFE31F58416__INCLUDED_)
#define AFX_FASTECHPUBLIC_IO_H__C22EE9E3_BCDD_43ED_B749_CDFE31F58416__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// jtkim 20130625
// include the lib file for compile
#ifdef _WIN64
	#pragma			comment(lib, ".\\64bit\\FAS_HSSI.lib")
#else
	#pragma			comment(lib, ".\\32bit\\HSSI.lib")
#endif
/////////////////////////////////////////////////////////
//DLL ������ ���濹�� //2011.1007 �߰� 
/////////////////////////////////////////////////////////
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

enum
{
	FAS_CHECK_FALSE = -9999,	// -9999	// ��ġ���� �����ؾ� �ϴµ�, �������϶�
	FAS_CHECK_BD_FALSE,			// -9998	// ��ġ���� �����ؾ� �ϴµ�, �������϶�
	FAS_CHECK_AXIS_FALSE,		// -9997	// ��ġ���� �����ؾ� �ϴµ�, �������϶�
	FAS_FALSE = 0,				// 0		// ������
	FAS_TRUE,					// 1		// ����
	FAS_PROCEED,				// 2		// ������ (���� ��������, ���������� ���� ����.)
	FAS_OPEN_FAIL,				// 3		// ����̺� ���� ����.
	FAS_CLOSE_FAIL,				// 4		// ����̺� �ݱ� ����.
	FAS_NOT_OPEN,				// 5		// ���� ���� ����.
	FAS_INVALID_BD_NUM,			// 6		// ���� ��ȣ ����.
	FAS_INVALID_AXIS_NUM,		// 7		// �� ��ȣ ����.
	FAS_INVALID_PARAMETER_NUM,	// 8		// �Ķ���� ����.
	FAS_PARAMETER_RANGE_ERROR,	// 9		// ���� �� ����.
	FAS_ZERO_RETURN_ERROR,		// 10
	FAS_TIMEOUT_ERROR,			// 11		// Ÿ�� �ƿ� ����.
	FAS_UNKNOWN_ERROR,			// 12		// �������� ����.
		
	FAS_SETTING_ERROR,			// 13		// ���� ����.
	FAS_INIT_ERROR,				// 14		// �ʱ�ȭ ����.
	FAS_READ_ERROR,				// 15		// �ش� �Լ� �б� ����.
		
	FAS_MOT_ALARM_ERROR,		// 16		// Motor�� �˶��� �ɸ� �����̴�.
	FAS_MOT_POWER_ERROR,		// 17		// Motor�� �Ŀ��� ON���� �ʴ´�.
	FAS_MOT_DONE_ERROR,			// 18		// Motor�� Motion Done�� ���� �ʴ´�.
	FAS_MOT_RETRY,				// 19		// Motor ������ ��õ� �����̴�.
	FAS_MOT_ERROR,				// 20		// Motor ������ ��õ� Ƚ�� �ʰ� ���� 
	FAS_MOT_SAFETY,				// 21		// Motor �̵��� ���ÿ� ���������� �Ҿ��Ͽ� ���� �Ұ� ����  //2007.09.27 kimgangmin add
		
	BOARD_OPEN_FAILED,
	SLAVE_ENABLE_FAILED,
	MASTER_SEARCH_FAILED,
	PORT_SEARCH_FAILED,
	SLAVE_SEARCH_FAILED,
	SPEED_SET_FAILED,
	SPEED_GET_FAILED
};

//#define HSSI_MAX_IO		7715 //max io board 2�� ������ �����Ͽ� ������� //max io board 4�� ������ ó���Ѵ� 
#define HSSI_MAX_IO		10000

class CFastechPublic_IO  
{
public:
	CFastechPublic_IO();
	virtual ~CFastechPublic_IO();

public:
	int			mn_errormsg_debug_mode;		//���� �޼��� ��¹�� => 0:������ , 1:���

	int			n_in[HSSI_MAX_IO];			// ��ü �� ��Ʈ I/O ���� ���� ����
	int			n_out[HSSI_MAX_IO];			// ��ü �ƿ� ��Ʈ I/O ���� ���� ����

	bool		CYLFlag[HSSI_MAX_IO];
	long		CYLTime[HSSI_MAX_IO][3];

	DWORD		m_dwWaitTime[30][HSSI_MAX_IO][3];
	// jtkim 20160125
	int			m_nWaitIO[30][HSSI_MAX_IO];
	BOOL		m_bWaitRetry[30][HSSI_MAX_IO];

	int			m_nIoBoardInitialFlag;	//mn_io_board_initial_flag;	//i/O ���尡 �ʱ�ȣ�� �Ϸ�Ǿ����� Ȯ���ϴ� �÷��� 

public:
	int Initialize_Board();
	void Initialize_Map();
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//2011.1019 james lee WORD ������ I/O�� �����Ѵ�
	//Ver 0.0.1_2011.1019
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	int IO_ReadWrite(int n_bd_num, int n_port_num, int n_slave_num, int n_rw_mode);//2011.1019 james //Word ������ ����� ����ϴ� �Լ� 
	int get_in_bit_wait(int nNum, int nIO, int nOnOff, int nTime = 0);
	int get_in_bit(int n_IONo, int n_OnOff);  //2011.1019 Simulation ���۱��� �������� onoff �߰� //Word ������ ����� ����ϴ� �Լ�
	int set_out_bit(int n_IONo, int n_OnOff); //2011.1019 Simulation ���۱��� �������� onoff �߰� //Word ������ ����� ����ϴ� �Լ�
	int get_out_bit(int n_IONo, int n_OnOff); //2011.1019 Simulation ���۱��� �������� onoff �߰� //Word ������ ����� ����ϴ� �Լ�
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////


	int  Search_IO_BoardInformation(int nType, int nData);             // HSSI ���� ������ Ȯ�� �� �˻� �Լ�
	int  Set_IO_BoardOpen(INT* piMasterNo);           // I/O ���� ���� �Լ�
	int  Set_IO_BoardClose(bool bCloseMethod);                         // I/O ���� ���� �Լ�
	int  Search_IO_Master(INT* piMasterNo);                            // PC�� ����� ������ ���� ���� �˻� �Լ�
	int  Search_IO_Port(int nMasterID, INT* piPortNo);                 // ������ ���忡 ����� PORT ���� �˻� �Լ�
	int  Search_IO_Slave(int nMasterID, int nPortNo, INT* piSlaveNo);  // PORT�� ����� SLAVE �˻� �Լ�
	BOOL Check_IO_Slave(int nMasterID, int nPortNo, int nSlaveID);     // PORT�� ����� SLAVE [ENABLE/DISABLE] ���� �˻� �Լ�

	int  Set_IO_HSSISpeed(int nMasterID, int nPortNo, int nType);                  // PORT�� SLAVE I/O ������ ��� �ӵ� ���� �Լ�
	int  Get_IO_HSSISpeed(int nMasterID, int nPortNo, INT* piType);                // PORT�� SLAVE I/O ������ ��� �ӵ� ���� �Լ�
	int  Set_IO_PortAutoEnable(int nMasterID, int nPortNo);                           // PORT�� ����� ��� SLAVE I/O ��� �ڵ� ENABLE �Լ�
	int  Set_IO_SlaveEnable(int nMasterID, int nPortNo, int nSlaveNo, BOOL bEnable);  // SLAVE I/O ��� ���� ENABLE/DISABLE ���� �Լ�
	
	int  Set_IO_DefineWord(int nMasterID, int nPortNo, int nSlaveNo, WORD wData);               // SLAVE I/O ����� ����� ���� �Լ� [WORD ����]
	int  Set_IO_DefineByte(int nMasterID, int nPortNo, int nSlaveNo, int nType, BYTE btData);   // SLAVE I/O ����� ����� ���� �Լ� [BYTE ����]
	int  Set_IO_DefineBit(int nIONo, BOOL bData);                                               // SLAVE I/O ����� ����� ���� �Լ� [BIT ����]
	int  Get_IO_DefineWord(int nMasterID, int nPortNo, int nSlaveNo, WORD* pwData);             // SLAVE I/O ����� ����� ���� ���� ���� [WORD ����]
	int  Get_IO_DefineByte(int nMasterID, INT iPortNo, int nSlaveNo, int nType, BYTE* pbData);  // SLAVE I/O ����� ����� ���� ���� ���� [BYTE ����]
	int  Get_IO_DefineBit(int nIONo, BOOL* pbData);                                             // SLAVE I/O ����� ����� ���� ���� ���� [BIT ����]
	
	int  Set_IO_LatchWord(int nMasterID, int nPortNo, int nSlaveNo, WORD wData);                  // SLAVE I/O ����� �Է��� LATCH ���� ���� �Լ� [WORD ����]
	int  Set_IO_LatchByte(int nMasterID, int nPortNo, int nSlaveNo, int nType, BYTE btData);      // SLAVE I/O ����� �Է��� LATCH ���� ���� �Լ� [BYTE ����]
	int  Set_IO_LatchBit(int nIONo, BOOL bData);                                                  // SLAVE I/O ����� �Է��� LATCH ���� ���� �Լ� [BIT ����]
	WORD Get_IO_LatchWord(int nMasterID, int nPortNo, int nSlaveNo);                              // SLAVE I/O ����� LATCH ���� ���� �Լ� [WORD ����]
	BYTE Get_IO_LatchByte(int nMasterID, INT iPortNo, int nSlaveNo, int nType);                   // SLAVE I/O ����� LATCH ���� ���� �Լ� [BYTE ����]
	BOOL Get_IO_LatchBit(int nIONo);                                                              // SLAVE I/O ����� LATCH ���� ���� �Լ� [BIT ����]
	int  Reset_IO_LatchWord(int nMasterID, int nPortNo, int nSlaveNo, WORD wData);                // SLAVE I/O ����� LATCH ���� ���� �Լ� [WORD ����]
	int  Reset_IO_LatchByte(int nMasterID, int nPortNo, int nSlaveNo, int nType, BYTE btData);    // SLAVE I/O ����� LATCH ���� ���� �Լ� [BYTE ����]
	int  Reset_IO_LatchBit(int nIONo, BOOL bData);                                                // SLAVE I/O ����� LATCH ���� ���� �Լ� [BIT ����]
	
	int  Check_IO_MasterStatus(INT nMasterID);                            // ������ ���� ���� ���� �˻� �Լ�
	int  Check_IO_PortStatus(int nMasterID, int nPortNo);                 // PORT ���� ���� �� ���� ���� �˻� �Լ�
	int  Check_IO_SlaveStatus(int nMasterID, int nPortNo, int nSlaveNo);  // SLAVE I/O MODULE�� ENABLE ���·� ������ ���忡 ����Ǿ� �ִ��� �˻� �Լ�

	int  Check_IO_BoardExist(int nMaxSlot, int nMaxPort, int nMaxSlave);  // ���� ���� ���� �˻� �Լ�

	BOOL Check_Bit(long nValue, int nBitPos);            // Ư�� ��ġ BIT�� �˻� �Լ�
	void Set_Bit(long &nValue, int nBit, BOOL bOn);      // Ư�� ��ġ BIT ���� �Լ�

	
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// I/O ���� LIB
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int  Initialize_Auto_IO_Board(void);   // I/O ���� ���̺귯�� �ʱ�ȭ �Լ�

	int  Set_Out_Word(int nIONo, WORD wData);              // SLAVE I/O ��� ��� ��ȣ �߻� �Լ� [WORD ����]
	int  Set_Out_Byte(int nIONo, int nType, BYTE btData);  // SLAVE I/O ��� ��� ��ȣ �߻� �Լ� [BYTE ����]
	int  Set_Out_Bit (int nIONo, BOOL bOnOff);             // SLAVE I/O ��� ��� ��ȣ �߻� �Լ� [BIT ����]

	WORD Get_In_Word(int nIONo);                           // SLAVE I/O ��� �Է� ��ȣ READ �Լ� [WORD ����]
	BYTE Get_In_Byte(int nIONo, int nType);                // SLAVE I/O ��� �Է� ��ȣ READ �Լ� [BYTE ����]
	BOOL Get_In_Bit(int nIONo);                            // SLAVE I/O ��� �Է� ��ȣ READ �Լ� [BIT ����]

//	BOOL Get_In_Bit(int nIONo, int n_SetTime);//james 2010,0128 , long * np_GetTime) ; //james 2009.5.27 �����ϰ� �ִ� �ð��� ã�� ���� �߰� 

	WORD Get_Out_Word(int nIONo);							// SLAVE I/O ��� ��� ��ȣ READ �Լ� [WORD ����]
	BYTE Get_Out_Byte(int nIONo, int nType);				// SLAVE I/O ��� ��� ��ȣ READ �Լ� [BYTE ����]
	BOOL Get_Out_Bit(int nIONo);							// SLAVE I/O ��� ��� ��ȣ READ �Լ� [BIT ����]

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Word(16bit) ������ �����Ѵ� //Picker ���� ��� ���� �ۼ�  
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int Lib_Set_Output_16(int nIoNum, int nOnOff, int *np_Data);  
	int Lib_Get_Input_16(int nIoNum, int nOnOff, int *np_Data);
	int Lib_Get_Output_16(int nIoNum, int nOnOff, int *np_Data);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//	int Check_IO_Port_Yield(int n_io, int n_time, int n_yield = 70);
//	int Check_IO_Port(int n_io, int n_time = 0, int n_flag = CTL_ON);
		

	
	
	// **************************************************************************
	// I/O ���� ���� Ŭ����                                                      
	// **************************************************************************
	char* ErrorOccurInfo(int nErrorNum);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�

	int OnCylinderActionCheck(int n_cyl, int n_sen1, int n_sen2, int, int n_wait, int n_limit);
	int OnCylinderActionCheck(int n_cyl, int n_sen, int n_act, int n_wait, int n_limit);
	void OnCylinderAction(int n_cyl, int n_cyl2, int n_act);
	void OnCylinderAction(int n_cyl, int n_act);

};
extern CFastechPublic_IO  FAS_IO;  

#endif // !defined(AFX_FASTECHPUBLIC_IO_H__C22EE9E3_BCDD_43ED_B749_CDFE31F58416__INCLUDED_)
