// MySqlAlg.cpp: implementation of the MySqlAlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Handler.h"
#include "Alg_Mysql.h"
#include "AlgMemory.h"
#include "PublicFunction.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CAlg_Mysql Mysql;

MYSQL_RES *pRes;
MYSQL_ROW pRow;
MYSQL_FIELD *pFields;

CAlg_Mysql::CAlg_Mysql()
{
	// Mysql DB 정의
	CString str_tmp;
	m_bFlag = FALSE;

	// Mysql DB 정의
	// Mysql DB 정의
	m_nTableCnt			= 5;
	m_strTableName[0]	= _T("no");
	m_strTableName[1]	= _T("CIR");
	m_strTableName[2]	= _T("X");
	m_strTableName[3]	= _T("Y");
	m_strTableName[4]	= _T("Z");


	m_nTableNum[0]		= 5;
	m_nTableNum[1]		= 20;
	m_nTableNum[2]		= 10;
	m_nTableNum[3]		= 10;
	m_nTableNum[4]		= 10;

	m_nTableType[0]		= ID_TYPE;
	m_nTableType[1]		= TEXT_TYPE;
	m_nTableType[2]		= DOUBLE_TYPE;
	m_nTableType[3]		= DOUBLE_TYPE;
	m_nTableType[4]		= DOUBLE_TYPE;

	m_nOperatorTableCnt			=	5;
	m_strOperatorTableName[0]	=	_T("ID");
	m_strOperatorTableName[1]	=	_T("DATE");
	m_strOperatorTableName[2]	=	_T("NAME");
	m_strOperatorTableName[3]	=	_T("CREM");
	m_strOperatorTableName[4]	=	_T("NUMBER");


	m_nOperatorTableNum[0]		=	5;
	m_nOperatorTableNum[1]		=	40;
	m_nOperatorTableNum[2]		=	40;
	m_nOperatorTableNum[3]		=	40;
	m_nOperatorTableNum[4]		=	40;

	m_nOperatorTableType[0]		=	ID_TYPE;
	m_nOperatorTableType[1]		=	DATEAUTO_TYPE;
	m_nOperatorTableType[2]		=	TEXT_TYPE;
	m_nOperatorTableType[3]		=	TEXT_TYPE;
	m_nOperatorTableType[4]		=	TEXT_TYPE;


	m_nToolTableCnt				=	5;
	m_strToolTableName[0]		=	_T("No");
	m_strToolTableName[1]		=	_T("DATE");
	m_strToolTableName[2]		=	_T("MODEL_NAME");
	m_strToolTableName[3]		=	_T("PART_NO");
	m_strToolTableName[4]		=	_T("ARRAY");

	m_nToolTableNum[0]			=	5;
	m_nToolTableNum[1]			=	80;
	m_nToolTableNum[2]			=	80;
	m_nToolTableNum[3]			=	30;
	m_nToolTableNum[4]			=	5;

	m_nToolTableType[0]			=	ID_TYPE;
	m_nToolTableType[1]			=	DATEAUTO_TYPE;
	m_nToolTableType[2]			=	TEXT_TYPE;
	m_nToolTableType[3]			=	TEXT_TYPE;
	m_nToolTableType[4]			=	INT_TYPE;

	m_nPcbTableCnt				=	15;
	m_strPcbTableName[0]		=	_T("No");
	m_strPcbTableName[1]		=	_T("START_TIME");
	m_strPcbTableName[2]		=	_T("END_TIME");
	m_strPcbTableName[3]		=	_T("BIN");;
	m_strPcbTableName[4]		=	_T("COUNT");
	m_strPcbTableName[5]		=	_T("SLOT");
	m_strPcbTableName[6]		=	_T("LOT");
	m_strPcbTableName[7]		=	_T("PART");
	m_strPcbTableName[8]		=	_T("BARCODE");
	m_strPcbTableName[9]		=	_T("SERIAL");
	m_strPcbTableName[10]		=	_T("ARRAY");
	m_strPcbTableName[11]		=	_T("PPID");
	m_strPcbTableName[12]		=	_T("PSID");
	m_strPcbTableName[13]		=	_T("WWN");
	m_strPcbTableName[14]		=	_T("CSERIAL");

	m_nPcbTableNum[0]			=	5;
	m_nPcbTableNum[1]			=	40;
	m_nPcbTableNum[2]			=	40;
	m_nPcbTableNum[3]			=	5;
	m_nPcbTableNum[4]			=	2;
	m_nPcbTableNum[5]			=	3;
	m_nPcbTableNum[6]			=	40;
	m_nPcbTableNum[7]			=	40;
	m_nPcbTableNum[8]			=	40;
	m_nPcbTableNum[9]			=	40;
	m_nPcbTableNum[10]			=	40;
	m_nPcbTableNum[11]			=	40;
	m_nPcbTableNum[12]			=	40;
	m_nPcbTableNum[13]			=	40;
	m_nPcbTableNum[14]			=	40;

	m_nPcbTableType[0]			=	ID_TYPE;
	m_nPcbTableType[1]			=	DATETIME_TYPE;
	m_nPcbTableType[2]			=	DATETIME_TYPE;
	m_nPcbTableType[3]			=	INT_TYPE;
	m_nPcbTableType[4]			=	INT_TYPE;
	m_nPcbTableType[5]			=	INT_TYPE;
	m_nPcbTableType[6]			=	TEXT_TYPE;
	m_nPcbTableType[7]			=	TEXT_TYPE;
	m_nPcbTableType[8]			=	TEXT_TYPE;
	m_nPcbTableType[9]			=	TEXT_TYPE;
	m_nPcbTableType[10]			=	TEXT_TYPE;
	m_nPcbTableType[11]			=	TEXT_TYPE;
	m_nPcbTableType[12]			=	TEXT_TYPE;
	m_nPcbTableType[13]			=	TEXT_TYPE;
	m_nPcbTableType[14]			=	TEXT_TYPE;

	m_nScrapTableCnt			=	5;
	m_strScrapTableName[0]		=	_T("No");
	m_strScrapTableName[1]		=	_T("Date");
	m_strScrapTableName[2]		=	_T("Lot_No");
	m_strScrapTableName[3]		=	_T("Part_No");
	m_strScrapTableName[4]		=	_T("Scrap_Code");
	m_strScrapTableName[5]		=	_T("Serial");
	m_strScrapTableName[6]		=	_T("Array_SN");

	m_nScrapTableNum[0]			=	7;
	m_nScrapTableNum[1]			=	40;
	m_nScrapTableNum[2]			=	20;
	m_nScrapTableNum[3]			=	20;
	m_nScrapTableNum[4]			=	10;
	m_nScrapTableNum[5]			=	20;
	m_nScrapTableNum[6]			=	30;

	m_nScrapTableType[0]		=	ID_TYPE;
	m_nScrapTableType[1]		=	DATEAUTO_TYPE;
	m_nScrapTableType[2]		=	TEXT_TYPE;
	m_nScrapTableType[3]		=	TEXT_TYPE;
	m_nScrapTableType[4]		=	TEXT_TYPE;
	m_nScrapTableType[5]		=	TEXT_TYPE;
	m_nScrapTableType[6]		=	TEXT_TYPE;

	m_nPartNoTableCnt			=	4;
	m_strPartNoTableName[0]		=	_T("No");
	m_strPartNoTableName[1]		=	_T("Date");
	m_strPartNoTableName[2]		=	_T("Part_No");
	m_strPartNoTableName[3]		=	_T("Device_Type");

	m_nPartNoTableNum[0]		=	5;
	m_nPartNoTableNum[1]		=	25;
	m_nPartNoTableNum[2]		=	25;
	m_nPartNoTableNum[3]		=	10;

	m_nPartNoTableType[0]		=	ID_TYPE;
	m_nPartNoTableType[1]		=	DATEAUTO_TYPE;
	m_nPartNoTableType[2]		=	TEXT_TYPE;
	m_nPartNoTableType[3]		=	TEXT_TYPE;

	m_nTypeTableCnt				=	3;
	m_strTypeTableName[0]		=	_T("No");
	m_strTypeTableName[1]		=	_T("Date");
	m_strTypeTableName[2]		=	_T("Device_Type");

	m_nTypeTableNum[0]			=	5;
	m_nTypeTableNum[1]			=	25;
	m_nTypeTableNum[2]			=	10;

	m_nTypeTableType[0]			=	ID_TYPE;
	m_nTypeTableType[1]			=	DATEAUTO_TYPE;
	m_nTypeTableType[2]			=	TEXT_TYPE;

	m_nLotTableCnt				=	9;
	m_strLotTableName[0]		=	_T("No");
	m_strLotTableName[1]		=	_T("DATE");
	m_strLotTableName[2]		=	_T("LOT");
	m_strLotTableName[3]		=	_T("PART");
	m_strLotTableName[4]		=	_T("TOTAL");
	m_strLotTableName[5]		=	_T("P_COUNT");
	m_strLotTableName[6]		=	_T("P_YIELD");
	m_strLotTableName[7]		=	_T("C_COUNT");
	m_strLotTableName[8]		=	_T("C_YIELD");
	
	m_nLotTableNum[0]			=	5;
	m_nLotTableNum[1]			=	40;
	m_nLotTableNum[2]			=	40;
	m_nLotTableNum[3]			=	40;
	m_nLotTableNum[4]			=	10;
	m_nLotTableNum[5]			=	10;
	m_nLotTableNum[6]			=	10;
	m_nLotTableNum[7]			=	10;
	m_nLotTableNum[8]			=	10;

	m_nLotTableType[0]			=	ID_TYPE;
	m_nLotTableType[1]			=	DATEAUTO_TYPE;
	m_nLotTableType[2]			=	TEXT_TYPE;
	m_nLotTableType[3]			=	TEXT_TYPE;
	m_nLotTableType[4]			=	TEXT_TYPE;
	m_nLotTableType[5]			=	TEXT_TYPE;
	m_nLotTableType[6]			=	TEXT_TYPE;
	m_nLotTableType[7]			=	TEXT_TYPE;
	m_nLotTableType[8]			=	TEXT_TYPE;

	////2015.1009
	mn_dbtime_table_cnt		= 15;

	mstr_dbtime_table_name[0] 	= "no";
	mstr_dbtime_table_name[1]	= "DATE";

	mstr_dbtime_table_name[2]	= "slot";
	mstr_dbtime_table_name[3]	= "REF_TIME";
	mstr_dbtime_table_name[4]	= "RUN_TIME";				
	mstr_dbtime_table_name[5]	= "TEST_TIME";			
	mstr_dbtime_table_name[6]	= "LT_RD";
	mstr_dbtime_table_name[7]	= "LT_SocketOff";
	mstr_dbtime_table_name[8]	= "LT_Instandby";
	mstr_dbtime_table_name[9]	= "LT_OutReady";
	mstr_dbtime_table_name[10]	= "LT_Stop";
	mstr_dbtime_table_name[11]	= "LT_UserStop";								
	mstr_dbtime_table_name[12]	= "OutPut";
	mstr_dbtime_table_name[13]	= "Yield";
	mstr_dbtime_table_name[14]	= "Pass"; 

	mn_dbtime_table_type[0]  = ID_TYPE;
	mn_dbtime_table_type[1]  = DATE_TYPE;
	mn_dbtime_table_type[2]  = TEXT_TYPE;
	mn_dbtime_table_type[3]  = INT_TYPE;
	mn_dbtime_table_type[4]  = INT_TYPE;
	mn_dbtime_table_type[5]  = INT_TYPE;
	mn_dbtime_table_type[6]	 = INT_TYPE;
	mn_dbtime_table_type[7]	 = INT_TYPE;
	mn_dbtime_table_type[8] = INT_TYPE;
	mn_dbtime_table_type[9] = INT_TYPE;
	mn_dbtime_table_type[10] = INT_TYPE;
	mn_dbtime_table_type[11] = INT_TYPE;
	mn_dbtime_table_type[12] = INT_TYPE;
	mn_dbtime_table_type[13] = FLOAT_TYPE;
	mn_dbtime_table_type[14] = INT_TYPE; 

	mn_dbtime_table_num[0]		= 5;
	mn_dbtime_table_num[1]		= 20;
	mn_dbtime_table_num[2]		= 20;
	mn_dbtime_table_num[3]		= 6;
	mn_dbtime_table_num[4]		= 6;
	mn_dbtime_table_num[5]		= 6;
	mn_dbtime_table_num[6]		= 6;
	mn_dbtime_table_num[7]		= 6;
	mn_dbtime_table_num[8]		= 6;
	mn_dbtime_table_num[9]		= 6;
	mn_dbtime_table_num[10]		= 6;
	mn_dbtime_table_num[11]		= 6;
	mn_dbtime_table_num[12]		= 6;
	mn_dbtime_table_num[13]		= 6;
	mn_dbtime_table_num[14]		= 6; 
	
	if(MySql_Open(0, _T("localhost"), _T("root"), _T("1111"), 3306))	
	{
		WaitTime(0, 0,1000);
		WaitTime(0, 1,300);	
	}
	
	MySql_Close(0);
	
	m_pRes[0] = NULL;
}

