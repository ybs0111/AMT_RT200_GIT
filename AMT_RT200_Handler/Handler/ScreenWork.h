
#pragma once



// CScreenWork �� ���Դϴ�.
#include "WorkTab.h"
#include "XPGroupBox.h"
#include "BtnST.h"

class CScreenWork : public CFormView
{
	DECLARE_DYNCREATE(CScreenWork)

public :
	CImageList			*m_pImagelist;

	CTreeCtrl			m_treeView;

	CXPGroupBox			m_groupWorkMode;

	HICON				m_hIcon;
	CBitmap				m_bitmap;

	CImageList			m_imageList;

	void				OnInitGroupBox();
	void				OnInitLabel();
	void				OnInitButton();
	void				OnInitTree();

public:
	CWorkTab			m_tabWork;

	CScreenWork();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CScreenWork();

public:
	enum { IDD = IDD_SCREEN_WORK };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL	DestroyWindow();
	virtual void	OnInitialUpdate();
	afx_msg BOOL	OnEraseBkgnd(CDC* pDC);
	afx_msg void	OnSelchangedTreeView(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void	PostNcDestroy();
	afx_msg void	OnDestroy();
};


