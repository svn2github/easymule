#include "StdAfx.h"
#include ".\upnpnatmappingkey.h"

CUPnpNatMappingKey::CUPnpNatMappingKey(void)
{
	Empty();
}

CUPnpNatMappingKey::~CUPnpNatMappingKey(void)
{
}

CUPnpNatMappingKey::CUPnpNatMappingKey(const CUPnpNatMappingKey	&key)
{
	*this = key;
}

CUPnpNatMappingKey& CUPnpNatMappingKey::operator=(const CUPnpNatMappingKey &right)
{
	if (this == &right)
		return *this;

	m_strRemoteHost		= right.m_strRemoteHost;
	m_usExternalPort	= right.m_usExternalPort;
	m_strProtocol		= right.m_strProtocol;

	return *this;
}

bool CUPnpNatMappingKey::operator==(const CUPnpNatMappingKey &right) const
{
	if (this == &right)
		return true;

	if (m_usExternalPort != right.m_usExternalPort)
		return false;

	if (0 != m_strProtocol.CompareNoCase(right.m_strProtocol))
		return false;

	if (m_strRemoteHost != right.m_strRemoteHost)
		return false;

	return true;
}

bool CUPnpNatMappingKey::operator!=(const CUPnpNatMappingKey &right) const
{
	return !(*this == right);
}

bool CUPnpNatMappingKey::operator<(const CUPnpNatMappingKey &right) const
{
	if (this == &right)
		return false;

	if (m_usExternalPort != right.m_usExternalPort)
		return m_usExternalPort < right.m_usExternalPort;

	if (0 != m_strProtocol.CompareNoCase(right.m_strProtocol))
		return m_strProtocol < right.m_strProtocol;

	if (0 != m_strRemoteHost.CompareNoCase(right.m_strRemoteHost))
		return m_strRemoteHost < right.m_strRemoteHost;

	return false;
}

void CUPnpNatMappingKey::Empty()
{
	m_strRemoteHost.Empty();
	m_usExternalPort = 0;
	m_strProtocol.Empty();
}

void CUPnpNatMappingKey::Serialize(CArchive& ar)
{
	if (ar.IsLoading())
	{
		ar >> m_strRemoteHost;
		ar >> m_usExternalPort;
		ar >> m_strProtocol;
	}
	else
	{
		ar << m_strRemoteHost;
		ar << m_usExternalPort;
		ar << m_strProtocol;
	}
}