CAlg_Mysql::~CAlg_Mysql()
{
	int i;

	if(m_strquery != NULL)
	{
		for (i=0; i<MAX_NUM; i++)
		{
			if (m_strquery[i] != NULL)
			{
				delete [] m_strquery[i];
				m_strquery[i] = NULL;
			}
		}
	}
}

BOOL CAlg_Mysql::MySql_Open(int nNum, CString hostname, CString username,  CString password, int port)
{
	// 데이터 베이스 열기
	mysql_init(&m_pMySql[nNum]);
	
	char chHost[100];
	char chName[100];
	char chPass[100];
	char chQurey[8192];

	CString strMsg;

	clsFunc.OnStringToChar(hostname, chHost);
	clsFunc.OnStringToChar(username, chName);
	clsFunc.OnStringToChar(password, chPass);

	if(!(mysql_real_connect(&m_pMySql[nNum], chHost, chName, chPass, NULL, port, NULL, NULL)))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);
		return FALSE;
	}

	// 한글 지원하기 위해
	clsFunc.OnStringToChar(_T("SET NAMES euckr"), chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return FALSE;
	}
	
	m_bFlag = TRUE;
	return TRUE;
}

BOOL CAlg_Mysql::Database_Create(int nNum, CString databasename)
{
	// 데이터 베이스 새로 추가하기 
	CString strQuery;
	CString strMsg;

	char chQurey[8192];

	strQuery = _T("CREATE DATABASE ") + databasename;

	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return FALSE;
	}

	return TRUE;
}

