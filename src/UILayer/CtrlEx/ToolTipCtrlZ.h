#pragma once


// CToolTipCtrlZ

class CToolTipCtrlZ : public CToolTipCtrl
{
	DECLARE_DYNAMIC(CToolTipCtrlZ)

public:
	CToolTipCtrlZ();
	virtual ~CToolTipCtrlZ();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


