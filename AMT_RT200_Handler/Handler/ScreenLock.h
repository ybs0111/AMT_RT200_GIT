#pragma once

#include ".\\Controls\\PictureEx.h"      // GIF �ε� Ŭ����
#include ".\\Controls\\MultiColorStatic.h"
#include ".\\Controls\\BannerStatic.h"      // ��� ���ڿ� Ŭ����
#include "BtnST.h"
// CScreenLock �� ���Դϴ�.

class CScreenLock : public CFormView
{
	DECLARE_DYNCREATE(CScreenLock)

public:
	CScreenLock();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CScreenLock();
	//==============================================================//
	// ��� ���� Ŭ���� ���� ����
	BOOL				m_bBold;
	BOOL				m_bItalic;
	BOOL				m_bUnderlined;

	int					m_nCount;
	int					m_nWidth;
	int					m_nHeight;
	int					m_nBannerWidth;
	int					m_nBannerHeight;
	int					m_nBannerTop;
	int					m_nBannerLeft;
	int					m_nSpeed;

	COLORREF			m_crBackColor;
	COLORREF			m_crForeColor;
	//--------------------------------------------------------------//

	CPictureEx			mp_gifSysLock;
	CBannerStatic		mp_stcWarning;

	CButtonST			m_btnSysUnLock;
	void				OnInitButton();
	void				OnSetPicture(); 
//	void				OnSetBanner(); 

	//==============================================================//
	// ��� ���� Ŭ���� �Լ�
	void				OnSetInitVariable();  // ���� �ʱ�ȭ
	void				OnSetBanner();  // Į�� ��� ����

	void				OnSetScrollSpeed(int nzSpeed);  // ��� ��ũ�� �ӵ� ����
	void				OnChangeColor(int nzType, COLORREF crzColor);  // ��� Į�� ����
	void				OnChangeOrientation(int nzOrient);  // ��� ��� ���� ����
	void				OnBannerRemoveAll();  // ��ʷ� ��µ� ��� ���ڿ� ����
	void				OnAddString(CString szBanner);  // ��� ���ڿ� �߰�
	//--------------------------------------------------------------//

public:
	enum { IDD = IDD_SCREEN_LOCK };
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
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedBtnSysunlock();
	virtual BOOL DestroyWindow();
	virtual void PostNcDestroy();
	afx_msg void OnDestroy();
};


