// WebBrowser.cpp : 实现文件
//

#include "stdafx.h"
#include "emule.h"
#include "WebBrowserWnd.h"
#include "otherfunctions.h"
#include "Preferences.h"
#include "UserMsgs.h"

#define VERYCD_HOMEPAGE _T("http://www.verycd.com/start/")
//#define VERYCD_HOMEPAGE _T("about:blank")
#define POST_FORM_HEADER _T("Content-Type: application/x-www-form-urlencoded\r\n")

// CWebBrowserWnd 对话框

IMPLEMENT_DYNAMIC(CWebBrowserWnd, CDialog)
CWebBrowserWnd::CWebBrowserWnd(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CWebBrowserWnd::IDD, pParent)
{
	m_pExplorer = NULL;
	//m_pExplorer = new CHtmlCtrl(); // VC-SearchDream[2006-12-25]: Move to OnInitDialog 
	m_strOpenUrl = VERYCD_HOMEPAGE;
}

CWebBrowserWnd::~CWebBrowserWnd()
{
	 if (m_pExplorer)
	 {
		 m_pExplorer = NULL, delete m_pExplorer;
	 }
}

void CWebBrowserWnd::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
//	DDX_Control(pDX, IDC_CUSTOM_BAR, m_toolbar);
//	DDX_Control(pDX, IDC_VERYEARTH, m_animate);
	DDX_Control(pDX, IDC_STATIC_WEBINFO, m_status);
//	DDX_Control(pDX, IDC_ADDRESSBOX, m_addressbox);
}


BEGIN_MESSAGE_MAP(CWebBrowserWnd, CResizableDialog)
	ON_CBN_SELENDOK(IDC_COMBO_ADDRESS,OnNewAddress)
	ON_WM_KEYDOWN()

	ON_COMMAND( TB_BACK, OnBack )
	ON_COMMAND( TB_FORWARD, OnForward )
	ON_COMMAND( TB_STOP, OnStop )
	ON_COMMAND( TB_HOME, OnHomePage )
	ON_COMMAND( TB_REFRESH, OnRefresh )

	ON_MESSAGE(UM_HC_BEFORE_NAVI, OnHcBeforeNavi)
	ON_MESSAGE(UM_HC_NAVI_CMPL, OnHcNaviCmpl)
	ON_MESSAGE(UM_HC_DOC_CMPL, OnDocCmpl)
	ON_MESSAGE(UM_HC_STATUS_TXT_CHANGE, OnStatusTxtChange)
	ON_MESSAGE(UM_HC_TITLE_CHANGE, OnTitleChange)

//	ON_WM_ERASEBKGND()
//ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CWebBrowserWnd 消息处理程序
void CWebBrowserWnd::Localize(void)
{
	if (!::IsWindow(m_hWnd))
		return;

	GetDlgItem(IDC_STATIC_BROWSER)->SetWindowText(GetResString(IDS_WEBBROWSER));  
//	GetDlgItem(IDC_STATIC_ADDRESS)->SetWindowText(GetResString(IDS_ADDRESS));
//	m_toolbar.Localize();

	if (::IsWindow(m_staticError.GetSafeHwnd()))
		m_staticError.SetWindowText(GetResString(m_uStridDisableReason));
}

void CWebBrowserWnd::SetAddress(LPCTSTR /*lpszURL*/)
{
//	m_addressbox.SetWindowText(lpszURL);
}

void CWebBrowserWnd::StartAnimation()
{
//	m_animate.Play(0,(UINT)-1,(UINT)-1);
}

void CWebBrowserWnd::StopAnimation()
{
//	m_animate.Stop();
}

void CWebBrowserWnd::OnNewAddress()
{
	CString str;
	//m_addressbox.GetLBText(m_addressbox.GetCurSel(), str);
	m_pExplorer->Navigate2(str, 0, NULL);
}
void CWebBrowserWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(nChar == '\r')
	{
		OnNewAddressEnter();
	}

	CResizableDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

//Chocobo Start
//浏览器访问指定页面，added by Chocobo on 2006.08.07
//方便帮助链接在内置浏览器中显示
void CWebBrowserWnd::Navigate(LPCTSTR lpszURL)
{
	if (NULL == m_pExplorer)	return;
	m_pExplorer->Navigate2(lpszURL,0,0,0,0,0);
}
//Chocobo End

