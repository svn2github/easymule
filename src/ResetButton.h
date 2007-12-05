#pragma once


// CResetButton

class CResetButton : public CStatic
{
	DECLARE_DYNAMIC(CResetButton)

public:
	CResetButton();
	virtual ~CResetButton();

public:
	void	SetImageList(CImageList* pImageList)	{ m_pImageList = pImageList; }
	void	ShowIcon(int nIconNumber);
protected:
	DECLARE_MESSAGE_MAP()

private:
	CImageList*	m_pImageList;
protected:
	int m_nCurrentIcon;
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};


