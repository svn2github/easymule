//this file is part of eMule
//Copyright (C)2002-2006 Merkur ( strEmail.Format("%s@%s", "devteam", "emule-project.net") / http://www.emule-project.net )
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#include "stdafx.h"
#include <wininet.h>
#include "HttpClient.h"
#include "PartFile.h"
#include "Packets.h"
#include "ListenSocket.h"
#include "HttpClientReqSocket.h"
#include "Preferences.h"
#include "OtherFunctions.h"
#include "Statistics.h"
#include "ClientCredits.h"
#include "StringConversion.h"

#include "DNSManager.h"
#include "GlobalVariable.h"
#include "emule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


///////////////////////////////////////////////////////////////////////////////
// CHttpClient

IMPLEMENT_DYNAMIC(CHttpClient, CUpDownClient)

CHttpClient::CHttpClient()
{
	m_nRangeOffset = 0;
	m_iRedirected  = 0;
	m_clientSoft   = SO_URL;

	m_bHeadResponsed = False;
	m_uiFileSize	 = 0;

	m_uReqStart		=0; 
	m_uReqEnd		=0;

	m_bDataTransfering = FALSE;
	m_bKnownSize	= FALSE;
	m_bFirstHeader	= TRUE;

	m_iPeerType = ptHttp;
	m_iUrlEncodeTypeToUse = UET_NOENCODE;
	m_iUrlEncodeTypeSucced = UET_NONE;
	m_ErrorCount = 0;
	lastError = 0;
	bAddOtherSources = true;
}

void CHttpClient::SetRequestFile(CPartFile* pReqFile)
{
	CUpDownClient::SetRequestFile(pReqFile);
	
	if ( reqfile && reqfile->GetPartFileSizeStatus()==FS_KNOWN )
	{
		m_nPartCount = reqfile->GetPartCount();
		if(NULL==m_abyPartStatus)
			m_abyPartStatus = new uint8[m_nPartCount];
		memset(m_abyPartStatus, 0, m_nPartCount);
		m_bCompleteSource = false;
	}
}

// pszUrl - 不一定已经做了utf-8编码的http url
bool CHttpClient::SetUrl(LPCTSTR pszUrl, uint32 nIP)
{
	m_strURL = pszUrl;
	m_nIP = nIP;
	m_strURL = CString(pszUrl); // Add URL Record
	if( _tcsrchr( pszUrl+7,_T('/') )==NULL )
		m_strURL += _T("/");
	
	if( m_strURL.Find(_T('<'))>0 )
	{
		CString strExtraInfo;
		strExtraInfo = m_strURL.Mid( m_strURL.Find(_T('<'))+1 );
		strExtraInfo = strExtraInfo.Left( strExtraInfo.GetLength()-1 );		
		m_strRefer = strExtraInfo.Mid( strExtraInfo.Find(_T('='))+1 ); //先暂时认为<>中只有referer=...
		m_strURL = m_strURL.Left( m_strURL.Find(_T('<')) );
	}

	USES_CONVERSION;
/*
	TCHAR szCanonUrl[INTERNET_MAX_URL_LENGTH];
	DWORD dwCanonUrlSize = ARRSIZE(szCanonUrl);
	if (!InternetCanonicalizeUrl(pszUrl, szCanonUrl, &dwCanonUrlSize, ICU_NO_ENCODE))
	{
		return false;
	}

	TCHAR szUrl[INTERNET_MAX_URL_LENGTH];
	DWORD dwUrlSize = ARRSIZE(szUrl);
	if (!InternetCanonicalizeUrl(szCanonUrl, szUrl, &dwUrlSize, ICU_DECODE | ICU_NO_ENCODE | ICU_BROWSER_MODE))
	{
		return false;
	}
*/

	TCHAR szScheme[INTERNET_MAX_SCHEME_LENGTH];
	TCHAR szHostName[INTERNET_MAX_HOST_NAME_LENGTH];
	TCHAR szUrlPath[INTERNET_MAX_PATH_LENGTH];
	TCHAR szUserName[INTERNET_MAX_USER_NAME_LENGTH];
	TCHAR szPassword[INTERNET_MAX_PASSWORD_LENGTH];
	TCHAR szExtraInfo[INTERNET_MAX_URL_LENGTH];
	URL_COMPONENTS Url = {0};
	Url.dwStructSize = sizeof(Url);
	Url.lpszScheme = szScheme;
	Url.dwSchemeLength = ARRSIZE(szScheme);
	Url.lpszHostName = szHostName;
	Url.dwHostNameLength = ARRSIZE(szHostName);
	Url.lpszUserName = szUserName;
	Url.dwUserNameLength = ARRSIZE(szUserName);
	Url.lpszPassword = szPassword;
	Url.dwPasswordLength = ARRSIZE(szPassword);
	Url.lpszUrlPath = szUrlPath;
	Url.dwUrlPathLength = ARRSIZE(szUrlPath);
	Url.lpszExtraInfo = szExtraInfo;
	Url.dwExtraInfoLength = ARRSIZE(szExtraInfo);
	
	if (!InternetCrackUrl(m_strURL, 0, 0, &Url))
	{
		return false;
	}

	if (Url.dwSchemeLength == 0 || Url.nScheme != INTERNET_SCHEME_HTTP)		// we only support "http://"
		return false;
	if (Url.dwHostNameLength == 0)			// we must know the hostname
		return false;
	if (Url.dwUserNameLength != 0)			// no support for user/password
		return false;
	if (Url.dwPasswordLength != 0)			// no support for user/password
		return false;
	if (Url.dwUrlPathLength == 0)			// we must know the URL path on that host
		return false;

	m_strHostA = szHostName;

/*
	TCHAR szEncodedUrl[INTERNET_MAX_URL_LENGTH];
	DWORD dwEncodedUrl = ARRSIZE(szEncodedUrl);
	if (!InternetCanonicalizeUrl(pszUrl, szEncodedUrl, &dwEncodedUrl, ICU_ENCODE_PERCENT))
	{
		return false;
	}
*/

	//CString strUrlPathEncoded = EncodeUrlUtf8( CString(Url.lpszUrlPath) + CString(Url.lpszExtraInfo) );
	m_strUrlPath = CString(Url.lpszUrlPath) + CString(Url.lpszExtraInfo);//CStringA(strUrlPathEncoded);	
	m_strUrlPath.Trim();

	m_nUrlStartPos = (uint64)-1;

	//NOTE: be very careful with what is stored in the following IP/ID/Port members!
	if (nIP)
	{
		m_nConnectIP = nIP;
		m_dwUserIP	 = nIP;
		ResetIP2Country();
	}
	else
	{
		m_nConnectIP = inet_addr(T2A(szHostName));
	}

	m_nUserIDHybrid = htonl(m_nConnectIP);
	ASSERT( m_nUserIDHybrid != 0 );
	m_nUserPort = Url.nPort;

	CString sUserName;
#ifdef _DEBUG_PEER
	sUserName.Format( _T("%s(%d)"),ipstr(nIP),m_iPeerIndex);
#else
	sUserName = ipstr( nIP );
#endif
	SetUserName( sUserName );

	return true;
}

