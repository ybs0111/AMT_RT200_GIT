// Interface.h : Interface DLL의 기본 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.


// CInterfaceApp
// 이 클래스의 구현을 보려면 Interface.cpp를 참조하십시오.
//

class CInterfaceApp : public CWinApp
{
public:
	CInterfaceApp();

// 재정의입니다.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

// DLL Open
extern "C" __declspec(dllexport)void		Open(CWnd *pWnd);
// DLL Close
extern "C" __declspec(dllexport)void		Close();
// RFID Reader 시작
extern "C" __declspec(dllexport)void		RfidCreate();
// RFID Reader 종료
extern "C" __declspec(dllexport)void		RfidDelete();
// DLL UI에서 Tag수량 및 정보 설정가능
// RFID Tag 정보 설정 (Tag Num, Tag 시작위치, Tag 길이, 쓰기 사용유무(0만 적용))
extern "C" __declspec(dllexport)void		RfidTagInfo(int nItem, int nStart, int nNum, int nWriteYesNo);
// 마지막 RFID 정보 요청 (채널, Tag번호) 
extern "C" __declspec(dllexport)void		RfidReadData(int nChan, int nItem);
// 마지막 RFID 정보 요청 (채널)
extern "C" __declspec(dllexport)void		RfidReadAllData(int nChan);
// 마지막 RFID Error 요청
extern "C" __declspec(dllexport)void		RfidLastErrorMsg();
// RFID Tag 읽기 (채널)
extern "C" __declspec(dllexport)void		RfidReadTag(int nChan);
// Tag 항목별 RFID Tag 정보 쓰기 (채널, Tag Num, 데이터)
extern "C" __declspec(dllexport)void		RfidWriteData(int nChan, int nItem, CString strMsg);
// 전체 RFID Tag 정보 쓰기 (채널, 데이터)
extern "C" __declspec(dllexport)void		RfidWriteAllData(int nChan, CString *strMsg);
// BARCODE 통신포트 시작(1 ~ 5)
extern "C" __declspec(dllexport)int			BarcodeCreate(int nBarcode);
// BARCODE 통신포트 종료(1 ~ 5)
extern "C" __declspec(dllexport)void		BarcodeDelete(int nBarcode);
// BARCODE 정보 읽기 (1 ~ 5)
extern "C" __declspec(dllexport)void		BarcodeRead(int nBarcode);
// BARCODE 정보 읽기 (1 ~ 5, Trigger)
extern "C" __declspec(dllexport)void		BarcodeReadT(int nBarcode, CString strTrigger);
// 마지막 BARCODE Error 요청 
extern "C" __declspec(dllexport)void		BarcodeLastErrorMsg();