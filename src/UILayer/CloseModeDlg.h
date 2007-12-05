#pragma once


// CCloseModeDlg 对话框

class CCloseModeDlg : public CDialog
{
	DECLARE_DYNAMIC(CCloseModeDlg)

public:
	CCloseModeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCloseModeDlg();

// 对话框数据
	enum { IDD = IDD_CLOSE_MODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	int		m_iCloseMode;
	static bool m_bAskingClose;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
