#include "StdAfx.h"
#include ".\wndmgr.h"

CWndMgr	theWndMgr;

CWndMgr::CWndMgr(void)
{
	int i;
	for (i = 0; i < WI_MAX; i++)
		m_ahWnds[i] = NULL;

	m_pTbcShare = NULL;
}

CWndMgr::~CWndMgr(void)
{
}

