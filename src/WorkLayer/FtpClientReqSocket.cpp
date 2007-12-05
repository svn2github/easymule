#include "StdAfx.h"
#include "ftpclientreqsocket.h"

#include "emule.h"
#include "UpDownClient.h"
#include "SafeFile.h"
#include "Packets.h"
#include "ListenSocket.h"
#include "Preferences.h"
#include "Statistics.h"
#include "SourceUrl.h"
#include "Log.h"
#include "FtpClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CFtpClientReqSocket, CClientReqSocket)

CFtpClientReqSocket::CFtpClientReqSocket(CUpDownClient* client) : CClientReqSocket(client)
{
	SetConnectionEncryption(false, NULL, false); 
	m_FtpState = ftpNone;
	m_bPassive = TRUE;

	m_bDecLaunchTimes = FALSE;
	m_bServerErr = FALSE; 
}

CFtpClientReqSocket::~CFtpClientReqSocket(void)
{
	if (client)
		client->socket = 0; //父类 CClientReqSocket析构中暂不处理ftp的Socket,放到这处理
	//Close();
}

inline bool IsEntireResponse(const char* pData,int nLen)
{
	if(nLen<6) 
		return false;

	if( *(pData+nLen-2)!='\r' || *(pData+nLen-1)!='\n' ) 
		return false;

	char *p=strstr(pData,"\r\n");

	if( *(pData+3)==' ' && p-pData+2==nLen ) 
		return true;

	return IsEntireResponse( p+2,nLen-(p-pData+2 ) );
}

void CFtpClientReqSocket::DataReceived(const BYTE* pucData, UINT uSize)
{	
	if( !IsEntireResponse((const char*)pucData,uSize) ) //先检查是否有一个完整HeaderLine回来了..
		return ;

	CStringA strHeader,strDesc;
	strHeader = CStringA(pucData);
	strDesc = strHeader.Mid( 4 );
	strHeader = strHeader.Left(3);

	OnHeaderLine( strHeader,strDesc );
		
/*
	while( uSize>0 && ReadLine( pData,uSize,strLine ) )
	{
		strLine.Trim( " \t\r\n" );

		if ( strLine.GetLength() > 3 )
		{
			CStringA sLastHeader = strLine.Left( 3 ).TrimRight( " \t\r\n" );
			OnHeaderLine( sLastHeader, strLine.Mid( 4 ).TrimLeft( " \t\r\n" ) );
		}
		pData += (strLine.GetLength() + 2);
		uSize -= (strLine.GetLength() + 2);
	}
*/
}

//this function is copy from shareaza
inline bool ParsePASVArgs (const CStringA& args, SOCKADDR_IN& host)
{
	CString strValue (args);
	int begin = strValue.Find ('(');
	int end = strValue.Find (')');

	if (begin == -1 || end == -1 || end - begin < 12)
	{
		return false;
	}

	strValue = strValue.Mid (begin + 1, end - begin - 1);
	ZeroMemory (&host, sizeof (host));
	host.sin_family = AF_INET;
	int d;
	// h1
	d = strValue.Find (',');
	if (d == -1)
		return false;
	host.sin_addr.S_un.S_un_b.s_b1 = (unsigned char) (_tstoi (strValue.Mid (0, d)) & 0xff);
	strValue = strValue.Mid (d + 1);
	// h2
	d = strValue.Find (',');
	if (d == -1)
		return false;
	host.sin_addr.S_un.S_un_b.s_b2 = (unsigned char) (_tstoi (strValue.Mid (0, d)) & 0xff);
	strValue = strValue.Mid (d + 1);
	// h3
	d = strValue.Find (',');
	if (d == -1)
		return false;
	host.sin_addr.S_un.S_un_b.s_b3 = (unsigned char) (_tstoi (strValue.Mid (0, d)) & 0xff);
	strValue = strValue.Mid (d + 1);
	// h4
	d = strValue.Find (',');
	if (d == -1)
		return false;
	host.sin_addr.S_un.S_un_b.s_b4 = (unsigned char) (_tstoi (strValue.Mid (0, d)) & 0xff);
	strValue = strValue.Mid (d + 1);
	// p1
	d = strValue.Find (',');
	if (d == -1)
		return false;
	host.sin_port = (unsigned char) (_tstoi (strValue.Mid (0, d)) & 0xff);
	strValue = strValue.Mid (d + 1);
	// p2
	host.sin_port += (unsigned char) (_tstoi (strValue) & 0xff) * 256;
	return true;
}

