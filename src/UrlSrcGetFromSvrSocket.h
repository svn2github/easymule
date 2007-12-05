#pragma once


#include "SendGetUrlReqSocket.h"
#include "UrlSrcFromSvrMgr.h"
// CUrlSrcGetFromSvrSocket 命令目标

class CUrlSrcGetFromSvrSocket : public CSendGetUrlReqSocket
{
public:
	CUrlSrcGetFromSvrSocket(CUrlSrcFromSvrMgr *pMgr, bool bStart);
	virtual ~CUrlSrcGetFromSvrSocket();

protected:
	virtual CStringA GetServer();
	virtual CStringA GetUrlPath();

	virtual bool	ProcessHttpResponse();
	virtual bool	ProcessHttpResponseBody(const BYTE* pucData, UINT size);

	bool	ProcessHttpResponse_Start();
	bool	ProcessHttpResponseBody_Start(const BYTE* pucData, UINT size);
	bool	ProcessHttpResponse_Finished();
	bool	ProcessHttpResponseBody_Finished(const BYTE* pucData, UINT size);
protected:
	CUrlSrcFromSvrMgr		*m_pMgr;
	
	bool					m_bStart;	//true:		表示文件开始下载。
										//false:	表示文件完成下载。

};


