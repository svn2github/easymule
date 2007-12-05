#pragma once

class CMessageParam
{
public:
	WPARAM wParam;
	LPARAM lParam;
	CMessageParam()
	{
		wParam = 0;
		lParam = 0;
	}
};

class CUIMessageParam : public CMessageParam
{
public:
	CSingleLock * m_pLock;
	CUIMessageParam(const CMessageParam & param)
	{
		m_pLock = NULL;

		wParam = param.wParam;
		lParam = param.lParam;
	}
	~CUIMessageParam();
};

typedef CAutoPtr<CUIMessageParam> aMessageParam;

class CLogMessageParam : public CMessageParam
{
public:
	void * pTag;
	UINT msg;
};
class CMessageLog
{
	friend class CMessageParam;
public:
	~CMessageLog(void);
	static CMessageLog * GetInstace();
	UINT SaveMessage(UINT msg, WPARAM wParam, LPARAM lParam, void * pTag, bool bCredible);
	aMessageParam GetMessage(UINT uIndex);
	void RemoveTag(void * pTag);
private:
	CMessageLog();
	const CMessageLog & operator=(const CMessageLog &);
	CMessageLog(const CMessageLog &);
	CMutex m_Mutex;
	UINT m_uIndex;
	CRBMap<UINT, CLogMessageParam> m_MsgParams;
	CRBMap<void *, CList<UINT, UINT&>* > m_MsgTags;
#ifdef _DEBUG
	CList<void * > m_RemovedTags;
#endif
};
