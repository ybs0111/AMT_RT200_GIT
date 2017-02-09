// ZebraPrint.cpp: implementation of the CZebraPrint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "ZebraPrint.h"
#include "io.h"			// 파일 존재 유무 검사 함수 호출하기 위해서는 반드시 필요
#include "Variable.h"
#include "AlgMemory.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CZebraPrint clsZebra;

CZebraPrint::CZebraPrint()
{
	m_nStstusMode = 0;
}

CZebraPrint::~CZebraPrint()
{

}

bool CZebraPrint::OnPrintStatus(CString strCheckData, int nPort)
{
	//3.22 Keyence
	int     nLength;  // 데이터 총 길이 저장 변수 
	CString strBody;  // BODY 데이터 저장 변수 
	CString str_tmp;
	char  cp_cmd[1024];
	
	int     i=0, j=0;
	int     nStartIndex, nEndIndex;  // BCR 데이터의 시작 및 종료 위치 저장 변수 
	int     nReceivedDataCount=0;
	CString strTempDataCount;  // 데이터 갯수 저장 변수 
	int		nResultData[100];
	
	CString str_Tmp;
	CString str_Tmp_Data;
	
	memset(&cp_cmd, 0, sizeof(cp_cmd));  // 데이터 저장 변수 초기화 
	
	// **************************************************************************
	// 수신 데이터 최소 길이 검사한다 [최소 3자리 이상]                          
	// **************************************************************************
	nLength = strCheckData.GetLength();
	if(nLength < 3)
	{
		return false;
	}
	// **************************************************************************
	strCheckData.TrimLeft(_T("\r\n"));
	strCheckData.TrimRight(_T("\r\n"));
	
	strBody = strCheckData;//.Mid(0, nLength - 1);  // [테일] 제거 
	strBody.TrimLeft(0x02);		// STX 제거 
	strBody.TrimRight(0x03);	// ETX 제거 

	if(nLength > 50)					//HS의 데이타가 50자리가 넘음
	{
		strBody.Replace(_T("\2"), _T(""));		// 중간에 있는 STX 제거
		strBody.Replace(_T("\3"), _T(","));		// 중간에 있는 ETX를 ,로 변경
	}
	else
	{
		strBody = strBody + _T(",");
	}
	
/*	
	// **************************************************************************
	// 에러 메시지 수신 유무 검사한다                                            
	// **************************************************************************
	if(strBody.Find("ERROR") >= 0)
	{
		sprintf(st_msg.c_normal_msg,"Label Print Status Received ERROR!");
		st_handler.cWndList->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
		return false;
	}
	// **************************************************************************
*/	
	//sprintf(cp_cmd, "%s", strBody);
	OnStringToChar(strBody, cp_cmd);
	nLength = strBody.GetLength();
	
	nReceivedDataCount = 0;
	nStartIndex = 0;
	
	// ==============================================================================
	// 실재로 읽은 BCR 데이터 전역 변수에 설정한다
	// ==============================================================================
	for(j = 0 ; j < 25 ; j++)
	{	
		nEndIndex   = strBody.Find(_T(","), nStartIndex);  // BCR 데이터 종료 위치 설정 
		
		if (nEndIndex == -1)
		{
			if (nLength - nStartIndex > 0)
			{
				nEndIndex = nLength - 1;
			}
			else
			{
				break;
			}
		}
		
		nResultData[j] = _wtoi(strBody.Mid(nStartIndex, nEndIndex - nStartIndex));  // BCR 데이터 문자열 파싱 
		nReceivedDataCount++;
		nStartIndex = nEndIndex + 1;
		
		OnPrintStatus(j, nResultData[j], nPort);
	}

	m_nPrintStatusBin = NVR_PASS;

	return true;
}

void CZebraPrint::Rewind_Mode_TCP(int port)
{
	CString str;
	str = "^XA^MMT^XZ";
	//sprintf(st_client_info[port].chSend,"^XA^MMT^XZ");//2015,0109

	OnStringToChar(str,st_client_info[port].chSend);


	::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + port, CLIENT_SEND, port); // LABEL CLIENT	
}

