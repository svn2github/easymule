// DlgMainTabResource.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgMainTabResource.h"
#include "TabItem_Normal.h"
#include "TabItem_NormalCloseable.h"
#include "emule.h"
#include "emuleDlg.h"
#include "WebBrowserWnd.h"
#include "SearchListCtrl.h"
#include "SearchDlg.h"
#include "SearchResultsWnd.h"
#include "HelpIDs.h"
#include "UserMsgs.h"
#include "CmdFuncs.h"
#include "TabItem_Wnd.h"
#include "PageTabBkDraw.h"
#include "TabItem_WebBrowser.h"
#include "kademlia/kademlia/kademlia.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

SSearchParams* GetParameters(CString expression)
{
	CString strExpression = expression;

	CString strExtension;

	UINT uAvailability = 0;

	UINT uComplete = 0;

	CString strCodec;

	ULONG ulMinBitrate = 0;

	ULONG ulMinLength = 0;
	
	SSearchParams* pParams = new SSearchParams;
	pParams->strExpression = strExpression;
	pParams->eType         = SearchTypeEd2kGlobal;
	pParams->strFileType   = _T("");
	pParams->strMinSize    = _T("");
	pParams->ullMinSize    = 0;
	pParams->strMaxSize    = _T("");
	pParams->ullMaxSize    = 0;
	pParams->uAvailability = uAvailability;
	pParams->strExtension  = strExtension;
	pParams->uComplete     = uComplete;
	pParams->strCodec      = strCodec;
	pParams->ulMinBitrate  = ulMinBitrate;
	pParams->ulMinLength   = ulMinLength;

	return pParams;
}

// CDlgMainTabResource 对话框

IMPLEMENT_DYNAMIC(CDlgMainTabResource, CDialog)
CDlgMainTabResource::CDlgMainTabResource(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CDlgMainTabResource::IDD, pParent)
{
	m_dwCounter = 0;
	m_SearchMap.InitHashTable(1031);
}

CDlgMainTabResource::~CDlgMainTabResource()
{
	// {begin} 09/27/2007 Added by Soar Chin to resolve memory leaking
	POSITION pos = m_SearchMap.GetStartPosition();
	while (pos != NULL) 
	{
		int key;
		SSearchParams * params;
		m_SearchMap.GetNextAssoc(pos, key, params);
		if(params != NULL)
			delete params;
	}
	m_SearchMap.RemoveAll();
	// {end}   09/27/2007 Added by Soar Chin to resolve memory leaking
}

void CDlgMainTabResource::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgMainTabResource, CResizableDialog)
	ON_NOTIFY(NMC_TW_ACTIVE_TAB_CHANDED, IDC_RESOURCE_TAB_WND, OnNMActiveTabChanged)
	ON_NOTIFY(NMC_TW_TAB_DESTROY, IDC_RESOURCE_TAB_WND, OnNMTabDestroy)
	ON_NOTIFY(NMC_TW_TAB_CREATE, IDC_RESOURCE_TAB_WND, OnNMTabCreate)
	ON_MESSAGE(UM_RESTAB_WB_DESTROY, OnWbTabDestroy)
END_MESSAGE_MAP()

void CDlgMainTabResource::OpenNewUrl(LPCTSTR lpszUrl, LPCTSTR lpszCaption, BOOL bSetActive, BOOL bClosable)
{
	CmdFuncs::SetMainActiveTab(CMainTabWnd::TI_RESOURCE);
	CWebBrowserWnd	*pwbw = new CWebBrowserWnd;
	if (NULL != lpszUrl)
		pwbw->SetOpenUrl(lpszUrl);
	pwbw->Create(IDD_WEBBROWSER);

	//CmdFuncs::TabWnd_AddCloseTab(&m_tabWnd, lpszCaption, pwbw->GetSafeHwnd(), TRUE, pwbw, bSetActive);
	POSITION			pos;
	CTabItem_WebBrowser	*pWbItem = NULL;

	pWbItem = new CTabItem_WebBrowser;
	pWbItem->SetCaption(lpszCaption);
	pWbItem->SetRelativeWnd(pwbw->GetSafeHwnd(), NULL, TRUE, pwbw);
	pWbItem->SetWbWnd(pwbw);
	pWbItem->SetCustomData(TCD_WEB_BROWSER);
	pWbItem->EnableClose(bClosable);
	pwbw->SetAssocItem(pWbItem);

	pos = m_tabWnd.AddTab(pWbItem);
	pWbItem = NULL;
	
	if (bSetActive)
		m_tabWnd.SetActiveTab(pos);



}

