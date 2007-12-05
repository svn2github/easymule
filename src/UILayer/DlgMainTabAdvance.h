#pragma once
#include "resource.h"
#include "AdvanceTabWnd.h"

// CDlgMainTabAdvance 对话框

class CDlgMainTabAdvance : public CDialog
{
	DECLARE_DYNAMIC(CDlgMainTabAdvance)

public:
	CDlgMainTabAdvance(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMainTabAdvance();

// 对话框数据
	enum { IDD = IDD_MAINTAB_ADVANCE };
protected:
	CAdvanceTabWnd				m_tbwAdvance;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
protected:
	virtual void OnCancel();
	virtual void OnOK();
};
