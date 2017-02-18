// MyJamData.cpp: implementation of the CMyJamData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "MyJamData.h"
// #include "MonitoringLog.h"
// #include "Alg_Database.h"
// 
// #include "Public_Function.h"  // 모터 보드 및 I/O 보드 관련 클래스
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
// 발생 알람 정보 설정 함수                                                      
// -> strPath     : 알람 정보 저장 파일 존재 폴더                                
// -> strDataFile : 알람 정보 저장된 File Name(File Name+확장자)                       
// -> strCode     : 발생한 알람 Code                                             
// ******************************************************************************
CString CMyJamData::OnAlarmDisplayInfoLoad(CString strPath, CString strDataFile, CString strActionFile, CString strCode)
{

	CString sFilePath;  // 폴더 저장 변수
	CString sAlarmChk;  // 함수 리턴 정보 저장 변수
	int n_pos;
	
	sFilePath = strPath;
	if (sFilePath.Right(1) != _T("\\"))  // 파일 경로 구분자 존재 검사
	{
		sFilePath += _T("\\");
	}

	sAlarmChk = OnAlarmBmpDataLoad(sFilePath, strDataFile, strCode);  // 발생 알람 부위 표시용 그림 File Name 설정 함수
	n_pos = sAlarmChk.Find(_T("Trouble"));
	if (n_pos >= 0)  // 에러 발생
	{
		return sAlarmChk;
	}

	sAlarmChk = OnAlarmStyleDataLoad(sFilePath, strDataFile, strCode);  // 알람 발생 부위 표시 형태 및 출력 위치 설정 함수
	n_pos = sAlarmChk.Find(_T("Trouble"));
	if (n_pos >= 0)  // 에러 발생
	{
		return sAlarmChk;
	}

	sAlarmChk = OnAlarmRepairDataLoad(sFilePath, strActionFile, strCode);  // 발생한 알람에 대한 조치 사항 설정 함수
	n_pos = sAlarmChk.Find(_T("Trouble"));
	if (n_pos >= 0)  // 에러 발생
	{
		return sAlarmChk;
	}

	return sAlarmChk;
}
// ******************************************************************************

// ******************************************************************************
// 발생 알람 부위 표시용 그림 File Name 설정 함수                                
// -> strPath     : 알람 표시 그림 파일 존재 폴더                                
// -> strDataFile : 알람 표시 그림 File Name(File Name+확장자)                   
// -> strCode     : 발생한 알람 Code                                             
// ******************************************************************************
CString CMyJamData::OnAlarmBmpDataLoad(CString strPath, CString strDataFile, CString strCode)
{
	CString sReturnData;  // 함수 리턴 플래그
	CString sFile;  // 알람 표시용 그림에 대한 [폴더]+[File Name] 저장 변수
	CString sTemp;  // 알람 표시용 그림 File Name 임시 저장 변수

	char chBuf[1000];

	sReturnData = _T("OK");  // 함수 리턴 플래그 초기화

	sFile = strPath + strDataFile;  // [폴더]+[File Name] 설정


	if (!GetPrivateProfileString(strCode, _T("DEF"), _T(""), (LPWSTR)chBuf, 1000, sFile))
	{
		sReturnData = _T("File Name : ") + sFile;
		sReturnData += _T("\r\n");
		sReturnData += _T("[DEF] The information we read. Trouble -> Code : ");
		sReturnData += strCode;

		return sReturnData;
	}

	sTemp.Format(_T("%s"), chBuf);
	st_alarm_info.strBmpFile = sTemp;  // 알람 표시용 그램 File Name 설정

	return sReturnData;
}
// ******************************************************************************

