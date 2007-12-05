#pragma once

// CDlgMainTabSidePanel 对话框
#include "Resource.h"
#include "afxwin.h"
#include "SearchButton.h"
//#include "SpeedMeter.h"
#include "SearchBarCtrl.h"

class CDlgMainTabSidePanel : public CDialog
{
	DECLARE_DYNAMIC(CDlgMainTabSidePanel)
	//DECLARE_ANCHOR_MAP()	//Added by thilon on 2007.02.03, for Resize

public:
	CDlgMainTabSidePanel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMainTabSidePanel();

// 对话框数据
	enum { IDD = IDD_MAINTAB_SIDEPANEL };
public:
	CSearchBarCtrl m_SearchBarCtrl;
	CSearchButton m_SearchButton;

public:
	int		GetDesireWidth();
	void	Resize();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	//afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	LRESULT	OnEraseBkgndEx(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	
protected:
	virtual void OnCancel();

protected:
	

//	afx_msg void OnBnClickedSearchbutton();
protected:
	virtual void OnOK();
public:
	afx_msg void OnPaint();
};
