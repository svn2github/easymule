// AddTaskListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "AddTaskListCtrl.h"
#include ".\addtasklistctrl.h"
#include "UserMsgs.h"
#include "Util.h"
#include "ED2KLink.h"
#include "CmdFuncs.h"
#include "otherfunctions.h"
#include "StringConversion.h"

// CAddTaskListCtrl

IMPLEMENT_DYNAMIC(CAddTaskListCtrl, CListCtrl)
CAddTaskListCtrl::CAddTaskListCtrl()
{
}

CAddTaskListCtrl::~CAddTaskListCtrl()
{
	SItemData		*pItemData;
	POSITION	pos = m_itemDatas.GetHeadPosition();
	while (NULL != pos)
	{
		pItemData = m_itemDatas.GetNext(pos);
		SAFE_DELETE(pItemData);
	}
	m_itemDatas.RemoveAll();
}


BEGIN_MESSAGE_MAP(CAddTaskListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnLvnItemchanged)

	ON_MESSAGE(UM_ADDTASK_DOC_ADDED, OnDocAdded)
	ON_MESSAGE(UM_ADDTASK_DOC_MODIFIED, OnDocModified)
	ON_MESSAGE(UM_ADDTASK_DOC_REMOVED, OnDocRemoved)
	
	ON_MESSAGE(UM_ADDTASK_DOC_URL_ADDED, OnDocUrlAdded)
	ON_MESSAGE(UM_ADDTASK_DOC_URL_MODIFIED, OnDocUrlModified)
	ON_MESSAGE(UM_ADDTASK_DOC_URL_REMOVED, OnDocUrlRemoved)

	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()



int CAddTaskListCtrl::GetInsertIndex(LPCTSTR lpszFileName)
{
	int	i;
	int	iCount = GetItemCount();
	CString	strItemText;
	for (i = 0; i < iCount; i++)
	{
		strItemText = GetItemText(i, 0);
		if (strItemText > lpszFileName)
			break;
	}
	return i;
}

void CAddTaskListCtrl::CalStrByLinkText(LPCTSTR lpszLinkText, CString &strFileName, CString &strFileSize)
{
	CED2KFileLink	*pLink = NULL;
	try
	{
		pLink = (CED2KFileLink*) CED2KFileLink::CreateLinkFromUrl(lpszLinkText);
	}
	catch(...)
	{
		strFileName.Empty();
		strFileSize.Empty();
		SAFE_DELETE(pLink);
	}

	strFileName = pLink->GetName();
	strFileSize = CmdFuncs::GetFileSizeDisplayStr(pLink->GetSize());
	SAFE_DELETE(pLink);
}

void CAddTaskListCtrl::CalStrByUrl(const CString &strUrl, CString &strFileName)
{	CString sUrlShort = strUrl.Right( strUrl.GetLength() - strUrl.Find(_T("://")) -3 );
	if( sUrlShort.Find(_T('<'))>0 )
		sUrlShort = sUrlShort.Left( sUrlShort.Find(_T('<')) ); //cut the referer info..

	int iIndex = sUrlShort.ReverseFind(_T('/'));
	if( iIndex==-1 )
	{
		strFileName = _T("Index.htm");
		return;
	}
	else
	{   
		if(sUrlShort.Find(_T('?'))>0)
		{
			strFileName = sUrlShort.Left(sUrlShort.Find(_T('?')));
			strFileName = strFileName.Right(strFileName.GetLength() -1 -strFileName.ReverseFind(_T('/')));
		}
		if(sUrlShort.Find(_T('#'))>0 )
		{   
			strFileName = sUrlShort.Right( sUrlShort.GetLength() - 1 - iIndex );
		    strFileName = strFileName.Left( strFileName.Find(_T('#')));
		}
		if(strFileName.IsEmpty())
		{
			strFileName = sUrlShort.Right( sUrlShort.GetLength() - 1 - iIndex );
		    if (strFileName.IsEmpty())
		        strFileName = _T("Index.htm");
		}
	}	
}

