#include "StdAfx.h"
#include ".\filemgr.h"
#include "Preferences.h"
#include "GlobalVariable.h"
#include "UIMessage.h"
#include "ED2KLink.h"
#include "resource.h"
#include <io.h>

#include "DNSManager.h"
#include "SourceURL.h"

#include "UserMsgs.h"
#include "WndMgr.h"
#include "CmdFuncs.h"
#include "StringConversion.h"

#include "DownloadedListCtrl.h"

#define FM_LOCK  CSingleLock lock(&m_Mutex, true)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFileMgr::CFileMgr(void)
{
}

CFileMgr::~CFileMgr(void)
{
	POSITION pos = m_FileList.GetHeadPosition();
	while(pos)
	{
		CFileTaskItem * item= m_FileList.GetNextValue(pos);
		delete item;
	}
	m_FileList.RemoveAll();

	pos=m_UrlList.GetHeadPosition();
	while(pos)
	{
		delete m_UrlList.GetNextValue(pos);
	}
	m_UrlList.RemoveAll();
}

CString CFileMgr::GetDatabaseFile()                   
{
	return thePrefs.GetMuleDirectory(EMULE_CONFIGDIR) + _T("FileList.dat");
}

bool CFileMgr::NewDownloadFile(CString strlink, CString strFilepath, int cat,bool bNewTask,CFileTaskItem* pFileTaskItem)  
{
	FM_LOCK;

	if(strFilepath.IsEmpty())  
	{
		strFilepath = thePrefs.GetTempDir();
	}
	bool bOk=false;            
	int curPos = 0;
	CString resToken = strlink.Tokenize(_T("\t\n\r"), curPos); 
	while (resToken != _T(""))
	{
		if (resToken.Right(1) != _T("/"))
			resToken += _T("/");                                     
		try
		{
			CED2KLink* pLink = CED2KLink::CreateLinkFromUrl(resToken.Trim());
			if (pLink)
			{
				if (pLink->GetKind() == CED2KLink::kFile)
				{
					if(strFilepath.GetAt(strFilepath.GetLength()-1)!='\\')
						strFilepath+='\\';

					CED2KFileLink * pFilelink = (CED2KFileLink*) pLink;
					pFilelink->m_strFilepath = strFilepath;
					CPartFile * partfile=CGlobalVariable::downloadqueue->AddFileLinkToDownload(pLink->GetFileLink(), cat,bNewTask);
					if(partfile)
					{
						bOk=true;						
						if( partfile->GetFileSize()>(uint64)0 )
							partfile->SetPartFileSizeStatus( FS_KNOWN );
                        if(bNewTask)
						{
						   CFileTaskItem * pItem=new CFileTaskItem;
						   pItem->m_nFileState = FILESTATE_DOWNLOAD; 
						   pItem->m_FileName = partfile->GetFileName();
						   pItem->m_strFilePath = strFilepath+partfile->GetFileName();
						   pItem->m_strEd2kLink = strlink;
						   pItem->m_metBakId = partfile->GetMetBakId(); 
						   m_FileList.SetAt(partfile->GetFileHash(), pItem);
						   partfile->m_pFileTaskItem = pItem;
						}
						else if( pFileTaskItem )
						{
							pFileTaskItem->m_metBakId = partfile->GetMetBakId();
						}
					}
				}
				else
				{
					delete pLink;
					throw CString(_T("bad link"));
				}
				delete pLink;
			}
		}
		catch(CString error)
		{
			TCHAR szBuffer[200];
			_sntprintf(szBuffer, ARRSIZE(szBuffer), GetResString(IDS_ERR_INVALIDLINK), error);
			LogError(LOG_STATUSBAR, GetResString(IDS_ERR_LINKERROR), szBuffer);
		}
		resToken = strlink.Tokenize(_T("\t\n\r"), curPos);
	}
   
	if( bOk && bNewTask ) 
		SaveFileInfo();
	return bOk;
}
bool CFileMgr::LoadSharedFile(CFileTaskItem * pNewItem)
{
	//原来的实现大部分代码和AddLocalSharedFile重复,所以重构
	return AddLocalSharedFile( pNewItem->m_strFilePath,true,true,pNewItem);
}

bool CFileMgr::LoadDownloadFile(CFileTaskItem * pNewItem)   
{
	CPartFile* toadd = new CPartFile();
	toadd->SetMetBakId(pNewItem->m_metBakId);
	if (toadd->LoadPartFile(pNewItem->m_strFilePath))
	{
		toadd->m_pFileTaskItem = pNewItem;		
		CGlobalVariable::downloadqueue->AddNewPartFile(toadd);			// to downloadqueue
		if (toadd->GetStatus(true) == PS_READY)
		{
			int n=pNewItem->m_strFilePath.ReverseFind('\\');
			CString strPath=pNewItem->m_strFilePath.Mid(0, n+1);
			if(! CGlobalVariable::sharedfiles->IsSharedPath(strPath))
				CGlobalVariable::sharedfiles->SafeAddKFile(toadd); // part files are always shared files
		}
		//  Comment UI
		SendMessage(CGlobalVariable::m_hListenWnd, WM_FILE_ADD_DOWNLOAD, 0, (LPARAM)toadd);
		//theApp.emuledlg->transferwnd->downloadlistctrl.AddFile(toadd);// show in downloadwindow
	}
	else
	{
		// VC-yunchenn.chen[2007-07-12]: 可能会是另一个emule打开，在这里判断是否这种情况
		CFileException fexpPart;
		CFile fileTest;
		if (!fileTest.Open(pNewItem->m_strFilePath + _T(".part"), CFile::modeReadWrite|CFile::shareDenyWrite|CFile::osSequentialScan, &fexpPart))
		{
			if(fexpPart.m_cause==CFileException::sharingViolation)
				return true;
			TRACE(_T("Failed to open met file\n"));
		}
		else fileTest.Close();

		if (toadd->LoadPartFile(pNewItem->m_strFilePath, true))
		{
			toadd->m_pFileTaskItem = pNewItem;			
			toadd->SavePartFile(); // resave backup

			m_FileList.SetAt(toadd->GetFileHash(), pNewItem);
			CGlobalVariable::downloadqueue->AddNewPartFile(toadd);
			if (toadd->GetStatus(true) == PS_READY)
				CGlobalVariable::sharedfiles->SafeAddKFile(toadd); // part files are always shared files

			//  Comment UI
			SendMessage(CGlobalVariable::m_hListenWnd, WM_FILE_ADD_DOWNLOAD, 0, (LPARAM)toadd);
			//theApp.emuledlg->transferwnd->downloadlistctrl.AddFile(toadd);// show in downloadwindow

			AddLogLine(false, GetResString(IDS_RECOVERED_PARTMET), toadd->GetFileName());
		}
		else
		{
			delete toadd;
			if(pNewItem->m_nFileState == FILESTATE_DOWNLOAD)
			{
				CString strPartFileName = pNewItem->m_strFilePath+_T(".part");
				_tremove(strPartFileName);
                if( 0!=pNewItem->m_metBakId )
				{
					CString strMetBAkFileName;
					strMetBAkFileName.Format(_T("%s%u.part.met.bak"),thePrefs.GetMuleDirectory(EMULE_METBAKDIR),pNewItem->m_metBakId);
					_tremove(strMetBAkFileName);
				}
				//加入到下载列表中
				CString strFilePath = pNewItem->m_strFilePath;
				int len = strFilePath.GetLength();
				int index = strFilePath.ReverseFind('\\');
				strFilePath.Delete(index+1,len-index);
				NewDownloadFile(pNewItem->m_strEd2kLink,strFilePath,0,false,pNewItem);
				return true;
			}
			return false;
		}
	}


	if( toadd->GetFileSize()>(uint64)0 && toadd->GetPartFileSizeStatus()!=FS_NOSIZE )
		toadd->SetPartFileSizeStatus( FS_KNOWN );

	return true;
}

