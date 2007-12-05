// MenuXP.h: interface for the CMenuXP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MENUXP_H__19788C14_E961_4B79_97E6_3DD120DEAF4E__INCLUDED_)
#define AFX_MENUXP_H__19788C14_E961_4B79_97E6_3DD120DEAF4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMenuXP
{

//DECLARE_DYNAMIC( CMenuXP )

public:
	void				EnableHook();
	static void			EnableHook(BOOL bEnable);
	static	void		RegisterEdge(int nLeft, int nTop, int nLength);
	void				SetWatermark(HBITMAP hBitmap);
	BOOL				AddMenu(CMenu* pMenu, BOOL bChild = FALSE);
	void				DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void				MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	CMenuXP();
	virtual		~CMenuXP();

protected:

	HBITMAP		m_hOldMark;
	CBitmap		m_bmWatermark;
	CDC			m_dcWatermark;
	CSize		m_czWatermark;
	BOOL		m_bEnable;
	int			m_nCheckIcon;

	static int		m_nEdgeLeft;
	static int		m_nEdgeTop;
	static int		m_nEdgeSize;
	static BOOL		m_bPrinted;

protected:
	void		DrawWatermark(CDC* pDC, CRect* pRect, int nOffX, int nOffY);
	void		DrawMenuText(CDC* pDC, CRect* pRect, const CString& strText);

protected:
	static LPCTSTR m_pszModeSuffix[3][4];
	BOOL m_bUnhook;
	static HHOOK m_hMsgHook;
	static LRESULT CALLBACK MsgHook(int nCode, WPARAM wParam, LPARAM lParam);
	static LPCTSTR wpnOldProc;
	static LRESULT	CALLBACK MenuProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	CMap<DWORD, DWORD, CString, CString&> m_pStrings;
public:
	UINT TrackPopupMenu(CMenu* pszMenu, const CPoint& point, UINT nDefaultID = 0, UINT nFlags = 0, CWnd* pWnd = AfxGetMainWnd());
};

extern CMenuXP MenuXP;

#endif // !defined(AFX_MENUXP_H__19788C14_E961_4B79_97E6_3DD120DEAF4E__INCLUDED_)
