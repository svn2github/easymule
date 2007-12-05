#pragma once
#include "BrowserToolbarCtrl.h"
#include "HtmlCtrl.h"

class CBrowserToolbarInfo
{
public:
	CBrowserToolbarInfo(void);
	virtual ~CBrowserToolbarInfo(void);

public:
	void SetBrowserToolbarCtrl(CBrowserToolbarCtrl* pBrowserToolbarCtrl);
	CBrowserToolbarCtrl* GetBrowserToolbarCtrl(void);

	void SetHtmlCtrl(CHtmlCtrl*	pHtmlCtrl);
	CHtmlCtrl* GetHtmlCtrl(void);

	BOOL Enable(void);

protected:
	CBrowserToolbarCtrl* m_pBrowserToolbarCtrl;
	CHtmlCtrl*			 m_pHtmlCtrl;
};