bool CFileMgr::LoadDownloadUrlFile( CString& strUrl, CFileTaskItem * pFileTaskItem )  
{
	CPartFile* toadd = new CPartFile();
	toadd->SetMetBakId(pFileTaskItem->m_metBakId);
	if (toadd->LoadPartFile(pFileTaskItem->m_strFilePath))
	{
		toadd->m_pFileTaskItem = pFileTaskItem;		
		toadd->StopFile(false, false); // Pause the file
		CGlobalVariable::downloadqueue->AddNewPartFile(toadd);			// to downloadqueue		
		if (toadd->GetStatus(true) == PS_READY)
		{
			int n = pFileTaskItem->m_strFilePath.ReverseFind('\\');
			CString strPath = pFileTaskItem->m_strFilePath.Mid(0, n+1);
			if(! CGlobalVariable::sharedfiles->IsSharedPath(strPath))
			{
				CGlobalVariable::sharedfiles->SafeAddKFile(toadd); // part files are always shared files
			}
		}

		if( toadd->GetFileSize()>(uint64)0 && toadd->GetPartFileSizeStatus()!=FS_NOSIZE )
			toadd->SetPartFileSizeStatus( FS_KNOWN );
		
		toadd->SetPartFileURL(strUrl);
	
		//  Comment UI
		SendMessage(CGlobalVariable::m_hListenWnd, WM_FILE_ADD_DOWNLOAD, 0, (LPARAM)toadd);
	}
	else
	{
		CFileException fexpPart;
		CFile fileTest;
		if (!fileTest.Open(pFileTaskItem->m_strFilePath + _T(".part"), CFile::modeReadWrite|CFile::shareDenyWrite|CFile::osSequentialScan, &fexpPart))
		{
			if(fexpPart.m_cause==CFileException::sharingViolation)
				return true;
			TRACE(_T("Failed to open met file\n"));
		}
		else fileTest.Close();

		if (toadd->LoadPartFile(pFileTaskItem->m_strFilePath, true))
		{
			toadd->m_pFileTaskItem = pFileTaskItem;
			toadd->SavePartFile(); // resave backup
			m_UrlList.SetAt(strUrl, pFileTaskItem);

			toadd->StopFile(false, false); // Pause the file
			CGlobalVariable::downloadqueue->AddNewPartFile(toadd);
			
			if (toadd->GetStatus(true) == PS_READY)
			{
				CGlobalVariable::sharedfiles->SafeAddKFile(toadd); // part files are always shared files
			}
			else
			{
				if( CGlobalVariable::m_DNSManager )
				CGlobalVariable::m_DNSManager->AddUrlToDNS(strUrl, toadd);
			}

			//  Comment UI
			SendMessage(CGlobalVariable::m_hListenWnd, WM_FILE_ADD_DOWNLOAD, 0, (LPARAM)toadd);

			AddLogLine(false, GetResString(IDS_RECOVERED_PARTMET), toadd->GetFileName());
		}
		else 
		{
			delete toadd;	
			if(pFileTaskItem->m_nFileState == FILESTATE_DOWNLOAD)
			{ 
				CString strPartFileName = pFileTaskItem->m_strFilePath+_T(".part");
				_tremove(strPartFileName);
				if( 0!=pFileTaskItem->m_metBakId )
				{
					CString strMetBAkFileName;
					strMetBAkFileName.Format(_T("%s%u.part.met.bak"),thePrefs.GetMuleDirectory(EMULE_METBAKDIR),pFileTaskItem->m_metBakId);
					_tremove(strMetBAkFileName);
				}
				CString strPath;
				strPath = pFileTaskItem->m_strFilePath;
				int index = strPath.ReverseFind('\\');
				strPath.Delete(index,strPath.GetLength()-index);
				CmdFuncs::ActualllyAddUrlDownload(strUrl,strPath,false,pFileTaskItem);							  
				return true;
			}
		}
	}
	return true;
}

void CFileMgr::HashFailed (CString strFile)
{
#ifdef _DEBUG	
	if( CGlobalVariable::IsRunning() )
		ASSERT(false);
#endif

	FM_LOCK;

	strFile.MakeLower();
	if(m_WaitforHashList.RemoveKey(strFile))
		SaveFileInfo();
}

