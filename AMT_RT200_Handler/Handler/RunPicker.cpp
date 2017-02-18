#include "StdAfx.h"
#include "RunPicker.h"
#include "variable.h"
//#include "CV_test.h"
#include "FastechPublic_IO.h"
#include "ComizoaPublic.h"
#include "CtlBd_Library.h"
#include "Ctlbd_Variable.h"

CRunPicker clsRunPicker;

CRunPicker::CRunPicker(void)
{
	
}


CRunPicker::~CRunPicker(void)
{
}

void CRunPicker::OnThreadRUN()
{
	switch(st_handler_info.nRunStatus)
	{
	case dINIT:
		OnRunInit();
		break;

	case dRUN:
		if (st_sync_info.nInitRobot != INIT_COMPLETE)	break;

		OnPickerRun();
		break;
	}
}
void CRunPicker::OnRunInit()
{
	

}

void CRunPicker::OnPickerRun()
{

}