// ******************************************************************************
// 알람 발생 부위 표시 형태 및 출력 위치 설정 함수                               
// -> strPath     : 부위 표시 형태 저장 파일 폴더                                
// -> strDataFile : 부위 표시 형태 저장 File Name(File Name+확장자)              
// -> strCode     : 발생한 알람 Code                                             
// ******************************************************************************
CString CMyJamData::OnAlarmStyleDataLoad(CString strPath, CString strDataFile, CString strCode)
{
	CString sChkData;		// 함수 리턴 플래그
	CString sChkFile;		// 알람 부위 표시 정보에 대한 [폴더]+[File Name] 저장 변수
	CString sChk, sChk2;  // 알람 부위 표시 형태 및 위치 저장 변수
	int nStart=0, nEnd, i;		// 읽을 데이터 위치 저장 변수
	int nMax=4;					// 읽을 데이터 최대 갯수 (화살표 모양으로 초기화)

	char chr_buf[1000];
	char num[20];

	sChkData = _T("OK");  // 함수 리턴 플래그 초기화

	sChkFile = strPath + strDataFile;  // [폴더]+[File Name] 설정


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
		if (nEnd == -1)  // 구분자 미존재
		{
			break;
		}
		else            // 구분자 존재
		{
			sChk2 = sChk.Mid(nStart, nEnd-nStart);  // 구분자 사이의 한 개 아이템 설정
			sChk2.TrimLeft(' ');
			sChk2.TrimRight(' ');

			if (i == 0)  // 알람 부위 표시 형태
			{
				st_alarm_info.strStyle = sChk2;       // 알람 부이 표시 형태 설정
				if (st_alarm_info.strStyle == "CIRCLE")  // 부위 표시 정보 8개 존재
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
					st_alarm_info.nCx = atoi(num);         // X축 픽셀 위치
				}
				else if (i == 2)
				{
					st_alarm_info.nCy = atoi(num);         // Y축 픽셀 위치
				}
				else if (i == 3)
				{
					st_alarm_info.nOffset = atoi(num);     // 원 크기
				}
				else if (i == 4)
				{
					st_alarm_info.nLineWidth = atoi(num);  // 라인 두께
				}
				else if (i == 5)
				{
					st_alarm_info.nR = atoi(num);          // RGB 중 R 값
				}
				else if (i == 6)
				{
					st_alarm_info.nG = atoi(num);          // RGB 중 G 값
				}
				else if (i == 7) 
				{
					st_alarm_info.nB = atoi(num);          // RGB 중 B 값
				}
				// **************************************************************
			}

			nStart = nEnd + 1;  // 다음 데이터 위치 설정
		}
	}

	if (i != nMax)  // Data Format 에러
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
// 발생한 알람에 대한 조치 사항 설정 함수                                        
// -> strPath       : 알람 조치 사항 저장된 파일 폴더                            
// -> strActionFile : 알람 조치 사항 저장된 File Name(File Name+확장자)          
// -> strCode       : 발생한 알람 Code                                           
// ******************************************************************************
CString CMyJamData::OnAlarmRepairDataLoad(CString strPath, CString strActionFile, CString strCode)
{
	CString sReturnVal;		// 함수 리턴 플래그
	CString sRepairFile;	// 조치 사항에 대한 [폴더]+[File Name] 저장 변수
	CString sActItem;		// 조치 사항 읽을 위치 설정 변수
	CString sActTemp;		// 조치 사항 임시 저장 변수
	int nChkCnt;				// 조치 사항 갯수 저장 변수
	int i; 
	
	char chr_buf[1000];

	sReturnVal = _T("OK");  // 함수 리턴 플래그 초기화

	sRepairFile = strPath + strActionFile;  // [폴더]+[File Name] 설정
	
	if (!GetPrivateProfileString(strCode, _T("COUNT"), _T("0"), (LPWSTR)chr_buf, 10, sRepairFile))
	{
		sReturnVal = _T("File Name : ") + sRepairFile;
		sReturnVal += _T("\r\n");
		sReturnVal += _T("[COUNT] The information we read. Trouble -> Code : ");
		sReturnVal += strCode;

		return sReturnVal;
	}

	nChkCnt = atoi(chr_buf);  // 읽을 조치 사항 갯수 설정
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

	(st_alarm_info.strRepairMsg).Empty();  //  조치 사항 저장 변수 초기화

	for (i=0; i<(st_alarm_info.nActionCnt); i++)   
	{
		sActItem.Format(_T("%d"), i+1);  // 조치 사항 정보 읽을 위치 설정
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

		st_alarm_info.strRepairMsg += _T("\r\n");			//조치 사항에 맨 첫줄을 띄자.

		sActTemp.Format(_T("%s"), chr_buf);				// 조치 사항 설정
		st_alarm_info.strRepairMsg += sActTemp;	

		if (i != ((st_alarm_info.nActionCnt)-1))  
			st_alarm_info.strRepairMsg += "\r\n";			// 조치 사항 단락 구분자 설정
	}
	// **************************************************************************
	return sReturnVal;
}
// ******************************************************************************

