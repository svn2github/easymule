#pragma once


// CTabItem_Normal
#include "TabItem.h"
#include "CxImage\xImage.h"

class CTabItem_Normal : public CTabItem
{
	DECLARE_DYNCREATE(CTabItem_Normal)

public:
	enum {MARGIN_H = 10};
	CTabItem_Normal();
	virtual ~CTabItem_Normal();

	virtual void SetCaption(LPCTSTR lpszText);
	void	SetCustomData(DWORD dwData){m_dwCustomData = dwData;}
	//void	SetIcon(HICON hIcon){m_hIcon = hIcon;}
	void	SetIcon(LPCTSTR lpszPngResource);
	void	SetRelativeWnd(HWND hRelativeWnd, HWND hRelaWndOldParent = NULL,
							BOOL bAutoDelRelaWndObject = FALSE, CWnd* pRelaWndObjectToDel = NULL);
	virtual	int	GetDesireLength(void);


protected:
	virtual void	OnCreate(void);

	void	SetRelaWndParent();
	virtual BOOL	GetIconRect(CRect &rect);
	virtual BOOL	GetTextRect(CRect &rect);

	CxImage		m_imgIcon;
	bool		m_bHasIcon;

	//DECLARE_MESSAGE_MAP()
protected:
	void	DrawActiveBk(CDC* pDC, const CRect &rect);
	void	DrawHover(CDC* pDC, const CRect &rect);
	void	DrawInactiveBk(CDC* pDC, const CRect &rect);
	void	DrawItem(CDC* pDC, const CRect &rect, COLORREF clrFrmOutside, COLORREF clrFrmInside, COLORREF aclrLayers[4]);
	
public:
	virtual void Paint(CDC* pDC);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnMouseHover(void);
	virtual void OnMouseLeave(void);
};