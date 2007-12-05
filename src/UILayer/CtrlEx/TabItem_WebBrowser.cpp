#include "StdAfx.h"
#include ".\tabitem_webbrowser.h"

CTabItem_WebBrowser::CTabItem_WebBrowser(void)
{
	m_pWbw = NULL;
	//m_iProgressPercent = 100;

	//m_arrProgressIcons[0] = LoadIcon(::AfxGetInstanceHandle(), _T("CONTACT4"));
	//m_arrProgressIcons[1] = LoadIcon(::AfxGetInstanceHandle(), _T("CONTACT3"));
	//m_arrProgressIcons[2] = LoadIcon(::AfxGetInstanceHandle(), _T("CONTACT2"));
	//m_arrProgressIcons[3] = LoadIcon(::AfxGetInstanceHandle(), _T("CONTACT1"));
	//m_arrProgressIcons[4] = LoadIcon(::AfxGetInstanceHandle(), _T("CONTACT0"));

	//m_hIcon = m_arrProgressIcons[4];
}

CTabItem_WebBrowser::~CTabItem_WebBrowser(void)
{
	//int	i;
	//for (i = 0; i < ICON_COUNT; i ++)
	//{
	//	if (NULL != m_arrProgressIcons[i])
	//	{
	//		DestroyIcon(m_arrProgressIcons[i]);
	//	}
	//}
}

int CTabItem_WebBrowser::GetDesireLength()
{
	return 150;
}
void CTabItem_WebBrowser::SetWbWnd(CWebBrowserWnd *pWbw)
{
	m_pWbw = pWbw;
}

//void CTabItem_WebBrowser::OnProgressChanged(long lProgress, long lProgressMax)
//{
//	if (0 == lProgressMax)
//		m_iProgressPercent = 100;
//	else
//		m_iProgressPercent = lProgress * 100 / lProgressMax;
//
//	int	iIconIndex;
//	if (100 == m_iProgressPercent)
//		iIconIndex = ICON_COUNT - 1;
//	else
//		iIconIndex = m_iProgressPercent / 25;
//
//	m_hIcon = m_arrProgressIcons[iIconIndex];
//	
//	Invalidate();
//
////	UpdateCaption();
//}

//void CTabItem_WebBrowser::UpdateCaption()
//{
//	//if (m_iProgressPercent >= 100)
//	//	m_strCaption = m_strTitle;
//	//else
//		m_strCaption.Format(_T("[%3d%%] %s"), m_iProgressPercent, m_strTitle);
//	Invalidate();

