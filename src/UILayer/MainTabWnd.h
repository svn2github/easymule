#pragma once

#include "TabWnd.h"

// CMainTabWnd
#include "DlgMainTabResource.h"
#include "DlgMaintabDownload.h"
#include "DlgMainTabShare.h"
#include "DlgMainTabSidePanel.h"
#include "DlgMainTabAdvance.h"
#include "SpeedMeterDlg.h"

#include "Localizee.h"
//#include "StatDlg.h"

class CMainTabWnd : public CTabWnd, public CLocalizee
{
	DECLARE_DYNAMIC(CMainTabWnd)
	LOCALIZEE_WND_CANLOCALIZE()
public:
	CMainTabWnd();
	virtual ~CMainTabWnd();

	BOOL	CreateEx(const RECT& rect, CWnd* pParentWnd, UINT nID);

	CDlgMainTabResource			m_dlgResource;
	CDlgMaintabDownload			m_dlgDownload;
	CDlgMainTabShare			m_dlgShare;
	CDlgMainTabAdvance			m_dlgAdvance;

	CDlgMainTabSidePanel		m_dlgSidePanel;
	/*CSpeedMeterDlg				m_SpeedMeterDlg;*/

	enum ETabId
	{
		TI_RESOURCE,
		TI_DOWNLOAD,
		TI_SHARE,
		TI_ADVANCE,
		TI_BN,
		
		TI_MAX
	};
	POSITION	m_aposTabs[TI_MAX];
	BOOL		IsTabShowed(ETabId eTabId);
	void		AddTabById(ETabId eTabId);
	void		RemoveTabById(ETabId eTabId);
	void		SetActiveTabById(ETabId eTabId){SetActiveTab(m_aposTabs[eTabId]);}
protected:
	void		Localize();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	afx_msg void OnPaint();
};


