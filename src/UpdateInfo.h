#pragma once

#define UPDATE_NODOWNLOAD  0
#define UPDATE_DOWNLOADING 1
#define UPDATE_DOWNLOADED  2



// CUpdateInfo ÃüÁîÄ¿±ê

class CUpdateInfo : public CObject
{
public:
	CUpdateInfo();
	virtual ~CUpdateInfo();

public:
	void SetUpdateHash(CString hash);
	CString GetUpdateHash(void);

	int	 GetUpdateState(const uchar* hash);

	BOOL UpdateInfoFileExists(void);

	BOOL isUpdateFile(CString hash);

	CString GetUpdateInfoFile(void);

	int Compare(CString hash);

	BOOL DeleteUpdate(CString hash);

protected:
	BOOL	OpenUpdateInfo(void);
	void	SaveUpdateInfo(void);

protected:
	CString	m_Hash;
public:
	virtual void Serialize(CArchive& ar);
};