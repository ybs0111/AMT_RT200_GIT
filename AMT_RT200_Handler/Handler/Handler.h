
// Handler.h : Handler ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CHandlerApp:
// �� Ŭ������ ������ ���ؼ��� Handler.cpp�� �����Ͻʽÿ�.

class CHandlerApp : public CWinApp
{
public:
	CString m_strVer;

public:
	CHandlerApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CHandlerApp theApp;
