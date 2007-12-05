#pragma once

#include "stdafx.h"
#include "MainTabWnd.h"
#include "ED2KLink.h"
#include "SearchFile.h"

class CTabWnd;


namespace CmdFuncs
{
	void	OpenPreferencesWnd(void);
	void	OpenNewUrl(LPCTSTR lpszUrl, LPCTSTR lpszCaption);
	void	GotoGuide();
	void	SetMainActiveTab(CMainTabWnd::ETabId eTabId);

	void	PopupNewTaskDlg(void);
	void	AddEd2kLinksToDownload(CString strlink, int cat = 0);
	void	AddFileLinkToDownload(CED2KFileLink* pLink, int cat = 0);
	void	AddSearchToDownload(CSearchFile* toadd, uint8 paused = 2, int cat = 0);
	void	AddSearchToDownload(CString strlink, uint8 paused = 2, int cat = 0);
	void	AddMultiLinksTask(LPCTSTR lpszLinks);

	void	AddUrlToDownload(LPCTSTR lpszUrl);

	CString	GetFileSizeDisplayStr(const EMFileSize &fs);


	POSITION	TabWnd_AddNormalTab(CTabWnd *pTabWnd, LPCTSTR lpszCaption, HWND hRelativeWnd, LPCTSTR lpszPngIcon = NULL);
	POSITION	TabWnd_AddCloseTab(CTabWnd *pTabWnd, LPCTSTR lpszCaption,
									HWND hRelativeWnd, BOOL bAutoDelRelaWndObject = FALSE, CWnd* pRelaWndObjectToDel = NULL,
									BOOL bSetActive = FALSE);
	POSITION	TabWnd_AddMainButton(CTabWnd *pTabWnd, LPCTSTR lpszCaption, HWND hRelativeWnd, LPCTSTR lpszPngIcon = NULL, LPCTSTR lpszPngActiveIcon = NULL,
								BOOL bSetActive = FALSE, POSITION posInsertBeside = NULL, BOOL bAfter = TRUE);

	void	SetShareTabText(LPCTSTR lpszText);

	void	SetResActiveSearchTabText(LPCTSTR lpszText);
	void	UpdateResSearchParam(int iIndex, SSearchParams *pSearchParams);

	void	ImportUnfinishedTasks(void);

	void	OpenFolder(CKnownFile *file);

	void ActualllyAddUrlDownload(const CString &strUrl, const CString & strLocation,bool bNewTask = true,CFileTaskItem* pFileTaskItem=NULL);
}
