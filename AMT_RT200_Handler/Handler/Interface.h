// Interface.h : Interface DLL�� �⺻ ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CInterfaceApp
// �� Ŭ������ ������ ������ Interface.cpp�� �����Ͻʽÿ�.
//

class CInterfaceApp : public CWinApp
{
public:
	CInterfaceApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

// DLL Open
extern "C" __declspec(dllexport)void		Open(CWnd *pWnd);
// DLL Close
extern "C" __declspec(dllexport)void		Close();
// RFID Reader ����
extern "C" __declspec(dllexport)void		RfidCreate();
// RFID Reader ����
extern "C" __declspec(dllexport)void		RfidDelete();
// DLL UI���� Tag���� �� ���� ��������
// RFID Tag ���� ���� (Tag Num, Tag ������ġ, Tag ����, ���� �������(0�� ����))
extern "C" __declspec(dllexport)void		RfidTagInfo(int nItem, int nStart, int nNum, int nWriteYesNo);
// ������ RFID ���� ��û (ä��, Tag��ȣ) 
extern "C" __declspec(dllexport)void		RfidReadData(int nChan, int nItem);
// ������ RFID ���� ��û (ä��)
extern "C" __declspec(dllexport)void		RfidReadAllData(int nChan);
// ������ RFID Error ��û
extern "C" __declspec(dllexport)void		RfidLastErrorMsg();
// RFID Tag �б� (ä��)
extern "C" __declspec(dllexport)void		RfidReadTag(int nChan);
// Tag �׸� RFID Tag ���� ���� (ä��, Tag Num, ������)
extern "C" __declspec(dllexport)void		RfidWriteData(int nChan, int nItem, CString strMsg);
// ��ü RFID Tag ���� ���� (ä��, ������)
extern "C" __declspec(dllexport)void		RfidWriteAllData(int nChan, CString *strMsg);
// BARCODE �����Ʈ ����(1 ~ 5)
extern "C" __declspec(dllexport)int			BarcodeCreate(int nBarcode);
// BARCODE �����Ʈ ����(1 ~ 5)
extern "C" __declspec(dllexport)void		BarcodeDelete(int nBarcode);
// BARCODE ���� �б� (1 ~ 5)
extern "C" __declspec(dllexport)void		BarcodeRead(int nBarcode);
// BARCODE ���� �б� (1 ~ 5, Trigger)
extern "C" __declspec(dllexport)void		BarcodeReadT(int nBarcode, CString strTrigger);
// ������ BARCODE Error ��û 
extern "C" __declspec(dllexport)void		BarcodeLastErrorMsg();