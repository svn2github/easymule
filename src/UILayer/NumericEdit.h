#pragma once


// NumericEdit

class CNumericEdit : public CEdit
{
	DECLARE_DYNAMIC(CNumericEdit)

public:
	CNumericEdit(int iMaxWholeDigits = 5, int iMaxDecimalPlaces = 0);
	virtual ~CNumericEdit();

protected:
	int m_iMaxWholeDigits;
	int m_iMaxDecimalPlaces;
	TCHAR m_cNegativeSign;
	TCHAR m_cDecimalPoint;
	CString m_strPrefix;

protected:
	DECLARE_MESSAGE_MAP()

public:
	void SetMaxWholeDigits(int nMaxWholeDigits);
	void SetMaxDecimalPlaces(int iMaxDecimalPlaces);

	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	CString GetText(void) const;
	bool IsNegativeAllowed() const;
protected:
	CString GetNumericText(const CString& strText, bool bUseMathSymbols = false) const;
};