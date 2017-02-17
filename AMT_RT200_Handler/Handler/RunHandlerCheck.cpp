#include "StdAfx.h"
#include "RunHandlerCheck.h"
#include "PublicFunction.h"
#include "Variable.h"
#include "FastechPublic_IO.h"
#include "ComizoaPublic.h"
#include "Ctlbd_Variable.h"
#include "math.h"
#include "CtlBd_Library.h"
#include "XgemClient.h"
#include "AlgMemory.h"

CRunHandlerCheck clsHdChk;

CRunHandlerCheck::CRunHandlerCheck(void)
{
	m_nStartStep			= 0;
	m_nStopStep				= 0;
	m_nResetStep			= 0;
	m_nBuzzerStep			= 0;
	m_nAutoStep				= 0;
	m_nManualStep			= 0;
	m_nOldHifixMode			= -1;
}


CRunHandlerCheck::~CRunHandlerCheck(void)
{
}

void CRunHandlerCheck::OnStartCheck()
{
	//int nRet;
//	int nNum;

	CString mstr_temp; //2015.0407 james 

	if (st_handler_info.nSystemLock != NO)	
	{
		return;
	}

	if (st_handler_info.nMenuLock != NO)	
	{
		return;
	}

	if (st_handler_info.nSwitchLock != NO)	
	{
		return;
	}

	if (st_handler_info.nRunStatus == dRUN)	
	{
		return;
	}

	if (st_handler_info.nInitialSuccess != YES)	
	{
		return;
	}


	switch(m_nStartStep)
	{
		case 0:
			// [STOP] ���¿����� [START] �����ϵ��� �����Ѵ�
			if (st_handler_info.nRunStatus == dSTOP)
			{
				int nRet1,nRet2;
				nRet1 = FAS_IO.get_in_bit(st_io_info.i_StartChk, IO_OFF);

				//if (FAS_IO.get_in_bit(st_io_info.i_StartChk, IO_OFF) == IO_ON)
				if (nRet1 == IO_ON)
				{
					nRet2 = FAS_IO.get_in_bit(st_io_info.i_StopChk, IO_OFF);
					// ���� STOP ��ư�� �Բ� ���� ��쿡�� �����Ѵ�
					//if (FAS_IO.get_in_bit(st_io_info.i_StopChk, IO_OFF) == IO_ON)
					if (nRet2 == IO_ON)
					{
						break;
					}
					
					if (st_handler_info.nMenuNum != IDW_SCREEN_MAIN)	
					{
						if (st_handler_info.cWndList != NULL)  
						{
							clsMem.OnAbNormalMessagWrite(_T("Please Check Screen Main Status."));
							st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG); 
						}

						break;
					}

					m_dwStartWaitTime[0] = GetCurrentTime();
					m_nStartStep = 100;
				}
			}
			break;

		case 100: 
			if (FAS_IO.get_in_bit(st_io_info.i_StartChk, IO_ON) == IO_ON)
			{
				m_dwStartWaitTime[1] = GetCurrentTime();
				m_dwStartWaitTime[2] = m_dwStartWaitTime[1] - m_dwStartWaitTime[0];

				if (m_dwStartWaitTime[2] <= 0)
				{
					m_dwStartWaitTime[0] = GetCurrentTime();
				}
				else if (m_dwStartWaitTime[2] > (DWORD)100)
				{
					m_dwStartWaitTime[0] = GetCurrentTime();
					m_nStartStep = 200;
					
				}
			}
			else
			{
				m_nStartStep = 0;
			}
			break;

	case 200:
			// =============================================================================
			// RUN Ű I/O �Է��� Off �Ǿ����� �˻�                                   
			// -> �˶� ȭ�� ��� �����̸� �˶� ȭ�� ���� -> ���� ȭ������ ��ȯ       
			// =============================================================================

		m_dwStartWaitTime[1] = GetCurrentTime();
		m_dwStartWaitTime[2] = m_dwStartWaitTime[1] - m_dwStartWaitTime[0];

		if(m_dwStartWaitTime[2] > 100)
		{
			if(FAS_IO.get_in_bit(st_io_info.i_StartChk, IO_OFF) == IO_OFF)
			{
				m_nStartStep = 300;
			}
// 			else
// 			{
// 				m_nStartStep = 0;
// 			}
		}//2012,1220
		else if(m_dwStartWaitTime[2] < 0)
		{
			m_dwStartWaitTime[0] = GetCurrentTime();
		}

		if(m_dwStartWaitTime[2] > 3000)
		{
			m_nStartStep = 0;
		}
		break;

		case 300:
			if(OnRunPossibleCheck() == RET_GOOD)
			{
				// �˶��� ��µ� ���¿��� START ��ư ���� ��쿡�� �˶��� ���� �����Ų��
				if (st_handler_info.nMenuNum != IDW_SCREEN_MAIN)	
				{
					::SendMessage(st_handler_info.hWnd, WM_FORM_CHANGE, SCREEN_MAIN, 0);  // ���� ȭ������ ��ȯ ��û
				}

				m_nStartStep = 400;
			}
			else
			{
				st_handler_info.nRunStatus = dWARNING;

				m_nStartStep = 0;
			}
			break;


		case 400:
			clsFunc.OnMCRun();

			if (st_handler_info.cWndMain != NULL)
			{
//				st_handler_info.cWndMain->SendMessage(WM_WORK_COMMAND, MAIN_RECOVERY_CREATE_REQ, 0);
			}

			m_nStartStep = 0;
			break;

			/*
		case 500:
			// 20140812 jtkim
			if (st_basic_info.nModeXgemRunChk == YES && st_basic_info.nModeXgem == YES)
			{
				if (clsXgem.m_strOldRecipe != st_basic_info.strDeviceName)
				{
					m_dwStartWaitTime[0] = GetCurrentTime();

					clsXgem.m_nRunStatus = HOST_WAIT;

					clsXgem.OnMcRecipeCreate(0, st_basic_info.strDeviceName, st_path_info.strPathRecipe);

					m_nStartStep = 600;
				}
				else
				{
					clsXgem.OnMcRecipeSelete(st_basic_info.strDeviceName);

					m_nStartStep = 1200;
				}
//				clsXgem.OnMcRecipeSelete(st_basic_info.strDeviceName);
			}
			else
			{
				clsXgem.OnMcRecipeSelete(st_basic_info.strDeviceName);

				m_nStartStep = 1200;
			}
			break;

		case 600:
			// jtkim 20150418
			if (clsXgem.m_nRunStatus == HOST_RUN)
			{
				m_nStartStep = 1200;
			}
			else if (clsXgem.m_nRunStatus == HOST_STOP)
			{
				st_other_info.nBuzYesNo = YES;
				st_other_info.strBoxMsg = _T("XGem RCMD Stop. \r\n RCMD Check");

				if (st_handler_info.cWndMain != NULL)
				{
					st_handler_info.cWndMain->SendMessage(WM_WORK_COMMAND, MAIN_MESSAGE_BOX_CREATE_REQ, 0);
				}

				// jtkim 20130625
				clsFunc.OnMCStop();

				m_nStartStep = 0;
			}
			else
			{
				m_dwStartWaitTime[1] = GetCurrentTime();
				m_dwStartWaitTime[2] = m_dwStartWaitTime[1] - m_dwStartWaitTime[0];

				if (m_dwStartWaitTime[2] <= (DWORD)0)
				{
					m_dwStartWaitTime[0] = GetCurrentTime();
					break;
				}

				if (m_dwStartWaitTime[2] > (DWORD)10000)
				{
					if (st_handler_info.cWndList != NULL)	
					{
						//clsFunc.OnStringToChar(_T("[XGEM] Start Check Error"), st_other_info.cNormalMsg);
						clsMem.OnAbNormalMessagWrite(_T("[XGEM] RCMD Start Check Error"));
						st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
					}

					st_other_info.nBuzYesNo = YES;
					st_other_info.strBoxMsg = _T("XGem RCMD TimeOut. \r\n RCMD Check");

					if (st_handler_info.cWndMain != NULL)
					{
						st_handler_info.cWndMain->SendMessage(WM_WORK_COMMAND, MAIN_MESSAGE_BOX_CREATE_REQ, 0);
					}
					// jtkim 20130625
					clsFunc.OnMCStop();

					m_nStartStep = 0;
				}
			}
			break;

		case 1000:
			st_handler_info.nRunStatus	= dRECOVERY;
			
			m_nStartStep = 1100;
			break;
			
		case 1100:
			if (st_recovery_info.nRecoveryFlag[PART_MGZ_ELV]		== NO && st_recovery_info.nRecoveryFlag[PART_PCB_SHUTTLE]	== NO &&
				st_recovery_info.nRecoveryFlag[PART_PCB_ROBOT]		== NO && st_recovery_info.nRecoveryFlag[PART_JIG_ROBOT]		== NO &&
				st_recovery_info.nRecoveryFlag[PART_JIG_ELV]		== NO && st_recovery_info.nRecoveryFlag[PART_JIG_MOVE_CVY]	== NO &&
				st_recovery_info.nRecoveryFlag[PART_JIG_SPRT_CVY]	== NO && st_recovery_info.nRecoveryFlag[PART_MGZ_IN_CVY]	== NO &&
				st_recovery_info.nRecoveryFlag[PART_MGZ_OUT_CVY]	== NO)
			{
//				clsFunc.OnRecoveryConveyor();

				if (st_handler_info.cWndMain != NULL)
				{
//					st_handler_info.cWndMain->SendMessage(WM_WORK_COMMAND, MAIN_RECOVERY_DELETE_REQ, 0);
				}

				m_nStartStep = 1200;
			}
			else
			{
				if (st_handler_info.nRunStatus == dSTOP)
				{
					if (st_handler_info.cWndMain != NULL)
					{
//						st_handler_info.cWndMain->SendMessage(WM_WORK_COMMAND, MAIN_RECOVERY_DELETE_REQ, 0);
					}
					m_nStartStep = 0;
				}
			}
			break;

		case 1200:
			st_handler_info.nRunStatus = dRUN;

			if (st_work_info.nBdReadyChk == YES)
			{
				st_work_info.nBdReadyChk = NO;
			}

			if (st_handler_info.nMenuLock != FALSE)
			{
				st_handler_info.nMenuLock = FALSE;
			}

			if (st_handler_info.nAlarmScreen )
			{
				st_handler_info.nAlarmScreen = FALSE;
			}

			// 20140812 jtkim
			clsXgem.OnMcStatus(dRUN);

			if (CTL_Lib.m_nMcEmg == YES)
			{
				CTL_Lib.m_nMcEmg = NO;
			}

			if(st_handler_info.cWndTitle != NULL)
			{
				st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, MACHINE_STATUS, st_handler_info.nRunStatus);
			}

			if (st_handler_info.cWndList != NULL)  // ����Ʈ �� ȭ�� ����
			{
				clsMem.OnNormalMessageWrite(_T("MC Start Status...."));
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
			m_nStartStep = 0;

			st_handler_info.mn_userstop = FALSE; //2015.1005

			break;
			*/
	}
	
}