void CZebraPrint::Print_Complete_Set_TCP(int port)
{
/*	CString mstr_data[3];

	mstr_data[0] = "^XA^SXK,D,Y,Y";
	mstr_data[1].Format("%d", st_basic.n_Port[port]);
	mstr_data[2] = "^XZ";

	sprintf(st_client[port].chSend,"%s,%s,%s%s", mstr_data[0], st_basic.mstr_IP[port], mstr_data[1], mstr_data[2]);
	::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_2, CLIENT_SEND, port); // LABEL CLIENT	*/
}

void CZebraPrint::Print_Complete_Reset_TCP(int port)
{
/*	CString mstr_data[3];
	
	mstr_data[0] = "^XA^SXK,D,N,N";
	mstr_data[1].Format("%d", st_basic.n_Port[port]);
	mstr_data[2] = "^XZ";
	
	sprintf(st_client[port].chSend,"%s,%s,%s%s", mstr_data[0], st_basic.mstr_IP[port], mstr_data[1], mstr_data[2]);
	::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_2, CLIENT_SEND, port); // LABEL CLIENT	*/
}

void CZebraPrint::Printer_Status_TCP(int port)
{
	::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + port, CLIENT_CONNECT, port); // LABEL CLIENT	
	::Sleep(1000);
	CString str;
	str = "~HS";
	//sprintf(st_client_info[port].chSend,_T("%s"),_T("~HS"));
	OnStringToChar(str,st_client_info[port].chSend);
	::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + port, CLIENT_SEND, port); // LABEL CLIENT	
}


bool CZebraPrint::OnPrintStatus_Serial(CString strCheckData, int nPort)
{
/*	//3.22 Keyence
	int     nLength;  // 데이터 총 길이 저장 변수 
	CString strBody;  // BODY 데이터 저장 변수 
	
	char  cp_cmd[1024];
	
	int     j;
	int     nStartIndex, nEndIndex;  // BCR 데이터의 시작 및 종료 위치 저장 변수 
	int     nReceivedDataCount=0;
	CString strTempDataCount;  // 데이터 갯수 저장 변수 
	int     nResultData[100];
	
	CString str_Tmp;
	CString str_Tmp_Data;
	
	memset(&cp_cmd, 0, sizeof(cp_cmd));  // 데이터 저장 변수 초기화 
	
	// **************************************************************************
	// 수신 데이터 최소 길이 검사한다 [최소 3자리 이상]                          
	// **************************************************************************
	nLength = strCheckData.GetLength();
	if(nLength < 3)
	{
		return false;
	}
	// **************************************************************************
	strCheckData.TrimLeft("\r\n");
	strCheckData.TrimRight("\r\n");

	strBody = strCheckData;//.Mid(0, nLength - 1);  // [테일] 제거 
	strBody.TrimLeft(0x02);
	strBody.TrimRight(0x03);
	strBody = strBody + ",";
	
	// **************************************************************************
	// 에러 메시지 수신 유무 검사한다                                            
	// **************************************************************************
	if(strBody.Find("ERROR") >= 0)
	{
		sprintf(st_msg.c_normal_msg,"Label Print ERROR!!!");
		st_handler.cWndList->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
		return false;
	}
	// **************************************************************************
	
	sprintf(cp_cmd, "%s", strBody);
	nLength = strBody.GetLength();
		
	nReceivedDataCount = 0;
	nStartIndex = 0;

	// ==============================================================================
	// 실재로 읽은 BCR 데이터 전역 변수에 설정한다
	// ==============================================================================
		for(j = 0 ; j < 13 ; j++)
		{

			nEndIndex   = strBody.Find(",", nStartIndex);  // BCR 데이터 종료 위치 설정 
			
			if (nEndIndex == -1)
			{
				if (nLength - nStartIndex > 0)
				{
					nEndIndex = nLength - 1;
				}
				else
				{
					break;
				}
			}

			nResultData[j] = atoi(strBody.Mid(nStartIndex, nEndIndex - nStartIndex));  // BCR 데이터 문자열 파싱 
			nReceivedDataCount++;
			nStartIndex = nEndIndex + 1;

			if (st_barcode.n_barcode_hs_cnt == 0)
 			{
				OnPrintStatus(j, nResultData[j], nPort);
				if (j == 11) 
				{
					st_barcode.n_barcode_hs_cnt += 1;
					break;
				}
 			}
 			else if (st_barcode.n_barcode_hs_cnt == 1)
 			{
				OnPrintStatus(j+12, nResultData[j], nPort);
				if (j == 10) 
				{
					st_barcode.n_barcode_hs_cnt += 1;
					break;
				}
 			}
 			else if (st_barcode.n_barcode_hs_cnt == 2)
 			{
				OnPrintStatus(j+23, nResultData[j], nPort);
				if (j == 1) 
				{
					st_barcode.n_barcode_hs_cnt = 0;
					break;
				}
 			}
		}
		
*/		
	return true;
}

