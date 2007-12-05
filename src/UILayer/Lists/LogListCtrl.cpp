// LogListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "emule.h"
#include "LogListCtrl.h"
#include "emuleDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// CLogListCtrl

IMPLEMENT_DYNAMIC(CLogListCtrl, CListCtrl)
CLogListCtrl::CLogListCtrl()
{
}

CLogListCtrl::~CLogListCtrl()
{

}


BEGIN_MESSAGE_MAP(CLogListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomdraw)
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CLogListCtrl 消息处理程序
void CLogListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	if( !CGlobalVariable::IsRunning() )
	{
		return;
	}

	try
	{
		LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;

		switch(lplvcd->nmcd.dwDrawStage)
		{
		case CDDS_PREPAINT:
			*pResult = CDRF_NOTIFYITEMDRAW;
			break;
		case CDDS_ITEMPREPAINT:
			*pResult = CDRF_NOTIFYSUBITEMDRAW;
			break;
		case (CDDS_ITEMPREPAINT | CDDS_SUBITEM):
			{
				CTraceEvent* event = (CTraceEvent*)(CListCtrl::GetItemData(lplvcd->nmcd.dwItemSpec));

				ASSERT(event != NULL);
				ASSERT(lplvcd->iSubItem >= 0);

				if (!IsBadReadPtr(event, sizeof(CTraceEvent*)))
				{
					lplvcd->clrText = event->GetTextColor();
					lplvcd->clrTextBk = event->GetBkColor();
				}

				*pResult = CDRF_DODEFAULT;
			}
			break;

		default:
			break;
		}
	}
	catch (...)
	{
		*pResult = CDRF_DODEFAULT;
	}
}

void CLogListCtrl::OnDestroy()
{
	DeleteAllItems();
	CListCtrl::OnDestroy();
}

BOOL CLogListCtrl::DeleteItem(int nItem, BOOL bSelectNextItem)
{

	if (bSelectNextItem)
	{
		SetItemStates(nItem + 1, LLC_ITEM_SELECTED);
	}

	FreeItemMemory(nItem);

	return CListCtrl::DeleteItem(nItem);
}

int CLogListCtrl::DeleteAllItems(DWORD dwStates)
{
	int nItemCount = CListCtrl::GetItemCount();

	if (dwStates & LLC_ITEM_ALL)
	{
		SetRedraw(FALSE);
		for (int i = 0; i < nItemCount; i++)
		{
			FreeItemMemory(i);
		}

		CListCtrl::DeleteAllItems();
		SetRedraw(TRUE);
		return nItemCount;
	}

	int nDelCount = 0;
	SetRedraw(FALSE);

	for (int i = 0; i < nItemCount; i++)
	{
		if (ItemStatesTest(i, dwStates))
		{
			DeleteItem(i--);
			nItemCount--;
			nDelCount++;
		}
	}	

	CListCtrl::DeleteAllItems();
	SetRedraw(TRUE);

	return nDelCount;
}

BOOL CLogListCtrl::SetItemStates(int nItem, DWORD dwNewStates)
{
	if (dwNewStates == LLC_ITEM_NONE)
	{
		return FALSE;
	}

	BOOL bResult = TRUE;

	if (dwNewStates & LLC_ITEM_SELECTED)
	{
		bResult &= CListCtrl::SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
	}

	if (dwNewStates & LLC_ITEM_UNSELECTED)
	{
		bResult &= CListCtrl::SetItemState(nItem, 0, LVIS_SELECTED);
	}

	if (dwNewStates & LLC_ITEM_FOCUSED)
	{
		bResult &= CListCtrl::SetItemState(nItem, LVIS_FOCUSED, LVIS_FOCUSED);
	}

	if (dwNewStates & LLC_ITEM_UNFOCUSED)
	{
		bResult &= CListCtrl::SetItemState(nItem, 0, LVIS_FOCUSED);
	}

	return bResult;
}

BOOL CLogListCtrl::ItemStatesTest(int nItem, DWORD dwStates) const
{
	if (dwStates == LLC_ITEM_NONE)
		return FALSE;

	if (dwStates & LLC_ITEM_ALL)
		return TRUE;

	return (GetItemStates(nItem) & dwStates) == dwStates;
}

DWORD CLogListCtrl::GetItemStates(int nItem) const
{
	DWORD dwStates = 0;

	if (CListCtrl::GetItemState(nItem, LVIS_SELECTED))
	{
		dwStates |= LLC_ITEM_SELECTED;
	}
	else
	{
		dwStates |= LLC_ITEM_UNSELECTED;
	}

	if (CListCtrl::GetItemState(nItem, LVIS_FOCUSED))
	{
		dwStates |= LLC_ITEM_FOCUSED;
	}
	else
	{
		dwStates |= LLC_ITEM_UNFOCUSED;
	}

	return dwStates;
}

void CLogListCtrl::FreeItemMemory(int nItem)
{
	CTraceEvent* pEvent = (CTraceEvent*)(CListCtrl::GetItemData(nItem));

	if (pEvent != NULL)
	{
		pEvent = NULL;
	}
}

void CLogListCtrl::AddLog(CTraceEvent* event)
{
	if (!theApp.emuledlg->IsRunning())
	{
		return;
	}

	int iItemCount = GetItemCount();
	int iItem = InsertItem(LVIF_PARAM | LVIF_TEXT, iItemCount, event->GetTime(), 0, 0, 0, (LPARAM)event);
	SetItemText(iItem, 1, event->GetText());
	Update(iItem);
	EnsureVisible(iItem, FALSE);
	SortItems(SortProc, (LPARAM)this);
}

void CLogListCtrl::RemoveLog(CTraceEvent* event)
{
	if (!theApp.emuledlg->IsRunning())
	{
		return;
	}

	LVFINDINFO find;
	find.flags = LVFI_PARAM;
	find.lParam = (LPARAM)event;
	int result = FindItem(&find);

	if (result != -1) 
	{
		CListCtrl::DeleteItem(result);
	}
}

void CLogListCtrl::ShowSelectedPeerLogs(CUpDownClient* pClient)
{
	if (!pClient)
	{
		return;
	}

	DeleteAllItems();

	SetRedraw(FALSE);
	for(POSITION pos = pClient->GetEventList()->GetTailPosition(); pos != NULL;)
	{
		AddLog((pClient->GetEventList())->GetPrev(pos));
	}
	SetRedraw(TRUE);
}

void CLogListCtrl::ShowSelectedFileLogs(CPartFile* pPartFile)
{
	if (!pPartFile)
	{
		return;
	}

	DeleteAllItems();

	SetRedraw(FALSE);
	for(POSITION pos = pPartFile->GetEventList()->GetTailPosition(); pos != NULL;)
	{
		AddLog((pPartFile->GetEventList())->GetPrev(pos));
	}
	SetRedraw(TRUE);
}

int CLogListCtrl::SortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CLogListCtrl* pLogListCtrl = (CLogListCtrl*)lParamSort;
	CString StrItem1 = pLogListCtrl->GetItemText(lParam1, 0);
	CString StrItem2 = pLogListCtrl->GetItemText(lParam2, 0);
	return 0;
}

void CLogListCtrl::Init(void)
{
	SortItems(SortProc, (LPARAM)this);
}
