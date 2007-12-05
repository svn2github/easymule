#include "StdAfx.h"
#include "ftpclient.h"

#include <wininet.h>
#include "PartFile.h"
#include "Preferences.h"
#include "ClientCredits.h"
#include "globalvariable.h"
#include "emule.h"

IMPLEMENT_DYNAMIC(CFtpClient, CUpDownClient)

CFtpClient::CFtpClient(void)
{
	m_clientSoft		= SO_URL;
	m_strClientSoftware = _T("ftp server");
	m_pFtpDataSocket	= NULL;
	m_uiFileSize		= 0;
    
	m_iPeerType = ptFtp;
	m_ErrorCount = 0;
	m_nIP = 0 ;
	bAddOtherSources = true;
}

CFtpClient::~CFtpClient(void)
{
	//CMDSocket 会在CUpDownClient中Delete并CloseSocket

	if( m_pFtpDataSocket )
	{
		m_pFtpDataSocket->client = NULL; 
		delete m_pFtpDataSocket;
	}
}

void CFtpClient::SetRequestFile(CPartFile* pReqFile)
{
	CUpDownClient::SetRequestFile(pReqFile);
	if (reqfile)
	{
		m_nPartCount    = reqfile->GetPartCount();
		if(NULL==m_abyPartStatus)
			m_abyPartStatus = new uint8[m_nPartCount];
		memset(m_abyPartStatus, 0, m_nPartCount);
		m_bCompleteSource = false;
	}
}

bool CFtpClient::SetUrl(LPCTSTR pszUrl, uint32 nIP)
{
	m_strUrl = pszUrl;
	m_nIP = nIP;
	if ( !m_SourceURL.ParseFTP( pszUrl ) )
	{
		return false;
	}

	m_nUrlStartPos = (uint64)-1;

	//SetUserName(m_SourceURL.m_sAddress);

	if (nIP)
	{
		m_nConnectIP = nIP;
		m_dwUserIP	 = nIP;
		ResetIP2Country();
	}
	else
	{
		return false; // We do not do DNS here
	}

	m_nUserIDHybrid = htonl(m_nConnectIP);
	ASSERT( m_nUserIDHybrid != 0 );
	m_nUserPort     = m_SourceURL.m_nPort;

	CString sUserName;
#ifdef _DEBUG_PEER
	sUserName.Format( _T("%s(%d)"),ipstr(nIP),m_iPeerIndex);
#else
	sUserName = ipstr( nIP );
#endif
	SetUserName( sUserName );

	return true;
}

bool CFtpClient::SendFtpBlockRequests()
{
	USES_CONVERSION;
	
	m_dwLastBlockReceived = ::GetTickCount();
	
	if (reqfile == NULL)
	{
		throw CString(_T("Failed to send block requests - No 'reqfile' attached"));
	}

	if (m_PendingBlocks_list.IsEmpty())
	{
		while( reqfile->GetBlockRange( this ) )
		{
			CreateBlockRequests(INetPeerBlockReqCount);	//PARTSIZE / EMBLOCKSIZE + 1
			if( !m_PendingBlocks_list.IsEmpty() )
				break;
		}			
	}	
	
	if( m_PendingBlocks_list.IsEmpty() )
	{
		SetDownloadState(DS_NONEEDEDPARTS);
		return false;
	}

	POSITION pos = m_PendingBlocks_list.GetHeadPosition();
	Pending_Block_Struct* pending = m_PendingBlocks_list.GetNext(pos);

	m_uReqStart = pending->block->StartOffset;
	m_uReqEnd   = pending->block->EndOffset;
	
	bool bMergeBlocks = true;
	
	while (pos)
	{
		POSITION posLast = pos;
		pending = m_PendingBlocks_list.GetNext(pos);
		if (bMergeBlocks && pending->block->StartOffset == m_uReqEnd + 1)
		{
			m_uReqEnd = pending->block->EndOffset;
		}
		else
		{
			bMergeBlocks = false;
			reqfile->RemoveBlockFromList(pending->block->StartOffset, pending->block->EndOffset);
			delete pending->block;
			delete pending;
			m_PendingBlocks_list.RemoveAt(posLast);
		}
	}

	m_nUrlStartPos = m_uReqStart;

	Debug( _T("BlockRequest(Ftp):BlockCount=%d,start=%I64u,end=%I64u \n"),m_PendingBlocks_list.GetCount(),m_uReqStart,m_uReqEnd );

	return true;
}

