#pragma once


// CSearchButton

class CSearchButton : public CButton
{

public:
	CSearchButton();
	virtual ~CSearchButton();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	//{{AFX_VIRTUAL(CButtonST)
	//virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL
protected:
	virtual void PreSubclassWindow();
	void DrawInactive(CDC* pDC, const CRect& rect);
	void DrawHover(CDC* pDC,const CRect& rect);
	void DrawPressed(CDC* pDC, const CRect& rect);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
protected:
	HICON	m_hIcon;
	BOOL	m_bHover;
	BOOL	m_bIsPressed;
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

public:
	BOOL Create(CWnd* pParentWnd, CRect rect, LPCTSTR lpszCaption = NULL, DWORD dwStyle = WS_CHILD | WS_VISIBLE, UINT nID = 123);

	void GetDesireSize(CSize &size);
private:
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClicked();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};