// UserComment.cpp : 实现文件
//

#include "stdafx.h"
#include "emule.h"
#include "UserComment.h"
#include ".\usercomment.h"


// CUserComment 对话框

IMPLEMENT_DYNAMIC(CUserComment, CDialog)
CUserComment::CUserComment(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CUserComment::IDD, pParent)
{
	m_pwebUserComment = 0;
}

CUserComment::~CUserComment()
{
}

void CUserComment::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUserComment, CResizableDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////
// Added by thilon on 2007.02.14, for Resize


// CUserComment 消息处理程序

BOOL CUserComment::OnInitDialog()
{
	CResizableDialog::OnInitDialog();

	if(! m_pwebUserComment)
	{
		//  don't delete it, auto-deleted
		m_pwebUserComment = new CHtmlCtrl;
		CRect rect;  
		GetClientRect(&rect);
		m_pwebUserComment->Create(NULL, NULL ,WS_CHILD | WS_VISIBLE &~WS_BORDER, rect,this, 34346,NULL);
		m_pwebUserComment->SetSilent(true);
	}

	AddAnchor(m_pwebUserComment->m_hWnd,TOP_LEFT, BOTTOM_RIGHT);
	return TRUE;
}

void CUserComment::Refresh(CKnownFile * file)
{
/*
	if(m_pwebUserComment)
	{
		CString strURL = _T("http://beta.verycd.com/files/");

		strURL += CreateED2kLink(file, false);
		strURL.Replace(_T("|"), _T("%7C"));
		m_pwebUserComment->Navigate2(strURL, 0, NULL);
	}*/
	
	if( !m_pwebUserComment || !file )
		return;
	CString strFileEd2k = CreateED2kLink(file, false);
	if( strFileEd2k.IsEmpty() )
	{
		m_pwebUserComment->Navigate2(_T("about:blank"), 0, NULL);
		return;
	}
	
	bool bFileisFinished = true;
	if( file->IsKindOf(RUNTIME_CLASS(CPartFile)) )
	{
		if( ((CPartFile*)file)->getPartfileStatus()!=PS_COMPLETE )
			bFileisFinished = false;
	}

	CString strCommentUrl = bFileisFinished ? thePrefs.m_strFinishedFileCommentUrl : thePrefs.m_strPartFileCommentUrl;
	strCommentUrl.Replace(_T("[ed2k]"),strFileEd2k);
	strCommentUrl.Replace(_T("|"), _T("%7C"));
	m_pwebUserComment->Navigate2(strCommentUrl, 0, NULL);
}

void CUserComment::OnSize(UINT nType, int cx, int cy)
{
	CResizableDialog::OnSize(nType, cx, cy);
	Invalidate(FALSE);
}
