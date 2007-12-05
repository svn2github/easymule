#pragma once
#include "SpeedMeter.h"

// CSpeedMeterDlg 对话框

class CSpeedMeterDlg : public CDialog
{
	DECLARE_DYNAMIC(CSpeedMeterDlg)

public:
	CSpeedMeterDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSpeedMeterDlg();

// 对话框数据
	enum { IDD = IDD_MAINTAB_SPEEDMETER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CSpeedMeter	m_CtrlSpeedMeter;
public:
	int GetDesireWidth();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	LRESULT	OnEraseBkgndEx(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