// ******************************************************************************
// 발생한 알람 화면 출력 함수                                                    
// ******************************************************************************
void CMyJamData::OnAlarmDisplay()
{
	CString sCnt, sTemp;
	char chTempCode[10], chTempCnt[10], chBuf[20]={0,};
 	
 	CString sDisplayTime;	// 파일 출력용 시간 저장 변수
 	CString sAlarmCode;		// 발생한 알람 코드 저장 변수
 	CString sAlarmMsg;		// 발생한 알람 메시지 저장 변수
 	int nCurHour;				// 현재 시간 저장 변수
 	int nChk;					// 숫자형 알람 코드 저장 변수
// 	long nChkTime;			// 알람 발생 시간 간격 저장 변수
 	int nDataSave = 0;		// Jam을 저장할 것이냐?
 
 	COleDateTime cCurTime;
 	CTime tAlarmOccuredTime;

 	// **********************************************************************
 	// 장비 동작 중 알람 발생했는지 검사한다                                     
 	// -> 알람 발생한 경우 로그 파일 및 임시 파일에 저장하고 화면에 출력한다     
 	// **********************************************************************

	//st_alarm_info.strCode = _T("010006");
 	// 알람 발생110308
 	if (st_handler_info.nRunStatus == dJAM || st_handler_info.nRunStatus == dWARNING || st_handler_info.nRunStatus == dLOTEND)  
 	{
 		if (st_handler_info.nAlarmScreen == TRUE)	return;
 
 		// 리스트 바 화면 존재
 		if (st_handler_info.cWndList != NULL)  
 		{
			sAlarmMsg.Format(_T("Alarm Code = %s"), st_alarm_info.strCode);
//			wsprintfA(st_other_info.cAbnormalMsg, "%S", st_other_info.strAbnormalMsg);
			//clsFunc.OnStringToChar(st_other_info.strAbnormalMsg, st_other_info.cAbnormalMsg);
			clsMem.OnAbNormalMessagWrite(sAlarmMsg);
 			// 동작 실패 출력 요청
 			st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  
 		}

 		// *****************************************************
 		// 알람 화면 출력 불가능한 경우 검사한다.                                
 		// -> st_handler.n_system_lock : SYSTEM LOCK 상태 플래그                
 		// *****************************************************
 		if (st_handler_info.nSystemLock != FALSE)  // SYSTEM LOCK
 		{
 			return;  // 강제 리턴
 		}
 
 		// ***************************
 		// 알람 발생한 시간 설정한다                                                 
 		// ***************************
 		cCurTime = COleDateTime::GetCurrentTime();
 		nCurHour = cCurTime.GetHour();
 
 		tAlarmOccuredTime = CTime::GetCurrentTime();
 		sDisplayTime = tAlarmOccuredTime.Format("%c");
 
 		// **********************************************************************
 		// 알람 화면 출력 상태 및 알람 카운트 여부 검사한다                      
 		// -> 이미 알람 화면 출력 중인 경우 재 출력할 필요 없으며                
 		//    이 경우에 발생하는 알람은 무시한다                                 
 		//  : 알람 화면 출력 중에 알람 Code 변경되면 출력 메시지 달라지기 때문   
 		// -> st_handler.n_alarm_screen : 알람 화면 출력 상태 플래그             
 		// -> st_alarm.n_cur_state : 알람 저장 유무 플래그                       
 		// **********************************************************************
 		// 알람 화면 미출력
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
 
 			//발생한 알람을 List에 넣는다.
 			if (st_alarm_info.strCode != "")
 			{
 				// 마지막 알람을 저장한다.
 				// st_handler_info.str_last_alarm = alarm.str_code;	
 				
				// On_Alarm_Info_Load함수를 MainFrame 생성자에서 호출하여
				// Error.txt의 파일 값을 가져와 글로벌 변수에 대입후 
				// Error.txt의 파일에 일치하는 코드를 찾는 함수이며 
				// 일치하는 CODE의 주변 내용을 가져와 변수에 등록하는 함수이다. 
 				OnAlarmInfoSettoVariable(st_alarm_info.strCode);  
 				// *********************************************

				// 알람 발생 정보를 파일에 저장한다
 				CtlBdFunc.Alarm_Error_Occurrence(st_alarm_info.nCountMode, st_alarm_info.nTypeMode, st_handler_info.nRunStatus, st_alarm_info.strCode);

				//wsprintfA(chTempCode, "%S", st_alarm_info.strCode);
				clsFunc.OnStringToChar(st_alarm_info.strCode, chTempCode);
 				sCnt.Format(_T("%d"), st_alarm_info.nCurState);

				//wsprintfA(chTempCnt, "%S", sCnt);
				clsFunc.OnStringToChar(sCnt, chTempCnt);
 
 				// *******************************************
 				// 바로 전에 발생한 알람 코드와 비교하여  
 				// 동일한 경우 알람 카운트 미진행      
 				// -> 알람 카운트 시간 간격은 30초로 설정한다                                
 				// *******************************************
 				st_alarm_info.stl_cur_alarm_time = GetTickCount() / 1000;
 
 				nChk = _wtoi(st_alarm_info.strCode);
 				
				// 현재 발생한 알람과 이전에 발생한 알람 시간을 비교 후 설정 시간 뒤에 발생하였다면 저장 그렇지 않으면 저장하지 않는다. 
				// 설정 시간이 없는 관계로 일단 주석처리한다. 
				/*
 				n_chk_time = st_alarm_info.stl_cur_alarm_time - st_alarm_info.stl_prev_alarm_time; 
 				
 				if (n_chk_time < st_alarm_info.nAlarmSaveDelayTM) 
 				{
 					st_alarm_info.stl_prev_alarm_time = st_alarm_info.stl_cur_alarm_time;
 				}
 				else
 				{
 					st_alarm_info.stl_prev_alarm_time = st_alarm_info.stl_cur_alarm_time;
 					// Jam을 저장 및 Count를 한다.
 					n_data_save = YES;			
 				}
				*/
 			
 				// 현재 발생한 ALARM CODE와 이전에 발생한 ALARM CODE를 
				// 비교하여 같은 ALARM CODE는 저장하지 않는다. 
				if (st_alarm_info.stl_prev_code == nChk)  
 				{
 					st_alarm_info.stl_prev_alarm_time = st_alarm_info.stl_cur_alarm_time;
 				}
 				else
 				{
 					// **********************************************************************
 					// 발생한 알람 정보로 이전 정보를 대체한다                               
 					// -> 다음에 발생하는 알람과 비교하는 정보로 사용된다                    
 					// **********************************************************************
 					st_alarm_info.stl_prev_code = nChk;
 					st_alarm_info.stl_prev_alarm_time = st_alarm_info.stl_cur_alarm_time;
 					// **********************************************************************
 					
					// ALARM 정보를 저장하는 플레그이다. 
 					nDataSave = YES;			
 				}
 				// **************************************************************************
 			}
 
			// ALARM CODE를 저장하기 플레그 확인한다. 
 			if (nDataSave == YES)
 			{
 				// ERROR.txt 파일의 내용: <000001 1 0 "X_Axis Motor Home Check Error.">에서 2번째 값 1은 저장 유/무를 확인하는 값이며
				// 이 값을 확인한다. 
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
 			// 알람 화면 출력 플래그 설정	
 			st_handler_info.nAlarmScreen = TRUE;	

 			// 알람 사운드와 알람 리셋 램프 ON
 			//Func.uSetAlarmPort();  
 
 			// 메인 프레임에 알람 화면 출력 요청 

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

 			// 모션 보드 함수에서 RUN 상태일 경우에만 위치값 저장하게 되어 있다
 			// 헌데, 현재 설비 상태를 설정해주는 곳이 없어 이곳에 추가한다
 			COMI.mn_run_status = st_handler_info.nRunStatus;

			clsFunc.OnMCStop();
 		}
 	}
}

