// ZebraPrint.h: interface for the CZebraPrint class.
#pragma once

enum
{
		JPL_COMM					 = 	0,				
		JPL_PAPER_OUT_FLAG,		
		JPL_PAUSE_FLAG,			
		JPL_LABEL_LENGTH,		
		JPL_RCV_BUF_FORMAT_NO,		
		JPL_BUF_FULL_FLAG,		
		JPL_COMM_DMODE_FLAG,		
		JPL_PARTIAL_FORMAT_FLAG,	
		JPL_UNUSED,				
		JPL_CORRUP_RAM_FLAG,	
		JPL_UNDER_TEM_RANGE,		
		JPL_OVER_TEM_RANGE,		
		JPL_FUNC_SETTING,	
		JPL_UNUSED1,						
		JPL_HEAD_UP_FLAG,					
		JPL_RIBBON_OUTFLAG,					
		JPL_THEMAL_TRANSFER_MD_FLAG,	
		JPL_PRINT_MODE,					
		JPL_PRINT_WIDTH_MODE,				
		JPL_LABEL_WAIT_FLAG,				
		JPL_LABEL_REMAING,				
		JPL_PRINT_MOVING_FLAG,			
		JPL_GRAPHIC_IMG_SAV_NO,		
		JPL_PASS_WORD,
		JPL_RAM_INIT_STATE,
};

//#include "SrcBase/ANetwork.h"

class CZebraPrint  
{
public:
	int					m_nPrintStatusBin;
	int					m_nStstusMode;
	// String 1 <STX>aaa,b,c,dddd,eee,f,g,h,iii,j,k,l<ETX><CR><LF>
	// 014,0,0,0237,001,0,0,0,000,0,0,0. [���� return data]
	int					m_nComm;						//	014
	int					m_nPaperOutFlag;				//	0		�����۰� ����. 
	int					m_nPauseFlag;					//	0		�Ͻ����� �����̴�. 
	int					m_nLabelLength;					//	0237	
	int					m_nRcvBufFormatNo;				//	001
	int					m_nBufFullFlag;					//	0
	int					m_nCommDModeFlag;				//	0
	int					m_nPartialFormatFlag;			//	0
	int					m_nUnused;						//  0
	int					m_nCorruptRamFlag;				//	000
	int					m_nUnderTemRange;				//	0
	int					m_nOverTemRange;				//	0 
	
	// String 2 <STX>mmm,n,o,p,q,r,s,t,uuuuuuuu,v,www<ETX><CR><LF>
	// 001,0,0,0,1,0,6,0,00000000,1,000. [���� return data]
	int					m_nFuncSetting;					// 001
	int					m_nUnused1;						// 0		������� ����
	int					m_nHeadUpFlag;					// 0		�ش��� open �Ǿ� �ִ�. 
	int					m_nRibbonOutFlag;				// 0		������ ����.  
	int					m_nThermalTransferModeFlag;		// 1		������ ��� �÷��� 
	int					m_nPrintMode;					// 6
	int					m_nPrintWidthMode;				// 0		�� ��� �μ� 
	int					m_nLabelWaitFlag;				// 00000000 �� ��� �÷���
	int					m_nLabelsRemaing;				// 1		��ġ�� �����ִ� ���̺�
	int					m_nPrintMovingFlag;				// 000		�����ϴ� ���� �μ� �÷���
	int					m_nGraphicImgSavNo;				//			�׷��� �̹����� ���ڴ� �޸𸮿� �����
	
	// String 3 <STX>xxxx,y<ETX><CR><LF>
	int					m_nPsWord;
	int					m_nRamInitState;

	bool				OnPrintStatus(CString strCheckData, int nPort);
	void				Rewind_Mode_TCP(int port);
	void				Print_Complete_Set_TCP(int port);
	void				Print_Complete_Reset_TCP(int port);
	void				Printer_Status_TCP(int port);

	bool				OnPrintStatus_Serial(CString strCheckData, int nPort);
	void				Rewind_Mode_Serial(int port);
	void				Print_Complete_Set_Serial(int port);
	void				Print_Complete_Reset_Serial(int port);
	void				Printer_Status_Serial(int port);
		
	int					Rewind_Mode_LPT1();
	int					Print_Complete_Set_LPT1();
	int					Print_Complete_Reset_LPT1();

	void				OnPrintStatus(int nRtnSplit, int nFlagName, int nPort);
	int					OnBarcodeMake(int nDir, int nPort);
	int					OnOutLptPort(CString str_text);
	void				OnPrintAnswerMode(int nMethod, int nMode, int nPort); // 20140906 jtkim
	void				OnPrintLabelMode(int nMethod, int nMode, int nPort); // 20140906 jtkim
	void				OnPrintPauseMode(int nMethod, int nMode, int nPort); // 20140906 jtkim
	void				OnPrintStatusCheck(int nMethod, int nPort); // 20140906 jtkim
	CString				OnPrintLabelOffset(int nOffsetX, int nOffsetY, CString strText);
	void				OnPrintOutput(int nMethod, int nPort, int nOffsetX, int nOffsetY, CString strText);
	void				OnPrintCalibration(int nMethod, int nPort);
	void				OnPrintComplete(CString strRev);
	////2015.0413
	void				SetDarkness_TCP(int nPort);
	void				LabelTop_TCP(int nPort);
	void				LabelHome_TCP(int nPort);
	////
	void			   OnStringToChar(CString strData, char chData[]);
public:
	CZebraPrint();
	virtual ~CZebraPrint();

};

extern CZebraPrint clsZebra;
