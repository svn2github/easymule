#pragma once

#include <AfxTempl.h>
#include <AfxMt.h>

class CThreadsMgr
{
public:
	CThreadsMgr(void);
	~CThreadsMgr(void);

	typedef void (*CLEAN_PROC) (HANDLE hThread);
	static void	CleanProc_WaitAndDelWinThd(HANDLE hThread);
	static void	CleanProc_DelWinThd(HANDLE hThread);

	static CWinThread* BegingThreadAndRecDown(CLEAN_PROC cleanProc, AFX_THREADPROC pfnThreadProc, LPVOID pParam,
												int nPriority = THREAD_PRIORITY_NORMAL, UINT nStackSize = 0,
												DWORD dwCreateFlags = 0, LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL);
	static CWinThread* BegingThreadAndRecDown(CLEAN_PROC cleanProc, CRuntimeClass* pThreadClass,
												int nPriority = THREAD_PRIORITY_NORMAL, UINT nStackSize = 0,
												DWORD dwCreateFlags = 0, LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL);


	void	RegThread(DWORD dwThreadId, CLEAN_PROC pfnCleanProc);
	void	UnregThread(DWORD dwThreadId);

	void	CleanAllThreads();
protected:
	CMap<DWORD, DWORD, CLEAN_PROC, const CLEAN_PROC&>	m_mapRecs;
	CCriticalSection m_cs;
};

extern CThreadsMgr	theThreadsMgr;

class CUnregThreadAssist
{
public:
	CUnregThreadAssist(DWORD dwThreadId){m_dwThreadId = dwThreadId;}
	~CUnregThreadAssist(){theThreadsMgr.UnregThread(m_dwThreadId);}
protected:
	DWORD	m_dwThreadId;
};
