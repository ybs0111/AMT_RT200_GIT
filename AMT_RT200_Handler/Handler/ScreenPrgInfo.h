#pragma once


#include ".\\Union\\Grid\\GridCtrl.h"
#include ".\\Union\\Grid\\GridURLCell.h"
#include ".\\Union\\Grid\\GridCellCombo.h"
#include ".\\Union\\Grid\\GridCellCheck.h"
#include ".\\Union\\Grid\\GridCellNumeric.h"
#include ".\\Union\\Grid\\GridCellDateTime.h"
#include "BtnST.h"
// CScreenPrgInfo 폼 뷰입니다.

class CScreenPrgInfo : public CFormView
{
	DECLARE_DYNCREATE(CScreenPrgInfo)

public:
	CScreenPrgInfo();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CScreenPrgInfo();

	int m_nExelSelect;

	CGridCtrl			m_pGridExelSheet;

	void OnInitButton();

	void OnExelReadInfo(int nExelSelect);
	void OnGridDataUpdate();;
	void OnInitGridExelSheet(int nExelSelect);

	void OnGridInitIOInfo();		// I/O 화면의 틀을 구성한다. 
	void OnGridInitPartIOInfo();	// PART I/O 화면 틀을 구성한다. 
	void OnGridInitMotorAxisInfo();	// MOTOR AXIS 화면 틀을 구성한다. 
	void OnGridInitMotorPartInfo();	// MOTOR PART INFO 화면 들을 구성한다. 
	void OnGridInitWaitTimeInfo();	// WAIT TIME 화면 틀을 구성한다. 

public:
	enum { IDD = IDD_SCREEN_PGM_INFO };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	virtual void PostNcDestroy();
};


