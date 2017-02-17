// MyJamData.cpp: implementation of the CMyJamData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "MyJamData.h"
// #include "MonitoringLog.h"
// #include "Alg_Database.h"
// 
// #include "Public_Function.h"  // ���� ���� �� I/O ���� ���� Ŭ����
#include "io.h" 
#include "Variable.h"
#include "CtlBd_Function.h"
#include "ComizoaPublic.h"
#include "PublicFunction.h"
#include "FastechPublic_IO.h"
#include "AlgMemory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMyJamData			clsMyJamData;
CMyJamData::CMyJamData()
{

}

CMyJamData::~CMyJamData()
{

}

// ******************************************************************************
// �߻� �˶� ���� ���� �Լ�                                                      
// -> strPath     : �˶� ���� ���� ���� ���� ����                                
// -> strDataFile : �˶� ���� ����� File Name(File Name+Ȯ����)                       
// -> strCode     : �߻��� �˶� Code                                             
// ******************************************************************************
CString CMyJamData::OnAlarmDisplayInfoLoad(CString strPath, CString strDataFile, CString strActionFile, CString strCode)
{

	CString sFilePath;  // ���� ���� ����
	CString sAlarmChk;  // �Լ� ���� ���� ���� ����
	int n_pos;
	
	sFilePath = strPath;
	if (sFilePath.Right(1) != _T("\\"))  // ���� ��� ������ ���� �˻�
	{
		sFilePath += _T("\\");
	}

	sAlarmChk = OnAlarmBmpDataLoad(sFilePath, strDataFile, strCode);  // �߻� �˶� ���� ǥ�ÿ� �׸� File Name ���� �Լ�
	n_pos = sAlarmChk.Find(_T("Trouble"));
	if (n_pos >= 0)  // ���� �߻�
	{
		return sAlarmChk;
	}

	sAlarmChk = OnAlarmStyleDataLoad(sFilePath, strDataFile, strCode);  // �˶� �߻� ���� ǥ�� ���� �� ��� ��ġ ���� �Լ�
	n_pos = sAlarmChk.Find(_T("Trouble"));
	if (n_pos >= 0)  // ���� �߻�
	{
		return sAlarmChk;
	}

	sAlarmChk = OnAlarmRepairDataLoad(sFilePath, strActionFile, strCode);  // �߻��� �˶��� ���� ��ġ ���� ���� �Լ�
	n_pos = sAlarmChk.Find(_T("Trouble"));
	if (n_pos >= 0)  // ���� �߻�
	{
		return sAlarmChk;
	}

	return sAlarmChk;
}
// ******************************************************************************

// ******************************************************************************
// �߻� �˶� ���� ǥ�ÿ� �׸� File Name ���� �Լ�                                
// -> strPath     : �˶� ǥ�� �׸� ���� ���� ����                                
// -> strDataFile : �˶� ǥ�� �׸� File Name(File Name+Ȯ����)                   
// -> strCode     : �߻��� �˶� Code                                             
// ******************************************************************************
CString CMyJamData::OnAlarmBmpDataLoad(CString strPath, CString strDataFile, CString strCode)
{
	CString sReturnData;  // �Լ� ���� �÷���
	CString sFile;  // �˶� ǥ�ÿ� �׸��� ���� [����]+[File Name] ���� ����
	CString sTemp;  // �˶� ǥ�ÿ� �׸� File Name �ӽ� ���� ����

	char chBuf[1000];

	sReturnData = _T("OK");  // �Լ� ���� �÷��� �ʱ�ȭ

	sFile = strPath + strDataFile;  // [����]+[File Name] ����


	if (!GetPrivateProfileString(strCode, _T("DEF"), _T(""), (LPWSTR)chBuf, 1000, sFile))
	{
		sReturnData = _T("File Name : ") + sFile;
		sReturnData += _T("\r\n");
		sReturnData += _T("[DEF] The information we read. Trouble -> Code : ");
		sReturnData += strCode;

		return sReturnData;
	}

	sTemp.Format(_T("%s"), chBuf);
	st_alarm_info.strBmpFile = sTemp;  // �˶� ǥ�ÿ� �׷� File Name ����

	return sReturnData;
}
// ******************************************************************************

