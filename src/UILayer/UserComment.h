#pragma once

#include "resource.h"
// CUserComment 对话框
#include "WebBrowserWnd.h"
#include "ResizableLib\ResizableDialog.h"

class CUserComment : public CResizableDialog
{
	DECLARE_DYNAMIC(CUserComment)

public:
	CUserComment(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUserComment();

	void Refresh(CKnownFile * file);

// 对话框数据
	enum { IDD = IDD_DIALOG_COMMENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

	CHtmlCtrl * m_pwebUserComment;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
