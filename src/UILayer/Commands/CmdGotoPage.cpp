#include "StdAfx.h"
#include ".\cmdgotopage.h"
#include "eMule.h"
#include "eMuleDlg.h"

CCmdGotoPage::CCmdGotoPage(void)
{
}

CCmdGotoPage::~CCmdGotoPage(void)
{
}

void CCmdGotoPage::GotoDownloading(void)
{
	theApp.emuledlg->m_mainTabWnd.SetActiveTabById(CMainTabWnd::TI_DOWNLOAD);
	theApp.emuledlg->m_mainTabWnd.m_dlgDownload.SetDownloadlistActiveTab(CDlgMaintabDownload::TI_DOWNLOADING);
}