void CAddTaskListCtrl::SetItemByDocItem(int iItemIndex, const CAddTaskDoc::SItem &item, DWORD dwItemMask)
{
	if (-1 == iItemIndex)
		return; 

	if (CAddTaskDoc::IM_TEXT & dwItemMask)
	{
		CString			strFileName;
		CString			strFileSize;
		CalStrByLinkText(item.strLinkText, strFileName, strFileSize);
		SetItemText(iItemIndex, 0, strFileName);
		SetItemText(iItemIndex, 1, strFileSize);
	}
	if (CAddTaskDoc::IM_CHECK & dwItemMask)
	{
		SetCheck(iItemIndex, item.bCheck);
	}
}

int CAddTaskListCtrl::GetItemIndexByKey(const CFileHashKey &key)
{
	SItemData	*pItemData = NULL;

	int	i;
	int	iCount = GetItemCount();
	for (i = 0; i < iCount; i++)
	{
		pItemData = (SItemData*) GetItemData(i);
 		if (IT_ED2K == pItemData->iType
			&& pItemData->fhk == key)
			return i;
	}
	return -1;
}

int CAddTaskListCtrl::GetItemIndexByUrl(LPCTSTR lpszUrl)
{
	SItemData	*pItemData = NULL;

	int	i;
	int	iCount = GetItemCount();
	for (i = 0; i < iCount; i++)
	{
		pItemData = (SItemData*) GetItemData(i);
		if (IT_URL == pItemData->iType
			&& 0 == pItemData->strUrl.CompareNoCase(lpszUrl))
			return i;
	}
	return -1;
}

void CAddTaskListCtrl::CheckAllItems(BOOL bCheck)
{
	int		i;
	for (i = 0; i < GetItemCount(); i++)
	{
		SetCheck(i, bCheck);
	}
}
void CAddTaskListCtrl::CheckAllSelectedItems(BOOL bCheck)
{
	int	iIndex;
	POSITION pos = GetFirstSelectedItemPosition();
	while (NULL != pos)
	{
		iIndex = GetNextSelectedItem(pos);
		SetCheck(iIndex, bCheck);
	}
}

// CAddTaskListCtrl 消息处理程序


void CAddTaskListCtrl::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码


	BOOL bNewCheck = (pNMLV->uNewState >> 12) - 1;
	BOOL bOldCheck = (pNMLV->uOldState >> 12) - 1;
	if (bNewCheck != bOldCheck)
	{
		SItemData	*pItemData = (SItemData*) GetItemData(pNMLV->iItem);
		if (NULL == pItemData)
			return;

		switch (pItemData->iType)
		{
		case IT_ED2K:
			{
				CAddTaskDoc::SItem	item;
				item.bCheck = bNewCheck;
				m_pDoc->SetItem(pItemData->fhk, item, CAddTaskDoc::IM_CHECK, GetSafeHwnd());
			}
			break;
		case IT_URL:
			{
				m_pDoc->CheckUrl(pItemData->strUrl, bNewCheck, GetSafeHwnd());
			}
			break;
		}

	}

	*pResult = 0;
}

LRESULT CAddTaskListCtrl::OnDocAdded(WPARAM /*wParam*/, LPARAM lParam)
{
	if (0 == lParam)
		return 0;

	CFileHashKey			*pKey = (CFileHashKey*) lParam;
	CAddTaskDoc::SItem		item;
	if (!m_pDoc->GetItem(*pKey, item))
		return 0;

	CString			strFileName;
	CString			strFileSize;
	CalStrByLinkText(item.strLinkText, strFileName, strFileSize);

	SItemData		*pItemData = new SItemData;
	pItemData->iType = IT_ED2K;
	pItemData->fhk = *pKey;
	m_itemDatas.AddTail(pItemData);

	int			iItemIndex;
	iItemIndex = InsertItem(GetInsertIndex(strFileName), _T(""));
	SetItemData(iItemIndex, (DWORD_PTR)pItemData);
	SetItemByDocItem(iItemIndex, item);

	return 0;
}
LRESULT CAddTaskListCtrl::OnDocModified(WPARAM wParam, LPARAM lParam)
{
	if (0 == wParam || 0 == lParam)
		return 0;


	DWORD					dwModifiedMask = wParam;
	CFileHashKey			*pKey = (CFileHashKey*) lParam;
	CAddTaskDoc::SItem		item;
	if (!m_pDoc->GetItem(*pKey, item))
		return 0;

	SetItemByDocItem(GetItemIndexByKey(*pKey), item, dwModifiedMask);
	return 0;
}

