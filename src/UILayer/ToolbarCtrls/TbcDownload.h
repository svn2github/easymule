#pragma once


// CTbcDownload
#include "ToolBarCtrlZ.h"
#include "Localizee.h"

class CTbcDownload : public CToolBarCtrlZ, public CLocalizee
{
	DECLARE_DYNAMIC(CTbcDownload)
	LOCALIZEE_WND_CANLOCALIZE()

public:
	CTbcDownload();
	virtual ~CTbcDownload();

	void Localize();
protected:
	enum {BUTTON_COUNT = 6};
	void		InitImageList();
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


