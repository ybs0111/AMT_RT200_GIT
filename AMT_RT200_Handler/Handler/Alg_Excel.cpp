#include "StdAfx.h"
#include "Alg_Excel.h"
#include "Variable.h"

CAlg_Excel clsExcel;

CAlg_Excel::CAlg_Excel(void)
{
	m_clsExcel = NULL;
}


CAlg_Excel::~CAlg_Excel(void)
{//ybs setup
}


bool CAlg_Excel::OnOpenIOMap(CString strFilename)
{
	int i, j, k, l;
	int nRow;
	int nPos[2];
	int nLength;

	CString strTmp;
	CString strData;

	if (m_clsExcel != NULL)
	{
		delete m_clsExcel;
		m_clsExcel = NULL;
	}


	m_clsExcel = new CXLAutomation;

	if (!m_clsExcel->OpenExcelFile(strFilename))
	{
		m_clsExcel->ReleaseExcel();

		delete m_clsExcel;
		m_clsExcel = NULL;

		return false;
	}

	// Excel IO정보 초기화.....
	m_nIOBoardCnt = 0; 
	memset(m_nIOMaster,		-1, sizeof(m_nIOMaster));
	memset(m_nIOPort,		-1, sizeof(m_nIOPort));
	memset(m_nIOPortNum,	-1, sizeof(m_nIOPortNum));
	memset(m_nIOBitType,	-1, sizeof(m_nIOBitType));
	memset(m_nOUTIOBitNum,	-1, sizeof(m_nOUTIOBitNum));
	memset(m_nINIOBitNum,	-1, sizeof(m_nINIOBitNum));

	for (i=0; i<3; i++)
	{
		for (j=0; j<4; j++)
		{
			for (k=0; k<8; k++)
			{
				nRow	= (i*(28*8*3)) + (j*(28*8)) + (28 * k) + 3;
				strTmp	= m_clsExcel->GetCellValueCString(1, nRow);

				if (strTmp != _T(""))
				{
					nLength						= strTmp.GetLength();
					nPos[0]						= strTmp.Find(_T(":"), 0);
					strData						= strTmp.Mid(nPos[0] +1, nLength - (nPos[0] + 1));
					m_nIOPortNum[i][j][k]		= _wtoi(strData);

					m_strIOPortLabel[i][j][k].Format(_T("[%02d]"), m_nIOPortNum[i][j][k]);		//2015.02.21 son

					nRow						= (i*(28*8*3)) + (j*(28*8)) + (28 * k) + 2;
					strTmp						= m_clsExcel->GetCellValueCString(5, nRow);
					nLength						= strTmp.GetLength();
					nPos[0]						= strTmp.Find(_T(":"), 0);
					m_strIOPortLabel[i][j][k]	+= strTmp.Mid(nPos[0] +1, nLength - (nPos[0] + 1));

					for (l=0; l<16; l++)
					{
						nRow	= (i*(28*8*3)) + (j*(28*8)) + (28 * k) + (l + 5);
						strTmp	= m_clsExcel->GetCellValueCString(4, nRow);

						if (strTmp != _T(""))
						{
							// 20130821 ook
							m_strIOBitDescrip[i][j][k][l]	= strTmp;
							m_strIOBitLabel[i][j][k][l]		= m_clsExcel->GetCellValueCString(2, nRow);

							m_nIOBitNum[i][j][k][l]			=  (i*10000) + (j*1000) + (100 * k) + l;

							strTmp	= m_clsExcel->GetCellValueCString(3, nRow);
							if(strTmp == "O")
							{
								m_nIOBitType[i][j][k][l]	= IO_OUT;
//								m_nOUTIOBitNum[i][j][k][l]	=  (i*10000) + (j*1000) + (k*100) + l;
							}
							else
							{
								m_nIOBitType[i][j][k][l]	= IO_IN;
//								m_nINIOBitNum[i][j][k][l]	=  (i*10000) + (j*1000) + (k*100) + l;
							}
						}
						else
						{		
							m_strIOBitLabel[i][j][k][l]	= _T("");
							m_nIOBitType[i][j][k][l]	= IO_NONE;
						}
					}
					m_nIOPort[i][j]++;
				}
				else
				{
					break;
				}
			}
		}
	}

	return true;
}