void CDlgMainTabResource::OpenNewSearchResult(LPCTSTR lpszCaption)
{
	CmdFuncs::SetMainActiveTab(CMainTabWnd::TI_RESOURCE);

	if (CGlobalVariable::serverconnect->IsConnected())
	{
		DWORD CustomData = 0xF0F00000;

		if (m_dwCounter > 0x20)
		{
			return;
		}

		SSearchParams * pSearchParams = GetParameters(lpszCaption);

		CustomData          += m_dwCounter;

		theApp.emuledlg->searchwnd->m_pwndResults->m_iCurSearchIndexInRes = m_dwCounter;
		m_SearchMap.SetAt(m_dwCounter, pSearchParams);
		m_dwCounter++;

		CString	strDisplayText;
		strDisplayText = GetResString(IDS_TABTITLE_SEARCH_RESULT);
		strDisplayText += lpszCaption;


		CTabItem_NormalCloseable	*pTi = NULL;
		pTi = new CTabItem_NormalCloseable;
		pTi->SetCaption(strDisplayText);
		pTi->SetRelativeWnd(theApp.emuledlg->searchwnd->m_pwndResults->GetSafeHwnd());
		pTi->SetCustomData(CustomData);
		pTi->SetDesireLength(150);
		m_tabWnd.AddTab(pTi, TRUE);

		theApp.emuledlg->searchwnd->m_pwndResults->StartSearch(pSearchParams);
	}
	else
	{
		if (Kademlia::CKademlia::IsConnected())
		{
			DWORD CustomData = 0xF0F00000;

			if (m_dwCounter > 0x20)
			{
				return;
			}

			SSearchParams * pSearchParams = GetParameters(lpszCaption);
			pSearchParams->eType = SearchTypeKademlia;

			CustomData          += m_dwCounter;

			theApp.emuledlg->searchwnd->m_pwndResults->m_iCurSearchIndexInRes = m_dwCounter;
			m_SearchMap.SetAt(m_dwCounter, pSearchParams);
			m_dwCounter++;

			CString	strDisplayText;
			strDisplayText = GetResString(IDS_TABTITLE_SEARCH_RESULT);
			strDisplayText += lpszCaption;


			CTabItem_NormalCloseable	*pTi = NULL;
			pTi = new CTabItem_NormalCloseable;
			pTi->SetCaption(strDisplayText);
			pTi->SetRelativeWnd(theApp.emuledlg->searchwnd->m_pwndResults->GetSafeHwnd());
			pTi->SetCustomData(CustomData);
			pTi->SetDesireLength(150);
			m_tabWnd.AddTab(pTi, TRUE);

			theApp.emuledlg->searchwnd->m_pwndResults->StartSearch(pSearchParams);
		}
		else
		{
			MessageBox(GetResString(IDS_ERR_NOTCONNECTED),GetResString(IDS_CAPTION),MB_ICONWARNING);
		}
	}
}

void CDlgMainTabResource::ShowVerycdPage()
{
	OpenNewUrl(NULL, GetResString(IDS_VERYCD), TRUE, FALSE);
	//CTabItem_WebBrowser	*pWbItem = NULL;

	//pWbItem = new CTabItem_WebBrowser;
	//pWbItem->SetCaption(GetResString(IDS_VERYCD));
	//pWbItem->SetRelativeWnd(theApp.emuledlg->webbrowser->GetSafeHwnd());
	//pWbItem->SetWbWnd(theApp.emuledlg->webbrowser);
	//pWbItem->EnableClose(FALSE);
	//pWbItem->SetCustomData(TCD_WEB_BROWSER);

	//m_tabWnd.AddTab(pWbItem);
	//pWbItem = NULL;
}
// CDlgMainTabResource 消息处理程序

