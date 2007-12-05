#pragma once

#include "TabItem.h"

class CTabItem_Wnd : public CTabItem
{
	DECLARE_DYNCREATE(CTabItem_Wnd)
public:
	CTabItem_Wnd(void);
	~CTabItem_Wnd(void);

	void	SetItemWnd(CWnd *pWnd, BOOL bAutoDelete);
	void	SetWindowLength(int iLength){m_iWndLength = iLength;}
	virtual	int	GetDesireLength(void);

	virtual void	Create(CTabBar *pParentBar, POSITION myPos);
	//virtual void	Destroy();

	virtual void Paint(CDC* pDC);
	virtual	void OnSize();

protected:
	virtual BOOL CanActive(){return FALSE;}
protected:
	CWnd	*m_pWnd;
	BOOL	m_bAutoDelete;
	int		m_iWndLength;
};