inline bool FTPisOK( const CStringA& str )
{
	return ( str.GetLength () == 3 && str [0] == '2' );
}

/// 收到一个headerLine后对应发下一个Command或连接数据通道(  edit code from shareaza )
BOOL CFtpClientReqSocket::OnHeaderLine ( CStringA& strHeader, CStringA& strValue )
{
	TRACE( ">> %s: %s\n", strHeader, strValue );
	if( GetClient() )
	{
		CString strTraceLog;
		strTraceLog.Format(_T("%s %s"),(CString)strHeader,(CString)strValue);
		GetClient()->AddPeerLog(new CTraceServerMessage(strTraceLog));
	}
	if( strHeader[0]=='4' )
	{
		m_bDecLaunchTimes = TRUE;
	}
	else if( strHeader[0]=='5' )
	{
		m_bServerErr = TRUE;
	}
	if( m_bDecLaunchTimes || m_bServerErr )
	{
		(DYNAMIC_DOWNCAST(CFtpClient,GetClient()))->OnCmdSocketErr(strHeader);
		return TRUE;
	}
   
	switch( m_FtpState )
	{
	case ftpConnecting:
		if ( strHeader == "220" ) // Connected
		{		
			m_FtpState = ftpUSER;
			return SendCommand();
		}
		break;

	case ftpUSER:
		if ( strHeader == "331" )	// Access allowed
		{				
			m_FtpState = ftpPASS;
			return SendCommand ();
		} 		
		// Wrong login or other errors
		// 530: This FTP server is anonymous only.
		break;

	case ftpPASS:
		if ( strHeader == "230" )	// Logged in
		{
			m_FtpState = ftpSIZE;  // ftpSIZE_TYPE
			return SendCommand();
		} 		
		break;

	case ftpSIZE_TYPE:
		if ( strHeader == "200" )	// Type I setted
		{
			m_FtpState = ftpSIZE;
			return SendCommand();
		} 		
		break;

	case ftpSIZE:
		if ( strHeader == "213" )	// SIZE reply
		{
			uint64 ui64FileSize = _atoi64( strValue );
			if( (DYNAMIC_DOWNCAST(CFtpClient,GetClient()))->OnFileSizeKnown( ui64FileSize ) ) //can go on 
			{
				m_FtpState = ftpRETR_TYPE;
				return SendCommand();
			}
			else
			{
				m_FtpState = ftpABOR; //< 对应的Peer不参与下载,socket abort 结束!
				return SendCommand();
			}
		} 				
		break;

	case ftpRETR_TYPE:
		if ( strHeader == "200" )	// Type I setted
		{
			m_FtpState = ftpRETR_PASVPORT;
			return SendCommand();
		} 
		break;

	case ftpRETR_PASVPORT:
		if ( strHeader == "227" || strHeader == "200" )	// Entered passive or active mode
		{			
			if ( m_bPassive )
			{
				SOCKADDR_IN host;
				if( !ParsePASVArgs( strValue, host ) )
				{
					// Wrong PASV reply format
					ASSERT( FALSE );
					return FALSE;
				}
				else
				{
					CFtpClientDataSocket *pClientDataSocket = (DYNAMIC_DOWNCAST(CFtpClient,GetClient()))->GetClientDataSocket(false);
					if( NULL!=pClientDataSocket )
						(DYNAMIC_DOWNCAST(CFtpClient,GetClient()))->CloseDataSocket();
					pClientDataSocket = (DYNAMIC_DOWNCAST(CFtpClient,GetClient()))->GetClientDataSocket();
					if( pClientDataSocket->Create() )
					{
						/// 按ftp服务器给的ip地址和端口开始连通数据通道.
						pClientDataSocket->Connect( (SOCKADDR*)&host, sizeof host );
						return TRUE;
					}
					else
					{
						m_FtpState = ftpABOR;
					}
					//return TRUE;
				}
			}
			else
			{
				m_FtpState = ftpABOR;				
			}		
			m_FtpState = ftpRETR_REST;
			return SendCommand();
		} 		
		break;

	case ftpRETR_REST:
		if ( strHeader == "350" )	// Offset setted
		{		
			m_FtpState = ftpRETR;
			return SendCommand (); /// ok,可以开始向ftp服务器请求要下载的文件了
		}
		break;

	case ftpRETR:
		if ( strHeader == "125" || strHeader == "150" )	// Transfer started
		{
			GetClient()->SetDownloadState( DS_DOWNLOADING );
			m_FtpState = ftpDownloading;			
			return TRUE; // Downloading
		} 
		else if ( strHeader == "226" || strHeader == "426" )	// Transfer completed
		{
			m_FtpState = ftpABOR; // Aborting
			return FALSE;
		} 

		break;

	case ftpABOR:
		{
			// Downloading
			//m_FtpState = ftpDownloading;
			//return SendRequest();
		}
		break;
	case ftpDownloading:
		{
			/// 下载中如果没出错，那就是下完了
		}
		break;
	case ftpClose:
		{									
			if( GetClient() && (DYNAMIC_DOWNCAST(CFtpClient,GetClient()))->PendingBlockReqCount()>0 )
			{
				m_FtpState = ftpRETR_PASVPORT;
				SendCommand();
			}			
		}
		break;
	default:
		ASSERT( FALSE ); // Really unexpected errors
	}

	return FALSE;
}

