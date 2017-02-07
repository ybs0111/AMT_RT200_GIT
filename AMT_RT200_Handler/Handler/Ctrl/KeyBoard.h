// KeyBoard.h : KeyBoard DLL�� �⺻ ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CKeyBoardApp
// �� Ŭ������ ������ ������ KeyBoard.cpp�� �����Ͻʽÿ�.
//

class CKeyBoardApp : public CWinApp
{
public:
	CKeyBoardApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern "C" __declspec(dllexport)void			KeyBoard(CString *strKey);
extern "C" __declspec(dllexport)void			KeyPadI(int nMin, int nMax, int *nKey);
extern "C" __declspec(dllexport)void			KeyPadD(double dMin, double dMax, double *dKey);
