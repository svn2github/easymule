#pragma once
#include "tabwnd.h"
#include "CoolBarCtrl.h"
#include "TbcDownload.h"

class CDownloadTabWnd : public CTabWnd
{
	DECLARE_MESSAGE_MAP()
public:

	CDownloadTabWnd(void);
	virtual ~CDownloadTabWnd(void);
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

public:
	CTbcDownload	m_Toolbar;
protected:
	void		InitToolBar(void);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
