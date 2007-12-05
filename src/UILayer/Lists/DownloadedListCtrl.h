#pragma once


// CDownloadedListCtrl
#include "resource.h"
#include "KnownFile.h"
#include "Localizee.h"
#include "MenuXP.h"

#include "FileMgr.h"
#include "AffirmDeleteDlg.h"

#include "MuleListCtrl.h"
struct ItemData;
class CDownloadedListCtrl : public CMuleListCtrl, public CLocalizee
{
	DECLARE_DYNAMIC(CDownloadedListCtrl)
	LOCALIZEE_WND_CANLOCALIZE()
public:
	CDownloadedListCtrl();
	virtual ~CDownloadedListCtrl();

protected:
	void OpenFile(const CKnownFile* file);
	void UpdateFile(const CKnownFile* file);
	int FindFile(const CKnownFile* pFile);
	int FindCompleteFile(const CFileTaskItem *pCompleteFile);

protected:
	CMenuXP* m_pMenuXP;
	enum EColId
	{
		CI_FILE_NAME,
		CI_FILE_SIZE,
		CI_FILE_PATH,
		CI_FILE_FINISHEDTIME,
		CI_FILE_COMMENT,

		CI_MAX
	};
	static int CALLBACK CmpProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	int		m_iCurSortCol;
	bool	m_bSortAscending;
	bool			sortstat[4];
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg LRESULT OnAddFile(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRemoveFile(WPARAM wParam, LPARAM lParam);

    afx_msg LRESULT OnCompletedAdd(WPARAM WParam,LPARAM lParam);
	afx_msg LRESULT OnCompletedDelete(WPARAM WParam,LPARAM lParam);

	afx_msg LRESULT OnUpdateFile(WPARAM WParam,LPARAM lParam);

	afx_msg void OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	static int CALLBACK SortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
public:
	afx_msg void OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
	void Localize(void);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
struct ItemData
{
public:
	 int type;
	 void *pItem;
};

