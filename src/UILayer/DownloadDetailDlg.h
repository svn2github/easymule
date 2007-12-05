#pragma once


// CDownloadDetailDlg 对话框

class CDownloadDetailDlg : public CDialog
{
	DECLARE_DYNAMIC(CDownloadDetailDlg)

public:
	CDownloadDetailDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDownloadDetailDlg();

	void SetPartFileInfo(CKnownFile	*file);
// 对话框数据
	enum { IDD = IDD_DOWNLOAD_DETAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