BOOL CAlg_Mysql::Database_Delete(int nNum, CString databasename)
{
	// 데이터 베이스 삭제하기
	CString strQuery;
	CString strMsg;

	char chQurey[8192];

	strQuery = _T("DROP DATABASE ") + databasename;
	
	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return FALSE;
	}
	
	return TRUE;
}

/*
TYNYINT_TYPE			0
SMALLINT_TYPE			1
MEDIUMINT_TYPE			2
INT_TYPE				3
BIGINT_TYPE				4
FLOAT_TYPE				5
DOUBLE_TYPE				6
DATE_TYPE				7
DATETIME_TYPE			8
TIMESTAMP_TYPE			9 
TIME_TYPE				10
YEAR_TYPE				11
CHAR_TYPE				12
VARCHAR_TYPE			13
TINYBLOB_TYPE			14
TINYTEXT_TYPE			15
BLOB_TYPE				16
TEXT_TYPE				17
MEDIUMBLOB_TYPE			18
MEDIUMTEXT_TYPE			19
LONGBLOB_TYPE			20
LONGTEXT_TYPE			21
NUMBER_TYEP				22
*/

BOOL CAlg_Mysql::Table_Create(int nNum, CString database, CString tablename, int itemnum, CString *itemname, int *itemtype, int *itemhundred)
{
	CString strQuery;
	CString strItem;
	CString strTmp;
	CString strMsg;

	char chQurey[8192];

	int i;

 	strQuery = _T("USE ") + database;

	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return FALSE;
	}


	for(i=0; i<itemnum; i++)
	{
		switch(itemtype[i])
		{
			case ID_TYPE:
				strTmp.Format(_T("%s INT PRIMARY KEY NOT NULL AUTO_INCREMENT"), itemname[i]);
				strItem += strTmp;
				break;
			case INT_TYPE:
				strTmp.Format(_T("%s INT"), itemname[i]);
				strItem += strTmp;
				break;
			case FLOAT_TYPE:
				strTmp.Format(_T("%s FLOAT"), itemname[i]);
				strItem += strTmp;
				break;
			case DOUBLE_TYPE:
				strTmp.Format(_T("%s DOUBLE"), itemname[i]);
				strItem += strTmp;
				break;
			case DATE_TYPE:
				strTmp.Format(_T("%s DATE"), itemname[i]);
				strItem += strTmp;
				break;
			case DATEAUTO_TYPE:
				strTmp.Format(_T("%s TIMESTAMP"), itemname[i]);
				strItem += strTmp;
				break;
			case TEXT_TYPE:
				strTmp.Format(_T("%s CHAR(%d)"), itemname[i], itemhundred[i]);
				strItem += strTmp;
				break;	
		}

		if((itemnum - 1) > i)
		{
			strItem += ",";
		}
	}
	strQuery.Format(_T("CREATE TABLE %s(%s);"), tablename, strItem);

	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strTmp = mysql_error(&m_pMySql[nNum]);

		if(strTmp == _T("Table 'automation_machine' already exists"))
		{
			return TRUE;
		}
		else
		{
			strMsg	= mysql_error(&m_pMySql[nNum]);
			clsMem.OnNormalMessageWrite(strMsg);

			return FALSE;
		}
	}

	return TRUE;
}

BOOL CAlg_Mysql::Table_Delete(int nNum, CString tablename)
{
	CString strQuery;
	CString strMsg;

	char chQurey[8192];

	strQuery = _T("DROP TABLE ") + tablename;
	
	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return FALSE;
	}

	return TRUE;
}

BOOL CAlg_Mysql::Record_Add(int nNum, CString tablename, CString *itemdata, BOOL bFlag)
{
	// 레코드 추가 
	MYSQL_RES *Res;
	MYSQL_ROW Row;
	MYSQL_FIELD *Fields;

	CString strValue;
	CString strItem;
	CString strTmp;
	CString strQuery;
	CString strType;
	CString strMsg;

	char chQurey[8192];

	int i;
	int pos;
	int num;

	strQuery.Format(_T("DESCRIBE %s"), tablename);
	
	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return FALSE;
	}
	
	Res = mysql_store_result(&m_pMySql[nNum]);
	num = (int)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);

	for(i=0; i<num; i++)
	{
		if (bFlag == FALSE)
		{
			Row = mysql_fetch_row(Res);

			if(itemdata[i] != _T(""))
			{
				strItem += Row[0];
				strTmp = Row[1];
				pos = strTmp.Find(_T("("), 0);
				if(pos < 0)
				{
					pos = strTmp.GetLength();
				}
				strType = strTmp.Mid(0, pos);
				strType.MakeUpper();

				strTmp = _T("");
				if(strType == _T("INT"))
				{
					strTmp += itemdata[i];
				}
				else if(strType == _T("FLOAT"))
				{
					strTmp += itemdata[i];
				}
				else if(strType == _T("DOUBLE"))
				{
					strTmp += itemdata[i];
				}
				else if(strType == _T("DATETIME"))
				{
					strTmp.Format(_T("'%s'"), itemdata[i]);
				}
				else if(strType == _T("DATE"))
				{
					strTmp.Format(_T("'%s'"), itemdata[i]);
				}
				else if(strType == _T("TIMESTAMP"))
				{

				}
				else if(strType == _T("VARCHAR"))
				{
					strTmp.Format(_T("'%s'"), itemdata[i]);
				}
				else if(strType == _T("CHAR"))
				{
					strTmp.Format(_T("'%s'"), itemdata[i]);
				}
				else
				{
					strTmp += itemdata[i];
				}

				strValue += strTmp;

				if((num - 1) > i)
				{
					strItem += _T(",");
					strValue += _T(",");
				}
			}
		}
		else if (bFlag == TRUE)
		{
			Row = mysql_fetch_row(Res);

			if (i>0)
			{
				if(itemdata[i-1] != _T(""))
				{
					strItem += Row[0];
					strTmp = Row[1];
					pos = strTmp.Find(_T("("), 0);
					if(pos < 0)
					{
						pos = strTmp.GetLength();
					}
					strType = strTmp.Mid(0, pos);
					strType.MakeUpper();

					strTmp = _T("");
					if(strType == _T("INT"))
					{
						strTmp += itemdata[i-1];
					}
					else if(strType == _T("FLOAT"))
					{
						strTmp += itemdata[i-1];
					}
					else if(strType == _T("DOUBLE"))
					{
						strTmp += itemdata[i-1];
					}
					else if(strType == _T("DATETIME"))
					{
						strTmp.Format(_T("'%s'"), itemdata[i-1]);
					}
					else if(strType == _T("DATE"))
					{
						strTmp.Format(_T("'%s'"), itemdata[i-1]);
					}
					else if(strType == _T("TIMESTAMP"))
					{

					}
					else if(strType == _T("VARCHAR"))
					{
						strTmp.Format(_T("'%s'"), itemdata[i-1]);
					}
					else if(strType == _T("CHAR"))
					{
						strTmp.Format(_T("'%s'"), itemdata[i-1]);
					}
					else
					{
						strTmp += itemdata[i-1];
					}

					strValue += strTmp;

					if((num - 1) > i)
					{
						strItem += _T(",");
						strValue += _T(",");
					}
				}
			}
		}
	}

	strQuery.Format(_T("INSERT INTO %s(%s) VALUES(%s)"), tablename, strItem, strValue);

	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return FALSE;
	}
	
	mysql_free_result(Res);
	return TRUE;
}

