// NumericEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "NumericEdit.h"


// NumericEdit

IMPLEMENT_DYNAMIC(CNumericEdit, CEdit)
CNumericEdit::CNumericEdit(int iMaxWholeDigits /*= 5*/, int iMaxDecimalPlaces /*= 0*/)
{
	m_iMaxWholeDigits = iMaxWholeDigits >= 0 ? iMaxWholeDigits : -iMaxWholeDigits;
	m_iMaxDecimalPlaces = iMaxDecimalPlaces;

	m_cDecimalPoint = _T('.');
	m_iMaxDecimalPlaces = 0;
}

CNumericEdit::~CNumericEdit()
{
}

 
BEGIN_MESSAGE_MAP(CNumericEdit, CEdit)
	ON_WM_CHAR()
END_MESSAGE_MAP()

void CNumericEdit::SetMaxWholeDigits(int iMaxWholeDigits)
{
	ASSERT(iMaxWholeDigits);

	//bool bAllowNegative = (iMaxWholeDigits >= 0);

	if (iMaxWholeDigits < 0)
	{
		iMaxWholeDigits = -iMaxWholeDigits;
	}

	if (m_iMaxWholeDigits == iMaxWholeDigits)
	{
		return;
	}

	m_iMaxWholeDigits = iMaxWholeDigits;
}

void CNumericEdit::SetMaxDecimalPlaces(int iMaxDecimalPlaces)
{
	ASSERT(iMaxDecimalPlaces >= 0);
	if (m_iMaxDecimalPlaces == iMaxDecimalPlaces)
	{
		return;
	}

	m_iMaxDecimalPlaces = iMaxDecimalPlaces;
}

void CNumericEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{	
	TCHAR tChar = static_cast<TCHAR>(nChar);

	int iStart, iEnd;
	GetSel(iStart, iEnd);

	CString strText = GetText();
	CString strNumericText = GetNumericText(strText);
	int iDecimalPos = strText.Find(m_cDecimalPoint);

	int iNumericDecimalPos = strNumericText.Find(m_cDecimalPoint);
	int iLen = strText.GetLength();
	int iNumericLen = strNumericText.GetLength();
	int iPrefixLen = m_strPrefix.GetLength();

	bool bNeedAdjustment = false;

	if (iStart < iPrefixLen && _istprint(tChar))
	{
		TCHAR cPrefix = m_strPrefix[iStart];

		if (cPrefix == tChar)
		{
			if (iLen > iStart)
			{
				iEnd = (iEnd == iLen ? iEnd : (iStart + 1));
				SetSel(iStart, iEnd);
				ReplaceSel(CString(tChar), TRUE);
			}
			else
			{
				CEdit::OnChar(nChar, nRepCnt, nFlags);
			}
		}
		else 
		{
			if ((_istdigit(tChar) || tChar == m_cNegativeSign || tChar == m_cDecimalPoint))
			{
				iEnd = (iEnd == iLen ? iEnd : (iPrefixLen));
				SetSel(iStart, iEnd);
				ReplaceSel(m_strPrefix.Mid(iStart), TRUE);

				CEdit::OnChar(nChar, nRepCnt, nFlags);
			}
		}
		
		return;
	}

	//检测是否负号
	if (tChar == m_cNegativeSign && IsNegativeAllowed())
	{
		if (iStart == iPrefixLen)
		{
			if (!strNumericText.IsEmpty() && strNumericText[0] == m_cNegativeSign)
			{
				iEnd = (iEnd == iLen ? iEnd : (iStart + 1));
				SetSel(iStart, iEnd);
				ReplaceSel(CString(m_cNegativeSign), TRUE);
				return;
			}
		}
		else
		{
			if (strNumericText[0] == m_cNegativeSign)
			{
				SetSel(iPrefixLen, iPrefixLen + 1);
				ReplaceSel(_T(""), TRUE);
				SetSel(iStart - 1, iEnd - 1);
			}
			else
			{
				SetSel(iPrefixLen, iPrefixLen);
				ReplaceSel(CString(m_cNegativeSign), TRUE);
				SetSel(iStart + 1, iEnd + 1);
			}

			return;
		}
	}
	else
	{
		if (tChar == m_cDecimalPoint && m_iMaxDecimalPlaces > 0)
		{
			if (iDecimalPos >= 0)
			{
				if (iDecimalPos >= iStart && iDecimalPos < iEnd)
				{
					bNeedAdjustment = true;
				}
				else
				{
					SetSel(iDecimalPos + 1, iDecimalPos + 1);
				}
				return;
			}
			else
				bNeedAdjustment = true;
		}
	}

	if (_istdigit(tChar))
	{
		if (iDecimalPos >= 0 && iDecimalPos < iStart)
		{
			if (strNumericText.Mid(iNumericDecimalPos + 1).GetLength() == m_iMaxDecimalPlaces)
			{
				if (iStart <= iDecimalPos + m_iMaxDecimalPlaces)
				{
					iEnd = (iEnd == iLen ? iEnd : (iStart + 1));
					SetSel(iStart, iEnd);
					ReplaceSel(CString(tChar), TRUE);
				}
				return;
			}
		}
		else
		{
			bool bIsNegative = (!strNumericText.IsEmpty() && strNumericText[0] == m_cNegativeSign);

			if (iStart == m_iMaxWholeDigits + bIsNegative + 0 + iPrefixLen)
			{
				if (/*m_uFlags & AddDecimalAfterMaxWholeDigits &&*/ m_iMaxDecimalPlaces > 0)
				{
					iEnd = (iEnd == iLen ? iEnd : (iStart + 2));
					SetSel(iStart, iEnd);
					ReplaceSel(CString(m_cDecimalPoint) + tChar, TRUE);
				}
				return;
			}

			if (strNumericText.Mid(0, iNumericDecimalPos >= 0 ? iNumericDecimalPos : iNumericLen).GetLength() == m_iMaxWholeDigits + bIsNegative)
			{
				/*if (strText[iStart] == m_cGroupSeparator)
				{
					nStart++;
				}*/

				iEnd = (iEnd == iLen ? iEnd : (iStart + 1));
				SetSel(iStart, iEnd);
				ReplaceSel(CString(tChar), TRUE);

				return;
			}

			bNeedAdjustment = true;
		}
	}

	else if (!_istprint(tChar))
	{
		bNeedAdjustment = true;
	}
	else
	{
		return;
	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

CString CNumericEdit::GetText(void) const
{
	CString strText;
	GetWindowText(strText);
	return strText;
}

CString CNumericEdit::GetNumericText(const CString& strText, bool bUseMathSymbols) const
{
	CString strNewText;
	bool bIsNegative = false;
	bool bHasDecimalPoint = false;

	for (int iPos = 0, nLen = strText.GetLength(); iPos < nLen; iPos++)
	{
		TCHAR c = strText[iPos];
		if (_istdigit(c))
		{
			strNewText += c;
		}
		else if (c == m_cNegativeSign)
		{
			bIsNegative = true;
		}
		else if (c == m_cDecimalPoint && !bHasDecimalPoint)
		{
			bHasDecimalPoint = true;
			strNewText += (bUseMathSymbols ? _T('.') : m_cDecimalPoint);
		}
	}

	if (bIsNegative)
	{
		;//strNewText.Insert(0, bUseMathSymbols ? '-' : m_cNegativeSign);
	}

	return strNewText;
}

bool CNumericEdit::IsNegativeAllowed() const
{
	return false;
}
