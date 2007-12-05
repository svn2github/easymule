// SpeedMeterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "emule.h"
#include "SpeedMeterDlg.h"
#include "Util.h"
#include ".\speedmeterdlg.h"


// CSpeedMeterDlg 对话框

IMPLEMENT_DYNAMIC(CSpeedMeterDlg, CDialog)
CSpeedMeterDlg::CSpeedMeterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSpeedMeterDlg::IDD, pParent)
{
}

CSpeedMeterDlg::~CSpeedMeterDlg()
{
}

void CSpeedMeterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSpeedMeterDlg, CDialog)
	ON_WM_PAINT()
	ON_MESSAGE(WM_ERASEBKGND, OnEraseBkgndEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CSpeedMeterDlg 消息处理程序
int CSpeedMeterDlg::GetDesireWidth()
{
	return 50;
	//CRect	rtClient;
	//if (NULL != GetSafeHwnd())
	//{
	//	GetClientRect(&rtClient);
	//	return rtClient.Width();
	//}
	//else
	//{
	//	return 0;
	//}
}
BOOL CSpeedMeterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_CtrlSpeedMeter.CreateEx(NULL, NULL, NULL, WS_CHILD | WS_VISIBLE /*| WS_BORDER*/, CRect(0, 0, 0, 0), this, 312);
	m_CtrlSpeedMeter.SetRange(0, 100);
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CSpeedMeterDlg::OnPaint()
{
	CPaintDC dc(this);

	CClientRect	rtClient(this);
	CBufferDC	bufDC(dc.GetSafeHdc(), rtClient);

	SendMessage(WM_ERASEBKGND, (WPARAM) bufDC.GetSafeHdc(), 1);
}

LRESULT	CSpeedMeterDlg::OnEraseBkgndEx(WPARAM wParam, LPARAM lParam)
{
	if (1 == lParam)
	{
		::SendMessage( ::GetParent(GetSafeHwnd()), WM_ERASEBKGND, wParam, lParam);
	}

	return 1;
}

void CSpeedMeterDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CRect rtSpeedMeter;
	GetClientRect(&rtSpeedMeter);

	rtSpeedMeter.right -= 2;

	if(m_CtrlSpeedMeter.GetSafeHwnd())
	{
		m_CtrlSpeedMeter.MoveWindow(&rtSpeedMeter);
	}
}
