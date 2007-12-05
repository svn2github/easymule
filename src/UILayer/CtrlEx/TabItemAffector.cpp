// TabItemAffector.cpp : 实现文件
//

#include "stdafx.h"
#include "TabItemAffector.h"
#include "TabItem.h"

// CTabItemAffector
CTabItemAffector::CTabItemAffector()
{
	m_pAssocItem = NULL;
}

CTabItemAffector::~CTabItemAffector()
{
	if (NULL != m_pAssocItem)
		m_pAssocItem->SetAffector(NULL);
}

void CTabItemAffector::SetAssocItem(CTabItem *pAssocItem)
{
	if (NULL != m_pAssocItem)
		m_pAssocItem->m_pAffector = NULL;

	m_pAssocItem = pAssocItem;
}

void CTabItemAffector::SetItemCaption(LPCTSTR lpszCaption)
{
	if (NULL == m_pAssocItem)
		return;

	m_pAssocItem->m_strCaption = lpszCaption;
	m_pAssocItem->Invalidate();
}

// CTabItemAffector 成员函数