CHttpClient::~CHttpClient()
{
	
}

bool CHttpClient::SendHttpBlockRequests()
{	
	USES_CONVERSION;	
	m_dwLastBlockReceived = ::GetTickCount();	
	if (reqfile == NULL)
	{
		throw CString(_T("Failed to send block requests - No 'reqfile' attached"));
	}

	CStringA strHttpRequest;

	if( reqfile->GetPartFileSizeStatus()==FS_KNOWN && reqfile->GetFileSize()!=(uint64)0 && reqfile->GetPartCount()>0 )
	{
		if (m_PendingBlocks_list.IsEmpty())
		{
			while( reqfile->GetBlockRange( this ) )
			{
				CreateBlockRequests(1);	//PARTSIZE / EMBLOCKSIZE + 1
				if( !m_PendingBlocks_list.IsEmpty() )
					break;
			}			
		}
		
		if (m_PendingBlocks_list.IsEmpty())
		{
			SetDownloadState(DS_NONEEDEDPARTS);		
			return false;
		}
		else
		{	
			EnsureContinueBlockRequest();
			m_nUrlStartPos = m_uReqStart;
		}
		
		ASSERT( (m_uReqEnd-m_uReqStart)>0 );

		FormatRequestHeader( strHttpRequest,NULL,NULL,m_uReqStart,0); //ToDo-HUBY
	}
	else
	{
		FormatRequestHeader( strHttpRequest,NULL,NULL,0,0 ); //第一次,从头位置发请求,因为还不知道大小
		m_nUrlStartPos = 0;
	}
 
	CRawPacket* pHttpPacket = new CRawPacket(strHttpRequest);
	
	theStats.AddUpDataOverheadFileRequest(pHttpPacket->size);
	
	socket->SendPacket(pHttpPacket);

	STATIC_DOWNCAST(CHttpClientDownSocket, socket)->SetHttpState(HttpStateRecvExpected);

#ifdef _DEBUG_PEER
	Debug( _T("Peer(%d)-pt:(%d),PendingBlocks(%d),FormatRequestHeader:%-24hs \n"),m_iPeerIndex,m_iPeerType,m_PendingBlocks_list.GetCount(),(LPCSTR)strHttpRequest );
#endif

	return true;
}

bool CHttpClient::TryToConnect( bool /*bIgnoreMaxCon*/, CRuntimeClass* /*pClassSocket*/ )
{
	if( reqfile==NULL )
		return false;

	// 后加入的Peer可以预先领取一遍干活任务,领不到活就不连接
	if( reqfile->GetPartFileSizeStatus()==FS_KNOWN && reqfile->GetFileSize()!=(uint64)0 && reqfile->GetPartCount()>0 )
	{
/*
		if( !m_bCompleteSource )
			SetCompletePartStatus(); /// 先假定该Peer已经有完整数据*/


		if (m_PendingBlocks_list.IsEmpty())
		{
			while( reqfile->GetBlockRange( this ) )
			{
				CreateBlockRequests(1);	//
				if( !m_PendingBlocks_list.IsEmpty() )
					break;
			}		
		}

		if (m_PendingBlocks_list.IsEmpty())
		{
			SetDownloadState(DS_NONEEDEDPARTS);		
			return false;
		}
	}
	else
	{
		reqfile->m_dwTickGetFileSize = GetTickCount();
	}
	
	//////////////////////////////////////////////////////////////////////////

	if (!socket || !socket->IsConnected())
	{
		if (socket)
			socket->Safe_Delete();

		socket = static_cast<CClientReqSocket*>(RUNTIME_CLASS(CHttpClientDownSocket)->CreateObject());
		socket->SetClient(this);

		if (!socket->Create())
		{
			socket->Safe_Delete();
			return true;
		}
	}
	else
	{
		return true;
	}

	m_nDownloadState = DS_CONNECTING;
	return Connect();		
}

