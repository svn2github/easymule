#include "StdAfx.h"
#include ".\localizemgr.h"

CLocalizeMgr theLocalizeMgr;

CLocalizeMgr::CLocalizeMgr(void)
{
}

CLocalizeMgr::~CLocalizeMgr(void)
{
}

void CLocalizeMgr::Register(CLocalizee *pLocalizee)
{
	m_lstLocalizees.AddHead(pLocalizee);
}
void CLocalizeMgr::UnRegister(CLocalizee *pLocalizee)
{
	m_lstLocalizees.RemoveAt(m_lstLocalizees.Find(pLocalizee));
}

void CLocalizeMgr::LocalizeAll(void)
{
	CLocalizee *pLocalizee;
	POSITION	pos = m_lstLocalizees.GetHeadPosition();
	while (NULL != pos)
	{
		pLocalizee = m_lstLocalizees.GetNext(pos);

		if ( NULL != pLocalizee && ! ::IsBadWritePtr(pLocalizee, sizeof(CLocalizee)) )
		{
			if (pLocalizee->CanLocalize())
				pLocalizee->Localize();
		}
	}
}