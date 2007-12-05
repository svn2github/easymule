#pragma once


// CProgressStatic

class CProgressStatic : public CStatic
{
	DECLARE_DYNAMIC(CProgressStatic)

public:
	CProgressStatic();
	virtual ~CProgressStatic();

	BOOL	CreatePS(CWnd *pParentWnd);
	void	UpdatePos();
	void	UpdateProgress(long lProgressPercent);
	void	UpdateProgress(long lProgress, long lProgressMax);
protected:
	long	m_lProgressPercent;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