LRESULT CAddTaskListCtrl::OnDocRemoved(WPARAM /*wParam*/, LPARAM lParam)
{
	if (0 == lParam)
		return 0;

	CFileHashKey *pKey = (CFileHashKey*) lParam;
	int	iItemIndex = GetItemIndexByKey(*pKey);
	if (-1 == iItemIndex)
		return 0;

	DeleteItem(iItemIndex);
	return 0;
}

LRESULT CAddTaskListCtrl::OnDocUrlAdded(WPARAM /*wParam*/, LPARAM lParam)
{
	if (0 == lParam)
		return 0;

	CString			strUrl = (LPCTSTR) lParam;

	CString			strFileName;
	CalStrByUrl(strUrl, strFileName);

	SItemData		*pItemData = new SItemData;
	pItemData->iType = IT_URL;
	pItemData->strUrl = strUrl;
	m_itemDatas.AddTail(pItemData);

	// VC-SearchDream[2007-05-22]: String Conversion
	strFileName = OptUtf8ToStr(URLDecode(strFileName));
	strFileName.Trim();

	int			iItemIndex;
	iItemIndex = InsertItem(GetInsertIndex(strFileName), _T(""));
	SetItemData(iItemIndex, (DWORD_PTR)pItemData);
	SetItemText(iItemIndex, 0, strFileName);
	SetCheck(iItemIndex, TRUE);

	return 0;
}

LRESULT CAddTaskListCtrl::OnDocUrlModified(WPARAM wParam, LPARAM lParam)
{
	if (0 == lParam)
		return 0;

	BOOL	bCheck = (BOOL) wParam;
	CString	strUrl = (LPCTSTR) lParam;

	int	iItemIndex;
	iItemIndex = GetItemIndexByUrl(strUrl);
	if (-1 == iItemIndex)
		return 0;

	SetCheck(iItemIndex, bCheck);
	return 0;
}

LRESULT CAddTaskListCtrl::OnDocUrlRemoved(WPARAM /*wParam*/, LPARAM lParam)
{
	if (0 == lParam)
		return 0;

	CString	strUrl = (LPCTSTR) lParam;

	int	iItemIndex;
	iItemIndex = GetItemIndexByUrl(strUrl);
	if (-1 == iItemIndex)
		return 0;

	DeleteItem(iItemIndex);
	return 0;
}

void CAddTaskListCtrl::OnRButtonDown(UINT /*nFlags*/, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMenu	menu;
	menu.CreatePopupMenu();

	menu.AppendMenu(MF_STRING, MP_SELECT, GetResString(IDS_CHECK));
	menu.AppendMenu(MF_STRING, MP_SELECT_ALL, GetResString(IDS_CHECK_ALL));
	menu.AppendMenu(MF_STRING, MP_DESELECT, GetResString(IDS_UNCHECK));
	menu.AppendMenu(MF_STRING, MP_DESELECT_ALL, GetResString(IDS_UNCHECK_ALL));

	CPoint	pt = point;
	ClientToScreen(&pt);
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
	menu.DestroyMenu();

	//CListCtrl::OnRButtonDown(nFlags, point);
}

BOOL CAddTaskListCtrl::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	BOOL	bProcessed = TRUE;
	switch(wParam)
	{
	case MP_SELECT:	//Select
		CheckAllSelectedItems(TRUE);
		break;
	case MP_SELECT_ALL:	//Select all
		CheckAllItems(TRUE);
		break;
	case MP_DESELECT:	//Deselect
		CheckAllSelectedItems(FALSE);
		break;
	case MP_DESELECT_ALL:	//Deselect all
		CheckAllItems(FALSE);
		break;
	default:
		bProcessed = FALSE;
		break;
	}
	if (bProcessed)
		return bProcessed;

	return CListCtrl::OnCommand(wParam, lParam);
}
