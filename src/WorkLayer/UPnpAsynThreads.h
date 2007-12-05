#pragma once

#include "UPnpMgr.h"
#include "UPnpNatMapping.h"

class CUPnpAsynThreadsParam
{
public:
	CUPnpMgr			*pUpnpMgr;
	CUPnpNatMapping		mapping;
	BOOL				bRetryRand;
	MSG					notifyParam;
	DWORD				dwCustomParam;
	BOOL				bHasCleanedFillupBug;
};

class CUPnpAsynThreadsResult
{
public:
	DWORD				dwCustomParam;
	WORD				wInternalPort;
	WORD				wExternalPort;
	BOOL				bCleanedFillupBug;
	DWORD				dwActionErrorCode;
};

class CUPnpAsynThreads
{
public:
	CUPnpAsynThreads(void);
	~CUPnpAsynThreads(void);


	static void AddNatPortMappingAsyn(CUPnpMgr	*pMgr,
										const CUPnpNatMapping &mapping,
										HWND hNotifyWnd,
										UINT uNotifyMessage,
										BOOL bRetryRand = FALSE,
										DWORD dwCustomParam = 0,
										BOOL bHasCleanedFillupBug = TRUE);
	static UINT AFX_CDECL AddNATPortMappingAsynProc(LPVOID lpParam);

	static void CleanMapedPortQuickly(CUPnpMgr *pMgr);
};
