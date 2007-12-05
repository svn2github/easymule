// ResetButton.cpp : 实现文件
//

#include "stdafx.h"
#include "emule.h"
#include "ResetButton.h"
#include ".\resetbutton.h"


// CResetButton

IMPLEMENT_DYNAMIC(CResetButton, CStatic)
CResetButton::CResetButton()
{
	m_pImageList = NULL;
	m_nCurrentIcon = 0;
}

CResetButton::~CResetButton()
{
	if (m_pImageList != NULL)
	{
		m_pImageList->DeleteImageList();
	}
	delete m_pImageList;
}


BEGIN_MESSAGE_MAP(CResetButton, CStatic)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CResetButton 消息处理程序


BOOL CResetButton::OnEraseBkgnd(CDC* /*pDC*/)
{
	//return CStatic::OnEraseBkgnd(pDC);
	return TRUE;
}

void CResetButton::OnPaint()
{
	CPaintDC dc( this );
	CRect rect;
	GetClientRect( &rect );
	dc.FillSolidRect( rect, GetSysColor(COLOR_WINDOW) );
	m_pImageList->Draw(&dc, m_nCurrentIcon, CPoint(2, (rect.bottom - 16) / 2), ILD_NORMAL);
	
}

void CResetButton::ShowIcon(int nIconNumber)
{
	if (nIconNumber == m_nCurrentIcon)
	{
		return;
	}

	m_nCurrentIcon = nIconNumber;
	Invalidate();
	UpdateWindow();
}