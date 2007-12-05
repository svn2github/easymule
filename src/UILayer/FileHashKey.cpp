#include "StdAfx.h"
#include ".\filehashkey.h"

CFileHashKey::CFileHashKey(void)
{
	memset(m_acFileHash, 0, HASH_SIZE);
}

CFileHashKey::~CFileHashKey(void)
{
}

CFileHashKey& CFileHashKey::operator=(const unsigned char acFileHash[HASH_SIZE])
{
	memcpy(m_acFileHash, acFileHash, HASH_SIZE);
	return *this;
}

bool CFileHashKey::operator<(const CFileHashKey& right) const
{
	if (this == &right)
		return false;

	if (memcmp(m_acFileHash, right.m_acFileHash, HASH_SIZE) < 0)
		return true;
	else
		return false;
}

BOOL CFileHashKey::operator==(const CFileHashKey& right) const
{
	return (0 == memcmp(m_acFileHash, right.m_acFileHash, HASH_SIZE));
}
