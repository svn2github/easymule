// TbcAdvance.cpp : 实现文件
//

#include "stdafx.h"
#include "TbcAdvance.h"
#include ".\tbcadvance.h"
#include "eMule.h"
#include "MenuCmds.h"
#include "UserMsgs.h"
#include "WndMgr.h"
#include "StrSafe.h"
// CTbcAdvance

IMPLEMENT_DYNAMIC(CTbcAdvance, CToolBarCtrlZ)
CTbcAdvance::CTbcAdvance()
{
	m_iConnectServerStatus = 0;
}

CTbcAdvance::~CTbcAdvance()
{
}


BEGIN_MESSAGE_MAP(CTbcAdvance, CToolBarCtrlZ)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_MESSAGE(UM_TB_CHANGE_CONN_STATE, OnTbEnableButton)
END_MESSAGE_MAP()

void CTbcAdvance::Localize()
{
	ToolBarCtrl_SetText(this, 0, GetConnectServerText());
	ToolBarCtrl_SetText(this, 1, GetResString(IDS_WIZ1));
	ToolBarCtrl_SetText(this, 2, GetResString(IDS_OPTIONS));
	if (NULL != GetParent())
		GetParent()->SendMessage(WM_SIZE);
}

void CTbcAdvance::InitImageList()
{
	AddImageIcon(_T("PNG_CONNECT_SVR"));
	AddImageIcon(_T("PNG_WIZARD"));
	AddImageIcon(_T("PNG_OPTIONS"));
	AddImageIcon(_T("PNG_DISCONNECT_SVR"));

	AddDisableImageIcon(_T("PNG_CONNECT_SVR"));
	AddDisableImageIcon(_T("PNG_WIZARD"));
	AddDisableImageIcon(_T("PNG_OPTIONS"));
	AddDisableImageIcon(_T("PNG_DISCONNECT_SVR"));
}

CString CTbcAdvance::GetConnectServerText()
{
	switch(m_iConnectServerStatus) 
	{
	case 0:		// Disconnected
		return GetResString(IDS_TB_CONNECT_SERVER);
		break;
	case 1:		// Connecting
		return GetResString(IDS_TB_CANCEL_CONNECTING);
		break;
	case 2:		// Connected
		return GetResString(IDS_TB_DISCONNECT_SERVER);
		break;
	default:
		return CString(_T(""));
	}
}

// CTbcAdvance 消息处理程序


int CTbcAdvance::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CToolBarCtrlZ::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	theWndMgr.SetWndHandle(CWndMgr::WI_ADVANCE_TOOLBAR, GetSafeHwnd());

	InitImageList();

	AddSingleString(GetResString(IDS_TB_CONNECT_SERVER));
	AddSingleString(GetResString(IDS_WIZ1));
	AddSingleString(GetResString(IDS_OPTIONS));


	TBBUTTON	tbb[BUTTON_COUNT];
	CString		str;

	tbb[0].idCommand = MP_CONNECT;
	tbb[1].idCommand = MP_HM_1STSWIZARD;
	tbb[2].idCommand = MP_HM_PREFS;

	int	i = 0;
	for (i = 0; i < BUTTON_COUNT; i++)
	{
		tbb[i].iString = i;
		tbb[i].iBitmap = i;
		tbb[i].fsState = TBSTATE_ENABLED;
		tbb[i].fsStyle = TBSTYLE_BUTTON | BTNS_AUTOSIZE;
	}
	AddButtons(BUTTON_COUNT, tbb);

	Localize();

	return 0;
}

void CTbcAdvance::OnDestroy()
{
	CToolBarCtrlZ::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	theWndMgr.SetWndHandle(CWndMgr::WI_ADVANCE_TOOLBAR, NULL);
}

LRESULT CTbcAdvance::OnTbEnableButton(WPARAM wParam, LPARAM /*lParam*/)
{
	if (m_iConnectServerStatus == (int) wParam)
		return 0;

	CString			strText;
	TBBUTTONINFO	tbbi;

	ZeroMemory(&tbbi, sizeof(tbbi));
	tbbi.cbSize = sizeof(tbbi);
	tbbi.dwMask = TBIF_BYINDEX | TBIF_COMMAND | TBIF_IMAGE | TBIF_TEXT;

	switch (wParam)
	{
	case 0:		// Disconnected
		tbbi.idCommand = MP_CONNECT;
		tbbi.iImage = 0;
		break;
	case 1:		// Connecting
		tbbi.idCommand = MP_DISCONNECT;
		tbbi.iImage = BUTTON_COUNT;
		break;
	case 2:		// Connected
		tbbi.idCommand = MP_DISCONNECT;
		tbbi.iImage = BUTTON_COUNT;
		break;
	default:
		return 0;
	}

	m_iConnectServerStatus = wParam;
	strText = GetConnectServerText();
	tbbi.pszText = strText.LockBuffer();
	tbbi.cchText = strText.GetLength();
	SetButtonInfo(0, &tbbi);
	strText.UnlockBuffer();

	if (NULL != GetParent())
		GetParent()->SendMessage(WM_SIZE);

	return 0;
}
