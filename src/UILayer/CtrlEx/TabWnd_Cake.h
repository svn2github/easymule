#pragma once

#include "TabWnd.h"

class CTabWnd_Cake : public CTabWnd
{
public:
	CTabWnd_Cake(void);
	~CTabWnd_Cake(void);
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
