#pragma once

class CWBNotifyReceiver
{
public:
	CWBNotifyReceiver(void);
	virtual ~CWBNotifyReceiver(void) = 0;

	virtual void OnTitleChanged(LPCTSTR /*lpszTitle*/){};
	virtual void OnProgressChanged(long /*lProgress*/, long /*lProgressMax*/){};
};
