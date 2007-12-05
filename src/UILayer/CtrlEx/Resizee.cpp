#include "StdAfx.h"
#include ".\resizee.h"

#include "Resizer.h"

namespace TabWnd{

CResizee::CResizee()
{
	m_pResizer = NULL;
	m_posInResizer = NULL;
	m_rect.SetRect(0, 0, 0, 0);
	m_uAttribute = 0;
	m_iDesireLength = 100;
	m_bDynDesireLength = FALSE;
}
CResizee::~CResizee(void)
{
	DetachFromResizer();
}

void CResizee::AttachToResizer(CResizer *pResizer)
{
	m_pResizer = pResizer;
	m_posInResizer = m_pResizer->AddResizee(this);
}
void CResizee::DetachFromResizer()
{
	if (NULL != m_pResizer)
		m_pResizer->RemoveResizee(this);
	m_posInResizer = NULL;
}

const CRect& CResizee::GetRect(void)
{
	return m_rect;
}
void CResizee::SetRect(const CRect &rect)
{
	m_rect = rect;
	OnSize();
}


}//namespace TabWnd{
