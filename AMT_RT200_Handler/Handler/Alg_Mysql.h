// MySqlAlg.h: interface for the MySqlAlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYSQLALG_H__E843957F_0AF5_4BB5_A596_7A50140E00C7__INCLUDED_)
#define AFX_MYSQLALG_H__E843957F_0AF5_4BB5_A596_7A50140E00C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define ID_TYPE					0
#define INT_TYPE				1
#define FLOAT_TYPE				2
#define DOUBLE_TYPE				3
#define TEXT_TYPE				4
#define DATE_TYPE				5
#define DATEAUTO_TYPE			6
#define DATETIME_TYPE			7
/*
#define TYNYINT_TYPE			0
#define SMALLINT_TYPE			1
#define MEDIUMINT_TYPE			2
#define INT_TYPE				3
#define BIGINT_TYPE				4
#define FLOAT_TYPE				5
#define DOUBLE_TYPE				6
#define DATE_TYPE				7
#define DATETIME_TYPE			8
#define TIMESTAMP_TYPE			9 
#define TIME_TYPE				10
#define YEAR_TYPE				11
#define CHAR_TYPE				12
#define VARCHAR_TYPE			13
#define TINYBLOB_TYPE			14
#define TINYTEXT_TYPE			15
#define BLOB_TYPE				16
#define TEXT_TYPE				17
#define MEDIUMBLOB_TYPE			18
#define MEDIUMTEXT_TYPE			19
#define LONGBLOB_TYPE			20
#define LONGTEXT_TYPE			21
#define NUMBER_TYEP				22
*/
#define		MAX_NUM				50
#include "mysql.h"

class CAlg_Mysql  
{
public:
	CString			*m_strquery[MAX_NUM];

	CString			m_strTableName[5];
	int				m_nTableCnt;
	int				m_nTableNum[5];
	int				m_nTableType[5];

	CString			m_strOperatorTableName[5];
	int				m_nOperatorTableCnt;
	int				m_nOperatorTableNum[5];
	int				m_nOperatorTableType[5];

	CString			m_strToolTableName[5];
	int				m_nToolTableCnt;
	int				m_nToolTableNum[5];
	int				m_nToolTableType[5];

	CString			m_strPcbTableName[15];
	int				m_nPcbTableCnt;
	int				m_nPcbTableNum[15];
	int				m_nPcbTableType[15];
/*
	CString			m_strGmsTableName[7];
	int				m_nGmsTableCnt;
	int				m_nGmsTableNum[7];
	int				m_nGmsTableType[7];
*/
	CString			m_strScrapTableName[7];
	int				m_nScrapTableCnt;
	int				m_nScrapTableNum[7];
	int				m_nScrapTableType[7];

	CString			m_strPartNoTableName[4];
	int				m_nPartNoTableCnt;
	int				m_nPartNoTableNum[4];
	int				m_nPartNoTableType[4];

	CString			m_strTypeTableName[3];
	int				m_nTypeTableCnt;
	int				m_nTypeTableNum[3];
	int				m_nTypeTableType[3];

	CString			m_strLotTableName[9];
	int				m_nLotTableCnt;
	int				m_nLotTableNum[9];
	int				m_nLotTableType[9];

	long			m_nread_cnt[MAX_NUM];
	long			m_nfind_cnt[MAX_NUM];

	BOOL			m_bFlag;

	//2015.1009
	CString			mstr_dbtime_table_name[16];
	int				mn_dbtime_table_cnt;
	int				mn_dbtime_table_num[16];
	int				mn_dbtime_table_type[16];
	////

	MYSQL			m_pMySql[MAX_NUM];
	MYSQL_RES		*m_pRes[MAX_NUM];
	MYSQL_ROW		m_pRow[MAX_NUM];
	MYSQL_FIELD		*m_pFields[MAX_NUM];
	unsigned int	m_nHeaderNum;
	LONG64			m_nRecordNum;

public:
	float Record_Max(int nNum, CString item);
	float Record_Min(int nNum, CString item);

	BOOL ReadFile(int nNum, CString strFileName);
	BOOL WriteFile(int nNum, CString strFileName);
	BOOL Record_Sort(int nNum, int mode, CString tablename, CString itemsearch);
	BOOL WaitTime(int nNum, int mode, int time);
	CString Record_Read(int nNum);
	BOOL Table_Select(int nNum, CString databasename, CString tablename);
	long Record_Date_Search(int nNum, CString tablename, CString itemsearch, CString range, COleDateTime time[]);
	long Record_Text_Search(int nNum, CString tablename, CString itemsearch, CString value);
	long Record_Text_Search(int nNum, CString tablename, CString itemsearch1, CString itemsearch2, CString value1, CString value2);
	long Record_Double_Search(int nNum, CString tablename, CString itemsearch, CString range, double value[]);
	long Record_Double_Search(int nNum, CString tablename, CString itemsearch1, CString itemsearch2, double x[], double y[]);
	long Record_Float_Search(int nNum, CString tablename, CString itemsearch, CString range, float value[]);
	long Record_Int_Search(int nNum, CString tablename, CString itemsearch, CString range, int value[]);
	BOOL Record_Date_Delete(int nNum, CString tablename, CString itemname, CString range, COleDateTime time[]);
	BOOL Record_Text_Delete(int nNum, CString tablename, CString itemname, CString value);
	BOOL Record_Double_Delete(int nNum, CString tablename, CString itemname, CString range, double value[]);
	BOOL Record_Float_Delete(int nNum, CString tablename, CString itemname, CString range, float value[]);
	BOOL Record_Int_Delete(int nNum, CString tablename, CString itemname, CString range, int value[]);
	BOOL Record_Updata(int nNum, CString tablename, CString search_item_1, CString search_data_1, CString search_item_2, CString search_data_2, CString change_item, CString data);
	BOOL Database_Select(int nNum, CString databasename);
	void MySql_Close(int nNum);
	BOOL Record_Add(int nNum, CString tablename, CString *itemdata, BOOL bFlag=FALSE);
	BOOL Table_Delete(int nNum, CString tablename);
	BOOL Table_Create(int nNum, CString databasename, CString tablename, int itemcount, CString *itemname, int *itemtype, int *itemhundred);
	BOOL Database_Delete(int nNum, CString databasename);
	BOOL Database_Create(int nNum, CString databasename);
	BOOL MySql_Open(int nNum, CString hostname = _T("localhost"), CString username = _T("root"), CString password = _T("1111"), int port = 3306);
//	BOOL Get_Record_List(CString recordname, CString tablename, int *nRecordBuff);
//	BOOL Get_Record_List(CString recordname, CString tablename, int *nBuffCnt, CString *strRecordBuff);
	CAlg_Mysql();
	virtual ~CAlg_Mysql();

};

extern CAlg_Mysql Mysql;
#endif // !defined(AFX_MYSQLALG_H__E843957F_0AF5_4BB5_A596_7A50140E00C7__INCLUDED_)
