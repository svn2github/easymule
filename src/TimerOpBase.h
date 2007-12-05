#pragma once


// CTimerOpBase

class CTimerOpBase : public CWnd
{
	DECLARE_DYNAMIC(CTimerOpBase)

public:
	CTimerOpBase();
	virtual ~CTimerOpBase();

	BOOL	Start(UINT uInterval, WPARAM wParam = 0, LPARAM lParam = 0, UINT uTimes = 1);
	void	Stop();

protected:
	virtual	void TimerOp(WPARAM wParam, LPARAM lParam) = 0;
	WPARAM		m_wParam;
	LPARAM		m_lParam;
	UINT		m_uLeftTimes;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT nIDEvent);
};


