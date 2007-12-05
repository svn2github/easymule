#include "stdafx.h"
#include "SeeFileManager.h"
#include "PartFile.h"

CSeeFileManager::CSeeFileManager()
{

}

CSeeFileManager::~CSeeFileManager()
{

}

void CSeeFileManager::AddSeeFile(FILEKEY & key, CPartFile* pPartFile)
{
	m_SeeFiles.SetAt(key, pPartFile);
}

void CSeeFileManager::RemoveSeeFile(FILEKEY & key)
{
	CPartFile * pPartFile = NULL;

	if(m_SeeFiles.Lookup(key, pPartFile))
	{
		if (pPartFile)
		{
			pPartFile->SetSeeOnDownloading(NULL, false);
		}

		m_SeeFiles.RemoveKey(key);
	}
}

void CSeeFileManager::POPOneSeeFile()
{
	CPartFile * pPartFile = NULL;

	pPartFile = m_SeeFiles.GetAt(m_SeeFiles.GetHeadPosition())->m_value;

	if (pPartFile)
	{
		pPartFile->SetSeeOnDownloading(NULL, false);
	}

	m_SeeFiles.RemoveAt(m_SeeFiles.GetHeadPosition());
}

int CSeeFileManager::GetSeeFileCount()
{
	return m_SeeFiles.GetCount();
}