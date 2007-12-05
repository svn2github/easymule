#include "StdAfx.h"
#include ".\utilmisc.h"


LPCTSTR GetAppDir()
{
	static TCHAR szAppDir[MAX_PATH] = {_T('\0')};
	if (_T('\0') != szAppDir[0])
		return szAppDir;

	GetModuleFileName(NULL, szAppDir, MAX_PATH * sizeof(TCHAR) );
	TCHAR *pc = _tcsrchr(szAppDir, _T('\\'));
	*pc = _T('\0');

	return szAppDir;
}
