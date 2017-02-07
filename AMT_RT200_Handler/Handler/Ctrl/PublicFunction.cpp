#include "StdAfx.h"
#include "PublicFunction.h"

CPublicFunction clsFunc;

CPublicFunction::CPublicFunction(void)
{
	int i;
	
	for(i=0; i<20; i++)
	{
		m_pFont[i]	= NULL;
		m_pFont[i]	= new CFont;
		m_pFont[i]->CreateFont(i + 10,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0, _T("MS Sans Serif"));		//Bitstream Vera Sans Mono
	}
}


CPublicFunction::~CPublicFunction(void)
{
	int i;
	
	for(i=0; i<20; i++)
	{
		if (m_pFont[i] != NULL)
		{
			delete m_pFont[i];
			m_pFont[i] = NULL;
		}
	}
}

LOGFONT CPublicFunction::OnLogFont(int nFontSize)
{
	LOGFONT lf;
	::ZeroMemory(&lf,sizeof(LOGFONT));
	lf.lfHeight = nFontSize;
	lf.lfWeight = 10;
	lf.lfWeight = 10;
	lf.lfItalic = false;
	lf.lfUnderline = false;
	lf.lfStrikeOut = false;
	lf.lfCharSet=VIETNAMESE_CHARSET;

	return lf;
}