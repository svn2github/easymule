// TimerOp_UrlClientRetry.cpp : 实现文件
//

#include "stdafx.h"
#include "TimerOp_UrlClientRetry.h"
#include "HttpClient.h"
#include "Preferences.h"
#include "Log.h"


// CTimerOp_UrlClientRetry

IMPLEMENT_DYNAMIC(CTimerOp_UrlClientRetry, CTimerOpBase)
CTimerOp_UrlClientRetry::CTimerOp_UrlClientRetry()
{
}

CTimerOp_UrlClientRetry::~CTimerOp_UrlClientRetry()
{
}

BEGIN_MESSAGE_MAP(CTimerOp_UrlClientRetry, CTimerOpBase)
END_MESSAGE_MAP()

void CTimerOp_UrlClientRetry::TimerOp(WPARAM wParam, LPARAM /*lParam*/)
{
	CHttpClient	*pUrlClient = (CHttpClient*) wParam;

	if (NULL != pUrlClient && !IsBadWritePtr(pUrlClient, sizeof(CHttpClient)) )
	{
		if (thePrefs.GetVerbose())
			AddDebugLogLine(false, _T("Retry to connect <%s>"), pUrlClient->GetUserName());

		pUrlClient->TryToConnect(true);
	}
}


// CTimerOp_UrlClientRetry 消息处理程序

