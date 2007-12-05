// TimerOpBase.cpp : 实现文件
//

#include "stdafx.h"
#include "TimerOpBase.h"


// CTimerOpBase

IMPLEMENT_DYNAMIC(CTimerOpBase, CWnd)
CTimerOpBase::CTimerOpBase()
{
	m_wParam		= 0;
	m_lParam		= 0;
	m_uLeftTimes	= 0;

	CreateEx(0, _T("static"), NULL, 0, CRect(0, 0, 0, 0), NULL, 0);
}

CTimerOpBase::~CTimerOpBase()
{
	Stop();
	DestroyWindow();
}

BEGIN_MESSAGE_MAP(CTimerOpBase, CWnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CTimerOpBase 消息处理程序

BOOL CTimerOpBase::Start(UINT uInterval, WPARAM wParam, LPARAM lParam, UINT uTimes)
{
	m_wParam		= wParam;
	m_lParam		= lParam;
	m_uLeftTimes	= uTimes;

	SetTimer(1, uInterval, NULL);

	return TRUE;
}

void CTimerOpBase::Stop()
{
	KillTimer(1);

	m_wParam		= 0;
	m_lParam		= 0;
	m_uLeftTimes	= 0;
}

void CTimerOpBase::OnTimer(UINT nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (1 == nIDEvent)
	{
		if (0 != m_uLeftTimes)	// 当 0 == m_uLeftTimes 时，表示让定时器一直循环下去。
		{
			m_uLeftTimes--;
			if (0 == m_uLeftTimes)
				KillTimer(1);
		}
		
		TimerOp(m_wParam, m_lParam);
	}

	CWnd::OnTimer(nIDEvent);
}