void CZebraPrint::Rewind_Mode_Serial(int port)
{
/*	CString strLabelPrint;
	
	CString str_print_data;
	
	str_print_data = "^XA^MMR^XZ";	//Rewind
	strLabelPrint.Format("%c%s%c", 0x02, str_print_data, 0x03);//바코드 리더기 읽기 시작 명령
	
	st_serial.mstr_snd[port] = strLabelPrint;
	::PostMessage(st_handler.hWnd, WM_DATA_SEND, port + 1, 0);
	::Sleep(100);
	
	if (st_handler.cWndList != NULL)  // 리스트 바 화면 존재
	{
		sprintf(st_msg.c_normal_msg,"[BCR Test] %s",strLabelPrint);
		st_handler.cWndList->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
	}*/
}

void CZebraPrint::Print_Complete_Set_Serial(int port)
{
/*	CString strLabelPrint;
	
	CString str_print_data;
	
	str_print_data = "^XA^SXK,A,Y,Y^XZ";//20120906
	strLabelPrint.Format("%c%s%c", 0x02, str_print_data, 0x03);//바코드 리더기 읽기 시작 명령
	
	st_serial.mstr_snd[port] = strLabelPrint;
	::PostMessage(st_handler.hWnd, WM_DATA_SEND, port, 0);
	::Sleep(100);
	
	if (st_handler.cWndList != NULL)  // 리스트 바 화면 존재
	{
		sprintf(st_msg.c_normal_msg,"[BCR Test] %s",strLabelPrint);
		st_handler.cWndList->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
	}*/
}

void CZebraPrint::Print_Complete_Reset_Serial(int port)
{
/*	CString strLabelPrint;
	
	CString str_print_data;
	
	str_print_data = "^XA^SXK,A,N,N^XZ";//20120906
	strLabelPrint.Format("%c%s%c", 0x02, str_print_data, 0x03);//바코드 리더기 읽기 시작 명령
	
	st_serial.mstr_snd[port] = strLabelPrint;
	::PostMessage(st_handler.hWnd, WM_DATA_SEND, port + 1, 0);
	::Sleep(100);
	
	if (st_handler.cWndList != NULL)  // 리스트 바 화면 존재
	{
		sprintf(st_msg.c_normal_msg,"[BCR Test] %s",strLabelPrint);
		st_handler.cWndList->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
	}*/
}


int CZebraPrint::Rewind_Mode_LPT1()
{
	CString str_print_data;
	
	str_print_data = "^XA^MMR^XZ";	//Rewind
	
	if(OnOutLptPort(str_print_data) == CTLBD_RET_ERROR)
	{
		return CTLBD_RET_ERROR;
	}
	
	return CTLBD_RET_GOOD;
}

int CZebraPrint::Print_Complete_Set_LPT1()	//20120630
{
	CString str_print_data;
	
	str_print_data = "^XA^SXK,B,Y,Y^XZ";//20120906
	
	if(OnOutLptPort(str_print_data) == CTLBD_RET_ERROR)
	{
		return CTLBD_RET_ERROR;
	}
	
	return CTLBD_RET_GOOD;
}