bool CFtpClient::TryToConnect(bool bIgnoreMaxCon, CRuntimeClass* /*pClassSocket*/)
{
	if( m_pFtpDataSocket )
	{
		delete m_pFtpDataSocket;
		m_pFtpDataSocket = NULL;
	}

	if( reqfile->GetPartFileSizeStatus()==FS_UNKNOWN )
	{
		reqfile->m_dwTickGetFileSize = GetTickCount();
	}

	return CUpDownClient::TryToConnect(bIgnoreMaxCon, RUNTIME_CLASS(CFtpClientReqSocket));	
}

bool CFtpClient::Connect()
{
	if (GetConnectIP() != 0 && GetConnectIP() != INADDR_NONE)
	{
		CString temp;
		temp.Format(GetResString(IDS_CONNECT_INFOMATION),m_SourceURL.m_sAddress,m_nUserPort);
		AddPeerLog(new CTraceInformation(temp));

		return CUpDownClient::Connect();
	}

	//Try to always tell the socket to WaitForOnConnect before you call Connect.
	socket->WaitForOnConnect();
	//socket->Connect(m_strHostA, m_nUserPort);

	return true;
}

void CFtpClient::OnSocketConnected(int nErrorCode)
{
	if (nErrorCode == 0)
	{
		AddPeerLog(new CTraceInformation(GetResString(IDS_CONNECTED)));
		SetDownloadState( DS_CONNECTED );
	}
}

bool CFtpClient::SendHelloPacket()
{
	return true;
}


bool CFtpClient::Disconnected(LPCTSTR pszReason, bool bFromSocket ,CClientReqSocket* pSocket)
{
 	if( pSocket && pSocket->IsKindOf(RUNTIME_CLASS(CFtpClientDataSocket)) )
	{   //只是ftpDataSocket Disconnect
		CloseDataSocket();
		if( socket==NULL )
			return false;
		CFtpClientReqSocket* reqSocket =  DYNAMIC_DOWNCAST(CFtpClientReqSocket,socket);
		reqSocket->SetFtpState(ftpClose); 		
		return false;
	}

#ifdef _DEBUG_PEER
	Debug( _T("Peer(%d)-Ftp Disconnected because-%s \n"),m_iPeerIndex,pszReason );
#endif	

	//return CUpDownClient::Disconnected(CString(_T("CFtpClient::Disconnected")) + pszReason, bFromSocket);
	ASSERT( CGlobalVariable::clientlist->IsValidClient(this) );

	m_iErrTimes++;

	SetDownloadState( DS_ERROR, CString(_T("Disconnected: ")) + pszReason);		

	if ( GetDownloadState() != DS_DOWNLOADING )
	{		
		// ensure that all possible block requests are removed from the partfile
		ClearDownloadBlockRequests();
	}

	if (bFromSocket && socket)
	{
		ASSERT( CGlobalVariable::listensocket->IsValidSocket(socket) );
		socket->Safe_Delete();
	}

	socket = NULL;

	CloseDataSocket();

	if( m_pBlockRangeToDo!=NULL )
	{
		m_pBlockRangeToDo->m_pClient = NULL;
		m_pBlockRangeToDo->m_dwTakeOverTime = 0;
		m_pBlockRangeToDo = NULL;
	}

	if( GetDownloadState()==DS_ERROR )
		SetCompletePartStatus(false);

	UpdateUI(UI_UPDATE_PEERLIST);

	CString sTemp;
	sTemp.Format(GetResString(IDS_AFTER_RECONNECT),m_iErrTimes*thePrefs.GetRetryDelay());
	AddPeerLog(new CTraceError(sTemp));
	reqfile->m_TotalRetryTimes++;
	reqfile->RetryManage(reqfile->m_TotalRetryTimes);
 
	return false;
}