void CRunHandlerCheck::OnStopCheck()
{
	if (st_handler_info.nSystemLock != NO)	
	{
		return;
	}

	if (st_handler_info.nSwitchLock != NO)	
	{
		return;
	}

	if (st_handler_info.nRunStatus == dSTOP)	
	{
		return;
	}

	switch(m_nStopStep)
	{
		case 0:
			if(FAS_IO.get_in_bit(st_io_info.i_StopChk, IO_OFF) == IO_ON)
			{
				if (FAS_IO.get_in_bit(st_io_info.i_StartChk, IO_ON) == IO_ON)
				{
					break;
				}

				m_dwStopWaitTime[0] = GetCurrentTime();
				m_nStopStep = 100;
			}
			break;

		case 100:
			if(FAS_IO.get_in_bit(st_io_info.i_StopChk, IO_OFF) == IO_ON)
			{
				m_dwStopWaitTime[1] = GetCurrentTime();
				m_dwStopWaitTime[2] = m_dwStopWaitTime[1] - m_dwStopWaitTime[0];

				if(m_dwStopWaitTime[2] <= 0)
				{
					m_dwStopWaitTime[0] = GetCurrentTime();
					break;
				}

				if(m_dwStopWaitTime[2] > (DWORD)100)
				{
					m_dwStopWaitTime[0] = GetCurrentTime();
					m_nStopStep = 200;
				}
			}
			else
			{
				m_nStopStep = 0;
			}
			break;

		case 200:
			if(FAS_IO.get_in_bit(st_io_info.i_StopChk, IO_ON) == IO_OFF)
			{
				// �ʱ�ȭ �۾��߿� STOP ��ư�� ������ �ʱ�ȭ �۾��� �ߴ��Ѵ�
				if (st_handler_info.nRunStatus == dINIT)
				{
					m_nStopStep = 0;
					break;
				}
				// jtkim 20130625
				clsFunc.OnMCStop();

				// 20140812 jtkim
			//	clsXgem.OnMcStatus(dSTOP);

				// jtkim 20150721
				if (st_work_info.nCycleYesNo == YES)
				{
					st_work_info.nCycleYesNo = NO;
				}

				if(st_handler_info.cWndTitle != NULL)
				{
					st_handler_info.cWndTitle->PostMessage(WM_STATUS_CHANGE, MACHINE_STATUS, st_handler_info.nRunStatus);
				}

				if (st_handler_info.cWndList != NULL)  // ����Ʈ �� ȭ�� ����
				{
					clsMem.OnNormalMessageWrite(_T("MC Stop Status...."));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
				}
				m_nStopStep = 0;

				st_handler_info.mn_userstop = CTL_YES;//2015.1005

			}
			break;
	}
}


