#include "StdAfx.h"
#include ".\metalinkexecutor.h"

#include "PartFile.h"
#include "MetaLinkParser.h"

CMetalinkExecutor::CMetalinkExecutor(void)
{
}

CMetalinkExecutor::~CMetalinkExecutor(void)
{
}

bool CMetalinkExecutor::execute( CPartFile * partFile )
{
	// 
	const CString & filepath = partFile->GetFilePath();
	
	if( -1 == filepath.Find( _T(".metalink") ) ) {
		// 不是 .metalink 文件，忽略
		return false;
	}

	CMetaLinkParser parser( filepath );
	if( parser.GetErrorCode() != METALINK_OK ) {
		// 不是合法的 meta link 文件
		return false;
	}

	bool ret = partFile->ChangedToMetalinkFile( &parser );

	// 
	if( !ret ) {
		return false;
	}

	// 进行处理

	return true;
}
