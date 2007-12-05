#pragma once
#include "TabItem_NormalCloseable.h"
#include "WBNotifyReceiver.h"
#include "WebBrowserWnd.h"

class CTabItem_WebBrowser : public CTabItem_NormalCloseable, public CWBNotifyReceiver
{
public:
	CTabItem_WebBrowser(void);
	~CTabItem_WebBrowser(void);

	virtual int GetDesireLength();
	void	SetWbWnd(CWebBrowserWnd *pWbw);

	CWebBrowserWnd*	GetAssocWbw(){return m_pWbw;}
protected:

	//void	UpdateCaption();
	CWebBrowserWnd	*m_pWbw;
	//int				m_iProgressPercent;
	//CString			m_strTitle;

	//enum {ICON_COUNT = 5};
	//HICON			m_arrProgressIcons[ICON_COUNT];
};
