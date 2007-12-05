#pragma once
#include "PPgGeneral.h"
#include "PPgConnection.h"
#include "PPgServer.h"
#include "PPgDirectories.h"
#include "PPgFiles.h"
#include "PPgDisplay.h"
#include "PPgWebServer.h"
#include "PPgScheduler.h"
#include "PPgProxy.h"
#include "PPgAntiLeech.h"
#include "PPGTweaks.h"
#if defined(_DEBUG) || defined(USE_DEBUG_DEVICE)
#include "PPgDebug.h"
#endif
#include "otherfunctions.h"
#include "TreePropSheet.h"
#include "ToolTipCtrlZ.h"

class CPreferencesDlg : public CTreePropSheet
{
	DECLARE_DYNAMIC(CPreferencesDlg)

public:
	CPreferencesDlg();
	virtual ~CPreferencesDlg();
	
	CPPgGeneral		m_wndGeneral;
	CPPgConnection	m_wndConnection;
	CPPgDisplay		m_wndDisplay;
	CPPgDirectories	m_wndDirectories;
	CPPgFiles		m_wndFiles;
	// VC-kernel[2007-02-05]:to be deleted
	CPPgServer		m_wndServer;
	//CPPgStats		m_wndStats;
	//CPPgNotify		m_wndNotify;
	//CPPgIRC			m_wndIRC;
	CPPgTweaks		m_wndTweaks;
	//CPPgSecurity	m_wndSecurity;
	CPPgWebServer	m_wndWebServer;
	CPPgScheduler	m_wndScheduler;
	CPPgProxy		m_wndProxy;
	//CPPgMessages	m_wndMessages;
	// VC-kernel[2007-02-05]:to be deleted
	CPPgAntiLeech   m_wndAntiLeech;
#if defined(_DEBUG) || defined(USE_DEBUG_DEVICE)
	CPPgDebug		m_wndDebug;
#endif


	void Localize();
	void SetStartPage(UINT uStartPageID);

protected:
	LPCTSTR m_pPshStartPage;
	bool m_bSaveIniFile;
	
	CToolTipCtrlZ	m_ttc;
	BOOL			m_bTipAdded;
	void			AddTreeItemTip(int iPage, LPCTSTR lpszTipText, UINT uIdTool);


	virtual BOOL OnInitDialog();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnDestroy();
	afx_msg void OnHelp();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnPageAdvanceChanged();// VC-kernel[2007-02-09]:
	afx_msg LRESULT OnSetCurSel(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
