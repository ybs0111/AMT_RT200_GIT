
// HandlerView.h : CHandlerView Ŭ������ �������̽�
//

#pragma once

#include "resource.h"


class CHandlerView : public CFormView
{
protected: // serialization������ ��������ϴ�.
	CHandlerView();
	DECLARE_DYNCREATE(CHandlerView)

public:
	enum{ IDD = IDD_ABOUTBOX };

// Ư���Դϴ�.
public:
	CHandlerDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual void OnInitialUpdate(); // ���� �� ó�� ȣ��Ǿ����ϴ�.

// �����Դϴ�.
public:
	virtual ~CHandlerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	CButton m_btnTest;
	afx_msg void OnBnClickedButton1();
};

#ifndef _DEBUG  // HandlerView.cpp�� ����� ����
inline CHandlerDoc* CHandlerView::GetDocument() const
   { return reinterpret_cast<CHandlerDoc*>(m_pDocument); }
#endif

