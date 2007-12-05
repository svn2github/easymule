#include "stdafx.h"
#include "TraverseBySvr.h"

//#include "../emule.h"
#include "NatThread.h"
#include "NatSocket.h"
#include "ClientList.h"
#include "updownclient.h"
#include "ListenSocket.h"
#include "sockets.h"
#include "GlobalVariable.h"

extern void T_TRACE(char* fmt, ...);

CTraverseBySvr::CTraverseBySvr(const uchar * uh, CTraverseStrategy * pNext)  : CTraverseStrategy(uh, pNext)
{
	m_dwTraverseIp = 0;
	m_wTraversePort = 0;

	m_Sock = NULL;

	m_SendPingTime = m_SendReqTime = m_SendConnAckTime = 0;
	m_nSendReqCount = 0;
	m_nSendPingCount = 0;
	m_nPassivePing = 0;

	m_bAcceptor = false;
	m_dwClientIp = 0;
	m_wClientPort = 0;

	m_dwState = 0;
	m_nSendConnAck = 0;
	m_dwConnAskNumber = GetTickCount();

}

void CTraverseBySvr::StopTraverse()
{
	CTraverseStrategy * strategy=GetNextStrategy();
	SetNextStrategy(NULL);
	delete strategy;
}

void CTraverseBySvr::SendPacket()
{
	m_bFailed = m_dwState & NAT_E_NOPEER;

	try
	{
		if(m_dwState & NAT_S_WAKEUP)
		{
			if(time(NULL)-m_SendPingTime >4)
			{
				if(m_nPassivePing> 15)
				{
					//  maybe try to next strategy
					//_AddLogLine(false, _T("Passive Unconnected NatSock was deleted. timeout. %s."), UserHashToString(m_UserHash));
					Failed();
				}
				else SendPingPacket();
			}
			return;
		}

		if(!m_bFailed && time(NULL)-m_SendReqTime > 5) //  5 seconds
		{
			//  failed to connect
			if(m_nSendReqCount>12)
			{
				m_dwState|=NAT_E_TIMEOUT;
				if(m_nSendReqCount>23)
					Failed();
				m_nSendReqCount++;
			}
			else
			{
				if(m_nSendReqCount==0)
					_AddLogLine(false, _T("Begin to connect %s by Nat-traverse server."), UserHashToString(m_UserHash));
				SendConnectReq();
			}
		}

		if(m_bFailed)
		{
			if(m_Sock)// && pClientSock->client)
			{
				//m_Sock->m_bUseNat = false;
			}

			if(m_dwState & NAT_E_NOPEER)
			{
				//  maybe try to next strategy
				//_AddLogLine(false, _T("Unconnected NatSock was deleted. server return E_NOPEER. %s."), UserHashToString(m_UserHash));
			}
			else
			{
				//  maybe try to next strategy
				//_AddLogLine(false, _T("Unconnected NatSock was deleted. time out. %s."), UserHashToString(m_UserHash));
			}
		}

		if(m_dwState & NAT_S_SYNC && time(NULL)-m_SendPingTime >4)
		{
			if(m_nPassivePing> 15)
			{
				//  maybe try to next strategy
				//_AddLogLine(false, _T("Passive Unconnected NatSock was deleted. timeout. %s."), UserHashToString(m_UserHash));
				Failed();
			}
			else SendPingPacket();
		}
	}
	catch(...)
	{
		TRACE("Exception: %s\n", __FUNCTION__);
		//  the CAsyncSocketEx maybe is deleted
		Failed();
	}
}

int CTraverseBySvr::SendConnectReq()
{
	if(m_dwState & NAT_E_TIMEOUT) return 1;

	m_SendReqTime = time(NULL);
	m_nSendReqCount++;

	uchar pack[16*2+4];
	GetMyUserHash((uchar*)pack);
	memcpy(pack+16, &m_dwConnAskNumber, 4);
	memcpy(pack+20, m_UserHash, 16);

	//TRACE(_T("send conn request for %s\n"),UserHashToString(m_UserHash));

	return CNatSocket::SendPacket(m_dwTraverseIp, m_wTraversePort, OP_NAT_SYNC2, pack, 16*2+4);
}

int CTraverseBySvr::SendPingPacket()
{
	if(m_dwState & NAT_E_TIMEOUT) return 1;

	m_nPassivePing++;
	m_SendPingTime = time(NULL);

/*	T_TRACE("send ping packet for %02X%02X%02X%02X. ConnAck=%08x\n", m_UserHash[0],m_UserHash[1],m_UserHash[2],m_UserHash[3],
		m_dwConnAskNumber);*/

	if(m_dwClientIp && m_wClientPort)
	{
		uchar pack[20];
		GetMyUserHash(pack);
		memcpy(pack+16, &m_dwConnAskNumber, 4);
		return CNatSocket::SendPacket(m_dwClientIp, m_wClientPort, OP_NAT_PING, pack, 20);
	}

	return -1;
}

bool CTraverseBySvr::Initialize()
{
	CGlobalVariable::natthread->GetTraversalSvr(m_dwTraverseIp, m_wTraversePort);
	if(!m_dwTraverseIp || !m_wTraversePort) return false;

	CUpDownClient * client= CGlobalVariable::clientlist->FindClientByUserHash(m_UserHash);
	if(!client || !client->socket) return false;
	m_Sock = client->socket;

	CNatSocket * nsock=CGlobalVariable::natthread->WakeupNatSocketFromPool(m_UserHash);
	if(nsock)
	{
		nsock->m_dwConnAskNumber = m_dwConnAskNumber;

		m_dwState = NAT_S_WAKEUP;
		m_dwClientIp = nsock->GetTargetIP();
		m_wClientPort = nsock->GetTargetPort();
	}
	return true;
}

