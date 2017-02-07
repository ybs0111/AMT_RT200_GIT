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
//	CAlarmListFile mcls_alarm_list;  // �˶� ����Ʈ ���� ���� Ŭ���� ���� ����
	
// Operations
public:
	CString OnAlarmDisplayInfoLoad(CString strPath, CString strDataFile, CString strActionFile, CString strCode);  // �߻� �˶� ���� ���� �Լ�
	
	CString OnAlarmBmpDataLoad(CString strPath, CString strDataFile, CString strCode);       // �߻� �˶� ���� ǥ�ÿ� �׸� ���ϸ� ���� �Լ�
	CString OnAlarmStyleDataLoad(CString strPath, CString strDataFile, CString strCode);     // �˶� �߻� ���� ǥ�� ���� �� ��� ��ġ ���� �Լ�
	CString OnAlarmRepairDataLoad(CString strPath, CString strActionFile, CString strCode);  // �߻��� �˶��� ���� ��ġ ���� ���� �Լ�
	
	void OnAlarmDisplay();								// �߻��� �˶� ȭ�� ��� �Լ�
	void OnOccuredAlarmSavingtoFile();					// �߻��� �˶� �α� ���Ͽ� �����ϴ� �Լ�
	void OnAlarmInfoLoad();								// ���Ͽ� ����� ��� �˶� ���� ���� ������ �����ϴ� �Լ�
	void OnAlarmInfoSettoVariable(CString strErrCode);  // �߻��� �˶� �޽��� ���� ������ �����ϴ� �Լ�
};
extern CMyJamData			clsMyJamData;
#endif // !defined(AFX_MYJAMDATA_H__7F7F1DF4_BB1D_46A4_AFA1_76841EFF5980__INCLUDED_)
