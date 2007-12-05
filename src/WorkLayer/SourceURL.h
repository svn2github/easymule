#pragma once

class CSourceURL  
{
	// Construction
public:
	CSourceURL(LPCTSTR pszURL = NULL);

	// Attributes
public:
	//CString		m_sURL;
	CString		m_sAddress;
	CString		m_sPath;
	CString		m_sLogin;
	CString		m_sPassword;
	uint16      m_nPort;

	// Operations
public:
	void	Clear();
	BOOL	ParseFTP(LPCTSTR pszURL);
	BOOL	ParseHTTP(LPCTSTR pszURL);
	BOOL	Resolve(LPCTSTR pszHost, int nPort, SOCKADDR_IN* pHost, BOOL bNames = TRUE) const;
};