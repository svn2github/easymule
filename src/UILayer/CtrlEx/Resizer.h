#pragma once

#include <AfxTempl.h>
#include "TabWndDef.h"

namespace TabWnd{

class CResizee;
class CResizer
{
public:
	CResizer(void);
	~CResizer(void);

	void	ResizeAll(const CRect &rtBound, ETabBarPos eBarPos);
	void	ResizeFrom(const CRect &rtBound, CResizee *pResizeeStart, ETabBarPos eBarPos);
	//void	SetRectByPrev(const CRect &rtBound, ETabBarPos eBarPos, CResizee *pResizee);
	//BOOL	CalRectByPrev(const CRect &rtBound, ETabBarPos eBarPos, CResizee *pResizee, CRect &rtItem);
	
	const CRect&	GetBarMarginLogic(){return m_rtBarMarginLogic;}
	void			SetBarMarginLogic(const CRect &rect);

	void	MoveResizeeTo(CResizee *pResizee, CResizee *pResizeeAfter, BOOL bAfter = TRUE);
protected:
	friend class CResizee;
	POSITION	AddResizee(CResizee *pResizee);
	void		RemoveResizee(CResizee *pResizee);

	CList<CResizee*, CResizee*>		m_resizees;

	CRect		m_rtBarMarginLogic;
	int			m_iFixedTabsTotalLength;
	int			m_iFixedTabsCount;
};
}//namespace TabWnd{