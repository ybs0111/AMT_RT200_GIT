#pragma once
#include "Variable.h"

class CRunConveyor
{
public:
	CRunConveyor(void);
	~CRunConveyor(void);

public:
	int m_nRobot_Conv;
	int m_nRunStep[3];
	int m_nInitStep;
	int m_nWaitMode;
	int m_nSmemaStep;
	int m_nRearSmemaStep;
	int m_nPosCheckStep;
	int m_nErrorStep;
	int m_TurnConvPosCheck[2];
	int m_nSmemaFlag;

	CString m_strAlarmCode;

	bool				m_bWStopperFlag[2];
	bool				m_bBStopperFlag[2];
	DWORD			m_dwWStopperWaitTime[2][3];
	

	DWORD           m_dwConveyorWaitTime[3][3];
	long           m_lMotorOnTimeCheck[3];
	long           m_lWait_Smema[3];

	bool			m_bCylFlag[2];
	DWORD			m_dwCylWaitTime[2][3];
	DWORD			m_dwWaitTime[3];

	bool			m_bSensorOnFlag;

public:
	void	OnThreadRUN();
	void	OnFrontConvRunMove();
	void    OnTurnConvMove();
	void    OnRearConvMove();
	void    Smema_Rear();
	void    Smema_Front();
	int    OnTurnConvPosCheck();
	int    OnConvPcvCheck();
	void   OnPcbDataReset();

	void    OnRunInit();
	void    OnPcbType(int nType);
	void    OnSetInConvStopperUpDn(int nMode, int nUpDn);
	int    OnGetInConvStopperUpDn(int nMode, int nUpDn);

	void    OnSetTurnConvStopperUpDn(int nMode, int nUpDn);
	int    OnGetTurnConvStopperUpDn(int nMode, int nUpDn);

	void    OnSetOutConvStopperUpDn(int nMode, int nUpDn);
	int    OnGetOutConvStopperUpDn(int nMode, int nUpDn);
	
	void    OnSetTableMotorCylOnOff(int nMode, int nUpDn);
	int    OnGetTableMotorCylOnOff(int nMode, int nUpDn);

	void    OnSetTableTurnCylOnOff(int nMode, int nUpDn);
	int    OnGetTableTurnCylOnOff(int nMode, int nUpDn);

	void   OnPickerWork();

	int    OnConvWaitTime(int nConv, int nWaitMode);
};

extern CRunConveyor	clsRunConveyor;