void CAlg_Mysql::MySql_Close(int nNum)
{
	m_bFlag = FALSE;
	mysql_close(&m_pMySql[nNum]);
}

BOOL CAlg_Mysql::Database_Select(int nNum, CString databasename)
{
	// 데이터 베이스 선택하기 
	CString strMsg;

	char chDb[1024];

	clsFunc.OnStringToChar(databasename, chDb);
	if(mysql_select_db(&m_pMySql[nNum], chDb))
	{
		return FALSE;
	}

	return TRUE;
}


BOOL CAlg_Mysql::Record_Updata(int nNum, CString tablename, CString search_item_1, CString search_data_1, CString search_item_2, CString search_data_2, CString change_item, CString data)
{
	CString strQuery;
	CString strMsg;

	char chQurey[8192];

	if(search_item_2 == _T(""))
	{
		strQuery.Format(_T("UPDATE %s SET %s='%s' where %s='%s'"), tablename, change_item, data, search_item_1, search_data_1);
	}
	else
	{
		strQuery.Format(_T("UPDATE %s SET %s='%s' where %s='%s' and %s='%s'"), tablename, change_item, data, search_item_1, search_data_1, search_item_2, search_data_2);
	}

	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return FALSE;
	}
	
	return TRUE;
}

BOOL CAlg_Mysql::Record_Int_Delete(int nNum, CString tablename, CString itemname, CString range, int value[])
{
	CString strQuery;
	CString strMsg;

	char chQurey[8192];

	if(range == _T("<"))
	{
		strQuery.Format(_T("DELETE FROM %s WHERE %s < %d"), tablename, itemname, value[0]);
	}
	else if(range == _T(">"))
	{
		strQuery.Format(_T("DELETE FROM %s WHERE %s > %d"), tablename, itemname, value[0]);
	}
	else if(range == _T("<>"))
	{
		strQuery.Format(_T("DELETE FROM %s WHERE %s > %d AND %s < %d"), tablename, itemname, value[0], itemname, value[1]);
	}
	else if(range == _T("><"))
	{
		strQuery.Format(_T("DELETE FROM %s WHERE %s < %d OR %s > %d"), tablename, itemname, value[0], itemname, value[1]);
	}
	else
	{
		strQuery.Format(_T("DELETE FROM %s WHERE %s = %d"), tablename, itemname, value[0]);
	}
	
	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return FALSE;
	}
	return TRUE;
}

BOOL CAlg_Mysql::Record_Float_Delete(int nNum, CString tablename, CString itemname, CString range, float value[])
{
	CString strQuery;
	CString strMsg;

	char chQurey[8192];

	if(range == _T("<"))
	{
		strQuery.Format(_T("DELETE FROM %s WHERE %s < %f"), tablename, itemname, value[0]);
	}
	else if(range == _T(">"))
	{
		strQuery.Format(_T("DELETE FROM %s WHERE %s > %f"), tablename, itemname, value[0]);
	}
	else if(range == _T("<>"))
	{
		strQuery.Format(_T("DELETE FROM %s WHERE %s > %f AND %s < %f"), tablename, itemname, value[0], itemname, value[1]);
	}
	else if(range == _T("><"))
	{
		strQuery.Format(_T("DELETE FROM %s WHERE %s < %f OR %s > %f"), tablename, itemname, value[0], itemname, value[1]);
	}
	else
	{
		strQuery.Format(_T("DELETE FROM %s WHERE %s = %f"), tablename, itemname, value[0]);
	}
	
	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return FALSE;
	}
	return TRUE;
}

BOOL CAlg_Mysql::Record_Double_Delete(int nNum, CString tablename, CString itemname, CString range, double value[])
{
	CString strQuery;
	CString strMsg;

	char chQurey[8192];

	if(range == _T("<"))
	{
		strQuery.Format(_T("DELETE FROM %s WHERE %s < %f"), tablename, itemname, value[0]);
	}
	else if(range == _T(">"))
	{
		strQuery.Format(_T("DELETE FROM %s WHERE %s > %f"), tablename, itemname, value[0]);
	}
	else if(range == _T("<>"))
	{
		strQuery.Format(_T("DELETE FROM %s WHERE %s > %f AND %s < %f"), tablename, itemname, value[0], itemname, value[1]);
	}
	else if(range == _T("><"))
	{
		strQuery.Format(_T("DELETE FROM %s WHERE %s < %f OR %s > %f"), tablename, itemname, value[0], itemname, value[1]);
	}
	else
	{
		strQuery.Format(_T("DELETE FROM %s WHERE %s = %f"), tablename, itemname, value[0]);
	}
	
	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return FALSE;
	}
	return TRUE;
}

BOOL CAlg_Mysql::Record_Text_Delete(int nNum, CString tablename, CString itemname, CString value)
{
	CString strQuery;
	CString strMsg;

	char chQurey[8192];

	strQuery.Format(_T("DELETE FROM %s WHERE %s = '%s'"), tablename, itemname, value);

	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return FALSE;
	}
	return TRUE;
}

