#pragma once

namespace TabWnd{

class CResizer;
class CResizee
{
	friend class CResizer;
public:
	CResizee();
	~CResizee(void);

	enum {ATTR_FIXLEN = 0x1, ATTR_TAIL = 0x2, ATTR_FILL = 0x4};

	void			AttachToResizer(CResizer *pResizer);
	void			DetachFromResizer();
	const CRect&	GetRect(void);
	void			SetRect(const CRect &rect);

	UINT			GetAttribute(){return m_uAttribute;}
	void			SetAttribute(UINT uAttribute){m_uAttribute = uAttribute;}

	BOOL			IsFixedLength(){return m_uAttribute & ATTR_FIXLEN;}
	void			SetDesireLength(int iLength){m_iDesireLength = iLength;}
	virtual	int		GetDesireLength(void){return m_iDesireLength;}
	void			SetDynDesireLength(BOOL bDynDesireLength){m_bDynDesireLength = bDynDesireLength;}
	BOOL			IsDynDesireLength(){return m_bDynDesireLength;}


	BOOL			IsStickTail(){return m_uAttribute & ATTR_TAIL;}
	BOOL			IsFill(){return m_uAttribute & ATTR_FILL;}

protected:
	virtual	void OnSize() = 0;
private:
	//void	SetRect(const CRect &rect);
	
	CResizer	*m_pResizer;
	POSITION	m_posInResizer;
	CRect		m_rect;
	UINT		m_uAttribute;
	int			m_iDesireLength;
	BOOL		m_bDynDesireLength;
};

}//namespace TabWnd{
