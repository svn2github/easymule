#include "StdAfx.h"
#include ".\cmdmaintabop.h"
#include "eMule.h"
#include "eMuleDlg.h"


CCmdMainTabOp::CCmdMainTabOp(void)
{
}

CCmdMainTabOp::~CCmdMainTabOp(void)
{
}

BOOL CCmdMainTabOp::IsTabShowed(CMainTabWnd::ETabId eTabId)
{
	return theApp.emuledlg->m_mainTabWnd.IsTabShowed(eTabId);
}

void CCmdMainTabOp::AddTabById(CMainTabWnd::ETabId eTabId)
{
	theApp.emuledlg->m_mainTabWnd.AddTabById(eTabId);
}

void CCmdMainTabOp::RemoveTabById(CMainTabWnd::ETabId eTabId)
{
	theApp.emuledlg->m_mainTabWnd.RemoveTabById(eTabId);
}