bool CTraverseBySvr::ProcessPacket(const uchar * data, int len, DWORD ip, WORD port)
{
	uint8 opcode=data[0];
	//UINT realsize=len-1;
	const uchar * realdata=data+1;

	switch(opcode)
	{
	case OP_NAT_FAILED:
		assert(memcmp(realdata+1, m_UserHash, 16)==0);
		if(realdata[0]==1)
		{
			m_dwState |= NAT_E_NOPEER;
			TRACE(_T("*** Server tell no Registered Peer about %s\n"), UserHashToString(m_UserHash));
			Failed();
			//StopTraverse();
			return true;
		}
		break;
	case OP_NAT_SYNC2:
	case OP_NAT_SYNC:
		OnRecvSync(data, len, ip, port);
		return true;
	case OP_NAT_PING:
		OnRecvPing(data, len, ip, port);
		return true;
	}

	return false;
}

void CTraverseBySvr::OnRecvSync(const uchar * data, int /*len*/, DWORD /*ip*/, WORD /*port*/)
{
	//UINT realsize=len-1;
	const uchar * realdata=data+1;

	DWORD cip = *(DWORD*)(realdata);
	WORD cport = *(WORD*)(realdata+4);

	if(cip==0 || cport==0)
	{
		return;
	}
	const uchar * id=realdata+6;

	DWORD ConnAck = 0;
	memcpy(&ConnAck, id+16, 4);

	TRACE("recv sync, ConnAck=%08x\n", ConnAck);
#ifdef _DEBUG
	in_addr addr;
	addr.s_addr = cip;
	TRACE("peer addr of %02X%02X%02X%02X = %s:%d\n", id[0], id[1], id[2],id[3],
		inet_ntoa(addr), ntohs(cport));
#endif
	uchar myid[16];
	GetMyUserHash(myid);
	if(memcmp(id, myid, 16)==0)
	{
		_AddLogLine(false, _T("ERROR: recv myself for traversal"));
		return;
	}

	CAsyncSocketEx * pASock=NULL;
	CUpDownClient * pClient= CGlobalVariable::clientlist->FindClientByUserHash(id);
	if(pClient)
	{
		pASock = pClient->socket;

		if(! pASock)
		{
			CRuntimeClass * pClassSocket = RUNTIME_CLASS(CClientReqSocket);
			CClientReqSocket * socket = static_cast<CClientReqSocket*>(pClassSocket->CreateObject());
			socket->SetClient(pClient);
			if (!socket->Create())
			{
				socket->Safe_Delete();
				return;
			}

			pClient->socket = socket;
			pASock = socket;
		}
		ASSERT(pASock);
		if(! pASock) return;
	}
	else
	{
		TRACE("\n->%s: Accept a incoming sock for %02X%02X%02X%02X\n", __FUNCTION__,
			id[0],id[1],id[2],id[3]);

		pASock = CGlobalVariable::natthread->GetTempASock(id);
	}

	m_dwClientIp = cip;
	m_wClientPort = cport;

	m_dwState |=NAT_S_SYNC;
	SendPingPacket();
}

void CTraverseBySvr::OnRecvPing(const uchar * data, int /*len*/, DWORD ip, WORD port)
{
	//UINT realsize=len-1;
	const uchar * realdata=data+1;

	const uchar * hash=realdata;

	DWORD ConnAck=0;
	memcpy(&ConnAck, hash+16, 4);
	TRACE("recv ping 4. %02X%02X%02X%02X, ConnAck=%d\n", hash[0], hash[1], hash[2], hash[3],
		ConnAck);
	CAsyncSocketEx * pASock=NULL;

	if(ConnAck && ConnAck!=m_dwConnAskNumber)
		m_dwConnAskNumber = ConnAck;
	pASock = m_Sock;

	ASSERT(pASock);
	if(!pASock)
	{
		TRACE("no AsyncsocketEx for unconnect sk\n");
		Failed();
		return;
	}

	CNatSocket * nsock = CGlobalVariable::natthread->FindNatSocket(hash);
	if(nsock)
	{
		if(nsock->GetParent()!=pASock)
		{
			CGlobalVariable::natthread->ResetNatSocket(nsock->GetParent(), pASock);
		}
		//pASock->TriggerEvent(FD_CONNECT);
		//pASock->TriggerEvent(FD_WRITE);
		Finish();
		return ;
	}

	nsock=new CNatSocket(pASock);
	nsock->m_dwConnAskNumber= m_dwConnAskNumber;
	sUserModeTCPConfig * tc=new sUserModeTCPConfig(ip, port);
	nsock->SetConfig(tc);

	nsock->m_TraversalType = CNatSocket::Traversal_bysvr;
//	nsock->m_bTraverseBySvr= true;
//	m_dwTraverseBySvr++;

	memcpy(nsock->GetUserHash(), hash, 16);

	CGlobalVariable::natthread->AddNatSocket(pASock, nsock);
	CGlobalVariable::natthread->RemoveUnconnSocket(hash);	//ADDED by VC-fengwen on 2007/10/15 : sock连通后，在unconnectsocket里要清除它。

	_AddLogLine(false, _T("NAT traversal connected. %s."), UserHashToString(hash));

	CGlobalVariable::natthread->RemoveTempASock(hash);

	//pASock->TriggerEvent(FD_CONNECT);
	//pASock->TriggerEvent(FD_WRITE);
	CClientReqSocket * pClientSock = DYNAMIC_DOWNCAST(CClientReqSocket, pASock);
	if(pClientSock && pClientSock->client)
	{
		pClientSock->client->SetIP(ip);
		pClientSock->client->ResetIP2Country();
	}
	TRACE("*********** notify connect sock=%08x, client=%08x\n", pASock, pClientSock->client);
	Finish();
}
