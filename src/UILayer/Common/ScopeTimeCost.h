#pragma once

class CScopeTimeCost
{
public:
	CScopeTimeCost(LPCTSTR lpszScopeName)
	{
		m_strScopeName = lpszScopeName;
		m_dwStartTick = GetTickCount();
		TRACE(_T("[ScopeTimeCost]%s : START\n"), m_strScopeName);
	}
	~CScopeTimeCost(void)
	{
		TRACE(_T("[ScopeTimeCost]%s : %d\n"), m_strScopeName, GetTickCount() - m_dwStartTick);
	}
protected:
	CString	m_strScopeName;
	DWORD	m_dwStartTick;
};
