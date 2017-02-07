#pragma once


// CDialog_About ��ȭ �����Դϴ�.
#include ".\\Controls\\StarWarsCtrl.h"  // ��Ÿ���� ��Ʈ�� Ŭ����
#include "BtnST.h"

class CDialog_About : public CDialog
{
	DECLARE_DYNAMIC(CDialog_About)

public:
	CDialog_About(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialog_About();

	CStarWarsCtrl	mp_imgAbout;

	CButtonST			m_btnUdate;
	CButtonST			m_btnAboutOK;

	void OnSetButton();  // Į�� ��ư ����
	void OnSetUserControl(); 

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_ABOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnUpdate();
	afx_msg void OnBnClickedBtnAboutOk();
};
