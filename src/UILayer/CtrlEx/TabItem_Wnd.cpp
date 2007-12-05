#include "StdAfx.h"
#include ".\tabitem_wnd.h"
#include "Util.h"
#include "UserMsgs.h"
CTabItem_Wnd::CTabItem_Wnd(void)
{
	m_pWnd			= NULL;
	m_bAutoDelete	= FALSE;
	m_iWndLength	= 100;
}

CTabItem_Wnd::~CTabItem_Wnd(void)
{
	if (m_bAutoDelete)
		SAFE_DELETE(m_pWnd);
}

IMPLEMENT_DYNCREATE(CTabItem_Wnd, CTabItem)

void CTabItem_Wnd::SetItemWnd(CWnd *pWnd, BOOL bAutoDelete)
{
	SetAttribute(ATTR_FIXLEN);

	if (m_bAutoDelete && NULL != m_pWnd)
	{
		SAFE_DELETE(m_pWnd);
		m_bAutoDelete = FALSE;
	}

	m_pWnd = pWnd;
	m_bAutoDelete = bAutoDelete;
}

int	CTabItem_Wnd::GetDesireLength(void)
{
	if (IsDynDesireLength())
	{
		if (NULL == m_pWnd)
			return 0;
		else
			return m_pWnd->SendMessage(UM_GET_DESIRE_LENGTH);
	}
	else
		return m_iWndLength;

}

void CTabItem_Wnd::Create(CTabBar *pParentBar, POSITION myPos)
{
	CTabItem::Create(pParentBar, myPos);
	m_pWnd->SetParent((CWnd*)pParentBar);
}

void CTabItem_Wnd::Paint(CDC* /*pDC*/)
{
	if (NULL != m_pWnd)
		m_pWnd->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
}

void CTabItem_Wnd::OnSize()
{
	CRect	rect = GetRect();
	if (NULL != m_pWnd)
		m_pWnd->MoveWindow(&rect, FALSE);
}
