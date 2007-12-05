
#include "stdafx.h"
#include ".\TabWndDef.h"

namespace TabWnd
{

	void Real2Logic(CRect &rect, ETabBarPos eRealPos)
	{
		CRect	rtResult;
		switch (eRealPos)
		{
		case TBP_TOP:
		default:
			rtResult = rect;
			break;
		case TBP_RIGHT:
			rtResult.left	= rect.top;
			rtResult.top	= - rect.right;
			rtResult.right	= rect.bottom;
			rtResult.bottom	= - rect.left;
			break;
		case TBP_BOTTOM:
			rtResult.left	= rect.left;
			rtResult.top	= - rect.bottom;
			rtResult.right	= rect.right;
			rtResult.bottom	= - rect.top;
			break;
		case TBP_LEFT:
			rtResult.left	= rect.top;
			rtResult.top	= rect.left;
			rtResult.right	= rect.bottom;
			rtResult.bottom	= rect.right;
			break;
		}
		rect = rtResult;
	}

	void Logic2Real(CRect &rect, ETabBarPos eRealPos)
	{
		CRect	rtResult;
		switch (eRealPos)
		{
		case TBP_TOP:
		default:
			rtResult = rect;
			break;
		case TBP_RIGHT:
			rtResult.left	= - rect.bottom;
			rtResult.top	= rect.left;
			rtResult.right	= - rect.top;
			rtResult.bottom	= rect.right;
			break;
		case TBP_BOTTOM:
			rtResult.left	= rect.left;
			rtResult.top	= - rect.bottom;
			rtResult.right	= rect.right;
			rtResult.bottom	= - rect.top;
			break;
		case TBP_LEFT:
			rtResult.left	= rect.top;
			rtResult.top	= rect.left;
			rtResult.right	= rect.bottom;
			rtResult.bottom	= rect.right;
			break;
		}
		rect = rtResult;
	}
}
