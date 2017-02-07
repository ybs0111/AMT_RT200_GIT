// CtlBd_Function.h: interface for the CCtlBd_Function class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTLBD_FUNCTION_H__42B7D6CE_6969_44C4_90D5_6819F722BB71__INCLUDED_)
#define AFX_CTLBD_FUNCTION_H__42B7D6CE_6969_44C4_90D5_6819F722BB71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCtlBd_Function  
{
public:
	CCtlBd_Function();
	virtual ~CCtlBd_Function();

public:
	CString ms_ErrMsg;

	WORD mw_IOMaskMap[4][2][8][16]; //master, port, slave, i/o bit



public:
	int Robot_Clash_Safety_Check(int n_Mode, int n_SourceAxis, int n_ReferenceAxis, double d_Safety_Gap, double d_TargetPos);

	int MotorSpeed_Control(int n_Axis, double d_CurrentPos, double d_TargetPos, double * dp_Speed);
	int Alarm_Error_Occurrence(int n_Count, int n_Type, int n_Status, CString s_JamCode);

	int Board_Initialize(int n_mode);
	int Send_Error_Message(int n_Mode, int n_Axis, CString s_JamCode, CString s_JamMsg);

	int OnMotor_SafetyCheck(int nMode, int nAxis, double dTargetPos);

	int Initialize_FASIO_Board(void);

	int Initialize_InterruptMask_TriggerConfig(void) ;



};
extern CCtlBd_Function  CtlBdFunc;



#endif // !defined(AFX_CTLBD_FUNCTION_H__42B7D6CE_6969_44C4_90D5_6819F722BB71__INCLUDED_)
