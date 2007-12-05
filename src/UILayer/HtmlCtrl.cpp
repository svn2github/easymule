// HtmlCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "emule.h"
#include "HtmlCtrl.h"
#include "WebbrowserWnd.h"
#include "UserMsgs.h"
#include "FindWnd.h"
#include ".\htmlctrl.h"

// CHtmlCtrl

IMPLEMENT_DYNCREATE(CHtmlCtrl, CHtmlView)


void CHtmlCtrl::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHtmlCtrl, CHtmlView)
	ON_WM_DESTROY()
//	ON_WM_ERASEBKGND()
	ON_WM_MOUSEACTIVATE()
	ON_WM_SIZE()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CHtmlCtrl 诊断

#ifdef _DEBUG
void CHtmlCtrl::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CHtmlCtrl::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG


// CHtmlCtrl 消息处理程序
BOOL CHtmlCtrl::CreateFromStatic(UINT nID, CWnd* pParent)
{
	CStatic wndStatic;
	if (!wndStatic.SubclassDlgItem(nID, pParent))
		return FALSE;

	// Get static control rect, convert to parent's client coords.
	CRect rc;
	wndStatic.GetWindowRect(&rc);
	pParent->ScreenToClient(&rc);
	wndStatic.DestroyWindow();

	// create HTML control (CHtmlView)
	return Create(NULL,						 // class name
		NULL,										 // title
		(WS_CHILD | WS_VISIBLE ),			 // style
		rc,										 // rectangle
		pParent,									 // parent
		nID,										 // control ID
		NULL);									 // frame/doc context not used
}

//开始链接时会触发此事件
void CHtmlCtrl::OnBeforeNavigate2(LPCTSTR lpszURL, DWORD /*nFlags*/, LPCTSTR lpszTargetFrameName, CByteArray& /*baPostedData*/, LPCTSTR /*lpszHeaders*/, BOOL* pbCancel)
{
	//MODIFIED by fengwen on 2007/02/13	<begin> :	其父窗口不一定是CWebBrowserWnd
	//((CWebBrowserWnd*)GetParent())->SetAddress(lpszURL);
	//((CWebBrowserWnd*)GetParent())->StartAnimation();
	
	if (NULL == lpszTargetFrameName || _T('\0') == lpszTargetFrameName[0])
	{
		GetParent()->SendMessage(UM_HC_BEFORE_NAVI, 0, (LPARAM) lpszURL);
	}
	//MODIFIED by fengwen on 2007/02/13	<end> :	其父窗口不一定是CWebBrowserWnd
	pbCancel = FALSE;
}

void CHtmlCtrl::OnDestroy()
{
	// This is probably unnecessary since ~CHtmlView does it, but
	// safer to mimic CHtmlView::OnDestroy.
	if (m_pBrowserApp) 
	{
		m_pBrowserApp = NULL;
	}
	CWnd::OnDestroy(); // bypass CView doc/frame stuff
}

void CHtmlCtrl::OnDocumentComplete(LPCTSTR /*lpszURL*/)
{
	//MODIFIED by fengwen on 2007/02/13	<begin> :	其父窗口不一定是CWebBrowserWnd
	//CWebBrowserWnd* pBrowser = ((CWebBrowserWnd*)GetParent());
	//pBrowser->StopAnimation();
	if(theApp.m_BrowserToolbarInfo.Enable())
	{
		theApp.m_BrowserToolbarInfo.GetBrowserToolbarCtrl()->EnableButton(TB_STOP, FALSE);
	}
	
	GetParent()->SendMessage(UM_HC_DOC_CMPL, 0, 0);
	//MODIFIED by fengwen on 2007/02/13	<end> :	其父窗口不一定是CWebBrowserWnd
}

