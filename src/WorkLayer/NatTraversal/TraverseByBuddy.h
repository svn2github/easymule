#pragma once
#include "TraverseStrategy.h"
#include "ClientList.h"
#include "updownclient.h"
#include "ListenSocket.h"
#include "sockets.h"

class CTraverseByBuddy : public CTraverseStrategy
{
public:
	CTraverseByBuddy(const uchar * uh, CTraverseStrategy * pNext);

	virtual bool Initialize();
	virtual void SendPacket();
	virtual bool ProcessPacket(const uchar * data, int len, DWORD ip, WORD port);
private:
	int SendConnectReq();
	int SendPingPacket();
	void OnRecvSync(const uchar * data, int len, DWORD ip, WORD port);
	void OnRecvPing(const uchar * data, int len, DWORD ip, WORD port);

	DWORD m_dwTraverseIp;
	WORD m_wTraversePort;

	int			m_nSendReqCount ,m_nSendPingCount , m_nSendConnAck;
	int			m_nPassivePing;
	DWORD		m_SendReqTime, m_SendPingTime, m_SendConnAckTime;
	DWORD		m_dwClientIp;
	WORD		m_wClientPort;
	CAsyncSocketEx * m_Sock;

	bool		m_bAcceptor;
	bool		m_bByBuddy;
	DWORD		m_dwState;
	DWORD		m_dwConnAskNumber;
};
