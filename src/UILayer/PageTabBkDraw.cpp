#include "StdAfx.h"
#include ".\pagetabbkdraw.h"
#include "FaceManager.h"

CPageTabBkDraw::CPageTabBkDraw(void)
{
}

CPageTabBkDraw::~CPageTabBkDraw(void)
{
}

void CPageTabBkDraw::Draw(CDC *pDC, const CRect &rect)
{
	CFaceManager::GetInstance()->DrawImage(II_PAGETAB_BK, pDC->GetSafeHdc(), rect, 2);
}
