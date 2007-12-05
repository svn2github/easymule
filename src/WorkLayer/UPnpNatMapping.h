#pragma once

class CUPnpNatMapping
{
public:
	CUPnpNatMapping(void);
	~CUPnpNatMapping(void);

	CUPnpNatMapping(const CUPnpNatMapping	&entry);
	CUPnpNatMapping&		operator=(const CUPnpNatMapping &entry);

	void	Empty();
public:
	WORD		m_wInternalPort;				// Port mapping internal port
	WORD		m_wExternalPort;				// Port mapping external port
	CString		m_strProtocol;					// Protocol-> TCP (UPNPNAT_PROTOCOL:UNAT_TCP) || UDP (UPNPNAT_PROTOCOL:UNAT_UDP)
	CString		m_strDescription;				// Port mapping description
	CString		m_strInternalClient;

};