void CWebBrowserWnd::OnNewAddressEnter()
{

	//CString str;
	//m_addressbox.GetWindowText(str);
	//
	//m_pExplorer->Navigate2(str, 0, NULL);
	//if(!m_addressbox.FindString(0,str))
	//{
 //       m_addressbox.InsertString(0,str);
	//}
}

CString CWebBrowserWnd::ResourceToURL(LPCTSTR lpszURL)
{
	// This functions shows how to convert an URL to point
	// within the application
	// I only use it once to get the startup page

	CString m_strURL;
	HINSTANCE hInstance = AfxGetResourceHandle();
	ASSERT(hInstance != NULL);
	
	LPTSTR lpszModule = new TCHAR[_MAX_PATH];
	
	if (GetModuleFileName(hInstance, lpszModule, _MAX_PATH))
	{
		m_strURL.Format(_T("res://%s/%s"), lpszModule, lpszURL);
	}
	
	delete []lpszModule;

	return m_strURL;
}

void CWebBrowserWnd::OnBack()
{
	m_pExplorer->GoBack();
}

void CWebBrowserWnd::OnForward()
{
	m_pExplorer->GoForward();
}

void CWebBrowserWnd::OnStop()
{
	m_pExplorer->Stop();
}

void CWebBrowserWnd::OnHomePage()
{
	m_pExplorer->Navigate2(VERYCD_HOMEPAGE, NULL,NULL,NULL,NULL);
}

void CWebBrowserWnd::OnRefresh()
{
	StartAnimation();
    m_pExplorer->Refresh();
}

void CWebBrowserWnd::DisableBrowser(UINT uStridReason)
{
	GetDlgItem(IDC_STATIC_BROWSER)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CUSTOM_BAR)->ShowWindow(SW_HIDE);

	m_uStridDisableReason = uStridReason;

	CRect rect;
	GetClientRect(&rect);
	m_staticError.Create(NULL, WS_CHILD|WS_VISIBLE, rect, this, IDC_RUNTIMEERROR);
	m_staticError.SetWindowText(GetResString(m_uStridDisableReason));
	m_staticError.SetFont(&(theApp.m_fontLog));
}

