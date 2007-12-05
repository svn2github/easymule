#pragma once

//#include "preferences.h"
//#include "afxwin.h"
// CPPgAntiLeech dialog

class CPPgAntiLeech : public CPropertyPage
{
	DECLARE_DYNAMIC(CPPgAntiLeech)

public:
	CPPgAntiLeech();
	virtual ~CPPgAntiLeech();

	// Dialog Data
	enum { IDD = IDD_PPG_ANTILEECH };

	virtual BOOL OnApply();
	virtual BOOL OnInitDialog();

	void Localize();


protected:
	void LoadSettings();

	// Generated message map functions
	afx_msg void OnSettingsChange() {SetModified();}
	DECLARE_MESSAGE_MAP()

	// for dialog data exchange and validation
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	
public:
	afx_msg void OnBnClickedAntiLeecher(); //Xman Anti-Leecher
	afx_msg void OnBnClickedDlpreload(); //Xman DLP
	afx_msg void OnBnClickedHplink(); //Xman Xtreme Links
	afx_msg void OnBnClickedForumlink(); //Xman Xtreme Links
	afx_msg void OnBnClickedVotelink(); //Xman Xtreme Links
};
