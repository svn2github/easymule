#pragma once


// CDlgMainTabResource 对话框
#include "resource.h"
//#include "ResourceTabWnd.h"
#include "TabWnd.h"
#include "ResizableLib\ResizableDialog.h"
#include "SearchParams.h"
#include "BrowserToolbarCtrl.h"

class CDlgMainTabResource : public CResizableDialog
{
	DECLARE_DYNAMIC(CDlgMainTabResource)

public:
	CDlgMainTabResource(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMainTabResource();

// 对话框数据
	enum { IDD = IDD_MAINTAB_RESOURCE };

	void	OpenNewUrl(LPCTSTR lpszUrl, LPCTSTR lpszCaption, BOOL bSetActive = TRUE, BOOL bClosable = TRUE);
	void    OpenNewSearchResult(LPCTSTR lpszCaption);
	CTabWnd			m_tabWnd;

	void	UpdateSearchParam(int iIndex, SSearchParams *pSearchParams);
protected:
	enum _TabWndCustomData {TCD_WEB_BROWSER = 1};
	//CResourceTabWnd		m_tabWnd;

	CBrowserToolbarCtrl		m_browserToolbar;

	DWORD           m_dwCounter;
	CMap <int, int, SSearchParams*, SSearchParams*> m_SearchMap;

	void	ShowVerycdPage();
	void	UpdateWbsClosableStatus();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNMActiveTabChanged(NMHDR* pNMHDR, LRESULT *pResult);
	afx_msg void OnNMTabDestroy(NMHDR* pNMHDR, LRESULT *pResult);
	afx_msg void OnNMTabCreate(NMHDR* pNMHDR, LRESULT *pResult);
	LRESULT OnWbTabDestroy(WPARAM wParam, LPARAM lParam);
	
};
