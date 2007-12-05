#pragma once
#include "afxwin.h"
#include "afxcmn.h"

class CPPgGeneral : public CPropertyPage
{
	DECLARE_DYNAMIC(CPPgGeneral)

public:
	CPPgGeneral();
	virtual ~CPPgGeneral();

// Dialog Data
	enum { IDD = IDD_PPG_GENERAL };

	void Localize(void);

protected:
	UINT m_iFileBufferSize;
	CComboBox m_language;
	void LoadSettings(void);
	void UpdateEd2kLinkFixCtrl();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();
	virtual BOOL OnSetActive();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSettingsChange()					{ SetModified(); }
	afx_msg void OnBnClickedEd2kfix();
	afx_msg void OnBnClickedEditWebservices();
	afx_msg void OnLangChange();
	afx_msg void OnBnClickedCheck4Update();
	afx_msg void OnCbnCloseupLangs();
	afx_msg void OnHelp();
	afx_msg void OnWebBroswerChange();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
public:
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedDownloadbuf();
	afx_msg void OnDeltaposBufspin(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox m_CtrlCloseMode;
	afx_msg void OnCbnSelchangeCloseMode();
	afx_msg void OnBnClickedNickFrm();
	CComboBox m_DownloadBuffSizeCtrl;
public:
	afx_msg void OnCbnSelchangeComboBuf();
};
