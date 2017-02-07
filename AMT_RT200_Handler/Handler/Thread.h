// Thread.h:
//
//////////////////////////////////////////////////////////////////////

#ifndef _AFX_THREAD_H__
#define _AFX_THREAD_H__

extern CWinThread *m_thread[10];
extern HANDLE m_thrHandle[10];

//==================================================================//
// 전역 쓰레드 선언
//==================================================================//
extern UINT OnThreadHandlerCheck(LPVOID  lParam);		// m_thread[1]
extern UINT	OnThreadReadIO(LPVOID lParam);				// m_thread[0]
extern UINT	OnThreadTowerLamp(LPVOID lParam);			// m_thread[2]
extern UINT OnThreadAlarmDisplay(LPVOID lParam);		// m_thread[3]
extern UINT OnThreadPicker(LPVOID lParam);				// m_thread[4]
extern UINT OnThreadRobot(LPVOID lParam);				// m_thread[5]
extern UINT OnThreadConv(LPVOID lParam);				// m_thread[6]
extern UINT OnThreadLabel(LPVOID lParam);				// m_thread[7]
extern int AllStop[2];  // 쓰레드 강제 멈춤 플래그

#endif