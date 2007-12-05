#pragma once

class CUPnpNatMappingKey
{
public:
	CUPnpNatMappingKey(void);
	~CUPnpNatMappingKey(void);

	CUPnpNatMappingKey(const CUPnpNatMappingKey	&key);
	CUPnpNatMappingKey&		operator=(const CUPnpNatMappingKey &right);
	bool					operator==(const CUPnpNatMappingKey &right) const;
	bool					operator!=(const CUPnpNatMappingKey &right) const;
	bool					operator<(const CUPnpNatMappingKey &right) const;

	void	Empty();

	void	Serialize(CArchive& ar);
public:

	CString		m_strRemoteHost;
	USHORT		m_usExternalPort;
	CString		m_strProtocol;
};
