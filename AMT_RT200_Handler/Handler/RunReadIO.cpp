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
// ������ ����
//==================================================================//
void CRunReadIO::OnThreadRun() 
{
	OnThreadExecute();  // ������ ����
}

//==================================================================//
// ������ ���� ���� �ʱ�ȭ
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
// ������ ����d
//==================================================================//
void CRunReadIO::OnThreadExecute() 
{
	// I/O ���尡 ������ �ʴ� ��� ���� ����
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
