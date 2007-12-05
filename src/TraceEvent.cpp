// TraceEvent.cpp : 实现文件
//

#include "stdafx.h"
//#include "LogListDemo.h"
#include "TraceEvent.h"

IMPLEMENT_DYNAMIC(CTraceEvent, CObject)
IMPLEMENT_DYNAMIC(CTraceError, CTraceEvent)
IMPLEMENT_DYNAMIC(CTraceInformation, CTraceEvent)
IMPLEMENT_DYNAMIC(CTraceSendMessage, CTraceEvent)
IMPLEMENT_DYNAMIC(CTraceServerMessage, CTraceEvent)
IMPLEMENT_DYNAMIC(CTraceKadMessage, CTraceEvent)

// CTraceEvent

CTraceEvent::CTraceEvent()
{
	m_time = CTime::GetCurrentTime();
}

CTraceEvent::~CTraceEvent()
{
}

CString CTraceEvent::GetTime(void)
{
	CString time = m_time.Format(_T("%a %b %d %H:%M:%S %Y "));
	return time;
}


// CTraceError 成员函数
CTraceError::CTraceError(const CString & strError)
{
	m_strError = strError;
}

COLORREF CTraceError::GetTextColor()
{
	return RGB(139, 0, 0);
}

CString CTraceError::GetText()
{
	return m_strError;
}

COLORREF CTraceError::GetBkColor()
{
	return RGB(255, 192, 203);
}

//CTraceInfomation函数
CTraceInformation::CTraceInformation(const CString & strInfomation)
{
	m_strInfomation = strInfomation;
}

COLORREF CTraceInformation::GetTextColor()
{
	return RGB(0, 147, 0);
}

CString CTraceInformation::GetText()
{
	return m_strInfomation;
}

COLORREF CTraceInformation::GetBkColor()
{
	return RGB(152, 251, 152);
}

//CTraceSendMessage
CTraceSendMessage::CTraceSendMessage(const CString & strSendMessage)
{
	m_strSendMessage = strSendMessage;
}

COLORREF CTraceSendMessage::GetTextColor()
{
	return RGB(0, 0, 139);
}

CString CTraceSendMessage::GetText()
{
	return m_strSendMessage;
}

COLORREF CTraceSendMessage::GetBkColor()
{
	return RGB(175, 238, 238);
}

//CTraceServerMessage
CTraceServerMessage::CTraceServerMessage(const CString & strServerMessage)
{
	m_strServerMessage = strServerMessage;
}

COLORREF CTraceServerMessage::GetTextColor()
{
	return RGB(0, 0, 0);
}

CString CTraceServerMessage::GetText()
{
	return m_strServerMessage;
}

COLORREF CTraceServerMessage::GetBkColor()
{
	return RGB(230, 230, 230);
}

//CTraceKadMessage
CTraceKadMessage::CTraceKadMessage(const CString & strKadMessage)
{
	m_strKadMessage = strKadMessage;
}

COLORREF CTraceKadMessage::GetTextColor()
{
	return RGB(204, 0, 0);
}

CString CTraceKadMessage::GetText()
{
	return m_strKadMessage;
}

COLORREF CTraceKadMessage::GetBkColor()
{
	return RGB(255, 255, 136);
}