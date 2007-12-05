#pragma once
#include "afxwin.h"


// CAffirmDeleteDlg 对话框

class CAffirmDeleteDlg : public CDialog
{
	DECLARE_DYNAMIC(CAffirmDeleteDlg)

public:
	CAffirmDeleteDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAffirmDeleteDlg();
    virtual BOOL OnInitDialog();
// 对话框数据
	enum { IDD = IDD_AFFIRMDELETE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CButton IsChecked;
	bool bIsDeleteFile;
};
