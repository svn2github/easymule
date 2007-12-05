#include "StdAfx.h"
#include ".\tabwnd_cake.h"

CTabWnd_Cake::CTabWnd_Cake(void)
{
}

CTabWnd_Cake::~CTabWnd_Cake(void)
{
}
BEGIN_MESSAGE_MAP(CTabWnd_Cake, CTabWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

int CTabWnd_Cake::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTabWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	SetBarPos(TBP_LEFT);
	SetBarBreadth(38);

	COLORREF	clrBk = RGB(0x99, 0, 0);
	SetBkColor(clrBk, FALSE);
	SetJointColor(clrBk, clrBk);
	SetBoardColor(clrBk);


	return 0;
}
