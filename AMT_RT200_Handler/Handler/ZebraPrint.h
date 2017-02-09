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
	// 014,0,0,0237,001,0,0,0,000,0,0,0. [실제 return data]
	int					m_nComm;						//	014
	int					m_nPaperOutFlag;				//	0		페이퍼가 없다. 
	int					m_nPauseFlag;					//	0		일시정시 상태이다. 
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
	// 001,0,0,0,1,0,6,0,00000000,1,000. [실제 return data]
	int					m_nFuncSetting;					// 001
	int					m_nUnused1;						// 0		사용하지 않음
	int					m_nHeadUpFlag;					// 0		해더가 open 되어 있다. 
	int					m_nRibbonOutFlag;				// 0		리본이 없다.  
	int					m_nThermalTransferModeFlag;		// 1		열전송 모드 플레그 
	int					m_nPrintMode;					// 6
	int					m_nPrintWidthMode;				// 0		폭 모드 인쇄 
	int					m_nLabelWaitFlag;				// 00000000 라벨 대기 플레그
	int					m_nLabelsRemaing;				// 1		베치에 남아있는 레이블
	int					m_nPrintMovingFlag;				// 000		포멧하는 동안 인쇄 플레그
	int					m_nGraphicImgSavNo;				//			그래픽 이미지의 숫자는 메모리에 저장됨
	
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