void CFileMgr::HashCompleted(CKnownFile* file)
{
	if( !file ) 
		return;

	FM_LOCK;

	CString strKey= file->GetFilePath();
	strKey.MakeLower();
	int nFileState = 0;

	bool bSave=false;

	if(m_WaitforHashList.Lookup(strKey, nFileState))
	{
		m_WaitforHashList.RemoveKey(strKey);

		CFileTaskItem * pItem =NULL;
		FILEKEY key=file->GetFileHash();

		if(m_FileList.Lookup(key, pItem))
		{
			pItem->m_nFileState = nFileState;
		}
		else if(nFileState)
		{
			pItem = new CFileTaskItem;
			pItem->m_nFileState = nFileState;
		    pItem->m_FileName = file->GetFileName();
			pItem->m_strFilePath = file->GetFilePath();// >GetFileName();
			m_FileList.SetAt(key, pItem);
		}

		bSave = true;
	}
	else
	{
		CFileTaskItem * pItem = NULL;
		if(m_FileList.Lookup(file->GetFileHash(), pItem) && pItem )
		{
			pItem->m_nFileState = FILESTATE_SHARE;
		}
	}
	if(bSave) SaveFileInfo();
}

void CFileMgr::RemoveEdLinkFileTask(CString strEd2kUrl)
{
	if(!m_FileList.IsEmpty())
	{  
		CFileTaskItem *pFileItem = NULL;
		FILEKEY key;
		POSITION pos = m_FileList.GetHeadPosition();
		while(pos!=NULL)
		{
			m_FileList.GetNextAssoc( pos,key, pFileItem);
			if(pFileItem->m_strEd2kLink == strEd2kUrl)
			{
				m_FileList.RemoveKey(key);
				delete pFileItem;
				break;
			}		 
		}
		SaveFileInfo();
	}
}

void CFileMgr::RemoveFileItem(CAbstractFile * pFile,bool bKeepShared)
{
	if(! pFile) return;

	FM_LOCK;

	CFileTaskItem * pItem =NULL;
	FILEKEY key=pFile->GetFileHash();
	if( m_FileList.Lookup(key, pItem) && pItem )
	{
		if(bKeepShared)
		{
			pItem->m_nFileState = FILESTATE_SHARE_TASK_DELED;
		}
		else
		{
			m_FileList.RemoveKey(key);
			delete pItem;
		}
		
		SaveFileInfo();
	}
}
void CFileMgr::UpdateFileItem(const CKnownFile *pFile)
{
   if(!pFile) return;
   FM_LOCK;
   CFileTaskItem *pItem = NULL;
   FILEKEY key = pFile->GetFileHash();
   m_FileList.Lookup(key,pItem);
   if( NULL==pItem )   
   {
	   m_UrlList.Lookup(pFile->GetPartFileURL(),pItem); 
   }
	
   if( pItem )
   {
	   pItem->m_FileName = pFile->GetFileName();
	   pItem->m_strFilePath = pFile->GetFilePath();
	   if(pItem->m_strFilePath.Right(5).CompareNoCase(_T(".part"))==0)
	   {
		   pItem->m_strFilePath.Delete(pItem->m_strFilePath.GetLength()-5, 5);
	   }
	   if( pFile->IsKindOf(RUNTIME_CLASS(CPartFile)) )
			pItem->m_metBakId = ((CPartFile*)pFile)->GetMetBakId();
	   SaveFileInfo();
   }     
}

void CFileMgr::SaveFileInfo()                             
{
	FM_LOCK;

	CFile file;
	if(file.Open(GetDatabaseFile()+_T(".tmp"), CFile::modeWrite|CFile::modeCreate))
	{
		try
		{
			CArchive ar(&file, CArchive::store);
			ar<<(uint32)CURRENT_FILELIST_VERSION;
			uint32 nTotalfile = m_FileList.GetCount();
			ar<<nTotalfile;

			POSITION pos = m_FileList.GetHeadPosition();
			for(; pos; )
			{
				FILEKEY fk=m_FileList.GetKeyAt(pos);
				CFileTaskItem * pNewItem = m_FileList.GetNextValue(pos);

				ar.Write(fk.key, 16);
				pNewItem->Serialize(ar, CURRENT_FILELIST_VERSION);
			}

			nTotalfile = m_WaitforHashList.GetCount();
			ar<<nTotalfile;
			pos=m_WaitforHashList.GetHeadPosition();
			for(; pos; )
			{
				ar<<m_WaitforHashList.GetKeyAt(pos);
				ar<<m_WaitforHashList.GetNextValue(pos);
			}

			nTotalfile = m_UrlList.GetCount();
			ar<<nTotalfile;
			pos=m_UrlList.GetHeadPosition();
			while(pos)
			{
				ar<<m_UrlList.GetKeyAt(pos);
				CFileTaskItem * pFileTaskItem = m_UrlList.GetNextValue(pos);
				pFileTaskItem->Serialize(ar,CURRENT_FILELIST_VERSION);
			}

			ar.Close();
			file.Close();
		}
		catch(...)
		{
			ASSERT(false);
			TRACE("Failed to save filelist\n");
			return;
		}

		CString strNewfile = GetDatabaseFile()+_T(".tmp");
		if(_taccess(strNewfile, 0)==0)
		{
			DeleteFile(GetDatabaseFile());
			CFile::Rename(strNewfile, GetDatabaseFile());
		}
	}
}

