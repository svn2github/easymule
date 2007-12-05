#pragma once

#include "ED2KLink.h"

class CPartFile;
///////////////////////////////////////////////////////////////////////////////////////////////////

struct Hostname_Entry 
{
	uchar		fileid[16];
	CStringA	strHostname;
	uint16		port;
	CString		strURL;
	CString		strRefer;	
	CPartFile*  pPartFile;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

class CDNSResolveWnd : public CWnd
{
public:
	CDNSResolveWnd();
	virtual ~CDNSResolveWnd();

	void AddToResolve(const uchar* fileid, LPCSTR pszHostname, uint16 port, LPCTSTR pszURL = NULL, LPCTSTR lpszRefer = NULL,CPartFile* pPartFile =NULL );	

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnDNSResolved(WPARAM wParam, LPARAM lParam);

private:
	CTypedPtrList<CPtrList, Hostname_Entry*> m_HostEntryList;
	char m_aucHostnameBuffer[MAXGETHOSTSTRUCT];
};

///////////////////////////////////////////////////////////////////////////////////////////////////

class CPartFile;
class CDNSManager
{
public:
	CDNSManager();
	virtual ~CDNSManager();

	void AddUrlToDNS(const CString &strUrl, CPartFile * pPartFile);

	void AddToResolved(CPartFile* pFile, SUnresolvedHostname* pUH, LPCTSTR lpszRefer = NULL);
	void AddToResolved(const uchar* fileid, LPCSTR pszHostname, uint16 port, LPCTSTR pszURL = NULL, LPCTSTR lpszRefer = NULL,CPartFile* pPartFile =NULL );

protected:

	void DisPatch(Hostname_Entry * resolved, CList<uint32> & iplist);

protected:
	CDNSResolveWnd m_DNSWnd;
	friend class CDNSResolveWnd;

	CRBMap<CString, CPartFile*> m_DNSMap; // VC-SearchDream[2007-07-23]: This is for HTTP and FTP Direct DownLoad
};

///////////////////////////////////////////////////////////////////////////////////////////////////
