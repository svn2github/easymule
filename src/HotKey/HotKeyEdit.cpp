/** @file HotKeyEdit.cpp @brief 本文件的简要说明，分行无效。
 <pre>
 *	Copyright (c) 2007，Emule
 *	All rights reserved.
 *
 *	当前版本：
 *	作    者：kernel
 *	完成日期：2007-05-15
 *
 *	取代版本：none
 *	作    者：none
 *	完成日期：none
 </pre>*/

//

#include "stdafx.h"
#include "emule.h"
#include "HotKeyEdit.h"


// CHotKeyEdit

IMPLEMENT_DYNAMIC(CHotKeyEdit, CHotKeyCtrl)
CHotKeyEdit::CHotKeyEdit()
{
}

CHotKeyEdit::~CHotKeyEdit()
{
}


BEGIN_MESSAGE_MAP(CHotKeyEdit, CHotKeyCtrl)
	//ON_MESSAGE(WM_KEYDOWN,OnKeyDown)
	//ON_MESSAGE(WM_SYSKEYDOWN,OnKeyDown)
	//ON_KEYDOWN()
	ON_WM_KEYDOWN()
	ON_WM_SYSKEYDOWN()
END_MESSAGE_MAP()



// CHotKeyEdit 消息处理程序

void CHotKeyEdit::OnKeyDown(UINT nChar,UINT nRepCnt,UINT nFlags)
{
	GetParent()->PostMessage(WM_HK_CHANGE);
	CHotKeyCtrl::OnKeyDown(nChar,nRepCnt,nFlags);
}

void CHotKeyEdit::OnSysKeyDown(UINT nChar,UINT nRepCnt,UINT nFlags)
{
	GetParent()->PostMessage(WM_HK_CHANGE);
	CHotKeyCtrl::OnSysKeyDown(nChar,nRepCnt,nFlags);
}