#include <tchar.h>

#ifndef WORDFILTER_H
#define WORDFILTER_H

/////////////////////////////////////////////////////////////////////////////
//
// WordFilter.h : interface of the CWordFilter, used to filter limited words
//                Added by Soar Chin
//
/////////////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define FLITER_FILE _T("wordfilter.txt")

class CWordFilter
{
private:
	int		m_count;			// Count of limited words
	TCHAR **	m_filterwords;	// Limited words for filter
	int **	m_kmpvalue;			// KMP Next Value
	bool	m_filterall;
	void Free();
public:
	CWordFilter():
		m_count(0), m_filterwords(NULL), m_kmpvalue(NULL), m_filterall(false) {} // Init values
	~CWordFilter();
	void	Init();				// Init class and read from data file
	bool	VerifyString(const CString & sString);	// Verify if the string has limited words
};

extern CWordFilter WordFilter;

#endif // WORDFILTER_H