BOOL CFtpClientReqSocket::Connect(SOCKADDR* pSockAddr, int iSockAddrLen)
{
	BOOL bRet = CClientReqSocket::Connect( pSockAddr,iSockAddrLen );
	m_FtpState = ftpConnecting;
	return bRet;
}

void CFtpClientReqSocket::OnConnect(int nErrorCode)
{
	CClientReqSocket::OnConnect(nErrorCode);

	if (GetClient())
		GetClient()->OnSocketConnected(nErrorCode);
}

void CFtpClientReqSocket::OnClose(int nErrorCode)
{
	CClientReqSocket::OnClose(nErrorCode);	
}

BOOL CFtpClientReqSocket::SendCommand( )
{
	CSourceURL &sourceUrl = (DYNAMIC_DOWNCAST(CFtpClient,GetClient()))->m_SourceURL;
	CStringA strLine;

	switch( m_FtpState )
	{
	case ftpUSER:// Sending login
		{			
			strLine = "USER ";
			strLine += sourceUrl.m_sLogin;
			break;
		}
	case ftpPASS:// Sending password
		{			
			strLine = "PASS ";
			strLine += sourceUrl.m_sPassword;
			break;
		}
	case ftpLIST_PASVPORT:// Selecting passive or active mode
		{			
			if ( m_bPassive )
			{
				strLine = "PASV";
			}
			break;
		}
	case ftpSIZE:// Listing file size
		{
			strLine = "SIZE ";
			strLine += sourceUrl.m_sPath;
			break;
		}
	case ftpSIZE_TYPE:
	case ftpRETR_TYPE:// Selecting BINARY type for transfer
		{			
			strLine = "TYPE I";
			break;
		}
	case ftpRETR_PASVPORT:
		{
			// Selecting passive or active mode
			if ( m_bPassive )
			{
				strLine = "PASV";
			}
			break;
		}
	case ftpRETR_REST: /// 注意断点续传 
		{			
			uint64 uiStartPos;
			(DYNAMIC_DOWNCAST(CFtpClient,GetClient()))->GetUrlStartPosForReq( uiStartPos ); //< 通知Peer层获取下载任务,并计算请求起始点
			strLine.Format( "REST %I64u", uiStartPos );
			break;
		}
	case ftpRETR: // Retrieving file
		{			
			strLine = _T("RETR ");
			strLine += sourceUrl.m_sPath;
			break;
		}
	case ftpABOR:// Transfer aborting
		{			
			strLine = _T("ABOR");
			break;
		}
	default:
		return TRUE;
	}
	
	if( GetClient() )
		GetClient()->AddPeerLog(new CTraceSendMessage((CString)strLine));

	strLine  += "\r\n";

	TRACE( "<< %s",  (LPCSTR) strLine );

/*
	char buffer[1024];

	// Convert Unicode String to MutiBytes String
	int nMulti = WideCharToMultiByte( CP_ACP, 0, (LPCWSTR)strLine.GetBuffer(), strLine.GetLength(), NULL, 0 , NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, strLine.GetBuffer(), strLine.GetLength(), buffer, nMulti, NULL, NULL);
	strLine.ReleaseBuffer();
*/

	CRawPacket* pFtpCMDPacket = new CRawPacket(strLine);
	theStats.AddUpDataOverheadFileRequest(pFtpCMDPacket->size);
	SendPacket(pFtpCMDPacket);

	return TRUE;
}

