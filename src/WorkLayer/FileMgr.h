#pragma once

#include "SearchFile.h"
#include "KnownFile.h"

//注意：请不要改随意这些宏定义的值，因为他们已经被任务本地存储化了,修改值会引起加载错误! VC-Huby
#define FILESTATE_NOT_EXIST				0   //filemgr 中不存在该文件任务 
#define FILESTATE_DOWNLOAD				1
#define FILESTATE_COMPLETED				2	//下载完成	
#define FILESTATE_HASH					3
#define FILESTATE_SHARE					4	//已下载完成并共享  FILESTATE_SHARE_TASK_DOWNLOADED
#define FILESTATE_LOCAL_SHARE			5	//本地共享(本地文件通过分享页面上'Add Share files'分享出去的)
#define FILESTATE_DELETED				6   //删除(本地已删除对应的数据文件..)
#define FILESTATE_ZEROSIZE_DOWNLOADED	7	//(http/ftp)0字节大小已下载完成
#define FILESTATE_SHARE_TASK_DELED      8   //已下载的任务被删，但文件还在,仍需要保持分享状态.

class FILEKEY
{
public:
	FILEKEY()
	{
	}
	FILEKEY(const uchar * id)
	{
		memcpy(key, id, 16);
	}
	uchar key[16];
	bool operator<(const FILEKEY & id2) const
	{
		return memcmp(key, id2.key, 16) < 0;
	}
	bool operator==(const FILEKEY & id2) const
	{
		return memcmp(key, id2.key, 16) == 0;
	}
	bool operator>(const FILEKEY & id2) const
	{
		return memcmp(key, id2.key, 16) > 0;
	}
};


class CFileTaskItem
{
public:
	CFileTaskItem()
	{
		m_nFileState = FILESTATE_NOT_EXIST;
		m_FileSize = 0;
		m_metBakId = 0;
	}
	void Serialize(CArchive & ar, int iVerion)
	{		
		if(ar.IsLoading())
		{
			if (iVerion >=1)
			{
				int nStringNum = 0;
				ar>>m_nFileState;
				ar>>nStringNum;
				ASSERT(nStringNum>=1);
				ar>>m_strFilePath;
				ar>>m_strUrl;
				ar>>m_FileSize;
				ar>>m_FileName;
			}
			if (iVerion >= 2)
				ar>>m_strEd2kLink;
			if(iVerion >= 3)
				ar>>m_tFiletime;
			if(iVerion >= 4)
				ar>>m_metBakId;
		}
		else
		{
			if (iVerion >=1)
			{
				ar<<m_nFileState;
				ar<<int(1);
				ar<<m_strFilePath;
				ar<<m_strUrl;
				ar<<m_FileSize;
				ar<<m_FileName;
			}
			if (iVerion >= 2)
				ar<<m_strEd2kLink;
			if(iVerion >= 3)
				ar<<m_tFiletime;
			if(iVerion >= 4)
				ar<<m_metBakId;
		}
	}
	
	int m_nFileState;
	CString m_strFilePath; //完整的路径文件名 fullPathName
	CString m_strUrl;
	uint64 m_FileSize;
	CString m_FileName;
	CString m_strEd2kLink;
	CTime m_tFiletime;
	UINT32 m_metBakId;
};

class CFileMgr
{
public:
	CFileMgr(void);
	~CFileMgr(void);

	enum{CURRENT_FILELIST_VERSION = 4};

	void LoadFiles();
	void ReloadLocalSharedFiles();
	bool NewDownloadFile(CString strlink, CString strFilepath, int cat,bool bNewTask = true,CFileTaskItem* pFileTaskItem=NULL);
	void RemoveFileItem(CAbstractFile * pFile,bool bKeepShared=false);
	void RemoveEdLinkFileTask(CString strEd2kUrl);
	bool AddLocalSharedFile(const CString & strFile, bool bNeedHash, bool bLoadFromFileListDat=false,CFileTaskItem *pFileItem=NULL);
	//void NewDownloadBySearchFile(CSearchFile* toadd, uint8 paused, CString path, int cat);
	bool AddPartFile(CPartFile * partfile, int filestate);

	CString GetUrl(const uchar * hashkey);
	CFileTaskItem* GetFileTaskItem(const uchar * hashkey);
	CFileTaskItem* GetFileTaskItem(const CString& strUrl);
	int GetFileState(const uchar * hashkey);
	int GetFileState(const CString & strLink);
	//bool IsFileDownloading(CString strFilename);
	bool IsFileLocalShared(CKnownFile * file);

	void DownloadCompleted(CPartFile * partfile);
	//void DownloadCompleted(CString strUrl);
	void HashCompleted(CKnownFile * file);
	void HashFailed (CString strFile);
	
	CFileTaskItem* AddURLTask(CString strUrl,const CString strLocalDir);
	// VC-yunchenn.chen[2007-07-12]: -1不修改旧值，不存在返回false。 strLocalPath为空不修改旧值
	bool ModifyURLState(CString strUrl, CString strLocalPath, int nFileState=-1);
	void RemoveURLTask(CString strUrl);

	int	GetUrlTaskState(CString strUrl);
	BOOL AddDownLoadRequest(const CString & strUrl, const CString & strFilePath, CPartFile* &pPartFile,bool bNewTask);
	void UpdateFileItem(const CKnownFile *pFile);

	void OnPartfileUrlChanged( const CString & old_url , const CString & new_url , CPartFile * partfile );

protected:
	//  for loading
	bool AfterLoadFileItem(CFileTaskItem * pFileItem);
	bool LoadDownloadFile(CFileTaskItem * pNewItem);
	bool LoadSharedFile(CFileTaskItem * pNewItem);
	bool LoadDownloadUrlFile( CString& strUrl, CFileTaskItem * pFileTaskItem );

	CString GetDatabaseFile();
	CRBMap<FILEKEY,CFileTaskItem*> m_FileList;
	CMutex m_Mutex;
	CRBMap<CString, int> m_WaitforHashList;
	CRBMap<CString, CFileTaskItem*> m_UrlList;

	void SaveFileInfo();
};

extern CString UrlConvert( CString strUrl );

