// CtlBd_Library.h: interface for the CCtlBd_Library class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTLBD_LIBRARY_H__95978C35_3D8D_44B9_A9DF_E9F5E1C39135__INCLUDED_)
#define AFX_CTLBD_LIBRARY_H__95978C35_3D8D_44B9_A9DF_E9F5E1C39135__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CCtlBd_Library  
{
public:
	CCtlBd_Library();
	virtual ~CCtlBd_Library();

public:		
	CString str_Motor_Msg;//20150428 james 
	
	CString m_strMotorAlarmCode;

	int  m_nMotRetry_Cnt[100];
	int  m_nStep_Single[100];
	int  m_nStep_Linear[100];
	int  m_nLinearRetry_Cnt[100];

	DWORD m_dwUntil_Wait[100][3];

	char mc_normal_msg[1000]; //��񿡼� �߻��ϴ� �޼����� �����Ѵ� 

	int mn_run_status;	//����� ���� ���� ���� 
	
	char mc_alarmcode[10]; //alarm code ����
	int mn_alarm_type;	//�˶��� ������ ����[��)operator error ����, maint �� error ���� ����]
	int mn_alarm_count; //�˶� ī��Ʈ ���� ���� [0:ī��Ʈ���� ����, 1:ī��Ʈ ��]
	int mn_jampos; //jam �� �߻��� ��ġ�� ������ �����Ѵ� 

	int m_nMcEmg;
	int	mn_empty_sd_flag_1;
	int	mn_empty_sd_flag_2;
	int	mn_reject_sd_flag;
	int mn_unload_sd_flag;
	int mn_cart_sd_flag;

	double md_empty_sd_pos_1;
	double md_empty_sd_pos_2;
	double md_reject_sd_pos;
	double md_unload_sd_pos;
	double md_cart_sd_pos;

	double m_dMvOldPos[100];	// motor safety old pos;
	int	   m_nRetryCnt[100];

	DWORD  m_dwJigElvChk[3];
	bool   m_bJigElvChk;

public:
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 20150428 james
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	int HomeCheck_Mot(int n_Axis, int n_HomeMode, int n_TimeOut);
	int Debug_File_Create(int n_mode,  char* s_msg); //2012.1029 jamestest  CString s_msg)CString s_msg);
	int FileSizeCheck(CString s_filename, long l_size, int n_check);
	void OnStringToChar(CString strData, char chData[]);
	void Motor_Error_Occurrence(int n_Mode, long l_Axis, long l_ErrorIDNum, CString strErrorMsg); //20150428 james �˶����� ���� ���� 
	/////////////////////////// 


	
	int SD_Sensor_Enable(int n_Mode, int n_AxisNum, int n_Enable);

	int Linear_Move(int n_RunMethod, int n_LinearMapIndex, long l_AxisCnt, long *lp_AxisNum, double *dp_TargetPosList, int n_SpeedRate) ;
	int Single_Move(int n_RunMethod, int n_MotorNum, double d_MovePos, int n_SpeedRate=100);	// �Էµ� ��ġ�� ���� �̵�
	///////////////////////////////////////////////////

	int IO_Safety(int n_io, int n_onoff);
	BOOL CreateFolder(LPCTSTR sz_path);  //folder ���� �ʿ�� ��� 
	int FileSizeCheck(char * cp_filename, int n_size, int n_check);
	int LogFile_Write(char * cp_dir_path, char * cp_msg);

	
	int Initialize_motor_board(int n_bd_type, CString s_filename);	//motor ���� �ʱ�ȭ�� ��� 
	int Initialize_io_Board(int n_bd_type);	//i/o ���� �ʱ�ȭ�� ��� 

	int Motor_SafetyCheck(int n_mode, int n_axis, double d_targetpos);	//���� ���� ���� ���ý� ��� 
	int Alarm_Error_Occurrence(int nJamPos, int nRunStatus, CString strCode); //��� �˶� �߻���ų�� ����ϴ� ���� 

	int OnMotorGoCheck(int n_motor, double d_pos);



	//	int alarm_func(int n_mode, char * cp_jamcode, char * cp_msg);
	//int char_msg_func(int n_mode, char * cp_source_msg); //2011.0910 

};
extern CCtlBd_Library CTL_Lib;


#endif // !defined(AFX_CTLBD_LIBRARY_H__95978C35_3D8D_44B9_A9DF_E9F5E1C39135__INCLUDED_)