// **************************************
// 발생한 알람 로그 파일에 저장하는 함수                                         
// **************************************
void CMyJamData::OnOccuredAlarmSavingtoFile()
{
//	char chTitleTime[20];
//	char chAlarmCode[10];
	char chAlarmMsg[1000];
//	char chMsg[1000];

	CString strDisplayTime;	// 파일 출력용 시간 저장 변수
	CString strAlarmCode;		// 발생한 알람 Code 저장 변수
	CString strAlarmMsg;		// 발생한 알람 메시지 저장 변수
	CString strTemp;
	CString strAlarmData;		// 발생한 알람 메시지 저장 변수
	int nCurHour;				// 현재 시간 저장 변수

	COleDateTime cCurTime;
	CTime tAlarmOccuredTime;

	CString strFileMonth, strFileDay, strTitleTime;
	int nCurYear, nCurMonth, nCurDay;		// 년, 월, 일 저장 변수

	CString str_month_path, str_day_path;

	FILE *fp;
	CString sLoadFile = _T("");

	// **************************************************************************
	// 알람 발생한 시간 설정한다                                                 
	// -> SHIFT 상태 검사 및 알람 발생 시간 저장하기 위하여 필요                 
	// **************************************************************************
	cCurTime = COleDateTime::GetCurrentTime();
	nCurHour = cCurTime.GetHour();

	tAlarmOccuredTime	= CTime::GetCurrentTime();
	strDisplayTime		= tAlarmOccuredTime.Format("%c");
	// **************************************************************************

	// **************************************************************************
	// 알람 Code 및 알람 메시지 함수 변수에 설정한다                             
	// -> 현재는 이 함수 호출 전에 새로운 알람이 발생하는 경우 알람 Code와       
	//    조치 사항 및 기타 정보가 일치하지 않을 수 있다                         
	// -> alarm.str_code : 현재 발생한 알람 Code 정보 저장 변수                  
	// -> st_alarm.str_cur_msg : 발생한 알람 메시지 저장 변수                    
	// **************************************************************************
	strAlarmCode	= st_alarm_info.strCode;
	strAlarmMsg		= st_alarm_info.strCurrMsg;
	// **************************************************************************

	// **************************************************************************
	// 일별, 월별 알람 발생 횟수 임시 파일에 추가한다                            
	// **************************************************************************
	// 일별, 월별 알람 정보 리스트 파일에 추가 함수
	//	mcls_alarm_list.On_Daily_ListFile_Add(str_alarm_code, str_display_time, str_alarm_msg); 
	// **************************************************************************

	// **************************************************************************
	// 바로 전에 발생한 알람 Code와 비교하여 동일한 경우 알람 카운트 미진행      
	// -> 알람 카운트 시간 간격은 30초로 설정한다                                
	// **************************************************************************

	// **************************************************************************
	// 파일 이름으로 사용할 날짜 설정한다                                        
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

 		// 월 데이타 기록
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
 		// 일 데이타 기록
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
 	// SHIFT별 알람 발생 횟수 임시 저장 파일에 알람 추가한다                     
 	// -> 알람 발생 횟수 임시 저장 파일 미존재 시 생성할 알람 리스트 파일 이름은 
 	//    일별 파일 이름에서 헤더를 제외한 정보와 동일하다                       
 	// **************************************************************************
 	if (nCurHour>=6 && nCurHour<14)  // SHIFT 중 DAY
 	{
 		// SHIFT별 알람 발생 횟수 정보 임시 파일에 추가 함수
 		// mcls_alarm_list.On_Shift_File_Add(1, str_alarm_code, str_display_time);  
 	}
 	else if (nCurHour >= 14 && nCurHour < 22)  // SHIFT 중 SWING
 	{
 		// SHIFT별 알람 발생 횟수 정보 임시 파일에 추가 함수
 		// mcls_alarm_list.On_Shift_File_Add(2, str_alarm_code, str_display_time);  
 	}
 	else 
 	{
 		// SHIFT별 알람 발생 횟수 정보 임시 파일에 추가 함수
 		// mcls_alarm_list.On_Shift_File_Add(3, str_alarm_code, str_display_time);  
 	}
 	// **************************************************************************
 
 	// **************************************************************************
 	// LOT별 알람 발생 횟수 임시 파일에 추가한다                                 
 	// **************************************************************************
 	// LOT별 알람 발생 횟수 정보 임시 파일에 추가 함수
 	// mcls_alarm_list.On_Lot_ListFile_Add(str_alarm_code, str_display_time);  
 	// **************************************************************************
}
// ******************************************************************************

