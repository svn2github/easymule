#pragma once

class CWndMgr
{
public:
	CWndMgr(void);
	~CWndMgr(void);

	enum EWndId
	{
		WI_DOWNLOADED_LISTCTRL,
		WI_MAINTAB_DOWNLOAD_DLG,
		WI_ADVANCE_TOOLBAR,
		WI_MAX
	};

	void	SetWndHandle(EWndId eWndId, HWND hWnd){m_ahWnds[eWndId] = hWnd;}
	HWND	GetWndHandle(EWndId eWndId){return m_ahWnds[eWndId];}

	LRESULT	SendMsgTo(EWndId eWndId, UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		return ::SendMessage(GetWndHandle(eWndId), uMsg, wParam, lParam);
	}
	BOOL PostMsgTo(EWndId eWndId, UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		return ::PostMessage(GetWndHandle(eWndId), uMsg, wParam, lParam);
	}

	CToolBarCtrl	*m_pTbcShare;

protected:
	HWND	m_ahWnds[WI_MAX];
};

extern CWndMgr	theWndMgr;