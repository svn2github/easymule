#pragma once

#include "FileMgr.h"

class CPartFile;
class CSeeFileManager
{
public:

	CSeeFileManager();
	virtual ~CSeeFileManager();

	void AddSeeFile(FILEKEY & key, CPartFile* pPartFile);
	void RemoveSeeFile(FILEKEY & key);
	void POPOneSeeFile();
	int  GetSeeFileCount();

protected:

	CRBMap<FILEKEY, CPartFile*> m_SeeFiles; 
};