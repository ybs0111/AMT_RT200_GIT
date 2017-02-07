#pragma once

#include "FtpClient.h"

class CFtpComm
{
public:
	CFtpComm(void);
	~CFtpComm(void);

public:
	CFtpClient		m_pFtp;

	BOOL			OnScrapCodeDownload(int nMode, CString strPath, CString strFilename);
	void			OnScrapCodeFind(int nMode, CString strLoadFile);
	BOOL			OnRecipeDownload(CString strPath);
};
