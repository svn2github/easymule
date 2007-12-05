#pragma once

#include "ClientReqSocket.h"

enum E_FTP_STATES 
{
	ftpNone,
	ftpConnecting,              // Initial state
	ftpUSER, ftpPASS,           // Authenticating
	ftpSIZE_TYPE, ftpSIZE,      // File size getting via SIZE: TYPE I -> SIZE
	ftpLIST_TYPE, ftpLIST_PASVPORT, ftpLIST, // File size getting via LIST: TYPE A -> PASV or PORT -> LIST
	ftpDownloading,             // Intermediate state
	ftpRETR_TYPE, ftpRETR_PASVPORT, ftpRETR_REST, ftpRETR, // Downloading: TYPE I -> PASV or PORT -> REST -> RETR
	ftpABOR,                     // Aborting after each transfer (for some strange FTP servers)
	ftpClose
};

/// 处理Ftp命令通道协议的Socket
class CFtpClientReqSocket : public CClientReqSocket
{
	DECLARE_DYNCREATE(CFtpClientReqSocket)

public:

	CFtpClientReqSocket(CUpDownClient* client = NULL);
	~CFtpClientReqSocket(void);

	virtual CUpDownClient* GetClient() const 
	{ 
		return client; 
	}
	
	virtual bool IsRawDataMode() const 
	{ 
		return true; 
	}	
		
	virtual BOOL Connect(SOCKADDR* pSockAddr, int iSockAddrLen);
	BOOL SendCommand( ); //< 发送FTP CMD

	void SetFtpState( E_FTP_STATES ftpState ){ m_FtpState=ftpState; }
	virtual void	SetDownloadLimit(uint32 limit); //<ftp 的限速应该特殊处理,限的应该是数据通道,而不是命令通道..
	virtual UINT	GetTimeOut();
protected:
	virtual void DataReceived(const BYTE* pucData, UINT uSize);
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
    
private:	
	BOOL			OnHeaderLine ( CStringA& strHeader, CStringA& strValue ); //< Header返回,一般是以'\r\n'结尾,对应每一个发送的命令
public:
	E_FTP_STATES	m_FtpState; //< FTP命令通道状态..
private:
	BOOL			m_bPassive; //< 对于下载,最好采用PASV模式,方便内网用户
	BOOL			m_bDecLaunchTimes; //< 属于客户端错误(连接过多等情况..)
	BOOL			m_bServerErr; //< ftp server 端错误
};

/// 处理Ftp数据通道协议的Socket
class CFtpClientDataSocket : public CClientReqSocket
{
	DECLARE_DYNCREATE(CFtpClientDataSocket)

public:
	CFtpClientDataSocket(CUpDownClient* in_client = NULL);
	~CFtpClientDataSocket(void);

	virtual CUpDownClient* GetClient() const 
	{ 
		return client; 
	}
	virtual bool IsRawDataMode() const 
	{ 
		return true; 
	}
protected:
	virtual void DataReceived(const BYTE* pucData, UINT uSize);
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};
