// MyBasicData.cpp: implementation of the CMyBasicData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "MyBasicData.h"
#include "Ctlbd_Variable.h"
#include "PublicFunction.h"
#include "FastechPublic_IO.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CMyBasicData clsBasic;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMyBasicData::CMyBasicData()
{

}

CMyBasicData::~CMyBasicData()
{

}

void CMyBasicData::OnPassWord_Load(CString strLoadLevel)
{
	CString strPass;
	CString strLoadFile;
	TCHAR chPass[25];

	(st_other_info.strPassword).Empty();		// 암호 저장 변수 초기화

	strLoadFile = st_path_info.strBasic + st_basic_info.strDeviceName;

	if(strLoadLevel == _T("SysLock"))			// SYSTEM LOCK 암호
	{
		:: GetPrivateProfileString(_T("Password"), _T("SysLock"), _T("M"), chPass, 20, strLoadFile);

	}
	else if (strLoadLevel == _T("Level1"))	// 메인트 암호
	{
		:: GetPrivateProfileString(_T("Password"), _T("Level_1"), _T("M"), chPass, 20, strLoadFile);
	}
	else if (strLoadLevel == _T("Level2"))	// 티칭 암호
	{
		:: GetPrivateProfileString(_T("Password"), _T("Level_2"), _T("T"), chPass, 20, strLoadFile);
	}
	else  
	{
		return;
	}

	strPass.Format(_T("%s"), chPass);
	strPass.MakeUpper();     // 문자열 대문자로 변경
	strPass.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	strPass.TrimRight(' ');  // 우측 문자열 공백 제거

	st_other_info.strPassword = strPass;  // 암호 전역 변수에 설정
}

void CMyBasicData::OnPassWord_Save(CString strSaveLevel, CString strPassData)
{
	CString strSaveFile;
	
	strSaveFile = st_path_info.strBasic + st_basic_info.strDeviceName;

	if (strSaveLevel=="SysLock")         // SYSTEM LOCK 암호
	{
		:: WritePrivateProfileString(_T("Password"), _T("SysLock"), LPCTSTR(strPassData), strSaveFile);
	}
	else if (strSaveLevel=="Level1")  // 메인트 암호
	{
		:: WritePrivateProfileString(_T("Password"), _T("Level_1"), LPCTSTR(strPassData), strSaveFile);
	}
	else if (strSaveLevel=="Level2")  // 티칭 암호
	{
		:: WritePrivateProfileString(_T("Password"), _T("Level_2"), LPCTSTR(strPassData), strSaveFile);
	}
}

CString CMyBasicData::OnStep_File_Index_Load()
{
	CString strLoadFile;
	char chLoad[20];

	strLoadFile = st_path_info.strBasic + st_basic_info.strDeviceName;

	:: GetPrivateProfileString(_T("Thread_Step_file"), _T("File_Index"), _T("00"), (LPWSTR)chLoad, 20, strLoadFile);
//	sprintf(chLoad,"%S", chLoad);
	strLoadFile = chLoad;
	strLoadFile.TrimLeft(' ');               
	strLoadFile.TrimRight(' ');

	return strLoadFile;  // 파일 인덱스 리턴
}

void CMyBasicData::OnStep_File_Index_Save(CString strIndex)
{
	CString strSaveFile;

	strSaveFile = st_path_info.strBasic + st_basic_info.strDeviceName;

	:: WritePrivateProfileString(_T("Thread_Step_file"), _T("File_Index"), LPCTSTR(strIndex), strSaveFile);
}

CString CMyBasicData::OnStep_File_Name_Load()
{
	CString strFileName;  // 파일명 저장 변수
	CString strLoadFile;
	char chLoad[20];

	strLoadFile = st_path_info.strBasic + st_basic_info.strDeviceName;

	:: GetPrivateProfileString(_T("Thread_Step_file"), _T("File_Name"), _T("DEFAULT"),(LPWSTR)chLoad, 20, strLoadFile);
//	sprintf(chLoad,"%S", chLoad);
	strFileName = chLoad;
	strFileName.TrimLeft(' ');               
	strFileName.TrimRight(' ');

	return strFileName;  // 파일명 리턴
}

void CMyBasicData::OnStep_File_Name_Save(CString strFile)
{
	:: WritePrivateProfileString(_T("Thread_Step_file"), _T("File_Name"), LPCTSTR(strFile), st_path_info.strBasic);
}

void CMyBasicData::OnMotorSpeed_Set_Data_Load()
{
	int  nChk=0, i=0;
	double dChk;
	CString strTemp;       // 임시 저장 변수
	CString strMotorName;
	CString strLoadFile;	
	char chData[20];
	
	for(i = 0; i < MAXMOTOR; i++)
	{
		strMotorName.Format(_T("%02d_AXIS_SPEED"), i+1);
		:: GetPrivateProfileString(strMotorName, _T("ACC"), _T("100"), (LPWSTR)chData, 10, st_path_info.strFileBasic);
		strTemp.Format(_T("%s"), chData);
		dChk = _wtof(strTemp);
		st_motor_info[i].d_accel[0] = dChk;
		st_motor_info[i].d_spd_vel[1] = dChk;
		
		:: GetPrivateProfileString(strMotorName, _T("DEC"), _T("100"), (LPWSTR)chData, 10, st_path_info.strFileBasic);
		strTemp.Format(_T("%s"), chData);
		dChk = _wtof(strTemp);
		st_motor_info[i].d_decel[0] = dChk;
		st_motor_info[i].d_spd_vel[2] = dChk;
		
		:: GetPrivateProfileString(strMotorName, _T("MAX"), _T("100000"), (LPWSTR)chData, 10, st_path_info.strFileBasic);
		strTemp.Format(_T("%s"), chData);
		dChk = _wtof(strTemp);
		st_motor_info[i].d_spd_max[0] = dChk;
		
		:: GetPrivateProfileString(strMotorName, _T("VEL"), _T("1000"), (LPWSTR)chData, 10, st_path_info.strFileBasic);
		strTemp.Format(_T("%s"), chData);
		dChk = _wtof(strTemp);
		st_motor_info[i].d_spd_vel[0] = dChk;
		
		:: GetPrivateProfileString(strMotorName, _T("HOME"), _T("500"), (LPWSTR)chData, 10, st_path_info.strFileBasic);
		strTemp.Format(_T("%s"), chData);
		dChk = _wtof(strTemp);
		st_motor_info[i].d_spd_home[0] = dChk;
		
		:: GetPrivateProfileString(strMotorName, _T("JOG"), _T("300"), (LPWSTR)chData, 10, st_path_info.strFileBasic);
		strTemp.Format(_T("%s"), chData);
		dChk = _wtof(strTemp);
		st_motor_info[i].d_spd_jog[0] = dChk;
		
		:: GetPrivateProfileString(strMotorName, _T("VEL_PER"), _T("50"), (LPWSTR)chData, 10, st_path_info.strFileBasic);
		strTemp.Format(_T("%s"), chData);
		nChk = _wtoi(strTemp);
		st_motor_info[i].n_per_vel = nChk;
		
		:: GetPrivateProfileString(strMotorName, _T("HOME_PER"), _T("5"), (LPWSTR)chData, 10, st_path_info.strFileBasic);
		strTemp.Format(_T("%s"), chData);
		nChk = _wtoi(strTemp);
		st_motor_info[i].n_per_home = nChk;
		
		:: GetPrivateProfileString(strMotorName, _T("JOG_PER"), _T("5"), (LPWSTR)chData, 10, st_path_info.strFileBasic);
		strTemp.Format(_T("%s"), chData);
		nChk = _wtoi(strTemp);
		st_motor_info[i].n_per_jog = nChk;
		
		:: GetPrivateProfileString(strMotorName, _T("ALLOW"), _T("100"), (LPWSTR)chData, 10, st_path_info.strFileBasic);
		strTemp.Format(_T("%s"), chData);
		dChk = _wtof(strTemp);
		st_motor_info[i].d_allow = dChk;

		:: GetPrivateProfileString(strMotorName, _T("LIMIT_M"), _T("0"), (LPWSTR)chData, 10, st_path_info.strFileBasic);
		strTemp.Format(_T("%s"), chData);
		dChk = _wtof(strTemp);
		st_motor_info[i].d_limit_position[0] = dChk;

		:: GetPrivateProfileString(strMotorName, _T("LIMIT_P"), _T("3000"), (LPWSTR)chData, 10, st_path_info.strFileBasic);
		strTemp.Format(_T("%s"), chData);
		dChk = _wtof(strTemp);
		st_motor_info[i].d_limit_position[1] = dChk;
	}
	
	//Speed Rate
	:: GetPrivateProfileString(_T("SPEED_RATE"), _T("RUN"), _T("100"), (LPWSTR)chData, 10, st_path_info.strFileBasic);
	strTemp.Format(_T("%s"), chData);
	nChk = _wtoi(strTemp);
	st_handler_info.nRunSpeed = nChk;
	
	:: GetPrivateProfileString(_T("SPEED_RATE"), _T("MANUAL"), _T("80"), (LPWSTR)chData, 10, st_path_info.strFileBasic);
	strTemp.Format(_T("%s"), chData);
	nChk = _wtoi(strTemp);
	st_handler_info.nManualSpeed = nChk;
}

void CMyBasicData::OnMotorSpeed_Set_Data_Save()
{
	CString strTemp;       // 임시 저장 변수
	CString strMotorName;
	CString strSaveFile;
//	char chBuf[20];

	for(int i = 0; i < MAXMOTOR; i++)
	{
		strMotorName.Format(_T("%02d_AXIS_SPEED"), i+1);
//		LPCTSTR(_itoa((int)(st_motor_info[i].d_accel[0]), chBuf, 10));
		strTemp.Format(_T("%.3f"), st_motor_info[i].d_accel[0]);
		:: WritePrivateProfileString(strMotorName, _T("ACC"), LPCTSTR(strTemp), st_path_info.strFileBasic);
		
//		LPCTSTR(_itoa((int)(st_motor_info[i].d_decel[0]), chBuf, 10));
//		strTemp = chBuf;
		strTemp.Format(_T("%.3f"), st_motor_info[i].d_decel[0]);
		:: WritePrivateProfileString(strMotorName, _T("DEC"), LPCTSTR(strTemp), st_path_info.strFileBasic);
		
//		LPCTSTR(_itoa((int)(st_motor_info[i].d_spd_max[0]), chBuf, 10));
//		strTemp = chBuf;
		strTemp.Format(_T("%.3f"), st_motor_info[i].d_spd_max[0]);
		:: WritePrivateProfileString(strMotorName, _T("MAX"), LPCTSTR(strTemp), st_path_info.strFileBasic);
		
//		LPCTSTR(_itoa((int)(st_motor_info[i].d_spd_vel[0]), chBuf, 10));
//		strTemp = chBuf;
		strTemp.Format(_T("%.3f"), st_motor_info[i].d_spd_vel[0]);
		:: WritePrivateProfileString(strMotorName, _T("VEL"), LPCTSTR(strTemp), st_path_info.strFileBasic);
		
//		LPCTSTR(_itoa((int)(st_motor_info[i].d_spd_home[0]), chBuf, 10));
//		strTemp = chBuf;
		strTemp.Format(_T("%.3f"), st_motor_info[i].d_spd_home[0]);
		:: WritePrivateProfileString(strMotorName, _T("HOME"), LPCTSTR(strTemp), st_path_info.strFileBasic);
		
//		LPCTSTR(_itoa((int)(st_motor_info[i].d_spd_jog[0]), chBuf, 10));
//		strTemp = chBuf;
		strTemp.Format(_T("%.3f"), st_motor_info[i].d_spd_jog[0]);
		:: WritePrivateProfileString(strMotorName, _T("JOG"), LPCTSTR(strTemp), st_path_info.strFileBasic);
		
//		LPCTSTR(_itoa((st_motor_info[i].n_per_vel), chBuf, 10));
//		strTemp = chBuf;
		strTemp.Format(_T("%d"), st_motor_info[i].n_per_vel);
		:: WritePrivateProfileString(strMotorName, _T("VEL_PER"), LPCTSTR(strTemp), st_path_info.strFileBasic);
		
//		LPCTSTR(_itoa((st_motor_info[i].n_per_home), chBuf, 10));
//		strTemp = chBuf;
		strTemp.Format(_T("%d"), st_motor_info[i].n_per_home);
		:: WritePrivateProfileString(strMotorName, _T("HOME_PER"), LPCTSTR(strTemp), st_path_info.strFileBasic);
		
//		LPCTSTR(_itoa((st_motor_info[i].n_per_jog), chBuf, 10));
//		strTemp = chBuf;
		strTemp.Format(_T("%d"), st_motor_info[i].n_per_jog);
		:: WritePrivateProfileString(strMotorName, _T("JOG_PER"), LPCTSTR(strTemp), st_path_info.strFileBasic);
		

		strTemp.Format(_T("%.2f"), st_motor_info[i].d_allow);
		:: WritePrivateProfileString(strMotorName, _T("ALLOW"), LPCTSTR(strTemp), st_path_info.strFileBasic);
		
//		LPCTSTR(_itoa((int)(st_motor_info[i].d_limit_position[0]), chBuf, 10));
//		strTemp = chBuf;
		strTemp.Format(_T("%.3f"), st_motor_info[i].d_limit_position[0]);
		:: WritePrivateProfileString(strMotorName, _T("LIMIT_M"), LPCTSTR(strTemp), st_path_info.strFileBasic);

//		LPCTSTR(_itoa((int)(st_motor_info[i].d_limit_position[1]), chBuf, 10));
//		strTemp = chBuf;
		strTemp.Format(_T("%.3f"), st_motor_info[i].d_limit_position[1]);
		:: WritePrivateProfileString(strMotorName, _T("LIMIT_P"), LPCTSTR(strTemp), st_path_info.strFileBasic);
	}
	
	// Speed Rate
//	LPCTSTR(_itoa((int)(st_handler_info.nRunSpeed), chBuf, 10));
//	strTemp = chBuf;
	strTemp.Format(_T("%d"), st_handler_info.nRunSpeed);
	:: WritePrivateProfileString(_T("SPEED_RATE"), _T("RUN"), LPCTSTR(strTemp), st_path_info.strFileBasic);
	
//	LPCTSTR(_itoa((int)(st_handler_info.nManualSpeed), chBuf, 10));
//	strTemp = chBuf;
	strTemp.Format(_T("%d"), st_handler_info.nManualSpeed);
	:: WritePrivateProfileString(_T("SPEED_RATE"), _T("MANUAL"), LPCTSTR(strTemp), st_path_info.strFileBasic);
}

void CMyBasicData::OnWaitTime_Data_Load()
{
	CString strTemp, strMsg, strHead;       // 임시 저장 변수
	CString strLoadFile, strOnName, strOffName, strLimitName;
	int nChk, i;
	char chData[20];

	for(i=0; i<MAX_WAIT_TIME; i++)
	{
		strHead.Format(_T("TIME_[%02d]"), i);
		
		strOnName.Format(_T("%s_ON"), strHead);
		:: GetPrivateProfileString(_T("ON_WAIT_TIME"), strOnName, _T("100"), (LPWSTR)chData, 10, st_path_info.strFileBasic);
		strTemp.Format(_T("%s"), chData);
		nChk = _wtoi(strTemp);
		st_wait_info.nOnWaitTime[i] = nChk;
		
		strOffName.Format(_T("%s_OFF"), strHead);
		:: GetPrivateProfileString(_T("OFF_WAIT_TIME"), strOffName, _T("100"), (LPWSTR)chData, 10, st_path_info.strFileBasic);
		strTemp.Format(_T("%s"), chData);
		nChk = _wtoi(strTemp);
		st_wait_info.nOffWaitTime[i] = nChk;
		
		strLimitName.Format(_T("%s_LIMIT"), strHead);
		:: GetPrivateProfileString(_T("LIMIT_TIME"), strLimitName, _T("100"), (LPWSTR)chData, 10, st_path_info.strFileBasic);
		strTemp.Format(_T("%s"), chData);
		nChk = _wtoi(strTemp);
		st_wait_info.nLimitWaitTime[i] = nChk;
	}
}

void CMyBasicData::OnWaitTime_Data_Save()
{
	CString strTemp, strPart;  // 로딩 정보 임시 저장 변수
	CString strSaveFile;
	CString strOnName, strOffName, strLimitName;
	CString strData;
	int i;
	
	for(i=0; i<MAX_WAIT_TIME; i++)
	{
		strTemp.Format(_T("TIME_[%02d]"), i);
		
		strOnName.Format(_T("%s_ON"), strTemp);
		strData.Format(_T("%d"), st_wait_info.nOnWaitTime[i]);
		:: WritePrivateProfileString(_T("ON_WAIT_TIME"), strOnName, LPCTSTR(strData), st_path_info.strFileBasic);
		
		strOffName.Format(_T("%s_OFF"), strTemp);
		strData.Format(_T("%d"), st_wait_info.nOffWaitTime[i]);
		:: WritePrivateProfileString(_T("OFF_WAIT_TIME"), strOffName, LPCTSTR(strData), st_path_info.strFileBasic);
		
		strLimitName.Format(_T("%s_LIMIT"), strTemp);
		strData.Format(_T("%d"), st_wait_info.nLimitWaitTime[i]);
		:: WritePrivateProfileString(_T("LIMIT_TIME"), strLimitName, LPCTSTR(strData), st_path_info.strFileBasic);	
	}
}

