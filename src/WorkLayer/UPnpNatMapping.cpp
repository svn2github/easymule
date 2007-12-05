#include "StdAfx.h"
#include ".\upnpnatmapping.h"

CUPnpNatMapping::CUPnpNatMapping(void)
{
	Empty();
}

CUPnpNatMapping::~CUPnpNatMapping(void)
{
}

CUPnpNatMapping::CUPnpNatMapping(const CUPnpNatMapping	&entry)
{
	*this = entry;
}

CUPnpNatMapping& CUPnpNatMapping::operator=(const CUPnpNatMapping &entry)
{
	if (this == &entry)
		return *this;

	m_wInternalPort		= entry.m_wInternalPort;
	m_wExternalPort		= entry.m_wExternalPort;
	m_strProtocol		= entry.m_strProtocol;
	m_strDescription	= entry.m_strDescription;
	m_strInternalClient	= entry.m_strInternalClient;

	return *this;
}

void CUPnpNatMapping::Empty()
{
	m_wInternalPort		= 0;
	m_wExternalPort		= 0;
	m_strProtocol.Empty();
	m_strDescription.Empty();
	m_strInternalClient.Empty();
}
