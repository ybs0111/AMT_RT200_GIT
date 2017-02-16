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
public:
	int m_nInitStep;
	int m_nLabelFeeder;
	int m_nInterFaceStep;
	long m_lWaitInterFace[3];

	CString m_strAlarmCode;
	
};

extern CRunLabelFeeder	clsRunLabelFeeder;