/*
void CFileMgr::NewDownloadBySearchFile(CSearchFile* toadd, uint8 paused, CString path, int cat)
{
	if (toadd->GetFileSize()== (uint64)0 || CGlobalVariable::downloadqueue->IsFileExisting(toadd->GetFileHash()))
		return;

	if (toadd->GetFileSize() > OLD_MAX_EMULE_FILE_SIZE && !thePrefs.CanFSHandleLargeFiles()){
		LogError(LOG_STATUSBAR, GetResString(IDS_ERR_FSCANTHANDLEFILE));
		return;
	}

	if(path.GetAt(path.GetLength()-1)!='\\')
		path+='\\';

	CPartFile* newfile = new CPartFile(toadd,path, cat);
	if (newfile->GetStatus() == PS_ERROR)
	{
		delete newfile;
		return;
	}
	else
	{
		FM_LOCK;

		CFileTaskItem * pNewItem=new CFileTaskItem;
		pNewItem->m_nFileState = FILESTATE_DOWNLOAD;
		pNewItem->m_strFilePath = path+newfile->GetFileName();
		m_FileList.SetAt(newfile->GetFileHash(), pNewItem);
	}	
	if (paused == 2)
		paused = (uint8)thePrefs.AddNewFilesPaused();
	CGlobalVariable::downloadqueue->AddDownload(newfile, (paused==1));

	// If the search result is from OP_GLOBSEARCHRES there may also be a source
	if (toadd->GetClientID() && toadd->GetClientPort()){
		CSafeMemFile sources(1+4+2);
		try{
			sources.WriteUInt8(1);
			sources.WriteUInt32(toadd->GetClientID());
			sources.WriteUInt16(toadd->GetClientPort());
			sources.SeekToBegin();
			newfile->AddSources(&sources, toadd->GetClientServerIP(), toadd->GetClientServerPort(), false);
		}
		catch(CFileException* error){
			ASSERT(0);
			error->Delete();
		}
	}

	// Add more sources which were found via global UDP search
	const CSimpleArray<CSearchFile::SClient>& aClients = toadd->GetClients();
	for (int i = 0; i < aClients.GetSize(); i++){
		CSafeMemFile sources(1+4+2);
		try{
			sources.WriteUInt8(1);
			sources.WriteUInt32(aClients[i].m_nIP);
			sources.WriteUInt16(aClients[i].m_nPort);
			sources.SeekToBegin();
			newfile->AddSources(&sources,aClients[i].m_nServerIP, aClients[i].m_nServerPort, false);
		}
		catch(CFileException* error){
			ASSERT(0);
			error->Delete();
			break;
		}
	}

	SaveFileInfo();
}*/


void CFileMgr::DownloadCompleted(CPartFile * partfile)
{
	if( partfile==NULL )
		return;

	FM_LOCK;

	CFileTaskItem * pFileTaskItem = NULL;
	CString strUrl = partfile->GetPartFileURL().Trim();
	if( strUrl!=_T("") )
	{		
		if( m_UrlList.Lookup( strUrl,pFileTaskItem ) )
		{
			ASSERT(pFileTaskItem);
			if( partfile->GetFileSize()==(uint64)0 )
			{
				pFileTaskItem->m_nFileState = FILESTATE_ZEROSIZE_DOWNLOADED;
			}
			else
			{
				pFileTaskItem->m_nFileState = FILESTATE_SHARE;
				m_UrlList.RemoveKey(strUrl);
				m_FileList.SetAt(partfile->GetFileHash(),pFileTaskItem); //moved from m_UrlList to m_FileList		
			}			
		}
		// else ASSERT(false);
	}
	
	if( !pFileTaskItem && !isnulmd4(partfile->GetFileHash()) ) // [11/26/2007 VC-Huby]: 有可能PartFileURL不为空,但已经拿到了FileHash的任务
	{
		FILEKEY key=partfile->GetFileHash();
		m_FileList.Lookup(key, pFileTaskItem); 	
		ASSERT(pFileTaskItem);
		if(NULL==pFileTaskItem)
		{
			pFileTaskItem = new CFileTaskItem;
			m_FileList.SetAt(key, pFileTaskItem);
		}
	}

	ASSERT(pFileTaskItem);

	//更新此已下载任务的其它信息..
	if(pFileTaskItem)
	{
		pFileTaskItem->m_strFilePath = partfile->GetFilePath();	
		pFileTaskItem->m_FileName = partfile->GetFileName();
		pFileTaskItem->m_strUrl = strUrl;
		pFileTaskItem->m_strEd2kLink = CreateED2kLink(partfile);
		pFileTaskItem->m_FileSize = partfile->GetFileSize();
		pFileTaskItem->m_tFiletime = CTime::GetCurrentTime();	
	}
	
	SaveFileInfo();
}

/*
bool CFileMgr::IsFileDownloading(CString strFilename)
{
	FM_LOCK;

	POSITION pos=m_FileList.GetHeadPosition();
	while(pos)
	{
		CFileTaskItem * pItem = m_FileList.GetNextValue(pos);
		if(pItem)
		{
			CString strPart=pItem->m_strFilePath+_T(".part");
			CString strPartMet=pItem->m_strFilePath+_T(".part.met");
			CString strPartMetBak=pItem->m_strFilePath+_T(".part.met.bak");
			if(strFilename.CompareNoCase(strPart)==0 ||
				strFilename.CompareNoCase(strPartMetBak)==0 ||
				strFilename.CompareNoCase(strPartMet)==0)
			return true;
		}
	}

	return false;
}*/

bool CFileMgr::AfterLoadFileItem(CFileTaskItem * pFileItem)
{
	if(pFileItem->m_nFileState == FILESTATE_DOWNLOAD)
		return LoadDownloadFile(pFileItem);
	else if(pFileItem->m_nFileState == FILESTATE_SHARE || pFileItem->m_nFileState==FILESTATE_LOCAL_SHARE
		|| pFileItem->m_nFileState==FILESTATE_HASH||pFileItem->m_nFileState == FILESTATE_COMPLETED
		|| pFileItem->m_nFileState == FILESTATE_DELETED || pFileItem->m_nFileState ==FILESTATE_SHARE_TASK_DELED)
		return LoadSharedFile(pFileItem);
	return false;
}

