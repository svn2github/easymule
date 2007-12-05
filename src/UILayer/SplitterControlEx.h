#pragma once
#include "SplitterControl.h"

typedef struct SPCEX_NMHDR
{
	NMHDR hdr;
	int flag;
} SPCEX_NMHDR;

class CSplitterControlEx : public CSplitterControl
{
public:
	CSplitterControlEx(void);
	virtual ~CSplitterControlEx(void);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();

protected:
	CRect m_rtBtn;
protected:
	void DrawTriangle(CDC* pDC, CRect& rect, COLORREF clr, bool bup = 1);
	BOOL HitTest(const CPoint& point);
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

private:
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

protected:
	BOOL m_bHover;
	BOOL m_bPress;
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