BOOL CAlg_Mysql::Record_Date_Delete(int nNum, CString tablename, CString itemname, CString range, COleDateTime time[])
{
	CString strQuery;
	CString strTmp[2];
	CString strMsg;

	char chQurey[8192];

	if(range == _T("<"))
	{
		strTmp[0].Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());
		strQuery.Format(_T("DELETE FROM %s WHERE %s < '%s'"), tablename, itemname, strTmp[0]);
	}
	else if(range == _T(">"))
	{
		strTmp[0].Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());
		strQuery.Format(_T("DELETE FROM %s WHERE %s > '%s'"), tablename, itemname, strTmp[0]);
	}
	else if(range == _T("<>"))
	{
		strTmp[0].Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());

		strTmp[1].Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), time[1].GetYear(), 
														  time[1].GetMonth(),
														  time[1].GetDay(),
														  time[1].GetHour(),
														  time[1].GetMinute(),
														  time[1].GetSecond());
		strQuery.Format(_T("DELETE FROM %s WHERE %s > '%s' AND %s < '%s'"), tablename, itemname, strTmp[0], itemname, strTmp[1]);
	}
	else if(range == _T("><"))
	{
		strTmp[0].Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());

		strTmp[1].Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), time[1].GetYear(), 
														  time[1].GetMonth(),
														  time[1].GetDay(),
														  time[1].GetHour(),
														  time[1].GetMinute(),
														  time[1].GetSecond());
		strQuery.Format(_T("DELETE FROM %s WHERE %s < '%s' OR %s > '%s'"), tablename, itemname, strTmp[0], itemname, strTmp[1]);
	}
	else
	{
		strTmp[0].Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());
		strQuery.Format(_T("DELETE FROM %s WHERE %s = '%s'"), tablename, itemname, strTmp[0]);
	}

	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return FALSE;
	}
	return TRUE;
}

long CAlg_Mysql::Record_Int_Search(int nNum, CString tablename, CString itemsearch, CString range, int value[])
{
	MYSQL_RES *Res;
	MYSQL_ROW Row;
	MYSQL_FIELD *Fields;

	CString strQuery;
	CString stritem;
	CString strTmp[2], strData;
	CString strMsg;

	char chQurey[8192];

	int i, j;
	int num;
	m_nfind_cnt[nNum] = 0;

	stritem = _T("");

	strQuery.Format(_T("DESCRIBE %s"), tablename);
	
	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return FALSE;
	}
	
	Res = mysql_store_result(&m_pMySql[nNum]);
	num = (int)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);
	
	for(i=0; i<num; i++)
	{
		Row = mysql_fetch_row(Res);
		
		stritem += Row[0];
		if((num-1) != i)
		{
			stritem += ",";
		}
	}

	if(range == _T("<"))
	{
		strQuery.Format(_T("SELECT %s FROM %s WHERE %s > %d"), stritem, tablename, itemsearch, value[0]);
	}
	else if(range == _T(">"))
	{
		strQuery.Format(_T("SELECT %s FROM %s WHERE %s < %d"), stritem, tablename, itemsearch, value[0]);
	}
	else if(range == _T("<>"))
	{
		strQuery.Format(_T("SELECT %s FROM %s WHERE %s > %d AND %s < %d"), stritem, tablename, itemsearch, value[0], itemsearch, value[1]);
	}
	else if(range == _T("><"))
	{
		strQuery.Format(_T("SELECT %s FROM %s WHERE %s < %d OR %s > %d"), stritem, tablename, itemsearch, value[0], itemsearch, value[1]);
	}
	else
	{
		strQuery.Format(_T("SELECT %s FROM %s WHERE %s = %d"), stritem, tablename, itemsearch, value[0]);
	}

	mysql_free_result(Res);

	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return m_nfind_cnt[nNum];
	}

	Res = mysql_store_result(&m_pMySql[nNum]);
	m_nfind_cnt[nNum] = (long)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);

	m_strquery[nNum] = new CString[m_nfind_cnt[nNum]];

	for(i=0; i<m_nfind_cnt[nNum]; i++)
	{
		num = mysql_num_fields(Res);
		Row = mysql_fetch_row(Res);

		if(Row == NULL)
		{
			mysql_free_result(Res);
		}

		strData = _T("");
		for(j=0; j<num; j++)
		{
			strData += Row[j];
			strData += _T(",");
		}

		m_strquery[nNum][i] = strData;
	}
	
	mysql_free_result(Res);
	m_nread_cnt[nNum] = 0;

	return m_nfind_cnt[nNum];
}

long CAlg_Mysql::Record_Float_Search(int nNum, CString tablename, CString itemsearch, CString range, float value[])
{
	MYSQL_RES *Res;
	MYSQL_ROW Row;
	MYSQL_FIELD *Fields;

	CString strQuery;
	CString stritem;
	CString strData;
	CString strMsg;

	char chQurey[8192];

	int i, j;
	int num;
	m_nfind_cnt[nNum] = 0;

	stritem = _T("");
	strQuery.Format(_T("DESCRIBE %s"), tablename);
	
	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return m_nfind_cnt[nNum];
	}
	
	Res = mysql_store_result(&m_pMySql[nNum]);
	num = (int)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);
	
	for(i=0; i<num; i++)
	{
		Row = mysql_fetch_row(Res);
		
		stritem += Row[0];
		if((num-1) != i)
		{
			stritem += ",";
		}
	}
	
	if(range == _T("<"))
	{
		strQuery.Format(_T("SELECT %s FROM %s WHERE %s > %f"), stritem, tablename, itemsearch, value[0]);
	}
	else if(range == _T(">"))
	{
		strQuery.Format(_T("SELECT %s FROM %s WHERE %s < %f"), stritem, tablename, itemsearch, value[0]);
	}
	else if(range == _T("<>"))
	{
		strQuery.Format(_T("SELECT %s FROM %s WHERE %s > %f AND %s < %f"), stritem, tablename, itemsearch, value[0], itemsearch, value[1]);
	}
	else if(range == _T("><"))
	{
		strQuery.Format(_T("SELECT %s FROM %s WHERE %s < %f OR %s > %f"), stritem, tablename, itemsearch, value[0], itemsearch, value[1]);
	}
	else
	{
		strQuery.Format(_T("SELECT %s FROM %s WHERE %s = %f"), stritem, tablename, itemsearch, value[0]);
	}

	mysql_free_result(Res);

	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return m_nfind_cnt[nNum];
	}

	Res = mysql_store_result(&m_pMySql[nNum]);
	m_nfind_cnt[nNum] = (long)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);

	m_strquery[nNum] = new CString[m_nfind_cnt[nNum]];

	for(i=0; i<m_nfind_cnt[nNum]; i++)
	{
		num = mysql_num_fields(Res);
		Row = mysql_fetch_row(Res);

		if(Row == NULL)
		{
			mysql_free_result(Res);
		}

		strData = _T("");
		for(j=0; j<num; j++)
		{
			strData += Row[j];
			strData += _T(",");
		}

		m_strquery[nNum][i] = strData;
	}

	mysql_free_result(Res);
	m_nread_cnt[nNum] = 0;

	return m_nfind_cnt[nNum];
}

