#pragma once

enum ETabBarPos
{
	TBP_LEFT,
	TBP_TOP,
	TBP_RIGHT,
	TBP_BOTTOM
};

namespace TabWnd
{
	void Real2Logic(CRect &rect, ETabBarPos eRealPos);
	void Logic2Real(CRect &rect, ETabBarPos eRealPos);

	inline void Real2LogicSolid(CRect &rect, ETabBarPos eRealPos)
	{
		rect.right--;
		rect.bottom--;
		Real2Logic(rect, eRealPos);
	}
	inline void LogicSolid2Real(CRect &rect, ETabBarPos eRealPos)
	{
		Logic2Real(rect, eRealPos);
		rect.right++;
		rect.bottom++;
	}
}
