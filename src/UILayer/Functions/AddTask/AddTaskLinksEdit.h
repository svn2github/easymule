#pragma once

#include "AddTaskDoc.h"
// CAddTaskLinksEdit

class CAddTaskLinksEdit : public CEdit
{
	DECLARE_DYNAMIC(CAddTaskLinksEdit)

public:
	CAddTaskLinksEdit();
	virtual ~CAddTaskLinksEdit();

	void	SetDoc(CAddTaskDoc	*pDoc){m_pDoc = pDoc;}
	void	UpdateLinksByWindowText();
protected:
	void	ConvertStrToStrList(CList<CString> *pList, const CString &str);

	void	AddText(LPCTSTR lpszText);
	void	RemoveLine(LPCTSTR lpszText);

	void	SetText(const CFileHashKey &key, LPCTSTR lpszText);
	void	RemoveText(const CFileHashKey &key);

	CString	RemoveLine(const CString &str, int iStart, int iEnd);

	BOOL	FindLineByKey(const CFileHashKey &key, int &iStartPos, int &iEndPos);

	CAddTaskDoc	*m_pDoc;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChange();
	afx_msg LRESULT OnDocAdded(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDocModified(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDocUrlAdded(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDocUrlModified(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDocUrlRemoved(WPARAM wParam, LPARAM lParam);
};