void CFileMgr::LoadFiles()
{
	CGlobalVariable::sharedfiles->Initialize();

	FM_LOCK;

	bool bSaveAfterLoad = false;
	CFile file;
	TRY
	{
		//int count =0;
		if(file.Open(GetDatabaseFile(), CFile::modeRead))
		{
			CArchive ar(&file, CArchive::load);
			int iFileListVer;
			int nTotalfile = 0;
			ar>>iFileListVer;

			if (iFileListVer < 3)
			{
				MessageBox(NULL,GetResString(IDS_FILE_LIST),GetResString(IDS_CAPTION),MB_OK|MB_ICONWARNING);
				m_FileList.RemoveAll();
				m_UrlList.RemoveAll();
				bSaveAfterLoad = true;
			}
			else
			{
				ar>>nTotalfile;
				TRACE("\n\n%s: begin load file (%d)\n\n", __FUNCTION__, nTotalfile);
				for(int i=0; i<nTotalfile; i++)
				{
					FILEKEY key;
					ar.Read(key.key, 16);

					CFileTaskItem * pNewItem = new CFileTaskItem;
					pNewItem->Serialize(ar, iFileListVer);

					TRACE("filestate on loading: %d\n", pNewItem->m_nFileState);

					if( pNewItem->m_FileName.IsEmpty() && pNewItem->m_strFilePath.IsEmpty() )
					{
						delete pNewItem; 
						pNewItem  = NULL;
						bSaveAfterLoad = true;	
						continue;
					}					
					int nOldState = pNewItem->m_nFileState;
					UINT32 nOldMetBakId = pNewItem->m_metBakId;

					if(!AfterLoadFileItem(pNewItem))
					{
						pNewItem->m_nFileState = FILESTATE_DELETED;
					}					
#ifdef _DEBUG
					if(pNewItem) TRACE(_T("%02d file loaded: %s\n"), i, pNewItem->m_strFilePath);
#endif
					if( nOldState==FILESTATE_LOCAL_SHARE && pNewItem->m_nFileState==FILESTATE_DELETED )
					{
						delete pNewItem; //对于本地添加的分享,如果已经被删除,filemgr不再管理,其它的仍然保存					
						pNewItem  = NULL;
					}
					else
					{
						m_FileList.SetAt(key, pNewItem);
					}

					if( pNewItem==NULL || pNewItem->m_nFileState!=nOldState
						|| nOldMetBakId != pNewItem->m_metBakId )
						bSaveAfterLoad = true;						
				}

				ar>>nTotalfile;
				for(int i=0; i<nTotalfile; i++)
				{
					CString strFile;
					int state;
					ar>>strFile;
					ar>>state;

					m_WaitforHashList.SetAt(strFile, state);
					CGlobalVariable::sharedfiles->HashNewFile(strFile);
				}

				ar>>nTotalfile;
				for(int i=0; i<nTotalfile; i++)
				{
					CFileTaskItem * pFileTaskItem = new CFileTaskItem;
					CString url;
					ar>>url;
					pFileTaskItem->Serialize(ar, iFileListVer);
					ASSERT( pFileTaskItem->m_nFileState==FILESTATE_DOWNLOAD || pFileTaskItem->m_nFileState==FILESTATE_ZEROSIZE_DOWNLOADED );
					if(pFileTaskItem && (pFileTaskItem->m_nFileState == FILESTATE_DOWNLOAD || pFileTaskItem->m_nFileState == FILESTATE_ZEROSIZE_DOWNLOADED))
						m_UrlList.SetAt(url, pFileTaskItem);
				}
			}
			ar.Close();
			file.Close();
		}	
	}
	CATCH_ALL(e)
	{
		file.Close();
		MessageBox(NULL,GetResString(IDS_DESTROYED_FILE),GetResString(IDS_CAPTION),MB_OK|MB_ICONWARNING);
		m_FileList.RemoveAll();
		m_UrlList.RemoveAll();
		bSaveAfterLoad = true;
		e->Delete();
	}
	END_CATCH_ALL

	//emule 的下载任务导入到easyMule
    if(!PathFileExists(GetDatabaseFile()))
	       CGlobalVariable::downloadqueue->LoadOldVersionPartFile();
	CGlobalVariable::downloadqueue->SortByPriority();
	CGlobalVariable::downloadqueue->CheckDiskspace();
	CGlobalVariable::downloadqueue->ExportPartMetFilesOverview();
	
	PostMessage(CGlobalVariable::m_hListenWnd, WM_SHAREDFILE_RELOADFILELIST, 0, 0);

	for (POSITION pos = m_UrlList.GetHeadPosition(); pos;)
	{
		CString strUrl = m_UrlList.GetKeyAt(pos);
		CFileTaskItem * pFileTaskItem = m_UrlList.GetNextValue(pos);
		if( pFileTaskItem->m_nFileState==FILESTATE_ZEROSIZE_DOWNLOADED)
		{
			::SendMessage(theWndMgr.GetWndHandle(CWndMgr::WI_DOWNLOADED_LISTCTRL), UM_CPL_LC_ADDFILE, 0, (LPARAM)pFileTaskItem);
		}
		else if(pFileTaskItem->m_nFileState != FILESTATE_SHARE)
		{
			UINT32 iOldMetBakId = pFileTaskItem->m_metBakId;
			if( !LoadDownloadUrlFile(strUrl, pFileTaskItem) )
			{
				pFileTaskItem->m_nFileState = FILESTATE_DELETED;
				bSaveAfterLoad = true;
			}
			else if( iOldMetBakId!=pFileTaskItem->m_metBakId )
			{
				bSaveAfterLoad = true;
			}
		}
	}

	if (CGlobalVariable::downloadqueue)
		CGlobalVariable::downloadqueue->AddPartFilesToShare(); // read partfiles to sharedfilelist

	if(bSaveAfterLoad)
	{
		SaveFileInfo();
	}
}

