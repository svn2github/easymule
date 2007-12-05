#pragma once

// CBufferMovieDlg 对话框

class CBufferMovieDlg : public CDialog
{
	DECLARE_DYNAMIC(CBufferMovieDlg)

public:
	CBufferMovieDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBufferMovieDlg();

	void Localize(void);

// 对话框数据
	enum { IDD = IDD_DIALOG_BUFFERINGMOVIE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

private:
	afx_msg LRESULT OnUpdateGUIFileProgress(WPARAM wParam, LPARAM lParam);

protected:
	CWnd* m_Parent;
};