// ******************************************************************************
// �˶� �߻� ���� ǥ�� ���� �� ��� ��ġ ���� �Լ�                               
// -> strPath     : ���� ǥ�� ���� ���� ���� ����                                
// -> strDataFile : ���� ǥ�� ���� ���� File Name(File Name+Ȯ����)              
// -> strCode     : �߻��� �˶� Code                                             
// ******************************************************************************
CString CMyJamData::OnAlarmStyleDataLoad(CString strPath, CString strDataFile, CString strCode)
{
	CString sChkData;		// �Լ� ���� �÷���
	CString sChkFile;		// �˶� ���� ǥ�� ������ ���� [����]+[File Name] ���� ����
	CString sChk, sChk2;  // �˶� ���� ǥ�� ���� �� ��ġ ���� ����
	int nStart=0, nEnd, i;		// ���� ������ ��ġ ���� ����
	int nMax=4;					// ���� ������ �ִ� ���� (ȭ��ǥ ������� �ʱ�ȭ)

	char chr_buf[1000];
	char num[20];

	sChkData = _T("OK");  // �Լ� ���� �÷��� �ʱ�ȭ

	sChkFile = strPath + strDataFile;  // [����]+[File Name] ����


	if (!GetPrivateProfileString(strCode, _T("STYLE"), _T(""), (LPWSTR)chr_buf, 1000, sChkFile))
	{
		sChkData = _T("File Name : ") + sChkFile;
		sChkData += _T("\r\n");
		sChkData += _T("[STYLE] The information we read. Trouble -> Code : ");
		sChkData += strCode;

		return sChkData;
	}

	sChk.Format(_T("%s"), chr_buf);

	for(i=0; i<nMax; i++)
	{
		nEnd = sChk.Find(';', nStart);
		if (nEnd == -1)  // ������ ������
		{
			break;
		}
		else            // ������ ����
		{
			sChk2 = sChk.Mid(nStart, nEnd-nStart);  // ������ ������ �� �� ������ ����
			sChk2.TrimLeft(' ');
			sChk2.TrimRight(' ');

			if (i == 0)  // �˶� ���� ǥ�� ����
			{
				st_alarm_info.strStyle = sChk2;       // �˶� ���� ǥ�� ���� ����
				if (st_alarm_info.strStyle == "CIRCLE")  // ���� ǥ�� ���� 8�� ����
				{
					nMax = 8;    
				}
				else if (st_alarm_info.strStyle == "NONE")
				{
					nMax = 1;
				}
			}
			else
			{
				//lstrcpy(num, str_chk2);
				//wsprintfA(num, "%S", sChk2);
				clsFunc.OnStringToChar(sChk2, num);
				if (i == 1)
				{
					st_alarm_info.nCx = atoi(num);         // X�� �ȼ� ��ġ
				}
				else if (i == 2)
				{
					st_alarm_info.nCy = atoi(num);         // Y�� �ȼ� ��ġ
				}
				else if (i == 3)
				{
					st_alarm_info.nOffset = atoi(num);     // �� ũ��
				}
				else if (i == 4)
				{
					st_alarm_info.nLineWidth = atoi(num);  // ���� �β�
				}
				else if (i == 5)
				{
					st_alarm_info.nR = atoi(num);          // RGB �� R ��
				}
				else if (i == 6)
				{
					st_alarm_info.nG = atoi(num);          // RGB �� G ��
				}
				else if (i == 7) 
				{
					st_alarm_info.nB = atoi(num);          // RGB �� B ��
				}
				// **************************************************************
			}

			nStart = nEnd + 1;  // ���� ������ ��ġ ����
		}
	}

	if (i != nMax)  // Data Format ����
	{
		sChkData = _T("File Name : ") + sChkFile;
		sChkData += _T("\r\n");
		sChkData += _T("[STYLE] Data Format. Trouble -> Code : ");
		sChkData += strCode;

		return sChkData;
	}

	return sChkData;
}
// ******************************************************************************

