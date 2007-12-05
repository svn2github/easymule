#include "StdAfx.h"
#include ".\joint.h"
#include "Util.h"
namespace TabWnd{

CJoint::CJoint(void)
{
	m_rtInTabWnd.SetRect(0, 0, 0, 0);
	m_ptStart.SetPoint(0, 0);
	m_ptEnd.SetPoint(0, 0);
	m_clrJoint = RGB(0, 0, 0);
	m_clrGap = RGB(0, 0, 0);
}

CJoint::~CJoint(void)
{
}

void CJoint::Draw(CDC *pDC)
{
	CBrush		brush(m_clrJoint);
	pDC->FrameRect(&m_rtInTabWnd, &brush);

	CBrush		brushGap(m_clrGap);
	CRect		rtGap(m_ptStart, m_ptEnd);
	pDC->FrameRect(&rtGap, &brushGap);

	//CPenDC	penDc(pDC->GetSafeHdc(), m_clrGap);
	//pDC->MoveTo(m_ptStart);
	//pDC->LineTo(m_ptEnd);
}

void CJoint::SetGap(CPoint ptStart, CPoint ptEnd)
{
	m_ptStart = ptStart;
	m_ptEnd = ptEnd;
}

void CJoint::SetJointColor(COLORREF clrJoint, COLORREF clrGap)
{
	m_clrJoint = clrJoint;
	m_clrGap = clrGap;
}

}//namespace TabWnd{