#pragma once
#include "Util.h"
#include "RectDraw.h"

class CBkDraw
{
public:
	CBkDraw(void)
	{
		m_pBkDraw = NULL;
	}
	~CBkDraw(void)
	{
		SAFE_DELETE(m_pBkDraw);
	}

	void	SetBkDraw(CRectDraw	*pDraw)
	{
		if (pDraw == m_pBkDraw)
			return;

		SAFE_DELETE(m_pBkDraw);
		m_pBkDraw = pDraw;
	}
protected:
	BOOL	DrawBk(CDC *pDC, const CRect &rect)
	{
		if (NULL != m_pBkDraw)
		{
			m_pBkDraw->Draw(pDC, rect);
			return TRUE;
		}
		return FALSE;
	}
private:
	CRectDraw	*m_pBkDraw;
};
