#pragma once

class CClientReqSocket;
class CUpDownClient;

// VC-SearchDream[2007-04-18]: For SourceExchange NAT Begin
enum EUDPState
{
	UDP_DISCONNECT = 0,
	UDP_CONNECTED  = 1
};
// VC-SearchDream[2007-04-18]: For SourceExchange NAT End

class CSourceExchangeNAT
{
public:

	CSourceExchangeNAT();
	virtual ~CSourceExchangeNAT();

	static void CheckUDPTunnel(CUpDownClient * pClient);

	static bool ProcessUDPPacket(const BYTE* packet, UINT size, uint8 opcode, uint32 ip, uint16 port);
	static bool ProcessTCPPacket(const BYTE* packet, uint32 size, UINT opcode, UINT uRawSize, CClientReqSocket * socket);

protected:

	static void CheckConnection(CUpDownClient * pClient);
	static void CheckForTimeOut(CUpDownClient * pClient);

	static bool SendUDPPacket(DWORD ip, WORD port, const uchar * data, int len, CUpDownClient * pClient);
};