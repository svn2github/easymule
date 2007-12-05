#include "StdAfx.h"
#include ".\maintabbkdraw.h"
#include "FaceManager.h"

CMainTabBkDraw::CMainTabBkDraw(void)
{
}

CMainTabBkDraw::~CMainTabBkDraw(void)
{
}

void CMainTabBkDraw::Draw(CDC *pDC, const CRect &rect)
{
	CFaceManager::GetInstance()->DrawImage(II_MAINTAB_BK, pDC->GetSafeHdc(), rect, 1);
}
