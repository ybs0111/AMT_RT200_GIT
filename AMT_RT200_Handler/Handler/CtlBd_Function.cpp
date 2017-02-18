// CtlBd_Function.cpp: implementation of the CCtlBd_Function class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "CtlBd_Function.h"
#include "Cmmsdk.h"
#include "CmmsdkDef.h"
#include "CtlBd_Variable.h"
#include "ComizoaPublic.h"
#include "FastechPublic_IO.h"
//#include "FAS_HSSI.h"
#include "math.h"
#include "PublicFunction.h"
#include "Variable.h"
#include "AlgMemory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCtlBd_Function  CtlBdFunc;
CCtlBd_Function::CCtlBd_Function()
{
}

CCtlBd_Function::~CCtlBd_Function()
{
//st_ctlbd.n_TotalMotBoard_Number
}

int CCtlBd_Function::Board_Initialize(int n_mode)
{
	int nRet = 0;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//Motor Board Initialize 
	///////////////////////////////////////////////////////////////////////////////////////////////////////
/*	if(CTLBD_MOTBD_TYPE == 0) //0:�Ľ��� ���带 ����ϴ� ����̸�, 1:Ŀ������ ���͸� ����ϴ� ��� 
	{
//		st_ctlbd.n_TotalMotBoard_Number = 3;  //���ͺ��尡 3�� �ִ� 
//		st_ctlbd.n_TotalMotorAxis_Number= 18; //������ �� ������ 18���̴�  
//		st_ctlbd.n_MotorAxis_Number[0] = 8;   //0�� ���ͺ���� 8��� �����̴�  
//		st_ctlbd.n_MotorAxis_Number[1] = 6;   //1�� ���ͺ���� 6��� �����̴� 
//		st_ctlbd.n_MotorAxis_Number[2] = 4;   //2�� ���ͺ���� 4��� �����̴� 
	}
	else if(CTLBD_MOTBD_TYPE == 1) //1:Ŀ������ ���带 ����ϴ� ����̸�, 1:Ŀ������ ���͸� ����ϴ� ��� 
	{


	}*/

	return  RET_GOOD;
}

