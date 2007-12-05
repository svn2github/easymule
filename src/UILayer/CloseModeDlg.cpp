// CloseModeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "emule.h"
#include "CloseModeDlg.h"
#include ".\closemodedlg.h"


// CCloseModeDlg 对话框

bool CCloseModeDlg::m_bAskingClose = false;

IMPLEMENT_DYNAMIC(CCloseModeDlg, CDialog)
CCloseModeDlg::CCloseModeDlg(CWnd* pParent /*=NULL*/) : CDialog(CCloseModeDlg::IDD, pParent)
{
	m_iCloseMode= -1;
}

CCloseModeDlg::~CCloseModeDlg()
{
	m_bAskingClose = false;
}

void CCloseModeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_CLOSE_0, m_iCloseMode);
}


BEGIN_MESSAGE_MAP(CCloseModeDlg, CDialog)
END_MESSAGE_MAP()


// CCloseModeDlg 消息处理程序

BOOL CCloseModeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bAskingClose = true;

	SetWindowText(GetResString(IDS_CLOSEMODE_CAPTION));
	GetDlgItem(IDC_STATIC)->SetWindowText(GetResString(IDS_CLOSEMODE_TEXT));
	GetDlgItem(IDC_CLOSE_0)->SetWindowText(GetResString(IDS_MINIMISE));
	GetDlgItem(IDC_CLOSE_1)->SetWindowText(GetResString(IDS_SHUTDOWN));
    GetDlgItem(IDOK)->SetWindowText(GetResString(IDS_OK));
	GetDlgItem(IDCANCEL)->SetWindowText(GetResString(IDS_CANCEL));
	switch(thePrefs.GetCloseMode())
	{
	case 0:
		m_iCloseMode = 0;
		break;
	case 1:
		m_iCloseMode = 1;
		break;
	case 2:
		m_iCloseMode = 0;
		break;
	default:
		assert(0);
	}

	UpdateData( FALSE );
	return TRUE;
}

void CCloseModeDlg::OnOK()
{
	UpdateData();

	switch ( m_iCloseMode )
	{
	case 0:
		thePrefs.SetCloseMode(2);
		break;
	case 1:
		thePrefs.SetCloseMode(1);
		break;
	default:
		assert(0);
	}

	CDialog::OnOK();
}