long CAlg_Mysql::Record_Double_Search(int nNum, CString tablename, CString itemsearch, CString range, double value[])
{
	MYSQL_RES *Res;
	MYSQL_ROW Row;
	MYSQL_FIELD *Fields;

	CString strQuery;
	CString stritem;
	CString strData;
	CString strMsg;

	char chQurey[8192];

	int i, j;
	int num;
	m_nfind_cnt[nNum] = 0;

	stritem = _T("");
	strQuery.Format(_T("DESCRIBE %s"), tablename);
	
	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return m_nfind_cnt[nNum];
	}

	Res = mysql_store_result(&m_pMySql[nNum]);
	num = (int)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);
	
	for(i=0; i<num; i++)
	{
		Row = mysql_fetch_row(Res);
		
		stritem += Row[0];
		if((num-1) != i)
		{
			stritem += _T(",");
		}
	}

	//	SELECT name,age from guest where age <= 25 ;
	if(range == _T("<"))
	{
		strQuery.Format(_T("SELECT %s FROM %s WHERE %s > %f"), stritem, tablename, itemsearch, value[0]);
	}
	else if(range == _T(">"))
	{
		strQuery.Format(_T("SELECT %s FROM %s WHERE %s < %f"), stritem, tablename, itemsearch, value[0]);
	}
	else if(range == _T("<>"))
	{
		strQuery.Format(_T("SELECT %s FROM %s WHERE %f <= %s AND %s <= %f"), stritem, tablename, value[0], itemsearch, itemsearch, value[1]);
	}
	else if(range == _T("><"))
	{
		strQuery.Format(_T("SELECT %s FROM %s WHERE %s > %f OR %s < %f"), stritem, tablename, itemsearch, value[0], itemsearch, value[1]);
	}
	else
	{
		strQuery.Format(_T("SELECT %s FROM %s WHERE %s = %f"), stritem, tablename, itemsearch, value[0]);
	}

	mysql_free_result(Res);

	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return m_nfind_cnt[nNum];
	}

	Res = mysql_store_result(&m_pMySql[nNum]);
	m_nfind_cnt[nNum] = (long)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);

	m_strquery[nNum] = new CString[m_nfind_cnt[nNum]];

	for(i=0; i<m_nfind_cnt[nNum]; i++)
	{
		num = mysql_num_fields(Res);
		Row = mysql_fetch_row(Res);

		if(Row == NULL)
		{
			mysql_free_result(Res);
		}

		strData = _T("");
		for(j=0; j<num; j++)
		{
			strData += Row[j];
			strData += _T(",");
		}

		m_strquery[nNum][i] = strData;
	}

	mysql_free_result(Res);
	m_nread_cnt[nNum] = 0;

	return m_nfind_cnt[nNum];
}

long CAlg_Mysql::Record_Double_Search(int nNum, CString tablename, CString itemsearch1, CString itemsearch2, double x[], double y[])
{
	MYSQL_RES *Res;
	MYSQL_ROW Row;
	MYSQL_FIELD *Fields;
	
	CString strQuery;
	CString stritem;
	CString strData;
	CString strMsg;

	char chQurey[8192];

	int i, j;
	int num;
	m_nfind_cnt[nNum] = 0;
	
	stritem = _T("");
	strQuery.Format(_T("DESCRIBE %s"), tablename);
	
	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return m_nfind_cnt[nNum];
	}
	
	Res = mysql_store_result(&m_pMySql[nNum]);
	num = (int)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);
	
	for(i=0; i<num; i++)
	{
		Row = mysql_fetch_row(Res);
		
		stritem += Row[0];
		if((num-1) != i)
		{
			stritem += _T(",");
		}
	}
	
	//	SELECT name,age from guest where age <= 25 ;
	
	strQuery.Format(_T("SELECT %s FROM %s WHERE %f <= %s AND %s <= %f AND %f <= %s AND %s <= %f"), stritem, tablename, x[0], itemsearch1, itemsearch1, x[1], y[0], itemsearch2, itemsearch2, y[1]);
	
	
	mysql_free_result(Res);
	
	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return m_nfind_cnt[nNum];
	}
	
	Res = mysql_store_result(&m_pMySql[nNum]);
	m_nfind_cnt[nNum] = (long)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);
	
	m_strquery[nNum] = new CString[m_nfind_cnt[nNum]];
	
	for(i=0; i<m_nfind_cnt[nNum]; i++)
	{
		num = mysql_num_fields(Res);
		Row = mysql_fetch_row(Res);
		
		if(Row == NULL)
		{
			mysql_free_result(Res);
		}
		
		strData = _T("");
		for(j=0; j<num; j++)
		{
			strData += Row[j];
			strData += _T(",");
		}
		
		m_strquery[nNum][i] = strData;
	}
	
	mysql_free_result(Res);
	m_nread_cnt[nNum] = 0;
	
	return m_nfind_cnt[nNum];
}

long CAlg_Mysql::Record_Text_Search(int nNum, CString tablename, CString itemsearch, CString value)
{
	MYSQL_RES *Res;
	MYSQL_ROW Row;
	MYSQL_FIELD *Fields;

	CString strQuery;
	CString stritem;
	CString strTmp[2], strData;
	CString strMsg;

	char chQurey[8192];

	int i, j;
	int num;
	m_nfind_cnt[nNum] = 0;

	stritem = _T("");
	strQuery.Format(_T("DESCRIBE %s"), tablename);
	//strQuery.Format(_T("SELECT *FROM %s"), tablename);
	
	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return m_nfind_cnt[nNum];
	}
	
	Res = mysql_store_result(&m_pMySql[nNum]);
	num = (int)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);
	
	for(i=0; i<num; i++)
	{
		Row = mysql_fetch_row(Res);
		
		stritem += Row[0];
		if((num-1) != i)
		{
			stritem += _T(",");
		}
	}

	strQuery.Format(_T("SELECT *FROM %s WHERE %s = '%s'"), tablename, itemsearch, value);

	mysql_free_result(Res);

	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		mysql_free_result(m_pRes[nNum]);
		return m_nfind_cnt[nNum];
	}

	Res = mysql_store_result(&m_pMySql[nNum]);
	m_nfind_cnt[nNum] = (long)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);

	m_strquery[nNum] = new CString[m_nfind_cnt[nNum]];

	for(i=0; i<m_nfind_cnt[nNum]; i++)
	{
		num = mysql_num_fields(Res);
		Row = mysql_fetch_row(Res);

		if(Row == NULL)
		{
			mysql_free_result(Res);
		}

		strData = _T("");
		for(j=0; j<num; j++)
		{
			strData += Row[j];
			strData += _T(",");
		}

		m_strquery[nNum][i] = strData;
	}

	mysql_free_result(Res);
	m_nread_cnt[nNum] = 0;

	return m_nfind_cnt[nNum];
}

