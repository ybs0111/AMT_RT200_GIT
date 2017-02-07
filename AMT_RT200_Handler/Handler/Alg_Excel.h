#pragma once

#define xlNormal	-4143

#include "XLAutomation.h"

class CAlg_Excel
{
public:
	// Motor 관련 정보...
	CString			m_strPartInfo[20];
	CString			m_strPartName[20];
	CString			m_strPartAxisName[20][10];
	CString			m_strAxisPosName[20][10][30];
	CString			m_strAxisPosInfo[20][10][30];
	int				m_nAxisPosData[100][30];
	CString			m_strAxisName[100];
	// IO 관련 정보......
	int				m_nMotorSelectPartNum;			// 모터 화면에서 선택된 Part 번보
	CString			m_strIOPortLabel[3][4][8];		// IO Port Name......
	CString			m_strIOBitLabel[3][4][8][16];	// IO Bit Name......
	CString			m_strIOBitDescrip[3][4][8][16];	// IO Bit Description......
	// WAIT TIME 관련 정보.....
	CString			m_strWaitTimeLabel[100];

	// Motor 관련 정보
	int				m_nPartCnt;
	int				m_nMotorMaxCnt;
	int				m_nPartAxisCnt[20];
	int				m_nPartAxis[20][10];
	int				m_nAxisPosNum[20][10];
	int				m_nAxisMinusEl[20][10];
	int				m_nAxisPlusEl[20][10];
	int				m_nAxisHome[20][10];
	int				m_nAxisDirection[20][10];
	int				m_nAxisLimitCnt[20][10];
	int				m_nAxisLimitInfo[20][10][3];
	int				m_nAxisAccount[20][10];
	// IO 관련정보
	int				m_nIOBoardCnt;						// IO Board 수량 최대 3장까지 설치.....
	int				m_nIOMaster[3];						// 각각의 IO Board Master 수량.....
	int				m_nIOPort[3][4];					// 각각의 IO Port 수량....
	int				m_nIOBitType[3][4][8][16];			// IO Type [0] : None [1] : INPUT [2] : OUTPUT.....
	int				m_nINIOBitNum[3][4][8][16];			// IN_IO Number.....	
	int				m_nOUTIOBitNum[3][4][8][16];			// OUT_IO Number.....	
	int				m_nIOBitNum[3][4][8][16];			// IO Number.....	
	int				m_nIOPortNum[3][4][8];				// IO Port Number.....
	// PART IO 관련정보
	int 			m_nPartInOutTPutCnt[20][2];			// PART : INPUT/OUTPUT
	int 			m_nFirstPortNo[20][2][20];			// PART : INPUT/OUTPUT : PORT를 
	int 			m_nPartAccBittNo[20][2][20][20];			// PART : INPUT/OUTPUT : PORT를 
	int 			m_nPartInOutTPutPortCnt[20][2][20];	// PART : INPUT/OUTPUT : PORT
	CString			m_sPartInOutPutName[10][2][20][16];	// part : INPUT/OUTPUT : PORT : BIT
	
	// WAIT TIME 관련정보......
	int				m_nWaitTimeMaxCnt;
	int				m_nOnUsed[100];
	int				m_nOffUsed[100];
	int				m_nLimitUsed[100];

	CXLAutomation	*m_clsExcel;
public:
	CAlg_Excel();
	~CAlg_Excel();
	bool OnOpenIOMap(CString strFilename);
	bool OnCloseIOMap();
	bool OnOpenPartIOMap(CString strFilename);
	bool OnClosePartIOMap();
	bool OnOpenMotorPart(CString strFilename);
	bool OnCloseMotorPart();
	bool OnOpenMotorPosition(CString strFilename);
	bool OnCloseMotorPosition();
	bool OnOpenWaitTime(CString strFilename);
	bool OnCloseWaitTime();
};
extern CAlg_Excel clsExcel;