// ******************************************************************************
// �߻��� �˶��� ���� ��ġ ���� ���� �Լ�                                        
// -> strPath       : �˶� ��ġ ���� ����� ���� ����                            
// -> strActionFile : �˶� ��ġ ���� ����� File Name(File Name+Ȯ����)          
// -> strCode       : �߻��� �˶� Code                                           
// ******************************************************************************
CString CMyJamData::OnAlarmRepairDataLoad(CString strPath, CString strActionFile, CString strCode)
{
	CString sReturnVal;		// �Լ� ���� �÷���
	CString sRepairFile;	// ��ġ ���׿� ���� [����]+[File Name] ���� ����
	CString sActItem;		// ��ġ ���� ���� ��ġ ���� ����
	CString sActTemp;		// ��ġ ���� �ӽ� ���� ����
	int nChkCnt;				// ��ġ ���� ���� ���� ����
	int i; 
	
	char chr_buf[1000];

	sReturnVal = _T("OK");  // �Լ� ���� �÷��� �ʱ�ȭ

	sRepairFile = strPath + strActionFile;  // [����]+[File Name] ����
	
	if (!GetPrivateProfileString(strCode, _T("COUNT"), _T("0"), (LPWSTR)chr_buf, 10, sRepairFile))
	{
		sReturnVal = _T("File Name : ") + sRepairFile;
		sReturnVal += _T("\r\n");
		sReturnVal += _T("[COUNT] The information we read. Trouble -> Code : ");
		sReturnVal += strCode;

		return sReturnVal;
	}

	nChkCnt = atoi(chr_buf);  // ���� ��ġ ���� ���� ����
	if (nChkCnt<1)
	{
		sReturnVal = _T("File Name : ") + sRepairFile;
		sReturnVal += _T("\r\n");
		sReturnVal += _T("[COUNT] Data Format Trouble -> Code : ");
		sReturnVal += strCode;

		return sReturnVal;
	}
	else
	{
		st_alarm_info.nActionCnt = nChkCnt;
	}

	(st_alarm_info.strRepairMsg).Empty();  //  ��ġ ���� ���� ���� �ʱ�ȭ

	for (i=0; i<(st_alarm_info.nActionCnt); i++)   
	{
		sActItem.Format(_T("%d"), i+1);  // ��ġ ���� ���� ���� ��ġ ����
		if (!GetPrivateProfileString(strCode, (LPCTSTR)sActItem, _T(""), (LPWSTR)chr_buf, 1000, sRepairFile))
		{
			sReturnVal = _T("File Name : ") + sRepairFile;
			sReturnVal += _T("\r\n");
			sReturnVal += _T("An action subject. The information we read. Trouble");

			sReturnVal += _T("\r\n");
			sReturnVal += _T("Code : ");
			sReturnVal += strCode;

			sReturnVal += _T("\r\n");
			sReturnVal += _T("Position : ");
			sReturnVal += sActItem;

			return sReturnVal;
		}
		// **********************************************************************

		st_alarm_info.strRepairMsg += _T("\r\n");			//��ġ ���׿� �� ù���� ����.

		sActTemp.Format(_T("%s"), chr_buf);				// ��ġ ���� ����
		st_alarm_info.strRepairMsg += sActTemp;	

		if (i != ((st_alarm_info.nActionCnt)-1))  
			st_alarm_info.strRepairMsg += "\r\n";			// ��ġ ���� �ܶ� ������ ����
	}
	// **************************************************************************
	return sReturnVal;
}
// ******************************************************************************

