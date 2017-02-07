
// HandlerView.cpp : CHandlerView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Handler.h"
#endif

#include "HandlerDoc.h"
#include "HandlerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHandlerView

IMPLEMENT_DYNCREATE(CHandlerView, CFormView)

BEGIN_MESSAGE_MAP(CHandlerView, CFormView)

END_MESSAGE_MAP()

// CHandlerView 생성/소멸

CHandlerView::CHandlerView()
	: CFormView(CHandlerView::IDD)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CHandlerView::~CHandlerView()
{
}

void CHandlerView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_btnTest);
}

BOOL CHandlerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CHandlerView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}


// CHandlerView 진단

#ifdef _DEBUG
void CHandlerView::AssertValid() const
{
	CFormView::AssertValid();
}

void CHandlerView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CHandlerDoc* CHandlerView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHandlerDoc)));
	return (CHandlerDoc*)m_pDocument;
}
#endif //_DEBUG


// CHandlerView 메시지 처리기

