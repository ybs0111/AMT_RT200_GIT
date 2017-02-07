// WorkTab.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Handler.h"
#include "WorkTab.h"
#include "WorkMaintenance.h"
#include "WorkMotorSpeed.h"
#include "WorkRecipe.h"
#include "WorkWaitTime.h"
#include "WorkInterface.h"
// CWorkTab

IMPLEMENT_DYNAMIC(CWorkTab, CTabCtrl)

CWorkTab::CWorkTab()
{
	m_nViewMode			= -1;

	m_pMaintenance		= NULL;
	m_pMotorSpeed		= NULL;
	m_pRecipe			= NULL;
	m_pWaitTime			= NULL;
	m_pInterface		= NULL;
	m_pRfid				= NULL;
	m_pFtp				= NULL;
	m_pCok				= NULL;
	m_pPartNo			= NULL;
	m_pManualFunc		= NULL;
	m_pTsiteOffset		= NULL;
//	m_pCustomIoFunc		= NULL;
}

CWorkTab::~CWorkTab()
{
	if(m_pMaintenance != NULL)
	{
		m_pMaintenance->DestroyWindow();
		delete m_pMaintenance;
		m_pMaintenance = NULL;
	}
	
	if(m_pMotorSpeed != NULL)
	{
		m_pMotorSpeed->DestroyWindow();
		delete m_pMotorSpeed;
		m_pMotorSpeed = NULL;
	}
	
	if(m_pRecipe != NULL)
	{
		m_pRecipe->DestroyWindow();
		delete m_pRecipe;
		m_pRecipe = NULL;
	}
	
	if(m_pWaitTime != NULL)
	{
		m_pWaitTime->DestroyWindow();
		delete m_pWaitTime;
		m_pWaitTime = NULL;
	}

	if(m_pInterface != NULL)
	{
		m_pInterface->DestroyWindow();
		delete m_pInterface;
		m_pInterface = NULL;
	}
}


BEGIN_MESSAGE_MAP(CWorkTab, CTabCtrl)
END_MESSAGE_MAP()



// CWorkTab 메시지 처리기입니다.
void CWorkTab::OnInitTab(CWnd *pWnd, CRect rectArea)
{
	m_rectArea = rectArea;

	m_pMaintenance		= new CWorkMaintenance;
	m_pMaintenance->Create(IDD_WORK_MAINTENANCE, this);

	m_pMotorSpeed		= new CWorkMotorSpeed;
	m_pMotorSpeed->Create(IDD_WORK_SPEED, this);

	m_pRecipe			= new CWorkRecipe;
	m_pRecipe->Create(IDD_WORK_RECIPE, this);

	m_pWaitTime			= new CWorkWaitTime;
	m_pWaitTime->Create(IDD_WORK_WAIT, this);

	m_pInterface		= new CWorkInterface;
	m_pInterface->Create(IDD_WORK_INTERFACE, this);

	OnViewChange(0);
}

void CWorkTab::OnViewChange(int nView)
{
	if (nView == m_nViewMode) return;

	m_nViewMode = nView;

	m_pMaintenance->ShowWindow(SW_HIDE);
	m_pMotorSpeed->ShowWindow(SW_HIDE);
	m_pRecipe->ShowWindow(SW_HIDE);
	m_pWaitTime->ShowWindow(SW_HIDE);
	m_pInterface->ShowWindow(SW_HIDE);

	switch (nView)
	{
		case 0:
			m_pMaintenance->ShowWindow(SW_SHOW);
			break;

		case 1:
			m_pInterface->ShowWindow(SW_SHOW);
			break;

		case 2:
			m_pWaitTime->ShowWindow(SW_SHOW);
			break;

		case 3:
			m_pMotorSpeed->ShowWindow(SW_SHOW);
			break;

		case 4:
			m_pRecipe->ShowWindow(SW_SHOW);
			break;

		case 5:
		
			break;

		case 6:
		
			break;

		case 7:
		
			break;

		case 8:
			
			break;

		case 9:
		
			break;

		case 10:
		
			break;

		case 11:

			break;
	}
}

