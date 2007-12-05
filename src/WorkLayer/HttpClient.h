#pragma once

#include "UpDownClient.h"
#include "TimerOp_UrlClientRetry.h"

//////////////////////////////////////////////////////////////////////////
enum URLEncodeType
{
	UET_NONE,
	UET_NOENCODE, //对url不做编码处理，直接发送..
	UET_UTF8,	
	UET_UTF16
};


///////////////////////////////////////////////////////////////////////////////
// CHttpClient

class CHttpClient : public CUpDownClient
{
	DECLARE_DYNAMIC(CHttpClient)

public:
	CHttpClient();
	virtual ~CHttpClient();

	bool SetUrl(LPCTSTR pszUrl, uint32 nIP = 0);

	virtual void SetRequestFile(CPartFile* pReqFile);
	virtual bool IsEd2kClient() const { return false; }

	virtual bool TryToConnect(bool bIgnoreMaxCon = false, CRuntimeClass* pClassSocket = NULL);
	virtual bool Connect();
	virtual void OnSocketConnected(int nErrorCode);
	virtual void ConnectionEstablished(); //< handshake finished.. just override the CUpDownClient::ConnectionEstablished
	virtual bool Disconnected(LPCTSTR pszReason, bool bFromSocket = false, CClientReqSocket* pSocket=NULL);

	virtual uint32  GetTimeUntilReask() const;

	virtual void ProcessRawData(const BYTE * pucData, UINT uSize);
	virtual void ProcessNoSizeRawData(const BYTE * pucData, UINT uSize);

	virtual bool SendHelloPacket();
	virtual void CreateBlockRequests(int iMaxBlocks);
	virtual bool SendHttpBlockRequests();
	virtual void SendCancelTransfer(Packet* packet);

	bool ProcessHttpDownResponse(const CStringAArray& astrHeaders);

	void OnNoSizeFileComplete();

	virtual void	Pause();	//ADDED by VC-fengwen 2007/08/03 : 

	CStringA m_strHostA;
	int m_iRedirected;

	CTimerOp_UrlClientRetry		m_latishConnectRetry;	
	CString						m_strRefer;	
	URLEncodeType				m_iUrlEncodeTypeToUse;
	URLEncodeType				m_iUrlEncodeTypeSucced;
private:
	/// 确保PendingBlockList 中是连续的Block请求
	void		EnsureContinueBlockRequest( );

	// VC-SearchDream[2007-07-23]: Add URL Record
	CString						m_strURL;
	// VC-SearchDream[2007-06-19]: for no size file downloading
	uint64						m_nRangeOffset;		

	BOOL						m_bDataTransfering;
	BOOL						m_bKnownSize;
	BOOL						m_bFirstHeader;

	void	FormatRequestHeader( CStringA& strHttpRequest,char *pCookie,char *pReferer,uint64 uiFrom,uint64 uiTo ); //< 拼出 http request header stringlist	
	bool	m_bHeadResponsed;
	uint64	m_uiFileSize;
//public:
//	HttpRequest m_HttpRequest;
//	HttpResponse m_HttpResponse;
public:
	UINT lastError;
	int m_ErrorCount;
	CString m_strUrl;
	uint32  m_nIP ;
    bool bAddOtherSources;
};