/// 限速时,ftp命令通道不应该限速，应该限的是数据通道..
void CFtpClientReqSocket::SetDownloadLimit(uint32 limit)
{
	CFtpClient *pFtpClient = DYNAMIC_DOWNCAST(CFtpClient,GetClient());

	if( pFtpClient!=NULL )
	{
		CFtpClientDataSocket *pDataSocket = pFtpClient->GetClientDataSocket( false );
		if( pDataSocket!=NULL )
			pDataSocket->SetDownloadLimit( limit );
	}
}

UINT CFtpClientReqSocket::GetTimeOut()
{
	if( m_FtpState!=ftpDownloading )
	{
		return CEMSocket::GetTimeOut();
	}
	else
	{
		return 4*CEMSocket::GetTimeOut();
	}
}

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CFtpClientDataSocket, CClientReqSocket)

CFtpClientDataSocket::CFtpClientDataSocket(CUpDownClient* in_client) : CClientReqSocket(NULL)
{	
	client = in_client; //can not call SetClient here!!
	SetConnectionEncryption(false, NULL, false); // just to make sure - disable protocol encryption explicit
}

CFtpClientDataSocket::~CFtpClientDataSocket(void)
{
	Close();
}

void CFtpClientDataSocket::DataReceived(const BYTE* pucData, UINT uSize)
{
	try
	{
		GetClient()->ProcessRawData(pucData, uSize);
	}
	catch( ... )
	{
	}
}

void CFtpClientDataSocket::OnClose(int)
{
	if( 0 == GetClient() )
		return;

	CFtpClientReqSocket* reqSocket = (DYNAMIC_DOWNCAST(CFtpClient,GetClient()))->GetClientReqSocket();
	if( reqSocket==NULL )
		return;

	reqSocket->SetFtpState(ftpClose);
}

/// 数据通道连通之后,命令通道就可以开始发“数据传输”相关命令了..
void CFtpClientDataSocket::OnConnect(int nErrorCode)
{
	CFtpClientReqSocket* reqSocket = (DYNAMIC_DOWNCAST(CFtpClient,GetClient()))->GetClientReqSocket();
	if( reqSocket==NULL )
		return;

	if (nErrorCode == 0)
	{
		reqSocket->SetFtpState(ftpRETR_REST); //< 通知命令通道可以开始找断点续传的位置了,但还要看服务器是否支持		
	}
	else
	{
		reqSocket->SetFtpState(ftpABOR);		
	}

	reqSocket->SendCommand();

	SetTimeOut( 4*CONNECTION_TIMEOUT );
}