long CAlg_Mysql::Record_Text_Search(int nNum, CString tablename, CString itemsearch1, CString itemsearch2, CString value1, CString value2)
{
	MYSQL_RES *Res;
	MYSQL_ROW Row;
	MYSQL_FIELD *Fields;
	
	CString strQuery;
	CString stritem;
	CString strTmp[2], strData;
	CString strMsg;

	char chQurey[8192];
	int i, j;
	int num;
	m_nfind_cnt[nNum] = 0;
	
	stritem = _T("");
	strQuery.Format(_T("DESCRIBE %s"), tablename);
	
	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return m_nfind_cnt[nNum];
	}
	
	Res = mysql_store_result(&m_pMySql[nNum]);
	num = (int)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);
	
	for(i=0; i<num; i++)
	{
		Row = mysql_fetch_row(Res);
		
		stritem += Row[0];
		if((num-1) != i)
		{
			stritem += _T(",");
		}
	}
	
	strQuery.Format(_T("SELECT %s FROM %s WHERE %s = '%s' AND %s = '%s'"), stritem, tablename, itemsearch1, value1, itemsearch2, value2);
	
	mysql_free_result(Res);
	
	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);
		
		mysql_free_result(m_pRes[nNum]);
		return m_nfind_cnt[nNum];
	}
	
	Res = mysql_store_result(&m_pMySql[nNum]);
	m_nfind_cnt[nNum] = (long)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);
	
	m_strquery[nNum] = new CString[m_nfind_cnt[nNum]];
	
	for(i=0; i<m_nfind_cnt[nNum]; i++)
	{
		num = mysql_num_fields(Res);
		Row = mysql_fetch_row(Res);
		
		if(Row == NULL)
		{
			mysql_free_result(Res);
		}
		
		strData = _T("");
		for(j=0; j<num; j++)
		{
			strData += Row[j];
			strData += _T(",");
		}
		
		m_strquery[nNum][i] = strData;
	}
	
	mysql_free_result(Res);
	m_nread_cnt[nNum] = 0;
	
	return m_nfind_cnt[nNum];
}

long CAlg_Mysql::Record_Date_Search(int nNum, CString tablename, CString itemsearch, CString range, COleDateTime time[])
{
	MYSQL_RES *Res;
	MYSQL_ROW Row;
	MYSQL_FIELD *Fields;

	CString strQuery;
	CString stritem;
	CString strTmp[2], strData;
	CString strMsg;

	char chQurey[8192];

	int i, j;
	int num;
	m_nfind_cnt[nNum] = 0;
	
	stritem = _T("");
	strQuery.Format(_T("DESCRIBE %s"), tablename);
	
	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return m_nfind_cnt[nNum];
	}
	
	Res = mysql_store_result(&m_pMySql[nNum]);
	num = (int)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);
	
	for(i=0; i<num; i++)
	{
		Row = mysql_fetch_row(Res);
		stritem += Row[0];
		if((num-1) != i)
		{
			stritem += _T(",");
		}
	}

	if(range == _T("<"))
	{
		strTmp[0].Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());

		strQuery.Format(_T("SELECT %s FROM %s WHERE %s > '%s'"), stritem, tablename, itemsearch, strTmp[0]);
	}
	else if(range == _T(">"))
	{
		strTmp[0].Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());

		strQuery.Format(_T("SELECT %s FROM %s WHERE %s < '%s'"), stritem, tablename, itemsearch, strTmp[0]);
	}
	else if(range == _T("<>"))
	{
		strTmp[0].Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());
		strTmp[1].Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), time[1].GetYear(), 
														  time[1].GetMonth(),
														  time[1].GetDay(),
														  time[1].GetHour(),
													      time[1].GetMinute(),
														  time[1].GetSecond());
		strQuery.Format(_T("SELECT %s FROM %s WHERE %s > '%s' AND %s < '%s'"), stritem, tablename, itemsearch, strTmp[0], itemsearch, strTmp[1]);
	}
	else if(range == _T("><"))
	{
		strTmp[0].Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());
		strTmp[1].Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), time[1].GetYear(), 
														  time[1].GetMonth(),
														  time[1].GetDay(),
														  time[1].GetHour(),
														  time[1].GetMinute(),
														  time[1].GetSecond());
		strQuery.Format(_T("SELECT %s FROM %s WHERE %s < '%s' OR %s > '%s'"), stritem, tablename, itemsearch, strTmp[0], itemsearch, strTmp[1]);
	}
	else
	{
		strTmp[0].Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());
		strQuery.Format(_T("SELECT %s FROM %s WHERE %s = '%s'"), stritem, tablename, itemsearch, strTmp[0]);
	}

	mysql_free_result(Res);

	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		mysql_free_result(m_pRes[nNum]);
		return m_nfind_cnt[nNum];
	}

	Res = mysql_store_result(&m_pMySql[nNum]);
	m_nfind_cnt[nNum] = (long)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);

	m_strquery[nNum] = new CString[m_nfind_cnt[nNum]];

	for(i=0; i<m_nfind_cnt[nNum]; i++)
	{
		num = mysql_num_fields(Res);
		Row = mysql_fetch_row(Res);

		if(Row == NULL)
		{
			mysql_free_result(Res);
		}

		strData = _T("");
		for(j=0; j<num; j++)
		{
			strData += Row[j];
			strData += _T(",");
		}

		m_strquery[nNum][i] = strData;
	}

	mysql_free_result(Res);
	m_nread_cnt[nNum] = 0;

	return m_nfind_cnt[nNum];
}

BOOL CAlg_Mysql::Table_Select(int nNum, CString databasename, CString tablename)
{
	CString strQuery;
	int num, i;
	CString strTmp;
	CString strMsg;

	char chQurey[8192];

	BOOL find;

	MYSQL_RES *Res;
	MYSQL_ROW Row;
	MYSQL_FIELD *Fields;

	clsFunc.OnStringToChar(_T("SHOW TABLES"), chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return FALSE;
	}

	Res = mysql_store_result(&m_pMySql[nNum]);
	num = (int)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);
	
	find = FALSE;
	for(i=0; i<num; i++)
	{
		Row = mysql_fetch_row(Res);
		
		strTmp = Row[0];

		strTmp.MakeUpper();
		tablename.MakeUpper();
		
		if(strTmp == tablename)
		{
			find = TRUE;
			break;
		}
	}

	mysql_free_result(Res);

	if(find)
	{
		clsFunc.OnStringToChar(tablename, chQurey);
		if(mysql_select_db(&m_pMySql[nNum], chQurey))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	
	return FALSE;
}

