#pragma once

#include "Variable.h"
typedef struct _EC_F_CLIENT_QUEUE_
{
	int			m_nRear;
	int			m_nFront;
	int			m_nCount;

	char		m_chMsg[100][5000];
} EC_F_CLIENT_QUEUE, *P_EC_F_CLIENT_QUEUE;

class CClientEcFirst
{
public:
	HANDLE					m_hMemMapEcFirst;
	P_EC_F_CLIENT_QUEUE		m_pQueueEcFirst;

	int						m_nHeader;
	int						m_nHeaderLength;
	int						m_nTestEndFlag;
	int						m_nOnlineEndFlag;
	int						m_nScrapInfoFlag;
	int						m_nSeeLotFlag;
	int						m_nDelScrapFlg;
	int						m_nLotEndFlag;
	int						m_nVerFlag;
	int						m_nStatusFlag;
	int						m_nModeChangFlag;
	int						m_nFullCarrierFlag;
	int						m_nEmptyCarrierFlag;
	int						m_nToolChangFlag;
	int						m_nMatchFlag;
	int						m_nDateFlag;
	// jtkim 20150806
	int						m_nPcbInfoFlag;
	CString					m_strSerial;
	CString					m_strPpid;
	CString					m_strWwn;
	CString					m_strCserial;
	CString					m_strPsid;

	CString					m_strHeader;
	CString					m_strData;

	// jtkim 20150805
	void					OnEcPcbInfo(CString strLotId, CString strPartId, CString strSerial);
	void					OnOpen();
	void					OnClose();
	void					OnEcSLotEnd(tagPCB_INFO pcb_info);
	void					OnEcTestEnd(CString strLotId, CString strPartId);
	void					OnEcOnlineEnd(tagLOT_DISPLAY_INFO lot_display);
	void					OnEcScrapInfo(CString strLotId);
	void					OnEcSeeLot(CString strLotId);
	void					OnEcDeleteScrap(CString strLotId, CString strSerial, CString strArray);
	void					OnEcLotEnd(CString strLotId);
	void					OnEqpVersionUpdate(CString strDescrip);
	void					OnEqpStatusChange(CString strMode, CString strCode, CString strDescrip);
	void					OnEqpModeChange(CString strMode);
	void					OnEqpFullCarrierMove(CString srtCarrierId);
	void					OnEqpEmptyCarrierMove();
	void					OnEqpToolChange(CString strToolId);
	void					OnEqpMatch(CString strLotId, CString strCarrierId);
	void					OnEqpDate();
	void					OnCommunication();
	void					OnMessageWrite(CString strMsg);
	CString					OnMessageRead();
	CString					OnCharToString(char chData[], int nLength);
	void					OnStringToChar(CString strData, char chData[]);
	void					OnDataReceive(CString strMsg);
	void					OnDataAnalysis(CString strMsg);
	CString					OnDataItemAnalysis(int nOpt, int nPos, CString strMsg, CString strFind);

public:
	bool					m_bThread;

	int						m_nRetry;
	int						m_nCommStep;
	int						m_nCommBin;

	CString					m_strSendMsg;
	CString					m_strErrorMsg;

	DWORD					m_dwWaitTime[3];

	HANDLE					m_thrHandle;

public:
	CClientEcFirst(void);
	~CClientEcFirst(void);
};
extern CClientEcFirst clsEcFirst;
