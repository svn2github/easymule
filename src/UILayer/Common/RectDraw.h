#pragma once

class CRectDraw
{
public:
	CRectDraw(void);
	~CRectDraw(void);

	virtual	void	Draw(CDC *pDC, const CRect &rect) = 0;
};
