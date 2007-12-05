#pragma once


namespace TabWnd{

class CJoint
{
public:
	CJoint(void);
	~CJoint(void);

	void	Move(const CRect &rect){m_rtInTabWnd = rect;}
	void	Draw(CDC *pDC);

	void	SetGap(CPoint ptStart, CPoint ptEnd);
	void	SetJointColor(COLORREF clrJoint, COLORREF clrGap);
protected:
	CRect	m_rtInTabWnd;
	CPoint	m_ptStart;
	CPoint	m_ptEnd;
	COLORREF m_clrJoint;
	COLORREF m_clrGap;
};

}//namespace TabWnd{