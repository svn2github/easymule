#include "StdAfx.h"
#include ".\publisher.h"

CPublisher::CPublisher(void)
{
}

CPublisher::~CPublisher(void)
{
}

void CPublisher::RegisterWnd(HWND hWnd)
{
	m_lstReceivers.AddTail(hWnd);
}
void CPublisher::UnRegisterWnd(HWND hWnd)
{
	m_lstReceivers.RemoveAt(m_lstReceivers.Find(hWnd));
}

void CPublisher::SendMsgToAllReceivers(UINT uMsg, WPARAM wParam, LPARAM lParam, HWND hExcept)
{
	HWND		hWnd;
	POSITION	pos = m_lstReceivers.GetHeadPosition();
	while (NULL != pos)
	{
		hWnd = m_lstReceivers.GetNext(pos);

		if (hExcept != hWnd)
			::SendMessage(hWnd, uMsg, wParam, lParam);
	}
}