bool CAlg_Excel::OnCloseIOMap()
{
	if (m_clsExcel == NULL)
	{
		return true;
	}

	if (!m_clsExcel->ReleaseExcel())
	{
		delete m_clsExcel;
		m_clsExcel = NULL;

		return false;
	}

	delete m_clsExcel;
	m_clsExcel = NULL;

	return true;
}

bool CAlg_Excel::OnOpenPartIOMap(CString strFilename)
{
	int i, j;
	int nRow, nCol;
	int nAccTmp = 0;
	int nPart, nPartCnt, nBit;
	// jtkim 20130625
	int nPortAccCnt, nPortNo, nOldPortNo = -1;

	CString strTmp;
	CString strData;

	if (m_clsExcel != NULL)
	{
		delete m_clsExcel;
		m_clsExcel = NULL;
	}


	m_clsExcel = new CXLAutomation;

	if (!m_clsExcel->OpenExcelFile(strFilename))
	{
		m_clsExcel->ReleaseExcel();

		delete m_clsExcel;
		m_clsExcel = NULL;

		return false;
	}

	// Excel IO정보 초기화.....
	memset(m_nPartInOutTPutCnt,	-1, sizeof(m_nPartInOutTPutCnt));
	memset(m_nFirstPortNo,	-1, sizeof(m_nFirstPortNo));
	memset(m_nPartAccBittNo,	-1, sizeof(m_nPartAccBittNo));
	memset(m_nPartInOutTPutPortCnt,	-1, sizeof(m_nPartInOutTPutPortCnt));


	for (i=0; i<10; i++)
	{
		for (j=0; j<2; j++)
		{
			nRow = 3;
			nCol = 4;

			// Part 값을 가져온다. 
			nPart = _wtoi(m_clsExcel->GetCellValueCString((i*6)+nCol - 2, nRow - 2));

			// Part Input Output 카운트 값을 가져온다. 
			nPartCnt = _wtoi(m_clsExcel->GetCellValueCString((i*6)+(j*3)+nCol, nRow));
				
 			m_nPartInOutTPutCnt[nPart][j/*In:Out*/] = nPartCnt;
 
 			for(int k=0; k<m_nPartInOutTPutCnt[nPart][j/*In:Out*/] + 1; k++)
 			{
				strTmp = m_clsExcel->GetCellValueCString((i*6)+(j*3)+nCol, 5+k);

				if(strTmp != _T(""))
				{
					nPortNo = _wtoi(m_clsExcel->GetCellValueCString((i*6)+(j*3)+nCol-2, nRow+2+k));
					if(nOldPortNo != nPortNo) {
						nOldPortNo = nPortNo;
						nPortAccCnt =0;

						// Part별 Port의 첫번째를 등록하여 최초 화면 보여줄때 사용한다. 
						clsExcel.m_nFirstPortNo[i][j][nAccTmp] = nPortNo;
						
						nAccTmp++;
					}
					else {
						m_nPartInOutTPutPortCnt[nPart][j][nPortNo] = nPortAccCnt++;
					}

					nBit	= _wtoi(m_clsExcel->GetCellValueCString((i*6)+(j*3)+nCol-1, nRow+2+k));
					clsExcel.m_nPartAccBittNo[nPart][j][nPortNo][nPortAccCnt] = nBit;
 					m_sPartInOutPutName[nPart][j][nPortNo][nBit] = m_clsExcel->GetCellValueCString((i*6)+(j*3)+nCol, 5+k);
				}
 			}
			nAccTmp = 0;
		}
	}

	return true;
}

bool CAlg_Excel::OnClosePartIOMap()
{
	if (m_clsExcel == NULL)
	{
		return true;
	}

	if (!m_clsExcel->ReleaseExcel())
	{
		delete m_clsExcel;
		m_clsExcel = NULL;

		return false;
	}

	delete m_clsExcel;
	m_clsExcel = NULL;

	return true;
}