/**
*	<BR>功能说明：添加本地单个分享文件（包括已下载分享和本地添加的分享）
                  现在有两种情况调用此函数:
				  1 从分享页面UI上"add share files",选择单个或多个文件来分享（注意不是选择目录分享）
				    这种情况需要算hash后开始分享
				  2 从本地filelist.dat加载
*	@param strFile 完整的文件路径名
*	@param bNeedHash 如果没有已知hash信息，是否需要hash
*	@param bLoadFromFileListDat 是否是从本地filelist.dat中加载,而不是从其它接口新增..
*	@return true 添加成功
*/
bool CFileMgr::AddLocalSharedFile(const CString & strFile, bool bNeedHash, bool bLoadFromFileListDat, CFileTaskItem *pFileItem)
{
	if(!bLoadFromFileListDat)
	{
		POSITION pos=m_FileList.GetHeadPosition();
		while(pos)
		{
			CFileTaskItem * pItem=m_FileList.GetNextValue(pos);
			if(pItem && (pItem->m_nFileState==FILESTATE_LOCAL_SHARE || pItem->m_nFileState==FILESTATE_SHARE)
				&& pItem->m_strFilePath.CompareNoCase(strFile)==0)
			{
				//  exists the local shared file checked by filename
				return true;
			}
		}
	}
	
	CFile file;
	if( !file.Open(strFile, CFile::modeRead) )
	{
		AddDebugLogLine(false, _T("Failed to open file of \"%s\""), strFile);
		if( pFileItem && (pFileItem->m_nFileState==FILESTATE_SHARE || pFileItem->m_nFileState==FILESTATE_DELETED) )
			::SendMessage(theWndMgr.GetWndHandle(CWndMgr::WI_DOWNLOADED_LISTCTRL), UM_CPL_LC_ADDFILE, 0, (LPARAM)pFileItem); //属于已下载任务,但文件被删除的情况         
		return false;
	}

	CFileStatus fs;
	if( !file.GetStatus(fs) ) 
		return false;

	uint32 fdate = (UINT)fs.m_mtime.GetTime();
	if (fdate == 0)
		fdate = (UINT)-1;
	if (fdate == -1)
	{
		if (thePrefs.GetVerbose())
			AddDebugLogLine(false, _T("Failed to get file date of \"%s\""), strFile);
	}
	else
		AdjustNTFSDaylightFileTime(fdate, strFile);

	int nPathEnd=strFile.ReverseFind('\\');
	CString strDirectory = strFile.Mid(0, nPathEnd);
	CString strFileName = strFile.Mid(nPathEnd+1);
	CKnownFile* toadd = CGlobalVariable::knownfiles->FindKnownFile(strFileName, fdate, fs.m_size);
	if (toadd)
	{
		CCKey key(toadd->GetFileHash());
		CKnownFile* pFileInMap=CGlobalVariable::sharedfiles->GetKnownFile(key);
		if (!pFileInMap)
		{
			toadd->SetPath(strDirectory);
			toadd->SetFilePath(strFile);
			if(! CGlobalVariable::sharedfiles->IsSharedPath(strDirectory))
			{
				CGlobalVariable::sharedfiles->AddFile(toadd);
			}
		}
		else
		{
			TRACE(_T("%hs: File already in shared file list: %s \"%s\"\n"), __FUNCTION__, md4str(pFileInMap->GetFileHash()), pFileInMap->GetFilePath());		
		}

		if(!bLoadFromFileListDat) 
		{
			FILEKEY fk=toadd->GetFileHash();
			CFileTaskItem * pItem=NULL;
			if(! m_FileList.Lookup(fk, pItem) || pItem==NULL)
			{
				pItem=new CFileTaskItem;			
				m_FileList.SetAt(fk, pItem);
			}			
			pItem->m_strFilePath = strFile;
			pItem->m_nFileState = FILESTATE_LOCAL_SHARE;						
			SaveFileInfo();	
			UINotify(WM_SHAREDFILE_ADDFILE, 0, (LPARAM)toadd, toadd);
		}

		if( pFileItem && pFileItem->m_nFileState==FILESTATE_SHARE)
			SendMessage(CGlobalVariable::m_hListenWnd, WM_FILE_ADD_COMPLETED, 0, (LPARAM)toadd);
	}
	else
	{
		CString strKey=strFile;
		strKey.MakeLower();
		m_WaitforHashList.SetAt(strKey, FILESTATE_LOCAL_SHARE);

		if(bNeedHash) 
			CGlobalVariable::sharedfiles->HashNewFile(strFile);
	}
	
	return true;
}

bool CFileMgr::IsFileLocalShared(CKnownFile * file)
{
	return GetFileState(file->GetFileHash())==FILESTATE_LOCAL_SHARE;
}

void CFileMgr::ReloadLocalSharedFiles()
{
	POSITION pos = m_FileList.GetHeadPosition();
	while(pos)
	{
		CFileTaskItem * pItem = m_FileList.GetNextValue(pos);
		if(pItem->m_nFileState != FILESTATE_LOCAL_SHARE)
		{
			//  共享文件会丢失的bug
			if(pItem->m_nFileState != FILESTATE_SHARE)
				continue;
		}

		CString strFile = pItem->m_strFilePath;
		CFile file;
		if(! file.Open(strFile, CFile::modeRead))
		{
			AddDebugLogLine(false, _T("Failed to open file of \"%s\""), strFile);
			continue ;
		}

		CFileStatus fs;
		if(! file.GetStatus(fs)) continue;

		uint32 fdate = (UINT)fs.m_mtime.GetTime();
		if (fdate == 0)
			fdate = (UINT)-1;
		if (fdate == -1)
		{
			if (thePrefs.GetVerbose())
				AddDebugLogLine(false, _T("Failed to get file date of \"%s\""), strFile);
		}
		else
			AdjustNTFSDaylightFileTime(fdate, strFile);

		int nPathEnd=strFile.ReverseFind('\\');
		CString strDirectory = strFile.Mid(0, nPathEnd);
		CString strFileName = strFile.Mid(nPathEnd+1);
		CKnownFile* toadd = CGlobalVariable::knownfiles->FindKnownFile(strFileName, fdate, fs.m_size);
		if (toadd)
		{
			CCKey key(toadd->GetFileHash());
			CKnownFile* pFileInMap=CGlobalVariable::sharedfiles->GetKnownFile(key);
			if (!pFileInMap)
			{
				toadd->SetPath(strDirectory);
				toadd->SetFilePath(strFile);
				if(! CGlobalVariable::sharedfiles->IsSharedPath(strDirectory))
					CGlobalVariable::sharedfiles->AddFile(toadd);
			}
		}
		else
		{
			CString strKey=strFile;
			strKey.MakeLower();
			m_WaitforHashList.SetAt(strKey, FILESTATE_LOCAL_SHARE);

			CGlobalVariable::sharedfiles->HashNewFile(strFile);
		}
	}
}