/// 下载的文件数据来了,在这里处理
void CFtpClient::ProcessRawData(const BYTE * pucData, UINT uSize)
{ 
	// VC-wangna[2007-11-26]: Add other connect
	if(bAddOtherSources)//判断是否需要加入其它几个连接
	{
		int i = 1;
		int iMax = thePrefs.GetMaxSourceConnect();
		while(i< iMax)
		{
			CFtpClient * client = new CFtpClient;
			client->bAddOtherSources = false;
			if (!client->SetUrl(m_strUrl, m_nIP))
			{
				LogError(LOG_STATUSBAR, _T("Failed to process URL source \"%s\""), m_strUrl);
				delete client;
				return;
			}
			client->SetRequestFile(reqfile);
			client->SetSourceFrom(SF_FTP);
			if (CGlobalVariable::downloadqueue->CheckAndAddSource(reqfile, client))
			{
				reqfile->UpdatePartsInfo(); //TODO: 对于INet Peer，这里可能不需要了
			}
			i++;
		}
		bAddOtherSources = false;
	}
	// VC-wangna[2007-11-26]: Add other connect
	
#ifdef _DEBUG_PEER_DATA
	Debug( _T("Receive Data from FTP size : %d \n"), uSize);
#endif
	if (reqfile == NULL)
	{
		throw CString(_T("Failed to process FTP data block - No 'reqfile' attached"));
	}

	if (reqfile->IsStopped() || (reqfile->GetStatus() != PS_READY && reqfile->GetStatus() != PS_EMPTY))
	{
		throw CString(_T("Failed to process FTP data block - File not ready for receiving data"));
	}

	if (m_nUrlStartPos == (uint64)-1)
	{
		throw CString(_T("Failed to process FTP data block - Unexpected file data"));
	}

	if( socket!=NULL )
	{
		socket->ResetTimeOutTimer(); /// 避免命令通道Socket超时检测中断.
	}

	uint64 nStartPos = m_nUrlStartPos;
	uint64 nEndPos   = m_nUrlStartPos + uSize;

	m_nUrlStartPos  += uSize;

	m_dwLastBlockReceived = ::GetTickCount();

	if( nEndPos == nStartPos || uSize != nEndPos - nStartPos )
	{
		throw CString(_T("Failed to process FTP data block - Invalid block start/end offsets"));
	}

	thePrefs.Add2SessionTransferData(GetClientSoft(), (GetClientSoft()==SO_URL) ? (UINT)-2 : (UINT)-1, false, false, uSize);

	m_nDownDataRateMS += uSize;

	if (credits)
	{
		credits->AddDownloaded(uSize, GetIP());
	}

	nEndPos--;

	UINT iBlockRequestToGet = max(min(GetDownloadDatarate()*10,EMBLOCKSIZE*10),uSize)/EMBLOCKSIZE +1;
	if( m_pBlockRangeToDo )
		iBlockRequestToGet = min(iBlockRequestToGet,m_pBlockRangeToDo->m_iBlockIndexE-m_pBlockRangeToDo->m_iBlockCurrentDoing+1);
	if( (UINT)m_PendingBlocks_list.GetCount()<iBlockRequestToGet ) //网络数据来了,但此Peer还没领取Block任务或是没领够
	{
		while( reqfile->GetBlockRange( this ) )
		{
			CreateBlockRequests(iBlockRequestToGet);	 //PARTSIZE / EMBLOCKSIZE + 1
			if( !m_PendingBlocks_list.IsEmpty() )
				break;
		}		
	}

	if( m_PendingBlocks_list.IsEmpty() )
	{
		Disconnected( _T( "NONEEDEDPARTS from this Peer.") );
		SetDownloadState( DS_NONEEDEDPARTS );
		TRACE("%s - Dropping packet(%d)-Peer(%d),because DS_NONEEDEDPARTS \n", __FUNCTION__,uSize,m_iPeerIndex);
		return;
	}

	uint32 lenWrittenTotal =0;

	for (POSITION pos = m_PendingBlocks_list.GetHeadPosition(); pos != NULL; )
	{
		POSITION posLast = pos;
		Pending_Block_Struct *cur_block = m_PendingBlocks_list.GetNext(pos);
		if (cur_block->block->StartOffset <= nStartPos && nStartPos <= cur_block->block->EndOffset)
		{
			if (thePrefs.GetDebugClientTCPLevel() > 0)
			{
				// NOTE: 'Left' is only accurate in case we have one(!) request block!
				void* p = m_pPCDownSocket ? (void*)m_pPCDownSocket : (void*)socket;
				Debug(_T("%08x  Start=%I64u  End=%I64u  Size=%u  Left=%I64u  %s\n"), p, nStartPos, nEndPos, uSize, cur_block->block->EndOffset - (nStartPos + uSize) + 1, DbgGetFileInfo(reqfile->GetFileHash()));
			}

			m_nLastBlockOffset = nStartPos;

			// nightsuns: 防止文件被 metalink 这一类重定向了
			CPartFile* tempfile = this->reqfile;
			uint32 lenWritten = reqfile->WriteToBuffer(uSize, pucData, nStartPos, nEndPos, cur_block->block, this);

			if (lenWritten > 0 && tempfile->GetPartFileSizeStatus() != FS_UNKNOWN )
			{
				m_nTransferredDown += uSize;
				m_nCurSessionPayloadDown += lenWritten;
				SetTransferredDownMini();

				lenWrittenTotal +=lenWritten;

				if (nEndPos >= cur_block->block->EndOffset)
				{
					reqfile->RemoveBlockFromList(cur_block->block->StartOffset, cur_block->block->EndOffset);
#ifdef _DEBUG_PEER
					Debug( _T("Peer(%d)-Ftp Finished BlockJob(%d):%I64u-%I64u \n"),m_iPeerIndex,cur_block->block->BlockIdx,cur_block->block->StartOffset,cur_block->block->EndOffset );
#endif

					if(m_pBlockRangeToDo)
					{
						if( (cur_block->block->BlockIdx+1)<=m_pBlockRangeToDo->m_iBlockIndexE )
						{
							m_pBlockRangeToDo->m_iBlockCurrentDoing=cur_block->block->BlockIdx+1;
							ASSERT( m_pBlockRangeToDo->m_iBlockCurrentDoing>=m_pBlockRangeToDo->m_iBlockIndexS 
								&& m_pBlockRangeToDo->m_iBlockCurrentDoing<=m_pBlockRangeToDo->m_iBlockIndexE);							
						}
						else
						{
#ifdef _DEBUG_PEER
							Debug( _T("Peer(%d)-Ftp Finished BlockRange(%d-%d-%d)\n"),m_iPeerIndex,m_pBlockRangeToDo->m_iBlockIndexS,m_pBlockRangeToDo->m_iBlockCurrentDoing,m_pBlockRangeToDo->m_iBlockIndexE );
#endif
							CString sLogOut;
							sLogOut.Format(GetResString(IDS_FINISHED_BLOCK),m_pBlockRangeToDo->m_iBlockIndexS,m_pBlockRangeToDo->m_iBlockIndexE);
							AddPeerLog(new CTraceInformation(sLogOut));
							m_pBlockRangeToDo->m_bFinished = true;
							m_pBlockRangeToDo = NULL;
						}
					}

					delete cur_block->block;
					delete cur_block;
					m_PendingBlocks_list.RemoveAt(posLast);
					
					if( m_pBlockRangeToDo && m_pBlockRangeToDo->m_bFinished )
					{
						ASSERT( m_PendingBlocks_list.IsEmpty() );
						ClearDownloadBlockRequests();
					}

					if (m_PendingBlocks_list.IsEmpty())
					{
						if (thePrefs.GetDebugClientTCPLevel() > 0)
						{
							DebugSend("More block requests", this);
						}

						int iBlockRequestToGet = max(min(GetDownloadDatarate()*10,EMBLOCKSIZE*10),uSize-lenWrittenTotal)/EMBLOCKSIZE +1;
						CreateBlockRequests(iBlockRequestToGet);	//PARTSIZE / EMBLOCKSIZE+1
					}

					if( !m_PendingBlocks_list.IsEmpty() )
					{
						cur_block = m_PendingBlocks_list.GetHead();
						if( cur_block )
						{
							if( cur_block->block->EndOffset < m_nUrlStartPos || m_nUrlStartPos < cur_block->block->StartOffset ) //跳点
							{
								CloseDataSocket();
								if( socket==NULL )
									return;
								CFtpClientReqSocket* reqSocket =  DYNAMIC_DOWNCAST(CFtpClientReqSocket,socket);
								reqSocket->SetFtpState(ftpRETR_PASVPORT); 
								reqSocket->SendCommand(); 		
								return;
							}
						}
					}
					
				}//if 填充block完成
			}
			
			return;
		}
		else //跳点,和http类似,但ftp只需要断开DataSocket并重连,然后reqSocket重发Rest
		{
			CloseDataSocket();
			if( socket==NULL )
				return;
			CFtpClientReqSocket* reqSocket =  DYNAMIC_DOWNCAST(CFtpClientReqSocket,socket);
			reqSocket->SetFtpState(ftpClose); 
			return;
			//reqSocket->SendCommand(); 			
		}
	}//end for


#ifdef _DEBUG_PEER
	TRACE("%s - Dropping packet,lenwritenTotal/uSize:(%d/%d)-Peer(%d)\n", __FUNCTION__,lenWrittenTotal,uSize,m_iPeerIndex);
#endif
	
}