void CRunHandlerCheck::OnResetCheck()
{
	if (st_handler_info.nSystemLock != NO)	
	{
		return;
	}

	if (st_handler_info.nSwitchLock != NO)	
	{
		return;
	}

	switch(m_nResetStep)
	{
		case 0:
			// **********************************************************************
			//  Reset Ű I/O �Է��� On �������� Ȯ��                                 
			// **********************************************************************
			if(FAS_IO.get_in_bit(st_io_info.i_ResetChk, IO_OFF) == IO_ON)
			{
				m_dwResetWaitTime[0] = GetCurrentTime();
				m_nResetStep = 100;
			}
			break;

		case 100: 
			if(FAS_IO.get_in_bit(st_io_info.i_ResetChk, IO_OFF) == IO_ON)
			{
				m_dwResetWaitTime[1] = GetCurrentTime();
				m_dwResetWaitTime[2] = m_dwResetWaitTime[1] - m_dwResetWaitTime[0];

				if(m_dwResetWaitTime[2] <= 0)
				{
					m_dwResetWaitTime[0] = GetCurrentTime();
					break;
				}

				if(m_dwResetWaitTime[2] > (DWORD)100)
				{
					m_nResetStep = 200;
				}
			}
			else
			{
				m_nResetStep = 0;
			}
			break;

		case 200:
			if (FAS_IO.get_in_bit(st_io_info.i_ResetChk, IO_ON) == IO_OFF)
			{
				st_alarm_info.nResetStatus = NO;

				if (st_alarm_info.nResetStatus == YES)
				{
					st_alarm_info.nResetStatus = NO;  // �˶� ���� �۾� �ʿ� ����
					
					if (st_handler_info.cWndList != NULL)	
					{
						//wsprintfA(st_other_info.cNormalMsg, "%S", _T("[JAM RESET] JAM RESET."));
						//clsFunc.OnStringToChar(_T("[JAM RESET] JAM RESET."), st_other_info.cNormalMsg);
						clsMem.OnNormalMessageWrite(_T("[JAM RESET] JAM RESET."));
						st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
					}

					// ���� ���°� RUN �����̸� ���常 �����ϰ� RUN ���� �ƴϸ� [STOP] ���·� �����Ѵ�
					// jtkim 20130625
					clsFunc.OnMCStop();  // ���� STOP ���¿� ���� ��ư ���� ���� ����

					// 20140812 jtkim
				//	clsXgem.OnMcStatus(dSTOP);

					clsFunc.OnSound(IO_OFF);
				}
				else
				{
					if (st_handler_info.cWndList != NULL)	
					{
						//wsprintfA(st_other_info.cNormalMsg, "%S", _T("[SOUND] OFF."));
						//clsFunc.OnStringToChar(_T("[SOUND] OFF."), st_other_info.cNormalMsg);
						clsMem.OnNormalMessageWrite(_T("[SOUND] OFF."));
						st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
					}
					// jtkim 20130625
					clsFunc.OnSound(IO_OFF);  // ���� ���� ON/OFF
				}

				m_nResetStep = 0;
			}
			break;
	}
}


