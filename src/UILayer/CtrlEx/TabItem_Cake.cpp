#include "StdAfx.h"
#include ".\tabitem_cake.h"
#include "Util.h"
#include "FaceManager.h"

CTabItem_Cake::CTabItem_Cake(void)
{
	SetAttribute(ATTR_FIXLEN);
}

CTabItem_Cake::~CTabItem_Cake(void)
{
}

void CTabItem_Cake::Paint(CDC* pDC)
{
	CRect		rtCake;
	rtCake = GetRect();

	if (GetActive())
		CFaceManager::GetInstance()->DrawImage(II_DETAILTAB_A, pDC->GetSafeHdc(), rtCake);
	else
	{
		if (IsHover())
			CFaceManager::GetInstance()->DrawImage(II_DETAILTAB_H, pDC->GetSafeHdc(), rtCake);
		else
			CFaceManager::GetInstance()->DrawImage(II_DETAILTAB_N, pDC->GetSafeHdc(), rtCake);
	}
	CRect		rtIcon;
	enum{ICONSIZE = 32};

	rtIcon = rtCake;
	rtIcon.DeflateRect((rtCake.Width() - ICONSIZE)/2, (rtCake.Height() - ICONSIZE)/2);
	rtIcon.OffsetRect(1, 0);

	if (m_bHasIcon)
		m_imgIcon.Draw(pDC->GetSafeHdc(), rtIcon.left, rtIcon.top);

}

