// AdvanceTabWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "AdvanceTabWnd.h"
#include ".\advancetabwnd.h"

#include "eMule.h"
#include "eMuleDlg.h"
#include "TabItem_Normal.h"
#include "PageTabBkDraw.h"
#include "ServerWnd.h"
#include "KademliaWnd.h"
#include "StatisticsDlg.h"

#include "TabItem_Wnd.h"

// CAdvanceTabWnd

IMPLEMENT_DYNAMIC(CAdvanceTabWnd, CTabWnd)
CAdvanceTabWnd::CAdvanceTabWnd()
{
}

CAdvanceTabWnd::~CAdvanceTabWnd()
{
}


BEGIN_MESSAGE_MAP(CAdvanceTabWnd, CTabWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()


void CAdvanceTabWnd::Localize()
{
	SetTabText(m_aposTabs[TI_SERVER], GetResString(IDS_SERVER));
	SetTabText(m_aposTabs[TI_KAD], GetResString(IDS_EM_KADEMLIA));
	SetTabText(m_aposTabs[TI_STAT], GetResString(IDS_EM_STATISTIC));
}

void CAdvanceTabWnd::InitToolBar(void)
{
	m_toolbar.Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, AFX_IDW_TOOLBAR);
	m_toolbar.SetOwner(theApp.emuledlg);
	m_toolbar.SetIndent(8);

	//CSize	szToolBar;
	//m_toolbar.GetMaxSize(&szToolBar);

	CTabItem_Wnd	*pTabItemWnd = new CTabItem_Wnd;
	pTabItemWnd->SetItemWnd(&m_toolbar, FALSE);
	//pTabItemWnd->SetWindowLength(szToolBar.cx + 8);
	pTabItemWnd->SetDynDesireLength(TRUE);
	AddTab(pTabItemWnd);
	pTabItemWnd = NULL;

}

// CAdvanceTabWnd 消息处理程序


int CAdvanceTabWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTabWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	SetBarBkDraw(new CPageTabBkDraw);
	SetBarMarginLogic(CRect(10, 0, 0, 0));

	InitToolBar();

	CTabItem_Normal	*pNormalTabItem = NULL;

	pNormalTabItem = new CTabItem_Normal;
	pNormalTabItem->SetCaption(GetResString(IDS_SERVER));
	pNormalTabItem->SetRelativeWnd(theApp.emuledlg->serverwnd->GetSafeHwnd());
	m_aposTabs[TI_SERVER] = AddTab(pNormalTabItem);
	pNormalTabItem = NULL;

	pNormalTabItem = new CTabItem_Normal;
	pNormalTabItem->SetCaption(GetResString(IDS_EM_KADEMLIA));
	pNormalTabItem->SetRelativeWnd(theApp.emuledlg->kademliawnd->GetSafeHwnd());
	m_aposTabs[TI_KAD] = AddTab(pNormalTabItem);
	pNormalTabItem = NULL;

	pNormalTabItem = new CTabItem_Normal;
	pNormalTabItem->SetCaption(GetResString(IDS_EM_STATISTIC));
	pNormalTabItem->SetRelativeWnd(theApp.emuledlg->statisticswnd->GetSafeHwnd());
	m_aposTabs[TI_STAT] = AddTab(pNormalTabItem);
	pNormalTabItem = NULL;

	return 0;
}