void CRunHandlerCheck::OnBuzzerCheck()
{
	if (st_handler_info.nSystemLock != NO)	
	{
		return;
	}

	if (st_handler_info.nSwitchLock != NO)	
	{
		return;
	}

	switch(m_nBuzzerStep)
	{
		case 0:
			if(FAS_IO.get_in_bit(st_io_info.i_BuzzOffChk, IO_OFF) == IO_ON)
			{
				m_dwBuzzerWaitTime[0] = GetCurrentTime();
				m_nBuzzerStep = 100;
			}
			break;
			
		case 100: 
			if(FAS_IO.get_in_bit(st_io_info.i_BuzzOffChk, IO_ON) == IO_ON)
			{
				m_dwBuzzerWaitTime[1] = GetCurrentTime();
				m_dwBuzzerWaitTime[2] = m_dwBuzzerWaitTime[1] - m_dwBuzzerWaitTime[0];

				if(m_dwBuzzerWaitTime[2] <= 0)
				{
					m_dwBuzzerWaitTime[0] = GetCurrentTime();
					break;
				}

				if(m_dwBuzzerWaitTime[2] > (DWORD)100)
				{
					m_nBuzzerStep = 200;
				}
			}
			else
			{
				m_nBuzzerStep = 0;
			}
			break;
			
		case 200:
			if (FAS_IO.get_in_bit(st_io_info.i_BuzzOffChk, IO_OFF) == IO_OFF)
			{
				if (st_handler_info.cWndList != NULL)	
				{
					//wsprintfA(st_other_info.cNormalMsg, "%S", _T("[SOUND] OFF."));
					//clsFunc.OnStringToChar(_T("[SOUND] OFF."), st_other_info.cNormalMsg);
					clsMem.OnNormalMessageWrite(_T("[SOUND] OFF."));
					st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
				}
				// jtkim 20130625
				clsFunc.OnSound(IO_OFF);  // ���� ���� ON/OFF
				
				m_nBuzzerStep = 0;
			}
			break;
	}
}