void CFtpClient::SendCancelTransfer(Packet* /*packet*/)
{
	if (socket)
	{
		socket->Safe_Delete();
	}

	if( m_pFtpDataSocket )
	{
		delete m_pFtpDataSocket;
		m_pFtpDataSocket = NULL;
	}
}

void CFtpClient::CheckDownloadTimeout()
{

}

bool CFtpClient::OnFileSizeKnown( uint64 uiFileSize )
{
	if (reqfile == NULL)
	{
		throw CString(_T("Failed to process received HTTP data block - No 'reqfile' attached"));
	}

	m_uiFileSize = uiFileSize;

	CString sLogOut;
	sLogOut.Format(GetResString(IDS_GET_FILESIZE),m_uiFileSize);
	AddPeerLog(new CTraceInformation(sLogOut));

	if ( reqfile->GetPartFileSizeStatus()==FS_KNOWN ) //和下载任务已知大小不一致,该Peer无效
	{ 
		if( reqfile->GetFileSize()>(uint64)0 && m_uiFileSize!=reqfile->GetFileSize() )
		{
			SetCompletePartStatus(false);
			CString strError;
			strError.Format(_T("This peer fileSize is not OK: =%i64"), m_uiFileSize);
			AddPeerLog(new CTraceInformation(sLogOut));
			throw strError;
		}		
		else // this peer filesize is ok :)
		{
			SetCompletePartStatus();
			if( reqfile->m_BlockRangeList.IsEmpty() )
				reqfile->SplitFileToBlockRange();
		}
	}
	else if( reqfile->GetPartFileSizeStatus()==FS_UNKNOWN ) //reqfile->GetFileSize()==(uint64)0
	{						
		if( m_uiFileSize>0 )
		{			
			reqfile->OnGetFileSizeFromInetPeer( m_uiFileSize );// VC-Huby[2007-07-30]:通知任务层
			// VC-Huby[2007-07-30]:set part property of this ftp peer directly
			SetCompletePartStatus();		
		}
		else //虽然知道大小,但确实为零
		{			
			return false;
		}			
	}
	
	reqfile->UpdatePartsInfo();

	return true;
}

