#pragma once
#include "RectDraw.h"

class CPageTabBkDraw : public CRectDraw
{
public:
	CPageTabBkDraw(void);
	~CPageTabBkDraw(void);

	virtual	void	Draw(CDC *pDC, const CRect &rect);
};
