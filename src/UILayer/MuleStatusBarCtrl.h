#pragma once

enum EStatusBarPane
{
	SBarLog = 0,
	//SBarUsers,	//COMMENTED by fengwen on %DATE% <begin>	状态栏不显示用户数
	SBarUpDown,
	SBarConnected,
	SBarChatMsg
};

class CMuleStatusBarCtrl : public CStatusBarCtrl
{
	DECLARE_DYNAMIC(CMuleStatusBarCtrl)

public:
	CMuleStatusBarCtrl();
	virtual ~CMuleStatusBarCtrl();

	void Init(void);

protected:
	int GetPaneAtPosition(CPoint& point) const;
	CString GetPaneToolTipText(EStatusBarPane iPane) const;

	virtual int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDblClk(UINT nFlags,CPoint point);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