bool CHttpClient::Connect()
{
	if (GetConnectIP() != 0 && GetConnectIP() != INADDR_NONE)
	{

		CString temp;
		temp.Format(GetResString(IDS_CONNECT_INFOMATION),(CString)m_strHostA, m_nUserPort);
		AddPeerLog(new CTraceInformation(temp));
/*
		temp.Format(_T("Connecting %s [IP=%s:%d]"), (CString)m_strHostA, ipstr(GetConnectIP()), m_nUserPort);
		AddPeerLog(new CTraceInformation(temp));
*/

		return CUpDownClient::Connect();
	}

	//Try to always tell the socket to WaitForOnConnect before you call Connect.
	socket->WaitForOnConnect();
	socket->Connect(m_strHostA, m_nUserPort);
	return true;
}

void CHttpClient::OnSocketConnected(int nErrorCode)
{
	m_bFirstHeader = TRUE;

	if (nErrorCode == 0)
	{
		AddPeerLog(new CTraceInformation(GetResString(IDS_CONNECTED)));
		SetDownloadState( DS_CONNECTED );
		SendHttpBlockRequests();
	}
	else 
	{
		SetDownloadState(DS_ERROR);
	}
}

bool CHttpClient::SendHelloPacket()
{
	//SendHttpBlockRequests();
	return true;
}

