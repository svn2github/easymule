#pragma once

#include "TimerOpBase.h"
// CTimerOp_UrlClientRetry

class CTimerOp_UrlClientRetry : public CTimerOpBase
{
	DECLARE_DYNAMIC(CTimerOp_UrlClientRetry)

public:
	CTimerOp_UrlClientRetry();
	virtual ~CTimerOp_UrlClientRetry();

protected:
	virtual	void TimerOp(WPARAM wParam, LPARAM lParam);

protected:
	DECLARE_MESSAGE_MAP()
};


