#pragma once
#include <afxwin.h>

class CBetterSP2
{
public:
	CBetterSP2(void);
	~CBetterSP2(void);

public:
	  int		DetectSystemInformation(void);
	  int		ChangeTCPIPValue(DWORD dwValue);
	  int		ChangeTCPIPValue_Vista(DWORD dwValue);
	   
protected:
	TCHAR		m_szFileName[MAX_PATH + 128];
	DWORD		m_dwPosition;
	DWORD		m_dwVersion[4];		//tcpip.sys°æ±¾ºÅ

protected:

	  int		GetFileVersion(LPCTSTR pFile);
	DWORD		GetCurrentLimit(LPCTSTR pFile);
	DWORD		GetCurrentLimit_Vista(LPCTSTR pFile);
	 void		SetFileName(void);
	 int		IsLimitedVersion();
};