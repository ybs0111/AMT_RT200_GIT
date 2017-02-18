#pragma once
#include "Variable.h"

class CRunLabelFeeder
{
public:
	CRunLabelFeeder(void);
	~CRunLabelFeeder(void);

public:
	void OnThreadRUN(void);      // 쓰레드 동작
	void OnRunInit(void);     // 쓰레드 관련 변수 초기화
	void OnFeederInterface();
	void OnRunFeeder();
	//kwlee 2017.0216
	void OnSetRejectCyl(int nMode, int nUpDn);
	int  OnGetRejectCyl(int nMode, int nUpDn);

public:
	int m_nInitStep;
	int m_nLabelFeeder;
	int m_nInterFaceStep;
	bool m_bCylFlag[2];
	double m_dwWRejectCylWaitTime[2][3];
	double m_dwWFeederWaitTime[3];

	long m_lWaitInterFace[3];

	CString m_strAlarmCode;
	
};

extern CRunLabelFeeder	clsRunLabelFeeder;