// ******************************************************************************
// �߻��� �˶� ȭ�� ��� �Լ�                                                    
// ******************************************************************************
void CMyJamData::OnAlarmDisplay()
{
	CString sCnt, sTemp;
	char chTempCode[10], chTempCnt[10], chBuf[20]={0,};
 	
 	CString sDisplayTime;	// ���� ��¿� �ð� ���� ����
 	CString sAlarmCode;		// �߻��� �˶� �ڵ� ���� ����
 	CString sAlarmMsg;		// �߻��� �˶� �޽��� ���� ����
 	int nCurHour;				// ���� �ð� ���� ����
 	int nChk;					// ������ �˶� �ڵ� ���� ����
// 	long nChkTime;			// �˶� �߻� �ð� ���� ���� ����
 	int nDataSave = 0;		// Jam�� ������ ���̳�?
 
 	COleDateTime cCurTime;
 	CTime tAlarmOccuredTime;

 	// **********************************************************************
 	// ��� ���� �� �˶� �߻��ߴ��� �˻��Ѵ�                                     
 	// -> �˶� �߻��� ��� �α� ���� �� �ӽ� ���Ͽ� �����ϰ� ȭ�鿡 ����Ѵ�     
 	// **********************************************************************

	//st_alarm_info.strCode = _T("010006");
 	// �˶� �߻�110308
 	if (st_handler_info.nRunStatus == dJAM || st_handler_info.nRunStatus == dWARNING || st_handler_info.nRunStatus == dLOTEND)  
 	{
 		if (st_handler_info.nAlarmScreen == TRUE)	return;
 
 		// ����Ʈ �� ȭ�� ����
 		if (st_handler_info.cWndList != NULL)  
 		{
			sAlarmMsg.Format(_T("Alarm Code = %s"), st_alarm_info.strCode);
//			wsprintfA(st_other_info.cAbnormalMsg, "%S", st_other_info.strAbnormalMsg);
			//clsFunc.OnStringToChar(st_other_info.strAbnormalMsg, st_other_info.cAbnormalMsg);
			clsMem.OnAbNormalMessagWrite(sAlarmMsg);
 			// ���� ���� ��� ��û
 			st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  
 		}

 		// *****************************************************
 		// �˶� ȭ�� ��� �Ұ����� ��� �˻��Ѵ�.                                
 		// -> st_handler.n_system_lock : SYSTEM LOCK ���� �÷���                
 		// *****************************************************
 		if (st_handler_info.nSystemLock != FALSE)  // SYSTEM LOCK
 		{
 			return;  // ���� ����
 		}
 
 		// ***************************
 		// �˶� �߻��� �ð� �����Ѵ�                                                 
 		// ***************************
 		cCurTime = COleDateTime::GetCurrentTime();
 		nCurHour = cCurTime.GetHour();
 
 		tAlarmOccuredTime = CTime::GetCurrentTime();
 		sDisplayTime = tAlarmOccuredTime.Format("%c");
 
 		// **********************************************************************
 		// �˶� ȭ�� ��� ���� �� �˶� ī��Ʈ ���� �˻��Ѵ�                      
 		// -> �̹� �˶� ȭ�� ��� ���� ��� �� ����� �ʿ� ������                
 		//    �� ��쿡 �߻��ϴ� �˶��� �����Ѵ�                                 
 		//  : �˶� ȭ�� ��� �߿� �˶� Code ����Ǹ� ��� �޽��� �޶����� ����   
 		// -> st_handler.n_alarm_screen : �˶� ȭ�� ��� ���� �÷���             
 		// -> st_alarm.n_cur_state : �˶� ���� ���� �÷���                       
 		// **********************************************************************
 		// �˶� ȭ�� �����
 		if (st_handler_info.nAlarmScreen != TRUE)  
 		{
 			if (st_handler_info.nRunStatus == dLOTEND)	
			{
 				st_alarm_info.nResetStatus = NO;
 			}
 			else 
			{
				st_alarm_info.nResetStatus = YES;
			}
 
 			//�߻��� �˶��� List�� �ִ´�.
 			if (st_alarm_info.strCode != "")
 			{
 				// ������ �˶��� �����Ѵ�.
 				// st_handler_info.str_last_alarm = alarm.str_code;	
 				
				// On_Alarm_Info_Load�Լ��� MainFrame �����ڿ��� ȣ���Ͽ�
				// Error.txt�� ���� ���� ������ �۷ι� ������ ������ 
				// Error.txt�� ���Ͽ� ��ġ�ϴ� �ڵ带 ã�� �Լ��̸� 
				// ��ġ�ϴ� CODE�� �ֺ� ������ ������ ������ ����ϴ� �Լ��̴�. 
 				OnAlarmInfoSettoVariable(st_alarm_info.strCode);  
 				// *********************************************

				// �˶� �߻� ������ ���Ͽ� �����Ѵ�
 				CtlBdFunc.Alarm_Error_Occurrence(st_alarm_info.nCountMode, st_alarm_info.nTypeMode, st_handler_info.nRunStatus, st_alarm_info.strCode);

				//wsprintfA(chTempCode, "%S", st_alarm_info.strCode);
				clsFunc.OnStringToChar(st_alarm_info.strCode, chTempCode);
 				sCnt.Format(_T("%d"), st_alarm_info.nCurState);

				//wsprintfA(chTempCnt, "%S", sCnt);
				clsFunc.OnStringToChar(sCnt, chTempCnt);
 
 				// *******************************************
 				// �ٷ� ���� �߻��� �˶� �ڵ�� ���Ͽ�  
 				// ������ ��� �˶� ī��Ʈ ������      
 				// -> �˶� ī��Ʈ �ð� ������ 30�ʷ� �����Ѵ�                                
 				// *******************************************
 				st_alarm_info.stl_cur_alarm_time = GetTickCount() / 1000;
 
 				nChk = _wtoi(st_alarm_info.strCode);
 				
				// ���� �߻��� �˶��� ������ �߻��� �˶� �ð��� �� �� ���� �ð� �ڿ� �߻��Ͽ��ٸ� ���� �׷��� ������ �������� �ʴ´�. 
				// ���� �ð��� ���� ����� �ϴ� �ּ�ó���Ѵ�. 
				/*
 				n_chk_time = st_alarm_info.stl_cur_alarm_time - st_alarm_info.stl_prev_alarm_time; 
 				
 				if (n_chk_time < st_alarm_info.nAlarmSaveDelayTM) 
 				{
 					st_alarm_info.stl_prev_alarm_time = st_alarm_info.stl_cur_alarm_time;
 				}
 				else
 				{
 					st_alarm_info.stl_prev_alarm_time = st_alarm_info.stl_cur_alarm_time;
 					// Jam�� ���� �� Count�� �Ѵ�.
 					n_data_save = YES;			
 				}
				*/
 			
 				// ���� �߻��� ALARM CODE�� ������ �߻��� ALARM CODE�� 
				// ���Ͽ� ���� ALARM CODE�� �������� �ʴ´�. 
				if (st_alarm_info.stl_prev_code == nChk)  
 				{
 					st_alarm_info.stl_prev_alarm_time = st_alarm_info.stl_cur_alarm_time;
 				}
 				else
 				{
 					// **********************************************************************
 					// �߻��� �˶� ������ ���� ������ ��ü�Ѵ�                               
 					// -> ������ �߻��ϴ� �˶��� ���ϴ� ������ ���ȴ�                    
 					// **********************************************************************
 					st_alarm_info.stl_prev_code = nChk;
 					st_alarm_info.stl_prev_alarm_time = st_alarm_info.stl_cur_alarm_time;
 					// **********************************************************************
 					
					// ALARM ������ �����ϴ� �÷����̴�. 
 					nDataSave = YES;			
 				}
 				// **************************************************************************
 			}
 
			// ALARM CODE�� �����ϱ� �÷��� Ȯ���Ѵ�. 
 			if (nDataSave == YES)
 			{
 				// ERROR.txt ������ ����: <000001 1 0 "X_Axis Motor Home Check Error.">���� 2��° �� 1�� ���� ��/���� Ȯ���ϴ� ���̸�
				// �� ���� Ȯ���Ѵ�. 
 				if (st_alarm_info.nCurState == 1)				
 				{
 					st_alarm_info.nAlarmOccured++;
 					st_alarm_info.nAlarmOccuredDay++;
 // jtkim 20130704
 //					sTemp = LPCTSTR(_itoa(st_alarm_info.nAlarmOccuredDay, chBuf, 10)) ;
					sTemp = LPCTSTR(_itoa_s(st_alarm_info.nAlarmOccuredDay, chBuf, sizeof(chBuf), 10)) ;
 					:: WritePrivateProfileString(_T("BasicData"), _T("Day_Alarm_Count"), LPCTSTR(sTemp), st_path_info.strBasic);
 				}
 
 				OnOccuredAlarmSavingtoFile();
 
 			//	M_Log.SetMonitoringLog(LOG_TYPE_ALARM);
 			}
 
			clsFunc.OnSound(IO_ON);
 			// �˶� ȭ�� ��� �÷��� ����	
 			st_handler_info.nAlarmScreen = TRUE;	

 			// �˶� ����� �˶� ���� ���� ON
 			//Func.uSetAlarmPort();  
 
 			// ���� �����ӿ� �˶� ȭ�� ��� ��û 

			if (st_handler_info.nMenuLock != FALSE)	
			{
				st_handler_info.nMenuLock = FALSE;
			}

			// jtkim 20150721
			if (st_work_info.nCycleYesNo == YES)
			{
				st_work_info.nCycleYesNo = NO;
			}

			// jtkim 20150721
			if (st_work_info.nTsiteCycleYesNo == YES)
			{
				st_work_info.nTsiteCycleYesNo = NO;
			}

			// jtkim 20150824
			st_work_info.dwFrontTime[0] = st_work_info.dwRearTime[0] = GetTickCount();

			if (st_handler_info.nDlgLock == FALSE)
			{
 				::PostMessage(st_handler_info.hWnd, WM_FORM_CHANGE, SCREEN_ALARM, 0);  
			}

 			if (st_handler_info.cWndTitle != NULL)	
			{
 				st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, MACHINE_STATUS, st_handler_info.nRunStatus);
 			}

 			// ��� ���� �Լ����� RUN ������ ��쿡�� ��ġ�� �����ϰ� �Ǿ� �ִ�
 			// �嵥, ���� ���� ���¸� �������ִ� ���� ���� �̰��� �߰��Ѵ�
 			COMI.mn_run_status = st_handler_info.nRunStatus;

			clsFunc.OnMCStop();
 		}
 	}
}

