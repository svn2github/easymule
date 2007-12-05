#pragma once

#include "resource.h"
// CUpLoading 对话框
#include "UploadListCtrl.h"
#include "ResizableLib\ResizableDialog.h"

class CUpLoading : public CResizableDialog
{
	DECLARE_DYNAMIC(CUpLoading)

public:
	CUpLoading(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUpLoading();

// 对话框数据
	enum { IDD = IDD_DIALOG_UPLOADING };
	CUploadListCtrl			uploadlistctrl;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
