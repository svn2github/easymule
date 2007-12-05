#pragma once

#define SAFE_DELETE(p)	if (NULL != p) {delete p; p = NULL;}
#define SAFE_ARRAY_DELETE(p)	if (NULL != p) {delete[] p; p = NULL;}
#define bool2BOOL(b) ((b) != false)
#define BOOL2bool(B) ((B) != FALSE)

#define BEGIN_NAMESPACE(ns) namespace ns {
#define END_NAMESPACE() }


template <class TYPE>
class CSimpleIterator
{
	CSimpleIterator();
	~CSimpleIterator();
public:
	virtual void SeekToBegin(void) = 0;
	virtual BOOL IsEnd(void) = 0;
	virtual TYPE GetAndForward(void) = 0;
};

LPCTSTR GetAppDir();
