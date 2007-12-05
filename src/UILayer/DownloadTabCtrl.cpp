#include "StdAfx.h"
#include ".\downloadtabctrl.h"
#include "MenuCmds.h"
#include "resource.h"

#include "emule.h"
#include "emuleDlg.h"
#include "TransferWnd.h"
#include "TabItem_Wnd.h"

CDownloadTabWnd::CDownloadTabWnd(void)
{
}

CDownloadTabWnd::~CDownloadTabWnd(void)
{
}
BEGIN_MESSAGE_MAP(CDownloadTabWnd, CTabWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int CDownloadTabWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTabWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	ModifyStyle(0, WS_CLIPCHILDREN, 0);

	InitToolBar();

	return 0;
}

void CDownloadTabWnd::InitToolBar(void)
{
	m_Toolbar.Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, AFX_IDW_TOOLBAR);
	m_Toolbar.SetOwner(&(theApp.emuledlg->transferwnd->downloadlistctrl));
	m_Toolbar.SetIndent(8);

	CTabItem_Wnd	*pTabItemWnd = new CTabItem_Wnd;
	pTabItemWnd->SetItemWnd(&m_Toolbar, FALSE);

	pTabItemWnd->SetDynDesireLength(TRUE);
	AddTab(pTabItemWnd);
	pTabItemWnd = NULL;
}


void CDownloadTabWnd::OnSize(UINT nType, int cx, int cy)
{
	CTabWnd::OnSize(nType, cx, cy);

	CRect rcWnd;
	GetWindowRect(&rcWnd);
	Invalidate(FALSE);
}
