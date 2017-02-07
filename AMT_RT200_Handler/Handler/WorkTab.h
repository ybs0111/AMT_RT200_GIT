#pragma once


// CWorkTab
class CWorkMaintenance;
class CWorkMotorSpeed;
class CWorkRecipe;
class CWorkWaitTime;
class CWorkInterface;
class CWorkRfid;
class CWorkFtp;
class CWorkCok;
class CWorkPartNo;
class CWorkManualFunction;
class CWorkTestSiteOffset;
class CWorkCustomIoFunction;

class CWorkTab : public CTabCtrl
{
	DECLARE_DYNAMIC(CWorkTab)
public:
	int						m_nViewMode;
	CWorkMaintenance		*m_pMaintenance;
	CWorkMotorSpeed			*m_pMotorSpeed;
	CWorkRecipe				*m_pRecipe;
	CWorkWaitTime			*m_pWaitTime;
	CWorkInterface			*m_pInterface;
	CWorkRfid				*m_pRfid;
	CWorkFtp				*m_pFtp;
	CWorkCok				*m_pCok;
	CWorkPartNo				*m_pPartNo;
	CWorkManualFunction		*m_pManualFunc;
	CWorkTestSiteOffset		*m_pTsiteOffset;
	CWorkCustomIoFunction	*m_pCustomIoFunc;

	CRect					m_rectArea;
	void					OnInitTab(CWnd *pWnd, CRect rectArea);
	void					OnViewChange(int nView);
public:
	CWorkTab();
	virtual ~CWorkTab();

protected:
	DECLARE_MESSAGE_MAP()
};