int CZebraPrint::Print_Complete_Reset_LPT1()	//20120630
{
	CString str_print_data;
	
	str_print_data = "^XA^SXK,B,N,N^XZ";//20120906
	
	if(OnOutLptPort(str_print_data) == CTLBD_RET_ERROR)
	{
		return CTLBD_RET_ERROR;
	}
	
	return CTLBD_RET_GOOD;
}

void CZebraPrint::Printer_Status_Serial(int port)
{
/*	CString strLabelPrint;
	
	CString str_print_data;
	
	str_print_data = "~HS";//20120906
	strLabelPrint.Format("%c%s%c", 0x02, str_print_data, 0x03);//바코드 리더기 읽기 시작 명령
	
	st_serial.mstr_snd[port] = strLabelPrint;
	::PostMessage(st_handler.hWnd, WM_DATA_SEND, port, 0);
	::Sleep(100);
	
	if (st_handler.cWndList != NULL)  // 리스트 바 화면 존재
	{
		sprintf(st_msg.c_normal_msg,"[BCR Test] %s",strLabelPrint);
		st_handler.cWndList->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
	}*/
}

void CZebraPrint::OnPrintStatus(int nRtnSplit, int nFlagName, int nPort)
{
	int nComPort = nPort;
	
	switch(nRtnSplit)
	{
		case JPL_COMM:
			m_nComm				= nFlagName;
			break;

		case JPL_PAPER_OUT_FLAG:	//용지 없음 
			m_nPaperOutFlag		= nFlagName;
			break;

		case JPL_PAUSE_FLAG:		//Pause 버튼 체크
			m_nPauseFlag		= nFlagName;
			break;

		case JPL_LABEL_LENGTH:
			m_nLabelLength		= nFlagName;
			break;

		case JPL_RCV_BUF_FORMAT_NO:
			m_nRcvBufFormatNo	= nFlagName;
			break;

		case JPL_BUF_FULL_FLAG:
			m_nBufFullFlag		= nFlagName;
			break;

		case JPL_COMM_DMODE_FLAG:
			m_nCommDModeFlag	= nFlagName;
			break;

		case JPL_PARTIAL_FORMAT_FLAG:
			m_nPartialFormatFlag= nFlagName;
			break;

		case JPL_UNUSED:
			m_nUnused			= nFlagName;
			break;

		case JPL_CORRUP_RAM_FLAG:
			m_nCorruptRamFlag	= nFlagName;
			break;

		case JPL_UNDER_TEM_RANGE:
			m_nUnderTemRange	= nFlagName;
			break;

		case JPL_OVER_TEM_RANGE:
			m_nOverTemRange		= nFlagName;
			break;

		case JPL_FUNC_SETTING:
			m_nFuncSetting			= nFlagName;
			break;

		case JPL_UNUSED1:
			m_nUnused				= nFlagName;
			break;

		case JPL_HEAD_UP_FLAG:		//Head Up, Down 체크
			m_nHeadUpFlag			= nFlagName;
			break;

		case JPL_RIBBON_OUTFLAG:	//리본 없음 
			m_nRibbonOutFlag		= nFlagName;
			break;

		case JPL_THEMAL_TRANSFER_MD_FLAG:
			m_nThermalTransferModeFlag = nFlagName;
			break;

		case JPL_PRINT_MODE:
			m_nPrintMode			= nFlagName;
			break;

		case JPL_PRINT_WIDTH_MODE:
			m_nPrintWidthMode		= nFlagName;
			break;

		case JPL_LABEL_WAIT_FLAG:
			m_nLabelWaitFlag		= nFlagName;
			break;

		case JPL_LABEL_REMAING:
			m_nLabelsRemaing		= nFlagName;
			break;

		case JPL_PRINT_MOVING_FLAG:
			m_nPrintMovingFlag		= nFlagName;
			break;

		case JPL_GRAPHIC_IMG_SAV_NO:
			m_nGraphicImgSavNo		= nFlagName;
			break;

		case JPL_PASS_WORD:
			m_nPsWord				= nFlagName;
			break;

		case JPL_RAM_INIT_STATE:
			m_nRamInitState			= nFlagName;
			break;	
	}
}


