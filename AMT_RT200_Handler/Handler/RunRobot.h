#pragma once
#include "Variable.h"

class CRunRobot
{
public:
	CRunRobot(void);
	~CRunRobot(void);
	
	
public:
	int				m_nConvStep;
	int				m_nRunStep;
	int             m_nFeederStep;
	int				m_nInitStep;
	int				m_nRdimmJobState;

	int				m_npTemp_Picker_YesNo[MAX_PICKCNT]; 
	int				m_nLinearMove_Index;
	int             m_nPickCnt;
	int             m_nPickerNum;
	int             mn_Retry;
	
	int             m_nCnt;
	int             m_nVaccumCnt;
	int             m_nRobot_Z;
	int             m_nRobot_T;
	int             m_nRobot_P;
	int             m_nRobot_X;
	int             m_nRobot_Y;
	int             m_nCntBcrNum;
	int             m_nMove_Flag[5];
	int             m_nJobNextPos; //kwlee 2016.1229
	
	int             m_nPrintOutCheck;
	int             m_nBarcodeReadCheck[2];
	int             m_nBcrRetryCnt[2];
	//kwlee 2017.0204
	int             m_nPrintOutPutCnt;
	int             m_nEmptyCntBufferCnt;
	int             m_nInterFaceStep;
	int             m_nLabelReq;
	int             m_nLabelFailCheck;

	bool			m_bLabelFeederNum;
	
	//kwlee 2017.0307
	int            m_nCurrLabelChk;
	int            m_nCheckStep;
	int            m_nVisionPos;
	//kwlee 2017.0315
	int            m_nVisionStep; 
	int            m_nVisionRetry;
	//
	int            m_nSodimmSecondPlace;
	CString         m_strBarcode[2];
	CString         m_strPrintSerial[2];
	CString			m_strAlarmCode;
		
	double          m_dPitch_x;
	double          m_dPitch_Y;
	double			m_dpTargetPosList[4];
	double			m_dpSpdRatio[3]; 
	
	
	DWORD           m_dwTimeCheck[3];
	DWORD           m_dwBcrTime[3];
	

	DWORD           m_dwCheckOnOffTime[3]; //kwlee 2017.0307
	DWORD			m_dwPickUpDnWaitTime[2][3];
	DWORD			m_dwWRejectCylWaitTime[2][3]; //kwlee 2017.0216
	DWORD           m_dwLabelOutTime[3]; //kwlee 2017.0307
	DWORD           m_dwClampUpDnWaitTime[2][3];
	DWORD           m_dwStopperCylWaitTime[3];
	DWORD           m_dwBcrConvMotorWaitTime[3];
	
	DWORD           m_dwTacTimeCheck[3]; //kwlee2017.0328

	bool            m_bSecondPos;
	bool            m_PickSecondPOs;
	bool			m_bPickUpDnFlag[2];
	bool			m_bCylFlag[2];

	long			m_lAxisCnt;              //현재의 IndexNum에서 사용 가능한 모터 수량 최대 4개  
	long			m_lpAxisNum[4];	         //현재의 IndexNum에서 사용하는 모터 실재 번호를 가진다 
	long            m_lFeederWaitTime[3];
	long            m_lVaccumOffTime[3];
	long            m_lTurnConvWaitTime[3];
	
public:
	void	OnThreadRUN();
	void	OnRunInit();
	void    OnRobotRun();
	void	OnSetPickerUpDn(int nMode, int nOnOff, int *npPickerInfo);
	int		OnGetPickerUpDn(int nMode, int nOnOff, int *npPickerInfo);
	void    OnSetRejectCylUpDn(int nMode, int nUpDn);
	int     OnGetRejectCylUpDn(int nMode, int nUpDn);

	//kwlee 2017.0307
	void    OnSetFeederClampCylUpDn(int nMode, int nOnOff);
	int     OnGetFeederClampCylUpDn(int nMode, int nOnOff);

	int	    OnFeederInterface();
	
	void    OnFeederReq(int nStep);
	void    OnSelectPickType(int nType);
	int     GetIntefaceInfo(int mStep) { return m_nInterFaceStep == mStep; }
	void    SetInterfaceStep(int mStep) { m_nInterFaceStep = mStep; }
	
	void    OnSetLabelPick(int nMode, int nPickCnt);
	
	void    OnSetLabelPlace(int nPickCnt);
	void    OnVaccummSet(int nMode, int nPickCnt,int OnOff);
	void    OnBlowSet(int nMode, int nPickCnt , int OnOff);
	void    OnGetPickCheck(int nMode, int nPickCnt);
	int     OnGetVaccumgmCheck(int OnOff,int nPickCnt);
	void    OnDataExchange(int nMode, int nPickPlace);
	int     OnPrinterFeeder(int nCnt, int nFailCheck);
    int		OnBufferDataTransfer(int nCnt);
	int     OnFailLabelCheck();
	void    OnBarcodeReadCheck();
	void    OnResetRejectData();

	void    OnLabelSensorCheck(); //kwlee 2017.0307
	int     OnVisionDataCheck(int nPos);//kwlee 2017.0315
	
	int		OnBcrConveyorOnOff(int nOnOff);
};

extern CRunRobot	clsRunRobot;