#pragma once

// CDlgAddTask 对话框
#include <set>
using namespace std;

#include "resource.h"
#include "afxcmn.h"
#include "ED2KLink.h"
#include "SearchFile.h"
#include "FileHashKey.h"
#include "AddTaskDoc.h"
#include "AddTaskListCtrl.h"
#include "AddTaskLinksEdit.h"
#include "ToolTipCtrlZ.h"

class CDlgAddTask : public CDialog
{
	DECLARE_DYNAMIC(CDlgAddTask)

public:
	CDlgAddTask(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAddTask();

// 对话框数据
	enum { IDD = IDD_ADDTASK };

	static void PopBlankTaskDlg(void);
	static void AddNewTask(LPCTSTR lpszLink, int cat = 0);
	static void AddNewTask(CED2KFileLink* pLink, int cat = 0);
	static void AddNewTask(CSearchFile* toadd, uint8 paused = 2, int cat = 0);
	static void AddNewTask(LPCTSTR lpszLink, uint8 paused = 2, int cat = 0);
	static void AddNewUrlTask(LPCTSTR lpszUrl);
	static void AddMultiLinks(LPCTSTR lpszLinks);
	static BOOL GetAddState(void); // Added by Soar Chin 09/06/2007

	void	Localize(void);

	void	AddTask(const CFileHashKey &key, const CAddTaskDoc::SItem &item);
	void	AddTask(LPCTSTR lpszUrl);
	void	AddLinks(LPCTSTR lpszLinks);
	BOOL	IsDlgPopedUp(void);
	void	PopupDlg(BOOL bBlank = FALSE);
protected:
	static CDlgAddTask	*ms_pInstance;
	

	CToolTipCtrlZ	m_ttc;
	
	CAddTaskDoc		m_doc;

	static ULONG	m_uAddState; // Added by Soar Chin 09/06/2007
	struct ItemData
	{
		ItemData();
		~ItemData();

		int				cat;
		CED2KFileLink	*pLink;
		CSearchFile		*pSearchFile;
		uint8			paused;
		CString			strLink;
	};

	static CDlgAddTask*	GetInstance();

	void	LoadHistoryLocs();
	void	SaveHistoryLocs();
	int		AddLocToCmb(LPCTSTR lpszLoc);

	BOOL	CheckLocation(const CString &strLocation);
	void	ApplyDocToTaskMgr(LPCTSTR lpszLocation);
	//void	ActualllyAddUrlDownload(const CString &strUrl, const CString & strLocation);
	
	void	UpdateFreeSpaceValue(void);

	struct SSpDownLink	//special download link (links in the states of downloading, downloaded, ...)
	{
		int			iLinkType;	// 0-ed2k, 1-url
		CString		strLink;
		int			iCat;		// Category
		int			iState;
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox			m_cmbLocation;
	CAddTaskListCtrl	m_lcContent;
	CAddTaskLinksEdit	m_editLinks;
	

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBnBrowse();
	afx_msg void OnNcDestroy();
protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	afx_msg void OnDestroy();
	afx_msg void OnCbnSelchangeComboLocation();
	BOOL m_bCheckSaveDirectly;
	afx_msg void OnCbnEditchangeComboLocation();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	void	DeletedDownloadedFile(void);
};
