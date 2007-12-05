// AffirmDeleteDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "emule.h"
#include "AffirmDeleteDlg.h"
#include ".\affirmdeletedlg.h"


// CAffirmDeleteDlg 对话框

IMPLEMENT_DYNAMIC(CAffirmDeleteDlg, CDialog)
CAffirmDeleteDlg::CAffirmDeleteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAffirmDeleteDlg::IDD, pParent)
{
}

CAffirmDeleteDlg::~CAffirmDeleteDlg()
{
}

void CAffirmDeleteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, IsChecked);
}


BEGIN_MESSAGE_MAP(CAffirmDeleteDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CAffirmDeleteDlg 消息处理程序
BOOL CAffirmDeleteDlg::OnInitDialog()
{  
   CDialog::OnInitDialog();
   this->SetWindowText(GetResString(IDS_DELETE_FILE));
   GetDlgItem(IDC_STATIC)->SetWindowText(GetResString(IDS_DELETE_TASK));
   GetDlgItem(IDC_CHECK1)->SetWindowText(GetResString(IDS_DELETE_DISKFILE));
   GetDlgItem(IDOK)->SetWindowText(GetResString(IDS_OK));
   GetDlgItem(IDCANCEL)->SetWindowText(GetResString(IDS_CANCEL));
   HICON hicon = ::LoadIcon(NULL, IDI_QUESTION);
   if(!hicon)
	   return false;
   CStatic *pStatic = (CStatic *)GetDlgItem(IDC_IMAGE);
   if(pStatic)
       pStatic->SetIcon(hicon);
   return TRUE;
}
void CAffirmDeleteDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if(IsChecked.GetCheck() == 1)
		bIsDeleteFile = true;
	else
		bIsDeleteFile = false;
	OnOK();
}
