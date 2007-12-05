#pragma once
#include "IEManagerWnd.h"


// CHtmlCtrl Html 视图

#include "ProgressStatic.h"

class CHtmlCtrl : public CHtmlView
{
	DECLARE_DYNCREATE(CHtmlCtrl)

public:
	CHtmlCtrl(){ };           // 动态创建所使用的受保护的构造函数
	virtual ~CHtmlCtrl()
	{
	}

	BOOL CreateFromStatic(UINT nID, CWnd* pParent);

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	CProgressStatic		m_sttcProgress;
public:
	virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
	afx_msg void OnDestroy();
	virtual void OnDocumentComplete(LPCTSTR lpszURL);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	virtual void OnNavigateComplete2(LPCTSTR strURL);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnStatusTextChange(LPCTSTR lpszText);
protected:
	virtual void PostNcDestroy();
public:
	virtual void OnTitleChange(LPCTSTR lpszText);
	virtual void OnProgressChange(long nProgress, long nProgressMax);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnCommandStateChange(long nCommand, BOOL bEnable);
protected:
	HWND GetBrowserHwnd(void);
	CIEManagerWnd m_IEManagerWnd;
public:
	BOOL bForwordEnable;
	BOOL bBackEnable;
};

