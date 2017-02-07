// ACriticalSection.h: interface for the ACriticalSection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACRITICALSECTION_H__3265B9CE_EEE7_4F1C_B00B_D7F8C0BDEA26__INCLUDED_)
#define AFX_ACRITICALSECTION_H__3265B9CE_EEE7_4F1C_B00B_D7F8C0BDEA26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ACriticalSection  
{
public:
	ACriticalSection();
	virtual ~ACriticalSection();

	void EnterCriticalSection();
	void LeaveCriticalSection();


	CRITICAL_SECTION cs;
};

extern ACriticalSection g_cs;

#endif // !defined(AFX_ACRITICALSECTION_H__3265B9CE_EEE7_4F1C_B00B_D7F8C0BDEA26__INCLUDED_)
