#pragma once


// CIEManagerWnd

class CIEManagerWnd : public CWnd
{
	DECLARE_DYNAMIC(CIEManagerWnd)

public:
	CIEManagerWnd();
	virtual ~CIEManagerWnd();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};