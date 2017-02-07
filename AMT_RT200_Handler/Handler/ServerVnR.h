#pragma once
class CServerVnR
{
public:
	int		m_nDataCount;

	double	m_dVolt[10][60];
	double  m_dResistance[10][60];

	void	OnMakeTime();
	void	OnMakeAsd();
	void	OnMakePngok();
	void OnDataReceive(CString strRev);

public:
	CServerVnR(void);
	~CServerVnR(void);
};
extern CServerVnR clsVnR;