CFtpClientDataSocket* CFtpClient::GetClientDataSocket( bool bCreateIfNull )
{
	if( m_pFtpDataSocket==NULL && bCreateIfNull )
	{		
		m_pFtpDataSocket = new CFtpClientDataSocket(this);		
	}

	return m_pFtpDataSocket;
}

CFtpClientReqSocket*  CFtpClient::GetClientReqSocket( )
{
	if( socket )
		return DYNAMIC_DOWNCAST(CFtpClientReqSocket,socket);
	else
		return NULL;
}

bool CFtpClient::GetUrlStartPosForReq( uint64& uiUrlStartPos )
{
	//对于ftp peer,一定是已经商量好了 filesize,然后获取BlockJob才之知道准确的数据请求位置..
	ASSERT( reqfile->GetPartFileSizeStatus()==FS_KNOWN && reqfile->GetFileSize()!=(uint64)0 && reqfile->GetPartCount()>0 );
	
	SendFtpBlockRequests( ); ///领取要下载的Block任务
	
	uiUrlStartPos = m_nUrlStartPos;

	return true;
}

void CFtpClient::Pause( )
{
	CUpDownClient::Pause( );

	//socket->Safe_Delete(); //先shutdown,再closesocket(这个在其它地方已经做了)
	if( m_pFtpDataSocket )
	{
		m_pFtpDataSocket->Safe_Delete(); 
		m_pFtpDataSocket = NULL;
	}
}

void CFtpClient::ConnectionEstablished()
{
	SetDownloadState(DS_CONNECTED);
}