// **************************************
// �߻��� �˶� �α� ���Ͽ� �����ϴ� �Լ�                                         
// **************************************
void CMyJamData::OnOccuredAlarmSavingtoFile()
{
//	char chTitleTime[20];
//	char chAlarmCode[10];
	char chAlarmMsg[1000];
//	char chMsg[1000];

	CString strDisplayTime;	// ���� ��¿� �ð� ���� ����
	CString strAlarmCode;		// �߻��� �˶� Code ���� ����
	CString strAlarmMsg;		// �߻��� �˶� �޽��� ���� ����
	CString strTemp;
	CString strAlarmData;		// �߻��� �˶� �޽��� ���� ����
	int nCurHour;				// ���� �ð� ���� ����

	COleDateTime cCurTime;
	CTime tAlarmOccuredTime;

	CString strFileMonth, strFileDay, strTitleTime;
	int nCurYear, nCurMonth, nCurDay;		// ��, ��, �� ���� ����

	CString str_month_path, str_day_path;

	FILE *fp;
	CString sLoadFile = _T("");

	// **************************************************************************
	// �˶� �߻��� �ð� �����Ѵ�                                                 
	// -> SHIFT ���� �˻� �� �˶� �߻� �ð� �����ϱ� ���Ͽ� �ʿ�                 
	// **************************************************************************
	cCurTime = COleDateTime::GetCurrentTime();
	nCurHour = cCurTime.GetHour();

	tAlarmOccuredTime	= CTime::GetCurrentTime();
	strDisplayTime		= tAlarmOccuredTime.Format("%c");
	// **************************************************************************

	// **************************************************************************
	// �˶� Code �� �˶� �޽��� �Լ� ������ �����Ѵ�                             
	// -> ����� �� �Լ� ȣ�� ���� ���ο� �˶��� �߻��ϴ� ��� �˶� Code��       
	//    ��ġ ���� �� ��Ÿ ������ ��ġ���� ���� �� �ִ�                         
	// -> alarm.str_code : ���� �߻��� �˶� Code ���� ���� ����                  
	// -> st_alarm.str_cur_msg : �߻��� �˶� �޽��� ���� ����                    
	// **************************************************************************
	strAlarmCode	= st_alarm_info.strCode;
	strAlarmMsg		= st_alarm_info.strCurrMsg;
	// **************************************************************************

	// **************************************************************************
	// �Ϻ�, ���� �˶� �߻� Ƚ�� �ӽ� ���Ͽ� �߰��Ѵ�                            
	// **************************************************************************
	// �Ϻ�, ���� �˶� ���� ����Ʈ ���Ͽ� �߰� �Լ�
	//	mcls_alarm_list.On_Daily_ListFile_Add(str_alarm_code, str_display_time, str_alarm_msg); 
	// **************************************************************************

	// **************************************************************************
	// �ٷ� ���� �߻��� �˶� Code�� ���Ͽ� ������ ��� �˶� ī��Ʈ ������      
	// -> �˶� ī��Ʈ �ð� ������ 30�ʷ� �����Ѵ�                                
	// **************************************************************************

	// **************************************************************************
	// ���� �̸����� ����� ��¥ �����Ѵ�                                        
	// **************************************************************************
 	if (st_alarm_info.nCurState == 1)				
 	{
 		cCurTime			= COleDateTime::GetCurrentTime();
 		tAlarmOccuredTime	= CTime::GetCurrentTime() ;
 
 		nCurYear	= cCurTime.GetYear();  
 		nCurMonth	= cCurTime.GetMonth();  
 		nCurDay		= cCurTime.GetDay();  
 
 		strFileMonth.Format(_T("%04d%02d"), nCurYear, nCurMonth);
 		strFileDay.Format(_T("%04d%02d%02d"), nCurYear, nCurMonth, nCurDay);
 		strTitleTime = tAlarmOccuredTime.Format(_T("%Y/%m/%d %H:%M:%S"));
 		
 		str_month_path = _T("d:\\AMT8562\\Alarm\\Monthly\\MT") + strFileMonth + _T(".TXT");
 		str_day_path = _T("d:\\AMT8562\\Alarm\\Daily\\DY") + strFileDay + _T(".TXT");
 
		char chMode[10], chMonthPath[50], chDayPath[50];
		//wsprintfA(chMode, "%S", _T("a+"));
		clsFunc.OnStringToChar(_T("a+"), chMode);
		//wsprintfA(chMonthPath, "%S", str_month_path);
		clsFunc.OnStringToChar(str_month_path, chMonthPath);

 		// �� ����Ÿ ���
// jtkim 20130704
// 		if ((fp = fopen(chMonthPath, chMode)) == NULL) 
		fopen_s(&fp, chMonthPath, "a+");
		if(!fp)
 		{
 //			AfxMessageBox(_T("The failure because we open the file."));
 			return;
 		}

		strAlarmData		= _T("");
		strTemp.Format(_T("| %4d |"), st_alarm_info.nAlarmNum);
		strAlarmData		= strTemp; 
		strTemp.Format(_T(" %6s |"), strAlarmCode);
		strAlarmData		+= strTemp; 
		strTemp.Format(_T(" %-44s |"), strAlarmMsg);
		strAlarmData		+= strTemp;
		strTemp.Format(_T("          |     |"));
		strAlarmData		+= strTemp;
		strTemp.Format(_T(" %17s |"), strTitleTime);
		strAlarmData		+= strTemp;
		clsFunc.OnStringToChar(strAlarmData, chAlarmMsg);

		fprintf(fp,"%s\n",chAlarmMsg) ;
 		fclose(fp);

		//wsprintfA(chDayPath, "%S", str_day_path);
		clsFunc.OnStringToChar(str_day_path, chDayPath);
 		// �� ����Ÿ ���
// jtkim 20130704
// 		if ((fp = fopen(chDayPath, chMode)) == NULL)  
		fopen_s(&fp, chDayPath, "a+");
		if(!fp)
 		{
 			//AfxMessageBox(_T("The failure because we open the file."));
 			return;
 		}

		fprintf(fp,"%s\n",chAlarmMsg) ;

		st_alarm_info.nAlarmNum++;

 		fclose(fp);
 	}
 	// **************************************************************************
 	// SHIFT�� �˶� �߻� Ƚ�� �ӽ� ���� ���Ͽ� �˶� �߰��Ѵ�                     
 	// -> �˶� �߻� Ƚ�� �ӽ� ���� ���� ������ �� ������ �˶� ����Ʈ ���� �̸��� 
 	//    �Ϻ� ���� �̸����� ����� ������ ������ �����ϴ�                       
 	// **************************************************************************
 	if (nCurHour>=6 && nCurHour<14)  // SHIFT �� DAY
 	{
 		// SHIFT�� �˶� �߻� Ƚ�� ���� �ӽ� ���Ͽ� �߰� �Լ�
 		// mcls_alarm_list.On_Shift_File_Add(1, str_alarm_code, str_display_time);  
 	}
 	else if (nCurHour >= 14 && nCurHour < 22)  // SHIFT �� SWING
 	{
 		// SHIFT�� �˶� �߻� Ƚ�� ���� �ӽ� ���Ͽ� �߰� �Լ�
 		// mcls_alarm_list.On_Shift_File_Add(2, str_alarm_code, str_display_time);  
 	}
 	else 
 	{
 		// SHIFT�� �˶� �߻� Ƚ�� ���� �ӽ� ���Ͽ� �߰� �Լ�
 		// mcls_alarm_list.On_Shift_File_Add(3, str_alarm_code, str_display_time);  
 	}
 	// **************************************************************************
 
 	// **************************************************************************
 	// LOT�� �˶� �߻� Ƚ�� �ӽ� ���Ͽ� �߰��Ѵ�                                 
 	// **************************************************************************
 	// LOT�� �˶� �߻� Ƚ�� ���� �ӽ� ���Ͽ� �߰� �Լ�
 	// mcls_alarm_list.On_Lot_ListFile_Add(str_alarm_code, str_display_time);  
 	// **************************************************************************
}
// ******************************************************************************