void CRunHandlerCheck::OnAutoCheck()
{
	int nRet;

	if (st_handler_info.nSystemLock != NO)	
	{
		return;
	}

	if (st_handler_info.nSwitchLock != NO)	
	{
		return;
	}

	if(st_handler_info.nRunStatus	== dRUN || st_handler_info.nRunStatus	== dINIT)
	{
		return;
	}

	if(st_handler_info.nMachineMode	== MACHINE_AUTO)
	{
		return;
	}

	switch(m_nAutoStep)
	{
		case 0:		//2014.01.21 �ּ�ó��
// 			if(FAS_IO.get_in_bit(st_io_info.i_FrontSelectSwChk, IO_OFF) == IO_ON &&
// 			   FAS_IO.get_in_bit(st_io_info.i_RearSelectSwChk1, IO_OFF)	== IO_ON &&
// 			   FAS_IO.get_in_bit(st_io_info.i_RearSelectSwChk2, IO_OFF)	== IO_ON)
			if (FAS_IO.get_in_bit(st_io_info.i_AutoChk, IO_OFF) == IO_ON)
			{
				//if (FAS_IO.get_in_bit(st_io_info.i_AutoModeChk, IO_OFF) == IO_ON)
				{
					m_dwAutoWaitTime[0] = GetTickCount();
					m_nAutoStep = 100;
				}
			}
			break;

		case 100:	//2014.01.21 �ּ�ó��
// 			if(FAS_IO.get_in_bit(st_io_info.i_FrontSelectSwChk, IO_OFF) == IO_ON &&
// 			   FAS_IO.get_in_bit(st_io_info.i_RearSelectSwChk1, IO_OFF)	== IO_ON &&
// 			   FAS_IO.get_in_bit(st_io_info.i_RearSelectSwChk2, IO_OFF)	== IO_ON)
 			if (FAS_IO.get_in_bit(st_io_info.i_AutoChk, IO_OFF) == IO_ON)
			{
				m_dwAutoWaitTime[1] = GetTickCount();
				m_dwAutoWaitTime[2] = m_dwAutoWaitTime[1] - m_dwAutoWaitTime[0];

				if(m_dwAutoWaitTime[2] <= 0)
				{
					m_dwAutoWaitTime[0] = GetTickCount();
					break;
				}

				if(m_dwAutoWaitTime[2] > (DWORD)200)
				{
					m_nAutoStep = 200;
				}
			}
			else
			{
				m_nAutoStep = 0;
			}
			break;

		case 200:
			nRet = clsFunc.OnDoorOpenCheck();

			if(nRet == RET_GOOD)
			{
				m_nAutoStep = 400;
			}
			else if(nRet != RET_PROCEED)
			{
				st_handler_info.nRunStatus			= dWARNING;
				st_alarm_info.nAlarmAssignSection	= 6000;

				m_nAutoStep = 300;
			}
			break;

		case 300:
			nRet = clsFunc.OnDoorOpenCheck();

			if(nRet == RET_GOOD)
			{
				m_nAutoStep = 0;
			}
			break;

		case 400:
			m_dwAutoWaitTime[0] = GetTickCount();
			m_nAutoStep = 500;
			break;

		case 500:
			m_dwAutoWaitTime[1] = GetTickCount();
			m_dwAutoWaitTime[2] = m_dwAutoWaitTime[1] - m_dwAutoWaitTime[0];
			
			if(m_dwAutoWaitTime[2] <= 0)
			{
				m_dwAutoWaitTime[0] = GetTickCount();
				break;
			}

			if(m_dwAutoWaitTime[2] > (DWORD)500)
			{
				m_nAutoStep = 600;
			}
			break;

		case 600:
			// jtkim 20130625
			clsFunc.OnDoorClose();

			st_handler_info.nMachineMode = MACHINE_AUTO;
			m_nAutoStep = 0;
			break;
	}
}


