#pragma once
#include "RectDraw.h"

class CMainTabBkDraw : public CRectDraw
{
public:
	CMainTabBkDraw(void);
	~CMainTabBkDraw(void);

	virtual	void	Draw(CDC *pDC, const CRect &rect);

};