int CZebraPrint::OnBarcodeMake(int nDir, int nPort)
{
/*	int mn_existence;   // 파일 존재 여부 설정 변수 
	int n_by= -1, n_bc = -1;
	int pos[3];
	int length;
	
	char fileName[256];   // 출력할 파일 정보 저장 변수 
	char buffer[1024+1]; // 파일 내용 중 한 라인 정보 임시 저장 변수 
	
	CString str_print_data;
	CString str_tmp;
	CString str_description; // 선택된 파일 내용 임시 저장 변수 
	
	sprintf(fileName, "C:\\AMT8490\\BARCODE.TXT");
	
	mn_existence = access(fileName, 0);
	
	if (mn_existence != -1)  
	{
		FILE *fp = fopen ( fileName, "r" );
		
		if ( fp == (FILE *)NULL ) 
		{
			return CTLBD_RET_ERROR;
		}
		
		str_description.Empty();  // 파일 내용 저장 변수 초기화
		
		while (!feof(fp))
		{
			if (fgets(buffer, 1024, fp) == NULL)  break;
			
			if (strlen(buffer))   buffer[strlen(buffer)] = 0;
			strcat (buffer, "\r\n");
			str_description += buffer;
		}
		
		fclose(fp);
	}
	
	length = str_description.GetLength();
	
	pos[0] = str_description.Find("^BY", 0);
	pos[1] = str_description.Find("^FD", pos[0]);
	pos[2] = str_description.Find("^FS", pos[1]);
	
	if (nDir == 0)
	{
		str_tmp = "^BXN";
	}
	else
	{
		str_tmp = "^BXI";
	}
	
	str_print_data = str_description.Mid(0, pos[1]);
	str_print_data += st_barcode.str_barcode_tag;
	str_print_data += str_description.Mid(pos[2]+3, length - (pos[2]+3));
	
	if(st_basic.n_mode_barcode_print == 1)
	{	
//		st_serial.mstr_snd[nPort-1] = str_print_data;
	}
	else
	{	
		if(OnOutLptPort(str_print_data) == CTLBD_RET_ERROR)
		{
			return CTLBD_RET_ERROR;
		}
	}
	*/
	return CTLBD_RET_GOOD;
}

int CZebraPrint::OnOutLptPort(CString str_text)
{
	HANDLE hLpt;
	DWORD dwWritten; 
	
    hLpt = CreateFile(_T("LPT1:"), GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	
    if (hLpt == INVALID_HANDLE_VALUE)
	{ 
        AfxMessageBox(_T("Can not open LPT1 !!!"));
		
		return CTLBD_RET_ERROR;
	}
    else
	{ 
        WriteFile(hLpt, str_text, str_text.GetLength(), &dwWritten, NULL);
        CloseHandle(hLpt); 
    }
	
	return CTLBD_RET_GOOD;
}


void CZebraPrint::OnPrintAnswerMode(int nMethod, int nMode, int nPort)//2014.1104 ysj
{
	CString strPrint;
	CString strMsg;

	
	if(nPort == CLS_BCR_PRINTER1)
	{
		if (nMode == 0)
		strPrint = _T("^XA^SXK,D,Y,Y,192.168.0.16,9100^XZ");	 // print 응답 확인 
		else 
		strPrint = _T("^XA^SXK,D,N,N,192.168.0.16,9100^XZ");	 // print 응답 확인 
	}


// 	if (nMode == 0)
// 	{
// 		strPrint = "^XA^SXK,D,Y,Y,192.168.1.16,9101^XZ";	 // print AA´a E®AI ¸ðμa 
// 	}
// 	else
// 	{
// 		strPrint = "^XA^SXK,D,N,N,192.168.1.16,9101^XZ";	// print AA´a E®AI ¾ECO.
// 	}
	
	if (nMethod == 0)
	{
		clsZebra.OnOutLptPort(strPrint);
	}
	else if (nMethod == 1)
	{
//		st_serial.str_snd[nPort] = strPrint;

//		::SendMessage(st_handler.hWnd, WM_DATA_SEND, nPort, 0);
	}
	else
	{
		//sprintf(st_client_info[nPort].chSend,"%s",strPrint);
		OnStringToChar( strPrint, st_client_info[nPort].chSend);
		::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + nPort, CLIENT_SEND, nPort); // LABEL CLIENT	
	}

	if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
	{
// 		sprintf(st_msg.c_normal_msg,"[PRINT_Answer] %s", strPrint);
// 		st_handler_info.cWndList->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청

		strMsg.Format(_T("[PRINT_Answer] %s"), strPrint);
		clsMem.OnNormalMessageWrite(strMsg);
	}
}


