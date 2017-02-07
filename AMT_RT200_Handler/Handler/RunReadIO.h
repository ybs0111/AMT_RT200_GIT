// CRunReadIO.h: interface for the CRunReadIO class.
//
//////////////////////////////////////////////////////////////////////

 #if !defined(AFX_RUNREADIO_H__F3998ED3_4ABD_4580_9A30_500D60821706__INCLUDED_)
 #define AFX_RUNREADIO_H__F3998ED3_4ABD_4580_9A30_500D60821706__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Variable.h"
#include "FastechPublic_IO.h"

class CRunReadIO  
{
public:
	CRunReadIO();
	virtual ~CRunReadIO();


// Attributes
private:
	bool m_bInitFlag;
	
// Attributes
public:
	
// Operations
private:
	
// Operations
public:
	void OnThreadRun(void);      // ������ ����
	void OnThreadInit(void);     // ������ ���� ���� �ʱ�ȭ
	void OnThreadExecute(void);  // ������ ����

};
extern CRunReadIO	clsReadIO;

#endif // !defined(AFX_RUNREADIO_H__F3998ED3_4ABD_4580_9A30_500D60821706__INCLUDED_)
