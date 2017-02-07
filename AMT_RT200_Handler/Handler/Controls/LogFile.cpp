///////////////////////////////////////////////////////////////////////////////
/* ****************************************************************************** */
// FILE NAME : LogFile.cpp
//
// Copyright (C) 2007 DEnT Corporation 
// All rights reserved.
//
// CREATED BY  : Lee Kyu Il
// REVISION    :
//     1> 2007. 03. 09 : Created by Lee Kyu Il
// DESCRIPTION :
//     Implementation of the Log and associated classes
//	   This code may be used in compiled form in any way you desire. This
//	   file may be redistributed unmodified by any means PROVIDING it is
//	   not sold for profit without the authors written consent, and
//	   providing that this notice and the authors name and all copyright
//	   notices remains intact.
//
//	   An email letting me know how you are using it would be nice as well.
//
//
// REFERENCE   :
// 
/* ****************************************************************************** */
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogFile.h"

#include <direct.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CLogFile::CLogFile()
{
	mpLogFile = NULL;
}

/********************************************************************/
/*                                                                  */
/* Description   : Destructor                                       */
/*               : Close if Log File Opened                         */
/********************************************************************/
CLogFile::~CLogFile()
{
	OnCloseFile();
}

/********************************************************************/
/*                                                                  */
/* Function name : OnOpenFile                                       */
/* Description   : Log File Open Function                           */
/********************************************************************/
void CLogFile::OnOpenFile(LPCTSTR strFile, bool bAppend, long lTruncate) 
{
	mlTruncate   = lTruncate;
	mstrFileName = strFile;

	CString strTempFile = mstrPath;
	strTempFile += strFile;

	TCHAR  szFile[MAX_PATH+1];
	_tcscpy(szFile, strTempFile);

	if ( !(mpLogFile == fopen(szFile, bAppend ? "a" : "w")) )
	{
		OnCreateDirectories(szFile);  // Log File Saving Folder Making Function 
		mpLogFile = fopen(szFile, bAppend ? "a" : "w");
	}

	InitializeCriticalSection(&mcsLogSync);
}

/********************************************************************/
/*                                                                  */
/* Function name : OnCloseFile                                      */
/* Description   : Log File Close Function                          */
/********************************************************************/
void CLogFile::OnCloseFile() 
{
	if (mpLogFile)
	{
		fputs("\n=============== [Finish Logging] ================\n\n", mpLogFile);
		fclose(mpLogFile);

		DeleteCriticalSection(&mcsLogSync);  // 수정 [2007.03.09] 
	}

	// 주석 처리 [2007.03.09]
	// OnOpenFile() 함수가 실행된 경우에만 InitializeCriticalSection() 실행 됨 
	// 이런 이유로 위치 변경 함 
	//DeleteCriticalSection(&mcsLogSync);
}

/********************************************************************/
/*                                                                  */
/* Function name : OnChangeFile                                     */
/* Description   : Log File Exchange Function                       */
/********************************************************************/
void CLogFile::OnChangeFile(LPCTSTR strFile, bool bAppend, long lTruncate) 
{
	if (strFile != mstrFileName)
	{
		OnCloseFile();  // Log File Close Function 
		OnOpenFile(strFile, bAppend, lTruncate);  // Log File Open Function 
	}
}

/********************************************************************/
/*                                                                  */
/* Function name : OnSetPath                                        */
/* Description   : Log Creating Path Setting Function               */
/********************************************************************/
void CLogFile::OnSetPath(LPCTSTR pzPath) 
{
	mstrPath = pzPath;  // File Folder Set 
	if ( mstrPath.Right(1) != _T("\\") )
	{
		mstrPath += _T("\\");
	}
}

/********************************************************************/
/*                                                                  */
/* Function name : OnCreateDirectories                              */
/* Description   : Log File Saving Folder Making Function           */
/********************************************************************/
void CLogFile::OnCreateDirectories(CString strFileName) 
{
	CString strDRVName, strFolder;

	int  nDrive, nPer;

	strDRVName = strFileName.Left(3);
	nDrive = strDRVName[0] - 'A' + 1;
	_chdrive(nDrive);

	for(nPer=1; nPer <= 20; nPer++)
	{
		_chdir("..");
	}

	strFileName = strFileName.Mid(3);
	while( !strFileName.IsEmpty() )
	{
		nPer = strFileName.Find('\\');
		if (nPer == -1)
		{
			break;
		}

		strFolder = strFileName.Left(nPer);
		if ( _chdir(strFolder) )
		{
			_mkdir(strFolder);
			_chdir(strFolder);
		}

		strFileName = strFileName.Mid(nPer+1);
	}
}
