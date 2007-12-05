
#pragma once


// CIEFilter ÃüÁîÄ¿±ê
class CIEFilter : public CCmdTarget
{
	DECLARE_DYNCREATE(CIEFilter)

public:
	CIEFilter();
	virtual ~CIEFilter();

protected:
	DECLARE_MESSAGE_MAP()

	DECLARE_OLECREATE(CIEFilter) 
	DECLARE_INTERFACE_MAP()

	BEGIN_INTERFACE_PART(InternetProtocolImpl, IInternetProtocol)
		INIT_INTERFACE_PART(CIEFilter, InternetProtocolImpl)

		STDMETHOD(Start)(LPCWSTR /*szUrl*/, IInternetProtocolSink * /*pIProtSink*/,
						IInternetBindInfo * /*pIBindInfo*/, DWORD /*grfSTI*/, DWORD /*dwReserved*/)
		{
			return INET_E_USE_DEFAULT_PROTOCOLHANDLER;
		}
		STDMETHOD(Continue)(PROTOCOLDATA * /*pStateInfo*/)
		{
			return S_OK;
		}
		STDMETHOD(Abort)(HRESULT /*hrReason*/,DWORD /*dwOptions*/)
		{
			return E_NOTIMPL;
		}
		STDMETHOD(Terminate)(DWORD /*dwOptions*/)
		{
			return  S_OK;
		}
		STDMETHOD(Suspend)()
		{
			return E_NOTIMPL;
		}
		STDMETHOD(Resume)()
		{
			return E_NOTIMPL;
		}
		STDMETHOD(Read)(void * /*pv*/,ULONG /*cb*/,ULONG * /*pcbRead*/)
		{
			return E_NOTIMPL;
		}
		STDMETHOD(Seek)(LARGE_INTEGER /*dlibMove*/, DWORD /*dwOrigin*/, ULARGE_INTEGER * /*plibNewPosition*/)
		{
			return E_NOTIMPL;
		}
		STDMETHOD(LockRequest)(DWORD /*dwOptions*/)
		{
			return  S_OK;
		}
		STDMETHOD(UnlockRequest)()
		{
			return  S_OK;
		}

	END_INTERFACE_PART(InternetProtocolImpl)

};