bool CFileMgr::ModifyURLState(CString strUrl, CString strLocalPath, int nFileState)
{
	FM_LOCK;

	strUrl.MakeLower();
	CFileTaskItem * pFileTaskItem = NULL;
	if( m_UrlList.Lookup(strUrl, pFileTaskItem) && pFileTaskItem )
	{
		if( nFileState!=-1 ) 
			pFileTaskItem->m_nFileState = nFileState;
		if( !strLocalPath.IsEmpty() ) 
			pFileTaskItem->m_strFilePath = strLocalPath;
		SaveFileInfo();
		return true;
	}

	return false;
}

CFileTaskItem* CFileMgr::AddURLTask(CString strUrl,const CString strLocalDir)
{
	FM_LOCK;

	CFileTaskItem * pFileTaskItem = NULL;
	if( !m_UrlList.Lookup(strUrl.Trim(), pFileTaskItem) )
	{
		pFileTaskItem = new CFileTaskItem;		
		m_UrlList.SetAt(strUrl.Trim(), pFileTaskItem);
	}

	if(pFileTaskItem)
	{
		pFileTaskItem->m_nFileState = FILESTATE_DOWNLOAD;		
		pFileTaskItem->m_strFilePath = strLocalDir;
		SaveFileInfo();
	}	

	return pFileTaskItem;
}
void CFileMgr::RemoveURLTask(CString strUrl)
{
	FM_LOCK;

	CFileTaskItem * pFileTaskItem = NULL;
	if(m_UrlList.Lookup(strUrl, pFileTaskItem))
	{
		m_UrlList.RemoveKey(strUrl);	
	}

	SaveFileInfo();
}

int CFileMgr::GetFileState(const CString & strlink)
{
	int nRet = 0;
	int curPos = 0;
	CString resToken = strlink.Tokenize(_T("\t\n\r"), curPos);
	while (resToken != _T(""))
	{
		if (resToken.Right(1) != _T("/"))
			resToken += _T("/");
		try
		{
			CED2KLink* pLink = CED2KLink::CreateLinkFromUrl(resToken.Trim());
			if (pLink)
			{
				if (pLink->GetKind() == CED2KLink::kFile)
				{
					CED2KFileLink * pFilelink = (CED2KFileLink*) pLink;

					nRet = GetFileState(pFilelink->GetHashKey());
				}
				delete pLink;
			}
			if(nRet) break;
		}
		catch(CString error)
		{
			TCHAR szBuffer[200];
			_sntprintf(szBuffer, ARRSIZE(szBuffer), GetResString(IDS_ERR_INVALIDLINK), error);
			LogError(LOG_STATUSBAR, GetResString(IDS_ERR_LINKERROR), szBuffer);
		}
		resToken = strlink.Tokenize(_T("\t\n\r"), curPos);
	}

	return nRet;
}

/// 1 必须去掉extraInfo <referer=...> 后来判断处理
/// 2 strUrl 参数必须做统一编码后来判断是否重复
int CFileMgr::GetUrlTaskState(CString strUrl)
{
	FM_LOCK;

	strUrl.Trim();
	strUrl.MakeLower();	
		
	CString strUrlConverted;	///VC-Huby[2007-09-12]: process the the url with <referer=..>				
	if( strUrl.Find(_T('<'))>0 )
		strUrlConverted = UrlConvert( strUrl.Left(strUrl.Find(_T('<'))) ); //need cut the <referer=..> info first	
	else
		strUrlConverted = UrlConvert( strUrl );
	if(strUrlConverted.Find(_T('#'))>0 )
		strUrlConverted.Remove(_T('#'));

	CFileTaskItem * pFileTaskItem = NULL;
	CString sUrlInFileMgr;
	POSITION pos = m_UrlList.GetHeadPosition();
	while(pos)
	{
		sUrlInFileMgr = m_UrlList.GetKeyAt(pos);
  	   if( sUrlInFileMgr.Find(_T('<'))>0 )
			sUrlInFileMgr = sUrlInFileMgr.Left( sUrlInFileMgr.Find(_T('<')) ); //cut the extra info
		if(sUrlInFileMgr.Find(_T('#'))>0)
			sUrlInFileMgr.Remove(_T('#'));
		sUrlInFileMgr.MakeLower();
		sUrlInFileMgr = UrlConvert( sUrlInFileMgr );
		pFileTaskItem = m_UrlList.GetNextValue(pos);
		if( !sUrlInFileMgr.CompareNoCase(strUrlConverted) )
		{
			return pFileTaskItem->m_nFileState;
		}
	}

	pos = m_FileList.GetHeadPosition();
	while(pos)
	{
		pFileTaskItem = m_FileList.GetNextValue(pos);
		sUrlInFileMgr = pFileTaskItem->m_strUrl;
		if( sUrlInFileMgr.IsEmpty() )
			continue;
		if( sUrlInFileMgr.Find(_T('<'))>0 )
			sUrlInFileMgr = sUrlInFileMgr.Left( sUrlInFileMgr.Find(_T('<')) ); //cut the extra info
		if(sUrlInFileMgr.Find(_T('#'))>0 )
			sUrlInFileMgr.Remove(_T('#'));
		sUrlInFileMgr.MakeLower();
		sUrlInFileMgr = UrlConvert( sUrlInFileMgr );
		if( !sUrlInFileMgr.CompareNoCase(strUrlConverted) )
		{
			return pFileTaskItem->m_nFileState;
		}
	}

/* the origin code,don't process extrainfo of <referer=...>
	if(m_UrlList.Lookup(strUrl, pInfo))
	{
		return pInfo->m_nFileState;
	}
*/
	return FILESTATE_NOT_EXIST;
}

CString CFileMgr::GetUrl(const uchar * hashkey)
{
	FM_LOCK;
	CString strUrl;
	CFileTaskItem * pItem = NULL;
	if(m_FileList.Lookup(hashkey, pItem) && pItem)
	{
		strUrl = pItem->m_strUrl;
	}
	return strUrl;
}

