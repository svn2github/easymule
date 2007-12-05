#include "StdAfx.h"
#include ".\iefiltermgr.h"
#include "IEFilter.h"

CIEFilterMgr	g_ieFilterMgr;

CIEFilterMgr::CIEFilterMgr(void)
{
	m_pSession = NULL;
	m_pFactory = NULL;

	StartFilter();
}

CIEFilterMgr::~CIEFilterMgr(void)
{
	EndFilter();
}

void CIEFilterMgr::StartFilter()
{
	CLSID ciIEFilter = CIEFilter::factory.GetClassID();

	HRESULT		hr;
	if(NULL == m_pSession)
	{
		IClassFactory *pCF = &CIEFilter::factory.m_xClassFactory;
		hr = pCF->QueryInterface(IID_IClassFactory, (void **) &m_pFactory);
		
		if (SUCCEEDED(hr))
		{
			if (NULL != m_pFactory)
			{
				hr = CoInternetGetSession(0, &m_pSession, 0);
				if(SUCCEEDED(hr))
				{
					m_pSession->RegisterNameSpace(m_pFactory, ciIEFilter, L"http", 0, NULL, 0);
					m_pSession->RegisterNameSpace(m_pFactory, ciIEFilter, L"https", 0, NULL, 0);
				}
			}
		}
	}
}

void CIEFilterMgr::EndFilter()
{
	if(NULL != m_pSession)
	{
		m_pSession->UnregisterNameSpace(m_pFactory, L"http");
		m_pSession->UnregisterNameSpace(m_pFactory, L"https");
		m_pSession->Release();
		m_pSession = NULL;
	}
	if(NULL != m_pFactory)
	{
		m_pFactory->Release();
		m_pFactory = NULL;
	}
}
