#pragma once

class CIEFilterMgr
{
public:
	CIEFilterMgr(void);
	~CIEFilterMgr(void);

public:
	void	StartFilter();
	void	EndFilter();
	IInternetSession	*m_pSession;
	IClassFactory		*m_pFactory;

};