int CFileMgr::GetFileState(const uchar * hashkey)
{
	FM_LOCK;
	CFileTaskItem * pItem = NULL;
	if(m_FileList.Lookup(hashkey, pItem) && pItem)
	{
		return pItem->m_nFileState;
	}
	return 0;
}

CFileTaskItem* CFileMgr::GetFileTaskItem(const uchar * hashkey)
{
	FM_LOCK;
	CFileTaskItem * pItem = NULL;
	if(m_FileList.Lookup(hashkey, pItem) && pItem)
	{
		return pItem;
	}
	return NULL;
}

CFileTaskItem* CFileMgr::GetFileTaskItem(const CString& strUrl)
{
	FM_LOCK;
	CFileTaskItem * pItem = NULL;
	if(m_UrlList.Lookup(strUrl, pItem) && pItem)
	{
		return pItem;
	}
	return NULL;
}


bool CFileMgr::AddPartFile(CPartFile * partfile, int filestate)
{
	if(partfile == NULL)
		return false;
	FM_LOCK;

	if(m_FileList.Lookup(partfile->GetFileHash()))
	{   
		return false;
	}

	CFileTaskItem * pItem=new CFileTaskItem;
	pItem->m_nFileState = filestate;
	pItem->m_FileName = partfile->GetFileName();
	pItem->m_strFilePath = partfile->GetFilePath();//+_T("\\") +partfile->GetFileName();
	if(pItem->m_strFilePath.GetLength()>5)
	{
		CString test=pItem->m_strFilePath.Right(5);
		if(test.CompareNoCase(_T(".part"))==0)
		{
			pItem->m_strFilePath.Delete(pItem->m_strFilePath.GetLength()-5, 5);
		}
	}

	if( !partfile->HasNullHash() )
		m_FileList.SetAt(partfile->GetFileHash(), pItem);
	else if( partfile->GetPartFileURL()!=_T("") )
		m_UrlList.SetAt( partfile->GetPartFileURL(),pItem);

	SaveFileInfo();
	return true;
}

/// strUrl have not cut <referer=...>
//  strFilePath is only path,no filename
BOOL CFileMgr::AddDownLoadRequest(const CString & strUrl, const CString & strFilePath, CPartFile* &pPartFile,bool bNewTask)
{
	CSourceURL SourceUrl;

	CString strUrlCutExtra;

	if( strUrl.Find(_T('<'))>0 )
	{
		strUrlCutExtra = strUrl.Left( strUrl.Find(_T('<')) );
	}
	else
	{
		strUrlCutExtra = strUrl;
	}

	if (strUrl.Left(7).CompareNoCase(_T("http://")) == 0)
	{
		SourceUrl.ParseHTTP(strUrlCutExtra);
	}
	else if (strUrl.Left(6).CompareNoCase(_T("ftp://")) == 0)
	{
		SourceUrl.ParseFTP(strUrlCutExtra);
	}

	int OffSet = 0;

	if (SourceUrl.m_sPath.Find('/') != -1)
	{
		OffSet = 1;
	}

	CString strName;
	if(SourceUrl.m_sPath.Find(_T('?'))>0)
	{
         strName = SourceUrl.m_sPath.Left(SourceUrl.m_sPath.Find(_T('?')));
		 strName = strName.Right(strName.GetLength() - 1 - strName.ReverseFind('/'));
	}
	if(SourceUrl.m_sPath.Find(_T('#'))>0)
	{   
		strName = SourceUrl.m_sPath.Right(
					SourceUrl.m_sPath.GetLength() - SourceUrl.m_sPath.ReverseFind('/') - OffSet);
        strName = strName.Left( strName.Find(_T('#')) );
	}
	if(strName.IsEmpty())
		strName = SourceUrl.m_sPath.Right(
		SourceUrl.m_sPath.GetLength() - SourceUrl.m_sPath.ReverseFind('/') - OffSet);
	strName = OptUtf8ToStr(URLDecode( strName ));

	pPartFile = new CPartFile(); //CPartFile * 

	// 处理文件名为空的情况
	if (strName == _T(""))
	{
		strName = _T("index.htm");
	}

	pPartFile->SetFileName(strName);
	pPartFile->SetPartFileSizeStatus(FS_UNKNOWN);
	pPartFile->SetFileSize((uint64)1u/*(uint64)PARTSIZE*/);	
	pPartFile->SetPath(strFilePath);//pPartFile->SetFilePath(strFilePath);
	pPartFile->SetPartFileURL(strUrl);
    
	CGlobalVariable::downloadqueue->AddDownload(pPartFile, thePrefs.AddNewFilesPaused(),bNewTask);

	CString strTmp=strFilePath;
	// VC-yunchenn.chen[2007-07-16]: fix a bug, path lost '\' between directory and file
	if(! strFilePath.IsEmpty() && strFilePath.GetAt(strFilePath.GetLength()-1)!='\\')
	{
		strTmp += _T("\\");
	}

	// VC-SearchDream[2007-07-12]: Add the URL to File Manager
	if(bNewTask)
	{
		CString strWholeFilePath = strTmp + pPartFile->GetFileName();
		CFileTaskItem* pFileTaskItem = AddURLTask(strUrl,strWholeFilePath);
		if(pPartFile) 
			pPartFile->m_pFileTaskItem = pFileTaskItem;
	}
	return TRUE;
}

void CFileMgr::OnPartfileUrlChanged( const CString & old_url , const CString & new_url , CPartFile * partfile )
{
	CFileTaskItem *pItem = NULL;
	m_UrlList.Lookup( old_url , pItem );
	if( pItem ) {
		m_UrlList.RemoveKey( old_url );
		m_UrlList.SetAt( new_url , pItem );

		pItem->m_strUrl = new_url;
//		pItem->m_strFilePath = _T("");
		pItem->m_FileName = _T("");
		pItem->m_FileSize = 0;
		
		this->SaveFileInfo();
	}
}
