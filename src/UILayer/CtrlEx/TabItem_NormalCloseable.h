#pragma once


#include "TabItem_Normal.h"
// CTabItem_NormalCloseable ÃüÁîÄ¿±ê

class CTabItem_NormalCloseable : public CTabItem_Normal
{
	DECLARE_DYNCREATE(CTabItem_NormalCloseable)

public:
	CTabItem_NormalCloseable();
	virtual ~CTabItem_NormalCloseable();

	void	EnableClose(BOOL bEnable){m_bEnableClose = bEnable;}
	virtual void OnSize(void);
	virtual void Paint(CDC* pDC);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual void OnMouseMove(UINT nFlags, CPoint point);

	enum {CLOSEBN_SIZE = 8};
	virtual BOOL	GetTextRect(CRect &rect);
	virtual int		GetDesireLength();

protected:
	BOOL	m_bEnableClose;
	CRect	m_rtCloseBn;
	BOOL	m_isCloseBtnOver;
};