void CZebraPrint::OnPrintLabelMode(int nMethod, int nMode, int nPort)
{
	CString strPrint;
	CString strMsg;
	
	if (nMode == 0)
	{
		strPrint = "^XA^MMT^XZ";	//Tear off
	}
	else if (nMode == 1)
	{
		strPrint = "^XA^MMP^XZ";	//Peel off
	}
	else
	{
		strPrint = "^XA^MMR^XZ";	//Rewind
	}
	
	if (nMethod == 0)
	{
		clsZebra.OnOutLptPort(strPrint);
	}
	else if (nMethod == 1)
	{
//		st_serial.str_snd[nPort] = strPrint;
		
		::SendMessage(st_handler_info.hWnd, WM_DATA_SEND, nPort, 0);
	}
	else
	{
		//sprintf(st_client_info[nPort].chSend,"%s",strPrint);
		OnStringToChar( strPrint, st_client_info[nPort].chSend );
		::SendMessage(st_handler_info.hWnd, WM_ZEBRA_CLIENT_MSG + nPort, CLIENT_SEND, 0); // LABEL CLIENT	
	}
	
	if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
	{
// 		sprintf(st_msg.c_normal_msg,"[PRINT_LabelMode] %s", strPrint);
// 		st_handler_info.cWndList->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청

		strMsg.Format(_T("[PRINT_LabelMode] %s"), strPrint);
		clsMem.OnNormalMessageWrite(strMsg);
	}
}


void CZebraPrint::OnPrintPauseMode(int nMethod, int nMode, int nPort)
{
	CString strPrint;
	CString strMsg;
	
	if (nMode == 0)
	{
		strPrint = "^XA~PP^XZ";	//
	}
	else
	{
		strPrint = "^XA~PS^XZ";	//
	}

	if (nMethod == 0)
	{
		clsZebra.OnOutLptPort(strPrint);
	}
	else if (nMethod == 1)
	{
//		st_serial.str_snd[nPort] = strPrint;
		
//		::SendMessage(st_handler.hWnd, WM_DATA_SEND, nPort, 0);
	}
	else
	{
		//sprintf(st_client_info[nPort].chSend,"%s",strPrint);
		OnStringToChar( strPrint, st_client_info[nPort].chSend );
		::SendMessage(st_handler_info.hWnd, WM_ZEBRA_CLIENT_MSG + nPort, CLIENT_SEND, 0); // LABEL CLIENT	
	}
	
	if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
	{
// 		sprintf(st_msg.c_normal_msg,"[PRINT_PauseMode] %s", strPrint);
// 		st_handler_info.cWndList->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청

		strMsg.Format(_T("[PRINT_PauseMode] %s"), strPrint);
		clsMem.OnNormalMessageWrite(strMsg);
	}
}


