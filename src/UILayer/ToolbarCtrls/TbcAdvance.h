#pragma once


// CTbcAdvance
#include "ToolBarCtrlZ.h"
#include "Localizee.h"

class CTbcAdvance : public CToolBarCtrlZ, public CLocalizee
{
	DECLARE_DYNAMIC(CTbcAdvance)
	LOCALIZEE_WND_CANLOCALIZE()
public:
	CTbcAdvance();
	virtual ~CTbcAdvance();
	
	virtual void Localize();

protected:
	enum {BUTTON_COUNT = 3};
	void InitImageList();

	int			m_iConnectServerStatus;
	CString		GetConnectServerText();
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	LRESULT OnTbEnableButton(WPARAM wParam, LPARAM lParam);
};


