// GridControlAlg.h: interface for the GridControlAlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIDCONTROLALG_H__CA1BC410_875D_4DC1_AB79_8E1B05C82B1C__INCLUDED_)
#define AFX_GRIDCONTROLALG_H__CA1BC410_875D_4DC1_AB79_8E1B05C82B1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ssdllmfc.h"
class GridControlAlg  
{
public:
	int GridCellRow_Search(TSpread *grid, int row, int col_s, int col_e, CString str_find);
	int GridCellCol_Search(TSpread *grid, int col, int row_s, int row_e, CString str_find);
	int GridCellCol(TSpread *grid);
	int GridCellRow(TSpread *grid);
	void GridCellWidth_P(TSpread *grid, int col, int width);
	void GridCellHeight_P(TSpread *grid, int row, int height);
	void GridCellScroll(TSpread *grid, int pos);
	void GridReset(TSpread *grid);
	void GridAutoSize(TSpread *grid, BOOL used);
	void GridCellWidth_L(TSpread *grid, int col, double width);
	void GridCellHeight_L(TSpread *grid, int row, double height);
	void GridFileOpen(TSpread *grid, CString filename, CString sheetname);
	void GridFileSave(TSpread *grid, CString filename, CString sheetname);
	void GridCellMerge(TSpread *grid, int start_row, int start_col, int count_row, int count_col);
	void GridCellCols(TSpread *grid, int cols);
	void GridCellRows(TSpread *grid, int rows);
	CString GridCellText(TSpread *grid, int row, int col);
	void GridCellText(TSpread *grid, int row, int col, CString text);
	void GridCellFont(TSpread *grid, int row, int col, CString fontname, int fontsize);
	void GridCellColor(TSpread *grid, int row, int col, COLORREF bk, COLORREF tk);
	void GridCellSelectDisplay(TSpread *grid, BOOL used);
	void GridVertical(TSpread *grid, BOOL used);
	void GridHorizontal(TSpread *grid, BOOL used);
	void GridColHeader(TSpread *grid, BOOL used);
	void GridRowHeader(TSpread *grid, BOOL used);
	void GridCellControlStatic(TSpread *grid, int row, int col);
	void GridCellControlNumber(TSpread *grid, int row, int col, int min, int max, int pos);
	void GridCellControlCheck(TSpread *grid, int row, int col);
	void GridCellControlTime(TSpread *grid, int row, int col);
	void GridCellControlCombo(TSpread *grid, int row, int col, CString list);
	void GridCellControlEdit(TSpread *grid, int row, int col);
	void GridCellControlDate(TSpread *grid, int row, int col);
	GridControlAlg();
	virtual ~GridControlAlg();

};

#endif // !defined(AFX_GRIDCONTROLALG_H__CA1BC410_875D_4DC1_AB79_8E1B05C82B1C__INCLUDED_)
