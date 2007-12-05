#pragma once

#include <afxtempl.h>

class CPublisher
{
public:
	CPublisher(void);
	~CPublisher(void);

	void	RegisterWnd(HWND hWnd);
	void	UnRegisterWnd(HWND hWnd);

protected:
	void	SendMsgToAllReceivers(UINT uMsg, WPARAM wParam, LPARAM lParam, HWND hExcept = NULL);

	CList<HWND>		m_lstReceivers;
};
