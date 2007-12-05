#pragma once
#include "EMSocket.h"

class CUpDownClient;
class CPacket;
//class CTimerWnd;

enum SocketState 
{
	SS_Other,		//These are sockets we created that may or may not be used.. Or incoming connections.
	SS_Half,		//These are sockets that we called ->connect(..) and waiting for some kind of response.
	SS_Complete	//These are sockets that have responded with either a connection or error.
};

class CClientReqSocket : public CEMSocket
{
	friend class CListenSocket;
	DECLARE_DYNCREATE(CClientReqSocket)

public:
	CClientReqSocket(CUpDownClient* in_client = NULL);

	void	SetClient(CUpDownClient* pClient);
	void	Disconnect(LPCTSTR pszReason);
	void	WaitForOnConnect();
	void	ResetTimeOutTimer();
	bool	CheckTimeOut();

	bool	IsLan();
	virtual UINT GetTimeOut();
	virtual void Safe_Delete();

	bool	Create();
	virtual void SendPacket(Packet* packet, bool delpacket = true, bool controlpacket = true, uint32 actualPayloadSize = 0, bool bForceImmediateSend = false);
	virtual SocketSentBytes SendControlData(uint32 maxNumberOfBytesToSend, uint32 overchargeMaxBytesToSend);
	virtual SocketSentBytes SendFileAndControlData(uint32 maxNumberOfBytesToSend, uint32 overchargeMaxBytesToSend);
	virtual void OnFailConnect(int nErrorCode);
	void	DbgAppendClientInfo(CString& str);
	CString DbgGetClientInfo();

	CUpDownClient*	client;
    virtual void Close() { CAsyncSocketEx::Close(); }
protected:
	virtual ~CClientReqSocket();
	//virtual void Close() { CAsyncSocketEx::Close(); }
	void	Delete_Timed();

	virtual void OnConnect(int nErrorCode);
	void		 OnClose(int nErrorCode);
	void		 OnSend(int nErrorCode);
	void		 OnReceive(int nErrorCode);
	void		 OnError(int nErrorCode);

	virtual bool PacketReceived(Packet* packet);
	int			 PacketReceivedSEH(Packet* packet);
	bool		 PacketReceivedCppEH(Packet* packet);

	bool	ProcessPacket(const BYTE* packet, uint32 size,UINT opcode);
	bool	ProcessExtPacket(const BYTE* packet, uint32 size, UINT opcode, UINT uRawSize);
	void	PacketToDebugLogLine(LPCTSTR protocol, const uchar* packet, uint32 size, UINT opcode);
	void	SetConState(SocketState val);

	uint32	timeout_timer;
	bool	m_bDeleteThis;
	uint32	deltimer;
	bool	m_bPortTestCon;
	uint32	m_nOnConnect;
public:
	bool IsDeleteThis() const
	{
		return m_bDeleteThis;
	}
	bool IsValidSocket() const
	{
		return m_SocketData.hSocket != INVALID_SOCKET;
	}
};
