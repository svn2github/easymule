#pragma once

#include "Localizee.h"

class CLocalizeMgr
{
public:
	CLocalizeMgr(void);
	~CLocalizeMgr(void);

	void	LocalizeAll(void);

	void	Register(CLocalizee *pLocalizee);
	void	UnRegister(CLocalizee *pLocalizee);

protected:
	CList<CLocalizee*>		m_lstLocalizees;

};

extern CLocalizeMgr theLocalizeMgr;
