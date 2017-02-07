#pragma once


// CDialog_Image_View 대화 상자입니다.
#include "Picture.h"

class CDialog_Image_View : public CDialog
{
	DECLARE_DYNAMIC(CDialog_Image_View)

public:
	CString				m_strFileName;
	CPicture			m_imgThumb;

	BOOL				Create();

public:
	CDialog_Image_View(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialog_Image_View();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_IMAGE_VIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
};
