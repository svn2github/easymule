#pragma once

#include "MainTabWnd.h"

class CCmdMainTabOp
{
public:
	CCmdMainTabOp(void);
	~CCmdMainTabOp(void);

	BOOL		IsTabShowed(CMainTabWnd::ETabId eTabId);
	void		AddTabById(CMainTabWnd::ETabId eTabId);
	void		RemoveTabById(CMainTabWnd::ETabId eTabId);
};