CString CAlg_Mysql::Record_Read(int nNum)
{
	CString strTmp;

	strTmp = _T("");
	m_nread_cnt[nNum]++;

	strTmp = m_strquery[nNum][m_nread_cnt[nNum]-1];
	if(m_nread_cnt == m_nfind_cnt)
	{
		if (m_strquery[nNum] != NULL)
		{
			delete [] m_strquery[nNum];
			m_strquery[nNum] = NULL;
		}
	}
	return strTmp;
}

BOOL CAlg_Mysql::WaitTime(int nNum, int mode, int time)
{
	CString strQuery;

	char chQurey[8192];

	if(mode == 0)	
	{	
		strQuery.Format(_T("set global max_connections=%d"), time);
	}
	else	
	{	
		strQuery.Format(_T("set wait_timeout=%d"), time);	
	}
	
	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))		
	{
		return FALSE;	
	}
	
	return TRUE;    
}

BOOL CAlg_Mysql::Record_Sort(int nNum, int mode, CString tablename, CString itemsearch)
{
	CString strQuery;
	CString strMsg;

	char chQurey[8192];

	if(mode == 0)
	{
		strQuery.Format(_T("SELECT * FROM %s ORDER BY %s ASC"), tablename, itemsearch);
	}
	else
	{
		strQuery.Format(_T("SELECT * FROM %s ORDER BY %s DESC"), tablename, itemsearch);
	}

	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))		
	{
		return FALSE;	
	}
	
	return TRUE;
}

/*
SELECT * INTO OUTFILE 'YOUR_FILE.CSV'
 FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"'
 LINES TERMINATED BY '\n'
 FROM YOUR_TABLE;
*/
BOOL CAlg_Mysql::WriteFile(int nNum, CString strFileName)
{
	CString strQuery;
	CString strMsg;

	char chQurey[8192];
	//DATA
	strQuery.Format(_T("SELECT * INTO OUTFILE '%s' FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '%c' LINES TERMINATED BY '\r\n' FROM DATA"), strFileName, _T('"'));	

	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))		
	{
		return FALSE;
	}

	return TRUE;
}

/*
Load Data Local InFile 'FILE_PATH' Into Table androids FIELDS TERMINATED BY ',' ENCLOSED BY '"' LINES TERMINATED BY '\n'
*/

BOOL CAlg_Mysql::ReadFile(int nNum, CString strFileName)
{
	CString strQuery;
	CString strMsg;

	char chQurey[8192];

	//DATA
	strQuery.Format(_T("LOAD DATA LOCAL INFILE '%s' INTO TABLE DATA FIELDS TERMINATED BY ',' ENCLOSED BY '%c' LINES TERMINATED BY '\n'"), strFileName, _T('"'));	
//	strQuery.Format("SELECT * INTO INFILE '%s' FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '%c' LINES TERMINATED BY '\r\n' FROM DATA", strFileName, '"');	

	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))		
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);
		
		return FALSE;
	}

	return TRUE;
}

// SELECT MAX(article) AS article FROM shop
float CAlg_Mysql::Record_Max(int nNum, CString item)
{
	MYSQL_RES *Res;
	MYSQL_ROW Row;
	MYSQL_FIELD *Fields;
	int num;

	CString strQuery;
	CString stritem;
	CString strMsg;

	char chQurey[8192];

	int i;

	float dVal = 0.0;

	//DATA
	strQuery.Format(_T("SELECT MAX(%s) AS %s FROM DATA"), item, item);	

	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))		
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);
		
		return dVal;
	}

	Res = mysql_store_result(&m_pMySql[nNum]);
	num = (int)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);

	for(i=0; i<num; i++)
	{
		Row = mysql_fetch_row(Res);
		
		stritem += Row[0];
		if((num-1) != i)
		{
			stritem += _T(",");
		}
	}

	dVal = (float)_wtof(stritem);

	return dVal;
}

float CAlg_Mysql::Record_Min(int nNum, CString item)
{
	MYSQL_RES *Res;
	MYSQL_ROW Row;
	MYSQL_FIELD *Fields;
	int num;

	CString strQuery;
	CString stritem;
	CString strMsg;

	char chQurey[8192];

	int i;

	float dVal = 0.0;

	//DATA
	strQuery.Format(_T("SELECT MIN(%s) AS %s FROM DATA"), item, item);	

	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))		
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);
		
		return dVal;
	}

	Res = mysql_store_result(&m_pMySql[nNum]);
	num = (int)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);

	for(i=0; i<num; i++)
	{
		Row = mysql_fetch_row(Res);
		
		stritem += Row[0];
		if((num-1) != i)
		{
			stritem += _T(",");
		}
	}

	dVal = (float)_wtof(stritem);

	return dVal;
}

//BOOL CAlg_Mysql::Get_Record_List(CString recordname, CString tablename, int *nRecordBuff)
//{
//	MYSQL_RES *Res;
//	MYSQL_ROW Row;
//	MYSQL_FIELD *Fields;
//
//	int num, i;
//	CString strQuery;
//	CString strMsg;
//	char chQurey[8192];
//
//	strQuery.Format(_T("SELECT %s FROM %s"), recordname, tablename);
//
//	clsFunc.OnStringToChar(strQuery, chQurey);
//	if(mysql_query(&m_pMySql, chQurey))		
//	{
//		strMsg	= mysql_error(&m_pMySql);
//		AfxMessageBox(strMsg);
//
//		return FALSE;
//	}
//	Res = mysql_store_result(&m_pMySql);
//	
//	Fields = mysql_fetch_fields(Res);
//	num = (int)mysql_num_rows(Res);
//	nRecordBuff[0] = num;
//
//	for (i=0; i<num; i++)
//	{
//		Row = mysql_fetch_row(Res);
//		nRecordBuff[i+1] = atoi(Row[0]);
//	}
//
//	return TRUE;
//}

//BOOL CAlg_Mysql::Get_Record_List(CString recordname, CString tablename, int *nBuffCnt, CString *strRecordBuff)
//{
//	MYSQL_RES *Res;
//	MYSQL_ROW Row;
//	MYSQL_FIELD *Fields;
//
//	int num, i;
//	CString strQuery;
//	CString strMsg;
//	char chQurey[8192];
//
//	strQuery.Format(_T("SELECT %s FROM %s"), recordname, tablename);
//
//	clsFunc.OnStringToChar(strQuery, chQurey);
//	if(mysql_query(&m_pMySql, chQurey))		
//	{
//		strMsg	= mysql_error(&m_pMySql);
//		AfxMessageBox(strMsg);
//
//		return FALSE;
//	}
//	Res = mysql_store_result(&m_pMySql);
//
//	Fields = mysql_fetch_fields(Res);
//	num = (int)mysql_num_rows(Res);
//	*nBuffCnt = num;
//
//	for (i=0; i<num; i++)
//	{
//		Row = mysql_fetch_row(Res);
//		strRecordBuff[i] = Row[0];
//	}
//
//	return TRUE;
//}