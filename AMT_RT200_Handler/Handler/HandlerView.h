
// HandlerView.h : CHandlerView 클래스의 인터페이스
//

#pragma once

#include "resource.h"


class CHandlerView : public CFormView
{
protected: // serialization에서만 만들어집니다.
	CHandlerView();
	DECLARE_DYNCREATE(CHandlerView)

public:
	enum{ IDD = IDD_ABOUTBOX };

// 특성입니다.
public:
	CHandlerDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현입니다.
public:
	virtual ~CHandlerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	CButton m_btnTest;
	afx_msg void OnBnClickedButton1();
};

#ifndef _DEBUG  // HandlerView.cpp의 디버그 버전
inline CHandlerDoc* CHandlerView::GetDocument() const
   { return reinterpret_cast<CHandlerDoc*>(m_pDocument); }
#endif