void CRunHandlerCheck::OnManualCheck()
{
	if (st_handler_info.nSystemLock != NO)	
	{
		return;
	}

	if (st_handler_info.nSwitchLock != NO)	
	{
		return;
	}

//	if(st_handler_info.nRunStatus	!= dSTOP)
	if(st_handler_info.nRunStatus	== dRUN || st_handler_info.nRunStatus	== dINIT)
	{
		return;
	}

	if(st_handler_info.nMachineMode	== MACHINE_MANUAL)
	{
		return;
	}

	switch(m_nManualStep)
	{
		case 0:		//2014.01.21 �ּ�ó��
			if(FAS_IO.get_in_bit(st_io_info.i_AutoChk, IO_ON) == IO_OFF)
// 			   FAS_IO.get_in_bit(st_io_info.i_RearSelectSwChk1, IO_ON)  == IO_OFF||
// 			   FAS_IO.get_in_bit(st_io_info.i_RearSelectSwChk2, IO_ON)  == IO_OFF)
			{
				m_dwManualWaitTime[0] = GetTickCount();
				m_nManualStep = 100;
			}

			break;

		case 100:	//2014.01.21 �ּ�ó��
// 			if(FAS_IO.get_in_bit(st_io_info.i_FrontSelectSwChk, IO_ON) == IO_OFF ||
// 			   FAS_IO.get_in_bit(st_io_info.i_RearSelectSwChk1, IO_ON)  == IO_OFF||
// 			   FAS_IO.get_in_bit(st_io_info.i_RearSelectSwChk2, IO_ON)  == IO_OFF)
			if(FAS_IO.get_in_bit(st_io_info.i_AutoChk, IO_ON) == IO_OFF)
			{
				m_dwManualWaitTime[1] = GetTickCount();
				m_dwManualWaitTime[2] = m_dwManualWaitTime[1] - m_dwManualWaitTime[0];

				if(m_dwManualWaitTime[2] <= 0)
				{
					m_dwManualWaitTime[0] = GetTickCount();
					break;
				}

				if(m_dwManualWaitTime[2] > (DWORD)200)
				{
					m_nManualStep = 200;
				}
			}
			else
			{
				m_nManualStep = 0;
			}
			break;

		case 200:
			m_dwManualWaitTime[0] = GetTickCount();
			m_nManualStep = 300;
			break;

		case 300:
			m_dwManualWaitTime[1] = GetTickCount();
			m_dwManualWaitTime[2] = m_dwManualWaitTime[1] - m_dwManualWaitTime[0];
			
			if(m_dwManualWaitTime[2] <= 0)
			{
				m_dwManualWaitTime[0] = GetTickCount();
				break;
			}

			if(m_dwManualWaitTime[2] > (DWORD)500)
			{
				m_nManualStep = 400;
			}
			break;

		case 400:
			st_handler_info.nMachineMode = MACHINE_MANUAL;

			// �Ŵ��� ���� ����Ǹ� ������ STOP ���·� �����Ѵ�
			// jtkim 20130625
			clsFunc.OnMCStop();  // ���� STOP ���¿� ���� ��ư ���� ���� ����
			//clsFunc.OnDoorOpen();  // SAFETY DOOR LOCK ����
			m_nManualStep = 0;
			break;
	}
}


