#pragma once



// CTabItem_MainTabBn ÃüÁîÄ¿±ê
#include "TabItem.h"

class CTabItem_MainTabBn : public CTabItem
{
	DECLARE_DYNCREATE(CTabItem_MainTabBn)
public:
	CTabItem_MainTabBn();
	virtual ~CTabItem_MainTabBn();

	virtual int	GetDesireLength(void){return 20;}

	virtual void Paint(CDC* pDC);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnMouseHover(void);
	virtual void OnMouseLeave(void);
protected:
	virtual BOOL CanActive(){return FALSE;}
protected:
	void	DrawActiveBk(CDC* pDC, const CRect &rect);
	void	DrawInactiveBk(CDC* pDC, const CRect &rect);
	void	DrawHover(CDC* pDC, const CRect &rect);
	void	DrawTriangle(CDC* pDC, CRect &rect);
};