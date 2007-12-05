// DlgMainTabAdvance.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgMainTabAdvance.h"
#include ".\dlgmaintabadvance.h"
// CDlgMainTabAdvance 对话框

IMPLEMENT_DYNAMIC(CDlgMainTabAdvance, CDialog)
CDlgMainTabAdvance::CDlgMainTabAdvance(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMainTabAdvance::IDD, pParent)
{
}

CDlgMainTabAdvance::~CDlgMainTabAdvance()
{
}

void CDlgMainTabAdvance::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMainTabAdvance, CDialog)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDlgMainTabAdvance 消息处理程序

int CDlgMainTabAdvance::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	ModifyStyle(0, WS_CLIPCHILDREN);

	// TODO:  在此添加您专用的创建代码
	m_tbwAdvance.Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this);

	return 0;
}

void CDlgMainTabAdvance::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	m_tbwAdvance.MoveWindow(0, 0, cx, cy, FALSE);
}

BOOL CDlgMainTabAdvance::OnEraseBkgnd(CDC* /*pDC*/)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

void CDlgMainTabAdvance::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnCancel();
}

void CDlgMainTabAdvance::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnOK();
}
