// ToolTipCtrlZ.cpp : 实现文件
//

#include "stdafx.h"
#include "ToolTipCtrlZ.h"
#include ".\tooltipctrlz.h"


// CToolTipCtrlZ

IMPLEMENT_DYNAMIC(CToolTipCtrlZ, CToolTipCtrl)
CToolTipCtrlZ::CToolTipCtrlZ()
{
}

CToolTipCtrlZ::~CToolTipCtrlZ()
{
}


BEGIN_MESSAGE_MAP(CToolTipCtrlZ, CToolTipCtrl)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CToolTipCtrlZ 消息处理程序


int CToolTipCtrlZ::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CToolTipCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	GetParent()->EnableToolTips();
	Activate(TRUE);
	SetMaxTipWidth(SHRT_MAX);
	SetMargin(CRect(4, 4, 4, 4));


	return 0;
}
