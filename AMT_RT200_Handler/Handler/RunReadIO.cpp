// RunReadIO.cpp: implementation of the CRunReadIO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RunReadIO.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRunReadIO	clsReadIO;

CRunReadIO::CRunReadIO()
{

}

CRunReadIO::~CRunReadIO()
{

}

//==================================================================//
// 쓰레드 동작
//==================================================================//
void CRunReadIO::OnThreadRun() 
{
	OnThreadExecute();  // 쓰레드 제어
}

//==================================================================//
// 쓰레드 관련 변수 초기화
//==================================================================//
void CRunReadIO::OnThreadInit() 
{
	if (m_bInitFlag == true)	
	{
		return;
	}
	m_bInitFlag = true;
}

//==================================================================//
// 쓰레드 제어d
//==================================================================//
void CRunReadIO::OnThreadExecute() 
{
	// I/O 보드가 열리지 않는 경우 강제 리턴
 	if (FAS_IO.m_nIoBoardInitialFlag == NO)	
	{
		return;
	}

	FAS_IO.IO_ReadWrite(0, 0, 0, 1);
	FAS_IO.IO_ReadWrite(0, 0, 1, 1);
	FAS_IO.IO_ReadWrite(0, 0, 2, 1);
	FAS_IO.IO_ReadWrite(0, 0, 3, 1);
	FAS_IO.IO_ReadWrite(0, 0, 4, 1);
	FAS_IO.IO_ReadWrite(0, 0, 5, 1);
	FAS_IO.IO_ReadWrite(0, 0, 6, 1);
	FAS_IO.IO_ReadWrite(0, 0, 7, 1);

	FAS_IO.IO_ReadWrite(0, 1, 0, 1);

}
