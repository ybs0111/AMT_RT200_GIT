#include "StdAfx.h"
#include "FtpComm.h"
#include "Variable.h"
#include "PublicFunction.h"
//CFtpComm clsFtp;

CFtpComm::CFtpComm(void)
{
}


CFtpComm::~CFtpComm(void)
{
}

 BOOL CFtpComm::OnRecipeDownload(CString strPath)
 {
// 	CString strData;
// 	CString strFtpName	= _T("D:\\AMT8562\\");
// 	CString strFilename = _T("RECIPE_RULE.TXT");
// 
// 	strFtpName += strFilename;
// 	// jtkim 20150929
//     DeleteFile(strFtpName);
// 	
// 	if (m_pFtp.Open(st_client_info[FTP_NETWORK].strIp, 
// 					st_basic_info.strFtpUser,
// 					st_basic_info.strFtpPass,
// 					st_client_info[FTP_NETWORK].nPort))
// 	{ 
// 		strData.Format(_T("%s/%s"), strPath, strFilename);
// 
// 		m_pFtp.DownLoad(strData, strFtpName);
// 
// 		m_pFtp.Close();
// 
// 		return TRUE;
// 	}
// 
// 	m_pFtp.Close();
// 
 	return FALSE;
 }

 BOOL CFtpComm::OnScrapCodeDownload(int nMode, CString strPath, CString strFilename)
 {
// 	CString strData;
// 	CString strFtpName = _T("D:\\AMT8562\\");
// 
// 	int i, j;
// 
// 	strFtpName += strFilename;
// 	DeleteFile(strFtpName);
// 
// 	st_code_info[nMode].m_nScrapBin = BD_NONE;
// 
// 	for (i=0; i<10000; i++)
// 	{
// 		for (j=0; j<2; j++)
// 		{
// 			st_code_info[nMode].m_nScrapCode[i][j] = st_recipe_info.nTestRetest_Count;
// 		}
// 	}
// 	
// 	if (m_pFtp.Open(st_client_info[FTP_NETWORK].strIp, 
// 					st_basic_info.strFtpUser,
// 					st_basic_info.strFtpPass,
// 					st_client_info[FTP_NETWORK].nPort))
// 	{ 
// 		strData.Format(_T("%s/%s"), strPath, strFilename);
// 
// 		m_pFtp.DownLoad(strData, strFtpName);
// 
// 		m_pFtp.Close();
// 		st_code_info[nMode].m_nScrapBin = BD_DATA_GOOD;
// 
// 		return TRUE;
// 	}
// 
// 	m_pFtp.Close();
// 	st_code_info[nMode].m_nScrapBin = BD_DATA_REJECT;
// 
 	return FALSE;
 }

void CFtpComm::OnScrapCodeFind(int nMode, CString strLoadFile)
{
	//CString strLoadFile = _T("D:\\AMT8562\\RETEST_RULE.TXT");
	CString strDefaultLoadFile = _T("D:\\AMT8562\\File\\RETEST_RULE.TXT");

	TCHAR chData[100];

	int nCount;
	int nCode;
	int nCurr, nNext;
	int i;

	BOOL bFind = TRUE;

	CString strTemp;
	CString strHead;
	CString strData;

	GetPrivateProfileString(_T("SIT-07"), _T("Total"), _T("0"), chData, sizeof(chData), strLoadFile);
	strTemp.Format(_T("%s"), chData);
	nCount = _wtoi(strTemp);

	if (nCount > 0)
	{
		for (i=0; i<nCount; i++)
		{
			strHead.Format(_T("%d"), i+1);
			GetPrivateProfileString(_T("SIT-07"), strHead, _T("0"), chData, sizeof(chData), strLoadFile);
			strData.Format(_T("%s"), chData);
			
			nCode = -1;
			nCurr = -1;
			nNext = -1;

			nCode = _wtoi(strData.Mid(0, 4));
			nCurr = _wtoi(strData.Mid(5, 1));
			nNext = _wtoi(strData.Mid(7, 1));

			if (nCode < 0 || nCurr < 0 || nNext < 0)
			{
				bFind = FALSE;
				break;
			}
			st_code_info[nMode].m_nScrapCode[nCode][0] = nCurr;
			st_code_info[nMode].m_nScrapCode[nCode][1] = nNext;
		}
	}
	else
	{
		GetPrivateProfileString(_T("SIT-07"), _T("Total"), _T("0"), chData, sizeof(chData), strDefaultLoadFile);
		strTemp.Format(_T("%s"), chData);
		nCount = _wtoi(strTemp);

		for (i=0; i<nCount; i++)
		{
			strHead.Format(_T("%d"), i+1);
			GetPrivateProfileString(_T("SIT-07"), strHead, _T("0"), chData, sizeof(chData), strDefaultLoadFile);
			strData.Format(_T("%s"), chData);

			nCode = _wtoi(strData.Mid(0, 4));
			nCurr = _wtoi(strData.Mid(5, 1));
			nNext = _wtoi(strData.Mid(7, 1));

			if (nCode < 0)
			{
				break;
			}
			st_code_info[nMode].m_nScrapCode[nCode][0] = nCurr;
			st_code_info[nMode].m_nScrapCode[nCode][1] = nNext;
		}
	}

	if (bFind == FALSE)
	{
		GetPrivateProfileString(_T("SIT-07"), _T("Total"), _T("0"), chData, sizeof(chData), strDefaultLoadFile);
		strTemp.Format(_T("%s"), chData);
		nCount = _wtoi(strTemp);

		for (i=0; i<nCount; i++)
		{
			strHead.Format(_T("%d"), i+1);
			GetPrivateProfileString(_T("SIT-07"), strHead, _T("0"), chData, sizeof(chData), strDefaultLoadFile);
			strData.Format(_T("%s"), chData);

			nCode = _wtoi(strData.Mid(0, 4));
			nCurr = _wtoi(strData.Mid(5, 1));
			nNext = _wtoi(strData.Mid(7, 1));

			if (nCode < 0)
			{
				break;
			}
			st_code_info[nMode].m_nScrapCode[nCode][0] = nCurr;
			st_code_info[nMode].m_nScrapCode[nCode][1] = nNext;
		}
	}
}