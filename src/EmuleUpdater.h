#pragma once
#include <afxinet.h>
#include "afxwin.h"
#include "afxcmn.h"

#define UPDATECHECK_BROWSER_STRING _T("Update search")
class CEmuleUpdater : public CWnd
{
		DECLARE_DYNAMIC(CEmuleUpdater);
public:
	CString				m_sFileToDownloadInto;
private:
protected:
	UINT		m_iMajor;		//主版本号
	UINT		m_iMinor;		//副版本号
	UINT		m_iUpdate;		//更新号
	UINT		m_iBuild;		//建立日期

	CString				m_sError;

	CString				m_sServer;
	CString				m_sObject;
	CString				m_sFilename;
	INTERNET_PORT		m_nPort;
	DWORD				m_dwServiceType;

	CString				m_sURLToDownload;

	CString m_sUserName;
	CString m_sPassword;

	CFile				m_FileToWrite;
	CWinThread*			m_pThread;

	HINTERNET	m_hInternetSession;
	HINTERNET	m_hHttpConnection;
	HINTERNET	m_hHttpFile;

	BOOL		m_bAbort;
	BOOL		m_bSafeToClose;

public:
	CEmuleUpdater(void);
	~CEmuleUpdater(void);

public:
	void	Check(const CString& strURL);
	void DestroyUpdater(void);

protected:
	afx_msg LRESULT OnThreadFinished(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
protected:

	void OnStatusCallBack(HINTERNET hInternet, DWORD dwInternetStatus,
						  LPVOID lpvStatusInformation, DWORD dwStatusInformationLength);
	void	GetCurrentVersion(void);

	void	UpdateAvailable(const CString& strURL);
	void	UpdateNotAvailable(void);
	void	UpdateNoCheck(void);

	static UINT _DownloadThread(LPVOID pParam);
	
	void DownloadThread(void);

	void DownloadSuceed(void);

	void HandleThreadErrorWithLastError(CString strIDError, DWORD dwLastError = 0);
	void HandleThreadError(CString strIDError);
	static void CALLBACK _OnStatusCallBack(HINTERNET hInternet, DWORD dwContext, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength);

	
public:
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	void CancelUpdater(void);
};