bool CHttpClient::Disconnected(LPCTSTR pszReason, bool bFromSocket, CClientReqSocket* /*pSocket*/)
{
#ifdef _DEBUG_PEER
	Debug( _T("Peer(%d)-Http Disconnected because-%s \n"),m_iPeerIndex,pszReason );
#endif	

	CHttpClientDownSocket* s = STATIC_DOWNCAST(CHttpClientDownSocket, socket);

	//TRACE(_T("%hs: HttpState=%u, Reason=%s\n"), __FUNCTION__, s==NULL ? -1 : s->GetHttpState(), pszReason);
	// TODO: This is a mess..
	if (s && (s->GetHttpState() == HttpStateRecvExpected || s->GetHttpState() == HttpStateRecvBody))
	{
		m_fileReaskTimes.RemoveKey(reqfile); // ZZ:DownloadManager (one resk timestamp for each file)
	}

	ASSERT( CGlobalVariable::clientlist->IsValidClient(this) );	

	if ( GetDownloadState() == DS_DOWNLOADING )
	{	
		SetDownloadState( DS_ERROR, CString(_T("Disconnected: ")) + pszReason);		
		m_iErrTimes++;
	}
	else if( GetDownloadState() == DS_REDIRECTED )
	{
	}
	else
	{
		if( m_PendingBlocks_list.GetCount()==0 && reqfile->GetPartFileSizeStatus()==FS_KNOWN )
		{		
			SetDownloadState( DS_NONEEDEDPARTS );
		}
		else
		{
			SetDownloadState( DS_ERROR );
			m_iErrTimes++;
			// ensure that all possible block requests are removed from the partfile
			ClearDownloadBlockRequests();
		}		
	}

	if (!bFromSocket && socket)
	{
		ASSERT( CGlobalVariable::listensocket->IsValidSocket(socket) );
		socket->Safe_Delete();
	}

	socket = NULL;

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

bool CHttpClient::ProcessHttpDownResponse(const CStringAArray& astrHeaders)
{
	if (reqfile == NULL)
	{   
		AddPeerLog(new CTraceError(_T("Failed to process received HTTP data block - No 'reqfile' attached")));
		throw CString(_T("Failed to process received HTTP data block - No 'reqfile' attached"));
	}
	
	try
	{
		if (astrHeaders.GetCount() == 0)
		{
			AddPeerLog(new CTraceError(_T("Unexpected HTTP response - No headers available")));
			throw CString(_T("Unexpected HTTP response - No headers available"));
		}

		const CStringA& rstrHdr = astrHeaders.GetAt(0);
		AddPeerLog(new CTraceServerMessage((CString)rstrHdr));

#ifdef _DEBUG_PEER
		TRACE( "Peer(%d) responsed:%s \n",m_iPeerIndex,rstrHdr );
#endif

		UINT uHttpMajVer, uHttpMinVer, uHttpStatusCode;
		if (sscanf(rstrHdr, "HTTP/%u.%u %u", &uHttpMajVer, &uHttpMinVer, &uHttpStatusCode) != 3)
		{
			CString strError;
			strError.Format(_T("Unexpected HTTP response: \"%hs\""), rstrHdr);
			AddPeerLog(new CTraceError((CString)strError));

			throw strError;
		}

		if (uHttpMajVer != 1 || (uHttpMinVer != 0 && uHttpMinVer != 1))
		{
			CString strError;
			strError.Format(_T("Unexpected HTTP version: \"%hs\""), rstrHdr);
			AddPeerLog(new CTraceError((CString)strError));

			throw strError;
		}
        if(uHttpStatusCode >= 400)
		{
             if(lastError == uHttpStatusCode)
			        m_ErrorCount++;
			 else
			 {
				 lastError = uHttpStatusCode;
				 m_ErrorCount = 0;
			 }
			 if(m_ErrorCount == 2)
			 {   
				 CHttpClientDownSocket* s = STATIC_DOWNCAST(CHttpClientDownSocket, socket);
				 reqfile->ConnectManage(this,m_nConnectIP,s);
				 m_ErrorCount = 0;
			 }
		}
		if( uHttpStatusCode==HTTP_STATUS_NOT_FOUND )
		{
			if( m_iUrlEncodeTypeToUse==UET_NOENCODE )
				m_iUrlEncodeTypeToUse = UET_UTF8;
			else if( m_iUrlEncodeTypeToUse==UET_UTF8 )
				m_iUrlEncodeTypeToUse=UET_UTF16;
			else if( m_iUrlEncodeTypeToUse==UET_UTF16 )
				m_iUrlEncodeTypeToUse = UET_NOENCODE;
		}
		else if( m_iUrlEncodeTypeSucced==UET_NONE )
		{
 			m_iUrlEncodeTypeSucced = m_iUrlEncodeTypeToUse;
			if( m_iUrlEncodeTypeToUse!=UET_NOENCODE )
				reqfile->m_UrlEncodeTypeMap.SetAt(m_strURL,m_iUrlEncodeTypeSucced); /// 方便其它同Url的Peer
		}

		bool bExpectData  = uHttpStatusCode == HTTP_STATUS_OK || uHttpStatusCode == HTTP_STATUS_PARTIAL_CONTENT;
		bool bRedirection = uHttpStatusCode == HTTP_STATUS_MOVED || uHttpStatusCode == HTTP_STATUS_REDIRECT;
		if (!bExpectData && !bRedirection)
		{
			CString strError;
			strError.Format(_T("Unexpected HTTP status code \"%u\""), uHttpStatusCode);
			//AddPeerLog(new CTraceError((CString)strError));
			SetCompletePartStatus( false );
			throw strError;
		}

		bool bNewLocation = false;
		bool bValidContentRange = false;
		uint64 ui64ContentLength = 0;

		for (int i = 1; i < astrHeaders.GetCount(); i++)
		{
			const CStringA& rstrHdr = astrHeaders.GetAt(i);
			AddPeerLog(new CTraceServerMessage((CString)rstrHdr));
#ifdef _DEBUG_PEER
			TRACE( rstrHdr );
			TRACE( "\n" );
#endif
			if (bExpectData && strnicmp(rstrHdr, "Content-Range:", 14) == 0)//非0开始的请求回应
			{
				uint64 ui64Start = 0, ui64End = 0;

				if (sscanf((LPCSTR)rstrHdr + 14," bytes %I64u - %I64u / %I64u", &ui64Start, &ui64End, &m_uiFileSize) != 3)
				{
					CString strError;
					strError.Format(_T("Unexpected HTTP header field \"%hs\""), rstrHdr);
					AddPeerLog(new CTraceError((CString)strError));

					throw strError;
				}
				if ( ( ui64Start!=m_uReqStart || ui64End!=m_uReqEnd || m_uiFileSize!=reqfile->GetFileSize() ) 
					&& (uHttpStatusCode!=HTTP_STATUS_PARTIAL_CONTENT) )				
				{
					CString strError;
					strError.Format(_T("Unexpected HTTP header field \"%hs\""), rstrHdr);
					AddPeerLog(new CTraceError((CString)strError));

					throw strError;
				}
				bValidContentRange = true;
			}
			else if (bExpectData && strnicmp(rstrHdr, "Content-Length:", 15) == 0)//从0开始的请求回应
			{
				m_bKnownSize = TRUE;
				ui64ContentLength = _atoi64((LPCSTR)rstrHdr + 15);			
			} 
			else if (strnicmp(rstrHdr, "Server:", 7) == 0)
			{
				if (m_strClientSoftware.IsEmpty())
				{
					m_strClientSoftware = rstrHdr.Mid(7).Trim();
				}
			}
			else if ( strnicmp(rstrHdr, "Content-Disposition:", 20) == 0 )
			{
				CString sDispParam = CString(rstrHdr.Mid(20).Trim()) + _T(";");
				CString sParamItem;				
				while( sDispParam.Find(_T(';'))>0 ) //遍历查找服务端给的 filename
				{
					sParamItem = sDispParam.Left( sDispParam.Find(_T(';')) ).Trim();
					if( sParamItem.Find(_T('='))>0 )
					{
						if( sParamItem.Left(sParamItem.Find(_T('=')))==_T("filename") )
						{
							CString sFileName = sParamItem.Mid(sParamItem.Find(_T('='))+1);
							if( sFileName.Left(1)==_T("\"") || sFileName.Left(1)==_T("\'") )
							{
								sFileName = sFileName.Left( sFileName.GetLength()-1 );
								sFileName = sFileName.Right( sFileName.GetLength()-1 );
							}
							reqfile->SetFileName( sFileName );
						}
					}
					sDispParam = sDispParam.Mid( sDispParam.Find(_T(';'))+1 );
					sDispParam.Trim();
				}
			}
			else if (bRedirection && strnicmp(rstrHdr, "Location:", 9) == 0)
			{
				CString strLocation(rstrHdr.Mid(9).Trim());

				// VC-SearchDream[2007-07-23]: the following condition need DNS
				if ((strLocation.Left(7).CompareNoCase(_T("http://")) == 0) || (strLocation.Left(6).CompareNoCase(_T("ftp://")) == 0))
				{
#ifdef _DEBUG_PEER
					TRACE( _T("Peer(%d) redirected to %s,now addToDNS.\n"),m_iPeerIndex,strLocation);
#endif
					CString sLogOut;
					sLogOut.Format(GetResString(IDS_REDIRECT),strLocation);
					AddPeerLog(new CTraceInformation((CString)sLogOut));
					if( CGlobalVariable::m_DNSManager )
						CGlobalVariable::m_DNSManager->AddUrlToDNS(strLocation, reqfile);
					//reqfile->SetPartFileURL( strLocation );	//COMMENTED by VC-fengwen 2007/08/16 : 保留源下载地址作唯一标识
					//以下得出新文件名
					CString sUrlShort = strLocation.Right( strLocation.GetLength() - strLocation.Find(_T("://")) -3 );
					CString strFileName;
					int iIndex = sUrlShort.ReverseFind(_T('/'));
					if( iIndex==-1 )
					{
						strFileName = _T("Index.htm");
					}
					else
					{
						strFileName = sUrlShort.Right( sUrlShort.GetLength() - 1 - iIndex );
						if( strFileName.Find(_T('?'))>0 )
							strFileName = strFileName.Left( strFileName.Find(_T('?')) );
						if( strFileName.Find(_T('#'))>0 )
							strFileName = strFileName.Left( strFileName.Find(_T('#')) );
						if (strFileName.IsEmpty())
							strFileName = _T("Index.htm");
					}	
					reqfile->SetFileName( strFileName );
					SetDownloadState( DS_REDIRECTED );
					Disconnected(_T("This url of this httpClient has been redirected!"));
					//return false;
				}
				/*

				if (!SetUrl(strLocation))
				{
				CString strError;
				strError.Format(_T("Failed to process HTTP redirection URL \"%s\""), strLocation);
				throw strError;
				}
				*/

				bNewLocation = true;
			}
		}//for

		if (bNewLocation)
		{
			m_bDataTransfering = FALSE;

			m_iRedirected++;
			if (m_iRedirected >= 3)
			{
				AddPeerLog(new CTraceError(_T("Max. HTTP redirection count exceeded")));
				throw CString(_T("Max. HTTP redirection count exceeded"));
			}

			return false;				// tell our old parent object (which was marked as to get deleted 
			// and which is no longer attached to us) to disconnect.
		}
		else
		{
			m_bDataTransfering = TRUE;

			if( m_uiFileSize==0 )
				m_uiFileSize = ui64ContentLength; //"Content-Range:"中如果没有拿到FileSize,那么就靠"Content-Length:"决定

			CString sLogOut;	
			if ( reqfile->GetPartFileSizeStatus()==FS_KNOWN && reqfile->GetFileSize()>(uint64)0  ) //和下载任务已知大小不一致,该Peer无效
			{ 				
				if( m_uiFileSize!=reqfile->GetFileSize() )
				{
					CString sLogOut;
					sLogOut.Format( _T("This peer FileSize is not OK: =%i64"),m_uiFileSize );
					AddPeerLog(new CTraceError((CString)sLogOut));

					throw sLogOut;
				}				
				else
				{
					SetCompletePartStatus( );
					if( reqfile->m_BlockRangeList.IsEmpty() )
						reqfile->SplitFileToBlockRange();
				}
			}
			else if( reqfile->GetPartFileSizeStatus()==FS_UNKNOWN ) //reqfile->GetFileSize()==(uint64)0
			{				
				//ADDED by VC-fengwen 2007/08/01 <bein> : 当此时还不知道文件大小时，则以“无大小”初始PartFile
				if( !m_bKnownSize )
				{
				    AddPeerLog(new CTraceError(GetResString(IDS_UNKNOWN_FILE)));
					reqfile->InitNosizePartFile();
				}			
				//ADDED by VC-fengwen 2007/08/01 <end> : 当此时还不知道文件大小时，则以“无大小”初始PartFile
				else if( m_uiFileSize>0 )
				{
					sLogOut.Format(GetResString(IDS_GET_FILESIZE),m_uiFileSize);
					AddPeerLog(new CTraceInformation(sLogOut));
					reqfile->OnGetFileSizeFromInetPeer( m_uiFileSize );// VC-Huby[2007-07-30]:通知任务层
					// VC-Huby[2007-07-30]:set part property of this http peer directly
					SetCompletePartStatus( );
					CreateBlockRequests(1);	//PARTSIZE / EMBLOCKSIZE+1
				}
				else //虽然知道大小,但确实为零
				{	
					AddPeerLog(new CTraceInformation(GetResString(IDS_ZERO_INFO)));
					Disconnected(_T("Filesize of this httpClient url is zero!"));					
					SetDownloadState(DS_NONEEDEDPARTS);
					reqfile->ZeroSize_CompleteDownLoad();
					return false;
				}			
			}

			if (m_bFirstHeader && !m_bKnownSize)
			{
				m_nRangeOffset = 0;
				m_nTransferredDown = 0;
			}

		}

		if (!bExpectData
			&& !bValidContentRange
			&& uHttpStatusCode != HTTP_STATUS_NOT_FOUND)
		{
			//m_latishConnectRetry.Start(30000, (WPARAM) this);
			return true;
		}

		SetDownloadState(DS_DOWNLOADING);	
		reqfile->UpdatePartsInfo();

		m_bFirstHeader = FALSE;

		return true;
	}
	catch ( ... )
	{
		//m_iErrTimes++;
		SetDownloadState( DS_ERROR );
		Disconnected( _T(" HttpDown Repsponse Err!") );
		//AddPeerLog(new CTraceError(_T("HttpDown Response Err!")));
		return false;
	}	
}

void CHttpClient::ProcessNoSizeRawData(const BYTE * pucData, UINT uSize)
{
	if (reqfile->IsStopped() || (reqfile->GetStatus() != PS_READY && reqfile->GetStatus() != PS_EMPTY))
	{
		throw CString(_T("Failed to process HTTP data block - File not ready for receiving data"));
	}

	if (!(GetDownloadState() == DS_DOWNLOADING || GetDownloadState() == DS_NONEEDEDPARTS))
	{
		throw CString(_T("Failed to process HTTP data block - Invalid download state"));
	}

	m_dwLastBlockReceived = ::GetTickCount();

	thePrefs.Add2SessionTransferData(GetClientSoft(), (GetClientSoft()==SO_URL) ? (UINT)-2 : (UINT)-1, false, false, uSize);
	m_nDownDataRateMS += uSize;


	uint32 lenWritten = reqfile->NoSize_WriteToBuffer(uSize, pucData, m_nRangeOffset, m_nRangeOffset + uSize - 1, NULL, this);
	m_nRangeOffset += uSize;

	if (lenWritten > 0)
	{
		m_nTransferredDown += uSize;
		m_nCurSessionPayloadDown += lenWritten;
		SetTransferredDownMini();
	}
}

void CHttpClient::ProcessRawData(const BYTE * pucData, UINT uSize)
{   
	// VC-wangna[2007-11-26]: Add other connect
	if(bAddOtherSources)//判断是否需要加入其它几个连接
	{
		int i = 1;
		int iMax = thePrefs.GetMaxSourceConnect();
		while(i <iMax)
		{
			CHttpClient* client = new CHttpClient;
			client->bAddOtherSources = false;
			if (!client->SetUrl(m_strURL, m_nIP))
			{
				LogError(LOG_STATUSBAR, _T("Failed to process URL source \"%s\""), m_strURL);
				delete client;
				return;
			}
			if( _tcslen(m_strRefer)>0 )
				client->m_strRefer = m_strRefer;
			client->SetRequestFile(reqfile);
			client->SetSourceFrom(SF_HTTP);
			if (CGlobalVariable::downloadqueue->CheckAndAddSource(reqfile, client))
			{
				reqfile->UpdatePartsInfo();
			}
			i++;
		}
		bAddOtherSources = false;
	}
   // VC-wangna[2007-11-26]: Add other connect

	if (!m_bKnownSize)
	{
		ProcessNoSizeRawData(pucData, uSize);
		return;
	}

	if (reqfile == NULL)
	{
		throw CString(_T("Failed to process HTTP data block - No 'reqfile' attached"));
	}

	if (reqfile->IsStopped() || (reqfile->GetStatus() != PS_READY && reqfile->GetStatus() != PS_EMPTY))
	{
		throw CString(_T("Failed to process HTTP data block - File not ready for receiving data"));
	}

	if (m_nUrlStartPos == (uint64)-1)
	{
		throw CString(_T("Failed to process HTTP data block - Unexpected file data"));
	}

	uint64 nStartPos = m_nUrlStartPos;
	uint64 nEndPos = m_nUrlStartPos + uSize;

	m_nUrlStartPos += uSize;

	if (!(GetDownloadState() == DS_DOWNLOADING || GetDownloadState() == DS_NONEEDEDPARTS))
		throw CString(_T("Failed to process HTTP data block - Invalid download state"));

	m_dwLastBlockReceived = ::GetTickCount();

	if (nEndPos == nStartPos || uSize != nEndPos - nStartPos)
		throw CString(_T("Failed to process HTTP data block - Invalid block start/end offsets"));

	thePrefs.Add2SessionTransferData(GetClientSoft(), (GetClientSoft()==SO_URL) ? (UINT)-2 : (UINT)-1, false, false, uSize);
	m_nDownDataRateMS += uSize;
	if (credits)
		credits->AddDownloaded(uSize, GetIP());
	nEndPos--;
	
/*
	if( m_PendingBlocks_list.IsEmpty() ) //网络数据来了,但此Peer还没领取Block任务
	{	
		int iBlockRequestToGet = max(min(GetDownloadDatarate()*10,EMBLOCKSIZE*10),uSize)/EMBLOCKSIZE +1; //VC-Huby[2007-08-29]: 领取Peer 10s内就可以完成的Block,避免其它Peer打劫后不连续		
		while( reqfile->GetBlockRange( this ) )
		{
			CreateBlockRequests(iBlockRequestToGet);	 //PARTSIZE / EMBLOCKSIZE + 1
			if( !m_PendingBlocks_list.IsEmpty() )
				break;
		}		
	}*/

	/// 优化为每次数据来了后预留一定的BlockRequest给此Peer
	UINT iBlockRequestToGet = max(min(GetDownloadDatarate()*10,EMBLOCKSIZE*10),uSize)/EMBLOCKSIZE +1;
	if( m_pBlockRangeToDo )
		iBlockRequestToGet = min(iBlockRequestToGet,m_pBlockRangeToDo->m_iBlockIndexE-m_pBlockRangeToDo->m_iBlockCurrentDoing+1);
	if( (UINT)m_PendingBlocks_list.GetCount()<iBlockRequestToGet )
	{
		//iBlockRequestToGet = iBlockRequestToGet-m_PendingBlocks_list.GetCount();
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

				if (nEndPos >= cur_block->block->EndOffset)//网络数据把cur_block gap填充完毕
				{
					reqfile->RemoveBlockFromList(cur_block->block->StartOffset, cur_block->block->EndOffset);
#ifdef _DEBUG_PEER
					Debug( _T("Peer(%d)-Http Finished BlockJob(%d):%I64u-%I64u \n"),m_iPeerIndex,cur_block->block->BlockIdx,cur_block->block->StartOffset,cur_block->block->EndOffset );
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
							Debug( _T("Peer(%d)-Http Finished BlockRange(%d-%d-%d)\n"),m_iPeerIndex,m_pBlockRangeToDo->m_iBlockIndexS,m_pBlockRangeToDo->m_iBlockCurrentDoing,m_pBlockRangeToDo->m_iBlockIndexE );
#endif							
							CString sLogOut;
							CString strBlock;
							strBlock.Format(_T("(%d-%d)."),m_pBlockRangeToDo->m_iBlockIndexS,m_pBlockRangeToDo->m_iBlockIndexE);
							sLogOut += GetResString(IDS_FINISHED_BLOCK) + strBlock;
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
							DebugSend("More block requests", this);
						
						int iBlockRequestToGet = max(min(GetDownloadDatarate()*10,EMBLOCKSIZE*10),uSize-lenWrittenTotal)/EMBLOCKSIZE +1;
						CreateBlockRequests(iBlockRequestToGet);	//PARTSIZE / EMBLOCKSIZE+1
					}
				}
			}
			return;
		}
		else
		{
			/// 需断开重连(可能是往前跳了,或是往后跳太大了),跳点重新发http请求
#ifdef _DEBUG_PEER
			Debug( _T("Peer(%d) Need ReConnect,req cur_Block[%d](%I64u-%I64u),but urlPos(%I64u,%I64u) \n"),m_iPeerIndex,
				cur_block->block->BlockIdx,cur_block->block->StartOffset,cur_block->block->EndOffset,nStartPos,nEndPos );
#endif			
			socket->Safe_Delete();
			socket = NULL;
			TryToConnect(); /// 直接立刻在新位置重连
			return;
		}
	}//for

