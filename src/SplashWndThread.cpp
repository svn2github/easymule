// SplashWndThread.cpp : 实现文件
//

#include "stdafx.h"
#include "emule.h"
#include "SplashWndThread.h"
#include "SplashScreen.h"


// CSplashWndThread

IMPLEMENT_DYNCREATE(CSplashWndThread, CWinThread)

CSplashWndThread::CSplashWndThread()
{
}

CSplashWndThread::~CSplashWndThread()
{
}

BOOL CSplashWndThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	m_splashScreen.Create(m_splashScreen.IDD, CWnd::GetDesktopWindow());
	m_splashScreen.ShowWindow(SW_SHOW);

	return TRUE;
}

int CSplashWndThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CSplashWndThread, CWinThread)
END_MESSAGE_MAP()


// CSplashWndThread 消息处理程序
