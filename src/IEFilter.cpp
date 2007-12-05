// IEFilter.cpp : 实现文件
//

#include "stdafx.h"
#include "IEFilter.h"


// CIEFilter

IMPLEMENT_DYNCREATE(CIEFilter, CCmdTarget)
CIEFilter::CIEFilter()
{
	EnableAggregation();
}

CIEFilter::~CIEFilter()
{
}


BEGIN_MESSAGE_MAP(CIEFilter, CCmdTarget)
END_MESSAGE_MAP()

// {E156A047-A940-43dd-A8D9-B020B0C6DF68}
IMPLEMENT_OLECREATE(CIEFilter, "IeFilter", 0xe156a047, 0xa940, 0x43dd, 0xa8, 0xd9, 0xb0, 0x20, 0xb0, 0xc6, 0xdf, 0x68);

BEGIN_INTERFACE_MAP(CIEFilter, CCmdTarget)
	INTERFACE_PART(CIEFilter, IID_IInternetProtocol, InternetProtocolImpl)
END_INTERFACE_MAP()


STDMETHODIMP_(ULONG) CIEFilter::XInternetProtocolImpl::AddRef( )
{
	METHOD_PROLOGUE_EX(CIEFilter, InternetProtocolImpl)
	return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CIEFilter::XInternetProtocolImpl::Release( )
{
	METHOD_PROLOGUE_EX(CIEFilter, InternetProtocolImpl)
	return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP CIEFilter::XInternetProtocolImpl::QueryInterface( REFIID iid, LPVOID FAR* ppvObj )
{
	METHOD_PROLOGUE_EX(CIEFilter, InternetProtocolImpl)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj) ;
}


// CIEFilter 消息处理程序
