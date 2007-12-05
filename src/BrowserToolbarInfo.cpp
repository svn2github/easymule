#include "StdAfx.h"
#include ".\browsertoolbarinfo.h"

CBrowserToolbarInfo::CBrowserToolbarInfo(void)
{
	m_pBrowserToolbarCtrl = NULL;
	m_pHtmlCtrl = NULL;
}

CBrowserToolbarInfo::~CBrowserToolbarInfo(void)
{
	if(m_pBrowserToolbarCtrl)
	{
		m_pBrowserToolbarCtrl = NULL;
	}

	if(m_pBrowserToolbarCtrl)
	{
		m_pHtmlCtrl = NULL;
	}
}

void CBrowserToolbarInfo::SetBrowserToolbarCtrl(CBrowserToolbarCtrl* pBrowserToolbarCtrl)
{
	m_pBrowserToolbarCtrl = pBrowserToolbarCtrl;
}

CBrowserToolbarCtrl* CBrowserToolbarInfo::GetBrowserToolbarCtrl(void)
{
	return m_pBrowserToolbarCtrl;
}

void CBrowserToolbarInfo::SetHtmlCtrl(CHtmlCtrl* pHtmlCtrl)
{
	m_pHtmlCtrl = pHtmlCtrl;
}

CHtmlCtrl* CBrowserToolbarInfo::GetHtmlCtrl(void)
{
	return m_pHtmlCtrl;
}

BOOL CBrowserToolbarInfo::Enable(void)
{
	if(m_pHtmlCtrl && m_pBrowserToolbarCtrl)
	{
		return TRUE;
	}

	return FALSE;
}