#ifdef _DEBUG_PEER
	TRACE("%s - Dropping packet,lenwritenTotal/uSize:(%d/%d)-Peer(%d)\n", __FUNCTION__,lenWrittenTotal,uSize,m_iPeerIndex);
#endif
}

void CHttpClient::SendCancelTransfer(Packet* /*packet*/)
{
	if (socket)
	{
		STATIC_DOWNCAST(CHttpClientDownSocket, socket)->SetHttpState(HttpStateUnknown);
		socket->Safe_Delete();
	}
}

void CHttpClient::OnNoSizeFileComplete()
{
	if (reqfile != NULL)
	{
		if (m_bDataTransfering && !m_bKnownSize)
		{
			SetDownloadState(DS_NONEEDEDPARTS);
			reqfile->NoSize_CompleteDownLoad();
			CGlobalVariable::filemgr.ModifyURLState(reqfile->GetPartFileURL(), _T(""),FILESTATE_COMPLETED);
		}	
	}
}

void CHttpClient::FormatRequestHeader( CStringA& strHttpRequest,char * /*pCookie*/,char * /*pReferer*/,uint64 uiFrom,uint64 uiTo )
{
	CStringA temp;
	
	URLEncodeType urlEncodeTypeToFormat;
	if( m_iUrlEncodeTypeSucced!=UET_NONE )
	{
		urlEncodeTypeToFormat = m_iUrlEncodeTypeSucced;
	}
	else if(  reqfile->m_UrlEncodeTypeMap.Lookup( m_strURL,urlEncodeTypeToFormat ) ) 
	{
		m_iUrlEncodeTypeSucced = urlEncodeTypeToFormat;
	}
	else
	{
		urlEncodeTypeToFormat = m_iUrlEncodeTypeToUse;
	}

	CStringA strUrlPathToGet;
	if( urlEncodeTypeToFormat==UET_UTF8 )
	{
		strUrlPathToGet = EncodeUrlUtf8(m_strUrlPath);
	}
	else if( urlEncodeTypeToFormat==UET_NOENCODE )
	{
		strUrlPathToGet = m_strUrlPath;
	}
	else if( urlEncodeTypeToFormat==UET_UTF16 )
	{
		strUrlPathToGet = m_strUrlPath; //TODO
	}

	strHttpRequest.AppendFormat("GET %s HTTP/1.1\r\n", strUrlPathToGet);//TODO-0727
	AddPeerLog(new CTraceSendMessage((CString)strHttpRequest));

	strHttpRequest.AppendFormat("Accept: */*\r\n");
	AddPeerLog(new CTraceSendMessage(_T("Accept: */*")));

	if(uiTo>uiFrom)
	{
		strHttpRequest.AppendFormat("Range: bytes=%I64u-%I64u\r\n", uiFrom, uiTo);

		temp.Format("Range: bytes=%I64u-%I64u", uiFrom, uiTo);
		AddPeerLog(new CTraceSendMessage((CString)temp));
	}
	else
	{
		strHttpRequest.AppendFormat("Range: bytes=%I64u-\r\n", uiFrom);

		temp.Format("Range: bytes=%I64u-", uiFrom);
		AddPeerLog(new CTraceSendMessage((CString)temp));
	}

	strHttpRequest.AppendFormat("Connection: close\r\n"); //Connection: Keep-Alive
	AddPeerLog(new CTraceSendMessage(_T("Connection: close")));

	strHttpRequest.AppendFormat("Host: %s\r\n", m_strHostA);

	temp.Format("Host: %s\r\n", m_strHostA);
	AddPeerLog(new CTraceSendMessage((CString)temp));

	if ( 0 == m_strHostA.CompareNoCase("verycd.com") )
	{
		strHttpRequest.AppendFormat("User-Agent: eMule VeryCD 0.47c\r\n");	//浏览器客户端标示，告诉我们自己的服务器这是eMule，另外可以附上版本号，用户操作系统等信息
		AddPeerLog(new CTraceSendMessage((_T("User-Agent: eMule VeryCD 0.48a"))));
	}
	else
	{
		strHttpRequest.AppendFormat("User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0; .NET CLR 1.1.4322)\r\n");	//浏览器客户端标示，伪装成IE浏览器
		AddPeerLog(new CTraceSendMessage(_T("User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0; .NET CLR 1.1.4322)")));
	}

	if( m_strRefer.IsEmpty() ) //url中没有给定默认referer时候取默认域名为 referer
	{
		char buffer[1024];
		CString strRefer = m_strURL.Left(m_strURL.ReverseFind('/') + 1);
		int nMulti = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)strRefer.GetBuffer(),strRefer.GetLength(), NULL, 0 , NULL, NULL);
		WideCharToMultiByte(CP_ACP, 0, strRefer.GetBuffer(), strRefer.GetLength(), buffer, nMulti, NULL, NULL);
		m_strRefer = CString(buffer, nMulti);
	}	

	temp.Format("Referer: %s", CStringA(m_strRefer));
	AddPeerLog(new CTraceSendMessage((CString)temp));

	strHttpRequest.AppendFormat("Referer: %s\r\n", CStringA(m_strRefer));

	strHttpRequest.AppendFormat("\r\n");

	if (thePrefs.GetDebugClientTCPLevel() > 0)
	{
		Debug(_T("Sending HTTP request:\n%hs"), strHttpRequest);
	}
}

void CHttpClient::Pause()
{
	CUpDownClient::Pause();
	AddPeerLog(new CTraceError(_T("User cancel")));

	if (!m_bKnownSize)
	{
		m_bDataTransfering = FALSE;
	}
}

void CHttpClient::ConnectionEstablished()
{
	//m_iErrTimes =0;
	SetDownloadState(DS_CONNECTED);
}

void CHttpClient::CreateBlockRequests(int iMaxBlocks)
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

void CHttpClient::EnsureContinueBlockRequest()
{
	//以下计算发送请求数据的起始和结束位置
	POSITION pos = m_PendingBlocks_list.GetHeadPosition();
	Pending_Block_Struct* pending = m_PendingBlocks_list.GetNext(pos);
	m_uReqStart = pending->block->StartOffset;
	m_uReqEnd = pending->block->EndOffset;

	bool bMergeBlocks = true;
	while (pos)
	{
		POSITION posLast = pos;
		pending = m_PendingBlocks_list.GetNext(pos);
		if (bMergeBlocks && pending->block->StartOffset == m_uReqEnd + 1) //连续的Block请求
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
}

uint32  CHttpClient::GetTimeUntilReask() const
{	
	ASSERT( reqfile );

	DWORD dwTickNow = GetTickCount();
	DWORD dwLastAskedtime = GetLastAskedTime(reqfile);

	if( (dwTickNow-dwLastAskedtime)<INetFileAskTimeInterval )
		return INetFileAskTimeInterval-(dwTickNow-dwLastAskedtime);
	else
		return 0; //OK,已超时INetFileAskTimeInterval,看看是否有需要下载的任务..		
}

////////////////////////////////////////////////////////////////////////////
bool CUpDownClient::ProcessHttpDownResponse(const CStringAArray& )
{
	ASSERT(0);
	return false;
}
