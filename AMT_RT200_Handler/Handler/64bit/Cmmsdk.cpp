/*******************************************************************************
* [ Cmmsdk.cpp ]
* Source(C/C++) file for loading CMMSDK library
* - Update Data: 2013/04/01
* - Provider: COMIZOA Co., Ltd.
* - Phone: +82-42-936-6500~6
* - Fax  : +82-42-936-6507
* - URL  : http://www.comizoa.co.kr,  http://www.comizoa.com
********************************************************************************/

#include "stdafx.h" // VC++ 이 아닌 다른곳에서 컴파일할 때는 이 구문을 삭제하십시오.

#if defined(__BCPLUSPLUS__)  // Borland C++ Builder specific
 #include <vcl.h>
 #pragma hdrstop
#endif

#include <windows.h>
#define _CMM_SDK_LOADER_C_
#include "Cmmsdk.h"
#include <stdio.h>

#define __DLL_FILE_FULL_PATH__ ".\\64bit\\CmmSDK.DLL"

typedef struct{
    char *name;
    FARPROC* ptr;
}LIB_FUNCT;

static HINSTANCE g_hCmmLibrary=NULL;

BOOL cmmLoadDll(void)
{
	LIB_FUNCT Func[] = {
				{"cmmGnDeviceLoad", 	 (FARPROC*)&cmmGnDeviceLoad},
				{"cmmGnDeviceUnload",	 (FARPROC*)&cmmGnDeviceUnload},
				{"cmmGnDeviceIsLoaded",  (FARPROC*)&cmmGnDeviceIsLoaded},
				{"cmmGnDeviceReset",     (FARPROC*)&cmmGnDeviceReset},
				{"cmmGnInitFromFile",	 (FARPROC*)&cmmGnInitFromFile},
				{"cmmGnInitFromFile",	 (FARPROC*)&cmmGnInitFromFile_MapOnly},
				{"cmmGnSetServoOn",	 (FARPROC*)&cmmGnSetServoOn},
				{"cmmGnGetServoOn",	 (FARPROC*)&cmmGnGetServoOn},
				{"cmmGnSetAlarmRes",	 (FARPROC*)&cmmGnSetAlarmRes},
				{"cmmGnGetAlarmRes",	 (FARPROC*)&cmmGnGetAlarmRes},
				{"cmmGnPulseAlarmRes",	 (FARPROC*)&cmmGnPulseAlarmRes},
				{"cmmGnSetSimulMode",    (FARPROC*)&cmmGnSetSimulMode},
				{"cmmGnGetSimulMode",	 (FARPROC*)&cmmGnGetSimulMode},
				{"cmmGnPutInternalSTA",	 (FARPROC*)&cmmGnPutInternalSTA},
				{"cmmGnSetEmergency",    (FARPROC*)&cmmGnSetEmergency},
				{"cmmGnGetEmergency",    (FARPROC*)&cmmGnGetEmergency},
				{"cmmGnBitShift", 	 (FARPROC*)&cmmGnBitShift},
				{"cmmCfgSetMioProperty", (FARPROC*)&cmmCfgSetMioProperty}, 
				{"cmmCfgGetMioProperty", (FARPROC*)&cmmCfgGetMioProperty},
				{"cmmCfgSetFilter",	 (FARPROC*)&cmmCfgSetFilter},
				{"cmmCfgGetFilter",	 (FARPROC*)&cmmCfgGetFilter},
				{"cmmCfgSetFilterAB",	 (FARPROC*)&cmmCfgSetFilterAB},
				{"cmmCfgGetFilterAB",	 (FARPROC*)&cmmCfgGetFilterAB},
				{"cmmCfgSetInMode",	 (FARPROC*)&cmmCfgSetInMode},
				{"cmmCfgGetInMode",	 (FARPROC*)&cmmCfgGetInMode},
				{"cmmCfgSetOutMode",	 (FARPROC*)&cmmCfgSetOutMode},
				{"cmmCfgGetOutMode",	 (FARPROC*)&cmmCfgGetOutMode},
				{"cmmCfgSetCtrlMode",	 (FARPROC*)&cmmCfgSetCtrlMode},
				{"cmmCfgGetCtrlMode",	 (FARPROC*)&cmmCfgGetCtrlMode},
				{"cmmCfgSetInOutRatio",  (FARPROC*)&cmmCfgSetInOutRatio},
				{"cmmCfgGetInOutRatio",  (FARPROC*)&cmmCfgGetInOutRatio},
				{"cmmCfgSetUnitDist",	 (FARPROC*)&cmmCfgSetUnitDist},
				{"cmmCfgGetUnitDist",	 (FARPROC*)&cmmCfgGetUnitDist},
				{"cmmCfgSetUnitSpeed",	 (FARPROC*)&cmmCfgSetUnitSpeed},
				{"cmmCfgGetUnitSpeed",	 (FARPROC*)&cmmCfgGetUnitSpeed}, 
				{"cmmCfgSetSpeedRange",	 (FARPROC*)&cmmCfgSetSpeedRange}, 
				{"cmmCfgGetSpeedRange",	 (FARPROC*)&cmmCfgGetSpeedRange}, 
				{"cmmCfgSetSpeedPattern",(FARPROC*)&cmmCfgSetSpeedPattern},
				{"cmmCfgGetSpeedPattern",(FARPROC*)&cmmCfgGetSpeedPattern},
				{"cmmCfgSetSpeedPattern_T",(FARPROC*)&cmmCfgSetSpeedPattern_T},
				{"cmmCfgGetSpeedPattern_T",(FARPROC*)&cmmCfgGetSpeedPattern_T},
				{"cmmCfgSetMinAccTime",(FARPROC*)&cmmCfgSetMinAccTime},
				{"cmmCfgGetMinAccTime",(FARPROC*)&cmmCfgGetMinAccTime},
				{"cmmCfgSetVelCorrRatio", (FARPROC*)&cmmCfgSetVelCorrRatio},
				{"cmmCfgGetVelCorrRatio", (FARPROC*)&cmmCfgGetVelCorrRatio},
				{"cmmCfgSetMinCorrVel", (FARPROC*)&cmmCfgSetMinCorrVel},
				{"cmmCfgGetMinCorrVel", (FARPROC*)&cmmCfgGetMinCorrVel},
				{"cmmCfgSetActSpdCheck" ,(FARPROC*)&cmmCfgSetActSpdCheck},
				{"cmmCfgGetActSpdCheck" ,(FARPROC*)&cmmCfgGetActSpdCheck},
				{"cmmCfgSetSoftLimit",	 (FARPROC*)&cmmCfgSetSoftLimit},
				{"cmmCfgGetSoftLimit",	  (FARPROC*)&cmmCfgGetSoftLimit},
				{"cmmCfgSetRingCntr",	  (FARPROC*)&cmmCfgSetRingCntr},
				{"cmmCfgGetRingCntr",	  (FARPROC*)&cmmCfgGetRingCntr},
				{"cmmCfgSetSeqMode", (FARPROC*)&cmmCfgSetSeqMode},
				{"cmmCfgGetSeqMode", (FARPROC*)&cmmCfgGetSeqMode},				
				{"cmmCfgSetManExtLimit", (FARPROC*)&cmmCfgSetManExtLimit},
				{"cmmCfgGetManExtLimit", (FARPROC*)&cmmCfgGetManExtLimit},
				{"cmmHomeSetConfig",	  (FARPROC*)&cmmHomeSetConfig},
				{"cmmHomeGetConfig",	  (FARPROC*)&cmmHomeGetConfig},
				{"cmmHomeSetPosClrMode",	  (FARPROC*)&cmmHomeSetPosClrMode},
				{"cmmHomeGetPosClrMode",	  (FARPROC*)&cmmHomeGetPosClrMode},
				{"cmmHomeSetSpeedPattern",(FARPROC*)&cmmHomeSetSpeedPattern},
				{"cmmHomeGetSpeedPattern",(FARPROC*)&cmmHomeGetSpeedPattern},
				{"cmmHomeSetSpeedPattern_T",(FARPROC*)&cmmHomeSetSpeedPattern_T},
				{"cmmHomeGetSpeedPattern_T",(FARPROC*)&cmmHomeGetSpeedPattern_T},
				{"cmmHomeMoveStart",	  (FARPROC*)&cmmHomeMoveStart},
				{"cmmHomeMove",		  (FARPROC*)&cmmHomeMove},
				{"cmmHomeMoveAll",	  (FARPROC*)&cmmHomeMoveAll},
				{"cmmHomeMoveAllStart",	  (FARPROC*)&cmmHomeMoveAllStart},
				{"cmmHomeGetSuccess",	  (FARPROC*)&cmmHomeGetSuccess}, 
				{"cmmHomeSetSuccess",	  (FARPROC*)&cmmHomeSetSuccess}, 
				{"cmmHomeIsBusy",	  (FARPROC*)&cmmHomeIsBusy}, 
				{"cmmHomeWaitDone",	  (FARPROC*)&cmmHomeWaitDone}, 
				{"cmmSxSetSpeedRatio",	  (FARPROC*)&cmmSxSetSpeedRatio},
				{"cmmSxGetSpeedRatio",	  (FARPROC*)&cmmSxGetSpeedRatio},
				{"cmmSxMoveStart",	  (FARPROC*)&cmmSxMoveStart},
				{"cmmSxMove",		  (FARPROC*)&cmmSxMove},
				{"cmmSxMoveToStart",	  (FARPROC*)&cmmSxMoveToStart},
				{"cmmSxMoveTo",		  (FARPROC*)&cmmSxMoveTo},
				{"cmmSxVMoveStart",	  (FARPROC*)&cmmSxVMoveStart},
				{"cmmSxStop",		  (FARPROC*)&cmmSxStop},
				{"cmmSxStopEmg",	  (FARPROC*)&cmmSxStopEmg},
				{"cmmSxIsDone",		  (FARPROC*)&cmmSxIsDone},
				{"cmmSxWaitDone",	  (FARPROC*)&cmmSxWaitDone},
				{"cmmSxGetTargetPos",	  (FARPROC*)&cmmSxGetTargetPos},
				{"cmmSxOptSetIniSpeed",   (FARPROC*)&cmmSxOptSetIniSpeed},
				{"cmmSxOptGetIniSpeed",   (FARPROC*)&cmmSxOptGetIniSpeed},
				{"cmmSxSetCorrection",	  (FARPROC*)&cmmSxSetCorrection},
				{"cmmSxGetCorrection",	  (FARPROC*)&cmmSxGetCorrection},
				{"cmmSxOptSetSyncMode",   (FARPROC*)&cmmSxOptSetSyncMode},
				{"cmmSxOptGetSyncMode",   (FARPROC*)&cmmSxOptGetSyncMode},
				{"cmmSxOptSetSyncOut",   (FARPROC*)&cmmSxOptSetSyncOut},
				{"cmmSxOptGetSyncOut",   (FARPROC*)&cmmSxOptGetSyncOut},
				{"cmmSxOptSetRdpOffset",  (FARPROC*)&cmmSxOptSetRdpOffset},
				{"cmmSxOptGetRdpOffset",  (FARPROC*)&cmmSxOptGetRdpOffset},
				{"cmmMxMove",	(FARPROC*)&cmmMxMove},
				{"cmmMxMoveStart",	(FARPROC*)&cmmMxMoveStart},
				{"cmmMxMoveTo",	(FARPROC*)&cmmMxMoveTo},
				{"cmmMxMoveToStart",	(FARPROC*)&cmmMxMoveToStart},
				{"cmmMxVMoveStart",	(FARPROC*)&cmmMxVMoveStart},
				{"cmmMxStop",	(FARPROC*)&cmmMxStop},
				{"cmmMxStopEmg",	(FARPROC*)&cmmMxStopEmg},
				{"cmmMxIsDone",	(FARPROC*)&cmmMxIsDone},
				{"cmmMxWaitDone",	(FARPROC*)&cmmMxWaitDone},
				{"cmmIxMapAxes",	(FARPROC*)&cmmIxMapAxes},
				{"cmmIxSetSpeedPattern", (FARPROC*)&cmmIxSetSpeedPattern},
				{"cmmIxGetSpeedPattern", (FARPROC*)&cmmIxGetSpeedPattern},
				{"cmmIxSetSpeedPattern_T", (FARPROC*)&cmmIxSetSpeedPattern_T},
				{"cmmIxGetSpeedPattern_T", (FARPROC*)&cmmIxGetSpeedPattern_T},
				{"cmmIxLine",	(FARPROC*)&cmmIxLine},
				{"cmmIxLineStart",	(FARPROC*)&cmmIxLineStart},
				{"cmmIxLineTo",	(FARPROC*)&cmmIxLineTo},
				{"cmmIxLineToStart",	(FARPROC*)&cmmIxLineToStart},
				{"cmmIxArcA",	(FARPROC*)&cmmIxArcA},
				{"cmmIxArcAStart",	(FARPROC*)&cmmIxArcAStart},
				{"cmmIxArcATo",	(FARPROC*)&cmmIxArcATo},
				{"cmmIxArcAToStart",	(FARPROC*)&cmmIxArcAToStart},
				{"cmmIxArcP",	(FARPROC*)&cmmIxArcP},
				{"cmmIxArcPStart",	(FARPROC*)&cmmIxArcPStart},
				{"cmmIxArcPTo",	(FARPROC*)&cmmIxArcPTo},
				{"cmmIxArcPToStart",	(FARPROC*)&cmmIxArcPToStart},
				{"cmmIxArc3PStart",	(FARPROC*)&cmmIxArc3PStart},
				{"cmmIxArc3P",	(FARPROC*)&cmmIxArc3P},
				{"cmmIxArc3PToStart",	(FARPROC*)&cmmIxArc3PToStart},
				{"cmmIxArc3PTo",	(FARPROC*)&cmmIxArc3PTo},
				{"cmmIxIsDone",	(FARPROC*)&cmmIxIsDone},
				{"cmmIxWaitDone",	(FARPROC*)&cmmIxWaitDone},
				{"cmmIxxHelOnceSetSpeed",	(FARPROC*)&cmmIxxHelOnceSetSpeed},
				{"cmmIxxHelOnceGetSpeed",	(FARPROC*)&cmmIxxHelOnceGetSpeed},
				{"cmmIxxHelOnce",	(FARPROC*)&cmmIxxHelOnce},
				{"cmmIxxHelOnceStart",	(FARPROC*)&cmmIxxHelOnceStart},
				{"cmmIxxSplineBuild",	(FARPROC*)&cmmIxxSplineBuild},
				{"cmmIxStop",	(FARPROC*)&cmmIxStop},
				{"cmmIxStopEmg",	(FARPROC*)&cmmIxStopEmg},
				{"cmmExVMoveStart",	(FARPROC*)&cmmExVMoveStart},
				{"cmmExMoveStart",	(FARPROC*)&cmmExMoveStart},
				{"cmmExMoveToStart",	(FARPROC*)&cmmExMoveToStart},
				{"cmmPlsrSetInMode",	(FARPROC*)&cmmPlsrSetInMode},
				{"cmmPlsrGetInMode",	(FARPROC*)&cmmPlsrGetInMode},
				{"cmmPlsrSetGain",	(FARPROC*)&cmmPlsrSetGain},
				{"cmmPlsrGetGain",	(FARPROC*)&cmmPlsrGetGain},
				{"cmmPlsrHomeMoveStart",	(FARPROC*)&cmmPlsrHomeMoveStart},
				{"cmmPlsrMoveStart",	(FARPROC*)&cmmPlsrMoveStart},
				{"cmmPlsrMove",	(FARPROC*)&cmmPlsrMove},
				{"cmmPlsrMoveToStart",	(FARPROC*)&cmmPlsrMoveToStart},
				{"cmmPlsrMoveTo",	(FARPROC*)&cmmPlsrMoveTo},
				{"cmmPlsrVMoveStart",	(FARPROC*)&cmmPlsrVMoveStart},
				{"cmmPlsrIsActive",	(FARPROC*)&cmmPlsrIsActive},
				{"cmmMsRegisterSlave",	(FARPROC*)&cmmMsRegisterSlave},
				{"cmmMsUnregisterSlave",	(FARPROC*)&cmmMsUnregisterSlave},
				{"cmmMsCheckSlaveState",	(FARPROC*)&cmmMsCheckSlaveState},
				{"cmmMsGetMasterAxis",	(FARPROC*)&cmmMsGetMasterAxis},
				//====================== Overriding FUNCTIONS =========================================//
				{"cmmOverrideSpeedSet", (FARPROC*)&cmmOverrideSpeedSet},
				{"cmmOverrideSpeedSetAll", (FARPROC*)&cmmOverrideSpeedSetAll},
				{"cmmOverrideMove", (FARPROC*)&cmmOverrideMove},
				{"cmmOverrideMoveTo", (FARPROC*)&cmmOverrideMoveTo},
				//====================== LIST-MOTION FUNCTIONS ========================================//
				{"cmmLmMapAxes", (FARPROC*)&cmmLmMapAxes},
				{"cmmLmBeginList", (FARPROC*)&cmmLmBeginList},
				{"cmmLmEndList", (FARPROC*)&cmmLmEndList},
				{"cmmLmIsDone", (FARPROC*)&cmmLmIsDone},
				{"cmmLmWaitDone", (FARPROC*)&cmmLmWaitDone},
				{"cmmLmStartMotion", (FARPROC*)&cmmLmStartMotion},
				{"cmmLmAbortMotion", (FARPROC*)&cmmLmAbortMotion},
				{"cmmLmAbortMotionEx", (FARPROC*)&cmmLmAbortMotionEx},
				{"cmmLmCurSequence", (FARPROC*)&cmmLmCurSequence},
				{"cmmLmImmediacySet", (FARPROC*)&cmmLmImmediacySet},
				{"cmmLmDoPutOne", (FARPROC*)&cmmLmDoPutOne},
				{"cmmLmDoPutMulti", (FARPROC*)&cmmLmDoPutMulti},
				{"cmmLmDoPulseOne", (FARPROC*)&cmmLmDoPulseOne},
				{"cmmLmDoPulseMulti", (FARPROC*)&cmmLmDoPulseMulti},
				//====================== 상태감시 FUNCTIONS ===========================================//
				{"cmmStSetCount", (FARPROC*)&cmmStSetCount},
				{"cmmStGetCount", (FARPROC*)&cmmStGetCount},
				{"cmmStSetPosition", (FARPROC*)&cmmStSetPosition},
				{"cmmStGetPosition", (FARPROC*)&cmmStGetPosition},
				{"cmmStGetSpeed", (FARPROC*)&cmmStGetSpeed},
				{"cmmStReadMotionState", (FARPROC*)&cmmStReadMotionState},
				{"cmmStReadMioStatuses", (FARPROC*)&cmmStReadMioStatuses},
				{"cmmStGetMstString", (FARPROC*)&cmmStGetMstString},
				{"cmmMstAll_SetCfg", (FARPROC*)&cmmMstAll_SetCfg},
				{"cmmMstAll_GetCfg", (FARPROC*)&cmmMstAll_GetCfg},
				{"cmmMstAll_ManScan", (FARPROC*)&cmmMstAll_ManScan},
				{"cmmMstAll_AutoStart", (FARPROC*)&cmmMstAll_AutoStart},
				{"cmmMstAll_AutoStop", (FARPROC*)&cmmMstAll_AutoStop},
				{"cmmMstAll_AutoGetBuf", (FARPROC*)&cmmMstAll_AutoGetBuf},
				{"cmmMstAll_AutoGetData", (FARPROC*)&cmmMstAll_AutoGetData},
				{"cmmMstAll_AutoGetInfo", (FARPROC*)&cmmMstAll_AutoGetInfo},
				{"cmmMstAll_AutoGetInfo2", (FARPROC*)&cmmMstAll_AutoGetInfo2},
				//====================== INTERRUPT FUNCTIONS ==========================================//
				{"cmmIntSetMask", (FARPROC*)&cmmIntSetMask},
				{"cmmIntGetMask", (FARPROC*)&cmmIntGetMask},
				{"cmmIntHandlerSetup", (FARPROC*)&cmmIntHandlerSetup},
				{"cmmIntHandlerEnable", (FARPROC*)&cmmIntHandlerEnable},
				{"cmmIntReadFlag", (FARPROC*)&cmmIntReadFlag},
				{"cmmIntReadErrorStatus", (FARPROC*)&cmmIntReadErrorStatus},
				{"cmmIntReadEventStatus", (FARPROC*)&cmmIntReadEventStatus},
				//====================== LATCH FUNCTIONS ==============================================//
				{"cmmLtcIsLatched", (FARPROC*)&cmmLtcIsLatched},
				{"cmmLtcReadLatch", (FARPROC*)&cmmLtcReadLatch},
				{"cmmLtcQue_SetCfg", (FARPROC*)&cmmLtcQue_SetCfg},
				{"cmmLtcQue_GetCfg", (FARPROC*)&cmmLtcQue_GetCfg},
				{"cmmLtcQue_SetEnable", (FARPROC*)&cmmLtcQue_SetEnable},
				{"cmmLtcQue_GetEnable", (FARPROC*)&cmmLtcQue_GetEnable},
				{"cmmLtcQue_GetItemCount", (FARPROC*)&cmmLtcQue_GetItemCount},
				{"cmmLtcQue_ResetItemCount", (FARPROC*)&cmmLtcQue_ResetItemCount},
				{"cmmLtcQue_Deque", (FARPROC*)&cmmLtcQue_Deque},
				{"cmmLtcQue_PeekAt", (FARPROC*)&cmmLtcQue_PeekAt},
				//====================== Position Compare FUNCTIONS ===================================//
				{"cmmCmpErrSetConfig", (FARPROC*)&cmmCmpErrSetConfig},
				{"cmmCmpErrGetConfig", (FARPROC*)&cmmCmpErrGetConfig},
				{"cmmCmpGenSetConfig", (FARPROC*)&cmmCmpGenSetConfig},
				{"cmmCmpGenGetConfig", (FARPROC*)&cmmCmpGenGetConfig},
				{"cmmCmpTrgSetConfig", (FARPROC*)&cmmCmpTrgSetConfig},
				{"cmmCmpTrgGetConfig", (FARPROC*)&cmmCmpTrgGetConfig},
				{"cmmCmpTrgSetOneData", (FARPROC*)&cmmCmpTrgSetOneData},
				{"cmmCmpTrgGetCurData", (FARPROC*)&cmmCmpTrgGetCurData},
				{"cmmCmpTrgContRegTable", (FARPROC*)&cmmCmpTrgContRegTable},
				{"cmmCmpTrgContBuildTable", (FARPROC*)&cmmCmpTrgContBuildTable},
				{"cmmCmpTrgContStart", (FARPROC*)&cmmCmpTrgContStart},
				{"cmmCmpTrgContStop", (FARPROC*)&cmmCmpTrgContStop},
				{"cmmCmpTrgContIsActive", (FARPROC*)&cmmCmpTrgContIsActive},
				{"cmmCmpTrgHigh_WriteData", (FARPROC*)&cmmCmpTrgHigh_WriteData},
				{"cmmCmpTrgHigh_ReadData", (FARPROC*)&cmmCmpTrgHigh_ReadData},
				{"cmmCmpTrgHigh_Start", (FARPROC*)&cmmCmpTrgHigh_Start},
				{"cmmCmpTrgHigh_Stop", (FARPROC*)&cmmCmpTrgHigh_Stop},
				{"cmmCmpTrgHigh_Check", (FARPROC*)&cmmCmpTrgHigh_Check},
				
				{"cmmCmpQue_SetEnable", (FARPROC*)&cmmCmpQue_SetEnable},
				{"cmmCmpQue_GetEnable", (FARPROC*)&cmmCmpQue_GetEnable},
				{"cmmCmpQue_SetQueSize", (FARPROC*)&cmmCmpQue_SetQueSize},
				{"cmmCmpQue_GetQueSize", (FARPROC*)&cmmCmpQue_GetQueSize},
				{"cmmCmpQue_Enque", (FARPROC*)&cmmCmpQue_Enque},
				{"cmmCmpQue_GetEnqueCnt", (FARPROC*)&cmmCmpQue_GetEnqueCnt},
				{"cmmCmpQue_GetOutCnt", (FARPROC*)&cmmCmpQue_GetOutCnt},
				{"cmmCmpQue_SetOutCnt", (FARPROC*)&cmmCmpQue_SetOutCnt},
				{"cmmCmpQue_SetLtcLinkMode", (FARPROC*)&cmmCmpQue_SetLtcLinkMode},
				{"cmmCmpQue_GetLtcLinkMode", (FARPROC*)&cmmCmpQue_GetLtcLinkMode},

				//====================== Digital In/Out FUNCTIONS =====================================//
				{"cmmDiSetInputLogic", (FARPROC*)&cmmDiSetInputLogic},
				{"cmmDiGetInputLogic", (FARPROC*)&cmmDiGetInputLogic},
				{"cmmDiGetOne", (FARPROC*)&cmmDiGetOne},
				{"cmmDiGetMulti", (FARPROC*)&cmmDiGetMulti},
				{"cmmDiGetOneF", (FARPROC*)&cmmDiGetOneF},
				{"cmmDiGetMultiF", (FARPROC*)&cmmDiGetMultiF},
				{"cmmDoSetOutputLogic", (FARPROC*)&cmmDoSetOutputLogic},
				{"cmmDoGetOutputLogic", (FARPROC*)&cmmDoGetOutputLogic},
				{"cmmDoPutOne", (FARPROC*)&cmmDoPutOne},
				{"cmmDoGetOne", (FARPROC*)&cmmDoGetOne},
				{"cmmDoPulseOne", (FARPROC*)&cmmDoPulseOne},
				{"cmmDoPutMulti", (FARPROC*)&cmmDoPutMulti},
				{"cmmDoGetMulti", (FARPROC*)&cmmDoGetMulti},
				{"cmmDoPulseMulti", (FARPROC*)&cmmDoPulseMulti},
				//====================== Advanced FUNCTIONS ===========================================//
				{"cmmAdvGetNumAvailAxes", (FARPROC*)&cmmAdvGetNumAvailAxes},
				{"cmmAdvGetNumDefinedAxes", (FARPROC*)&cmmAdvGetNumDefinedAxes},
				{"cmmAdvGetNumAvailDioChan", (FARPROC*)&cmmAdvGetNumAvailDioChan},
				{"cmmAdvGetNumDefinedDioChan", (FARPROC*)&cmmAdvGetNumDefinedDioChan},
				{"cmmAdvGetMotDeviceId", (FARPROC*)&cmmAdvGetMotDeviceId},
				{"cmmAdvGetMotDevInstance", (FARPROC*)&cmmAdvGetMotDevInstance},
				{"cmmAdvGetDioDeviceId", (FARPROC*)&cmmAdvGetDioDeviceId},
				{"cmmAdvGetDioDevInstance", (FARPROC*)&cmmAdvGetDioDevInstance},
				{"cmmAdvGetDeviceHandle", (FARPROC*)&cmmAdvGetDeviceHandle},
				{"cmmAdvWriteMainSpace", (FARPROC*)&cmmAdvWriteMainSpace},
				{"cmmAdvReadMainSpace", (FARPROC*)&cmmAdvReadMainSpace},
				{"cmmAdvWriteRegister", (FARPROC*)&cmmAdvWriteRegister},
				{"cmmAdvReadRegister", (FARPROC*)&cmmAdvReadRegister},
				{"cmmAdvGetMioCfg1Dword", (FARPROC*)&cmmAdvGetMioCfg1Dword},
				{"cmmAdvSetMioCfg1Dword", (FARPROC*)&cmmAdvSetMioCfg1Dword},
				{"cmmAdvSetToolboxMode", (FARPROC*)&cmmAdvSetToolboxMode},
				{"cmmAdvGetString", (FARPROC*)&cmmAdvGetString},
				{"cmmAdvErcOut", (FARPROC*)&cmmAdvErcOut},
				{"cmmAdvErcReset", (FARPROC*)&cmmAdvErcReset},
				{"cmmAdvSetExtOptions", (FARPROC*)&cmmAdvSetExtOptions},
				{"cmmAdvEnumMotDevices", (FARPROC*)&cmmAdvEnumMotDevices},
				{"cmmAdvGetMotDevMap", (FARPROC*)&cmmAdvGetMotDevMap},
				{"cmmAdvEnumDioDevices", (FARPROC*)&cmmAdvEnumDioDevices},
				{"cmmAdvGetDioDevMap", (FARPROC*)&cmmAdvGetDioDevMap},
				{"cmmAdvInitFromCmeBuffer", (FARPROC*)&cmmAdvInitFromCmeBuffer},
				{"cmmAdvInitFromCmeBuffer_MapOnly", (FARPROC*)&cmmAdvInitFromCmeBuffer_MapOnly},
				{"cmmAdvGetLatestCmeFile", (FARPROC*)&cmmAdvGetLatestCmeFile},
				{"cmmAdvGetAxisCapability", (FARPROC*)&cmmAdvGetAxisCapability},
				//====================== DEBUG-LOGGING FUNCTIONS ======================================//
				{"cmmDlogSetup", (FARPROC*)&cmmDlogSetup},
				{"cmmDlogAddComment", (FARPROC*)&cmmDlogAddComment},
				{"cmmDlogGetCurLevel", (FARPROC*)&cmmDlogGetCurLevel},
				{"cmmDlogGetCurFilePath", (FARPROC*)&cmmDlogGetCurFilePath},
				{"cmmDlogEnterManMode", (FARPROC*)&cmmDlogEnterManMode},
				{"cmmDlogExitManMode", (FARPROC*)&cmmDlogExitManMode},
				//====================== ERROR HANDLING FUNCTIONS =====================================//
				{"cmmErrGetLastCode", (FARPROC*)&cmmErrGetLastCode},
				{"cmmErrClearLastCode", (FARPROC*)&cmmErrClearLastCode},
				{"cmmErrParseAxis", (FARPROC*)&cmmErrParseAxis},
				{"cmmErrParseReason", (FARPROC*)&cmmErrParseReason},
				{"cmmErrGetString", (FARPROC*)&cmmErrGetString},
				{"cmmErrShowLast", (FARPROC*)&cmmErrShowLast},
				{"cmmErrSetSkipShowMessage", (FARPROC*)&cmmErrSetSkipShowMessage},
				{"cmmErrGetSkipShowMessage", (FARPROC*)&cmmErrGetSkipShowMessage},
				{"cmmErrSetEnableAutoMessage", (FARPROC*)&cmmErrSetEnableAutoMessage},
				{"cmmErrGetEnableAutoMessage", (FARPROC*)&cmmErrGetEnableAutoMessage},
				//====================== UTILITY FUNCTIONS =====================================//
				{"cmmUtlProcessWndMsgS", (FARPROC*)&cmmUtlProcessWndMsgS},
				{"cmmUtlProcessWndMsgM", (FARPROC*)&cmmUtlProcessWndMsgM},
				{"cmmUtlDelayMicroSec", (FARPROC*)&cmmUtlDelayMicroSec},				
				{"cmmUtlReadUserTable", (FARPROC*)&cmmUtlReadUserTable},
				{"cmmUtlWriteUserTable", (FARPROC*)&cmmUtlWriteUserTable},
				//====================== Extended LIST MOTION FUNCTIONS ========================//
				{"cmmLmxStart", (FARPROC*)&cmmLmxStart},
				{"cmmLmxPause", (FARPROC*)&cmmLmxPause},
				{"cmmLmxResume", (FARPROC*)&cmmLmxResume},
				{"cmmLmxEnd", (FARPROC*)&cmmLmxEnd},
				{"cmmLmxSetSeqMode", (FARPROC*)&cmmLmxSetSeqMode},
				{"cmmLmxGetSeqMode", (FARPROC*)&cmmLmxGetSeqMode},
				{"cmmLmxGetNextItemId", (FARPROC*)&cmmLmxGetNextItemId},
				{"cmmLmxSetNextItemId", (FARPROC*)&cmmLmxSetNextItemId},
				{"cmmLmxSetNextItemParam", (FARPROC*)&cmmLmxSetNextItemParam},
				{"cmmLmxGetNextItemParam", (FARPROC*)&cmmLmxGetNextItemParam},
				{"cmmLmxGetRunItemParam", (FARPROC*)&cmmLmxGetRunItemParam},
				{"cmmLmxGetRunItemStaPos", (FARPROC*)&cmmLmxGetRunItemStaPos},
				{"cmmLmxGetRunItemTargPos", (FARPROC*)&cmmLmxGetRunItemTargPos},
				{"cmmLmxGetSts", (FARPROC*)&cmmLmxGetSts},
								
				// TODO 
				{NULL, NULL}
			   };
	LIB_FUNCT *f;
	
	if((g_hCmmLibrary = LoadLibraryA (__DLL_FILE_FULL_PATH__)) == NULL)
    	return FALSE;
	
	f = &Func[0];
	while(f->ptr != NULL)
	{
		*(f->ptr) = GetProcAddress ((HMODULE)g_hCmmLibrary, f->name);
		if(*(f->ptr) == NULL){
			//printf("Failed to load \"%s\" Function\n", f->name);
		}
		f++;
	}
	return TRUE;
}

void cmmUnloadDll(void)
{
	if(g_hCmmLibrary){
		FreeLibrary(g_hCmmLibrary);
		g_hCmmLibrary = NULL;
	}
}

