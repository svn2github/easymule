// CmdFuncs.cpp : 实现文件
//

#include "stdafx.h"
#include "CmdFuncs.h"
#include "eMule.h"
#include "eMuleDlg.h"
#include "WebBrowserWnd.h"
#include "DlgAddTask.h"
#include "GlobalVariable.h"

#include "TabWnd.h"
#include "TabItem_Normal.h"
#include "TabItem_NormalCloseable.h"
#include "TabItem_MainButton.h"
#include <StrSafe.h>

#include "DNSManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace CmdFuncs
{
	void	OpenPreferencesWnd(void)
	{
		theApp.emuledlg->ShowPreferences((UINT)-1);
	}

	void	OpenNewUrl(LPCTSTR lpszUrl, LPCTSTR lpszCaption)
	{
		if(theApp.emuledlg->webbrowser!=NULL
			&& theApp.emuledlg->webbrowser->IsBrowserCanUse())
		{
			SetMainActiveTab(CMainTabWnd::TI_RESOURCE);
			theApp.emuledlg->m_mainTabWnd.m_dlgResource.OpenNewUrl(lpszUrl, lpszCaption);
		}
		else
			ShellExecute(NULL, NULL, lpszUrl, NULL, thePrefs.GetMuleDirectory(EMULE_EXECUTEABLEDIR), SW_SHOWDEFAULT);


	}

	void	GotoGuide()
	{
		OpenNewUrl(_T("http://www.emule.org.cn/guide/"), GetResString(IDS_HELP));
	}

	void	SetMainActiveTab(CMainTabWnd::ETabId eTabId)
	{
		theApp.emuledlg->m_mainTabWnd.SetActiveTab(theApp.emuledlg->m_mainTabWnd.m_aposTabs[eTabId]);
	}

	void	PopupNewTaskDlg(void)
	{
		CDlgAddTask::PopBlankTaskDlg();
	}


	void	AddEd2kLinksToDownload(CString strlink, int cat)
	{
		CDlgAddTask::AddNewTask(strlink, cat);
	}

	void	AddFileLinkToDownload(CED2KFileLink* pLink, int cat)
	{
		CDlgAddTask::AddNewTask(pLink, cat);
	}

	void	AddSearchToDownload(CSearchFile* toadd, uint8 paused, int cat)
	{
		CDlgAddTask::AddNewTask(toadd, paused, cat);
	}

	void	AddSearchToDownload(CString strlink, uint8 paused, int cat)
	{
		CDlgAddTask::AddNewTask(strlink, paused, cat);
	}

	void	AddUrlToDownload(LPCTSTR lpszUrl)
	{
		CDlgAddTask::AddNewUrlTask(lpszUrl);
	}
	void	AddMultiLinksTask(LPCTSTR lpszLinks)
	{
		CDlgAddTask::AddMultiLinks(lpszLinks);
	}

	CString	GetFileSizeDisplayStr(const EMFileSize &fs)
	{
		uint64		u64FileSize;
		CString		strFileSize;

		u64FileSize = fs;
		if (u64FileSize > 1024*1024)
			strFileSize.Format(_T("%.2lf MB"), ((double)u64FileSize)/(1024*1024));
		else if (u64FileSize > 1024)
			strFileSize.Format(_T("%.2lf KB"), ((double)u64FileSize)/1024);
		else
			strFileSize.Format(_T("%d Bytes"), u64FileSize);

		return strFileSize;
	}

	POSITION	TabWnd_AddNormalTab(CTabWnd *pTabWnd, LPCTSTR lpszCaption, HWND hRelativeWnd, LPCTSTR lpszPngIcon)
	{
		CTabItem_Normal	*pNormalTabItem = NULL;

		pNormalTabItem = new CTabItem_Normal;
		pNormalTabItem->SetCaption(lpszCaption);
		pNormalTabItem->SetRelativeWnd(hRelativeWnd);
		pNormalTabItem->SetIcon(lpszPngIcon);
		return pTabWnd->AddTab(pNormalTabItem);
	}

	POSITION	TabWnd_AddCloseTab(CTabWnd *pTabWnd, LPCTSTR lpszCaption,
									HWND hRelativeWnd, BOOL bAutoDelRelaWndObject, CWnd* pRelaWndObjectToDel,
									BOOL bSetActive)
	{
		CTabItem_NormalCloseable	*pTabItem = NULL;

		pTabItem = new CTabItem_NormalCloseable;
		pTabItem->SetCaption(lpszCaption);
		pTabItem->SetRelativeWnd(hRelativeWnd, NULL, bAutoDelRelaWndObject, pRelaWndObjectToDel);
		return pTabWnd->AddTab(pTabItem, bSetActive);
	}

	POSITION	TabWnd_AddMainButton(CTabWnd *pTabWnd, LPCTSTR lpszCaption, HWND hRelativeWnd, LPCTSTR lpszPngIcon, LPCTSTR lpszPngActiveIcon,
									BOOL bSetActive, POSITION posInsertBeside, BOOL bAfter)
	{
		CTabItem_MainButton	*pTabItem = NULL;

		pTabItem = new CTabItem_MainButton;
		pTabItem->SetCaption(lpszCaption);
		pTabItem->SetRelativeWnd(hRelativeWnd);
		pTabItem->SetIcon(lpszPngIcon);
		pTabItem->SetActivedIcon(lpszPngActiveIcon);
		return pTabWnd->AddTab(pTabItem, bSetActive, posInsertBeside, bAfter);
	}

	void	SetShareTabText(LPCTSTR lpszText)
	{
		theApp.emuledlg->m_mainTabWnd.m_dlgShare.SetShareText(lpszText);
	}

	void	SetResActiveSearchTabText(LPCTSTR lpszText)
	{
		CTabWnd *ptw = &(theApp.emuledlg->m_mainTabWnd.m_dlgResource.m_tabWnd);
		POSITION posActive = ptw->GetActiveTab();

		DWORD dwCustomData = ptw->GetTabData(posActive);
		DWORD HighData = dwCustomData & 0xFFFF0000;

		
		if (HighData == 0xF0F00000)
		{
			ptw->SetTabText(posActive, lpszText);
		}
	}

	void	UpdateResSearchParam(int iIndex, SSearchParams *pSearchParams)
	{
		theApp.emuledlg->m_mainTabWnd.m_dlgResource.UpdateSearchParam(iIndex, pSearchParams);
	}

	void	ImportUnfinishedTasks(void)
	{
		TCHAR buffer[MAX_PATH] = {0};
		
		StringCchCopy(buffer, MAX_PATH, thePrefs.GetMuleDirectory(EMULE_INCOMINGDIR));
		if(SelectDir(AfxGetMainWnd()->GetSafeHwnd(),buffer,GetResString(IDS_SEL_IMPORT_TASK_DIR), NULL, FALSE))
		{
			CGlobalVariable::downloadqueue->ScanPartFile(CString(buffer));
		}

	}

	void	OpenFolder(CKnownFile *file)
	{
		if (file && !file->IsPartFile())
		{
			CString	strParam;
			strParam.Format(_T(" %s, /select, %s"), file->GetPath(), file->GetFilePath());
			ShellExecute(NULL, _T("open"), _T("explorer.exe"), strParam, NULL, SW_SHOW);
		}
	}

	//注意: strLocation 只是存放的的路径名，不包含文件名
	void ActualllyAddUrlDownload(const CString &strUrl, const CString & strLocation, bool bNewTask,CFileTaskItem* pFileTaskItem)
	{
		//CString strUrlEncoded = URLEncode( strUrl );
		//CString urlDecoded = OptUtf8ToStr(URLDecode(strUrl));

		//CString urlConverted = UrlConvert(strUrl);

		// VC-SearchDream[2007-04-06]: Direct HTTP and FTP DownLoad
		if ( (strUrl.Left(7).CompareNoCase(_T("http://")) == 0) || (strUrl.Left(6).CompareNoCase(_T("ftp://")) == 0) )
		{
			TCHAR szScheme[INTERNET_MAX_SCHEME_LENGTH];
			TCHAR szHostName[INTERNET_MAX_HOST_NAME_LENGTH];
			TCHAR szUrlPath[INTERNET_MAX_PATH_LENGTH];
			TCHAR szUserName[INTERNET_MAX_USER_NAME_LENGTH];
			TCHAR szPassword[INTERNET_MAX_PASSWORD_LENGTH];
			TCHAR szExtraInfo[INTERNET_MAX_URL_LENGTH];

			// URL Split
			URL_COMPONENTS Url = {0};
			Url.dwStructSize = sizeof(Url);
			Url.lpszScheme = szScheme;
			Url.dwSchemeLength = ARRSIZE(szScheme);
			Url.lpszHostName = szHostName;
			Url.dwHostNameLength = ARRSIZE(szHostName);
			Url.lpszUserName = szUserName;
			Url.dwUserNameLength = ARRSIZE(szUserName);
			Url.lpszPassword = szPassword;
			Url.dwPasswordLength = ARRSIZE(szPassword);
			Url.lpszUrlPath = szUrlPath;
			Url.dwUrlPathLength = ARRSIZE(szUrlPath);
			Url.lpszExtraInfo = szExtraInfo;
			Url.dwExtraInfoLength = ARRSIZE(szExtraInfo);

			if (InternetCrackUrl(strUrl, 0, 0, &Url) && Url.dwHostNameLength > 0)
			{
				CPartFile* pPartFile=NULL;
				if( CGlobalVariable::filemgr.AddDownLoadRequest(strUrl,strLocation, pPartFile,bNewTask) ) //urlConverted
				{					
					if( bNewTask )
					{
						SUnresolvedHostname* hostname = new SUnresolvedHostname;
						hostname->strURL = strUrl;//hostname->strURL = urlConverted;
						hostname->strHostname = szHostName;
						CGlobalVariable::m_DNSManager->AddToResolved(pPartFile, hostname,NULL);
						delete hostname;
					}
					else
					{
						pPartFile->StopFile();
						if(pFileTaskItem) 
							pFileTaskItem->m_metBakId = pPartFile->GetMetBakId();
					}
				}
			}
		}
	}
}