int CRunHandlerCheck::OnRunPossibleCheck()
{
	//int i;
	CString strAlram;

	if (clsFunc.OnDoorOpenCheck() == RET_ERROR)
	{
		return RET_ERROR;
	}

	
	if (FAS_IO.m_nIoBoardInitialFlag == NO )
	{

		CTL_Lib.Alarm_Error_Occurrence(333, dWARNING, _T("900001"));

		if (st_handler_info.cWndList != NULL) 
		{
			clsMem.OnAbNormalMessagWrite(_T("I/O Board Initialized Fail..."));
			st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
		return RET_ERROR;
	}
					
	if (COMI.mn_motorbd_init_end == NO)
	{
		CTL_Lib.Alarm_Error_Occurrence(334, dWARNING, _T("900002"));
						
		if (st_handler_info.cWndList != NULL)  // ����Ʈ �� ȭ�� ����
		{
			clsMem.OnAbNormalMessagWrite(_T("Motor Board Initialized Fail..."));
			st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}	

		return RET_ERROR;
	}

	if (FAS_IO.get_in_bit(st_io_info.i_AutoChk, IO_ON) == IO_OFF)
	{
		//st_other_info.nBuzYesNo = NO;
		st_other_info.strBoxMsg = _T("Please Check Auto Key Status.");

		if (st_handler_info.cWndMain != NULL)
		{
			st_handler_info.cWndMain->SendMessage(WM_WORK_COMMAND, MAIN_MESSAGE_BOX_CREATE_REQ, 0);
		}
	}

	if (FAS_IO.get_in_bit(st_io_info.i_EmoSwChk, IO_ON) == IO_OFF)
	{
		//st_other_info.nBuzYesNo = NO;
		st_other_info.strBoxMsg = _T("Please Check EMO Status.");

		if (st_handler_info.cWndMain != NULL)
		{
			st_handler_info.cWndMain->SendMessage(WM_WORK_COMMAND, MAIN_MESSAGE_BOX_CREATE_REQ, 0);
		}
	}

	if (FAS_IO.get_in_bit(st_io_info.i_AirDownSwChk, IO_ON) == IO_OFF)
	{
		//st_other_info.nBuzYesNo = NO;
		st_other_info.strBoxMsg = _T("Please Air Status Check.");

		if (st_handler_info.cWndMain != NULL)
		{
			st_handler_info.cWndMain->SendMessage(WM_WORK_COMMAND, MAIN_MESSAGE_BOX_CREATE_REQ, 0);
		}
	}
	
	return RET_GOOD;
}
// 
// 
void CRunHandlerCheck::OnLightCurtainCheck()
{
	int nRet;
	

	if (st_handler_info.nRunStatus == dSTOP) return;

	nRet = clsFunc.OnLightCurtainCheck();
}


