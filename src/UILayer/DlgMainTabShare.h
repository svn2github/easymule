#pragma once
// CDlgMainTabShare 对话框
#include "resource.h"
#include "TabWnd.h"
#include "TbcShare.h"

class CDlgMainTabShare : public CDialog
{
	DECLARE_DYNAMIC(CDlgMainTabShare)

public:
	CDlgMainTabShare(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMainTabShare();

// 对话框数据
	enum { IDD = IDD_MAINTAB_SHARE };

	void	SetShareText(LPCTSTR lpszText);
protected:
	CTabWnd			m_tabWnd;
	CTbcShare		m_toolbar;

	POSITION		m_posShare;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};