BOOL CDlgMainTabResource::OnInitDialog()
{
	CResizableDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ModifyStyle(0, WS_CLIPCHILDREN, 0);

	CRect	rect;
	GetDlgItem(IDC_TAB1)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_tabWnd.Create(WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN, rect, this, IDC_RESOURCE_TAB_WND);
	//m_tabWnd.SetBkColor(GetSysColor(COLOR_3DFACE), FALSE);
	//m_tabWnd.SetJointColor(RGB(121, 138, 169), RGB(255, 255, 255));
	CPageTabBkDraw	*pBarBkDraw = new CPageTabBkDraw;
	m_tabWnd.SetBarBkDraw(pBarBkDraw);


	//	add WebBrowser Toolbar	<begin>

	m_browserToolbar.Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_CUSTOM_BAR);
	m_browserToolbar.Init();
	m_browserToolbar.SetOwner(theApp.emuledlg->webbrowser);
	m_browserToolbar.SetIndent(8);

	theApp.m_BrowserToolbarInfo.SetBrowserToolbarCtrl(&m_browserToolbar);


	//CSize	size;
	//m_browserToolbar.GetMaxSize(&size);
	
	CTabItem_Wnd	*pTabItemWnd = new CTabItem_Wnd;
	pTabItemWnd->SetItemWnd(&m_browserToolbar, FALSE);
//	pTabItemWnd->SetWindowLength(size.cx + 8);
	pTabItemWnd->SetDynDesireLength(TRUE);
	m_tabWnd.AddTab(pTabItemWnd);
	pTabItemWnd = NULL;

	//	add WebBrowser Toolbar	<end>


	ShowVerycdPage();




	AddAnchor(m_tabWnd,TOP_LEFT,BOTTOM_RIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CDlgMainTabResource::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		// Don't handle Ctrl+Tab in this window. It will be handled by main window.
		if (pMsg->wParam == VK_TAB && GetAsyncKeyState(VK_CONTROL) < 0)
			return FALSE;

		if (VK_RETURN == pMsg->wParam || VK_ESCAPE == pMsg->wParam)
			return FALSE;
	}

	return CResizableDialog::PreTranslateMessage(pMsg);
}

void CDlgMainTabResource::OnNMActiveTabChanged(NMHDR* pNMHDR, LRESULT *pResult)
{
	NMTW_TABOP *pTabOp = reinterpret_cast<NMTW_TABOP*>(pNMHDR);

	//pTabOp->posOld;
	//pTabOp->posTab;
	DWORD CustomData = m_tabWnd.GetTabData(pTabOp->posTab);

	DWORD LowData  = CustomData & 0x0000FFFF;
	DWORD HighData = CustomData & 0xFFFF0000;

	if (HighData ^ 0x0F0FFFFF)
	{
		SSearchParams *pSearchParams=NULL;
		BOOL bSearch = m_SearchMap.Lookup(LowData, pSearchParams);

		if ( bSearch && pSearchParams )
		{
			theApp.emuledlg->searchwnd->m_pwndResults->searchlistctrl.ShowResults(pSearchParams->dwSearchID);
			theApp.emuledlg->searchwnd->m_pwndResults->UpdateParamDisplay(pSearchParams);
			theApp.emuledlg->searchwnd->m_pwndResults->m_iCurSearchIndexInRes = LowData;
		}
	}

	
	if (TCD_WEB_BROWSER == CustomData)
	{
		//m_browserToolbar.EnableAllButtons(TRUE);

		CTabItem_WebBrowser *pItem = (CTabItem_WebBrowser*) m_tabWnd.GetTabItem(pTabOp->posTab);
		if (NULL != pItem)
			m_browserToolbar.SetOwner(pItem->GetAssocWbw());
		BOOL bForwordEnable = pItem->GetAssocWbw()->m_pExplorer->bForwordEnable;
		BOOL bBackEnable = pItem->GetAssocWbw()->m_pExplorer->bBackEnable;
		theApp.m_BrowserToolbarInfo.GetBrowserToolbarCtrl()->EnableButton(TB_FORWARD,bForwordEnable);
		theApp.m_BrowserToolbarInfo.GetBrowserToolbarCtrl()->EnableButton(TB_BACK, bBackEnable); 
		theApp.m_BrowserToolbarInfo.GetBrowserToolbarCtrl()->EnableButton(TB_REFRESH, TRUE);
        theApp.m_BrowserToolbarInfo.GetBrowserToolbarCtrl()->EnableButton(TB_HOME, TRUE);
	}
	else
		m_browserToolbar.EnableAllButtons(FALSE);

	*pResult = 0;
}

