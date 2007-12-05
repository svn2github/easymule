#pragma once

// CTbcShare
#include "ToolBarCtrlZ.h"
#include "Localizee.h"

class CTbcShare : public CToolBarCtrlZ, public CLocalizee
{
	DECLARE_DYNAMIC(CTbcShare)
	LOCALIZEE_WND_CANLOCALIZE()

public:
	CTbcShare();
	virtual ~CTbcShare();

	void Localize();
protected:
	enum {BUTTON_COUNT = 5};
	void	InitImageList();
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
};


