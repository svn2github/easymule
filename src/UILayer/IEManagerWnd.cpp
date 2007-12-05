// IEManagerWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "emule.h"
#include "IEManagerWnd.h"
#include ".\iemanagerwnd.h"


// CIEManagerWnd

IMPLEMENT_DYNAMIC(CIEManagerWnd, CWnd)
CIEManagerWnd::CIEManagerWnd()
{
}

CIEManagerWnd::~CIEManagerWnd()
{
}


BEGIN_MESSAGE_MAP(CIEManagerWnd, CWnd)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CIEManagerWnd 消息处理程序


BOOL CIEManagerWnd::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}
