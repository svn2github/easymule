#pragma once


#include "ListenSocket.h"
#include "HttpClientReqSocket.h"

// CSendGetUrlReqSocket ÃüÁîÄ¿±ê

class CSendGetUrlReqSocket : public CHttpClientReqSocket
{
public:
	CSendGetUrlReqSocket();
	virtual ~CSendGetUrlReqSocket();

	bool	SendRequest(void);

protected:
	virtual CStringA GetServer() = 0;
	virtual UINT GetPort()				{return 80;}
	virtual CStringA GetUrlPath() = 0;

	virtual void	DataReceived(const BYTE* pucData, UINT uSize);
	virtual bool	ProcessHttpResponse();
	virtual bool	ProcessHttpResponseBody(const BYTE* pucData, UINT size);

protected:
	virtual void	OnConnect(int nErrorCode);

};