void CZebraPrint::OnPrintStatusCheck(int nMethod, int nPort) // 20140906 jtkim
{
	CString strPrint;
	CString strMsg;
	
	m_nPrintStatusBin = NVR_NONE;

	strPrint = "~HS";//"^XA~HS^XZ";	//
//	strPrint += "^XA^SXK,A,Y,Y^XZ";
	
	if (nMethod == 0)
	{
		clsZebra.OnOutLptPort(strPrint);
	}
	else if (nMethod == 1)
	{
//		st_serial.str_snd[nPort] = strPrint;
		
//		::SendMessage(st_handler.hWnd, WM_DATA_SEND, nPort, 0);
	}
	else
	{
		//sprintf(st_client_info[nPort].chSend,"%s",strPrint);
		OnStringToChar( strPrint, st_client_info[nPort].chSend );
		::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + nPort, CLIENT_SEND, nPort); // LABEL CLIENT	

	}
	
	if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
	{
// 		sprintf(st_msg.c_normal_msg,"[PRINT_Status] %s", strPrint);
// 		st_handler_info.cWndList->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청

		strMsg.Format(_T("[PRINT_Status] %s"), strPrint);
		clsMem.OnNormalMessageWrite(strMsg);
	}
}


CString	CZebraPrint::OnPrintLabelOffset(int nOffsetX, int nOffsetY, CString strText)
{
	CString strData;
	CString strOld;
	CString strNew;

	int nPos;
	int nX, nY;

	strData = strText;
	nPos = strData.Find(_T("LH"), 0);

	strOld = strData.Mid(nPos, 9);

	nX = _wtoi(strOld.Mid(2, 3));
	nY = _wtoi(strOld.Mid(6, 3));

	strNew.Format(_T("LH%03d,%03d"), nX + nOffsetX, nY + nOffsetY);
	strNew = strNew + _T("^MMT");
	strData.Replace(strOld, strNew);

	return strData;
}


void CZebraPrint::OnPrintOutput(int nMethod, int nPort, int nOffsetX, int nOffsetY, CString strText)
{
	CString strPrint;
	CString strMsg;
	
	m_nPrintStatusBin = NVR_NONE;
	
	strPrint = OnPrintLabelOffset(nOffsetX, nOffsetY, strText);
	
	if (nMethod == 0)
	{
		clsZebra.OnOutLptPort(strPrint);
	}
	else if (nMethod == 1)
	{
//		st_serial.str_snd[nPort] = strPrint;
		
//		::SendMessage(st_handler.hWnd, WM_DATA_SEND, nPort, 0);
	}
	else
	{
		//sprintf(st_client_info[nPort].chSend,"%s",strPrint);
		OnStringToChar( strPrint, st_client_info[nPort].chSend );
		::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + nPort, CLIENT_SEND, nPort); // LABEL CLIENT	
	}
	
	if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
	{
// 		sprintf(st_msg.c_normal_msg,"[PRINT_Output] %s", strPrint);
// 		st_handler.cWndList->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
// 		CString sTmp;
// 		sTmp.Format(_T("[PRINT_Output] %s"), strPrint);
// 		Func.On_LogFile_Add(LOG_TOTAL, sTmp);

		strMsg.Format(_T("[PRINT_Output] %s"), strPrint);
		clsMem.OnNormalMessageWrite(strMsg);

	}
}


void CZebraPrint::OnPrintCalibration(int nMethod, int nPort)
{
	CString strPrint;
	CString strMsg;
	
	m_nPrintStatusBin = NVR_NONE;
	
	strPrint = "^XA~JC^XZ";	//
	
	if (nMethod == 0)
	{
		clsZebra.OnOutLptPort(strPrint);
	}
	else if (nMethod == 1)
	{
//		st_serial.str_snd[nPort] = strPrint;
		
//		::SendMessage(st_handler.hWnd, WM_DATA_SEND, nPort, 0);
	}
	else
	{
		//sprintf(st_client_info[nPort].chSend,"%s",strPrint);
		OnStringToChar( strPrint, st_client_info[nPort].chSend);
		::SendMessage(st_handler_info.hWnd, WM_ZEBRA_CLIENT_MSG + nPort, CLIENT_SEND, 0); // LABEL CLIENT	
	}
	
	if (st_handler_info.cWndList != NULL)  // 리스트 바 화면 존재
	{
// 		sprintf(st_msg.c_normal_msg,"[PRINT_Cal] %s", strPrint);
// 		st_handler_info.cWndList->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청

		strMsg.Format(_T("[PRINT_Cal] %s"), strPrint);
		clsMem.OnNormalMessageWrite(strMsg);
	}
}


