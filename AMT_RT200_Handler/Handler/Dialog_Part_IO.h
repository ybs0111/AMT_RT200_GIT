#pragma once

#include ".\\Union\\Grid\\GridCtrl.h"
#include ".\\Union\\Grid\\GridURLCell.h"
#include ".\\Union\\Grid\\GridCellCombo.h"
#include ".\\Union\\Grid\\GridCellCheck.h"
#include ".\\Union\\Grid\\GridCellNumeric.h"
#include ".\\Union\\Grid\\GridCellDateTime.h"
#include "BtnST.h"
// CDialog_Part_IO 대화 상자입니다.

#define TMR_PART_IO_INPUT_UPDATE	1000

class CDialog_Part_IO : public CDialog
{
	DECLARE_DYNAMIC(CDialog_Part_IO)

public:
	CDialog_Part_IO(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialog_Part_IO();

	int nInOut;
	int nPartNo[3];
	int nPortNo[3];

	int nInPartNo[20];
	int nInPotrNo[20];
	int nInBitNo[20];

	int nOutPartNo[20];
	int nOutPotrNo[20];
	int nOutBitNo[20];

	CTreeCtrl			m_treePartIoList;
	CImageList			*m_pImagelist;

	CGridCtrl			m_pGridInPutIo;
	CGridCtrl			m_pGridOutPutIo;

	void OnInitButton();

	void OnInitTree();
	void OnInitGridDataShow();

	void OnInputInitGrid();
	void OnOutputInitGrid();

	void OnDisplayInputIO();
	void OnDisplayOutputIO();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_PART_IO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedTreeMotorList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCellInputClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnCellOutputClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnBnClickedBtnPartIoExit();
	afx_msg void OnDestroy();

	CButtonST m_btnPartIoExit;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
