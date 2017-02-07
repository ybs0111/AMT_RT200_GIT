#if !defined(AFX_Animate_H__31696054_73E4_4AC0_9F8C_C6739EEC9C94__INCLUDED_)
#define AFX_Animate_H__31696054_73E4_4AC0_9F8C_C6739EEC9C94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Animate.h : header file
//
//#include "DCBmp.h"
/////////////////////////////////////////////////////////////////////////////
// CAnimate window

// ======CAxisMod============
class CAxisMod:public CWnd
{
public:
	CAxisMod();
	virtual ~CAxisMod();

public:
	double  m_dEqCurPos_X;
	double  m_dEqCurPos_Y;
	double  m_dEqWidth;
	int		m_nImgCurPos_X;
	int		m_nImgCurPos_Y;
	CPoint  m_HomePoint;
	double  m_dRatio;
	double	m_dRatio_Y;
	
	CDC      m_DcAxis[5]; 
  	CBitmap  m_BitAxis[5];
	CBitmap* m_pBitAxis[5];	
	CDC      m_DcAxisMask[5]; 
  	CBitmap  m_BitAxisMask[5];
	CBitmap* m_pBitAxisMask[5];

	CSize	m_ImgSize[5];

public:

	void SetImgHomePos(const CPoint point)		{m_HomePoint    = point;}
	void SetImgPosX(const int x)				{m_nImgCurPos_X = x;}
	void SetImgPosY(const int y)				{m_nImgCurPos_Y = y;}
	void SetRatioEqToImg(const double dValue)	{m_dRatio		= dValue;}								
//	HBITMAP GetRotatedBitmap(HBITMAP hBitmap, float radians, COLORREF clrBack);

public: 
	virtual void ComverEqPosToImgPos(int nMode);							// 실제 좌표  - >  이미지 좌표  
	virtual void DrawImg(CDC &dc, int nMode);	
	virtual void CreateImg(CDC *pDC, int nImg1, int nImgMask1, int nImg2, int nImgMask2, int nImg3, int nImgMask3, int nImg4, int nImgMask4, int nImg5, int nImgMask5);// 이미지  원 이미지와 MASK 이미지로 비트맵 생성
};

// ======CGlass============
class CEqGlass:CWnd
{
public:
	CEqGlass();	
	virtual ~CEqGlass();
	
	CSize	 m_sizGlassSize,m_sizGlassSizeMod;
	CPoint   m_nGlassPos;
	int		 m_nDegree;
	

public: 
	void SetCellDir(int nDir) {m_nDir = nDir;}
	void CellRectCf(CRect &CellRect);	
	void RotateRectAngle(CRect rect,double nDegree, CRect rectCen);
	void DrawImg(CDC &dc);

private:
	int	   m_nDir;
	CPoint m_arrGlassPoint[4];

};

class CAnimate : public CWnd
{
// Construction
public:
	CAnimate();
	
	CDC				m_pDcFrame; 
   	CBitmap			m_bmpFrame;
 	CBitmap*		m_pbmpFrame;

	CDC				m_pDcLfTsite; 
	CBitmap			m_bmpLfTsite; 
	CBitmap*		m_pbmpLfTsite; 

	CDC				m_pDcRiTsite;
	CBitmap			m_bmpRiTiste; 
	CBitmap*		m_pbmpRiTsite; 

	CDC				m_pDcCvyAssy;
	CBitmap			m_bmpCvyAssy; 
	CBitmap*		m_pbmpCvyAssy; 

	CDC				m_pDcLdBuff;
	CBitmap			m_bmpLdBuff; 
	CBitmap*		m_pbmpLdBuff; 

	CDC				m_pDcUnldBuff;
	CBitmap			m_bmpUnldBuff; 
	CBitmap*		m_pbmpUnldBuff; 

	CDC				m_pDcRetestBuff;
	CBitmap			m_bmpRetestBuff; 
	CBitmap*		m_pbmpRetestBuff; 

	CDC				m_pDcRejectBuff;
	CBitmap			m_bmpRejectBuff; 
	CBitmap*		m_pbmpRejectBuff; 

	CDC				m_pDcCokBuff;
	CBitmap			m_bmpCokBuff; 
	CBitmap*		m_pbmpCokBuff; 
	
	CDC				m_pDcLdRbt;
	CBitmap			m_bmpLdRbt; 
	CBitmap*		m_pbmpLdRbt; 

	CDC				m_pDcUnldRbt;
	CBitmap			m_bmpUnldRbt; 
	CBitmap*		m_pbmpUnldRbt; 

	CDC				m_pDcTSiteRbt;
	CBitmap			m_bmpTSiteRbt; 
	CBitmap*		m_pbmpTSiteRbt; 

	CDC				m_pDcLdCvyFr;
	CBitmap			m_bmpLdCvyFr; 
	CBitmap*		m_pbmpLdCvyFr;

	CDC				m_pDcLdCvyRe;
	CBitmap			m_bmpLdCvyRe; 
	CBitmap*		m_pbmpLdCvyRe;

	CDC				m_pDcLdRailFr;
	CBitmap			m_bmpLdRailFr; 
	CBitmap*		m_pbmpLdRailFr;

	CDC				m_pDcLdRailRe;
	CBitmap			m_bmpLdRailRe; 
	CBitmap*		m_pbmpLdRailRe;

	CDC				m_pDcEmptyStk;
	CBitmap			m_bmpEmptyStk; 
	CBitmap*		m_pbmpEmptyStk;

	CDC				m_pDcUnldRail;
	CBitmap			m_bmpUnldRail; 
	CBitmap*		m_pbmpUnldRail;

	CDC				m_pDcUnldElv;
	CBitmap			m_bmpUnldElv; 
	CBitmap*		m_pbmpUnldElv;

	CDC				m_pDcUnldCvy;
	CBitmap			m_bmpUnldCvy; 
	CBitmap*		m_pbmpUnldCvy;

	CDC				m_pDcMvCvy;
	CBitmap			m_bmpMvCvy; 
	CBitmap*		m_pbmpMvCvy;

	CRect m_BtnRect[3];
	BOOL m_bClicked[3];

	int nRandCol;
	int nColChange;
	int nColChangeWait[3];

public:

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnimate)
	public:
	//}}AFX_VIRTUAL
	virtual BOOL Create ( DWORD exstyle, DWORD style, const CRect& r,CWnd *p, UINT id, CCreateContext *pc=NULL ) ;

// Implementation
public:
	void OnTestMove(double &dPos,int nTpos);
	void OnActionDisplay();
	void OnMotorStepMonitoring(CDC *pDC);
	void OnMagazineCntCheck(CDC *pDC, int nMagazineLoadExist, int nMagazineUnloadExist);

	void OnBtnUndLfInfoReset(CDC *pDC);
	void OnBtnCovLfInfoReset(CDC *pDC);
	void OnInitAniWnd();
	void OnAniPioStatus();
	virtual ~CAnimate();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAnimate)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
private:
	CFont	m_AniFont;
	CPen	m_AniPen;
	CBrush  m_AniBrush;
	CFont	m_AniBicFont;
	CFont	m_AniSmallFont;
	CPoint	m_CursorPoint;
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Animate_H__31696054_73E4_4AC0_9F8C_C6739EEC9C94__INCLUDED_)
