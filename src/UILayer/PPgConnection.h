#pragma once
#include "BetterSP2.h"   //Added by thilon 2006.08.07
#include "ToolTipCtrlZ.h"
#include "numericedit.h"
class CPPgConnection : public CPropertyPage
{
	DECLARE_DYNAMIC(CPPgConnection)

public:
	CPPgConnection();
	virtual ~CPPgConnection();

// Dialog Data
	enum { IDD = IDD_PPG_CONNECTION };

	void Localize(void);
	void LoadSettings(void);

protected:
	bool guardian;

	// VC-kernel[2007-03-02]:
	CSliderCtrl m_ctlMaxDown;
	CSliderCtrl m_ctlMaxUp;
	CComboBox m_ctlConnectionType;

	// VC-kernel[2007-02-27]:
	bool m_iUPnPNat;
	bool m_iUPnPTryRandom;

	int m_iMaxHalfOpen;
	int	m_isXP;					//Added by thilon on 2006.08.07
	CBetterSP2 m_betterSP2;		//Added by thilon on 2006.08.07
	int m_iTCPIPInit;			//Added by thilon on 2006.08.07

	CToolTipCtrlZ	m_ttc;

	void ShowLimitValues();
	void SetRateSliderTicks(CSliderCtrl& rRate);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSettingsChange()					{ SetModified(); }
	afx_msg void OnEnChangeUDPDisable();
	afx_msg void OnLimiterChange();
	afx_msg void OnBnClickedWizard();
	afx_msg void OnBnClickedNetworkKademlia();
	afx_msg void OnHelp();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnBnClickedOpenports();
	afx_msg void OnStartPortTest();
	afx_msg void OnEnChangeTCP();
	afx_msg void OnEnChangeUDP();
	afx_msg void OnEnChangePorts(uint8 istcpport);
public:
	afx_msg void OnBnClickedRandomPort();
	afx_msg void OnChangeSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeDownloadCap();
	afx_msg void OnEnChangeUploadCap();
	afx_msg void OnCbnSelchangeConnectiontype();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	CNumericEdit m_MaxHalfConEdit;
	CNumericEdit m_MaxConEdit;
	CNumericEdit m_MaxSourcePerFileEdit;
	CNumericEdit m_DownloadEdit;
	CNumericEdit m_UploadEdit;
	CNumericEdit m_PortEdit;
	CNumericEdit m_UDPPortEdit;
};
