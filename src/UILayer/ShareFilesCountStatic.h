#pragma once


// CShareFilesCountStatic

class CShareFilesCountStatic : public CStatic
{
	DECLARE_DYNAMIC(CShareFilesCountStatic)

public:
	CShareFilesCountStatic();
	virtual ~CShareFilesCountStatic();
protected:
	void	DrawBk(CDC	*pDC, const CRect &rect);
	void	DrawText(CDC *pDC, const CRect &rect);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
protected:
	virtual void PreSubclassWindow();
};


