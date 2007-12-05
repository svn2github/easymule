#include "StdAfx.h"
#include "sourceurl.h"


//////////////////////////////////////////////////////////////////////
// CSourceURL construction

CSourceURL::CSourceURL(LPCTSTR pszURL)
{
	Clear();
}

//////////////////////////////////////////////////////////////////////
// CSourceURL clear

void CSourceURL::Clear()
{
	//m_sURL.Empty();
	m_sAddress.Empty();
	m_sPath.Empty();
	m_sLogin.Empty ();
	m_sPassword.Empty ();
}

//////////////////////////////////////////////////////////////////////
// CSourceURL HTTP

BOOL CSourceURL::ParseHTTP(LPCTSTR pszURL)
{
	m_nPort = 80;

	if ( _tcsncmp( pszURL, _T("http://"), 7 ) != 0 ) return FALSE;

	CString strURL = pszURL + 7;

	int nSlash = strURL.Find( _T('/') );

	if ( nSlash >= 0 )
	{
		m_sAddress	= strURL.Left( nSlash );
		m_sPath		= strURL.Mid( nSlash );
	}
	else
	{
		m_sAddress = strURL;
		m_sPath = _T("/");
	}

	int nAt = m_sAddress.Find( _T('@') );
	if ( nAt >= 0 ) m_sAddress = m_sAddress.Mid( nAt + 1 );

	if ( m_sAddress.IsEmpty() ) return FALSE;

	int nColon = m_sAddress.Find( ':' );

	/*uint16 nPort;*/

	if ( nColon >= 0 )
	{
		CString strPort;

		strPort = m_sAddress.Mid( nColon + 1 );

		m_nPort = (UINT16)_ttoi(strPort);

		//if ( _stscanf( m_sAddress.Mid( nColon + 1 ), _T("%i"), &nPort ) == 1 ) 
		//{
		//	m_nPort = htons( nPort );
		//}

		m_sAddress = m_sAddress.Left( nColon );
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// CSourceURL FTP
extern CString UrlConvert( CString strUrl );
BOOL CSourceURL::ParseFTP(LPCTSTR pszURL)
{
	// URI format
	// ftp://[user[:password]@]host[:port][/path]

	//m_sURL = pszURL;
	//m_sURL = UrlConvert(m_sURL);

	m_nPort = 21; // This is the Default Port Number

	if ( _tcsncmp( pszURL, _T("ftp://"), 6 ) != 0 ) return FALSE;

	CString strURL ( pszURL + 6 );
	strURL = UrlConvert(strURL);

	int nSlash = strURL.Find( _T('/') );

	if ( nSlash >= 0 )
	{
		m_sAddress	= strURL.Left( nSlash );
		m_sPath		= strURL.Mid( nSlash );
	}
	else
	{
		m_sAddress = strURL;
		m_sPath = _T("/");
	}

	int nAt = m_sAddress.Find( _T('@') );
	if ( nAt >= 0 )
	{
		m_sLogin = m_sAddress.Left( nAt );
		m_sAddress = m_sAddress.Mid( nAt + 1 );

		int nColon = m_sLogin.Find( _T(':') );
		if ( nColon >= 0 )
		{
			m_sPassword = m_sLogin.Mid( nColon + 1 );			
			m_sLogin = m_sLogin.Left( nColon );			
		}
	}
	else
	{
		m_sLogin = _T("anonymous");
		m_sPassword = _T("guest@verycd.com");
	}

	if ( m_sAddress.IsEmpty() || m_sLogin.IsEmpty() )
	{
		return FALSE;
	}

	int nColon = m_sAddress.Find( ':' );

	/*uint16 nPort;*/

	if ( nColon >= 0 )
	{
		CString strPort;

		strPort = m_sAddress.Mid( nColon + 1 );

		m_nPort = (UINT16)_ttoi(strPort);

		//if ( _stscanf( m_sAddress.Mid( nColon + 1 ), _T("%i"), &nPort ) == 1 )
		//{
		//	m_nPort = htons( nPort );
		//}

		m_sAddress = m_sAddress.Left( nColon );
	}

	return TRUE;
}

BOOL CSourceURL::Resolve(LPCTSTR pszHost, int nPort, SOCKADDR_IN* pHost, BOOL bNames) const
{
	ZeroMemory( pHost, sizeof(*pHost) );
	pHost->sin_family	= PF_INET;
	pHost->sin_port		= htons( (UINT16)nPort );

	if ( pszHost == NULL || *pszHost == 0 ) return FALSE;

	CString strHost( pszHost );

	int nColon = strHost.Find( ':' );

	if ( nColon >= 0 )
	{
		if ( _stscanf( strHost.Mid( nColon + 1 ), _T("%i"), &nPort ) == 1 )
		{
			pHost->sin_port = htons( (UINT16)nPort );
		}

		strHost = strHost.Left( nColon );
	}

	USES_CONVERSION;
	LPCSTR pszaHost = T2CA( (LPCTSTR)strHost );

	DWORD dwIP = inet_addr( pszaHost );

	if ( dwIP == INADDR_NONE )
	{
		if ( ! bNames ) return TRUE;

		HOSTENT* pLookup = gethostbyname( pszaHost );

		if ( pLookup == NULL ) return FALSE;

		CopyMemory( &pHost->sin_addr, pLookup->h_addr, 4 );
	}
	else
	{
		CopyMemory( &pHost->sin_addr, &dwIP, 4 );
	}

	return TRUE;
}