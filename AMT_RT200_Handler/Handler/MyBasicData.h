// MyBasicData.h: interface for the CMyBasicData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYBASICDATA_H__F10E96B3_4961_4A37_B43F_FDAD481A0494__INCLUDED_)
#define AFX_MYBASICDATA_H__F10E96B3_4961_4A37_B43F_FDAD481A0494__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Variable.h"  // 전역 변수 정의 클래스 추가 

class CMyBasicData  
{
public:
	CMyBasicData();
	virtual ~CMyBasicData();

// Attributes
public:

// Operations
public:
	void				OnAnimateDataLoad();
	void				OnAnimateDataSave();
	void				OnRecoveryDataLoad();
	void				OnRecoveryDataSave();
	void				OnRecipe_Data_Save_As(CString strDeviceName);
	void				OnRecipe_Data_Save();
	void				OnRecipe_Data_Load();
	void				OnRecipe_Data_Load(CString strDeviceName);
	CString				GetWaitTimeName(int nMode);
	void				OnModuleRobot_Teach_Data_Save_As(CString strDeviceName);
	void				OnMaintenance_Data_Save_As(CString strDeviceName);
	void				OnInterface_Data_Save_As(CString strDeviceName);
	void				OnBasic_Data_Save_As(CString strDeviceName);
	void				OnInterface_Data_Save();
	void				OnInterface_Data_Load();
	void				OnDevice_Folder_Save();
	void				OnDeviec_Folder_Load();
	CString				OnGet_File_Name();
	void				OnBasic_Data_Save();
	void				OnBasic_Data_Load(int nMode);
	void				OnBasic_Data_Load(CString strDeviceName);
	void				OnModuleRobot_Teach_Data_Save();
	void				OnModuleRobot_Teach_Data_Load();
	void				OnModuleRobot_Teach_Data_Load(CString strDeviceName);
	void				OnMaintenance_Data_Save();
	void				OnMaintenance_Data_Load();
	void				OnWaitTime_Data_Save();
	void				OnWaitTime_Data_Load();
	void				OnMotorSpeed_Set_Data_Save();
	void				OnMotorSpeed_Set_Data_Load();
	void				OnStep_File_Name_Save(CString strFile);
	CString				OnStep_File_Name_Load();
	void				OnStep_File_Index_Save(CString strIndex);
	CString				OnStep_File_Index_Load();
	void				OnPassWord_Save(CString strSaveLevel, CString strPassData);
	void				OnPassWord_Load(CString strLoadLevel);
	void                OnFillratio_Data_Load(); //kwlee 2015.1116
	
};

extern CMyBasicData clsBasic;
#endif // !defined(AFX_MYBASICDATA_H__F10E96B3_4961_4A37_B43F_FDAD481A0494__INCLUDED_)
