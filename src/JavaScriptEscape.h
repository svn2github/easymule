// JavaScriptEscape.h: interface for the JavaScriptEscape class.
// Added by thilon on 2006.08.28
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JAVASCRIPTESCAPE_H__93ED2E50_F5D4_4B07_8D06_9B16B4CAD05D__INCLUDED_)
#define AFX_JAVASCRIPTESCAPE_H__93ED2E50_F5D4_4B07_8D06_9B16B4CAD05D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class JavaScriptEscape  
{
public:
	JavaScriptEscape();
	virtual ~JavaScriptEscape();

	CString Escape(CString strValue);
	CString UnEscape(CString strValue);

protected:
	CString javaEscape(CString s);
};

#endif // !defined(AFX_JAVASCRIPTESCAPE_H__93ED2E50_F5D4_4B07_8D06_9B16B4CAD05D__INCLUDED_)