void CDlgMainTabResource::OnNMTabDestroy(NMHDR* pNMHDR, LRESULT *pResult)
{
	theApp.emuledlg->searchwnd->m_pwndResults->SetPos(0);
	NMTW_TABOP *pTabOp = reinterpret_cast<NMTW_TABOP*>(pNMHDR);

	//pTabOp->posTab;

	DWORD CustomData = m_tabWnd.GetTabData(pTabOp->posTab);

	DWORD LowData  = CustomData & 0x0000FFFF;
	DWORD HighData = CustomData & 0xFFFF0000;

	if (HighData ^ 0x0F0FFFFF)
	{
		SSearchParams *pSearchParams=NULL;
		BOOL bSearch = m_SearchMap.Lookup(LowData, pSearchParams);

		if ( bSearch && pSearchParams)
		{
			theApp.emuledlg->searchwnd->m_pwndResults->searchlistctrl.RemoveResults(pSearchParams->dwSearchID);
			m_SearchMap.RemoveKey(LowData);
		}

		delete pSearchParams;
	}

	if (TCD_WEB_BROWSER == CustomData)
		PostMessage(UM_RESTAB_WB_DESTROY);

	*pResult = 0;
}

void CDlgMainTabResource::OnNMTabCreate(NMHDR* pNMHDR, LRESULT *pResult)
{
	NMTW_TABOP *pTabOp = reinterpret_cast<NMTW_TABOP*>(pNMHDR);

	DWORD dwCustomData = m_tabWnd.GetTabData(pTabOp->posTab);
	if (TCD_WEB_BROWSER == dwCustomData)
		UpdateWbsClosableStatus();

	*pResult = 0;
}

void CDlgMainTabResource::UpdateWbsClosableStatus()
{
	int		iWbCount = 0;
	CTabItem_WebBrowser *pWbItem = NULL;

	POSITION	pos =m_tabWnd.GetFirstTab();
	CTabItem	*pItem;
	while (NULL != pos)
	{
		pItem = m_tabWnd.GetNextTab(pos);
		if (NULL != pItem)
		{
			if (TCD_WEB_BROWSER == pItem->m_dwCustomData)
			{
				pWbItem = (CTabItem_WebBrowser*)pItem;
				pWbItem->EnableClose(TRUE);
				iWbCount++;
			}
		}
	}

	if (1 == iWbCount && NULL != pWbItem)
	{
		pWbItem->EnableClose(FALSE);
	}
}

LRESULT CDlgMainTabResource::OnWbTabDestroy(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	UpdateWbsClosableStatus();
	return 0;
}

void CDlgMainTabResource::UpdateSearchParam(int iIndex, SSearchParams *pSearchParams)
{
	SSearchParams	*psp;
	if (m_SearchMap.Lookup(iIndex, psp))
	{
		delete psp;
		m_SearchMap.SetAt(iIndex, pSearchParams);
	}
}
