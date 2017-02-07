#pragma once


// CDialog_Image_View ��ȭ �����Դϴ�.
#include "Picture.h"

class CDialog_Image_View : public CDialog
{
	DECLARE_DYNAMIC(CDialog_Image_View)

public:
	CString				m_strFileName;
	CPicture			m_imgThumb;

	BOOL				Create();

public:
	CDialog_Image_View(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialog_Image_View();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_IMAGE_VIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
};