// ******************************************************************************
// ���Ͽ� ����� ��� �˶� ���� ���� ������ �����ϴ� �Լ�                        
// ******************************************************************************
void CMyJamData::OnAlarmInfoLoad()
{
	CString strLoadFile;
	CString strReadData;  // ���Ͽ� ����� �� ���� ���� ���� ����
	CString strTempData;
	CString strCommentData;
	int nLen;
	int nPos = 0 ;

	char chr_s[300];
	FILE  *fp ;  

	strLoadFile = _T("C:\\AMT_RT200\\Bmp\\Error.txt"); 

	// **************************************************************************

	char chFileName[50];
	char chMode[50];
	//wsprintfA(chFileName, "%S", sLoadFile);
	clsFunc.OnStringToChar(strLoadFile, chFileName);
	//wsprintfA(chMode, "%S", _T("rt"));
	clsFunc.OnStringToChar(_T("rt"), chMode);
	// **************************************************************************
    // �˶� ���� ����� ���� ����                                                
    // **************************************************************************
//jtkim 20130704
//	if ((fp=fopen(chLoadFile, "rt"))==NULL)  
//	if ((fp = fopen(chLoadFile, chMode)) == NULL)  
	fopen_s(&fp, chFileName, "rt");
	if(!fp)
	{
		AfxMessageBox(_T("The failure because we open the file."));
		return ;
	}
	// **************************************************************************

	// **************************************************************************
    // ������ ���� �������� ��� �о� ���� ������ �����Ѵ�                       
    // **************************************************************************
	while(!feof(fp))  // ���� ������ �˻�
	{
//		sprintf(chr_s, "");
		//wsprintfA(chr_s, "%S", _T(""));
		clsFunc.OnStringToChar(_T(""), chr_s);
		
        if (fgets(chr_s,300,fp)==NULL && ferror(fp))
		{   
			AfxMessageBox(_T("The failure because we read the file."));
			return ;
		}
		
		strReadData.Format(_T("%S"),chr_s);  // ���Ͽ��� ���� �� ���� ���� ����

		nLen = lstrlen(strReadData);		//���� üũ

		if (!strReadData.IsEmpty() && nLen > 2)
		{
			strCommentData = strReadData.Left(2);		//�ּ������� Ȯ���Ѵ�.

			if (strCommentData != "//")					//�ּ����� �ƴ϶��...
			{
				st_alarm_info.strEContent[nPos]	= strReadData ;         // ���Ͽ��� �ε��� �� ���� ���� ����
				st_alarm_info.strECode[nPos]	= strReadData.Left(6) ;    // �˶� Code ����
				st_alarm_info.strEState[nPos]	= strReadData.Mid(7,1) ;  // �˶� ���� ����
				st_alarm_info.strEPart[nPos]	= strReadData.Mid(9,1) ;   // �˶� �߻� ���� ����

				strTempData = strReadData.Mid(12,270);
				if (st_alarm_info.strECode[nPos] != "999999")
				{
					st_alarm_info.strEMsg[nPos] = strTempData.Left(strTempData.GetLength() - 2);
				}
				else
				{
					st_alarm_info.strEMsg[nPos] = strTempData.Left(strTempData.GetLength() - 1) ;  // ���� ���� �˶� �޽��� ����
				}
				nPos++;
			}
		}
	}
	// **************************************************************************

	fclose(fp) ;  // ���� ����
}
// ******************************************************************************

