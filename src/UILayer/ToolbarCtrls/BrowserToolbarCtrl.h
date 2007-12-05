#pragma once
//#include "toolbarctrlx.h"

#define NUM_BROWSER_BUTTON 5

#define IDC_BROWSERBAR  63000
#define IDC_BROWSERBUTTON	63001

#define	TB_BACK			(IDC_BROWSERBUTTON + 0)
#define	TB_FORWARD		(IDC_BROWSERBUTTON + 1)
#define	TB_STOP			(IDC_BROWSERBUTTON + 2)
#define TB_REFRESH      (IDC_BROWSERBUTTON + 3)  
#define	TB_HOME			(IDC_BROWSERBUTTON + 4)

#define	MULE_BROWSERBAR_BAND_NR	1

//  CBrowserToolbarCtrl
#include "Localizee.h"
#include "ToolBarCtrlZ.h"

class CBrowserToolbarCtrl : public CToolBarCtrlZ, public CLocalizee
{
	DECLARE_DYNAMIC(CBrowserToolbarCtrl)
	LOCALIZEE_WND_CANLOCALIZE()
public:
	CBrowserToolbarCtrl();
	virtual ~CBrowserToolbarCtrl();

	void Init();
	void Localize();
	void Refresh();

	void EnableAllButtons(BOOL bEnable = TRUE);

protected:
	TBBUTTON	TBButtons[NUM_BROWSER_BUTTON];    // Added by thilon on 2006.08.02
	TCHAR		TBStrings[NUM_BROWSER_BUTTON][200];

	void	InitImageList();
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


