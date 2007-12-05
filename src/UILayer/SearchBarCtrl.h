#pragma once
#include "SearchEdit.h"

#define IMAGE_WIDTH		20
#define MARGIN_WIDTH	4
//#define BUTTON_WIDTH	20
// CSearchBar

class CSearchBarCtrl : public CWnd
{
	DECLARE_DYNAMIC(CSearchBarCtrl)

public:
	CSearchBarCtrl();
	virtual ~CSearchBarCtrl();

protected:
	CFont	m_Font;
	CSearchEdit m_SearchEdit;
	CImageList m_ImageList;


protected:
	void ShowImage(int nImageNumber);
	void DrawTriangle(CDC* pDC, CRect &rect);

	int			m_nCurrentIcon;
	ESearchType m_eType;

	BOOL		m_isFocus;

	UINT		m_Type;

	CMenu		m_TypeMenu;

	BOOL		m_bHover;
protected:
	DECLARE_MESSAGE_MAP()
public:
	BOOL Create(CWnd* pParentWnd, CRect rect, DWORD dwStyle = WS_CHILD | WS_VISIBLE, UINT nID = 123);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CSearchEdit* GetEditor(void);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	LRESULT	OnEraseBkgndEx(WPARAM wParam, LPARAM lParam);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

private:
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

public:
	void SetSearchType(ESearchType eType);
	ESearchType GetSearchType(void);

	void SetType(UINT type);
	UINT GetType(void);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};