int CCtlBd_Function::Robot_Clash_Safety_Check(int n_Mode, int n_SourceAxis, int n_ReferenceAxis, double d_Safety_Gap, double d_TargetPos)
{//d_Safety_Gap = 300; //300mm �̻� ������ �־�� �Ѵ� //DC Loading ����Ʈ 

	//n_Mode => 0: ���� �̵��� �����ϰ� üũ�� �ϴ� ����, 1: ���Ͱ� �̵����϶� üũ�ϴ� ���� 
	int nRet = RET_ERROR;

	return nRet;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inposition ���� �� �Ÿ��� �ӵ��� �����Ѵ� 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CCtlBd_Function::MotorSpeed_Control(int n_Axis, double d_CurrentPos, double d_TargetPos, double * dp_Speed)
{
	double dCurrentPos = 0, dTargetPos = 0, dVal =0, d_abs_val = 0, dAccDecTime=0; 


	return RET_GOOD;
}


int CCtlBd_Function::Alarm_Error_Occurrence(int n_Count, int n_Type, int n_Status, CString s_JamCode)
{
	CString strMsg;

     st_alarm_info.nCountMode		= n_Count; 
	 st_alarm_info.nTypeMode		= n_Type; 
	 st_alarm_info.strCode			= s_JamCode; 
	 st_handler_info.nRunStatus		= n_Status; 
	  
//	  MyJamData.On_Alarm_Info_Set_to_Variable(alarm.str_code);
	  
	if (st_handler_info.cWndList != NULL)  // ����Ʈ �� ȭ�� ����
	{
		if(n_Count == MOT_DEBUG)
		{
			strMsg.Format(_T("[MOT SAFETY CHECK]=> [%s] [%s]"), st_alarm_info.strCode, st_alarm_info.strCurrMsg);
			//wsprintfA(st_other_info.cAbnormalMsg, "%S", strMsg);
			
			if (st_handler_info.cWndList != NULL)
			{
				//clsFunc.OnStringToChar(strMsg, st_other_info.cAbnormalMsg);
			
				clsMem.OnAbNormalMessagWrite(strMsg);
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
			}
		}
		else
		{
			strMsg.Format(_T("[%s] [%s]"), st_alarm_info.strCode, st_alarm_info.strCurrMsg);
			//wsprintfA(st_other_info.cAbnormalMsg, "%S", strMsg);
			//clsFunc.OnStringToChar(strMsg, st_other_info.cAbnormalMsg);

			if (st_handler_info.cWndList != NULL)
			{
				clsMem.OnAbNormalMessagWrite(strMsg);
				st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
			}
		}
	}
	  
	return RET_GOOD;
}

int CCtlBd_Function::Send_Error_Message(int n_Mode, int n_Axis, CString s_JamCode, CString s_JamMsg)
{
//	long lErrorCode;
//	short int nErrorParseAxis;
//	short int nErrorParseReason;
	char cErrorMsg[200];
	CString sMsg, sTemp;
	
	memset(&cErrorMsg, 0, sizeof(cErrorMsg));  // ������ ���� ���� �ʱ�ȭ 
/*
	if(n_Mode == MOT_ERR_MSG) 
	{//���� �޼����� �޾� ����Ʈ ȭ�鿡 ���÷��� �Ѵ�   
		if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
		{
			st_other.str_abnormal_msg = s_JamMsg;
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
		}		
	}
	else if(n_Mode == MOT_ERR_CODE)
	{//jamcode  ���� 		
		if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
		{
			st_other.str_abnormal_msg = s_JamMsg;
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
		}	
	}
*/	
/*	if(CTLBD_DEBUG_MODE) 
	{
		if(CTLBD_MOTBD_TYPE == 0) //�Ľ��� ���� 
		{

		}
		else if(CTLBD_MOTBD_TYPE == 1) //Ŀ������ ���� 
		{
			cmmErrGetLastCode(&lErrorCode);						//������ �߻��� ���� �ڵ� Ȯ��  
			nErrorParseAxis = cmmErrParseAxis(lErrorCode);     //������ �߻���Ų �� ������ �����´� 
			nErrorParseReason = cmmErrParseReason(lErrorCode); //error code�� �޴´� 
			cmmErrGetString(lErrorCode, cErrorMsg, 200);

			sTemp.Format(_T("Msg=%s"), cErrorMsg);
			sMsg.Format(_T("Motor Num:%d, Code=%d, Axis=%d, reason=%d, %s"), n_Axis, lErrorCode, nErrorParseAxis, nErrorParseReason, sTemp);
			if (st_handler_info.cWndList != NULL)  // ����Ʈ �� ȭ�� ����
			{
				wsprintfA(st_other_info.cAbnormalMsg, "%S", sMsg);
				st_handler_info.cWndList->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
				if(MOT_DEBUG) clsFunc.OnLogFileAdd(3, sMsg);
			}		

			sMsg.Format(_T("Index or Axis=%d, %s"), n_Axis, s_JamMsg);
			if (st_handler_info.cWndList != NULL)  // ����Ʈ �� ȭ�� ����
			{
//				st_other.str_abnormal_msg = sMsg;
//				sprintf(st_other.c_abnormal_msg, st_other.str_abnormal_msg);
				wsprintfA(st_other_info.cAbnormalMsg, "%S", sMsg);
				st_handler_info.cWndList->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
				if(MOT_DEBUG) clsFunc.OnLogFileAdd(3, sMsg);
			}	
		}
	} 
	st_alarm_info.strCode = s_JamCode; //alarm.n_count_mode = 0; alarm.n_type_mode = CTL_dWARNING;
*/	
	return YES; 
}

int CCtlBd_Function::OnMotor_SafetyCheck(int n_Mode, int n_Axis, double d_TargetPos)
{/*
	// n_Mode = 0:Home Check, 1:Start, 2:Check, 3:Jog
	CString strMsg;
	int nRet = 0, nRet_1 = 0, nRet_2 = 0;
	double d_CurPos[3] = {0,};
	double d_Pos[3] = {0,};
	double d_GapCheck[3] = {0,};
	int i = 0;
	 
	int n_SetTime = 50; //091127 20->50���� ���� 100; //500ms ���� ��� ����Ǹ� �׶� ��μ� I/O ���� return 

	// **************************************************************************
	// ���� �˶� ���� �˻��Ѵ�                                                   
	// -> �˶� �߻��� ��� �˶� �����Ѵ�                                         
	// **************************************************************************
	//software limit �� ���� üũ ���� 
	if(st_handler_info.nRunStatus == dRUN)
	{
		nRet = clsFunc.OnDoorOpenCheck();		
		
		if(nRet == RET_ERROR)
		{		// ���� �����ְ�, ������ġ �������̸� ������ �����Ѵ�. 2K6/06/09/ViboX
			return RET_ERROR;
		}
	}


	if (COMI.Get_MotAlarmStatus(n_Axis) == RET_ERROR)           // ���� ALARM ���� �˻� �Լ� 
	{
		//091216 �߰� 
		if(st_motor_info[n_Axis].n_retry_time_flag == NO)
		{
			st_motor_info[n_Axis].n_retry_time_flag		= YES;
			st_motor_info[n_Axis].l_retry_time_wait[0]	= GetCurrentTime();
			COMI.Set_MotAlarmClear(n_Axis);
			if (st_handler_info.cWndList != NULL)  // ����Ʈ �� ȭ�� ����
			{
				strMsg.Format(_T("[Motor Alarm Reset_1] Axis=%d, Wait:%d"), n_Axis, 500);
				//wsprintfA(st_other_info.cNormalMsg, "%S", strMsg);
				//clsFunc.OnStringToChar(strMsg, st_other_info.cNormalMsg);
				clsMem.OnNormalMessageWrite(strMsg);
				st_handler_info.cWndList->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ���� ���� ��� ��û
			}
		}

		st_motor_info[n_Axis].l_retry_time_wait[1] = GetCurrentTime();
		st_motor_info[n_Axis].l_retry_time_wait[2] = st_motor_info[n_Axis].l_retry_time_wait[1] - st_motor_info[n_Axis].l_retry_time_wait[0];
		if(st_motor_info[n_Axis].l_retry_time_wait[2] < 0)
		{
			st_motor_info[n_Axis].n_retry_time_flag		= YES;
			st_motor_info[n_Axis].l_retry_time_wait[0]	= GetCurrentTime();

			return RET_RETRY;
		}
		else if(st_motor_info[n_Axis].l_retry_time_wait[2] < 500) //500ms�� �̻��� �Ǿ����� �������� üũ)
		{
			return RET_RETRY;
		}

		if (COMI.Set_MotAlarmClear(n_Axis) == RET_GOOD)       // ���� ALARM CLEAR �Լ� 
		{
			st_motor_info[n_Axis].n_retry_time_flag = NO; //091216 

			//091119 james			Sleep(1000);  // ���� �ð� �Ŀ� ���� Ȯ���ϱ� ���� SLEEP ����Ѵ� 
			if (st_handler_info.cWndList != NULL)  // ����Ʈ �� ȭ�� ����
			{
				strMsg.Format(_T("[Motor Alarm Reset_2] Axis=%d, Wait:%d"), n_Axis, 500);
				//wsprintfA(st_other_info.cNormalMsg, "%S", strMsg);
				//clsFunc.OnStringToChar(strMsg, st_other_info.cNormalMsg);
				clsMem.OnNormalMessageWrite(strMsg);
				st_handler_info.cWndList->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ���� ���� ��� ��û
			}

			if (COMI.Get_MotAlarmStatus(n_Axis) == RET_ERROR )  // ���� ALARM ���� �˻� �Լ� 
			{
				if (st_motor_info[n_Axis].n_retry_cnt > MOT_RTY_CNT)
				{			
					st_alarm_info.strCode.Format(_T("%02d0002"), n_Axis);
					CtlBdFunc.ms_ErrMsg = _T("[Motor_SafetyCheck_1] Alarm Check Error");				
					CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, n_Axis, st_alarm_info.strCode, CtlBdFunc.ms_ErrMsg);
					st_motor_info[n_Axis].n_retry_cnt = 0;  // �˶� ���� �õ� Ƚ��

					return RET_ERROR;
				}
				else
				{
					st_motor_info[n_Axis].n_retry_cnt++ ;

					return RET_RETRY;
				}
			}
		}
		else
		{
			st_motor_info[n_Axis].n_retry_time_flag = NO; //091216 

			if (st_motor_info[n_Axis].n_retry_cnt > MOT_RTY_CNT)
			{
				st_alarm_info.strCode.Format(_T("%02d0002"), n_Axis);
				CtlBdFunc.ms_ErrMsg = _T("[Motor_SafetyCheck_2] Alarm Check Error");				
				CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, n_Axis, st_alarm_info.strCode, CtlBdFunc.ms_ErrMsg);
				
				st_motor_info[n_Axis].n_retry_cnt = 0;  // �˶� ���� �õ� Ƚ�� 

				return RET_ERROR;
			}
			else
			{
				st_motor_info[n_Axis].n_retry_cnt++ ;

				return RET_RETRY;
			}
		}
	}
	// **************************************************************************

	// **************************************************************************
	// ���� �Ŀ� ���� �˻��Ѵ�                                                   
	// -> ���� POWER OFF �� POWER ON ���·� �����                               
	// **************************************************************************
	if (COMI.Get_MotPower(n_Axis) == RET_ERROR )          // ���� POWER ���� �˻� �Լ� 
	{
		//091216 �߰� 
		if(st_motor_info[n_Axis].n_retry_time_flag == NO)
		{
			st_motor_info[n_Axis].n_retry_time_flag		= YES;
			st_motor_info[n_Axis].l_retry_time_wait[0]	= GetCurrentTime();

			COMI.Set_MotPower(n_Axis, NO);
			if (st_handler_info.cWndList != NULL)  // ����Ʈ �� ȭ�� ����
			{
				strMsg.Format(_T("[Motor Power Reset_1] Axis=%d, Wait:%d"), n_Axis, 500);
				//wsprintfA(st_other_info.cNormalMsg, "%S", strMsg);
				//clsFunc.OnStringToChar(strMsg, st_other_info.cNormalMsg);
				clsMem.OnNormalMessageWrite(strMsg);
				st_handler_info.cWndList->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ���� ���� ��� ��û
			}
		}

		st_motor_info[n_Axis].l_retry_time_wait[1] = GetCurrentTime();
		st_motor_info[n_Axis].l_retry_time_wait[2] = st_motor_info[n_Axis].l_retry_time_wait[1] - st_motor_info[n_Axis].l_retry_time_wait[0];
		if(st_motor_info[n_Axis].l_retry_time_wait[2] < 0)
		{
			st_motor_info[n_Axis].n_retry_time_flag		= YES;
			st_motor_info[n_Axis].l_retry_time_wait[0]	= GetCurrentTime();
			
			return RET_RETRY;
		}
		else if(st_motor_info[n_Axis].l_retry_time_wait[2] < 500) //500ms�� �̻��� �Ǿ����� �������� üũ)
		{
			return RET_RETRY;
		}

		if (COMI.Set_MotPower(n_Axis, NO) == RET_GOOD)       // ���� POWER ON ���� �Լ� 
		{
			st_motor_info[n_Axis].n_retry_time_flag = NO; //091216 

//091119 james			Sleep(1000);  // ���� �ð� �Ŀ� ���� Ȯ���ϱ� ���� SLEEP ����Ѵ� 
			if (st_handler_info.cWndList != NULL)  // ����Ʈ �� ȭ�� ����
			{
				strMsg.Format(_T("[Motor Power Reset_2] Axis=%d, Wait:%d"), n_Axis, 500);
				//wsprintfA(st_other_info.cNormalMsg, "%S", strMsg);
				//clsFunc.OnStringToChar(strMsg, st_other_info.cNormalMsg);
				clsMem.OnNormalMessageWrite(strMsg);
				st_handler_info.cWndList->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ���� ���� ��� ��û
			}

			if (COMI.Get_MotPower(n_Axis) == RET_ERROR)  // ���� POWER ���� �˻� �Լ� 
			{
				if (st_motor_info[n_Axis].n_retry_cnt > (MOT_RTY_CNT))
				{ 
					st_alarm_info.strCode.Format(_T("%02d0005"), n_Axis);
					CtlBdFunc.ms_ErrMsg = _T("[Motor_SafetyCheck_1] Power Check Error");				
					CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, n_Axis, st_alarm_info.strCode, CtlBdFunc.ms_ErrMsg);

					st_motor_info[n_Axis].n_retry_cnt = 0;
					return RET_ERROR;
				}
				else
				{
					st_motor_info[n_Axis].n_retry_cnt++ ;
					return RET_RETRY;
				}
			}
		}
		else
		{
			st_motor_info[n_Axis].n_retry_time_flag = NO; //091216 

			if (st_motor_info[n_Axis].n_retry_cnt > MOT_RTY_CNT)
			{
				st_alarm_info.strCode.Format(_T("%02d0005"),  n_Axis); 
				CtlBdFunc.ms_ErrMsg = _T("[Motor_SafetyCheck_2] Power Check Error");				
				CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, n_Axis, st_alarm_info.strCode, CtlBdFunc.ms_ErrMsg);
				st_motor_info[n_Axis].n_retry_cnt = 0;
				return RET_ERROR;
			}
			else
			{
				st_motor_info[n_Axis].n_retry_cnt++ ;
				return RET_RETRY;
			}
		}
	}

	st_motor_info[n_Axis].n_retry_time_flag = NO;//091216
 */
	return RET_GOOD;
}


int CCtlBd_Function::Initialize_FASIO_Board(void) 
{
	int m=0, p=0, s=0;
	int nRet=0;
	WORD wMaskMap = 0;
/*
	INT nHSSI_speed=0;
	INT nMasterNo=0, nPortNo =0, nSlaveNo=0; 

	nRet = FAS_IO.Set_IO_BoardOpen(&nMasterNo, START_NOTHING);  // I/O ���� ���� �Լ� 
	if (nRet == CTLBD_RET_ERROR)
	{
		return CTLBD_RET_ERROR; //IO_BOARD_OPEN_FAILED;
	}

	nRet = FAS_IO.Search_IO_Master(&nMasterNo);              // PC�� ����� ������ ���� ���� �˻� �Լ� 
	if (nRet == CTLBD_RET_ERROR)
	{
		return CTLBD_RET_ERROR;
	}
	
	for(m=0; m<nMasterNo; m++)
	{
		nRet = FAS_IO.Search_IO_Port(m, &nPortNo);						// ������ ���忡 ����� PORT ���� �˻� �Լ� 
		if (nRet == CTLBD_RET_ERROR) return CTLBD_RET_ERROR;	//PORT_SEARCH_FAILED;
		
		for(p=0; p<nPortNo; p++)
		{
			nRet = FAS_IO.Set_IO_HSSISpeed(m, p, PORT_SPEED_10);		// PORT�� SLAVE I/O ������ ��� �ӵ� ���� �Լ� 
			if (nRet == CTLBD_RET_ERROR) return CTLBD_RET_ERROR; //IO_SPEED_SET_FAILED;
			 
			nRet = FAS_IO.Search_IO_Slave(m, p, &nSlaveNo);			// PORT�� ����� SLAVE �˻� �Լ� 
			if (nRet == CTLBD_RET_ERROR) return CTLBD_RET_ERROR; //IO_SLAVE_SEARCH_FAILED;

			for(s=0; s<nSlaveNo; s++)
			{
				nRet = FAS_IO.Set_IO_SlaveEnable(m, p, s, CTLBD_YES);		// SLAVE I/O ��� ���� ENABLE/DISABLE ���� �Լ� 
				if (nRet == CTLBD_RET_ERROR)	return CTLBD_RET_ERROR; //IO_SLAVE_ENABLE_FAILED;

				wMaskMap = 0; //clear
				for(i=0; i<16; i++) //0~15 ������ Port 16bit ����, ������ I/O ���¸� �����Ѵ� 
				{			
					wMaskMap = wMaskMap | (WORD)pow(2, mw_IOMaskMap[m][p][s][i]);					 
				}
				FAS_IO.Set_IO_DefineWord(m, p, s, wMaskMap); // x Master, x��Ʈ, x�� �����̺� , 16bit ����
				//FAS_IO.Set_IO_DefineWord(0, 0, 0, 0x0003); // 0�� Master, 0�� ��Ʈ, 0�� �����̺� , 16bit ����
				//FAS_IO.Set_IO_DefineWord(0, 0, 1, 0x0003); // 0�� Master, 0�� ��Ʈ, 1�� �����̺� , 16bit ����
				//FAS_IO.Set_IO_DefineWord(0, 1, 0, 0x00ff); // 0�� Master, 1�� ��Ʈ, 0�� �����̺� , 16bit ����
			}
		}
	}
*/	
	return RET_GOOD;
}



int CCtlBd_Function::Initialize_InterruptMask_TriggerConfig(void) 
{
	int i, nAxis;
//	st_motor[0].l_Set_InterruptMask[12] = CTL_YES;
//	COMI.Set_InterruptMask(0, st_motor[0].l_Set_InterruptMask);
	
//	double dRatio[3] = {100,100,100};
	//Set_TriggerSet_OneCompareConfig(int n_Axis, int n_Source, int n_Method, double d_ComparePosition)
//	COMI.Set_TriggerSet_OneCompareConfig(0, 0, 1, 5000);


//	st_motor[].n_interrupt_flag

/*
	"Normal Stop", // 0
	"Succesive start", // 1
	"", // 2
	"", // 3
	"Start of acceleration", //4
	"End of acceleration", // 5
	"Start of deceleration", // 6
	"End of deceleration", // 7
	"", // 8
	"", // 9
	"Position error tolerance exceed", // 10
	"General Comparator", // 11
	"Compared trigger", // 12
	"CLR signal resets the counter value", // 13
	"LTC input making counter value latched", // 14
	"ORG input signal ON", // 15
	"SD input signal ON", // 16
	"+DR input changes", // 17
	"-DR input changes" // 18

int		n_interrupt_source;  //���ͷ�Ʈ ó�� ���� 
	    //n_Mode : 	szCompareSource[4][50] = { 
		//0				"COMMAND Counter",
		//1				"Feedback Counter",
		//2			    "Deviation Counter",
		//3			    "General Counter"

	int		n_interrupt_method; //����ϴ� ���� ����  
		//CHAR szCompareMethod[6][50] = {
		//0				"Disable",
		//1				"CMP == COUNTER (No direction)",
		//2				"CMP == COUNTER (Only when counting up)",
		//3				"CMP == COUNTER (Only when counting down)",
		//4				"CMP > COUNTER",
		//5				"CMP < COUNTER"
*/	
	for(i=0; i<5; i++)	//for(nAxis=0; nAxis<MOT_MAXMOTOR; nAxis++)
	{
		if(i == 0)
		{
			nAxis = 0; 
			st_motor_info[nAxis].n_interrupt_flag = YES; //dc load robot Y ������ ���� 
		}
		else if(i == 1)
		{
			nAxis = 5;
			st_motor_info[nAxis].n_interrupt_flag = YES; //dc unload robot Y ������ ����
		}
		else if(i == 2) 
		{
			nAxis = 16;	
			st_motor_info[nAxis].n_interrupt_flag = YES; //insert robot Y ������ ����
		}
		else if(i == 3)
		{
			nAxis = 19;		//2009.5.27 james ���� Ǫ�� �߰� 
			//st_motor[nAxis].n_interrupt_flag = CTL_YES; //���� Ǫ�� ������ ����
		}
		else if(i == 4)
		{
			nAxis = 20;	
			st_motor_info[nAxis].n_interrupt_flag = YES; //remove robot Y ������ ����
		}

		if(st_motor_info[nAxis].n_interrupt_flag == YES) //���ͷ�Ʈ�� ����ϴ� �����̸� 
		{
			st_motor_info[nAxis].l_Set_InterruptMask[0]		= NO; //"Normal Stop", // 0
			st_motor_info[nAxis].l_Set_InterruptMask[1]		= NO; //"Succesive start", // 1
			st_motor_info[nAxis].l_Set_InterruptMask[2]		= NO; //"", // 2
			st_motor_info[nAxis].l_Set_InterruptMask[3]		= NO; //"", // 3
			st_motor_info[nAxis].l_Set_InterruptMask[4]		= NO; //"Start of acceleration", //4
			st_motor_info[nAxis].l_Set_InterruptMask[5]		= NO; //End of acceleration", // 5
			st_motor_info[nAxis].l_Set_InterruptMask[6]		= NO; //"Start of deceleration", // 6
			st_motor_info[nAxis].l_Set_InterruptMask[7]		= NO; //"End of deceleration", // 7
			st_motor_info[nAxis].l_Set_InterruptMask[8]		= NO; //"", // 8
			st_motor_info[nAxis].l_Set_InterruptMask[9]		= NO; //"", // 9
			st_motor_info[nAxis].l_Set_InterruptMask[10]	= NO; //"Position error tolerance exceed", // 10
			st_motor_info[nAxis].l_Set_InterruptMask[11]	= NO; //"General Comparator", // 11

			//�̰͸� �켱 ����Ѵ� 
			st_motor_info[nAxis].l_Set_InterruptMask[12]	= YES; //"Compared trigger", // 12

			st_motor_info[nAxis].l_Set_InterruptMask[13]	= NO; //"CLR signal resets the counter value", // 13
			st_motor_info[nAxis].l_Set_InterruptMask[14]	= NO; //"LTC input making counter value latched", // 14
			st_motor_info[nAxis].l_Set_InterruptMask[15]	= NO; //"ORG input signal ON", // 15
			st_motor_info[nAxis].l_Set_InterruptMask[16]	= NO; //"SD input signal ON", // 16
			st_motor_info[nAxis].l_Set_InterruptMask[17]	= NO; //"+DR input changes", // 17
			st_motor_info[nAxis].l_Set_InterruptMask[18]	= NO; //"-DR input changes" // 18

			//���ͷ�Ʈ ����Ű ������ �����Ѵ� 
			//Set_InterruptMask(int n_Axis, long l_Set_InterruptMask[19])
			COMI.Set_InterruptMask(nAxis, st_motor_info[nAxis].l_Set_InterruptMask);

			//���ͷ�Ʈ �߻� ���� �� ���� ���� 
			//Set_TriggerSet_OneCompareConfig(int n_Axis, int n_Source, int n_Method, double d_ComparePosition)
			COMI.Set_TriggerSet_OneCompareConfig(nAxis, st_motor_info[nAxis].n_interrupt_source, st_motor_info[nAxis].n_interrupt_method, st_motor_info[nAxis].n_interrupt_position);
		}
	}		
	return RET_GOOD;
}
