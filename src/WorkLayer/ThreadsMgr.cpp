#include "StdAfx.h"
#include ".\threadsmgr.h"

CThreadsMgr	theThreadsMgr;

CThreadsMgr::CThreadsMgr(void)
{
}

CThreadsMgr::~CThreadsMgr(void)
{
	m_mapRecs.RemoveAll();
}

void CThreadsMgr::CleanProc_WaitAndDelWinThd(HANDLE hThread)
{
	if (NULL == hThread)
		return;

	DWORD dwErr = WaitForSingleObject(hThread, 5000);

	if (WAIT_TIMEOUT == dwErr)
	{
		TerminateThread(hThread, 1);
	}

	return;
}

void CThreadsMgr::CleanProc_DelWinThd(HANDLE hThread)
{
	if (NULL == hThread)
		return;

	TerminateThread(hThread, 1);
	return;
}

CWinThread* CThreadsMgr::BegingThreadAndRecDown(CThreadsMgr::CLEAN_PROC cleanProc, AFX_THREADPROC pfnThreadProc, LPVOID pParam,
								   int nPriority, UINT nStackSize,
								   DWORD dwCreateFlags, LPSECURITY_ATTRIBUTES lpSecurityAttrs)
{
	BOOL bResumeThread;
	bResumeThread = (dwCreateFlags & CREATE_SUSPENDED) ? FALSE : TRUE;

	dwCreateFlags |= CREATE_SUSPENDED;
	CWinThread *pThread = ::AfxBeginThread(pfnThreadProc, pParam, nPriority, nStackSize, dwCreateFlags, lpSecurityAttrs);

	if (NULL != pThread)
	{
		theThreadsMgr.RegThread(pThread->m_nThreadID, cleanProc);

		if (bResumeThread)
			pThread->ResumeThread();
	}

	return pThread;
}

CWinThread* CThreadsMgr::BegingThreadAndRecDown(CThreadsMgr::CLEAN_PROC cleanProc, CRuntimeClass* pThreadClass,
									  int nPriority, UINT nStackSize,
									  DWORD dwCreateFlags, LPSECURITY_ATTRIBUTES lpSecurityAttrs)
{
	BOOL bResumeThread;
	bResumeThread = (dwCreateFlags & CREATE_SUSPENDED) ? FALSE : TRUE;

	dwCreateFlags |= CREATE_SUSPENDED;
	CWinThread *pThread = ::AfxBeginThread(pThreadClass, nPriority, nStackSize, dwCreateFlags, lpSecurityAttrs);

	if (NULL != pThread)
	{
		theThreadsMgr.RegThread(pThread->m_nThreadID, cleanProc);
		
		if (bResumeThread)
			pThread->ResumeThread();
	}

	return pThread;
}

void CThreadsMgr::RegThread(DWORD dwThreadId, CLEAN_PROC pfnCleanProc)
{
	ASSERT(NULL != pfnCleanProc);
	if (NULL == pfnCleanProc)
		return;
	
	CSingleLock	localLock(&m_cs, TRUE);
	m_mapRecs.SetAt(dwThreadId, pfnCleanProc);
}

void CThreadsMgr::UnregThread(DWORD dwThreadId)
{
	CSingleLock	localLock(&m_cs, TRUE);
	m_mapRecs.RemoveKey(dwThreadId);
}

void CThreadsMgr::CleanAllThreads()
{
	POSITION	pos;
	DWORD		dwThreadId;
	HANDLE		hThread;
	CLEAN_PROC	pfnCleanProc;

	// MODIFIED by by VC-nightsuns <begin> on 2007/11/08 : 解决程序退出的缓慢
	CMap<DWORD, DWORD, CLEAN_PROC, const CLEAN_PROC&>	tempThreads;
	{
		CSingleLock	localLock(&m_cs, TRUE);
		// 复制一份，CMap竟然不支持 operator = 
		POSITION i = this->m_mapRecs.GetStartPosition();
		while( i ) 
		{
			this->m_mapRecs.GetNextAssoc(i, dwThreadId, pfnCleanProc);
			tempThreads.SetAt( dwThreadId , pfnCleanProc );
		}

//		tempThreads = this->m_mapRecs;
		m_mapRecs.RemoveAll();
	}

	pos = tempThreads.GetStartPosition();
	while (NULL != pos)
	{
		tempThreads.GetNextAssoc(pos, dwThreadId, pfnCleanProc);

		if (NULL != pfnCleanProc)
		{
			hThread = OpenThread(THREAD_ALL_ACCESS, TRUE, dwThreadId);
			pfnCleanProc(hThread);
			CloseHandle(hThread);
		}
	}

	// MODIFIED by by VC-nightsuns <end> on 2007/11/08 : 解决程序退出的缓慢
}