//==================================================================//
// �߻��� �˶� �޽��� ���� ������ �Ҵ�
//==================================================================//
void CMyJamData::OnAlarmInfoSettoVariable(CString strErrCode) 
{
	CTime time;
	int nCNT;
	int nCurrNum = -1;   // �˶� ���� ����� ����ü�� �迭 ��ġ
	CString strAlarmCode;  // �߻��� �˶� �ڵ�

	strAlarmCode = strErrCode;
	strAlarmCode.TrimLeft(' ');
	strAlarmCode.TrimRight(' ');

	// �߻��� �˶��� �迭 ��ġ �˻��Ѵ�
	for(nCNT=0; nCNT<_MAX_ALARM_CNT; nCNT++)
	{
		if ((st_alarm_info.strECode[nCNT]).Compare(strAlarmCode) == 0)
		{
			nCurrNum = nCNT;
			break;
		}
	}

	if (nCurrNum == -1)  // �˶� ������
	{
		st_alarm_info.strPcode	= st_alarm_info.strCode;
		st_alarm_info.strCode	= "999999";
		strAlarmCode			= "999999";

		for(nCNT=0; nCNT<_MAX_ALARM_CNT; nCNT++)
		{
			if ((st_alarm_info.strECode[nCNT]).Compare(strAlarmCode) == 0)
			{
				nCurrNum = nCNT;
				break;
			}
		}
	}

	st_alarm_info.nCurState		= _wtoi(st_alarm_info.strEState[nCurrNum]);
	st_alarm_info.strCurrMsg	= st_alarm_info.strEMsg[nCurrNum];
	st_alarm_info.strCurPart	= st_alarm_info.strEPart[nCurrNum];
	st_alarm_info.strContent	= st_alarm_info.strEContent[nCurrNum];
}
