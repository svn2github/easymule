#pragma once

#include "TabWnd.h"
#include "Localizee.h"
#include "TbcAdvance.h"
// CAdvanceTabWnd

class CAdvanceTabWnd : public CTabWnd , public CLocalizee
{
	DECLARE_DYNAMIC(CAdvanceTabWnd)
	LOCALIZEE_WND_CANLOCALIZE()
public:
	CAdvanceTabWnd();
	virtual ~CAdvanceTabWnd();

	void Localize();
protected:
	enum ETabId
	{
		TI_SERVER,
		TI_KAD,
		TI_STAT,

		TI_MAX
	};
	POSITION	m_aposTabs[TI_MAX];

	CTbcAdvance	m_toolbar;
	
	void InitToolBar(void);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


