// ***************************************************************
//  FtpClient   version:  1.0   ·  date: 08/09/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// Ftp Peer 定义,从ParFile获取下载数据任务,并把下载后的数据交给PartFile
// Ftp Peer 需要管理两个Socket通道( 命令Socket和数据Socket)
//                                         vc-huby
// ***************************************************************

#pragma once
#include "updownclient.h"

#include "SourceURL.h"
#include "FTPClientReqSocket.h"


class CFtpClient : public CUpDownClient
{
	DECLARE_DYNAMIC(CFtpClient)
public:
	CFtpClient(void);
	virtual ~CFtpClient(void);

	bool SetUrl(LPCTSTR pszUrl, uint32 nIP = 0);

	virtual void SetRequestFile(CPartFile* pReqFile);	
	virtual bool IsEd2kClient() const 
	{ 
		return false; 
	}

	virtual bool TryToConnect(bool bIgnoreMaxCon = false, CRuntimeClass* pClassSocket = NULL);
	virtual bool Connect();
	
	virtual void OnSocketConnected(int nErrorCode);
	virtual void ConnectionEstablished();
	virtual bool Disconnected(LPCTSTR pszReason, bool bFromSocket = false,CClientReqSocket* pSocket=NULL);

	virtual void Pause( );
	virtual uint32  GetTimeUntilReask() const; ///< http/ftp Peer如果没在下载,可以10s后再询问一次.

	bool		 OnFileSizeKnown( uint64 uiFileSize );
	void		 OnCmdSocketErr(CStringA strError);
	virtual void ProcessRawData(const BYTE * pucData, UINT uSize);

	virtual bool SendHelloPacket();
	virtual bool SendFtpBlockRequests();
	virtual void SendCancelTransfer(Packet* packet);
	virtual void CheckDownloadTimeout();
	
	CFtpClientDataSocket* GetClientDataSocket( bool bCreateIfNull=true );
	CFtpClientReqSocket*  GetClientReqSocket( );

	virtual void	CreateBlockRequests(int iMaxBlocks);
	bool		 GetUrlStartPosForReq(  uint64& uiUrlStartPos ); //< 计算此ftp peer应该从数据的哪个位置开始发请求

	CSourceURL			m_SourceURL;  
	uint64				m_uiFileSize;		 //< 此INet peer的远程文件大小,和PartFileSize大小不一致的时候不参与下载	

	void				CloseDataSocket();
	int					PendingBlockReqCount(){ return m_PendingBlocks_list.GetCount();}
private:
	
	CFtpClientDataSocket *	m_pFtpDataSocket; //< 发PASV命令成功后才会创建数据通道传文件数据
public:
	CStringA lastError;
	int m_ErrorCount;
	CString m_strUrl;
	uint32  m_nIP ;
    bool bAddOtherSources;
};