/// VC-Huby[2007-08-28] TODO: 其实基本和httpClient一样处理,将来再重构..
void CFtpClient::CreateBlockRequests(int iMaxBlocks)
{
	ASSERT( iMaxBlocks >= 1 /*&& iMaxBlocks <= 3*/ );

	if( reqfile && reqfile->m_BlockRangeList.IsEmpty() )
		return CUpDownClient::CreateBlockRequests( iMaxBlocks );

	if( m_pBlockRangeToDo==NULL )
	{
		reqfile->GetBlockRange( this );
	}

	if( m_pBlockRangeToDo==NULL )
		return;//return CUpDownClient::CreateBlockRequests( iMaxBlocks );

	iMaxBlocks = min( iMaxBlocks,int(m_pBlockRangeToDo->m_iBlockIndexE-m_pBlockRangeToDo->m_iBlockCurrentDoing+1) ); /// m_iBlockCurrentDoing 还没完成的
	ASSERT( iMaxBlocks >= 1 );

	if (m_DownloadBlocks_list.IsEmpty())
	{
		uint16 count;
		if (iMaxBlocks > m_PendingBlocks_list.GetCount())
		{
			count = (uint16)(iMaxBlocks - m_PendingBlocks_list.GetCount());
		}
		else
		{
			count = 0;
		}

		if (count)
		{
			Requested_Block_Struct** toadd = new Requested_Block_Struct*[count]; //不一定能领取到 count 个,有些可能被ed peer已经领走或做完了
			if( reqfile->GetBlockRequestFromBlockRange(this,toadd,&count) )
			{
				for (UINT i = 0; i < count; i++)
					m_DownloadBlocks_list.AddTail(toadd[i]);
			}
			delete[] toadd;		
		}
	}

	while (m_PendingBlocks_list.GetCount() < iMaxBlocks && !m_DownloadBlocks_list.IsEmpty())
	{
		Pending_Block_Struct* pblock = new Pending_Block_Struct;
		pblock->block = m_DownloadBlocks_list.RemoveHead();
		m_PendingBlocks_list.AddTail(pblock);
	}

	if( m_PendingBlocks_list.IsEmpty() ) /// 领不到活干了,说明当前的BlockRange做完了
	{
#ifdef _DEBUG_PEER
		Debug( _T("Peer(%d) Finished BlockRange(%d-%d-%d)-can note get PendingBlocks from it. \n"),m_iPeerIndex,m_pBlockRangeToDo->m_iBlockIndexS,m_pBlockRangeToDo->m_iBlockCurrentDoing,m_pBlockRangeToDo->m_iBlockIndexE);
#endif
		m_pBlockRangeToDo->m_bFinished = true;
		m_pBlockRangeToDo->m_pClient	= NULL;
		m_pBlockRangeToDo = NULL;
	}

#ifdef _DEBUG_PEER
	TRACE( "%s-Peer(%d)-speed(%d),PendingBlocksCount=%d,DownloadBlocksCount=%d.",__FUNCTION__,m_iPeerIndex,GetDownloadDatarate(),m_PendingBlocks_list.GetCount(),m_DownloadBlocks_list.GetCount());
	if( m_pBlockRangeToDo )
		Debug( _T("from BlockRange(%d-%d-%d) "),m_pBlockRangeToDo->m_iBlockIndexS,m_pBlockRangeToDo->m_iBlockCurrentDoing,m_pBlockRangeToDo->m_iBlockIndexE);
	TRACE( "\n" );
#endif

}

void CFtpClient::CloseDataSocket( )
{
	if(m_pFtpDataSocket)
	{
		delete m_pFtpDataSocket;
		m_pFtpDataSocket = NULL;
	}
}

uint32  CFtpClient::GetTimeUntilReask() const
{	
	ASSERT( reqfile );
	
	DWORD dwTickNow = GetTickCount();
	DWORD dwLastAskedtime = GetLastAskedTime(reqfile);
	
	if( (dwTickNow-dwLastAskedtime)<INetFileAskTimeInterval )
		return INetFileAskTimeInterval-(dwTickNow-dwLastAskedtime);
	else
		return 0; //OK,已超时INetFileAskTimeInterval,看看是否有需要下载的任务..		
}

void CFtpClient::OnCmdSocketErr(CStringA strError)
{ 
	if(lastError == strError)
		m_ErrorCount++;
	else
	{
		lastError = strError;
		m_ErrorCount = 0;
	}
	if(m_ErrorCount == 2)
	{
		CFtpClientReqSocket* reqSocket =  DYNAMIC_DOWNCAST(CFtpClientReqSocket,socket);
		reqfile->ConnectManage(this,m_nConnectIP,reqSocket);
	}
	SetCompletePartStatus(false);
}