bool CAlg_Excel::OnOpenMotorPart(CString strFilename)
{
	int i, j;

	CString strData;

	if (m_clsExcel != NULL)
	{
		delete m_clsExcel;
		m_clsExcel = NULL;
	}


	m_clsExcel = new CXLAutomation;

	if (!m_clsExcel->OpenExcelFile(strFilename))
	{
		m_clsExcel->ReleaseExcel();

		delete m_clsExcel;
		m_clsExcel = NULL;

		return false;
	}

	m_nPartCnt			= 0;
	memset(m_nPartAxisCnt,	-1, sizeof(m_nPartAxisCnt));
	memset(m_nPartAxis,		-1, sizeof(m_nPartAxis));

	strData	= m_clsExcel->GetCellValueCString(2, 4);
	m_nPartCnt	= _wtoi(strData);

	if (m_nPartCnt == 0) return false;

	for (i=0; i<m_nPartCnt; i++)
	{
		strData				= m_clsExcel->GetCellValueCString(2, (i*2)+7);
		m_nPartAxisCnt[i]	= _wtoi(strData);
		m_strPartName[i]	= m_clsExcel->GetCellValueCString(3, (i*2)+7);
		m_strPartInfo[i]	= m_clsExcel->GetCellValueCString(4, (i*2)+7);

		if(m_nPartAxisCnt[i] > 0)
		{
			for(j=0; j<m_nPartAxisCnt[i]; j++)
			{
				strData					= m_clsExcel->GetCellValueCString(j+5, (i*2)+8);
				m_nPartAxis[i][j]		= _wtoi(strData);
				m_strPartAxisName[i][j]	= m_clsExcel->GetCellValueCString(j+5, (i*2)+7);
			}
		}
	}

	return true;
}


bool CAlg_Excel::OnCloseMotorPart()
{
	if (m_clsExcel == NULL)
	{
		return true;
	}

	if (!m_clsExcel->ReleaseExcel())
	{
		delete m_clsExcel;
		m_clsExcel = NULL;

		return false;
	}

	delete m_clsExcel;
	m_clsExcel = NULL;

	return true;
}


bool CAlg_Excel::OnOpenMotorPosition(CString strFilename)
{
	int i, j, k;
	int nAxis;

	CString strData;

	if (m_clsExcel != NULL)
	{
		delete m_clsExcel;
		m_clsExcel = NULL;
	}


	m_clsExcel = new CXLAutomation;

	if (!m_clsExcel->OpenExcelFile(strFilename))
	{
		m_clsExcel->ReleaseExcel();

		delete m_clsExcel;
		m_clsExcel = NULL;

		return false;
	}

	memset(m_nAxisPosNum,		-1, sizeof(m_nAxisPosNum));
	memset(m_nAxisMinusEl,		-1, sizeof(m_nAxisMinusEl));
	memset(m_nAxisPlusEl,		-1, sizeof(m_nAxisPlusEl));
	memset(m_nAxisHome,			-1, sizeof(m_nAxisHome));
	memset(m_nAxisLimitCnt,		0, sizeof(m_nAxisLimitCnt));
	memset(&m_nAxisLimitInfo,   0, sizeof(&m_nAxisLimitInfo));
	memset(m_nAxisDirection,	-1, sizeof(m_nAxisDirection));
	memset(m_nAxisAccount,		-1, sizeof(m_nAxisAccount));

	strData									= m_clsExcel->GetCellValueCString(2, 1);
	m_nMotorMaxCnt							= _wtoi(strData);

	for(i=0; i<m_nMotorMaxCnt; i++)
	{
		//
		//m_strAxisName[i]					= m_clsExcel->GetCellValueCString((3 * (i) + 2), 4);
		m_strAxisName[i]					= m_clsExcel->GetCellValueCString((2 * (i) + 2), 4);
		
	}

	for(i=0; i<m_nPartCnt; i++)
	{
		for(j=0; j<m_nPartAxisCnt[i]; j++)
		{
			strData									= m_clsExcel->GetCellValueCString((m_nPartAxis[i][j] * 2) + 2, 16);
			m_nAxisPosNum[i][j]						= _wtoi(strData);

			strData									= m_clsExcel->GetCellValueCString((m_nPartAxis[i][j] * 2) + 2, 3);
			nAxis									= _wtoi(strData);

			strData									= m_clsExcel->GetCellValueCString((m_nPartAxis[i][j] * 2) + 2, 8);
//			m_nAxisMinusEl[i][j]					= _wtoi(strData);
			m_nAxisLimitInfo[i][j][0]				= _wtoi(strData);

			strData									= m_clsExcel->GetCellValueCString((m_nPartAxis[i][j] * 2) + 2, 9);
//			m_nAxisPlusEl[i][j]						= _wtoi(strData);
			m_nAxisLimitInfo[i][j][1]				= _wtoi(strData);

			strData									= m_clsExcel->GetCellValueCString((m_nPartAxis[i][j] * 2) + 2, 10);
//			m_nAxisPlusEl[i][j]						= _wtoi(strData);
			m_nAxisLimitInfo[i][j][2]				= _wtoi(strData);

			strData									= m_clsExcel->GetCellValueCString((m_nPartAxis[i][j] * 2) + 2, 7);
//			m_nAxisHome[i][j]						= _wtoi(strData);
			m_nAxisLimitCnt[i][j]					= _wtoi(strData);

			strData									= m_clsExcel->GetCellValueCString((m_nPartAxis[i][j] * 2) + 2, 11);
			m_nAxisDirection[i][j]					= _wtoi(strData);

			strData									= m_clsExcel->GetCellValueCString((m_nPartAxis[i][j] * 2) + 2, 12);
			m_nAxisAccount[i][j]					= _wtoi(strData);

			for(k=0; k<m_nAxisPosNum[i][j]; k++)
			{
				m_strAxisPosName[i][j][k]			= m_clsExcel->GetCellValueCString((m_nPartAxis[i][j] * 2) + 2, k+17);
				strData								= m_clsExcel->GetCellValueCString(((m_nPartAxis[i][j] * 2) + 2)+1, k+17);
				m_nAxisPosData[nAxis][k]			= _wtoi(strData);
				m_strAxisPosInfo[i][j][k]			= m_clsExcel->GetCellValueCString(((m_nPartAxis[i][j] * 2) + 2)+2, k+17);
			}
		}
	}

	return true;
}


