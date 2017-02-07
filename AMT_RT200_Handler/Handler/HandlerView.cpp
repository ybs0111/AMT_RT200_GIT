
// HandlerView.cpp : CHandlerView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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

// CHandlerView ����/�Ҹ�

CHandlerView::CHandlerView()
	: CFormView(CHandlerView::IDD)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

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
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CFormView::PreCreateWindow(cs);
}

void CHandlerView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}


// CHandlerView ����

#ifdef _DEBUG
void CHandlerView::AssertValid() const
{
	CFormView::AssertValid();
}

void CHandlerView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CHandlerDoc* CHandlerView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHandlerDoc)));
	return (CHandlerDoc*)m_pDocument;
}
#endif //_DEBUG


// CHandlerView �޽��� ó����

