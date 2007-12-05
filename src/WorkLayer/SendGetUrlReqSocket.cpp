// SendGetUrlReqSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "SendGetUrlReqSocket.h"
#include "packets.h"
#include "Preferences.h"
#include "log.h"


// CSendGetUrlReqSocket

CSendGetUrlReqSocket::CSendGetUrlReqSocket()
{
}

CSendGetUrlReqSocket::~CSendGetUrlReqSocket()
{
}


// CSendGetUrlReqSocket 成员函数
bool CSendGetUrlReqSocket::SendRequest(void)
{
	CStringA strUrlPath = GetUrlPath();
	if (strUrlPath.IsEmpty())
		return false;

	if (!Create())
		return false;

	Connect(GetServer(), GetPort());

	return true;
}

void CSendGetUrlReqSocket::OnConnect(int nErrorCode)
{
	if (0 != nErrorCode)
	{
		if (thePrefs.GetVerbose())
		{
			CString		strUrlPath(GetUrlPath());
			CString		strServer(GetServer());

			strUrlPath.Replace(_T("%"), _T("%%"));
			AddDebugLogLine(false, _T("将要取%s，但连接%s返回失败。"), strUrlPath, strServer);
		}

		return;
	}

	CStringA strHttpRequest;
	strHttpRequest.AppendFormat("GET %s HTTP/1.0\r\n", GetUrlPath());
	strHttpRequest.AppendFormat("Host: %s\r\n", GetServer());
	strHttpRequest.AppendFormat("Accept: */*\r\n");
	//strHttpRequest.AppendFormat("Connection: Keep-Alive\r\n");
	strHttpRequest.AppendFormat("\r\n");

	if (thePrefs.GetVerbose())
	{
		CString		strRequest(strHttpRequest);
		strRequest.Replace(_T("%"), _T("%%"));

		AddDebugLogLine(false, _T("与服务器 %s 连接成功，准备发送:"), CString(GetServer()));
		AddDebugLogLine(false, strRequest);
	}


	CRawPacket* pHttpPacket = new CRawPacket(strHttpRequest);
	SendPacket(pHttpPacket);
	SetHttpState(HttpStateRecvExpected);
}

void CSendGetUrlReqSocket::DataReceived(const BYTE* pucData, UINT uSize)
{
	if (thePrefs.GetVerbose())
	{
		CString		strUrlPath(GetUrlPath());
		strUrlPath.Replace(_T("%"), _T("%%"));

		CStringA	straRecv((char *)pucData, uSize);
		AddDebugLogLine(false, _T("取%s时，返回结果："), strUrlPath);
		AddDebugLogLine(false, CString(straRecv));
	}

	CHttpClientReqSocket::DataReceived(pucData, uSize);
}


bool CSendGetUrlReqSocket::ProcessHttpResponse()
{
	return CHttpClientReqSocket::ProcessHttpResponse();
}

bool CSendGetUrlReqSocket::ProcessHttpResponseBody(const BYTE* pucData, UINT size)
{
	return CHttpClientReqSocket::ProcessHttpResponseBody(pucData, size);
}

