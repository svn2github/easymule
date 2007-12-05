#pragma once


#include <WinSock2.h>
#include "UPnpError.h"

class CUPnpNat
{
public:
	CUPnpNat(void);
	~CUPnpNat(void);

	enum {SEARCH_TIMEOUT_MS = 15000,
			DEF_ACTION_TIMEOUT_MS = 9000};

	void SetBindAddress(LPCTSTR lpszBindAddress){m_strBindAddress = lpszBindAddress;}
	void SetActionTimeout(int iActionTimeoutMs = DEF_ACTION_TIMEOUT_MS){m_iActionTimeoutMs = iActionTimeoutMs;}
	int	 GetActionTimeout(){return m_iActionTimeoutMs;}


	HRESULT	SearchDevice(BOOL bUseDefaultGateway = TRUE);


	HRESULT	AddPortMapping(LPCTSTR lpszRemoteHost,
							USHORT usExternalPort, LPCTSTR lpszPortMappingProtocol,
							USHORT usInternalPort, LPCTSTR lpszInternalClient,
							LPCTSTR lpszPortMappingDescription = NULL,
							BOOL bPortMappingEnabled = TRUE,
							ULONG ulPortMappingLeaseDuration = 0);

	HRESULT	DeletePortMapping(LPCTSTR lpszRemoteHost,
							USHORT usExternalPort,
							LPCTSTR lpszPortMappingProtocol);


	HRESULT	GetSpecificPortMappingEntry(LPCTSTR lpszRemoteHost,					//[in]
										USHORT usExternalPort,					//[in]
										LPCTSTR lpszPortMappingProtocol,		//[in]
										USHORT *pusInternalPort,				//[out]
										CString *pstrInternalClient,			//[out]
										bool *pbEnable,							//[out]
										CString *pstrDescription,				//[out]
										ULONG *pulPortMappingLeaseDuration);	//[out]

	
	HRESULT	GetGenericPortMappingEntry(USHORT usIndex,					//[in]
										CString *pstrRemoteHost,		//[out]
										USHORT *pusExternalPort,		//[out]
										CString *pstrProtocol,			//[out]
										USHORT *pusInternalPort,		//[out]
										CString *pstrInternalClient,	//[out]
										bool *pbEnable,					//[out]
										CString *pstrDescription);		//[out]

	DWORD	GetLastActionErrorCode(){return m_dwLastErrorCode;}

protected:
	void	SetLastActionErrorCode(DWORD dwLastErrorCode){m_dwLastErrorCode = dwLastErrorCode;}

	HRESULT SOAP_action(CString addr, UINT16 port, const CString request, CString &response);
	int SSDP_sendRequest(SOCKET s, UINT32 ip, UINT16 port, const CString& request);

	bool		InternalSearch(int version, BOOL bUseDefaultGateway = FALSE);
	bool		GetDescription();
	CString		GetProperty(const CString& name, CString& response);
	bool		isComplete() const { return !m_controlurl.IsEmpty(); }
	bool		Valid()const{return (/*!m_name.IsEmpty()&&*/!m_description.IsEmpty());}

	HRESULT		InvokeCommand(const CString& name, const CString& args, CString &strResponse);

	static BOOL IsLengthedHttpPacketComplete(const char *packet, int len);
protected:
	CString		m_strBindAddress;
	int			m_iActionTimeoutMs;

	DWORD		m_dwLastErrorCode;

	BOOL		m_isSearched;

	int			m_version;
	CString		m_devicename;
	CString		m_name;
	CString		m_description;
	CString		m_baseurl;
	CString		m_controlurl;
	CString		m_friendlyname;
	CString		m_modelname;

};
