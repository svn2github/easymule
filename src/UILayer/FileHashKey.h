#pragma once

class CFileHashKey
{
public:
	CFileHashKey(void);
	~CFileHashKey(void);

	CFileHashKey& operator=(const unsigned char acFileHash[16]);
	bool operator<(const CFileHashKey& right) const;
	BOOL operator==(const CFileHashKey& right) const;

protected:
	enum {HASH_SIZE = 16};
	unsigned char	m_acFileHash[HASH_SIZE];
};
