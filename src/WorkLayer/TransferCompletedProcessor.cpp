#include "stdafx.h"

#include "TransferCompletedProcessor.h"
#include "PartFile.h"

#include "MetalinkExecutor.h"

CTransferCompletedProcessor::CTransferCompletedProcessor()
{
	this->m_vecExcutors.push_back( new CMetalinkExecutor );
}

CTransferCompletedProcessor::~CTransferCompletedProcessor()
{
	for( size_t i = 0; i < this->m_vecExcutors.size(); i++ ) {
		delete this->m_vecExcutors[i];
	}

	this->m_vecExcutors.clear();
}

bool CTransferCompletedProcessor::OnFileTransferCompleted( CPartFile * partFile )
{
	ASSERT( partFile );

	for( size_t i = 0; i < this->m_vecExcutors.size(); i++ ) {
		if( this->m_vecExcutors[i]->execute( partFile ) ) {
			return true;
		}
	}

	return false;
}
