#include "Variable.h"
#include <afxtempl.h>

#pragma once

class CPublicFunction : public CObject
{
public:
	// jtkim 20160107
	void			OnDailyCycleData(CString strLotNo, CString strPartNo, double dTime, CTime tStart, CTime tEnd);
	// jtkim 20150929
	CString			OnRecipeCheck(CString strPartNo);
	int				OnRecipeMatch(CString strFileName, CString strPartNo);
	int				OnRecoveryReadWrite(int nMode, int nReadWrite);

	CFont			*m_pFont[20];

	CString			m_strThreadStep[100];
	CTime			m_tThreadTime[100];

	CArray			<tagLOT_HISTORY_INFO, tagLOT_HISTORY_INFO&>	m_ArLotHistory;
public:
	CPublicFunction(void);
	~CPublicFunction(void);
	LOGFONT			OnLogFont(int nFontSize);
	void			OnLogFileAdd(int nMsgSelect, CString strMsg);
	int				OnFileSizeCheck(CString strFileName, long lSize, int nCheck);
	int				OnLightCurtainCheck();
	int				OnDoorOpenCheck();
	void			OnDoorOpen();			// jtkim 20130625
	void			OnDoorClose();			// jtkim 20130625
	int				OnMpCheck();
	int				OnEncoderCheck(double dPos);
	bool			OnCreateFolder(CString strPath);
	int				OnEmoCheck();
	int				OnMainAirCheck();
	int				OnMainPowerCheck();
	int				OnMotorPowerCheck();
	int				OnIoSafetyCheck(int nIO);
	void			OnLotStartClear();
	BOOL			IsNum(CString strData);
	// ************************************
	// 장비 상태 변경 함수이다. 
	// ************************************
	void			OnMCRun();				// jtkim 20130625
	void			OnMCStop();				// jtkim 20130625
	void			OnSound(int nOnOff);	// jtkim 20130625
	// ************************************
	void			OnLogBarcode(CString strMsg);
	void			OnLogBarcodeNg(CString strMsg);
	void			OnSerialize(CArchive &ar);
	void			OnDailyDataSave();
	void			OnHourDataSave(CString strHourName, int nQty, double dSocketYield, int nStart, int nOn, int nOff, CString strOff, CString strOn);
	void			OnTimeReset();
	void			OnInitVariable();		// jtkim 20130625
	void			OnIOPortClear();
	void			OnMotorSpeed();
	void			OnAxisEncoderReset(int nAxis);
	void			OnThreadFunctionStepTrace(int nPos, int nStep);
	CString			OnCharToString(char chData[], int nLength);
	void			OnStringToChar(CString strData, char chData[]);
	CString			ThreadID(int nID); //2015.02.10;

	

public:

};

extern CPublicFunction clsFunc;
