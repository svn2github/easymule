#pragma once
#include "afxtempl.h"

// CSpeedMeter

class CSpeedMeter : public CWnd
{
	DECLARE_DYNAMIC(CSpeedMeter)

public:
	CSpeedMeter();
	virtual ~CSpeedMeter();

protected:
	COLORREF	m_crDownload;
	COLORREF	m_crUpload;

	CPen		m_DownloadPen;
	CPen		m_UploadPen;

	CBitmap*	m_pOldBMP;
	CBitmap		m_MemBMP;

	CList<UINT,UINT>	m_UpdataList;
	CList<UINT,UINT>	m_DownloadList;

protected:
	CDC		m_MemDC;

	bool	m_bInit;

	UINT	m_nMax;
	UINT	m_nMin;
	UINT	m_nStep;

protected:
	DECLARE_MESSAGE_MAP()
public:
	void AddValues(UINT uUpdatarate, UINT uDowndatarate, bool bRedraw = true);

	void SetRange(UINT nMin, UINT nMax, bool bDelete = false);
	void GetRange(UINT& nMax, UINT& nMin);

	void ResetGraph(bool bDelete = true);


protected:
	void AddValues2Graph(UINT uUpdatarate, UINT uDowndatarate);
public:
	afx_msg void OnPaint();
protected:
	void ReCreateGraph(CDC* pDC);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};