int CHtmlCtrl::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	return CHtmlView::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CHtmlCtrl::OnNavigateComplete2(LPCTSTR /*strURL*/)
{
	//MODIFIED by fengwen on 2007/02/13	<begin> :	其父窗口不一定是CWebBrowserWnd
	//CWebBrowserWnd* pBrowser = ((CWebBrowserWnd*)GetParent());
	//pBrowser->SetAddress(GetLocationURL());
	if(theApp.m_BrowserToolbarInfo.Enable())
	{
		theApp.m_BrowserToolbarInfo.GetBrowserToolbarCtrl()->EnableButton(TB_STOP, TRUE);
	}
	
	GetParent()->SendMessage(UM_HC_NAVI_CMPL, 0, (LPARAM) (LPCTSTR) GetLocationURL());
	//MODIFIED by fengwen on 2007/02/13	<end> :	其父窗口不一定是CWebBrowserWnd

	HWND hwnd = GetBrowserHwnd();

	if(!m_IEManagerWnd.m_hWnd)
	{
		m_IEManagerWnd.SubclassWindow(hwnd);
	}
}

void CHtmlCtrl::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);			//Changed by thilon on 2006.08.10

	if (::IsWindow(m_wndBrowser.m_hWnd)) 
	{ 
		CRect rect; 
		GetClientRect(rect); 
		// 就这一句与CHtmlView的不同
		::AdjustWindowRectEx(rect, GetStyle(), FALSE, WS_EX_CLIENTEDGE);
		m_wndBrowser.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	} 
	if (NULL != m_sttcProgress.GetSafeHwnd())
	{
		m_sttcProgress.UpdatePos();
	}
}

void CHtmlCtrl::OnStatusTextChange(LPCTSTR lpszText)
{
	//MODIFIED by fengwen on 2007/02/13	<begin> :	其父窗口不一定是CWebBrowserWnd
	//((CWebBrowserWnd*)GetParent())->m_status.SetWindowText(lpszText);
	GetParent()->SendMessage(UM_HC_STATUS_TXT_CHANGE, 0, (LPARAM) lpszText);
	//MODIFIED by fengwen on 2007/02/13	<end> :	其父窗口不一定是CWebBrowserWnd
}

void CHtmlCtrl::PostNcDestroy()
{
	CHtmlView::PostNcDestroy();
}

void CHtmlCtrl::OnTitleChange(LPCTSTR lpszText)
{
	// TODO: 在此添加专用代码和/或调用基类
	GetParent()->SendMessage(UM_HC_TITLE_CHANGE, 0, (LPARAM) lpszText);

	CHtmlView::OnTitleChange(lpszText);
}

void CHtmlCtrl::OnProgressChange(long nProgress, long nProgressMax)
{
	m_sttcProgress.UpdateProgress(nProgress, nProgressMax);

	CHtmlView::OnProgressChange(nProgress, nProgressMax);
}

int CHtmlCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CHtmlView::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_sttcProgress.CreatePS(this);
    bForwordEnable = false;
	bBackEnable = false;
	return 0;
}


void CHtmlCtrl::OnCommandStateChange(long nCommand, BOOL bEnable)
{

	if(theApp.m_BrowserToolbarInfo.Enable())
	{
		switch(nCommand)
		{
		case CSC_NAVIGATEFORWARD:
			theApp.m_BrowserToolbarInfo.GetBrowserToolbarCtrl()->EnableButton(TB_FORWARD, bEnable);
			bForwordEnable = bEnable;
			break;
		case CSC_NAVIGATEBACK:
			theApp.m_BrowserToolbarInfo.GetBrowserToolbarCtrl()->EnableButton(TB_BACK, bEnable);
			bBackEnable = bEnable;
			break;
		default:
			CHtmlView::OnCommandStateChange(nCommand, bEnable);
		}
	}
}
HWND CHtmlCtrl::GetBrowserHwnd(void)
{
	CFindWnd ies(this->GetSafeHwnd(), L"Internet Explorer_Server");
	return ies.m_hWnd;
}
