#pragma once


//#include "AsyncSocketEx.h"
// CUrlSrcFromSvrMgr 命令目标
class CPartFile;

class CUrlSrcFromSvrMgr : public CObject
	{
	friend class CUrlSrcGetFromSvrSocket;		//由于 CClientReqSocket 在连接断开后会 delete 自己。所以另建一个类CUrlSrcFromSvrSocket来处理连接。
public:
	CUrlSrcFromSvrMgr();
	virtual ~CUrlSrcFromSvrMgr();

	void	SetAssocPartFile(CPartFile *pAssocPartFile);
	bool	GetSrcFromServerAsyn(void);							//异步到服务器取url源。
	bool	SendReq_FileDownloaded(void);						//告诉服务器此文件下载完毕。

	bool	AddSrcToServer(LPCTSTR lpszUrl);								//把url源添加到服务器上保存起来。

protected:
	bool	IsExistInFetchedList(LPCTSTR lpszUrl);


protected:
	CPartFile	*m_pAssocPartFile;								//与之相关联的PartFile的指针。

	CStringList			m_strlstFetchedUrlSources;				//在服务器上的url源。（从服务器上取来url源后，存放在此。）
};


