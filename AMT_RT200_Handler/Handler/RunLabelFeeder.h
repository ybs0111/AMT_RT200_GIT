#pragma once
#include "Variable.h"

class CRunLabelFeeder
{
public:
	CRunLabelFeeder(void);
	~CRunLabelFeeder(void);

public:
	void OnThreadRUN(void);      // ������ ����
	void OnRunInit(void);     // ������ ���� ���� �ʱ�ȭ
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