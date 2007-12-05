// TbcShare.cpp : 实现文件
//

#include "stdafx.h"
#include "TbcShare.h"
#include ".\tbcshare.h"
#include "emule.h"
#include "MenuCmds.h"
#include "CIF.h"
#include "WndMgr.h"
// CTbcShare

IMPLEMENT_DYNAMIC(CTbcShare, CToolBarCtrlZ)
CTbcShare::CTbcShare()
{
}

CTbcShare::~CTbcShare()
{
}


BEGIN_MESSAGE_MAP(CTbcShare, CToolBarCtrlZ)
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

void CTbcShare::Localize()
{
	ToolBarCtrl_SetText(this, 0, GetResString(IDS_OPEN));
	ToolBarCtrl_SetText(this, 1, GetResString(IDS_OPENFOLDER));
	ToolBarCtrl_SetText(this, 2, GetResString(IDS_RENAME));
	ToolBarCtrl_SetText(this, 3, GetResString(IDS_DELETE_FILE));
	ToolBarCtrl_SetText(this, 4, GetResString(IDS_SF_RELOAD));

	if (NULL != GetParent())
		GetParent()->SendMessage(WM_SIZE);
}

void CTbcShare::InitImageList()
{
	AddImageIcon(_T("PNG_TBOPENFILE"));
	AddImageIcon(_T("PNG_TBOPENFOLDER"));
	AddImageIcon(_T("PNG_TBRENAME"));
	AddImageIcon(_T("PNG_TBREMOVE"));
	AddImageIcon(_T("PNG_BTREFRESH"));


	AddDisableImageIcon(_T("PNG_TBOPENFILE_D"));
	AddDisableImageIcon(_T("PNG_TBOPENFOLDER_D"));
	AddDisableImageIcon(_T("PNG_TBRENAME_D"));
	AddDisableImageIcon(_T("PNG_TBREMOVE_D"));
	AddDisableImageIcon(_T("PNG_BTREFRESH_D"));

}

// CTbcShare 消息处理程序


int CTbcShare::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CToolBarCtrlZ::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	theWndMgr.m_pTbcShare = this;

	int	i = 0;

	InitImageList();

	AddSingleString(GetResString(IDS_OPEN));
	AddSingleString(GetResString(IDS_OPENFOLDER));
	AddSingleString(GetResString(IDS_RENAME));
	AddSingleString(GetResString(IDS_DELETE_FILE));
	AddSingleString(GetResString(IDS_SF_RELOAD));


	TBBUTTON	tbb[BUTTON_COUNT];
	CString		str;

	tbb[0].idCommand = MP_OPEN;
	tbb[1].idCommand = MP_OPENFOLDER;
	tbb[2].idCommand = MP_RENAME;
	tbb[3].idCommand = MP_REMOVE;
	tbb[4].idCommand = MP_REFRESH;

	for (i = 0; i < BUTTON_COUNT; i++)
	{
		tbb[i].iString = i;
		tbb[i].iBitmap = i;
		tbb[i].fsState = 0;
		tbb[i].fsStyle = TBSTYLE_BUTTON | BTNS_AUTOSIZE;
	}
	AddButtons(BUTTON_COUNT, tbb);

	EnableButton(MP_REFRESH);

	Localize();
	return 0;
}

void CTbcShare::OnDestroy()
{
	__super::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	theWndMgr.m_pTbcShare = NULL;

}
