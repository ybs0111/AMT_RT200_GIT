#pragma once
class CPublicFunction
{
public:
	CFont			*m_pFont[20];

	LOGFONT			OnLogFont(int nFontSize);

	CPublicFunction(void);
	~CPublicFunction(void);
};

extern CPublicFunction clsFunc;