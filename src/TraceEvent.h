#pragma once



// CTraceEvent ÃüÁîÄ¿±ê

class CTraceEvent : public CObject
{
	DECLARE_DYNAMIC(CTraceEvent)
public:
	CTraceEvent();
	virtual ~CTraceEvent();

public:
	virtual COLORREF GetTextColor() = 0;
	virtual COLORREF GetBkColor() = 0;

	virtual CString GetText(void) = NULL;
	virtual CString GetTime(void);

public:
	enum { None = -1 };
protected:
	CTime	m_time;
};

class CTraceError : public CTraceEvent
{
	DECLARE_DYNAMIC(CTraceError)
public:
	CTraceError(const CString& strError);
	virtual ~CTraceError() {}

public:
	virtual COLORREF GetTextColor();
	virtual COLORREF GetBkColor();

	virtual CString GetText(void);

protected:
	CString m_strError;

};

//CTraceInformation
class CTraceInformation : public CTraceEvent
{
	DECLARE_DYNAMIC(CTraceInformation)
public:
	CTraceInformation(const CString& strInfomation);
	virtual ~CTraceInformation() {}

public:
	virtual COLORREF GetTextColor();
	virtual COLORREF GetBkColor();

	virtual CString GetText(void);

protected:
	CString m_strInfomation;
};

//CTraceSendMessage
class CTraceSendMessage : public CTraceEvent
{
	DECLARE_DYNAMIC(CTraceSendMessage)
public:
	CTraceSendMessage(const CString& strSendMessage);
	virtual ~CTraceSendMessage() {}

public:
	virtual COLORREF GetTextColor();
	virtual COLORREF GetBkColor();

	virtual CString GetText(void);

protected:
	CString m_strSendMessage;
};

//CTraceServerMessage
class CTraceServerMessage : public CTraceEvent
{
	DECLARE_DYNAMIC(CTraceServerMessage)
public:
	CTraceServerMessage(const CString& strServerMessage);
	virtual ~CTraceServerMessage() {}

public:
	virtual COLORREF GetTextColor();
	virtual COLORREF GetBkColor();

	virtual CString GetText(void);

protected:
	CString m_strServerMessage;
};

//CTraceKadMessage
class CTraceKadMessage : public CTraceEvent
{
	DECLARE_DYNAMIC(CTraceKadMessage)
public:
	CTraceKadMessage(const CString& strKadMessage);
	virtual ~CTraceKadMessage() {}

public:
	virtual COLORREF GetTextColor();
	virtual COLORREF GetBkColor();

	virtual CString GetText(void);

protected:
	CString m_strKadMessage;
};