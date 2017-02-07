// MyJamData.h: interface for the CMyJamData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYJAMDATA_H__7F7F1DF4_BB1D_46A4_AFA1_76841EFF5980__INCLUDED_)
#define AFX_MYJAMDATA_H__7F7F1DF4_BB1D_46A4_AFA1_76841EFF5980__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyJamData  
{
public:
	CMyJamData();
	virtual ~CMyJamData();
	
// Attributes
public:
//	CAlarmListFile mcls_alarm_list;  // 알람 리스트 파일 생성 클래스 변수 선언
	
// Operations
public:
	CString OnAlarmDisplayInfoLoad(CString strPath, CString strDataFile, CString strActionFile, CString strCode);  // 발생 알람 정보 설정 함수
	
	CString OnAlarmBmpDataLoad(CString strPath, CString strDataFile, CString strCode);       // 발생 알람 부위 표시용 그림 파일명 설정 함수
	CString OnAlarmStyleDataLoad(CString strPath, CString strDataFile, CString strCode);     // 알람 발생 부위 표시 형태 및 출력 위치 설정 함수
	CString OnAlarmRepairDataLoad(CString strPath, CString strActionFile, CString strCode);  // 발생한 알람에 대한 조치 사항 설정 함수
	
	void OnAlarmDisplay();								// 발생한 알람 화면 출력 함수
	void OnOccuredAlarmSavingtoFile();					// 발생한 알람 로그 파일에 저장하는 함수
	void OnAlarmInfoLoad();								// 파일에 저장된 모든 알람 정보 전역 변수에 설정하는 함수
	void OnAlarmInfoSettoVariable(CString strErrCode);  // 발생한 알람 메시지 전역 변수에 설정하는 함수
};
extern CMyJamData			clsMyJamData;
#endif // !defined(AFX_MYJAMDATA_H__7F7F1DF4_BB1D_46A4_AFA1_76841EFF5980__INCLUDED_)
