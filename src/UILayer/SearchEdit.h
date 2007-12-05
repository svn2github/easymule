#pragma once

#include "Localizee.h"
#include "ResetButton.h"
#include "emule.h"

// CSearchEditor

class CSearchEdit : public CEdit, public CLocalizee
{
	DECLARE_DYNAMIC(CSearchEdit)
	LOCALIZEE_WND_CANLOCALIZE()
public:
	CSearchEdit();
	virtual ~CSearchEdit();

public:
	BOOL m_bTipinfo;

protected:
	DECLARE_MESSAGE_MAP()

protected:
	CFont			m_Font;
	CResetButton	m_ResetButton;

	BOOL			m_bFocusing;
public:
	BOOL Create(CWnd* pParentWnd, CRect rect, DWORD dwStyle = ES_LEFT | WS_CHILD | WS_VISIBLE, UINT nID = 123);
	afx_msg void OnEnKillfocus();
	afx_msg void OnEnSetfocus();

	void Localize();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};