// ******************************************************************************
// 파일에 저장된 모든 알람 정보 전역 변수에 설정하는 함수                        
// ******************************************************************************
void CMyJamData::OnAlarmInfoLoad()
{
	CString strLoadFile;
	CString strReadData;  // 파일에 저장된 한 라인 정보 저장 변수
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
    // 알람 정보 저장된 파일 연다                                                
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
    // 마지막 파일 정보까지 모두 읽어 전역 변수에 설정한다                       
    // **************************************************************************
	while(!feof(fp))  // 파일 마지막 검사
	{
//		sprintf(chr_s, "");
		//wsprintfA(chr_s, "%S", _T(""));
		clsFunc.OnStringToChar(_T(""), chr_s);
		
        if (fgets(chr_s,300,fp)==NULL && ferror(fp))
		{   
			AfxMessageBox(_T("The failure because we read the file."));
			return ;
		}
		
		strReadData.Format(_T("%S"),chr_s);  // 파일에서 읽은 한 라인 정보 설정

		nLen = lstrlen(strReadData);		//빈줄 체크

		if (!strReadData.IsEmpty() && nLen > 2)
		{
			strCommentData = strReadData.Left(2);		//주석줄인지 확인한다.

			if (strCommentData != "//")					//주석줄이 아니라면...
			{
				st_alarm_info.strEContent[nPos]	= strReadData ;         // 파일에서 로딩한 한 라인 정보 설정
				st_alarm_info.strECode[nPos]	= strReadData.Left(6) ;    // 알람 Code 설정
				st_alarm_info.strEState[nPos]	= strReadData.Mid(7,1) ;  // 알람 상태 설정
				st_alarm_info.strEPart[nPos]	= strReadData.Mid(9,1) ;   // 알람 발생 부위 설정

				strTempData = strReadData.Mid(12,270);
				if (st_alarm_info.strECode[nPos] != "999999")
				{
					st_alarm_info.strEMsg[nPos] = strTempData.Left(strTempData.GetLength() - 2);
				}
				else
				{
					st_alarm_info.strEMsg[nPos] = strTempData.Left(strTempData.GetLength() - 1) ;  // 공백 제거 알람 메시지 설정
				}
				nPos++;
			}
		}
	}
	// **************************************************************************

	fclose(fp) ;  // 파일 종료
}
// ******************************************************************************

//==================================================================//
// 발생한 알람 메시지 전역 변수에 할당
//==================================================================//
void CMyJamData::OnAlarmInfoSettoVariable(CString strErrCode) 
{
	CTime time;
	int nCNT;
	int nCurrNum = -1;   // 알람 정보 저장된 구조체의 배열 위치
	CString strAlarmCode;  // 발생한 알람 코드

	strAlarmCode = strErrCode;
	strAlarmCode.TrimLeft(' ');
	strAlarmCode.TrimRight(' ');

	// 발생한 알람의 배열 위치 검색한다
	for(nCNT=0; nCNT<_MAX_ALARM_CNT; nCNT++)
	{
		if ((st_alarm_info.strECode[nCNT]).Compare(strAlarmCode) == 0)
		{
			nCurrNum = nCNT;
			break;
		}
	}

	if (nCurrNum == -1)  // 알람 미존재
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
