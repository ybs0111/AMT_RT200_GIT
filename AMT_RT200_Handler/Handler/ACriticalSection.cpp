// ACriticalSection.cpp: implementation of the ACriticalSection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ACriticalSection.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
ACriticalSection g_cs;

ACriticalSection::ACriticalSection()
{
	::InitializeCriticalSection( &cs );
}

ACriticalSection::~ACriticalSection()
{
	::DeleteCriticalSection( &cs );
}

void ACriticalSection::EnterCriticalSection()
{
	::EnterCriticalSection( &cs );
}

void ACriticalSection::LeaveCriticalSection()
{
	::LeaveCriticalSection( &cs );
}
