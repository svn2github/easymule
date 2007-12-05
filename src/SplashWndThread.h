#pragma once



// CSplashWndThread

class CSplashWndThread : public CWinThread
{
	DECLARE_DYNCREATE(CSplashWndThread)

protected:
	CSplashWndThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CSplashWndThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
protected:
	CSplashScreen		m_splashScreen;
protected:
	DECLARE_MESSAGE_MAP()
};


