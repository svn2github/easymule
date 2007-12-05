#pragma once

#define	WM_HK_CHANGE	WM_USER+500

// CHotKeyEdit

class CHotKeyEdit : public CHotKeyCtrl
{
	DECLARE_DYNAMIC(CHotKeyEdit)

public:
	CHotKeyEdit();
	virtual ~CHotKeyEdit();

	//afx_msg LRESULT OnKeyDown(WPARAM,LPARAM);
	afx_msg void OnKeyDown(UINT nChar,UINT nRepCnt,UINT nFlags);
	afx_msg void OnSysKeyDown(UINT nChar,UINT nRepCnt,UINT nFlags);

protected:
	DECLARE_MESSAGE_MAP()
};