void CZebraPrint::OnPrintComplete(CString strRev)
{
	int nNum;

	nNum = -1;
	nNum = strRev.Find(_T("PQ JOB COMPLETED"), 0);
	
	if (nNum >= 0)
	{
		m_nPrintStatusBin = NVR_PASS;
	}
	else
	{
		m_nPrintStatusBin = NVR_FAIL;
	}

// 	if (st_handler.cWndList != NULL)  // ¸®½ºÆ® ¹U E­¸e A¸Ac
// 	{
// 		sprintf(st_msg.c_normal_msg,"[PRINT_COMPLETE] %s", strRev);
// 		st_handler.cWndList->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // μ¿AU ½CÆÐ Aa·A ¿aA≫
// 	}

}



////2015.0413
void CZebraPrint::SetDarkness_TCP(int nPort)
{
	CString mstr_data[2];
	CString str_print_data;
	int i;	
	
	for(i=0; i<2; i++)
	{
		mstr_data[i] = "";
	}
	str_print_data = "";

	mstr_data[0] = "~SD";

	::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + nPort, CLIENT_CONNECT, nPort); // LABEL CLIENT	
	::Sleep(500);
	if(nPort == CLS_BCR_PRINTER1)
	{
		str_print_data.Format(_T("%d"), st_barcode.mn_labelset_sd[0]);

		mstr_data[1] = mstr_data[0] + str_print_data;
		//sprintf(st_client_info[nPort].chSend,"%s",mstr_data[1]);//2015,0109
		OnStringToChar( mstr_data[1], st_client_info[nPort].chSend );
	}

	::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + nPort, CLIENT_SEND, nPort); // LABEL CLIENT	

}

void CZebraPrint::LabelTop_TCP(int nPort)
{
	CString mstr_data[3];
	CString str_print_data;
	int i;	
	
	for(i=0; i<3; i++)
	{
		mstr_data[i] = "";
	}
	str_print_data = "";
	
	mstr_data[0] = "^XA^LT";
		
	::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + nPort, CLIENT_CONNECT, nPort); // LABEL CLIENT	
	::Sleep(500);
	if(nPort == CLS_BCR_PRINTER1)
	{
		str_print_data.Format( _T("%d"), st_barcode.mn_labelset_lt[0]);
		mstr_data[1] = mstr_data[0] + str_print_data;
		mstr_data[2] = mstr_data[1] + _T("^XZ");
		//sprintf(st_client_info[nPort].chSend,"%s",mstr_data[2]);//2015,0109
		OnStringToChar(mstr_data[2], st_client_info[nPort].chSend);
	}

	
	::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + nPort, CLIENT_SEND, nPort); // LABEL CLIENT	
	
}

void CZebraPrint::LabelHome_TCP(int nPort)
{
	CString mstr_data[3];
	CString str_print_data;
	int i;	

	for(i=0; i<3; i++)
	{
		mstr_data[i] = "";
	}
	str_print_data = "";

	mstr_data[0] = "^XA^LH";
	
	if(nPort == CLS_BCR_PRINTER1)
	{
		str_print_data.Format(_T("%d,%d"), st_barcode.mn_labelset_lhx[0], st_barcode.mn_labelset_lhy[0]);
		mstr_data[1] = mstr_data[0] + str_print_data;
		mstr_data[2] = mstr_data[1] + _T("^XZ");
		//sprintf(st_client_info[nPort].chSend,"%s",mstr_data[2]);//2015,0109
		OnStringToChar( mstr_data[2], st_client_info[nPort].chSend );
	}

	
	::SendMessage(st_handler_info.hWnd, WM_CLIENT_MSG + nPort, CLIENT_SEND, nPort); // LABEL CLIENT	
	
}
////


void CZebraPrint::OnStringToChar(CString strData, char chData[])
{
	int nCnt;
	wchar_t *chBuf;

	chBuf = strData.GetBuffer(strData.GetLength());
	nCnt  = WideCharToMultiByte(CP_ACP, 0, chBuf, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, chBuf, -1, chData, nCnt, 0, 0);
}