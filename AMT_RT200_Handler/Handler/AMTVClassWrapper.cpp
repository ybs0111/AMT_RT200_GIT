#include "stdafx.h"
#include "AMTVClassWrapper.h"

#import "AMTLabelVisionDLL.tlb" no_namespace named_guids

AMTVClassWrapper::AMTVClassWrapper(void)
{
	CoInitialize(NULL);
	HRESULT hr = CoCreateInstance(CLSID_VisionClassImp, 
		NULL, 
		CLSCTX_INPROC_SERVER, 
		IID_IVisionClass, 
		reinterpret_cast<void**>(&mp_VisionClass));
	if(FAILED(hr))
	{
		m_isSucceeded = false;
		CoUninitialize();
	}
	else
	{
		m_isSucceeded = true;
		OutputDebugString(_T("Succeeded VisionClass.\n"));
	}
}

AMTVClassWrapper::~AMTVClassWrapper(void)
{
	CoUninitialize();
}

void AMTVClassWrapper::ShowMessage()
{
	if(NULL != mp_VisionClass)
	{
		CString title  = _T("AMTVClassWrapper");
		CString label = _T("C# DLL ShowMessage을 호출했다");
		
		_bstr_t bstrTitle = (LPCTSTR)title;
		_bstr_t bstrLabel = (LPCTSTR)label;
		
		//mp_VisionClass->ShowMessage(bstrTitle, bstrLabel);
	}
}

int AMTVClassWrapper::Add(int a, int b)
{
	if(NULL != mp_VisionClass)
	{
		//return mp_VisionClass->Add(a, b);
	}
	return 0;
}

void AMTVClassWrapper::ShowForm()
{
	if(NULL != mp_VisionClass)
	{
		mp_VisionClass->AMTV_ShowForm();
	}
}

void AMTVClassWrapper::HideForm()
{
	if(NULL != mp_VisionClass)
	{
		mp_VisionClass->AMTV_HideForm();
	}
}

BOOL AMTVClassWrapper::ChangeChannel(int ChannelNum)
{
	if(NULL != mp_VisionClass)
	{
		//return mp_VisionClass->AMTV_ChangeChannel(ChannelNum);
	}
	return FALSE;
}

BOOL AMTVClassWrapper::Result(int nNum, BSTR* str)
{
	if(NULL != mp_VisionClass)
	{
		return mp_VisionClass->AMTV_Result(nNum, str);
	}
	return FALSE;
}

BOOL AMTVClassWrapper::EdgeScore(double *lEdgeScore)
{
	if(NULL != mp_VisionClass)
	{
		//return mp_VisionClass->AMTV_EdgeScore(lEdgeScore);
	}
	return FALSE;
}

BOOL AMTVClassWrapper::PatternMatching(int nNum, double *OffsetX, double *OffsetY)
{
	if(NULL != mp_VisionClass)
	{
		//return mp_VisionClass->AMTV_PatternMatching(nNum, OffsetX, OffsetY);
	}
	return FALSE;
}

void AMTVClassWrapper::Exit()
{
	if(NULL != mp_VisionClass)
	{
		mp_VisionClass->AMTV_Exit();
	}
}