BOOL CWebBrowserWnd::OnInitDialog()
{
	CResizableDialog::OnInitDialog();

	// VC-SearchDream[2006-12-25]: Move the following code from the tail of m_animate.Open(IDR_VERYEARTH)
	// Add Browser
	CRect toolRect,frameRect,addressRect,staticRect, statusRect, loaderRect;
	GetDlgItem(IDC_BROWSERFRAME)->GetWindowRect(&frameRect);
	GetDlgItem(IDC_BROWSERFRAME)->DestroyWindow();	
	::MapWindowPoints(NULL, m_hWnd, (LPPOINT)&frameRect, 2);

	//ADDED by fengwen on 2006/12/29	<begin> :
	if (! thePrefs.m_bShowBroswer)
	{
		DisableBrowser(IDS_WEBBROWSER_DISABLED);
		return TRUE;
	}

	//ADDED by fengwen on 2006/12/29	<end> :
	// VC-SearchDream[2006-12-25]: Add try-catch
	try
	{
		m_pExplorer = new CHtmlCtrl(); // VC-SearchDream[2006-12-25]: Move from CWebBrowserWnd
		m_pExplorer->Create(NULL, NULL ,WS_CHILD | WS_VISIBLE &~WS_BORDER,frameRect,this, IDC_BROWSERFRAME,NULL);
		m_pExplorer->SetSilent(true);
	}
	catch(...)
	{
		if (m_pExplorer)
		{
			delete m_pExplorer;
			m_pExplorer = NULL;
		}

		//CString strError;
		//strError.Format(GetResString(IDS_CREATE_WEBBROWSER_FAIL), VERYCD_HOMEPAGE);
		DisableBrowser(IDS_CREATE_WEBBROWSER_FAIL);
		//ShellExecute(this->m_hWnd, _T("open"), CString(VERYCD_HOMEPAGE), _T(""), _T(""), SW_SHOW);

		return TRUE; // Directly Return
	}
	// VC-SearchDream[2006-12-25]: Move the up code from the tail of m_animate.Open(IDR_VERYEARTH)
	theApp.m_BrowserToolbarInfo.SetHtmlCtrl(m_pExplorer);

	// Add Toolbar
//	m_toolbar.Init();

	// Add Title
	GetDlgItem(IDC_STATIC_BROWSER)->SetWindowText(GetResString(IDS_WEBBROWSER));  
//	GetDlgItem(IDC_STATIC_ADDRESS)->SetWindowText(GetResString(IDS_ADDRESS));

	m_browsericon = theApp.LoadIcon(_T("WEBBROWSER"));

	// Add VeryCD Earth
//	m_animate.Open(IDR_VERYEARTH);

	//工具条位置
	//CSize size;
	//m_toolbar.GetMaxSize(&size);
	//m_toolbar.GetWindowRect(&toolRect);
	//ScreenToClient(&toolRect);
	//m_toolbar.MoveWindow(toolRect.left,toolRect.top,size.cx,toolRect.Height());

	// Set Static_Address Postion
//	m_toolbar.GetWindowRect(&toolRect);
	//GetDlgItem(IDC_STATIC_ADDRESS)->GetWindowRect(&staticRect);
	//staticRect.left = toolRect.right;
	//staticRect.right = staticRect.left + 50 ;
	//ScreenToClient(&staticRect);
	//GetDlgItem(IDC_STATIC_ADDRESS)->MoveWindow(&staticRect);

	// Set Address Box Position
	//m_addressbox.GetWindowRect(&addressRect);
	//addressRect.left = toolRect.right + 55;
	//ScreenToClient(&addressRect);
	//m_addressbox.MoveWindow(&addressRect);
//	m_addressbox.SetItemHeight(-1,160);

	// Set Window Style
	UINT uStyle = ::GetWindowLong(m_pExplorer->m_hWnd,GWL_EXSTYLE);
	uStyle |= ~WS_EX_STATICEDGE;
	::SetWindowLong(m_pExplorer->m_hWnd,GWL_EXSTYLE,uStyle);

	CRect	rtClient;
	GetClientRect(rtClient);
	m_pExplorer->MoveWindow(&rtClient, FALSE);

//	AddAnchor(GetDlgItem(IDC_STATIC_ADDRESS)->m_hWnd,TOP_LEFT,NOANCHOR);
//	AddAnchor(m_addressbox.m_hWnd,TOP_LEFT,TOP_RIGHT);
	AddAnchor(m_pExplorer->m_hWnd,TOP_LEFT,BOTTOM_RIGHT);
//	AddAnchor(GetDlgItem(IDC_VERYEARTH)->m_hWnd,TOP_RIGHT,NOANCHOR);
	AddAnchor(m_status.m_hWnd,BOTTOM_LEFT, BOTTOM_RIGHT);

	// Navigate Homepage
	m_pExplorer->SetSilent(true);
	m_pExplorer->Navigate2(m_strOpenUrl, NULL,NULL,NULL,NULL);

	return TRUE;
}

LRESULT CWebBrowserWnd::OnHcBeforeNavi(WPARAM /*wParam*/, LPARAM lParam)
{
	SetAddress((LPCTSTR) lParam);
	StartAnimation();
	return 0;
}
LRESULT CWebBrowserWnd::OnHcNaviCmpl(WPARAM /*wParam*/, LPARAM lParam)
{
	SetAddress((LPCTSTR) lParam);
	return 0;
}
LRESULT CWebBrowserWnd::OnDocCmpl(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	StopAnimation();
	return 0;
}	

LRESULT CWebBrowserWnd::OnStatusTxtChange(WPARAM /*wParam*/, LPARAM lParam)
{
	m_status.SetWindowText((LPCTSTR) lParam);
	return 0;
}

LRESULT CWebBrowserWnd::OnTitleChange(WPARAM /*wParam*/, LPARAM lParam)
{
	SetItemCaption((LPCTSTR) lParam);
	return 0;
}

//防止按下回车键，浏览器窗口消失
void CWebBrowserWnd::OnOK()
{
	return;
}

void CWebBrowserWnd::OnCancel()
{
	return;
}
