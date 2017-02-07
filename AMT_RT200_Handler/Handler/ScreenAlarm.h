#pragma once



// CScreenAlarm �� ���Դϴ�.
#include ".\\Controls\\BmpImageST.h"		// �˶� ��Ʈ�� �ε� Ŭ���� �߰� 
#include "GradientStatic.h"
#include "XPGroupBox.h"
#include "ThumbNailControl.h"
#include ".\\Union\\Grid\\GridCtrl.h"
#include ".\\Union\\Grid\\GridURLCell.h"
#include ".\\Union\\Grid\\GridCellCombo.h"
#include ".\\Union\\Grid\\GridCellCheck.h"
#include ".\\Union\\Grid\\GridCellNumeric.h"
#include ".\\Union\\Grid\\GridCellDateTime.h"
#include "BtnST.h"

class	 CDialog_Image_View;
class	 CDialogAlarmIO;
class    CDialogAlarmMotor;

class CScreenAlarm : public CFormView
{
	DECLARE_DYNCREATE(CScreenAlarm)
private:
	CDialogAlarmIO				*m_pAlarmIO;
	CDialogAlarmMotor			*m_pAlarmMotor;

	CString						*m_strJamCode;
	int							m_nLastAlarm;

public:
	int							m_nRemoveYesNo[10];

	int							m_nInCnt;
	int							m_nOutCnt;
	int							m_nIn[16];
	int							m_nOut[16];
	int							m_nMotorCnt;
	int							m_nMotor;

	CGridCtrl					m_pGridRepair;

	CDialog_Image_View			*m_pImageView;

	CBmpImageST					m_imgAlarm;

	CThumbNailControl			m_listThumb;

	CButtonST					m_btnPrevAlarm[10];
	CButtonST					m_chkYes;
	CButtonST					m_chkNo;
	CButtonST					m_btnAlarmClean;
	CButtonST					m_btnAlarmIO;
	CButtonST					m_btnAlarmMotor;
	CButtonST					m_btnDoorOpen;

	CXPGroupBox					m_groupPrevAlarm;
	CXPGroupBox					m_groupCode;

	CGradientStatic				m_msgAlarmPart;
	CGradientStatic				m_msgAlarmPartData;
	CGradientStatic				m_msgAlarmCode;
	CGradientStatic				m_msgAlarmCodeData;

	void						OnAlarmInfoDisplay(CString strCode);
	int							OnAlarmDataLoading();
	void						OnAlarmImageStyle(CString strPath);
	void						OnInitGroupBox();
	void						OnInitLabel();
	void						OnInitButton();
	CString						OnAlarmDisplayInfoLoad(CString strPath, CString strDataFile, CString strActionFile, CString strCode);
	CString						OnAlarmBmpDataLoad(CString strPath, CString strDataFile, CString strCode);       // �߻� �˶� ���� ǥ�ÿ� �׸� ���ϸ� ���� �Լ�
	CString						OnAlarmStyleDataLoad(CString strPath, CString strDataFile, CString strCode);     // �˶� �߻� ���� ǥ�� ���� �� ��� ��ġ ���� �Լ�
	CString						OnAlarmRepairDataLoad(CString strPath, CString strActionFile, CString strCode);  // �߻��� �˶��� ���� ��ġ ���� ���� �Լ�
	void						OnAlarmIODataLoad(CString strPath, CString strIOFile, CString strCode); 
	void						OnAlarmMotorDataLoad(CString strPath, CString strMotorFile, CString strCode); 
	void						OnAlarmImageDataLoad(CString strPath, CString strImageFile, CString strCode); 
	void						OnAlarmPrevButtonListSet(int nCount);
	void						OnAlarmPrevListSet();
	CString						OnAlarmPrevAlarmCheck(int nCnt);
	void						OnAlarmPrevListCaptionSet(int nCnt);
	void						OnAlarmPrevAlarmDisplay(int nAlarmPos);
	void						OnGridRepair();
	void						OnGridRepairData(int nCount, CString strRepair);
	void						OnImageThumbnail(CString strPath);
	
public:
	CScreenAlarm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CScreenAlarm();

public:
	enum { IDD = IDD_SCREEN_ALARM };
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
	virtual BOOL DestroyWindow();
	virtual void OnInitialUpdate();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnImageClicked( WPARAM wParam, LPARAM lParam );
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnPrevAlarm1();
	afx_msg void OnClickedBtnPrevAlarm10();
	afx_msg void OnClickedBtnPrevAlarm2();
	afx_msg void OnClickedBtnPrevAlarm3();
	afx_msg void OnClickedBtnPrevAlarm4();
	afx_msg void OnClickedBtnPrevAlarm5();
	afx_msg void OnClickedBtnPrevAlarm6();
	afx_msg void OnClickedBtnPrevAlarm7();
	afx_msg void OnClickedBtnPrevAlarm8();
	afx_msg void OnClickedBtnPrevAlarm9();
	afx_msg void OnBnClickedBtnAlarmClean();
	afx_msg void OnClickedChkNo();
	afx_msg void OnClickedChkYes();
	afx_msg void OnClickedBtnAlarmIo();
	afx_msg void OnClickedBtnAlarmMotor();
	afx_msg void OnDestroy();
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedBtnDoor();
	afx_msg LRESULT	OnAlarmWorkInfoCommand(WPARAM wParam, LPARAM lParam); 
};