void CMyBasicData::OnMaintenance_Data_Load()
{
	CString strTemp, strPart;  // 로딩 정보 임시 저장 변수
	int nChk;
	CString strLoadFile;
	
	char chData[20];
	
	// **************************************************************************
	// 타워 램프 RED 상태 로딩하여 전역 변수에 설정한다                          
	// ->  STOP			= 0,
	// 		   RUN			= 1,
	// 		   ALARM		= 2,
	// 		   LOTEND		= 3,
	// 		   INIT			= 4,
	// 		   WARRING		= 5,
	// 		   LOCK			= 6,
	// 		   SELFCHECK	= 7														 
	// -> 로딩 값 [0:OFF    1:ON    2:FLICKER]                                   
	// **************************************************************************
//	str_load_file = st_path.str_basic + st_basic.str_device_name;  // 티칭 데이터 저장 파일 설정
	strLoadFile = st_path_info.strBasic + _T("Maintenance.TXT");  // 티칭 데이터 저장 파일 설정

	:: GetPrivateProfileString(_T("TowerLampData_Red"), _T("Stop_State"), _T("0"), (LPWSTR)chData, 10, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	nChk = _wtoi(strTemp);
	if (nChk<0 || nChk>2)
	{
		st_lamp_info.nLampR[0] = 0;
	}
	else
	{
		st_lamp_info.nLampR[0] = nChk;
	}
	
	:: GetPrivateProfileString(_T("TowerLampData_Red"), _T("Run_State"), _T("0"), (LPWSTR)chData, 10, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	nChk = _wtoi(strTemp);
	if (nChk<0 || nChk>2)
	{
		st_lamp_info.nLampR[1] = 0;
	}
	else
	{
		st_lamp_info.nLampR[1] = nChk;
	}
	
	:: GetPrivateProfileString(_T("TowerLampData_Red"), _T("Alarm_State"), _T("0"), (LPWSTR)chData, 10, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	nChk = _wtoi(strTemp);
	if (nChk<0 || nChk>2)
	{
		st_lamp_info.nLampR[2] = 2;
	}
	else
	{
		st_lamp_info.nLampR[2] = nChk;
	}

	:: GetPrivateProfileString(_T("TowerLampData_Red"), _T("LotEnd_State"), _T("0"), (LPWSTR)chData, 10, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	nChk = _wtoi(strTemp);
	if (nChk<0 || nChk>2)
	{
		st_lamp_info.nLampR[3] = 2;
	}
	else
	{
		st_lamp_info.nLampR[3] = nChk;
	}

	:: GetPrivateProfileString(_T("TowerLampData_Red"), _T("Initial_State"), _T("0"), (LPWSTR)chData, 10, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	nChk = _wtoi(strTemp);
	if (nChk<0 || nChk>2)
	{
		st_lamp_info.nLampR[4] = 2;
	}
	else
	{
		st_lamp_info.nLampR[4] = nChk;
	}

	:: GetPrivateProfileString(_T("TowerLampData_Red"), _T("Warring_State"), _T("0"), (LPWSTR)chData, 10, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	nChk = _wtoi(strTemp);
	if (nChk<0 || nChk>2)
	{
		st_lamp_info.nLampR[5] = 2;
	}
	else
	{
		st_lamp_info.nLampR[5] = nChk;
	}

	:: GetPrivateProfileString(_T("TowerLampData_Red"), _T("Lock_State"), _T("0"), (LPWSTR)chData, 10, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	nChk = _wtoi(strTemp);
	if (nChk<0 || nChk>2)
	{
		st_lamp_info.nLampR[6] = 2;
	}
	else
	{
		st_lamp_info.nLampR[6] = nChk;
	}

	:: GetPrivateProfileString(_T("TowerLampData_Red"), _T("SelfCheck_State"), _T("0"), (LPWSTR)chData, 10, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	nChk = _wtoi(strTemp);
	if (nChk<0 || nChk>2)
	{
		st_lamp_info.nLampR[7] = 2;
	}
	else
	{
		st_lamp_info.nLampR[7] = nChk;
	}

	:: GetPrivateProfileString(_T("TowerLampData_Red"), _T("Idle_State"), _T("0"), (LPWSTR)chData, 10, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	nChk = _wtoi(strTemp);
	if (nChk<0 || nChk>2)
	{
		st_lamp_info.nLampR[8] = 2;
	}
	else
	{
		st_lamp_info.nLampR[8] = nChk;
	}
	// **************************************************************************


	// **************************************************************************
	// 타워 램프 YELLOW 상태 로딩하여 전역 변수에 설정한다                       
	// ->  STOP			= 0,
// 		   RUN			= 1,
// 		   ALARM		= 2,
// 		   LOTEND		= 3,
// 		   INIT			= 4,
// 		   WARRING		= 5,
// 		   LOCK			= 6,
// 		   SELFCHECK	= 7														 
	// -> 로딩 값 [0:OFF    1:ON    2:FLICKER]                                   
	// **************************************************************************
	:: GetPrivateProfileString(_T("TowerLampData_Yellow"), _T("Stop_State"), _T("0"), (LPWSTR)chData, 10, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	nChk = _wtoi(strTemp);
	if (nChk<0 || nChk>2)
	{
		st_lamp_info.nLampY[0] = 0;
	}
	else
	{
		st_lamp_info.nLampY[0] = nChk;
	}

	:: GetPrivateProfileString(_T("TowerLampData_Yellow"), _T("Run_State"), _T("0"), (LPWSTR)chData, 10, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	nChk = _wtoi(strTemp);
	if (nChk<0 || nChk>2)
	{
		st_lamp_info.nLampY[1] = 1;
	}
	else
	{
		st_lamp_info.nLampY[1] = nChk;
	}

	:: GetPrivateProfileString(_T("TowerLampData_Yellow"), _T("Alarm_State"), _T("0"), (LPWSTR)chData, 10, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	nChk = _wtoi(strTemp);
	if (nChk<0 || nChk>2)
	{
		st_lamp_info.nLampY[2] = 0;
	}
	else
	{
		st_lamp_info.nLampY[2] = nChk;
	}

	:: GetPrivateProfileString(_T("TowerLampData_Yellow"), _T("LotEnd_State"), _T("0"), (LPWSTR)chData, 10, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	nChk = _wtoi(strTemp);
	if (nChk<0 || nChk>2)
	{
		st_lamp_info.nLampY[3] = 2;
	}
	else
	{
		st_lamp_info.nLampY[3] = nChk;
	}

	:: GetPrivateProfileString(_T("TowerLampData_Yellow"), _T("Initial_State"), _T("0"), (LPWSTR)chData, 10, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	nChk = _wtoi(strTemp);
	if (nChk<0 || nChk>2)
	{
		st_lamp_info.nLampY[4] = 2;
	}
	else
	{
		st_lamp_info.nLampY[4] = nChk;
	}

	:: GetPrivateProfileString(_T("TowerLampData_Yellow"), _T("Warring_State"), _T("0"), (LPWSTR)chData, 10, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	nChk = _wtoi(strTemp);
	if (nChk<0 || nChk>2)
	{
		st_lamp_info.nLampY[5] = 2;
	}
	else
	{
		st_lamp_info.nLampY[5] = nChk;
	}

	:: GetPrivateProfileString(_T("TowerLampData_Yellow"), _T("Lock_State"), _T("0"), (LPWSTR)chData, 10, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	nChk = _wtoi(strTemp);
	if (nChk<0 || nChk>2)
	{
		st_lamp_info.nLampY[6] = 2;
	}
	else
	{
		st_lamp_info.nLampY[6] = nChk;
	}

	:: GetPrivateProfileString(_T("TowerLampData_Yellow"), _T("SelfCheck_State"), _T("0"), (LPWSTR)chData, 10, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	nChk = _wtoi(strTemp);
	if (nChk<0 || nChk>2)
	{
		st_lamp_info.nLampY[7] = 2;
	}
	else
	{
		st_lamp_info.nLampY[7] = nChk;
	}

	:: GetPrivateProfileString(_T("TowerLampData_Yellow"), _T("Idle_State"), _T("0"), (LPWSTR)chData, 10, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	nChk = _wtoi(strTemp);
	if (nChk<0 || nChk>2)
	{
		st_lamp_info.nLampY[8] = 2;
	}
	else
	{
		st_lamp_info.nLampY[8] = nChk;
	}
	// **************************************************************************

	// **************************************************************************
	// 타워 램프 GREEN 상태 로딩하여 전역 변수에 설정한다                        
	// ->  STOP			= 0,
// 		   RUN			= 1,
// 		   ALARM		= 2,
// 		   LOTEND		= 3,
// 		   INIT			= 4,
// 		   WARRING		= 5,
// 		   LOCK			= 6,
// 		   SELFCHECK	= 7														 
	// -> 로딩 값 [0:OFF    1:ON    2:FLICKER]                                   
	// **************************************************************************
	:: GetPrivateProfileString(_T("TowerLampData_Green"), _T("Stop_State"), _T("0"), (LPWSTR)chData, 10, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	nChk = _wtoi(strTemp);
	if (nChk<0 || nChk>2)
	{
		st_lamp_info.nLampG[0] = 1;
	}
	else
	{
		st_lamp_info.nLampG[0] = nChk;
	}

	:: GetPrivateProfileString(_T("TowerLampData_Green"), _T("Run_State"), _T("0"), (LPWSTR)chData, 10, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	nChk = _wtoi(strTemp);
	if (nChk<0 || nChk>2)
	{
		st_lamp_info.nLampG[1] = 0;
	}
	else
	{
		st_lamp_info.nLampG[1] = nChk;
	}

	:: GetPrivateProfileString(_T("TowerLampData_Green"), _T("Alarm_State"), _T("0"), (LPWSTR)chData, 10, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	nChk = _wtoi(strTemp);
	if (nChk<0 || nChk>2)
	{
		st_lamp_info.nLampG[2] = 0;
	}
	else
	{
		st_lamp_info.nLampG[2] = nChk;
	}

	:: GetPrivateProfileString(_T("TowerLampData_Green"), _T("LotEnd_State"), _T("0"), (LPWSTR)chData, 10, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	nChk = _wtoi(strTemp);
	if (nChk<0 || nChk>2)
	{
		st_lamp_info.nLampG[3] = 2;
	}
	else
	{
		st_lamp_info.nLampG[3] = nChk;
	}

	:: GetPrivateProfileString(_T("TowerLampData_Green"), _T("Initial_State"), _T("0"), (LPWSTR)chData, 10, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	nChk = _wtoi(strTemp);
	if (nChk<0 || nChk>2)
	{
		st_lamp_info.nLampG[4] = 2;
	}
	else
	{
		st_lamp_info.nLampG[4] = nChk;
	}

	:: GetPrivateProfileString(_T("TowerLampData_Green"), _T("Warring_State"), _T("0"), (LPWSTR)chData, 10, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	nChk = _wtoi(strTemp);
	if (nChk<0 || nChk>2)
	{
		st_lamp_info.nLampG[5] = 2;
	}
	else
	{
		st_lamp_info.nLampG[5] = nChk;
	}

	:: GetPrivateProfileString(_T("TowerLampData_Green"), _T("Lock_State"), _T("0"), (LPWSTR)chData, 10, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	nChk = _wtoi(strTemp);
	if (nChk<0 || nChk>2)
	{
		st_lamp_info.nLampG[6] = 2;
	}
	else
	{
		st_lamp_info.nLampG[6] = nChk;
	}

	:: GetPrivateProfileString(_T("TowerLampData_Green"), _T("SelfCheck_State"), _T("0"), (LPWSTR)chData, 10, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	nChk = _wtoi(strTemp);
	if (nChk<0 || nChk>2)
	{
		st_lamp_info.nLampG[7] = 2;
	}
	else
	{
		st_lamp_info.nLampG[7] = nChk;
	}

	:: GetPrivateProfileString(_T("TowerLampData_Green"), _T("Idle_State"), _T("0"), (LPWSTR)chData, 10, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	nChk = _wtoi(strTemp);
	if (nChk<0 || nChk>2)
	{
		st_lamp_info.nLampG[8] = 2;
	}
	else
	{
		st_lamp_info.nLampG[8] = nChk;
	}
	// **************************************************************************

	// **************************************************************************
	// 부저 사용 모드 로딩하여 전역 변수에 설정                                  
	// -> 0:사용    1:미사용                                                     
	// **************************************************************************
	:: GetPrivateProfileString(_T("TowerLampData"), _T("n_buzzer_mode"), _T("0"), (LPWSTR)chData, 10, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	nChk = _wtoi(strTemp);
	if (nChk<0 || nChk>1)
	{
		st_lamp_info.nBuzzerMode = 1;
	}
	else
	{
		st_lamp_info.nBuzzerMode = nChk;
	}
	// **************************************************************************

	// **************************************************************************
	// 장비 호기 및 장비 코드 로딩하여 전역 변수에 설정                          
	// -> 로딩 정보에서 앞/뒤 공백은 제거한다                                    
	// **************************************************************************
	:: GetPrivateProfileString(_T("TowerLampData"), _T("str_equip_no"), _T("AMT"), (LPWSTR)chData, 20, strLoadFile);
//	sprintf(chData,"%S", chData);
	st_lamp_info.strEquipNo.Format(_T("%s"), chData);
	(st_lamp_info.strEquipNo).TrimLeft(' ');               
	(st_lamp_info.strEquipNo).TrimRight(' ');

	:: GetPrivateProfileString(_T("TowerLampData"), _T("str_equip_code"), _T("AMT"), (LPWSTR)chData, 20, strLoadFile);
//	sprintf(chData,"%S", chData);
	st_lamp_info.strEquipCode.Format(_T("%s"), chData);
	(st_lamp_info.strEquipCode).TrimLeft(' ');               
	(st_lamp_info.strEquipCode).TrimRight(' ');
	// **************************************************************************

	// **************************************************************************
	// 타워 램프 ON/OFF 대기 시간 로딩하여 전역 변수에 설정                      
	// **************************************************************************
	:: GetPrivateProfileString(_T("TowerLampData"), _T("n_lamp_on_time_w"), _T("0"), (LPWSTR)chData, 10, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	nChk = _wtoi(strTemp);
	if (nChk < 1)
	{
		st_lamp_info.nLampWaitTime = 500;
	}
	else
	{
		st_lamp_info.nLampWaitTime = nChk;
	}
	// ***************************************************************************/
}

void CMyBasicData::OnMaintenance_Data_Save()
{
	CString strTemp, strPart;  // 로딩 정보 임시 저장 변수
	CString strSaveFile;;

	// **************************************************************************
	// 타워 램프 RED 상태 정보를 파일에 저장한다                                 
	// ->  STOP			= 0,
// 		   RUN			= 1,
// 		   ALARM		= 2,
// 		   LOTEND		= 3,
// 		   INIT			= 4,
// 		   WARRING		= 5,
// 		   LOCK			= 6,
// 		   SELFCHECK	= 7														 
	// -> 로딩 값 [0:OFF    1:ON    2:FLICKER]                                   
	// **************************************************************************
//	str_save_file = st_path.str_basic + st_basic.str_device_name;  // 티칭 데이터 저장 파일 설정
	strSaveFile = st_path_info.strBasic + _T("Maintenance.TXT");

	strTemp.Format(_T("%d"), st_lamp_info.nLampR[0]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Red"), _T("Stop_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampR[1]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Red"), _T("Run_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampR[2]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Red"), _T("Alarm_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampR[3]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Red"), _T("LotEnd_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampR[4]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Red"), _T("Initial_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampR[5]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Red"), _T("Warring_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampR[6]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Red"), _T("Lock_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampR[7]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Red"), _T("SelfCheck_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampR[8]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Red"), _T("Idle_State"), LPCTSTR(strTemp), strSaveFile);
	// **************************************************************************


	// **************************************************************************
	// 타워 램프 YELLOW 상태 정보를 파일에 저장한다                              
	// ->  STOP			= 0,
// 		   RUN			= 1,
// 		   ALARM		= 2,
// 		   LOTEND		= 3,
// 		   INIT			= 4,
// 		   WARRING		= 5,
// 		   LOCK			= 6,
// 		   SELFCHECK	= 7														 
	// -> 로딩 값 [0:OFF    1:ON    2:FLICKER]                                   
	// **************************************************************************
	strTemp.Format(_T("%d"),st_lamp_info.nLampY[0]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Yellow"), _T("Stop_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampY[1]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Yellow"), _T("Run_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampY[2]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Yellow"), _T("Alarm_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampY[3]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Yellow"), _T("LotEnd_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampY[4]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Yellow"), _T("Initial_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampY[5]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Yellow"), _T("Warring_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampY[6]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Yellow"), _T("Lock_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampY[7]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Yellow"), _T("SelfCheck_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampY[8]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Yellow"), _T("Idle_State"), LPCTSTR(strTemp), strSaveFile);

	// **************************************************************************

	// **************************************************************************
	// 타워 램프 GREEN 상태 정보를 파일에 저장한다                               
	// ->  STOP			= 0,
// 		   RUN			= 1,
// 		   ALARM		= 2,
// 		   LOTEND		= 3,
// 		   INIT			= 4,
// 		   WARRING		= 5,
// 		   LOCK			= 6,
// 		   SELFCHECK	= 7														 
	// -> 로딩 값 [0:OFF    1:ON    2:FLICKER]                                   
	// **************************************************************************
	strTemp.Format(_T("%d"),st_lamp_info.nLampG[0]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Green"), _T("Stop_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampG[1]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Green"), _T("Run_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampG[2]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Green"), _T("Alarm_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampG[3]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Green"), _T("LotEnd_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampG[4]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Green"), _T("Initial_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampG[5]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Green"), _T("Warring_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampG[6]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Green"), _T("Lock_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampG[7]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Green"), _T("SelfCheck_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampG[8]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Green"), _T("Idle_State"), LPCTSTR(strTemp), strSaveFile);
	// **************************************************************************

	// **************************************************************************
	// 부저 사용 모드 로딩하여 전역 변수에 설정                                  
	// -> 0:사용    1:미사용                                                     
	// **************************************************************************
	strTemp.Format(_T("%d"),st_lamp_info.nBuzzerMode) ;
	:: WritePrivateProfileString(_T("TowerLampData"), _T("n_buzzer_mode"), LPCTSTR(strTemp), strSaveFile);
	// **************************************************************************

	// **************************************************************************
	// 장비 호기 및 장비 코드 로딩하여 전역 변수에 설정                          
	// -> 로딩 정보에서 앞/뒤 공백은 제거한다                                    
	// **************************************************************************
	:: WritePrivateProfileString(_T("TowerLampData"), _T("str_equip_no"), LPCTSTR(st_lamp_info.strEquipNo), strSaveFile);
	:: WritePrivateProfileString(_T("TowerLampData"), _T("str_equip_code"), LPCTSTR(st_lamp_info.strEquipCode), strSaveFile);
	// **************************************************************************

	// **************************************************************************
	// 타워 램프 ON/OFF 대기 시간 로딩하여 전역 변수에 설정                      
	// **************************************************************************
	strTemp.Format(_T("%d"),st_lamp_info.nLampWaitTime);
	:: WritePrivateProfileString(_T("TowerLampData"), _T("n_lamp_on_time_w"), LPCTSTR(strTemp), strSaveFile);
}

void CMyBasicData::OnModuleRobot_Teach_Data_Save()
{
	CString strTemp, strMsg, strSaveFail;       // 임시 저장 변수
	CString strPos;
	CString strHead, strItem;
	int i, j;
	
	strSaveFail = st_path_info.strFileMotor + st_basic_info.strDeviceName;
	
	for(i=0; i<MAXMOTOR; i++)
	{
		strHead.Format(_T("Motor%d"),i+1);
		for(j=0; j<M_MAX_POS; j++)
		{
			strItem.Format(_T("%02d_Axis_[%02d]"), i+1, j+1); 
			strTemp.Format(_T("%.3f"), st_motor_info[i].d_pos[j]);
			:: WritePrivateProfileString(strHead, strItem, LPCTSTR(strTemp), strSaveFail);
		}
	}
}

void CMyBasicData::OnModuleRobot_Teach_Data_Load()
{
	CString strTemp, strMsg, strSaveFail;       // 임시 저장 변수
	CString strPos;
	CString strHead, strItem;
	int i, j;
	char chData[100];

	strSaveFail = st_path_info.strFileMotor + st_basic_info.strDeviceName;
	
	for(i=0; i<MAXMOTOR; i++)
	{
		strHead.Format(_T("Motor%d"),i+1);
		for(j=0; j<M_MAX_POS; j++)
		{
			strItem.Format(_T("%02d_Axis_[%02d]"), i+1, j+1); 
			:: GetPrivateProfileString(strHead, strItem, _T("0.0"), (LPWSTR)chData, 10, strSaveFail);
			strTemp.Format(_T("%s"), chData);
			st_motor_info[i].d_pos[j] = _wtof(strTemp);;
		}
	}
}

void CMyBasicData::OnModuleRobot_Teach_Data_Load(CString strDeviceName)
{
	CString strTemp, strMsg, strSaveFail;       // 임시 저장 변수
	CString strPos;
	CString strHead, strItem;
	int i, j;
	char chData[100];

	strSaveFail = st_path_info.strFileMotor + strDeviceName;
	
	for(i=0; i<MAXMOTOR; i++)
	{
		strHead.Format(_T("Motor%d"),i+1);
		for(j=0; j<M_MAX_POS; j++)
		{
			strItem.Format(_T("%02d_Axis_[%02d]"), i+1, j+1); 
			:: GetPrivateProfileString(strHead, strItem, _T("0.0"), (LPWSTR)chData, 10, strSaveFail);
			strTemp.Format(_T("%s"), chData);
			st_motor_info[i].d_pos[j] = _wtof(strTemp);;
		}
	}
}

void CMyBasicData::OnBasic_Data_Load(int nMode)
{
	CString str_load_device;	// 로딩 디바이스명 저장 변수
	CString str_load_pgm;		// 로딩 디바이스명 저장 변수
	CString str_load_file;
	CString str_chk_ext;		// 파일 확장자 저장 변수
	CString str_temp, stemp;	// 저장할 정보 임시 저장 변수 
	CString strTemp;
	CString str_pos;
	TCHAR chr_data[50], chr_buf[20];
	TCHAR chData[50];
	int mn_chk, i, j, k;
	double md_chk;

	memset(&chr_data, 0, sizeof(chr_data));
	memset(&chr_buf, 0, sizeof(chr_buf));

	// 최종 파일명 가져오기
	if(st_basic_info.strDeviceName == "")
	{
		GetPrivateProfileString(_T("FILE_NAME"), _T("Device_Type"), _T("DEFAULT.TXT"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		st_basic_info.strDeviceName = (LPCTSTR)chr_data;
	}
	// 읽을 경로 
	str_load_file = st_path_info.strBasic + st_basic_info.strDeviceName;  // 티칭 데이터 저장 파일 설정

	st_basic_info.nCtrlMode = EQP_OFF_LINE;

	GetPrivateProfileString(_T("BASIC_SCREEN"), _T("strEqp"), _T(""), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
	st_basic_info.strEqp = (LPCTSTR)chr_data;

	if (nMode == 0)
	{
		GetPrivateProfileString(_T("BASIC_SCREEN"), _T("nModeInterface"), _T("1"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		str_temp.Format(_T("%s"), chr_data);
		mn_chk = _wtoi(str_temp);
		if (mn_chk < 0)
		{
			st_basic_info.nModeInterface = 1;
		}
		else  st_basic_info.nModeInterface = mn_chk;

		GetPrivateProfileString(_T("BASIC_SCREEN"), _T("nModeXgem"), _T("1"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		str_temp.Format(_T("%s"), chr_data);
		mn_chk = _wtoi(str_temp);
		if (mn_chk < 0)
		{
			st_basic_info.nModeXgem = 1;
		}
		else  st_basic_info.nModeXgem = mn_chk;

		GetPrivateProfileString(_T("BASIC_SCREEN"), _T("nModeXgemRunChk"), _T("1"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		str_temp.Format(_T("%s"), chr_data);
		mn_chk = _wtoi(str_temp);
		if (mn_chk < 0)
		{
			st_basic_info.nModeXgemRunChk = 1;
		}
		else  st_basic_info.nModeXgemRunChk = mn_chk;

		GetPrivateProfileString(_T("BASIC_SCREEN"), _T("nModeXgemInterface"), _T("1"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		str_temp.Format(_T("%s"), chr_data);
		mn_chk = _wtoi(str_temp);
		if (mn_chk < 0)
		{
			st_basic_info.nModeXgemInterface = 1;
		}
		else  st_basic_info.nModeXgemInterface = mn_chk;

		GetPrivateProfileString(_T("BASIC_SCREEN"), _T("nModeDevice"), _T("1"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		str_temp.Format(_T("%s"), chr_data);
		mn_chk = _wtoi(str_temp);
		if (mn_chk < 0)
		{
			st_basic_info.nModeDevice = 1;
		}
		else  st_basic_info.nModeDevice = mn_chk;

		GetPrivateProfileString(_T("BASIC_SCREEN"), _T("nModeWork"), _T("1"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		str_temp.Format(_T("%s"), chr_data);
		mn_chk = _wtoi(str_temp);
		if (mn_chk < 0)
		{
			st_basic_info.nModeWork = 1;
		}
		else  st_basic_info.nModeWork = mn_chk;
	}

	GetPrivateProfileString(_T("BASIC_SCREEN"), _T("nModeJigStack"), _T("1"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
	str_temp.Format(_T("%s"), chr_data);
	mn_chk = _wtoi(str_temp);
	if (mn_chk < 0)
	{
		st_basic_info.nModeJigStack = 1;
	}
	else  st_basic_info.nModeJigStack = mn_chk;

	GetPrivateProfileString(_T("BASIC_SCREEN"), _T("strLastOperatorName"), _T("MAINT"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
	st_worker_info.strWorker_Name.Format(_T("%s"), chr_data);

	GetPrivateProfileString(_T("BASIC_SCREEN"), _T("strLastOperatorCrem"), _T("A"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
	st_worker_info.strWorker_Crem.Format(_T("%s"), chr_data);

	GetPrivateProfileString(_T("BASIC_SCREEN"), _T("strLastOperatorId"), _T("AUTO"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
	st_worker_info.strWorker_Num.Format(_T("%s"), chr_data);

	GetPrivateProfileString(_T("BASIC_SCREEN"), _T("nModeFrontSmema"), _T("1"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
	str_temp.Format(_T("%s"), chr_data);
	mn_chk = _wtoi(str_temp);
	if (mn_chk < 0)
	{
		st_basic_info.nModeFrontSmema = 1;
	}
	else  st_basic_info.nModeFrontSmema = mn_chk;

	GetPrivateProfileString(_T("BASIC_SCREEN"), _T("nModeRearSmema"), _T("1"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
	str_temp.Format(_T("%s"), chr_data);
	mn_chk = _wtoi(str_temp);
	if (mn_chk < 0)
	{
		st_basic_info.nModeRearSmema = 1;
	}
	else  st_basic_info.nModeRearSmema = mn_chk;

	GetPrivateProfileString(_T("BASIC_SCREEN"), _T("nModeJigLevel"), _T("1"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
	str_temp.Format(_T("%s"), chr_data);
	mn_chk = _wtoi(str_temp);
	if (mn_chk < 0)
	{
		st_basic_info.nModeJigLevel = 1;
	}
	else  st_basic_info.nModeJigLevel = mn_chk;

	GetPrivateProfileString(_T("BASIC_SCREEN"), _T("nModeRfid"), _T("1"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
	str_temp.Format(_T("%s"), chr_data);
	mn_chk = _wtoi(str_temp);
	if (mn_chk < 0)
	{
		st_basic_info.nModeRfid = 1;
	}
	else  st_basic_info.nModeRfid = mn_chk;

	//kwlee 2017.0204
	GetPrivateProfileString(_T("BASIC_SCREEN"), _T("nBarcodeErrorSkip"), _T("1"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
	str_temp.Format(_T("%s"), chr_data);
	mn_chk = _wtoi(str_temp);
	if (mn_chk < 0)
	{
		st_basic_info.nBarcodeErrorSkip = 1;
	}
	else  st_basic_info.nBarcodeErrorSkip = mn_chk;
	///
	GetPrivateProfileString(_T("BASIC_SCREEN"), _T("nModeLdBcr"), _T("1"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
	str_temp.Format(_T("%s"), chr_data);
	mn_chk = _wtoi(str_temp);
	if (mn_chk < 0)
	{
		st_basic_info.nModeLdBcr = 1;
	}
	else  st_basic_info.nModeLdBcr = mn_chk;

	GetPrivateProfileString(_T("BASIC_SCREEN"), _T("nModeUnLdBcr"), _T("1"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
	str_temp.Format(_T("%s"), chr_data);
	mn_chk = _wtoi(str_temp);
	if (mn_chk < 0)
	{
		st_basic_info.nModeUnLdBcr = 1;
	}
	else  st_basic_info.nModeUnLdBcr = mn_chk;

	GetPrivateProfileString(_T("BASIC_SCREEN"), _T("nVnRTime"), _T("0"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
	str_temp.Format(_T("%s"), chr_data);
	st_basic_info.nVnRTime = _wtoi(str_temp);

	GetPrivateProfileString(_T("BASIC_SCREEN"), _T("nUldGoodTrayStack_Count"), _T("0"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
	str_temp.Format(_T("%s"), chr_data);
	st_basic_info.nUldGoodTrayStack_Count = _wtoi(str_temp);

	//kwlee 2017.0202
	GetPrivateProfileString(_T("BASIC_SCREEN"), _T("nVisionErrorCnt"), _T("0"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
	str_temp.Format(_T("%s"), chr_data);
	st_basic_info.nVisionErrorCnt = _wtoi(str_temp);
	///////

	//kwlee 2017.0220
	GetPrivateProfileString(_T("BASIC_SCREEN"), _T("nBarcodeReadPos"), _T("0"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
	str_temp.Format(_T("%s"), chr_data);
	st_basic_info.nBarcodeReadPos = _wtoi(str_temp);
	///////

	GetPrivateProfileString(_T("BASIC_SCREEN"), _T("ModelName"), _T("MDX_MSATA"), chr_data, sizeof(chr_data), str_load_file);
	st_basic_info.strModelName = (LPCTSTR)chr_data;

	GetPrivateProfileString(_T("BASIC_SCREEN"), _T("nPcbArray"), _T("1"), chr_data, sizeof(chr_data), str_load_file);
	str_temp.Format(_T("%s"), chr_data);
	mn_chk = _wtoi(str_temp);
	if (mn_chk < 0)
	{
		st_basic_info.nPcbArray = 1;
	}
	else  st_basic_info.nPcbArray = mn_chk;

	//kwlee 2016.0323
	GetPrivateProfileString(_T("BASIC_SCREEN"), _T("nPcbRowCnt"), _T("1"), chr_data, sizeof(chr_data), str_load_file);
	str_temp.Format(_T("%s"), chr_data);
	mn_chk = _wtoi(str_temp);
	if (mn_chk < 0)
	{
		st_basic_info.nRowCnt = 1;
	}
	else  st_basic_info.nRowCnt = mn_chk;
	
	GetPrivateProfileString(_T("BASIC_SCREEN"), _T("nPcbColCnt"), _T("1"), chr_data, sizeof(chr_data), str_load_file);
	str_temp.Format(_T("%s"), chr_data);
	mn_chk = _wtoi(str_temp);
	if (mn_chk < 0)
	{
		st_basic_info.nColCnt = 1;
	}
	else  st_basic_info.nColCnt = mn_chk;

	GetPrivateProfileString(_T("BASIC_SCREEN"), _T("nPcbType"), _T("1"), chr_data, sizeof(chr_data), str_load_file);
	str_temp.Format(_T("%s"), chr_data);
	mn_chk = _wtoi(str_temp);
	if (mn_chk < 0)
	{
		st_basic_info.nPcbType = 1;
	}
	else  st_basic_info.nPcbType = mn_chk;


	GetPrivateProfileString(_T("BASIC_SCREEN"), _T("nPcbTurn"), _T("1"), chr_data, sizeof(chr_data), str_load_file);
	str_temp.Format(_T("%s"), chr_data);
	mn_chk = _wtoi(str_temp);
	if (mn_chk < 0)
	{
		st_basic_info.nPcbTurnEnable = 1;
	}
	else  st_basic_info.nPcbTurnEnable = mn_chk;

	///////
	GetPrivateProfileString(_T("BASIC_SCREEN"), _T("nRobotPickPos"), _T("1"), chr_data, sizeof(chr_data), str_load_file);
	str_temp.Format(_T("%s"), chr_data);
	mn_chk = _wtoi(str_temp);
	if (mn_chk < 0)
	{
		st_basic_info.nRobotPickPos = 0;
	}
	else  st_basic_info.nRobotPickPos = mn_chk;


	GetPrivateProfileString(_T("BASIC_SCREEN"), _T("nLeftSize"), _T("1"), chr_data, sizeof(chr_data), str_load_file);
	str_temp.Format(_T("%s"), chr_data);
	mn_chk = _wtoi(str_temp);
	if (mn_chk < 0)
	{
		st_basic_info.nLeftSize = 0;
	}
	else  st_basic_info.nLeftSize = mn_chk;


	GetPrivateProfileString(_T("BASIC_SCREEN"), _T("nRobotPlacePos"), _T("1"), chr_data, sizeof(chr_data), str_load_file);
	str_temp.Format(_T("%s"), chr_data);
	mn_chk = _wtoi(str_temp);
	if (mn_chk < 0)
	{
		st_basic_info.nRobotPlacePos = 0;
	}
	else  st_basic_info.nRobotPlacePos = mn_chk;
	/////////////

	for (int i =0; i<st_basic_info.nRowCnt; i++)
	{
		for (int j =0; j<st_basic_info.nColCnt; j++)
		{
			strTemp.Format(_T("PCB_Sel_%02d_%02d"), i+1, j+1);
			GetPrivateProfileString(_T("BASIC_SCREEN"), strTemp, _T(""), chr_data, sizeof(chr_data), str_load_file);
			str_temp.Format(_T("%s"), chr_data);
			mn_chk = _wtoi(str_temp);
			if (mn_chk < 0)
			{
				st_Pcb_info.nPcbSelect[i][j] = 0;
			}
			else  st_Pcb_info.nPcbSelect[i][j] = mn_chk;
		}
	}
	////////////

	//////////////
	for (int i =0; i<2; i++)
	{
		for (int j =0; j<6; j++)
		{
			strTemp.Format(_T("Picker_Sel_%02d_%02d"), i+1, j+1);
			GetPrivateProfileString(_T("BASIC_SCREEN"), strTemp, _T(""), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
			str_temp.Format(_T("%s"), chr_data);
			mn_chk = _wtoi(str_temp);
			if (mn_chk < 0)
			{
				st_basic_info.nCellPos[i][j] = 0;
			}
			else  st_basic_info.nCellPos[i][j] = mn_chk;
		}
	}

	for (int i =0; i<PICKCNT; i++)
	{
		strTemp.Format(_T("Picker_Use_%02d"), i+1);
		GetPrivateProfileString(_T("BASIC_SCREEN"), strTemp, _T(""), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		str_temp.Format(_T("%s"), chr_data);
		mn_chk = _wtoi(str_temp);
		if (mn_chk < 0)
		{
			st_basic_info.nPickerSelect[i] = 0;
		}
		else  st_basic_info.nPickerSelect[i] = mn_chk;
	}
	//////////////


	GetPrivateProfileString(_T("BASIC_SCREEN"), _T("nRetry"), _T("1"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
	str_temp.Format(_T("%s"), chr_data);
	mn_chk = _wtoi(str_temp);
	if (mn_chk < 0)
	{
		st_basic_info.nRetry = 1;
	}
	else  st_basic_info.nRetry = mn_chk;

	GetPrivateProfileString(_T("BASIC_SCREEN"), _T("nModeHook"), _T("1"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
	str_temp.Format(_T("%s"), chr_data);
	mn_chk = _wtoi(str_temp);
	if (mn_chk < 0)
	{
		st_basic_info.nModeHook = 1;
	}
	else  st_basic_info.nModeHook = mn_chk;

	GetPrivateProfileString(_T("BASIC_SCREEN"), _T("bEnableFlag"), _T("1"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
	str_temp.Format(_T("%s"), chr_data);
	mn_chk = _wtoi(str_temp);
	if (mn_chk < 0)
	{
		st_basic_info.bEnableFlag = true;
	}
	else
	{
		if (mn_chk == false)
		{
			st_basic_info.bEnableFlag = false;
		}
		else
		{
			st_basic_info.bEnableFlag = true;
		}
	}

	:: GetPrivateProfileString(_T("BASIC_SCREEN"), _T("nRetestCount"), _T("1"), (LPWSTR)chr_data, 30, st_path_info.strFileBasic);
	str_temp.Format(_T("%s"), chr_data);
	st_recipe_info.nTestRetest_Count = _wtoi(str_temp);

	:: GetPrivateProfileString(_T("BASIC_SCREEN"), _T("nFailCount"), _T("1"), (LPWSTR)chr_data, 30, st_path_info.strFileBasic);
	str_temp.Format(_T("%s"), chr_data);
	st_recipe_info.nFailCnt = _wtoi(str_temp);

	:: GetPrivateProfileString(_T("BASIC_SCREEN"), _T("nPartNoDgt"), _T("1"), (LPWSTR)chr_data, 30, st_path_info.strFileBasic);
	str_temp.Format(_T("%s"), chr_data);
	st_basic_info.nPartNoDgt = _wtoi(str_temp);

	:: GetPrivateProfileString(_T("BASIC_SCREEN"), _T("strDevice"), _T(""), (LPWSTR)chr_data, 30, st_path_info.strFileBasic);
	str_temp.Format(_T("%s"), chr_data);
	st_basic_info.strDevice = str_temp;
	
	if (nMode == 0)
	{
		GetPrivateProfileString(_T("ALARM"), _T("COUNT"), _T("1"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		str_temp.Format(_T("%s"), chr_data);
		mn_chk = _wtoi(str_temp);
		if (mn_chk < 0)
		{
			st_alarm_info.nAlarmNum = 0;
		}
		else st_alarm_info.nAlarmNum = mn_chk;

		GetPrivateProfileString(_T("TIME"), _T("RUN_TIME"), _T("1"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		str_temp.Format(_T("%s"), chr_data);
		mn_chk = _wtoi(str_temp);
		if (mn_chk < 0)
		{
			st_handler_info.tRun = 0;
		}
		st_handler_info.tRun = mn_chk;

		GetPrivateProfileString(_T("TIME"), _T("STOP_TIME"), _T("1"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		str_temp.Format(_T("%s"), chr_data);
		mn_chk = _wtoi(str_temp);
		if (mn_chk < 0)
		{
			st_handler_info.tStop = 0;
		}
		st_handler_info.tStop = mn_chk;

		GetPrivateProfileString(_T("TIME"), _T("JAM_TIME"), _T("1"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		str_temp.Format(_T("%s"), chr_data);
		mn_chk = _wtoi(str_temp);
		if (mn_chk < 0)
		{
			st_handler_info.tJam = 0;
		}
		st_handler_info.tJam = mn_chk;

	}

	int nYear, nMonth, nDay, nHour, nMinute, nSecond;

	if (nMode == 0)
	{
		GetPrivateProfileString(_T("TIME"), _T("CREATE_YEAR"), _T("2014"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		str_temp.Format(_T("%s"), chr_data);
		mn_chk = _wtoi(str_temp);
		if (mn_chk < 0)
		{
			nYear = 2014;
		}
		else  nYear = mn_chk;

		GetPrivateProfileString(_T("TIME"), _T("CREATE_MONTH"), _T("1"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		str_temp.Format(_T("%s"), chr_data);
		mn_chk = _wtoi(str_temp);
		if (mn_chk < 0)
		{
			nMonth = 1;
		}
		else  nMonth = mn_chk;

		GetPrivateProfileString(_T("TIME"), _T("CREATE_DAY"), _T("1"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		str_temp.Format(_T("%s"), chr_data);
		mn_chk = _wtoi(str_temp);
		if (mn_chk < 0)
		{
			nDay = 1;
		}
		else  nDay = mn_chk;

		GetPrivateProfileString(_T("TIME"), _T("CREATE_HOUR"), _T("22"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		str_temp.Format(_T("%s"), chr_data);
		mn_chk = _wtoi(str_temp);
		if (mn_chk < 0)
		{
			nHour = 22;
		}
		else  nHour = mn_chk;

		GetPrivateProfileString(_T("TIME"), _T("CREATE_MINUTE"), _T("0"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		str_temp.Format(_T("%s"), chr_data);
		mn_chk = _wtoi(str_temp);
		if (mn_chk < 0)
		{
			nMinute = 0;
		}
		else  nMinute = mn_chk;

		GetPrivateProfileString(_T("TIME"), _T("CREATE_SECOND"), _T("0"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		str_temp.Format(_T("%s"), chr_data);
		mn_chk = _wtoi(str_temp);
		if (mn_chk < 0)
		{
			nSecond = 0;
		}
		else  nSecond = mn_chk;

		st_handler_info.tCreate.SetDateTime(nYear, nMonth, nDay, nHour, nMinute, nSecond);
	}

	// 20140811 jtkim
	GetPrivateProfileString(_T("BASIC_SCREEN"), _T("strToolName"), _T("MAINT"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
	st_basic_info.strToolName.Format(_T("%s"), chr_data);

	GetPrivateProfileString(_T("BASIC_SCREEN"), _T("strFtpUser"), _T(""), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
	st_basic_info.strFtpUser.Format(_T("%s"), chr_data);

	GetPrivateProfileString(_T("BASIC_SCREEN"), _T("strFtpPass"), _T(""), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
	st_basic_info.strFtpPass.Format(_T("%s"), chr_data);

	GetPrivateProfileString(_T("BASIC_SCREEN"), _T("strPathFtpGms"), _T(""), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
	st_path_info.strPathFtpGms.Format(_T("%s"), chr_data);

	for (i=0; i<8; i++)
	{
		strTemp.Format(_T("VnR_Voltage_Spec #%02d"), i+1);
		GetPrivateProfileString(_T("BASIC_SCREEN"), strTemp, _T(""), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		st_basic_info.strVnrSpec[1][i].Format(_T("%s"), chr_data);

		strTemp.Format(_T("VnR_Resistance_Spec #%02d"), i+1);
		GetPrivateProfileString(_T("BASIC_SCREEN"), strTemp, _T(""), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		st_basic_info.strVnrSpec[0][i].Format(_T("%s"), chr_data);

		strTemp.Format(_T("VnR_Voltage_Ucl #%02d"), i+1);
		GetPrivateProfileString(_T("BASIC_SCREEN"), strTemp, _T(""), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		st_basic_info.strVnrUcl[1][i].Format(_T("%s"), chr_data);

		strTemp.Format(_T("VnR_Resistance_Ucl #%02d"), i+1);
		GetPrivateProfileString(_T("BASIC_SCREEN"), strTemp, _T(""), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		st_basic_info.strVnrUcl[0][i].Format(_T("%s"), chr_data);

		strTemp.Format(_T("VnR_Voltage_Lcl #%02d"), i+1);
		GetPrivateProfileString(_T("BASIC_SCREEN"), strTemp, _T(""), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		st_basic_info.strVnrLcl[1][i].Format(_T("%s"), chr_data);

		strTemp.Format(_T("VnR_Resistance_Lcl #%02d"), i+1);
		GetPrivateProfileString(_T("BASIC_SCREEN"), strTemp, _T(""), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		st_basic_info.strVnrLcl[0][i].Format(_T("%s"), chr_data);
	}



	if (nMode == 0)
	{
		for (i=0; i<HSSI_MAX_IO; i++)
		{
			strTemp.Format(_T("OUT_STATUS_%06d"), i);
			GetPrivateProfileString(_T("IO_DATA"), strTemp, _T(""), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
			str_temp.Format(_T("%s"), chr_data);
			mn_chk = _wtoi(str_temp);
			if (mn_chk < 0)
			{
				FAS_IO.n_out[i] = 0;
			}
			else  FAS_IO.n_out[i] = mn_chk;
		}
	}

	:: GetPrivateProfileString(_T("BASIC"), _T("nUph"), _T(""), (LPWSTR)chData, 30, st_path_info.strFileBasic);
	strTemp.Format(_T("%s"), chData);
	st_count_info.nUph = _wtoi(strTemp);
	if (st_count_info.nUph < 0) st_count_info.nUph = 0;

	:: GetPrivateProfileString(_T("BASIC"), _T("nOldHifix"), _T(""), (LPWSTR)chData, 30, st_path_info.strFileBasic);
	strTemp.Format(_T("%s"), chData);
	st_work_info.nOldHifix = _wtoi(strTemp);

	:: GetPrivateProfileString(_T("BASIC"), _T("nLastMdlCount"), _T(""), (LPWSTR)chData, 30, st_path_info.strFileBasic);
	strTemp.Format(_T("%s"), chData);
	st_basic_info.nLastMdlCount = _wtoi(strTemp);

	:: GetPrivateProfileString(_T("BASIC"), _T("nBcrRead"), _T(""), (LPWSTR)chData, 5, st_path_info.strFileBasic);
	strTemp.Format(_T("%s"), chData);
	st_basic_info.nBcrRead = _wtoi(strTemp);

	:: GetPrivateProfileString(_T("BASIC"), _T("nBcrFail"), _T(""), (LPWSTR)chData, 5, st_path_info.strFileBasic);
	strTemp.Format(_T("%s"), chData);
	st_basic_info.nBcrFail = _wtoi(strTemp);

	//str_load_file]
	if (nMode == 0)
	{
		for (i=0; i<2; i++)
		{
			for (j=0; j<2; j++)
			{
				strTemp.Format(_T("COUNT_IN_%02d_%02d"), i+1, j+1);
				GetPrivateProfileString(_T("BASIC_SCREEN"), strTemp, _T(""), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
				str_temp.Format(_T("%s"), chr_data);
				mn_chk = _wtoi(str_temp);
				if (mn_chk < 0)
				{
					st_count_info.nInCount[i][j] = 0;
				}
				else  st_count_info.nInCount[i][j] = mn_chk;

				strTemp.Format(_T("COUNT_PRIME_%02d_%02d"), i+1, j+1);
				GetPrivateProfileString(_T("BASIC_SCREEN"), strTemp, _T(""), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
				str_temp.Format(_T("%s"), chr_data);
				mn_chk = _wtoi(str_temp);
				if (mn_chk < 0)
				{
					st_count_info.nPrimeCount[i][j] = 0;
				}
				else  st_count_info.nPrimeCount[i][j] = mn_chk;

				strTemp.Format(_T("COUNT_PASS_%02d_%02d"), i+1, j+1);
				GetPrivateProfileString(_T("BASIC_SCREEN"), strTemp, _T(""), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
				str_temp.Format(_T("%s"), chr_data);
				mn_chk = _wtoi(str_temp);
				if (mn_chk < 0)
				{
					st_count_info.nPassCount[i][j] = 0;
				}
				else  st_count_info.nPassCount[i][j] = mn_chk;

				strTemp.Format(_T("COUNT_PRIME_REJECT_%02d_%02d"), i+1, j+1);
				GetPrivateProfileString(_T("BASIC_SCREEN"), strTemp, _T(""), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
				str_temp.Format(_T("%s"), chr_data);
				mn_chk = _wtoi(str_temp);
				if (mn_chk < 0)
				{
					st_count_info.nPrimeRejectCount[i][j] = 0;
				}
				else  st_count_info.nPrimeRejectCount[i][j] = mn_chk;

				strTemp.Format(_T("COUNT_REJECT_%02d_%02d"), i+1, j+1);
				GetPrivateProfileString(_T("BASIC_SCREEN"), strTemp, _T(""), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
				str_temp.Format(_T("%s"), chr_data);
				mn_chk = _wtoi(str_temp);
				if (mn_chk < 0)
				{
					st_count_info.nRejectCount[i][j] = 0;
				}
				else  st_count_info.nRejectCount[i][j] = mn_chk;
			}
		}
	}

	if (nMode == 0)
	{
		// jtkim 20150709
		GetPrivateProfileString(_T("BASIC_SCREEN"), _T("UPH_COUNT"), _T(""), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		str_temp.Format(_T("%s"), chr_data);
		mn_chk = _wtoi(str_temp);
		if (mn_chk < 0)
		{
			st_count_info.nUph = 0;
		}
		else  st_count_info.nUph = mn_chk;

		// jtkim 20150709
		GetPrivateProfileString(_T("BASIC_SCREEN"), _T("DAILY_UPH_COUNT"), _T(""), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		str_temp.Format(_T("%s"), chr_data);
		mn_chk = _wtoi(str_temp);
		if (mn_chk < 0)
		{
			st_count_info.nDailyUph = 0;
		}
		else  st_count_info.nDailyUph = mn_chk;

		// jtkim 20150709
		GetPrivateProfileString(_T("BASIC_SCREEN"), _T("nUphCnt"), _T(""), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		str_temp.Format(_T("%s"), chr_data);
		mn_chk = _wtoi(str_temp);
		if (mn_chk < 0)
		{
			st_count_info.nUphCnt = 0;
		}
		else  st_count_info.nUphCnt = mn_chk;


		// jtkim 20150709
		GetPrivateProfileString(_T("BASIC_SCREEN"), _T("dHourPer"), _T(""), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		str_temp.Format(_T("%s"), chr_data);
		md_chk = _wtof(str_temp);
		if (mn_chk < 0)
		{
			st_count_info.dHourPer = 0.0f;
		}
		else  st_count_info.dHourPer = md_chk;

		GetPrivateProfileString(_T("BASIC_SCREEN"), _T("dDailyPer"), _T(""), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		str_temp.Format(_T("%s"), chr_data);
		md_chk = _wtof(str_temp);
		if (mn_chk < 0)
		{
			st_count_info.dDailyPer = 0.0f;
		}
		else  st_count_info.dDailyPer = md_chk;
	}

	for (i=0; i<12; i++)
	{
		strTemp.Format(_T("TSITE_OFFSET_Y_%02d"), i);
		GetPrivateProfileString(_T("TSITE_OFFSET"), strTemp, _T(""), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		str_temp.Format(_T("%s"), chr_data);
		md_chk = _wtof(str_temp);
		if (md_chk < -1)
		{
			st_recipe_info.dTsiteOffsetY[i] = 0;
		}
		else  st_recipe_info.dTsiteOffsetY[i] = md_chk;
	}

	if (nMode == 0)
	{
		for (i=0; i<24; i++)
		{
			strTemp.Format(_T("UPH%d"), i);
			GetPrivateProfileString(_T("LATEST_UPH"), strTemp, _T("0"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
			str_temp.Format(_T("%s"), chr_data);
			mn_chk = _wtoi(str_temp);

			st_count_info.nLatestUph[i] = mn_chk;
		}
	}

	if (nMode == 0)
	{
		// jtkim 20150323
		for (i=0; i<2; i++)
		{
			for (j=0; j<2; j++)
			{
				for (k=0; k<12; k++)
				{
					strTemp.Format(_T("BdTestCnt_%02d_%02d_%02d"), i+1, j+1, k+1);
					GetPrivateProfileString(_T("BASIC_SCREEN"), strTemp, _T(""), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
					str_temp.Format(_T("%s"), chr_data);
					mn_chk = _wtoi(str_temp);
					if (mn_chk < 0)
					{
						st_bd_info[i][j].nBdTestCnt[k] = 0;
					}
					else  st_bd_info[i][j].nBdTestCnt[k] = mn_chk;

					strTemp.Format(_T("BdPassCnt_%02d_%02d_%02d"), i+1, j+1, k+1);
					GetPrivateProfileString(_T("BASIC_SCREEN"), strTemp, _T(""), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
					str_temp.Format(_T("%s"), chr_data);
					mn_chk = _wtoi(str_temp);
					if (mn_chk < 0)
					{
						st_bd_info[i][j].nBdPassCnt[k] = 0;
					}
					else  st_bd_info[i][j].nBdPassCnt[k] = mn_chk;

					strTemp.Format(_T("BdInfo_%02d_%02d_%02d"), i+1, j+1, k+1);
					GetPrivateProfileString(_T("BASIC_SCREEN"), strTemp, _T(""), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
					str_temp.Format(_T("%s"), chr_data);
					mn_chk = _wtoi(str_temp);
					if (mn_chk < 0)
					{
						st_bd_info[i][j].nBdInfo[k] = 0;
					}
					else  st_bd_info[i][j].nBdInfo[k] = mn_chk;

					strTemp.Format(_T("BdBin_%02d_%02d_%02d"), i+1, j+1, k+1);
					GetPrivateProfileString(_T("BASIC_SCREEN"), strTemp, _T(""), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
					str_temp.Format(_T("%s"), chr_data);
					mn_chk = _wtoi(str_temp);
					if (mn_chk < 0)
					{
						st_bd_info[i][j].nBdBin[k] = 0;
					}
					else  st_bd_info[i][j].nBdBin[k] = mn_chk;

					strTemp.Format(_T("BdFailContiCnt_%02d_%02d_%02d"), i+1, j+1, k+1);
					GetPrivateProfileString(_T("BASIC_SCREEN"), strTemp, _T(""), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
					str_temp.Format(_T("%s"), chr_data);
					mn_chk = _wtoi(str_temp);
					if (mn_chk < 0)
					{
						st_bd_info[i][j].nBdFailContiCnt[k] = 0;
					}
					else  st_bd_info[i][j].nBdFailContiCnt[k] = mn_chk;

					strTemp.Format(_T("strBinHistory_%02d_%02d_%02d"), i+1, j+1, k+1);
					GetPrivateProfileString(_T("BASIC_SCREEN"), strTemp, _T(""), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
					str_temp.Format(_T("%s"), chr_data);
					st_bd_info[i][j].strBinHistory[k] = str_temp;
				}
			}
		}
	}


	if (nMode == 0)
	{
		for (i=0; i<4; i++)
		{
			strTemp.Format(_T("BUFFER_COK_NUM_%02d"), i+1);
			GetPrivateProfileString(_T("BASIC_SCREEN"), strTemp, _T(""), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
			str_temp.Format(_T("%s"), chr_data);
			mn_chk = _wtoi(str_temp);
			if (mn_chk < 0)
			{
				st_Cok_Buff_info[i].st_pcb_info.nCOK_IDNum = 0;
			}
			else  st_Cok_Buff_info[i].st_pcb_info.nCOK_IDNum = mn_chk;

			strTemp.Format(_T("BUFFER_COK_COUNT_%02d"), i+1);
			GetPrivateProfileString(_T("BASIC_SCREEN"), strTemp, _T(""), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
			str_temp.Format(_T("%s"), chr_data);
			mn_chk = _wtoi(str_temp);
			if (mn_chk < 0)
			{
				st_Cok_Buff_info[i].nCokCount = 0;
			}
			else  st_Cok_Buff_info[i].nCokCount = mn_chk;

			for (j=0; j<24; j++)
			{
				strTemp.Format(_T("BUFFER_COK_%02d_%02d"), i+1, j+1);
				GetPrivateProfileString(_T("BASIC_SCREEN"), strTemp, _T(""), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
				str_temp.Format(_T("%s"), chr_data);
				mn_chk = _wtoi(str_temp);
				if (mn_chk < 0)
				{
					st_Cok_Buff_info[i].st_pcb_info.nYesNo[j] = NO;
				}
				else  st_Cok_Buff_info[i].st_pcb_info.nYesNo[j] = mn_chk;
			}
		}

		for (i=0; i<5; i++)
		{
			strTemp.Format(_T("COK_MODE_%02d"), i+1);
			GetPrivateProfileString(_T("BASIC_SCREEN"), strTemp, _T(""), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
			str_temp.Format(_T("%s"), chr_data);
			st_basic_info.strHifixType[i] = str_temp;
		}
	}

	if (nMode == 0)
	{
		for (i=0; i<4; i++)
		{
			strTemp.Format(_T("BUFFER_COK_NUM_%02d"), i+1);
			GetPrivateProfileString(_T("BASIC_SCREEN"), strTemp, _T(""), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
			str_temp.Format(_T("%s"), chr_data);
			mn_chk = _wtoi(str_temp);
			if (mn_chk < 0)
			{
				st_Cok_Buff_info[i].st_pcb_info.nCOK_IDNum = 0;
			}
			else  st_Cok_Buff_info[i].st_pcb_info.nCOK_IDNum = mn_chk;
		}
	}

	for (i=0; i<M_MAX_MOTOR_NUM; i++)
	{
		strTemp.Format(_T("MOTOR_LAST_%02d"), i+1);
		GetPrivateProfileString(_T("BASIC_SCREEN"), strTemp, _T(""), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		str_temp.Format(_T("%s"), chr_data);
		st_work_info.dCurrPos[i] = _wtoi(str_temp);
	}

	:: GetPrivateProfileString(_T("BASIC_SCREEN"), _T("strScrapName"), _T(""), (LPWSTR)chr_data, 30, st_path_info.strFileBasic);
	str_temp.Format(_T("%s"), chr_data);
	st_basic_info.strScrapName = str_temp;

	:: GetPrivateProfileString(_T("BASIC_SCREEN"), _T("strScrapHead"), _T(""), (LPWSTR)chr_data, 30, st_path_info.strFileBasic);
	str_temp.Format(_T("%s"), chr_data);
	st_basic_info.strScrapHead = str_temp;

	// jtkim 20150530 ftp 사용관련
	:: GetPrivateProfileString(_T("BASIC"), _T("nFtpVer"), _T(""), (LPWSTR)chData, 5, st_path_info.strFileBasic);
	strTemp.Format(_T("%s"), chData);
	st_handler_info.nFtpVer = _wtoi(strTemp);

	:: GetPrivateProfileString(_T("BASIC"), _T("nVerUpdate"), _T(""), (LPWSTR)chData, 5, st_path_info.strFileBasic);
	strTemp.Format(_T("%s"), chData);
	st_handler_info.nVerUpdate = _wtoi(strTemp);

	:: GetPrivateProfileString(_T("BASIC"), _T("nPgmVer"), _T(""), (LPWSTR)chData, 5, st_path_info.strFileBasic);
	strTemp.Format(_T("%s"), chData);
	st_handler_info.nPgmVer = _wtoi(strTemp);

	:: GetPrivateProfileString(_T("BASIC"), _T("nAutoLine"), _T(""), (LPWSTR)chData, 5, st_path_info.strFileBasic);
	strTemp.Format(_T("%s"), chData);
	st_handler_info.nAutoLine = _wtoi(strTemp);

	// 20150623 in-line
	GetPrivateProfileString(_T("BASIC_SCREEN"), _T("strProcess"), _T(""), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
	st_basic_info.strProcess.Format(_T("%s"), chr_data);

	// jtkim 20150530 socket 사용관련
	:: GetPrivateProfileString(_T("BASIC_SCREEN"), _T("nSocketCount"), _T(""), (LPWSTR)chData, 20, st_path_info.strFileBasic);
	strTemp.Format(_T("%s"), chData);
	st_basic_info.nSocketCount = _wtoi(strTemp);

	// jtkim 20150902
	:: GetPrivateProfileString(_T("HIFIX"), _T("HIFIX_01"), _T(""), (LPWSTR)chData, 20, st_path_info.strFileBasic);
	st_work_info.strHifix[0].Format(_T("%s"), chData);

	:: GetPrivateProfileString(_T("HIFIX"), _T("HIFIX_02"), _T(""), (LPWSTR)chData, 20, st_path_info.strFileBasic);
	st_work_info.strHifix[1].Format(_T("%s"), chData);

	:: GetPrivateProfileString(_T("HIFIX"), _T("HIFIX_03"), _T(""), (LPWSTR)chData, 20, st_path_info.strFileBasic);
	st_work_info.strHifix[2].Format(_T("%s"), chData);

	:: GetPrivateProfileString(_T("HIFIX"), _T("HIFIX_04"), _T(""), (LPWSTR)chData, 20, st_path_info.strFileBasic);
	st_work_info.strHifix[3].Format(_T("%s"), chData);

	:: GetPrivateProfileString(_T("HIFIX"), _T("HIFIX_05"), _T(""), (LPWSTR)chData, 20, st_path_info.strFileBasic);
	st_work_info.strHifix[4].Format(_T("%s"), chData);

	// jtkim 20150930
	:: GetPrivateProfileString(_T("BASIC"), _T("nRecipeYesNo"), _T(""), (LPWSTR)chData, 5, st_path_info.strFileBasic);
	strTemp.Format(_T("%s"), chData);
	st_basic_info.nRecipeYesNo = _wtoi(strTemp);

	// jtkim 20150930
	:: GetPrivateProfileString(_T("BASIC"), _T("nBarcodeBlock"), _T(""), (LPWSTR)chData, 5, st_path_info.strFileBasic);
	strTemp.Format(_T("%s"), chData);
	st_basic_info.nBarcodeBlock = _wtoi(strTemp);

	// jtkim 20151229 사용관련
	:: GetPrivateProfileString(_T("BASIC_SCREEN"), _T("nRetestCount"), _T("1"), (LPWSTR)chData, 20, st_path_info.strFileBasic);
	strTemp.Format(_T("%s"), chData);
	st_basic_info.nRetestCount = _wtoi(strTemp);

	// jtkim 20160124
	:: GetPrivateProfileString(_T("BASIC_SCREEN"), _T("nAbortPer"), _T("10"), (LPWSTR)chData, 20, st_path_info.strFileBasic);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.nAbortPer = _wtoi(strTemp);
}

void CMyBasicData::OnBasic_Data_Load(CString strDeviceName)
{
	CString str_load_device;	// 로딩 디바이스명 저장 변수
	CString str_load_pgm;		// 로딩 디바이스명 저장 변수
	CString str_load_file;
	CString str_chk_ext;		// 파일 확장자 저장 변수
	CString str_temp, stemp;	// 저장할 정보 임시 저장 변수 
	CString str_pos;
	TCHAR chr_data[50], chr_buf[20];
//	int mn_chk;

	memset(&chr_data, 0, sizeof(chr_data));
	memset(&chr_buf, 0, sizeof(chr_buf));

	

	// 읽을 경로 
	str_load_file = st_path_info.strBasic + strDeviceName;  // 티칭 데이터 저장 파일 설정
}

void CMyBasicData::OnBasic_Data_Save()
{
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	CString str_save_file;
	CString str_part, str_chk_ext;
	CString str_pos, str_tmp;
	COleDateTime time_cur;

	int i, j;

	/* ************************************************************************** */
	/*  데이터 저장할 파일 설정한다 [파일 확장자 검사]                               */
	/* ************************************************************************** */
	:: WritePrivateProfileString(_T("FILE_NAME"), _T("Device_Type"), LPCTSTR(st_basic_info.strDeviceName), st_path_info.strFileBasic);

	str_save_file = st_path_info.strBasic + st_basic_info.strDeviceName;  // 티칭 데이터 저장 파일 설정

	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("strEqp"), LPCTSTR(st_basic_info.strEqp), st_path_info.strFileBasic);

	mstr_temp.Format(_T("%d"), st_basic_info.nModeInterface);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("nModeInterface"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	mstr_temp.Format(_T("%d"), st_basic_info.nModeXgem);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("nModeXgem"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	mstr_temp.Format(_T("%d"), st_basic_info.nModeXgemRunChk);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("nModeXgemRunChk"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	mstr_temp.Format(_T("%d"), st_basic_info.nModeXgemInterface);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("nModeXgemInterface"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	mstr_temp.Format(_T("%d"), st_basic_info.nCtrlMode);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("nCtrlMode"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	mstr_temp.Format(_T("%d"), st_basic_info.nModeDevice);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("nModeDevice"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	mstr_temp.Format(_T("%d"), st_basic_info.nModeWork);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("nModeWork"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	mstr_temp.Format(_T("%d"), st_basic_info.nModeJigStack);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("nModeJigStack"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	//	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("strLastOperatorName"), st_worker_info.strWorker_Name, st_path_info.strFileBasic);
	//	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("strLastOperatorCrem"), st_worker_info.strWorker_Crem, st_path_info.strFileBasic);
	//	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("strLastOperatorId"), st_worker_info.strWorker_Num, st_path_info.strFileBasic);

	mstr_temp.Format(_T("%d"), st_basic_info.nModeFrontSmema);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("nModeFrontSmema"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	mstr_temp.Format(_T("%d"), st_basic_info.nModeRearSmema);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("nModeRearSmema"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	mstr_temp.Format(_T("%d"), st_basic_info.nModeJigLevel);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("nModeJigLevel"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	mstr_temp.Format(_T("%d"), st_basic_info.nModeRfid);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("nModeRfid"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);
	//kwlee 2017.0204 
 	mstr_temp.Format(_T("%d"), st_basic_info.nBarcodeErrorSkip);
 	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("nBarcodeErrorSkip"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);
	//

	mstr_temp.Format(_T("%d"), st_basic_info.nModeLdBcr);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("nModeLdBcr"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	mstr_temp.Format(_T("%d"), st_basic_info.nModeUnLdBcr);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("nModeUnLdBcr"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	mstr_temp.Format(_T("%d"), st_basic_info.nVnRTime);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("nVnRTime"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	mstr_temp.Format(_T("%d"), st_basic_info.nUldGoodTrayStack_Count);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("nUldGoodTrayStack_Count"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	//:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("ModelName"), LPCTSTR(st_basic_info.strModelName), str_save_file);
	
	mstr_temp.Format(_T("%d"), st_basic_info.nPcbArray);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("nPcbArray"), LPCTSTR(mstr_temp), str_save_file);

	mstr_temp.Format(_T("%d"), st_basic_info.nRetry);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("nRetry"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	//kwlee 2017.0202
	mstr_temp.Format(_T("%d"), st_basic_info.nVisionErrorCnt);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("nVisionErrorCnt"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	mstr_temp.Format(_T("%d"), st_basic_info.nBarcodeReadPos);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("nBarcodeReadPos"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);
	//

	//kwlee 2016.0323
	mstr_temp.Format(_T("%d"), st_basic_info.nRowCnt);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("nPcbRowCnt"), LPCTSTR(mstr_temp),str_save_file);

	mstr_temp.Format(_T("%d"), st_basic_info.nColCnt);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("nPcbColCnt"), LPCTSTR(mstr_temp), str_save_file);

	mstr_temp.Format(_T("%d"), st_basic_info.nPcbType);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("nPcbType"), LPCTSTR(mstr_temp), str_save_file);

	mstr_temp.Format(_T("%d"), st_basic_info.nPcbTurnEnable);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("nPcbTurn"), LPCTSTR(mstr_temp), str_save_file);

	mstr_temp.Format(_T("%d"), st_basic_info.nRobotPickPos);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("nRobotPickPos"), LPCTSTR(mstr_temp), str_save_file);

	mstr_temp.Format(_T("%d"), st_basic_info.nLeftSize);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("nLeftSize"), LPCTSTR(mstr_temp), str_save_file);

	mstr_temp.Format(_T("%d"), st_basic_info.nRobotPlacePos);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("nRobotPlacePos"), LPCTSTR(mstr_temp), str_save_file);

	for (int i =0; i<st_basic_info.nRowCnt; i++)
	{
		for (int j =0; j<st_basic_info.nColCnt; j++)
		{
			mstr_temp.Format(_T("PCB_Sel_%02d_%02d"), i+1, j+1);
			str_tmp.Format(_T("%d"), st_Pcb_info.nPcbSelect[i][j]);
			:: WritePrivateProfileString(_T("BASIC_SCREEN"), LPCTSTR(mstr_temp), str_tmp, str_save_file);
		}
	}
	///////////////
	for (int i =0; i<2; i++)
	{
		for (int j =0; j<6; j++)
		{
			mstr_temp.Format(_T("Picker_Sel_%02d_%02d"), i+1, j+1);
			str_tmp.Format(_T("%d"), st_basic_info.nCellPos[i][j]);
			:: WritePrivateProfileString(_T("BASIC_SCREEN"), LPCTSTR(mstr_temp), str_tmp, st_path_info.strFileBasic);
		}
	}

	for (int i =0; i<PICKCNT; i++)
	{
		mstr_temp.Format(_T("Picker_Use_%02d"), i+1);
		str_tmp.Format(_T("%d"), st_basic_info.nPickerSelect[i]);
		:: WritePrivateProfileString(_T("BASIC_SCREEN"), LPCTSTR(mstr_temp), str_tmp, st_path_info.strFileBasic);
	}
	///////////////////

	mstr_temp.Format(_T("%d"), st_basic_info.bEnableFlag);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("bEnableFlag"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	mstr_temp.Format(_T("%d"), st_basic_info.nModeHook);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("nModeHook"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	mstr_temp.Format(_T("%d"), st_recipe_info.nTestRetest_Count);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("nRetestCount"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	mstr_temp.Format(_T("%d"), st_recipe_info.nFailCnt);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("nFailCount"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	mstr_temp.Format(_T("%d"), st_basic_info.nPartNoDgt);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("nPartNoDgt"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("strDevice"), LPCTSTR(st_basic_info.strDevice), st_path_info.strFileBasic);

	mstr_temp.Format(_T("%d"), st_alarm_info.nAlarmNum);
	:: WritePrivateProfileString(_T("ALARM"), _T("COUNT"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	mstr_temp.Format(_T("%d"), st_handler_info.tRun);
	:: WritePrivateProfileString(_T("TIME"), _T("RUN_TIME"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	mstr_temp.Format(_T("%d"), st_handler_info.tStop);
	:: WritePrivateProfileString(_T("TIME"), _T("STOP_TIME"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	mstr_temp.Format(_T("%d"), st_handler_info.tJam);
	:: WritePrivateProfileString(_T("TIME"), _T("JAM_TIME"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	////2015.0914
	mstr_temp.Format(_T("%d"),st_handler_info.m_tDRef);
	:: WritePrivateProfileString(_T("TIME"), _T("Daily_Ref_Time"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	CString str;
// 	for (j =0; j<TSITE_SOCKET_CNT; j++)
// 	{
// 		str.Format(_T("Daily_RunDown_Time[%d]"), j);
// 		mstr_temp.Format(_T("%d"),st_handler_info.m_tDRdown[j]);
// 		:: WritePrivateProfileString(_T("TIME"), str, LPCTSTR(mstr_temp), st_path_info.strFileBasic);
// 
// 		str.Format(_T("m_tDStop[%d]"), j);
// 		mstr_temp.Format(_T("%d"),st_handler_info.m_tDStop[j]);
// 		:: WritePrivateProfileString(_T("TIME"), str, LPCTSTR(mstr_temp), st_path_info.strFileBasic);
// 
// 		str.Format(_T("Daily_UserStop[%d]"), j);
// 		mstr_temp.Format(_T("%d"),st_handler_info.m_tDUserStop[j]);
// 		:: WritePrivateProfileString(_T("TIME"), str, LPCTSTR(mstr_temp), st_path_info.strFileBasic);
// 		
// 		//kwlee 2015.1127
// // 		str.Format(_T("InStandBy_Time[%d]"), j);
// // 		mstr_temp.Format(_T("%d"),st_handler_info.m_tDInStandby[j]);
// // 		:: WritePrivateProfileString(_T("TIME"), str, LPCTSTR(mstr_temp), st_path_info.strFileBasic);
// 
// 		str.Format(_T("InStandBy_Time[%d]"), j);
// 		mstr_temp.Format(_T("%d"),st_DB_time.n_Db_time[INSTANBY][j]);
// 		:: WritePrivateProfileString(_T("TIME"), str, LPCTSTR(mstr_temp), st_path_info.strFileBasic);
// 		///////////
// 	}

// 	for ( k = 0; k < TSITE_SOCKET_CNT; k++ )
// 	{
// 		str.Format(_T("Daily_Test_Time[%d]"), k);
// 		mstr_temp.Format(_T("%d"),st_handler_info.m_tDtest[k]);
// 		:: WritePrivateProfileString(_T("TIME"), str, LPCTSTR(mstr_temp), st_path_info.strFileBasic);
// 
// 		str.Format(_T("Daily_SokOff_Time[%d]"), k);
// 		mstr_temp.Format(_T("%d"),st_handler_info.m_tDSokOff[k]);
// 		:: WritePrivateProfileString(_T("TIME"), str, LPCTSTR(mstr_temp), st_path_info.strFileBasic);
// 		//kwlee 2015.1127
// // 		str.Format(_T("OutStandBy_Time[%d]"), k);
// // 		mstr_temp.Format(_T("%d"),st_handler_info.m_tDOutStandby[k]);
// // 		:: WritePrivateProfileString(_T("TIME"), str, LPCTSTR(mstr_temp), st_path_info.strFileBasic);
// 
// 		str.Format(_T("OutStandBy_Time[%d]"), k);
// 		mstr_temp.Format(_T("%d"),st_DB_time.n_Db_time[OUTREADY][k]);
// 		:: WritePrivateProfileString(_T("TIME"), str, LPCTSTR(mstr_temp), st_path_info.strFileBasic);
// 	}
	////////////////

	mstr_temp.Format(_T("%d"), st_handler_info.tCreate.GetYear());
	:: WritePrivateProfileString(_T("TIME"), _T("CREATE_YEAR"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	mstr_temp.Format(_T("%d"), st_handler_info.tCreate.GetMonth());
	:: WritePrivateProfileString(_T("TIME"), _T("CREATE_MONTH"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	mstr_temp.Format(_T("%d"), st_handler_info.tCreate.GetDay());
	:: WritePrivateProfileString(_T("TIME"), _T("CREATE_DAY"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	mstr_temp.Format(_T("%d"), st_handler_info.tCreate.GetHour());
	:: WritePrivateProfileString(_T("TIME"), _T("CREATE_HOUR"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	mstr_temp.Format(_T("%d"), st_handler_info.tCreate.GetMinute());
	:: WritePrivateProfileString(_T("TIME"), _T("CREATE_MINUTE"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	mstr_temp.Format(_T("%d"), st_handler_info.tCreate.GetSecond());
	:: WritePrivateProfileString(_T("TIME"), _T("CREATE_SECOND"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	// 20140811 jtkim
	//:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("strToolName"), st_basic_info.strToolName, st_path_info.strFileBasic);
	//
	//:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("strFtpUser"), st_basic_info.strFtpUser, st_path_info.strFileBasic);
	//
	//:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("strFtpPass"), st_basic_info.strFtpPass, st_path_info.strFileBasic);
	//
	//:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("strPathFtpGms"), st_path_info.strPathFtpGms, st_path_info.strFileBasic);

	mstr_temp.Format(_T("%d"), st_lot_info[LOT_CURR].nLotStatus);
	:: WritePrivateProfileString(_T("LOT_INFO"), _T("LOT_CURR_STATUS"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	mstr_temp.Format(_T("%d"), st_lot_info[LOT_NEXT].nLotStatus);
	:: WritePrivateProfileString(_T("LOT_INFO"), _T("LOT_NEXT_STATUS"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

// 	for (i=0; i<8; i++)
// 	{
// 		mstr_temp.Format(_T("VnR_Voltage_Spec #%02d"), i+1);
// 		:: WritePrivateProfileString(_T("BASIC_SCREEN"), mstr_temp, st_basic_info.strVnrSpec[1][i], st_path_info.strFileBasic);
// 		
// 		mstr_temp.Format(_T("VnR_Resistance_Spec #%02d"), i+1);
// 		:: WritePrivateProfileString(_T("BASIC_SCREEN"), mstr_temp, st_basic_info.strVnrSpec[0][i], st_path_info.strFileBasic);
// 
// 		mstr_temp.Format(_T("VnR_Voltage_Ucl #%02d"), i+1);
// 		:: WritePrivateProfileString(_T("BASIC_SCREEN"), mstr_temp, st_basic_info.strVnrUcl[1][i], st_path_info.strFileBasic);
// 
// 		mstr_temp.Format(_T("VnR_Resistance_Ucl #%02d"), i+1);
// 		:: WritePrivateProfileString(_T("BASIC_SCREEN"), mstr_temp, st_basic_info.strVnrUcl[0][i], st_path_info.strFileBasic);
// 
// 		mstr_temp.Format(_T("VnR_Voltage_Lcl #%02d"), i+1);
// 		:: WritePrivateProfileString(_T("BASIC_SCREEN"), mstr_temp, st_basic_info.strVnrLcl[1][i], st_path_info.strFileBasic);
// 
// 		mstr_temp.Format(_T("VnR_Resistance_Lcl #%02d"), i+1);
// 		:: WritePrivateProfileString(_T("BASIC_SCREEN"), mstr_temp, st_basic_info.strVnrLcl[0][i], st_path_info.strFileBasic);
// 	}

	for (i=0; i<HSSI_MAX_IO; i++)
	{
		str_tmp.Format(_T("OUT_STATUS_%06d"), i);
		mstr_temp.Format(_T("%d"), FAS_IO.n_out[i]);
		:: WritePrivateProfileString(_T("IO_DATA"), str_tmp, LPCTSTR(mstr_temp), st_path_info.strFileBasic);
	}

	mstr_temp.Format(_T("%d"), st_count_info.nUph);
	:: WritePrivateProfileString(_T("BASIC"), _T("nUph"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	mstr_temp.Format(_T("%d"), st_work_info.nOldHifix);
	:: WritePrivateProfileString(_T("BASIC"), _T("nOldHifix"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	mstr_temp.Format(_T("%d"), st_basic_info.nLastMdlCount);
	:: WritePrivateProfileString(_T("BASIC"), _T("nLastMdlCount"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	mstr_temp.Format(_T("%d"), st_basic_info.nBcrRead);
	:: WritePrivateProfileString(_T("BASIC"), _T("nBcrRead"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	mstr_temp.Format(_T("%d"), st_basic_info.nBcrFail);
	:: WritePrivateProfileString(_T("BASIC"), _T("nBcrFail"), LPCTSTR(mstr_temp), st_path_info.strFileBasic);

	for (i=0; i<2; i++)
	{
		for (j=0; j<2; j++)
		{
/*
			mstr_temp.Format(_T("COUNT_IN_%02d_%02d"), i+1, j+1);
			str_tmp.Format(_T("%d"), st_count_info.nInCount[i][j]);
			:: WritePrivateProfileString(_T("BASIC_SCREEN"), mstr_temp, str_tmp, st_path_info.strFileBasic);

			mstr_temp.Format(_T("COUNT_PRIME_%02d_%02d"), i+1, j+1);
			str_tmp.Format(_T("%d"), st_count_info.nPrimeCount[i][j]);
			:: WritePrivateProfileString(_T("BASIC_SCREEN"), mstr_temp, str_tmp, st_path_info.strFileBasic);

			mstr_temp.Format(_T("COUNT_PASS_%02d_%02d"), i+1, j+1);
			str_tmp.Format(_T("%d"), st_count_info.nPassCount[i][j]);
			:: WritePrivateProfileString(_T("BASIC_SCREEN"), mstr_temp, str_tmp, st_path_info.strFileBasic);

			mstr_temp.Format(_T("COUNT_PRIME_REJECT_%02d_%02d"), i+1, j+1);
			str_tmp.Format(_T("%d"), st_count_info.nPrimeRejectCount[i][j]);
			:: WritePrivateProfileString(_T("BASIC_SCREEN"), mstr_temp, str_tmp, st_path_info.strFileBasic);

			mstr_temp.Format(_T("COUNT_REJECT_%02d_%02d"), i+1, j+1);
			str_tmp.Format(_T("%d"), st_count_info.nRejectCount[i][j]);
			:: WritePrivateProfileString(_T("BASIC_SCREEN"), mstr_temp, str_tmp, st_path_info.strFileBasic);
*/
// 			mstr_temp.Format(_T("COUNT_IN_%02d_%02d"), i+1, j+1);
// 			str_tmp.Format(_T("%d"), st_count_info.nInCount[i][j]);
// 			:: WritePrivateProfileString(_T("BASIC_SCREEN"), mstr_temp, str_tmp, st_path_info.strFileBasic);
// 
// 			mstr_temp.Format(_T("COUNT_PRIME_%02d_%02d"), i+1, j+1);
// 			str_tmp.Format(_T("%d"), st_count_info.nPrimeCount[i][j]);
// 			:: WritePrivateProfileString(_T("BASIC_SCREEN"), mstr_temp, str_tmp, st_path_info.strFileBasic);
// 
// 			mstr_temp.Format(_T("COUNT_PASS_%02d_%02d"), i+1, j+1);
// 			str_tmp.Format(_T("%d"), st_count_info.nPassCount[i][j]);
// 			:: WritePrivateProfileString(_T("BASIC_SCREEN"), mstr_temp, str_tmp, st_path_info.strFileBasic);
// 
// 			mstr_temp.Format(_T("COUNT_PRIME_REJECT_%02d_%02d"), i+1, j+1);
// 			str_tmp.Format(_T("%d"), st_count_info.nPrimeRejectCount[i][j]);
// 			:: WritePrivateProfileString(_T("BASIC_SCREEN"), mstr_temp, str_tmp, st_path_info.strFileBasic);
// 
// 			mstr_temp.Format(_T("COUNT_REJECT_%02d_%02d"), i+1, j+1);
// 			str_tmp.Format(_T("%d"), st_count_info.nRejectCount[i][j]);
// 			:: WritePrivateProfileString(_T("BASIC_SCREEN"), mstr_temp, str_tmp, st_path_info.strFileBasic);
		}
	}

	// jtkim 20150709
	str_tmp.Format(_T("%d"), st_count_info.nUph);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("UPH_COUNT"), LPCTSTR(str_tmp), st_path_info.strFileBasic);

	// jtkim 20150709
	str_tmp.Format(_T("%d"), st_count_info.nDailyUph);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("DAILY_UPH_COUNT"), LPCTSTR(str_tmp), st_path_info.strFileBasic);

	// jtkim 20150709
	str_tmp.Format(_T("%d"), st_count_info.nUphCnt);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("nUphCnt"), LPCTSTR(str_tmp), st_path_info.strFileBasic);

	// jtkim 20150709
	str_tmp.Format(_T("%.2f"), st_count_info.dHourPer);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("dHourPer"), LPCTSTR(str_tmp), st_path_info.strFileBasic);

	str_tmp.Format(_T("%.2f"), st_count_info.dDailyPer);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("dDailyPer"), LPCTSTR(str_tmp), st_path_info.strFileBasic);

// 	for (i=0; i<12; i++)
// 	{
// 		mstr_temp.Format(_T("TSITE_OFFSET_Y_%02d"), i);
// 		str_tmp.Format(_T("%.3f"), st_recipe_info.dTsiteOffsetY[i]);
// 		:: WritePrivateProfileString(_T("TSITE_OFFSET"), mstr_temp, str_tmp, st_path_info.strFileBasic);
// 	}
// 
// 	for (i=0; i<24; i++)		//2015.03.17 sjs
// 	{
// 		mstr_temp.Format(_T("UPH_%dh"), i);
// 		str_tmp.Format(_T("%d"), st_count_info.nLatestUph[i]);
// 		:: WritePrivateProfileString(_T("LATEST_UPH"), mstr_temp, str_tmp, st_path_info.strFileBasic);
// 	}

	// jtkim 20150323
// 	for (i=0; i<2; i++)
// 	{
// 		for (j=0; j<2; j++)
// 		{
// 			for (k=0; k<12; k++)
// 			{
// 				mstr_temp.Format(_T("BdTestCnt_%02d_%02d_%02d"), i+1, j+1, k+1);
// 				str_tmp.Format(_T("%d"), st_bd_info[i][j].nBdTestCnt[k]);
// 				:: WritePrivateProfileString(_T("BASIC_SCREEN"), mstr_temp, str_tmp, st_path_info.strFileBasic);
// 
// 				mstr_temp.Format(_T("BdPassCnt_%02d_%02d_%02d"), i+1, j+1, k+1);
// 				str_tmp.Format(_T("%d"), st_bd_info[i][j].nBdPassCnt[k]);
// 				:: WritePrivateProfileString(_T("BASIC_SCREEN"), mstr_temp, str_tmp, st_path_info.strFileBasic);
// 
// 				mstr_temp.Format(_T("BdInfo_%02d_%02d_%02d"), i+1, j+1, k+1);
// 				str_tmp.Format(_T("%d"), st_bd_info[i][j].nBdInfo[k]);
// 				:: WritePrivateProfileString(_T("BASIC_SCREEN"), mstr_temp, str_tmp, st_path_info.strFileBasic);
// 
// 				mstr_temp.Format(_T("BdBin_%02d_%02d_%02d"), i+1, j+1, k+1);
// 				str_tmp.Format(_T("%d"), st_bd_info[i][j].nBdBin[k]);
// 				:: WritePrivateProfileString(_T("BASIC_SCREEN"), mstr_temp, str_tmp, st_path_info.strFileBasic);
// 
// 				mstr_temp.Format(_T("BdFailContiCnt_%02d_%02d_%02d"), i+1, j+1, k+1);
// 				str_tmp.Format(_T("%d"), st_bd_info[i][j].nBdFailContiCnt[k]);
// 				:: WritePrivateProfileString(_T("BASIC_SCREEN"), mstr_temp, str_tmp, st_path_info.strFileBasic);
// 
// 				mstr_temp.Format(_T("strBinHistory_%02d_%02d_%02d"), i+1, j+1, k+1);
// 				:: WritePrivateProfileString(_T("BASIC_SCREEN"), mstr_temp, st_bd_info[i][j].strBinHistory[k], st_path_info.strFileBasic);
// 			}
// 		}
// 	}

	// jtkim 20150330 cok


// 	for (i=0; i<4; i++)
// 	{
// 		mstr_temp.Format(_T("BUFFER_COK_NUM_%02d"), i+1);
// 		str_tmp.Format(_T("%d"), st_Cok_Buff_info[i].st_pcb_info.nCOK_IDNum);
// 		:: WritePrivateProfileString(_T("BASIC_SCREEN"), mstr_temp, str_tmp, st_path_info.strFileBasic);
// 
// 		mstr_temp.Format(_T("BUFFER_COK_COUNT_%02d"), i+1);
// 		str_tmp.Format(_T("%d"), st_Cok_Buff_info[i].nCokCount);
// 		:: WritePrivateProfileString(_T("BASIC_SCREEN"), mstr_temp, str_tmp, st_path_info.strFileBasic);
// 
// 		for (j=0; j<24; j++)
// 		{
// 			mstr_temp.Format(_T("BUFFER_COK_%02d_%02d"), i+1, j+1);
// 			str_tmp.Format(_T("%d"), st_Cok_Buff_info[i].st_pcb_info.nYesNo[j]);
// 			:: WritePrivateProfileString(_T("BASIC_SCREEN"), mstr_temp, str_tmp, st_path_info.strFileBasic);
// 		}
// 	}

// 	for (i=0; i<5; i++)
// 	{
// 		mstr_temp.Format(_T("COK_MODE_%02d"), i+1);
// 		:: WritePrivateProfileString(_T("BASIC_SCREEN"), mstr_temp, st_basic_info.strHifixType[i], st_path_info.strFileBasic);
// 	}

	for (i=0; i<M_MAX_MOTOR_NUM; i++)
	{
		mstr_temp.Format(_T("MOTOR_LAST_%02d"), i+1);
		str_tmp.Format(_T("%.3f"), st_work_info.dCurrPos[i]);
		:: WritePrivateProfileString(_T("BASIC_SCREEN"), mstr_temp, LPCTSTR(str_tmp), st_path_info.strFileBasic);
	}

// 	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("strScrapName"), st_basic_info.strScrapName, st_path_info.strFileBasic);
// 
// 	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("strScrapHead"), st_basic_info.strScrapHead, st_path_info.strFileBasic);
// 
// 	// 20150623 in-line
// 	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("strProcess"), st_basic_info.strProcess, st_path_info.strFileBasic);


	// jtkim 20150709
// 	str_tmp.Format(_T("%d"), st_handler_info.nFtpVer);
// 	:: WritePrivateProfileString(_T("BASIC"), _T("nFtpVer"), str_tmp, st_path_info.strFileBasic);
// 
// 	// jtkim 20150709
// 	str_tmp.Format(_T("%d"), st_basic_info.nSocketCount);
// 	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("nSocketCount"), str_tmp, st_path_info.strFileBasic);

	// jtkim 20150902
// 	:: WritePrivateProfileString(_T("HIFIX"), _T("HIFIX_01"), st_work_info.strHifix[0], st_path_info.strFileBasic);
// 
// 	:: WritePrivateProfileString(_T("HIFIX"), _T("HIFIX_02"), st_work_info.strHifix[1], st_path_info.strFileBasic);
// 
// 	:: WritePrivateProfileString(_T("HIFIX"), _T("HIFIX_03"), st_work_info.strHifix[2], st_path_info.strFileBasic);
// 
// 	:: WritePrivateProfileString(_T("HIFIX"), _T("HIFIX_04"), st_work_info.strHifix[3], st_path_info.strFileBasic);
// 
// 	:: WritePrivateProfileString(_T("HIFIX"), _T("HIFIX_05"), st_work_info.strHifix[4], st_path_info.strFileBasic);

	// jtkim 20150930
	str_tmp.Format(_T("%d"), st_basic_info.nRecipeYesNo);
	:: WritePrivateProfileString(_T("BASIC"), _T("nRecipeYesNo"), LPCTSTR(str_tmp), st_path_info.strFileBasic);

	// jtkim 20151229 사용관련
	str_tmp.Format(_T("%d"), st_basic_info.nRetestCount);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("nRetestCount"), LPCTSTR(str_tmp), st_path_info.strFileBasic);

	// jtkim 20160124
	str_tmp.Format(_T("%d"), st_recipe_info.nAbortPer);
	:: WritePrivateProfileString(_T("BASIC_SCREEN"), _T("nAbortPer"), LPCTSTR(str_tmp), st_path_info.strFileBasic);
}

CString CMyBasicData::OnGet_File_Name()
{
	CString strTemp;       // 임시 저장 변수
	CString strSaveFile;  // 저장 파일 임시 설정 변수
	CString strChkExt;    // 파일 확장자 저장 변수
	CString strPart, strPart2;
	CString strNewSaveFile;
	int nPos;
	
	// **************************************************************************
    // Socket Contact Count 데이터 저장할 파일 설정한다 [파일 확장자 검사]        
    // **************************************************************************
	strSaveFile = st_path_info.strPathDvc + st_basic_info.strDeviceName;  // 티칭 데이터 저장 파일 설정
	
	nPos = strSaveFile.Find(_T("."));  // 확장자 위치 검사
	
	if (nPos == -1)
	{
		strSaveFile += _T(".DAT");  // 확장자 추가
	}
	else 
	{
		strChkExt = strSaveFile.Mid(nPos);  // 파일 확장자 설정

		if (strChkExt != _T(".TXT"))  
		{
			strSaveFile = st_path_info.strPathDvc + _T("DEFAULT.TXT");  // 티칭 데이터 저장 새로운 파일 설정
/*			
			if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
			{
				st_other.str_abnormal_msg = _T("[DEVICE FILE] The error happened at a file extension.");
				sprintf(st_other.c_abnormal_msg, st_other.str_abnormal_msg);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 오류 출력 요청
			}*/
		}
	}
	// **************************************************************************
	
	return strSaveFile;  // 파일명 리턴 
}

void CMyBasicData::OnDeviec_Folder_Load()
{

}

void CMyBasicData::OnDevice_Folder_Save()
{
	
}

void CMyBasicData::OnInterface_Data_Load()
{
	int i;
	CString str_temp, str_name;
	char ch_data[100];
//	char ch_tmp;

	for(i=0; i<10; i++)
	{
		str_name.Format(_T("%02d_CLIENT_IP"), i);
		:: GetPrivateProfileString(_T("INTERFACE_SCREEN"), str_name, _T(""), (LPWSTR)ch_data, 100, st_path_info.strFileBasic);
//		sprintf(ch_data,"%S", ch_data);
//		clsFunc.OnCharToString(ch_data, 100);
		str_temp.Format(_T("%s"), ch_data);
		st_client_info[i].strIp = str_temp;

		str_name.Format(_T("%02d_CLIENT_PORT"), i);
		:: GetPrivateProfileString(_T("INTERFACE_SCREEN"), str_name, _T(""), (LPWSTR)ch_data, 100, st_path_info.strFileBasic);
		str_temp.Format(_T("%s"), ch_data);
		st_client_info[i].nPort = _wtoi(str_temp);

		str_name.Format(_T("%02d_SERVER_PORT"), i);
		:: GetPrivateProfileString(_T("INTERFACE_SCREEN"), str_name, _T(""), (LPWSTR)ch_data, 100, st_path_info.strFileBasic);
		str_temp.Format(_T("%s"), ch_data);
		st_server_info[i].nPort = _wtoi(str_temp);

		str_name.Format(_T("PORT_%02d"), i+1);
		:: GetPrivateProfileString(_T("SERIAL"), str_name, _T("1"), (LPWSTR)ch_data, 100, st_path_info.strFileBasic);
		str_temp.Format(_T("%s"), ch_data);
		st_serial_info.nSerialPort[i] = _wtoi(str_temp);
		if(st_serial_info.nSerialPort[i] < 1)
			st_serial_info.nSerialPort[i] = 1;
		
		str_name.Format(_T("BAUDRATE_%02d"), i+1);
		:: GetPrivateProfileString(_T("SERIAL"), str_name, _T("9600"), (LPWSTR)&ch_data, 100, st_path_info.strFileBasic);
		str_temp.Format(_T("%s"), ch_data);
		st_serial_info.nSerialBaudrate[i] = _wtoi(str_temp);

		
		str_name.Format(_T("DATA_%02d"), i+1);
		:: GetPrivateProfileString(_T("SERIAL"), str_name, _T("8"), (LPWSTR)ch_data, 100, st_path_info.strFileBasic);
		str_temp.Format(_T("%s"), ch_data);
		st_serial_info.nSerialData[i] = _wtoi(str_temp);
		
		str_name.Format(_T("STOP_%02d"), i+1);
		:: GetPrivateProfileString(_T("SERIAL"), str_name, _T("1"), (LPWSTR)ch_data, 100, st_path_info.strFileBasic);
		str_temp.Format(_T("%s"), ch_data);
		st_serial_info.nSerialStop[i] = _wtoi(str_temp);
		
		str_name.Format(_T("PARITY_%02d"), i+1);
		:: GetPrivateProfileString(_T("SERIAL"), str_name, _T("0"), (LPWSTR)ch_data, 100, st_path_info.strFileBasic);
		str_temp.Format(_T("%s"), ch_data);
		st_serial_info.nSerialParity[i] = _wtoi(str_temp);	
	}
}

void CMyBasicData::OnInterface_Data_Save()
{
	int i;
	CString str_tmp, str_name;

	for(i=0; i<10; i++)
	{
		str_name.Format(_T("%02d_CLIENT_IP"), i);
		str_tmp.Format(_T("%s"), st_client_info[i].strIp);
		:: WritePrivateProfileString(_T("INTERFACE_SCREEN"), LPCTSTR(str_name), LPCTSTR(str_tmp), st_path_info.strFileBasic);

		str_name.Format(_T("%02d_CLIENT_PORT"), i);
		str_tmp.Format(_T("%d"), st_client_info[i].nPort);
		:: WritePrivateProfileString(_T("INTERFACE_SCREEN"), LPCTSTR(str_name), LPCTSTR(str_tmp), st_path_info.strFileBasic);

		str_name.Format(_T("%02d_SERVER_PORT"), i);
		str_tmp.Format(_T("%d"), st_server_info[i].nPort);
		:: WritePrivateProfileString(_T("INTERFACE_SCREEN"), LPCTSTR(str_name), LPCTSTR(str_tmp), st_path_info.strFileBasic);

		str_name.Format(_T("PORT_%02d"), i+1);
		str_tmp.Format(_T("%d"), st_serial_info.nSerialPort[i]);
		:: WritePrivateProfileString(_T("SERIAL"), LPCTSTR(str_name), LPCTSTR(str_tmp), st_path_info.strFileBasic);

		str_name.Format(_T("BAUDRATE_%02d"), i+1);
		str_tmp.Format(_T("%d"), st_serial_info.nSerialBaudrate[i]);
		:: WritePrivateProfileString(_T("SERIAL"), LPCTSTR(str_name), LPCTSTR(str_tmp), st_path_info.strFileBasic);

		str_name.Format(_T("DATA_%02d"), i+1);
		str_tmp.Format(_T("%d"), st_serial_info.nSerialData[i]);
		:: WritePrivateProfileString(_T("SERIAL"), LPCTSTR(str_name), LPCTSTR(str_tmp), st_path_info.strFileBasic);

		str_name.Format(_T("STOP_%02d"), i+1);
		str_tmp.Format(_T("%d"), st_serial_info.nSerialStop[i]);
		:: WritePrivateProfileString(_T("SERIAL"), LPCTSTR(str_name), LPCTSTR(str_tmp), st_path_info.strFileBasic);

		str_name.Format(_T("PARITY_%02d"), i+1);
		str_tmp.Format(_T("%d"), st_serial_info.nSerialParity[i]);
		:: WritePrivateProfileString(_T("SERIAL"), LPCTSTR(str_name), LPCTSTR(str_tmp), st_path_info.strFileBasic);
	}
}

void CMyBasicData::OnInterface_Data_Save_As(CString strDeviceName)
{
	int i;
	CString str_tmp, str_name;

	for(i=0; i<10; i++)
	{
		str_name.Format(_T("%02d_CLIENT_IP"), i);
		str_tmp.Format(_T("%s"), st_client_info[i].strIp);
		:: WritePrivateProfileString(_T("INTERFACE_SCREEN"), str_name, LPCTSTR(str_tmp), st_path_info.strFileBasic);

		str_name.Format(_T("%02d_CLIENT_PORT"), i);
		str_tmp.Format(_T("%d"), st_client_info[i].nPort);
		:: WritePrivateProfileString(_T("INTERFACE_SCREEN"), str_name, LPCTSTR(str_tmp), st_path_info.strFileBasic);

		str_name.Format(_T("%02d_SERVER_PORT"), i);
		str_tmp.Format(_T("%d"), st_server_info[i].nPort);
		:: WritePrivateProfileString(_T("INTERFACE_SCREEN"), str_name, LPCTSTR(str_tmp), st_path_info.strFileBasic);

		str_name.Format(_T("PORT_%02d"), i+1);
		str_tmp.Format(_T("%d"), st_serial_info.nSerialPort[i]);
		:: WritePrivateProfileString(_T("SERIAL"), str_name, LPCTSTR(str_tmp), st_path_info.strFileBasic);

		str_name.Format(_T("BAUDRATE_%02d"), i+1);
		str_tmp.Format(_T("%d"), st_serial_info.nSerialBaudrate[i]);
		:: WritePrivateProfileString(_T("SERIAL"), str_name, LPCTSTR(str_tmp), st_path_info.strFileBasic);

		str_name.Format(_T("DATA_%02d"), i+1);
		str_tmp.Format(_T("%d"), st_serial_info.nSerialData[i]);
		:: WritePrivateProfileString(_T("SERIAL"), str_name, LPCTSTR(str_tmp), st_path_info.strFileBasic);

		str_name.Format(_T("STOP_%02d"), i+1);
		str_tmp.Format(_T("%d"), st_serial_info.nSerialStop[i]);
		:: WritePrivateProfileString(_T("SERIAL"), str_name, LPCTSTR(str_tmp), st_path_info.strFileBasic);

		str_name.Format(_T("PARITY_%02d"), i+1);
		str_tmp.Format(_T("%d"), st_serial_info.nSerialParity[i]);
		:: WritePrivateProfileString(_T("SERIAL"), str_name, LPCTSTR(str_tmp), st_path_info.strFileBasic);
	}
}

void CMyBasicData::OnBasic_Data_Save_As(CString strDeviceName)
{
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	CString str_save_file;
	CString str_part, str_chk_ext;
	CString str_pos;
	COleDateTime time_cur;

	/* ************************************************************************** */
	/*  데이터 저장할 파일 설정한다 [파일 확장자 검사]                            */
	/* ************************************************************************** */
	:: WritePrivateProfileString(_T("FILE_NAME"), _T("Device_Type"), LPCWSTR(strDeviceName), st_path_info.strFileBasic);

	str_save_file = st_path_info.strBasic + strDeviceName;  // 티칭 데이터 저장 파일 설정

	mstr_temp.Format(_T("%d"), st_basic_info.nModeDevice);
	:: WritePrivateProfileString(_T("BASIC"), _T("DEVICE_MODE"), LPCWSTR(strDeviceName), str_save_file);
}

void CMyBasicData::OnMaintenance_Data_Save_As(CString strDevice)
{
	CString strTemp, strPart;  // 로딩 정보 임시 저장 변수
//	char chBuf[20] ;
	CString strSaveFile;;

	// **************************************************************************
	// 타워 램프 RED 상태 정보를 파일에 저장한다                                 
	// ->  STOP			= 0,
	// 		   RUN			= 1,
	// 		   ALARM		= 2,
	// 		   LOTEND		= 3,
	// 		   INIT			= 4,
	// 		   WARRING		= 5,
	// 		   LOCK			= 6,
	// 		   SELFCHECK	= 7														 
	// -> 로딩 값 [0:OFF    1:ON    2:FLICKER]                                   
	// **************************************************************************
//	str_save_file = st_path.str_basic + str_device;  // 티칭 데이터 저장 파일 설정
	strSaveFile = st_path_info.strBasic + _T("Maintenance.TXT");

	strTemp.Format(_T("%d"), st_lamp_info.nLampR[0]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Red"), _T("Stop_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampR[1]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Red"), _T("Run_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampR[2]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Red"), _T("Alarm_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampR[3]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Red"), _T("LotEnd_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampR[4]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Red"), _T("Initial_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampR[5]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Red"), _T("Warring_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampR[6]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Red"), _T("Lock_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampR[7]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Red"), _T("SelfCheck_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampR[8]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Red"), _T("Idle_State"), LPCTSTR(strTemp), strSaveFile);
	// **************************************************************************


	// **************************************************************************
	// 타워 램프 YELLOW 상태 정보를 파일에 저장한다                              
	// ->  STOP			= 0,
// 		   RUN			= 1,
// 		   ALARM		= 2,
// 		   LOTEND		= 3,
// 		   INIT			= 4,
// 		   WARRING		= 5,
// 		   LOCK			= 6,
// 		   SELFCHECK	= 7														 
	// -> 로딩 값 [0:OFF    1:ON    2:FLICKER]                                   
	// **************************************************************************
	strTemp.Format(_T("%d"),st_lamp_info.nLampY[0]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Yellow"), _T("Stop_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampY[1]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Yellow"), _T("Run_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampY[2]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Yellow"), _T("Alarm_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampY[3]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Yellow"), _T("LotEnd_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampY[4]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Yellow"), _T("Initial_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampY[5]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Yellow"), _T("Warring_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampY[6]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Yellow"), _T("Lock_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampY[7]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Yellow"), _T("SelfCheck_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampY[8]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Yellow"), _T("Idle_State"), LPCTSTR(strTemp), strSaveFile);

	// **************************************************************************

	// **************************************************************************
	// 타워 램프 GREEN 상태 정보를 파일에 저장한다                               
	// ->  STOP			= 0,
// 		   RUN			= 1,
// 		   ALARM		= 2,
// 		   LOTEND		= 3,
// 		   INIT			= 4,
// 		   WARRING		= 5,
// 		   LOCK			= 6,
// 		   SELFCHECK	= 7														 
	// -> 로딩 값 [0:OFF    1:ON    2:FLICKER]                                   
	// **************************************************************************
	strTemp.Format(_T("%d"),st_lamp_info.nLampG[0]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Green"), _T("Stop_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampG[1]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Green"), _T("Run_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampG[2]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Green"), _T("Alarm_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampG[3]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Green"), _T("LotEnd_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampG[4]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Green"), _T("Initial_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampG[5]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Green"), _T("Warring_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampG[6]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Green"), _T("Lock_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampG[7]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Green"), _T("SelfCheck_State"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"),st_lamp_info.nLampG[8]) ;
	:: WritePrivateProfileString(_T("TowerLampData_Green"), _T("Idle_State"), LPCTSTR(strTemp), strSaveFile);
	// **************************************************************************

	// **************************************************************************
	// 부저 사용 모드 로딩하여 전역 변수에 설정                                  
	// -> 0:사용    1:미사용                                                     
	// **************************************************************************
	strTemp.Format(_T("%d"),st_lamp_info.nBuzzerMode) ;
	:: WritePrivateProfileString(_T("TowerLampData"), _T("n_buzzer_mode"), LPCTSTR(strTemp), strSaveFile);
	// **************************************************************************

	// **************************************************************************
	// 장비 호기 및 장비 코드 로딩하여 전역 변수에 설정                          
	// -> 로딩 정보에서 앞/뒤 공백은 제거한다                                    
	// **************************************************************************
	:: WritePrivateProfileString(_T("TowerLampData"), _T("str_equip_no"), LPCTSTR(st_lamp_info.strEquipNo), strSaveFile);
	:: WritePrivateProfileString(_T("TowerLampData"), _T("str_equip_code"), LPCTSTR(st_lamp_info.strEquipCode), strSaveFile);
	// **************************************************************************

	// **************************************************************************
	// 타워 램프 ON/OFF 대기 시간 로딩하여 전역 변수에 설정                      
	// **************************************************************************
	strTemp.Format(_T("%d"),st_lamp_info.nLampWaitTime);
	:: WritePrivateProfileString(_T("TowerLampData"), _T("n_lamp_on_time_w"), LPCTSTR(strTemp), strSaveFile);
	// **************************************************************************
}

void CMyBasicData::OnModuleRobot_Teach_Data_Save_As(CString strDeviceName)
{
	CString strTemp, strMsg, strSaveFail;       // 임시 저장 변수
	CString strPos;
	CString strHead, strItem;
	int i, j;
	
	strSaveFail = st_path_info.strFileMotor + strDeviceName;
	
	for(i=0; i<MAXMOTOR; i++)
	{
		strHead.Format(_T("Motor%d"),i+1);
		for(j=0; j<M_MAX_POS; j++)
		{
			strItem.Format(_T("%02d_Axis_[%02d]"), i+1, j+1); 
			strTemp.Format(_T("%.3f"), st_motor_info[i].d_pos[j]);
			:: WritePrivateProfileString(strHead, strItem, LPCTSTR(strTemp), strSaveFail);
		}
	}
}





CString CMyBasicData::GetWaitTimeName(int n_mode)
{
	CString strName;

	strName = "";

	switch(n_mode)
	{
		case 0:
			strName = "0";
			break;

		case 1:
			strName = "1";
			break;

		case 2:
			strName = "2";
			break;

		case 3:
			strName = "3";
			break;

		case 4:
			strName = "4";
			break;

		case 5:
			strName = "5";
			break;

		case 6:
			strName = "6";
			break;

		case 7:
			strName = "7";
			break;

		case 8:
			strName = "8";
			break;

		case 9:
			strName = "9";
			break;

		case 10:
			strName = "10";
			break;

		case 11:
			strName = "11";
			break;

		case 12:
			strName = "12";
			break;

		case 13:
			strName = "13";
			break;

		case 14:
			strName = "14";
			break;

		case 15:
			strName = "15";
			break;

		case 16:
			strName = "16";
			break;

		case 17:
			strName = "17";
			break;

		case 18:
			strName = "18";
			break;

		case 19:
			strName = "19";
			break;
	}

	return strName;
}

void CMyBasicData::OnRecipe_Data_Load()
{
	CString strTemp;  // 로딩 정보 임시 저장 변수
	CString strHead;
	CString strLoadFile;

	char chData[200];

	int i, j, nChk;
	
	// 읽을 경로 
	strLoadFile = st_path_info.strBasic + st_basic_info.strDeviceName;  

	:: GetPrivateProfileString(_T("RECIPE"), _T("nTrayY"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.nTrayY = _wtoi(strTemp);

	:: GetPrivateProfileString(_T("RECIPE"), _T("nTrayX"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.nTrayX = _wtoi(strTemp);

	:: GetPrivateProfileString(_T("RECIPE"), _T("nRejectTray_X"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.nRejectTray_X = _wtoi(strTemp);

	:: GetPrivateProfileString(_T("RECIPE"), _T("nRetestTray_X"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.nRetestTray_X = _wtoi(strTemp);

	:: GetPrivateProfileString(_T("RECIPE"), _T("dTrayYOffset"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.dTrayYOffset = _wtof(strTemp);

	:: GetPrivateProfileString(_T("RECIPE"), _T("dTrayXOffset"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.dTrayXOffset = _wtof(strTemp);

	:: GetPrivateProfileString(_T("RECIPE"), _T("dBufferPitch_Y"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.dBufferPitch_Y = _wtof(strTemp);

	:: GetPrivateProfileString(_T("RECIPE"), _T("dBufferPitch_X"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.dBufferPitch_X = _wtof(strTemp);

	:: GetPrivateProfileString(_T("RECIPE"), _T("dTSiteOffsetX"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.dTSiteOffsetX = _wtof(strTemp);

	:: GetPrivateProfileString(_T("RECIPE"), _T("dCOKBuffPitch_X"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.dCOKBuffPitch_X = _wtof(strTemp);

	:: GetPrivateProfileString(_T("RECIPE"), _T("dRejTrayPick"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.dRejTrayPick = _wtof(strTemp);

	:: GetPrivateProfileString(_T("RECIPE"), _T("dRejTrayPlace"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.dRejTrayPlace = _wtof(strTemp);
	

	:: GetPrivateProfileString(_T("RECIPE"), _T("nCOKBuff_IDNum_Type"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.nHifix = st_recipe_info.nCOKBuff_IDNum_Type = _wtoi(strTemp);

	:: GetPrivateProfileString(_T("RECIPE"), _T("strHifix"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.strHifix = strTemp;

	:: GetPrivateProfileString(_T("RECIPE"), _T("Buffer_DvcCheckSensor_Mode"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.Buffer_DvcCheckSensor_Mode = _wtoi(strTemp);
/*
	for (i=0; i<5; i++)
	{
		strHead.Format(_T("Hifix_%02d_Name"), i);
		:: GetPrivateProfileString(_T("RECIPE"), strHead, _T(""), (LPWSTR)chData, 30, strLoadFile);
		strTemp.Format(_T("%s"), chData);
		st_recipe_info.strHifix[i] = strTemp;
	}*/

	:: GetPrivateProfileString(_T("RECIPE"), _T("nBufferRotator_PlateSite_Degrees"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.nBufferRotator_PlateSite_Degrees = _wtoi(strTemp);

	:: GetPrivateProfileString(_T("RECIPE"), _T("nBufferRotator_TestSite_Degrees"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.nBufferRotator_TestSite_Degrees = _wtoi(strTemp);

	// jtkim 20151017
	:: GetPrivateProfileString(_T("RECIPE"), _T("nBufferRotator_Barcode_Degrees"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.nBufferRotator_Barcode_Degrees = _wtoi(strTemp);

	:: GetPrivateProfileString(_T("RECIPE"), _T("nAbortTime"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.nAbortTime = _wtoi(strTemp);

	:: GetPrivateProfileString(_T("RECIPE"), _T("nBcrBank"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.nBcrBank = _wtoi(strTemp);

	:: GetPrivateProfileString(_T("RECIPE"), _T("bUnldFlagA"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	nChk = _wtoi(strTemp);
	if (nChk>=0 && nChk<2)
	{
		st_recipe_info.bUnldBcrFlag[0]	=	nChk;
	}
	else
	{
		st_recipe_info.bUnldBcrFlag[0]	=	FALSE;
	}

	:: GetPrivateProfileString(_T("RECIPE"), _T("bUnldFlagB"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	nChk = _wtoi(strTemp);
	if (nChk>=0 && nChk<2)
	{
		st_recipe_info.bUnldBcrFlag[1]	=	nChk;
	}
	else
	{
		st_recipe_info.bUnldBcrFlag[1]	=	FALSE;
	}

	for (i=0; i<4; i++)
	{
		strHead.Format(_T("COK_BUFF_#%d"), i+1);

		::GetPrivateProfileString(_T("RECIPE"), strHead, _T("-1"), (LPWSTR)chData, 30, strLoadFile);
		strTemp.Format(_T("%s"), chData);
		nChk = _wtoi(strTemp);

		if (nChk > 0 && nChk < 9)
		{
			st_recipe_info.nCokType[i]	=	nChk;
		}
		else
		{
			st_recipe_info.nCokType[i]	=	0;
		}
	}

	for (i=0; i<4; i++)
	{
		strHead.Format(_T("COK_TYPE_#%d_NAME"), i+1);

		::GetPrivateProfileString(_T("RECIPE"), strHead, _T(""), (LPWSTR)chData, 30, strLoadFile);
		strTemp.Format(_T("%s"), chData);
		st_recipe_info.strCokTypeName[i]	=	strTemp;

		for (j=0; j<3; j++)
		{
			strHead.Format(_T("COK_TYPE_#%d_CHECK_#%d"), i+1, j+1);

			::GetPrivateProfileString(_T("RECIPE"), strHead, _T("false"), (LPWSTR)chData, 30, strLoadFile);
			strTemp.Format(_T("%s"), chData);

			if (strTemp == _T("true")) st_recipe_info.bCokChk[i][j]	=	true;
			else	st_recipe_info.bCokChk[i][j]	=	false;
		}
	}

	:: GetPrivateProfileString(_T("RECIPE"), _T("strPartNo"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.strPartNo = strTemp;

/*
	for (i=0; i<2; i++)		//SOCKET COMMENT 2015.08.04
	{
		for (j=0; j<12; j++)
		{
			strHead.Format(_T("#%02d SOCKET_COMMENT"), (i*12) + (j+1));

			:: GetPrivateProfileString(_T("RECIPE"), strHead, _T(""), (LPWSTR)chData, 200, strLoadFile);

			st_bd_info[0][i].strSocketComment[j].Format(_T("%s"), chData);
		}
	}

	for (i=0; i<2; i++)
	{
		for (j=0; j<12; j++)
		{
			strHead.Format(_T("#%02d SOCKET_COMMENT"), (((i*12)  + (j+1)) + 24));

			:: GetPrivateProfileString(_T("RECIPE"), strHead, _T(""), (LPWSTR)chData, 200, strLoadFile);

			st_bd_info[1][i].strSocketComment[j].Format(_T("%s"), chData);
		}
	}
*/
	:: GetPrivateProfileString(_T("RECIPE"), _T("nIndividualPick"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.nIndividualPick = _wtoi(strTemp);

	:: GetPrivateProfileString(_T("RECIPE"), _T("d15TOffset"), _T("0"), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.d15TOffset = _wtoi(strTemp);
}


void CMyBasicData::OnRecipe_Data_Load(CString strDeviceName)
{
	CString strTemp;  // 로딩 정보 임시 저장 변수
	CString strLoadFile;
	CString strHead;

	char chData[200];

	int i, j, nChk;
	
	// 읽을 경로  
	strLoadFile = st_path_info.strBasic + strDeviceName; 

	:: GetPrivateProfileString(_T("RECIPE"), _T("nTrayY"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.nTrayY = _wtoi(strTemp);

	:: GetPrivateProfileString(_T("RECIPE"), _T("nTrayX"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.nTrayX = _wtoi(strTemp);

	:: GetPrivateProfileString(_T("RECIPE"), _T("nRejectTray_X"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.nRejectTray_X = _wtoi(strTemp);

	:: GetPrivateProfileString(_T("RECIPE"), _T("nRetestTray_X"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.nRetestTray_X = _wtoi(strTemp);

	:: GetPrivateProfileString(_T("RECIPE"), _T("dTrayYOffset"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.dTrayYOffset = _wtof(strTemp);

	:: GetPrivateProfileString(_T("RECIPE"), _T("dTrayXOffset"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.dTrayXOffset = _wtof(strTemp);

	:: GetPrivateProfileString(_T("RECIPE"), _T("dBufferPitch_Y"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.dBufferPitch_Y = _wtof(strTemp);

	:: GetPrivateProfileString(_T("RECIPE"), _T("dBufferPitch_X"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.dBufferPitch_X = _wtof(strTemp);

	:: GetPrivateProfileString(_T("RECIPE"), _T("dTSiteOffsetX"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.dTSiteOffsetX = _wtof(strTemp);

	:: GetPrivateProfileString(_T("RECIPE"), _T("dCOKBuffPitch_X"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.dCOKBuffPitch_X = _wtof(strTemp);

	:: GetPrivateProfileString(_T("RECIPE"), _T("dRejTrayPick"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.dRejTrayPick = _wtof(strTemp);

	:: GetPrivateProfileString(_T("RECIPE"), _T("dRejTrayPlace"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.dRejTrayPlace = _wtof(strTemp);


	:: GetPrivateProfileString(_T("RECIPE"), _T("nCOKBuff_IDNum_Type"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.nHifix = st_recipe_info.nCOKBuff_IDNum_Type = _wtoi(strTemp);

	:: GetPrivateProfileString(_T("RECIPE"), _T("strHifix"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.strHifix = strTemp;

	:: GetPrivateProfileString(_T("RECIPE"), _T("Buffer_DvcCheckSensor_Mode"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.Buffer_DvcCheckSensor_Mode = _wtoi(strTemp);
/*
	for (i=0; i<5; i++)
	{
		strHead.Format(_T("Hifix_%02d_Name"), i);
		:: GetPrivateProfileString(_T("RECIPE"), strHead, _T(""), (LPWSTR)chData, 30, strLoadFile);
		strTemp.Format(_T("%s"), chData);
		st_recipe_info.strHifix[i] = strTemp;
	}
*/
	:: GetPrivateProfileString(_T("RECIPE"), _T("nBufferRotator_PlateSite_Degrees"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.nBufferRotator_PlateSite_Degrees = _wtoi(strTemp);

	:: GetPrivateProfileString(_T("RECIPE"), _T("nBufferRotator_TestSite_Degrees"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.nBufferRotator_TestSite_Degrees = _wtoi(strTemp);

	// jtkim 20151017
	:: GetPrivateProfileString(_T("RECIPE"), _T("nBufferRotator_Barcode_Degrees"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.nBufferRotator_Barcode_Degrees = _wtoi(strTemp);

	:: GetPrivateProfileString(_T("RECIPE"), _T("nAbortTime"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.nAbortTime = _wtoi(strTemp);

	:: GetPrivateProfileString(_T("RECIPE"), _T("nBcrBank"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.nBcrBank = _wtoi(strTemp);

	:: GetPrivateProfileString(_T("RECIPE"), _T("bUnldFlagA"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	nChk = _wtoi(strTemp);
	if (nChk>=0 && nChk<2)
	{
		st_recipe_info.bUnldBcrFlag[0]	=	nChk;
	}
	else
	{
		st_recipe_info.bUnldBcrFlag[0]	=	FALSE;
	}

	:: GetPrivateProfileString(_T("RECIPE"), _T("bUnldFlagB"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	nChk = _wtoi(strTemp);
	if (nChk>=0 && nChk<2)
	{
		st_recipe_info.bUnldBcrFlag[1]	=	nChk;
	}
	else
	{
		st_recipe_info.bUnldBcrFlag[1]	=	FALSE;
	}

	for (i=0; i<4; i++)
	{
		strHead.Format(_T("COK_BUFF_#%d"), i+1);

		::GetPrivateProfileString(_T("RECIPE"), strHead, _T("-1"), (LPWSTR)chData, 30, strLoadFile);
		strTemp.Format(_T("%s"), chData);
		nChk = _wtoi(strTemp);

		if (nChk > 0 && nChk < 9)
		{
			st_recipe_info.nCokType[i]	=	nChk;
		}
		else
		{
			st_recipe_info.nCokType[i]	=	0;
		}
	}

	for (i=0; i<4; i++)
	{
		strHead.Format(_T("COK_TYPE_#%d_NAME"), i+1);

		::GetPrivateProfileString(_T("RECIPE"), strHead, _T(""), (LPWSTR)chData, 30, strLoadFile);
		strTemp.Format(_T("%s"), chData);
		st_recipe_info.strCokTypeName[i]	=	strTemp;

		for (j=0; j<3; j++)
		{
			strHead.Format(_T("COK_TYPE_#%d_CHECK_#%d"), i+1, j+1);

			::GetPrivateProfileString(_T("RECIPE"), strHead, _T("false"), (LPWSTR)chData, 30, strLoadFile);
			strTemp.Format(_T("%s"), chData);

			if (strTemp == _T("true")) st_recipe_info.bCokChk[i][j]	=	true;
			else	st_recipe_info.bCokChk[i][j]	=	false;
		}
	}

	:: GetPrivateProfileString(_T("RECIPE"), _T("strPartNo"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.strPartNo = strTemp;

	for (i=0; i<2; i++)		//SOCKET COMMENT 2015.08.04
	{
		for (j=0; j<12; j++)
		{
			strHead.Format(_T("#%02d SOCKET_COMMENT"), (((i+1)*12) + (j+1)));

			:: GetPrivateProfileString(_T("RECIPE"), strHead, _T(""), (LPWSTR)chData, 200, strLoadFile);

			st_bd_info[0][i].strSocketComment[j].Format(_T("%s"), chData);
		}
	}
	for (i=0; i<2; i++)
	{
		for (j=0; j<12; j++)
		{
			strHead.Format(_T("#%02d SOCKET_COMMENT"), (((i+1)*12  + (j+1)) + 24));

			:: GetPrivateProfileString(_T("RECIPE"), strHead, _T(""), (LPWSTR)chData, 200, strLoadFile);

			st_bd_info[1][i].strSocketComment[j].Format(_T("%s"), chData);
		}
	}

	:: GetPrivateProfileString(_T("RECIPE"), _T("nIndividualPick"), _T(""), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.nIndividualPick = _wtoi(strTemp);

	:: GetPrivateProfileString(_T("RECIPE"), _T("d15TOffset"), _T("0"), (LPWSTR)chData, 30, strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_recipe_info.d15TOffset = _wtoi(strTemp);
}


void CMyBasicData::OnRecipe_Data_Save()
{
	CString strTemp;  // 저장할 정보 임시 저장 변수 
	CString strSaveFile;
	CString strPart, strChkExt;
	CString strPos;
	CString strHead;

	int i, j;
	COleDateTime time_cur;

	strSaveFile = st_path_info.strBasic + st_basic_info.strDeviceName;  // 티칭 데이터 저장 파일 설정

	strTemp.Format(_T("%d"), st_recipe_info.nTrayY);
	:: WritePrivateProfileString(_T("RECIPE"), _T("nTrayY"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"), st_recipe_info.nTrayX);
	:: WritePrivateProfileString(_T("RECIPE"), _T("nTrayX"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"), st_recipe_info.nRejectTray_X);
	:: WritePrivateProfileString(_T("RECIPE"), _T("nRejectTray_X"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"), st_recipe_info.nRetestTray_X);
	:: WritePrivateProfileString(_T("RECIPE"), _T("nRetestTray_X"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%.3f"), st_recipe_info.dTrayYOffset);
	:: WritePrivateProfileString(_T("RECIPE"), _T("dTrayYOffset"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%.3f"), st_recipe_info.dTrayXOffset);
	:: WritePrivateProfileString(_T("RECIPE"), _T("dTrayXOffset"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%.3f"), st_recipe_info.dBufferPitch_Y);
	:: WritePrivateProfileString(_T("RECIPE"), _T("dBufferPitch_Y"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%.3f"), st_recipe_info.dBufferPitch_X);
	:: WritePrivateProfileString(_T("RECIPE"), _T("dBufferPitch_X"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%.3f"), st_recipe_info.dRejTrayPick);
	:: WritePrivateProfileString(_T("RECIPE"), _T("dRejTrayPick"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%.3f"), st_recipe_info.dRejTrayPlace);
	:: WritePrivateProfileString(_T("RECIPE"), _T("dRejTrayPlace"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%.3f"), st_recipe_info.dTSiteOffsetX);
	:: WritePrivateProfileString(_T("RECIPE"), _T("dTSiteOffsetX"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%.3f"), st_recipe_info.dCOKBuffPitch_X);
	:: WritePrivateProfileString(_T("RECIPE"), _T("dCOKBuffPitch_X"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"), st_recipe_info.nCOKBuff_IDNum_Type);
	:: WritePrivateProfileString(_T("RECIPE"), _T("nCOKBuff_IDNum_Type"), LPCTSTR(strTemp), strSaveFile);

//	:: WritePrivateProfileString(_T("RECIPE"), _T("nCOKBuff_IDNum_Type"), LPCTSTR(st_recipe_info.strHifix), strSaveFile);

	strTemp.Format(_T("%d"), st_recipe_info.Buffer_DvcCheckSensor_Mode);
	:: WritePrivateProfileString(_T("RECIPE"), _T("Buffer_DvcCheckSensor_Mode"), LPCTSTR(strTemp), strSaveFile);
/*
	for (i=0; i<5; i++)
	{
		strHead.Format(_T("Hifix_%02d_Name"), i);
		:: WritePrivateProfileString(_T("RECIPE"), strHead, LPCTSTR(st_recipe_info.strHifix[i]), strSaveFile);
	}
*/
	strTemp.Format(_T("%d"), st_recipe_info.nBufferRotator_PlateSite_Degrees);
	:: WritePrivateProfileString(_T("RECIPE"), _T("nBufferRotator_PlateSite_Degrees"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"), st_recipe_info.nBufferRotator_TestSite_Degrees);
	:: WritePrivateProfileString(_T("RECIPE"), _T("nBufferRotator_TestSite_Degrees"), LPCTSTR(strTemp), strSaveFile);

	// jtkim 20151017
	strTemp.Format(_T("%d"), st_recipe_info.nBufferRotator_Barcode_Degrees);
	:: WritePrivateProfileString(_T("RECIPE"), _T("nBufferRotator_Barcode_Degrees"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"), st_recipe_info.nAbortTime);
	:: WritePrivateProfileString(_T("RECIPE"), _T("nAbortTime"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"), st_recipe_info.bUnldBcrFlag[0]);
	:: WritePrivateProfileString(_T("RECIPE"), _T("bUnldFlagA"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"), st_recipe_info.bUnldBcrFlag[1]);
	:: WritePrivateProfileString(_T("RECIPE"), _T("bUnldFlagB"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"), st_recipe_info.nBcrBank);
	:: WritePrivateProfileString(_T("RECIPE"), _T("nBcrBank"), LPCTSTR(strTemp), strSaveFile);

	for (i=0; i<4; i++)
	{
		strHead.Format(_T("COK_BUFF_#%d"), i+1);
		strTemp.Format(_T("%d"), st_recipe_info.nCokType[i]);
		:: WritePrivateProfileString(_T("RECIPE"), strHead, LPCTSTR(strTemp), strSaveFile);
	}

	for (i=0; i<4; i++)
	{
		strHead.Format(_T("COK_TYPE_#%d_NAME"), i+1);
		:: WritePrivateProfileString(_T("RECIPE"), strHead, LPCTSTR(st_recipe_info.strCokTypeName[i]), strSaveFile);

		for (j=0; j<3; j++)
		{
			strHead.Format(_T("COK_TYPE_#%d_CHECK_#%d"), i+1, j+1);

			strTemp = _T("false");
			if (st_recipe_info.bCokChk[i][j]) strTemp = _T("true");

			:: WritePrivateProfileString(_T("RECIPE"), strHead, LPCTSTR(strTemp), strSaveFile);
		}
	}

	:: WritePrivateProfileString(_T("RECIPE"), _T("strPartNo"), LPCTSTR(st_recipe_info.strPartNo), strSaveFile);
/*
	for (i=0; i<2; i++)		//SOCKET COMMENT 2015.08.04
	{
		for (j=0; j<12; j++)
		{
			strHead.Format(_T("#%02d SOCKET_COMMENT"), ((i*12) + (j+1)));

			strTemp = st_bd_info[0][i].strSocketComment[j];

			::WritePrivateProfileString(_T("RECIPE"), strHead, LPCTSTR(strTemp), strSaveFile);
		}
	}
	for (i=0; i<2; i++)
	{
		for (j=0; j<12; j++)
		{
			strHead.Format(_T("#%02d SOCKET_COMMENT"), (((i*12) + (j+1)) + 24));

			strTemp = st_bd_info[1][i].strSocketComment[j];

			::WritePrivateProfileString(_T("RECIPE"), strHead, LPCTSTR(strTemp), strSaveFile);
		}
	}
*/
	strTemp.Format(_T("%d"), st_recipe_info.nIndividualPick);
	:: WritePrivateProfileString(_T("RECIPE"), _T("nIndividualPick"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%.3f"), st_recipe_info.d15TOffset);
	:: WritePrivateProfileString(_T("RECIPE"), _T("d15TOffset"), LPCTSTR(strTemp), strSaveFile);
}

void CMyBasicData::OnRecipe_Data_Save_As(CString strDeviceName)
{
	CString strTemp;  // 저장할 정보 임시 저장 변수 
	CString strSaveFile;
	CString strPart, strChkExt;
	CString strPos;
	CString strHead;

	int i, j;

	COleDateTime time_cur;

	strSaveFile = st_path_info.strBasic + strDeviceName;  // 티칭 데이터 저장 파일 설정

	strTemp.Format(_T("%d"), st_recipe_info.nTrayY);
	:: WritePrivateProfileString(_T("RECIPE"), _T("nTrayY"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"), st_recipe_info.nTrayX);
	:: WritePrivateProfileString(_T("RECIPE"), _T("nTrayX"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"), st_recipe_info.nRejectTray_X);
	:: WritePrivateProfileString(_T("RECIPE"), _T("nRejectTray_X"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"), st_recipe_info.nRetestTray_X);
	:: WritePrivateProfileString(_T("RECIPE"), _T("nRetestTray_X"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%.3f"), st_recipe_info.dTrayYOffset);
	:: WritePrivateProfileString(_T("RECIPE"), _T("dTrayYOffset"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%.3f"), st_recipe_info.dTrayXOffset);
	:: WritePrivateProfileString(_T("RECIPE"), _T("dTrayXOffset"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%.3f"), st_recipe_info.dBufferPitch_Y);
	:: WritePrivateProfileString(_T("RECIPE"), _T("dBufferPitch_Y"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%.3f"), st_recipe_info.dBufferPitch_X);
	:: WritePrivateProfileString(_T("RECIPE"), _T("dBufferPitch_X"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%.3f"), st_recipe_info.dRejTrayPick);
	:: WritePrivateProfileString(_T("RECIPE"), _T("dRejTrayPick"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%.3f"), st_recipe_info.dRejTrayPlace);
	:: WritePrivateProfileString(_T("RECIPE"), _T("dRejTrayPlace"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%.3f"), st_recipe_info.dTSiteOffsetX);
	:: WritePrivateProfileString(_T("RECIPE"), _T("dTSiteOffsetX"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%.3f"), st_recipe_info.dCOKBuffPitch_X);
	:: WritePrivateProfileString(_T("RECIPE"), _T("dCOKBuffPitch_X"), LPCTSTR(strTemp), strSaveFile);


	strTemp.Format(_T("%d"), st_recipe_info.nCOKBuff_IDNum_Type);
	:: WritePrivateProfileString(_T("RECIPE"), _T("nCOKBuff_IDNum_Type"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"), st_recipe_info.Buffer_DvcCheckSensor_Mode);
	:: WritePrivateProfileString(_T("RECIPE"), _T("Buffer_DvcCheckSensor_Mode"), LPCTSTR(strTemp), strSaveFile);
/*
	for (i=0; i<5; i++)
	{
		strHead.Format(_T("Hifix_%02d_Name"), i);
		:: WritePrivateProfileString(_T("RECIPE"), strHead, LPCTSTR(st_recipe_info.strHifix[i]), strSaveFile);
	}
*/
	strTemp.Format(_T("%d"), st_recipe_info.nBufferRotator_PlateSite_Degrees);
	:: WritePrivateProfileString(_T("RECIPE"), _T("nBufferRotator_PlateSite_Degrees"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"), st_recipe_info.nBufferRotator_TestSite_Degrees);
	:: WritePrivateProfileString(_T("RECIPE"), _T("nBufferRotator_TestSite_Degrees"), LPCTSTR(strTemp), strSaveFile);

	// jtkim 20151017
	strTemp.Format(_T("%d"), st_recipe_info.nBufferRotator_Barcode_Degrees);
	:: WritePrivateProfileString(_T("RECIPE"), _T("nBufferRotator_Barcode_Degrees"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"), st_recipe_info.nAbortTime);
	:: WritePrivateProfileString(_T("RECIPE"), _T("nAbortTime"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"), st_recipe_info.bUnldBcrFlag[0]);
	:: WritePrivateProfileString(_T("RECIPE"), _T("bUnldFlagA"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"), st_recipe_info.bUnldBcrFlag[1]);
	:: WritePrivateProfileString(_T("RECIPE"), _T("bUnldFlagB"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%d"), st_recipe_info.nBcrBank);
	:: WritePrivateProfileString(_T("RECIPE"), _T("nBcrBank"), LPCTSTR(strTemp), strSaveFile);

	for (i=0; i<4; i++)
	{
		strHead.Format(_T("COK_BUFF_#%d"), i+1);
		strTemp.Format(_T("%d"), st_recipe_info.nCokType[i]);
		:: WritePrivateProfileString(_T("RECIPE"), strHead, LPCTSTR(strTemp), strSaveFile);
	}

	for (i=0; i<4; i++)
	{
		strHead.Format(_T("COK_TYPE_#%d_NAME"), i+1);
		:: WritePrivateProfileString(_T("RECIPE"), strHead, LPCTSTR(st_recipe_info.strCokTypeName[i]), strSaveFile);

		for (j=0; j<3; j++)
		{
			strHead.Format(_T("COK_TYPE_#%d_CHECK_#%d"), i+1, j+1);

			strTemp = _T("false");
			if (st_recipe_info.bCokChk[i][j]) strTemp = _T("true");

			:: WritePrivateProfileString(_T("RECIPE"), strHead, LPCTSTR(strTemp), strSaveFile);
		}
	}

	:: WritePrivateProfileString(_T("RECIPE"), _T("strPartNo"), LPCTSTR(st_recipe_info.strPartNo), strSaveFile);
/*
	for (i=0; i<2; i++)		//SOCKET COMMENT 2015.08.04
	{
		for (j=0; j<12; j++)
		{
			strHead.Format(_T("#%02d SOCKET_COMMENT"), ((i * 12) + (j+1)));

			strTemp = st_bd_info[0][i].strSocketComment[j];

			::WritePrivateProfileString(_T("RECIPE"), strHead, LPCTSTR(strTemp), strSaveFile);
		}
	}
	for (i=0; i<2; i++)
	{
		for (j=0; j<12; j++)
		{
			strHead.Format(_T("#%02d SOCKET_COMMENT"), ((i * 12) + (j+1) + 24));

			strTemp = st_bd_info[1][i].strSocketComment[j];

			::WritePrivateProfileString(_T("RECIPE"), strHead, LPCTSTR(strTemp), strSaveFile);
		}
	}
*/
	strTemp.Format(_T("%d"), st_recipe_info.nIndividualPick);
	:: WritePrivateProfileString(_T("RECIPE"), _T("nIndividualPick"), LPCTSTR(strTemp), strSaveFile);

	strTemp.Format(_T("%.3f"), st_recipe_info.d15TOffset);
	:: WritePrivateProfileString(_T("RECIPE"), _T("d15TOffset"), LPCTSTR(strTemp), strSaveFile);
}

void CMyBasicData::OnRecoveryDataLoad()
{

}


void CMyBasicData::OnRecoveryDataSave()
{
	
}


void CMyBasicData::OnAnimateDataLoad()
{
	CString str_load_device;	// 로딩 디바이스명 저장 변수
	CString str_load_file;
	CString str_temp, stemp;	// 저장할 정보 임시 저장 변수 
	CString str_pos;
	TCHAR chr_data[50], chr_buf[20];
	int nVal;
	int i;
	double dVal;

	memset(&chr_data, 0, sizeof(chr_data));
	memset(&chr_buf, 0, sizeof(chr_buf));

	// 최종 파일명 가져오기
	if(st_basic_info.strDeviceName == "")
	{
		GetPrivateProfileString(_T("FILE_NAME"), _T("Device_Type"), _T("DEFAULT.TXT"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		st_basic_info.strDeviceName = (LPCTSTR)chr_data;
	}
	// 읽을 경로 
	str_load_file = st_path_info.strBasic + st_basic_info.strDeviceName;  // 티칭 데이터 저장 파일 설정

	for (i=0; i<20; i++)
	{
		str_pos.Format(_T("MAX_POS_%02d"), i);
		GetPrivateProfileString(_T("ANIMATE_MAX"), str_pos, _T("1"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		str_temp.Format(_T("%s"), chr_data);
		dVal = _wtof(str_temp);
		if (dVal < 0.0)
		{
			st_animate_info.dMaxPos[i] = 0.0;
		}
		else
		{
			st_animate_info.dMaxPos[i] = dVal;
		}

		str_pos.Format(_T("LENGTH_POS_%02d"), i);
		GetPrivateProfileString(_T("ANIMATE_LENGTH"), str_pos, _T("1"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		str_temp.Format(_T("%s"), chr_data);
		nVal = _wtoi(str_temp);
		if (nVal < 0.0)
		{
			st_animate_info.nDistance[i] = 0;
		}
		else
		{
			st_animate_info.nDistance[i] = nVal;
		}
	}
}
//kwlee 2015.1116
void CMyBasicData::OnFillratio_Data_Load()
{
	CString str_load_device;	// 로딩 디바이스명 저장 변수
	CString str_load_pgm;		// 로딩 디바이스명 저장 변수
	CString str_load_file;
	CString str_temp, stemp;	// 저장할 정보 임시 저장 변수 

	TCHAR chr_data[50], chr_buf[20];
	//TCHAR chData[50];
	int mn_chk, j, k;
	//double md_chk;

	memset(&chr_data, 0, sizeof(chr_data));
	memset(&chr_buf, 0, sizeof(chr_buf));

	//2015.1020
	GetPrivateProfileString(_T("TIME"), _T("Daily_Ref_Time"), _T("0"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
	//mn_chk = _wtoi(chr_data);
	str_temp.Format(_T("%s"), chr_data);
	mn_chk = _wtoi(str_temp);

	if (mn_chk < 0)
	{
		st_handler_info.m_tDRef = 0;
	}
	else  st_handler_info.m_tDRef = mn_chk;

	GetPrivateProfileString(_T("TIME"), _T("RUN_TIME"), _T("1"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
	str_temp.Format(_T("%s"), chr_data);
	mn_chk = _wtoi(str_temp);
	if (mn_chk < 0)
	{
		st_handler_info.tRun = 0;
	}
	else st_handler_info.tRun = mn_chk;

	CString str;

	for (j =0; j<TSITE_SOCKET_CNT; j++)
	{
		str.Format(_T("Daily_RunDown_Time[%d]"), j);
		GetPrivateProfileString(_T("TIME"), str, _T("0"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		//mn_chk = _wtoi(chr_data);
		str_temp.Format(_T("%s"), chr_data);
		mn_chk = _wtoi(str_temp);

		if (mn_chk < 0)
		{
			st_handler_info.m_tDRdown[j] = 0;
		}
		else  st_handler_info.m_tDRdown[j] = mn_chk;

		str.Format(_T("Daily_Stop[%d]"), j);
		GetPrivateProfileString(_T("TIME"), str, _T("0"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		//mn_chk = _wtoi(chr_data);
		str_temp.Format(_T("%s"), chr_data);
		mn_chk = _wtoi(str_temp);

		if (mn_chk < 0)
		{
			st_handler_info.m_tDStop[j] = 0;
		}
		else  st_handler_info.m_tDStop[j] = mn_chk;

		str.Format(_T("Daily_UserStop[%d]"), j);
		GetPrivateProfileString(_T("TIME"), str, _T("0"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		//mn_chk = _wtoi(chr_data);

		str_temp.Format(_T("%s"), chr_data);
		mn_chk = _wtoi(str_temp);

		if (mn_chk < 0)
		{
			st_handler_info.m_tDUserStop[j] = 0;
		}
		else  st_handler_info.m_tDUserStop[j] = mn_chk;

		str.Format(_T("InStandBy_Time[%d]"), j);
		//mstr_temp.Format(_T("%d"),st_handler_info.m_tDInStandby[j]);
		GetPrivateProfileString(_T("TIME"), str, _T("0"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		//mn_chk = _wtoi(chr_data);
		str_temp.Format(_T("%s"), chr_data);
		mn_chk = _wtoi(str_temp);
		//kwlee 2015.1127
// 		if (mn_chk < 0)
// 		{
// 			st_handler_info.m_tDInStandby[j] = 0;
// 		}
// 		else  st_handler_info.m_tDInStandby[j] = mn_chk;

		if (mn_chk < 0)
		{
			st_DB_time.n_Db_time[INSTANBY][j] = 0;
		}
		else  st_DB_time.n_Db_time[INSTANBY][j] = mn_chk;

	}

	for ( k = 0; k < TSITE_SOCKET_CNT; k++ )
	{
		str.Format(_T("Daily_Test_Time[%d]"), k);
		GetPrivateProfileString(_T("TIME"), str, _T("0"), chr_data,sizeof(chr_data), st_path_info.strFileBasic);
		//mn_chk = _wtoi(chr_data);
		str_temp.Format(_T("%s"), chr_data);
		mn_chk = _wtoi(str_temp);
		if (mn_chk < 0)
		{
			st_handler_info.m_tDtest[k] = 0;
		}
		else  st_handler_info.m_tDtest[k] = mn_chk;

		str.Format(_T("Daily_SokOff_Time[%d]"), k);
		GetPrivateProfileString(_T("TIME"), str, _T("0"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		//mn_chk = _wtoi(chr_data);
		str_temp.Format(_T("%s"), chr_data);
		mn_chk = _wtoi(str_temp);
		if (mn_chk < 0)
		{
			st_handler_info.m_tDSokOff[k] = 0;
		}
		else  st_handler_info.m_tDSokOff[k] = mn_chk;

		str.Format(_T("OutStandBy_Time[%d]"), k);
		GetPrivateProfileString(_T("TIME"), str, _T("0"), chr_data,sizeof(chr_data), st_path_info.strFileBasic);
		//mn_chk = _wtoi(chr_data);
		str_temp.Format(_T("%s"), chr_data);
		mn_chk = _wtoi(str_temp);
		//kwlee 2015.1127
// 		if (mn_chk < 0)
// 		{
// 			st_handler_info.m_tDOutStandby[k] = 0;
// 		}
// 		else  st_handler_info.m_tDOutStandby[k] = mn_chk;

		if (mn_chk < 0)
		{
			st_DB_time.n_Db_time[OUTREADY][k] = 0;
		}
		else st_DB_time.n_Db_time[OUTREADY][k] = mn_chk;

		//////
		str.Format(_T("Fail_Device[%d]"), k);
		GetPrivateProfileString(_T("TIME"), str, _T("0"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		//mn_chk = _wtoi(chr_data);
		str_temp.Format(_T("%s"), chr_data);
		mn_chk = _wtoi(str_temp);

		str.Format(_T("Pass_Device[%d]"), k);
		GetPrivateProfileString(_T("TIME"), str, _T("0"), chr_data, sizeof(chr_data), st_path_info.strFileBasic);
		//mn_chk = _wtoi(chr_data);
		str_temp.Format(_T("%s"), chr_data);
		mn_chk = _wtoi(str_temp);
	}
	///////////////////////////	
}

void CMyBasicData::OnAnimateDataSave()
{

}



