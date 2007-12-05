#pragma once

#include "BkDraw.h"
// CToolBarCtrlZ

class CToolBarCtrlZ : public CToolBarCtrl, public CBkDraw
{
	DECLARE_DYNAMIC(CToolBarCtrlZ)

public:
	CToolBarCtrlZ();
	virtual ~CToolBarCtrlZ();

	int	AddSingleString(LPCTSTR lpszText);
	int GetMaxLength();
protected:
	void	DrawItem(CDC *pDC, int iIndex, const CRect &rtItem, BOOL bHover = FALSE);
	void	CleanupAllImages(void);
	void	AddDisableImageIcon(LPCTSTR lpszResource);
	void	AddImageIcon(LPCTSTR lpszResource);
protected:
	CArray<CxImage*, CxImage*>		m_arrImgs;
	CArray<CxImage*, CxImage*>		m_arrDisableImgs;
	CImageList						m_ilFake;

	CMouseMgr	m_mouseMgr;		// 为兼容windows2000(2000下没有HotItem)
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPaint();
	LRESULT OnGetDesireLength(WPARAM wParam, LPARAM lParam);
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


