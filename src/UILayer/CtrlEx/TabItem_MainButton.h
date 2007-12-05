#pragma once
#include "TabItem_Normal.h"
#include "CxImage\ximage.h"

class CTabItem_MainButton : public CTabItem_Normal
{
	DECLARE_DYNCREATE(CTabItem_MainButton)
public:
	CTabItem_MainButton(void);
	virtual ~CTabItem_MainButton(void);

public:
	enum{MARGIN_HEAD = 18, ICON_TXT_GAP = 14, MARGIN_TAIL = 24,
		TOTAL_GAP = MARGIN_HEAD + ICON_TXT_GAP + MARGIN_TAIL};

	//void	SetActivedIcon(HICON hIcon);
	void	SetActivedIcon(LPCTSTR lpszPngResource);
	virtual	int	GetDesireLength(void);
protected:
	bool	m_bHasActiveIco;
	CxImage	m_imgActiveIco;
	//HICON	m_hIconActived;

protected:
	virtual void Paint(CDC* pDC);

public:
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnMouseHover(void);
	virtual void OnMouseLeave(void);
};
