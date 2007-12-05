#pragma once
#include "TraceEvent.h"

//Item 状态，选中，未选中等等
#define LLC_ITEM_NONE			0x0000 
#define	LLC_ITEM_ALL			0x0001 
#define	LLC_ITEM_SELECTED		0x0002 
#define	LLC_ITEM_UNSELECTED		0x0004
#define	LLC_ITEM_FOCUSED		0x0020	// Focused item
#define	LLC_ITEM_UNFOCUSED		0x0040	// Unfocused items
// CLogListCtrl

class CLogListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CLogListCtrl)

public:
	CLogListCtrl();
	virtual ~CLogListCtrl();

public:
	BOOL	DeleteItem(int nItem, BOOL bSelectNextItem = FALSE);
	int		DeleteAllItems(DWORD dwStates = LLC_ITEM_ALL);

	//Item状态
	BOOL	SetItemStates(int nItem, DWORD dwNewStates);
	BOOL	ItemStatesTest(int nItem, DWORD dwStates) const;
	DWORD	GetItemStates(int nItem) const;

	void	AddLog(CTraceEvent* event);
	void	RemoveLog(CTraceEvent* event);

	void	ShowSelectedPeerLogs(CUpDownClient* client);
	void	ShowSelectedFileLogs(CPartFile* pPartFile);

protected:
	void FreeItemMemory(int nItem);

protected:
	static int CALLBACK SortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
	void Init(void);
};