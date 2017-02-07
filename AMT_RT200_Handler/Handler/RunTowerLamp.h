// RunTowerLamp.h: interface for the CRunTowerLamp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNTOWERLAMP_H__9C2074DB_6A6E_4E44_B927_9AC2480A4711__INCLUDED_)
#define AFX_RUNTOWERLAMP_H__9C2074DB_6A6E_4E44_B927_9AC2480A4711__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Variable.h"
#include "FastechPublic_IO.h"

class CRunTowerLamp  
{
public:
	CRunTowerLamp();
	virtual ~CRunTowerLamp();


// Attributes
public:
	int					m_nLopCNT;

	bool				m_bInitFlag;
	int					m_nLampStep;
	long				m_lLampTime[3];
	long				m_lWaitTime[3];
	int					m_nLampType;
	int					m_nOldStatus;

// Operations
public:
	void				OnThreadRun();      // 쓰레드 동작
	void				OnTowerLamp(int nLamp);
	void				OnSwitchLamp(int nLamp);
	void				OnRunInit();
	void				OnTowerLamp();
};
extern CRunTowerLamp	clsTowerLamp;

#endif // !defined(AFX_RUNTOWERLAMP_H__9C2074DB_6A6E_4E44_B927_9AC2480A4711__INCLUDED_)
