#pragma once
class CXgemClient
{
public:
	CString				m_strOldRecipe; // jtkim 20150418
	CString				m_strHeader;
	CString				m_strData;

	int					m_nXgemInterface;
	int					m_nXgemReq;
	int					m_nRunStatus;
	int					m_nHeader;
	int					m_nHeaderLength;

	void				OnDataReceive(CString strMsg);
	void				OnDataAnalysis(CString strMsg);
	CString				OnDataItemAnalysis(CString strMsg, CString strFind);
	void				OnXgemInitialize(CString strPath);
	void				OnMcRecipeCreate(int nMode, CString strRecipe, CString strPath);
	void				OnMcRecipeSelete(CString strRecipe);
	void				OnMcStatus(int nStatus);
	void				OnMcLotStart(CString strLotNo, CString strPartNo);
	void				OnMcLotEnd(CString strLotNo, CString strPartNo, int nTotal, int nPass, int nFail, COleDateTimeSpan time);
	void				OnMcAlarm(CString strCode, int nStatus);
	void				OnMcPCBInfo(CString strBarcode);
	void				OnMcInterface(int nMode);
//	void				OnXgemRecipe(int nMode, CString strPath, CString strModel); //lkw
public:
	CXgemClient(void);
	~CXgemClient(void);
};

extern CXgemClient clsXgem;