bool CAlg_Excel::OnCloseMotorPosition()
{
	if (m_clsExcel == NULL)
	{
		return true;
	}

	if (!m_clsExcel->ReleaseExcel())
	{
		delete m_clsExcel;
		m_clsExcel = NULL;

		return false;
	}

	delete m_clsExcel;
	m_clsExcel = NULL;

	return true;
}

bool CAlg_Excel::OnOpenWaitTime(CString strFilename)
{
	int i;
	
	CString strData;

	if (m_clsExcel != NULL)
	{
		delete m_clsExcel;
		m_clsExcel = NULL;
	}


	m_clsExcel = new CXLAutomation;

	if (!m_clsExcel->OpenExcelFile(strFilename))
	{
		m_clsExcel->ReleaseExcel();

		delete m_clsExcel;
		m_clsExcel = NULL;

		return false;
	}

	memset(m_nOnUsed,		-1, sizeof(m_nOnUsed));
	memset(m_nOffUsed,		-1, sizeof(m_nOffUsed));
	memset(m_nLimitUsed,	-1, sizeof(m_nLimitUsed));

	strData									= m_clsExcel->GetCellValueCString(3, 1);
	m_nWaitTimeMaxCnt						= _wtoi(strData);

	for(i=0; i<m_nWaitTimeMaxCnt; i++)
	{
		m_strWaitTimeLabel[i]				= m_clsExcel->GetCellValueCString(2, i + 4);

		strData								= m_clsExcel->GetCellValueCString(4, i + 4);
		m_nOnUsed[i]						= _wtoi(strData);

		strData								= m_clsExcel->GetCellValueCString(5, i + 4);
		m_nOffUsed[i]						= _wtoi(strData);

		strData								= m_clsExcel->GetCellValueCString(6, i + 4);
		m_nLimitUsed[i]						= _wtoi(strData);
	}

	return true;
}


bool CAlg_Excel::OnCloseWaitTime()
{
	if (m_clsExcel == NULL)
	{
		return true;
	}

	if (!m_clsExcel->ReleaseExcel())
	{
		delete m_clsExcel;
		m_clsExcel = NULL;

		return false;
	}

	delete m_clsExcel;
	m_clsExcel = NULL;

	return true;
}