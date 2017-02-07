#pragma once


#include ".\\Union\\Grid\\GridCtrl.h"
#include ".\\Union\\Grid\\GridURLCell.h"
#include ".\\Union\\Grid\\GridCellCombo.h"
#include ".\\Union\\Grid\\GridCellCheck.h"
#include ".\\Union\\Grid\\GridCellNumeric.h"
#include ".\\Union\\Grid\\GridCellDateTime.h"
#include "BtnST.h"
// CScreenPrgInfo �� ���Դϴ�.

class CScreenPrgInfo : public CFormView
{
	DECLARE_DYNCREATE(CScreenPrgInfo)

public:
	CScreenPrgInfo();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CScreenPrgInfo();

	int m_nExelSelect;

	CGridCtrl			m_pGridExelSheet;

	void OnInitButton();

	void OnExelReadInfo(int nExelSelect);
	void OnGridDataUpdate();;
	void OnInitGridExelSheet(int nExelSelect);

	void OnGridInitIOInfo();		// I/O ȭ���� Ʋ�� �����Ѵ�. 
	void OnGridInitPartIOInfo();	// PART I/O ȭ�� Ʋ�� �����Ѵ�. 
	void OnGridInitMotorAxisInfo();	// MOTOR AXIS ȭ�� Ʋ�� �����Ѵ�. 
	void OnGridInitMotorPartInfo();	// MOTOR PART INFO ȭ�� ���� �����Ѵ�. 
	void OnGridInitWaitTimeInfo();	// WAIT TIME ȭ�� Ʋ�� �����Ѵ�. 

public:
	enum { IDD = IDD_SCREEN_PGM_INFO };
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
	afx_msg void OnDestroy();
	virtual void PostNcDestroy();
};


