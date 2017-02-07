#pragma once



// CScreenList form view
#include "Variable.h"
#include "XPGroupBox.h"
#include "xShadeButton.h"	// 조그 버튼 생성 클래스 추가
#include ".\\Union\\Grid\\GridCtrl.h"
#include ".\\Union\\Grid\\GridURLCell.h"
#include ".\\Union\\Grid\\GridCellCombo.h"
#include ".\\Union\\Grid\\GridCellCheck.h"
#include ".\\Union\\Grid\\GridCellNumeric.h"
#include ".\\Union\\Grid\\GridCellDateTime.h"
#include "BtnST.h"
class CScreenList : public CFormView
{
	DECLARE_DYNCREATE(CScreenList)

public:
	int					m_nListType;

	CImageList			*m_pImagelist;

	CGridCtrl			m_pGridData;

	CTreeCtrl			m_treeFileList;
	CTreeCtrl			m_treeFileType;

	CXPGroupBox			m_groupFileType;
	CXPGroupBox			m_groupFileList;
	CXPGroupBox			m_groupData;

	HICON				m_hIcon;
	CBitmap				m_bitmap;
	CImageList			m_imageList;

	CButtonST			m_btnSave;

	void				OnInitGroupBox();
	void				OnInitTreeFileList();
	void				OnInitTreeFileType();
	void				OnInitButton();
	void				OnInitGridData();
	void				OnTreeListData(int nList);

public:
	CScreenList();           // protected constructor used by dynamic creation
	virtual ~CScreenList();

public:
	enum { IDD = IDD_SCREEN_LIST };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL DestroyWindow();
	virtual void OnInitialUpdate();
	afx_msg void OnSelchangedTreeFileList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickedBtnSave();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSelchangedTreeFileType(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void PostNcDestroy();
	afx_msg void OnDestroy();
};


