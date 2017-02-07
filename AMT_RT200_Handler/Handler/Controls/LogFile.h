///////////////////////////////////////////////////////////////////////////////
/* ****************************************************************************** */
// FILE NAME : LogFile.h
//
// Copyright (C) 2007 DEnT Corporation 
// All rights reserved.
//
// CREATED BY  : Lee Kyu Il
// REVISION    :
//     1> 2007. 03. 09 : Created by Lee Kyu Il
// DESCRIPTION :
//     not depends on MFC ore ATL
//	   file name could use absolute path or just the name, in which case the 
//	   file will be created at the same place with the binary module, no concern 
//	   with corect directory, which always bring me trouble.
//	   every log line has a time stamp attached, with million seconds.
//	   uses printf like format to write log lines
//	   uses a preprocessor definition _DEBUG_LOG to switch if write log file
//	   multi thread safe, finally added:)
//
//	   An email letting me know how you are using it would be nice as well.
//
//
// REFERENCE   :
// 
/* ****************************************************************************** */
///////////////////////////////////////////////////////////////////////////////

#ifndef _ATA_LOGFILE_
#define _ATA_LOGFILE_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define MAX_BUF	40960

class CLogFile
{
public:
	void OnCreateDirectories(CString strFileName);  // Log File Saving Folder Making Function 
	
	// Constructor 
	CLogFile();

	// Destructor 
	~CLogFile();  // Close if Log File Opened 

	void OnOpenFile(LPCTSTR strFile, bool bAppend = TRUE, long lTruncate = MAX_BUF);    // Log File Open Function 
	void OnChangeFile(LPCTSTR strFile, bool bAppend = TRUE, long lTruncate = MAX_BUF);  // Log File Exchange Function 
	void OnCloseFile();  // Log File Close Function 
	void OnSetPath(LPCTSTR pzPath);  // Log Creating Path Setting Function 

	// Write Log info into the Log File 
	// with printf like parameters support 
	void OnWrite(LPCTSTR pszFormat, ...)
	{
		if ( !mpLogFile )
		{
			return;
		}

		EnterCriticalSection(&mcsLogSync);

		/************************************************************/
		/*                                                          */
		/* Description   : write the formated log string to szLog   */
		/************************************************************/
		//TCHAR  szLog[256];
		TCHAR  szLog[MAX_BUF];
		va_list  argList;

		va_start( argList, pszFormat );
		vsprintf( szLog, pszFormat, argList );
		va_end( argList );


		/************************************************************/
		/*                                                          */
		/* Description   : Get Current Time                         */
		/************************************************************/
		SYSTEMTIME  time;
		::GetLocalTime(&time);
		
		//TCHAR  szLine[256];
		TCHAR  szLine[MAX_BUF];

		sprintf(szLine, "%04d/%02d/%02d %02d:%02d:%02d:%03d \t%s\n",
			time.wYear, time.wMonth, time.wDay,
			time.wHour, time.wMinute, time.wSecond, time.wMilliseconds,
			szLog);

		fputs(szLine, mpLogFile);
		fflush(mpLogFile);

		LeaveCriticalSection(&mcsLogSync);
	}


private:
	FILE*  mpLogFile;

	CRITICAL_SECTION  mcsLogSync;

	long mlTruncate;
	CString mstrPath;
	CString	mstrFileName;
};

#endif //_ATA_LOGFILE_
